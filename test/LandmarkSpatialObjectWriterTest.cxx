/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    LandmarkSpatialObjectWriterTest.cxx
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

#include "itkLandmarkSpatialObject.h"
#include "itkSpatialObjectWriter.h"
#include "itkSpatialObjectReader.h"
#include "itkTestingMacros.h"


int
LandmarkSpatialObjectWriterTest(int itkNotUsed(argc), char * argv[])
{
  constexpr unsigned int Dimension = 3;

  using WriterType = itk::SpatialObjectWriter<Dimension, unsigned short>;

  using LandmarkType = itk::LandmarkSpatialObject<Dimension>;
  using LandmarkPointer = LandmarkType::Pointer;
  using SpatialPointType = LandmarkType::SpatialObjectPointType;
  using LandmarkPointListType = LandmarkType::LandmarkPointListType;

  LandmarkPointListType listOfPoints;

  SpatialPointType p;

  p.SetColor(1, 0, 0, 1);

  p.SetPositionInObjectSpace(0.4, 0.5, 0.7);
  listOfPoints.push_back(p);

  p.SetPositionInObjectSpace(0.5, 0.4, 0.7);
  listOfPoints.push_back(p);

  p.SetPositionInObjectSpace(0.6, 0.3, 0.7);
  listOfPoints.push_back(p);

  p.SetPositionInObjectSpace(0.7, 0.2, 0.7);
  listOfPoints.push_back(p);


  LandmarkPointer landmarkSpatialObject = LandmarkType::New();
  landmarkSpatialObject->SetPoints(listOfPoints);
  landmarkSpatialObject->GetProperty().SetName("Landmark 1");

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput(landmarkSpatialObject);

  writer->SetFileName(argv[1]);
  writer->SetBinaryPoints(false);

  TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
