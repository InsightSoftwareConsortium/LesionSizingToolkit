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
#ifndef __itkVectorFiniteDifferenceFunction_h
#define __itkVectorFiniteDifferenceFunction_h

#include "itkFiniteDifferenceFunction.h"
#include "vnl/vnl_matrix_fixed.h"

namespace itk {

/** \class VectorFiniteDifferenceFunction
  * \brief The VectorFiniteDifferenceFunction class is a generic function
  * object intended to test whether the FiniteDifferenceSolver can be used with
  * images of multiple components.
  *
  * The distinction between components and phases is important. The function
  * can take a speed image of multiple components. The function can also
  * update N level sets, where N is the number of phases.
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

  /** Run-time type information (and related methods) */
  itkTypeMacro( VectorFiniteDifferenceFunction, FiniteDifferenceFunction );

  /** Extract some parameters from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);

  /** Convenient typedefs. */
  typedef double                                                TimeStepType;
  typedef typename Superclass::ImageType                        ImageType;
  typedef typename Superclass::PixelType                        PixelType;
  typedef typename NumericTraits< PixelType >::ScalarRealType   ScalarValueType;
  typedef typename Superclass::PixelRealType                    PixelRealType;
  typedef typename Superclass::RadiusType                       RadiusType;
  typedef typename Superclass::NeighborhoodType                 NeighborhoodType;
  typedef typename Superclass::NeighborhoodScalesType           NeighborhoodScalesType;
  typedef typename Superclass::FloatOffsetType                  FloatOffsetType;

  /** The vector type that will be used in the calculations. */
  //  typedef
  typedef FixedArray<ScalarValueType, itkGetStaticConstMacro(ImageDimension)> VectorType;

  /** Compute the equation value. */
  virtual ScalarValueType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void *globalData,
                                  unsigned int component,
                                  const FloatOffsetType& = FloatOffsetType(0.0) ) const;

  virtual TimeStepType ComputeGlobalTimeStep(void *GlobalData) const;

  /** Be sure to set the number of components before setting the number of the
   * levelset before calling GetGlobalDataPointer(), since it makes allocations
   * one time based on it. */
  virtual void *GetGlobalDataPointer() const = 0;
  virtual void ReleaseGlobalDataPointer(void *GlobalData) const =0;

  /** Set the number of components (number of speed images) */
  virtual void SetNumberOfComponents( unsigned int n ) { m_NumberOfComponents = n; }
  unsigned int GetNumberOfComponents() const { return m_NumberOfComponents; }

  /** Set the number of phases (number of level sets) */
  virtual void SetNumberOfPhases( unsigned int n ) { m_NumberOfPhases = n; }
  unsigned int GetNumberOfPhases() const { return m_NumberOfPhases; }

protected:
  VectorFiniteDifferenceFunction();
  virtual ~VectorFiniteDifferenceFunction() {}
  void PrintSelf(std::ostream &s, Indent indent) const;

  unsigned int m_NumberOfComponents;
  unsigned int m_NumberOfPhases;

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
