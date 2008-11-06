/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int main( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage featureImage outputImage " << std::endl;
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;

  typedef itk::GeodesicActiveContourLevelSetSegmentationModule< Dimension >   SegmentationModuleType;

  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();
  
  typedef SegmentationModuleType::InputImageType       InputImageType;
  typedef SegmentationModuleType::FeatureImageType     FeatureImageType;
  typedef SegmentationModuleType::OutputImageType      OutputImageType;

  typedef itk::ImageFileReader< InputImageType >       InputReaderType;
  typedef itk::ImageFileReader< FeatureImageType >     FeatureReaderType;
  typedef itk::ImageFileWriter< OutputImageType >      WriterType;

  InputReaderType::Pointer inputReader = InputReaderType::New();
  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();

  inputReader->SetFileName( argv[1] );
  featureReader->SetFileName( argv[2] );

  try 
    {
    inputReader->Update();
    featureReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  typedef SegmentationModuleType::InputSpatialObjectType          InputSpatialObjectType;
  typedef SegmentationModuleType::FeatureSpatialObjectType        FeatureSpatialObjectType;
  typedef SegmentationModuleType::OutputSpatialObjectType         OutputSpatialObjectType;

  InputSpatialObjectType::Pointer inputObject = InputSpatialObjectType::New();
  FeatureSpatialObjectType::Pointer featureObject = FeatureSpatialObjectType::New();

  inputObject->SetImage( inputReader->GetOutput() );
  featureObject->SetImage( featureReader->GetOutput() );

  segmentationModule->SetInput( inputObject );
  segmentationModule->SetFeature( featureObject );

  segmentationModule->Update();

  typedef SegmentationModuleType::SpatialObjectType    SpatialObjectType;
  SpatialObjectType::ConstPointer segmentation = segmentationModule->GetOutput();

  OutputSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputSpatialObjectType * >( segmentation.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( outputImage );

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

  std::cout << "Class name = " << segmentationModule->GetNameOfClass() << std::endl;
  
  return EXIT_SUCCESS;
}
