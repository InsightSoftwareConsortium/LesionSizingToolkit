/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationMethod.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLesionSegmentationMethod_txx
#define __itkLesionSegmentationMethod_txx

#include "itkLesionSegmentationMethod.h"


namespace itk
{

/*
 * Constructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>
::LesionSegmentationMethod()
{
  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform
}


/*
 * Destructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>
::~LesionSegmentationMethod()
{
}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << "Region of Interest " << this->m_RegionOfInterest << std::endl;
  os << "Initial Segmentation " << this->m_InitialSegmentation << std::endl;
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::GenerateData()
{

}

} // end namespace itk

#endif
