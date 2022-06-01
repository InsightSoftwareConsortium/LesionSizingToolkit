/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkTestingMacros.h"


int
itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage ";
    std::cerr << " [sigma alpha1 alpha2 " << std::endl;
    std::cerr << " sigmoidAlpha sigmoidBeta]" << std::endl;
    return EXIT_FAILURE;
  }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image<InputPixelType, Dimension>;

  using InputImageReaderType = itk::ImageFileReader<InputImageType>;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(inputImageReader->Update());


  using AggregatorType = itk::MinimumFeatureAggregator<Dimension>;

  AggregatorType::Pointer featureAggregator = AggregatorType::New();

  using FeatureGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator<Dimension>;
  using SpatialObjectType = FeatureGeneratorType::SpatialObjectType;

  FeatureGeneratorType::Pointer featureGenerator1 = FeatureGeneratorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(featureGenerator1, SatoVesselnessSigmoidFeatureGenerator, FeatureGenerator);

  FeatureGeneratorType::Pointer featureGenerator2 = FeatureGeneratorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(featureGenerator2, SatoVesselnessSigmoidFeatureGenerator, FeatureGenerator);

  FeatureGeneratorType::Pointer featureGenerator3 = FeatureGeneratorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(featureGenerator3, SatoVesselnessSigmoidFeatureGenerator, FeatureGenerator);

  FeatureGeneratorType::Pointer featureGenerator4 = FeatureGeneratorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(featureGenerator4, SatoVesselnessSigmoidFeatureGenerator, FeatureGenerator);


  double smallestSigma = 1.0;
  if (argc > 3)
  {
    smallestSigma = std::stod(argv[3]);
  }
  featureGenerator1->SetSigma(smallestSigma);
  ITK_TEST_SET_GET_VALUE(smallestSigma, featureGenerator1->GetSigma());
  double smallestSigma2 = smallestSigma * 2.0;
  featureGenerator2->SetSigma(smallestSigma2);
  ITK_TEST_SET_GET_VALUE(smallestSigma2, featureGenerator2->GetSigma());
  double smallestSigma3 = smallestSigma * 4.0;
  featureGenerator3->SetSigma(smallestSigma3);
  ITK_TEST_SET_GET_VALUE(smallestSigma3, featureGenerator3->GetSigma());
  double smallestSigma4 = smallestSigma * 8.0;
  featureGenerator4->SetSigma(smallestSigma4);
  ITK_TEST_SET_GET_VALUE(smallestSigma4, featureGenerator4->GetSigma());

  double alpha1 = 0.5;
  if (argc > 4)
  {
    alpha1 = std::stod(argv[4]);
  }
  featureGenerator1->SetAlpha1(alpha1);
  ITK_TEST_SET_GET_VALUE(alpha1, featureGenerator1->GetAlpha1());
  featureGenerator2->SetAlpha1(alpha1);
  ITK_TEST_SET_GET_VALUE(alpha1, featureGenerator2->GetAlpha1());
  featureGenerator3->SetAlpha1(alpha1);
  ITK_TEST_SET_GET_VALUE(alpha1, featureGenerator3->GetAlpha1());
  featureGenerator4->SetAlpha1(alpha1);
  ITK_TEST_SET_GET_VALUE(alpha1, featureGenerator4->GetAlpha1());

  double alpha2 = 2.0;
  if (argc > 5)
  {
    alpha2 = std::stod(argv[5]);
  }
  featureGenerator1->SetAlpha2(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator1->GetAlpha2());
  featureGenerator2->SetAlpha2(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator2->GetAlpha2());
  featureGenerator3->SetAlpha2(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator3->GetAlpha2());
  featureGenerator4->SetAlpha2(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator4->GetAlpha2());

  double sigmoidAlpha = -1.0;
  if (argc > 6)
  {
    sigmoidAlpha = std::stod(argv[6]);
  }
  featureGenerator1->SetSigmoidAlpha(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator1->GetSigmoidAlpha());
  featureGenerator2->SetSigmoidAlpha(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator2->GetSigmoidAlpha());
  featureGenerator3->SetSigmoidAlpha(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator3->GetSigmoidAlpha());
  featureGenerator4->SetSigmoidAlpha(alpha2);
  ITK_TEST_SET_GET_VALUE(alpha2, featureGenerator4->GetSigmoidAlpha());

  double sigmoidBeta = 90.0;
  if (argc > 7)
  {
    sigmoidBeta = std::stod(argv[7]);
  }
  featureGenerator1->SetSigmoidBeta(sigmoidBeta);
  ITK_TEST_SET_GET_VALUE(sigmoidBeta, featureGenerator1->GetSigmoidBeta());
  featureGenerator2->SetSigmoidBeta(sigmoidBeta);
  ITK_TEST_SET_GET_VALUE(sigmoidBeta, featureGenerator2->GetSigmoidBeta());
  featureGenerator3->SetSigmoidBeta(sigmoidBeta);
  ITK_TEST_SET_GET_VALUE(sigmoidBeta, featureGenerator3->GetSigmoidBeta());
  featureGenerator4->SetSigmoidBeta(sigmoidBeta);
  ITK_TEST_SET_GET_VALUE(sigmoidBeta, featureGenerator4->GetSigmoidBeta());


  using SpatialObjectType = AggregatorType::SpatialObjectType;

  using InputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, InputPixelType>;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage(inputImage);

  featureGenerator1->SetInput(inputObject);
  featureGenerator2->SetInput(inputObject);
  featureGenerator3->SetInput(inputObject);
  featureGenerator4->SetInput(inputObject);

  featureAggregator->AddFeatureGenerator(featureGenerator1);
  featureAggregator->AddFeatureGenerator(featureGenerator2);
  featureAggregator->AddFeatureGenerator(featureGenerator3);
  featureAggregator->AddFeatureGenerator(featureGenerator4);

  ITK_TRY_EXPECT_NO_EXCEPTION(featureAggregator->Update());


  SpatialObjectType::ConstPointer finalFeature = featureAggregator->GetFeature();

  using OutputImageSpatialObjectType = AggregatorType::OutputImageSpatialObjectType;
  using OutputImageType = AggregatorType::OutputImageType;

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputImageSpatialObjectType *>(finalFeature.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter<OutputImageType>;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(outputImage);
  writer->UseCompressionOn();

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  featureAggregator->Print(std::cout);


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
