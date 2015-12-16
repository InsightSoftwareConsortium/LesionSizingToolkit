cmake_minimum_required(VERSION 2.8.9)
project(LesionSizingToolkit)

option(LSTK_USE_VTK "Build visualization helper tools." OFF)
if(LSTK_USE_VTK)
  set(myVTKLibraries
    vtkRenderingVolume
    vtkIOImage
    vtkInteractionWidgets)

  find_package(VTK REQUIRED COMPONENTS ${myVTKLibraries})
  set(VERSION_MIN "6.0.0")
  if(${VTK_VERSION} VERSION_LESS ${VERSION_MIN})
    message(ERROR " LSTK_USE_VTK requires VTK version ${VERSION_MIN} or newer but the current version is ${VTK_VERSION}")
  endif()
  include(${VTK_USE_FILE})

  set(ITK_VTK_LSTK_LIBRARIES
   ${LesionSizingToolkit_LIBRARIES}
   ${myVTKLibraries}
   )
  add_subdirectory(Examples)
  add_subdirectory(Utilities)
endif()

if(NOT ITK_SOURCE_DIR)
  find_package(ITK REQUIRED)
  list(APPEND CMAKE_MODULE_PATH ${ITK_CMAKE_DIR})
  include(ITKModuleExternal)
else()
  itk_module_impl()
endif()
