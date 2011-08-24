PROJECT(Visualization)

# Find VTK.
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET(ITK_VTK_LIBRARIES
 ${ITKLesionSizingToolkit_LIBRARIES}
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

ADD_EXECUTABLE(LesionSurfaceExtraction LesionSurfaceExtraction.cxx )
TARGET_LINK_LIBRARIES(LesionSurfaceExtraction ${ITK_VTK_LIBRARIES})

ADD_EXECUTABLE(IsoSurfaceVolumeEstimation IsoSurfaceVolumeEstimation.cxx )
TARGET_LINK_LIBRARIES(IsoSurfaceVolumeEstimation ${ITK_VTK_LIBRARIES})

