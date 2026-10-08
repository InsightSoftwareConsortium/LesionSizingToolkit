include_directories("${LesionSizingToolkit_SOURCE_DIR}/src")  # to include itkIncludeRequiredIOFactories.h
include_directories("${LesionSizingToolkit_SOURCE_DIR}/include")
# ITK 6 exports one interface library per module; ITK 5.4 provides only the list variable.
if(TARGET ITK::LesionSizingToolkitModule)
  set(_lstk_libraries ITK::LesionSizingToolkitModule)
elseif(TARGET LesionSizingToolkitModule)
  set(_lstk_libraries LesionSizingToolkitModule)
else()
  set(_lstk_libraries ${LesionSizingToolkit_LIBRARIES})
endif()
add_executable(itkCannyEdgeDetectionImageFilter1  itkCannyEdgeDetectionImageFilter1.cxx )
target_link_libraries(itkCannyEdgeDetectionImageFilter1 ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkCannyEdgeDetectionImageFilter2  itkCannyEdgeDetectionImageFilter2.cxx )
target_link_libraries(itkCannyEdgeDetectionImageFilter2 ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkDicomSeriesReadImageWrite itkDicomSeriesReadImageWrite.cxx)
target_link_libraries(itkDicomSeriesReadImageWrite ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkGradientMagnitudeImageFilter itkGradientMagnitudeImageFilter.cxx)
target_link_libraries(itkGradientMagnitudeImageFilter ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkGradientMagnitudeRecursiveGaussianImageFilter itkGradientMagnitudeRecursiveGaussianImageFilter.cxx)
target_link_libraries(itkGradientMagnitudeRecursiveGaussianImageFilter ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkImageReadRegionOfInterestAroundSeedWrite itkImageReadRegionOfInterestAroundSeedWrite.cxx)
target_link_libraries(itkImageReadRegionOfInterestAroundSeedWrite ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkImageReadRegionOfInterestWrite itkImageReadRegionOfInterestWrite.cxx)
target_link_libraries(itkImageReadRegionOfInterestWrite ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkImageReadWrite itkImageReadWrite.cxx)
target_link_libraries(itkImageReadWrite ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkLaplacianRecursiveGaussianImageFilter itkLaplacianRecursiveGaussianImageFilter.cxx)
target_link_libraries(itkLaplacianRecursiveGaussianImageFilter ${_lstk_libraries} ${ITK_LIBRARIES})

add_executable(itkResampleVolumeToBeIsotropic itkResampleVolumeToBeIsotropic.cxx)
target_link_libraries(itkResampleVolumeToBeIsotropic ${_lstk_libraries} ${ITK_LIBRARIES})
