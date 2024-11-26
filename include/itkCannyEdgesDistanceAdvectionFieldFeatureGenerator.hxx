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
#ifndef itkCannyEdgesDistanceAdvectionFieldFeatureGenerator_hxx
#define itkCannyEdgesDistanceAdvectionFieldFeatureGenerator_hxx


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::CannyEdgesDistanceAdvectionFieldFeatureGenerator()
{
  this->SetNumberOfRequiredInputs(1);

  this->m_CastFilter = CastFilterType::New();
  this->m_DistanceMapFilter = DistanceMapFilterType::New();
  this->m_CannyFilter = CannyEdgeFilterType::New();
  this->m_MultiplyFilter = MultiplyFilterType::New();
  this->m_GradientFilter = GradientFilterType::New();

  typename OutputImageSpatialObjectType::Pointer outputObject = OutputImageSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());

  this->m_Sigma = 1.0;
  this->m_UpperThreshold = NumericTraits<InternalPixelType>::max();
  this->m_LowerThreshold = NumericTraits<InternalPixelType>::min();
}


/*
 * Destructor
 */
template <unsigned int NDimension>
CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::~CannyEdgesDistanceAdvectionFieldFeatureGenerator() =
  default;

template <unsigned int NDimension>
void
CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::SetInput(const SpatialObjectType * spatialObject)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<SpatialObjectType *>(spatialObject));
}

template <unsigned int NDimension>
const typename CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::SpatialObjectType *
CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::GetFeature() const
{
  return static_cast<const SpatialObjectType *>(this->ProcessObject::GetOutput(0));
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
CannyEdgesDistanceAdvectionFieldFeatureGenerator<NDimension>::GenerateData()
{
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

  this->m_CastFilter->SetInput(inputImage);
  this->m_CannyFilter->SetInput(this->m_CastFilter->GetOutput());
  this->m_DistanceMapFilter->SetInput(this->m_CannyFilter->GetOutput());

  this->m_CannyFilter->SetSigma(this->m_Sigma);
  this->m_CannyFilter->SetUpperThreshold(this->m_UpperThreshold);
  this->m_CannyFilter->SetLowerThreshold(this->m_LowerThreshold);
  this->m_CannyFilter->SetOutsideValue(NumericTraits<InternalPixelType>::Zero);

  this->m_DistanceMapFilter->Update();

  m_GradientFilter->SetInput(m_DistanceMapFilter->GetOutput());
  m_GradientFilter->Update();

  m_MultiplyFilter->SetInput1(m_GradientFilter->GetOutput());
  m_MultiplyFilter->SetInput2(m_DistanceMapFilter->GetOutput());
  m_MultiplyFilter->Update();

  typename OutputImageType::Pointer outputImage = this->m_MultiplyFilter->GetOutput();

  outputImage->DisconnectPipeline();

  auto * outputObject = dynamic_cast<OutputImageSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(outputImage);
}

} // end namespace itk

#endif
