/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkShapeDetectionLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkShapeDetectionLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkTestingMacros.h"


int
itkShapeDetectionLevelSetSegmentationModuleTest1(int argc, char * argv[])
{
  if (argc < 4)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage featureImage outputImage ";
    std::cerr << " [propagationScaling curvatureScaling maximumNumberOfIterations]" << std::endl;
    return EXIT_FAILURE;
  }


  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::ShapeDetectionLevelSetSegmentationModule<Dimension>;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  EXERCISE_BASIC_OBJECT_METHODS(
    segmentationModule, ShapeDetectionLevelSetSegmentationModule, SinglePhaseLevelSetSegmentationModule);

  using InputImageType = SegmentationModuleType::InputImageType;
  using FeatureImageType = SegmentationModuleType::FeatureImageType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  using InputReaderType = itk::ImageFileReader<InputImageType>;
  using InputWriterType = itk::ImageFileWriter<InputImageType>;
  using FeatureReaderType = itk::ImageFileReader<FeatureImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  using RescaleFilterType = itk::RescaleIntensityImageFilter<InputImageType, InputImageType>;

  InputReaderType::Pointer   inputReader = InputReaderType::New();
  InputWriterType::Pointer   inputWriter = InputWriterType::New();
  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();

  inputReader->SetFileName(argv[1]);

  TRY_EXPECT_NO_EXCEPTION(inputReader->Update());


  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetInput(inputReader->GetOutput());

  rescaler->SetOutputMinimum(-4.0);
  rescaler->SetOutputMaximum(4.0);

  TRY_EXPECT_NO_EXCEPTION(rescaler->Update());


  featureReader->SetFileName(argv[2]);

  TRY_EXPECT_NO_EXCEPTION(featureReader->Update());

  using InputSpatialObjectType = SegmentationModuleType::InputSpatialObjectType;
  using FeatureSpatialObjectType = SegmentationModuleType::FeatureSpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;

  InputSpatialObjectType::Pointer   inputObject = InputSpatialObjectType::New();
  FeatureSpatialObjectType::Pointer featureObject = FeatureSpatialObjectType::New();

  inputObject->SetImage(rescaler->GetOutput());
  featureObject->SetImage(featureReader->GetOutput());

  segmentationModule->SetInput(inputObject);
  segmentationModule->SetFeature(featureObject);


  double propagationScaling = 1.0;
  if (argc > 4)
  {
    propagationScaling = std::stod(argv[4]);
  }
  segmentationModule->SetPropagationScaling(propagationScaling);
  TEST_SET_GET_VALUE(propagationScaling, segmentationModule->GetPropagationScaling());

  double curvatureScaling = 1.0;
  if (argc > 5)
  {
    curvatureScaling = std::stod(argv[5]);
  }
  segmentationModule->SetCurvatureScaling(curvatureScaling);
  TEST_SET_GET_VALUE(curvatureScaling, segmentationModule->GetCurvatureScaling());

  unsigned int maximumNumberOfIterations = 50;
  if (argc > 6)
  {
    maximumNumberOfIterations = std::stoi(argv[6]);
  }
  segmentationModule->SetMaximumNumberOfIterations(maximumNumberOfIterations);
  TEST_SET_GET_VALUE(maximumNumberOfIterations, segmentationModule->GetMaximumNumberOfIterations());


  TRY_EXPECT_NO_EXCEPTION(segmentationModule->Update());

  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputSpatialObjectType *>(segmentation.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[3]);
  writer->SetInput(outputImage);

  TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
