/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest3.cxx

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
#include "itkSimpleFilterWatcher.h"
#include "itkTestingMacros.h"

int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest3(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << itkNameOfTestExecutableMacro(argv) << " InputImage OutputImage" << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension{ 2 };
  using InputImage = itk::Image<float, Dimension>;
  using OutputImage = itk::Image<unsigned char, Dimension>;
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<InputImage, InputImage>;

  auto reader = itk::ImageFileReader<InputImage>::New();
  reader->SetFileName(argv[1]);

  auto                           filter = FilterType::New();
  const itk::SimpleFilterWatcher watcher(filter);
  filter->SetInput(reader->GetOutput());

  constexpr FilterType::OutputImagePixelType upperThreshold{ 30 };
  filter->SetUpperThreshold(upperThreshold);
  ITK_TEST_SET_GET_VALUE(upperThreshold, filter->GetUpperThreshold());

  constexpr FilterType::OutputImagePixelType lowerThreshold{ 15 };
  filter->SetLowerThreshold(lowerThreshold);
  ITK_TEST_SET_GET_VALUE(lowerThreshold, filter->GetLowerThreshold());

  const FilterType::SigmaArrayType sigma(1.0);
  filter->SetSigmaArray(sigma);
  ITK_TEST_SET_GET_VALUE(sigma, filter->GetSigmaArray());

  auto rescale = itk::RescaleIntensityImageFilter<InputImage, OutputImage>::New();
  rescale->SetInput(filter->GetOutput());
  rescale->SetOutputMinimum(0);
  rescale->SetOutputMaximum(255);

  auto writer = itk::ImageFileWriter<OutputImage>::New();
  writer->SetInput(rescale->GetOutput());
  writer->SetFileName(argv[2]);
  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  return EXIT_SUCCESS;
}
