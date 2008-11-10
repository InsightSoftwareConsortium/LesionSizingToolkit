/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest3.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLesionSegmentationMethod.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageMaskSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkConnectedThresholdSegmentationModule.h"

int main( int argc, char * argv [] )
{
  const unsigned int Dimension = 3;

  typedef itk::LesionSegmentationMethod< Dimension >   MethodType;

  MethodType::Pointer  lesionSegmentationMethod = MethodType::New();
  
  typedef itk::ImageMaskSpatialObject< Dimension > ImageMaskSpatialObjectType;

  ImageMaskSpatialObjectType::Pointer regionOfInterest = ImageMaskSpatialObjectType::New();

  lesionSegmentationMethod->SetRegionOfInterest( regionOfInterest );

  ImageMaskSpatialObjectType::Pointer initialSegmentation = ImageMaskSpatialObjectType::New();

  lesionSegmentationMethod->SetInitialSegmentation( initialSegmentation );

  typedef itk::SatoVesselnessSigmoidFeatureGenerator< Dimension > VesselnessGeneratorType;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  typedef itk::LungWallFeatureGenerator< Dimension > LungWallGeneratorType;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  lesionSegmentationMethod->AddFeatureGenerator( lungWallGenerator );
  lesionSegmentationMethod->AddFeatureGenerator( vesselnessGenerator );

  typedef itk::ConnectedThresholdSegmentationModule< Dimension >   SegmentationModuleType;
  
  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();

  lesionSegmentationMethod->SetSegmentationModule( segmentationModule );

  lesionSegmentationMethod->Update();

  lesionSegmentationMethod->Print( std::cout );

  
  return EXIT_SUCCESS;
}
