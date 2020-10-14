/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkGeodesicActiveContourLevelSetSegmentationModule_h
#define itkGeodesicActiveContourLevelSetSegmentationModule_h

#include "itkSinglePhaseLevelSetSegmentationModule.h"
#include "itkGeodesicActiveContourLevelSetImageFilter.h"

namespace itk
{

/** \class GeodesicActiveContourLevelSetSegmentationModule
 * \brief This class applies the GeodesicActiveContourLevelSet segmentation
 * method.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT GeodesicActiveContourLevelSetSegmentationModule
  : public SinglePhaseLevelSetSegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(GeodesicActiveContourLevelSetSegmentationModule);

  /** Standard class type alias. */
  using Self = GeodesicActiveContourLevelSetSegmentationModule;
  using Superclass = SinglePhaseLevelSetSegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(GeodesicActiveContourLevelSetSegmentationModule, SinglePhaseLevelSetSegmentationModule);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = typename Superclass::SpatialObjectType;
  using SpatialObjectPointer = typename Superclass::SpatialObjectPointer;

  /** Types of images and spatial objects inherited from the superclass. */
  using OutputPixelType = typename Superclass::OutputPixelType;
  using InputImageType = typename Superclass::InputImageType;
  using FeatureImageType = typename Superclass::FeatureImageType;
  using OutputImageType = typename Superclass::OutputImageType;
  using InputSpatialObjectType = typename Superclass::InputSpatialObjectType;
  using FeatureSpatialObjectType = typename Superclass::FeatureSpatialObjectType;
  using OutputSpatialObjectType = typename Superclass::OutputSpatialObjectType;


protected:
  GeodesicActiveContourLevelSetSegmentationModule();
  ~GeodesicActiveContourLevelSetSegmentationModule() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkGeodesicActiveContourLevelSetSegmentationModule.hxx"
#endif

#endif
