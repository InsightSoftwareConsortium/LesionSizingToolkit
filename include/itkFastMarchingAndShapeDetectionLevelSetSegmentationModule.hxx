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
#ifndef itkFastMarchingAndShapeDetectionLevelSetSegmentationModule_hxx
#define itkFastMarchingAndShapeDetectionLevelSetSegmentationModule_hxx

#include "itkShapeDetectionLevelSetImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk
{


/**
 * Constructor
 */
template <unsigned int NDimension>
FastMarchingAndShapeDetectionLevelSetSegmentationModule<
  NDimension>::FastMarchingAndShapeDetectionLevelSetSegmentationModule()
{
  this->m_FastMarchingModule = FastMarchingModuleType::New();
  this->m_FastMarchingModule->SetDistanceFromSeeds(1.0);
  this->m_FastMarchingModule->SetStoppingValue(100.0);
  this->m_FastMarchingModule->InvertOutputIntensitiesOff();
  this->m_ShapeDetectionLevelSetModule = ShapeDetectionLevelSetModuleType::New();
  this->m_ShapeDetectionLevelSetModule->InvertOutputIntensitiesOff();
}


/**
 * Destructor
 */
template <unsigned int NDimension>
FastMarchingAndShapeDetectionLevelSetSegmentationModule<
  NDimension>::~FastMarchingAndShapeDetectionLevelSetSegmentationModule() = default;


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
FastMarchingAndShapeDetectionLevelSetSegmentationModule<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
FastMarchingAndShapeDetectionLevelSetSegmentationModule<NDimension>::GenerateData()
{
  // Report progress.
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);
  progress->RegisterInternalFilter(this->m_FastMarchingModule, 0.3);
  progress->RegisterInternalFilter(this->m_ShapeDetectionLevelSetModule, 0.7);

  this->m_FastMarchingModule->SetInput(this->GetInput());
  this->m_FastMarchingModule->SetFeature(this->GetFeature());
  this->m_FastMarchingModule->Update();

  m_ShapeDetectionLevelSetModule->SetInput(m_FastMarchingModule->GetOutput());
  m_ShapeDetectionLevelSetModule->SetFeature(this->GetFeature());
  m_ShapeDetectionLevelSetModule->SetMaximumRMSError(this->GetMaximumRMSError());
  m_ShapeDetectionLevelSetModule->SetMaximumNumberOfIterations(this->GetMaximumNumberOfIterations());
  m_ShapeDetectionLevelSetModule->SetPropagationScaling(this->GetPropagationScaling());
  m_ShapeDetectionLevelSetModule->SetCurvatureScaling(this->GetCurvatureScaling());
  m_ShapeDetectionLevelSetModule->Update();

  this->PackOutputImageInOutputSpatialObject(const_cast<OutputImageType *>(
    dynamic_cast<const OutputSpatialObjectType *>(m_ShapeDetectionLevelSetModule->GetOutput())->GetImage()));
}

} // end namespace itk

#endif
