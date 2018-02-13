/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest2.cxx

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
#include "itkDescoteauxSheetnessFeatureGenerator.h"
#include "itkFrangiTubularnessFeatureGenerator.h"
#include "itkGradientMagnitudeSigmoidFeatureGenerator.h"
#include "itkSatoLocalStructureFeatureGenerator.h"
#include "itkSatoVesselnessFeatureGenerator.h"

int itkLesionSegmentationMethodTest2( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  constexpr unsigned int Dimension = 3;

  using MethodType = itk::LesionSegmentationMethod< Dimension >;

  MethodType::Pointer  segmentationMethod = MethodType::New();
  
  using ImageMaskSpatialObjectType = itk::ImageMaskSpatialObject< Dimension >;

  ImageMaskSpatialObjectType::Pointer regionOfInterest = ImageMaskSpatialObjectType::New();

  segmentationMethod->SetRegionOfInterest( regionOfInterest );

  ImageMaskSpatialObjectType::Pointer initialSegmentation = ImageMaskSpatialObjectType::New();

  segmentationMethod->SetInitialSegmentation( initialSegmentation );

  using TubularnessGeneratorType = itk::FrangiTubularnessFeatureGenerator< Dimension >;
  TubularnessGeneratorType::Pointer tubularnessGenerator = TubularnessGeneratorType::New();

  using SheetnessGeneratorType = itk::DescoteauxSheetnessFeatureGenerator< Dimension >;
  SheetnessGeneratorType::Pointer sheetnessGenerator = SheetnessGeneratorType::New();

  using VesselnessGeneratorType = itk::SatoVesselnessFeatureGenerator< Dimension >;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  using LocalStructureGeneratorType = itk::SatoLocalStructureFeatureGenerator< Dimension >;
  LocalStructureGeneratorType::Pointer localStructureGenerator = LocalStructureGeneratorType::New();

  using GradientMagnitudeSigmoidGeneratorType = itk::GradientMagnitudeSigmoidFeatureGenerator< Dimension >;
  GradientMagnitudeSigmoidGeneratorType::Pointer gradientMagnitudeSigmoidGenerator =
    GradientMagnitudeSigmoidGeneratorType::New();

  segmentationMethod->AddFeatureGenerator( tubularnessGenerator );
  segmentationMethod->AddFeatureGenerator( sheetnessGenerator );
  segmentationMethod->AddFeatureGenerator( vesselnessGenerator );
  segmentationMethod->AddFeatureGenerator( localStructureGenerator );
  segmentationMethod->AddFeatureGenerator( gradientMagnitudeSigmoidGenerator );

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
