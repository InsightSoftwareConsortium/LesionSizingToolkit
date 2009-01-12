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

  if( argc < 5 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage (bright=1/dark=0) sigma numberOfScales [sheetness bloobiness noise ]" << std::endl;
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


  typedef itk::ImageSpatialObject< Dimension, InputPixelType  > InputImageSpatialObjectType;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );


  typedef itk::MaximumFeatureAggregator< Dimension >   AggregatorType;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();

  typedef itk::DescoteauxSheetnessFeatureGenerator< Dimension >         DescoteauxSheetnessFeatureGeneratorType;
  typedef DescoteauxSheetnessFeatureGeneratorType::SpatialObjectType    SpatialObjectType;
  typedef DescoteauxSheetnessFeatureGeneratorType::Pointer              FeatureGeneratorPointer;
  typedef std::vector< FeatureGeneratorPointer >                        FeatureGeneratorArray;

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
