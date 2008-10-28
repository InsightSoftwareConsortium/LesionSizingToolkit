/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationMethod.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "itkLesionSegmentationMethod.h"


namespace itk
{

/*
 * Constructor
 */
LesionSegmentationMethod
::LesionSegmentationMethod()
{
  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform
}


/*
 * Destructor
 */
LesionSegmentationMethod
::~LesionSegmentationMethod()
{
}


/*
 * PrintSelf
 */
void
LesionSegmentationMethod
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/*
 * Generate Data
 */
void
LesionSegmentationMethod
::GenerateData()
{

}

} // end namespace itk
