/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkMinimumFeatureAggregatorTest2.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkMinimumFeatureAggregator.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"

namespace itk
{

class MinimumFeatureAggregatorDerived : public MinimumFeatureAggregator<3>
{
public:
  /** Standard class typedefs. */
  typedef MinimumFeatureAggregatorDerived     Self;
  typedef MinimumFeatureAggregator<3>         Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MinimumFeatureAggregatorDerived, MinimumFeatureAggregator);

  typedef  Superclass::InputFeatureType    InputFeatureType;

  //
  // Need to make it public, to make testing possible.
  //
  const InputFeatureType * GetInputFeature( unsigned int id ) const
    {
    return this->Superclass::GetInputFeature( id );
    }
};

}

int itkMinimumFeatureAggregatorTest2( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage ";
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


  typedef itk::MinimumFeatureAggregatorDerived  AggregatorType;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();
  
  typedef itk::SatoVesselnessSigmoidFeatureGenerator< Dimension > VesselnessGeneratorType;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  typedef itk::LungWallFeatureGenerator< Dimension > LungWallGeneratorType;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  typedef itk::SigmoidFeatureGenerator< Dimension >   SigmoidFeatureGeneratorType;
  SigmoidFeatureGeneratorType::Pointer  sigmoidGenerator = SigmoidFeatureGeneratorType::New();
 
  featureAggregator->AddFeatureGenerator( lungWallGenerator );
  featureAggregator->AddFeatureGenerator( vesselnessGenerator );
  featureAggregator->AddFeatureGenerator( sigmoidGenerator );


  typedef AggregatorType::SpatialObjectType    SpatialObjectType;

  typedef itk::ImageSpatialObject< Dimension, InputPixelType  > InputImageSpatialObjectType;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  lungWallGenerator->SetInput( inputObject );
  vesselnessGenerator->SetInput( inputObject );
  sigmoidGenerator->SetInput( inputObject );

  lungWallGenerator->SetLungThreshold( -400 );

  vesselnessGenerator->SetSigma( 1.0 );
  vesselnessGenerator->SetAlpha1( 0.5 );
  vesselnessGenerator->SetAlpha2( 2.0 );
 
  sigmoidGenerator->SetAlpha(  1.0  );
  sigmoidGenerator->SetBeta( -200.0 );
 

  featureAggregator->Update();

  SpatialObjectType::ConstPointer finalFeature = featureAggregator->GetFeature();

  typedef AggregatorType::OutputImageSpatialObjectType       OutputImageSpatialObjectType;
  OutputImageSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputImageSpatialObjectType * >( finalFeature.GetPointer() );

  typedef AggregatorType::OutputImageType       OutputImageType;
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

  //
  // Exercise GetInputFeature()
  //
  if( featureAggregator->GetInputFeature( 0 ) != lungWallGenerator->GetFeature() )
    {
    std::cerr << "Failure to recover feature 0 with GetInputFeature()" << std::endl;
    return EXIT_FAILURE;
    }

  if( featureAggregator->GetInputFeature( 1 ) != vesselnessGenerator->GetFeature() )
    {
    std::cerr << "Failure to recover feature 1 with GetInputFeature()" << std::endl;
    return EXIT_FAILURE;
    }

  if( featureAggregator->GetInputFeature( 2 ) != sigmoidGenerator->GetFeature() )
    {
    std::cerr << "Failure to recover feature 2 with GetInputFeature()" << std::endl;
    return EXIT_FAILURE;
    }

  try 
    {
    featureAggregator->GetInputFeature( 3 );
    std::cerr << "Failure to catch an exception for GetInputFeature() with out of range argument" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cout << "Caught expected exception" << std::endl;
    std::cout << excp << std::endl;
    }

  return EXIT_SUCCESS;
}
