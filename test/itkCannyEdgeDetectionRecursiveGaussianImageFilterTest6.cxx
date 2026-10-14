/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest6.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkImage.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkTestingMacros.h"

#include <algorithm>
#include <vector>

namespace
{
using ImageType = itk::Image<float, 2>;
using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<ImageType, ImageType>;

ImageType::Pointer
CreateImage(itk::SizeValueType width, itk::SizeValueType height)
{
  ImageType::SizeType size{ { width, height } };
  auto                image = ImageType::New();
  image->SetRegions(ImageType::RegionType(size));
  image->Allocate(true);

  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetBufferedRegion());
  for (; !it.IsAtEnd(); ++it)
  {
    const auto index = it.GetIndex();
    const bool inside = index[0] >= 12 && index[0] < 40 && index[1] >= 12 && index[1] < 40;
    it.Set(inside ? 10.0f + 2.0f * index[0] + index[1] : 0.0f);
  }
  return image;
}

FilterType::Pointer
Run(const ImageType * image, float upper, float lower, const FilterType::SigmaArrayType & sigma)
{
  auto filter = FilterType::New();
  filter->SetInput(image);
  filter->SetSigmaArray(sigma);
  filter->SetUpperThreshold(upper);
  filter->SetLowerThreshold(lower);
  filter->Update();
  return filter;
}

itk::SizeValueType
CountEdges(const ImageType * image, float upper, float lower, const FilterType::SigmaArrayType & sigma)
{
  const auto filter = Run(image, upper, lower, sigma);

  itk::SizeValueType                       count = 0;
  itk::ImageRegionConstIterator<ImageType> it(filter->GetOutput(), filter->GetOutput()->GetBufferedRegion());
  for (; !it.IsAtEnd(); ++it)
  {
    count += it.Get() == 1.0f ? 1 : 0;
  }
  return count;
}

// Sorted nonzero responses of the non-maximum suppression stage.
std::vector<float>
Responses(const ImageType * image, const FilterType::SigmaArrayType & sigma)
{
  const auto                               filter = Run(image, 0.0f, 0.0f, sigma);
  const ImageType *                        nms = filter->GetNonMaximumSuppressionImage();
  std::vector<float>                       values;
  itk::ImageRegionConstIterator<ImageType> it(nms, nms->GetBufferedRegion());
  for (; !it.IsAtEnd(); ++it)
  {
    if (it.Get() > 0.0f)
    {
      values.push_back(it.Get());
    }
  }
  std::sort(values.begin(), values.end());
  return values;
}
} // namespace

int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest6(int, char *[])
{
  auto filter = FilterType::New();
  ITK_EXERCISE_BASIC_OBJECT_METHODS(filter, CannyEdgeDetectionRecursiveGaussianImageFilter, ImageToImageFilter);

  // Defaults.
  ITK_TEST_SET_GET_VALUE(1.0, filter->GetSigma());
  ITK_TEST_SET_GET_VALUE(0.0f, filter->GetUpperThreshold());
  ITK_TEST_SET_GET_VALUE(0.0f, filter->GetLowerThreshold());

  // Sigma accessors.
  filter->SetSigma(2.0);
  FilterType::SigmaArrayType expectedSigma(2.0);
  ITK_TEST_SET_GET_VALUE(expectedSigma, filter->GetSigmaArray());

  FilterType::SigmaArrayType anisotropic;
  anisotropic[0] = 0.8;
  anisotropic[1] = 1.6;
  filter->SetSigmaArray(anisotropic);
  ITK_TEST_SET_GET_VALUE(anisotropic, filter->GetSigmaArray());
  ITK_TEST_SET_GET_VALUE(0.8, filter->GetSigma());

  const auto modifiedTime = filter->GetMTime();
  filter->SetSigmaArray(anisotropic);
  ITK_TEST_SET_GET_VALUE(modifiedTime, filter->GetMTime());


  // No input.
  ITK_TRY_EXPECT_EXCEPTION(filter->Update());

  // The recursive Gaussian needs at least four pixels along each axis.
  filter = FilterType::New();
  auto tiny = CreateImage(3, 40);
  filter->SetInput(tiny);
  ITK_TRY_EXPECT_EXCEPTION(filter->Update());

  // Output matches the input geometry, and the non-maximum suppression image is available.
  auto image = CreateImage(52, 44);
  filter = FilterType::New();
  filter->SetInput(image);
  filter->SetUpperThreshold(20.0f);
  filter->SetLowerThreshold(10.0f);
  ITK_TRY_EXPECT_NO_EXCEPTION(filter->Update());
  ITK_TEST_SET_GET_VALUE(image->GetLargestPossibleRegion(), filter->GetOutput()->GetLargestPossibleRegion());
  ITK_TEST_EXPECT_TRUE(filter->GetNonMaximumSuppressionImage() != nullptr);
  ITK_TEST_SET_GET_VALUE(image->GetLargestPossibleRegion(),
                         filter->GetNonMaximumSuppressionImage()->GetLargestPossibleRegion());
  filter->Print(std::cout);

  // Strong seeds only, then the same seeds linked through weaker responses.
  const FilterType::SigmaArrayType isotropic(1.0);
  const auto                       responses = Responses(image, isotropic);
  ITK_TEST_EXPECT_TRUE(responses.size() > 20);
  const float weak = responses[responses.size() / 4];
  const float strong = responses[responses.size() * 9 / 10];

  const auto all = CountEdges(image, weak, weak, isotropic);
  const auto seedsOnly = CountEdges(image, strong, strong, isotropic);
  const auto linked = CountEdges(image, strong, weak, isotropic);
  const auto linkedMore = CountEdges(image, strong, 0.0f, isotropic);
  const auto none = CountEdges(image, responses.back() + 1.0f, 0.0f, isotropic);
  std::cout << "edges: all=" << all << " seedsOnly=" << seedsOnly << " linked=" << linked
            << " linkedMore=" << linkedMore << " none=" << none << std::endl;
  ITK_TEST_EXPECT_TRUE(seedsOnly > 0);
  ITK_TEST_EXPECT_TRUE(linked > seedsOnly);
  ITK_TEST_EXPECT_TRUE(linkedMore >= linked);
  ITK_TEST_EXPECT_TRUE(all >= linked);
  ITK_TEST_EXPECT_TRUE(none == 0);

  // Anisotropic smoothing changes the response.
  const auto isotropicResponses = Responses(image, isotropic);
  const auto anisotropicResponses = Responses(image, anisotropic);
  ITK_TEST_EXPECT_TRUE(isotropicResponses != anisotropicResponses);

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
