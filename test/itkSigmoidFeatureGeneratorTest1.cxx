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

int itkSigmoidFeatureGeneratorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [alpha beta]" << std::endl;
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

  typedef itk::SigmoidFeatureGenerator< Dimension >   SigmoidFeatureGeneratorType;
  typedef SigmoidFeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  SigmoidFeatureGeneratorType::Pointer  featureGenerator = SigmoidFeatureGeneratorType::New();
  

  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  double alpha =   1.0;
  double beta  = 100.0;

  if( argc > 3 )
    {
    alpha = atof( argv[3] );
    }

  if( argc > 4 )
    {
    beta = atof( argv[4] );
    }

  featureGenerator->SetAlpha( 0.0 );
  featureGenerator->SetBeta( 0.0 );

  featureGenerator->SetAlpha( alpha );
  if( featureGenerator->GetAlpha() != alpha )
    {
    std::cerr << "Error in Set/GetAlpha()" << std::endl;
    return EXIT_FAILURE;
    }

  featureGenerator->SetBeta( beta );
  if( featureGenerator->GetBeta() != beta )
    {
    std::cerr << "Error in Set/GetBeta()" << std::endl;
    return EXIT_FAILURE;
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
