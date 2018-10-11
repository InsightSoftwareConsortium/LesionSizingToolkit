/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest6.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// The test runs a shape detection level set from user supplied seed points
// and then runs the shape detection level set with the results from the
// fast marching to get the final segmentation.

#include "itkLesionSegmentationMethod.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLandmarksReader.h"
#include "itkImageMaskSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkGradientMagnitudeSigmoidFeatureGenerator.h"
#include "itkFastMarchingAndShapeDetectionLevelSetSegmentationModule.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkTestingMacros.h"


// Applies fast marhching followed by shape detection.
int itkLesionSegmentationMethodTest6( int argc, char * argv [] )
{
  if( argc < 3 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile inputImage toutputImage";
    std::cerr << " [RMSErrorForShapeDetection]"
              << " [IterationsForShapeDetection]"
              << " [CurvatureScalingForShapeDetection]"
              << " [PropagationScalingForShapeDetection]";
    std::cerr << " stopping time for fast marching";
    std::cerr << " distance from seeds for fast marching" << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image< InputPixelType, Dimension >;

  using InputImageReaderType = itk::ImageFileReader< InputImageType >;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[2] );

  TRY_EXPECT_NO_EXCEPTION( inputImageReader->Update() );


  using MethodType = itk::LesionSegmentationMethod< Dimension >;

  MethodType::Pointer  lesionSegmentationMethod = MethodType::New();

  using ImageMaskSpatialObjectType = itk::ImageMaskSpatialObject< Dimension >;

  ImageMaskSpatialObjectType::Pointer regionOfInterest = ImageMaskSpatialObjectType::New();

  lesionSegmentationMethod->SetRegionOfInterest( regionOfInterest );

  using VesselnessGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator< Dimension >;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  using LungWallGeneratorType = itk::LungWallFeatureGenerator< Dimension >;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  using SigmoidFeatureGeneratorType = itk::SigmoidFeatureGenerator< Dimension >;
  SigmoidFeatureGeneratorType::Pointer  sigmoidGenerator = SigmoidFeatureGeneratorType::New();

  using GradientMagnitudeSigmoidGeneratorType = itk::GradientMagnitudeSigmoidFeatureGenerator< Dimension >;
  GradientMagnitudeSigmoidGeneratorType::Pointer gradientMagnitudeSigmoidGenerator =
    GradientMagnitudeSigmoidGeneratorType::New();

  using FeatureAggregatorType = itk::MinimumFeatureAggregator< Dimension >;
  FeatureAggregatorType::Pointer featureAggregator = FeatureAggregatorType::New();

  featureAggregator->AddFeatureGenerator( lungWallGenerator );
  featureAggregator->AddFeatureGenerator( vesselnessGenerator );
  featureAggregator->AddFeatureGenerator( sigmoidGenerator );
  featureAggregator->AddFeatureGenerator( gradientMagnitudeSigmoidGenerator );

  lesionSegmentationMethod->AddFeatureGenerator( featureAggregator );

  using SpatialObjectType = MethodType::SpatialObjectType;

  using InputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, InputPixelType  >;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  lungWallGenerator->SetInput( inputObject );
  vesselnessGenerator->SetInput( inputObject );
  sigmoidGenerator->SetInput( inputObject );
  gradientMagnitudeSigmoidGenerator->SetInput( inputObject );

  lungWallGenerator->SetLungThreshold( -400 );

  vesselnessGenerator->SetSigma( 1.0 );
  vesselnessGenerator->SetAlpha1( 0.5 );
  vesselnessGenerator->SetAlpha2( 2.0 );

  sigmoidGenerator->SetAlpha(  1.0  );
  sigmoidGenerator->SetBeta( -200.0 );

  gradientMagnitudeSigmoidGenerator->SetSigma( 1.0 );
  gradientMagnitudeSigmoidGenerator->SetAlpha( -0.1 );
  gradientMagnitudeSigmoidGenerator->SetBeta( 150.0 );

  using SegmentationModuleType = itk::FastMarchingAndShapeDetectionLevelSetSegmentationModule< Dimension >;
  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  EXERCISE_BASIC_OBJECT_METHODS(segmentationModule,
    FastMarchingAndShapeDetectionLevelSetSegmentationModule,
    SinglePhaseLevelSetSegmentationModule );


  double maximumRMSError = 0.0002;
  if( argc > 4 )
    {
    maximumRMSError = std::stod( argv[4] );
    }
  segmentationModule->SetMaximumRMSError( maximumRMSError );
  TEST_SET_GET_VALUE( maximumRMSError, segmentationModule->GetMaximumRMSError() );

  unsigned int maximumNumberOfIterations = 300;
  if( argc > 5 )
    {
    maximumNumberOfIterations = std::stoi( argv[5] );
    }
  segmentationModule->SetMaximumNumberOfIterations( maximumNumberOfIterations );
  TEST_SET_GET_VALUE( maximumNumberOfIterations, segmentationModule->GetMaximumNumberOfIterations() );

  double curvatureScaling = 1.0;
  if( argc > 6 )
    {
    curvatureScaling = std::stod( argv[6] );
    }
  segmentationModule->SetCurvatureScaling( curvatureScaling );
  TEST_SET_GET_VALUE( curvatureScaling, segmentationModule->GetCurvatureScaling() );

  double propagationScaling = 10.0;
  if( argc > 7 )
    {
    propagationScaling = std::stod( argv[7] );
    }
  segmentationModule->SetPropagationScaling( propagationScaling );
  TEST_SET_GET_VALUE( propagationScaling, segmentationModule->GetPropagationScaling() );

  double stoppingValue = 500.0;
  if( argc > 8 )
    {
    stoppingValue = std::stod( argv[8] );
    }
  segmentationModule->SetStoppingValue( stoppingValue );
  TEST_SET_GET_VALUE( stoppingValue, segmentationModule->GetStoppingValue() );

  double distanceFromSeeds = 0.5;
  if( argc > 9 )
    {
    distanceFromSeeds = std::stod( argv[9] );
    }
  segmentationModule->SetDistanceFromSeeds( distanceFromSeeds );
  TEST_SET_GET_VALUE( distanceFromSeeds, segmentationModule->GetDistanceFromSeeds() );



  lesionSegmentationMethod->SetSegmentationModule( segmentationModule );

  using LandmarksReaderType = itk::LandmarksReader< Dimension >;

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName( argv[1] );
  TRY_EXPECT_NO_EXCEPTION( landmarksReader->Update() );


  lesionSegmentationMethod->SetInitialSegmentation( landmarksReader->GetOutput() );

  TRY_EXPECT_NO_EXCEPTION( lesionSegmentationMethod->Update() );


  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject =
    dynamic_cast< const OutputSpatialObjectType * >( segmentation.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter< OutputImageType >;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();


  segmentationModule->Print( std::cout );

  std::cout << "Name of class " << segmentationModule->GetNameOfClass() << std::endl;

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
