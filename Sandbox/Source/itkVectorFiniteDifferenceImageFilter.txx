/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorFiniteDifferenceImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorFiniteDifferenceImageFilter_txx_
#define __itkVectorFiniteDifferenceImageFilter_txx_

#include "itkVectorFiniteDifferenceImageFilter.h"
#include "itkVectorFiniteDifferenceFunction.h"

namespace itk {


template<class TInputImage, class TOutputImage>
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::VectorFiniteDifferenceImageFilter()
{
  this->SetDifferenceFunction( VectorFiniteDifferenceFunction<TInputImage>::New() );
}

template<class TInputImage, class TOutputImage>
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::~VectorFiniteDifferenceImageFilter()
{
}


template<class TInputImage, class TOutputImage>
void
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::AllocateUpdateBuffer()
{
}


template<class TInputImage, class TOutputImage>
void
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::ApplyUpdate(TimeStepType dt)
{
}


template<class TInputImage, class TOutputImage>
typename VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>::TimeStepType 
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::CalculateChange()
{
  TimeStepType step = 1.0;
  return step;
}


template<class TInputImage, class TOutputImage>
void
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::CopyInputToOutput()
{
}



template<class TInputImage, class TOutputImage>
void
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace itk

#endif
