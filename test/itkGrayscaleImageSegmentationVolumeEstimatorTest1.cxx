/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGrayscaleImageSegmentationVolumeEstimatorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkGrayscaleImageSegmentationVolumeEstimator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkGrayscaleImageSegmentationVolumeEstimatorTest1(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  int testStatus = EXIT_SUCCESS;

  constexpr unsigned int Dimension = 3;

  using VolumeEstimatorType = itk::GrayscaleImageSegmentationVolumeEstimator<Dimension>;

  VolumeEstimatorType::Pointer volumeEstimator = VolumeEstimatorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(
    volumeEstimator, GrayscaleImageSegmentationVolumeEstimator, SegmentationVolumeEstimator);

  using ImageSpatialObjectType = itk::ImageSpatialObject<Dimension>;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  volumeEstimator->SetInput(inputObject);

  TRY_EXPECT_EXCEPTION(volumeEstimator->Update());


  using InputImageType = VolumeEstimatorType::InputImageType;

  InputImageType::Pointer image = InputImageType::New();

  using InputImageSpatialObjectType = VolumeEstimatorType::InputImageSpatialObjectType;

  InputImageSpatialObjectType::Pointer inputImageSpatialObject = InputImageSpatialObjectType::New();

  volumeEstimator->SetInput(inputImageSpatialObject);

  inputImageSpatialObject->SetImage(image);

  InputImageType::SpacingType spacing;

  spacing[0] = 0.5;
  spacing[1] = 0.5;
  spacing[2] = 0.8;

  image->SetSpacing(spacing);


  InputImageType::SizeType size;
  size[0] = 101;
  size[1] = 101;
  size[2] = 101;

  InputImageType::RegionType region;
  region.SetSize(size);

  image->SetRegions(region);
  image->Allocate();

  using IteratorType = itk::ImageRegionIterator<InputImageType>;

  IteratorType itr(image, region);

  itr.GoToBegin();

  InputImageType::IndexType index;
  InputImageType::PointType point;

  //
  // Populate the image with a sphere
  //
  InputImageType::PointType center;

  center[0] = 50.0 * spacing[0];
  center[1] = 50.0 * spacing[1];
  center[2] = 50.0 * spacing[2];

  constexpr double radius = 15.0;

  while (!itr.IsAtEnd())
  {
    index = itr.GetIndex();
    image->TransformIndexToPhysicalPoint(index, point);
    const double distance = point.EuclideanDistanceTo(center);

    if (distance > radius)
    {
      itr.Set(-4.0);
    }
    else
    {
      itr.Set(4.0);
    }

    ++itr;
  }

  TRY_EXPECT_NO_EXCEPTION(volumeEstimator->Update());

  VolumeEstimatorType::RealType volume1 = volumeEstimator->GetVolume();

  const VolumeEstimatorType::RealObjectType * volumeObject = volumeEstimator->GetVolumeOutput();

  if (volumeObject->Get() != volume1)
  {
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "Error in GetVolumeOutput() and/or GetVolume() " << std::endl;
    testStatus = EXIT_FAILURE;
  }

  volumeEstimator->Print(std::cout);

  using WriterType = itk::ImageFileWriter<InputImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(image);
  writer->SetFileName("sphereForVolumeTest.mhd");

  TRY_EXPECT_NO_EXCEPTION(writer->Update());

  const double pi = atan(1.0) * 4.0;

  const double expectedVolume = (4.0 / 3.0) * pi * (radius * radius * radius);

  const double difference = volume1 - expectedVolume;

  const double percentage = 100.0 * itk::Math::abs(difference) / expectedVolume;

  const double epsilon = 1e-6;
  const double allowedVolumePercentageError = 1e-1; // 0.1%
  if (!itk::Math::FloatAlmostEqual(allowedVolumePercentageError, percentage, 10, epsilon))
  {
    std::cerr.precision(static_cast<int>(itk::Math::abs(std::log10(epsilon))));
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "Error in volume computation" << std::endl;
    std::cerr << "Expected value " << expectedVolume << std::endl;
    std::cerr << " differs from " << volume1;
    std::cerr << "by " << difference << std::endl;
    std::cerr << " , " << percentage << "%" << std::endl;
    testStatus = EXIT_FAILURE;
  }


  std::cout << "Test finished." << std::endl;
  return testStatus;
}
