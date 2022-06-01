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
#ifndef itkSegmentationVolumeEstimator_h
#define itkSegmentationVolumeEstimator_h

#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkSimpleDataObjectDecorator.h"

namespace itk
{

/** \class SegmentationVolumeEstimator
 * \brief Class for estimating the volume of a segmentation stored in a SpatialObject
 *
 * This class is the base class for specific implementation of volume estimators.
 *
 * SpatialObjects are used as input of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT SegmentationVolumeEstimator : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(SegmentationVolumeEstimator);

  /** Standard class type alias. */
  using Self = SegmentationVolumeEstimator;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** This is an abstract class, therefore it doesn't need the itkNewMacro() */

  /** Run-time type information (and related methods). */
  itkTypeMacro(SegmentationVolumeEstimator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject<NDimension>;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;
  using SpatialObjectConstPointer = typename SpatialObjectType::ConstPointer;

  /** Set the input SpatialObject representing the segmentation whose volume
   * will be estimated */
  using ProcessObject::SetInput;
  void
  SetInput(const SpatialObjectType * inputSpatialObject);

  /** Type of DataObjects used for scalar outputs */
  using RealType = double;
  using RealObjectType = SimpleDataObjectDecorator<RealType>;

  /** Return the computed Volume. The volume units will be relative to the
   * spacing units used by the input spatial object. For example, if the input
   * spatial object is using millimeters as the units of spacing then the units
   * of the volume computed in this class will be cubic millimeters. */
  RealType
  GetVolume() const;
  const RealObjectType *
  GetVolumeOutput() const;


protected:
  SegmentationVolumeEstimator();
  ~SegmentationVolumeEstimator() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkSegmentationVolumeEstimator.hxx"
#endif

#endif
