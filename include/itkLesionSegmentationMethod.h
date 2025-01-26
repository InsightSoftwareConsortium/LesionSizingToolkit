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
#ifndef itkLesionSegmentationMethod_h
#define itkLesionSegmentationMethod_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkDataObjectDecorator.h"
#include "itkSpatialObject.h"
#include "itkFeatureGenerator.h"
#include "itkSegmentationModule.h"
#include "itkProgressAccumulator.h"

namespace itk
{

/** \class LesionSegmentationMethod
 * \brief Class for coordinating components of a segmentation framework.
 *
 * This class was designed to facilitate the segmentation of lung lesions, but
 * yet to be useful for other segmentation tasks.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT LesionSegmentationMethod : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(LesionSegmentationMethod);

  /** Standard class type alias. */
  using Self = LesionSegmentationMethod;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(LesionSegmentationMethod);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject<NDimension>;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;
  using SpatialObjectConstPointer = typename SpatialObjectType::ConstPointer;

  /** SpatialObject that defines the Region of Interest in the input data */
  itkSetObjectMacro(RegionOfInterest, SpatialObjectType);
  itkGetConstObjectMacro(RegionOfInterest, SpatialObjectType);

  /** SpatialObject that defines the initial segmentation. This will be
   * used to initialize the segmentation process driven by the
   * LesionSegmentationMethod. */
  itkSetConstObjectMacro(InitialSegmentation, SpatialObjectType);
  itkGetConstObjectMacro(InitialSegmentation, SpatialObjectType);

  /** Type of the class that will generate input features in the form of
   * spatial objects. */
  using FeatureGeneratorType = FeatureGenerator<Dimension>;

  /**
   * Method for adding a feature generator that will compute the Nth feature to
   * be passed to the segmentation module.
   */
  void
  AddFeatureGenerator(FeatureGeneratorType * generator);

  /** Type of the segmentation module that encapsulate the actual segmentation
   * algorithm. */
  using SegmentationModuleType = SegmentationModule<Dimension>;
  using SegmentationModulePointer = typename SegmentationModuleType::Pointer;


  /**
   * Method for setting the class that encapsulates the actual segmentation
   * algorithm.
   */
  itkSetObjectMacro(SegmentationModule, SegmentationModuleType);


protected:
  LesionSegmentationMethod();
  ~LesionSegmentationMethod() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

private:
  SpatialObjectConstPointer m_RegionOfInterest;
  SpatialObjectConstPointer m_InitialSegmentation;

  using FeatureGeneratorPointer = typename FeatureGeneratorType::Pointer;
  using FeatureGeneratorArrayType = std::vector<FeatureGeneratorPointer>;
  using FeatureGeneratorIterator = typename FeatureGeneratorArrayType::iterator;
  using FeatureGeneratorConstIterator = typename FeatureGeneratorArrayType::const_iterator;

  FeatureGeneratorArrayType m_FeatureGenerators;

  SegmentationModulePointer m_SegmentationModule;

  ProgressAccumulator::Pointer m_ProgressAccumulator;


  /** This method calls the Update() method of each one of the feature generators */
  void
  UpdateAllFeatureGenerators();

  /** This method compares the number of available features against the number
   * of expected features, and it will throw an exception if they do not match.
   */
  void
  VerifyNumberOfAvailableFeaturesMatchedExpectations() const;

  /** Connect the outputs of feature generators as input to the segmentation module */
  void
  ConnectFeaturesToSegmentationModule();

  void
  ExecuteSegmentationModule();
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkLesionSegmentationMethod.hxx"
#endif

#endif
