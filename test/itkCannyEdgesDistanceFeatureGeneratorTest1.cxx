/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgesDistanceFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgesDistanceFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int itkCannyEdgesDistanceFeatureGeneratorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [sigma upperthreshold lowerthreshold]" << std::endl;
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

  using CannyEdgesDistanceFeatureGeneratorType = itk::CannyEdgesDistanceFeatureGenerator< Dimension >;
  using SpatialObjectType = CannyEdgesDistanceFeatureGeneratorType::SpatialObjectType;

  CannyEdgesDistanceFeatureGeneratorType::Pointer  featureGenerator = CannyEdgesDistanceFeatureGeneratorType::New();
  

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  double sigma = 1.0;
  double lowerthreshold = 100;
  double upperthreshold = 200;

  featureGenerator->SetSigma( 1.5 );
  featureGenerator->SetUpperThreshold( 1000 );
  featureGenerator->SetLowerThreshold(   50 );

  double tolerance = 1e-4;

  featureGenerator->SetSigma( sigma );

  if( vnl_math_abs( featureGenerator->GetSigma() - sigma ) > tolerance )
    {
    std::cerr << "Error in Set/GetSigma()" << std::endl;
    std::cerr << "It should be " << sigma << std::endl;
    std::cerr << "but got " << featureGenerator->GetSigma() << std::endl;
    return EXIT_FAILURE;
    }

  featureGenerator->SetLowerThreshold( lowerthreshold );
  if( vnl_math_abs( featureGenerator->GetLowerThreshold() - lowerthreshold ) > tolerance )
    {
    std::cerr << "Error in Set/GetLowerThreshold()" << std::endl;
    return EXIT_FAILURE;
    }

  featureGenerator->SetUpperThreshold( upperthreshold );
  if( vnl_math_abs( featureGenerator->GetUpperThreshold() - upperthreshold ) > tolerance )
    {
    std::cerr << "Error in Set/GetUpperThreshold()" << std::endl;
    return EXIT_FAILURE;
    }

  if( argc > 3 )
    {
    featureGenerator->SetSigma( atof( argv[3] ) );
    }

  if( argc > 4 )
    {
    featureGenerator->SetUpperThreshold( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator->SetLowerThreshold( atof( argv[5] ) );
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
