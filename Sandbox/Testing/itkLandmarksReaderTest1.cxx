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

int main( int argc, char * argv [] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " landmarksFile ";
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;

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

  typedef itk::LandmarkSpatialObject< Dimension >               InputSpatialObjectType;

  const InputSpatialObjectType * landmarkSpatialObject = NULL;

  while( spatialObjectItr != sceneChildren->end() ) 
    {
    std::string objectName = (*spatialObjectItr)->GetTypeName();
    if( objectName == "LandmarkSpatialObject" )
      {
      landmarkSpatialObject = 
        dynamic_cast< const InputSpatialObjectType * >( spatialObjectItr->GetPointer() );
      }
    spatialObjectItr++;
    }
 
  // landmarkSpatialObject 

  return EXIT_SUCCESS;
}
