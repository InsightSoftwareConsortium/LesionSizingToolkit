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


int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [(bright1/dark:0) sigma sheetness bloobiness noise ]" << std::endl;
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;
  typedef signed short   InputPixelType;

  typedef itk::Image< InputPixelType, Dimension > InputImageType;

  typedef itk::ImageFileReader< InputImageType > InputImageReaderType;
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


  typedef itk::MaximumFeatureAggregator< Dimension >   AggregatorType;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();

  typedef itk::DescoteauxSheetnessFeatureGenerator< Dimension >   DescoteauxSheetnessFeatureGeneratorType;
  typedef DescoteauxSheetnessFeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  DescoteauxSheetnessFeatureGeneratorType::Pointer  featureGenerator1 = DescoteauxSheetnessFeatureGeneratorType::New();
  DescoteauxSheetnessFeatureGeneratorType::Pointer  featureGenerator2 = DescoteauxSheetnessFeatureGeneratorType::New();
  DescoteauxSheetnessFeatureGeneratorType::Pointer  featureGenerator3 = DescoteauxSheetnessFeatureGeneratorType::New();
  DescoteauxSheetnessFeatureGeneratorType::Pointer  featureGenerator4 = DescoteauxSheetnessFeatureGeneratorType::New();
  
  if( argc > 3 )
    {
    featureGenerator1->SetDetectBrightSheets( atoi( argv[3] ) );
    featureGenerator2->SetDetectBrightSheets( atoi( argv[3] ) );
    featureGenerator3->SetDetectBrightSheets( atoi( argv[3] ) );
    featureGenerator4->SetDetectBrightSheets( atoi( argv[3] ) );
    }

  if( argc > 4 )
    {
    double smallestSigma = atof( argv[4] );
    featureGenerator1->SetSigma( smallestSigma );
    featureGenerator2->SetSigma( smallestSigma * 2.0 );
    featureGenerator3->SetSigma( smallestSigma * 4.0 );
    featureGenerator4->SetSigma( smallestSigma * 8.0 );
    }

  if( argc > 5 )
    {
    featureGenerator1->SetSheetnessNormalization( atof( argv[5] ) );
    featureGenerator2->SetSheetnessNormalization( atof( argv[5] ) );
    featureGenerator3->SetSheetnessNormalization( atof( argv[5] ) );
    featureGenerator4->SetSheetnessNormalization( atof( argv[5] ) );
    }

  if( argc > 6 )
    {
    featureGenerator1->SetBloobinessNormalization( atof( argv[6] ) );
    featureGenerator2->SetBloobinessNormalization( atof( argv[6] ) );
    featureGenerator3->SetBloobinessNormalization( atof( argv[6] ) );
    featureGenerator4->SetBloobinessNormalization( atof( argv[6] ) );
    }

  if( argc > 7 )
    {
    featureGenerator1->SetNoiseNormalization( atof( argv[7] ) );
    featureGenerator2->SetNoiseNormalization( atof( argv[7] ) );
    featureGenerator3->SetNoiseNormalization( atof( argv[7] ) );
    featureGenerator4->SetNoiseNormalization( atof( argv[7] ) );
    }

  typedef AggregatorType::SpatialObjectType    SpatialObjectType;

  typedef itk::ImageSpatialObject< Dimension, InputPixelType  > InputImageSpatialObjectType;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator1->SetInput( inputObject );
  featureGenerator2->SetInput( inputObject );
  featureGenerator3->SetInput( inputObject );
  featureGenerator4->SetInput( inputObject );

  featureAggregator->AddFeatureGenerator( featureGenerator1 );
  featureAggregator->AddFeatureGenerator( featureGenerator2 );
  featureAggregator->AddFeatureGenerator( featureGenerator3 );
  featureAggregator->AddFeatureGenerator( featureGenerator4 );

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

  typedef AggregatorType::OutputImageSpatialObjectType  OutputImageSpatialObjectType;
  typedef AggregatorType::OutputImageType               OutputImageType;

  OutputImageSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputImageSpatialObjectType * >( finalFeature.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  typedef itk::ImageFileWriter< OutputImageType >      OutputWriterType;
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
