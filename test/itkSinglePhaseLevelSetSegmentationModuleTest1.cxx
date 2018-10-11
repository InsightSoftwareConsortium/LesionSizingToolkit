/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSinglePhaseLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSinglePhaseLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"
#include "itkTestingMacros.h"


int itkSinglePhaseLevelSetSegmentationModuleTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::SinglePhaseLevelSetSegmentationModule< Dimension >;
  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  EXERCISE_BASIC_OBJECT_METHODS( segmentationModule,
    SinglePhaseLevelSetSegmentationModule,
    SegmentationModule );


  using ImageSpatialObjectType = itk::ImageSpatialObject< Dimension >;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  segmentationModule->SetInput( inputObject );

  ImageSpatialObjectType::Pointer featureObject = ImageSpatialObjectType::New();

  segmentationModule->SetFeature( featureObject );

  constexpr double propagationScaling = 1.3;
  segmentationModule->SetPropagationScaling( propagationScaling  );
  TEST_SET_GET_VALUE( propagationScaling, segmentationModule->GetPropagationScaling() );

  constexpr double curvatureScaling = 1.7;
  segmentationModule->SetCurvatureScaling( curvatureScaling );
  TEST_SET_GET_VALUE( curvatureScaling, segmentationModule->GetCurvatureScaling() );

  constexpr double advectionScaling = 1.9;
  segmentationModule->SetAdvectionScaling( advectionScaling );
  TEST_SET_GET_VALUE( advectionScaling, segmentationModule->GetAdvectionScaling() );

  constexpr double maximumRMSError = 0.01;
  segmentationModule->SetMaximumRMSError( maximumRMSError );
  TEST_SET_GET_VALUE( maximumRMSError, segmentationModule->GetMaximumRMSError() );

  constexpr unsigned int maximumNumberOfIterations = 179;
  segmentationModule->SetMaximumNumberOfIterations( maximumNumberOfIterations );
  TEST_SET_GET_VALUE( maximumNumberOfIterations, segmentationModule->GetMaximumNumberOfIterations() );


  TRY_EXPECT_NO_EXCEPTION( segmentationModule->Update() );


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
