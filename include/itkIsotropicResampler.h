/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkIsotropicResampler.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkIsotropicResampler_h
#define itkIsotropicResampler_h

#include "itkImage.h"
#include "itkImageSpatialObject.h"

namespace itk
{

/** \class IsotropicResampler
 * \brief Resamples the image to an isotropic resolution.
 *
 * This class resamples an image using BSplineInterpolator and produces
 * an isotropic image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT IsotropicResampler : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(IsotropicResampler);

  /** Standard class type alias. */
  using Self = IsotropicResampler;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IsotropicResampler, ProcessObject);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input to this
   * feature generator. */
  using InputPixelType = signed short;
  using InputImageType = Image<InputPixelType, Dimension>;
  using InputImageSpatialObjectType = ImageSpatialObject<NDimension, InputPixelType>;
  using InputImageSpatialObjectPointer = typename InputImageSpatialObjectType::Pointer;
  using SpatialObjectType = SpatialObject<NDimension>;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void
  SetInput(const SpatialObjectType * input);
  const SpatialObjectType *
  GetInput() const;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType *
  GetOutput() const;

  itkSetMacro(OutputSpacing, double);
  itkGetMacro(OutputSpacing, double);

protected:
  IsotropicResampler();
  ~IsotropicResampler() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void
  GenerateData() override;

private:
  using OutputPixelType = signed short;
  using OutputImageType = Image<OutputPixelType, Dimension>;

  using OutputImageSpatialObjectType = ImageSpatialObject<NDimension, OutputPixelType>;

  double m_OutputSpacing;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkIsotropicResampler.hxx"
#endif

#endif
