/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkShapeDetectionLevelSetSegmentationModuleTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkShapeDetectionLevelSetSegmentationModule.h"
#include "itkImage.h"
#include "itkSpatialObject.h"
#include "itkImageSpatialObject.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"

int itkShapeDetectionLevelSetSegmentationModuleTest1( int argc, char * argv [] )
{

  if( argc < 4 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage featureImage outputImage ";
    std::cerr << " [propagationScaling curvatureScaling maximumNumberOfIterations]" << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;

  using SegmentationModuleType = itk::ShapeDetectionLevelSetSegmentationModule< Dimension >;

  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();

  using InputImageType = SegmentationModuleType::InputImageType;
  using FeatureImageType = SegmentationModuleType::FeatureImageType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  using InputReaderType = itk::ImageFileReader< InputImageType >;
  using InputWriterType = itk::ImageFileWriter< InputImageType >;
  using FeatureReaderType = itk::ImageFileReader< FeatureImageType >;
  using WriterType = itk::ImageFileWriter< OutputImageType >;

  using RescaleFilterType = itk::RescaleIntensityImageFilter< InputImageType, InputImageType >;

  InputReaderType::Pointer inputReader = InputReaderType::New();
  InputWriterType::Pointer inputWriter = InputWriterType::New();
  FeatureReaderType::Pointer featureReader = FeatureReaderType::New();

  inputReader->SetFileName( argv[1] );


  RescaleFilterType::Pointer rescaler = RescaleFilterType::New();
  rescaler->SetInput( inputReader->GetOutput() );

  rescaler->SetOutputMinimum( -4.0 );
  rescaler->SetOutputMaximum(  4.0 );

  inputWriter->SetFileName( "inputSegmentation.mha" );

  inputWriter->SetInput( rescaler->GetOutput() );
  inputWriter->Update();

  featureReader->SetFileName( argv[2] );

  try
    {
    rescaler->Update();
    featureReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  using InputSpatialObjectType = SegmentationModuleType::InputSpatialObjectType;
  using FeatureSpatialObjectType = SegmentationModuleType::FeatureSpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;

  InputSpatialObjectType::Pointer inputObject = InputSpatialObjectType::New();
  FeatureSpatialObjectType::Pointer featureObject = FeatureSpatialObjectType::New();

  inputObject->SetImage( rescaler->GetOutput() );
  featureObject->SetImage( featureReader->GetOutput() );

  segmentationModule->SetInput( inputObject );
  segmentationModule->SetFeature( featureObject );

  double propagationScaling = 1.0;
  double curvatureScaling = 1.0;
  unsigned int maximumNumberOfIterations = 50;

  if( argc > 4 )
    {
    propagationScaling = atof( argv[4] );
    }

  if( argc > 5 )
    {
    curvatureScaling = atof( argv[5] );
    }

  if( argc > 6 )
    {
    maximumNumberOfIterations = atoi( argv[6] );
    }


  segmentationModule->SetPropagationScaling( propagationScaling );
  segmentationModule->SetCurvatureScaling( curvatureScaling );
  segmentationModule->SetMaximumNumberOfIterations( maximumNumberOfIterations );

  segmentationModule->Update();

  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
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
