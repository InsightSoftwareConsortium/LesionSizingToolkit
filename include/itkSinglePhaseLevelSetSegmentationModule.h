/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkSinglePhaseLevelSetSegmentationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkSinglePhaseLevelSetSegmentationModule_h
#define itkSinglePhaseLevelSetSegmentationModule_h

#include "itkSegmentationModule.h"
#include "itkImageSpatialObject.h"

namespace itk
{

/** \class SinglePhaseLevelSetSegmentationModule
 * \brief Class applies a single-phase level set segmentation method
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT SinglePhaseLevelSetSegmentationModule : public SegmentationModule<NDimension>
{
public:
  /** Standard class type alias. */
  using Self = SinglePhaseLevelSetSegmentationModule;
  using Superclass = SegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SinglePhaseLevelSetSegmentationModule, SegmentationModule);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = typename Superclass::SpatialObjectType;
  using SpatialObjectPointer = typename Superclass::SpatialObjectPointer;

  /** Types of the input, feature and output images. */
  using InputPixelType = float;
  using FeaturePixelType = float;
  using OutputPixelType = float;
  using InputImageType = Image< InputPixelType, NDimension >;
  using FeatureImageType = Image< FeaturePixelType, NDimension >;
  using OutputImageType = Image< OutputPixelType, NDimension >;

  /** Types of the Spatial objects used for input, feature and output images. */
  using InputSpatialObjectType = ImageSpatialObject< NDimension, InputPixelType >;
  using FeatureSpatialObjectType = ImageSpatialObject< NDimension, FeaturePixelType >;
  using OutputSpatialObjectType = ImageSpatialObject< NDimension, OutputPixelType >;

  /** Weight that controls the propagating behavior of the level set. */
  itkSetMacro( PropagationScaling, double );
  itkGetMacro( PropagationScaling, double );

  /** Weight that controls the behavior of curvature restriction in the
   * level set. */
  itkSetMacro( CurvatureScaling, double );
  itkGetMacro( CurvatureScaling, double );

  /** Weight that controls the behavior of the level set. */
  itkSetMacro( AdvectionScaling, double );
  itkGetMacro( AdvectionScaling, double );

  /** Value of RMS change under which the level set propagation will
   * stop. */
  itkSetMacro( MaximumRMSError, double );
  itkGetMacro( MaximumRMSError, double );

  /** Maximum number of iterations that the level set solve will run. */
  itkSetMacro( MaximumNumberOfIterations, unsigned int );
  itkGetMacro( MaximumNumberOfIterations, unsigned int );

  /** Invert the output image. This is a convenience method intended to make
   * uniform the convention that segmentations are encoded with positive values
   * in the pixels inside of the segmented object and negative values in the
   * pixels outside of the segmented object. This is opposed to the general
   * convention of ITK level sets, where the values inside the object are
   * negative, and for this reason they must be inverted here. By default the
   * intensities must be inverted, and therefore, by default this variable will
   * be set to true. However, when combining multiple level sets in a sequence,
   * this variable should be set to false. */
  itkSetMacro( InvertOutputIntensities, bool );
  itkGetMacro( InvertOutputIntensities, bool );
  itkBooleanMacro( InvertOutputIntensities );
  
protected:
  SinglePhaseLevelSetSegmentationModule();
  ~SinglePhaseLevelSetSegmentationModule() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

  /** Set the output image as cargo of the output SpatialObject. */
  void PackOutputImageInOutputSpatialObject( OutputImageType * outputImage );

  /** Extract the input image from the input spatial object. */
  const InputImageType * GetInternalInputImage() const;

  /** Extract the input feature image from the input feature spatial object. */
  const FeatureImageType * GetInternalFeatureImage() const;

private:
  SinglePhaseLevelSetSegmentationModule(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double        m_PropagationScaling;
  double        m_CurvatureScaling;
  double        m_AdvectionScaling;

  unsigned int  m_MaximumNumberOfIterations;
  double        m_MaximumRMSError;

  bool          m_InvertOutputIntensities;

  using ImageConstPointer = typename InputImageType::ConstPointer;
  mutable ImageConstPointer m_ZeroSetInputImage;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkSinglePhaseLevelSetSegmentationModule.hxx"
#endif

#endif
