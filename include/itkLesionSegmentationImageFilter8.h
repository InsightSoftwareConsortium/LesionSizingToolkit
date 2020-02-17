/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationImageFilter8.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkLesionSegmentationImageFilter8_h
#define itkLesionSegmentationImageFilter8_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkFixedArray.h"
#include "itkCommand.h"
#include "itkImageSpatialObject.h"
#include "itkLandmarkSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkCannyEdgesFeatureGenerator.h"
#include "itkFastMarchingAndGeodesicActiveContourLevelSetSegmentationModule.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkLesionSegmentationMethod.h"
#include "itkMinimumFeatureAggregator.h"
#include "itkIsotropicResamplerImageFilter.h"
#include <string>

namespace itk
{

/** \class LesionSegmentationImageFilter8
 * \ingroup LesionSizingToolkit
 */
template <typename TInputImage, typename TOutputImage>
class LesionSegmentationImageFilter8 : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard "Self" & Superclass type alias.  */
  using Self = LesionSegmentationImageFilter8;
  using Superclass = ImageToImageFilter<TInputImage, TOutputImage>;

  /** Image type alias support   */
  using InputImageType = TInputImage;
  using OutputImageType = TOutputImage;

  /** SmartPointer type alias support  */
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Define pixel types. */
  using InputImagePixelType = typename TInputImage::PixelType;
  using OutputImagePixelType = typename TOutputImage::PixelType;
  using IndexType = typename TInputImage::IndexType;
  using SpacingType = typename InputImageType::SpacingType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Typedef to describe the output image region type. */
  using OutputImageRegionType = typename TOutputImage::RegionType;
  using RegionType = typename TOutputImage::RegionType;

  /** Run-time type information (and related methods). */
  itkTypeMacro(LesionSegmentationImageFilter8, ImageToImageFilter);

  /** ImageDimension constant    */
  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;

  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  using CannyEdgesFeatureGeneratorType = CannyEdgesFeatureGenerator<ImageDimension>;
  using SigmaArrayType = typename CannyEdgesFeatureGeneratorType::SigmaArrayType;

  void
  GenerateInputRequestedRegion() throw(InvalidRequestedRegionError) override;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck, (Concept::HasNumericTraits<InputImagePixelType>));
  itkConceptMacro(OutputHasNumericTraitsCheck, (Concept::HasNumericTraits<OutputImagePixelType>));
  itkConceptMacro(SameDimensionCheck, (Concept::SameDimension<ImageDimension, OutputImageDimension>));
  itkConceptMacro(OutputIsFloatingPointCheck, (Concept::IsFloatingPoint<OutputImagePixelType>));
  /** End concept checking */
#endif

  /** Set the ROI */
  itkSetMacro(RegionOfInterest, RegionType);
  itkGetMacro(RegionOfInterest, RegionType);

  /** Set the beta for the sigmoid intensity feature */
  itkSetMacro(SigmoidBeta, double);
  itkGetMacro(SigmoidBeta, double);

  /** Turn On/Off isotropic resampling prior to running the segmentation */
  itkSetMacro(ResampleThickSliceData, bool);
  itkGetMacro(ResampleThickSliceData, bool);
  itkBooleanMacro(ResampleThickSliceData);

  /** If ResampleThickSliceData is ON, set the maximum anisotropy. Defauls
   * to twice the minimum spacing of the data. That will mean that
   * 0.7 x 0.7 x 1.25 mm CT data will not be resampled. However
   * 0.7 x 0.7 x 2.5 mm CT data will be resampled to 0.7 x 0.7 x 1.25 mm
   * thick slices. Values less than 1.0 result in supersampling of the
   * data. A value of 1 results in Isotropic resampling of the data.
   */
  itkSetMacro(AnisotropyThreshold, double);
  itkGetMacro(AnisotropyThreshold, double);

  /** Turn On/Off the use of vessel enhancing diffusion (R. Manniesing et al)
   * prior to computing the vesselness. This is slow. Defaults to false. */
  virtual void
  SetUseVesselEnhancingDiffusion(bool);
  itkBooleanMacro(UseVesselEnhancingDiffusion);

  using SeedSpatialObjectType = itk::LandmarkSpatialObject<ImageDimension>;
  using LandmarkPointListType = typename SeedSpatialObjectType::LandmarkPointListType;

  void
  SetSeeds(LandmarkPointListType p)
  {
    this->m_Seeds = p;
  }
  LandmarkPointListType
  GetSeeds()
  {
    return m_Seeds;
  }

  /** Report progress */
  void
  ProgressUpdate(Object * caller, const EventObject & event);

  // Return the status message
  const char *
  GetStatusMessage() const
  {
    return m_StatusMessage.length() ? m_StatusMessage.c_str() : nullptr;
  }

  /* Manually specify sigma. This defaults to the max spacing in the dataset */
  virtual void
  SetSigma(SigmaArrayType sigmas);

  /** Override the superclass implementation so as to set the flag on all the
   * filters within our lesion segmentation pipeline */
  void
  SetAbortGenerateData(const bool) override;

protected:
  LesionSegmentationImageFilter8();
  LesionSegmentationImageFilter8(const Self &) {}
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  void
  GenerateOutputInformation() override;
  void
  GenerateData() override;

  // Filters used by this class
  using LesionSegmentationMethodType = LesionSegmentationMethod<ImageDimension>;
  using VesselnessGeneratorType = SatoVesselnessSigmoidFeatureGenerator<ImageDimension>;
  using LungWallGeneratorType = LungWallFeatureGenerator<ImageDimension>;
  using SigmoidFeatureGeneratorType = SigmoidFeatureGenerator<ImageDimension>;
  using FeatureAggregatorType = MinimumFeatureAggregator<ImageDimension>;
  using SegmentationModuleType = FastMarchingAndGeodesicActiveContourLevelSetSegmentationModule<ImageDimension>;
  using CropFilterType = RegionOfInterestImageFilter<InputImageType, InputImageType>;
  using SpatialObjectType = typename SegmentationModuleType::SpatialObjectType;
  using OutputSpatialObjectType = typename SegmentationModuleType::OutputSpatialObjectType;
  using InputImageSpatialObjectType = ImageSpatialObject<ImageDimension, InputImagePixelType>;
  using IsotropicResamplerType = IsotropicResamplerImageFilter<InputImageType, InputImageType>;
  using SizeType = typename RegionType::SizeType;
  using SizeValueType = typename SizeType::SizeValueType;
  using CommandType = MemberCommand<Self>;


private:
  ~LesionSegmentationImageFilter8() override = default;
  ;

  double m_SigmoidBeta;
  double m_FastMarchingStoppingTime;
  double m_FastMarchingDistanceFromSeeds;

  typename LesionSegmentationMethodType::Pointer        m_LesionSegmentationMethod;
  typename LungWallGeneratorType::Pointer               m_LungWallFeatureGenerator;
  typename VesselnessGeneratorType::Pointer             m_VesselnessFeatureGenerator;
  typename SigmoidFeatureGeneratorType::Pointer         m_SigmoidFeatureGenerator;
  typename CannyEdgesFeatureGeneratorType::Pointer      m_CannyEdgesFeatureGenerator;
  typename FeatureAggregatorType::Pointer               m_FeatureAggregator;
  typename SegmentationModuleType::Pointer              m_SegmentationModule;
  typename CropFilterType::Pointer                      m_CropFilter;
  typename IsotropicResamplerType::Pointer              m_IsotropicResampler;
  typename CommandType::Pointer                         m_CommandObserver;
  RegionType                                            m_RegionOfInterest;
  std::string                                           m_StatusMessage;
  typename SeedSpatialObjectType::LandmarkPointListType m_Seeds;
  typename InputImageSpatialObjectType::Pointer         m_InputSpatialObject;
  bool                                                  m_ResampleThickSliceData;
  double                                                m_AnisotropyThreshold;
  bool                                                  m_UserSpecifiedSigmas;
};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkLesionSegmentationImageFilter8.hxx"
#endif

#endif
