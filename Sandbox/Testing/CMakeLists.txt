
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})
SET(TEST_DATA_ROOT ${LesionSizingKitSandbox_SOURCE_DIR})
SET(TEMP ${LesionSizingKitSandbox_BINARY_DIR}/Testing/Temporary)


ADD_EXECUTABLE(itkSystemInformation itkSystemInformation.cxx )
TARGET_LINK_LIBRARIES(itkSystemInformation ITKIO ITKNumerics)

ADD_TEST(itkSystemInformation ${CXX_TEST_PATH}/itkSystemInformation )
