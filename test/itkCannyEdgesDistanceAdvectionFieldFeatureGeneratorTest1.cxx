/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// The output image here is going to be an image of covariant vectors.
// (An advection field).
//
#include "itkCannyEdgesDistanceAdvectionFieldFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [variance upperthreshold lowerthreshold]" << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;
  using OutputPixelType = itk::CovariantVector<float>;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  using InputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, InputPixelType>;
  using OutputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, OutputPixelType>;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(reader->Update());


  using CannyEdgesDistanceAdvectionFieldFeatureGeneratorType =
    itk::CannyEdgesDistanceAdvectionFieldFeatureGenerator<Dimension>;
  using SpatialObjectType = CannyEdgesDistanceAdvectionFieldFeatureGeneratorType::SpatialObjectType;

  CannyEdgesDistanceAdvectionFieldFeatureGeneratorType::Pointer featureGenerator =
    CannyEdgesDistanceAdvectionFieldFeatureGeneratorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(
    featureGenerator, CannyEdgesDistanceAdvectionFieldFeatureGenerator, FeatureGenerator);


  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage(inputImage);

  featureGenerator->SetInput(inputObject);


  double sigma = 1.0;
  if (argc > 3)
  {
    sigma = std::stod(argv[3]);
  }
  featureGenerator->SetSigma(sigma);
  ITK_TEST_SET_GET_VALUE(sigma, featureGenerator->GetSigma());

  double upperThreshold =
    itk::NumericTraits<CannyEdgesDistanceAdvectionFieldFeatureGeneratorType::InternalPixelType>::max();
  if (argc > 4)
  {
    upperThreshold = std::stod(argv[4]);
  }
  featureGenerator->SetUpperThreshold(upperThreshold);
  ITK_TEST_SET_GET_VALUE(upperThreshold, featureGenerator->GetUpperThreshold());

  double lowerThreshold =
    itk::NumericTraits<CannyEdgesDistanceAdvectionFieldFeatureGeneratorType::InternalPixelType>::min();
  if (argc > 5)
  {
    lowerThreshold = std::stod(argv[5]);
  }
  featureGenerator->SetLowerThreshold(lowerThreshold);
  ITK_TEST_SET_GET_VALUE(lowerThreshold, featureGenerator->GetLowerThreshold());


  ITK_TRY_EXPECT_NO_EXCEPTION(featureGenerator->Update());


  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputImageSpatialObjectType *>(feature.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(outputImage);
  writer->UseCompressionOn();

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
