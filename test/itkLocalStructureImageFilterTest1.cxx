/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLocalStructureImageFilterTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLocalStructureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkTestingMacros.h"


int
itkLocalStructureImageFilterTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImage outputImage [sigma alpha gamma]" << std::endl;
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

  using LocalStructureFilterType = itk::LocalStructureImageFilter<EigenValueImageType, OutputImageType>;

  LocalStructureFilterType::Pointer localStructure = LocalStructureFilterType::New();

  ITK_EXERCISE_BASIC_OBJECT_METHODS(localStructure, LocalStructureImageFilter, UnaryFunctorImageFilter);

  hessian->SetInput(reader->GetOutput());
  eigen->SetInput(hessian->GetOutput());
  localStructure->SetInput(eigen->GetOutput());


  double sigma = 1.0;
  if (argc > 3)
  {
    sigma = std::stod(argv[3]);
  }
  hessian->SetSigma(sigma);
  ITK_TEST_SET_GET_VALUE(sigma, hessian->GetSigma());

  double alpha = 0.25;
  if (argc > 4)
  {
    alpha = std::stod(argv[4]);
  }
  localStructure->SetAlpha(alpha);
  // ITK_TEST_SET_GET_VALUE( alpha, localStructure->GetAlpha() );

  double gamma = 0.5;
  if (argc > 5)
  {
    gamma = std::stod(argv[5]);
  }
  localStructure->SetGamma(gamma);
  // ITK_TEST_SET_GET_VALUE( gamma, localStructure->GetGamma() );

  eigen->SetDimension(Dimension);

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(localStructure->GetOutput());

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
