/*=========================================================================

  Program:   Image Analysis Platform
  Module:    vtkContourVisualizationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Kitware Inc. All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __vtkContourVisualizationModule_h
#define __vtkContourVisualizationModule_h

#include "vtkObject.h"
#include "itkVector.h"

class vtkContourFilter;
class vtkPolyDataMapper;
class vtkActor;
class vtkProperty;
class vtkImageData;
class vtkImageReslice;


class vtkContourVisualizationModule : public vtkObject
{
public:
  static vtkContourVisualizationModule *
  New();

  // Image that holds the segmentation represented as a grayscale map This
  // representation is similar to a level set. The object is defined by the
  // iso-contour at level 127.5
  void
  SetSegmentation(vtkImageData *);

  // Origin and Orientation of the plane for which the contour is going to be
  // visualized.  This class only does visualization in orthogonal planes.
  // This class uses the Cutter filter in order to extract one slice from the
  // image, then applies the Contour filter to that slice. The resulting
  // contour as a vtkPolyData structure is passed to an Actor for
  // visualization.
  void
  SetPlaneOrigin(double origin[3]);
  void
  SetPlaneOrientation(int orientation);

  // Return the actor that is capable of displaying the extracted contour.
  // This actor can now be added to a vtkRenderer in order to compose a
  // visualization of the image and the overlapping contour of the
  // segmentation.
  vtkActor *
  GetActor();

  // set the color of the contour
  void
  SetContourColor(double r, double g, double b);

  // Get contour color
  void
  GetContourColor(double & r, double & g, double & b);

  // Set the visibility of the contour
  void
  SetContourVisibility(int state);

  // Get the visibility of the contour
  int
  GetVisibility();

  // Get the scalar range of the input image. Returns
  // (VTK_DOUBLE_MAX, VTK_DOUBLE_MIN) if no data exists.
  void
  GetScalarRange(double range[2]);

  // Get the iso-value
  double
  GetIsoValue();

  void
  SetIsoValue(double isovalue);

  // Update the contour filter to reflect the current state
  // of the segmentation.
  void
  Update();

  // Update the contour filter to reflect the current state
  // of the segmentation.
  void
  ForceContourUpdate();

  // Slice orientations used by ImageViewer
  // BTX
  enum
  {
    SLICE_ORIENTATION_YZ = 0,
    SLICE_ORIENTATION_XZ = 1,
    SLICE_ORIENTATION_XY = 2
  };
  // ETX

protected:
  vtkContourVisualizationModule();
  ~vtkContourVisualizationModule();

private:
  vtkContourVisualizationModule(const vtkContourVisualizationModule &); // Not implemented.
  void
  operator=(const vtkContourVisualizationModule &); // Not implemented.

  // Compute the iso-value from the data range
  void
  SetAutoIsoValue();

  // Objects need for displaying contours
  vtkContourFilter *  ContourFilter;
  vtkPolyDataMapper * PolyDataMapper;
  vtkActor *          Actor;
  vtkProperty *       Property;
  vtkImageReslice *   ResliceFilter;

  // properites that will be used by the visualization interface
  int    Visibility;
  double Color[3];

  // Position of the currently selected point in 3D
  // The slices will be cut across this point.
  double SlicePosition[3];

  // Orientation of the current slice across the dataset
  int SliceOrientation;

  // Indicate whether the user has called SetIsoValue() or not.
  bool IsoValueSetByUser;
};

#endif
