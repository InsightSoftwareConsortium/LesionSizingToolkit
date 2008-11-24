/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule_txx
#define __itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule_txx

#include "itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkGeodesicActiveContourLevelSetImageFilter.h"


namespace itk
{


/**
 * Constructor
 */
template <unsigned int NDimension>
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<NDimension>
::FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule()
{
  this->m_FastMarchingModule = FastMarchingModuleType::New();
  this->m_FastMarchingModule->SetDistanceFromSeeds(1.0);
  this->m_FastMarchingModule->SetThresholdOutput( false );
  this->m_FastMarchingModule->GradientMagnitudeSigmoidOn();
  this->m_GeodesicActiveContourLevelSetModule = GeodesicActiveContourLevelSetModuleType::New();
}


/**
 * Destructor
 */
template <unsigned int NDimension>
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<NDimension>
::~FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<NDimension>
::GenerateData()
{
  this->m_FastMarchingModule->SetInput( this->GetInput() );
  this->m_FastMarchingModule->SetFeature( this->GetFeature() );
  this->m_FastMarchingModule->Update();

  m_GeodesicActiveContourLevelSetModule->SetInput( m_FastMarchingModule->GetOutput() );
  m_GeodesicActiveContourLevelSetModule->SetFeature( this->GetFeature() );
  m_GeodesicActiveContourLevelSetModule->SetMaximumRMSError( this->GetMaximumRMSError() );
  m_GeodesicActiveContourLevelSetModule->SetMaximumNumberOfIterations( this->GetMaximumNumberOfIterations() );
  m_GeodesicActiveContourLevelSetModule->SetPropagationScaling( this->GetPropagationScaling() );
  m_GeodesicActiveContourLevelSetModule->SetCurvatureScaling( this->GetCurvatureScaling() );
  m_GeodesicActiveContourLevelSetModule->Update();

  this->PackOutputImageInOutputSpatialObject( const_cast< OutputImageType * >(
        dynamic_cast< const OutputSpatialObjectType * >(
        m_GeodesicActiveContourLevelSetModule->GetOutput())->GetImage()) );
}

} // end namespace itk

#endif
