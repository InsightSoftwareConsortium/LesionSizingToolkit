/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGeodesicActiveContourLevelSetSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGeodesicActiveContourLevelSetSegmentationModule_txx
#define __itkGeodesicActiveContourLevelSetSegmentationModule_txx

#include "itkGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkGeodesicActiveContourLevelSetImageFilter.h"


namespace itk
{


/**
 * Constructor
 */
template <unsigned int NDimension>
GeodesicActiveContourLevelSetSegmentationModule<NDimension>
::GeodesicActiveContourLevelSetSegmentationModule()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs( 1 );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
GeodesicActiveContourLevelSetSegmentationModule<NDimension>
::~GeodesicActiveContourLevelSetSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
GeodesicActiveContourLevelSetSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
GeodesicActiveContourLevelSetSegmentationModule<NDimension>
::GenerateData()
{
  const InputSpatialObjectType * inputObject =
    dynamic_cast< const InputSpatialObjectType * >( this->GetInput() );

  const FeatureSpatialObjectType * featureObject =
    dynamic_cast< const FeatureSpatialObjectType * >( this->GetFeature() );


  const InputImageType * inputImage = inputObject->GetImage();
  const FeatureImageType * featureImage = featureObject->GetImage();

  typedef itk::GeodesicActiveContourLevelSetImageFilter<
    InputImageType, FeatureImageType, OutputPixelType > FilterType;

  typedef itk::SpatialObject< NDimension >        SpatialObjectType;

  typename FilterType::Pointer filter = FilterType::New();

  filter->SetInput( inputImage );
  filter->SetFeatureImage( featureImage );

  filter->Update();

  typename OutputImageType::Pointer outputImage = filter->GetOutput();

  outputImage->DisconnectPipeline();

  OutputSpatialObjectType * outputObject =
    dynamic_cast< OutputSpatialObjectType * >(this->ProcessObject::GetOutput(0));

  outputObject->SetImage( outputImage );
}

} // end namespace itk

#endif
