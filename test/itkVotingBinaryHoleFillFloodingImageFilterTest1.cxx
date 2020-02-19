/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVotingBinaryHoleFillFloodingImageFilterTest1.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#  pragma warning(disable : 4786)
#endif


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVotingBinaryHoleFillFloodingImageFilter.h"
#include "itkTestingMacros.h"


int
itkVotingBinaryHoleFillFloodingImageFilterTest1(int argc, char * argv[])
{
  if (argc < 7)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputImageFile outputImageFile inputThreshold radius majority maxNumberOfIterations" << std::endl;
    return EXIT_FAILURE;
  }


  using InputPixelType = signed short;
  using OutputPixelType = unsigned char;

  constexpr unsigned int Dimension = 3;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(argv[1]);
  writer->SetFileName(argv[2]);

  using ThresholderType = itk::BinaryThresholdImageFilter<InputImageType, OutputImageType>;

  ThresholderType::Pointer thresholder = ThresholderType::New();

  thresholder->SetLowerThreshold(std::stoi(argv[3]));
  thresholder->SetUpperThreshold(itk::NumericTraits<InputPixelType>::max());

  thresholder->SetOutsideValue(0);
  thresholder->SetInsideValue(255);

  using FilterType = itk::VotingBinaryHoleFillFloodingImageFilter<OutputImageType, OutputImageType>;

  FilterType::Pointer filter = FilterType::New();

  EXERCISE_BASIC_OBJECT_METHODS(filter, VotingBinaryHoleFillFloodingImageFilter, VotingBinaryImageFilter);


  const unsigned int radius = std::stoi(argv[4]);

  OutputImageType::SizeType indexRadius;

  indexRadius[0] = radius; // radius along x
  indexRadius[1] = radius; // radius along y
  indexRadius[2] = radius; // radius along z

  filter->SetRadius(indexRadius);

  filter->SetBackgroundValue(0);
  filter->SetForegroundValue(255);

  const unsigned int majorityThreshold = std::stoi(argv[5]);

  filter->SetMajorityThreshold(majorityThreshold);

  const unsigned int maximumNumberOfIterations = std::stoi(argv[6]);
  filter->SetMaximumNumberOfIterations(maximumNumberOfIterations);
  TEST_SET_GET_VALUE(maximumNumberOfIterations, filter->GetMaximumNumberOfIterations());

  thresholder->SetInput(reader->GetOutput());
  filter->SetInput(thresholder->GetOutput());
  writer->SetInput(filter->GetOutput());

  TRY_EXPECT_NO_EXCEPTION(writer->Update());


  std::cout << "Iteration used = " << filter->GetCurrentIterationNumber() << std::endl;
  std::cout << "Pixels changes = " << filter->GetTotalNumberOfPixelsChanged() << std::endl;

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
