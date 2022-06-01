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
#ifndef itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule_hxx
#define itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule_hxx

#include "itkGeodesicActiveContourLevelSetImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk
{


/**
 * Constructor
 */
template <unsigned int NDimension>
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<
  NDimension>::FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule()
{
  this->m_FastMarchingModule = FastMarchingModuleType::New();
  this->m_FastMarchingModule->SetDistanceFromSeeds(1.0);
  this->m_FastMarchingModule->SetStoppingValue(100.0);
  this->m_FastMarchingModule->InvertOutputIntensitiesOff();
  this->m_GeodesicActiveContourLevelSetModule = GeodesicActiveContourLevelSetModuleType::New();
  this->m_GeodesicActiveContourLevelSetModule->InvertOutputIntensitiesOff();
}


/**
 * Destructor
 */
template <unsigned int NDimension>
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<
  NDimension>::~FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<NDimension>::PrintSelf(std::ostream & os,
                                                                                      Indent         indent) const
{
  Superclass::PrintSelf(os, indent);
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<NDimension>::GenerateData()
{
  // Report progress.
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(this->m_FastMarchingModule, 0.3);
  progress->RegisterInternalFilter(this->m_GeodesicActiveContourLevelSetModule, 0.7);

  this->m_FastMarchingModule->SetInput(this->GetInput());
  this->m_FastMarchingModule->SetFeature(this->GetFeature());
  this->m_FastMarchingModule->Update();

  m_GeodesicActiveContourLevelSetModule->SetInput(m_FastMarchingModule->GetOutput());
  m_GeodesicActiveContourLevelSetModule->SetFeature(this->GetFeature());
  m_GeodesicActiveContourLevelSetModule->SetMaximumRMSError(this->GetMaximumRMSError());
  m_GeodesicActiveContourLevelSetModule->SetMaximumNumberOfIterations(this->GetMaximumNumberOfIterations());
  m_GeodesicActiveContourLevelSetModule->SetPropagationScaling(this->GetPropagationScaling());
  m_GeodesicActiveContourLevelSetModule->SetCurvatureScaling(this->GetCurvatureScaling());
  m_GeodesicActiveContourLevelSetModule->SetAdvectionScaling(this->GetAdvectionScaling());
  m_GeodesicActiveContourLevelSetModule->Update();

  this->PackOutputImageInOutputSpatialObject(const_cast<OutputImageType *>(
    dynamic_cast<const OutputSpatialObjectType *>(m_GeodesicActiveContourLevelSetModule->GetOutput())->GetImage()));
}

} // end namespace itk

#endif
