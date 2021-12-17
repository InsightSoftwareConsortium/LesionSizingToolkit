/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkGrayscaleImageSegmentationVolumeEstimator_hxx
#define itkGrayscaleImageSegmentationVolumeEstimator_hxx

#include "itkImageSpatialObject.h"
#include "itkImageRegionIterator.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
GrayscaleImageSegmentationVolumeEstimator<NDimension>::GrayscaleImageSegmentationVolumeEstimator() = default;


/**
 * Destructor
 */
template <unsigned int NDimension>
GrayscaleImageSegmentationVolumeEstimator<NDimension>::~GrayscaleImageSegmentationVolumeEstimator() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
GrayscaleImageSegmentationVolumeEstimator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
GrayscaleImageSegmentationVolumeEstimator<NDimension>::GenerateData()
{
  typename InputImageSpatialObjectType::ConstPointer inputObject =
    dynamic_cast<const InputImageSpatialObjectType *>(this->ProcessObject::GetInput(0));

  if (!inputObject)
  {
    itkExceptionMacro("Missing input spatial object or incorrect type");
  }

  const InputImageType * inputImage = inputObject->GetImage();

  double sumOfIntensities = 0.0;

  double minimumIntensity = NumericTraits<double>::max();
  double maximumIntensity = NumericTraits<double>::NonpositiveMin();

  using IteratorType = ImageRegionConstIterator<InputImageType>;

  using ImageRegionType = typename InputImageType::RegionType;

  const ImageRegionType region = inputImage->GetBufferedRegion();

  IteratorType itr(inputImage, region);

  itr.GoToBegin();

  while (!itr.IsAtEnd())
  {
    const double pixelValue = itr.Get();

    if (pixelValue < minimumIntensity)
    {
      minimumIntensity = pixelValue;
    }

    if (pixelValue > maximumIntensity)
    {
      maximumIntensity = pixelValue;
    }

    sumOfIntensities += pixelValue;
    ++itr;
  }

  const unsigned int long numberOfPixels = region.GetNumberOfPixels();

  sumOfIntensities -= numberOfPixels * minimumIntensity;

  using SpacingType = typename InputImageType::SpacingType;

  const SpacingType spacing = inputImage->GetSpacing();

  double pixelVolume = spacing[0] * spacing[1] * spacing[2];

  //
  // Deal with eventual cases of negative spacing
  //
  if (pixelVolume < 0.0)
  {
    pixelVolume = -pixelVolume;
  }

  const double intensityRange = (maximumIntensity - minimumIntensity);

  double volumeEstimation = 0.0;

  if (intensityRange > 1e-6)
  {
    volumeEstimation = pixelVolume * sumOfIntensities / intensityRange;
  }

  auto * outputCarrier = static_cast<RealObjectType *>(this->ProcessObject::GetOutput(0));

  outputCarrier->Set(volumeEstimation);
}

} // end namespace itk

#endif
