
SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})
SET(TEST_DATA_ROOT ${LesionSizingToolkit_SOURCE_DIR}/Data)
SET(TEST_DATA_INPUT ${TEST_DATA_ROOT}/Input)
SET(TEST_DATA_BASELINE ${TEST_DATA_ROOT}/Baseline)
SET(TEMP ${LesionSizingToolkit_BINARY_DIR}/Testing/Temporary)

FIND_FILE(IMAGE_COMPARE_COMMAND
  ImageCompare
  PATHS ${ITK_DIR}/bin
  DOC "ITK Executable for comparing images"
  )

INCLUDE(CollectionTestMacro.cmake)

FIND_PATH(TEST_CORNELL_DATA_ROOT README.txt DOC "Directory with data from the Cornell ELCAP database")
FIND_PATH(TEST_NIST_DATA_ROOT  LevineJResNIST2008b.pdf DOC "Directory with data from the NIST Phantom database")
FIND_PATH(TEST_FDA_DATA_ROOT  phantom_diameter_measures.xls DOC "Directory with data from the FDA Phantom database")
FIND_PATH(TEST_VOLCANO_DATA_ROOT SCseeds.csv DOC "Directory with data from for the MICCAI Volcano challenge. http://www.via.cornell.edu/challenge")

OPTION(TEST_CORNELL_COLLECTION "Run tests in the collection of datasets from Cornell" OFF)
OPTION(TEST_NIST_PHANTOM_COLLECTION "Run tests in the collection of phantom datasets from NIST" OFF)
OPTION(TEST_FDA_PHANTOM_COLLECTION "Run tests in the collection of phantom datasets from FDA" OFF)
OPTION(TEST_VOLCANO_DATA_COLLECTION "Run tests at MICCAI Volcano challenge. http://www.via.cornell.edu/challenge" OFF)


ADD_EXECUTABLE(itkSystemInformation itkSystemInformation.cxx )
TARGET_LINK_LIBRARIES(itkSystemInformation ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(ImageReadWrite ImageReadWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadWrite ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(DicomSeriesReadImageWrite DicomSeriesReadImageWrite.cxx )
TARGET_LINK_LIBRARIES(DicomSeriesReadImageWrite ITKIO)

ADD_EXECUTABLE(ImageReadRegionOfInterestWrite ImageReadRegionOfInterestWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadRegionOfInterestWrite ITKIO)

ADD_EXECUTABLE(ImageReadRegionOfInterestAroundSeedWrite ImageReadRegionOfInterestAroundSeedWrite.cxx )
TARGET_LINK_LIBRARIES(ImageReadRegionOfInterestAroundSeedWrite ITKIO)

ADD_EXECUTABLE(CannyEdgeDetectionImageFilter1 CannyEdgeDetectionImageFilter1.cxx )
TARGET_LINK_LIBRARIES(CannyEdgeDetectionImageFilter1 ITKIO)

ADD_EXECUTABLE(CannyEdgeDetectionImageFilter2 CannyEdgeDetectionImageFilter2.cxx )
TARGET_LINK_LIBRARIES(CannyEdgeDetectionImageFilter2 ITKIO)

ADD_EXECUTABLE(CannyEdgeDetectionImageFilter3 CannyEdgeDetectionImageFilter3.cxx )
TARGET_LINK_LIBRARIES(CannyEdgeDetectionImageFilter3 ITKIO)

ADD_EXECUTABLE(LaplacianRecursiveGaussianImageFilter LaplacianRecursiveGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(LaplacianRecursiveGaussianImageFilter ITKIO)

ADD_EXECUTABLE(itkIsotropicResamplerTest1 itkIsotropicResamplerTest1.cxx )
TARGET_LINK_LIBRARIES(itkIsotropicResamplerTest1 ITKIO)

ADD_EXECUTABLE(ResampleVolumeToBeIsotropic ResampleVolumeToBeIsotropic.cxx )
TARGET_LINK_LIBRARIES(ResampleVolumeToBeIsotropic ITKIO)

ADD_EXECUTABLE(GradientMagnitudeImageFilter GradientMagnitudeImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeImageFilter ITKIO)

ADD_EXECUTABLE(GradientMagnitudeRecursiveGaussianImageFilter GradientMagnitudeRecursiveGaussianImageFilter.cxx )
TARGET_LINK_LIBRARIES(GradientMagnitudeRecursiveGaussianImageFilter ITKIO)

ADD_EXECUTABLE(LandmarkSpatialObjectWriterTest LandmarkSpatialObjectWriterTest.cxx )
TARGET_LINK_LIBRARIES(LandmarkSpatialObjectWriterTest ITKIO)

ADD_EXECUTABLE(itkFeatureGeneratorTest1 itkFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkRegionCompetitionImageFilterTest1 itkRegionCompetitionImageFilterTest1.cxx)
TARGET_LINK_LIBRARIES(itkRegionCompetitionImageFilterTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSegmentationVolumeEstimatorTest1 itkSegmentationVolumeEstimatorTest1.cxx)
TARGET_LINK_LIBRARIES(itkSegmentationVolumeEstimatorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkGrayscaleImageSegmentationVolumeEstimatorTest1 itkGrayscaleImageSegmentationVolumeEstimatorTest1.cxx)
TARGET_LINK_LIBRARIES(itkGrayscaleImageSegmentationVolumeEstimatorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkGrayscaleImageSegmentationVolumeEstimatorTest2 itkGrayscaleImageSegmentationVolumeEstimatorTest2.cxx)
TARGET_LINK_LIBRARIES(itkGrayscaleImageSegmentationVolumeEstimatorTest2 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkVotingBinaryHoleFillFloodingImageFilterTest1 itkVotingBinaryHoleFillFloodingImageFilterTest1.cxx)
TARGET_LINK_LIBRARIES(itkVotingBinaryHoleFillFloodingImageFilterTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLandmarksReaderTest1 itkLandmarksReaderTest1.cxx)
TARGET_LINK_LIBRARIES(itkLandmarksReaderTest1 ${LesionSizingToolkit_LIBRARIES})


ADD_TEST(itkSystemInformation ${CXX_TEST_PATH}/itkSystemInformation)


ADD_EXECUTABLE(itkLesionSegmentationMethodTest1 itkLesionSegmentationMethodTest1.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest2 itkLesionSegmentationMethodTest2.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest2 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest3 itkLesionSegmentationMethodTest3.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest3 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest4 itkLesionSegmentationMethodTest4.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest4 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest5 itkLesionSegmentationMethodTest5.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest5 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest6 itkLesionSegmentationMethodTest6.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest6 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest7 itkLesionSegmentationMethodTest7.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest7 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest8 itkLesionSegmentationMethodTest8.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest8 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest8b itkLesionSegmentationMethodTest8b.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest8b ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest9 itkLesionSegmentationMethodTest9.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest9 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLesionSegmentationMethodTest10 itkLesionSegmentationMethodTest10.cxx )
TARGET_LINK_LIBRARIES(itkLesionSegmentationMethodTest10 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkFeatureAggregatorTest1 itkFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFeatureAggregatorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkMinimumFeatureAggregatorTest1 itkMinimumFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkMinimumFeatureAggregatorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkMinimumFeatureAggregatorTest2 itkMinimumFeatureAggregatorTest2.cxx )
TARGET_LINK_LIBRARIES(itkMinimumFeatureAggregatorTest2 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkMaximumFeatureAggregatorTest1 itkMaximumFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkMaximumFeatureAggregatorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkMaximumFeatureAggregatorTest2 itkMaximumFeatureAggregatorTest2.cxx )
TARGET_LINK_LIBRARIES(itkMaximumFeatureAggregatorTest2 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1 itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1.cxx )
TARGET_LINK_LIBRARIES(itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSatoVesselnessFeatureGeneratorMultiScaleTest1 itkSatoVesselnessFeatureGeneratorMultiScaleTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessFeatureGeneratorMultiScaleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1 itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkWeightedSumFeatureAggregatorTest1 itkWeightedSumFeatureAggregatorTest1.cxx )
TARGET_LINK_LIBRARIES(itkWeightedSumFeatureAggregatorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkCannyEdgesFeatureGeneratorTest1 itkCannyEdgesFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkCannyEdgesDistanceFeatureGeneratorTest1 itkCannyEdgesDistanceFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesDistanceFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 itkGradientMagnitudeSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkGradientMagnitudeSigmoidFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSigmoidFeatureGeneratorTest1 itkSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSigmoidFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkBinaryThresholdFeatureGeneratorTest1 itkBinaryThresholdFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkBinaryThresholdFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLungWallFeatureGeneratorTest1 itkLungWallFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkLungWallFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkMorphologicalOpenningFeatureGeneratorTest1 itkMorphologicalOpenningFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkMorphologicalOpenningFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSatoVesselnessFeatureGeneratorTest1 itkSatoVesselnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSatoVesselnessSigmoidFeatureGeneratorTest1 itkSatoVesselnessSigmoidFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoVesselnessSigmoidFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSatoLocalStructureFeatureGeneratorTest1 itkSatoLocalStructureFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkSatoLocalStructureFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkLocalStructureImageFilterTest1 itkLocalStructureImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkLocalStructureImageFilterTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkDescoteauxSheetnessImageFilterTest1 itkDescoteauxSheetnessImageFilterTest1.cxx )
TARGET_LINK_LIBRARIES(itkDescoteauxSheetnessImageFilterTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkDescoteauxSheetnessImageFilterTest2 itkDescoteauxSheetnessImageFilterTest2.cxx )
TARGET_LINK_LIBRARIES(itkDescoteauxSheetnessImageFilterTest2 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkDescoteauxSheetnessFeatureGeneratorTest1 itkDescoteauxSheetnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkDescoteauxSheetnessFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkFrangiTubularnessFeatureGeneratorTest1 itkFrangiTubularnessFeatureGeneratorTest1.cxx )
TARGET_LINK_LIBRARIES(itkFrangiTubularnessFeatureGeneratorTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkVEDTest itkVEDTest.cxx )
TARGET_LINK_LIBRARIES(itkVEDTest ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSegmentationModuleTest1 itkSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkRegionGrowingSegmentationModuleTest1 itkRegionGrowingSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkRegionGrowingSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkSinglePhaseLevelSetSegmentationModuleTest1 itkSinglePhaseLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkSinglePhaseLevelSetSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkConnectedThresholdSegmentationModuleTest1 itkConnectedThresholdSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkConnectedThresholdSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkConfidenceConnectedSegmentationModuleTest1 itkConfidenceConnectedSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkConfidenceConnectedSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkFastMarchingSegmentationModuleTest1 itkFastMarchingSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkFastMarchingSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 itkGeodesicActiveContourLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_EXECUTABLE(itkShapeDetectionLevelSetSegmentationModuleTest1 itkShapeDetectionLevelSetSegmentationModuleTest1.cxx )
TARGET_LINK_LIBRARIES(itkShapeDetectionLevelSetSegmentationModuleTest1 ${LesionSizingToolkit_LIBRARIES})

ADD_TEST(itkLesionSegmentationMethodTest1 ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest1 )
ADD_TEST(itkLesionSegmentationMethodTest2 ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest2 )

ADD_TEST(itkLesionSegmentationMethodTest3 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LesionSegmentationMethodTest3_1.mha
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

ADD_TEST(itkIsotropicResamplerTest1 
  ${CXX_TEST_PATH}/itkIsotropicResamplerTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/IsotropicResamplerTest1.mha
  )

ADD_TEST(itkLandmarksReaderTest1
  ${EXECUTABLE_OUTPUT_PATH}/itkLandmarksReaderTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  )

ADD_TEST(itkVotingBinaryHoleFillFloodingImageFilterTest1
  ${EXECUTABLE_OUTPUT_PATH}/itkVotingBinaryHoleFillFloodingImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/VotingBinaryHoleFillFloodingImageFilterTest1_1.mha
  -400  # threshold value
  2     # neighborhood radius
  1     # majority
  100   # iterations
  )

ADD_TEST(itkConnectedThresholdSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConnectedThresholdSegmentationModuleTest1_1.mha
  -700
  500
  )

ADD_TEST(itkConfidenceConnectedSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkConfidenceConnectedSegmentationModuleTest1 
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
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
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  0.7
  -10.0
  90.0
  )

ADD_TEST(GradientMagnitudeImageFilterTest1
  ${CXX_TEST_PATH}/GradientMagnitudeImageFilter
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/GradientMagnitudeImageFilterTest1_1.mha
  0.7 # Sigma
  )

ADD_TEST(itkSigmoidFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SigmoidFeatureGeneratorTest1_1.mha
  1.0
  -700.0 
  )

ADD_TEST(itkBinaryThresholdFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkBinaryThresholdFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/BinaryThresholdGeneratorTest1_1.mha
  -700.0 
  )

ADD_TEST(itkLungWallFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LungWallFeatureGeneratorTest1_1.mha
  -400.0 
  )

ADD_TEST(itkMorphologicalOpenningFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkMorphologicalOpenningFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MorphologicalOpenningFeatureGeneratorTest1_1.mha
  -400.0 
  )

ADD_TEST(LandmarkSpatialObjectWriterTest1
  ${CXX_TEST_PATH}/LandmarkSpatialObjectWriterTest
  ${TEMP}/LandmarkSpatialObjectWriterTest1_1.txt
  )


ADD_TEST(CannyEdgeDetectionImageFilterTest1
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest1_1.mha
  )

ADD_TEST(CannyEdgeDetectionImageFilterTest2
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest2_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest2_1.mha
  )

ADD_TEST(CannyEdgeDetectionImageFilterTest3
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter3
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest3_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest3_1.mha
  )

ADD_TEST(CannyEdgeDetectionImageFilterTest4
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter1
  ${TEST_DATA_ROOT}/Synthetic/SphereLesion.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest4_1.mha
  0.7 # Sigma
  30  # Upper hysteresis threshold
  15  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest4_1.mha
  )

ADD_TEST(CannyEdgeDetectionImageFilterTest5
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter2
  ${TEST_DATA_ROOT}/Synthetic/SphereLesion.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest5_1.mha
  0.7 # Sigma
  10  # Upper hysteresis threshold
   5  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest5_1.mha
  )

ADD_TEST(CannyEdgeDetectionImageFilterTest6
  ${CXX_TEST_PATH}/CannyEdgeDetectionImageFilter3
  ${TEST_DATA_ROOT}/Synthetic/SphereLesion.mha
  ${TEMP}/CannyEdgeDetectionImageFilterTest6_1.mha
  0.7 # Sigma
  10  # Upper hysteresis threshold
   5  # Lower hysteresis threshold
  ${TEMP}/CannyEdgeDetectionNonMaximumSuppressionImageFilterTest6_1.mha
  )


IF( IMAGE_COMPARE_COMMAND )

  ADD_TEST(CannyRegressionTest1 ${IMAGE_COMPARE_COMMAND}
    ${TEMP}/CannyEdgeDetectionImageFilterTest1_1.mha
    ${TEST_DATA_BASELINE}/CannyEdgeDetectionImageFilterTest1_1.mha
    )
  ADD_TEST(CannyRegressionTest2 ${IMAGE_COMPARE_COMMAND}
    ${TEMP}/CannyEdgeDetectionImageFilterTest2_1.mha
    ${TEST_DATA_BASELINE}/CannyEdgeDetectionImageFilterTest2_1.mha
    )
  ADD_TEST(CannyRegressionTest3 ${IMAGE_COMPARE_COMMAND}
    ${TEMP}/CannyEdgeDetectionImageFilterTest3_1.mha
    ${TEST_DATA_BASELINE}/CannyEdgeDetectionImageFilterTest3_1.mha
    )
  ADD_TEST(CannyRegressionTest4 ${IMAGE_COMPARE_COMMAND}
    ${TEMP}/CannyEdgeDetectionImageFilterTest4_1.mha
    ${TEST_DATA_BASELINE}/CannyEdgeDetectionImageFilterTest4_1.mha
    )
  ADD_TEST(CannyRegressionTest5 ${IMAGE_COMPARE_COMMAND}
    ${TEMP}/CannyEdgeDetectionImageFilterTest5_1.mha
    ${TEST_DATA_BASELINE}/CannyEdgeDetectionImageFilterTest5_1.mha
    )
  ADD_TEST(CannyRegressionTest6 ${IMAGE_COMPARE_COMMAND}
    ${TEMP}/CannyEdgeDetectionImageFilterTest6_1.mha
    ${TEST_DATA_BASELINE}/CannyEdgeDetectionImageFilterTest6_1.mha
    )

ENDIF( IMAGE_COMPARE_COMMAND )

ADD_TEST(itkCannyEdgesDistanceFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkCannyEdgesDistanceFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  )

ADD_TEST(itkCannyEdgesFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkCannyEdgesFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  )

ADD_TEST(itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkCannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/CannyEdgesDistanceAdvectionFieldFeatureGeneratorTest1_1.mha
  0.7 # Sigma
  150 # Upper hysteresis threshold
  75  # Lower hysteresis threshold
  )

ADD_TEST(itkSatoVesselnessFeatureGeneratorTest1
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorTest1_1.mha
  1.0  # Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
  )

ADD_TEST(itkSatoVesselnessSigmoidFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorTest1_1.mha
  1.0   # Sigma
  0.5   # Alpha 1
  2.0   # Alpha 2
  -10   # Sigmoid Alpha
  80    # Sigmoid Bets
  )

ADD_TEST(itkSatoLocalStructureFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoLocalStructureFeatureGeneratorTest1_1.mha
  1.0
  0.5
  2.0
  )

ADD_TEST(itkLocalStructureImageFilterTest1 
  ${CXX_TEST_PATH}/itkLocalStructureImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LocalStructureImageFilterTest1_1.mha
  1.0
  0.5
  2.0
  )

ADD_TEST(itkDescoteauxSheetnessImageFilterTest1 
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessImageFilterTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnesImageFilterTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )

ADD_TEST(itkDescoteauxSheetnessImageFilterTest2 
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessImageFilterTest2
  ${TEMP}/DescoteauxSheetnesImageFilterTest2_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  1.0    # Sheetness (go easy on the synthetic image)
  0.5    # Bloobiness
  0.5    # Noise
  )

ADD_TEST(itkDescoteauxSheetnessFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnessFeatureGeneratorTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )

ADD_TEST(itkFrangiTubularnessFeatureGeneratorTest1 
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/FrangiTubularnessFeatureGeneratorTest1_1.mha
  1.0
  0.5
  2.0
  )

ADD_TEST(itkGeodesicActiveContourLevelSetSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkGeodesicActiveContourLevelSetSegmentationModuleTest1 
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  ${TEMP}/GeodesicActiveContourLevelSetSegmentationModuleTest1_1.mha
  )


SET_TESTS_PROPERTIES( itkGeodesicActiveContourLevelSetSegmentationModuleTest1 
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

ADD_TEST(itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${CXX_TEST_PATH}/itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/GradientMagnitudeSigmoidFeatureGeneratorTest1_1.mha
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest1_1.mha
  100.0   # Propagation Scaling
    1.0   # Curvature Scaling
  )


SET_TESTS_PROPERTIES( itkShapeDetectionLevelSetSegmentationModuleTest1 
  PROPERTIES DEPENDS itkConfidenceConnectedSegmentationModuleTest1)

ADD_TEST(itkShapeDetectionLevelSetSegmentationModuleTest2 
  ${CXX_TEST_PATH}/itkShapeDetectionLevelSetSegmentationModuleTest1 
  ${TEMP}/ConfidenceConnectedSegmentationModuleTest1_1.mha
  ${TEMP}/CannyEdgesDistanceFeatureGeneratorTest1_1.mha
  ${TEMP}/ShapeDetectionLevelSetSegmentationModuleTest2_1.mha
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
  ${TEMP}/FeatureAggregatorTest1_1.mha
  0.5
  1.0
  )

ADD_TEST(itkMinimumFeatureAggregatorTest1 
  ${CXX_TEST_PATH}/itkMinimumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest1_1.mha
  )

ADD_TEST(itkMinimumFeatureAggregatorTest2 
  ${CXX_TEST_PATH}/itkMinimumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MinimumFeatureAggregatorTest2_1.mha
  )

ADD_TEST(itkMaximumFeatureAggregatorTest1 
  ${CXX_TEST_PATH}/itkMaximumFeatureAggregatorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MaximumFeatureAggregatorTest1_1.mha
  0.5
  1.0
  )

ADD_TEST(itkMaximumFeatureAggregatorTest2 
  ${CXX_TEST_PATH}/itkMaximumFeatureAggregatorTest2
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/MaximumFeatureAggregatorTest2_1.mha
  0.5
  1.0
  )

ADD_TEST(itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1 
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/DescoteauxSheetnessFeatureGeneratorMultiScaleTest1_1.mha
  1      # Search for Bright sheets
  1.0    # Smallest Sigma
  3      # Number of scales
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )

ADD_TEST(itkSatoVesselnessFeatureGeneratorMultiScaleTest1 
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessFeatureGeneratorMultiScaleTest1_1.mha
  1.0  # First Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
  )

ADD_TEST(itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1 
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/SatoVesselnessSigmoidFeatureGeneratorMultiScaleTest1_1.mha
  1.0  # First Sigma
  0.5  # Alpha 1
  2.0  # Alpha 2
  -10  # Sigmoid Alpha
  80   # Sigmoid Bets
  )

ADD_TEST(itkWeightedSumFeatureAggregatorTest1 
  ${CXX_TEST_PATH}/itkWeightedSumFeatureAggregatorTest1
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

