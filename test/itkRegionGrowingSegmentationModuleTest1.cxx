/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkRegionGrowingSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkRegionGrowingSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"

int
itkRegionGrowingSegmentationModuleTest1(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::RegionGrowingSegmentationModule<Dimension>;
  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();

  using ImageSpatialObjectType = itk::ImageSpatialObject<Dimension>;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  segmentationModule->SetInput(inputObject);

  ImageSpatialObjectType::Pointer featureObject = ImageSpatialObjectType::New();

  segmentationModule->SetFeature(featureObject);


  segmentationModule->Update();

  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  segmentationModule->Print(std::cout);

  std::cout << "Class name = " << segmentationModule->GetNameOfClass() << std::endl;

  return EXIT_SUCCESS;
}
