/** Generic interface for protocol communication between an ITK filter
    and the VolView Plugin Interface */

#ifndef _vvITKFilterModule_h
#define _vvITKFilterModule_h

#include "vvITKFilterModuleBase.h"

#include <string.h>
#include <stdlib.h>

#include "itkImage.h"
#include "itkImportImageFilter.h"
#include "itkImageRegionConstIterator.h"


namespace VolView
{

namespace PlugIn
{

template <class TFilterType >
class FilterModule : public FilterModuleBase {

public:

   // Instantiate the image types
  typedef TFilterType                             FilterType;
  typedef typename FilterType::InputImageType     InputImageType;
  typedef typename FilterType::OutputImageType    OutputImageType;
  typedef typename InputImageType::PixelType      InputPixelType;
  typedef typename OutputImageType::PixelType     OutputPixelType;

  itkStaticConstMacro( Dimension, unsigned int, 
         itk::GetImageDimension< InputImageType >::ImageDimension );

  // Instantiate the ImportImageFilter
  // This filter is used for building an ITK image using 
  // the data passed in a buffer.
  typedef itk::ImportImageFilter< InputPixelType, 
                                  Dimension       > ImportFilterType;

  typedef typename ImportFilterType::SizeType      SizeType;
  typedef typename ImportFilterType::IndexType     IndexType;
  typedef typename ImportFilterType::RegionType    RegionType;



public:

  /**  Constructor */
  FilterModule() 
    {
    m_ImportFilter       = ImportFilterType::New();
    m_Filter             = FilterType::New();
    m_Filter->ReleaseDataFlagOn();
 
    m_Filter->SetInput( m_ImportFilter->GetOutput() );

    // Set the Observer for updating progress in the GUI
    m_Filter->AddObserver( itk::ProgressEvent(), this->GetCommandObserver() );
    m_Filter->AddObserver( itk::StartEvent(), this->GetCommandObserver() );
    m_Filter->AddObserver( itk::EndEvent(), this->GetCommandObserver() );
    m_LetITKAllocateOutputMemory = false;
    }



  /**  Destructor */
  virtual ~FilterModule() 
    {
    }


  /** Give access to the input image */
  const InputImageType * GetInput() 
    {  return m_ImportFilter->GetOutput(); }


  /** Return  a pointer to the internal filter */
  FilterType * GetFilter()
  {
    return m_Filter.GetPointer();
  }


  void SetLetITKAllocateOutputMemory(bool s)
  {
    m_LetITKAllocateOutputMemory = s;
  }

  /**  ProcessData performs the actual filtering on the data */
  virtual void 
  ProcessData( const vtkVVProcessDataStruct * pds )
  {

    this->InitializeProgressValue();
    this->SetCurrentFilterProgressWeight( 1.0 );

    const unsigned int numberOfComponents = this->GetPluginInfo()->InputVolumeNumberOfComponents;

    for(unsigned int component=0; component < numberOfComponents; component++ )
      {

      this->ImportPixelBuffer( component, pds );

      this->ExportPixelBuffer( component, pds );
      
      // Execute the filter
      try
        {
        m_Filter->Update();
        }
      catch( itk::ProcessAborted & )
        {
        return;
        }

      this->CopyOutputData( component, pds );

     }

  }



  /**  Copy the output data into the volview data structure */
  virtual void 
  CopyOutputData( unsigned int component, const vtkVVProcessDataStruct * pds )
  {

    // Copy the data (with casting) to the output buffer provided by the 
    // PlugIn API
    typename OutputImageType::ConstPointer outputImage =
      m_Filter->GetOutput();

    const unsigned int numberOfComponents = 
      this->GetPluginInfo()->OutputVolumeNumberOfComponents;
    
    if( !m_LetITKAllocateOutputMemory )
      {
      if (numberOfComponents == 1)
        {
        return;
        }
      }

   
    typedef itk::ImageRegionConstIterator< OutputImageType >  OutputIteratorType;

    OutputIteratorType ot( outputImage, outputImage->GetBufferedRegion() );
    
    OutputPixelType * outData = static_cast< OutputPixelType * >( pds->outData );

    outData += component;  // move to the start of the selected component;

    ot.GoToBegin(); 
    while( !ot.IsAtEnd() )
      {
      *outData = ot.Get();
      ++ot;
      outData += numberOfComponents;
      }

  } // end of CopyOutputData




  virtual void 
  ImportPixelBuffer( unsigned int component, const vtkVVProcessDataStruct * pds )
  {

    SizeType   size;
    IndexType  start;

    double     origin[3];
    double     spacing[3];

    size[0]     =  this->GetPluginInfo()->InputVolumeDimensions[0];
    size[1]     =  this->GetPluginInfo()->InputVolumeDimensions[1];
    size[2]     =  pds->NumberOfSlicesToProcess;

    for(unsigned int i=0; i<3; i++)
      {
      origin[i]   =  this->GetPluginInfo()->InputVolumeOrigin[i];
      spacing[i]  =  this->GetPluginInfo()->InputVolumeSpacing[i];
      start[i]    =  0;
      }

    RegionType region;

    region.SetIndex( start );
    region.SetSize(  size  );
   
    m_ImportFilter->SetSpacing( spacing );
    m_ImportFilter->SetOrigin(  origin  );
    m_ImportFilter->SetRegion(  region  );

    const unsigned int totalNumberOfPixels = region.GetNumberOfPixels();



    const unsigned int numberOfComponents = this->GetPluginInfo()->InputVolumeNumberOfComponents;

    const unsigned int numberOfPixelsPerSlice = size[0] * size[1];

    vtkVVPluginInfo * info = this->GetPluginInfo();
    if( !pds->inData )
      {
      info->SetProperty( info, VVP_ERROR, "The pointer to input data is NULL." ); 
      return;
      }


    if( numberOfComponents == 1 )
      {
      const bool         importFilterWillDeleteTheInputBuffer = false;

      InputPixelType *   dataBlockStart = 
                            static_cast< InputPixelType * >( pds->inData )  
                          + numberOfPixelsPerSlice * pds->StartSlice;

      m_ImportFilter->SetImportPointer( dataBlockStart, 
                                        totalNumberOfPixels,
                                        importFilterWillDeleteTheInputBuffer );
      }
    else 
      {
      const bool         importFilterWillDeleteTheInputBuffer = true;
      
      InputPixelType *   extractedComponent = new InputPixelType[ totalNumberOfPixels ];

      InputPixelType *   dataBlockStart = 
                            static_cast< InputPixelType * >( pds->inData )  
                          + numberOfPixelsPerSlice * pds->StartSlice
                          + component;

      InputPixelType *   inputData = dataBlockStart;

      for(unsigned int i=0; i<totalNumberOfPixels; i++, inputData += numberOfComponents )
        {
        extractedComponent[i] =  *inputData;
        }

      m_ImportFilter->SetImportPointer( extractedComponent, 
                                        totalNumberOfPixels,
                                        importFilterWillDeleteTheInputBuffer );
      }

  } // end of ImportPixelBuffer

  virtual void 
  ExportPixelBuffer( unsigned int component, 
                     const vtkVVProcessDataStruct * pds )
  {
    
    SizeType   size;
    IndexType  start;
    
    size[0]     =  this->GetPluginInfo()->OutputVolumeDimensions[0];
    size[1]     =  this->GetPluginInfo()->OutputVolumeDimensions[1];
    size[2]     =  pds->NumberOfSlicesToProcess;

    for(unsigned int i=0; i<3; i++)
      {
      start[i]    =  0;
      }

    RegionType region;

    region.SetIndex( start );
    region.SetSize(  size  );
   
    const unsigned int totalNumberOfPixels = region.GetNumberOfPixels();



    const unsigned int numberOfComponents = this->GetPluginInfo()->InputVolumeNumberOfComponents;

    const unsigned int numberOfPixelsPerSlice = size[0] * size[1];

    vtkVVPluginInfo * info = this->GetPluginInfo();
    if( !pds->outData )
      {
      info->SetProperty( info, VVP_ERROR, "The pointer to output data is NULL." ); 
      return;
      }

    if( ! m_LetITKAllocateOutputMemory )
      {
      if( numberOfComponents == 1 )
        {
        m_Filter->GetOutput()->SetRegions(region);
        m_Filter->GetOutput()->GetPixelContainer()->SetImportPointer( 
          static_cast< OutputPixelType * >( pds->outData ), 
          totalNumberOfPixels, false);
        m_Filter->GetOutput()->Allocate( );
        }
      }
    // otherwise let ITK allocate the memory
  
  } // end of ExportPixelBuffer

private:
    typename ImportFilterType::Pointer    m_ImportFilter;
    typename FilterType::Pointer          m_Filter;
    bool                                  m_LetITKAllocateOutputMemory;
};


} // end namespace PlugIn

} // end namespace VolView

#endif
