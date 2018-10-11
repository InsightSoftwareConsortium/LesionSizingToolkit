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
#include "itkTestingMacros.h"


namespace itk
{

class MinimumFeatureAggregatorSurrogate : public MinimumFeatureAggregator<3>
{
public:
  /** Standard class type alias. */
  using Self = MinimumFeatureAggregatorSurrogate;
  using Superclass = MinimumFeatureAggregator<3>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MinimumFeatureAggregatorSurrogate, MinimumFeatureAggregator);

  using InputFeatureType = Superclass::InputFeatureType;

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
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage ";
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image< InputPixelType, Dimension >;

  using InputImageReaderType = itk::ImageFileReader< InputImageType >;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[1] );

  TRY_EXPECT_NO_EXCEPTION( inputImageReader->Update() );


  using AggregatorType = itk::MinimumFeatureAggregatorSurrogate;

  AggregatorType::Pointer featureAggregator = AggregatorType::New();

  EXERCISE_BASIC_OBJECT_METHODS( featureAggregator,
    MinimumFeatureAggregatorSurrogate,
    MinimumFeatureAggregator );


  using VesselnessGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator< Dimension >;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  using LungWallGeneratorType = itk::LungWallFeatureGenerator< Dimension >;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  using SigmoidFeatureGeneratorType = itk::SigmoidFeatureGenerator< Dimension >;
  SigmoidFeatureGeneratorType::Pointer  sigmoidGenerator = SigmoidFeatureGeneratorType::New();

  featureAggregator->AddFeatureGenerator( lungWallGenerator );
  featureAggregator->AddFeatureGenerator( vesselnessGenerator );
  featureAggregator->AddFeatureGenerator( sigmoidGenerator );


  using SpatialObjectType = AggregatorType::SpatialObjectType;

  using InputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, InputPixelType  >;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  lungWallGenerator->SetInput( inputObject );
  vesselnessGenerator->SetInput( inputObject );
  sigmoidGenerator->SetInput( inputObject );


  LungWallGeneratorType::InputPixelType lungThreshold = -400;
  lungWallGenerator->SetLungThreshold( lungThreshold );
  TEST_SET_GET_VALUE( lungThreshold, lungWallGenerator->GetLungThreshold() );


  double sigma = 1.0;
  vesselnessGenerator->SetSigma( sigma );
  TEST_SET_GET_VALUE( sigma, vesselnessGenerator->GetSigma() );

  double alpha1 = 0.5;
  vesselnessGenerator->SetAlpha1( alpha1 );
  TEST_SET_GET_VALUE( alpha1, vesselnessGenerator->GetAlpha1() );

  double alpha2 = 2.0;
  vesselnessGenerator->SetAlpha2( alpha2 );
  TEST_SET_GET_VALUE( alpha2, vesselnessGenerator->GetAlpha2() );


  double alpha = 1.0;
  sigmoidGenerator->SetAlpha( alpha );
  TEST_SET_GET_VALUE( alpha, sigmoidGenerator->GetAlpha() );

  double beta = -200.0;
  sigmoidGenerator->SetBeta( beta );
  TEST_SET_GET_VALUE( beta, sigmoidGenerator->GetBeta() );


  TRY_EXPECT_NO_EXCEPTION( featureAggregator->Update() );

  SpatialObjectType::ConstPointer finalFeature = featureAggregator->GetFeature();

  using OutputImageSpatialObjectType = AggregatorType::OutputImageSpatialObjectType;
  OutputImageSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputImageSpatialObjectType * >( finalFeature.GetPointer() );

  using OutputImageType = AggregatorType::OutputImageType;
  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter< OutputImageType >;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  //
  // Exercise GetInputFeature()
  //
  if( featureAggregator->GetInputFeature( 0 ) != lungWallGenerator->GetFeature() )
    {
     std::cerr << "Test failed!" << std::endl;
    std::cerr << "Failure to recover feature 0 with GetInputFeature()" << std::endl;
    return EXIT_FAILURE;
    }

  if( featureAggregator->GetInputFeature( 1 ) != vesselnessGenerator->GetFeature() )
    {
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "Failure to recover feature 1 with GetInputFeature()" << std::endl;
    return EXIT_FAILURE;
    }

  if( featureAggregator->GetInputFeature( 2 ) != sigmoidGenerator->GetFeature() )
    {
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "Failure to recover feature 2 with GetInputFeature()" << std::endl;
    return EXIT_FAILURE;
    }

  TRY_EXPECT_EXCEPTION( featureAggregator->GetInputFeature( 3 ) );


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
