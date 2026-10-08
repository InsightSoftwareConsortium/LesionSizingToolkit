cmake_minimum_required(VERSION 3.16.3)
project(LesionSizingToolkit)

if(NOT ITK_SOURCE_DIR)
  find_package(ITK REQUIRED)
  list(APPEND CMAKE_MODULE_PATH ${ITK_CMAKE_DIR})
  include(ITKModuleExternal)
else()
  itk_module_impl()
endif()

option(LSTK_USE_VTK "Build visualization helper tools." OFF)
if(LSTK_USE_VTK)
  find_package(
    VTK 9.0 REQUIRED
    COMPONENTS
      CommonCore
      FiltersCore
      FiltersGeneral
      FiltersSources
      ImagingCore
      InteractionImage
      InteractionStyle
      InteractionWidgets
      IOGeometry
      IOImage
      IOLegacy
      RenderingCore
      RenderingFreeType
      RenderingOpenGL2
      RenderingVolume
      RenderingVolumeOpenGL2
      vtksys
  )
  # ITK 6 exports one interface library per module; ITK 5.4 provides only the list variable.
  if(TARGET ITK::LesionSizingToolkitModule)
    set(_lstk_libraries ITK::LesionSizingToolkitModule)
  elseif(TARGET LesionSizingToolkitModule)
    set(_lstk_libraries LesionSizingToolkitModule)
  else()
    set(_lstk_libraries ${LesionSizingToolkit_LIBRARIES})
  endif()
  set(ITK_VTK_LSTK_LIBRARIES ${_lstk_libraries} ${ITK_LIBRARIES} ${VTK_LIBRARIES})
  include_directories("${LesionSizingToolkit_SOURCE_DIR}/include")

  add_subdirectory(Examples)
  add_subdirectory(Utilities)
endif()
