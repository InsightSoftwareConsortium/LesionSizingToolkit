project(Visualization)

include_directories(${Visualization_SOURCE_DIR})

add_executable(ViewImageAndSegmentationSurface ViewImageAndSegmentationSurface.cxx )
target_link_libraries(ViewImageAndSegmentationSurface ${ITK_VTK_LSTK_LIBRARIES})
vtk_module_autoinit(TARGETS ViewImageAndSegmentationSurface MODULES ${VTK_LIBRARIES})

add_executable(ViewImageSlicesAndSegmentationContours
  ViewImageSlicesAndSegmentationContours.cxx
  vtkContourVisualizationModule.cxx
  )
target_link_libraries(ViewImageSlicesAndSegmentationContours ${ITK_VTK_LSTK_LIBRARIES})
vtk_module_autoinit(TARGETS ViewImageSlicesAndSegmentationContours MODULES ${VTK_LIBRARIES})

add_executable(LesionSurfaceExtraction LesionSurfaceExtraction.cxx )
target_link_libraries(LesionSurfaceExtraction ${ITK_VTK_LSTK_LIBRARIES})
vtk_module_autoinit(TARGETS LesionSurfaceExtraction MODULES ${VTK_LIBRARIES})

add_executable(IsoSurfaceVolumeEstimation IsoSurfaceVolumeEstimation.cxx )
target_link_libraries(IsoSurfaceVolumeEstimation ${ITK_VTK_LSTK_LIBRARIES})
vtk_module_autoinit(TARGETS IsoSurfaceVolumeEstimation MODULES ${VTK_LIBRARIES})

