/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMatrixCastImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatrixCastImageFilter_h
#define __itkMatrixCastImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkNumericTraitsFixedArrayPixel.h"

namespace itk
{

/** \class MatrixCastImageFilter
 *
 * \brief Casts input vector pixels to output vector pixel type.
 *
 * This filter is templated over the input image type and
 * output image type.
 *
 * The filter expect both images to have the same number of dimensions,
 * and that both the input and output have itk::Vector pixel types
 * of the same VectorDimension.
 *
 * \sa Vector
 *
 * \ingroup IntensityImageFilters  Multithreaded
 */
namespace Functor {

template< class TInput, class TOutput>
class MatrixCast
{
public:
  MatrixCast() {}
  ~MatrixCast() {}
  bool operator!=( const MatrixCast & ) const
    {
    return false;
    }
  bool operator==( const MatrixCast & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator()( const TInput & A ) const
    {
    typedef typename TOutput::ValueType OutputValueType;

    TOutput value;
    for( unsigned int k = 0; k < TOutput::Dimension; k++ )
      { value[k] = static_cast<OutputValueType>( A[k] ); }
    return value;
    }
};
}

template <class TInputImage, class TOutputImage>
class ITK_EXPORT MatrixCastImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage,
                        Functor::MatrixCast< typename TInputImage::PixelType,
                                             typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef MatrixCastImageFilter               Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage,
    Functor::MatrixCast<
      typename TInputImage::PixelType,
      typename TOutputImage::PixelType> >     Superclass;
  typedef SmartPointer<Self>                  Pointer;
  typedef SmartPointer<const Self>            ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(MatrixCastImageFilter,
               UnaryFunctorImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TInputImage::PixelType::ValueType>));
  itkConceptMacro(OutputHasNumericTraitsCheck,
    (Concept::HasNumericTraits<typename TOutputImage::PixelType::ValueType>));
  itkConceptMacro(InputConvertibleToOutputCheck,
    (Concept::Convertible<typename TInputImage::PixelType::ValueType,
                          typename TOutputImage::PixelType::ValueType>));
  /** End concept checking */
#endif

protected:
  MatrixCastImageFilter() {}
  virtual ~MatrixCastImageFilter() {}

private:
  MatrixCastImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk


#endif
