/*=========================================================================
const
  Program:   Lesion Sizing Toolkit
  Module:    itkRegionCompetitionImageFilterTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkBinaryThresholdImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkRegionCompetitionImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkTestingMacros.h"


int
itkRegionCompetitionImageFilterTest1(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  constexpr unsigned int Dimension = 3;

  using InputPixelType = signed short;
  using BinaryPixelType = unsigned char;
  using LabelPixelType = unsigned short;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using BinaryImageType = itk::Image<BinaryPixelType, Dimension>;
  using LabelImageType = itk::Image<LabelPixelType, Dimension>;

  using ThresholdFilterType = itk::BinaryThresholdImageFilter<InputImageType, BinaryImageType>;

  using ComponentsFilterType = itk::ConnectedComponentImageFilter<BinaryImageType, LabelImageType>;

  using RelabelFilterType = itk::RelabelComponentImageFilter<LabelImageType, LabelImageType>;

  using CompetitionFilterType = itk::RegionCompetitionImageFilter<InputImageType, LabelImageType>;


  ThresholdFilterType::Pointer thresholderFilter = ThresholdFilterType::New();

  ComponentsFilterType::Pointer componentsFilter = ComponentsFilterType::New();

  RelabelFilterType::Pointer relabelerFilter = RelabelFilterType::New();

  CompetitionFilterType::Pointer competitionFilter = CompetitionFilterType::New();


  //
  // Create an input image
  //
  InputImageType::Pointer inputImage = InputImageType::New();

  InputImageType::RegionType itkregion;
  InputImageType::SizeType   itksize;
  InputImageType::IndexType  itkindex;

  itksize[0] = 21;
  itksize[1] = 21;
  itksize[2] = 42;

  itkindex[0] = 0;
  itkindex[1] = 0;
  itkindex[2] = 0;

  itkregion.SetIndex(itkindex);
  itkregion.SetSize(itksize);

  inputImage->SetRegions(itkregion);
  inputImage->Allocate();

  InputImageType::SpacingType itkspacing;

  itkspacing[0] = 0.7;
  itkspacing[1] = 0.9;
  itkspacing[2] = 1.5;

  inputImage->SetSpacing(itkspacing);

  InputImageType::PointType itkorigin;

  itkorigin[0] = 129.5;
  itkorigin[1] = 137.5;
  itkorigin[2] = 159.5;

  inputImage->SetOrigin(itkorigin);

  //
  // Populate the pixel values in an asymmetric way to ensure that one
  // component is larger than the other.
  //
  InputImageType::IndexType index1;
  InputImageType::IndexType index2;

  index1[0] = 10;
  index1[1] = 10;
  index1[2] = 10;

  index2[0] = 10;
  index2[1] = 10;
  index2[2] = 31;

  InputImageType::PointType point1;
  InputImageType::PointType point2;

  inputImage->TransformIndexToPhysicalPoint(index1, point1);
  inputImage->TransformIndexToPhysicalPoint(index2, point2);


  using IteratorType = itk::ImageRegionIteratorWithIndex<InputImageType>;

  IteratorType itr(inputImage, itkregion);

  itr.GoToBegin();

  InputImageType::PointType point;

  while (!itr.IsAtEnd())
  {
    const InputImageType::IndexType & index = itr.GetIndex();
    inputImage->TransformIndexToPhysicalPoint(index, point);
    const double distance1 = point1.EuclideanDistanceTo(point);
    const double distance2 = point2.EuclideanDistanceTo(point);
    const double value1 = 1000.0 - distance1 * 50;
    const double value2 = 1000.0 - distance2 * 50;
    if (value1 > value2)
    {
      itr.Set(static_cast<InputPixelType>(value1));
    }
    else
    {
      itr.Set(static_cast<InputPixelType>(value2));
    }
    ++itr;
  }


  // Just for debugging, save input image
  using WriterType = itk::ImageFileWriter<InputImageType>;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(inputImage);
  writer->SetFileName("inputCompetitionImage.mha");

  ITK_TRY_EXPECT_NO_EXCEPTION(writer->Update());


  thresholderFilter->SetInput(inputImage);
  thresholderFilter->SetUpperThreshold(2000);
  thresholderFilter->SetLowerThreshold(400);

  ITK_TRY_EXPECT_NO_EXCEPTION(thresholderFilter->Update());

  componentsFilter->SetInput(thresholderFilter->GetOutput());
  relabelerFilter->SetInput(componentsFilter->GetOutput());

  relabelerFilter->Update();
  LabelImageType * labelImagePt = relabelerFilter->GetOutput();
  // Just for debugging, save input image of labels
  using LabelWriterType = itk::ImageFileWriter<LabelImageType>;
  LabelWriterType::Pointer labelWriter = LabelWriterType::New();
  labelWriter->SetInput(relabelerFilter->GetOutput());
  labelWriter->SetFileName("labeledImage.mha");

  ITK_TRY_EXPECT_NO_EXCEPTION(labelWriter->Update());


  competitionFilter->SetInput(inputImage);
  competitionFilter->SetInputLabels(labelImagePt);

  ITK_TRY_EXPECT_NO_EXCEPTION(competitionFilter->Update());


  // Write the output image
  labelWriter->SetInput(competitionFilter->GetOutput());
  labelWriter->SetFileName("labeledSegmentedImage.mha");

  ITK_TRY_EXPECT_NO_EXCEPTION(labelWriter->Update());


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
