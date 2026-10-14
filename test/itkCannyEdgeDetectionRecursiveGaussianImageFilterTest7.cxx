/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest7.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkImage.h"
#include "itkTestingMacros.h"

namespace
{
using ImageType = itk::Image<float, 2>;

ImageType::Pointer
CreateImage(itk::SizeValueType width, itk::SizeValueType height)
{
  ImageType::SizeType size{ { width, height } };
  auto                image = ImageType::New();
  image->SetRegions(ImageType::RegionType(size));
  image->Allocate(true);
  return image;
}
} // namespace

// One filter instance must handle successive inputs of different sizes.
int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest7(int, char *[])
{
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<ImageType, ImageType>;
  auto filter = FilterType::New();

  for (const auto & size : { ImageType::SizeType{ { 52, 44 } },
                             ImageType::SizeType{ { 70, 60 } },
                             ImageType::SizeType{ { 30, 36 } },
                             ImageType::SizeType{ { 52, 44 } } })
  {
    auto image = CreateImage(size[0], size[1]);
    filter->SetInput(image);
    ITK_TRY_EXPECT_NO_EXCEPTION(filter->UpdateLargestPossibleRegion());
    ITK_TEST_SET_GET_VALUE(image->GetLargestPossibleRegion(), filter->GetOutput()->GetLargestPossibleRegion());
  }

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
