/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest9.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// The test runs a shape detection level set from user supplied seed points
// and then runs the shape detection level set with the results from the
// fast marching to get the final segmentation.

#include "itkLesionSegmentationMethod.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLandmarksReader.h"
#include "itkImageMaskSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkGradientMagnitudeSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkTestingMacros.h"

// Applies fast marhching followed by segmentation using geodesic active contours.
int
itkLesionSegmentationMethodTest9(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile inputImage outputImage";
    std::cerr << " [RMSErrorForGeodesicActiveContour]"
              << " [IterationsForGeodesicActiveContour]"
              << " [CurvatureScalingForGeodesicActiveContour]"
              << " [PropagationScalingForGeodesicActiveContour]"
              << " [AdvectionScalingForGeodesicActiveContour]";
    std::cerr << " [stoppingTime]";
    std::cerr << " [distanceFromSeeds]" << std::endl;
    return EXIT_FAILURE;
  }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image<InputPixelType, Dimension>;

  using InputImageReaderType = itk::ImageFileReader<InputImageType>;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName(argv[2]);

  ITK_TRY_EXPECT_NO_EXCEPTION(inputImageReader->Update());

  using MethodType = itk::LesionSegmentationMethod<Dimension>;

  MethodType::Pointer lesionSegmentationMethod = MethodType::New();

  using ImageMaskSpatialObjectType = itk::ImageMaskSpatialObject<Dimension>;

  ImageMaskSpatialObjectType::Pointer regionOfInterest = ImageMaskSpatialObjectType::New();

  lesionSegmentationMethod->SetRegionOfInterest(regionOfInterest);

  using VesselnessGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator<Dimension>;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  using LungWallGeneratorType = itk::LungWallFeatureGenerator<Dimension>;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  using SigmoidFeatureGeneratorType = itk::SigmoidFeatureGenerator<Dimension>;
  SigmoidFeatureGeneratorType::Pointer sigmoidGenerator = SigmoidFeatureGeneratorType::New();

  using GradientMagnitudeSigmoidFeatureGeneratorType = itk::GradientMagnitudeSigmoidFeatureGenerator<Dimension>;
  GradientMagnitudeSigmoidFeatureGeneratorType::Pointer gradientMagnitudeSigmoidGenerator =
    GradientMagnitudeSigmoidFeatureGeneratorType::New();

  using FeatureAggregatorType = itk::MinimumFeatureAggregator<Dimension>;
  FeatureAggregatorType::Pointer featureAggregator = FeatureAggregatorType::New();
  featureAggregator->AddFeatureGenerator(lungWallGenerator);
  featureAggregator->AddFeatureGenerator(vesselnessGenerator);
  featureAggregator->AddFeatureGenerator(sigmoidGenerator);
  featureAggregator->AddFeatureGenerator(gradientMagnitudeSigmoidGenerator);
  lesionSegmentationMethod->AddFeatureGenerator(featureAggregator);

  using SpatialObjectType = MethodType::SpatialObjectType;
  using InputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, InputPixelType>;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage(inputImage);

  lungWallGenerator->SetInput(inputObject);
  vesselnessGenerator->SetInput(inputObject);
  sigmoidGenerator->SetInput(inputObject);
  gradientMagnitudeSigmoidGenerator->SetInput(inputObject);
  lungWallGenerator->SetLungThreshold(-400);
  vesselnessGenerator->SetSigma(1.0);
  vesselnessGenerator->SetAlpha1(0.5);
  vesselnessGenerator->SetAlpha2(2.0);
  vesselnessGenerator->SetSigmoidAlpha(-10.0);
  vesselnessGenerator->SetSigmoidBeta(80.0);
  sigmoidGenerator->SetAlpha(1.0);
  sigmoidGenerator->SetBeta(-200.0);
  gradientMagnitudeSigmoidGenerator->SetSigma(1.0);
  gradientMagnitudeSigmoidGenerator->SetAlpha(-100.0);
  gradientMagnitudeSigmoidGenerator->SetBeta(300);

  using SegmentationModuleType = itk::FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<Dimension>;
  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(segmentationModule,
                                FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule,
                                SinglePhaseLevelSetSegmentationModule);


  double maximumRMSError = 0.0002;
  if (argc > 4)
  {
    maximumRMSError = std::stod(argv[4]);
  }
  segmentationModule->SetMaximumRMSError(maximumRMSError);
  ITK_TEST_SET_GET_VALUE(maximumRMSError, segmentationModule->GetMaximumRMSError());

  unsigned int maximumNumberOfIterations = 300;
  if (argc > 5)
  {
    maximumNumberOfIterations = std::stoi(argv[5]);
  }
  segmentationModule->SetMaximumNumberOfIterations(maximumNumberOfIterations);
  ITK_TEST_SET_GET_VALUE(maximumNumberOfIterations, segmentationModule->GetMaximumNumberOfIterations());

  double curvatureScaling = 1.0;
  if (argc > 6)
  {
    curvatureScaling = std::stod(argv[6]);
  }
  segmentationModule->SetCurvatureScaling(curvatureScaling);
  ITK_TEST_SET_GET_VALUE(curvatureScaling, segmentationModule->GetCurvatureScaling());

  double propagationScaling = 500.0;
  if (argc > 7)
  {
    propagationScaling = std::stod(argv[7]);
  }
  segmentationModule->SetPropagationScaling(propagationScaling);
  ITK_TEST_SET_GET_VALUE(propagationScaling, segmentationModule->GetPropagationScaling());

  double advectionScaling = 0.0;
  if (argc > 8)
  {
    advectionScaling = std::stod(argv[8]);
  }
  segmentationModule->SetAdvectionScaling(advectionScaling);
  ITK_TEST_SET_GET_VALUE(advectionScaling, segmentationModule->GetAdvectionScaling());

  double stoppingValue = 5.0;
  if (argc > 9)
  {
    stoppingValue = std::stod(argv[9]);
  }
  segmentationModule->SetStoppingValue(stoppingValue);
  ITK_TEST_SET_GET_VALUE(stoppingValue, segmentationModule->GetStoppingValue());

  double distanceFromSeeds = 2.5;
  if (argc > 10)
  {
    distanceFromSeeds = std::stod(argv[10]);
  }
  segmentationModule->SetDistanceFromSeeds(distanceFromSeeds);
  ITK_TEST_SET_GET_VALUE(distanceFromSeeds, segmentationModule->GetDistanceFromSeeds());


  lesionSegmentationMethod->SetSegmentationModule(segmentationModule);

  using LandmarksReaderType = itk::LandmarksReader<Dimension>;

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(landmarksReader->Update());

  lesionSegmentationMethod->SetInitialSegmentation(landmarksReader->GetOutput());

  ITK_TRY_EXPECT_NO_EXCEPTION(lesionSegmentationMethod->Update());

  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputSpatialObjectType *>(segmentation.GetPointer());
  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter<OutputImageType>;
  OutputWriterType::Pointer writer = OutputWriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(outputImage);
  writer->UseCompressionOn();

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  //
  // Exercise the exception on the number of feature generators
  //
  lesionSegmentationMethod->AddFeatureGenerator(lungWallGenerator);

  ITK_TRY_EXPECT_EXCEPTION(lesionSegmentationMethod->Update());


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
