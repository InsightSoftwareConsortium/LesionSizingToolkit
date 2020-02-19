/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSigmoidFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSigmoidFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkSigmoidFeatureGeneratorTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [alpha beta]" << std::endl;
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

  using SigmoidFeatureGeneratorType = itk::SigmoidFeatureGenerator<Dimension>;
  using SpatialObjectType = SigmoidFeatureGeneratorType::SpatialObjectType;

  SigmoidFeatureGeneratorType::Pointer featureGenerator = SigmoidFeatureGeneratorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(featureGenerator, SigmoidFeatureGenerator, FeatureGenerator);

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage(inputImage);

  featureGenerator->SetInput(inputObject);

  double alpha = 1.0;
  if (argc > 3)
  {
    alpha = std::stod(argv[3]);
  }
  featureGenerator->SetAlpha(alpha);
  TEST_SET_GET_VALUE(alpha, featureGenerator->GetAlpha());

  double beta = 100.0;
  if (argc > 4)
  {
    beta = std::stod(argv[4]);
  }
  featureGenerator->SetBeta(beta);
  TEST_SET_GET_VALUE(beta, featureGenerator->GetBeta());


  TRY_EXPECT_NO_EXCEPTION(featureGenerator->Update());

  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputImageSpatialObjectType *>(feature.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(outputImage);
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
