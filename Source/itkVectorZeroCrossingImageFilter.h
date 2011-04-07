/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorZeroCrossingImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorZeroCrossingImageFilter_h
#define __itkVectorZeroCrossingImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
namespace itk
{
/** \class VectorZeroCrossingImageFilter
 *
 *  This filter finds the closest pixel to the zero-crossings (sign changes) in
 *  a signed itk::Image.  Pixels closest to zero-crossings are labeled with
 *  a foreground value.  All other pixels are marked with a background value.
 *  The algorithm works by detecting differences in sign among neighbors using
 *  city-block style connectivity (4-neighbors in 2d, 6-neighbors in 3d, etc.).
 *
 *  \par Inputs and Outputs
 *  The input to this filter is an itk::Image of arbitrary dimension.  The
 *  algorithm assumes a signed data type (zero-crossings are not defined for
 *  unsigned data types), and requires that operator>, operator<, operator==,
 *  and operator!= are defined.
 *
 *  \par
 *  The output of the filter is a binary, labeled image of user-specified type.
 *  By default, zero-crossing pixels are labeled with a default ``foreground''
 *  value of itk::NumericTraits<OutputDataType>::One, where OutputDataType is
 *  the data type of the output image.  All other pixels are labeled with a
 *  default ``background'' value of itk::NumericTraits<OutputDataType>::Zero.
 *
 *  \par Parameters
 *  There are two parameters for this filter.  ForegroundValue is the value
 *  that marks zero-crossing pixels.  The BackgroundValue is the value given to
 *  all other pixels.
 *
 *  \sa Image
 *  \sa Neighborhood
 *  \sa NeighborhoodOperator
 *  \sa NeighborhoodIterator
 *  \ingroup ImageFeatureExtraction */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT VectorZeroCrossingImageFilter
  : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard "Self" & Superclass typedef. */
  typedef VectorZeroCrossingImageFilter                     Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>     Superclass;

  /** Image typedef support   */
  typedef TInputImage                     InputImageType;
  typedef TOutputImage                    OutputImageType;

  /** SmartPointer typedef support  */
  typedef SmartPointer<Self>              Pointer;
  typedef SmartPointer<const Self>        ConstPointer;

  /** Define pixel types  */
  typedef typename TInputImage::PixelType   InputImagePixelType;
  typedef typename TOutputImage::PixelType  OutputImagePixelType;

  /** Pixel components types */
  typedef typename NumericTraits< InputImagePixelType >::ValueType   InputImagePixelComponentType;
  typedef typename NumericTraits< OutputImagePixelType >::ValueType  OutputImagePixelComponentType;

  /** Method for creation through the object factory.  */
  itkNewMacro(Self);

  /** Typedef to describe the output image region type. */
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorZeroCrossingImageFilter, ImageToImageFilter);

  /** ImageDimension enumeration   */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension );
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension );

  /** VectorZeroCrossingImageFilter needs a larger input requested
   * region than the output requested region (larger by the kernel
   * size to do comparisons between the central pixel and ite neighbors).
   * Thus VectorZeroCrossingImageFilter needs to provide an implementation
   * for GenerateInputRequestedRegion() in order to inform the
   * pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion()   */
  virtual void GenerateInputRequestedRegion() throw(InvalidRequestedRegionError);

  /** Set/Get the label value for zero-crossing pixels. */
  itkSetMacro(ForegroundValue, OutputImagePixelComponentType);
  itkGetMacro(ForegroundValue, OutputImagePixelComponentType);

  /** Set/Get the label value for non-zero-crossing pixels. */
  itkSetMacro(BackgroundValue, OutputImagePixelComponentType);
  itkGetMacro(BackgroundValue, OutputImagePixelComponentType);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  /** End concept checking */
#endif

protected:
  VectorZeroCrossingImageFilter()
    {
    m_ForegroundValue = NumericTraits<OutputImagePixelComponentType>::One;
    m_BackgroundValue = NumericTraits<OutputImagePixelComponentType>::Zero;
    }
  ~VectorZeroCrossingImageFilter(){}
  void PrintSelf(std::ostream& os, Indent indent) const;

  VectorZeroCrossingImageFilter(const Self&) {}
  OutputImagePixelComponentType m_BackgroundValue;
  OutputImagePixelComponentType m_ForegroundValue;

  /**
   * VectorZeroCrossingImageFilter can be implemented as a multithreaded filter.
   * Therefore,this implementation provides a ThreadedGenerateData() routine which
   * is called for each processing thread. The output image data is allocated
   * automatically by the superclass prior to calling ThreadedGenerateData().
   * ThreadedGenerateData can only write to the portion of the output image
   * specified by the parameter "outputRegionForThread"
   *
   * \sa ImageToImageFilter::ThreadedGenerateData(),
   *     ImageToImageFilter::GenerateData()
   */
  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                            int threadId );
};

} //end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorZeroCrossingImageFilter.txx"
#endif

#endif
