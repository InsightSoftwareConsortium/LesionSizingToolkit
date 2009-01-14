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


int main( int argc, char * argv [] )
{

  if( argc < 5 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputSegmentationImage MethodID DatasetID ouputTextFile " << std::endl;
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

  typedef VolumeEstimatorType::InputImageSpatialObjectType  InputImageSpatialObjectType;

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
  const std::string outpuFileName = argv[4];

  //
  // Append the value to the file containing estimations for this dataset.
  //
  std::ofstream ouputFile;

  ouputFile.open( outpuFileName.c_str(), std::ios_base::app );

  ouputFile << segmentationMethodID << "  ";
  ouputFile << datasetID << "  ";
  ouputFile << volume << "   ";
  ouputFile << radius << std::endl;

  ouputFile.close();

  return EXIT_SUCCESS;
}
