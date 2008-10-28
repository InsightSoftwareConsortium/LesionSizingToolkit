/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"

int main( int argc, char * argv [] )
{
  const unsigned int Dimension = 3;

  typedef itk::SegmentationModule< Dimension >   SegmentationModuleType;
  typedef SegmentationModuleType::SpatialObjectType    SpatialObjectType;

  SegmentationModuleType::Pointer  featureGenerator = SegmentationModuleType::New();
  
  typedef itk::ImageSpatialObject< Dimension > ImageSpatialObjectType;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  featureGenerator->SetInput( inputObject );

  featureGenerator->Update();

  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  featureGenerator->Print( std::cout );

  
  return EXIT_SUCCESS;
}
