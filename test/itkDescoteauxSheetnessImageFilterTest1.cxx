/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkDescoteauxSheetnessImageFilterTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkDescoteauxSheetnessImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkDescoteauxSheetnessImageFilterTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [(bright1/dark:0) sigma sheetness bloobiness noise]" << std::endl;
    return EXIT_FAILURE;
  }

  constexpr unsigned int Dimension = 3;

  using InputPixelType = signed short;
  using OutputPixelType = float;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using HessianFilterType = itk::HessianRecursiveGaussianImageFilter<InputImageType>;
  using HessianImageType = HessianFilterType::OutputImageType;
  using HessianPixelType = HessianImageType::PixelType;

  using EigenValueArrayType = itk::FixedArray<double, HessianPixelType::Dimension>;
  using EigenValueImageType = itk::Image<EigenValueArrayType, Dimension>;

  using EigenAnalysisFilterType = itk::SymmetricEigenAnalysisImageFilter<HessianImageType, EigenValueImageType>;

  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  HessianFilterType::Pointer       hessian = HessianFilterType::New();
  EigenAnalysisFilterType::Pointer eigen = EigenAnalysisFilterType::New();
  ReaderType::Pointer              reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  ITK_TRY_EXPECT_NO_EXCEPTION(reader->Update());


  using FilterType = itk::DescoteauxSheetnessImageFilter<EigenValueImageType, OutputImageType>;

  FilterType::Pointer sheetnessFilter = FilterType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(sheetnessFilter, DescoteauxSheetnessImageFilter, UnaryFunctorImageFilter);

  hessian->SetInput(reader->GetOutput());
  eigen->SetInput(hessian->GetOutput());
  sheetnessFilter->SetInput(eigen->GetOutput());


  bool detectBrightSheets = true;
  if (argc > 3)
  {
    detectBrightSheets = std::stoi(argv[3]);
  }
  sheetnessFilter->SetDetectBrightSheets(detectBrightSheets);
  // ITK_TEST_SET_GET_BOOLEAN( sheetnessFilter, DetectBrightSheets, detectBrightSheets );

  double sigma = 1.0;
  if (argc > 4)
  {
    hessian->SetSigma(std::stod(argv[4]));
  }
  hessian->SetSigma(sigma);
  ITK_TEST_SET_GET_VALUE(sigma, hessian->GetSigma());

  double sheetnessNormalization = 0.5;
  if (argc > 5)
  {
    sheetnessNormalization = std::stod(argv[5]);
  }
  sheetnessFilter->SetSheetnessNormalization(sheetnessNormalization);
  // ITK_TEST_SET_GET_VALUE( sheetnessNormalization, sheetnessFilter->GetSheetnessNormalization() );

  double bloobinessNormalization = 2.0;
  if (argc > 6)
  {
    bloobinessNormalization = std::stod(argv[6]);
  }
  sheetnessFilter->SetBloobinessNormalization(bloobinessNormalization);
  // ITK_TEST_SET_GET_VALUE( bloobinessNormalization, sheetnessFilter->GetBloobinessNormalization() );

  double noiseNormalization = 1.0;
  if (argc > 7)
  {
    noiseNormalization = std::stod(argv[7]);
  }
  sheetnessFilter->SetNoiseNormalization(noiseNormalization);
  // ITK_TEST_SET_GET_VALUE( noiseNormalization, sheetnessFilter->GetNoiseNormalization() );


  eigen->SetDimension(Dimension);


  ITK_TRY_EXPECT_NO_EXCEPTION(sheetnessFilter->Update());


  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(sheetnessFilter->GetOutput());

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
