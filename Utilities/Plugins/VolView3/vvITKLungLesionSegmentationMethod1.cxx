#include "vvITKFilterModuleDoubleOutput.h"
#include "itkImageSpatialObject.h"
#include "itkLandmarkSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkLesionSegmentationMethod.h"
#include <itksys/SystemTools.hxx>
#include <fstream>
#include <sstream>


// =======================================================================
// The main class definition
// =======================================================================
template <class PixelType> class LungLesionSegmentationMethod1Runner
{
public:
  // define our typedefs
  typedef LungLesionSegmentationMethod1Runner< PixelType > Self;
  typedef itk::Image< PixelType, 3 >              InputImageType;
  typedef itk::Image< unsigned char, 3 >          SegmentedOutputImageType;
  typedef itk::ImportImageFilter< PixelType, 3>   ImportFilterType;
  typedef itk::LesionSegmentationMethod< 3 >      LesionSegmentationMethodType;
  typedef itk::SatoVesselnessSigmoidFeatureGenerator< 3 > 
                                                  VesselnessGeneratorType;
  typedef itk::LungWallFeatureGenerator< 3 >      LungWallGeneratorType;
  typedef itk::SigmoidFeatureGenerator< 3 >       SigmoidFeatureGeneratorType;
  typedef itk::MinimumFeatureAggregator< 3 >      FeatureAggregatorType;
  typedef itk::FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule< 3 > SegmentationModuleType;
  typedef itk::RegionOfInterestImageFilter< InputImageType, InputImageType > CropFilterType; 
  typedef SegmentationModuleType::SpatialObjectType       SpatialObjectType;
  typedef SegmentationModuleType::OutputSpatialObjectType OutputSpatialObjectType;
  typedef SegmentationModuleType::OutputImageType             OutputImageType;
  typedef itk::ImageSpatialObject< 3, PixelType > InputImageSpatialObjectType;

  typedef typename InputImageType::RegionType     RegionType;
  typedef typename RegionType::IndexType          IndexType;
  typedef typename RegionType::SizeType           SizeType;
  typedef itk::MemberCommand< Self >  CommandType;


   // Description:
  // The funciton to call for progress of the optimizer
  void ProgressUpdate( itk::Object * caller, const itk::EventObject & event );

  // Description:
  // The constructor
  LungLesionSegmentationMethod1Runner();

  // Description:
  // The destructor
  virtual ~LungLesionSegmentationMethod1Runner();

  // Description:
  // Imports the two input images from Volview into ITK
  virtual void ImportPixelBuffer( vtkVVPluginInfo *info,
                                  const vtkVVProcessDataStruct * pds );

  // Description:
  // Copies the resulting data into the output image
  virtual void CopyOutputData( vtkVVPluginInfo *info,
                               const vtkVVProcessDataStruct * pds );

  // Description:
  // Sets up the pipeline and invokes the registration process
  int Execute( vtkVVPluginInfo *info, vtkVVProcessDataStruct *pds );

private:
  typename CommandType::Pointer         m_CommandObserver;
  unsigned int                          m_GACIterations;
  double                                m_GACRMSError;
  double                                m_GACCurvatureScaling;
  double                                m_GACPropagationScaling;
  double                                m_GACAdvectionScaling;
  double                                m_FastMarchingStoppingTime;
  double                                m_FastMarchingDistanceFromSeeds;
  typename LesionSegmentationMethodType::Pointer m_LesionSegmentationMethod;
  typename LungWallGeneratorType::Pointer        m_LungWallFeatureGenerator;
  typename VesselnessGeneratorType::Pointer      m_VesselnessFeatureGenerator;
  typename SigmoidFeatureGeneratorType::Pointer  m_SigmoidFeatureGenerator;
  typename FeatureAggregatorType::Pointer        m_FeatureAggregator;
  typename SegmentationModuleType::Pointer       m_SegmentationModule;
  typename CropFilterType::Pointer               m_CropFilter;
  typename InputImageSpatialObjectType::Pointer  m_InputSpatialObject;
  typename ImportFilterType::Pointer             m_ImportFilter;
  vtkVVPluginInfo                              * m_Info;
};



// =======================================================================
// progress Callback
template <class PixelType>
void LungLesionSegmentationMethod1Runner<PixelType>::
ProgressUpdate( itk::Object * caller, const itk::EventObject & event )
{
  if( typeid( itk::ProgressEvent ) == typeid( event ) )
    {  
    if (dynamic_cast< ImportFilterType * >(caller))
      {
      m_Info->UpdateProgress( m_Info, 
          0.05 * m_ImportFilter->GetProgress(), 
          "Importing input data.." );
      }

    if (dynamic_cast< CropFilterType * >(caller))
      {
      m_Info->UpdateProgress( m_Info, 
          0.05 + 0.05 * m_CropFilter->GetProgress(), 
          "Cropping data.." );
      }

    if (dynamic_cast< LesionSegmentationMethodType * >(caller))
      {
      m_Info->UpdateProgress( m_Info, 
          0.1 + 0.9 * m_LesionSegmentationMethod->GetProgress(), 
          "Segmenting.." );
      }
    }
}

// =======================================================================
// Constructor
template <class PixelType>
LungLesionSegmentationMethod1Runner<PixelType>::LungLesionSegmentationMethod1Runner()
{
  m_GACRMSError = 0.0002; 
  m_LesionSegmentationMethod = LesionSegmentationMethodType::New();
  m_LungWallFeatureGenerator = LungWallGeneratorType::New();
  m_VesselnessFeatureGenerator = VesselnessGeneratorType::New();
  m_SigmoidFeatureGenerator = SigmoidFeatureGeneratorType::New();
  m_FeatureAggregator = FeatureAggregatorType::New();
  m_SegmentationModule = SegmentationModuleType::New();
  m_ImportFilter  = ImportFilterType::New();
  m_CropFilter = CropFilterType::New();
  m_InputSpatialObject = InputImageSpatialObjectType::New();

  // Report progress.
  m_CommandObserver    = CommandType::New();
  m_CommandObserver ->SetCallbackFunction(
    this, &LungLesionSegmentationMethod1Runner::ProgressUpdate );
  m_LesionSegmentationMethod->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_ImportFilter->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_CropFilter->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );

  // Connect pipeline
  m_LungWallFeatureGenerator->SetInput( m_InputSpatialObject );
  m_SigmoidFeatureGenerator->SetInput( m_InputSpatialObject );
  m_VesselnessFeatureGenerator->SetInput( m_InputSpatialObject );
  m_FeatureAggregator->AddFeatureGenerator( m_LungWallFeatureGenerator );
  m_FeatureAggregator->AddFeatureGenerator( m_VesselnessFeatureGenerator );
  m_FeatureAggregator->AddFeatureGenerator( m_SigmoidFeatureGenerator );
  m_LesionSegmentationMethod->AddFeatureGenerator( m_FeatureAggregator );
  m_LesionSegmentationMethod->SetSegmentationModule( m_SegmentationModule );
  m_CropFilter->SetInput( m_ImportFilter->GetOutput() );

  // Populate some parameters
  m_LungWallFeatureGenerator->SetLungThreshold( -400 );
  m_VesselnessFeatureGenerator->SetSigma( 1.0 );
  m_VesselnessFeatureGenerator->SetAlpha1( 0.5 );
  m_VesselnessFeatureGenerator->SetAlpha2( 2.0 );
  m_SigmoidFeatureGenerator->SetAlpha( 1.0 );
  m_SigmoidFeatureGenerator->SetBeta( -200.0 );
  m_SegmentationModule->SetMaximumRMSError( m_GACRMSError );
}

// =======================================================================
// Constructor
template <class PixelType>
LungLesionSegmentationMethod1Runner<PixelType>::~LungLesionSegmentationMethod1Runner()
{
}

// =======================================================================
// Import data
template <class PixelType>
void LungLesionSegmentationMethod1Runner<PixelType>::
ImportPixelBuffer( vtkVVPluginInfo *info, const vtkVVProcessDataStruct *pds )
{
  SizeType   size;
  IndexType  start;

  double     origin[3];
  double     spacing[3];

  size[0]     =  info->InputVolumeDimensions[0];
  size[1]     =  info->InputVolumeDimensions[1];
  size[2]     =  info->InputVolumeDimensions[2];

  for(unsigned int i=0; i<3; i++)
    {
    origin[i]   =  info->InputVolumeOrigin[i];
    spacing[i]  =  info->InputVolumeSpacing[i];
    start[i]    =  0;
    }

  RegionType region;

  region.SetIndex( start );
  region.SetSize(  size  );

  m_ImportFilter->SetSpacing( spacing );
  m_ImportFilter->SetOrigin(  origin  );
  m_ImportFilter->SetRegion(  region  );

  unsigned int totalNumberOfPixels = region.GetNumberOfPixels();

  PixelType *dataBlockStart = static_cast< PixelType * >( pds->inData );

  m_ImportFilter->SetImportPointer( dataBlockStart,
                                    totalNumberOfPixels, false);
}


// =======================================================================
//  Copy the output data into the volview data structure
template <class PixelType>
void LungLesionSegmentationMethod1Runner<PixelType>::
CopyOutputData( vtkVVPluginInfo *info, const vtkVVProcessDataStruct * pds )
{
  // get some useful info
  unsigned int numberOfComponents = info->OutputVolumeNumberOfComponents;
  typedef itk::ImageRegionConstIterator< InputImageType > OutputIteratorType;
  PixelType * outData = static_cast< PixelType * >( pds->outData );

  // do we append or replace
  if (atoi(info->GetGUIProperty(info, 0, VVP_GUI_VALUE)))
    {
    // Copy the data (with casting) to the output buffer
    typename InputImageType::ConstPointer inputImage 
                        = m_ImportFilter->GetOutput();
    OutputIteratorType ot( inputImage, inputImage->GetBufferedRegion() );

    // copy the input image
    ot.GoToBegin();
    while( !ot.IsAtEnd() )
      {
      *outData = ot.Get();
      ++ot;
      outData += numberOfComponents;
      }
    outData = static_cast< PixelType * >( pds->outData ) + 1;
    }

  // Copy the data (with casting) to the output buffer
  SpatialObjectType::ConstPointer segmentation 
            = m_SegmentationModule->GetOutput();
  OutputSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputSpatialObjectType * >(segmentation.GetPointer());
  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  typedef itk::ImageRegionConstIterator< OutputImageType > Output2IteratorType;
  Output2IteratorType ot2( outputImage,
                           outputImage->GetBufferedRegion() );
  typename OutputImageType::PixelType outputPixelType;

  // copy the output image
  ot2.GoToBegin();
  while( !ot2.IsAtEnd() )
    {
    outputPixelType = ot2.Get();
    *outData = outputPixelType < 0.0 ? 255 : 0;
    ++ot2;
    outData += numberOfComponents;
    }
}


// =======================================================================
// Main execute method
template <class PixelType>
int LungLesionSegmentationMethod1Runner<PixelType>::
Execute( vtkVVPluginInfo *info, vtkVVProcessDataStruct *pds )
{
  m_Info = info;

  // Get the parameters.
  m_FastMarchingStoppingTime = atof(info->GetGUIProperty(info, 1, VVP_GUI_VALUE));
  m_FastMarchingDistanceFromSeeds = atof(info->GetGUIProperty(info, 2, VVP_GUI_VALUE));
  m_GACPropagationScaling = atof(info->GetGUIProperty(info, 3, VVP_GUI_VALUE));
  m_GACCurvatureScaling = atof(info->GetGUIProperty(info, 4, VVP_GUI_VALUE));
  m_GACAdvectionScaling = atof(info->GetGUIProperty(info, 5, VVP_GUI_VALUE));
  m_GACIterations = atoi(info->GetGUIProperty(info, 6, VVP_GUI_VALUE ));

  // Populate the parameters
  m_SegmentationModule->SetMaximumNumberOfIterations( m_GACIterations );
  m_SegmentationModule->SetCurvatureScaling( m_GACCurvatureScaling );
  m_SegmentationModule->SetPropagationScaling( m_GACPropagationScaling );
  m_SegmentationModule->SetAdvectionScaling( m_GACAdvectionScaling );
  m_SegmentationModule->SetStoppingValue( m_FastMarchingStoppingTime );
  m_SegmentationModule->SetDistanceFromSeeds( m_FastMarchingDistanceFromSeeds );

  // Import the input into the plugin.
  this->ImportPixelBuffer( info, pds );

  // Execute the filter
  m_ImportFilter->Update();
  std::cout << "Done importing data" << std::endl;

  // Do Cropping (if cropping is ON).  The

  typename InputImageType::RegionType  inputImageRegion;
  typename InputImageType::IndexType   inputImageRegionStart;
  typename InputImageType::SizeType    inputImageRegionSize;

  // convert cropping planes into indexes 
  int idx[6];
  for ( unsigned int j = 0; j < 6; ++j )
    {
    idx[j] = (int)( (m_Info->CroppingPlanes[j]-
                     m_Info->InputVolumeOrigin[j/2])/
                     m_Info->InputVolumeSpacing[j/2] + 0.5);

    // Truncate them if they go out of bounds
    if (idx[j] < 0) 
      {
      idx[j] = 0;
      }
    if (idx[j] >= m_Info->InputVolumeDimensions[j/2])
      {
      idx[j] = m_Info->InputVolumeDimensions[j/2] -1;
      }
    }

  inputImageRegionStart[0] = static_cast<int>( idx[0] );
  inputImageRegionStart[1] = static_cast<int>( idx[2] );
  inputImageRegionStart[2] = static_cast<int>( idx[4] );
  inputImageRegionSize[0] =  static_cast<int>( (idx[1] - idx[0] + 1 ) );
  inputImageRegionSize[1] =  static_cast<int>( (idx[3] - idx[2] + 1 ) );
  inputImageRegionSize[2] =  static_cast<int>( (idx[5] - idx[4] + 1 ) );
  inputImageRegion.SetIndex( inputImageRegionStart );
  inputImageRegion.SetSize( inputImageRegionSize );

  // Crop
  info->UpdateProgress( info, 0.0, "Cropping...");
  m_CropFilter->SetRegionOfInterest(inputImageRegion);
  m_CropFilter->Update();
  std::cout << "Done cropping data" << std::endl;

  // Convert the output of cropping to a spatial object that can be fed into
  // the lesion segmentation method
  typename InputImageType::Pointer inputImage = m_CropFilter->GetOutput();
  inputImage->DisconnectPipeline();
  m_InputSpatialObject->SetImage(inputImage);

  // Get the location of the seed points.
  const unsigned int numberOfSeeds = info->NumberOfMarkers;
  typedef itk::LandmarkSpatialObject< 3 > SeedSpatialObjectType;
  SeedSpatialObjectType::PointListType seeds;

  if (numberOfSeeds == 0)
    {
    info->SetProperty( info, VVP_ERROR, 
      "Please place at least one seed within the lesion"); 
    return -1;
    }
  
  // Populate the seeds into a LandmarkSpatialObject
  for( unsigned int i=0; i<numberOfSeeds; i++)
    {
    const MarkersCoordinatesType *seedCoords = info->Markers + 3*i;
    SeedSpatialObjectType::LandmarkPointType point;
    point.SetPosition(seedCoords[0], seedCoords[1], seedCoords[2]);
    seeds.push_back(point);
    }
  SeedSpatialObjectType::Pointer seedSpatialObject =
    SeedSpatialObjectType::New();
  seedSpatialObject->SetPoints(seeds);
  m_LesionSegmentationMethod->SetInitialSegmentation(seedSpatialObject);
  
  // Do the actual segmentation.

  info->UpdateProgress( info, 0.1, "Segmenting...");
  try
    {  
    std::cout << "Running Lesion segmentation method" << std::endl;
    m_LesionSegmentationMethod->Update();
    }
  catch( itk::ExceptionObject & except )
    {
    info->SetProperty( info, VVP_ERROR, except.what() ); 
    return -1;
    }

  return 0;
}


//=========================================================================
static int ProcessData(void *inf, vtkVVProcessDataStruct *pds)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  if (info->InputVolumeNumberOfComponents != 1)
    {
    info->SetProperty(
      info, VVP_ERROR, "The input volume must be single component.");
    return 1;
    }

  int result = 0;

  // Support all scalar types as input
  try
  {
  switch( info->InputVolumeScalarType )
    {
     case VTK_CHAR:
      {
      LungLesionSegmentationMethod1Runner<signed char> runner;
      result = runner.Execute( info, pds );
      break;
      }  
    case VTK_UNSIGNED_CHAR:
      {
      LungLesionSegmentationMethod1Runner<unsigned char> runner;
      result = runner.Execute( info, pds );
      break;
      } 
    case VTK_SHORT:
      {
      LungLesionSegmentationMethod1Runner<signed short> runner;
      result = runner.Execute( info, pds );
      break;
      } 
    case VTK_UNSIGNED_SHORT:
      {
      LungLesionSegmentationMethod1Runner<unsigned short> runner;
      result = runner.Execute( info, pds );
      break;
      }
    case VTK_INT:
      {
      LungLesionSegmentationMethod1Runner<signed int> runner;
      result = runner.Execute( info, pds );
      break;
      } 
    case VTK_UNSIGNED_INT:
      {
      LungLesionSegmentationMethod1Runner<unsigned int> runner;
      result = runner.Execute( info, pds );
      break;
      } 
    case VTK_LONG:
      {
      LungLesionSegmentationMethod1Runner<signed long> runner;
      result = runner.Execute( info, pds );
      break;
      }
    case VTK_UNSIGNED_LONG:
      {
      LungLesionSegmentationMethod1Runner<unsigned long> runner;
      result = runner.Execute( info, pds );
      break;
      }
    case VTK_FLOAT:
      {
      LungLesionSegmentationMethod1Runner<float> runner;
      result = runner.Execute( info, pds );
      break;
      }  
    }
  }
  catch( itk::ExceptionObject & except )
    {
    info->SetProperty( info, VVP_ERROR, except.what() );
    return -1;
    }

  return result;
}


//=========================================================================
static int UpdateGUI(void *inf)
{
  vtkVVPluginInfo *info = (vtkVVPluginInfo *)inf;

  info->SetGUIProperty(info, 0, VVP_GUI_LABEL, "Overlay");
  info->SetGUIProperty(info, 0, VVP_GUI_TYPE, VVP_GUI_CHECKBOX);
  info->SetGUIProperty(info, 0, VVP_GUI_DEFAULT, "0");
  info->SetGUIProperty(info, 0, VVP_GUI_HELP,
    "Checking the box results in the output format being having two components, the segmentation is overlaid on top of the input.");

  info->SetGUIProperty(info, 1, VVP_GUI_LABEL, "Fast marching stopping time");
  info->SetGUIProperty(info, 1, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 1, VVP_GUI_DEFAULT, "5.0");
  info->SetGUIProperty(info, 1, VVP_GUI_HELP, "Stopping time for the fast marching PDE.");
  info->SetGUIProperty(info, 1, VVP_GUI_HINTS , "1 50 0.1");

  info->SetGUIProperty(info, 2, VVP_GUI_LABEL, "Fast marching distance from seeds");
  info->SetGUIProperty(info, 2, VVP_GUI_TYPE, VVP_GUI_SCALE);
  info->SetGUIProperty(info, 2, VVP_GUI_DEFAULT, "2.0");
  info->SetGUIProperty(info, 2, VVP_GUI_HELP, "Distance from the seeds that the fast marching front will start from. The initial zero set will be placed at this value from the specified seeds.");
  info->SetGUIProperty(info, 2, VVP_GUI_HINTS , "0 20 1");
  
  info->SetGUIProperty(info, 3, VVP_GUI_LABEL, "GAC Propagation");
  info->SetGUIProperty(info, 3, VVP_GUI_TYPE, VVP_GUI_SCALE); 
  info->SetGUIProperty(info, 3, VVP_GUI_DEFAULT, "10.0"); 
  info->SetGUIProperty(info, 3, VVP_GUI_HELP, "Propagation weight for the geodesic active contour"); 
  info->SetGUIProperty(info, 3, VVP_GUI_HINTS , "1.0 10.0, .1"); 

  info->SetGUIProperty(info, 4, VVP_GUI_LABEL, "GAC Curvature");
  info->SetGUIProperty(info, 4, VVP_GUI_TYPE, VVP_GUI_SCALE); 
  info->SetGUIProperty(info, 4, VVP_GUI_DEFAULT, "1.0"); 
  info->SetGUIProperty(info, 4, VVP_GUI_HELP, "Curvature weight for the geodesic active contour"); 
  info->SetGUIProperty(info, 4, VVP_GUI_HINTS , "1.0 10.0, .1"); 

  info->SetGUIProperty(info, 5, VVP_GUI_LABEL, "GAC Advection");
  info->SetGUIProperty(info, 5, VVP_GUI_TYPE, VVP_GUI_SCALE); 
  info->SetGUIProperty(info, 5, VVP_GUI_DEFAULT, "50.0"); 
  info->SetGUIProperty(info, 5, VVP_GUI_HELP, "Advection weight for the geodesic active contour"); 
  info->SetGUIProperty(info, 5, VVP_GUI_HINTS , "1.0 100.0, .1"); 
  
  info->SetGUIProperty(info, 6, VVP_GUI_LABEL, "Iterations");
  info->SetGUIProperty(info, 6, VVP_GUI_TYPE, VVP_GUI_SCALE); 
  info->SetGUIProperty(info, 6, VVP_GUI_DEFAULT, "8"); 
  info->SetGUIProperty(info, 6, VVP_GUI_HELP, "Number of interactions for the Geodesic active contour level set.");
  info->SetGUIProperty(info, 6, VVP_GUI_HINTS , "10 500 1");

  info->OutputVolumeScalarType = info->InputVolumeScalarType;
  memcpy(info->OutputVolumeDimensions,info->InputVolumeDimensions,
         3*sizeof(int)); 
  memcpy(info->OutputVolumeSpacing,info->InputVolumeSpacing,
         3*sizeof(float));
  memcpy(info->OutputVolumeOrigin,info->InputVolumeOrigin,
         3*sizeof(float));


  // what output format is selected
  if (info->GetGUIProperty(info, 0, VVP_GUI_VALUE) &&
      atoi(info->GetGUIProperty(info, 0, VVP_GUI_VALUE)))
    {
    info->OutputVolumeNumberOfComponents = 2;
    }
  else
    {
    info->OutputVolumeNumberOfComponents = 1;
    }

  return 1;
}


extern "C" {

void VV_PLUGIN_EXPORT
vvITKLungLesionSegmentationMethod1Init(vtkVVPluginInfo *info) 
{
  vvPluginVersionCheck();

  // setup information that never changes
  info->ProcessData = ProcessData;
  info->UpdateGUI   = UpdateGUI;
  info->SetProperty(info, VVP_NAME, "CT lung lesions");
  info->SetProperty(info, VVP_GROUP, "Lesion Sizing");
  info->SetProperty(info, VVP_TERSE_DOCUMENTATION,
                            "Segment solid and part solid CT lung lesions");
  info->SetProperty(info, VVP_FULL_DOCUMENTATION,
    "Segments solid and part-solid lesions in CT lung datasets. At least seed point is required within the lesion. One would also do well to specify cropping planes that bound the lesion, so as to speed up the segmentation.");
  info->SetProperty(info, VVP_SUPPORTS_IN_PLACE_PROCESSING, "0");
  info->SetProperty(info, VVP_SUPPORTS_PROCESSING_PIECES,   "0");
  info->SetProperty(info, VVP_NUMBER_OF_GUI_ITEMS,          "7"); 
  info->SetProperty(info, VVP_REQUIRED_Z_OVERLAP,           "0");
  info->SetProperty(info, VVP_PER_VOXEL_MEMORY_REQUIRED,    "0");
  info->SetProperty(info, VVP_REQUIRES_SECOND_INPUT,        "0");
  info->SetProperty(info, VVP_RESULTING_COMPONENTS_ARE_INDEPENDENT, "0");
}

}

