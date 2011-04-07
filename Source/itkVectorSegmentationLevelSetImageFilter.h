/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorSegmentationLevelSetImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorSegmentationLevelSetImageFilter_h
#define __itkVectorSegmentationLevelSetImageFilter_h

#include "itkVectorSparseFieldLevelSetImageFilter.h"
#include "itkVectorSegmentationLevelSetFunction.h"
#include "itkNumericTraitsVectorPixel.h"

namespace itk {

/**
 *
 * \class VectorSegmentationLevelSetImageFilter
 *
 * \brief A base class which defines the API for implementing a special class of
 * image segmentation filters using level set methods.
 *
 * \par OVERVIEW
 * This object defines the framework for a class of segmentation filters which
 * use level set methods.  These filters work by constructing a ``feature image''
 * onto which the evolving level set locks as it moves.  In the feature image,
 * values that are close to zero are associated with object boundaries.  An
 * original (or preprocessed) image is given to the filter as the feature image
 * and a seed for the level set is given as the input of the filter.  The seed is
 * converted into a level set embedding which propagates according to the features
 * calculated from the original image.
 *
 * \par TEMPLATE PARAMETERS
 * There are two required and one optional template parameter for these
 * filters.
 *
 * TInputImage is the image type of the initial model you will input to the
 * filter using SetInput() or SetInitialImage().
 *
 * TFeatureImage is the image type of the image from which the filter will
 * calculate the speed term for segmentation (see INPUTS).
 *
 * TOutputImage is the image type used for the output image phi, the implicit
 * level set image.  This should really only ever be set as an image of pixel
 * type float or double and must have the same dimension as the input image.
 *
 * \par INPUTS
 * The input to any subclass of this filter is the seed image for the initial
 * level set embedding.  As with other subclasses of the
 * VectorSparseLevelSetImageFilter, the type of the input image is is not important.
 * The (RequestedRegion) size of the seed image must, however, match the
 * (RequestedRegion) size of the feature image.
 *
 * You must identify the initial front (surface) in the input image.  You do
 * this by specifying its isovalue through the method SetIsoSurfaceValue(float
 * f).  The algorithm will then initialize its solution using the front represented by
 * value f.  Note that the front is always represented by isosurface zero in
 * the output and not the isosurface you specified for the input.  This is
 * because, for simplicity, the filter will shift your input image so that the
 * active front has zero values.
 *
 * \par
 * Depending on the particular application and filter that you are using, the
 * feature image should be preprocessed with some type of noise reduction
 * filtering.  The feature image input can be of any type, but it will be cast
 * to floating point before calculations are done.
 *
 * \par OUTPUTS
 * The output of any subclass of this filter is a level set embedding as
 * described in VectorSparseFieldLevelSetImageFilter.  The zero crossings of the output
 * image give the pixels closest to the level set boundary.  By ITK convention,
 * NEGATIVE values are pixels INSIDE the segmented region and POSITIVE values are
 * pixels OUTSIDE the segmented region.
 *
 * \par PARAMETERS
 * The MaximumRMSChange parameter is used to determine when the solution has
 * converged.  A lower value will result in a tighter-fitting solution, but
 * will require more computations.  Too low a value could put the solver into
 * an infinite loop unless a reasonable NumberOfIterations parameter is set.
 * Values should always be greater than 0.0 and less than 1.0.
 *
 * \par
 * The NumberOfIterations parameter can be used to halt the solution after a
 * specified number of iterations, overriding the MaximumRMSChange halting
 * criteria.
 *
 * \par
 * The standard convention for ITK level-set segmentation filters is that
 * POSITIVE propagation (speed) and advection terms cause the surface to EXPAND
 * while negative terms cause the surface to CONTRACT.  When the
 * ReverseExpansionDirection parameter is set to TRUE (on), it tells the
 * function object to reverse the standard ITK convention so that NEGATIVE
 * terms cause EXPANSION and positive terms cause CONTRACTION.
 *
 * This parameter can be safely changed as appropriate for a particular
 * application or data set to achieve the desired behavior.
 *
 * \par
 * The FeatureScaling parameter controls the magnitude of the features calculated
 * for use in the level set propagation and advection speeds.  This value simply
 * sets both parameters to equal values at once.  Some filters may only use on of
 * these two terms and this method is a generic way to set either or both without
 * having to know which is in use.
 *
 * \par
 * The CurvatureScaling parameter controls the magnitude of the curvature values
 * which are calculated on the evolving isophote.  This is important in
 * controlling the relative effect of curvature in the calculation.  Default
 * value is 1.0.  Higher values relative to the other level set equation terms
 * (propagation and advection) will give a smoother result.
 *
 * \par
 * The PropagationScaling parameter controls the scaling of the scalar
 * propagation (speed) term relative to other terms in the level set
 * equation. Setting this value will  override any value already set by
 * FeatureScaling.
 *
 * \par
 * The AdvectionScaling parameter controls the scaling of the vector
 * advection field term relative to other terms in the level set
 * equation. Setting this value will  override any value already set by
 * FeatureScaling.
 *
 * \warning This is an abstract class. It is not intended to be instantiated
 * by itself. Instead, you should use the derived classes. This is the reason
 * why the New() operator (itkNewMacro) is missing from the class API.
 *
 * \par
 *  See LevelSetFunction for more information.*/
template <class TInputImage,
          class TFeatureImage,
          class TOutputImage >
class ITK_EXPORT VectorSegmentationLevelSetImageFilter
  : public VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage >
{
public:
  /** Inherited typedef from the superclass. Needs to be placed befroe
      the next macro. */
  typedef VectorSegmentationLevelSetImageFilter Self;

  /** Repeat definition from Superclass to satisfy Borland compiler
      quirks */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);

  /** Output image type typedefs */
  typedef TOutputImage                              OutputImageType;
  typedef typename OutputImageType::PixelType       OutputPixelType;

  /** Standard class typedefs */
  typedef VectorSparseFieldLevelSetImageFilter<
    TInputImage, OutputImageType>                   Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** Inherited typedef from the superclass. */
  typedef typename Superclass::ValueType            ValueType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::TimeStepType         TimeStepType;
  typedef typename Superclass::InputImageType       InputImageType;

  /** Local image typedefs */
  typedef TFeatureImage FeatureImageType;

  /** The generic level set function type */
  typedef VectorSegmentationLevelSetFunction<OutputImageType, FeatureImageType>
  SegmentationFunctionType;

  /** The type used for the advection field */
  typedef typename SegmentationFunctionType::VectorImageType VectorImageType;
  typedef typename SegmentationFunctionType::ImageType       SpeedImageType;
  typedef typename SegmentationFunctionType::MatrixValueType MatrixValueType;
  typedef typename SegmentationFunctionType::VectorValueType VectorValueType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorSegmentationLevelSetImageFilter, VectorSparseFieldLevelSetImageFilter);

  /** Set/Get the maximum number of iterations allowed for the solver.  This
   *  prevents infinite loops if a solution "bounces". */
  void SetMaximumIterations (unsigned int i)
    {
    itkWarningMacro("SetMaximumIterations is deprecated.  Please use SetNumberOfIterations instead.");
    this->SetNumberOfIterations(i);
    }
  unsigned int GetMaximumIterations()
    {
    itkWarningMacro("GetMaximumIterations is deprecated. Please use GetNumberOfIterations instead.");
    return this->GetNumberOfIterations();
    }

  /** Set/Get the feature image to be used for speed function of the level set
   *  equation.  Equivalent to calling Set/GetInput(1, ..) */
  virtual void SetFeatureImage(const FeatureImageType *f)
    {
    this->ProcessObject::SetNthInput( 1, const_cast< FeatureImageType * >(f) );
    m_SegmentationFunction->SetFeatureImage(f);
    }
  virtual FeatureImageType * GetFeatureImage()
    {
    return ( static_cast< FeatureImageType *>(this->ProcessObject::GetInput(1)) );
    }

  /** Set/Get the initial level set model.  Equivalent to calling SetInput(..)
   */
  virtual void SetInitialImage(InputImageType *f)
    {
    this->SetInput(f);
    }

  /** Set the feature image */
  void SetInput2(const FeatureImageType *input)
    {
    this->SetFeatureImage( input );
    }

  /** This function is for advanced applications.  Set the image sampled as the
   * speed term of this segmentation method.  In  general, the speed image is
   * generated automatically by a subclass of this filter. */
  void SetSpeedImage( SpeedImageType *s )
    {
    m_SegmentationFunction->SetSpeedImage( s );
    }

  /** This function is for advanced applications.  Set the image sampled as the
   * advection term of this segmentation method.  In general, the advection image
   * is generated automatically by a subclass of this filter. */
  void SetAdvectionImage( unsigned int component, VectorImageType *v )
  { m_SegmentationFunction->SetAdvectionImage( v ); }

  /** Return a pointer to the image sampled as the speed term of the
   * segmentation algorithm. */
  virtual const SpeedImageType *GetSpeedImage() const
  { return m_SegmentationFunction->GetSpeedImage(); }

  /** Return a pointer to the image sampled as the advection term of the
   * segmentation algorithm. */
  virtual const VectorImageType *GetAdvectionImage(unsigned int component) const
  { return m_SegmentationFunction->GetAdvectionImage(component); }

  /** Turn On/Off the flag which determines whether Positive or Negative speed
   * terms will cause surface expansion.  If set to TRUE then negative speed
   * terms will cause the surface to expand and positive speed terms will cause
   * the surface to contract.  If set to FALSE (default) then positive speed terms will
   * cause the surface to expand and negative speed terms will cause the
   * surface to contract.  This method can be safely used to reverse the
   * expansion/contraction as appropriate to a particular application or data
   * set. */
  itkSetMacro(ReverseExpansionDirection, bool);
  itkGetMacro(ReverseExpansionDirection, bool);
  itkBooleanMacro(ReverseExpansionDirection);

  /** Turn On/Off automatic generation of Speed and Advection terms when Update
      is called.  If set to Off, the Speed and Advection images must be set
      explicitly using SetSpeedImage, SetAdvectionImage OR the methods
      GenerateSpeedImage() and GenerateAdvectionImage() should be called prior
      to updating the filter. */
  itkSetMacro(AutoGenerateSpeedAdvection, bool);
  itkGetMacro(AutoGenerateSpeedAdvection, bool);
  itkBooleanMacro(AutoGenerateSpeedAdvection);

  /** Combined scaling of the propagation and advection speed
      terms. You should use either this -or- Get/SetPropagationScaling and
      Get/SetAdvectionScaling (if appropriate).  See subclasses for details
      on when and whether to set these parameters.*/
  void SetFeatureScaling(MatrixValueType v)
    {
    if (v != m_SegmentationFunction->GetPropagationWeights())
      {
      this->SetPropagationScaling(v);
      }
    if (v != m_SegmentationFunction->GetAdvectionWeights())
      {
      this->SetAdvectionScaling(v);
      }
    }

  /** Set/Get the scaling of the propagation speed.  Setting the FeatureScaling
      parameter overrides any previous values set for PropagationScaling. */
  void SetPropagationScaling(MatrixValueType v)
    {
    if (v != m_SegmentationFunction->GetPropagationWeights())
      {
      m_SegmentationFunction->SetPropagationWeights(v);
      this->Modified();
      }
    }
  MatrixValueType GetPropagationScaling() const
    {
    return m_SegmentationFunction->GetPropagationWeights();
    }

  /** Set/Get the scaling of the advection field.  Setting the FeatureScaling
      parameter will override any existing value for AdvectionScaling. */
  void SetAdvectionScaling(MatrixValueType v)
    {
    if (v != m_SegmentationFunction->GetAdvectionWeights())
      {
      m_SegmentationFunction->SetAdvectionWeights(v);
      this->Modified();
      }
    }
  MatrixValueType GetAdvectionScaling() const
    {
    return m_SegmentationFunction->GetAdvectionWeights();
    }

  /** Set/Get the scaling of the curvature. Use this parameter to increase the
      influence of curvature on the movement of the surface.  Higher values
      relative to Advection and Propagation values will give smoother surfaces. */
  void SetCurvatureScaling(MatrixValueType v)
    {
    if (v != m_SegmentationFunction->GetCurvatureWeights())
      {
      m_SegmentationFunction->SetCurvatureWeights(v);
      this->Modified();
      }
    }
  MatrixValueType GetCurvatureScaling() const
    {
    return m_SegmentationFunction->GetCurvatureWeights();
    }


  /** */
  void SetUseMinimalCurvature( bool b )
    {
    if ( m_SegmentationFunction->GetUseMinimalCurvature() != b)
      {
      m_SegmentationFunction->SetUseMinimalCurvature( b );
      this->Modified();
      }
    }
  bool GetUseMinimalCurvature() const
    {
    return m_SegmentationFunction->GetUseMinimalCurvature();
    }
  void UseMinimalCurvatureOn()
    {
    this->SetUseMinimalCurvature(true);
    }
  void UseMinimalCurvatureOff()
    {
    this->SetUseMinimalCurvature(false);
    }


  /** Set the segmentation function.  In general, this should only be
   *  called by a subclass of this object. It is made public to allow
   *  itk::Command objects access. The method is inline to avoid a
   *  problem with the gcc 2.95 compiler matching the declaration with
   *  the definition. */
  virtual void SetSegmentationFunction( SegmentationFunctionType *s )
    {
    this->m_SegmentationFunction = s;
    this->SetDifferenceFunction(m_SegmentationFunction);
    this->Modified();
    }

  virtual SegmentationFunctionType *GetSegmentationFunction() const
    {
    return m_SegmentationFunction;
    }


  /** Set/Get the maximum constraint for the curvature term factor in the time step
   *  calculation.  Changing this value from the default is not recommended or
   *   necessary but could be used to speed up the surface evolution at the risk
   *   of creating an unstable solution.*/
  void SetMaximumCurvatureTimeStep(double n)
    {
    if ( n != m_SegmentationFunction->GetMaximumCurvatureTimeStep() )
      {
      m_SegmentationFunction->SetMaximumCurvatureTimeStep(n);
      this->Modified();
      }
    }
  double GetMaximumCurvatureTimeStep() const
    {
    return m_SegmentationFunction->GetMaximumCurvatureTimeStep();
    }

  /** Set/Get the maximum constraint for the scalar/vector term factor of the time step
   *  calculation.  Changing this value from the default is not recommended or
   *  necessary but could be used to speed up the surface evolution at the risk
   *  of creating an unstable solution.*/
  void SetMaximumPropagationTimeStep(double n)
    {
    if (n != m_SegmentationFunction->GetMaximumPropagationTimeStep() )
      {
      m_SegmentationFunction->SetMaximumPropagationTimeStep(n);
      this->Modified();
      }
    }
  double GetMaximumPropagationTimeStep() const
    {
    return m_SegmentationFunction->GetMaximumPropagationTimeStep();
    }

  /** Allocate and calculate the speed term image in the SegmentationFunction
      object.  This method is called automatically on filter execution
      unless AutoGenerateSpeedAdvection is set to Off.*/
  void GenerateSpeedImage();

  /** Allocate and calculate the advection term image in the
      SegmentationFunction object  This method is called automatically on
      filter execution unless AutoGenerateSpeedAdvection is set to Off.*/
  void GenerateAdvectionImage();

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  /** End concept checking */
#endif

protected:
  virtual ~VectorSegmentationLevelSetImageFilter() {}
  VectorSegmentationLevelSetImageFilter();

  virtual void PrintSelf(std::ostream& os, Indent indent) const;

  /** Overrides parent implementation */
  virtual void InitializeIteration()
    {
    Superclass::InitializeIteration();
    // Estimate the progress of the filter
    this->SetProgress( (float) ((float)this->GetElapsedIterations()
                                / (float)this->GetNumberOfIterations()) );
    }

  /** Overridden from ProcessObject to set certain values before starting the
   * finite difference solver and then create an appropriate output */
  void GenerateData();

  /** Flag which sets the inward/outward direction of propagation speed. See
      SetReverseExpansionDirection for more information. */
  bool m_ReverseExpansionDirection;

  /** Flag to indicate whether Speed and Advection images are automatically
   *  generated when running the filter.  Otherwise, a pointer to images must
   *  be explicitly set or GenerateSpeedImage() and/or GenerateAdvectionImage()
   *  called directly before updating the filter */
  bool m_AutoGenerateSpeedAdvection;

private:
  VectorSegmentationLevelSetImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Make the SetDifferenceFunction() private to enforce the use of the
   * derived API SetVectorSegmentationLevelSetFunction(). */
  typedef typename Superclass::FiniteDifferenceFunctionType  FiniteDifferenceFunctionType;
  void SetDifferenceFunction( FiniteDifferenceFunctionType * f )
    {
    this->Superclass::SetDifferenceFunction( f );
    }

  SegmentationFunctionType *m_SegmentationFunction;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorSegmentationLevelSetImageFilter.txx"
#endif

#endif
