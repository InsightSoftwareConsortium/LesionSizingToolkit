/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    SurfaceExtraction.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or https://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkMetaImageReader.h"
#include "vtkContourFilter.h"
#include "vtkImageData.h"
#include "vtkPolyDataWriter.h"
#include "vtkSTLWriter.h"
#include "vtkSmartPointer.h"
#include "vtksys/SystemTools.hxx"
#include "vtkVersion.h"


#define VTK_CREATE(type, name) vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


int
main(int argc, char * argv[])
{

  if (argc < 4)
  {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " imageFileName isoValue outputSurface";
    std::cerr << std::endl;
    return 1;
  }

  VTK_CREATE(vtkMetaImageReader, imageReader);

  imageReader->SetFileName(argv[1]);
  imageReader->Update();

  float isoValue = atof(argv[2]);

  VTK_CREATE(vtkContourFilter, contourFilter);

  contourFilter->SetValue(0, isoValue);
#if VTK_MAJOR_VERSION <= 5
  contourFilter->SetInput(imageReader->GetOutput());
#else
  contourFilter->SetInputData(imageReader->GetOutput());
#endif


  std::string surfaceFileNameExtension = vtksys::SystemTools::GetFilenameLastExtension(argv[3]);

  if (surfaceFileNameExtension == ".vtk")
  {
    VTK_CREATE(vtkPolyDataWriter, polyDataWriter);
#if VTK_MAJOR_VERSION <= 5
    polyDataWriter->SetInput(contourFilter->GetOutput());
#else
    polyDataWriter->SetInputData(contourFilter->GetOutput());
#endif
    polyDataWriter->SetFileName(argv[3]);
    polyDataWriter->Update();
  }

  if (surfaceFileNameExtension == ".stl")
  {
    VTK_CREATE(vtkSTLWriter, stlWriter);
#if VTK_MAJOR_VERSION <= 5
    stlWriter->SetInput(contourFilter->GetOutput());
#else
    stlWriter->SetInputData(contourFilter->GetOutput());
#endif
    stlWriter->SetFileName(argv[3]);
    stlWriter->Update();
  }

  return EXIT_SUCCESS;
}
