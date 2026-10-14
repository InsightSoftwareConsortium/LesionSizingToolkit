/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest8.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkAddImageFilter.h"
#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkTestingMacros.h"

namespace
{
using ImageType = itk::Image<float, 2>;

ImageType::Pointer
CreateImage()
{
  ImageType::SizeType size{ { 48, 40 } };
  auto                image = ImageType::New();
  image->SetRegions(ImageType::RegionType(size));
  image->Allocate(true);

  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetBufferedRegion());
  for (; !it.IsAtEnd(); ++it)
  {
    const auto index = it.GetIndex();
    const bool inside = index[0] > 12 && index[0] < 36 && index[1] > 10 && index[1] < 30;
    it.Set(inside ? 80.0f : 0.0f);
  }
  return image;
}

double
Sum(const ImageType * image)
{
  double                                   sum = 0.0;
  itk::ImageRegionConstIterator<ImageType> it(image, image->GetBufferedRegion());
  for (; !it.IsAtEnd(); ++it)
  {
    sum += it.Get();
  }
  return sum;
}
} // namespace

// The internal sub-filters must not modify the output's pipeline state, which
// breaks a downstream consumer when the output may be released after use.
int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest8(int, char *[])
{
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<ImageType, ImageType>;
  auto filter = FilterType::New();
  filter->SetInput(CreateImage());
  filter->SetSigma(1.0);
  filter->SetUpperThreshold(20.0f);
  filter->SetLowerThreshold(10.0f);
  filter->ReleaseDataFlagOn();

  auto add = itk::AddImageFilter<ImageType, ImageType, ImageType>::New();
  add->SetInput1(filter->GetOutput());
  add->SetInput2(filter->GetOutput());

  double sums[3] = { 0.0, 0.0, 0.0 };
  for (double & sum : sums)
  {
    add->Modified();
    ITK_TRY_EXPECT_NO_EXCEPTION(add->Update());
    sum = Sum(add->GetOutput());
  }

  ITK_TEST_EXPECT_TRUE(sums[0] > 0.0);
  ITK_TEST_EXPECT_TRUE(sums[0] == sums[1]);
  ITK_TEST_EXPECT_TRUE(sums[1] == sums[2]);

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
