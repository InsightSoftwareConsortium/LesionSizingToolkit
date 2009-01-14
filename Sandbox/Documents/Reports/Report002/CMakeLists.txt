PROJECT(Report002)

 
#
# These are source files needed for the report generation.
#

SET(REPORT_SRCS
  algorithmic.sty
  algorithm.sty
  amssymb.sty
  fancyhdr.sty
  floatflt.sty
  fncychap.sty
  InsightArticle.cls
  InsightJournal.bib
  InsightJournal.ist
  InsightJournal.sty
  times.sty
  picins.sty
  )


#
#  Get the list of figures generated during testing
#
FILE(GLOB REPORT_GENERATED_FIGURES "${LesionSizingKitSandbox_BINARY_DIR}/Testing/Temporary/*.png")

SET(PROJECT_REPORT_SOURCE_DIR "${PROJECT_SOURCE_DIR}")
SET(PROJECT_REPORT_BINARY_DIR "${PROJECT_BINARY_DIR}")

#
# This adds a custom command for each source file in REPORT_SRCS
# that copies the file from the source directory to the binary
# directory where the pdf will be generated.
#
FOREACH(SOURCE_FILE ${REPORT_SRCS})
   ADD_CUSTOM_COMMAND(
     OUTPUT   ${PROJECT_REPORT_BINARY_DIR}/${SOURCE_FILE}
     DEPENDS  ${LesionSizingKitSandbox_SOURCE_DIR}/Documents/Latex/${SOURCE_FILE}
     COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${LesionSizingKitSandbox_SOURCE_DIR}/Documents/Latex/${SOURCE_FILE}
        ${PROJECT_REPORT_BINARY_DIR}/${SOURCE_FILE}
    )
   SET(COPY_RESULTS ${COPY_RESULTS} ${PROJECT_REPORT_BINARY_DIR}/${SOURCE_FILE})
ENDFOREACH(SOURCE_FILE ${REPORT_SRCS})

#
# This adds a custom command for each PNG image file in Testing/Temporary directory
# that copies the file from to the binary directory where the pdf will be generated.
#
FOREACH(SOURCE_FIGURE_FILE ${REPORT_GENERATED_FIGURES})
   GET_FILENAME_COMPONENT(SOURCE_FIGURE_FILENAME ${SOURCE_FIGURE_FILE} NAME)
   SET(DESTINATION_FIGURE_FILENAME ${PROJECT_REPORT_BINARY_DIR}/${SOURCE_FIGURE_FILENAME})
   ADD_CUSTOM_COMMAND(
     OUTPUT    ${DESTINATION_FIGURE_FILENAME}
     DEPENDS   ${SOURCE_FIGURE_FILE}
     COMMAND ${CMAKE_COMMAND} -E copy_if_different 
        ${SOURCE_FIGURE_FILE}
        ${DESTINATION_FIGURE_FILENAME}
    )
   SET(COPY_RESULTS ${COPY_RESULTS} ${DESTINATION_FIGURE_FILENAME})
ENDFOREACH(SOURCE_FIGURE_FILE ${REPORT_GENERATED_FIGURES})


SET(REPORT_ELEMENTS 
  )

#
# This adds a custom target that generates Report002.pdf
# This target depends on the list of copied files created
# with the custom command above and the Plots target.
#
ADD_CUSTOM_TARGET(ReportIJ002 ALL 
   COMMAND ${PDFLATEX_COMPILER} 
        ${PROJECT_REPORT_SOURCE_DIR}/Report002.tex 
        -output-directory ${PROJECT_REPORT_BINARY_DIR}
   COMMAND ${PDFLATEX_COMPILER} 
        ${PROJECT_REPORT_SOURCE_DIR}/Report002.tex 
        -output-directory ${PROJECT_REPORT_BINARY_DIR}
   DEPENDS ${COPY_RESULTS} ${REPORT_ELEMENTS} 
   WORKING_DIRECTORY ${PROJECT_REPORT_BINARY_DIR}
   )
 
