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
#ifndef itkSigmoidFeatureGenerator_hxx
#define itkSigmoidFeatureGenerator_hxx

#include "itkProgressAccumulator.h"

namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SigmoidFeatureGenerator<NDimension>::SigmoidFeatureGenerator()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->m_SigmoidFilter = SigmoidFilterType::New();

  this->m_SigmoidFilter->ReleaseDataFlagOn();

  typename OutputImageSpatialObjectType::Pointer outputObject = OutputImageSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());

  this->m_Alpha = -1.0;
  this->m_Beta = 128.0;
}


/*
 * Destructor
 */
template <unsigned int NDimension>
SigmoidFeatureGenerator<NDimension>::~SigmoidFeatureGenerator() = default;

template <unsigned int NDimension>
void
SigmoidFeatureGenerator<NDimension>::SetInput(const SpatialObjectType * spatialObject)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<SpatialObjectType *>(spatialObject));
}

template <unsigned int NDimension>
const typename SigmoidFeatureGenerator<NDimension>::SpatialObjectType *
SigmoidFeatureGenerator<NDimension>::GetFeature() const
{
  return static_cast<const SpatialObjectType *>(this->ProcessObject::GetOutput(0));
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
SigmoidFeatureGenerator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
SigmoidFeatureGenerator<NDimension>::GenerateData()
{
  // Report progress.
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(this->m_SigmoidFilter, 1.0);

  typename InputImageSpatialObjectType::ConstPointer inputObject =
    dynamic_cast<const InputImageSpatialObjectType *>(this->ProcessObject::GetInput(0));

  if (!inputObject)
  {
    itkExceptionMacro("Missing input spatial object or incorrect type");
  }

  const InputImageType * inputImage = inputObject->GetImage();

  if (!inputImage)
  {
    itkExceptionMacro("Missing input image");
  }

  this->m_SigmoidFilter->SetInput(inputImage);
  this->m_SigmoidFilter->SetAlpha(this->m_Alpha);
  this->m_SigmoidFilter->SetBeta(this->m_Beta);
  this->m_SigmoidFilter->SetOutputMinimum(0.0);
  this->m_SigmoidFilter->SetOutputMaximum(1.0);

  this->m_SigmoidFilter->Update();

  typename OutputImageType::Pointer outputImage = this->m_SigmoidFilter->GetOutput();

  outputImage->DisconnectPipeline();

  auto * outputObject = dynamic_cast<OutputImageSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(outputImage);
}

} // end namespace itk

#endif
