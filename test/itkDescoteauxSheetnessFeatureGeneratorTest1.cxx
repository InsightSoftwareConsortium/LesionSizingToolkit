/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkDescoteauxSheetnessFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkDescoteauxSheetnessFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int itkDescoteauxSheetnessFeatureGeneratorTest1( int argc, char * argv [] )
{
  if( argc < 3 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [(bright1/dark:0) sigma sheetness bloobiness noise ]" << std::endl;
    return EXIT_FAILURE;
    }

  constexpr unsigned int Dimension = 3;

  using InputPixelType = signed short;
  using OutputPixelType = float;

  using InputImageType = itk::Image< InputPixelType,  Dimension >;
  using OutputImageType = itk::Image< OutputPixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< InputImageType >;
  using WriterType = itk::ImageFileWriter< OutputImageType >;

  using InputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, InputPixelType  >;
  using OutputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, OutputPixelType >;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  TRY_EXPECT_NO_EXCEPTION( reader->Update() );

  using DescoteauxSheetnessFeatureGeneratorType = itk::DescoteauxSheetnessFeatureGenerator< Dimension >;
  using SpatialObjectType = DescoteauxSheetnessFeatureGeneratorType::SpatialObjectType;

  DescoteauxSheetnessFeatureGeneratorType::Pointer featureGenerator = DescoteauxSheetnessFeatureGeneratorType::New();

  EXERCISE_BASIC_OBJECT_METHODS( featureGenerator,
    DescoteauxSheetnessFeatureGenerator,
    FeatureGenerator );

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );


  bool detectBrightSheets = true;
  if( argc > 3 )
    {
    detectBrightSheets = std::stoi( argv[3] );
    }
  TEST_SET_GET_BOOLEAN( featureGenerator, DetectBrightSheets, detectBrightSheets );

  double sigma = 1.0;
  if( argc > 4 )
    {
    sigma = std::stod( argv[4] );
    }
  featureGenerator->SetSigma( sigma );
  TEST_SET_GET_VALUE( sigma, featureGenerator->GetSigma() );

  double sheetnessNormalization = 0.5;
  if( argc > 5 )
    {
    sheetnessNormalization = std::stod( argv[5] );
    }
  featureGenerator->SetSheetnessNormalization( sheetnessNormalization );
  TEST_SET_GET_VALUE( sheetnessNormalization, featureGenerator->GetSheetnessNormalization() );

  double bloobinessNormalization = 2.0;
  if( argc > 6 )
    {
    bloobinessNormalization = std::stod( argv[6] );
    }
  featureGenerator->SetBloobinessNormalization( bloobinessNormalization );
  TEST_SET_GET_VALUE( bloobinessNormalization, featureGenerator->GetBloobinessNormalization() );

  double noiseNormalization = 1.0;
  if( argc > 7 )
    {
    noiseNormalization = std::stod( argv[7] );
    }
  featureGenerator->SetNoiseNormalization( noiseNormalization );
  TEST_SET_GET_VALUE( noiseNormalization, featureGenerator->GetNoiseNormalization() );

  TRY_EXPECT_NO_EXCEPTION( featureGenerator->Update() );


  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast< const OutputImageSpatialObjectType * >( feature.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( outputImage );

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
