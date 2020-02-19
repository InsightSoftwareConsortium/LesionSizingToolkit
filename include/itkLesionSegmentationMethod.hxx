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
#ifndef itkLesionSegmentationMethod_hxx
#define itkLesionSegmentationMethod_hxx

#include "itkLesionSegmentationMethod.h"
#include "itkImageSpatialObject.h"
#include "itkImageRegionIterator.h"

// DEBUGGING code:
#include "itkImageFileWriter.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>::LesionSegmentationMethod()
{
  this->SetNumberOfRequiredOutputs(1); // for the Transform

  using OutputPixelType = float;
  using OutputSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());

  this->m_ProgressAccumulator = ProgressAccumulator::New();
  this->m_ProgressAccumulator->SetMiniPipelineFilter(this);
}


/**
 * Destructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>::~LesionSegmentationMethod() = default;


/**
 * Add a feature generator that will compute the Nth feature to be passed to
 * the segmentation module.
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::AddFeatureGenerator(FeatureGeneratorType * generator)
{
  this->m_FeatureGenerators.push_back(generator);
  this->Modified();
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "Region of Interest " << this->m_RegionOfInterest.GetPointer() << std::endl;
  os << "Initial Segmentation " << this->m_InitialSegmentation.GetPointer() << std::endl;
  os << "Segmentation Module " << this->m_SegmentationModule.GetPointer() << std::endl;

  os << "Feature generators = ";

  auto gitr = this->m_FeatureGenerators.begin();
  auto gend = this->m_FeatureGenerators.end();

  while (gitr != gend)
  {
    os << gitr->GetPointer() << std::endl;
    ++gitr;
  }
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::GenerateData()
{
  if (!this->m_SegmentationModule)
  {
    itkExceptionMacro("Segmentation Module has not been connected");
  }

  this->UpdateAllFeatureGenerators();

  this->VerifyNumberOfAvailableFeaturesMatchedExpectations();

  this->ConnectFeaturesToSegmentationModule();

  this->ExecuteSegmentationModule();
}


/**
 * Update feature generators
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::UpdateAllFeatureGenerators()
{
  auto gitr = this->m_FeatureGenerators.begin();
  auto gend = this->m_FeatureGenerators.end();

  while (gitr != gend)
  {
    this->m_ProgressAccumulator->RegisterInternalFilter(*gitr, 0.5 / this->m_FeatureGenerators.size());
    (*gitr)->Update();
    ++gitr;
  }
}


template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::VerifyNumberOfAvailableFeaturesMatchedExpectations() const
{
  const unsigned int expectedNumberOfFeatures = this->m_SegmentationModule->GetExpectedNumberOfFeatures();
  const unsigned int availableNumberOfFeatures = this->m_FeatureGenerators.size();

  if (expectedNumberOfFeatures != availableNumberOfFeatures)
  {
    itkExceptionMacro("Expecting " << expectedNumberOfFeatures << " but only got " << availableNumberOfFeatures);
  }
}


template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::ConnectFeaturesToSegmentationModule()
{
  if (!this->m_FeatureGenerators.empty())
  {
    if (this->m_FeatureGenerators[0]->GetFeature())
    {
      this->m_SegmentationModule->SetFeature(this->m_FeatureGenerators[0]->GetFeature());
    }
  }
}


template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>::ExecuteSegmentationModule()
{
  this->m_ProgressAccumulator->RegisterInternalFilter(this->m_SegmentationModule, 0.5);
  this->m_SegmentationModule->SetInput(this->m_InitialSegmentation);
  this->m_SegmentationModule->Update();
}


} // end namespace itk

#endif
