/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest8b.cxx

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

int itkLesionSegmentationMethodTest8b( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Applies fast marhching followed by segmentation using geodesic active contours. Arguments" << std::endl;
    std::cerr << argv[0] << "\n\tlandmarksFile\n\tinputImage\n\toutputImage ";
    std::cerr << "\n\t[SigmoidBeta] [-ResampleThickSliceData] [-UseVesselEnhancingDiffusion]" << std::endl;
    return EXIT_FAILURE;
    }

  bool useVesselEnhancingDiffusion = false, resampleThickSliceData = false;
  for (int i = 1; i < argc; i++)
    {
    useVesselEnhancingDiffusion |= (strcmp("-UseVesselEnhancingDiffusion", argv[i]) == 0);
    resampleThickSliceData |= (strcmp("-ResampleThickSliceData", argv[i]) == 0);
    }

  const unsigned int Dimension = 3;
  typedef signed short                              InputPixelType;
  typedef float                                     OutputPixelType;
  typedef itk::Image< InputPixelType,  Dimension >  InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >  OutputImageType;
  typedef itk::LesionSegmentationImageFilter8< 
          InputImageType, OutputImageType >         SegmentationMethodType;
  typedef itk::ImageFileReader< InputImageType >    InputImageReaderType;
  typedef itk::LandmarksReader< Dimension >         LandmarksReaderType;
  typedef itk::LandmarkSpatialObject< Dimension >   SeedSpatialObjectType;
  typedef itk::ImageFileWriter< OutputImageType >   OutputWriterType;

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

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();
  landmarksReader->SetFileName( argv[1] );
  landmarksReader->Update();
  const SeedSpatialObjectType * landmarks = landmarksReader->GetOutput();

  SegmentationMethodType::Pointer segmentationMethod = SegmentationMethodType::New();
  segmentationMethod->SetInput( inputImage );
  segmentationMethod->SetSeeds( landmarks->GetPoints() );
  segmentationMethod->SetRegionOfInterest( inputImage->GetBufferedRegion() );

  if( argc > 4 )
    {
    const double sigmoidBeta = atof( argv[4] );
    std::cout << "Using SigmoidBeta = " << sigmoidBeta << std::endl;
    segmentationMethod->SetSigmoidBeta( sigmoidBeta );
    }

  segmentationMethod->SetResampleThickSliceData( resampleThickSliceData );
  segmentationMethod->SetUseVesselEnhancingDiffusion( useVesselEnhancingDiffusion );

  try 
    {
    segmentationMethod->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

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
