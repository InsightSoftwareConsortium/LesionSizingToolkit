/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest8c.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// The test runs a shape detection level set from user supplied seed points
// and then runs the shape detection level set with the results from the 
// fast marching to get the final segmentation.

#include "itkLesionSegmentationImageFilter8.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLandmarksReader.h"
#include "itkIsotropicResampler.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Applies fast marhching followed by segmentation using geodesic active contours. Arguments" << std::endl;
    std::cerr << argv[0] << "\n\tlandmarksFile\n\tinputImage\n\toutputImage ";
    std::cerr << "\n\t[SigmoidBeta]" << std::endl;
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;
  typedef signed short   InputPixelType;
  typedef float          OutputPixelType;

  typedef itk::Image< InputPixelType,  Dimension > InputImageType;
  typedef itk::Image< OutputPixelType, Dimension > OutputImageType;

  typedef itk::LesionSegmentationImageFilter8< 
    InputImageType, OutputImageType > SegmentationMethodType;

  typedef itk::ImageFileReader< InputImageType > InputImageReaderType;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[2] );

  try 
    {
    inputImageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  const InputImageType * inputImage = inputImageReader->GetOutput();

  typedef itk::IsotropicResampler< Dimension >   ResampleFilterType;
  typedef ResampleFilterType::SpatialObjectType    SpatialObjectType;

  ResampleFilterType::Pointer  resampler = ResampleFilterType::New();
 
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  inputObject->SetImage( inputImage );

  resampler->SetInput( inputObject );

  try 
    {
    resampler->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  SpatialObjectType::ConstPointer resampledObject = resampler->GetOutput();


  typedef itk::LandmarksReader< Dimension >    LandmarksReaderType;
  
  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName( argv[1] );
  landmarksReader->Update();

  typedef itk::LandmarkSpatialObject< Dimension >   SeedSpatialObjectType;
  typedef SeedSpatialObjectType::PointListType      PointListType;

  const SeedSpatialObjectType * landmarks = landmarksReader->GetOutput();

  SegmentationMethodType::Pointer segmentationMethod = SegmentationMethodType::New();

  const InputImageType * resampledImage = resampledObject->GetITKImageBase();

  segmentationMethod->SetInput( resampledImage );
  segmentationMethod->SetSeeds( landmarks->GetPoints() );

  segmentationMethod->SetRegionOfInterest( inputImage->GetBufferedRegion() );

  if( argc > 4 )
    {
    const double sigmoidBeta = atof( argv[4] );
    std::cout << "Using SigmoidBeta = " << sigmoidBeta << std::endl;
    segmentationMethod->SetSigmoidBeta( sigmoidBeta );
    }

  typedef itk::ImageFileWriter< OutputImageType >      OutputWriterType;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( segmentationMethod->GetOutput() );
  writer->UseCompressionOn();

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
