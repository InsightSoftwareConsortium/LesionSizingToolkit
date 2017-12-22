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

int itkGrayscaleImageSegmentationVolumeEstimatorTest2( int argc, char * argv [] )
{
  if( argc < 6 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputSegmentationImage MethodID DatasetID ExpectedVolume ouputTextFile " << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef itk::GrayscaleImageSegmentationVolumeEstimator<Dimension>  VolumeEstimatorType;
  typedef VolumeEstimatorType::InputImageSpatialObjectType    InputSpatialObjectType;
  typedef VolumeEstimatorType::InputImageType                 InputImageType;
  typedef itk::ImageFileReader< InputImageType > InputImageReaderType;

  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();
  inputImageReader->SetFileName( argv[1] );

  try 
    {
    inputImageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();
  inputImage->DisconnectPipeline();

  InputSpatialObjectType::Pointer inputImageSpatialObject = InputSpatialObjectType::New();
  inputImageSpatialObject->SetImage( inputImage );

  VolumeEstimatorType::Pointer  volumeEstimator = VolumeEstimatorType::New();
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
  const double  expectedVolume = atof( argv[4] );
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

  return EXIT_SUCCESS;
}
