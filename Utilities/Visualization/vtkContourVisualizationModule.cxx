/*=========================================================================

  Program:   Image Analysis Platform
  Module:    vtkContourVisualizationModule.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Kitware Inc. All rights reserved.
  See Copyright.txt or https://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkContourVisualizationModule.h"
#include "vtkObjectFactory.h"
#include "vtkContourFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkImageData.h"
#include "vtkImageReslice.h"
#include "vtkVersion.h"

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkContourVisualizationModule);

vtkContourVisualizationModule::vtkContourVisualizationModule()
{
  this->ContourFilter = vtkContourFilter::New();
  this->PolyDataMapper = vtkPolyDataMapper::New();
  this->Actor = vtkActor::New();
  this->Property = vtkProperty::New();
  this->ResliceFilter = vtkImageReslice::New();

  this->ResliceFilter->SetOutputDimensionality(3);
  this->ResliceFilter->SetInterpolationModeToLinear();
#if VTK_MAJOR_VERSION <= 5
  this->ContourFilter->SetInput(this->ResliceFilter->GetOutput());
  this->PolyDataMapper->SetInput(this->ContourFilter->GetOutput());
#else
  this->ContourFilter->SetInputData(this->ResliceFilter->GetOutput());
  this->PolyDataMapper->SetInputData(this->ContourFilter->GetOutput());
#endif

  this->PolyDataMapper->ScalarVisibilityOff();
  this->PolyDataMapper->SetResolveCoincidentTopologyToPolygonOffset();

  this->Actor->SetMapper(this->PolyDataMapper);

  this->Property->SetRepresentationToSurface();
  this->Property->SetAmbient(0.1);
  this->Property->SetDiffuse(0.1);
  this->Property->SetSpecular(0.5);
  this->Property->SetLineWidth(2.0);
  this->Property->SetColor(1.0, 0.0, 0.0);
  this->Actor->SetProperty(this->Property);


  this->Color[0] = 1.0;
  this->Color[1] = 0.0;
  this->Color[2] = 0.0;

  this->Visibility = 1;

  this->SlicePosition[0] = 0;
  this->SlicePosition[1] = 0;
  this->SlicePosition[2] = 0;

  this->SliceOrientation = SLICE_ORIENTATION_XY;
  this->IsoValueSetByUser = false;
}

vtkContourVisualizationModule::~vtkContourVisualizationModule()
{
  if (this->ContourFilter)
  {
    this->ContourFilter->Delete();
  }
  if (this->Actor)
  {
    this->Actor->Delete();
  }
  if (this->PolyDataMapper)
  {
    this->PolyDataMapper->Delete();
  }
  if (this->Property)
  {
    this->Property->Delete();
  }
  if (this->ResliceFilter)
  {
    this->ResliceFilter->Delete();
  }
}

void
vtkContourVisualizationModule::SetSegmentation(vtkImageData * input)
{
  // Draw contours around the segmented regions
#if VTK_MAJOR_VERSION <= 5
  this->ResliceFilter->SetInput(input);
#else
  this->ResliceFilter->SetInputData(input);
#endif
}

void
vtkContourVisualizationModule::SetAutoIsoValue()
{
  double range[2];
  this->GetScalarRange(range);
  this->ContourFilter->SetValue(0, (range[1] + range[0]) / 2.0);
}

double
vtkContourVisualizationModule::GetIsoValue()
{
  return this->ContourFilter->GetValue(0);
}

void
vtkContourVisualizationModule::SetContourColor(double r, double g, double b)
{
  this->Color[0] = r;
  this->Color[1] = g;
  this->Color[2] = b;

  this->Property->SetColor(r, g, b);
  this->Modified();
}

void
vtkContourVisualizationModule::GetContourColor(double & r, double & g, double & b)
{
  r = this->Color[0];
  g = this->Color[1];
  b = this->Color[2];
}

int
vtkContourVisualizationModule::GetVisibility()
{
  return this->Visibility;
}

void
vtkContourVisualizationModule::SetContourVisibility(int state)
{
  this->Visibility = state;
  this->Actor->SetVisibility(this->Visibility);
  this->Modified();
}

vtkActor *
vtkContourVisualizationModule::GetActor()
{
  return this->Actor;
}

void
vtkContourVisualizationModule::SetPlaneOrigin(double origin[3])
{
  this->SlicePosition[0] = origin[0];
  this->SlicePosition[1] = origin[1];
  this->SlicePosition[2] = origin[2];
  this->Modified();
}

void
vtkContourVisualizationModule::Update()
{
  vtkImageData * segmentation = dynamic_cast<vtkImageData *>(this->ResliceFilter->GetInput());

  double * segmentationOrigin = segmentation->GetOrigin();

  double sliceOrigin[3];

  sliceOrigin[0] = segmentationOrigin[0];
  sliceOrigin[1] = segmentationOrigin[1];
  sliceOrigin[2] = segmentationOrigin[2];

  int * segmentationExtent = segmentation->GetExtent();

  int sliceExtent[6];

  sliceExtent[0] = segmentationExtent[0];
  sliceExtent[1] = segmentationExtent[1];
  sliceExtent[2] = segmentationExtent[2];
  sliceExtent[3] = segmentationExtent[3];
  sliceExtent[4] = segmentationExtent[4];
  sliceExtent[5] = segmentationExtent[5];


  switch (this->SliceOrientation)
  {
    case SLICE_ORIENTATION_YZ:
    {
      sliceExtent[0] = 0;
      sliceExtent[1] = 0;
      sliceOrigin[0] = this->SlicePosition[0];
      break;
    }
    case SLICE_ORIENTATION_XZ:
    {
      sliceExtent[2] = 0;
      sliceExtent[3] = 0;
      sliceOrigin[1] = this->SlicePosition[1];
      break;
    }
    case SLICE_ORIENTATION_XY:
    {
      sliceExtent[4] = 0;
      sliceExtent[5] = 0;
      sliceOrigin[2] = this->SlicePosition[2];
      break;
    }
  }

  this->ResliceFilter->SetOutputOrigin(sliceOrigin);

  this->ResliceFilter->SetOutputSpacing(segmentation->GetSpacing());

  this->ResliceFilter->SetOutputExtent(
    sliceExtent[0], sliceExtent[1], sliceExtent[2], sliceExtent[3], sliceExtent[4], sliceExtent[5]);

  if (!this->IsoValueSetByUser)
  {
    this->SetAutoIsoValue();
  }

  this->ContourFilter->Update();

  this->ContourFilter->GetOutput()->Print(std::cout);
}

void
vtkContourVisualizationModule::SetPlaneOrientation(int orientation)
{
  this->SliceOrientation = orientation;
  this->Modified();
}

void
vtkContourVisualizationModule::ForceContourUpdate()
{
  this->ResliceFilter->Modified();
  this->ContourFilter->Update();
}

void
vtkContourVisualizationModule::SetIsoValue(double isovalue)
{
  this->ContourFilter->SetValue(0, isovalue);
  this->IsoValueSetByUser = true;
}

void
vtkContourVisualizationModule::GetScalarRange(double range[2])
{
  range[0] = VTK_DOUBLE_MAX;
  range[1] = VTK_DOUBLE_MIN;

  vtkImageData * segmentation = dynamic_cast<vtkImageData *>(this->ResliceFilter->GetInput());
  if (segmentation)
  {
    segmentation->GetScalarRange(range);
    // deal with case of new segmentation
    if (range[0] == range[1] && segmentation->GetScalarType() == VTK_UNSIGNED_CHAR)
    {
      range[0] = 0;
      range[1] = 255;
    }
  }
}
