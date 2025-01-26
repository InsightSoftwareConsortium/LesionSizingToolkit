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
#ifndef itkMorphologicalOpenningFeatureGenerator_h
#define itkMorphologicalOpenningFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVotingBinaryHoleFillFloodingImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryMorphologicalOpeningImageFilter.h"
#include "itkCastImageFilter.h"

namespace itk
{

/** \class MorphologicalOpenningFeatureGenerator
 * \brief Generates a feature image based on intensity and removes small pieces from it.
 *
 * This feature generator thresholds the input image, runs an Openning
 * Mathematical Morphology Filter and then a Voting Hole Filling filter.
 * The net effect is the elimination of small islands and small holes
 * from the thresholded image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT MorphologicalOpenningFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  /** Standard class typedefs. */
  typedef MorphologicalOpenningFeatureGenerator Self;
  typedef FeatureGenerator<NDimension>          Superclass;
  typedef SmartPointer<Self>                    Pointer;
  typedef SmartPointer<const Self>              ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(MorphologicalOpenningFeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  typedef signed short                                   InputPixelType;
  typedef Image<InputPixelType, Dimension>               InputImageType;
  typedef ImageSpatialObject<NDimension, InputPixelType> InputImageSpatialObjectType;
  typedef typename InputImageSpatialObjectType::Pointer  InputImageSpatialObjectPointer;
  typedef typename Superclass::SpatialObjectType         SpatialObjectType;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void
  SetInput(const SpatialObjectType * input);
  const SpatialObjectType *
  GetInput() const;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType *
  GetFeature() const;

  /** Set the Hounsfield Unit value to threshold the Lung. */
  itkSetMacro(LungThreshold, InputPixelType);
  itkGetMacro(LungThreshold, InputPixelType);

protected:
  MorphologicalOpenningFeatureGenerator();
  virtual ~MorphologicalOpenningFeatureGenerator();
  void
  PrintSelf(std::ostream & os, Indent indent) const;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData();

private:
  MorphologicalOpenningFeatureGenerator(const Self &); // purposely not implemented
  void
  operator=(const Self &); // purposely not implemented

  typedef unsigned char                       InternalPixelType;
  typedef Image<InternalPixelType, Dimension> InternalImageType;

  typedef float                             OutputPixelType;
  typedef Image<OutputPixelType, Dimension> OutputImageType;

  typedef ImageSpatialObject<NDimension, OutputPixelType> OutputImageSpatialObjectType;

  typedef BinaryThresholdImageFilter<InputImageType, InternalImageType> ThresholdFilterType;
  typedef typename ThresholdFilterType::Pointer                         ThresholdFilterPointer;

  typedef BinaryBallStructuringElement<InternalPixelType, Dimension>                              KernelType;
  typedef BinaryMorphologicalOpeningImageFilter<InternalImageType, InternalImageType, KernelType> OpenningFilterType;
  typedef typename OpenningFilterType::Pointer                                                    OpenningFilterPointer;

  typedef VotingBinaryHoleFillFloodingImageFilter<InternalImageType, InternalImageType> VotingHoleFillingFilterType;
  typedef typename VotingHoleFillingFilterType::Pointer                                 VotingHoleFillingFilterPointer;

  typedef CastImageFilter<InternalImageType, OutputImageType> CastingFilterType;
  typedef typename CastingFilterType::Pointer                 CastingFilterPointer;


  ThresholdFilterPointer         m_ThresholdFilter;
  OpenningFilterPointer          m_OpenningFilter;
  VotingHoleFillingFilterPointer m_VotingHoleFillingFilter;
  CastingFilterPointer           m_CastingFilter;

  InputPixelType m_LungThreshold;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkMorphologicalOpenningFeatureGenerator.hxx"
#endif

#endif
