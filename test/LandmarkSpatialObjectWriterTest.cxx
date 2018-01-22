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
#pragma warning ( disable : 4786 )
#endif

#include "itkLandmarkSpatialObject.h"
#include "itkSpatialObjectWriter.h"
#include "itkSpatialObjectReader.h"

int LandmarkSpatialObjectWriterTest( int itkNotUsed(argc), char * argv [] )
{
  const unsigned int Dimension = 3;

  typedef itk::SpatialObjectWriter< Dimension, unsigned short >   WriterType;

  typedef itk::LandmarkSpatialObject< Dimension >     LandmarkType;
  typedef LandmarkType::Pointer                       LandmarkPointer;
  typedef LandmarkType::SpatialObjectPointType        SpatialPointType;
  typedef LandmarkType::PointListType                 PointListType;

  PointListType     listOfPoints;

  SpatialPointType p;

  p.SetColor(1,0,0,1);

  p.SetPosition( 0.4, 0.5, 0.7 );
  listOfPoints.push_back( p );

  p.SetPosition( 0.5, 0.4, 0.7 );
  listOfPoints.push_back( p );

  p.SetPosition( 0.6, 0.3, 0.7 );
  listOfPoints.push_back( p );

  p.SetPosition( 0.7, 0.2, 0.7 );
  listOfPoints.push_back( p );


  LandmarkPointer landmarkSpatialObject = LandmarkType::New();
  landmarkSpatialObject->SetPoints( listOfPoints );
  landmarkSpatialObject->GetProperty()->SetName("Landmark 1");

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput( landmarkSpatialObject );

  writer->SetFileName( argv[1] );
  writer->SetBinaryPoints(false);

  writer->Update();
  
  std::cout << " [TEST DONE]" << std::endl;
  
  return EXIT_SUCCESS;
}
