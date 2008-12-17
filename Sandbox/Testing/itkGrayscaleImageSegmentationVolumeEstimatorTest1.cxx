/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGrayscaleImageSegmentationVolumeEstimatorTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkGrayscaleImageSegmentationVolumeEstimator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"


int main( int argc, char * argv [] )
{
  const unsigned int Dimension = 3;

  typedef itk::GrayscaleImageSegmentationVolumeEstimator<Dimension>  VolumeEstimatorType;

  typedef VolumeEstimatorType::SpatialObjectType    SpatialObjectType;

  VolumeEstimatorType::Pointer  volumeEstimator = VolumeEstimatorType::New();
  
  typedef itk::ImageSpatialObject< Dimension > ImageSpatialObjectType;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  volumeEstimator->SetInput( inputObject );

  volumeEstimator->Update();

  VolumeEstimatorType::RealType volume1 = volumeEstimator->GetVolume();

  const VolumeEstimatorType::RealObjectType * volumeObject = volumeEstimator->GetVolumeOutput();

  if( volumeObject->Get() != volume1 )
    {
    std::cerr << "Error in GetVolumeOutput() and/or GetVolume() " << std::endl;
    return EXIT_FAILURE;
    }

  volumeEstimator->Print( std::cout );

  std::cout << "Class name = " << volumeEstimator->GetNameOfClass() << std::endl;
  
  return EXIT_SUCCESS;
}
