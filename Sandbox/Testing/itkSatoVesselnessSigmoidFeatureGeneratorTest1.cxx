/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSatoVesselnessSigmoidFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [sigma alpha1 alpha2 ";
    std::cerr << " sigmoidAlpha sigmoidBeta]" << std::endl;
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

  typedef itk::SatoVesselnessSigmoidFeatureGenerator< Dimension >   SatoVesselnessSigmoidFeatureGeneratorType;
  typedef SatoVesselnessSigmoidFeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  SatoVesselnessSigmoidFeatureGeneratorType::Pointer  featureGenerator = SatoVesselnessSigmoidFeatureGeneratorType::New();
  

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
    featureGenerator->SetAlpha1( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    featureGenerator->SetAlpha2( atof( argv[5] ) );
    }

  if( argc > 6 )
    {
    featureGenerator->SetSigmoidAlpha( atof( argv[6] ) );
    }

  if( argc > 7 )
    {
    featureGenerator->SetSigmoidBeta( atof( argv[7] ) );
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
