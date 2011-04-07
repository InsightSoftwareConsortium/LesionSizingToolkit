/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorShiftScaleImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorShiftScaleImageFilter_txx
#define __itkVectorShiftScaleImageFilter_txx
#include "itkVectorShiftScaleImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkMeasurementVectorTraits.h"

namespace itk {

template<class TInputImage, class TOutputImage>
VectorShiftScaleImageFilter<TInputImage, TOutputImage>
::VectorShiftScaleImageFilter()
{
  m_Shift = NumericTraits<RealType>::Zero;
  m_Scale = NumericTraits<RealType>::One;
  m_UnderflowCount = 0;
  m_OverflowCount = 0;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
}

template<class TInputImage, class TOutputImage>
VectorShiftScaleImageFilter<TInputImage, TOutputImage>
::~VectorShiftScaleImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
VectorShiftScaleImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData ()
{
  unsigned int numberOfThreads = this->GetNumberOfThreads();

  //  Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(numberOfThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(numberOfThreads);
  m_ThreadOverflow.Fill(0);
}

template<class TInputImage, class TOutputImage>
void
VectorShiftScaleImageFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData ()
{
  unsigned int numberOfThreads = this->GetNumberOfThreads();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;

  // Accumulate counts for each thread
  for(unsigned int i = 0; i < numberOfThreads; i++)
    {
    m_UnderflowCount += m_ThreadUnderflow[i];
    m_OverflowCount += m_ThreadOverflow[i];
    }
}

template<class TInputImage, class TOutputImage>
void
VectorShiftScaleImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{

  RealType value;
  ImageRegionConstIterator<TInputImage>  it (this->GetInput(), outputRegionForThread);
  ImageRegionIterator<TOutputImage> ot (this->GetOutput(), outputRegionForThread);

  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  const unsigned int numberOfComponents = 
    Statistics::MeasurementVectorTraits::GetLength( it.Get() );

  // shift and scale the input pixels
  while (!it.IsAtEnd())
    {
    for(unsigned int i=0; i < numberOfComponents; i++)
      {
      value[i] = (static_cast<ScalarRealType>(it.Get()[i]) + m_Shift[i]) * m_Scale[i];
      if (value[i] < NumericTraits<ScalarOutputImagePixelType>::NonpositiveMin())
        {
        value[i] = NumericTraits<ScalarOutputImagePixelType>::NonpositiveMin();
        m_ThreadUnderflow[threadId]++;
        }
      else if (value[i] > NumericTraits<ScalarOutputImagePixelType>::max())
        {
        value[i] = NumericTraits<ScalarOutputImagePixelType>::max();
        m_ThreadOverflow[threadId]++;
        }
      }

    ot.Set( static_cast<OutputImagePixelType>( value ) );

    ++it;
    ++ot;

    progress.CompletedPixel();
    }
}

template <class TInputImage, class TOutputImage>
void
VectorShiftScaleImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Shift: "  << m_Shift << std::endl;
  os << indent << "Scale: "  << m_Scale << std::endl;
  os << indent << "Computed values follow:" << std::endl;
  os << indent << "UnderflowCount: "  << m_UnderflowCount << std::endl;
  os << indent << "OverflowCount: "  << m_OverflowCount << std::endl;
}


}// end namespace itk
#endif
