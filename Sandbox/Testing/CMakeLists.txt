
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})
SET(TEST_DATA_ROOT ${LesionSizingKitSandbox_SOURCE_DIR}/../Data)
SET(TEMP ${LesionSizingKitSandbox_BINARY_DIR}/Testing/Temporary)

FIND_PATH(TEST_CORNELL_DATA_ROOT README.txt DOC "Directory with data from the Cornell ELCAP database")

OPTION(TEST_CORNELL_COLLECTION "Run tests in the collection of datasets from Cornell" OFF)

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

ADD_EXECUTABLE(itkFeatureGeneratorTest1 itkFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkRegionCompetitionImageFilterTest1 itkRegionCompetitionImageFilterTest1.cxx)
TARGET_LINK_LIBRARIES(itkRegionCompetitionImageFilterTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSegmentationVolumeEstimatorTest1 itkSegmentationVolumeEstimatorTest1.cxx)
TARGET_LINK_LIBRARIES(itkSegmentationVolumeEstimatorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkGrayscaleImageSegmentationVolumeEstimatorTest1 itkGrayscaleImageSegmentationVolumeEstimatorTest1.cxx)
TARGET_LINK_LIBRARIES(itkGrayscaleImageSegmentationVolumeEstimatorTest1 LesionSizingSandbox)


IF( ITK_USE_MULTI_PHASE_FRAMEWORK )

ADD_EXECUTABLE(itkVectorFiniteDifferenceImageFilterTest1 itkVectorFiniteDifferenceImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkVectorFiniteDifferenceImageFilterTest1 LesionSizingSandbox)

#ADD_EXECUTABLE(itkVectorFiniteDifferenceFunctionTest1 itkVectorFiniteDifferenceFunctionTest1.cxx )
#TARGET_LINK_LIBRARIES(itkVectorFiniteDifferenceFunctionTest1 LesionSizingSandbox)

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

ADD_EXECUTABLE(itkLesionSegmentationMethodTest3 itkLesionSegmentationMethodTest3.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest3 LesionSizingSandbox)

ADD_EXECUTABLE(itkLesionSegmentationMethodTest4 itkLesionSegmentationMethodTest4.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest4 LesionSizingSandbox)

ADD_EXECUTABLE(itkLesionSegmentationMethodTest5 itkLesionSegmentationMethodTest5.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest5 LesionSizingSandbox)

ADD_EXECUTABLE(itkLesionSegmentationMethodTest6 itkLesionSegmentationMethodTest6.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest6 LesionSizingSandbox)

ADD_EXECUTABLE(itkLesionSegmentationMethodTest7 itkLesionSegmentationMethodTest7.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest7 LesionSizingSandbox)

ADD_EXECUTABLE(itkFeatureAggregatorTest1 itkFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFeatureAggregatorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkMinimumFeatureAggregatorTest1 itkMinimumFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkMinimumFeatureAggregatorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkMinimumFeatureAggregatorTest2 itkMinimumFeatureAggregatorTest2.cxx )
TARGET_LINK_LIBRARIES(itkMinimumFeatureAggregatorTest2 LesionSizingSandbox)

ADD_EXECUTABLE(itkWeightedSumFeatureAggregatorTest1 itkWeightedSumFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkWeightedSumFeatureAggregatorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkCannyEdgesDistanceFeatureGeneratorTest1 itkCannyEdgesDistanceFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesDistanceFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 itkGradientMagnitudeSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSigmoidFeatureGeneratorTest1 itkSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSigmoidFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkLungWallFeatureGeneratorTest1 itkLungWallFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkLungWallFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSatoVesselnessFeatureGeneratorTest1 itkSatoVesselnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSatoVesselnessSigmoidFeatureGeneratorTest1 itkSatoVesselnessSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessSigmoidFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkSatoLocalStructureFeatureGeneratorTest1 itkSatoLocalStructureFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoLocalStructureFeatureGeneratorTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkLocalStructureImageFilterTest1 itkLocalStructureImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkLocalStructureImageFilterTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkDescoteauxSheetnessImageFilterTest1 itkDescoteauxSheetnessImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkDescoteauxSheetnessImageFilterTest1 LesionSizingSandbox)

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

ADD_EXECUTABLE(itkConfidenceConnectedSegmentationModuleTest1 itkConfidenceConnectedSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkConfidenceConnectedSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkFastMarchingSegmentationModuleTest1 itkFastMarchingSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkFastMarchingSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 LesionSizingSandbox)

ADD_EXECUTABLE(itkShapeDetectionLevelSetSegmentationModuleTest1 itkShapeDetectionLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkShapeDetectionLevelSetSegmentationModuleTest1 LesionSizingSandbox)

ADD_TEST(itkLesionSegmentationMethodTest1 ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest1 )
ADD_TEST(itkLesionSegmentationMethodTest2 ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest2 )

ADD_TEST(itkLesionSegmentationMethodTest3 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LesionSegmentationMethodTest3_1.mhd
  0.5
  1.0
  )

ADD_TEST(itkFeatureGeneratorTest1 ${CXX_TEST_PATH}/itkFeatureGeneratorTest1 )
ADD_TEST(itkSegmentationModuleTest1 ${CXX_TEST_PATH}/itkSegmentationModuleTest1 )
ADD_TEST(itkRegionGrowingSegmentationModuleTest1 ${CXX_TEST_PATH}/itkRegionGrowingSegmentationModuleTest1 )
ADD_TEST(itkSinglePhaseLevelSetSegmentationModuleTest1 ${CXX_TEST_PATH}/itkSinglePhaseLevelSetSegmentationModuleTest1 )

ADD_TEST(itkRegionCompetitionImageFilterTest1 ${CXX_TEST_PATH}/itkRegionCompetitionImageFilterTest1 )

ADD_TEST(itkSegmentationVolumeEstimatorTest1 ${CXX_TEST_PATH}/itkSegmentationVolumeEstimatorTest1 )

ADD_TEST(itkGrayscaleImageSegmentationVolumeEstimatorTest1 ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest1 )

ADD_TEST(itkConnectedThresholdSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConnectedThresholdSegmentationModuleTest1_1.mhd
  -700
  500
  )

ADD_TEST(itkConfidenceConnectedSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkConfidenceConnectedSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mhd
  1.7
  )

ADD_TEST(itkFastMarchingSegmentationModuleTest1-PartSolidLesion1
  ${CXX_TEST_PATH}/itkFastMarchingSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FastMarchingSegmentationModuleTest1-PartSolidLesion1_1.mha
  10.0 5.0 
  )

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

ADD_TEST(itkSigmoidFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SigmoidFeatureGeneratorTest1_1.mhd
  1.0
  -700.0 
  )

ADD_TEST(itkLungWallFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LungWallFeatureGeneratorTest1_1.mhd
  -400.0 
  )

ADD_TEST(LandmarkSpatialObjectWriterTest1
  ${CXX_TEST_PATH}/LandmarkSpatialObjectWriterTest
  ${TEMP}/LandmarkSpatialObjectWriterTest1_1.txt
  )


ADD_TEST(CannyEdgeDetectionImageFilterTest1
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest1_1.mhd
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest1_1.mhd
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

ADD_TEST(itkSatoVesselnessSigmoidFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorTest1_1.mhd
  1.0
  0.5
  2.0
  -10
  80
  )


ADD_TEST(itkSatoLocalStructureFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoLocalStructureFeatureGeneratorTest1_1.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(itkLocalStructureImageFilterTest1 
  ${CXX_TEST_PATH}/itkLocalStructureImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LocalStructureImageFilterTest1_1.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(itkDescoteauxSheetnessImageFilterTest1 
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnesImageFilterTest1_1.mhd
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
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mhd
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mhd
  ${TEMP}/GeodesicActiveContourLevelSetSegmentationModuleTest1_1.mhd
  )


SET_TESTS_PROPERTIES( itkGeodesicActiveContourLevelSetSegmentationModuleTest1 
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

ADD_TEST(itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mhd
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mhd
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest1_1.mhd
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
  )


SET_TESTS_PROPERTIES( itkShapeDetectionLevelSetSegmentationModuleTest1 
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

ADD_TEST(itkShapeDetectionLevelSetSegmentationModuleTest2 
  ${CXX_TEST_PATH}/itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mhd
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mhd
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest2_1.mhd
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
  )

SET_TESTS_PROPERTIES( itkShapeDetectionLevelSetSegmentationModuleTest2 
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)


IF(TEST_CORNELL_DATA_ROOT)

ADD_TEST(itkFeatureAggregatorTest1 
  ${CXX_TEST_PATH}/itkFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FeatureAggregatorTest1_1.mhd
  0.5
  1.0
  )

ADD_TEST(itkMinimumFeatureAggregatorTest1 
  ${CXX_TEST_PATH}/itkMinimumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest1_1.mhd
  0.5
  1.0
  )

ADD_TEST(itkMinimumFeatureAggregatorTest2 
  ${CXX_TEST_PATH}/itkMinimumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest2_1.mhd
  0.5
  1.0
  )

ADD_TEST(itkWeightedSumFeatureAggregatorTest1 
  ${CXX_TEST_PATH}/itkWeightedSumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/WeightedSumFeatureAggregatorTest1_1.mhd
  0.5
  1.0
  )

IF(TEST_CORNELL_COLLECTION)
  INCLUDE(FemurDataTests.cmake)
ENDIF(TEST_CORNELL_COLLECTION)

ENDIF(TEST_CORNELL_DATA_ROOT)

