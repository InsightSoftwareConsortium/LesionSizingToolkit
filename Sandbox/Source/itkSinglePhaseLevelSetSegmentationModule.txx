/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSinglePhaseLevelSetSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSinglePhaseLevelSetSegmentationModule_txx
#define __itkSinglePhaseLevelSetSegmentationModule_txx

#include "itkSinglePhaseLevelSetSegmentationModule.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SinglePhaseLevelSetSegmentationModule<NDimension>
::SinglePhaseLevelSetSegmentationModule()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs( 1 );

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput( 0, outputObject.GetPointer() );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
SinglePhaseLevelSetSegmentationModule<NDimension>
::~SinglePhaseLevelSetSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
SinglePhaseLevelSetSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
SinglePhaseLevelSetSegmentationModule<NDimension>
::GenerateData()
{

}

} // end namespace itk

#endif
