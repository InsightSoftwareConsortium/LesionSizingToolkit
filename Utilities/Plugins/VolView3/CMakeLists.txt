PROJECT(LesionSizingToolkitVolViewPlugins)

# Disable deprecation warnings for standard C functions and all the	 
# 1.276E1265 security warnings	 
if(CMAKE_COMPILER_2005)
  add_definitions(
    -D_CRT_FAR_MAPPINGS_NO_DEPRECATE
    -D_CRT_IS_WCTYPE_NO_DEPRECATE 
    -D_CRT_MANAGED_FP_NO_DEPRECATE
    -D_CRT_NONSTDC_NO_DEPRECATE
    -D_CRT_SECURE_NO_DEPRECATE
    -D_CRT_SECURE_NO_DEPRECATE_GLOBALS
    -D_CRT_SETERRORMODE_BEEP_SLEEP_NO_DEPRECATE
    -D_CRT_TIME_FUNCTIONS_NO_DEPRECATE
    -D_CRT_VCCLRIT_NO_DEPRECATE
    -D_SCL_SECURE_NO_DEPRECATE
    )
endif(CMAKE_COMPILER_2005)


#  Find where ITK was Built or Installed.
#
FIND_PACKAGE(ITK)
IF(ITK_FOUND)
  INCLUDE(${ITK_USE_FILE})
ELSE(ITK_FOUND)
  MESSAGE(FATAL_ERROR
          "Cannot build VolviewPlugIns without ITK.  Please set ITK_DIR.")
ENDIF(ITK_FOUND)

IF( ITK_BUILD_SHARED )

  # list all plugins here
  SET (PLUGINS 
    vvITKLungLesionSegmentationMethod1
    )

  set(WIN32_64BITS ${CMAKE_CL_64})

  # copy the plugins to a plugin directory
  FOREACH (PLUG ${PLUGINS})
    ADD_LIBRARY( ${PLUG} MODULE ${PLUG}.cxx )
    if(WIN32_64BITS)
      set_source_files_properties(${PLUG}.cxx 
        PROPERTIES COMPILE_FLAGS "/bigobj")
    endif(WIN32_64BITS)
    TARGET_LINK_LIBRARIES( ${PLUG} ${LesionSizingToolkit_LIBRARIES} ITKIO ITKCommon ITKBasicFilters ITKAlgorithms ITKNumerics ITKSpatialObject )
  ENDFOREACH (PLUG)

  SET( ${PROJECT_NAME}_PLUGINS_INSTALL_FILES ${ITK_PLUGINS_INSTALL_FILES} CACHE INTERNAL 
       "Plugins from ${PROJECT_NAME} that may be installed by a VolViewApplication.")
ENDIF( ITK_BUILD_SHARED )

