/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGradientMagnitudeSigmoidFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkGradientMagnitudeSigmoidFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"
#include "itkImageFileReader.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage " << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef signed short    InputPixelType;
  typedef itk::Image< InputPixelType, Dimension >    InputImageType;

  typedef itk::ImageFileReader< InputImageType >     ReaderType;

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

  typedef itk::GradientMagnitudeSigmoidFeatureGenerator< Dimension >   GradientMagnitudeSigmoidFeatureGeneratorType;
  typedef GradientMagnitudeSigmoidFeatureGeneratorType::SpatialObjectType    SpatialObjectType;

  GradientMagnitudeSigmoidFeatureGeneratorType::Pointer  featureGenerator = GradientMagnitudeSigmoidFeatureGeneratorType::New();
  
  typedef itk::ImageSpatialObject< Dimension, signed short > ImageSpatialObjectType;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = reader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  featureGenerator->SetInput( inputObject );

  featureGenerator->Update();

  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  featureGenerator->Print( std::cout );

  
  return EXIT_SUCCESS;
}
