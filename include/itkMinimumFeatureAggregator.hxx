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
#ifndef itkMinimumFeatureAggregator_hxx
#define itkMinimumFeatureAggregator_hxx

#include "itkImageSpatialObject.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
MinimumFeatureAggregator<NDimension>::MinimumFeatureAggregator() = default;


/**
 * Destructor
 */
template <unsigned int NDimension>
MinimumFeatureAggregator<NDimension>::~MinimumFeatureAggregator() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
MinimumFeatureAggregator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <unsigned int NDimension>
void
MinimumFeatureAggregator<NDimension>::ConsolidateFeatures()
{
  using FeaturePixelType = float;
  using FeatureImageType = Image<FeaturePixelType, NDimension>;
  using FeatureSpatialObjectType = ImageSpatialObject<NDimension, FeaturePixelType>;

  const auto * firstFeatureObject = dynamic_cast<const FeatureSpatialObjectType *>(this->GetInputFeature(0));

  const FeatureImageType * firstFeatureImage = firstFeatureObject->GetImage();

  typename FeatureImageType::Pointer consolidatedFeatureImage = FeatureImageType::New();

  consolidatedFeatureImage->CopyInformation(firstFeatureImage);
  consolidatedFeatureImage->SetRegions(firstFeatureImage->GetBufferedRegion());
  consolidatedFeatureImage->Allocate();
  consolidatedFeatureImage->FillBuffer(NumericTraits<FeaturePixelType>::max());

  const unsigned int numberOfFeatures = this->GetNumberOfInputFeatures();

  for (unsigned int i = 0; i < numberOfFeatures; i++)
  {
    const auto * featureObject = dynamic_cast<const FeatureSpatialObjectType *>(this->GetInputFeature(i));

    const FeatureImageType * featureImage = featureObject->GetImage();

    using FeatureIterator = ImageRegionIterator<FeatureImageType>;
    using FeatureConstIterator = ImageRegionConstIterator<FeatureImageType>;

    FeatureIterator      dstitr(consolidatedFeatureImage, consolidatedFeatureImage->GetBufferedRegion());
    FeatureConstIterator srcitr(featureImage, featureImage->GetBufferedRegion());

    dstitr.GoToBegin();
    srcitr.GoToBegin();

    while (!srcitr.IsAtEnd())
    {
      if (dstitr.Get() > srcitr.Get())
      {
        dstitr.Set(srcitr.Get());
      }
      ++srcitr;
      ++dstitr;
    }
  }

  auto * outputObject = dynamic_cast<FeatureSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(consolidatedFeatureImage);
}

} // end namespace itk

#endif
