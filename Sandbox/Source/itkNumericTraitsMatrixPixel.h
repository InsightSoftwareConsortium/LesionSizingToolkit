/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkNumericTraitsMatrixPixel.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNumericTraitsMatrixPixel_h
#define __itkNumericTraitsMatrixPixel_h

#include "itkNumericTraits.h"
#include "itkMatrix.h"

//
// This file defines numeric traits for Matrix< T > as pixel type.
//

namespace itk
{

//
// First we define a macro that can be customized to be used for a sequence of
// specializations or for a generic template instantiation. This Macro covers
// the implementation for good compilers and for Visual Studio 6.0.
//
#define itkNumericTraitsMatrixPixelMacro(T,M,N) \
template < _TEMPLATE_ARGUMENT_ >  \
class NumericTraits<Matrix< T, M, N > >  \
{ \
public: \
  typedef T ValueType; \
 \
  typedef _TYPENAME_ NumericTraits<T>::AbsType        ElementAbsType; \
  typedef _TYPENAME_ NumericTraits<T>::AccumulateType ElementAccumulateType; \
  typedef _TYPENAME_ NumericTraits<T>::FloatType      ElementFloatType; \
  typedef _TYPENAME_ NumericTraits<T>::PrintType      ElementPrintType; \
  typedef _TYPENAME_ NumericTraits<T>::RealType       ElementRealType; \
 \
  typedef Matrix<T>                   Self; \
 \
  typedef Matrix<ElementAbsType,M,N>          AbsType; \
  typedef Matrix<ElementAccumulateType,M,N>   AccumulateType; \
  typedef Matrix<ElementFloatType,M,N>        FloatType; \
  typedef Matrix<ElementPrintType,M,N>        PrintType; \
  typedef Matrix<ElementRealType,M,N>         RealType; \
 \
  typedef ElementRealType ScalarRealType; \
 \
  static const RealType max( const Self & a ) \
    {  \
      RealType b(a.Size());  \
      b.Fill( NumericTraits< T >::max() ); \
      return b; \
    } \
  static const RealType min( const Self & a ) \
    {  \
      RealType b(a.Size());  \
      b.Fill( NumericTraits< T >::min() ); \
      return b; \
    } \
  static const Self Zero( const Self  & a ) \
  {  \
    Self b(a.Size());  \
    b.Fill( NumericTraits< T >::Zero ); \
    return b; \
  } \
  static const Self One( const Self & a ) \
  {  \
    Self b(a.Size());  \
    b.Fill( NumericTraits< T >::One ); \
    return b; \
  } \
};


//
// Visual Studio 6.0 is not capable of managing the template implementation
// defined at the end of this file. Therefore we provide an alternative
// primitive implementation based on macros that define explicit
// instantiations.
//
#if defined( _MSC_VER ) && ( _MSC_VER < 1310 )

// These two symbols below are defined empty on purpose
#define _TYPENAME_
#define _TEMPLATE_ARGUMENT_

//
// List here the specializations of the Traits:
//
itkNumericTraitsMatrixPixelMacro( char );
itkNumericTraitsMatrixPixelMacro( unsigned char );
itkNumericTraitsMatrixPixelMacro( short );
itkNumericTraitsMatrixPixelMacro( unsigned short );
itkNumericTraitsMatrixPixelMacro( int );
itkNumericTraitsMatrixPixelMacro( unsigned int );
itkNumericTraitsMatrixPixelMacro( long );
itkNumericTraitsMatrixPixelMacro( unsigned long );
itkNumericTraitsMatrixPixelMacro( float );
itkNumericTraitsMatrixPixelMacro( double );

#else

// For all the other good compilers, we provide here a generic implementation
// based on creating types of Matrices whose components are the types of the
// NumericTraits from the original Matrices components. This implementation
// doesn't require specializations, since it is based on the concept that
//
//    NumericTraits< Matrix< T, M, N > >  is defined piecewise by
//    Matrix< NumericTraits< T >, M, N >
//
//
// By defining the following symbols, the Macro above gets customized to become
// a generic template implementation of the traits
//
#define _TYPENAME_            typename
#define _TEMPLATE_ARGUMENT_   class T, unsigned int M, unsigned int N

//
// Then we simply call the macro once with the generic template argument T.
//
itkNumericTraitsMatrixPixelMacro( T, M, N );

#endif


//
// Finally, to avoid contamination of other files with the symbols defined
// here, we undefine the helper macros
//
#undef _TYPENAME_
#undef _TEMPLATE_ARGUMENT_


} // end namespace itk

#endif // __itkNumericTraitsMatrix_h
