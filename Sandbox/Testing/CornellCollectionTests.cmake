#
#  Helper macro that will run the set of operations in a given dataset.
#
MACRO(TEST_DATASET DATASET_ID DATASET_DIRECTORY)

ADD_TEST(READ${DATASET_ID}
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${TEST_CORNELL_DATA_ROOT}/${DATASET_DIRECTORY}
  ${TEMP}/${DATASET_ID}.mhd
  )

ENDMACRO(TEST_DATASET)

TEST_DATASET(SM0055 "CornellPartSolid2/SM0055-20000101/SM0055/20000101-122401-2-1" )
TEST_DATASET(SL0074 "CornellPartSolid3/SL0074-20000101/SL0074/20000101-134856-2-1" )

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

# Test a solid lesion on the left lobe of the lung
ADD_TEST(Read003-SM0052-20000101 
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${TEST_CORNELL_DATA_ROOT}/SM0052-20000101/SM0052/20000101-154701-2-1
  ${TEMP}/SM0052-20000101_1.mha
  )

ADD_TEST(Read004-SM0054-20000101
  ${CXX_TEST_PATH}/DicomSeriesReadImageWrite
  ${TEST_CORNELL_DATA_ROOT}/SM0054-20000101/SM0054/20000101-141301-2-1
  ${TEMP}/SM0054-20000101_1.mha
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

# Extract ROI of a solid lesion from the left lobe of the lung
ADD_TEST(ROI003-SM0052-20000101 
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/SM0052-20000101_1.mha
  ${TEMP}/SM0052-20000101_1_ROI.mha
  317   193   154
  111   116   85
  )

# Extract ROI of a solid lesion from the left lobe of the lung attached
# heavily to the upper left lung wall.
ADD_TEST(ROI004-SM0054-20000101
  ${CXX_TEST_PATH}/ImageReadRegionOfInterestWrite
  ${TEMP}/SM0054-20000101_1.mha
  ${TEMP}/SM0054-20000101_1_ROI.mha
  324 235 129
  124 103 54
  )

ADD_TEST(GMSFGTest001
  ${CXX_TEST_PATH}/itkGradientMagnitudeSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/GMSFGTest001_01.mhd
  0.7
  -10.0
  90.0
  )

ADD_TEST(SFGTest001
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SFGTest001_01.mhd
  1.0
  -700.0 
  )


ADD_TEST(SFGTest-SM0052-20000101
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/SFGTest-SM0052-20000101.mha
  1.0  -700.0 )

ADD_TEST(SFGTest-SM0054-20000101
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEMP}/SM0054-20000101_1_ROI.mha
  ${TEMP}/SFGTest-SM0054-20000101.mha
  1.0  -700.0 )

ADD_TEST(LWFGTest-CornellPartSolid2
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/LWFGTest-CornellPartSolid2_1.mha -400.0 )

ADD_TEST(LWFGTest-CornellPartSolid1
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LWFGTest-CornellPartSolid1_1.mha -400.0 )

ADD_TEST(LWFGTest-SM0052-20000101
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/LWFGTest_SM0052-20000101_1.mha -400.0 )

ADD_TEST(LWFGTest-SM0054-20000101
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEMP}/SM0054-20000101_1_ROI.mha
  ${TEMP}/LWFGTest_SM0054-20000101_1.mha -400.0 )

ADD_TEST(SVFGTest001
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SVFGTest001_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(SVSFGTest001
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/SVSFGTest001_01.mhd
  1.0
  0.5
  2.0
  -10
  80
  )

ADD_TEST(SVSFGTest-SM0054-20000101
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEMP}/SM0054-20000101_1_ROI.mha
  ${TEMP}/SVSFGTest-SM0054-20000101_1.mha
  1.0 0.5 2.0 )

ADD_TEST(SVSFGTest-SM0052-20000101
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/SVSFGTest-SM0052-20000101_1.mha 
  1.0 0.5 2.0 )


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
  0.7
  -10.0
  90.0
  )

ADD_TEST(SFGTest002
  ${CXX_TEST_PATH}/itkSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SFGTest002_01.mhd
  1.0
  -700.0
  )

ADD_TEST(LWFGTest002
  ${CXX_TEST_PATH}/itkLungWallFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/LWFGTest002_01.mhd
  -400.0
  )

ADD_TEST(SVFGTest002
  ${CXX_TEST_PATH}/itkSatoVesselnessFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SVFGTest002_01.mhd
  1.0
  0.5
  2.0
  )

ADD_TEST(SVSFGTest002
  ${CXX_TEST_PATH}/itkSatoVesselnessSigmoidFeatureGeneratorTest1
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/SVSFGTest002_01.mhd
  1.0
  0.5
  2.0
  -10
  80
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
  ${TEST_DATA_ROOT}/Input/CornellPartSolid2Seeds1.txt
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/CTRGTest001_01.mhd
  -700
  500
  )

ADD_TEST(CTRGTest002
  ${CXX_TEST_PATH}/itkConnectedThresholdSegmentationModuleTest1
  ${TEST_DATA_ROOT}/Input/CornellPartSolid3Seeds1.txt
  ${TEMP}/CornellPartSolid3_1_ROI.mhd
  ${TEMP}/CTRGTest002_01.mhd
  -700
  500
  )

# Part solid lesion with some attachment to the right lung wall.
# CornellPartSolid2/SM0055-20000101
ADD_TEST(LSMT3_CornellPartSolid2
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/CornellPartSolid2Seeds1.txt
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/LesionSegmentationMethodTest3_CornellPartSolid2.mha
  0.5
  1.0
  )

ADD_TEST(LSMT3_SM0052-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/SM0052-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest3_SM0052-20000101.mha
  0.5
  1.0
  )

ADD_TEST(LSMT3_SM0054-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest3
  ${TEST_DATA_ROOT}/Input/SM0054-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0054-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest3_SM0054-20000101.mha
  0.5
  1.0
  )

ADD_TEST(LSMT4_SM0052-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest4
  ${TEST_DATA_ROOT}/Input/SM0052-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest4_SM0052-20000101.mha
  10 5
  )

ADD_TEST(LSMT5_SM0052-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest5
  ${TEST_DATA_ROOT}/Input/SM0052-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest5_SM0052-20000101.mha
  )

# Fast marching from a seed point followed by shape detection level set segmentation
ADD_TEST(LSMT6_SM0052-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${TEST_DATA_ROOT}/Input/SM0052-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest6_SM0052-20000101.mha )

ADD_TEST(LSMT6_SM0054-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${TEST_DATA_ROOT}/Input/SM0054-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0054-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest6_SM0054-20000101.mha )

ADD_TEST(LSMT6_CornellPartSolid2
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${TEST_DATA_ROOT}/Input/CornellPartSolid2Seeds1.txt
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/LesionSegmentationMethodTest6_CornellPartSolid2.mha )

ADD_TEST(LSMT6_CornellPartSolid1
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest6
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LesionSegmentationMethodTest6_CornellPartSolid1.mha  )

# Fast marching from a seed point followed by GAC level set segmentation
ADD_TEST(LSMT7_SM0052-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${TEST_DATA_ROOT}/Input/SM0052-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0052-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest7_SM0052-20000101.mha )

ADD_TEST(LSMT7_SM0054-20000101 
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${TEST_DATA_ROOT}/Input/SM0054-20000101-CroppedSeeds1.txt
  ${TEMP}/SM0054-20000101_1_ROI.mha
  ${TEMP}/LesionSegmentationMethodTest7_SM0054-20000101.mha )

ADD_TEST(LSMT7_CornellPartSolid2
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${TEST_DATA_ROOT}/Input/CornellPartSolid2Seeds1.txt
  ${TEMP}/CornellPartSolid2_1_ROI.mhd
  ${TEMP}/LesionSegmentationMethodTest7_CornellPartSolid2.mha )

ADD_TEST(LSMT7_CornellPartSolid1
  ${CXX_TEST_PATH}/itkLesionSegmentationMethodTest7
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCroppedSeeds1.txt
  ${TEST_DATA_ROOT}/Input/PartSolidLesionCropped.mha
  ${TEMP}/LesionSegmentationMethodTest7_CornellPartSolid1.mha  )


