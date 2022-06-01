/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkFeatureGeneratorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"

int
itkFeatureGeneratorTest1(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  constexpr unsigned int Dimension = 3;

  using FeatureGeneratorType = itk::FeatureGenerator<Dimension>;
  using SpatialObjectType = FeatureGeneratorType::SpatialObjectType;

  FeatureGeneratorType::Pointer featureGenerator = FeatureGeneratorType::New();

  using ImageSpatialObjectType = itk::ImageSpatialObject<Dimension>;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  featureGenerator->SetInput(inputObject);

  featureGenerator->Update();

  SpatialObjectType::ConstPointer feature = featureGenerator->GetFeature();

  featureGenerator->Print(std::cout);


  return EXIT_SUCCESS;
}
