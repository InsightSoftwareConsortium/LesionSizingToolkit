
FIND_PACKAGE(LATEX)

IF(NOT PDFLATEX_COMPILER)
  MESSAGE("pdflatex compiler was not found. Please pass to advanced mode and provide its full path")
ENDIF(NOT PDFLATEX_COMPILER)


FIND_PACKAGE(Gnuplot REQUIRED)

MARK_AS_ADVANCED( CLEAR GNUPLOT )

IF(NOT GNUPLOT)
  MESSAGE("Gnuplot was not found. Please provide the full path to a gnuplot executable in the GNUPLOT variable.")
ENDIF(NOT GNUPLOT)


IF( GNUPLOT AND PDFLATEX_COMPILER )
  ADD_SUBDIRECTORY( Report001 )
ENDIF( GNUPLOT AND PDFLATEX_COMPILER )
