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
#ifndef itkCannyEdgesFeatureGenerator_hxx
#define itkCannyEdgesFeatureGenerator_hxx



namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
CannyEdgesFeatureGenerator<NDimension>::CannyEdgesFeatureGenerator()
{
  this->SetNumberOfRequiredInputs(1);

  this->m_CastFilter = CastFilterType::New();
  this->m_RescaleFilter = RescaleFilterType::New();
  this->m_CannyFilter = CannyEdgeFilterType::New();

  typename OutputImageSpatialObjectType::Pointer outputObject = OutputImageSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());

  this->m_Sigma.Fill(1.0);
  this->m_UpperThreshold = NumericTraits<InternalPixelType>::max();
  this->m_LowerThreshold = NumericTraits<InternalPixelType>::min();

  this->m_RescaleFilter->SetOutputMinimum(1.0);
  this->m_RescaleFilter->SetOutputMaximum(0.0);

  this->m_RescaleFilter->SetWindowMinimum(0.0);
  this->m_RescaleFilter->SetWindowMaximum(1.0);
}


/*
 * Destructor
 */
template <unsigned int NDimension>
CannyEdgesFeatureGenerator<NDimension>::~CannyEdgesFeatureGenerator() = default;

template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>::SetInput(const SpatialObjectType * spatialObject)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<SpatialObjectType *>(spatialObject));
}

template <unsigned int NDimension>
const typename CannyEdgesFeatureGenerator<NDimension>::SpatialObjectType *
CannyEdgesFeatureGenerator<NDimension>::GetFeature() const
{
  return static_cast<const SpatialObjectType *>(this->ProcessObject::GetOutput(0));
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>::GenerateData()
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
  this->m_RescaleFilter->SetInput(this->m_CannyFilter->GetOutput());

  this->m_CannyFilter->SetSigmaArray(this->m_Sigma);
  this->m_CannyFilter->SetUpperThreshold(this->m_UpperThreshold);
  this->m_CannyFilter->SetLowerThreshold(this->m_LowerThreshold);
  this->m_CannyFilter->SetOutsideValue(NumericTraits<InternalPixelType>::Zero);

  this->m_RescaleFilter->Update();

  typename OutputImageType::Pointer outputImage = this->m_RescaleFilter->GetOutput();

  outputImage->DisconnectPipeline();

  auto * outputObject = dynamic_cast<OutputImageSpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetImage(outputImage);
}


// Set value of Sigma (isotropic)

template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>::SetSigma(ScalarRealType sigma)
{
  SigmaArrayType sigmas(sigma);
  this->SetSigmaArray(sigmas);
}


// Set value of Sigma (an-isotropic)

template <unsigned int NDimension>
void
CannyEdgesFeatureGenerator<NDimension>::SetSigmaArray(const SigmaArrayType & sigma)
{
  if (this->m_Sigma != sigma)
  {
    this->m_Sigma = sigma;
    this->Modified();
  }
}


// Get the sigma array.
template <unsigned int NDimension>
typename CannyEdgesFeatureGenerator<NDimension>::SigmaArrayType
CannyEdgesFeatureGenerator<NDimension>::GetSigmaArray() const
{
  return m_Sigma;
}


// Get the sigma scalar. If the sigma is anisotropic, we will just
// return the sigma along the first dimension.
template <unsigned int NDimension>
typename CannyEdgesFeatureGenerator<NDimension>::ScalarRealType
CannyEdgesFeatureGenerator<NDimension>::GetSigma() const
{
  return m_Sigma[0];
}

} // end namespace itk

#endif
