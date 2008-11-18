/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFeatureAggregator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFeatureAggregator_txx
#define __itkFeatureAggregator_txx

#include "itkFeatureAggregator.h"
#include "itkImageSpatialObject.h"
#include "itkImageRegionIterator.h"
#include "itkImageFileWriter.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
FeatureAggregator<NDimension>
::FeatureAggregator()
{
  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform

  typedef float                                                 OutputPixelType;
  typedef Image< OutputPixelType, NDimension >                  OutputImageType;
  typedef ImageSpatialObject< NDimension, OutputPixelType >     OutputSpatialObjectType;

  typename OutputSpatialObjectType::Pointer outputObject = OutputSpatialObjectType::New();

  this->ProcessObject::SetNthOutput( 0, outputObject.GetPointer() );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
FeatureAggregator<NDimension>
::~FeatureAggregator()
{
}


/**
 * Add a feature generator that will compute the Nth feature to be passed to
 * the segmentation module.
 */
template <unsigned int NDimension>
void
FeatureAggregator<NDimension>
::AddFeatureGenerator( FeatureGeneratorType * generator ) 
{
  this->m_FeatureGenerators.push_back( generator );
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
FeatureAggregator<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "Feature generators = ";
  
  FeatureGeneratorConstIterator gitr = this->m_FeatureGenerators.begin();
  FeatureGeneratorConstIterator gend = this->m_FeatureGenerators.end();

  while( gitr != gend )
    {
    os << indent << gitr->GetPointer() << std::endl;
    ++gitr;
    }

}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
FeatureAggregator<NDimension>
::GenerateData()
{
  this->UpdateAllFeatureGenerators();
  this->ConsolidateFeatures();
}


/**
 * Update feature generators
 */
template <unsigned int NDimension>
void
FeatureAggregator<NDimension>
::UpdateAllFeatureGenerators()
{
  FeatureGeneratorIterator gitr = this->m_FeatureGenerators.begin();
  FeatureGeneratorIterator gend = this->m_FeatureGenerators.end();

  while( gitr != gend )
    {
    (*gitr)->Update();
    ++gitr;
    }
}

 
template <unsigned int NDimension>
void
FeatureAggregator<NDimension>
::ConsolidateFeatures()
{
  //
  // Temporary implementation: compute the pixel-wise min of all the 
  // input feature images. MOVE THIS TO A DERIVED CLASS
  //
  typedef float                                                   FeaturePixelType;
  typedef Image< FeaturePixelType, NDimension >                   FeatureImageType;
  typedef ImageSpatialObject< NDimension, FeaturePixelType >      FeatureSpatialObjectType;

  const FeatureSpatialObjectType * firstFeatureObject =
    dynamic_cast< const FeatureSpatialObjectType * >( this->m_FeatureGenerators[0]->GetFeature() );

  const FeatureImageType * firstFeatureImage = firstFeatureObject->GetImage();

  typename FeatureImageType::Pointer consolidatedFeatureImage = FeatureImageType::New();

  consolidatedFeatureImage->CopyInformation( firstFeatureImage );
  consolidatedFeatureImage->SetRegions( firstFeatureImage->GetBufferedRegion() );
  consolidatedFeatureImage->Allocate();
  consolidatedFeatureImage->FillBuffer( NumericTraits< FeaturePixelType >::max() );

  const unsigned int numberOfFeatures = this->m_FeatureGenerators.size();

  for( unsigned int i = 0; i < numberOfFeatures; i++ )
    {
    const FeatureSpatialObjectType * featureObject =
      dynamic_cast< const FeatureSpatialObjectType * >( this->m_FeatureGenerators[i]->GetFeature() );

    const FeatureImageType * featureImage = featureObject->GetImage();

    typedef ImageRegionIterator< FeatureImageType >          FeatureIterator;
    typedef ImageRegionConstIterator< FeatureImageType >     FeatureConstIterator;

    FeatureIterator       dstitr( consolidatedFeatureImage, consolidatedFeatureImage->GetBufferedRegion() );
    FeatureConstIterator  srcitr( featureImage, featureImage->GetBufferedRegion() );

    dstitr.GoToBegin();
    srcitr.GoToBegin();
   
    while( !srcitr.IsAtEnd() )
      {
      if( dstitr.Get() > srcitr.Get() )
        {
        dstitr.Set( srcitr.Get() );
        }
      ++srcitr;
      ++dstitr;
      }
    }

  FeatureSpatialObjectType * outputObject =
    dynamic_cast< FeatureSpatialObjectType * >(this->ProcessObject::GetOutput(0));

  outputObject->SetImage( consolidatedFeatureImage );
   
}

} // end namespace itk

#endif
