/*=========================================================================

Copyright (c) 1998-2003 Kitware Inc. 469 Clifton Corporate Parkway,
Clifton Park, NY, 12065, USA.

All rights reserved. No part of this software may be reproduced, distributed,
or modified, in any form or by any means, without permission in writing from
Kitware Inc.

IN NO EVENT SHALL THE AUTHORS OR DISTRIBUTORS BE LIABLE TO ANY PARTY FOR
DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
OF THE USE OF THIS SOFTWARE, ITS DOCUMENTATION, OR ANY DERIVATIVES THEREOF,
EVEN IF THE AUTHORS HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

THE AUTHORS AND DISTRIBUTORS SPECIFICALLY DISCLAIM ANY WARRANTIES, INCLUDING,
BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE, AND NON-INFRINGEMENT.  THIS SOFTWARE IS PROVIDED ON AN
"AS IS" BASIS, AND THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE
MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.

=========================================================================*/
#ifndef vtkVVPluginAPI_h
#define vtkVVPluginAPI_h

#ifdef  __cplusplus
extern "C" {
#endif

/*=========================================================================

The following are the different types of parameters that the plugin API
  supports 
  
VVP_ERROR 

VVP_GROUP 
  what group in the GUI should this plugin be listed under, if this is not
  set it will fall under miscelaneous

VVP_NAME - a short name for the plugin to be used in the menu to select it 

VVP_TERSE_DOCUMENTATION - a short one sentence description of the plugin 

VVP_FULL_DOCUMENTATION 
  a long description of the plugin and how it works, can be very long 

VVP_SUPPORTS_IN_PLACE_PROCESSING
  does this plugin support processing the data in place. Normally the output
  volume is a separate block of memory from the input data. But for some
  filters they can write the output directly onto the input volume. This is
  called InPlace processing

VVP_SUPPORTS_PROCESSING_PIECES 
  does this plugin support procesing the data in pieces (slabs) 

VVP_NUMBER_OF_GUI_ITEMS 
  specify the GUI for this plugin. How many elements are there? 

VVP_PRODUCES_MESH_ONLY - does this plugin only produce a mesh? 

VVP_REQUIRED_Z_OVERLAP
  for plugins that support processing pieces, does it require any Z axis
  overlap. If so how many slices of overlap are required 

VVP_PER_VOXEL_MEMORY_REQUIRED
  an estimate of how much memory per input voxel component will be
  consumed by the filter in bytes. This does not include the input and
  output volumes but does include intermediate results. 

VVP_ABORT_PROCESSING  
  a flag indicating that the plugin should abort processing

VVP_REPORT_TEXT
  a string to be displayed in the GUI. This string can be used to provide
  information on the execution of the plugin. Exmaples of output include
  factors such as run time, number of polygons produced, number of iterations
  run, resulting error etc.

VVP_REQUIRES_SECOND_INPUT
  indicates that this plugin requires a second volume input to be
  provided. When set this will cause VolView to prompt for the second input
  and use it to fill in the plugin structure members for the second input.

VVP_SECOND_INPUT_OPTIONAL
  indicates that the second input that the plugin has is optional. Note that 
  if you need a second input, you must have VVP_REQUIRES_SECOND_INPUT. (This
  mandates the second input to be read prior to executing the filter). If both
  VVP_REQUIRES_SECOND_INPUT and VVP_SECOND_INPUT_OPTIONAL are present, the
  second input wizard will still be available. However the filter may be 
  executed both in the presence of the second input and in the absence of one.
  It is the filter author's responsibility to ensure that the filter runs 
  in both cases. Using the VVP_SECOND_INPUT_OPTIONAL without 
  VVP_REQUIRES_SECOND_INPUT is incorrect
  

VVP_REQUIRES_SPLINE_SURFACES
  indicates that this plugin requires one or more spline surfaces as input. 
  When set this will cause VolView to resample the existing spline surfaces
  to a resolution comparable to that of the data set. This can eventually 
  be a time consuming process.

VVP_NUMBER_OF_MARKERS_GROUPS
  specify the number of groups in which markers can be classified.

VVP_MARKERS_GROUP_NAME - a short name for identifying a group of 3D markers.
  
VVP_MARKERS_GROUP_DESCRIPTION
  a long description of the characteristics of this group of markers.

VVP_MARKERS_GROUP_COLOR
  a RGB triplet in with components in the range [0:255].

VVP_REQUIRES_SERIES_INPUT
  indicates that this plugin requires a series of filenames representing 
  a set of volumes potentially from a time series set. The actual differences
  are left to the interpretation of the plugin.

VVP_SUPPORTS_PROCESSING_SERIES_BY_VOLUMES  
  indicates that this plugin can process a series of volumes receiving them 
  one by one. This feature is useful for reducing the memory footprint of the
  plugin.

VVP_PRODUCES_OUTPUT_SERIES 
  indicates that this plugin produces a volume series as output. The series 
  is received by VolView and saved as individual volumes in files. The user
  is prompted for a base filename, and a series number is added to the base
  filename in order to create unique and consecutive names.

VVP_PRODUCES_PLOTTING_OUTPUT
  indicates that this plugin produces as output an array of data to be plotted
  in 2D. Volview will receive the array of data as a NxM matrix and will use
  the first column as the indepentent variable to be plotted. The N-1 other
  columns will be interpreted as dependent series in the plot.

VVP_PLOTTING_X_AXIS_TITLE 
  Title to be used for the X axis in plugins that return data to be plotted.

VVP_PLOTTING_Y_AXIS_TITLE 
  Title to be used for the X axis in plugins that return data to be plotted.

=========================================================================*/

#define VV_PLUGIN_API_VERSION 1
  
/* These types are returned by GetDataType to indicate pixel type. */
#ifndef VTK_VOID  
#define VTK_VOID            0
#define VTK_BIT             1 
#define VTK_CHAR            2
#define VTK_UNSIGNED_CHAR   3
#define VTK_SHORT           4
#define VTK_UNSIGNED_SHORT  5
#define VTK_INT             6
#define VTK_UNSIGNED_INT    7
#define VTK_LONG            8
#define VTK_UNSIGNED_LONG   9
#define VTK_FLOAT          10
#define VTK_DOUBLE         11 
#define VTK_ID_TYPE        12
#endif
  
/* the following defines enumerate the different GUI components supported by
 * plugins.  */
#define VV_GUI_SCALE     0
#define VVP_GUI_SCALE    "scale"
#define VV_GUI_CHOICE    1
#define VVP_GUI_CHOICE   "choice"
#define VV_GUI_CHECKBOX  2
#define VVP_GUI_CHECKBOX "checkbox"

#define VVP_ERROR                         0
#define VVP_NAME                          1
#define VVP_TERSE_DOCUMENTATION           2
#define VVP_FULL_DOCUMENTATION            3
#define VVP_SUPPORTS_IN_PLACE_PROCESSING  4
#define VVP_SUPPORTS_PROCESSING_PIECES    5 
#define VVP_NUMBER_OF_GUI_ITEMS           6
#define VVP_PRODUCES_MESH_ONLY            7
#define VVP_REQUIRED_Z_OVERLAP            8
#define VVP_PER_VOXEL_MEMORY_REQUIRED     9
#define VVP_ABORT_PROCESSING             10  
#define VVP_REPORT_TEXT                  11
#define VVP_GROUP                        12
#define VVP_REQUIRES_SECOND_INPUT        13

#define VVP_INPUT_DISTANCE_UNITS         14
#define VVP_INPUT_COMPONENT_1_UNITS      15
#define VVP_INPUT_COMPONENT_2_UNITS      16
#define VVP_INPUT_COMPONENT_3_UNITS      17
#define VVP_INPUT_COMPONENT_4_UNITS      18
#define VVP_INPUT_COMPONENTS_ARE_INDEPENDENT 19

#define VVP_INPUT_2_DISTANCE_UNITS         20
#define VVP_INPUT_2_COMPONENT_1_UNITS      21
#define VVP_INPUT_2_COMPONENT_2_UNITS      22
#define VVP_INPUT_2_COMPONENT_3_UNITS      23
#define VVP_INPUT_2_COMPONENT_4_UNITS      24
#define VVP_INPUT_2_COMPONENTS_ARE_INDEPENDENT 25

#define VVP_RESULTING_DISTANCE_UNITS     26
#define VVP_RESULTING_COMPONENT_1_UNITS  27
#define VVP_RESULTING_COMPONENT_2_UNITS  28
#define VVP_RESULTING_COMPONENT_3_UNITS  29
#define VVP_RESULTING_COMPONENT_4_UNITS  30
#define VVP_RESULTING_COMPONENTS_ARE_INDEPENDENT 31
  
#define VVP_MARKERS_GROUP_NAME 32
#define VVP_MARKERS_GROUP_DESCRIPTION 33
#define VVP_MARKERS_GROUP_COLOR 34
#define VVP_NUMBER_OF_MARKERS_GROUPS 35

#define VVP_REQUIRES_SPLINE_SURFACES     36

#define VVP_REQUIRES_SERIES_INPUT        37
#define VVP_SUPPORTS_PROCESSING_SERIES_BY_VOLUMES  38
#define VVP_PRODUCES_OUTPUT_SERIES 39
#define VVP_PRODUCES_PLOTTING_OUTPUT 40
#define VVP_PLOTTING_X_AXIS_TITLE 41
#define VVP_PLOTTING_Y_AXIS_TITLE 42

#define VVP_SECOND_INPUT_OPTIONAL  43

#define VVP_GUI_LABEL   0
#define VVP_GUI_TYPE    1
#define VVP_GUI_DEFAULT 2
#define VVP_GUI_HELP    3
#define VVP_GUI_HINTS   4
#define VVP_GUI_VALUE   5
  
  
/*=========================================================================
CM_PLUGIN_EXPORT should be used by the Init function of all plugins
=========================================================================*/
#ifdef _WIN32
#define VV_PLUGIN_EXPORT  __declspec( dllexport )
#else
#define VV_PLUGIN_EXPORT 
#endif

/*=========================================================================
Finally we define the key data structures and function prototypes
The first struture here is the vtkVVProcessDataStruct. This structure
gets passed into the ProcessData function.
=========================================================================*/
  typedef float MarkersCoordinatesType;
  typedef unsigned int MarkersGroupIdType;
  typedef unsigned char MarkersGroupColorComponentType;
  typedef struct {
    /* the input data pointer */
    void *inData; 
    /* the second input data pointer (for two input plugins) */
    void *inData2; 
    /* the output data pointer */
    void *outData; 
    /* what slice to start processing on (used for pieces) */    
    int StartSlice;         
    /* how many slices to process right now */
    int NumberOfSlicesToProcess;
    /* if this plugin produces a mesh / surface store it */
    /* in the following entries, these are all set by the plugin */
    /* and allocated by the plugin. VolView will copy them into */
    /* its own data structures, the format of these arrays follows */
    /* VTKs basic vtkPolyData structure */
    int NumberOfMeshPoints;
    float *MeshPoints;
    int NumberOfMeshCells;
    int *MeshCells;
    float *MeshNormals;
    float *MeshScalars;
    /* the series input data pointer (for series input plugins) */
    void *inDataSeries; 
    /* what volume to we are processing (used for series in volume-wise mode) */    
    int CurrentVolumeFromSeries;         
    /* the series output data pointer (for plugins producing output series) */
    void *outDataSeries; 
    /* the plotting output data pointer (for plugins producing plotting output) */
    void *outDataPlotting; 
  } vtkVVProcessDataStruct;

  typedef struct {
    /* these three members should not used by the plugin */
    unsigned char magic1;
    unsigned char magic2;
    void *Self;

    /* these are the characteristics of the input data */
    int InputVolumeScalarType;
    int InputVolumeScalarSize;
    int InputVolumeNumberOfComponents;
    int InputVolumeDimensions[3];
    float InputVolumeSpacing[3];
    float InputVolumeOrigin[3];
    double InputVolumeScalarRange[8]; /* actual scalar range */
    double InputVolumeScalarTypeRange[2]; /* possible scalar range */

    /* if there is a second input here are its characteristics */
    int InputVolume2ScalarType;
    int InputVolume2ScalarSize;
    int InputVolume2NumberOfComponents;
    int InputVolume2Dimensions[3];
    float InputVolume2Spacing[3];
    float InputVolume2Origin[3];
    double InputVolume2ScalarRange[8]; /* actual scalar range */
    double InputVolume2ScalarTypeRange[2]; /* possible scalar range */
    
    int NumberOfMarkers;
    float *Markers; /* the xyz positions of the seed points/markers */
    
    /* the -x x -y y -z z clopping planes in world coordinates, */
    /* there will be six values */
    float *CroppingPlanes; 
    
    /* specify the charateristics of the output data */
    int OutputVolumeScalarType;
    int OutputVolumeNumberOfComponents;
    int OutputVolumeDimensions[3];
    float OutputVolumeSpacing[3];
    float OutputVolumeOrigin[3];

    /* these are the methods that a plugin can invoke on VolView */
    /* the following function can be called to update progress */
    void  (*UpdateProgress) (void *info, float progress, const char *msg);

    /* for plugins that generate polygonal data they should call the */
    /* following function to assign the polygonal data to VolView */
    /* after which they can free any local memory that was being used */
    void  (*AssignPolygonalData) (void *info, vtkVVProcessDataStruct *pds);

    /* memory safe way to set/get properties */
    void        (*SetProperty) (void *info, int property, const char *value);
    const char *(*GetProperty) (void *info, int property);
    void        (*SetGUIProperty) (void *info, int num, int property, 
                                   const char *value);
    const char *(*GetGUIProperty) (void *info, int num, int property);

    /* you must implement the two following functions */
    int (*ProcessData) (void *info, vtkVVProcessDataStruct *pds);
    int (*UpdateGUI)   (void *info);

    MarkersGroupIdType     *MarkersGroupId; /* the Id of the group each marker belongs to */

    int NumberOfMarkersGroups; /* Number of categories for the markers */
    char * *MarkersGroupName;  /* Text names for the groups of markers */
    MarkersGroupColorComponentType *MarkersGroupColor; /* RGB color components for the group */

    /* Number of spline surfaces. These surfaces will be passed as an array of points */
    int NumberOfSplineSurfaces; 

    /* Number of points for every surface. This is not the number of control
     * points but the actual set of points used for representing the surface.
     * This is an array : n1,n2,n3...nm,  for the "m" surfaces */
    int *NumberOfSplineSurfacePoints; 

    /* Coordinates of the surface points. This is an array where the "u" is the
     * fastest changing index, and "v" is the slowest changing index. The
     * points of one surface are immediately followed by the points of the next
     * surface, if any */
    float * SplineSurfacePoints;

    /* List of filenames for the input time series.
     * 
     */
    char ** SeriesFileNames;
    
    /* if there is a series input here are its characteristics.
     * Note that it is assumed that ALL the volumes in the series are
     * coincident in physica space. That is, they have the same origin, same
     * extent, same pixel type. Therefore, these characteristics are passed a
     * single time and assumed to be pertinent for all the volumes in the
     * series. */
    int InputVolumeSeriesNumberOfVolumes; 
    int InputVolumeSeriesScalarType;
    int InputVolumeSeriesScalarSize;
    int InputVolumeSeriesNumberOfComponents;
    int InputVolumeSeriesDimensions[3];
    float InputVolumeSeriesSpacing[3];
    float InputVolumeSeriesOrigin[3];
    double InputVolumeSeriesScalarRange[8]; /* actual scalar range */
    double InputVolumeSeriesScalarTypeRange[2]; /* possible scalar range */
   
    /* specify the charateristics of the output volume series */
    int OutputVolumeSeriesNumberOfVolumes;         
    int OutputVolumeSeriesScalarType;
    int OutputVolumeSeriesScalarSize;
    int OutputVolumeSeriesNumberOfComponents;
    int OutputVolumeSeriesDimensions[3];
    float OutputVolumeSeriesSpacing[3];
    float OutputVolumeSeriesOrigin[3];

    /* specify the size of the array of plotting values */
    int OutputPlottingNumberOfColumns;
    int OutputPlottingNumberOfRows;

	// ADD NEW ELEMENTS AT THE END PLEASE
	
  } vtkVVPluginInfo;

  /* this is the type of the init function for the plugin. It must be
   * declared as a static "C" function with VV_PLUGIN_EXPORT. The name must
   * be "vv" plus the name of the plugin plus "Init". */
  typedef void (*VV_INIT_FUNCTION)(vtkVVPluginInfo *);
  
#ifdef  __cplusplus
}
#endif


/* OK this macro is useful for plugins that need to set the range of a slider
 * to the range of the voxel's data type. For example the Boundary plugin has
 * a slider where the user can set the value for the boundary voxels. For
 * unsigned char this slider should show 0 to 255. For larger short it would
 * show roughly -32000 to 32000. Both of those are fairly reasonable. The
 * problem comes with larger data types such as float, long, double. The
 * valid range of floats is huge and you probably do not want the slider to
 * cover the entire range (something crazy like -1e37 to 1e37). Son in these
 * cases we use a heuristic to find a "reasonable" range. The basic idea is
 * that we make the slider range the range of the data in the volume and then
 * extend that range by a factor of 3.  So if the user loads float dta
 * between 0 and 1 the range computed here would be -1 to 2 for example. The
 * other reason to do this is that most of the rest of VolView does not
 * handle wee the situation where there are large gaps in the data. For
 * example, float data where most of the structure is in the range of 0 to 1
 * but there are some outlier voxels with a value of 1e37. The texture
 * hardware/GUI doean't work well with these situations (not a bug, just part
 * of the design) so we try to avoid them.
 */
#define vvPluginSetGUIScaleRange(id) \
  { \
  char _tmp[1000]; \
  double _min = info->InputVolumeScalarTypeRange[0]; \
  double _max = info->InputVolumeScalarTypeRange[1]; \
  double _stepSize = 1.0; \
  /* for some large types let us be reasonable */ \
  if (info->InputVolumeScalarType == VTK_FLOAT || \
      info->InputVolumeScalarType == VTK_INT || \
      info->InputVolumeScalarType == VTK_UNSIGNED_INT || \
      info->InputVolumeScalarType == VTK_LONG || \
      info->InputVolumeScalarType == VTK_UNSIGNED_LONG || \
      info->InputVolumeScalarType == VTK_DOUBLE) \
    { \
    /* for non double we can safely do some additional calculations */ \
    if (info->InputVolumeScalarType != VTK_DOUBLE) \
      { \
      _min = 2.0*info->InputVolumeScalarRange[0] - \
            info->InputVolumeScalarRange[1]; \
      _max = 2.0*info->InputVolumeScalarRange[1] - \
            info->InputVolumeScalarRange[0]; \
      if (_min < info->InputVolumeScalarTypeRange[0]) \
        { \
        _min = info->InputVolumeScalarTypeRange[0]; \
        } \
      if (_max > info->InputVolumeScalarTypeRange[1]) \
        { \
        _max = info->InputVolumeScalarTypeRange[1]; \
        } \
      } \
    /* for integer types include zero in the range */ \
    if (info->InputVolumeScalarType == VTK_INT || \
        info->InputVolumeScalarType == VTK_UNSIGNED_INT || \
        info->InputVolumeScalarType == VTK_LONG || \
        info->InputVolumeScalarType == VTK_UNSIGNED_LONG) \
      { \
      if (_min > 0 ) \
        { \
        _min = 0; \
        } \
      if (_max < 0 ) \
        { \
        _max = 0; \
        } \
      } \
    _stepSize = _max*0.005 - _min*0.005; \
    } \
  sprintf(_tmp,"%g %g %g", _min, _max, _stepSize); \
  info->SetGUIProperty(info, id, VVP_GUI_HINTS , _tmp); \
  }
  

/* this macro should be called first inside every Init function to make sure
 * the plugin version matches the volview version */
#define vvPluginVersionCheck() \
  if (info->magic1 >= VV_PLUGIN_API_VERSION) { \
    info->magic1 = VV_PLUGIN_API_VERSION; } \
  else {info->magic1 = 0; return; }

// The vtkTemplateMacro is used to centralize the set of types
// supported by Execute methods.  It also avoids duplication of long
// switch statement case lists.
//
// This version of the macro allows the template to take any number of
// arguments.  Example usage:
// switch(array->GetDataType())
//   {
//   vtkTemplateMacro(myFunc(static_cast<VTK_TT*>(data), arg2));
//   }
#define vtkTemplateMacroCase(typeN, type, call)     \
  case typeN: { typedef type VTK_TT; call; }; break
#define vtkTemplateMacro(call)                                    \
  vtkTemplateMacroCase(VTK_DOUBLE, double, call);                 \
  vtkTemplateMacroCase(VTK_FLOAT, float, call);                   \
  vtkTemplateMacroCase(VTK_LONG, long, call);                     \
  vtkTemplateMacroCase(VTK_UNSIGNED_LONG, unsigned long, call);   \
  vtkTemplateMacroCase(VTK_INT, int, call);                       \
  vtkTemplateMacroCase(VTK_UNSIGNED_INT, unsigned int, call);     \
  vtkTemplateMacroCase(VTK_SHORT, short, call);                   \
  vtkTemplateMacroCase(VTK_UNSIGNED_SHORT, unsigned short, call); \
  vtkTemplateMacroCase(VTK_CHAR, char, call);                     \
  vtkTemplateMacroCase(VTK_UNSIGNED_CHAR, unsigned char, call)

// Legacy versions of vtkTemplateMacro:
#define vtkTemplateMacro3(func, a1, a2, a3) \
         vtkTemplateMacro(func (a1, a2, a3))
#define vtkTemplateMacro4(func, a1, a2, a3, a4) \
         vtkTemplateMacro(func (a1, a2, a3, a4))
#define vtkTemplateMacro5(func, a1, a2, a3, a4, a5) \
         vtkTemplateMacro(func (a1, a2, a3, a4, a5))
#define vtkTemplateMacro6(func, a1, a2, a3, a4, a5, a6) \
         vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6))
#define vtkTemplateMacro7(func, a1, a2, a3, a4, a5, a6, a7) \
         vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6, a7))
#define vtkTemplateMacro8(func, a1, a2, a3, a4, a5, a6, a7, a8) \
         vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6, a7, a8))
#define vtkTemplateMacro9(func, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
         vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6, a7, a8, a9))
#define vtkTemplateMacro10(func,a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
          vtkTemplateMacro(func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10))

#endif

