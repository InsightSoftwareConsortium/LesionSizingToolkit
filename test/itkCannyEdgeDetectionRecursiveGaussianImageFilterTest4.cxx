/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilterTest4.cxx

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
#include "itkCastImageFilter.h"
#include "itkTestingMacros.h"

int
itkCannyEdgeDetectionRecursiveGaussianImageFilterTest4(int argc, char * argv[])
{
  if (argc < 4)
  {
    std::cerr << "Usage: " << itkNameOfTestExecutableMacro(argv)
              << " InputImage OutputImage1 OutputImage2 [sigma upperThreshold lowerThreshold]" << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension{ 3 };
  using InputImage = itk::Image<short, Dimension>;
  using InternalImage = itk::Image<float, Dimension>;
  using FilterType = itk::CannyEdgeDetectionRecursiveGaussianImageFilter<InternalImage, InternalImage>;

  const double sigma = argc > 4 ? std::stod(argv[4]) : 0.7;
  const float  upperThreshold = argc > 5 ? std::stof(argv[5]) : 150.0f;
  const float  lowerThreshold = argc > 6 ? std::stof(argv[6]) : 75.0f;

  auto reader = itk::ImageFileReader<InputImage>::New();
  reader->SetFileName(argv[1]);

  auto cast = itk::CastImageFilter<InputImage, InternalImage>::New();
  cast->SetInput(reader->GetOutput());

  auto filter = FilterType::New();
  filter->SetInput(cast->GetOutput());
  filter->SetSigma(sigma);
  filter->SetUpperThreshold(upperThreshold);
  filter->SetLowerThreshold(lowerThreshold);

  auto writer = itk::ImageFileWriter<InternalImage>::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(argv[2]);
  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  // Looser thresholds must produce a different, re-executed result.
  filter->SetUpperThreshold(upperThreshold / 2.0f);
  filter->SetLowerThreshold(lowerThreshold / 2.0f);
  ITK_TRY_EXPECT_NO_EXCEPTION(filter->Update());

  // Restoring the thresholds must reproduce the first result.
  filter->SetUpperThreshold(upperThreshold);
  filter->SetLowerThreshold(lowerThreshold);
  writer->SetFileName(argv[3]);
  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  return EXIT_SUCCESS;
}
