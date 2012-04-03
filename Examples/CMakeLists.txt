#
# Insert examples here.
#
OPTION(LSTK_USE_VTK "Build visualization helper tools." OFF)


IF (LSTK_USE_VTK)
  FIND_PACKAGE(VTK REQUIRED)

  SET(VERSION_MIN "5.9.20110419")
  IF (${VTK_VERSION} VERSION_LESS ${VERSION_MIN})
    MESSAGE(ERROR " LSTK_USE_VTK requires VTK version ${VERSION_MIN} or newer but the current version is ${VTK_VERSION}")
  ENDIF()

  INCLUDE(${VTK_USE_FILE})

  SET(ITK_VTK_LSTK_LIBRARIES
   ${ITKLesionSizingToolkit_LIBRARIES}
   vtkRendering vtkIO vtkWidgets vtkHybrid )

 ADD_EXECUTABLE( LesionSegmentation
   LesionSegmentation.cxx
   itkLesionSegmentationCommandLineProgressReporter.cxx )
  TARGET_LINK_LIBRARIES( LesionSegmentation ${ITK_VTK_LSTK_LIBRARIES} )
ENDIF (LSTK_USE_VTK)
