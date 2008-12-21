/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ViewImageSlicesAndSegmentationContours.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkContourVisualizationModule.h"
#include "vtkSmartPointer.h"
#include "vtkMetaImageReader.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageViewer2.h"
#include "vtkImageActor.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkCamera.h"
#include "vtkWindowToImageFilter.h"
#include "vtkPNGWriter.h"

#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkLandmarkSpatialObject.h"

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

int main(int argc, char * argv [] )
{  

  // Load a scalar image and a segmentation mask and display the
  // scalar image with the contours of the mask overlaid on it.

  if( argc < 4 )
    {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " imageFileName landmarkFile ";
    std::cerr << " isoValue ";
    std::cerr << " [screenshot:1/0] screnshotFilename.png";
    std::cerr << " segmentationFilename [segmentationFilename2,...n]";
    std::cerr << std::endl;
    return 1;
    }
  

  typedef itk::SpatialObjectReader< 3, unsigned short > SpatialObjectReaderType;

  SpatialObjectReaderType::Pointer landmarkPointsReader = SpatialObjectReaderType::New();

  landmarkPointsReader->SetFileName( argv[2] );
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

  /** Type of the input set of seed points. They are stored in a Landmark Spatial Object. */
  typedef itk::LandmarkSpatialObject< 3 >  InputSpatialObjectType;

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
 
  typedef InputSpatialObjectType::PointListType            PointListType;
  typedef InputSpatialObjectType::SpatialObjectPointType   SpatialObjectPointType;
  typedef SpatialObjectPointType::PointType                PointType;


  const PointListType & points = landmarkSpatialObject->GetPoints();

  // Grab the first point in the list of seed points
  PointType point = points[0].GetPosition();

  std::cout << "Seed point = " << point << std::endl;

  double seedPoint[3];

  seedPoint[0] = point[0];
  seedPoint[1] = point[1];
  seedPoint[2] = point[2];

  typedef vtkSmartPointer< vtkContourVisualizationModule >  vtkContourVisualizationModulePointer;
  typedef std::vector< vtkContourVisualizationModulePointer >    ContoursContainer;

  ContoursContainer  contourModules;

  VTK_CREATE( vtkMetaImageReader, imageReader );

  imageReader->SetFileName( argv[1] );
  imageReader->Update();

  float isoValue = atof( argv[3] );

  std::cout << "Iso-Value = " << isoValue << std::endl;

  bool produceScreenshot = atoi( argv[4] );

  std::cout << "Produce Screen shot = " << produceScreenshot << std::endl;

  unsigned int numberOfArgumentsBeforeSegmentations = 5;

  if( produceScreenshot )
    {
    numberOfArgumentsBeforeSegmentations += 1;
    }

  VTK_CREATE( vtkImageViewer2, imageViewer );
  VTK_CREATE( vtkRenderWindow, renderWindow );
  VTK_CREATE( vtkRenderer,     renderer );
  VTK_CREATE( vtkRenderWindowInteractor, renderWindowInteractor );
  VTK_CREATE( vtkInteractorStyleImage, interactorStyle );

  renderWindow->SetSize(600, 600);
  renderWindow->AddRenderer(renderer);
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderWindowInteractor->SetInteractorStyle( interactorStyle );

  // Set the background to something grayish
  renderer->SetBackground(0.4392, 0.5020, 0.5647);

  // set the interpolation type to nearest neighbour
  imageViewer->GetImageActor()->SetInterpolate( 0 );

  imageViewer->SetInput( imageReader->GetOutput() );

  renderer->AddActor( imageViewer->GetImageActor() );


  const unsigned int numberOfSegmentations = argc - numberOfArgumentsBeforeSegmentations;

  for(unsigned int segmentationId=0; segmentationId < numberOfSegmentations; segmentationId++)
    {
    VTK_CREATE( vtkMetaImageReader, segmentationReader );

    std::string segmentationFileName = argv[segmentationId+numberOfArgumentsBeforeSegmentations];
    std::cout << "Segmentation file name = " << segmentationFileName << std::endl;
    segmentationReader->SetFileName( segmentationFileName.c_str() );
    segmentationReader->Update();

    VTK_CREATE( vtkContourVisualizationModule, newContourModule );
    contourModules.push_back( newContourModule );

    newContourModule->SetIsoValue( isoValue );
    newContourModule->SetContourColor( 1, 0, 0 );
    newContourModule->SetContourVisibility( 1 );
    newContourModule->SetPlaneOrigin( seedPoint );
    newContourModule->SetPlaneOrientation( vtkContourVisualizationModule::SLICE_ORIENTATION_XY );
    newContourModule->SetSegmentation( segmentationReader->GetOutput() );
 
    newContourModule->Update();

    renderer->AddActor( newContourModule->GetActor() );
    }


  // Bring up the render window and begin interaction.
  renderer->ResetCamera();

  vtkCamera * camera = renderer->GetActiveCamera();

  camera->SetParallelProjection( true );
  renderer->ResetCamera();
  camera->Zoom(1.2);

  if ( produceScreenshot ) 
    {
    VTK_CREATE( vtkWindowToImageFilter, windowToImageFilter );
    VTK_CREATE( vtkPNGWriter, screenShotWriter );

    windowToImageFilter->SetInput( renderWindow );
    windowToImageFilter->Update();

    std::string screenShotFileName = argv[5];

    std::cout << "Screen shot file name = " << screenShotFileName << std::endl;

    screenShotWriter->SetInput( windowToImageFilter->GetOutput() );
    screenShotWriter->SetFileName( screenShotFileName.c_str() );
    
    renderWindow->Render();
    screenShotWriter->Write();

    screenShotWriter->SetInput( NULL );
    windowToImageFilter->SetInput( NULL );
    }
  else
    {
    renderWindow->Render();
    renderWindowInteractor->Start();
    }

  return EXIT_SUCCESS;
}
