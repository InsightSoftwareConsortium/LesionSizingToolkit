/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLandmarksReader.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLandmarksReader_txx
#define __itkLandmarksReader_txx

#include "itkLandmarksReader.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
LandmarksReader<NDimension>
::LandmarksReader()
{
  this->SetNumberOfRequiredOutputs( 1 );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
LandmarksReader<NDimension>
::~LandmarksReader()
{
}

template <unsigned int NDimension>
const typename LandmarksReader<NDimension>::SpatialObjectType *
LandmarksReader<NDimension>
::GetOutput() const
{
  return static_cast<const SpatialObjectType*>(this->ProcessObject::GetOutput(0));
}

/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
LandmarksReader<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << this->m_FileName << std::endl;
}


} // end namespace itk

#endif
