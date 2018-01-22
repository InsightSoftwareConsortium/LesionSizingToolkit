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
#include "itkImageFileWriter.h"


int itkGrayscaleImageSegmentationVolumeEstimatorTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  const unsigned int Dimension = 3;

  typedef itk::GrayscaleImageSegmentationVolumeEstimator<Dimension>  VolumeEstimatorType;

  VolumeEstimatorType::Pointer  volumeEstimator = VolumeEstimatorType::New();
  
  typedef itk::ImageSpatialObject< Dimension > ImageSpatialObjectType;

  ImageSpatialObjectType::Pointer inputObject = ImageSpatialObjectType::New();

  volumeEstimator->SetInput( inputObject );

  try
    {
    volumeEstimator->Update();
    std::cerr << "Failed to catch expected exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cout << "Caught expected exception" << std::endl;
    std::cout << excp << std::endl;
    }

  typedef VolumeEstimatorType::InputImageType   InputImageType;

  InputImageType::Pointer image = InputImageType::New();

  typedef VolumeEstimatorType::InputImageSpatialObjectType  InputImageSpatialObjectType;

  InputImageSpatialObjectType::Pointer inputImageSpatialObject = InputImageSpatialObjectType::New();

  volumeEstimator->SetInput( inputImageSpatialObject );

  inputImageSpatialObject->SetImage( image );
 
  InputImageType::SpacingType spacing;

  spacing[0] = 0.5;
  spacing[1] = 0.5;
  spacing[2] = 0.8;
  
  image->SetSpacing( spacing );


  InputImageType::SizeType  size;
  size[0] = 101;
  size[1] = 101;
  size[2] = 101;

  InputImageType::RegionType region;
  region.SetSize( size );

  image->SetRegions( region );
  image->Allocate();

  typedef itk::ImageRegionIterator< InputImageType > IteratorType;

  IteratorType itr( image, region );

  itr.GoToBegin();

  InputImageType::IndexType index;
  InputImageType::PointType point;

  //
  // Populate the image with a sphere 
  //
  InputImageType::PointType center;

  center[0] = 50.0 * spacing[0];
  center[1] = 50.0 * spacing[1];
  center[2] = 50.0 * spacing[2];

  const double radius = 15.0;

  while( !itr.IsAtEnd() )
    {
    index = itr.GetIndex();
    image->TransformIndexToPhysicalPoint( index, point );
    const double distance = point.EuclideanDistanceTo( center ); 

    if( distance > radius )
      {
      itr.Set( -4.0 );
      }
    else
      {
      itr.Set( 4.0 );
      }

    ++itr;
    }

  volumeEstimator->Update();

  VolumeEstimatorType::RealType volume1 = volumeEstimator->GetVolume();

  const VolumeEstimatorType::RealObjectType * volumeObject = volumeEstimator->GetVolumeOutput();

  if( volumeObject->Get() != volume1 )
    {
    std::cerr << "Error in GetVolumeOutput() and/or GetVolume() " << std::endl;
    return EXIT_FAILURE;
    }

  volumeEstimator->Print( std::cout );

  typedef itk::ImageFileWriter< InputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( image );
  writer->SetFileName("sphereForVolumeTest.mhd");
  writer->Update();

  std::cout << "Class name = " << volumeEstimator->GetNameOfClass() << std::endl;
 
  const double pi = atan(1.0) * 4.0;

  const double expectedVolume = ( 4.0 / 3.0 ) * pi * ( radius * radius * radius );

  const double difference = volume1 - expectedVolume;

  const double percentage = 100.0 * vnl_math_abs( difference ) / expectedVolume;

  std::cout << "Expected  Volume = " << expectedVolume << std::endl;
  std::cout << "Estimated Volume = " << volume1 << std::endl;
  std::cout << "Difference       = " << difference << std::endl;
  std::cout << "Percentage       = " << percentage << "%" << std::endl;

  if( percentage > 0.1 )  // This is: 0.1%
    {
    std::cout << "Error too large. Test FAILED!" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
