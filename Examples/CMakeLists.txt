#
# Insert examples here.
#

ADD_EXECUTABLE( LesionSegmentation
   LesionSegmentation.cxx
   itkLesionSegmentationCommandLineProgressReporter.cxx )
TARGET_LINK_LIBRARIES( LesionSegmentation ${ITK_VTK_LSTK_LIBRARIES} )

