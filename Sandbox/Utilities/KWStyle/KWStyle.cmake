OPTION(LesionSizingToolkit_USE_KWSTYLE "Enable the use of KWStyle for checking coding style." OFF)
IF (LesionSizingToolkit_USE_KWSTYLE)

  OPTION(KWSTYLE_USE_VIM_FORMAT "Set KWStyle to generate errors with a VIM-compatible format." OFF)
  OPTION(KWSTYLE_USE_MSVC_FORMAT "Set KWStyle to generate errors with a VisualStudio-compatible format." OFF)

  FIND_PROGRAM(KWSTYLE_EXECUTABLE
  NAMES KWStyle 
  PATHS
  /usr/local/bin
  )

  CONFIGURE_FILE(
    Utilities/KWStyle/ITKFiles.txt.in
    ${PROJECT_BINARY_DIR}/Utilities/KWStyle/ITKFiles.txt)

  CONFIGURE_FILE(
    Utilities/KWStyle/ITK.kws.xml.in
    ${PROJECT_BINARY_DIR}/Utilities/KWStyle/ITK.kws.xml)

  SET(KWSTYLE_ARGUMENTS
    -xml ${PROJECT_BINARY_DIR}/Utilities/KWStyle/ITK.kws.xml -v -D ${PROJECT_BINARY_DIR}/Utilities/KWStyle/ITKFiles.txt  
    -o ${PROJECT_SOURCE_DIR}/Utilities/KWStyle/ITKOverwrite.txt
    )

  IF(KWSTYLE_USE_VIM_FORMAT)
    SET(KWSTYLE_ARGUMENTS -vim ${KWSTYLE_ARGUMENTS})
  ENDIF(KWSTYLE_USE_VIM_FORMAT)

  IF(KWSTYLE_USE_MSVC_FORMAT)
    SET(KWSTYLE_ARGUMENTS -msvc ${KWSTYLE_ARGUMENTS})
  ENDIF(KWSTYLE_USE_MSVC_FORMAT)

  ADD_CUSTOM_COMMAND(
    OUTPUT ${StatisticsRefactoring_BINARY_DIR}/KWStyleReport.txt
    COMMAND ${KWSTYLE_EXECUTABLE}
    ARGS    ${KWSTYLE_ARGUMENTS}
    COMMENT "Coding Style Checker"
    )
  ADD_CUSTOM_TARGET(StyleCheck DEPENDS ${StatisticsRefactoring_BINARY_DIR}/KWStyleReport.txt)
  ADD_TEST(KWStyleTest ${KWSTYLE_EXECUTABLE} ${KWSTYLE_ARGUMENTS})
ENDIF ( LesionSizingToolkit_USE_KWSTYLE )
