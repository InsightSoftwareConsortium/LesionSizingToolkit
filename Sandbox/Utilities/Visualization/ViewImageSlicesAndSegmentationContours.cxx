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

#include "itkCommand.h"
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "itkConfidenceConnectedImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkActor.h"
#include "vtkInteractorStyleImage.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkContourFilter.h"
#include "vtkImageData.h"
#include "vtkDataSet.h"
#include "vtkProperty.h"
#include "vtkImagePlaneWidget.h"
#include "vtkCellPicker.h"
#include "vtkPolyDataWriter.h"
#include "vtkSmartPointer.h"


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()

/**
 * This function will connect the given itk::VTKImageExport filter to
 * the given vtkImageImport filter.
 */
template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}

/**
 * This function will connect the given vtkImageExport filter to
 * the given itk::VTKImageImport filter.
 */
template <typename VTK_Exporter, typename ITK_Importer>
void ConnectPipelines(VTK_Exporter* exporter, ITK_Importer importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}


/**
 * This program implements an example connection between ITK and VTK
 * pipelines.  The combined pipeline flows as follows:
 *
 * itkImageFileReader ==> itkVTKImageExport ==>
 *    vtkImageImport ==> vtkImagePlaneWidget
 *
 * The resulting vtkImagePlaneWidget is displayed in a vtkRenderWindow.
 * Whenever the VTK pipeline executes, information is propagated
 * through the ITK pipeline.  If the ITK pipeline is out of date, it
 * will re-execute and cause the VTK pipeline to update properly as
 * well.
 */
int main(int argc, char * argv [] )
{  

  // Load a scalar image and a segmentation mask and display the
  // scalar image with the contours of the mask overlaid on it.

  if( argc < 3 )
    {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " imageFileName maskFileName";
    std::cerr << " [isocontourValue] [representation:0=surface/1=wireframe]";
    std::cerr << " [outputSurface.vtk]"; 
    std::cerr << std::endl;
    return 1;
    }
  
  try
    {
    typedef signed short    PixelType;
    typedef float           MaskPixelType;

    const unsigned int Dimension = 3;

    typedef itk::Image< PixelType, Dimension > ImageType;

    typedef itk::Image< MaskPixelType, Dimension > MaskImageType;
    
    typedef itk::ImageFileReader< ImageType > ReaderType;

    typedef itk::ImageFileReader< MaskImageType > MaskReaderType;

    ReaderType::Pointer reader  = ReaderType::New();
    reader->SetFileName( argv[1] );
    reader->Update();

    MaskReaderType::Pointer maskReader  = MaskReaderType::New();
    maskReader->SetFileName( argv[2] );
    maskReader->Update();

    
    typedef itk::VTKImageExport< ImageType >     ExportFilterType;
    typedef itk::VTKImageExport< MaskImageType > MaskExportFilterType;

    ExportFilterType::Pointer     itkExporter1 = ExportFilterType::New();
    MaskExportFilterType::Pointer itkExporter2 = MaskExportFilterType::New();

    itkExporter1->SetInput( reader->GetOutput() );
    itkExporter2->SetInput( maskReader->GetOutput() );

    // Create the vtkImageImport and connect it to the
    // itk::VTKImageExport instance.
    VTK_CREATE( vtkImageImport, vtkImporter1 );
    ConnectPipelines(itkExporter1, vtkImporter1.GetPointer() );
    
    VTK_CREATE( vtkImageImport, vtkImporter2 );
    ConnectPipelines(itkExporter2, vtkImporter2.GetPointer() );
    

    vtkImporter1->Update();
     
    //------------------------------------------------------------------------
    // VTK pipeline.
    //------------------------------------------------------------------------

    // Create a renderer, render window, and render window interactor to
    // display the results.
    VTK_CREATE( vtkRenderer, renderer );
    VTK_CREATE( vtkRenderWindow, renWin );
    VTK_CREATE( vtkRenderWindowInteractor, iren );

    VTK_CREATE(vtkInteractorStyleImage, interactorStyle);

    renWin->SetSize(600, 600);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);
    iren->SetInteractorStyle( interactorStyle );
    

    // use cell picker for interacting with the image orthogonal views.
    //
    VTK_CREATE(vtkCellPicker , picker);
    picker->SetTolerance(0.005);


    //assign default props to the ipw's texture plane actor
    VTK_CREATE(vtkProperty , ipwProp);
     

    // Create 3 orthogonal view using the ImagePlaneWidget
    //
    VTK_CREATE(vtkImagePlaneWidget , xImagePlaneWidget);
    VTK_CREATE(vtkImagePlaneWidget , yImagePlaneWidget);
    VTK_CREATE(vtkImagePlaneWidget , zImagePlaneWidget);

    // The 3 image plane widgets are used to probe the dataset.
    //
    xImagePlaneWidget->DisplayTextOn();
    xImagePlaneWidget->SetInput(vtkImporter1->GetOutput());
    xImagePlaneWidget->SetPlaneOrientationToXAxes();
    xImagePlaneWidget->SetSliceIndex(0);
    xImagePlaneWidget->SetPicker(picker);
    xImagePlaneWidget->RestrictPlaneToVolumeOn();
    xImagePlaneWidget->SetKeyPressActivationValue('x');
    xImagePlaneWidget->GetPlaneProperty()->SetColor(1, 0, 0);
    xImagePlaneWidget->SetTexturePlaneProperty(ipwProp);
    xImagePlaneWidget->SetResliceInterpolateToNearestNeighbour();

    yImagePlaneWidget->DisplayTextOn();
    yImagePlaneWidget->SetInput(vtkImporter1->GetOutput());
    yImagePlaneWidget->SetPlaneOrientationToYAxes();
    yImagePlaneWidget->SetSliceIndex(0);
    yImagePlaneWidget->SetPicker(picker);
    yImagePlaneWidget->RestrictPlaneToVolumeOn();
    yImagePlaneWidget->SetKeyPressActivationValue('y');
    yImagePlaneWidget->GetPlaneProperty()->SetColor(1, 1, 0);
    yImagePlaneWidget->SetTexturePlaneProperty(ipwProp);
    yImagePlaneWidget->SetLookupTable(xImagePlaneWidget->GetLookupTable());

    zImagePlaneWidget->DisplayTextOn();
    zImagePlaneWidget->SetInput(vtkImporter1->GetOutput());
    zImagePlaneWidget->SetPlaneOrientationToZAxes();
    zImagePlaneWidget->SetSliceIndex(0);
    zImagePlaneWidget->SetPicker(picker);
    zImagePlaneWidget->SetKeyPressActivationValue('z');
    zImagePlaneWidget->GetPlaneProperty()->SetColor(0, 0, 1);
    zImagePlaneWidget->SetTexturePlaneProperty(ipwProp);
    zImagePlaneWidget->SetLookupTable(xImagePlaneWidget->GetLookupTable());

    xImagePlaneWidget->SetInteractor( iren );
    xImagePlaneWidget->On();
     
    yImagePlaneWidget->SetInteractor( iren );
    yImagePlaneWidget->On();
     
    zImagePlaneWidget->SetInteractor( iren );
    zImagePlaneWidget->On();


    // Set the background to something grayish
    renderer->SetBackground(0.4392, 0.5020, 0.5647);


    // Draw contours around the segmented regions
    VTK_CREATE(vtkContourFilter , contour);
    contour->SetInput( vtkImporter2->GetOutput() );

    MaskPixelType contourValue = 0.0;

    if( argc > 3 )
      {
      contourValue = atoi( argv[3] );
      }

    contour->SetValue( 0, contourValue ); 


    VTK_CREATE(vtkPolyDataMapper , polyMapper);
    VTK_CREATE(vtkActor          , polyActor );

    polyActor->SetMapper( polyMapper );
    polyMapper->SetInput( contour->GetOutput() );

    VTK_CREATE(vtkProperty , property);
    property->SetAmbient(0.1);
    property->SetDiffuse(0.1);
    property->SetSpecular(0.5);
    property->SetColor(1.0,0.0,0.0);
    property->SetLineWidth(2.0);
    property->SetRepresentationToSurface();

    polyActor->SetProperty( property );
  
    renderer->AddActor( polyActor );
    

    if( argc > 4 )
      {
      int representation = atoi( argv[4] );
      switch(representation)
        {
        case 0:
          property->SetRepresentationToSurface();
          break;
        case 1:
          property->SetRepresentationToWireframe();
          break;
        }
      }

    if( argc > 5 )
      {
      VTK_CREATE(vtkPolyDataWriter , writer);
      writer->SetFileName(argv[5]);
      writer->SetInput( contour->GetOutput() );
      writer->Write();
      }

    // Bring up the render window and begin interaction.
    renderer->ResetCamera();
    renWin->Render();
    iren->Start();

    }
  catch( itk::ExceptionObject & e )
    {
    std::cerr << "Exception catched !! " << e << std::endl;
    }


  return 0;
}
