/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest5.cxx

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

namespace
{
using ImageType = itk::Image<float, 2>;

// A box whose edge strength varies along its contour, so that hysteresis
// linking decides which contour pixels survive.
ImageType::Pointer
CreateImage(const ImageType::IndexType & start)
{
  ImageType::SizeType size;
  size.Fill(64);

  auto image = ImageType::New();
  image->SetRegions(ImageType::RegionType(start, size));
  image->Allocate(true);

  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetBufferedRegion());
  for (; !it.IsAtEnd(); ++it)
  {
    const auto offset = it.GetIndex() - start;
    const bool inside = offset[0] >= 16 && offset[0] < 48 && offset[1] >= 16 && offset[1] < 48;
    it.Set(inside ? 10.0f + 90.0f * offset[0] / 64.0f + 40.0f * offset[1] / 64.0f : 0.0f);
  }
  return image;
}

ImageType::Pointer
Detect(const ImageType::IndexType & start)
{
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<ImageType, ImageType>;
  auto filter = FilterType::New();
  filter->SetInput(CreateImage(start));
  filter->SetSigma(1.0);
  filter->SetUpperThreshold(32.0f);
  filter->SetLowerThreshold(8.0f);
  filter->Update();
  ImageType::Pointer output = filter->GetOutput();
  output->DisconnectPipeline();
  return output;
}
} // namespace

// The detected edges must not depend on where the image region starts.
int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest5(int, char *[])
{
  ImageType::IndexType origin;
  origin.Fill(0);
  const auto reference = Detect(origin);

  int status = EXIT_SUCCESS;
  for (const auto & start : { ImageType::IndexType{ { 10, -7 } }, ImageType::IndexType{ { -30, 40 } } })
  {
    const auto                               shifted = Detect(start);
    itk::ImageRegionConstIterator<ImageType> refIt(reference, reference->GetBufferedRegion());
    itk::ImageRegionConstIterator<ImageType> shiftedIt(shifted, shifted->GetBufferedRegion());
    itk::SizeValueType                       differing = 0;
    for (; !refIt.IsAtEnd(); ++refIt, ++shiftedIt)
    {
      differing += refIt.Get() != shiftedIt.Get() ? 1 : 0;
    }
    if (differing != 0)
    {
      std::cerr << "Region starting at " << start << " differs from the origin-based result in " << differing
                << " pixels." << std::endl;
      status = EXIT_FAILURE;
    }
  }
  return status;
}
