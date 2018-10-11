/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkConfidenceConnectedSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkConfidenceConnectedSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLandmarksReader.h"
#include "itkTestingMacros.h"


int itkConfidenceConnectedSegmentationModuleTest1( int argc, char * argv [] )
{
  if( argc < 3 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile featureImage outputImage ";
    std::cerr << " [sigmaMultiplier] " << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::ConfidenceConnectedSegmentationModule< Dimension >;

  using FeatureImageType = SegmentationModuleType::FeatureImageType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  using FeatureReaderType = itk::ImageFileReader< FeatureImageType >;
  using OutputWriterType = itk::ImageFileWriter< OutputImageType >;

  using LandmarksReaderType = itk::LandmarksReader< Dimension >;
  
  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName( argv[1] );
  landmarksReader->Update();

  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();

  featureReader->SetFileName( argv[2] );

  TRY_EXPECT_NO_EXCEPTION( featureReader->Update() );


  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  EXERCISE_BASIC_OBJECT_METHODS( segmentationModule,
    ConfidenceConnectedSegmentationModule,
    RegionGrowingSegmentationModule );

  using InputSpatialObjectType = SegmentationModuleType::InputSpatialObjectType;
  using FeatureSpatialObjectType = SegmentationModuleType::FeatureSpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;

  InputSpatialObjectType::Pointer inputObject = InputSpatialObjectType::New();
  FeatureSpatialObjectType::Pointer featureObject = FeatureSpatialObjectType::New();

  FeatureImageType::Pointer featureImage = featureReader->GetOutput();

  featureImage->DisconnectPipeline();

  featureObject->SetImage( featureImage );

  segmentationModule->SetFeature( featureObject );
  segmentationModule->SetInput( landmarksReader->GetOutput() );


  double sigmaMultiplier = 2.0;
  if( argc > 4 )
    {
    sigmaMultiplier = std::stod( argv[4] );
    }
  segmentationModule->SetSigmaMultiplier( sigmaMultiplier );
  TEST_SET_GET_VALUE( sigmaMultiplier, segmentationModule->GetSigmaMultiplier() );


  TRY_EXPECT_NO_EXCEPTION( segmentationModule->Update() );

  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputSpatialObjectType * >( segmentation.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
