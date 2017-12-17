/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkFeatureAggregatorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkFeatureAggregator.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkImageMaskSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkConnectedThresholdSegmentationModule.h"

namespace itk 
{
template< unsigned int NDimension>
class FeatureAggregatorSurrogate : public FeatureAggregator< NDimension >
{
public:
  /** Standard class typedefs. */
  typedef FeatureAggregatorSurrogate        Self;
  typedef FeatureAggregator<NDimension>     Superclass;
  typedef SmartPointer<Self>                Pointer;
  typedef SmartPointer<const Self>          ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FeatureAggregatorSurrogate, FeatureAggregator);

  /** Dimension of the space */
  itkStaticConstMacro(Dimension, unsigned int, NDimension);


protected:
  FeatureAggregatorSurrogate() {};
  ~FeatureAggregatorSurrogate() ITK_OVERRIDE {};
  void PrintSelf(std::ostream& os, Indent indent) const ITK_OVERRIDE 
    {
    this->Superclass::PrintSelf( os, indent );
    }

private:
  FeatureAggregatorSurrogate(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typedef typename Superclass::FeatureGeneratorType             FeatureGeneratorType;
  typedef typename FeatureGeneratorType::Pointer                FeatureGeneratorPointer;
  typedef std::vector< FeatureGeneratorPointer >                FeatureGeneratorArrayType;
  typedef typename FeatureGeneratorArrayType::iterator          FeatureGeneratorIterator;
  typedef typename FeatureGeneratorArrayType::const_iterator    FeatureGeneratorConstIterator;

  void ConsolidateFeatures() ITK_OVERRIDE
    {
    typedef float                                                   FeaturePixelType;
    typedef Image< FeaturePixelType, NDimension >                   FeatureImageType;
    typedef ImageSpatialObject< NDimension, FeaturePixelType >      FeatureSpatialObjectType;

    const FeatureSpatialObjectType * firstFeatureObject =
      dynamic_cast< const FeatureSpatialObjectType * >( this->GetInputFeature(0) );

    const FeatureImageType * firstFeatureImage = firstFeatureObject->GetImage();

    typename FeatureImageType::Pointer consolidatedFeatureImage = FeatureImageType::New();

    consolidatedFeatureImage->CopyInformation( firstFeatureImage );
    consolidatedFeatureImage->SetRegions( firstFeatureImage->GetBufferedRegion() );
    consolidatedFeatureImage->Allocate();
    consolidatedFeatureImage->FillBuffer( NumericTraits< FeaturePixelType >::max() );

    const unsigned int numberOfFeatures = this->GetNumberOfInputFeatures();

    for( unsigned int i = 0; i < numberOfFeatures; i++ )
      {
      const FeatureSpatialObjectType * featureObject =
        dynamic_cast< const FeatureSpatialObjectType * >( this->GetInputFeature(i) );

      const FeatureImageType * featureImage = featureObject->GetImage();

      typedef ImageRegionIterator< FeatureImageType >          FeatureIterator;
      typedef ImageRegionConstIterator< FeatureImageType >     FeatureConstIterator;

      FeatureIterator       dstitr( consolidatedFeatureImage, consolidatedFeatureImage->GetBufferedRegion() );
      FeatureConstIterator  srcitr( featureImage, featureImage->GetBufferedRegion() );

      dstitr.GoToBegin();
      srcitr.GoToBegin();
     
      while( !srcitr.IsAtEnd() )
        {
        if( dstitr.Get() > srcitr.Get() )
          {
          dstitr.Set( srcitr.Get() );
          }
        ++srcitr;
        ++dstitr;
        }
      }

    FeatureSpatialObjectType * outputObject =
      dynamic_cast< FeatureSpatialObjectType * >(this->ProcessObject::GetOutput(0));

    outputObject->SetImage( consolidatedFeatureImage );
    }

};

} // end of itk namespace


int itkFeatureAggregatorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " landmarksFile inputImage outputImage ";
    std::cerr << " [lowerThreshold upperThreshold] " << std::endl;
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;
  typedef signed short   InputPixelType;

  typedef itk::Image< InputPixelType, Dimension > InputImageType;

  typedef itk::ImageFileReader< InputImageType > InputImageReaderType;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[2] );

  try 
    {
    inputImageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  typedef itk::FeatureAggregatorSurrogate< Dimension >   AggregatorType;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();
  
  typedef itk::SatoVesselnessSigmoidFeatureGenerator< Dimension > VesselnessGeneratorType;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  typedef itk::LungWallFeatureGenerator< Dimension > LungWallGeneratorType;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  typedef itk::SigmoidFeatureGenerator< Dimension >   SigmoidFeatureGeneratorType;
  SigmoidFeatureGeneratorType::Pointer  sigmoidGenerator = SigmoidFeatureGeneratorType::New();
 
  featureAggregator->AddFeatureGenerator( lungWallGenerator );
  featureAggregator->AddFeatureGenerator( vesselnessGenerator );
  featureAggregator->AddFeatureGenerator( sigmoidGenerator );


  typedef AggregatorType::SpatialObjectType    SpatialObjectType;

  typedef itk::ImageSpatialObject< Dimension, InputPixelType  > InputImageSpatialObjectType;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  lungWallGenerator->SetInput( inputObject );
  vesselnessGenerator->SetInput( inputObject );
  sigmoidGenerator->SetInput( inputObject );

  lungWallGenerator->SetLungThreshold( -400 );

  vesselnessGenerator->SetSigma( 1.0 );
  vesselnessGenerator->SetAlpha1( 0.5 );
  vesselnessGenerator->SetAlpha2( 2.0 );
 
  sigmoidGenerator->SetAlpha(  1.0  );
  sigmoidGenerator->SetBeta( -200.0 );
 
  typedef itk::ConnectedThresholdSegmentationModule< Dimension >   SegmentationModuleType;
  
  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();


  double lowerThreshold = 0.5;
  double upperThreshold = 1.0;

  if( argc > 4 )
    {
    lowerThreshold = atof( argv[4] );
    }

  if( argc > 5 )
    {
    upperThreshold = atof( argv[5] );
    }

  segmentationModule->SetLowerThreshold( lowerThreshold );
  segmentationModule->SetUpperThreshold( upperThreshold );

  featureAggregator->Update();

  
  typedef SegmentationModuleType::SpatialObjectType           SpatialObjectType;
  typedef SegmentationModuleType::OutputSpatialObjectType     OutputSpatialObjectType;
  typedef SegmentationModuleType::OutputImageType             OutputImageType;

  SpatialObjectType::ConstPointer segmentation = featureAggregator->GetFeature();

  OutputSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputSpatialObjectType * >( segmentation.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  typedef itk::ImageFileWriter< OutputImageType >      OutputWriterType;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();


  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  featureAggregator->Print( std::cout );

  return EXIT_SUCCESS;
}
