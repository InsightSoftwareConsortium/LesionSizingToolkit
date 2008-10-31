/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkRegionGrowingSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRegionGrowingSegmentationModule_txx
#define __itkRegionGrowingSegmentationModule_txx

#include "itkRegionGrowingSegmentationModule.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
RegionGrowingSegmentationModule<NDimension>
::RegionGrowingSegmentationModule()
{
  this->SetNumberOfRequiredOutputs( 1 );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
RegionGrowingSegmentationModule<NDimension>
::~RegionGrowingSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
RegionGrowingSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
RegionGrowingSegmentationModule<NDimension>
::GenerateData()
{

}

} // end namespace itk

#endif
