/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLandmarksReaderTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLandmarksReader.h"
#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkTestingMacros.h"


int itkLandmarksReaderTest1( int argc, char * argv [] )
{
  if( argc < 2 )
    {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile ";
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;

  using InputSpatialObjectType = itk::LandmarkSpatialObject< Dimension >;

  //
  //  Reading the landmarks file with the itkLandmarksReader.
  //
  using LandmarksReaderType = itk::LandmarksReader< Dimension >;

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  std::string inputFileName = argv[1];

  landmarksReader->SetFileName( inputFileName );
  landmarksReader->SetFileName( argv[1] );

  std::string recoveredFileName = landmarksReader->GetFileName();

  if( recoveredFileName != inputFileName )
    {
    std::cerr << "Error in Set/GetFileName()" << std::endl;
    return EXIT_FAILURE;
    }

  TRY_EXPECT_NO_EXCEPTION( landmarksReader->Update() );


  InputSpatialObjectType::ConstPointer landmarkSpatialObject1 = landmarksReader->GetOutput();

  //
  // Reading the landmarks file by using direct ITK classes
  //
  using SpatialObjectReaderType = itk::SpatialObjectReader< 3, unsigned short >;

  SpatialObjectReaderType::Pointer landmarkPointsReader = SpatialObjectReaderType::New();

  EXERCISE_BASIC_OBJECT_METHODS( landmarkPointsReader, SpatialObjectReader,
    Object );

  landmarkPointsReader->SetFileName( argv[1] );
  landmarkPointsReader->Update();

  SpatialObjectReaderType::ScenePointer scene = landmarkPointsReader->GetScene();

  if( !scene )
    {
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "No Scene." << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Number of object in the scene:" << scene->GetNumberOfObjects(1) << std::endl;

  using ObjectListType = SpatialObjectReaderType::SceneType::ObjectListType;

  ObjectListType * sceneChildren = scene->GetObjects(999999);

  ObjectListType::const_iterator spatialObjectItr = sceneChildren->begin();


  const InputSpatialObjectType * landmarkSpatialObject2 = nullptr;

  while( spatialObjectItr != sceneChildren->end() )
    {
    std::string objectName = (*spatialObjectItr)->GetTypeName();
    if( objectName == "LandmarkSpatialObject" )
      {
      landmarkSpatialObject2 =
        dynamic_cast< const InputSpatialObjectType * >( spatialObjectItr->GetPointer() );
      }
    spatialObjectItr++;
    }

  using PointListType = InputSpatialObjectType::PointListType;

  const unsigned int numberOfPoints1 = landmarkSpatialObject1->GetNumberOfPoints();
  const unsigned int numberOfPoints2 = landmarkSpatialObject2->GetNumberOfPoints();

  if( numberOfPoints1 != numberOfPoints2 )
    {
    std::cerr << "Test failed!" << std::endl;
    std::cerr << "Number of points is not consistent between two read methods" << std::endl;
    std::cerr << "Error in GetNumberOfPoints()" << std::endl;
    std::cerr << "Expected value " << numberOfPoints1 << std::endl;
    std::cerr << " differs from " << numberOfPoints2 << std::endl;
    return EXIT_FAILURE;
    }

  const PointListType & points1 = landmarkSpatialObject1->GetPoints();
  const PointListType & points2 = landmarkSpatialObject2->GetPoints();

  for( unsigned int i = 0; i < numberOfPoints1; ++i )
    {
    if( points1[i].GetPosition() != points2[i].GetPosition() )
      {
      std::cerr << "Test failed!" << std::endl;
      std::cerr << "Error : point " << i << " has different positions" << std::endl;
      std::cerr << "Expected value " << points2[i].GetPosition() << std::endl;
      std::cerr << " differs from" << points1[i].GetPosition() << std::endl;
      return EXIT_FAILURE;
      }
    }


  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
