/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorFiniteDifferenceFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorFiniteDifferenceFunction_h_
#define __itkVectorFiniteDifferenceFunction_h_

#include "itkFiniteDifferenceFunction.h"
#include "vnl/vnl_matrix_fixed.h"

namespace itk {

/** \class VectorFiniteDifferenceFunction
  * \brief The VectorFiniteDifferenceFunction class is a generic function
  * object intended to test whether the FiniteDifferenceSolver can be used with
  * images of multiple components.
 */
template <class TImageType>
class ITK_EXPORT VectorFiniteDifferenceFunction
  : public FiniteDifferenceFunction<TImageType>
{
public:
  /** Standard class typedefs. */
  typedef VectorFiniteDifferenceFunction            Self;
  typedef FiniteDifferenceFunction<TImageType>      Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( VectorFiniteDifferenceFunction, FiniteDifferenceFunction );

  /** Extract some parameters from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  /** Convenient typedefs. */
  typedef double TimeStepType;
  typedef typename Superclass::ImageType  ImageType;
  typedef typename Superclass::PixelType  PixelType;
  typedef typename NumericTraits< PixelType >::ScalarRealType   ScalarValueType;
  typedef typename Superclass::PixelRealType  PixelRealType;
  typedef typename Superclass::RadiusType RadiusType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename Superclass::NeighborhoodScalesType NeighborhoodScalesType;
  typedef typename Superclass::FloatOffsetType FloatOffsetType;

  /** The vector type that will be used in the calculations. */
  //  typedef
  typedef FixedArray<ScalarValueType, itkGetStaticConstMacro(ImageDimension)> VectorType;

  /** A global data type for this class of equations.  Used to store
   * values that are needed in calculating the time step and other intermediate
   * products such as derivatives that may be used by virtual functions called
   * from ComputeUpdate.  Caching these values here allows the ComputeUpdate
   * function to be const and thread safe.*/
  struct GlobalDataStruct
  {

  };

  /** Compute the equation value. */
  virtual ScalarValueType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void *globalData,
                                  unsigned int component,
                                  const FloatOffsetType& = FloatOffsetType(0.0) ) const;

  virtual TimeStepType ComputeGlobalTimeStep(void *GlobalData) const;

  virtual void *GetGlobalDataPointer() const
    {
    GlobalDataStruct *ans = new GlobalDataStruct();
    return ans; 
    }

  virtual void ReleaseGlobalDataPointer(void *GlobalData) const
    { delete (GlobalDataStruct *) GlobalData; }

protected:
  VectorFiniteDifferenceFunction()
  {
  }
  virtual ~VectorFiniteDifferenceFunction() {}
  void PrintSelf(std::ostream &s, Indent indent) const;
  

private:
  VectorFiniteDifferenceFunction(const Self&); //purposely not implemented
  void operator=(const Self&);   //purposely not implemented

  /** Declare this function just to satisfy the pure virtual declaration in the
   * superclass. This function however, is not intended to be used at all.
   * Instead the version that returns a scalar must be used. */
  virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void *globalData,
                                  const FloatOffsetType& = FloatOffsetType(0.0) ) 
  {
    return itk::NumericTraits< PixelType >::ZeroValue();
  }


};

} // namespace itk

#if ITK_TEMPLATE_TXX
# include "itkVectorFiniteDifferenceFunction.txx"
#endif

#endif

