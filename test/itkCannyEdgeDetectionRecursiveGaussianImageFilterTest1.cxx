/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkCommand.h"
#include "itkImage.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkTestingMacros.h"

#include <algorithm>
#include <vector>

namespace
{

template <unsigned int VDimension>
typename itk::Image<float, VDimension>::Pointer
CreateBoxImage(itk::SizeValueType sideLength, itk::IndexValueType boxLower, itk::IndexValueType boxUpper)
{
  using ImageType = itk::Image<float, VDimension>;
  typename ImageType::SizeType size;
  size.Fill(sideLength);

  auto image = ImageType::New();
  image->SetRegions(typename ImageType::RegionType(size));
  image->Allocate(true);

  itk::ImageRegionIteratorWithIndex<ImageType> it(image, image->GetBufferedRegion());
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {
    const auto index = it.GetIndex();
    bool       inside = true;
    for (unsigned int d = 0; d < VDimension; ++d)
    {
      inside = inside && index[d] >= boxLower && index[d] < boxUpper;
    }
    it.Set(inside ? 100.0f : 0.0f);
  }
  return image;
}

// True when the index lies within `band` voxels of the surface of the box.
template <unsigned int VDimension>
bool
IsNearBoxSurface(const itk::Index<VDimension> & index,
                 itk::IndexValueType            boxLower,
                 itk::IndexValueType            boxUpper,
                 itk::IndexValueType            band)
{
  bool withinOuter = true;
  bool withinInner = true;
  for (unsigned int d = 0; d < VDimension; ++d)
  {
    withinOuter = withinOuter && index[d] >= boxLower - band && index[d] < boxUpper + band;
    withinInner = withinInner && index[d] >= boxLower + band && index[d] < boxUpper - band;
  }
  return withinOuter && !withinInner;
}

class ProgressRecorder : public itk::Command
{
public:
  using Self = ProgressRecorder;
  using Superclass = itk::Command;
  using Pointer = itk::SmartPointer<Self>;
  itkNewMacro(Self);

  void
  Execute(itk::Object * caller, const itk::EventObject & event) override
  {
    this->Execute(static_cast<const itk::Object *>(caller), event);
  }

  void
  Execute(const itk::Object * caller, const itk::EventObject &) override
  {
    const auto * process = dynamic_cast<const itk::ProcessObject *>(caller);
    m_Values.push_back(process->GetProgress());
  }

  std::vector<float> m_Values;

protected:
  ProgressRecorder() = default;
};

template <unsigned int VDimension>
int
RunTest(itk::SizeValueType sideLength, itk::IndexValueType boxLower, itk::IndexValueType boxUpper)
{
  using ImageType = itk::Image<float, VDimension>;
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<ImageType, ImageType>;

  constexpr itk::IndexValueType band = 3;

  auto input = CreateBoxImage<VDimension>(sideLength, boxLower, boxUpper);

  auto filter = FilterType::New();
  ITK_EXERCISE_BASIC_OBJECT_METHODS(filter, CannyEdgeDetectionRecursiveGaussianImageFilter, ImageToImageFilter);

  filter->SetInput(input);
  filter->SetSigma(1.0);
  filter->SetUpperThreshold(20.0f);
  filter->SetLowerThreshold(10.0f);
  ITK_TEST_SET_GET_VALUE(20.0f, filter->GetUpperThreshold());
  ITK_TEST_SET_GET_VALUE(10.0f, filter->GetLowerThreshold());

  auto progress = ProgressRecorder::New();
  filter->AddObserver(itk::ProgressEvent(), progress);

  // The result must not depend on the number of work units.
  typename ImageType::Pointer reference;
  for (const unsigned int workUnits : { 1u, 2u, 3u, 8u })
  {
    filter->SetNumberOfWorkUnits(workUnits);
    progress->m_Values.clear();
    filter->Modified();
    ITK_TRY_EXPECT_NO_EXCEPTION(filter->Update());

    if (progress->m_Values.empty())
    {
      std::cerr << "No progress events with " << workUnits << " work units." << std::endl;
      return EXIT_FAILURE;
    }
    const auto [minProgress, maxProgress] = std::minmax_element(progress->m_Values.begin(), progress->m_Values.end());
    if (*minProgress < 0.0f || *maxProgress > 1.0f)
    {
      std::cerr << "Progress outside [0, 1]: [" << *minProgress << ", " << *maxProgress << "]" << std::endl;
      return EXIT_FAILURE;
    }
    if (*maxProgress < 0.9f)
    {
      std::cerr << "Progress never approached completion: max " << *maxProgress << std::endl;
      return EXIT_FAILURE;
    }

    typename ImageType::Pointer output = filter->GetOutput();
    output->DisconnectPipeline();

    if (reference.IsNull())
    {
      reference = output;
      continue;
    }
    itk::ImageRegionConstIteratorWithIndex<ImageType> refIt(reference, reference->GetBufferedRegion());
    itk::ImageRegionConstIteratorWithIndex<ImageType> outIt(output, output->GetBufferedRegion());
    for (; !refIt.IsAtEnd(); ++refIt, ++outIt)
    {
      if (refIt.Get() != outIt.Get())
      {
        std::cerr << "Output differs between 1 and " << workUnits << " work units at " << refIt.GetIndex() << std::endl;
        return EXIT_FAILURE;
      }
    }
  }

  // Edges are binary, present, and lie on the surface of the box.
  itk::SizeValueType                                edgeCount = 0;
  itk::ImageRegionConstIteratorWithIndex<ImageType> it(reference, reference->GetBufferedRegion());
  for (it.GoToBegin(); !it.IsAtEnd(); ++it)
  {
    const float value = it.Get();
    if (value != 0.0f && value != 1.0f)
    {
      std::cerr << "Non-binary edge value " << value << " at " << it.GetIndex() << std::endl;
      return EXIT_FAILURE;
    }
    if (value == 1.0f)
    {
      ++edgeCount;
      if (!IsNearBoxSurface<VDimension>(it.GetIndex(), boxLower, boxUpper, band))
      {
        std::cerr << "Edge pixel away from the box surface at " << it.GetIndex() << std::endl;
        return EXIT_FAILURE;
      }
    }
  }
  if (edgeCount == 0)
  {
    std::cerr << "No edge pixels detected." << std::endl;
    return EXIT_FAILURE;
  }

  // A smaller box must move the edges, which guards against a constant output.
  auto smallerInput = CreateBoxImage<VDimension>(sideLength, boxLower + 4, boxUpper - 4);
  filter->SetInput(smallerInput);
  filter->SetNumberOfWorkUnits(4);
  ITK_TRY_EXPECT_NO_EXCEPTION(filter->Update());
  itk::ImageRegionConstIteratorWithIndex<ImageType> smallIt(filter->GetOutput(),
                                                            filter->GetOutput()->GetBufferedRegion());
  for (smallIt.GoToBegin(); !smallIt.IsAtEnd(); ++smallIt)
  {
    if (smallIt.Get() == 1.0f && !IsNearBoxSurface<VDimension>(smallIt.GetIndex(), boxLower + 4, boxUpper - 4, band))
    {
      std::cerr << "Stale edge pixel after changing the input at " << smallIt.GetIndex() << std::endl;
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}

} // namespace

int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest1(int, char *[])
{
  if (RunTest<2>(64, 16, 48) != EXIT_SUCCESS)
  {
    return EXIT_FAILURE;
  }
  if (RunTest<3>(40, 10, 30) != EXIT_SUCCESS)
  {
    return EXIT_FAILURE;
  }

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
