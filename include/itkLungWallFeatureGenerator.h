/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLungWallFeatureGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkLungWallFeatureGenerator_h
#define itkLungWallFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVotingBinaryHoleFillFloodingImageFilter.h"

namespace itk
{

/** \class LungWallFeatureGenerator
 * \brief Generates a feature image 
 *
 * The typical use of this class would be to generate the feature image needed
 * by a Level Set filter to internally compute its speed image. This
 * transformation is very close to a simply thresholding selection on the input
 * image, but with the advantage of a smooth transition of intensities.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT LungWallFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  /** Standard class type alias. */
  using Self = LungWallFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LungWallFeatureGenerator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image< InputPixelType, Dimension >;
  using InputImageSpatialObjectType = ImageSpatialObject< NDimension, InputPixelType >;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = typename Superclass::SpatialObjectType;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void SetInput( const SpatialObjectType * input );
  const SpatialObjectType * GetInput() const;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType * GetFeature() const;

  /** Set the Hounsfield Unit value to threshold the Lung. */
  itkSetMacro( LungThreshold, InputPixelType );
  itkGetMacro( LungThreshold, InputPixelType );

protected:
  LungWallFeatureGenerator();
  ~LungWallFeatureGenerator() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

private:
  LungWallFeatureGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  using InternalPixelType = float;
  using InternalImageType = Image< InternalPixelType, Dimension >;

  using OutputPixelType = float;
  using OutputImageType = Image< OutputPixelType, Dimension >;

  using OutputImageSpatialObjectType = ImageSpatialObject< NDimension, OutputPixelType >;

  using ThresholdFilterType = BinaryThresholdImageFilter<
    InputImageType, InternalImageType >;
  using ThresholdFilterPointer = typename ThresholdFilterType::Pointer;

  using VotingHoleFillingFilterType = VotingBinaryHoleFillFloodingImageFilter<
    InternalImageType, OutputImageType >;
  using VotingHoleFillingFilterPointer = typename VotingHoleFillingFilterType::Pointer;

  ThresholdFilterPointer                m_ThresholdFilter;
  VotingHoleFillingFilterPointer        m_VotingHoleFillingFilter;

  InputPixelType                        m_LungThreshold;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkLungWallFeatureGenerator.hxx"
#endif

#endif
