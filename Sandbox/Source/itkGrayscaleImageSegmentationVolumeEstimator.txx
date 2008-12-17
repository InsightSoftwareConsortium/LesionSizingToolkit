/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkGrayscaleImageSegmentationVolumeEstimator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkGrayscaleImageSegmentationVolumeEstimator_txx
#define __itkGrayscaleImageSegmentationVolumeEstimator_txx

#include "itkGrayscaleImageSegmentationVolumeEstimator.h"
#include "itkImageSpatialObject.h"
#include "itkImageRegionIterator.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
GrayscaleImageSegmentationVolumeEstimator<NDimension>
::GrayscaleImageSegmentationVolumeEstimator()
{
}


/**
 * Destructor
 */
template <unsigned int NDimension>
GrayscaleImageSegmentationVolumeEstimator<NDimension>
::~GrayscaleImageSegmentationVolumeEstimator()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
GrayscaleImageSegmentationVolumeEstimator<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
GrayscaleImageSegmentationVolumeEstimator<NDimension>
::GenerateData()
{

}

} // end namespace itk

#endif
