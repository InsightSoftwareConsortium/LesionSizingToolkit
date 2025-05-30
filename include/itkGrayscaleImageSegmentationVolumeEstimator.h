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
#ifndef itkGrayscaleImageSegmentationVolumeEstimator_h
#define itkGrayscaleImageSegmentationVolumeEstimator_h

#include "itkSegmentationVolumeEstimator.h"

namespace itk
{

/** \class GrayscaleImageSegmentationVolumeEstimator
 * \brief Class for estimating the volume of a segmentation stored in a
 * SpatialObject that carries a gray-scale image of pixel type float. This
 * implementation assumes that the input image is roughly composed of two
 * regions with intensity plateaus, and with a narrow transition region between
 * the two regions.  Note that this doesn't mean that the regions must be a
 * single connected component.
 *
 * The estimation of the volume is done by the equivalent of rescaling the
 * intensity range to [0:1] and then adding the contributions of all the
 * pixels.
 *
 * The pixels size is, of course, taken into account.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT GrayscaleImageSegmentationVolumeEstimator : public SegmentationVolumeEstimator<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(GrayscaleImageSegmentationVolumeEstimator);

  /** Standard class type alias. */
  using Self = GrayscaleImageSegmentationVolumeEstimator;
  using Superclass = SegmentationVolumeEstimator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;
  using RealObjectType = typename Superclass::RealObjectType;

  /** Method for constructing new instances of this class. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(GrayscaleImageSegmentationVolumeEstimator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = typename Superclass::SpatialObjectType;
  using SpatialObjectPointer = typename Superclass::SpatialObjectPointer;
  using SpatialObjectConstPointer = typename Superclass::SpatialObjectConstPointer;

  /** Required type of the input */
  using InputPixelType = float;
  using InputImageSpatialObjectType = ImageSpatialObject<NDimension, InputPixelType>;
  using InputImageType = Image<InputPixelType, NDimension>;

protected:
  GrayscaleImageSegmentationVolumeEstimator();
  ~GrayscaleImageSegmentationVolumeEstimator() override;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkGrayscaleImageSegmentationVolumeEstimator.hxx"
#endif

#endif
