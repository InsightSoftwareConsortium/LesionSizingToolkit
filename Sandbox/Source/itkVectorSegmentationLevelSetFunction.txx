/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorSegmentationLevelSetFunction.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorSegmentationLevelSetFunction_txx_
#define __itkVectorSegmentationLevelSetFunction_txx_

#include "itkVectorSegmentationLevelSetFunction.h"

namespace itk {
  
template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::SetSpeedImage( ImageType *s )
{ 
  m_SpeedImage = s;
  m_Interpolator->SetInputImage(m_SpeedImage);
}

template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::SetAdvectionImage( MatrixImageType *s )
{
  m_AdvectionImage = s;
  m_MatrixInterpolator->SetInputImage(m_AdvectionImage);
}
  
template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::ReverseExpansionDirection()
{
  this->SetPropagationWeight( -1.0 * this->GetPropagationWeight() );
  this->SetAdvectionWeight( -1.0 * this->GetAdvectionWeight() );
}

template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::Initialize(const RadiusType &r)
{
  Superclass::Initialize(r);  
}

template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::AllocateSpeedImage()
{
  m_SpeedImage->SetRequestedRegion(m_FeatureImage->GetRequestedRegion());
  m_SpeedImage->SetBufferedRegion(m_FeatureImage->GetBufferedRegion());
  m_SpeedImage->SetLargestPossibleRegion(m_FeatureImage->GetLargestPossibleRegion());
  m_SpeedImage->Allocate();
  m_Interpolator->SetInputImage(m_SpeedImage);
}

template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::AllocateAdvectionImage()
{
  m_AdvectionImage->SetRequestedRegion(m_FeatureImage->GetRequestedRegion());
  m_AdvectionImage->SetBufferedRegion(m_FeatureImage->GetBufferedRegion());
  m_AdvectionImage->SetLargestPossibleRegion(m_FeatureImage->GetLargestPossibleRegion());
  m_AdvectionImage->Allocate();
  m_MatrixInterpolator->SetInputImage(m_AdvectionImage);
}

template <class TImageType, class TFeatureImageType>
typename VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>::ScalarValueType
VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::PropagationSpeed(const NeighborhoodType &neighborhood,
                   const FloatOffsetType &offset, GlobalDataStruct *) const
{
  IndexType idx = neighborhood.GetIndex();

  ContinuousIndexType cdx;
  for (unsigned i = 0; i < ImageDimension; ++i)
    {
    cdx[i] = static_cast<double>(idx[i]) - offset[i];
    }

  if ( m_Interpolator->IsInsideBuffer(cdx) )
    {
    return (static_cast<ScalarValueType>(
              m_Interpolator->EvaluateAtContinuousIndex(cdx)));
    }
  else return ( static_cast<ScalarValueType>(m_SpeedImage->GetPixel(idx)) );
}

template <class TImageType, class TFeatureImageType>
typename VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>::MatrixType
VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::AdvectionField(const NeighborhoodType &neighborhood,
                 const FloatOffsetType &offset, GlobalDataStruct *)  const
{
  IndexType idx = neighborhood.GetIndex();
  ContinuousIndexType cdx;
  for (unsigned i = 0; i < ImageDimension; ++i)
    {
    cdx[i] = static_cast<double>(idx[i]) - offset[i];
    }
  if ( m_MatrixInterpolator->IsInsideBuffer(cdx) )
    {
    return ( m_MatrixCast(m_MatrixInterpolator->EvaluateAtContinuousIndex(cdx)));
    }
  //Just return the default else
    return ( m_AdvectionImage->GetPixel(idx) );
  
}

} // end namespace itk

#endif
