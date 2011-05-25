#
# Insert examples here.
#
option (LSTK_USE_VTK "Build visualization helper tools." OFF)

IF (LSTK_USE_VTK)
  FIND_PACKAGE(VTK REQUIRED)
  INCLUDE(${VTK_USE_FILE})

  SET(ITK_VTK_LSTK_LIBRARIES
   ${ITK-LesionSizingToolkit_LIBRARIES}
   vtkRendering vtkIO vtkWidgets vtkHybrid )

 ADD_EXECUTABLE( LesionSegmentation
   LesionSegmentation.cxx
   itkLesionSegmentationCommandLineProgressReporter.cxx )
  TARGET_LINK_LIBRARIES( LesionSegmentation ${ITK_VTK_LSTK_LIBRARIES} )
ENDIF (LSTK_USE_VTK)
