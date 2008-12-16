/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFastMarchingSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFastMarchingSegmentationModule_txx
#define __itkFastMarchingSegmentationModule_txx

#include "itkFastMarchingSegmentationModule.h"
#include "itkImageRegionIterator.h"
#include "itkFastMarchingImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkSigmoidImageFilter.h"
#include "itkImageFileWriter.h"

namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
FastMarchingSegmentationModule<NDimension>
::FastMarchingSegmentationModule()
{
  m_ThresholdOutput = false;
  m_StoppingValue = static_cast<double>( static_cast<OutputPixelType>( 
                      NumericTraits<OutputPixelType>::max() / 2.0 ) );
  m_GradientMagnitudeSigmoid = true;
  m_SigmoidAlpha = -1.0;
  m_SigmoidBeta  = 5.0;
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs( 1 );

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput( 0, outputObject.GetPointer() );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
FastMarchingSegmentationModule<NDimension>
::~FastMarchingSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
FastMarchingSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
FastMarchingSegmentationModule<NDimension>
::GenerateData()
{
  typedef FastMarchingImageFilter<
    FeatureImageType, OutputImageType >      FilterType;
  typedef BinaryThresholdImageFilter< 
          OutputImageType, OutputImageType > ThresholdFilterType;
  typedef GradientMagnitudeRecursiveGaussianImageFilter<
          FeatureImageType >  DerivativeFilterType;
  typedef SigmoidImageFilter< FeatureImageType,
                              FeatureImageType > SigmoidFilterType;
  typename DerivativeFilterType::Pointer derivativeFilter;
  typename SigmoidFilterType::Pointer    sigmoidFilter;
  typename FilterType::Pointer filter = FilterType::New();

  const FeatureImageType * featureImage = this->GetInternalFeatureImage();
  
  if (m_GradientMagnitudeSigmoid)
    {

    // Default sigma is the minimum image spacing.
    double minSpacing = NumericTraits<double>::max();
    for (unsigned int i=0; i< Dimension; i++)
      {
      minSpacing = vnl_math_min(minSpacing, featureImage->GetSpacing()[i]);
      }
    const double sigmaDefault = minSpacing;
      
    derivativeFilter = DerivativeFilterType::New();
    derivativeFilter->SetInput( featureImage );
    derivativeFilter->SetSigma( sigmaDefault );
    sigmoidFilter = SigmoidFilterType::New();
    sigmoidFilter->SetOutputMaximum(1.0);
    sigmoidFilter->SetOutputMinimum(0.0);
    sigmoidFilter->SetInput( derivativeFilter->GetOutput() );
    sigmoidFilter->SetAlpha(m_SigmoidAlpha);
    sigmoidFilter->SetBeta(m_SigmoidBeta);
    sigmoidFilter->Update();
    filter->SetInput( sigmoidFilter->GetOutput() );
    }
  else
    {
    filter->SetInput( featureImage );
    }

  filter->SetStoppingValue( this->m_StoppingValue );

  const InputSpatialObjectType * inputSeeds = this->GetInternalInputLandmarks();
  const unsigned int numberOfPoints = inputSeeds->GetNumberOfPoints();

  typedef typename InputSpatialObjectType::SpatialObjectPointType   SpatialObjectPointType;
  typedef typename SpatialObjectPointType::PointType                PointType;
  typedef typename InputSpatialObjectType::PointListType            PointListType;
  typedef typename FeatureImageType::IndexType                      IndexType;
  typedef typename FeatureImageType::IndexType                      IndexType;
  typedef typename FilterType::NodeContainer                        NodeContainer;
  typedef typename FilterType::NodeType                             NodeType;
  typename NodeContainer::Pointer trialPoints = NodeContainer::New();
  
  const PointListType & points = inputSeeds->GetPoints();

  IndexType index;

  for( unsigned int i=0; i < numberOfPoints; i++ )
    {
    featureImage->TransformPhysicalPointToIndex( points[i].GetPosition(), index );
  
    NodeType node;

    // By starting the FastMarching front at this value,
    // the zero set will end up being placed at distance
    // = value from the seeds. That can be seen as computing
    // a distance map from the seeds.
    node.SetValue( -this->m_DistanceFromSeeds );
    
    node.SetIndex( index );
    trialPoints->InsertElement( i, node );  
    }

  filter->SetTrialPoints( trialPoints );
  filter->Update();

  if (this->m_ThresholdOutput)
    {
    // Threshold the result with a time from the time map.
    typename ThresholdFilterType::Pointer threshold = ThresholdFilterType::New();
    threshold->SetInput( filter->GetOutput() );
    threshold->SetUpperThreshold( itk::NumericTraits<OutputPixelType>::Zero ); 
    threshold->SetLowerThreshold( itk::NumericTraits<OutputPixelType>::NonpositiveMin() ); 
    threshold->SetInsideValue( 1 );
    threshold->SetOutsideValue( 0 );
    threshold->Update();
    this->PackOutputImageInOutputSpatialObject( threshold->GetOutput() );
    }
  else 
    {
    this->PackOutputImageInOutputSpatialObject( filter->GetOutput() );
    }
}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input image from the input SpatialObject.
 */
template <unsigned int NDimension>
const typename FastMarchingSegmentationModule<NDimension>::InputSpatialObjectType *
FastMarchingSegmentationModule<NDimension>
::GetInternalInputLandmarks() const
{
  const InputSpatialObjectType * inputObject =
    dynamic_cast< const InputSpatialObjectType * >( this->GetInput() );

  return inputObject;
}


/**
 * This method is intended to be used only by the subclasses to extract the
 * input feature image from the input feature SpatialObject.
 */
template <unsigned int NDimension>
const typename FastMarchingSegmentationModule<NDimension>::FeatureImageType *
FastMarchingSegmentationModule<NDimension>
::GetInternalFeatureImage() const
{
  const FeatureSpatialObjectType * featureObject =
    dynamic_cast< const FeatureSpatialObjectType * >( this->GetFeature() );

  const FeatureImageType * featureImage = featureObject->GetImage();

  return featureImage;
}


/**
 * This method is intended to be used only by the subclasses to insert the
 * output image as cargo of the output spatial object.
 */
template <unsigned int NDimension>
void
FastMarchingSegmentationModule<NDimension>
::PackOutputImageInOutputSpatialObject( OutputImageType * image )
{
  typename OutputImageType::Pointer outputImage = image;
  outputImage->DisconnectPipeline();
  OutputSpatialObjectType * outputObject =
    dynamic_cast< OutputSpatialObjectType * >(this->ProcessObject::GetOutput(0));
  outputObject->SetImage( outputImage );
}

/**
 * This method is intended to be used only by this class. It should be called
 * from the PackOutputImageInOutputSpatialObject() method.
 */
template <unsigned int NDimension>
void
FastMarchingSegmentationModule<NDimension>
::ConvertIntensitiesToCenteredRange( OutputImageType * image )
{
  typedef ImageRegionIterator< OutputImageType > IteratorType;

  IteratorType itr( image, image->GetBufferedRegion() );
  
  itr.GoToBegin();

  //
  // Convert intensities to centered range
  //
  while( !itr.IsAtEnd() )
    {
    if( itr.Get() )
      {
      itr.Set( 4.0 );
      }
    else
      {
      itr.Set( -4.0 );
      }
    ++itr;
    }
}


} // end namespace itk

#endif
