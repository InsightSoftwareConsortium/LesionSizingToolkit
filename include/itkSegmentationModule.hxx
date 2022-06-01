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
#ifndef itkSegmentationModule_hxx
#define itkSegmentationModule_hxx



namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SegmentationModule<NDimension>::SegmentationModule()
{
  this->SetNumberOfRequiredOutputs(1);
}


/**
 * Destructor
 */
template <unsigned int NDimension>
SegmentationModule<NDimension>::~SegmentationModule() = default;

template <unsigned int NDimension>
void
SegmentationModule<NDimension>::SetInput(const SpatialObjectType * spatialObject)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<SpatialObjectType *>(spatialObject));
}

template <unsigned int NDimension>
const typename SegmentationModule<NDimension>::SpatialObjectType *
SegmentationModule<NDimension>::GetInput() const
{
  // Process object is not const-correct so the const casting is required.
  const auto * input = dynamic_cast<const SpatialObjectType *>(this->ProcessObject::GetInput(0));
  return input;
}


template <unsigned int NDimension>
void
SegmentationModule<NDimension>::SetFeature(const SpatialObjectType * spatialObject)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<SpatialObjectType *>(spatialObject));
}


template <unsigned int NDimension>
const typename SegmentationModule<NDimension>::SpatialObjectType *
SegmentationModule<NDimension>::GetFeature() const
{
  // Process object is not const-correct so the const casting is required.
  const auto * feature = dynamic_cast<const SpatialObjectType *>(this->ProcessObject::GetInput(1));
  return feature;
}


template <unsigned int NDimension>
unsigned int
SegmentationModule<NDimension>::GetExpectedNumberOfFeatures() const
{
  return 1;
}


template <unsigned int NDimension>
const typename SegmentationModule<NDimension>::SpatialObjectType *
SegmentationModule<NDimension>::GetOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const SpatialObjectType *>(this->ProcessObject::GetOutput(0));
}


/** This non-const version is intended only for the internal use in the derived
 * classes. */
template <unsigned int NDimension>
typename SegmentationModule<NDimension>::SpatialObjectType *
SegmentationModule<NDimension>::GetInternalOutput()
{
  if (this->GetNumberOfOutputs() < 1)
  {
    return 0;
  }

  return static_cast<SpatialObjectType *>(this->ProcessObject::GetOutput(0));
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
SegmentationModule<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace itk

#endif
