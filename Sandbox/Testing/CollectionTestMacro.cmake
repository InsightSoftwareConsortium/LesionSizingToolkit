#
#  Helper macro that will run the set of operations in a given dataset.
#
MACRO(TEST_DATASET DATASET_ID DATASET_DIRECTORY ROI_X ROI_Y ROI_Z ROI_DX ROI_DY ROI_DZ)

SET(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mhd)

# Dicom to Meta
ADD_TEST(DTM_${DATASET_ID}
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${TEST_CORNELL_DATA_ROOT}/${DATASET_DIRECTORY}
  ${TEMP}/${DATASET_ID}.mhd
  )

# Extract Region of Interest
ADD_TEST(ROI_${DATASET_ID}
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/${DATASET_ID}.mhd
  ${DATASET_ROI}
  ${ROI_X} ${ROI_Y} ${ROI_Z} 
  ${ROI_DX} ${ROI_DY} ${ROI_DZ} 
  )

# Gradient Magnitude Sigmoid Feature Generator
ADD_TEST(GMSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/GMSFGTest${DATASET_ID}.mhd
  0.7    # Sigma
  -10.0  # Alpha
  90.0   # Beta
  )

# Sigmoid Feature Generator
ADD_TEST(SFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SFGTest${DATASET_ID}.mhd
  1.0    # Alpha
  -700.0 # Beta: Lung Threshold
  )

# Lung Wall Feature Generator
ADD_TEST(LWFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/LWFGTest${DATASET_ID}.mha
  -400.0 # Lung Threshold
  )

# Sato Vesselness Feature Generator
ADD_TEST(SVFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SVFGTest${DATASET_ID}.mhd
  1.0   # Sigma
  0.5   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  )

# Sato Vesselness Sigmoid Feature Generator
ADD_TEST(SVSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SVSFGTest${DATASET_ID}.mhd
  1.0   # Sigma
  0.5   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  -10.0 # Sigmoid Alpha
  80.0  # Sigmoid Beta
  )

ADD_TEST(SLSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SLSFGTest${DATASET_ID}.mhd
  1.0  # Sigma
  0.5  # Alpha
  2.0  # Gamma
  )

ADD_TEST(DSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/DSFGTest${DATASET_ID}.mhd
  1.0  # Sigma
  0.5  # Sheetness
  2.0  # Bloobiness
  1.0  # Noise
  )

ADD_TEST(FTFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/FTFGTest${DATASET_ID}.mhd
  1.0  # Sigma
  0.5  # Sheetness
  2.0  # Bloobiness
  1.0  # Noise
  )

ADD_TEST(CTRG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/CTRGTest${DATASET_ID}.mhd
  -700  # Lower Threshold
  500   # Upper Threshold
  )

ADD_TEST(LSMT3_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT3_${DATASET_ID}.mha
  0.5  # Lower Threshold
  1.0  # Upper Threshold
  )

ADD_TEST(LSMT4_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest4
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT4_${DATASET_ID}.mha
  10   # Stopping time for Fast Marching termination
  5    # Distance from seeds for Fast Marching initialization
  )

ADD_TEST(LSMT5_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest5
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT5_${DATASET_ID}.mha
  )

ADD_TEST(LSMT6_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT6_${DATASET_ID}.mha
  )

ADD_TEST(LSMT7_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${TEST_DATA_ROOT}/Input/${DATASET_ID}_Seeds.txt
  ${DATASET_ROI}
  ${TEMP}/LSMT7_${DATASET_ID}.mha
  )


ENDMACRO(TEST_DATASET)


