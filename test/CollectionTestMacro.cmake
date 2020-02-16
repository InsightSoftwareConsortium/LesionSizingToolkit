include( ParseArgumentsMacro.cmake )

#  Helper macro that will run the set of operations in a given dataset.
#
macro(FEATURE_SCREEN_SHOT DATASET_ID OBJECT_ID ISO_VALUE CONTOUR_ID)

set(DATASET_OBJECT_ID ${DATASET_ID}_${OBJECT_ID})
set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)

set(SEEDS_FILE ${TEST_DATA_ROOT}/Input/${DATASET_OBJECT_ID}_Seeds.txt)

add_test(SCRN_${CONTOUR_ID}_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${SEEDS_FILE}
  ${ISO_VALUE}
  1
  ${TEMP}/${CONTOUR_ID}_Test${DATASET_OBJECT_ID}.png
  ${TEMP}/${CONTOUR_ID}_Test${DATASET_ID}.mha
  )

endmacro(FEATURE_SCREEN_SHOT)


macro(VOLUME_ESTIMATION_A DATASET_ID OBJECT_ID SEGMENTATION_METHOD_ID EXPECTED_VOLUME)
set(DATASET_OBJECT_ID ${DATASET_ID}_${OBJECT_ID})
add_test(LSMTVEA${SEGMENTATION_METHOD_ID}_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkGrayscaleImageSegmentationVolumeEstimatorTest2
  ${TEMP}/LSMT${SEGMENTATION_METHOD_ID}_Test${DATASET_OBJECT_ID}.mha
  LSMT${SEGMENTATION_METHOD_ID}
  ${DATASET_ID}
  ${EXPECTED_VOLUME}
  ${TEMP}/VolumeEstimationA_${DATASET_OBJECT_ID}.txt
  )
endmacro(VOLUME_ESTIMATION_A)


macro(VOLUME_ESTIMATION_B DATASET_ID OBJECT_ID SEGMENTATION_METHOD_ID EXPECTED_VOLUME)
set(DATASET_OBJECT_ID ${DATASET_ID}_${OBJECT_ID})
set(ISO_VALUE -0.5) # Compensate for half-pixel shift in Canny-Edges
if(LSTK_USE_VTK)
  add_test(LSMTVEB${SEGMENTATION_METHOD_ID}_${DATASET_OBJECT_ID}
    ${CXX_TEST_PATH}/IsoSurfaceVolumeEstimation
    ${TEMP}/LSMT${SEGMENTATION_METHOD_ID}_Test${DATASET_OBJECT_ID}.mha
    ${ISO_VALUE}
    LSMT${SEGMENTATION_METHOD_ID}
    ${DATASET_ID}
    ${EXPECTED_VOLUME}
    ${TEMP}/VolumeEstimationB_${DATASET_OBJECT_ID}.txt
    )
endif(LSTK_USE_VTK)
endmacro(VOLUME_ESTIMATION_B)


macro(SEGMENTATION_SCREEN_SHOT DATASET_ID OBJECT_ID ISO_VALUE CONTOUR_ID)

set(DATASET_OBJECT_ID ${DATASET_ID}_${OBJECT_ID})
set(SEEDS_FILE ${TEST_DATA_ROOT}/Input/${DATASET_OBJECT_ID}_Seeds.txt)
set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)

add_test(SCRN_${CONTOUR_ID}_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${SEEDS_FILE}
  ${ISO_VALUE}
  1
  ${TEMP}/${CONTOUR_ID}_Test${DATASET_OBJECT_ID}.png
  ${TEMP}/${CONTOUR_ID}_Test${DATASET_OBJECT_ID}.mha
  )

endmacro(SEGMENTATION_SCREEN_SHOT)




macro(CONVERT_DICOM_TO_META COLLECTION_PATH DATASET_ID DATASET_DIRECTORY)

# Dicom to Meta
add_test(DTM_${DATASET_ID}
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${COLLECTION_PATH}/${DATASET_DIRECTORY}
  ${TEMP}/${DATASET_ID}.mha
  )

endmacro(CONVERT_DICOM_TO_META)


macro(EXTRACT_REGION_OF_INTEREST MASTER_DATASET_ID DATASET_ID ROI_X ROI_Y ROI_Z ROI_DX ROI_DY ROI_DZ)

set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)

# Extract Region of Interest
add_test(ROI_${DATASET_ID}
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/${MASTER_DATASET_ID}.mha
  ${DATASET_ROI}
  ${ROI_X} ${ROI_Y} ${ROI_Z} 
  ${ROI_DX} ${ROI_DY} ${ROI_DZ} 
  )

endmacro(EXTRACT_REGION_OF_INTEREST)


macro(GENERATE_FEATURES DATASET_ID)

set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)

# Gradient Magnitude Sigmoid Feature Generator
add_test(GMSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/GMSFG_Test${DATASET_ID}.mha
  0.7    # Sigma
  -0.1   # Alpha
  150.0  # Beta
  )

# Sigmoid Feature Generator
add_test(SFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SFG_Test${DATASET_ID}.mha
   100.0 # Alpha
  -500.0 # Beta: Lung Threshold
  )

# Binary Threshold Feature Generator
add_test(BTFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkBinaryThresholdFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/BTFG_Test${DATASET_ID}.mha
  -200.0 # Beta: Lung Threshold
  )

# Lung Wall Feature Generator
add_test(LWFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/LWFG_Test${DATASET_ID}.mha
  -400.0 # Lung Threshold
  )

# Morphological Openning Feature Generator
add_test(MOFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkMorphologicalOpenningFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/MOFG_Test${DATASET_ID}.mha
  -400.0 # Lung Threshold
  )

# Canny Edges Feature Generator
add_test(CEFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkCannyEdgesFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/CEFG_Test${DATASET_ID}.mha
    1.0 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )

# Canny Edges Feature Generator
add_test(CEDFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkCannyEdgesDistanceFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/CEDFG_Test${DATASET_ID}.mha
    1.0 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )

# Sato Vesselness Feature Generator
add_test(SVFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SVFG_Test${DATASET_ID}.mha
  1.0   # Sigma
  0.5   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  )

# Sato Vesselness Sigmoid Feature Generator
add_test(SVSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SVSFG_Test${DATASET_ID}.mha
  1.0   # Sigma
  0.1   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  -10.0 # Sigmoid Alpha
  80.0  # Sigmoid Beta
  )

# Vessel enhancing diffusion test
add_test(VED_${DATASET_ID}
  ${CXX_TEST_PATH}/itkVEDTest
  ${DATASET_ROI}
  ${TEMP}/VED_Test${DATASET_ID}.mha  )

add_test(SLSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkSatoLocalStructureFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/SLSFG_Test${DATASET_ID}.mha
  1.0  # Sigma
  0.5  # Alpha
  2.0  # Gamma
  )

add_test(DSFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkDescoteauxSheetnessFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/DSFG_Test${DATASET_ID}.mha
  1      # Search for Bright sheets
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )

add_test(FTFG_${DATASET_ID}
  ${CXX_TEST_PATH}/itkFrangiTubularnessFeatureGeneratorTest1
  ${DATASET_ROI}
  ${TEMP}/FTFG_Test${DATASET_ID}.mha
  1.0    # Sigma
  100.0  # Sheetness
  100.0  # Bloobiness
  100.0  # Noise
  )
endmacro(GENERATE_FEATURES)


macro(SCREEN_SHOT_FEATURES DATASET_ID OBJECT_ID)

if( LSTK_USE_VTK )

set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)
set(DATASET_OBJECT_ID ${DATASET_ID}_${OBJECT_ID})

# Screen shots of feature generators
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 GMSFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 SFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 LWFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 MOFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 SVFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 SVSFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 SLSFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 DSFG )
FEATURE_SCREEN_SHOT( ${DATASET_ID} ${OBJECT_ID} 0.5 FTFG )

add_test(SCRN_AFG_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${SEEDS_FILE}
  0.0
  1
  ${TEMP}/SCRN_AFG_${DATASET_OBJECT_ID}.png
  ${TEMP}/GMSFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/SFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/LWFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/MOFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/SVFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/SVSFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/SLSFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/DSFG_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/FTFG_Test${DATASET_OBJECT_ID}.mha
  )
endif( LSTK_USE_VTK )

endmacro(SCREEN_SHOT_FEATURES) 


macro(COMPUTE_SEGMENTATIONS DATASET_ID OBJECT_ID EXPECTED_VOLUME)

PARSE_ARGUMENTS( COMPUTE_SEGMENTATIONS_OPTIONS "" "AROUND_SEED" ${ARGN} )

set(DATASET_OBJECT_ID ${DATASET_ID}_${OBJECT_ID})
set(SEEDS_FILE ${TEST_DATA_ROOT}/Input/${DATASET_OBJECT_ID}_Seeds.txt)

set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha )
set(DATASET_ROI_SEED ${TEMP}/${DATASET_OBJECT_ID}_ROI.mha)
set(REGION_RADIUS 10)  # in millimeters

add_test(ROIS_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestAroundSeedWrite
  ${TEMP}/${DATASET_ID}_ROI.mha
  ${DATASET_ROI_SEED}
  ${SEEDS_FILE}
  ${REGION_RADIUS}
  )

if ( COMPUTE_SEGMENTATIONS_OPTIONS_AROUND_SEED )
  set(DATASET_ROI ${DATASET_ROI_SEED} )  
endif( COMPUTE_SEGMENTATIONS_OPTIONS_AROUND_SEED )

# Resample to isotropic with different interpolation kernels
add_test(RVTI_BSpline_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ResampleVolumeToBeIsotropic
  ${DATASET_ROI}
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_BSpline_Isotropic.mha
  0.2 # 0.2mm
  0   # BSpline interpolation  
  )
add_test(RVTI_HWSinc_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ResampleVolumeToBeIsotropic
  ${DATASET_ROI}
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_HWSinc_Isotropic.mha
  0.2 # 0.2mm
  1   # HammingWindowedSinc
  )
add_test(RVTI_Linear_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ResampleVolumeToBeIsotropic
  ${DATASET_ROI}
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_Linear_Isotropic.mha
  0.2 # 0.2mm
  2   # Linear interpolation  
  )

add_test(CEFG_BSpline_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkCannyEdgesFeatureGeneratorTest1
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_BSpline_Isotropic.mha
  ${TEMP}/CEFG_Test${DATASET_OBJECT_ID}_BSpline_Isotropic_Sigma0.47.mha
    .47 # Sigma
  150.0 # Upper threshold
   25.0 # Lower threshold
  )
add_test(CEFG_HWSinc_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkCannyEdgesFeatureGeneratorTest1
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_HWSinc_Isotropic.mha
  ${TEMP}/CEFG_Test${DATASET_OBJECT_ID}_HWSinc_Isotropic_Sigma0.47.mha
    .47 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )
add_test(CEFG_Linear_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkCannyEdgesFeatureGeneratorTest1
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_Linear_Isotropic.mha
  ${TEMP}/CEFG_Test${DATASET_OBJECT_ID}_Linear_Isotropic_Sigma0.47.mha
    .47 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )

add_test(LFG_BSpline_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/LaplacianRecursiveGaussianImageFilter
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_BSpline_Isotropic.mha
  ${TEMP}/LFG_Test${DATASET_OBJECT_ID}_BSpline_Isotropic_Sigma0.47.mha
    .47 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )
add_test(LFG_HWSinc_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/LaplacianRecursiveGaussianImageFilter
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_HWSinc_Isotropic.mha
  ${TEMP}/LFG_Test${DATASET_OBJECT_ID}_HWSinc_Isotropic_Sigma0.47.mha
    .47 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )
add_test(LFG_Linear_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/LaplacianRecursiveGaussianImageFilter
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_Linear_Isotropic.mha
  ${TEMP}/LFG_Test${DATASET_OBJECT_ID}_Linear_Isotropic_Sigma0.47.mha
    0.47 # Sigma
  150.0 # Upper threshold
   75.0 # Lower threshold
  )

# Sigmoid Feature Generator
add_test(SFG_BSpline_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_BSpline_Isotropic.mha
  ${TEMP}/SFG_BSpline_Test${DATASET_OBJECT_ID}.mha
   100.0 # Alpha
  -500.0 # Beta: Lung Threshold
  )

# Sato Vesselness Sigmoid Feature Generator
add_test(SVSFG_BSpline_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_BSpline_Isotropic.mha
  ${TEMP}/SVSFG_BSpline_Test${DATASET_OBJECT_ID}.mha
  1.0   # Sigma
  0.1   # Vesselness Alpha1
  2.0   # Vesselness Alpha2
  -10.0 # Sigmoid Alpha
  40.0  # Sigmoid Beta
  )

add_test(LWFG_BSpline_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEMP}/RVTI_Test${DATASET_OBJECT_ID}_BSpline_Isotropic.mha
  ${TEMP}/LWFG_BSpline_Test${DATASET_OBJECT_ID}.mha
  -400.0 # Lung Threshold
  )


add_test(CTRG_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/CTRG_Test${DATASET_OBJECT_ID}.mha
  -700  # Lower Threshold
  500   # Upper Threshold
  )

add_test(LSMT3_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT3_Test${DATASET_OBJECT_ID}.mha
  0.5  # Lower Threshold
  1.0  # Upper Threshold
  )

add_test(LSMT4_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest4
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT4_Test${DATASET_OBJECT_ID}.mha
  500   # Stopping time for Fast Marching termination
    5   # Distance from seeds for Fast Marching initialization
  )

add_test(LSMT5_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest5
  ${TEMP}/LSMT4_Test${DATASET_OBJECT_ID}.mha
  ${DATASET_ROI}
  ${TEMP}/LSMT5_Test${DATASET_OBJECT_ID}.mha
  0.0002  # RMS maximum error
  300     # Maximum number of iterations
   1.0    # Curvature scaling
  10.0    # Propagation scaling
  )

add_test(LSMT6_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT6_Test${DATASET_OBJECT_ID}.mha
  0.0002  # RMS maximum error
  300     # Maximum number of iterations
   1.0    # Curvature scaling
  10.0    # Propagation scaling
  500     # Stopping time for Fast Marching termination
    5     # Distance from seeds for Fast Marching initialization
  )

add_test(LSMT7_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT7_Test${DATASET_OBJECT_ID}.mha
  )

add_test(LSMT8_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8_Test${DATASET_OBJECT_ID}.mha
  )

add_test(LSMT8b_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8b
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8b_Test${DATASET_OBJECT_ID}.mha
  -200  # Threshold used for solid lesions
  )

add_test(LSMT8c_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8b
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8c_Test${DATASET_OBJECT_ID}.mha
  -500  # Threshold used for part-solid lesions
  )

add_test(LSMT8d_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8b
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8d_Test${DATASET_OBJECT_ID}.mha
  -200  # Threshold used for solid lesions
  -ResampleThickSliceData     # Supersample
  )

add_test(LSMT8e_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8b
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8e_Test${DATASET_OBJECT_ID}.mha
  -500  # Threshold used for part-solid lesions
  -ResampleThickSliceData     # Supersample
  )

add_test(LSMT8dVED_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8b
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8dVED_Test${DATASET_OBJECT_ID}.mha
  -200  # Threshold used for solid lesions
  -ResampleThickSliceData     # Supersample
  -UseVesselEnhancingDiffusion
  )

add_test(LSMT8eVED_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest8b
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT8eVED_Test${DATASET_OBJECT_ID}.mha
  -500  # Threshold used for part-solid lesions
  -ResampleThickSliceData     # Supersample
  -UseVesselEnhancingDiffusion
  )


add_test(LSMT9_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest9
  ${SEEDS_FILE}
  ${DATASET_ROI}
  ${TEMP}/LSMT9_Test${DATASET_OBJECT_ID}.mha
  )

# Supersampled version
add_test(LSMT10_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest10
  ${SEEDS_FILE}
  ${DATASET_ROI_SEED}
  ${TEMP}/LSMTS8_Test${DATASET_OBJECT_ID}.mha
  )


VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 3 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 4 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 5 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 6 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 7 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 8 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_A( ${DATASET_ID} ${OBJECT_ID} 9 ${EXPECTED_VOLUME} )

VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 3 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 4 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 5 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 6 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 7 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 9 ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8b ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8c ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8d ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8e ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8dVED ${EXPECTED_VOLUME} )
VOLUME_ESTIMATION_B( ${DATASET_ID} ${OBJECT_ID} 8eVED ${EXPECTED_VOLUME} )


if( LSTK_USE_VTK )

# Screen shots of segmentations
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT3 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT4 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT5 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT6 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT7 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT8 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT9 )
SEGMENTATION_SCREEN_SHOT( ${DATASET_ID}  ${OBJECT_ID}  0.0   LSMT8b )

add_test(SCRN_ALSM_${DATASET_OBJECT_ID}
  ${CXX_TEST_PATH}/ViewImageSlicesAndSegmentationContours
  ${DATASET_ROI}
  ${SEEDS_FILE}
  0.0
  1
  ${TEMP}/SCRN_ALSM_${DATASET_OBJECT_ID}.png
  ${TEMP}/LSMT3_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/LSMT4_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/LSMT5_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/LSMT6_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/LSMT7_Test${DATASET_OBJECT_ID}.mha
  ${TEMP}/LSMT8_Test${DATASET_OBJECT_ID}.mha
  )
endif( LSTK_USE_VTK )

endmacro(COMPUTE_SEGMENTATIONS)



macro(TEST_DATASET_FROM_COLLECTION COLLECTION_PATH DATASET_ID DATASET_DIRECTORY ROI_X ROI_Y ROI_Z ROI_DX ROI_DY ROI_DZ)

set(DATASET_ROI ${TEMP}/${DATASET_ID}_ROI.mha)

CONVERT_DICOM_TO_META( ${COLLECTION_PATH} ${DATASET_ID} ${DATASET_DIRECTORY} )
EXTRACT_REGION_OF_INTEREST( ${DATASET_ID} ${ROI_X} ${ROI_Y} ${ROI_Z} ${ROI_DX} ${ROI_DY} ${ROI_DZ} )
GENERATE_FEATURES( ${DATASET_ID} )
COMPUTE_SEGMENTATIONS( ${DATASET_ID} 001 400.0)

endmacro(TEST_DATASET_FROM_COLLECTION)

