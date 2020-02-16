itk_module_test()
set(LesionSizingToolkitTests
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
itkMorphologicalOpeningFeatureGeneratorTest1.cxx
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

CreateTestDriver(LesionSizingToolkit "${LesionSizingToolkit-Test_LIBRARIES}" "${LesionSizingToolkitTests}")

set(TEST_DATA_ROOT "${LesionSizingToolkit_SOURCE_DIR}/Data")
set(TEMP ${ITK_TEST_OUTPUT_DIR})

include(CollectionTestMacro.cmake)
find_path(TEST_CORNELL_DATA_ROOT README.txt DOC "Directory with data from the Cornell ELCAP database")
find_path(TEST_NIST_DATA_ROOT  LevineJResNIST2008b.pdf DOC "Directory with data from the NIST Phantom database")
find_path(TEST_FDA_DATA_ROOT  phantom_diameter_measures.xls DOC "Directory with data from the FDA Phantom database")
find_path(TEST_VOLCANO_DATA_ROOT SCseeds.csv DOC "Directory with data from for the MICCAI Volcano challenge. http://www.via.cornell.edu/challenge")

option(TEST_CORNELL_COLLECTION "Run tests in the collection of datasets from Cornell" OFF)
option(TEST_NIST_PHANTOM_COLLECTION "Run tests in the collection of phantom datasets from NIST" OFF)
option(TEST_FDA_PHANTOM_COLLECTION "Run tests in the collection of phantom datasets from FDA" OFF)
option(TEST_VOLCANO_DATA_COLLECTION "Run tests at MICCAI Volcano challenge. http://www.via.cornell.edu/challenge" OFF)



itk_add_test(NAME itkLesionSegmentationMethodTest1 COMMAND LesionSizingToolkitTestDriver itkLesionSegmentationMethodTest1)
itk_add_test(NAME itkLesionSegmentationMethodTest2 COMMAND LesionSizingToolkitTestDriver itkLesionSegmentationMethodTest2)

itk_add_test(NAME itkLesionSegmentationMethodTest3
  COMMAND LesionSizingToolkitTestDriver itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LesionSegmentationMethodTest3_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkFeatureGeneratorTest1 COMMAND LesionSizingToolkitTestDriver itkFeatureGeneratorTest1)
itk_add_test(NAME itkSegmentationModuleTest1 COMMAND LesionSizingToolkitTestDriver itkSegmentationModuleTest1)
itk_add_test(NAME itkRegionGrowingSegmentationModuleTest1 COMMAND LesionSizingToolkitTestDriver itkRegionGrowingSegmentationModuleTest1)
itk_add_test(NAME itkSinglePhaseLevelSetSegmentationModuleTest1 COMMAND LesionSizingToolkitTestDriver itkSinglePhaseLevelSetSegmentationModuleTest1)

itk_add_test(NAME itkRegionCompetitionImageFilterTest1 COMMAND LesionSizingToolkitTestDriver itkRegionCompetitionImageFilterTest1)

itk_add_test(NAME itkSegmentationVolumeEstimatorTest1 COMMAND LesionSizingToolkitTestDriver itkSegmentationVolumeEstimatorTest1)

itk_add_test(NAME itkGrayscaleImageSegmentationVolumeEstimatorTest1 COMMAND LesionSizingToolkitTestDriver itkGrayscaleImageSegmentationVolumeEstimatorTest1)

itk_add_test(NAME itkIsotropicResamplerTest1
  COMMAND LesionSizingToolkitTestDriver itkIsotropicResamplerTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/IsotropicResamplerTest1.mha
 )

itk_add_test(NAME itkLandmarksReaderTest1
   COMMAND LesionSizingToolkitTestDriver itkLandmarksReaderTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
 )

itk_add_test(NAME itkVotingBinaryHoleFillFloodingImageFilterTest1
  COMMAND LesionSizingToolkitTestDriver itkVotingBinaryHoleFillFloodingImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/VotingBinaryHoleFillFloodingImageFilterTest1_1.mha
  -400  # threshold value
  2     # neighborhood radius
  1     # majority
  100   # iterations
 )

itk_add_test(NAME itkConnectedThresholdSegmentationModuleTest1
  COMMAND LesionSizingToolkitTestDriver itkConnectedThresholdSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConnectedThresholdSegmentationModuleTest1_1.mha
  -700
  500
 )

itk_add_test(NAME itkConfidenceConnectedSegmentationModuleTest1
  COMMAND LesionSizingToolkitTestDriver itkConfidenceConnectedSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  1.7
 )

itk_add_test(NAME itkFastMarchingSegmentationModuleTest1-PartSolidLesion1
  COMMAND LesionSizingToolkitTestDriver itkFastMarchingSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FastMarchingSegmentationModuleTest1-PartSolidLesion1_1.mha
  10.0 5.0
 )

itk_add_test(NAME itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkGradientMagnitudeSigmoidFeatureGeneratorTest1
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
  COMMAND LesionSizingToolkitTestDriver itkSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SigmoidFeatureGeneratorTest1_1.mha
  1.0
  -700.0
 )

itk_add_test(NAME itkBinaryThresholdFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkBinaryThresholdFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/BinaryThresholdGeneratorTest1_1.mha
  -700.0
 )

itk_add_test(NAME itkLungWallFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkLungWallFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LungWallFeatureGeneratorTest1_1.mha
  -400.0
 )

itk_add_test(NAME itkMorphologicalOpeningFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkMorphologicalOpeningFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MorphologicalOpeningFeatureGeneratorTest1_1.mha
  -400.0
 )

itk_add_test(NAME LandmarkSpatialObjectWriterTest1
  COMMAND LesionSizingToolkitTestDriver LandmarkSpatialObjectWriterTest
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

itk_add_test(NAME itkCannyEdgeDetectionImageFilterTest4
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


itk_add_test(NAME itkCannyEdgesDistanceFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkCannyEdgesDistanceFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
 )

itk_add_test(NAME itkCannyEdgesFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkCannyEdgesFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
 )

itk_add_test(NAME itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
 )

itk_add_test(NAME itkSatoVesselnessFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkSatoVesselnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorTest1_1.mha
  1.0  # Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
 )

itk_add_test(NAME itkSatoVesselnessSigmoidFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorTest1_1.mha
  1.0   # Sigma
  0.5   # Alpha 1
  2.0   # Alpha 2
  -10   # Sigmoid Alpha
  80    # Sigmoid Bets
 )

itk_add_test(NAME itkSatoLocalStructureFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkSatoLocalStructureFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoLocalStructureFeatureGeneratorTest1_1.mha
  1.0
  0.5
  2.0
 )

itk_add_test(NAME itkLocalStructureImageFilterTest1
  COMMAND LesionSizingToolkitTestDriver itkLocalStructureImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LocalStructureImageFilterTest1_1.mha
  1.0
  0.5
  2.0
 )

itk_add_test(NAME itkDescoteauxSheetnessImageFilterTest1
  COMMAND LesionSizingToolkitTestDriver itkDescoteauxSheetnessImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnesImageFilterTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
 )

itk_add_test(NAME itkDescoteauxSheetnessImageFilterTest2
  COMMAND LesionSizingToolkitTestDriver itkDescoteauxSheetnessImageFilterTest2
  ${TEMP}/DescoteauxSheetnesImageFilterTest2_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  1.0    # Sheetness (go easy on the synthetic image)
  0.5    # Bloobiness
  0.5    # Noise
 )

itk_add_test(NAME itkDescoteauxSheetnessFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkDescoteauxSheetnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnessFeatureGeneratorTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
 )

itk_add_test(NAME itkFrangiTubularnessFeatureGeneratorTest1
  COMMAND LesionSizingToolkitTestDriver itkFrangiTubularnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FrangiTubularnessFeatureGeneratorTest1_1.mha
  1.0
  0.5
  2.0
 )

itk_add_test(NAME itkGeodesicActiveContourLevelSetSegmentationModuleTest1
  COMMAND LesionSizingToolkitTestDriver itkGeodesicActiveContourLevelSetSegmentationModuleTest1
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  ${TEMP}/GeodesicActiveContourLevelSetSegmentationModuleTest1_1.mha
 )


set_tests_properties( itkGeodesicActiveContourLevelSetSegmentationModuleTest1
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

itk_add_test(NAME itkShapeDetectionLevelSetSegmentationModuleTest1
  COMMAND LesionSizingToolkitTestDriver itkShapeDetectionLevelSetSegmentationModuleTest1
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest1_1.mha
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
 )


set_tests_properties( itkShapeDetectionLevelSetSegmentationModuleTest1
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

itk_add_test(NAME itkShapeDetectionLevelSetSegmentationModuleTest2
  COMMAND LesionSizingToolkitTestDriver itkShapeDetectionLevelSetSegmentationModuleTest1
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mha
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest2_1.mha
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
 )

set_tests_properties( itkShapeDetectionLevelSetSegmentationModuleTest2
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)


if(TEST_CORNELL_DATA_ROOT)

itk_add_test(NAME itkFeatureAggregatorTest1
  COMMAND LesionSizingToolkitTestDriver itkFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FeatureAggregatorTest1_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkMinimumFeatureAggregatorTest1
  COMMAND LesionSizingToolkitTestDriver itkMinimumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest1_1.mha
 )

itk_add_test(NAME itkMinimumFeatureAggregatorTest2
  COMMAND LesionSizingToolkitTestDriver itkMinimumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest2_1.mha
 )

itk_add_test(NAME itkMaximumFeatureAggregatorTest1
  COMMAND LesionSizingToolkitTestDriver itkMaximumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MaximumFeatureAggregatorTest1_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkMaximumFeatureAggregatorTest2
  COMMAND LesionSizingToolkitTestDriver itkMaximumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MaximumFeatureAggregatorTest2_1.mha
  0.5
  1.0
 )

itk_add_test(NAME itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1
  COMMAND LesionSizingToolkitTestDriver itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1
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
  COMMAND LesionSizingToolkitTestDriver itkSatoVesselnessFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorMultiScaleTest1_1.mha
  1.0  # First Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
 )

itk_add_test(NAME itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1
  COMMAND LesionSizingToolkitTestDriver itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1_1.mha
  1.0  # First Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
  -10  # Sigmoid Alpha
  80   # Sigmoid Bets
 )

itk_add_test(NAME itkWeightedSumFeatureAggregatorTest1
  COMMAND LesionSizingToolkitTestDriver itkWeightedSumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/WeightedSumFeatureAggregatorTest1_1.mha
  0.5
  1.0
 )

endif(TEST_CORNELL_DATA_ROOT)

if(TEST_CORNELL_COLLECTION)
  if(TEST_CORNELL_DATA_ROOT)
    include(CornellCollectionTests.cmake)
  endif(TEST_CORNELL_DATA_ROOT)
endif(TEST_CORNELL_COLLECTION)

if(TEST_NIST_PHANTOM_COLLECTION)
  if(TEST_NIST_DATA_ROOT)
    include(NISTPhantomCollectionTests.cmake)
  endif(TEST_NIST_DATA_ROOT)
endif(TEST_NIST_PHANTOM_COLLECTION)

if(TEST_FDA_PHANTOM_COLLECTION)
  if(TEST_FDA_DATA_ROOT)
    include(FDAPhantomCollectionTests.cmake)
  endif(TEST_FDA_DATA_ROOT)
endif(TEST_FDA_PHANTOM_COLLECTION)

if(TEST_VOLCANO_DATA_COLLECTION)
  if(TEST_VOLCANO_DATA_ROOT)
    include(VOLCANOCollectionTests.cmake)
  endif(TEST_VOLCANO_DATA_ROOT)
endif(TEST_VOLCANO_DATA_COLLECTION)

