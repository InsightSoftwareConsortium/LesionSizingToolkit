/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkSegmentationVolumeEstimatorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkSegmentationVolumeEstimator.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageMaskSpatialObject.h"

namespace itk
{

class VolumeEstimatorSurrogate : public SegmentationVolumeEstimator<3>
{
public:
  /** Standard class typedefs. */
  typedef VolumeEstimatorSurrogate Self;
  typedef ProcessObject            Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  itkNewMacro( Self );

};

}


int itkSegmentationVolumeEstimatorTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  const unsigned int Dimension = 3;

  typedef itk::VolumeEstimatorSurrogate                   VolumeEstimatorType;

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
