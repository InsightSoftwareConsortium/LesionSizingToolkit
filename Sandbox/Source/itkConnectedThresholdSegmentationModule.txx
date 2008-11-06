/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConnectedThresholdSegmentationModule.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConnectedThresholdSegmentationModule_txx
#define __itkConnectedThresholdSegmentationModule_txx

#include "itkConnectedThresholdSegmentationModule.h"


namespace itk
{

template < class TPixel, unsigned int NDimension, class TOutputImage >
class ConnectedThresholdIntantiator
{
public:
  typedef itk::Image< TPixel, NDimension >        InputImageType;
  typedef itk::ConnectedThresholdImageFilter<
    InputImageType, TOutputImage >                FilterType;

  typedef itk::SpatialObject< NDimension >        SpatialObjectType;

  typedef TOutputImage                            OutputImageType;

  static void Execute(const SpatialObjectType * input, 
  const SpatialObjectType * feature, SpatialObjectType * output )
    {
    typename FilterType::Pointer filter = FilterType::New();
    filter->Update();
    typename OutputImageType::Pointer outputImage = filter->GetOutput();
    outputImage->DisconnectPipeline();
    }

};

#define InstantiatorMacro( pixeltype ) \
  ConnectedThresholdIntantiator< pixeltype, NDimension, OutputImageType >::Execute( \
  this->GetInput(), this->GetFeature(), this->GetInternalOutput() );


/**
 * Constructor
 */
template <unsigned int NDimension>
ConnectedThresholdSegmentationModule<NDimension>
::ConnectedThresholdSegmentationModule()
{
  this->SetNumberOfRequiredInputs( 2 );
  this->SetNumberOfRequiredOutputs( 1 );
}


/**
 * Destructor
 */
template <unsigned int NDimension>
ConnectedThresholdSegmentationModule<NDimension>
::~ConnectedThresholdSegmentationModule()
{
}


/**
 * PrintSelf
 */
template <unsigned int NDimension>
void
ConnectedThresholdSegmentationModule<NDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
}


/**
 * Generate Data
 */
template <unsigned int NDimension>
void
ConnectedThresholdSegmentationModule<NDimension>
::GenerateData()
{
  InstantiatorMacro( unsigned char );
}

} // end namespace itk

#endif
