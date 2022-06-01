/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkGeodesicActiveContourLevelSetSegmentationModuleTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage featureImage outputImage";
    std::cerr << " [advectionScaling] [curvatureScaling] [propagationScaling] [maxIterations]" << std::endl;
    return EXIT_FAILURE;
  }


  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::GeodesicActiveContourLevelSetSegmentationModule<Dimension>;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(
    segmentationModule, GeodesicActiveContourLevelSetSegmentationModule, SinglePhaseLevelSetSegmentationModule);


  using InputImageType = SegmentationModuleType::InputImageType;
  using FeatureImageType = SegmentationModuleType::FeatureImageType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  using InputReaderType = itk::ImageFileReader<InputImageType>;
  using FeatureReaderType = itk::ImageFileReader<FeatureImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  InputReaderType::Pointer   inputReader = InputReaderType::New();
  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();

  inputReader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(inputReader->Update());

  featureReader->SetFileName(argv[2]);

  ITK_TRY_EXPECT_NO_EXCEPTION(featureReader->Update());


  using InputSpatialObjectType = SegmentationModuleType::InputSpatialObjectType;
  using FeatureSpatialObjectType = SegmentationModuleType::FeatureSpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;

  InputSpatialObjectType::Pointer   inputObject = InputSpatialObjectType::New();
  FeatureSpatialObjectType::Pointer featureObject = FeatureSpatialObjectType::New();

  inputObject->SetImage(inputReader->GetOutput());
  featureObject->SetImage(featureReader->GetOutput());

  segmentationModule->SetInput(inputObject);
  segmentationModule->SetFeature(featureObject);

  double advectionScaling = 1.0;
  if (argc > 4)
  {
    advectionScaling = std::stod(argv[4]);
  }
  segmentationModule->SetAdvectionScaling(advectionScaling);
  ITK_TEST_SET_GET_VALUE(advectionScaling, segmentationModule->GetAdvectionScaling());

  double curvatureScaling = 1.0;
  if (argc > 5)
  {
    curvatureScaling = std::stod(argv[5]);
  }
  segmentationModule->SetCurvatureScaling(curvatureScaling);
  ITK_TEST_SET_GET_VALUE(curvatureScaling, segmentationModule->GetCurvatureScaling());

  double propagationScaling = 100.0;
  if (argc > 6)
  {
    propagationScaling = std::stod(argv[6]);
  }
  segmentationModule->SetPropagationScaling(propagationScaling);
  ITK_TEST_SET_GET_VALUE(propagationScaling, segmentationModule->GetPropagationScaling());

  unsigned int maximumNumberOfIterations = 100;
  if (argc > 7)
  {
    maximumNumberOfIterations = std::stod(argv[7]);
  }
  segmentationModule->SetMaximumNumberOfIterations(maximumNumberOfIterations);
  ITK_TEST_SET_GET_VALUE(maximumNumberOfIterations, segmentationModule->GetMaximumNumberOfIterations());


  ITK_TRY_EXPECT_NO_EXCEPTION(segmentationModule->Update());

  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputSpatialObjectType *>(segmentation.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[3]);
  writer->SetInput(outputImage);

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
