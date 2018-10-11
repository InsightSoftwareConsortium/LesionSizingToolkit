/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest3.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLesionSegmentationMethod.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLandmarksReader.h"
#include "itkImageMaskSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkConnectedThresholdSegmentationModule.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkTestingMacros.h"


int itkLesionSegmentationMethodTest3( int argc, char * argv [] )
{
  if( argc < 3 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile inputImage outputImage ";
    std::cerr << " [lowerThreshold upperThreshold] " << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image< InputPixelType, Dimension >;

  using InputImageReaderType = itk::ImageFileReader< InputImageType >;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[2] );

  TRY_EXPECT_NO_EXCEPTION( inputImageReader->Update() );

  using LandmarksReaderType = itk::LandmarksReader< Dimension >;

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName( argv[1] );

  TRY_EXPECT_NO_EXCEPTION( landmarksReader->Update() );


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

  using FeatureAggregatorType = itk::MinimumFeatureAggregator< Dimension >;
  FeatureAggregatorType::Pointer featureAggregator = FeatureAggregatorType::New();

  featureAggregator->AddFeatureGenerator( lungWallGenerator );
  featureAggregator->AddFeatureGenerator( vesselnessGenerator );
  featureAggregator->AddFeatureGenerator( sigmoidGenerator );

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

  lungWallGenerator->SetLungThreshold( -400 );

  vesselnessGenerator->SetSigma( 1.0 );
  vesselnessGenerator->SetAlpha1( 0.5 );
  vesselnessGenerator->SetAlpha2( 2.0 );

  sigmoidGenerator->SetAlpha( 100.0  );
  sigmoidGenerator->SetBeta( -200.0 );

  using SegmentationModuleType = itk::ConnectedThresholdSegmentationModule< Dimension >;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  EXERCISE_BASIC_OBJECT_METHODS(segmentationModule,
    ConnectedThresholdSegmentationModule,
    RegionGrowingSegmentationModule );

  lesionSegmentationMethod->SetSegmentationModule( segmentationModule );


  double lowerThreshold = 0.5;
  if( argc > 4 )
    {
    lowerThreshold = std::stod( argv[4] );
    }
  segmentationModule->SetLowerThreshold( lowerThreshold );
  TEST_SET_GET_VALUE( lowerThreshold, segmentationModule->GetLowerThreshold() );

  double upperThreshold = 1.0;
  if( argc > 5 )
    {
    upperThreshold = std::stod( argv[5] );
    }
  segmentationModule->SetUpperThreshold( upperThreshold );
  TEST_SET_GET_VALUE( upperThreshold, segmentationModule->GettUpperThreshold() );


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

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
