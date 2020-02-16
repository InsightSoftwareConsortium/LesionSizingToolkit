
find_package(LATEX)

if(NOT PDFLATEX_COMPILER)
  message("pdflatex compiler was not found. Please pass to advanced mode and provide its full path")
endif(NOT PDFLATEX_COMPILER)


find_package(Gnuplot REQUIRED)

mark_as_advanced( CLEAR GNUPLOT )

if(NOT GNUPLOT)
  message("Gnuplot was not found. Please provide the full path to a gnuplot executable in the GNUPLOT variable.")
endif(NOT GNUPLOT)


if( GNUPLOT AND PDFLATEX_COMPILER )
  add_subdirectory( Report001 )
  add_subdirectory( Report002 )
endif( GNUPLOT AND PDFLATEX_COMPILER )
