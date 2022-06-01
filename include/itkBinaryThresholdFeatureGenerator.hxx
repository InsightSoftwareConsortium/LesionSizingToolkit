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
#ifndef itkBinaryThresholdFeatureGenerator_hxx
#define itkBinaryThresholdFeatureGenerator_hxx

#include "itkProgressAccumulator.h"

namespace itk
{

template <unsigned int NDimension>
BinaryThresholdFeatureGenerator<NDimension>::BinaryThresholdFeatureGenerator()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  this->m_BinaryThresholdFilter = BinaryThresholdFilterType::New();

  this->m_BinaryThresholdFilter->ReleaseDataFlagOn();

  typename OutputImageSpatialObjectType::Pointer outputObject = OutputImageSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());

  this->m_Threshold = 128.0;
}


template <unsigned int NDimension>
BinaryThresholdFeatureGenerator<NDimension>::~BinaryThresholdFeatureGenerator() = default;


template <unsigned int NDimension>
void
BinaryThresholdFeatureGenerator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


template <unsigned int NDimension>
void
BinaryThresholdFeatureGenerator<NDimension>::GenerateData()
{
  // Report progress.
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(this->m_BinaryThresholdFilter, 1.0);

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

  this->m_BinaryThresholdFilter->SetInput(inputImage);
  this->m_BinaryThresholdFilter->SetLowerThreshold(this->m_Threshold);
  this->m_BinaryThresholdFilter->SetUpperThreshold(itk::NumericTraits<OutputPixelType>::max());
  this->m_BinaryThresholdFilter->SetOutsideValue(0.0);
  this->m_BinaryThresholdFilter->SetInsideValue(1.0);

  this->m_BinaryThresholdFilter->Update();

  typename OutputImageType::Pointer outputImage = this->m_BinaryThresholdFilter->GetOutput();

  outputImage->DisconnectPipeline();

  auto * outputObject = dynamic_cast<OutputImageSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(outputImage);
}

} // end namespace itk

#endif
