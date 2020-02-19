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
#ifndef itkSegmentationVolumeEstimator_hxx
#define itkSegmentationVolumeEstimator_hxx

#include "itkSegmentationVolumeEstimator.h"
#include "itkImageSpatialObject.h"
#include "itkImageRegionIterator.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SegmentationVolumeEstimator<NDimension>::SegmentationVolumeEstimator()
{
  this->SetNumberOfRequiredInputs(1);  // for the segmentation Spatial Object
  this->SetNumberOfRequiredOutputs(1); // for the Volume

  RealObjectType::Pointer output = RealObjectType::New();
  this->ProcessObject::SetNthOutput(0, output.GetPointer());
}


/**
 * Destructor
 */
template <unsigned int NDimension>
SegmentationVolumeEstimator<NDimension>::~SegmentationVolumeEstimator() = default;


/**
 * Return the value of the estimated volume
 */
template <unsigned int NDimension>
typename SegmentationVolumeEstimator<NDimension>::RealType
SegmentationVolumeEstimator<NDimension>::GetVolume() const
{
  return this->GetVolumeOutput()->Get();
}


/**
 * Return the value of the estimated volume stored in a DataObject decorator
 * that can be passed down a pipeline.
 */
template <unsigned int NDimension>
const typename SegmentationVolumeEstimator<NDimension>::RealObjectType *
SegmentationVolumeEstimator<NDimension>::GetVolumeOutput() const
{
  return static_cast<const RealObjectType *>(this->ProcessObject::GetOutput(0));
}


/**
 * Set the input SpatialObject representing the segmentation whose volume will
 * be estimated by this class.
 */
template <unsigned int NDimension>
void
SegmentationVolumeEstimator<NDimension>::SetInput(const SpatialObjectType * inputSpatialObject)
{
  this->SetNthInput(0, const_cast<SpatialObjectType *>(inputSpatialObject));
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
SegmentationVolumeEstimator<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
SegmentationVolumeEstimator<NDimension>::GenerateData()
{
  // This method is intended to be overridden by derived classes
}

} // end namespace itk

#endif
