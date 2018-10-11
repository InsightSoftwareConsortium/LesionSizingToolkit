/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGrayscaleImageSegmentationVolumeEstimatorTest2.cxx

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
#include "itkImageFileReader.h"
#include "itksys/SystemTools.hxx"
#include "itkTestingMacros.h"


int itkGrayscaleImageSegmentationVolumeEstimatorTest2( int argc, char * argv [] )
{
  if( argc < 6 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputSegmentationImage MethodID DatasetID ExpectedVolume ouputTextFile " << std::endl;
    return EXIT_FAILURE;
    }

  constexpr unsigned int Dimension = 3;

  using VolumeEstimatorType = itk::GrayscaleImageSegmentationVolumeEstimator<Dimension>;
  using InputSpatialObjectType = VolumeEstimatorType::InputImageSpatialObjectType;
  using InputImageType = VolumeEstimatorType::InputImageType;
  using InputImageReaderType = itk::ImageFileReader< InputImageType >;

  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();
  inputImageReader->SetFileName( argv[1] );

  TRY_EXPECT_NO_EXCEPTION( inputImageReader->Update() );


  InputImageType::Pointer inputImage = inputImageReader->GetOutput();
  inputImage->DisconnectPipeline();

  InputSpatialObjectType::Pointer inputImageSpatialObject = InputSpatialObjectType::New();
  inputImageSpatialObject->SetImage( inputImage );

  VolumeEstimatorType::Pointer volumeEstimator = VolumeEstimatorType::New();

  EXERCISE_BASIC_OBJECT_METHODS( volumeEstimator,
    GrayscaleImageSegmentationVolumeEstimator, SegmentationVolumeEstimator );


  volumeEstimator->SetInput( inputImageSpatialObject );
  volumeEstimator->Update();

  const VolumeEstimatorType::RealType volume = volumeEstimator->GetVolume();

  //
  // Compute the radius of the equivalent-volume sphere
  //
  const double radius3 = ( ( volume * 3.0 ) / ( 4.0 * vnl_math::pi ) );
  const double radius = vnl_math_cuberoot( radius3 );


  const std::string segmentationMethodID = argv[2];
  const std::string datasetID = argv[3];
  const double expectedVolume = std::stod( argv[4] );
  const std::string outpuFileName = argv[5];

  const double volumeDifference = volume - expectedVolume;
  const double ratio = volume / expectedVolume;
  const double errorPercent = volumeDifference / expectedVolume * 100.0;

  //
  // Append the value to the file containing estimations for this dataset.
  //
  std::ofstream ouputFile;

  // Check if the file exists. If it does not, let's print out the axis labels
  // right at the top of the file.
  const bool fileExists = 
      itksys::SystemTools::FileExists( outpuFileName.c_str() );

  ouputFile.open( outpuFileName.c_str(), std::ios_base::app );

  if (!fileExists)
    {
    ouputFile << "SegmentationMethodID DatasetID ExpectedVolume ComputedVolume " 
               << "PercentError RatioOfComputedVolumeToExpectedVolume "
               << "ComputedRadius " << std::endl;
    }

  ouputFile << segmentationMethodID << "  ";
  ouputFile << datasetID << "  ";
  ouputFile << expectedVolume << "   ";
  ouputFile << volume << "   ";
  ouputFile << errorPercent << "   ";
  ouputFile << ratio << "   ";
  ouputFile << radius << std::endl;

  ouputFile.close();


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
