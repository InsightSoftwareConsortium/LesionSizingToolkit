/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkRegionGrowingSegmentationModule_hxx
#define itkRegionGrowingSegmentationModule_hxx

#include "itkImageRegionIterator.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
RegionGrowingSegmentationModule<NDimension>::RegionGrowingSegmentationModule()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());
}


/**
 * Destructor
 */
template <unsigned int NDimension>
RegionGrowingSegmentationModule<NDimension>::~RegionGrowingSegmentationModule() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
RegionGrowingSegmentationModule<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
RegionGrowingSegmentationModule<NDimension>::GenerateData()
{}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input image from the input SpatialObject.
 */
template <unsigned int NDimension>
const typename RegionGrowingSegmentationModule<NDimension>::InputSpatialObjectType *
RegionGrowingSegmentationModule<NDimension>::GetInternalInputLandmarks() const
{
  const auto * inputObject = dynamic_cast<const InputSpatialObjectType *>(this->GetInput());

  return inputObject;
}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input feature image from the input feature SpatialObject.
 */
template <unsigned int NDimension>
const typename RegionGrowingSegmentationModule<NDimension>::FeatureImageType *
RegionGrowingSegmentationModule<NDimension>::GetInternalFeatureImage() const
{
  const auto * featureObject = dynamic_cast<const FeatureSpatialObjectType *>(this->GetFeature());

  const FeatureImageType * featureImage = featureObject->GetImage();

  return featureImage;
}


/**
 * This method is intended to be used only by the subclasses to insert the
 * output image as cargo of the output spatial object.
 */
template <unsigned int NDimension>
void
RegionGrowingSegmentationModule<NDimension>::PackOutputImageInOutputSpatialObject(OutputImageType * image)
{
  typename OutputImageType::Pointer outputImage = image;

  outputImage->DisconnectPipeline();

  this->ConvertIntensitiesToCenteredRange(outputImage);

  auto * outputObject = dynamic_cast<OutputSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(outputImage);
}

/**
 * This method is intended to be used only by this class. It should be called
 * from the PackOutputImageInOutputSpatialObject() method.
 */
template <unsigned int NDimension>
void
RegionGrowingSegmentationModule<NDimension>::ConvertIntensitiesToCenteredRange(OutputImageType * image)
{
  using IteratorType = ImageRegionIterator<OutputImageType>;

  IteratorType itr(image, image->GetBufferedRegion());

  itr.GoToBegin();

  //
  // Convert intensities to centered range
  //
  while (!itr.IsAtEnd())
  {
    if (itr.Get())
    {
      itr.Set(4.0);
    }
    else
    {
      itr.Set(-4.0);
    }
    ++itr;
  }
}


} // end namespace itk

#endif
