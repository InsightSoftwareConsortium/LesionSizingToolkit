SET(LesionSizingSandbox_SRCS
  itkNumericTraitsMatrixPixel.cxx
  )

ADD_LIBRARY(LesionSizingSandbox ${LesionSizingSandbox_SRCS})

TARGET_LINK_LIBRARIES(LesionSizingSandbox ITKIO ITKNumerics ITKCommon)

