#
# Insert examples here.
#

add_executable( LesionSegmentation
   LesionSegmentation.cxx
   itkLesionSegmentationCommandLineProgressReporter.cxx )
target_link_libraries( LesionSegmentation ${ITK_VTK_LSTK_LIBRARIES} )

