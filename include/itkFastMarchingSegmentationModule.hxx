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
#ifndef itkFastMarchingSegmentationModule_hxx
#define itkFastMarchingSegmentationModule_hxx

#include "itkImageRegionIterator.h"
#include "itkFastMarchingImageFilter.h"
#include "itkIntensityWindowingImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
FastMarchingSegmentationModule<NDimension>::FastMarchingSegmentationModule()
{
  this->m_StoppingValue =
    static_cast<double>(static_cast<OutputPixelType>(NumericTraits<OutputPixelType>::max() / 2.0));

  this->m_DistanceFromSeeds = 0.0;

  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject.GetPointer());
}


/**
 * Destructor
 */
template <unsigned int NDimension>
FastMarchingSegmentationModule<NDimension>::~FastMarchingSegmentationModule() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
FastMarchingSegmentationModule<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Stopping Value = " << this->m_StoppingValue << std::endl;
  os << indent << "Distance from seeds = " << this->m_DistanceFromSeeds << std::endl;
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
FastMarchingSegmentationModule<NDimension>::GenerateData()
{
  using FilterType = FastMarchingImageFilter<FeatureImageType, OutputImageType>;

  typename FilterType::Pointer filter = FilterType::New();

  const FeatureImageType * featureImage = this->GetInternalFeatureImage();

  filter->SetInput(featureImage);

  filter->SetStoppingValue(this->m_StoppingValue);

  // Progress reporting - forward events from the fast marching filter.
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(filter, 0.9);

  const InputSpatialObjectType * inputSeeds = this->GetInternalInputLandmarks();
  const unsigned int             numberOfPoints = inputSeeds->GetNumberOfPoints();

  using LandmarkPointListType = typename InputSpatialObjectType::LandmarkPointListType;
  using IndexType = typename FeatureImageType::IndexType;
  using IndexType = typename FeatureImageType::IndexType;
  using NodeContainer = typename FilterType::NodeContainer;
  using NodeType = typename FilterType::NodeType;

  typename NodeContainer::Pointer trialPoints = NodeContainer::New();

  const LandmarkPointListType & points = inputSeeds->GetPoints();


  for (unsigned int i = 0; i < numberOfPoints; i++)
  {
    const IndexType index = featureImage->TransformPhysicalPointToIndex(points[i].GetPositionInObjectSpace());

    NodeType node;

    // By starting the FastMarching front at this value,
    // the zero set will end up being placed at distance
    // = value from the seeds. That can be seen as computing
    // a distance map from the seeds.
    node.SetValue(-this->m_DistanceFromSeeds);

    node.SetIndex(index);
    trialPoints->InsertElement(i, node);
  }

  filter->SetTrialPoints(trialPoints);
  filter->Update();

  // Rescale the values to make the output intensity fit in the expected
  // range of [-4:4]
  using WindowingFilterType = itk::IntensityWindowingImageFilter<OutputImageType, OutputImageType>;
  typename WindowingFilterType::Pointer windowing = WindowingFilterType::New();
  windowing->SetInput(filter->GetOutput());
  windowing->SetWindowMinimum(-this->m_DistanceFromSeeds);
  windowing->SetWindowMaximum(this->m_StoppingValue);
  windowing->SetOutputMinimum(-4.0);
  windowing->SetOutputMaximum(4.0);
  windowing->InPlaceOn();
  progress->RegisterInternalFilter(windowing, 0.1);
  windowing->Update();

  this->PackOutputImageInOutputSpatialObject(windowing->GetOutput());
}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input image from the input SpatialObject.
 */
template <unsigned int NDimension>
const typename FastMarchingSegmentationModule<NDimension>::InputSpatialObjectType *
FastMarchingSegmentationModule<NDimension>::GetInternalInputLandmarks() const
{
  const auto * inputObject = dynamic_cast<const InputSpatialObjectType *>(this->GetInput());

  return inputObject;
}

} // end namespace itk

#endif
