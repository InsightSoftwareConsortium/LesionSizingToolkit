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
#ifndef itkGeodesicActiveContourLevelSetSegmentationModule_hxx
#define itkGeodesicActiveContourLevelSetSegmentationModule_hxx

#include "itkGeodesicActiveContourLevelSetImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk
{


/**
 * Constructor
 */
template <unsigned int NDimension>
GeodesicActiveContourLevelSetSegmentationModule<NDimension>::GeodesicActiveContourLevelSetSegmentationModule() =
  default;


/**
 * Destructor
 */
template <unsigned int NDimension>
GeodesicActiveContourLevelSetSegmentationModule<NDimension>::~GeodesicActiveContourLevelSetSegmentationModule() =
  default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
GeodesicActiveContourLevelSetSegmentationModule<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
GeodesicActiveContourLevelSetSegmentationModule<NDimension>::GenerateData()
{
  using FilterType = GeodesicActiveContourLevelSetImageFilter<InputImageType, FeatureImageType, OutputPixelType>;

  typename FilterType::Pointer filter = FilterType::New();

  filter->SetInput(this->GetInternalInputImage());
  filter->SetFeatureImage(this->GetInternalFeatureImage());

  filter->SetMaximumRMSError(this->GetMaximumRMSError());
  filter->SetNumberOfIterations(this->GetMaximumNumberOfIterations());
  filter->SetPropagationScaling(this->GetPropagationScaling());
  filter->SetCurvatureScaling(this->GetCurvatureScaling());
  filter->SetAdvectionScaling(this->GetAdvectionScaling());
  filter->UseImageSpacingOn();

  // Progress reporting - forward events from the fast marching filter.
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(filter, 1.0);

  filter->Update();

  std::cout << std::endl;
  std::cout << "Max. no. iterations: " << filter->GetNumberOfIterations() << std::endl;
  std::cout << "Max. RMS error: " << filter->GetMaximumRMSError() << std::endl;
  std::cout << std::endl;
  std::cout << "No. elpased iterations: " << filter->GetElapsedIterations() << std::endl;
  std::cout << "RMS change: " << filter->GetRMSChange() << std::endl;

  this->PackOutputImageInOutputSpatialObject(filter->GetOutput());
}

} // end namespace itk

#endif
