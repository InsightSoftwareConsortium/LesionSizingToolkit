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
#ifndef itkSinglePhaseLevelSetSegmentationModule_hxx
#define itkSinglePhaseLevelSetSegmentationModule_hxx

#include "itkLandmarkSpatialObject.h"
#include "itkIntensityWindowingImageFilter.h"

namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SinglePhaseLevelSetSegmentationModule<NDimension>::SinglePhaseLevelSetSegmentationModule()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());

  this->m_MaximumNumberOfIterations = 100;
  this->m_MaximumRMSError = 0.00001;

  this->m_AdvectionScaling = 1.0;
  this->m_CurvatureScaling = 75.0;
  this->m_PropagationScaling = 100.0;
  this->m_ZeroSetInputImage = nullptr;
  this->m_InvertOutputIntensities = true;
}


/**
 * Destructor
 */
template <unsigned int NDimension>
SinglePhaseLevelSetSegmentationModule<NDimension>::~SinglePhaseLevelSetSegmentationModule() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
SinglePhaseLevelSetSegmentationModule<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "PropagationScaling = " << this->m_PropagationScaling << std::endl;
  os << indent << "CurvatureScaling = " << this->m_CurvatureScaling << std::endl;
  os << indent << "AdvectionScaling = " << this->m_AdvectionScaling << std::endl;
  os << indent << "MaximumRMSError = " << this->m_MaximumRMSError << std::endl;
  os << indent << "MaximumNumberOfIterations = " << this->m_MaximumNumberOfIterations << std::endl;
}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input image from the input SpatialObject.
 */
template <unsigned int NDimension>
const typename SinglePhaseLevelSetSegmentationModule<NDimension>::InputImageType *
SinglePhaseLevelSetSegmentationModule<NDimension>::GetInternalInputImage() const
{
  const auto * inputObject = dynamic_cast<const InputSpatialObjectType *>(this->GetInput());
  if (inputObject)
  {
    const InputImageType * inputImage = inputObject->GetImage();
    this->m_ZeroSetInputImage = const_cast<InputImageType *>(inputImage);
    return inputImage;
  }

  if (this->m_ZeroSetInputImage)
  {
    return this->m_ZeroSetInputImage;
  }

  return nullptr;
}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input feature image from the input feature SpatialObject.
 */
template <unsigned int NDimension>
const typename SinglePhaseLevelSetSegmentationModule<NDimension>::FeatureImageType *
SinglePhaseLevelSetSegmentationModule<NDimension>::GetInternalFeatureImage() const
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
SinglePhaseLevelSetSegmentationModule<NDimension>::PackOutputImageInOutputSpatialObject(OutputImageType * image)
{
  typename OutputImageType::Pointer outputImage = image;

  if (this->m_InvertOutputIntensities)
  {
    using CalculatorType = MinimumMaximumImageCalculator<OutputImageType>;
    typename CalculatorType::Pointer calculator = CalculatorType::New();
    calculator->SetImage(outputImage);
    calculator->Compute();
    using RescaleFilterType = IntensityWindowingImageFilter<OutputImageType, OutputImageType>;
    typename RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
    rescaler->SetInput(outputImage);
    rescaler->SetWindowMinimum(calculator->GetMinimum());
    rescaler->SetWindowMaximum(calculator->GetMaximum());
    rescaler->SetOutputMinimum(4.0);  // Note that the values must be [4:-4] here to
    rescaler->SetOutputMaximum(-4.0); // make sure that we invert and not just rescale.
    rescaler->InPlaceOn();
    rescaler->Update();
    outputImage = rescaler->GetOutput();
  }

  outputImage->DisconnectPipeline();

  auto * outputObject = dynamic_cast<OutputSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(outputImage);
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
SinglePhaseLevelSetSegmentationModule<NDimension>::GenerateData()
{}

} // end namespace itk

#endif
