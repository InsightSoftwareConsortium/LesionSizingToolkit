/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"

int main( int argc, char * argv [] )
{
  const unsigned int Dimension = 3;

  typedef itk::GeodesicActiveContourLevelSetSegmentationModule< Dimension >   SegmentationModuleType;
  typedef SegmentationModuleType::SpatialObjectType    SpatialObjectType;

  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();
  
  typedef itk::ImageSpatialObject< Dimension > ImageSpatialObjectType;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  segmentationModule->SetInput( inputObject );

  ImageSpatialObjectType::Pointer featureObject = ImageSpatialObjectType::New();

  segmentationModule->SetFeature( featureObject );


  segmentationModule->Update();

  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  segmentationModule->Print( std::cout );

  std::cout << "Class name = " << segmentationModule->GetNameOfClass() << std::endl;
  
  return EXIT_SUCCESS;
}
