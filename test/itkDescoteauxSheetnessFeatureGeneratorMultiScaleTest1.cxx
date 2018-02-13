/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkMaximumFeatureAggregator.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDescoteauxSheetnessFeatureGenerator.h"


int itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1( int argc, char * argv [] )
{

  if( argc < 5 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage (bright=1/dark=0) sigma numberOfScales [sheetness bloobiness noise ]" << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image< InputPixelType, Dimension >;

  using InputImageReaderType = itk::ImageFileReader< InputImageType >;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[1] );

  try 
    {
    inputImageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  using InputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, InputPixelType  >;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );


  using AggregatorType = itk::MaximumFeatureAggregator< Dimension >;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();

  using DescoteauxSheetnessFeatureGeneratorType = itk::DescoteauxSheetnessFeatureGenerator< Dimension >;
  using SpatialObjectType = DescoteauxSheetnessFeatureGeneratorType::SpatialObjectType;
  using FeatureGeneratorPointer = DescoteauxSheetnessFeatureGeneratorType::Pointer;
  using FeatureGeneratorArray = std::vector< FeatureGeneratorPointer >;

  FeatureGeneratorArray  featureGeneratorArray;

  unsigned int octave = 1;
  double smallestSigma = atof( argv[4] );
  
  const unsigned int numberOfScales = atoi( argv[5] );

  for( unsigned int k = 0; k < numberOfScales; k++ )
    {
    DescoteauxSheetnessFeatureGeneratorType::Pointer  featureGenerator = DescoteauxSheetnessFeatureGeneratorType::New();
    featureGeneratorArray.push_back( featureGenerator );

    featureGenerator->SetDetectBrightSheets( atoi( argv[3] ) );
    featureGenerator->SetSigma( smallestSigma * octave );

    if( argc > 6 )
      {
      featureGenerator->SetSheetnessNormalization( atof( argv[6] ) );
      }

    if( argc > 7 )
      {
      featureGenerator->SetBloobinessNormalization( atof( argv[7] ) );
      }

    if( argc > 8 )
      {
      featureGenerator->SetNoiseNormalization( atof( argv[8] ) );
      }

    octave *= 2;

    featureGenerator->SetInput( inputObject );
    featureAggregator->AddFeatureGenerator( featureGenerator );
    }

  try 
    {
    featureAggregator->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }
 
  SpatialObjectType::ConstPointer finalFeature = featureAggregator->GetFeature();

  using OutputImageSpatialObjectType = AggregatorType::OutputImageSpatialObjectType;
  using OutputImageType = AggregatorType::OutputImageType;

  OutputImageSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputImageSpatialObjectType * >( finalFeature.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter< OutputImageType >;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  featureAggregator->Print( std::cout );

  return EXIT_SUCCESS;
}
