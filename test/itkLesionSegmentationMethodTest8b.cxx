/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest8b.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

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
#include "itkTestingMacros.h"


// Applies fast marhching followed by segmentation using geodesic active contours.
int
itkLesionSegmentationMethodTest8b(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile inputImage outputImage";
    std::cerr << " [SigmoidBeta] [-ResampleThickSliceData] [-UseVesselEnhancingDiffusion]" << std::endl;
    return EXIT_FAILURE;
  }

  bool useVesselEnhancingDiffusion = false, resampleThickSliceData = false;
  for (int i = 1; i < argc; i++)
  {
    useVesselEnhancingDiffusion |= (strcmp("-UseVesselEnhancingDiffusion", argv[i]) == 0);
    resampleThickSliceData |= (strcmp("-ResampleThickSliceData", argv[i]) == 0);
  }

  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;
  using OutputPixelType = float;
  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;
  using SegmentationMethodType = itk::LesionSegmentationImageFilter8<InputImageType, OutputImageType>;
  using InputImageReaderType = itk::ImageFileReader<InputImageType>;
  using LandmarksReaderType = itk::LandmarksReader<Dimension>;
  using SeedSpatialObjectType = itk::LandmarkSpatialObject<Dimension>;
  using OutputWriterType = itk::ImageFileWriter<OutputImageType>;

  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();
  inputImageReader->SetFileName(argv[2]);

  ITK_TRY_EXPECT_NO_EXCEPTION(inputImageReader->Update());

  const InputImageType * inputImage = inputImageReader->GetOutput();

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();
  landmarksReader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(landmarksReader->Update());


  const SeedSpatialObjectType * landmarks = landmarksReader->GetOutput();

  SegmentationMethodType::Pointer segmentationMethod = SegmentationMethodType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(segmentationMethod, LesionSegmentationImageFilter8, ImageToImageFilter);


  segmentationMethod->SetInput(inputImage);
  segmentationMethod->SetSeeds(landmarks->GetPoints());
  segmentationMethod->SetRegionOfInterest(inputImage->GetBufferedRegion());

  double sigmoidBeta = -500.0;
  if (argc > 4)
  {
    sigmoidBeta = std::stod(argv[4]);
  }
  segmentationMethod->SetSigmoidBeta(sigmoidBeta);
  ITK_TEST_SET_GET_VALUE(sigmoidBeta, segmentationMethod->GetSigmoidBeta());

  segmentationMethod->SetResampleThickSliceData(resampleThickSliceData);
  segmentationMethod->SetUseVesselEnhancingDiffusion(useVesselEnhancingDiffusion);

  ITK_TRY_EXPECT_NO_EXCEPTION(segmentationMethod->Update());

  OutputWriterType::Pointer writer = OutputWriterType::New();
  writer->SetFileName(argv[3]);
  writer->SetInput(segmentationMethod->GetOutput());
  writer->UseCompressionOn();

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
