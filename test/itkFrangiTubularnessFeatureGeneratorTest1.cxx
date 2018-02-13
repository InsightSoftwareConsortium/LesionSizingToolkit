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

int itkFrangiTubularnessFeatureGeneratorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [sigma sheetness bloobiness noise ]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

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

  using FrangiTubularnessFeatureGeneratorType = itk::FrangiTubularnessFeatureGenerator< Dimension >;
  using SpatialObjectType = FrangiTubularnessFeatureGeneratorType::SpatialObjectType;

  FrangiTubularnessFeatureGeneratorType::Pointer  featureGenerator = FrangiTubularnessFeatureGeneratorType::New();
  

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  if( argc > 3 )
    {
    featureGenerator->SetSigma( atof( argv[3] ) );
    }

  if( argc > 4 )
    {
    featureGenerator->SetSheetnessNormalization( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator->SetBloobinessNormalization( atof( argv[5] ) );
    }

  if( argc > 6 )
    {
    featureGenerator->SetNoiseNormalization( atof( argv[6] ) );
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
