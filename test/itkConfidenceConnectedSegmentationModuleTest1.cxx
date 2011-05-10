/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkConfidenceConnectedSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkConfidenceConnectedSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLandmarksReader.h"

int itkConfidenceConnectedSegmentationModuleTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " landmarksFile featureImage outputImage ";
    std::cerr << " [sigmaMultiplier] " << std::endl;
    return EXIT_FAILURE;
    }


  const unsigned int Dimension = 3;

  typedef itk::ConfidenceConnectedSegmentationModule< Dimension >   SegmentationModuleType;

  typedef SegmentationModuleType::FeatureImageType     FeatureImageType;
  typedef SegmentationModuleType::OutputImageType      OutputImageType;

  typedef itk::ImageFileReader< FeatureImageType >     FeatureReaderType;
  typedef itk::ImageFileWriter< OutputImageType >      OutputWriterType;

  typedef itk::LandmarksReader< Dimension >    LandmarksReaderType;
  
  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName( argv[1] );
  landmarksReader->Update();

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
  segmentationModule->SetInput( landmarksReader->GetOutput() );

  const double sigmaMultiplier = (argc > 4) ? atof( argv[4] ) : 2.0;
  segmentationModule->SetSigmaMultiplier( sigmaMultiplier );

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
  std::cout << "Class name = " << segmentationModule->GetNameOfClass() << std::endl;
  
  //
  // Exerciser Get methods
  //
  segmentationModule->SetSigmaMultiplier( 0.0 );
  if( segmentationModule->GetSigmaMultiplier() != 0.0 )
    {
    std::cerr << "Error in Set/GetSigmaMultiplier() " << std::endl;
    return EXIT_FAILURE;
    }

  segmentationModule->SetSigmaMultiplier( sigmaMultiplier );
  if( segmentationModule->GetSigmaMultiplier() != sigmaMultiplier )
    {
    std::cerr << "Error in Set/GetSigmaMultiplier() " << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
