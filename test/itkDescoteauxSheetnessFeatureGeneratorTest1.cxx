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

int itkDescoteauxSheetnessFeatureGeneratorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [(bright1/dark:0) sigma sheetness bloobiness noise ]" << std::endl;
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

  try
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  using DescoteauxSheetnessFeatureGeneratorType = itk::DescoteauxSheetnessFeatureGenerator< Dimension >;
  using SpatialObjectType = DescoteauxSheetnessFeatureGeneratorType::SpatialObjectType;

  DescoteauxSheetnessFeatureGeneratorType::Pointer  featureGenerator = DescoteauxSheetnessFeatureGeneratorType::New();


  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  //
  // Exercise brightness methods
  //
  featureGenerator->DetectBrightSheetsOff();
  featureGenerator->SetDetectBrightSheets( true );
  if( !featureGenerator->GetDetectBrightSheets() )
    {
    std::cerr << "Error in Set/GetDetectBrightSheets()" << std::endl;
    return EXIT_FAILURE;
    }

  featureGenerator->SetDetectBrightSheets( false );
  if( featureGenerator->GetDetectBrightSheets() )
    {
    std::cerr << "Error in Set/GetDetectBrightSheets()" << std::endl;
    return EXIT_FAILURE;
    }

  featureGenerator->DetectBrightSheetsOn();
  if( !featureGenerator->GetDetectBrightSheets() )
    {
    std::cerr << "Error in DetectBrightSheetsOn()" << std::endl;
    return EXIT_FAILURE;
    }

  featureGenerator->DetectBrightSheetsOff();
  if( featureGenerator->GetDetectBrightSheets() )
    {
    std::cerr << "Error in DetectBrightSheetsOff()" << std::endl;
    return EXIT_FAILURE;
    }

  // Finally leave it On
  featureGenerator->DetectBrightSheetsOn();


  if( argc > 3 )
    {
    featureGenerator->SetDetectBrightSheets( atoi( argv[3] ) );
    }

  if( argc > 4 )
    {
    featureGenerator->SetSigma( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator->SetSheetnessNormalization( atof( argv[5] ) );
    }

  if( argc > 6 )
    {
    featureGenerator->SetBloobinessNormalization( atof( argv[6] ) );
    }

  if( argc > 7 )
    {
    featureGenerator->SetNoiseNormalization( atof( argv[7] ) );
    }

  try
    {
    featureGenerator->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  OutputImageSpatialObjectType::ConstPointer outputObject =
    dynamic_cast< const OutputImageSpatialObjectType * >( feature.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( outputImage );


  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Name Of Class = " << featureGenerator->GetNameOfClass() << std::endl;

  featureGenerator->Print( std::cout );


  return EXIT_SUCCESS;
}
