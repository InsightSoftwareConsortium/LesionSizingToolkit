/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLesionSegmentationMethod.h"
#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageMaskSpatialObject.h"

int itkLesionSegmentationMethodTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  const unsigned int Dimension = 3;

  typedef itk::LesionSegmentationMethod< Dimension >   MethodType;

  MethodType::Pointer  segmentationMethod = MethodType::New();
  
  typedef itk::ImageMaskSpatialObject< Dimension > ImageMaskSpatialObjectType;

  ImageMaskSpatialObjectType::Pointer regionOfInterest = ImageMaskSpatialObjectType::New();

  segmentationMethod->SetRegionOfInterest( regionOfInterest );

  const MethodType::SpatialObjectType * regionOfInterestReturned =
    segmentationMethod->GetRegionOfInterest();

  if( regionOfInterestReturned != regionOfInterest.GetPointer() )
    {
    std::cerr << "Error in Set/GetRegionOfInterest() " << std::endl;
    return EXIT_FAILURE;
    }

  ImageMaskSpatialObjectType::Pointer initialSegmentation = ImageMaskSpatialObjectType::New();

  segmentationMethod->SetInitialSegmentation( initialSegmentation );
  
  const MethodType::SpatialObjectType * initialSegmentationReturned =
    segmentationMethod->GetInitialSegmentation();

  if( initialSegmentationReturned != initialSegmentation.GetPointer() )
    {
    std::cerr << "Error in Set/GetInitialSegmentation() " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::FeatureGenerator< Dimension > FeatureGeneratorType;

  FeatureGeneratorType::Pointer featureGenerator = FeatureGeneratorType::New();

  segmentationMethod->AddFeatureGenerator( featureGenerator );

  try
    {
    segmentationMethod->Update();
    std::cerr << "Failed to catch expected exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cout << "Caught expected exception" << std::endl;
    std::cout << excp << std::endl;
    }

  segmentationMethod->Print( std::cout );

  
  return EXIT_SUCCESS;
}
