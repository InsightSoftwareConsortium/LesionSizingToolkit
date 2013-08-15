set(DOCUMENTATION "This module contains a collection of classes for performing
measurement of lesions. Its clinical motivation is the study of cancerous lung
lesions. However, the functionalities developed here are usable in a wider
range of applications. The Lesion Sizint Toolkit is build upon ITK and adds to
it specific classes for computing image features that are then combined into a
detection and measurement layer.")

itk_module(ITKLesionSizingToolkit
 DEPENDS
   ITKImageFeature
   ITKLevelSets
   ITKStatistics
   ITKIOSpatialObjects
   ITKRegionGrowing
   ITKLabelVoting
   ITKMathematicalMorphology
   ITKIOGDCM
   ITKIOMeta
   ITKVtkGlue
   ITKTestKernel #to handle IO in src
 EXCLUDE_FROM_ALL
 DESCRIPTION
   "${DOCUMENTATION}"
)

# Module ITKVtkGlue added for itkImageToVTKImageFilter.h
