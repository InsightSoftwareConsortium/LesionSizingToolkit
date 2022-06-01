/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkMaximumFeatureAggregator.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDescoteauxSheetnessFeatureGenerator.h"
#include "itkTestingMacros.h"


int
itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1(int argc, char * argv[])
{
  if (argc < 5)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage (bright=1/dark=0) sigma numberOfScales [sheetness bloobiness noise ]"
              << std::endl;
    return EXIT_FAILURE;
  }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image<InputPixelType, Dimension>;

  using InputImageReaderType = itk::ImageFileReader<InputImageType>;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(inputImageReader->Update());

  using InputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, InputPixelType>;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage(inputImage);


  using AggregatorType = itk::MaximumFeatureAggregator<Dimension>;

  AggregatorType::Pointer featureAggregator = AggregatorType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(featureAggregator, MaximumFeatureAggregator, UnaryFunctorImageFilter);

  using DescoteauxSheetnessFeatureGeneratorType = itk::DescoteauxSheetnessFeatureGenerator<Dimension>;
  using SpatialObjectType = DescoteauxSheetnessFeatureGeneratorType::SpatialObjectType;
  using FeatureGeneratorPointer = DescoteauxSheetnessFeatureGeneratorType::Pointer;
  using FeatureGeneratorArray = std::vector<FeatureGeneratorPointer>;

  FeatureGeneratorArray featureGeneratorArray;

  unsigned int octave = 1;
  double       smallestSigma = std::stod(argv[4]);

  const unsigned int numberOfScales = std::stoi(argv[5]);

  for (unsigned int k = 0; k < numberOfScales; ++k)
  {
    DescoteauxSheetnessFeatureGeneratorType::Pointer featureGenerator = DescoteauxSheetnessFeatureGeneratorType::New();
    featureGeneratorArray.push_back(featureGenerator);

    bool detectBrightSheets = std::stoi(argv[3]);
    ITK_TEST_SET_GET_BOOLEAN(featureGenerator, DetectBrightSheets, detectBrightSheets);

    double sigma = smallestSigma * octave;
    featureGenerator->SetSigma(sigma);
    ITK_TEST_SET_GET_VALUE(sigma, featureGenerator->GetSigma());

    double sheetnessNormalization = 0.5;
    if (argc > 6)
    {
      sheetnessNormalization = std::stod(argv[6]);
    }
    featureGenerator->SetSheetnessNormalization(sheetnessNormalization);
    // ITK_TEST_SET_GET_VALUE( sheetnessNormalization, featureGenerator->GetSheetnessNormalization() );

    double bloobinessNormalization = 2.0;
    if (argc > 7)
    {
      bloobinessNormalization = std::stod(argv[7]);
    }
    featureGenerator->SetBloobinessNormalization(bloobinessNormalization);
    // ITK_TEST_SET_GET_VALUE( bloobinessNormalization, featureGenerator->GetBloobinessNormalization() );

    double noiseNormalization = 1.0;
    if (argc > 8)
    {
      noiseNormalization = std::stod(argv[8]);
    }
    featureGenerator->SetNoiseNormalization(noiseNormalization);
    // ITK_TEST_SET_GET_VALUE( noiseNormalization, featureGenerator->GetNoiseNormalization() );

    octave *= 2;

    featureGenerator->SetInput(inputObject);
    featureAggregator->AddFeatureGenerator(featureGenerator);
  }

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


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
