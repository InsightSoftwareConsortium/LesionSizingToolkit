PROJECT(Visualization)

# Find VTK.
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET(ITK_VTK_LIBRARIES
 ITKBasicFilters ITKIO ITKCommon
 vtkRendering vtkIO vtkWidgets vtkHybrid
)

INCLUDE_DIRECTORIES(${Visualization_SOURCE_DIR})

ADD_EXECUTABLE(ViewImageAndSegmentationSurface ViewImageAndSegmentationSurface.cxx )
TARGET_LINK_LIBRARIES(ViewImageAndSegmentationSurface ${ITK_VTK_LIBRARIES})

ADD_EXECUTABLE(ViewImageSlicesAndSegmentationContours 
  ViewImageSlicesAndSegmentationContours.cxx
  vtkContourVisualizationModule.cxx
  )
TARGET_LINK_LIBRARIES(ViewImageSlicesAndSegmentationContours ${ITK_VTK_LIBRARIES})

ADD_EXECUTABLE(AntiAliasBinaryImageFilter AntiAliasBinaryImageFilter.cxx )
TARGET_LINK_LIBRARIES(AntiAliasBinaryImageFilter ${ITK_VTK_LIBRARIES})

ADD_EXECUTABLE(SurfaceExtraction SurfaceExtraction.cxx )
TARGET_LINK_LIBRARIES(SurfaceExtraction ${ITK_VTK_LIBRARIES})

