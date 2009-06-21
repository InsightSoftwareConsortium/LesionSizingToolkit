#############################################################################
# Convert DICOM images to meta images
# For instance convert 
#   VOLCANO/SC-20090526/SC0001/1.2.826.0.1.3680043.2.656.4.1.11.1/S02A01/1.2.826.0.1.3680043.2.656.4.1.11.3.*.dcm to SC-20090526-SC0001-1.2.826.0.1.3680043.2.656.4.1.11.1-S02A01.mha

macro( VOLCANO_CONVERT_DCM2MHA 
       VOLCANO_DATA_DIR # Full path to VOLCANO
       COLLECTION_NAME  # SC-20090526
       CASE_NAME        # SC0001
       STUDY_NAME       # 1.2.826.0.1.3680043.2.656.4.1.11.1
       INSTANCE_NAME    # S02A01
     )

  ADD_TEST( DTM_${COLLECTION_NAME}-${CASE_NAME}-${STUDY_NAME}-${INSTANCE_NAME}
    ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
    ${VOLCANO_DATA_DIR}/${COLLECTION_NAME}/${CASE_NAME}/${STUDY_NAME}/${INSTANCE_NAME}/
    ${TEMP}/${COLLECTION_NAME}-${CASE_NAME}-${STUDY_NAME}-${INSTANCE_NAME}.mha
    -IgnoreDirection
    )

endmacro( VOLCANO_CONVERT_DCM2MHA )

        
#############################################################################
macro( TEST_VOLCANO_DATASET
       VOLCANO_DATA_DIR # Full path to VOLCANO
       COLLECTION_NAME  # SC-20090526
       CASE_NAME        # SC0001
       STUDY_NAME       # 1.2.826.0.1.3680043.2.656.4.1.11.1
       INSTANCE_NAME    # S02A01
     )

  VOLCANO_CONVERT_DCM2MHA( 
       ${VOLCANO_DATA_DIR} # Full path to VOLCANO
       ${COLLECTION_NAME}  # SC-20090526
       ${CASE_NAME}        # SC0001
       ${STUDY_NAME}       # 1.2.826.0.1.3680043.2.656.4.1.11.1
       ${INSTANCE_NAME}    # S02A01
      )

  SET( FILENAME_BASE "${TEMP}/${COLLECTION_NAME}-${CASE_NAME}-${STUDY_NAME}-${INSTANCE_NAME}" )
  SET( SEEDS_FILE ${TEST_DATA_ROOT}/Input/${CASE_NAME}_${STUDY_NAME}_Seeds.txt )

  SET(DATASET_ROI ${FILENAME_BASE}_ROI.mha )
  SET(REGION_RADIUS 10)  # in millimeters

  ADD_TEST(ROIS_${COLLECTION_NAME}-${CASE_NAME}-${STUDY_NAME}-${INSTANCE_NAME}
    ${CXX_TEST_PATH}/ImageReadRegionOfInterestAroundSeedWrite
    ${FILENAME_BASE}.mha
    ${DATASET_ROI}
    ${SEEDS_FILE}
    ${REGION_RADIUS}
    )
   

endmacro( TEST_VOLCANO_DATASET )


#############################################################################
macro( TEST_VOLCANO_DATASET_PAIR
       VOLCANO_DATA_DIR # Full path to VOLCANO
       COLLECTION_NAME  # SC-20090526
       CASE_NAME        # SC0001
       STUDY_NAME1      # 1.2.826.0.1.3680043.2.656.4.1.11.1
       STUDY_NAME2      # 1.2.826.0.1.3680043.2.656.4.1.11.4
       INSTANCE_NAME1   # S02A01
       INSTANCE_NAME2   # S02A01
     )

  TEST_VOLCANO_DATASET( 
       ${VOLCANO_DATA_DIR} # Full path to VOLCANO
       ${COLLECTION_NAME}  # SC-20090526
       ${CASE_NAME}        # SC0001
       ${STUDY_NAME1}       # 1.2.826.0.1.3680043.2.656.4.1.11.1
       ${INSTANCE_NAME1}   # S02A01
     )
  TEST_VOLCANO_DATASET( 
       ${VOLCANO_DATA_DIR} # Full path to VOLCANO
       ${COLLECTION_NAME}  # SC-20090526
       ${CASE_NAME}        # SC0001
       ${STUDY_NAME2}       # 1.2.826.0.1.3680043.2.656.4.1.11.1
       ${INSTANCE_NAME2}   # S02A01
     )

endmacro( TEST_VOLCANO_DATASET_PAIR )


