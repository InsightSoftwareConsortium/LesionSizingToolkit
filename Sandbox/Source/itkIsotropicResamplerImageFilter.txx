/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkIsotropicResamplerImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkIsotropicResamplerImageFilter_txx
#define __itkIsotropicResamplerImageFilter_txx

#include "itkIsotropicResamplerImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk
{
  
template <class TInputImage, class TOutputImage>
IsotropicResamplerImageFilter<TInputImage, TOutputImage>::
IsotropicResamplerImageFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  this->SetNumberOfRequiredOutputs( 1 );

  this->m_OutputSpacing = 0.2;  // 0.2 mm
  this->m_DefaultPixelValue = 0.0;
}
  
template <class TInputImage, class TOutputImage>
IsotropicResamplerImageFilter<TInputImage, TOutputImage>::
~IsotropicResamplerImageFilter()
{
}
 
template< class TInputImage, class TOutputImage >
void
IsotropicResamplerImageFilter< TInputImage, TOutputImage >
::GenerateData()
{
  // Report progress.
  typename ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  const InputImageType * inputImage = this->GetInput();

  if( !inputImage )
    {
    itkExceptionMacro("Missing input image");
    }

  typedef itk::IdentityTransform< double, ImageDimension >  TransformType;

  typename TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();

  typedef itk::BSplineInterpolateImageFunction< InputImageType, double >  BSplineInterpolatorType;

  typename BSplineInterpolatorType::Pointer bsplineInterpolator = BSplineInterpolatorType::New();

#if ITK_VERSION_MAJOR > 3 || (ITK_VERSION_MAJOR == 3 && ITK_VERSION_MINOR >= 10)
  bsplineInterpolator->UseImageDirectionOn();
#endif
  bsplineInterpolator->SetSplineOrder( 3 );

  this->m_ResampleFilter->SetTransform( transform );
  this->m_ResampleFilter->SetInterpolator( bsplineInterpolator );
  this->m_ResampleFilter->SetDefaultPixelValue( this->m_DefaultPixelValue );

  const typename InputImageType::SpacingType & inputSpacing = inputImage->GetSpacing();

  typename InputImageType::SpacingType outputSpacing;


  outputSpacing[0] = this->m_OutputSpacing;
  outputSpacing[1] = this->m_OutputSpacing;
  outputSpacing[2] = this->m_OutputSpacing;

  this->m_ResampleFilter->SetOutputSpacing( outputSpacing );

  this->m_ResampleFilter->SetOutputOrigin( inputImage->GetOrigin() );
  this->m_ResampleFilter->SetOutputDirection( inputImage->GetDirection() );

  typedef typename InputImageType::SizeType   SizeType;

  SizeType inputSize = inputImage->GetLargestPossibleRegion().GetSize();
  
  typedef typename SizeType::SizeValueType SizeValueType;

  const double dx = inputSize[0] * inputSpacing[0] / outputSpacing[0];
  const double dy = inputSize[1] * inputSpacing[1] / outputSpacing[1];
  const double dz = inputSize[2] * inputSpacing[2] / outputSpacing[2];

  typename InputImageType::SizeType   finalSize;

  finalSize[0] = static_cast<SizeValueType>( dx );
  finalSize[1] = static_cast<SizeValueType>( dy );
  finalSize[2] = static_cast<SizeValueType>( dz );

  this->m_ResampleFilter->SetSize( finalSize );

  this->m_ResampleFilter->SetInput( inputImage );

  progress->RegisterInternalFilter( this->m_ResampleFilter, 1.0 );  

  this->m_ResampleFilter->Update();

  this->GraftOutput( this->m_ResampleFilter->GetOutput() );
}

template <class TInputImage, class TOutputImage>
void IsotropicResamplerImageFilter< TInputImage,TOutputImage >
::SetAbortGenerateData( bool abort )
{
  this->Superclass::SetAbortGenerateData(abort);
  this->m_ResampleFilter->SetAbortGenerateData(abort);
}


template <class TInputImage, class TOutputImage>
void 
IsotropicResamplerImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}//end of itk namespace
#endif

