/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    AntiAliasBinaryImageFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or https://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#  pragma warning(disable : 4786)
#endif

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkAntiAliasBinaryImageFilter.h"

int
main(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImage outputImageFloatPixelType ";
    std::cerr << " [RMS] [numberOfIterations]" << std::endl;
    return EXIT_FAILURE;
  }

  const std::string inputFilename = argv[1];
  const std::string outputFilename = argv[2];

  double       maximumRMSError = 0.01;
  unsigned int numberOfIterations = 50;

  if (argc > 4)
  {
    maximumRMSError = atof(argv[4]);
  }

  if (argc > 5)
  {
    numberOfIterations = atoi(argv[5]);
  }


  using PixelType = float;
  constexpr unsigned int Dimension = 3;

  using ImageType = itk::Image<PixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;

  using AntiAliasFilterType = itk::AntiAliasBinaryImageFilter<ImageType, ImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  AntiAliasFilterType::Pointer antiAliasFilter = AntiAliasFilterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  antiAliasFilter->SetInput(reader->GetOutput());
  writer->SetInput(antiAliasFilter->GetOutput());

  antiAliasFilter->SetMaximumRMSError(maximumRMSError);
  antiAliasFilter->SetNumberOfIterations(numberOfIterations);
  antiAliasFilter->SetNumberOfLayers(3);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Completed in " << antiAliasFilter->GetNumberOfIterations() << std::endl;

  return EXIT_SUCCESS;
}
