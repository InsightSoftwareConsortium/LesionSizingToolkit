#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMImageIOFactory.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkMetaImageIOFactory.h"
#include "itkImageSeriesReader.h"
#include "itkEventObject.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkMassProperties.h"
#include "vtkImageData.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyData.h"
#include "vtkSmartPointer.h"
#include "vtkSTLWriter.h"
#include "vtkRenderer.h"
#include "vtkCamera.h"
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
#include "vtkOutlineSource.h"
#include "vtkCommand.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"
#include "itkOrientImageFilter.h"
#include "vtkVersion.h"

// This needs to come after the other includes to prevent the global definitions
// of PixelType to be shadowed by other declarations.
#include "itkLesionSegmentationImageFilter8.h"
#include "itkLesionSegmentationCommandLineProgressReporter.h"
#include "LesionSegmentationCLI.h"

#define VTK_CREATE(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

// --------------------------------------------------------------------------
class SwitchVisibilityCallback : public vtkCommand
{
public:
  static SwitchVisibilityCallback *
  New()
  {
    return new SwitchVisibilityCallback;
  }

  void
  SetActor(vtkActor * aActor)
  {
    this->Actor = aActor;
  }
  void
  SetRenderWindow(vtkRenderWindow * aRenWin)
  {
    this->RenWin = aRenWin;
  }

  virtual void
  Execute(vtkObject * vtkNotUsed(caller), unsigned long, void *)
  {
    this->Actor->SetVisibility(1 - this->Actor->GetVisibility());
    this->RenWin->Render();
  }

protected:
  vtkActor *        Actor;
  vtkRenderWindow * RenWin;
};


// --------------------------------------------------------------------------
LesionSegmentationCLI::InputImageType::Pointer
GetImage(std::string dir, bool ignoreDirection)
{
  const unsigned int Dimension = LesionSegmentationCLI::ImageDimension;
  using ImageType = itk::Image<LesionSegmentationCLI::PixelType, Dimension>;

  using ReaderType = itk::ImageSeriesReader<ImageType>;
  ReaderType::Pointer reader = ReaderType::New();

  using ImageIOType = itk::GDCMImageIO;
  ImageIOType::Pointer dicomIO = ImageIOType::New();

  reader->SetImageIO(dicomIO);

  using NamesGeneratorType = itk::GDCMSeriesFileNames;
  NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();

  nameGenerator->SetUseSeriesDetails(true);
  nameGenerator->AddSeriesRestriction("0008|0021");
  nameGenerator->SetDirectory(dir);

  try
  {
    std::cout << std::endl << "The directory: " << std::endl;
    std::cout << std::endl << dir << std::endl << std::endl;
    std::cout << "Contains the following DICOM Series: ";
    std::cout << std::endl << std::endl;

    using SeriesIdContainer = std::vector<std::string>;

    const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();

    SeriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    SeriesIdContainer::const_iterator seriesEnd = seriesUID.end();
    while (seriesItr != seriesEnd)
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


    using FileNamesContainer = std::vector<std::string>;
    FileNamesContainer fileNames;

    fileNames = nameGenerator->GetFileNames(seriesIdentifier);

    FileNamesContainer::const_iterator fitr = fileNames.begin();
    FileNamesContainer::const_iterator fend = fileNames.end();

    while (fitr != fend)
    {
      std::cout << *fitr << std::endl;
      ++fitr;
    }


    reader->SetFileNames(fileNames);

    try
    {
      reader->Update();
    }
    catch (itk::ExceptionObject & ex)
    {
      std::cout << ex << std::endl;
      return NULL;
    }


    ImageType::Pointer       image = reader->GetOutput();
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
  catch (itk::ExceptionObject & ex)
  {
    std::cout << ex << std::endl;
    return NULL;
  }

  return NULL;
}

// --------------------------------------------------------------------------
int
ViewImageAndSegmentationSurface(LesionSegmentationCLI::InputImageType::Pointer image,
                                vtkPolyData *                                  pd,
                                double *                                       roi,
                                LesionSegmentationCLI &                        args)
{

  std::cout << "Setting up visualization..." << std::endl;


  using InputImageType = LesionSegmentationCLI::InputImageType;

  using RealITKToVTKFilterType = itk::ImageToVTKImageFilter<LesionSegmentationCLI::InputImageType>;
  RealITKToVTKFilterType::Pointer itk2vtko = RealITKToVTKFilterType::New();
  itk2vtko->SetInput(image);
  itk2vtko->Update();

  // display the results.
  VTK_CREATE(vtkRenderer, renderer);
  VTK_CREATE(vtkRenderWindow, renWin);
  VTK_CREATE(vtkRenderWindowInteractor, iren);

  renderer->GetActiveCamera()->ParallelProjectionOn();

  renWin->SetSize(600, 600);
  renWin->AddRenderer(renderer);
  iren->SetRenderWindow(renWin);

  // use cell picker for interacting with the image orthogonal views.
  //
  VTK_CREATE(vtkCellPicker, picker);
  picker->SetTolerance(0.005);


  // assign default props to the ipw's texture plane actor
  VTK_CREATE(vtkProperty, ipwProp);


  // Create 3 orthogonal view using the ImagePlaneWidget
  //
  vtkSmartPointer<vtkImagePlaneWidget> imagePlaneWidget[3];
  for (unsigned int i = 0; i < 3; i++)
  {
    imagePlaneWidget[i] = vtkSmartPointer<vtkImagePlaneWidget>::New();

    imagePlaneWidget[i]->DisplayTextOn();
#if VTK_MAJOR_VERSION <= 5
    imagePlaneWidget[i]->SetInput(itk2vtko->GetOutput());
#else
    imagePlaneWidget[i]->SetInputData(itk2vtko->GetOutput());
#endif


    imagePlaneWidget[i]->SetPlaneOrientation(i);
    imagePlaneWidget[i]->SetSlicePosition(pd->GetCenter()[i]);
    imagePlaneWidget[i]->SetPicker(picker);
    imagePlaneWidget[i]->RestrictPlaneToVolumeOn();
    double color[3] = { 0, 0, 0 };
    color[i] = 1;
    imagePlaneWidget[i]->GetPlaneProperty()->SetColor(color);
    imagePlaneWidget[i]->SetTexturePlaneProperty(ipwProp);
    imagePlaneWidget[i]->SetResliceInterpolateToLinear();
    imagePlaneWidget[i]->SetWindowLevel(1700, -500);
    imagePlaneWidget[i]->SetInteractor(iren);
    imagePlaneWidget[i]->On();
  }

  imagePlaneWidget[0]->SetKeyPressActivationValue('x');
  imagePlaneWidget[1]->SetKeyPressActivationValue('y');
  imagePlaneWidget[2]->SetKeyPressActivationValue('z');


  // Set the background to something grayish
  renderer->SetBackground(0.4392, 0.5020, 0.5647);

  VTK_CREATE(vtkPolyDataMapper, polyMapper);
  VTK_CREATE(vtkActor, polyActor);

  std::cout << "Computing surface normals for shading.." << std::endl;
  VTK_CREATE(vtkPolyDataNormals, normals);
#if VTK_MAJOR_VERSION <= 5
  normals->SetInput(pd);
#else
  normals->SetInputData(pd);
#endif
  normals->SetFeatureAngle(60.0);
  normals->Update();

  polyActor->SetMapper(polyMapper);
#if VTK_MAJOR_VERSION <= 5
  polyMapper->SetInput(normals->GetOutput());
#else
  polyMapper->SetInputData(normals->GetOutput());
#endif

  VTK_CREATE(vtkProperty, property);
  property->SetAmbient(0.1);
  property->SetDiffuse(0.1);
  property->SetSpecular(0.5);
  property->SetColor(0, 1, 0);
  property->SetLineWidth(2.0);
  property->SetRepresentationToSurface();

  polyActor->SetProperty(property);
  renderer->AddActor(polyActor);

  if (args.GetOptionWasSet("Wireframe"))
  {
    property->SetRepresentationToWireframe();
  }

  // Bring up the render window and begin interaction.
  renderer->ResetCamera();
  renWin->Render();

  VTK_CREATE(vtkInteractorStyleTrackballCamera, style);
  iren->SetInteractorStyle(style);

  SwitchVisibilityCallback * switchVisibility = SwitchVisibilityCallback::New();
  switchVisibility->SetRenderWindow(renWin);
  switchVisibility->SetActor(polyActor);
  iren->AddObserver(vtkCommand::UserEvent, switchVisibility);
  switchVisibility->Delete();

  if (args.GetOptionWasSet("ShowBoundingBox"))
  {
    VTK_CREATE(vtkOutlineSource, outline);
    outline->SetBounds(roi);
    VTK_CREATE(vtkPolyDataMapper, outlineMapper);
#if VTK_MAJOR_VERSION <= 5
    outlineMapper->SetInput(outline->GetOutput());
#else
    outlineMapper->SetInputData(outline->GetOutput());
#endif
    VTK_CREATE(vtkActor, outlineActor);
    outlineActor->SetMapper(outlineMapper);
    outlineActor->GetProperty()->SetColor(0, 1, 0);
    renderer->AddActor(outlineActor);
  }

  std::cout << "Bringing up visualization.." << std::endl;

  if (!args.GetValueAsString("Screenshot").empty())
  {

    double camPos[3][3] = { { 1, 0, 0 }, { 0, -1, 0 }, { 0, 0, -1 } };
    double viewUp[3][3] = { { 0, 0, 1 }, { 0, 0, 1 }, { 0, -1, 0 } };

    for (unsigned int i = 0; i < 3; i++)
    {
      std::ostringstream os;
      os << args.GetValueAsString("Screenshot") << "_" << i << ".png" << std::ends;

      renderer->GetActiveCamera()->SetPosition(camPos[i]);
      renderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
      renderer->GetActiveCamera()->SetViewUp(viewUp[i]);

      for (unsigned int j = 0; j < 3; j++)
      {
        imagePlaneWidget[j]->Off();
      }
      imagePlaneWidget[i]->On();
      imagePlaneWidget[i]->SetSlicePosition(pd->GetCenter()[i]);
      renderer->ResetCamera();
      renderer->ResetCameraClippingRange();

      // Reset the camera to the full size of the view for the screenshot
      double parallelScale = 0, bounds[6], l2norm = 0;
      itk2vtko->GetOutput()->GetBounds(bounds);
      for (unsigned int k = 0; k < 3; k++)
      {
        if (k != i)
        {
          l2norm += ((bounds[2 * k + 1] - bounds[2 * k]) * (bounds[2 * k + 1] - bounds[2 * k]));
          parallelScale = std::max(parallelScale, bounds[2 * k + 1] - bounds[2 * k]);
        }
      }
      renderer->GetActiveCamera()->Zoom(sqrt(l2norm) / parallelScale);

      renWin->Render();

      VTK_CREATE(vtkWindowToImageFilter, w2f);
      w2f->SetInput(renWin);

      VTK_CREATE(vtkPNGWriter, screenshotWriter);

      screenshotWriter->SetFileName(os.str().c_str());
      std::cout << "Screenshot saved to " << os.str().c_str() << std::endl;
      screenshotWriter->SetInputConnection(w2f->GetOutputPort());
      screenshotWriter->Write();
    }
  }
  else
  {
    iren->Start();
  }

  return EXIT_SUCCESS;
}


// --------------------------------------------------------------------------
int
main(int argc, char * argv[])
{
  // register DICOM and META IO factories
  itk::ObjectFactoryBase::RegisterFactory(itk::GDCMImageIOFactory::New());
  itk::ObjectFactoryBase::RegisterFactory(itk::MetaImageIOFactory::New());

  LesionSegmentationCLI args(argc, argv);

  using InputImageType = LesionSegmentationCLI::InputImageType;
  using RealImageType = LesionSegmentationCLI::RealImageType;
  const unsigned int ImageDimension = LesionSegmentationCLI::ImageDimension;

  using InputReaderType = itk::ImageFileReader<InputImageType>;
  using OutputWriterType = itk::ImageFileWriter<RealImageType>;
  using SegmentationFilterType = itk::LesionSegmentationImageFilter8<InputImageType, RealImageType>;


  // Read the volume
  InputReaderType::Pointer reader = InputReaderType::New();
  InputImageType::Pointer  image;

  std::cout << "Reading " << args.GetValueAsString("InputImage") << ".." << std::endl;
  if (!args.GetValueAsString("InputDICOMDir").empty())
  {
    std::cout << "Reading from DICOM dir " << args.GetValueAsString("InputDICOMDir") << ".." << std::endl;
    image = GetImage(args.GetValueAsString("InputDICOMDir"), args.GetValueAsBool("IgnoreDirection"));

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


  // To make sure the tumor polydata aligns with the image volume during
  // vtk rendering in ViewImageAndSegmentationSurface(),
  // reorient image so that the direction matrix is an identity matrix.
  using InputReaderType = itk::ImageFileReader<InputImageType>;
  itk::OrientImageFilter<InputImageType, InputImageType>::Pointer orienter =
    itk::OrientImageFilter<InputImageType, InputImageType>::New();
  orienter->UseImageDirectionOn();
  InputImageType::DirectionType direction;
  direction.SetIdentity();
  orienter->SetDesiredCoordinateDirection(direction);
  orienter->SetInput(image);
  orienter->Update();
  image = orienter->GetOutput();

  // Set the image object on the args
  args.SetImage(image);


  // Compute the ROI region

  double *                   roi = args.GetROI();
  InputImageType::RegionType region = image->GetLargestPossibleRegion();

  // convert bounds into region indices
  InputImageType::PointType p1, p2;
  InputImageType::IndexType pi1, pi2;
  InputImageType::IndexType startIndex;
  for (unsigned int i = 0; i < ImageDimension; i++)
  {
    p1[i] = roi[2 * i];
    p2[i] = roi[2 * i + 1];
  }

  image->TransformPhysicalPointToIndex(p1, pi1);
  image->TransformPhysicalPointToIndex(p2, pi2);

  InputImageType::SizeType roiSize;
  for (unsigned int i = 0; i < ImageDimension; i++)
  {
    roiSize[i] = itk::Math::abs(pi2[i] - pi1[i]);
    startIndex[i] = (pi1[i] < pi2[i]) ? pi1[i] : pi2[i];
  }
  InputImageType::RegionType roiRegion(startIndex, roiSize);
  std::cout << "ROI region is " << roiRegion << std::endl;
  if (!roiRegion.Crop(image->GetBufferedRegion()))
  {
    std::cerr << "ROI region has no overlap with the image region of" << image->GetBufferedRegion() << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "ROI region is " << roiRegion << " : covers voxels = " << roiRegion.GetNumberOfPixels() << " : "
            << image->GetSpacing()[0] * image->GetSpacing()[1] * image->GetSpacing()[2] * roiRegion.GetNumberOfPixels()
            << " mm^3" << std::endl;


  // Write ROI if requested
  if (args.GetOptionWasSet("OutputROI"))
  {
    using ROIFilterType = itk::RegionOfInterestImageFilter<InputImageType, InputImageType>;

    ROIFilterType::Pointer roiFilter = ROIFilterType::New();
    roiFilter->SetRegionOfInterest(roiRegion);

    using ROIWriterType = itk::ImageFileWriter<InputImageType>;
    ROIWriterType::Pointer roiWriter = ROIWriterType::New();
    roiWriter->SetFileName(args.GetValueAsString("OutputROI"));
    roiFilter->SetInput(image);
    roiWriter->SetInput(roiFilter->GetOutput());

    std::cout << "Writing the ROI image as: " << args.GetValueAsString("OutputROI") << std::endl;
    try
    {
      roiWriter->Update();
    }
    catch (itk::ExceptionObject & err)
    {
      std::cerr << "ExceptionObject caught !" << err << std::endl;
      return EXIT_FAILURE;
    }
  }


  // Progress reporting
  using ProgressReporterType = itk::LesionSegmentationCommandLineProgressReporter;
  ProgressReporterType::Pointer progressCommand = ProgressReporterType::New();

  // Run the segmentation filter. Clock ticking...

  std::cout << "\n Running the segmentation filter." << std::endl;
  SegmentationFilterType::Pointer seg = SegmentationFilterType::New();
  seg->SetInput(image);
  seg->SetSeeds(args.GetSeeds());
  seg->SetRegionOfInterest(roiRegion);
  seg->AddObserver(itk::ProgressEvent(), progressCommand);
  if (args.GetOptionWasSet("Sigma"))
  {
    seg->SetSigma(args.GetSigmas());
  }
  seg->SetSigmoidBeta(args.GetValueAsBool("PartSolid") ? -500 : -200);
  seg->Update();


  if (!args.GetValueAsString("OutputImage").empty())
  {
    std::cout << "Writing the output segmented level set." << args.GetValueAsString("OutputImage")
              << ". The segmentation is an isosurface of this image at a value of -0.5" << std::endl;
    OutputWriterType::Pointer writer = OutputWriterType::New();
    writer->SetFileName(args.GetValueAsString("OutputImage"));
    writer->SetInput(seg->GetOutput());
    writer->Update();
  }

  // Compute volume

  using RealITKToVTKFilterType = itk::ImageToVTKImageFilter<RealImageType>;
  RealITKToVTKFilterType::Pointer itk2vtko = RealITKToVTKFilterType::New();
  itk2vtko->SetInput(seg->GetOutput());
  itk2vtko->Update();

  std::cout << "Generating an isosurface of the zero-level set (iso-value of -0.5)" << std::endl;
  vtkSmartPointer<vtkMarchingCubes> mc = vtkSmartPointer<vtkMarchingCubes>::New();
#if VTK_MAJOR_VERSION <= 5
  mc->SetInput(itk2vtko->GetOutput());
#else
  mc->SetInputData(itk2vtko->GetOutput());
#endif

  mc->SetValue(0, -0.5);
  mc->Update();

  if (mc->GetOutput()->GetNumberOfCells() == 0)
  {
    std::cerr << "Segmentation failed !" << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Computing volume of mesh using discretized divergence" << std::endl;
  vtkSmartPointer<vtkMassProperties> mp = vtkSmartPointer<vtkMassProperties>::New();
#if VTK_MAJOR_VERSION <= 5
  mp->SetInput(mc->GetOutput());
#else
  mp->SetInputData(mc->GetOutput());
#endif
  const double volume = mp->GetVolume();

  std::cout << "Volume of segmentation mm^3 = " << volume << std::endl;

  if (args.GetOptionWasSet("OutputMesh"))
  {
    VTK_CREATE(vtkSTLWriter, writer);
    writer->SetFileName(args.GetValueAsString("OutputMesh").c_str());
#if VTK_MAJOR_VERSION <= 5
    writer->SetInput(mc->GetOutput());
#else
    writer->SetInputData(mc->GetOutput());
#endif
    writer->Write();
  }

  if (args.GetOptionWasSet("Visualize"))
  {
    return ViewImageAndSegmentationSurface(image, mc->GetOutput(), roi, args);
  }


  return EXIT_SUCCESS;
}
