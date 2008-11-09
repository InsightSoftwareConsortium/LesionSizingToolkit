/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSatoVesselnessSigmoidFeatureGenerator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSatoVesselnessSigmoidFeatureGenerator_txx
#define __itkSatoVesselnessSigmoidFeatureGenerator_txx

#include "itkSatoVesselnessSigmoidFeatureGenerator.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
SatoVesselnessSigmoidFeatureGenerator<NDimension>
::SatoVesselnessSigmoidFeatureGenerator()
{
  this->m_SigmoidFilter = SigmoidFilterType::New();

  this->m_SigmoidAlpha =  -1.0;
  this->m_SigmoidBeta = 90.0;
}


/*
 * Destructor
 */
template <unsigned int NDimension>
SatoVesselnessSigmoidFeatureGenerator<NDimension>
::~SatoVesselnessSigmoidFeatureGenerator()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
SatoVesselnessSigmoidFeatureGenerator<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "Sigmoid Alpha " << this->m_SigmoidAlpha << std::endl;
  os << indent << "Sigmoid Beta " << this->m_SigmoidBeta << std::endl;
}


/*
 * Generate Data
 */
template <unsigned int NDimension>
void
SatoVesselnessSigmoidFeatureGenerator<NDimension>
::GenerateData()
{
  this->Superclass::GenerateData();

  //
  // Take the output of the superclass, and do further processing on it.
  // 
  typename OutputImageSpatialObjectType::Pointer outputObject =
    dynamic_cast<OutputImageSpatialObjectType * >( this->GetInternalFeature() );

  if( !outputObject )
    {
    itkExceptionMacro("Missing output feature object or incorrect type");
    }

  const OutputImageType * inputImage = outputObject->GetImage();

  if( !inputImage )
    {
    itkExceptionMacro("Missing output image object");
    }

  this->m_SigmoidFilter->SetInput( inputImage );

  this->m_SigmoidFilter->SetAlpha( this->m_SigmoidAlpha );
  this->m_SigmoidFilter->SetBeta( this->m_SigmoidBeta );

  this->m_SigmoidFilter->SetOutputMinimum( 0.0 );
  this->m_SigmoidFilter->SetOutputMaximum( 1.0 );

  this->m_SigmoidFilter->Update();

  typename OutputImageType::Pointer outputImage = this->m_SigmoidFilter->GetOutput();

  outputImage->DisconnectPipeline();

  outputObject->SetImage( outputImage );
}

} // end namespace itk

#endif
