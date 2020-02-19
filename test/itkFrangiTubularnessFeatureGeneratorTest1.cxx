/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkFrangiTubularnessFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkFrangiTubularnessFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkFrangiTubularnessFeatureGeneratorTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [sigma sheetness bloobiness noise ]" << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension = 3;

  using InputPixelType = signed short;
  using OutputPixelType = float;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  using InputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, InputPixelType>;
  using OutputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, OutputPixelType>;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  TRY_EXPECT_NO_EXCEPTION(reader->Update());

  using FrangiTubularnessFeatureGeneratorType = itk::FrangiTubularnessFeatureGenerator<Dimension>;
  using SpatialObjectType = FrangiTubularnessFeatureGeneratorType::SpatialObjectType;

  FrangiTubularnessFeatureGeneratorType::Pointer featureGenerator = FrangiTubularnessFeatureGeneratorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(featureGenerator, FrangiTubularnessFeatureGenerator, FeatureGenerator);

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
  TEST_SET_GET_VALUE(sigma, featureGenerator->GetSigma());

  double sheetnessNormalization = 0.5;
  if (argc > 4)
  {
    sheetnessNormalization = std::stod(argv[4]);
  }
  featureGenerator->SetSheetnessNormalization(sheetnessNormalization);
  TEST_SET_GET_VALUE(sheetnessNormalization, featureGenerator->GetSheetnessNormalization());

  double bloobinessNormalization = 2.0;
  if (argc > 5)
  {
    bloobinessNormalization = std::stod(argv[5]);
  }
  featureGenerator->SetBloobinessNormalization(bloobinessNormalization);
  TEST_SET_GET_VALUE(bloobinessNormalization, featureGenerator->GetBloobinessNormalization());

  double noiseNormalization = 1.0;
  if (argc > 6)
  {
    noiseNormalization = std::stod(argv[6]);
  }
  featureGenerator->SetNoiseNormalization(noiseNormalization);
  TEST_SET_GET_VALUE(noiseNormalization, featureGenerator->GetNoiseNormalization());


  TRY_EXPECT_NO_EXCEPTION(featureGenerator->Update());


  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputImageSpatialObjectType *>(feature.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(outputImage);

  TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
