/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    itkVectorFiniteDifferenceFunction.txx
Language:  C++
Date:      $Date$
Version:   $Revision$

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorFiniteDifferenceFunction_txx_
#define __itkVectorFiniteDifferenceFunction_txx_

#include "itkVectorFiniteDifferenceFunction.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"

namespace itk {

template< class TImageType >
typename VectorFiniteDifferenceFunction< TImageType >::TimeStepType
VectorFiniteDifferenceFunction<TImageType>
::ComputeGlobalTimeStep(void *GlobalData) const
{
  TimeStepType dt;

  dt = 0.1; // FIXME: revisit this with a smarter method.

  return dt;
}

template< class TImageType >
typename VectorFiniteDifferenceFunction< TImageType >::ScalarValueType
VectorFiniteDifferenceFunction< TImageType >
::ComputeUpdate(const NeighborhoodType &it, void *globalData,
                unsigned int component, const FloatOffsetType& offset ) const
{
  const ScalarValueType center_value  = it.GetCenterPixel()[component];

  // Return the combination of all the terms.
  return ( center_value ); // FIXME: replace this with a real computation.
}

template <class TImageType>
void
VectorFiniteDifferenceFunction<TImageType>::
PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif
