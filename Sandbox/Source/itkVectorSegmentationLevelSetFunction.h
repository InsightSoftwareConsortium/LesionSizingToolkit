/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorSegmentationLevelSetFunction.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorSegmentationLevelSetFunction_h_
#define __itkVectorSegmentationLevelSetFunction_h_

#include "itkLevelSetFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkMatrixLinearInterpolateImageFunction.h"
#include "itkMatrixCastImageFilter.h"
#include "itkMatrix.h"

namespace itk {

/** \class VectorSegmentationLevelSetFunction

  \par
  This object defines the API for a class of function objects which perform
  level set based segmentations.  The VectorSegmentationLevelSetImageFilter objects
  use these VectorSegmentationLevelSetFunction objects to perform the numerical
  calculations which move a level set front to lock onto image features.

  \par
  In order to create a working function object, you must subclass the
  CalculateSpeedImage method to produce a "feature image" that is used by the
  parent LevelSetFunction class as the PropagationSpeed for its calculations.

  \sa VectorSegmentationLevelSetImageFilter
  \sa LevelSetFunction
*/
  
template <class TImageType, class TFeatureImageType = TImageType>
class ITK_EXPORT VectorSegmentationLevelSetFunction
  : public LevelSetFunction<TImageType>
{
public:
  /** Standard class typedefs. */
  typedef VectorSegmentationLevelSetFunction Self;
  typedef LevelSetFunction<TImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro( VectorSegmentationLevelSetFunction, LevelSetFunction );

  /** Extract some parameters from the superclass. */
  typedef typename Superclass::ImageType ImageType;
  typedef typename Superclass::RadiusType RadiusType;
  typedef typename Superclass::PixelRealType PixelRealType;
  typedef TFeatureImageType FeatureImageType;
  typedef typename Superclass::FloatOffsetType FloatOffsetType;
  typedef typename Superclass::ScalarValueType ScalarValueType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename FeatureImageType::PixelType FeatureScalarType;
  typedef typename ImageType::IndexType IndexType;
  typedef typename Superclass::VectorType VectorType;
  typedef typename Superclass::GlobalDataStruct GlobalDataStruct;

  /** Extract some parameters from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Define an image type for the advection field. */
  typedef Matrix< double, ImageDimension, ImageDimension >             MatrixType;
  typedef Image< MatrixType, itkGetStaticConstMacro(ImageDimension)>   MatrixImageType;


  /** Define a scalar interpolator */
  typedef LinearInterpolateImageFunction<ImageType>  InterpolatorType;

  /** Define a vector interpolator */
  typedef MatrixLinearInterpolateImageFunction<MatrixImageType> MatrixInterpolatorType;
  
  /** Continuous index type recognized by the interpolator */
  typedef typename InterpolatorType::ContinuousIndexType ContinuousIndexType;
  
  /** Set/Get the image which will be used to calculate the speed function. */
  virtual const FeatureImageType *GetFeatureImage() const
  { return m_FeatureImage.GetPointer(); }
  virtual void SetFeatureImage(const FeatureImageType *f)
  {    m_FeatureImage = f;  }
  
  /** Get/Set the image used as the speed function in the level set equation */
  virtual ImageType *GetSpeedImage() 
  { return m_SpeedImage.GetPointer(); }
  void SetSpeedImage( ImageType *s );
  
  /** Get/Set the image used as the advection field in the level set equation */
  virtual MatrixImageType * GetAdvectionImage() const
  { return m_AdvectionImage.GetPointer(); } 
  void SetAdvectionImage( MatrixImageType *s );
  
  
  /** This method creates the appropriate member variable operators for the
   * level-set calculations.  The argument to this function is a the radius
   * necessary for performing the level-set calculations. */
  virtual void Initialize(const RadiusType &r);

  /** This method must be defined in a subclass to implement a working function
   * object.  This method is called before the solver begins its work to
   * produce the speed image used as the level set function's Propagation speed
   * term.  See LevelSetFunction for more information. */
  virtual void CalculateSpeedImage() {}

  /** This method must be defined in a subclass to implement a working function
   * object.  This method is called before the solver begins its work to
   * produce the speed image used as the level set function's Advection field
   * term.  See LevelSetFunction for more information. */
  virtual void CalculateAdvectionImage() {}

  /** Allocates the image that will be used for the level set function's
   * Propagation Speed term.  See LevelSetFunction for more information. */
  virtual void AllocateSpeedImage();

  /** Allocates the image that will be used for the level set function's
   * Advection field term.  See LevelSetFunction for more information. */
  virtual void AllocateAdvectionImage();

  /** Determines whether Positive or Negative speed terms will cause surface
   * expansion.  This method flips the sign of all of the speed, advection, etc
   * terms.  By convention, filters should be written so that POSITIVE speed
   * terms cause surface expansion.  Calling this method will
   * toggle between the standard POSITIVE EXPANSION convention and the
   * nonstandard NEGATIVE EXPANSION convention.
   *
   * IMPORTANT:  When adding terms to the level-set equation through
   * subclassing you may need to override this function so that your new terms
   * will be properly adjusted. */
  virtual void ReverseExpansionDirection();
  
protected:
  /** The image whose features will be used to create a speed image */
  typename FeatureImageType::ConstPointer m_FeatureImage;

  /** The image holding the speed values for front propagation */
  typename ImageType::Pointer        m_SpeedImage;

  /** The image holding the advection field for front propation */
  typename MatrixImageType::Pointer  m_AdvectionImage;

  /** A casting functor to convert between vector types.  */
  Functor::MatrixCast< ITK_TYPENAME MatrixInterpolatorType::OutputType,
                       MatrixType > m_MatrixCast;

  /** Returns the propagation speed from the precalculated speed image.*/
  virtual ScalarValueType PropagationSpeed(const NeighborhoodType &,
                                           const FloatOffsetType &, GlobalDataStruct *gd) const;

  /** Advection field.  Returns a vector from the computed advectionfield.*/
  virtual MatrixType AdvectionField(const NeighborhoodType &,
                                    const FloatOffsetType &, GlobalDataStruct *gd) const;
  
  
  virtual ~VectorSegmentationLevelSetFunction() {}
  VectorSegmentationLevelSetFunction()
  {
    m_SpeedImage = ImageType::New();
    m_AdvectionImage = MatrixImageType::New();
    m_Interpolator = InterpolatorType::New();
    m_MatrixInterpolator = MatrixInterpolatorType::New();
  }

  typename InterpolatorType::Pointer m_Interpolator;
  typename MatrixInterpolatorType::Pointer m_MatrixInterpolator;

 
private:
  VectorSegmentationLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace

// Define instantiation macro for this template.
#define ITK_TEMPLATE_VectorSegmentationLevelSetFunction(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT VectorSegmentationLevelSetFunction< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef VectorSegmentationLevelSetFunction< ITK_TEMPLATE_2 x > \
                                         VectorSegmentationLevelSetFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkVectorSegmentationLevelSetFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkVectorSegmentationLevelSetFunction.txx"
#endif


#endif
