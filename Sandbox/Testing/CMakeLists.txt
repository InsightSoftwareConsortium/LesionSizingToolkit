
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})
SET(TEST_DATA_ROOT ${LesionSizingKitSandbox_SOURCE_DIR}/../Data)
SET(TEMP ${LesionSizingKitSandbox_BINARY_DIR}/Testing/Temporary)

FIND_PATH(TEST_CORNELL_DATA_ROOT README.txt DOC "Directory with data from the Cornell ELCAP database")

ADD_EXECUTABLE(itkSystemInformationSandbox itkSystemInformation.cxx )
TARGET_LINK_LIBRARIES(itkSystemInformationSandbox LesionSizingSandbox)

ADD_EXECUTABLE(DicomSeriesReadImageWrite DicomSeriesReadImageWrite.cxx )
TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite ITKIO)

ADD_EXECUTABLE(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadRegionOfInterestWrite ITKIO)

ADD_EXECUTABLE(CannyEdgeDetectionImageFilter CannyEdgeDetectionImageFilter.cxx )
TARGET_LINK_LIBRARIES(CannyEdgeDetectionImageFilter ITKIO)

ADD_EXECUTABLE(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeImageFilter ITKIO)

ADD_EXECUTABLE(LandmarkSpatialObjectWriterTest LandmarkSpatialObjectWriterTest.cxx )
TARGET_LINK_LIBRARIES(LandmarkSpatialObjectWriterTest ITKIO)

IF( ITK_USE_MULTI_PHASE_FRAMEWORK )

ADD_EXECUTABLE(itkVectorFiniteDifferenceImageFilterTest1 itkVectorFiniteDifferenceImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorFiniteDifferenceImageFilterTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorFiniteDifferenceFunctionTest1 itkVectorFiniteDifferenceFunctionTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorFiniteDifferenceFunctionTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorLevelSetFunctionTest1 itkVectorLevelSetFunctionTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorLevelSetFunctionTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorLevelSetFunctionTest2 itkVectorLevelSetFunctionTest2.cxx )
TARGET_LINK_LIBRARIES(itkVectorLevelSetFunctionTest2 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorSegmentationLevelSetFunctionTest1 itkVectorSegmentationLevelSetFunctionTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorSegmentationLevelSetFunctionTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorSparseFieldLevelSetImageFilterTest1 itkVectorSparseFieldLevelSetImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorSparseFieldLevelSetImageFilterTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorSparseFieldLevelSetImageFilterTest2 itkVectorSparseFieldLevelSetImageFilterTest2.cxx )
TARGET_LINK_LIBRARIES(itkVectorSparseFieldLevelSetImageFilterTest2 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorSparseFieldLevelSetNodeTest1 itkVectorSparseFieldLevelSetNodeTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorSparseFieldLevelSetNodeTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorSparseFieldCityBlockNeighborListTest1 itkVectorSparseFieldCityBlockNeighborListTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorSparseFieldCityBlockNeighborListTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorSegmentationLevelSetImageFilterTest1 itkVectorSegmentationLevelSetImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorSegmentationLevelSetImageFilterTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkNumericTraitsMatrixPixelTest1 itkNumericTraitsMatrixPixelTest1.cxx )
TARGET_LINK_LIBRARIES(itkNumericTraitsMatrixPixelTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkMatrixLinearInterpolateImageFunctionTest1 itkMatrixLinearInterpolateImageFunctionTest1.cxx )
TARGET_LINK_LIBRARIES(itkMatrixLinearInterpolateImageFunctionTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkMatrixInterpolateImageFunctionTest1 itkMatrixInterpolateImageFunctionTest1.cxx )
TARGET_LINK_LIBRARIES(itkMatrixInterpolateImageFunctionTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorShiftScaleImageFilterText1 itkVectorShiftScaleImageFilterText1.cxx )
TARGET_LINK_LIBRARIES(itkVectorShiftScaleImageFilterText1 LesionSizingSandbox)

ADD_EXECUTABLE(itkVectorZeroCrossingImageFilterTest1 itkVectorZeroCrossingImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorZeroCrossingImageFilterTest1 LesionSizingSandbox)


ADD_TEST(itkVectorFiniteDifferenceImageFilterTest1 ${CXX_TEST_PATH}/itkVectorFiniteDifferenceImageFilterTest1 )
ADD_TEST(itkVectorFiniteDifferenceFunctionTest1 ${CXX_TEST_PATH}/itkVectorFiniteDifferenceFunctionTest1 )
ADD_TEST(itkVectorSegmentationLevelSetFunctionTest1 ${CXX_TEST_PATH}/itkVectorSegmentationLevelSetFunctionTest1 )
ADD_TEST(itkVectorLevelSetFunctionTest1 ${CXX_TEST_PATH}/itkVectorLevelSetFunctionTest1 )
ADD_TEST(itkVectorLevelSetFunctionTest2 ${CXX_TEST_PATH}/itkVectorLevelSetFunctionTest2 )
ADD_TEST(itkVectorSparseFieldLevelSetImageFilterTest1 ${CXX_TEST_PATH}/itkVectorSparseFieldLevelSetImageFilterTest1 )
ADD_TEST(itkVectorSparseFieldLevelSetImageFilterTest2 ${CXX_TEST_PATH}/itkVectorSparseFieldLevelSetImageFilterTest2 )
ADD_TEST(itkVectorSparseFieldLevelSetNodeTest1 ${CXX_TEST_PATH}/itkVectorSparseFieldLevelSetNodeTest1 )
ADD_TEST(itkVectorSparseFieldCityBlockNeighborListTest1 ${CXX_TEST_PATH}/itkVectorSparseFieldCityBlockNeighborListTest1 )
ADD_TEST(itkVectorSegmentationLevelSetImageFilterTest1 ${CXX_TEST_PATH}/itkVectorSegmentationLevelSetImageFilterTest1 )
ADD_TEST(itkNumericTraitsMatrixPixelTest1 ${CXX_TEST_PATH}/itkNumericTraitsMatrixPixelTest1 )
ADD_TEST(itkMatrixLinearInterpolateImageFunctionTest1 ${CXX_TEST_PATH}/itkMatrixLinearInterpolateImageFunctionTest1 )
ADD_TEST(itkMatrixInterpolateImageFunctionTest1 ${CXX_TEST_PATH}/itkMatrixInterpolateImageFunctionTest1 )
ADD_TEST(itkVectorShiftScaleImageFilterText1 ${CXX_TEST_PATH}/itkVectorShiftScaleImageFilterText1 )
ADD_TEST(itkVectorZeroCrossingImageFilterTest1 ${CXX_TEST_PATH}/itkVectorZeroCrossingImageFilterTest1 )

ENDIF( ITK_USE_MULTI_PHASE_FRAMEWORK )

ADD_TEST(itkSystemInformationSandbox ${CXX_TEST_PATH}/itkSystemInformationSandbox )


ADD_EXECUTABLE(itkLesionSegmentationMethodTest1 itkLesionSegmentationMethodTest1.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkLesionSegmentationMethodTest2 itkLesionSegmentationMethodTest2.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest2 LesionSizingSandbox)

ADD_EXECUTABLE(itkFeatureGeneratorTest1 itkFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkCannyEdgesDistanceFeatureGeneratorTest1 itkCannyEdgesDistanceFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesDistanceFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 itkGradientMagnitudeSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSatoVesselnessFeatureGeneratorTest1 itkSatoVesselnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSatoLocalStructureFeatureGeneratorTest1 itkSatoLocalStructureFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoLocalStructureFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkDescoteauxSheetnessFeatureGeneratorTest1 itkDescoteauxSheetnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkDescoteauxSheetnessFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkFrangiTubularnessFeatureGeneratorTest1 itkFrangiTubularnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFrangiTubularnessFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSegmentationModuleTest1 itkSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkRegionGrowingSegmentationModuleTest1 itkRegionGrowingSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkRegionGrowingSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSinglePhaseLevelSetSegmentationModuleTest1 itkSinglePhaseLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkSinglePhaseLevelSetSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkConnectedThresholdSegmentationModuleTest1 itkConnectedThresholdSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkConnectedThresholdSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkShapeDetectionLevelSetSegmentationModuleTest1 itkShapeDetectionLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkShapeDetectionLevelSetSegmentationModuleTest1 LesionSizingSandbox)

ADD_TEST(itkLesionSegmentationMethodTest1 ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest1 )
ADD_TEST(itkLesionSegmentationMethodTest2 ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest2 )
ADD_TEST(itkFeatureGeneratorTest1 ${CXX_TEST_PATH}/itkFeatureGeneratorTest1 )
ADD_TEST(itkSegmentationModuleTest1 ${CXX_TEST_PATH}/itkSegmentationModuleTest1 )
ADD_TEST(itkRegionGrowingSegmentationModuleTest1 ${CXX_TEST_PATH}/itkRegionGrowingSegmentationModuleTest1 )
ADD_TEST(itkSinglePhaseLevelSetSegmentationModuleTest1 ${CXX_TEST_PATH}/itkSinglePhaseLevelSetSegmentationModuleTest1 )
ADD_TEST(itkConnectedThresholdSegmentationModuleTest1 ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1 )

ADD_TEST(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mhd
  0.7
  -10.0
  90.0
  )

ADD_TEST(GradientMagnitudeImageFilterTest1
  ${CXX_TEST_PATH}/GradientMagnitudeImageFilter
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/GradientMagnitudeImageFilterTest1_1.mhd
  0.7 # Sigma
  )

ADD_TEST(CannyEdgeDetectionImageFilterTest1
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest1_1.mhd
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  )

ADD_TEST(itkCannyEdgesDistanceFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkCannyEdgesDistanceFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mhd
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  )

ADD_TEST(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1_1.mhd
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  )

ADD_TEST(itkSatoVesselnessFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorTest1_1.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(itkSatoLocalStructureFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoLocalStructureFeatureGeneratorTest1_1.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(itkDescoteauxSheetnessFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnessFeatureGeneratorTest1_1.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(itkFrangiTubularnessFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FrangiTubularnessFeatureGeneratorTest1_1.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkGeodesicActiveContourLevelSetSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedManualInitialization1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mhd
  ${TEMP}/GeodesicActiveContourLevelSetSegmentationModuleTest1_1.mhd
  )

ADD_TEST(itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedManualInitialization1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mhd
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest1_1.mhd
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
  )

ADD_TEST(itkShapeDetectionLevelSetSegmentationModuleTest2 
  ${CXX_TEST_PATH}/itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedManualInitialization1.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mhd
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest2_1.mhd
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
  )


IF(TEST_CORNELL_DATA_ROOT)

ADD_TEST(Read001 
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${TEST_CORNELL_DATA_ROOT}/CornellPartSolid2/SM0055-20000101/SM0055/20000101-122401-2-1
  ${TEMP}/CornellPartSolid2_1.mhd
  )

ADD_TEST(Read002 
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${TEST_CORNELL_DATA_ROOT}/CornellPartSolid3/SL0074-20000101/SL0074/20000101-134856-2-1
  ${TEMP}/CornellPartSolid3_1.mhd
  )

ADD_TEST(ROI001 
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/CornellPartSolid2_1.mhd
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  270 180 120 
  160 100  69 
  )

ADD_TEST(ROI002 
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/CornellPartSolid3_1.mhd
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  100 170 180 
  100 120  64
  )


ADD_TEST(GMSFGTest001
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/GMSFGTest001_01.mhd
  1.0
  -1.0
  5.0
  )

ADD_TEST(SVFGTest001
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SVFGTest001_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(SLSFGTest001
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SLSFGTest001_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(DSFGTest001
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SLSFGTest001_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(FTFGTest001
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SLSFGTest001_01.mhd
  1.0
  0.5
  2.0
  )


ADD_TEST(GMSFGTest002
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/GMSFGTest002_01.mhd
  1.0
  -1.0
  5.0
  )

ADD_TEST(SVFGTest002
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SVFGTest002_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(SLSFGTest002
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SLSFGTest002_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(DSFGTest002
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SLSFGTest002_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(FTFGTest002
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SLSFGTest002_01.mhd
  1.0
  0.5
  2.0
  )

#
#  Region Growing tests
#
ADD_TEST(CTRGTest001
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/CTRGTest001_01.mhd
  100
  200
  )

ENDIF(TEST_CORNELL_DATA_ROOT)

