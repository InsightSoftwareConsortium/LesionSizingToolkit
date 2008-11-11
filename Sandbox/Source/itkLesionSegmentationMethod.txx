/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationMethod.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLesionSegmentationMethod_txx
#define __itkLesionSegmentationMethod_txx

#include "itkLesionSegmentationMethod.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>
::LesionSegmentationMethod()
{
  this->SetNumberOfRequiredOutputs( 1 );  // for the Transform
}


/**
 * Destructor
 */
template <unsigned int NDimension>
LesionSegmentationMethod<NDimension>
::~LesionSegmentationMethod()
{
}


/**
 * Add a feature generator that will compute the Nth feature to be passed to
 * the segmentation module.
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::AddFeatureGenerator( FeatureGeneratorType * generator ) 
{
  this->m_FeatureGenerators.push_back( generator );
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << "Region of Interest " << this->m_RegionOfInterest.GetPointer() << std::endl;
  os << "Initial Segmentation " << this->m_InitialSegmentation.GetPointer() << std::endl;
  os << "Segmentation Module " << this->m_SegmentationModule.GetPointer() << std::endl;

  os << "Feature generators = ";
  
  FeatureGeneratorConstIterator gitr = this->m_FeatureGenerators.begin();
  FeatureGeneratorConstIterator gend = this->m_FeatureGenerators.end();

  while( gitr != gend )
    {
    os << gitr->GetPointer() << std::endl;
    ++gitr;
    }

}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::GenerateData()
{
  this->UpdateAllFeatureGenerators();

  if( this->FeaturesNeedToBeConsolidated() )
    {
    this->ConsolidateFeatures();
    }
  this->ExecuteSegmentationModule();
}


/**
 * Define the segmentation module to be used.
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::SetSegmentationModule( SegmentationModuleType * segmentor )
{
  this->m_SegmentationModule = segmentor;
}


/**
 * Update feature generators
 */
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
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
bool
LesionSegmentationMethod<NDimension>
::FeaturesNeedToBeConsolidated() const
{
  const unsigned int expectedNumberOfFeatures = this->m_SegmentationModule->GetExpectedNumberOfFeatures();

  if( expectedNumberOfFeatures < this->m_FeatureGenerators.size() )
    {
    return true;
    }

  return false;
}

 
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::ConsolidateFeatures()
{
  // Temporary implementation: use only feature #1.
  // Future implementation: compute pixel-wise MIN of all the features. 
  this->m_SegmentationModule->SetFeature( this->m_FeatureGenerators[0]->GetFeature() );
}

 
template <unsigned int NDimension>
void
LesionSegmentationMethod<NDimension>
::ExecuteSegmentationModule()
{
  this->m_SegmentationModule->SetInput( this->m_InitialSegmentation ); 
  this->m_SegmentationModule->Update();
}


} // end namespace itk

#endif
