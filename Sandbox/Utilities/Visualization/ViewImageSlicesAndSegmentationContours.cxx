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

#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

int main(int argc, char * argv [] )
{  

  // Load a scalar image and a segmentation mask and display the
  // scalar image with the contours of the mask overlaid on it.

  if( argc < 3 )
    {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " imageFileName segmentationFilename";
    std::cerr << "[segmentationFilename2,...n]";
    std::cerr << std::endl;
    return 1;
    }
  
  typedef vtkSmartPointer< vtkContourVisualizationModule >  vtkContourVisualizationModulePointer;
  typedef std::vector< vtkContourVisualizationModulePointer >    ContoursContainer;

  ContoursContainer  contourModules;

  VTK_CREATE( vtkMetaImageReader, imageReader );

  imageReader->SetFileName( argv[1] );
  imageReader->Update();


  VTK_CREATE( vtkImageViewer2, imageViewer );
  VTK_CREATE( vtkRenderWindow, renderWindow );
  VTK_CREATE( vtkRenderer,     renderer );
  VTK_CREATE( vtkRenderWindowInteractor, renderWindowInteractor );

  renderWindow->SetSize(600, 600);
  renderWindow->AddRenderer(renderer);
  renderWindowInteractor->SetRenderWindow(renderWindow);

  // Set the background to something grayish
  renderer->SetBackground(0.4392, 0.5020, 0.5647);

  // set the interpolation type to nearest neighbour
  imageViewer->GetImageActor()->SetInterpolate( 0 );

  imageViewer->SetInput( imageReader->GetOutput() );

  renderer->AddActor( imageViewer->GetImageActor() );

  const unsigned int numberOfSegmentations = argc - 2;

  for(unsigned int segmentationId=0; segmentationId < numberOfSegmentations; segmentationId++)
    {
    VTK_CREATE( vtkMetaImageReader, segmentationReader );
    segmentationReader->SetFileName( argv[segmentationId+2] );
    segmentationReader->Update();

    VTK_CREATE( vtkContourVisualizationModule, newContourModule );
    contourModules.push_back( newContourModule );

    newContourModule->SetSegmentation( segmentationReader->GetOutput() );
    renderer->AddActor( newContourModule->GetActor() );
    }


  // Bring up the render window and begin interaction.
  renderer->ResetCamera();
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
