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
#ifndef __itkIsotropicResamplerImageFilter_h
#define __itkIsotropicResamplerImageFilter_h

#include "itkResampleImageFilter.h"
#include "itkImage.h"

namespace itk
{

/** \class IsotropicResamplerImageFilter
 *
 * \brief Resamples the image to an isotropic resolution.
 *
 * This class resamples an image using BSplineInterpolator and produces
 * an isotropic image.
 *
 */
template<class TInputImage, class TOutputImage>
class IsotropicResamplerImageFilter
  : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard "Self" & Superclass typedef.  */
  typedef IsotropicResamplerImageFilter                 Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;

  /** Image typedef support   */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;
      
  /** SmartPointer typedef support  */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Define pixel types. */
  typedef typename TOutputImage::PixelType  OutputImagePixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(IsotropicResamplerImageFilter, ImageToImageFilter);

  /** ImageDimension constant    */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  itkSetMacro( OutputSpacing, double );
  itkGetMacro( OutputSpacing, double );

  itkSetMacro( DefaultPixelValue, OutputImagePixelType );
  itkGetMacro( DefaultPixelValue, OutputImagePixelType );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  /** End concept checking */
#endif

  /** Override the superclass implementation so as to set the flag on all the
   * filters within our lesion segmentation pipeline */
  virtual void SetAbortGenerateData( bool );

protected:
  IsotropicResamplerImageFilter();
  void PrintSelf(std::ostream& os, Indent indent) const;

  void GenerateData();

private:
  virtual ~IsotropicResamplerImageFilter();
  IsotropicResamplerImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double    m_OutputSpacing;
  typedef ResampleImageFilter< TInputImage, TOutputImage > ResampleFilterType;
  typedef typename ResampleFilterType::Pointer             ResampleFilterPointer;

  ResampleFilterPointer     m_ResampleFilter;
  PixelType                 m_DefaultPixelValue;
};

} //end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkIsotropicResamplerImageFilter.txx"
#endif
  
#endif

