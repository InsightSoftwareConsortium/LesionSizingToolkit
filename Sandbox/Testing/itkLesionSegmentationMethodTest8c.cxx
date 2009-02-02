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
#include "itkIsotropicResamplerImageFilter.h"
#include "itkImageSpatialObject.h"

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

  InputImageType::SpacingType inputSpacing = inputImage->GetSpacing();

  typedef itk::IsotropicResamplerImageFilter< 
    InputImageType, InputImageType >   ResampleFilterType;

  ResampleFilterType::Pointer  resampler = ResampleFilterType::New();
 
  double minSpacing = 
    (inputSpacing[0] < inputSpacing[1] ? inputSpacing[0] : inputSpacing[1]);
  minSpacing = (minSpacing < inputSpacing[2] ? minSpacing : inputSpacing[2]);

  resampler->SetOutputSpacing( minSpacing / 2.0 );

  resampler->SetDefaultPixelValue( -1024 );

  resampler->SetInput( inputImage );

  try 
    {
    resampler->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::LandmarksReader< Dimension >    LandmarksReaderType;
  
  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName( argv[1] );
  landmarksReader->Update();

  typedef itk::LandmarkSpatialObject< Dimension >   SeedSpatialObjectType;
  typedef SeedSpatialObjectType::PointListType      PointListType;

  const SeedSpatialObjectType * landmarks = landmarksReader->GetOutput();

  SegmentationMethodType::Pointer segmentationMethod = SegmentationMethodType::New();

  // Set the sigma for canny as the maximum inputSpacing
  double maxSpacing = 
    (inputSpacing[0] > inputSpacing[1] ? inputSpacing[0] : inputSpacing[1]);
  maxSpacing = (maxSpacing > inputSpacing[2] ? maxSpacing : inputSpacing[2]);

  segmentationMethod->SetCannySigma( maxSpacing );

  segmentationMethod->SetInput( resampler->GetOutput() );
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
