/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSatoVesselnessSigmoidFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkSatoVesselnessSigmoidFeatureGeneratorTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [sigma alpha1 alpha2 ";
    std::cerr << " sigmoidAlpha sigmoidBeta]" << std::endl;
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

  ITK_TRY_EXPECT_NO_EXCEPTION(reader->Update());


  using SatoVesselnessSigmoidFeatureGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator<Dimension>;
  using SpatialObjectType = SatoVesselnessSigmoidFeatureGeneratorType::SpatialObjectType;

  SatoVesselnessSigmoidFeatureGeneratorType::Pointer featureGenerator =
    SatoVesselnessSigmoidFeatureGeneratorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(
    featureGenerator, SatoVesselnessSigmoidFeatureGenerator, SatoVesselnessFeatureGenerator);


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

  double alpha1 = 0.5;
  if (argc > 4)
  {
    alpha1 = std::stod(argv[4]);
  }
  featureGenerator->SetAlpha1(alpha1);
  ITK_TEST_SET_GET_VALUE(alpha1, featureGenerator->GetAlpha1());

  double alpha2 = 2.0;
  if (argc > 5)
  {
    alpha2 = std::stod(argv[5]);
  }
  featureGenerator->SetAlpha2(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator->GetAlpha2());

  double sigmoidAlpha = 1.0;
  if (argc > 6)
  {
    sigmoidAlpha = std::stod(argv[6]);
  }
  featureGenerator->SetSigmoidAlpha(sigmoidAlpha);
  ITK_TEST_SET_GET_VALUE(sigmoidAlpha, featureGenerator->GetSigmoidAlpha());

  double sigmoidBeta = -200.0;
  if (argc > 7)
  {
    sigmoidBeta = std::stod(argv[7]);
  }
  featureGenerator->SetSigmoidBeta(sigmoidBeta);
  ITK_TEST_SET_GET_VALUE(sigmoidBeta, featureGenerator->GetSigmoidBeta());


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
