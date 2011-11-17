/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSinglePhaseLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSinglePhaseLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"

int itkSinglePhaseLevelSetSegmentationModuleTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  const unsigned int Dimension = 3;

  typedef itk::SinglePhaseLevelSetSegmentationModule< Dimension >   SegmentationModuleType;
  typedef SegmentationModuleType::SpatialObjectType    SpatialObjectType;

  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();
  
  typedef itk::ImageSpatialObject< Dimension > ImageSpatialObjectType;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  segmentationModule->SetInput( inputObject );

  ImageSpatialObjectType::Pointer featureObject = ImageSpatialObjectType::New();

  segmentationModule->SetFeature( featureObject );

  // Initialize values to a base level
  segmentationModule->SetPropagationScaling ( 1.0 );
  segmentationModule->SetCurvatureScaling ( 1.0 );
  segmentationModule->SetAdvectionScaling ( 1.0 );
  segmentationModule->SetMaximumRMSError ( 1.0 );
  segmentationModule->SetMaximumNumberOfIterations ( 1 );

  const double propagationScaling = 1.3;
  const double curvatureScaling = 1.7;
  const double advectionScaling = 1.9;
  const double maximumRMSError = 0.01;
  const unsigned int maximumNumberOfIterations = 179;

  // Set specific values and check them back
  segmentationModule->SetPropagationScaling ( propagationScaling );
  if( segmentationModule->GetPropagationScaling() != propagationScaling )
    {
    std::cerr << "Error in Set/GetPropagationScaling()" << std::endl;
    return EXIT_FAILURE;
    }

  segmentationModule->SetCurvatureScaling ( curvatureScaling );
  if( segmentationModule->GetCurvatureScaling() != curvatureScaling )
    {
    std::cerr << "Error in Set/GetCurvatureScaling()" << std::endl;
    return EXIT_FAILURE;
    }

  segmentationModule->SetAdvectionScaling ( advectionScaling );
  if( segmentationModule->GetAdvectionScaling() != advectionScaling )
    {
    std::cerr << "Error in Set/GetAdvectionScaling()" << std::endl;
    return EXIT_FAILURE;
    }

  segmentationModule->SetMaximumRMSError ( maximumRMSError );
  if( segmentationModule->GetMaximumRMSError() != maximumRMSError )
    {
    std::cerr << "Error in Set/GetMaximumRMSError()" << std::endl;
    return EXIT_FAILURE;
    }

  segmentationModule->SetMaximumNumberOfIterations ( maximumNumberOfIterations );
  if( segmentationModule->GetMaximumNumberOfIterations() != maximumNumberOfIterations )
    {
    std::cerr << "Error in Set/GetMaximumNumberOfIterations()" << std::endl;
    return EXIT_FAILURE;
    }


  segmentationModule->Update();

  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  segmentationModule->Print( std::cout );

  std::cout << "Class name = " << segmentationModule->GetNameOfClass() << std::endl;
  
  return EXIT_SUCCESS;
}
