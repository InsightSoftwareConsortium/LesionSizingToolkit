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
#ifndef itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule_h
#define itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule_h

#include "itkSinglePhaseLevelSetSegmentationModule.h"
#include "itkFastMarchingSegmentationModule.h"
#include "itkGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkLandmarkSpatialObject.h"

namespace itk
{

/** \class FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule
 * \brief This class applies the GeodesicActiveContourLevelSet segmentation
 * method.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule
  : public SinglePhaseLevelSetSegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule);

  /** Standard class type alias. */
  using Self = FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule;
  using Superclass = SinglePhaseLevelSetSegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule);

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
  using FeatureSpatialObjectType = typename Superclass::FeatureSpatialObjectType;
  using OutputSpatialObjectType = typename Superclass::OutputSpatialObjectType;

  /** Type of the input set of seed points. They are stored in a Landmark Spatial Object. */
  using InputSpatialObjectType = LandmarkSpatialObject<NDimension>;

  /** Set the Fast Marching algorithm Stopping Value. The Fast Marching
   * algorithm is terminated when the value of the smallest trial point
   * is greater than the stopping value. */
  virtual void
  SetStoppingValue(double d)
  {
    m_FastMarchingModule->SetStoppingValue(d);
  }
  virtual double
  GetStoppingValue() const
  {
    return m_FastMarchingModule->GetStoppingValue();
  }

  /** Set the Fast Marching algorithm distance from seeds. */
  virtual void
  SetDistanceFromSeeds(double d)
  {
    m_FastMarchingModule->SetDistanceFromSeeds(d);
  }
  virtual double
  GetDistanceFromSeeds() const
  {
    return m_FastMarchingModule->GetDistanceFromSeeds();
  }

protected:
  FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule();
  ~FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

  using FastMarchingModuleType = FastMarchingSegmentationModule<Dimension>;
  typename FastMarchingModuleType::Pointer m_FastMarchingModule;
  using GeodesicActiveContourLevelSetModuleType = GeodesicActiveContourLevelSetSegmentationModule<Dimension>;
  typename GeodesicActiveContourLevelSetModuleType::Pointer m_GeodesicActiveContourLevelSetModule;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule.hxx"
#endif

#endif
