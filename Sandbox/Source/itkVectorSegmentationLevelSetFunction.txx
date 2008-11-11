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
#ifndef __itkVectorSegmentationLevelSetFunction_txx
#define __itkVectorSegmentationLevelSetFunction_txx

#include "itkVectorSegmentationLevelSetFunction.h"
#include "itkMeasurementVectorTraits.h"

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
::SetAdvectionImage( unsigned int component, VectorImageType *s )
{
  m_AdvectionImage[component] = s;
  m_VectorInterpolator->SetInputImage(this->m_AdvectionImage[component]);
}

template <class TImageType, class TFeatureImageType>
void VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::ReverseExpansionDirection()
{
  this->SetPropagationWeights( this->GetPropagationWeights() * (-1.0) );
  this->SetAdvectionWeights( this->GetAdvectionWeights() * (-1.0) );
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

  typedef ImageRegionConstIterator< FeatureImageType > FeatureIterator;
  FeatureIterator ftr( m_FeatureImage, m_FeatureImage->GetBufferedRegion() );
  ftr.GoToBegin();

  const unsigned int numberOfComponents = MeasurementVectorTraits::GetLength( ftr.Get() );

  this->m_AdvectionImage.resize( numberOfComponents );

  for( unsigned int component = 0; component < numberOfComponents; component++)
    {
    m_AdvectionImage[component] = VectorImageType::New();
    m_AdvectionImage[component]->SetRequestedRegion(m_FeatureImage->GetRequestedRegion());
    m_AdvectionImage[component]->SetBufferedRegion(m_FeatureImage->GetBufferedRegion());
    m_AdvectionImage[component]->SetLargestPossibleRegion(m_FeatureImage->GetLargestPossibleRegion());
    m_AdvectionImage[component]->Allocate();
    }
  m_VectorInterpolator->SetInputImage(m_AdvectionImage[0]);
}

template <class TImageType, class TFeatureImageType>
typename VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>::ScalarValueType
VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::PropagationSpeed(const NeighborhoodType &neighborhood,
                   const FloatOffsetType &offset, unsigned int component, GlobalDataStruct *) const
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
              m_Interpolator->EvaluateAtContinuousIndex(cdx)[component]));
    }

  return ( static_cast<ScalarValueType>( m_SpeedImage->GetPixel(idx)[component] ));
}


template <class TImageType, class TFeatureImageType>
typename VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>::ScalarValueType
VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::CurvatureSpeed(const NeighborhoodType &neighborhood,
                   const FloatOffsetType &offset, unsigned int component, GlobalDataStruct *) const
{
  IndexType idx = neighborhood.GetIndex();
  // FIXME : TO BE IMPLEMENTED
  return ( static_cast<ScalarValueType>( m_SpeedImage->GetPixel(idx)[component] ));
}


template <class TImageType, class TFeatureImageType>
typename VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>::VectorType
VectorSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::AdvectionField(const NeighborhoodType &neighborhood,
                 const FloatOffsetType &offset, unsigned int component, GlobalDataStruct *)  const
{
  IndexType idx = neighborhood.GetIndex();
  ContinuousIndexType cdx;
  for (unsigned i = 0; i < ImageDimension; ++i)
    {
    cdx[i] = static_cast<double>(idx[i]) - offset[i];
    }

  this->m_VectorInterpolator->SetInputImage( this->m_AdvectionImage[component]);

  if ( this->m_VectorInterpolator->IsInsideBuffer(cdx) )
    {
    return ( m_VectorCast(this->m_VectorInterpolator->EvaluateAtContinuousIndex(cdx)));
    }

  //Just return the default else
  return ( this->m_AdvectionImage[component]->GetPixel(idx) );

}

} // end namespace itk

#endif
