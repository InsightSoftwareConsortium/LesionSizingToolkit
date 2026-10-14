/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest2.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkTestingMacros.h"

int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest2(int argc, char * argv[])
{
  if (argc < 4)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << itkNameOfTestExecutableMacro(argv) << " InputImage OutputImage1 OutputImage2"
              << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension{ 2 };
  using InputImage = itk::Image<float, Dimension>;
  using OutputImage = itk::Image<unsigned char, Dimension>;
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<InputImage, InputImage>;

  auto reader = itk::ImageFileReader<InputImage>::New();
  reader->SetFileName(argv[1]);

  auto filter = FilterType::New();
  ITK_EXERCISE_BASIC_OBJECT_METHODS(filter, CannyEdgeDetectionRecursiveGaussianImageFilter, ImageToImageFilter);

  filter->SetInput(reader->GetOutput());

  FilterType::OutputImagePixelType upperThreshold = 25;
  filter->SetUpperThreshold(upperThreshold);
  ITK_TEST_SET_GET_VALUE(upperThreshold, filter->GetUpperThreshold());

  FilterType::OutputImagePixelType lowerThreshold = 10;
  filter->SetLowerThreshold(lowerThreshold);
  ITK_TEST_SET_GET_VALUE(lowerThreshold, filter->GetLowerThreshold());

  constexpr FilterType::ScalarRealType sigma = 1.0;
  filter->SetSigma(sigma);
  ITK_TEST_SET_GET_VALUE(sigma, filter->GetSigma());

  auto rescale = itk::RescaleIntensityImageFilter<InputImage, OutputImage>::New();
  rescale->SetInput(filter->GetOutput());
  rescale->SetOutputMinimum(0);
  rescale->SetOutputMaximum(255);

  auto writer = itk::ImageFileWriter<OutputImage>::New();
  writer->SetInput(rescale->GetOutput());
  writer->SetFileName(argv[2]);
  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  // Different thresholds must re-execute the pipeline.
  upperThreshold = 20;
  filter->SetUpperThreshold(upperThreshold);
  ITK_TEST_SET_GET_VALUE(upperThreshold, filter->GetUpperThreshold());

  lowerThreshold = 5;
  filter->SetLowerThreshold(lowerThreshold);
  ITK_TEST_SET_GET_VALUE(lowerThreshold, filter->GetLowerThreshold());

  ITK_TRY_EXPECT_NO_EXCEPTION(rescale->Update());

  // Restoring the thresholds must reproduce the first result.
  upperThreshold = 25;
  filter->SetUpperThreshold(upperThreshold);
  ITK_TEST_SET_GET_VALUE(upperThreshold, filter->GetUpperThreshold());

  lowerThreshold = 10;
  filter->SetLowerThreshold(lowerThreshold);
  ITK_TEST_SET_GET_VALUE(lowerThreshold, filter->GetLowerThreshold());

  writer->SetFileName(argv[3]);
  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  return EXIT_SUCCESS;
}
