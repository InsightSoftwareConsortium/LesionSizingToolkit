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

int itkLandmarksReaderTest1( int argc, char * argv [] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " landmarksFile ";
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;

  typedef itk::LandmarkSpatialObject< Dimension >    InputSpatialObjectType;

  //
  //  Reading the landmarks file with the itkLandmarksReader.
  //
  typedef itk::LandmarksReader< Dimension >    LandmarksReaderType;
  
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

  landmarksReader->Update();

  InputSpatialObjectType::ConstPointer landmarkSpatialObject1 = landmarksReader->GetOutput();

  //
  // Reading the landmarks file by using direct ITK classes
  //
  typedef itk::SpatialObjectReader< 3, unsigned short > SpatialObjectReaderType;

  SpatialObjectReaderType::Pointer landmarkPointsReader = SpatialObjectReaderType::New();

  landmarkPointsReader->SetFileName( argv[1] );
  landmarkPointsReader->Update();

  SpatialObjectReaderType::ScenePointer scene = landmarkPointsReader->GetScene();

  if( !scene )
    {
    std::cerr << "No Scene : [FAILED]" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Number of object in the scene:" << scene->GetNumberOfObjects(1) << std::endl;

  typedef SpatialObjectReaderType::SceneType::ObjectListType     ObjectListType;

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
 
  typedef InputSpatialObjectType::PointListType    PointListType;

  const unsigned int numberOfPoints1 = landmarkSpatialObject1->GetNumberOfPoints();
  const unsigned int numberOfPoints2 = landmarkSpatialObject2->GetNumberOfPoints();

  if( numberOfPoints1 != numberOfPoints2 )
    {
    std::cerr << "Number of points is not consistent between two read methods" << std::endl;
    return EXIT_FAILURE;
    }

  const PointListType & points1 = landmarkSpatialObject1->GetPoints();
  const PointListType & points2 = landmarkSpatialObject2->GetPoints();

  for( unsigned int i=0; i < numberOfPoints1; i++ )
    {
    if( points1[i].GetPosition() != points2[i].GetPosition() )
      {
      std::cerr << "Error : point " << i << " has different positions" << std::endl;
      std::cerr << "Expected position " << points2[i].GetPosition() << std::endl;
      std::cerr << "Received position " << points1[i].GetPosition() << std::endl;
      return EXIT_FAILURE;
      }
    }


  landmarkPointsReader->Print( std::cout );

  return EXIT_SUCCESS;
}
