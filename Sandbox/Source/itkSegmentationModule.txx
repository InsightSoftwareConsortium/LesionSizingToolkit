/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSegmentationModule_txx
#define __itkSegmentationModule_txx

#include "itkSegmentationModule.h"


namespace itk
{

/*
 * Constructor
 */
template <unsigned int NDimension>
SegmentationModule<NDimension>
::SegmentationModule()
{
  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform
}


/*
 * Destructor
 */
template <unsigned int NDimension>
SegmentationModule<NDimension>
::~SegmentationModule()
{
}

template <unsigned int NDimension>
void
SegmentationModule<NDimension>
::SetInput( const SpatialObjectType * spatialObject )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<SpatialObjectType *>( spatialObject ));
}

template <unsigned int NDimension>
const typename SegmentationModule<NDimension>::SpatialObjectType *
SegmentationModule<NDimension>
::GetFeature() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }

  return static_cast<const SpatialObjectType*>(this->ProcessObject::GetOutput(0));

}


/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
SegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
SegmentationModule<NDimension>
::GenerateData()
{

}

} // end namespace itk

#endif
