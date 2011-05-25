set(DOCUMENTATION "This module contains a collection of classes for performing
measurement of lesions. Its clinical motivation is the study of cancerous lung
lesions. However, the functionalities developed here are usable in a wider
range of applications. The Lesion Sizint Toolkit is build upon ITK and adds to
it specific classes for computing image features that are then combined into a
detection and measurement layer.")

itk_module(ITK-LesionSizingToolkit DEPENDS ITK-ImageFeature ITK-LevelSets ITK-Statistics ITK-IO-SpatialObjects ITK-RegionGrowing ITK-LabelVoting ITK-MathematicalMorphology  ITK-IO-GDCM ITK-IO-Meta ITK-VTK TEST_DEPENDS ITK-TestKernel DESCRIPTION "${DOCUMENTATION}")
