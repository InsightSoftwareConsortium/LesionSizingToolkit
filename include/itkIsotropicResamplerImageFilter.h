/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkIsotropicResamplerImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkIsotropicResamplerImageFilter_h
#define itkIsotropicResamplerImageFilter_h

#include "itkResampleImageFilter.h"
#include "itkImage.h"
#include "itkBSplineInterpolateImageFunction.h"

namespace itk
{

/** \class IsotropicResamplerImageFilter
 *
 * \brief Resamples the image to an isotropic resolution.
 *
 * This class resamples an image using BSplineInterpolator and produces
 * an isotropic image.
 *
 *\ingroup LesionSizingToolkit
 * \ingroup LesionSizingToolkit
 */
template<class TInputImage, class TOutputImage>
class IsotropicResamplerImageFilter
  : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(IsotropicResamplerImageFilter);

  /** Standard "Self" & Superclass type alias.  */
  using Self = IsotropicResamplerImageFilter;
  using Superclass = ImageToImageFilter<TInputImage, TOutputImage>;

  /** Image type alias support   */
  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;
  using OutputImagePointer = typename OutputImageType::Pointer;

  /** SmartPointer type alias support  */
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Define pixel types. */
  using OutputImagePixelType = typename TOutputImage::PixelType;
  using SizeType = typename InputImageType::SizeType;
  using SizeValueType = typename SizeType::SizeValueType;
  using SpacingType = typename InputImageType::SpacingType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IsotropicResamplerImageFilter, ImageToImageFilter);

  /** ImageDimension constant    */
  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;

  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  itkSetMacro( OutputSpacing, SpacingType );
  itkGetMacro( OutputSpacing, SpacingType );

  itkSetMacro( DefaultPixelValue, OutputImagePixelType );
  itkGetMacro( DefaultPixelValue, OutputImagePixelType );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  /** End concept checking */
#endif

  /** Override the superclass implementation so as to set the flag on all the
   * filters within our lesion segmentation pipeline */
  void SetAbortGenerateData( const bool ) override;

  /** ResampleImageFilter produces an image which is a different size
   * than its input.  As such, it needs to provide an implementation
   * for GenerateOutputInformation() in order to inform the pipeline
   * execution model.  The original documentation of this method is
   * below. \sa ProcessObject::GenerateOutputInformaton() */
  void GenerateOutputInformation( void ) override;

protected:
  IsotropicResamplerImageFilter();
  void PrintSelf(std::ostream& os, Indent indent) const override;

  void GenerateData() override;

private:
  ~IsotropicResamplerImageFilter() override;

  SpacingType m_OutputSpacing;
  using ResampleFilterType = ResampleImageFilter< TInputImage, TOutputImage >;
  using ResampleFilterPointer = typename ResampleFilterType::Pointer;

  ResampleFilterPointer     m_ResampleFilter;
  OutputImagePixelType      m_DefaultPixelValue;
};

} //end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkIsotropicResamplerImageFilter.hxx"
#endif

#endif
