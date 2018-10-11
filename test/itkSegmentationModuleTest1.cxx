/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"
#include "itkTestingMacros.h"


int itkSegmentationModuleTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::SegmentationModule< Dimension >;
  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  EXERCISE_BASIC_OBJECT_METHODS( segmentationModule, SegmentationModule,
    ProcessObject );


  using ImageSpatialObjectType = itk::ImageSpatialObject< Dimension >;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  segmentationModule->SetInput( inputObject );

  ImageSpatialObjectType::Pointer featureObject = ImageSpatialObjectType::New();

  segmentationModule->SetFeature( featureObject );

  TRY_EXPECT_NO_EXCEPTION( segmentationModule->Update() );


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
