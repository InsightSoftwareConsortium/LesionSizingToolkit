/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorSegmentationLevelSetImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorSegmentationLevelSetImageFilter_txx
#define __itkVectorSegmentationLevelSetImageFilter_txx

#include "itkVectorSegmentationLevelSetImageFilter.h"
#include "vnl/vnl_matrix.h"

namespace itk {

template <class TInputImage, class TFeatureImage, class TOutputImage>
void
VectorSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "m_ReverseExpansionDirection = " << m_ReverseExpansionDirection << std::endl;
  os << indent << "m_AutoGenerateSpeedAdvection = " << m_AutoGenerateSpeedAdvection << std::endl;
  os << indent << "m_SegmentationFunction = " << m_SegmentationFunction << std::endl;
}

template <class TInputImage, class TFeatureImage, class TOutputImage>
VectorSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputImage>
::VectorSegmentationLevelSetImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfLayers(TInputImage::ImageDimension);
  m_SegmentationFunction = 0;
  m_AutoGenerateSpeedAdvection = true;
  this->SetIsoSurfaceValue(NumericTraits<ValueType>::Zero);
  
  // Provide some reasonable defaults which will at least prevent infinite
  // looping.
  this->SetMaximumRMSError(0.02);
  this->SetNumberOfIterations(1000);
  m_ReverseExpansionDirection = false;
}

template <class TInputImage, class TFeatureImage, class TOutputImage>
void
VectorSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputImage>
::GenerateSpeedImage()
{
  m_SegmentationFunction->AllocateSpeedImage();
  m_SegmentationFunction->CalculateSpeedImage();
}

template <class TInputImage, class TFeatureImage, class TOutputImage>
void
VectorSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputImage>
::GenerateAdvectionImage()
{
  m_SegmentationFunction->AllocateAdvectionImage();
  m_SegmentationFunction->CalculateAdvectionImage();
}

template <class TInputImage, class TFeatureImage, class TOutputImage>
void
VectorSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputImage>
::GenerateData()
{

  // A positive speed value causes surface expansion, the opposite of the
  // default.  Flip the sign of the propagation and advection weights.
  if (m_ReverseExpansionDirection == true)
    {
    this->GetSegmentationFunction()->ReverseExpansionDirection();
    }
  
  // Allocate the images from which speeds will be sampled.
  if (this->GetState() == Superclass::UNINITIALIZED && m_AutoGenerateSpeedAdvection == true)
    {
    // Generate the speed image of any of the propagation weights are non-zero
    if (!this->GetSegmentationFunction()->GetPropagationWeights().GetVnlMatrix().is_zero())
      {
      this->GenerateSpeedImage();
      }
    
    // Generate the speed image of any of the advection weights are non-zero
    if (!this->GetSegmentationFunction()->GetAdvectionWeights().GetVnlMatrix().is_zero())
      {
      this->GenerateAdvectionImage();
      }
    }
  
  // Start the solver
  Superclass::GenerateData();
  
  // Reset all the signs of the weights.
  if (m_ReverseExpansionDirection == true)
    {
    this->GetSegmentationFunction()->ReverseExpansionDirection();
    }  
}

} // end namespace itk

#endif
