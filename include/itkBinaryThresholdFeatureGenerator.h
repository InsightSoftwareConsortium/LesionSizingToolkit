/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkBinaryThresholdFeatureGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkBinaryThresholdFeatureGenerator_h
#define itkBinaryThresholdFeatureGenerator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"

namespace itk
{

/** \class BinaryThresholdFeatureGenerator
 * \brief Generates a feature image by thresholding the input intensities.
 *
 * The typical use of this class would be to generate the feature image needed
 * by a Level Set filter to internally compute its speed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class BinaryThresholdFeatureGenerator : public FeatureGenerator<NDimension>
{
public:
  /** Standard class type alias. */
  using Self = BinaryThresholdFeatureGenerator;
  using Superclass = FeatureGenerator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BinaryThresholdFeatureGenerator, FeatureGenerator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image< InputPixelType, Dimension >;
  using InputImageSpatialObjectType = ImageSpatialObject< NDimension, InputPixelType >;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = typename Superclass::SpatialObjectType;

  /** Value to be used to threshold the image. */
  itkSetMacro( Threshold, double );
  itkGetMacro( Threshold, double );

protected:
  BinaryThresholdFeatureGenerator();
  ~BinaryThresholdFeatureGenerator() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

private:
  BinaryThresholdFeatureGenerator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  using OutputPixelType = float;
  using OutputImageType = Image< OutputPixelType, Dimension >;

  using OutputImageSpatialObjectType = ImageSpatialObject< NDimension, OutputPixelType >;

  using BinaryThresholdFilterType = BinaryThresholdImageFilter<
    InputImageType, OutputImageType >;
  using BinaryThresholdFilterPointer = typename BinaryThresholdFilterType::Pointer;

  BinaryThresholdFilterPointer    m_BinaryThresholdFilter;

  double                          m_Threshold;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkBinaryThresholdFeatureGenerator.hxx"
#endif

#endif
