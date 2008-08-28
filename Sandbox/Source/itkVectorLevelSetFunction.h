/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorLevelSetFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorLevelSetFunction_h_
#define __itkVectorLevelSetFunction_h_

#include "itkFiniteDifferenceFunction.h"
#include "itkMatrix.h"

namespace itk {

/** \class VectorLevelSetFunction
  * \brief The VectorLevelSetFunction class is a generic function object which can be
 * used to create a level set method filter when combined with an appropriate
 * finite difference image filter.  (See FiniteDifferenceImageFilter.)
 *
 * VectorLevelSetFunction implements a generic level set function.  This function is
 * an expanded form of the basic equation developed in [1].
 *
 * \f$\phi_{t} + \alpha
 * \stackrel{\rightharpoonup}{A}(\mathbf{x})\cdot\nabla\phi + \beta
 * P(\mathbf{x})\mid\nabla\phi\mid = \gamma Z(\mathbf{x})\kappa\mid\nabla\phi\mid\f$
 *
 * where \f$ \stackrel{\rightharpoonup}{A} \f$ is an advection term, \f$ P \f$
 * is a propagation (growth) term, and \f$ Z \f$ is a spatial modifier term for
 * the mean curvature \f$ \kappa \f$.  \f$ \alpha \f$, \f$ \beta \f$, and \f$
 * \gamma \f$ are all scalar constants.
 *
 * Terms in the equation above are supplied through virtual methods, which must
 * be subclassed to complete an implementation.  Terms can be eliminated from
 * the equation by setting the corresponding constants to zero. A wide variety
 * of level set methods can be implemented by subclassing this basic equation.
 *
 * In ITK, the usual sign convention is that the INSIDE of a surface contains
 * NEGATIVE values and the OUTSIDE of the surface contains POSITIVE values.
 *
 * \warning You MUST call Initialize() in the constructor of subclasses of this
 * object to set it up properly to do level-set Calculations.  The argument
 * that you pass Initialize is the radius of the neighborhood needed to perform
 * the calculations.  If your subclass does not do any additional neighborhood
 * processing, then the default radius should be 1 in each direction.
 *
 * \par REFERENCES
 * \par
 * [1] Sethian, J.A. Level Set Methods. Cambridge University Press. 1996.
 *
 * \ingroup FiniteDifferenceFunctions
 * \ingroup Functions
 */
template <class TImageType>
class ITK_EXPORT VectorLevelSetFunction
  : public FiniteDifferenceFunction<TImageType>
{
public:
  /** Standard class typedefs. */
  typedef VectorLevelSetFunction Self;
  typedef FiniteDifferenceFunction<TImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( VectorLevelSetFunction, FiniteDifferenceFunction );

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

  /** Define an image type for the advection field. */
  typedef Matrix< double, ImageDimension, ImageDimension >             MatrixType;
  typedef Image< MatrixType, itkGetStaticConstMacro(ImageDimension)>   MatrixImageType;

  /** A global data type for this class of equations.  Used to store
   * values that are needed in calculating the time step and other intermediate
   * products such as derivatives that may be used by virtual functions called
   * from ComputeUpdate.  Caching these values here allows the ComputeUpdate
   * function to be const and thread safe.*/
  struct GlobalDataStruct
  {
    ScalarValueType m_MaxAdvectionChange;
    ScalarValueType m_MaxPropagationChange;
    ScalarValueType m_MaxCurvatureChange;

    /** Hessian matrix */
    vnl_matrix_fixed<ScalarValueType,
                     itkGetStaticConstMacro(ImageDimension),
                     itkGetStaticConstMacro(ImageDimension)> m_dxy;
    
    /** Array of first derivatives*/
    ScalarValueType m_dx[itkGetStaticConstMacro(ImageDimension)];
    
    ScalarValueType m_dx_forward[itkGetStaticConstMacro(ImageDimension)];
    ScalarValueType m_dx_backward[itkGetStaticConstMacro(ImageDimension)];
    
    ScalarValueType m_GradMagSqr;
  };

  /** Advection field.  Default implementation returns a vector of zeros. */
  virtual MatrixType AdvectionField(const NeighborhoodType &,
                                    const FloatOffsetType &, GlobalDataStruct * = 0)  const
    { return m_ZeroVectorConstant; }

  /** Propagation speed.  This term controls surface expansion/contraction.
   *  Default implementation returns zero. */ 
  virtual ScalarValueType PropagationSpeed(
    const NeighborhoodType& ,
    const FloatOffsetType &, GlobalDataStruct * = 0 ) const
    { return NumericTraits<ScalarValueType>::Zero; }

  /** Curvature speed.  Can be used to spatially modify the effects of
      curvature . The default implementation returns one. */
  virtual ScalarValueType CurvatureSpeed(const NeighborhoodType &,
                                         const FloatOffsetType &, GlobalDataStruct * = 0
                                         ) const
    { return NumericTraits<ScalarValueType>::One; }

    /** Laplacian smoothing speed.  Can be used to spatially modify the 
      effects of laplacian smoothing of the level set function */
  virtual ScalarValueType LaplacianSmoothingSpeed(
    const NeighborhoodType &,
    const FloatOffsetType &, GlobalDataStruct * = 0) const
    { return NumericTraits<ScalarValueType>::One; }

  /** Alpha.  Scales all advection term values.*/ 
  virtual void SetAdvectionWeight(const ScalarValueType a)
    { m_AdvectionWeight = a; }
  ScalarValueType GetAdvectionWeight() const
    { return m_AdvectionWeight; }
  
  /** Beta.  Scales all propagation term values. */
  virtual void SetPropagationWeight(const ScalarValueType p)
    { m_PropagationWeight = p; }
  ScalarValueType GetPropagationWeight() const
    { return m_PropagationWeight; }
  
  /** Gamma. Scales all curvature weight values */
  virtual void SetCurvatureWeight(const ScalarValueType c)
    { m_CurvatureWeight = c; }
  ScalarValueType GetCurvatureWeight() const
    { return m_CurvatureWeight; }
  
  /** Weight of the laplacian smoothing term */
  void SetLaplacianSmoothingWeight(const ScalarValueType c)
    { m_LaplacianSmoothingWeight = c; }
  ScalarValueType GetLaplacianSmoothingWeight() const
    { return m_LaplacianSmoothingWeight; }
  
  /** Epsilon. */
  void SetEpsilonMagnitude(const ScalarValueType e)
    { m_EpsilonMagnitude = e; }
  ScalarValueType GetEpsilonMagnitude() const
    { return m_EpsilonMagnitude; }

  /** Compute the equation value. */
  virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void *globalData,
                                  const FloatOffsetType& = FloatOffsetType(0.0));

 /** Computes the time step for an update given a global data structure.
   * The data used in the computation may take different forms depending on
   * the nature of the equations.  This global data cannot be kept in the
   * instance of the equation object itself since the equation object must
   * remain stateless for thread safety.  The global data is therefore managed
   * for each thread by the finite difference solver filters. */
  virtual TimeStepType ComputeGlobalTimeStep(void *GlobalData) const;

  /** Returns a pointer to a global data structure that is passed to this
   * object from the solver at each calculation.  The idea is that the solver
   * holds the state of any global values needed to calculate the time step,
   * while the equation object performs the actual calculations.  The global
   * data should also be initialized in this method.  Global data can be used
   * for caching any values used or reused by the FunctionObject.  Each thread
   * should receive its own global data struct. */
  virtual void *GetGlobalDataPointer() const
    {
      GlobalDataStruct *ans = new GlobalDataStruct();
      ans->m_MaxAdvectionChange   = NumericTraits<ScalarValueType>::Zero;
      ans->m_MaxPropagationChange = NumericTraits<ScalarValueType>::Zero;
      ans->m_MaxCurvatureChange   = NumericTraits<ScalarValueType>::Zero;
      return ans; 
    }

  /** This method creates the appropriate member variable operators for the
   * level-set calculations.  The argument to this function is a the radius
   * necessary for performing the level-set calculations. */
  virtual void Initialize(const RadiusType &r);
  
  /** When the finite difference solver filter has finished using a global
   * data pointer, it passes it to this method, which frees the memory.
   * The solver cannot free the memory because it does not know the type
   * to which the pointer points. */
  virtual void ReleaseGlobalDataPointer(void *GlobalData) const
    { delete (GlobalDataStruct *) GlobalData; }

  /**  */
  virtual ScalarValueType ComputeCurvatureTerm(const NeighborhoodType &,
                                               const FloatOffsetType &,
                                               GlobalDataStruct *gd = 0
                                               );
  virtual ScalarValueType ComputeMeanCurvature(const NeighborhoodType &,
                                               const FloatOffsetType &,
                                               GlobalDataStruct *gd = 0
                                               );

  virtual ScalarValueType ComputeMinimalCurvature(const NeighborhoodType &,
                                                  const FloatOffsetType &,
                                                  GlobalDataStruct *gd = 0
                                                  );
  
  virtual ScalarValueType Compute3DMinimalCurvature(const NeighborhoodType &,
                                                    const FloatOffsetType &,
                                                    GlobalDataStruct *gd = 0
                                                    );
  
  /** */
  void SetUseMinimalCurvature( bool b )
  {
    m_UseMinimalCurvature = b;
  }
  bool GetUseMinimalCurvature() const
  {
    return m_UseMinimalCurvature;
  }
  void UseMinimalCurvatureOn()
  {
    this->SetUseMinimalCurvature(true);
  }
  void UseMinimalCurvatureOff()
  {
    this->SetUseMinimalCurvature(false);
  }

  /** Set/Get the maximum constraint for the curvature term factor in the time step
      calculation.  Changing this value from the default is not recommended or
      necessary, but can be used to speed up the surface evolution at the risk
      of creating an unstable solution.*/
  static void SetMaximumCurvatureTimeStep(double n)
  {
    m_DT = n;
  }
  static double GetMaximumCurvatureTimeStep()
  {
    return m_DT;
  }

  /** Set/Get the maximum constraint for the scalar/vector term factor of the time step
      calculation.  Changing this value from the default is not recommended or
      necessary, but can be used to speed up the surface evolution at the risk
      of creating an unstable solution.*/
  static void SetMaximumPropagationTimeStep(double n)
  {
    m_WaveDT = n;
  }
  static double GetMaximumPropagationTimeStep()
  {
    return m_WaveDT;
  }

protected:
  VectorLevelSetFunction()
  {
    m_EpsilonMagnitude = 1.0e-5;
    m_AdvectionWeight = m_PropagationWeight 
      = m_CurvatureWeight = m_LaplacianSmoothingWeight 
      = NumericTraits<ScalarValueType>::Zero;
    m_UseMinimalCurvature = false;
  }
  virtual ~VectorLevelSetFunction() {}
  void PrintSelf(std::ostream &s, Indent indent) const;
  
  /** Constants used in the time step calculation. */
  static double m_WaveDT;
  static double m_DT;

  /** Slices for the ND neighborhood. */
  std::slice x_slice[itkGetStaticConstMacro(ImageDimension)];

  /** The offset of the center pixel in the neighborhood. */
  ::size_t m_Center;

  /** Stride length along the y-dimension. */
  ::size_t m_xStride[itkGetStaticConstMacro(ImageDimension)];

  bool m_UseMinimalCurvature;

  /** This method's only purpose is to initialize the zero vector
   * constant. */
  static VectorType InitializeZeroVectorConstant();
  
  /** Zero vector constant. */
  static VectorType m_ZeroVectorConstant;

  /** Epsilon magnitude controls the lower limit for gradient magnitude. */
  ScalarValueType m_EpsilonMagnitude;
  
  /** Alpha. */
  ScalarValueType m_AdvectionWeight;

  /** Beta. */
  ScalarValueType m_PropagationWeight;

  /** Gamma. */
  ScalarValueType m_CurvatureWeight;

  /** Laplacean smoothing term */
  ScalarValueType m_LaplacianSmoothingWeight;

private:
  VectorLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&);   //purposely not implemented
};

} // namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_VectorLevelSetFunction(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT VectorLevelSetFunction< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef VectorLevelSetFunction< ITK_TEMPLATE_1 x > \
                                     VectorLevelSetFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkVectorLevelSetFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkVectorLevelSetFunction.txx"
#endif

#endif

