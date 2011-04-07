/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSymmetricEigenVectorsImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSymmetricEigenVectorsImageFilter_h
#define __itkSymmetricEigenVectorsImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkSymmetricEigenAnalysis.h"


namespace itk
{

// This functor class invokes the computation of Eigen Vectors for
// every pixel. The input pixel type must provide the API for the [][]
// operator, while the output pixel type must provide the API for the
// [] operator. Input pixel matrices should be symmetric.
// 
// The default operation is to order eigen values in ascending order.
// You may also use OrderEigenValuesBy( ) to order eigen values by
// magnitude as is common with use of tensors in vessel extraction.
namespace Functor {  
 
template< typename TInput, typename TOutputEigenValues, typename TOutputEigenVectors >
class SymmetricEigenValuesAndVectorsFunction
{
public:
  typedef typename TInput::RealValueType    RealValueType;
  typedef TInput                            MatrixType;

  SymmetricEigenValuesAndVectorsFunction() {}
  ~SymmetricEigenValuesAndVectorsFunction() {}

  typedef SymmetricEigenAnalysis< TInput, TOutputEigenValues, TOutputEigenVectors > CalculatorType;

  bool operator!=( const SymmetricEigenValuesAndVectorsFunction & ) const
    {
    return false;
    }
  bool operator==( const SymmetricEigenValuesAndVectorsFunction & other ) const
    {
    return !(*this != other);
    }

  inline TOutputEigenVectors operator()( const TInput & x )
    {
    TOutputEigenValues  eigenValues;
    TOutputEigenVectors eigenVectors;
    m_Calculator.ComputeEigenValuesAndVectors( x, eigenValues, eigenVectors );
    return eigenVectors;
    }

  /** Method to explicitly set the dimension of the matrix */
  void SetDimension( unsigned int n )
    {
    m_Calculator.SetDimension(n);
    }

  /** Typdedefs to order eigen values. 
   * OrderByValue:      lambda_1 < lambda_2 < ....
   * OrderByMagnitude:  |lambda_1| < |lambda_2| < .....
   * DoNotOrder:        Default order of eigen values obtained after QL method
   */
  typedef enum {
    OrderByValue=1,
    OrderByMagnitude,
    DoNotOrder
  } EigenValueOrderType;
 
  /** Order eigen values. Default is to OrderByValue:  lambda_1 <
   * lambda_2 < .... */
  void OrderEigenValuesBy( EigenValueOrderType order )
    {
    if( order == OrderByMagnitude )
      {
      m_Calculator.SetOrderEigenMagnitudes( true );
      }
    else if( order == DoNotOrder )
      {
      m_Calculator.SetOrderEigenValues( false );
      }
    }

private:
  CalculatorType m_Calculator;
}; 

}  // end namespace functor


/** \class SymmetricEigenValuesAndVectorsImageFilter
 * \brief Computes the Eigen-Values and Eigen-Vectors of an input image of
 * symmetric matrices.
 *
 * SymmetricEigenValuesAndVectorsImageFilter applies pixel-wise the invocation
 * for computing the eigen-values and eigen-vectors of the input matrix pixel.
 *
 * The OrderEigenValuesBy( .. ) method can be used to order eigen values 
 * in ascending order by value or magnitude or no ordering.
 * OrderByValue:      lambda_1 < lambda_2 < ....
 * OrderByMagnitude:  |lambda_1| < |lambda_2| < .....
 * DoNotOrder:        Default order of eigen values obtained after QL method
 *
 * The user of this class is explicitly supposed to set the dimension of the 
 * 2D matrix using the SetDimension() method.
 *
 * \ingroup IntensityImageFilters  Multithreaded  TensorObjects
 *
 */
template <typename  TInputImage, typename TEigenValue, typename  TOutputImage=TInputImage>
class ITK_EXPORT SymmetricEigenValuesAndVectorsImageFilter :
    public
UnaryFunctorImageFilter<TInputImage,TOutputImage, 
                        Functor::SymmetricEigenValuesAndVectorsFunction< 
                                        typename TInputImage::PixelType,
                                        TEigenValue,
                                        typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef SymmetricEigenValuesAndVectorsImageFilter   Self;
  typedef UnaryFunctorImageFilter<
    TInputImage,TOutputImage, 
    Functor::SymmetricEigenValuesAndVectorsFunction< 
      typename TInputImage::PixelType,
      TEigenValue,
      typename TOutputImage::PixelType > >            Superclass;
  typedef SmartPointer<Self>                          Pointer;
  typedef SmartPointer<const Self>                    ConstPointer;

  typedef typename Superclass::OutputImageType        OutputImageType;
  typedef typename TOutputImage::PixelType            OutputPixelType;
  typedef typename TInputImage::PixelType             InputPixelType;
  typedef typename InputPixelType::ValueType          InputValueType;
  typedef typename Superclass::FunctorType            FunctorType; 

  /** Typdedefs to order eigen values. 
   * OrderByValue:      lambda_1 < lambda_2 < ....
   * OrderByMagnitude:  |lambda_1| < |lambda_2| < .....
   * DoNotOrder:        Default order of eigen values obtained after QL method
   */
  typedef typename FunctorType::EigenValueOrderType         EigenValueOrderType;
 
  /** Order eigen values. Default is to OrderByValue:  lambda_1 <
   * lambda_2 < .... */
  void OrderEigenValuesBy( EigenValueOrderType order )
    {
    this->GetFunctor().OrderEigenValuesBy( order );
    }

  /** Run-time type information (and related methods).   */
  itkTypeMacro( SymmetricEigenValuesAndVectorsImageFilter, UnaryFunctorImageFilter );

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Print internal ivars */
  void PrintSelf(std::ostream& os, Indent indent) const
    { this->Superclass::PrintSelf( os, indent ); }
  
  /** Set the dimension of the tensor. (For example the SymmetricSecondRankTensor
   * is a pxp matrix) */
  void SetDimension( unsigned int p )
    {
    this->GetFunctor().SetDimension(p);
    }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck,
                  (Concept::HasNumericTraits<InputValueType>));
  /** End concept checking */
#endif

protected:
  SymmetricEigenValuesAndVectorsImageFilter() {};
  virtual ~SymmetricEigenValuesAndVectorsImageFilter() {};

private:
  SymmetricEigenValuesAndVectorsImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


  
} // end namespace itk
  
#endif
