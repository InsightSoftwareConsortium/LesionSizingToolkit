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
#ifndef itkFastMarchingSegmentationModule_h
#define itkFastMarchingSegmentationModule_h

#include "itkSinglePhaseLevelSetSegmentationModule.h"
#include "itkImageSpatialObject.h"
#include "itkLandmarkSpatialObject.h"

namespace itk
{

/** \class FastMarchingSegmentationModule
 * \brief Class applies a fast marching segmentation method
 *
 * Takes as input a landmark spatial object and a feature image and produces as
 * output a segmentation of the output level set. Threshold this at 0 and you
 * will get the zero set.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT FastMarchingSegmentationModule : public SinglePhaseLevelSetSegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(FastMarchingSegmentationModule);

  /** Standard class type alias. */
  using Self = FastMarchingSegmentationModule;
  using Superclass = SinglePhaseLevelSetSegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FastMarchingSegmentationModule, SinglePhaseLevelSetSegmentationModule);

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

  /** Set the Fast Marching algorithm Stopping Value. The Fast Marching
   * algorithm is terminated when the value of the smallest trial point
   * is greater than the stopping value. */
  itkSetMacro(StoppingValue, double);
  itkGetMacro(StoppingValue, double);

  /** Set the Fast Marching algorithm distance from seeds. */
  itkSetMacro(DistanceFromSeeds, double);
  itkGetMacro(DistanceFromSeeds, double);

protected:
  FastMarchingSegmentationModule();
  ~FastMarchingSegmentationModule() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

  /** Extract the input set of landmark points to be used as seeds. */
  const InputSpatialObjectType *
  GetInternalInputLandmarks() const;

  double m_StoppingValue;
  double m_DistanceFromSeeds;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkFastMarchingSegmentationModule.hxx"
#endif

#endif
