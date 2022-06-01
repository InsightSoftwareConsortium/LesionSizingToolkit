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
#ifndef itkSegmentationModule_h
#define itkSegmentationModule_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkDataObjectDecorator.h"
#include "itkSpatialObject.h"

namespace itk
{

/** \class SegmentationModule
 * \brief Class provides the abstract interface of the segmentation methods.
 *
 * The typical use of this class would be to generate a binary mask spatial
 * object representing a segmented object.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT SegmentationModule : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(SegmentationModule);

  /** Standard class type alias. */
  using Self = SegmentationModule;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SegmentationModule, ProcessObject);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject<NDimension>;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void
  SetInput(const SpatialObjectType * input);
  const SpatialObjectType *
  GetInput() const;

  /** Input data that carries the feature in the form of a
   * SpatialObject. */
  void
  SetFeature(const SpatialObjectType * feature);
  const SpatialObjectType *
  GetFeature() const;

  /** Output segmentation represented as a SpatialObject. */
  const SpatialObjectType *
  GetOutput() const;

  /** Returns the number of features that are expected by this Segmentation
   * Module. This method will be overloaded in derived classes. */
  unsigned int
  GetExpectedNumberOfFeatures() const;

protected:
  SegmentationModule();
  ~SegmentationModule() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /* Derived classes MUST implement the "void  GenerateData ()" method */

  /** Output segmentation represented as a SpatialObject. Non-const version
   * only for internal use. */
  SpatialObjectType *
  GetInternalOutput();
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkSegmentationModule.hxx"
#endif

#endif
