project(LesionSizingToolkit)
itk_module_impl()


OPTION(LSTK_USE_VTK "Build visualization helper tools." OFF)


IF (LSTK_USE_VTK)
  
  SET(myVTKLibraries
   vtkRenderingVolume
   vtkIOImage
   vtkInteractionWidgets)

  FIND_PACKAGE(VTK REQUIRED COMPONENTS ${myVTKLibraries})

  SET(VERSION_MIN "6.0.0")
  IF (${VTK_VERSION} VERSION_LESS ${VERSION_MIN})
    MESSAGE(ERROR " LSTK_USE_VTK requires VTK version ${VERSION_MIN} or newer but the current version is ${VTK_VERSION}")
  ENDIF()


  INCLUDE(${VTK_USE_FILE})

  SET(ITK_VTK_LSTK_LIBRARIES
   ${LesionSizingToolkit_LIBRARIES}
   ${myVTKLibraries}
   )

  ADD_SUBDIRECTORY(Examples)
  ADD_SUBDIRECTORY(Utilities)

ENDIF (LSTK_USE_VTK)
