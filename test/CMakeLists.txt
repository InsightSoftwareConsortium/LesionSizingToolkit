itk_module_test()
set(ITKLesionSizingToolkitTests
itkBinaryThresholdFeatureGeneratorTest1.cxx
itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1.cxx
itkCannyEdgesDistanceFeatureGeneratorTest1.cxx
itkCannyEdgesFeatureGeneratorTest1.cxx
itkConfidenceConnectedSegmentationModuleTest1.cxx
itkConnectedThresholdSegmentationModuleTest1.cxx
itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1.cxx
itkDescoteauxSheetnessFeatureGeneratorTest1.cxx
itkDescoteauxSheetnessImageFilterTest1.cxx
itkDescoteauxSheetnessImageFilterTest2.cxx
itkFastMarchingSegmentationModuleTest1.cxx
itkFeatureAggregatorTest1.cxx
itkFeatureGeneratorTest1.cxx
itkFrangiTubularnessFeatureGeneratorTest1.cxx
itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx
itkGradientMagnitudeSigmoidFeatureGeneratorTest1.cxx
itkGrayscaleImageSegmentationVolumeEstimatorTest1.cxx
itkGrayscaleImageSegmentationVolumeEstimatorTest2.cxx
itkIsotropicResamplerTest1.cxx
itkLandmarksReaderTest1.cxx
itkLesionSegmentationMethodTest10.cxx
itkLesionSegmentationMethodTest1.cxx
itkLesionSegmentationMethodTest2.cxx
itkLesionSegmentationMethodTest3.cxx
itkLesionSegmentationMethodTest4.cxx
itkLesionSegmentationMethodTest5.cxx
itkLesionSegmentationMethodTest6.cxx
itkLesionSegmentationMethodTest7.cxx
itkLesionSegmentationMethodTest8b.cxx
itkLesionSegmentationMethodTest8.cxx
itkLesionSegmentationMethodTest9.cxx
itkLocalStructureImageFilterTest1.cxx
itkLungWallFeatureGeneratorTest1.cxx
itkMaximumFeatureAggregatorTest1.cxx
itkMaximumFeatureAggregatorTest2.cxx
itkMinimumFeatureAggregatorTest1.cxx
itkMinimumFeatureAggregatorTest2.cxx
itkMorphologicalOpenningFeatureGeneratorTest1.cxx
itkRegionCompetitionImageFilterTest1.cxx
itkRegionGrowingSegmentationModuleTest1.cxx
itkSatoLocalStructureFeatureGeneratorTest1.cxx
itkSatoVesselnessFeatureGeneratorMultiScaleTest1.cxx
itkSatoVesselnessFeatureGeneratorTest1.cxx
itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1.cxx
itkSatoVesselnessSigmoidFeatureGeneratorTest1.cxx
itkSegmentationModuleTest1.cxx
itkSegmentationVolumeEstimatorTest1.cxx
itkShapeDetectionLevelSetSegmentationModuleTest1.cxx
itkSigmoidFeatureGeneratorTest1.cxx
itkSinglePhaseLevelSetSegmentationModuleTest1.cxx
itkVEDTest.cxx
itkVotingBinaryHoleFillFloodingImageFilterTest1.cxx
itkWeightedSumFeatureAggregatorTest1.cxx
LandmarkSpatialObjectWriterTest.cxx
)


set(ITK_TEST_DRIVER itkTestDriver)

CreateTestDriver(ITKLesionSizingToolkit "${ITKLesionSizingToolkit-Test_LIBRARIES}" "${ITKLesionSizingToolkitTests}")

set(TEST_DATA_ROOT "${ITKLesionSizingToolkit_SOURCE_DIR}/Data")
set(TEMP ${ITK_TEST_OUTPUT_DIR})

INCLUDE(CollectionTestMacro.cmake)
FIND_PATH(TEST_CORNELL_DATA_ROOT README.txt DOC "Directory with data from the Cornell ELCAP database")
FIND_PATH(TEST_NIST_DATA_ROOT  LevineJResNIST2008b.pdf DOC "Directory with data from the NIST Phantom database")
FIND_PATH(TEST_FDA_DATA_ROOT  phantom_diameter_measures.xls DOC "Directory with data from the FDA Phantom database")
FIND_PATH(TEST_VOLCANO_DATA_ROOT SCseeds.csv DOC "Directory with data from for the MICCAI Volcano challenge. http://www.via.cornell.edu/challenge")

OPTION(TEST_CORNELL_COLLECTION "Run tests in the collection of datasets from Cornell" OFF)
OPTION(TEST_NIST_PHANTOM_COLLECTION "Run tests in the collection of phantom datasets from NIST" OFF)
OPTION(TEST_FDA_PHANTOM_COLLECTION "Run tests in the collection of phantom datasets from FDA" OFF)
OPTION(TEST_VOLCANO_DATA_COLLECTION "Run tests at MICCAI Volcano challenge. http://www.via.cornell.edu/challenge" OFF)



itk_add_test(NAME itkLesionSegmentationMethodTest1 COMMAND ITKLesionSizingToolkitTestDriver itkLesionSegmentationMethodTest1)
itk_add_test(NAME itkLesionSegmentationMethodTest2 COMMAND ITKLesionSizingToolkitTestDriver itkLesionSegmentationMethodTest2)

itk_add_test(NAME itkLesionSegmentationMethodTest3
  COMMAND ITKLesionSizingToolkitTestDriver itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LesionSegmentationMethodTest3_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkFeatureGeneratorTest1 COMMAND ITKLesionSizingToolkitTestDriver itkFeatureGeneratorTest1)
itk_add_test(NAME itkSegmentationModuleTest1 COMMAND ITKLesionSizingToolkitTestDriver itkSegmentationModuleTest1)
itk_add_test(NAME itkRegionGrowingSegmentationModuleTest1 COMMAND ITKLesionSizingToolkitTestDriver itkRegionGrowingSegmentationModuleTest1)
itk_add_test(NAME itkSinglePhaseLevelSetSegmentationModuleTest1 COMMAND ITKLesionSizingToolkitTestDriver itkSinglePhaseLevelSetSegmentationModuleTest1)

itk_add_test(NAME itkRegionCompetitionImageFilterTest1 COMMAND ITKLesionSizingToolkitTestDriver itkRegionCompetitionImageFilterTest1)

itk_add_test(NAME itkSegmentationVolumeEstimatorTest1 COMMAND ITKLesionSizingToolkitTestDriver itkSegmentationVolumeEstimatorTest1)

itk_add_test(NAME itkGrayscaleImageSegmentationVolumeEstimatorTest1 COMMAND ITKLesionSizingToolkitTestDriver itkGrayscaleImageSegmentationVolumeEstimatorTest1)

itk_add_test(NAME itkIsotropicResamplerTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkIsotropicResamplerTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/IsotropicResamplerTest1.mha
 )

itk_add_test(NAME itkLandmarksReaderTest1
   COMMAND ITKLesionSizingToolkitTestDriver itkLandmarksReaderTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
 )

itk_add_test(NAME itkVotingBinaryHoleFillFloodingImageFilterTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkVotingBinaryHoleFillFloodingImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/VotingBinaryHoleFillFloodingImageFilterTest1_1.mha
  -400  # threshold value
  2     # neighborhood radius
  1     # majority
  100   # iterations
 )

itk_add_test(NAME itkConnectedThresholdSegmentationModuleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkConnectedThresholdSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConnectedThresholdSegmentationModuleTest1_1.mha
  -700
  500
 )

itk_add_test(NAME itkConfidenceConnectedSegmentationModuleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkConfidenceConnectedSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  1.7
 )

itk_add_test(NAME itkFastMarchingSegmentationModuleTest1-PartSolidLesion1
  COMMAND ITKLesionSizingToolkitTestDriver itkFastMarchingSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FastMarchingSegmentationModuleTest1-PartSolidLesion1_1.mha
  10.0 5.0
 )

itk_add_test(NAME itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  0.7
  -10.0
  90.0
 )

itk_add_test(NAME itkGradientMagnitudeImageFilterTest1
  COMMAND ${ITK_TEST_DRIVER}
  $<TARGET_FILE:itkGradientMagnitudeImageFilter>
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/GradientMagnitudeImageFilterTest1_1.mha
  0.7 # Sigma
 )

itk_add_test(NAME itkSigmoidFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SigmoidFeatureGeneratorTest1_1.mha
  1.0
  -700.0
 )

itk_add_test(NAME itkBinaryThresholdFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkBinaryThresholdFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/BinaryThresholdGeneratorTest1_1.mha
  -700.0
 )

itk_add_test(NAME itkLungWallFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkLungWallFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LungWallFeatureGeneratorTest1_1.mha
  -400.0
 )

itk_add_test(NAME itkMorphologicalOpenningFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkMorphologicalOpenningFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MorphologicalOpenningFeatureGeneratorTest1_1.mha
  -400.0
 )

itk_add_test(NAME LandmarkSpatialObjectWriterTest1
  COMMAND ITKLesionSizingToolkitTestDriver LandmarkSpatialObjectWriterTest
  ${TEMP}/LandmarkSpatialObjectWriterTest1_1.txt
 )

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest1
  COMMAND ${ITK_TEST_DRIVER}
--compare  ${TEST_DATA_ROOT}/Baseline/CannyEdgeDetectionImageFilterTest1_1.mha
           ${TEMP}/CannyEdgeDetectionImageFilterTest1_1.mha
  $<TARGET_FILE:itkCannyEdgeDetectionImageFilter1>
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest1_1.mha
 )

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest2
  COMMAND ${ITK_TEST_DRIVER}
--compare ${TEMP}/CannyEdgeDetectionImageFilterTest2_1.mha
          ${TEST_DATA_ROOT}/Baseline/CannyEdgeDetectionImageFilterTest2_1.mha
  $<TARGET_FILE:itkCannyEdgeDetectionImageFilter2>
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest2_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest2_1.mha
 )

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest3
  COMMAND ${ITK_TEST_DRIVER}
--compare ${TEMP}/CannyEdgeDetectionImageFilterTest3_1.mha
          ${TEST_DATA_ROOT}/Baseline/CannyEdgeDetectionImageFilterTest3_1.mha
  $<TARGET_FILE:itkCannyEdgeDetectionImageFilter3>
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest3_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest3_1.mha
 )

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest4
  COMMAND ${ITK_TEST_DRIVER}
--compare ${TEMP}/CannyEdgeDetectionImageFilterTest4_1.mha
          ${TEST_DATA_ROOT}/Baseline/CannyEdgeDetectionImageFilterTest4_1.mha
  $<TARGET_FILE:itkCannyEdgeDetectionImageFilter1>
  ${TEST_DATA_ROOT}/Synthetic/SphereLesion.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest4_1.mha
  0.7 # Sigma
  30  # Upper hysteresis threshold
  15  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest4_1.mha
 )

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest5
  COMMAND ${ITK_TEST_DRIVER}
--compare ${TEMP}/CannyEdgeDetectionImageFilterTest5_1.mha
          ${TEST_DATA_ROOT}/Baseline/CannyEdgeDetectionImageFilterTest5_1.mha
  $<TARGET_FILE:itkCannyEdgeDetectionImageFilter2>
  ${TEST_DATA_ROOT}/Synthetic/SphereLesion.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest5_1.mha
  0.7 # Sigma
  10  # Upper hysteresis threshold
   5  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest5_1.mha
 )

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest6
  COMMAND ${ITK_TEST_DRIVER}
--compare ${TEMP}/CannyEdgeDetectionImageFilterTest6_1.mha
          ${TEST_DATA_ROOT}/Baseline/CannyEdgeDetectionImageFilterTest6_1.mha
  $<TARGET_FILE:itkCannyEdgeDetectionImageFilter3>
  ${TEST_DATA_ROOT}/Synthetic/SphereLesion.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest6_1.mha
  0.7 # Sigma
  10  # Upper hysteresis threshold
   5  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest6_1.mha
 )




itk_add_test(NAME itkCannyEdgesDistanceFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkCannyEdgesDistanceFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
 )

itk_add_test(NAME itkCannyEdgesFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkCannyEdgesFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
 )

itk_add_test(NAME itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
 )

itk_add_test(NAME itkSatoVesselnessFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkSatoVesselnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorTest1_1.mha
  1.0  # Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
 )

itk_add_test(NAME itkSatoVesselnessSigmoidFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorTest1_1.mha
  1.0   # Sigma
  0.5   # Alpha 1
  2.0   # Alpha 2
  -10   # Sigmoid Alpha
  80    # Sigmoid Bets
 )

itk_add_test(NAME itkSatoLocalStructureFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkSatoLocalStructureFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoLocalStructureFeatureGeneratorTest1_1.mha
  1.0
  0.5
  2.0
 )

itk_add_test(NAME itkLocalStructureImageFilterTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkLocalStructureImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LocalStructureImageFilterTest1_1.mha
  1.0
  0.5
  2.0
 )

itk_add_test(NAME itkDescoteauxSheetnessImageFilterTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkDescoteauxSheetnessImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnesImageFilterTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
 )

itk_add_test(NAME itkDescoteauxSheetnessImageFilterTest2
  COMMAND ITKLesionSizingToolkitTestDriver itkDescoteauxSheetnessImageFilterTest2
  ${TEMP}/DescoteauxSheetnesImageFilterTest2_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  1.0    # Sheetness (go easy on the synthetic image)
  0.5    # Bloobiness
  0.5    # Noise
 )

itk_add_test(NAME itkDescoteauxSheetnessFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkDescoteauxSheetnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnessFeatureGeneratorTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
 )

itk_add_test(NAME itkFrangiTubularnessFeatureGeneratorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkFrangiTubularnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FrangiTubularnessFeatureGeneratorTest1_1.mha
  1.0
  0.5
  2.0
 )

itk_add_test(NAME itkGeodesicActiveContourLevelSetSegmentationModuleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkGeodesicActiveContourLevelSetSegmentationModuleTest1
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  ${TEMP}/GeodesicActiveContourLevelSetSegmentationModuleTest1_1.mha
 )


SET_TESTS_PROPERTIES( itkGeodesicActiveContourLevelSetSegmentationModuleTest1
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

itk_add_test(NAME itkShapeDetectionLevelSetSegmentationModuleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkShapeDetectionLevelSetSegmentationModuleTest1
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest1_1.mha
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
 )


SET_TESTS_PROPERTIES( itkShapeDetectionLevelSetSegmentationModuleTest1
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

itk_add_test(NAME itkShapeDetectionLevelSetSegmentationModuleTest2
  COMMAND ITKLesionSizingToolkitTestDriver itkShapeDetectionLevelSetSegmentationModuleTest1
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mha
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest2_1.mha
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
 )

SET_TESTS_PROPERTIES( itkShapeDetectionLevelSetSegmentationModuleTest2
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)


IF(TEST_CORNELL_DATA_ROOT)

itk_add_test(NAME itkFeatureAggregatorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FeatureAggregatorTest1_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkMinimumFeatureAggregatorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkMinimumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest1_1.mha
 )

itk_add_test(NAME itkMinimumFeatureAggregatorTest2
  COMMAND ITKLesionSizingToolkitTestDriver itkMinimumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest2_1.mha
 )

itk_add_test(NAME itkMaximumFeatureAggregatorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkMaximumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MaximumFeatureAggregatorTest1_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkMaximumFeatureAggregatorTest2
  COMMAND ITKLesionSizingToolkitTestDriver itkMaximumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MaximumFeatureAggregatorTest2_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnessFeatureGeneratorMultiScaleTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Smallest Sigma
  3      # Number of scales
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
 )

itk_add_test(NAME itkSatoVesselnessFeatureGeneratorMultiScaleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkSatoVesselnessFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorMultiScaleTest1_1.mha
  1.0  # First Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
 )

itk_add_test(NAME itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1_1.mha
  1.0  # First Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
  -10  # Sigmoid Alpha
  80   # Sigmoid Bets
 )

itk_add_test(NAME itkWeightedSumFeatureAggregatorTest1
  COMMAND ITKLesionSizingToolkitTestDriver itkWeightedSumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/WeightedSumFeatureAggregatorTest1_1.mha
  0.5
  1.0
 )

ENDIF(TEST_CORNELL_DATA_ROOT)

IF(TEST_CORNELL_COLLECTION)
  IF(TEST_CORNELL_DATA_ROOT)
    INCLUDE(CornellCollectionTests.cmake)
  ENDIF(TEST_CORNELL_DATA_ROOT)
ENDIF(TEST_CORNELL_COLLECTION)

IF(TEST_NIST_PHANTOM_COLLECTION)
  IF(TEST_NIST_DATA_ROOT)
    INCLUDE(NISTPhantomCollectionTests.cmake)
  ENDIF(TEST_NIST_DATA_ROOT)
ENDIF(TEST_NIST_PHANTOM_COLLECTION)

IF(TEST_FDA_PHANTOM_COLLECTION)
  IF(TEST_FDA_DATA_ROOT)
    INCLUDE(FDAPhantomCollectionTests.cmake)
  ENDIF(TEST_FDA_DATA_ROOT)
ENDIF(TEST_FDA_PHANTOM_COLLECTION)

IF(TEST_VOLCANO_DATA_COLLECTION)
  IF(TEST_VOLCANO_DATA_ROOT)
    INCLUDE(VOLCANOCollectionTests.cmake)
  ENDIF(TEST_VOLCANO_DATA_ROOT)
ENDIF(TEST_VOLCANO_DATA_COLLECTION)

