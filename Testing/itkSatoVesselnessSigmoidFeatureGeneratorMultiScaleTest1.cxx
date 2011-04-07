/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkMinimumFeatureAggregator.h"


int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage ";
    std::cerr << " [sigma alpha1 alpha2 " << std::endl;
    std::cerr << " sigmoidAlpha sigmoidBeta]" << std::endl;
    return EXIT_FAILURE;
    }


  const unsigned int     Dimension = 3;
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


  typedef itk::MinimumFeatureAggregator< Dimension >   AggregatorType;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();

  typedef itk::SatoVesselnessSigmoidFeatureGenerator< Dimension >   FeatureGeneratorType;
  typedef FeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  FeatureGeneratorType::Pointer  featureGenerator1 = FeatureGeneratorType::New();
  FeatureGeneratorType::Pointer  featureGenerator2 = FeatureGeneratorType::New();
  FeatureGeneratorType::Pointer  featureGenerator3 = FeatureGeneratorType::New();
  FeatureGeneratorType::Pointer  featureGenerator4 = FeatureGeneratorType::New();
  
  if( argc > 3 )
    {
    double smallestSigma = atof( argv[3] );
    featureGenerator1->SetSigma( smallestSigma );
    featureGenerator2->SetSigma( smallestSigma * 2.0 );
    featureGenerator3->SetSigma( smallestSigma * 4.0 );
    featureGenerator4->SetSigma( smallestSigma * 8.0 );
    }

  if( argc > 4 )
    {
    featureGenerator1->SetAlpha1( atof( argv[4] ) );
    featureGenerator2->SetAlpha1( atof( argv[4] ) );
    featureGenerator3->SetAlpha1( atof( argv[4] ) );
    featureGenerator4->SetAlpha1( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator1->SetAlpha2( atof( argv[5] ) );
    featureGenerator2->SetAlpha2( atof( argv[5] ) );
    featureGenerator3->SetAlpha2( atof( argv[5] ) );
    featureGenerator4->SetAlpha2( atof( argv[5] ) );
    }

  if( argc > 6 )
    {
    featureGenerator1->SetSigmoidAlpha( atof( argv[6] ) );
    featureGenerator2->SetSigmoidAlpha( atof( argv[6] ) );
    featureGenerator3->SetSigmoidAlpha( atof( argv[6] ) );
    featureGenerator4->SetSigmoidAlpha( atof( argv[6] ) );
    }

  if( argc > 7 )
    {
    featureGenerator1->SetSigmoidBeta( atof( argv[7] ) );
    featureGenerator2->SetSigmoidBeta( atof( argv[7] ) );
    featureGenerator3->SetSigmoidBeta( atof( argv[7] ) );
    featureGenerator4->SetSigmoidBeta( atof( argv[7] ) );
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
