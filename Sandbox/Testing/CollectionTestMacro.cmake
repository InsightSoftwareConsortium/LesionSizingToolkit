#
#  Helper macro that will run the set of operations in a given dataset.
#
MACRO(SCREEN_SHOT DATASET_ID ISO_VALUE CONTOUR_ID)

ADD_TEST(SCRN_${CONTOUR_ID}_${DATASET_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${ISO_VALUE}
  1
  ${TEMP}/${CONTOUR_ID}_Test${DATASET_ID}.png
  ${TEMP}/${CONTOUR_ID}_Test${DATASET_ID}.mha
  )

ENDMACRO(SCREEN_SHOT)



MACRO(TEST_DATASET_FROM_COLLECTION COLLECTION_PATH DATASET_ID DATASET_DIRECTORY ROI_X ROI_Y ROI_Z ROI_DX ROI_DY ROI_DZ)

SET(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)

# Dicom to Meta
ADD_TEST(DTM_${DATASET_ID}
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${COLLECTION_PATH}/${DATASET_DIRECTORY}
  ${TEMP}/${DATASET_ID}.mha
  )

# Extract Region of Interest
ADD_TEST(ROI_${DATASET_ID}
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/${DATASET_ID}.mha
  ${DATASET_ROI}
  ${ROI_X} ${ROI_Y} ${ROI_Z} 
  ${ROI_DX} ${ROI_DY} ${ROI_DZ} 
  )

# Gradient Magnitude Sigmoid Feature Generator
ADD_TEST(GMSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/GMSFG_Test${DATASET_ID}.mha
  0.7    # Sigma
  -0.1   # Alpha
  150.0  # Beta
  )

# Sigmoid Feature Generator
ADD_TEST(SFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SFG_Test${DATASET_ID}.mha
   100.0 # Alpha
  -200.0 # Beta: Lung Threshold
  )

# Lung Wall Feature Generator
ADD_TEST(LWFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/LWFG_Test${DATASET_ID}.mha
  -400.0 # Lung Threshold
  )

# Morphological Openning Feature Generator
ADD_TEST(MOFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkMorphologicalOpenningFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/MOFG_Test${DATASET_ID}.mha
  -400.0 # Lung Threshold
  )

# Canny Edges Feature Generator
ADD_TEST(CEFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkCannyEdgesDistanceFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/CEFG_Test${DATASET_ID}.mha
  1.0 # Variance
  5.0 # Upper threshold
  0.5 # Lower threshold
  )

# Sato Vesselness Feature Generator
ADD_TEST(SVFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SVFG_Test${DATASET_ID}.mha
  1.0   # Sigma
  0.5   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  )

# Sato Vesselness Sigmoid Feature Generator
ADD_TEST(SVSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SVSFG_Test${DATASET_ID}.mha
  1.0   # Sigma
  0.5   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  -10.0 # Sigmoid Alpha
  80.0  # Sigmoid Beta
  )

ADD_TEST(SLSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SLSFG_Test${DATASET_ID}.mha
  1.0  # Sigma
  0.5  # Alpha
  2.0  # Gamma
  )

ADD_TEST(DSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/DSFG_Test${DATASET_ID}.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )

ADD_TEST(FTFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/FTFG_Test${DATASET_ID}.mha
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )

ADD_TEST(CTRG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/CTRG_Test${DATASET_ID}.mha
  -700  # Lower Threshold
  500   # Upper Threshold
  )

IF( LSTK_SANDBOX_USE_VTK )

# Screen shots of feature generators
SCREEN_SHOT( ${DATASET_ID} 0.5 GMSFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 SFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 LWFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 MOFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 SVFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 SVSFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 SLSFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 DSFG )
SCREEN_SHOT( ${DATASET_ID} 0.5 FTFG )

ADD_TEST(SCRN_AFG_${DATASET_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  0.0
  1
  ${TEMP}/SCRN_AFG_${DATASET_ID}.png
  ${TEMP}/GMSFG_Test${DATASET_ID}.mha
  ${TEMP}/SFG_Test${DATASET_ID}.mha
  ${TEMP}/LWFG_Test${DATASET_ID}.mha
  ${TEMP}/MOFG_Test${DATASET_ID}.mha
  ${TEMP}/SVFG_Test${DATASET_ID}.mha
  ${TEMP}/SVSFG_Test${DATASET_ID}.mha
  ${TEMP}/SLSFG_Test${DATASET_ID}.mha
  ${TEMP}/DSFG_Test${DATASET_ID}.mha
  ${TEMP}/FTFG_Test${DATASET_ID}.mha
  )
ENDIF( LSTK_SANDBOX_USE_VTK )

ADD_TEST(LSMT3_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT3_Test${DATASET_ID}.mha
  0.5  # Lower Threshold
  1.0  # Upper Threshold
  )

ADD_TEST(LSMT4_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest4
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT4_Test${DATASET_ID}.mha
  500   # Stopping time for Fast Marching termination
    5   # Distance from seeds for Fast Marching initialization
  )

ADD_TEST(LSMT5_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest5
  ${TEMP}/LSMT4_Test${DATASET_ID}.mha
  ${DATASET_ROI}
  ${TEMP}/LSMT5_Test${DATASET_ID}.mha
  0.0002  # RMS maximum error
  300     # Maximum number of iterations
   1.0    # Curvature scaling
  10.0    # Propagation scaling
  )

ADD_TEST(LSMT6_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT6_Test${DATASET_ID}.mha
  0.0002  # RMS maximum error
  300     # Maximum number of iterations
   1.0    # Curvature scaling
  10.0    # Propagation scaling
  500     # Stopping time for Fast Marching termination
    5     # Distance from seeds for Fast Marching initialization
  )

ADD_TEST(LSMT7_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT7_Test${DATASET_ID}.mha
  )

ADD_TEST(LSMTVE3_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest2
  ${TEMP}/LSMT3_Test${DATASET_ID}.mha
  LSMT3
  ${DATASET_ID}
  ${TEMP}/VolumeEstimation_${DATASET_ID}.txt
  )

ADD_TEST(LSMTVE4_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest2
  ${TEMP}/LSMT4_Test${DATASET_ID}.mha
  LSMT4
  ${DATASET_ID}
  ${TEMP}/VolumeEstimation_${DATASET_ID}.txt
  )

ADD_TEST(LSMTVE5_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest2
  ${TEMP}/LSMT5_Test${DATASET_ID}.mha
  LSMT5
  ${DATASET_ID}
  ${TEMP}/VolumeEstimation_${DATASET_ID}.txt
  )

ADD_TEST(LSMTVE6_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest2
  ${TEMP}/LSMT6_Test${DATASET_ID}.mha
  LSMT6
  ${DATASET_ID}
  ${TEMP}/VolumeEstimation_${DATASET_ID}.txt
  )

ADD_TEST(LSMTVE7_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest2
  ${TEMP}/LSMT7_Test${DATASET_ID}.mha
  LSMT7
  ${DATASET_ID}
  ${TEMP}/VolumeEstimation_${DATASET_ID}.txt
  )

IF( LSTK_SANDBOX_USE_VTK )

# Screen shots of segmentations
SCREEN_SHOT( ${DATASET_ID}   0.0   LSMT3 )
SCREEN_SHOT( ${DATASET_ID}   0.0   LSMT4 )
SCREEN_SHOT( ${DATASET_ID}   0.0   LSMT5 )
SCREEN_SHOT( ${DATASET_ID}   0.0   LSMT6 )
SCREEN_SHOT( ${DATASET_ID}   0.0   LSMT7 )

ADD_TEST(SCRN_ALSM_${DATASET_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  0.0
  1
  ${TEMP}/SCRN_ALSM_${DATASET_ID}.png
  ${TEMP}/LSMT3_Test${DATASET_ID}.mha
  ${TEMP}/LSMT4_Test${DATASET_ID}.mha
  ${TEMP}/LSMT5_Test${DATASET_ID}.mha
  ${TEMP}/LSMT6_Test${DATASET_ID}.mha
  ${TEMP}/LSMT7_Test${DATASET_ID}.mha
  )
ENDIF( LSTK_SANDBOX_USE_VTK )

ENDMACRO(TEST_DATASET_FROM_COLLECTION)

