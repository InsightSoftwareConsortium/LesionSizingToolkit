cmake_minimum_required(VERSION 3.10.2)
project(LesionSizingToolkit)

if(NOT ITK_SOURCE_DIR)
  find_package(ITK REQUIRED)
  include(${ITK_USE_FILE})
  list(APPEND CMAKE_MODULE_PATH ${ITK_CMAKE_DIR})
  include(ITKModuleExternal)
else()
  itk_module_impl()
endif()

option(LSTK_USE_VTK "Build visualization helper tools." OFF)
if(LSTK_USE_VTK)
  if(NOT COMMAND vtk_module_config)
    macro(vtk_module_config ns)
       foreach(arg ${ARGN})
        if(${arg} MATCHES "^[Vv][Tt][Kk]")
          string(REGEX REPLACE "^[Vv][Tt][Kk]" "" _arg ${arg})
        else()
          set(_arg ${arg})
        endif()
        set(${ns}_LIBRARIES ${${ns}_LIBRARIES} VTK::${_arg})
       endforeach()
    endmacro()

    if(NOT VTK_RENDERING_BACKEND)
      set(VTK_RENDERING_BACKEND OpenGL2)
    endif()
  endif()

  if(NOT VTK_RENDERING_BACKEND)
    if(NOT COMMAND vtk_module_config)
      set(VTK_RENDERING_BACKEND OpenGL2)
    else()
      set(VTK_RENDERING_BACKEND OpenGL)
    endif()
  endif()

  set(_target_freetypeopengl)
  if(TARGET ${_target_prefix}RenderingFreeType${VTK_RENDERING_BACKEND})
    set(_target_freetypeopengl ${_target_prefix}RenderingFreeType${VTK_RENDERING_BACKEND})
  endif()

  set(_required_vtk_libraries
    ${_target_prefix}RenderingVolume
    #${_target_prefix}Rendering${VTK_RENDERING_BACKEND}
    ${_target_prefix}IOGeometry
    ${_target_prefix}IOLegacy
    ${_target_prefix}IOImage
    ${_target_prefix}InteractionWidgets
    ${_target_prefix}InteractionImage
    )

  vtk_module_config(ITK_VTK_LSTK
    ${_required_vtk_libraries}
    )

  set(ITK_VTK_LSTK_LIBRARIES ${LesionSizingToolkit_LIBRARIES} ${_required_vtk_libraries})

  # The VTK DICOMParser and vtkmetaio includes conflict with the ITK
  # versions. Here we remove them from the include directories.
  #
  string(REGEX REPLACE "[^;]*MetaIO;"
           "" ITK_VTK_LSTK_INCLUDE_DIRS "${ITK_VTK_LSTK_INCLUDE_DIRS}")
  string(REGEX REPLACE "[^;]*vtkmetaio;"
           "" ITK_VTK_LSTK_INCLUDE_DIRS "${ITK_VTK_LSTK_INCLUDE_DIRS}")
  string(REGEX REPLACE "[^;]*DICOMParser;"
           "" ITK_VTK_LSTK_INCLUDE_DIRS "${ITK_VTK_LSTK_INCLUDE_DIRS}")

  # Add preprocessor definitions needed to use VTK.
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS ${ITK_VTK_LSTK_VTK_DEFINITIONS})

  add_subdirectory(Examples)
  add_subdirectory(Utilities)
endif()
