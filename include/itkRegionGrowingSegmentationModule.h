/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkRegionGrowingSegmentationModule_h
#define itkRegionGrowingSegmentationModule_h

#include "itkSegmentationModule.h"
#include "itkImageSpatialObject.h"
#include "itkLandmarkSpatialObject.h"

namespace itk
{

/** \class RegionGrowingSegmentationModule
 * \brief Class applies a region growing segmentation method
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT RegionGrowingSegmentationModule : public SegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(RegionGrowingSegmentationModule);

  /** Standard class type alias. */
  using Self = RegionGrowingSegmentationModule;
  using Superclass = SegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(RegionGrowingSegmentationModule);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = typename Superclass::SpatialObjectType;
  using SpatialObjectPointer = typename Superclass::SpatialObjectPointer;

  /** Types of the input feature image and the output image */
  using OutputPixelType = float;
  using FeaturePixelType = float;
  using FeatureImageType = Image<FeaturePixelType, NDimension>;
  using OutputImageType = Image<OutputPixelType, NDimension>;

  /** Types of the Spatial objects used for the input feature image and the output image. */
  using FeatureSpatialObjectType = ImageSpatialObject<NDimension, FeaturePixelType>;
  using OutputSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  /** Type of the input set of seed points. They are stored in a Landmark Spatial Object. */
  using InputSpatialObjectType = LandmarkSpatialObject<NDimension>;

protected:
  RegionGrowingSegmentationModule();
  ~RegionGrowingSegmentationModule() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

  /** Set the output image as cargo of the output SpatialObject. */
  void
  PackOutputImageInOutputSpatialObject(OutputImageType * outputImage);

  /** Extract the input set of landmark points to be used as seeds. */
  const InputSpatialObjectType *
  GetInternalInputLandmarks() const;

  /** Extract the input feature image from the input feature spatial object. */
  const FeatureImageType *
  GetInternalFeatureImage() const;

private:
  void
  ConvertIntensitiesToCenteredRange(OutputImageType * outputImage);
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkRegionGrowingSegmentationModule.hxx"
#endif

#endif
