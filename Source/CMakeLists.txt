SET(LesionSizingToolkit_SRCS
  itkNumericTraitsMatrixPixel.cxx
  )

ADD_LIBRARY(LesionSizingToolkit ${LesionSizingToolkit_SRCS})

TARGET_LINK_LIBRARIES(LesionSizingToolkit ITKIO ITKNumerics ITKCommon)

