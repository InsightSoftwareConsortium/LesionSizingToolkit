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
  ${TEMP}/SLSFGTest${DATASET_ID}.mhd
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




TEST_DATASET(SM0055 
  "CornellPartSolid2/SM0055-20000101/SM0055/20000101-122401-2-1"
  270 180 120 
  160 100  69 
  )

TEST_DATASET(SL0074 
  "CornellPartSolid3/SL0074-20000101/SL0074/20000101-134856-2-1"
  100 170 180 
  100 120  64
  )

TEST_DATASET(SM0052 
  "SM0052-20000101/SM0052/20000101-154701-2-1"
  317   193   154
  111   116   85
  )

TEST_DATASET(SM0053 
  "SM0053-20000101/SM0053/20000101-123846-2-1"
  317   193   154
  111   116   85
  )


TEST_DATASET(SM0054
  "SM0054-20000101/SM0054/20000101-141301-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0057
  "SM0057-20000101/SM0057/20000101-113850-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0060
  "SM0060-20000101/SM0060/20000101-132756-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0061
  "SM0061-20000101/SM0061/20000101-150831-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0063
  "SM0063-20000101/SM0063/20000101-084112-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0065
  "SM0065-20000101/SM0065/20000101-125137-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0066
  "SM0066-20000101/SM0066/20000101-085728-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0067
  "SM0067-20000101/SM0067/20000101-133840-2-1"
  324 235 129
  124 103 54
  )

TEST_DATASET(SM0068
  "SM0068-20000101/SM0068/20000101-133453-2-1"
  324 235 129
  124 103 54
  )

