#include "LesionSegmentationCLI.h"
#include "itkLesionSegmentationImageFilter8.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "LesionSegmentationCLI.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkLesionSegmentationCommandLineProgressReporter.h"
#include "itkEventObject.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkMassProperties.h"
#include "vtkImageData.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkSTLWriter.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageData.h"
#include "vtkProperty.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellPicker.h"
#include "vtkPolyDataNormals.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCommand.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

typedef short PixelType;
const unsigned int ImageDimension = 3;
typedef itk::Image< PixelType, ImageDimension > InputImageType;
typedef itk::Image< float, ImageDimension > RealImageType;


// --------------------------------------------------------------------------
class SwitchVisibilityCallback : public vtkCommand
{
public:
  static SwitchVisibilityCallback *New()
    { return new SwitchVisibilityCallback; }

  void SetActor(vtkActor *aActor)
    {
    this->Actor=aActor;
    }
  void SetRenderWindow(vtkRenderWindow *aRenWin)
    {
    this->RenWin=aRenWin;
    }

  virtual void Execute(vtkObject *vtkNotUsed(caller), unsigned long, void*)
    {
    this->Actor->SetVisibility(1-this->Actor->GetVisibility());
    this->RenWin->Render();
    }

protected:
  vtkActor *Actor;
  vtkRenderWindow *RenWin;
};


// --------------------------------------------------------------------------
InputImageType::Pointer GetImage( std::string dir, bool ignoreDirection )
{
  const unsigned int Dimension = ImageDimension;
  typedef itk::Image< PixelType, Dimension >         ImageType;

  typedef itk::ImageSeriesReader< ImageType >        ReaderType;
  ReaderType::Pointer reader = ReaderType::New();

  typedef itk::GDCMImageIO       ImageIOType;
  ImageIOType::Pointer dicomIO = ImageIOType::New();

  reader->SetImageIO( dicomIO );

  typedef itk::GDCMSeriesFileNames NamesGeneratorType;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

  nameGenerator->SetUseSeriesDetails( true );
  nameGenerator->AddSeriesRestriction("0008|0021" );
  nameGenerator->SetDirectory( dir );

  try
    {
    std::cout << std::endl << "The directory: " << std::endl;
    std::cout << std::endl << dir << std::endl << std::endl;
    std::cout << "Contains the following DICOM Series: ";
    std::cout << std::endl << std::endl;

    typedef std::vector< std::string >    SeriesIdContainer;

    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();

    SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
    while( seriesItr != seriesEnd )
      {
      std::cout << seriesItr->c_str() << std::endl;
      seriesItr++;
      }


    std::string seriesIdentifier;
    seriesIdentifier = seriesUID.begin()->c_str();


    std::cout << std::endl << std::endl;
    std::cout << "Now reading series: " << std::endl << std::endl;
    std::cout << seriesIdentifier << std::endl;
    std::cout << std::endl << std::endl;


    typedef std::vector< std::string >   FileNamesContainer;
    FileNamesContainer fileNames;

    fileNames = nameGenerator->GetFileNames( seriesIdentifier );

    FileNamesContainer::const_iterator  fitr = fileNames.begin();
    FileNamesContainer::const_iterator  fend = fileNames.end();

    while( fitr != fend )
      {
      std::cout << *fitr << std::endl;
      ++fitr;
      }


    reader->SetFileNames( fileNames );

    try
      {
      reader->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return NULL;
      }


    ImageType::Pointer image = reader->GetOutput();
    ImageType::DirectionType direction;
    direction.SetIdentity();
    image->DisconnectPipeline();
    std::cout << "Image Direction:" << image->GetDirection() << std::endl;


    if (ignoreDirection)
      {
      std::cout << "Ignoring the direction of the DICOM image and using identity." << std::endl;
      image->SetDirection(direction);
      }
    return image;
    }
  catch (itk::ExceptionObject &ex)
    {
    std::cout << ex << std::endl;
    return NULL;
    }

  return NULL;
}

// --------------------------------------------------------------------------
int ViewImageAndSegmentationSurface(
    InputImageType::Pointer image, vtkPolyData *pd, LesionSegmentationCLI &args )
{

  std::cout << "Setting up visualization..." << std::endl;
  typedef itk::ImageToVTKImageFilter< InputImageType > RealITKToVTKFilterType;
  RealITKToVTKFilterType::Pointer itk2vtko = RealITKToVTKFilterType::New();
  itk2vtko->SetInput( image );
  itk2vtko->Update();

  // display the results.
  VTK_CREATE( vtkRenderer, renderer );
  VTK_CREATE( vtkRenderWindow, renWin );
  VTK_CREATE( vtkRenderWindowInteractor, iren );

  renWin->SetSize(600, 600);
  renWin->AddRenderer(renderer);
  iren->SetRenderWindow(renWin);

  // use cell picker for interacting with the image orthogonal views.
  //
  VTK_CREATE(vtkCellPicker , picker);
  picker->SetTolerance(0.005);


  //assign default props to the ipw's texture plane actor
  VTK_CREATE(vtkProperty , ipwProp);


  // Create 3 orthogonal view using the ImagePlaneWidget
  //
  VTK_CREATE(vtkImagePlaneWidget , xImagePlaneWidget);
  VTK_CREATE(vtkImagePlaneWidget , yImagePlaneWidget);
  VTK_CREATE(vtkImagePlaneWidget , zImagePlaneWidget);

  // The 3 image plane widgets are used to probe the dataset.
  //
  xImagePlaneWidget->DisplayTextOn();
  xImagePlaneWidget->SetInput(itk2vtko->GetOutput());
  xImagePlaneWidget->SetPlaneOrientationToXAxes();
  xImagePlaneWidget->SetSlicePosition(pd->GetCenter()[0]);
  xImagePlaneWidget->SetPicker(picker);
  xImagePlaneWidget->RestrictPlaneToVolumeOn();
  xImagePlaneWidget->SetKeyPressActivationValue('x');
  xImagePlaneWidget->GetPlaneProperty()->SetColor(0, 1, 0);
  xImagePlaneWidget->SetTexturePlaneProperty(ipwProp);
  xImagePlaneWidget->SetResliceInterpolateToNearestNeighbour();

  yImagePlaneWidget->DisplayTextOn();
  yImagePlaneWidget->SetInput(itk2vtko->GetOutput());
  yImagePlaneWidget->SetPlaneOrientationToYAxes();
  yImagePlaneWidget->SetSlicePosition(pd->GetCenter()[1]);
  yImagePlaneWidget->SetPicker(picker);
  yImagePlaneWidget->RestrictPlaneToVolumeOn();
  yImagePlaneWidget->SetKeyPressActivationValue('y');
  yImagePlaneWidget->GetPlaneProperty()->SetColor(1, 1, 0);
  yImagePlaneWidget->SetTexturePlaneProperty(ipwProp);
  yImagePlaneWidget->SetLookupTable(xImagePlaneWidget->GetLookupTable());

  zImagePlaneWidget->DisplayTextOn();
  zImagePlaneWidget->SetInput(itk2vtko->GetOutput());
  zImagePlaneWidget->SetPlaneOrientationToZAxes();
  zImagePlaneWidget->SetSlicePosition(pd->GetCenter()[2]);
  zImagePlaneWidget->SetPicker(picker);
  zImagePlaneWidget->SetKeyPressActivationValue('z');
  zImagePlaneWidget->GetPlaneProperty()->SetColor(0, 0, 1);
  zImagePlaneWidget->SetTexturePlaneProperty(ipwProp);
  zImagePlaneWidget->SetLookupTable(xImagePlaneWidget->GetLookupTable());
  xImagePlaneWidget->SetWindowLevel(1500,-650);
  yImagePlaneWidget->SetWindowLevel(1500,-650);
  zImagePlaneWidget->SetWindowLevel(1500,-650);

  xImagePlaneWidget->SetInteractor( iren );
  xImagePlaneWidget->On();

  yImagePlaneWidget->SetInteractor( iren );
  yImagePlaneWidget->On();

  zImagePlaneWidget->SetInteractor( iren );
  zImagePlaneWidget->On();

  // Set the background to something grayish
  renderer->SetBackground(0.4392, 0.5020, 0.5647);

  VTK_CREATE(vtkPolyDataMapper , polyMapper);
  VTK_CREATE(vtkActor          , polyActor );

  std::cout << "Computing surface normals for shading.." << std::endl;
  VTK_CREATE( vtkPolyDataNormals, normals );
  normals->SetInput(pd);
  normals->SetFeatureAngle(60.0);
  normals->Update();

  polyActor->SetMapper( polyMapper );
  polyMapper->SetInput( normals->GetOutput() );

  VTK_CREATE(vtkProperty , property);
  property->SetAmbient(0.1);
  property->SetDiffuse(0.1);
  property->SetSpecular(0.5);
  property->SetColor(1.0,0.0,0.0);
  property->SetLineWidth(2.0);
  property->SetRepresentationToSurface();

  polyActor->SetProperty( property );
  renderer->AddActor( polyActor );

  if (args.GetOptionWasSet("Wireframe"))
    {
    property->SetRepresentationToWireframe();
    }

  // Bring up the render window and begin interaction.
  renderer->ResetCamera();
  renWin->Render();

  VTK_CREATE( vtkInteractorStyleTrackballCamera, style );
  iren->SetInteractorStyle(style);

  SwitchVisibilityCallback *switchVisibility=SwitchVisibilityCallback::New();
  switchVisibility->SetRenderWindow(renWin);
  switchVisibility->SetActor(polyActor);
  iren->AddObserver(vtkCommand::UserEvent,switchVisibility);
  switchVisibility->Delete();

  std::cout << "Bringing up visualization.." << std::endl;
  iren->Start();

  return EXIT_SUCCESS;
}


// --------------------------------------------------------------------------
int main( int argc, char * argv[] )
{
  LesionSegmentationCLI args( argc, argv );

  typedef itk::ImageFileReader< InputImageType > InputReaderType;
  typedef itk::ImageFileWriter< RealImageType > OutputWriterType;
  typedef itk::LesionSegmentationImageFilter8<
          InputImageType, RealImageType > SegmentationFilterType;


  // Read the volume
  InputReaderType::Pointer reader = InputReaderType::New();
  InputImageType::Pointer image;

  std::cout << "Reading " << args.GetValueAsString("InputImage") << ".." << std::endl;
  if (!args.GetValueAsString("InputDICOMDir").empty())
    {
    std::cout << "Reading from DICOM dir " << args.GetValueAsString("InputDICOMDir") << ".." << std::endl;
    image = GetImage(
      args.GetValueAsString("InputDICOMDir"),
      args.GetValueAsBool("IgnoreDirection"));

    if (!image)
      {
      std::cerr << "Failed to read the input image" << std::endl;
      return EXIT_FAILURE;
      }
    }

  if (!args.GetValueAsString("InputImage").empty())
    {
    reader->SetFileName(args.GetValueAsString("InputImage"));
    reader->Update();
    image = reader->GetOutput();
    }


  // Compute the ROI region

  double *roi = args.GetROI();
  InputImageType::PointType origin = image->GetOrigin();
  InputImageType::SpacingType spacing = image->GetSpacing();
  InputImageType::RegionType region = image->GetLargestPossibleRegion();

  // convert bounds into region indices
  InputImageType::PointType p1, p2;
  InputImageType::IndexType pi1, pi2;
  for (int i = 0; i < ImageDimension; i++)
    {
    p1[i] = roi[2*i];
    p2[i] = roi[2*i+1];
    }

  image->TransformPhysicalPointToIndex(p1, pi1);
  image->TransformPhysicalPointToIndex(p2, pi2);

  InputImageType::SizeType roiSize;
  for (int i = 0; i < ImageDimension; i++)
    {
    roiSize[i] = pi2[i] - pi1[i];
    }
  InputImageType::RegionType roiRegion( pi1, roiSize );
  std::cout << "ROI region is " << roiRegion << std::endl;
  if (!roiRegion.Crop(image->GetBufferedRegion()))
    {
    std::cerr << "ROI region has no overlap with the image region"
              << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "ROI region cropped to image bounds is "
            << roiRegion << std::endl;


  // Write ROI if requested
  if (args.GetOptionWasSet("OutputROI"))
    {
    typedef itk::RegionOfInterestImageFilter<
      InputImageType, InputImageType > ROIFilterType;

    ROIFilterType::Pointer roiFilter = ROIFilterType::New();
    roiFilter->SetRegionOfInterest( roiRegion );

    typedef itk::ImageFileWriter< InputImageType > ROIWriterType;
    ROIWriterType::Pointer roiWriter = ROIWriterType::New();
    roiWriter->SetFileName( args.GetValueAsString("OutputROI") );
    roiFilter->SetInput( image );
    roiWriter->SetInput( roiFilter->GetOutput() );

    std::cout << "Writing the ROI image as: " <<
      args.GetValueAsString("OutputROI") << std::endl;
    try
      {
      roiWriter->Update();
      }
    catch( itk::ExceptionObject & err )
      {
      std::cerr << "ExceptionObject caught !" << err << std::endl;
      return EXIT_FAILURE;
      }
    }


  // Progress reporting
  typedef itk::LesionSegmentationCommandLineProgressReporter ProgressReporterType;
  ProgressReporterType::Pointer progressCommand =
    ProgressReporterType::New();

  // Run the segmentation filter. Clock ticking...

  std::cout << "\n Running the segmentation filter." << std::endl;
  SegmentationFilterType::Pointer seg = SegmentationFilterType::New();
  seg->SetInput(image);
  seg->SetSeeds(args.GetSeeds());
  seg->SetRegionOfInterest(roiRegion);
  seg->AddObserver( itk::ProgressEvent(), progressCommand );
  if (args.GetOptionWasSet("Sigma"))
    {
    seg->SetSigma(args.GetSigmas());
    }
  seg->SetSigmoidBeta(args.GetValueAsBool("PartSolid") ? -500 : -200 );
  seg->Update();


  if (!args.GetValueAsString("OutputImage").empty())
    {
    std::cout << "Writing the output segmented level set."
      << args.GetValueAsString("OutputImage") <<
      ". The segmentation is an isosurface of this image at a value of -0.5"
      << std::endl;
    OutputWriterType::Pointer writer = OutputWriterType::New();
    writer->SetFileName(args.GetValueAsString("OutputImage"));
    writer->SetInput(seg->GetOutput());
    writer->Update();
    }

  // Compute volume

  typedef itk::ImageToVTKImageFilter< RealImageType > RealITKToVTKFilterType;
  RealITKToVTKFilterType::Pointer itk2vtko = RealITKToVTKFilterType::New();
  itk2vtko->SetInput( seg->GetOutput() );
  itk2vtko->Update();

  std::cout << "Generating an isosurface of the zero-level set (iso-value of -0.5)" << std::endl;
  vtkSmartPointer< vtkMarchingCubes > mc =
    vtkSmartPointer< vtkMarchingCubes >::New();
  mc->SetInput(itk2vtko->GetOutput());
  mc->SetValue(0,-0.5);
  mc->Update();

  if (mc->GetOutput()->GetNumberOfCells() == 0)
    {
    std::cerr << "Segmentation failed !" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Computing volume of mesh using discretized divergence" << std::endl;
  vtkSmartPointer< vtkMassProperties > mp =
    vtkSmartPointer< vtkMassProperties >::New();
  mp->SetInput(mc->GetOutput());
  const double volume = mp->GetVolume();

  std::cout << "Volume of segmentation (mm^3) = " << volume << std::endl;

  if( args.GetOptionWasSet("OutputMesh"))
    {
    VTK_CREATE(vtkSTLWriter, writer);
    writer->SetFileName(args.GetValueAsString("OutputMesh").c_str());
    writer->SetInput( mc->GetOutput() );
    writer->Write();
    }

  if (args.GetOptionWasSet("Visualize"))
    {
    return ViewImageAndSegmentationSurface(
             image, mc->GetOutput(), args);
    }


  return EXIT_SUCCESS;
}
