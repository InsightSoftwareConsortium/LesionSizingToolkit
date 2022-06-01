/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLesionSegmentationMethod.h"
#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageMaskSpatialObject.h"
#include "itkTestingMacros.h"


int
itkLesionSegmentationMethodTest1(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  constexpr unsigned int Dimension = 3;

  using MethodType = itk::LesionSegmentationMethod<Dimension>;

  MethodType::Pointer segmentationMethod = MethodType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(segmentationMethod, LesionSegmentationMethod, ProcessObject);

  using ImageMaskSpatialObjectType = itk::ImageMaskSpatialObject<Dimension>;

  ImageMaskSpatialObjectType::Pointer regionOfInterest = ImageMaskSpatialObjectType::New();

  segmentationMethod->SetRegionOfInterest(regionOfInterest);

  const MethodType::SpatialObjectType * regionOfInterestReturned = segmentationMethod->GetRegionOfInterest();

  if (regionOfInterestReturned != regionOfInterest.GetPointer())
  {
    std::cerr << "Test failed! " << std::endl;
    std::cerr << "Error in Set/GetRegionOfInterest() " << std::endl;
    return EXIT_FAILURE;
  }

  ImageMaskSpatialObjectType::Pointer initialSegmentation = ImageMaskSpatialObjectType::New();

  segmentationMethod->SetInitialSegmentation(initialSegmentation);

  const MethodType::SpatialObjectType * initialSegmentationReturned = segmentationMethod->GetInitialSegmentation();

  if (initialSegmentationReturned != initialSegmentation.GetPointer())
  {
    std::cerr << "Test failed! " << std::endl;
    std::cerr << "Error in Set/GetInitialSegmentation() " << std::endl;
    return EXIT_FAILURE;
  }

  using FeatureGeneratorType = itk::FeatureGenerator<Dimension>;

  FeatureGeneratorType::Pointer featureGenerator = FeatureGeneratorType::New();

  segmentationMethod->AddFeatureGenerator(featureGenerator);

  ITK_TRY_EXPECT_EXCEPTION(segmentationMethod->Update());


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
