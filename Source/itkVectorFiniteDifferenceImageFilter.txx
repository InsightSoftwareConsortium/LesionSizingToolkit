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
#ifndef __itkVectorFiniteDifferenceImageFilter_txx
#define __itkVectorFiniteDifferenceImageFilter_txx

#include "itkVectorFiniteDifferenceImageFilter.h"
#include "itkVectorLevelSetFunction.h"
#include "itkMeasurementVectorTraits.h"

namespace itk {


template<class TInputImage, class TOutputImage>
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::VectorFiniteDifferenceImageFilter()
{
  this->SetDifferenceFunction( VectorLevelSetFunction<TInputImage>::New() );
}

template<class TInputImage, class TOutputImage>
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::~VectorFiniteDifferenceImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
VectorFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Test whether the output pixel type (or its components) are not of type
  // float or double:
  if( NumericTraits< OutputPixelValueType >::is_integer )
    {
    itkWarningMacro("Output pixel type MUST be float or double to prevent computational errors");
    }

  if (this->GetState() == Superclass::UNINITIALIZED)
    {

    // Allocate the output image
    this->AllocateOutputs();

    // Copy the input image to the output image.  Algorithms will operate
    // directly on the output image and the update buffer.
    this->CopyInputToOutput();

    // Find the number of components per pixel
    //
    typedef ImageRegionConstIterator< OutputImageType > OutputImageIteratorType;
    const OutputImageType * outputImage = this->GetOutput();
    OutputImageIteratorType otr( outputImage, outputImage->GetBufferedRegion() );
    otr.GoToBegin();
    this->m_NumberOfComponents = Statistics::MeasurementVectorTraits::GetLength( otr.Get() );
    std::cout << "NUMBER OF COMPONENTS = " << this->m_NumberOfComponents << std::endl;

    // Set the coefficients for the deriviatives on each of the finite
    // difference functions
    //
    double coeffs[TInputImage::ImageDimension];
    for (unsigned int i = 0; i < TInputImage::ImageDimension; i++)
      {
      coeffs[i] = (this->GetUseImageSpacing() ?
          1.0 / this->GetInput()->GetSpacing()[i] : 1.0);
      }

    if (this->m_DifferenceFunctions.size() != this->m_NumberOfComponents)
      {
      itkExceptionMacro( << "Number of difference functions must match the "
          << "number of components in the image." );
      }

    for (unsigned int i = 0; i < m_NumberOfComponents; i++)
      {
      if (m_DifferenceFunctions[i])
        {
        this->m_DifferenceFunctions[i]->SetScaleCoefficients(coeffs);
        }
      }


    // Perform any other necessary pre-iteration initialization.
    this->Initialize();

    // Allocate the internal update buffer.  This takes place entirely within
    // the subclass, since this class cannot define an update buffer type.
    this->AllocateUpdateBuffer();

    this->SetStateToInitialized();
    this->m_ElapsedIterations = 0;
    }

  // Iterative algorithm
  TimeStepType dt;

  while ( ! this->Halt() )
    {
    this->InitializeIteration(); // An optional method for precalculating
                                 // global values, or otherwise setting up
                                 // for the next iteration
    dt = this->CalculateChange();
    this->ApplyUpdate(dt);
    ++this->m_ElapsedIterations;

    // Invoke the iteration event.
    this->InvokeEvent( IterationEvent() );
    if( this->GetAbortGenerateData() )
      {
      this->InvokeEvent( IterationEvent() );
      this->ResetPipeline();
      throw ProcessAborted(__FILE__,__LINE__);
      }
    }

  if (this->m_ManualReinitialization == false)
    {
    this->SetStateToUninitialized(); // Reset the state once execution is
                                     // completed
    }

  // Any further processing of the solution can be done here.
  this->PostProcessOutput();
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
