/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkBinaryThresholdFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkBinaryThresholdFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int itkBinaryThresholdFeatureGeneratorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [threshold]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef signed short    InputPixelType;
  typedef float           OutputPixelType;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType >     ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >    WriterType;

  typedef itk::ImageSpatialObject< Dimension, InputPixelType  > InputImageSpatialObjectType;
  typedef itk::ImageSpatialObject< Dimension, OutputPixelType > OutputImageSpatialObjectType;

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

  typedef itk::BinaryThresholdFeatureGenerator< Dimension >   BinaryThresholdFeatureGeneratorType;
  typedef BinaryThresholdFeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  BinaryThresholdFeatureGeneratorType::Pointer  featureGenerator = BinaryThresholdFeatureGeneratorType::New();
  
  std::cout << featureGenerator->GetNameOfClass() << std::endl;
  featureGenerator->Print( std::cout );

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  double threshold  = 100.0;

  if( argc > 3 )
    {
    threshold = atof( argv[3] );
    }

  featureGenerator->SetThreshold( 0.0 );

  featureGenerator->SetThreshold( threshold );
  if( featureGenerator->GetThreshold() != threshold )
    {
    std::cerr << "Error in Set/GetThreshold()" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << featureGenerator->GetNameOfClass() << std::endl;
  featureGenerator->Print( std::cout );

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


  std::cout << "Name Of Class = " << featureGenerator->GetNameOfClass() << std::endl;

  featureGenerator->Print( std::cout );

 
  return EXIT_SUCCESS;
}
