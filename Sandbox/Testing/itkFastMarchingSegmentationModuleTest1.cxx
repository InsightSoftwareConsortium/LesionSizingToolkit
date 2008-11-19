/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkFastMarchingSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkFastMarchingSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSpatialObjectReader.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << "\n\tlandmarksFile\n\tfeatureImage\n\toutputImage ";
    std::cerr << "\n\tstopping time for fast marching";
    std::cerr << "\n\tdistance from seeds for fast marching" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;
  typedef itk::FastMarchingSegmentationModule< Dimension >   SegmentationModuleType;

  typedef SegmentationModuleType::FeatureImageType     FeatureImageType;
  typedef SegmentationModuleType::OutputImageType      OutputImageType;

  typedef itk::ImageFileReader< FeatureImageType >     FeatureReaderType;
  typedef itk::ImageFileWriter< OutputImageType >      OutputWriterType;

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

  typedef SpatialObjectReaderType::SceneType::ObjectListType     ObjectListType;
  ObjectListType * sceneChildren = scene->GetObjects(999999);
  ObjectListType::const_iterator spatialObjectItr = sceneChildren->begin();
  typedef SegmentationModuleType::InputSpatialObjectType  InputSpatialObjectType; 
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
 
  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();
  featureReader->SetFileName( argv[2] );
  try 
    {
    featureReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();
  
  typedef SegmentationModuleType::InputSpatialObjectType          InputSpatialObjectType;
  typedef SegmentationModuleType::FeatureSpatialObjectType        FeatureSpatialObjectType;
  typedef SegmentationModuleType::OutputSpatialObjectType         OutputSpatialObjectType;

  InputSpatialObjectType::Pointer inputObject = InputSpatialObjectType::New();
  FeatureSpatialObjectType::Pointer featureObject = FeatureSpatialObjectType::New();

  FeatureImageType::Pointer featureImage = featureReader->GetOutput();
  featureImage->DisconnectPipeline();
  featureImage->Print( std::cout );
  featureObject->SetImage( featureImage );

  segmentationModule->SetFeature( featureObject );
  segmentationModule->SetInput( landmarkSpatialObject );

  const double stoppingTime = (argc > 4) ? atof( argv[4] ) : 10.0;
  const double distanceFromSeeds = (argc > 5) ? atof( argv[5] ) : 5.0;

  segmentationModule->SetStoppingValue( stoppingTime );
  segmentationModule->SetDistanceFromSeeds( distanceFromSeeds );
  segmentationModule->Update();

  typedef SegmentationModuleType::SpatialObjectType    SpatialObjectType;
  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputSpatialObjectType * >( segmentation.GetPointer() );
  OutputImageType::ConstPointer outputImage = outputObject->GetImage();
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();
  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  segmentationModule->Print( std::cout );
  
  return EXIT_SUCCESS;
}
