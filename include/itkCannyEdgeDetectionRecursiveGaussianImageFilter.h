/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkCannyEdgeDetectionRecursiveGaussianImageFilter_h
#define itkCannyEdgeDetectionRecursiveGaussianImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkFixedArray.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkSmoothingRecursiveGaussianImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkMultiThreaderBase.h"
#include "itkDerivativeOperator.h"
#include "itkSparseFieldLayer.h"
#include "itkObjectStore.h"


namespace itk
{
using MultiThreader = MultiThreaderBase;


template <typename TValueType>
class ListNode
{
public:
  TValueType m_Value;

  ListNode * Next;
  ListNode * Previous;
};


/** \class CannyEdgeDetectionRecursiveGaussianImageFilter
 *
 * This filter is an implementation of a Canny edge detector for scalar-valued
 * images.  Based on John Canny's paper "A Computational Approach to Edge
 * Detection"(IEEE Transactions on Pattern Analysis and Machine Intelligence,
 * Vol. PAMI-8, No.6, November 1986),  there are four major steps used in the
 * edge-detection scheme:
 * (1) Smooth the input image with Gaussian filter.
 * (2) Calculate the second directional derivatives of the smoothed image.
 * (3) Non-Maximum Suppression: the zero-crossings of 2nd derivative are found,
 *     and the sign of third derivative is used to find the correct extrema.
 * (4) The hysteresis thresholding is applied to the gradient magnitude
 *      (multiplied with zero-crossings) of the smoothed image to find and
 *      link edges.
 *
 * \par Inputs and Outputs
 * The input to this filter should be a scalar, real-valued Itk image of
 * arbitrary dimension.  The output should also be a scalar, real-value Itk
 * image of the same dimensionality.
 *
 * \par Parameters
 * There are four parameters for this filter that control the sub-filters used
 * by the algorithm.
 *
 * \par
 * Sigma is used in the Gaussian smoothing of the input image.
 * See  itkSmoothingRecursiveGaussianImageFilter for information on these
 * parameters.
 *
 * \par
 * Threshold is the lowest allowed value in the output image.  Its data type is
 * the same as the data type of the output image. Any values below the
 * Threshold level will be replaced with the OutsideValue parameter value, whose
 * default is zero.
 *
 * \todo Edge-linking will be added when an itk connected component labeling
 * algorithm is available.
 *
 * \ingroup LesionSizingToolkit
 *
 * \sa SmoothingRecursiveGaussianImageFilter
 * \sa ZeroCrossingImageFilter
 * \sa ThresholdImageFilter */
template <typename TInputImage, typename TOutputImage>
class ITK_TEMPLATE_EXPORT CannyEdgeDetectionRecursiveGaussianImageFilter
  : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard "Self" & Superclass type alias.  */
  using Self = CannyEdgeDetectionRecursiveGaussianImageFilter;
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

  using GaussianImageFilterType = SmoothingRecursiveGaussianImageFilter<InputImageType, OutputImageType>;
  using ScalarRealType = typename GaussianImageFilterType::ScalarRealType;
  using SigmaArrayType = typename GaussianImageFilterType::SigmaArrayType;

  /** The default boundary condition is used unless overridden
   *in the Evaluate() method. */
  using DefaultBoundaryConditionType = ZeroFluxNeumannBoundaryCondition<OutputImageType>;

  /** The type of data structure that is passed to this function object
   * to evaluate at a pixel that does not lie on a data set boundary.
   */
  using NeighborhoodType = ConstNeighborhoodIterator<OutputImageType, DefaultBoundaryConditionType>;

  using ListNodeType = ListNode<IndexType>;
  using ListNodeStorageType = ObjectStore<ListNodeType>;
  using ListType = SparseFieldLayer<ListNodeType>;
  using ListPointerType = typename ListType::Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Typedef to describe the output image region type. */
  using OutputImageRegionType = typename TOutputImage::RegionType;

  /** Run-time type information (and related methods). */
  itkOverrideGetNameOfClassMacro(CannyEdgeDetectionRecursiveGaussianImageFilter);

  /** ImageDimension constant    */
  static constexpr unsigned int ImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  /** Typedef of double containers */
  using ArrayType = FixedArray<double, Self::ImageDimension>;

  /** Smoothing parameters for the Gaussian filter. */
  void
  SetSigmaArray(const SigmaArrayType & sigmas);
  void
  SetSigma(ScalarRealType sigma);
  SigmaArrayType
  GetSigmaArray() const;
  ScalarRealType
  GetSigma() const;

  /* Set the Threshold value for detected edges. */
  void
  SetThreshold(const OutputImagePixelType th)
  {
    this->m_Threshold = th;
    this->m_UpperThreshold = m_Threshold;
    this->m_LowerThreshold = m_Threshold / 2.0;
    itkLegacyReplaceBodyMacro(SetThreshold, 2.2, SetUpperThreshold);
  }

  OutputImagePixelType
  GetThreshold(OutputImagePixelType itkNotUsed(th))
  {
    itkLegacyReplaceBodyMacro(GetThreshold, 2.2, GetUpperThreshold);
    return this->m_Threshold;
  }

  ///* Set the Threshold value for detected edges. */
  itkSetMacro(UpperThreshold, OutputImagePixelType);
  itkGetMacro(UpperThreshold, OutputImagePixelType);

  itkSetMacro(LowerThreshold, OutputImagePixelType);
  itkGetMacro(LowerThreshold, OutputImagePixelType);

  /* Set the Thresholdvalue for detected edges. */
  itkSetMacro(OutsideValue, OutputImagePixelType);
  itkGetMacro(OutsideValue, OutputImagePixelType);

  OutputImageType *
  GetNonMaximumSuppressionImage() const
  {
    return this->m_MultiplyImageFilter->GetOutput();
  }

  /** CannyEdgeDetectionRecursiveGaussianImageFilter needs a larger input requested
   * region than the output requested region ( derivative operators, etc).
   * As such, CannyEdgeDetectionRecursiveGaussianImageFilter needs to provide an implementation
   * for GenerateInputRequestedRegion() in order to inform the
   * pipeline execution model.
   *
   * \sa ImageToImageFilter::GenerateInputRequestedRegion()  */
  void
  GenerateInputRequestedRegion() noexcept(false) override;

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputHasNumericTraitsCheck, (Concept::HasNumericTraits<InputImagePixelType>));
  itkConceptMacro(OutputHasNumericTraitsCheck, (Concept::HasNumericTraits<OutputImagePixelType>));
  itkConceptMacro(SameDimensionCheck, (Concept::SameDimension<ImageDimension, OutputImageDimension>));
  itkConceptMacro(InputIsFloatingPointCheck, (Concept::IsFloatingPoint<InputImagePixelType>));
  itkConceptMacro(OutputIsFloatingPointCheck, (Concept::IsFloatingPoint<OutputImagePixelType>));
  /** End concept checking */
#endif

protected:
  CannyEdgeDetectionRecursiveGaussianImageFilter();
  CannyEdgeDetectionRecursiveGaussianImageFilter(const Self &) {}
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  void
  GenerateData() override;

  using MultiplyImageFilterType = MultiplyImageFilter<OutputImageType, OutputImageType, OutputImageType>;

private:
  ~CannyEdgeDetectionRecursiveGaussianImageFilter() override = default;

  /** Thread-Data Structure   */
  struct CannyThreadStruct
  {
    CannyEdgeDetectionRecursiveGaussianImageFilter * Filter;
  };

  /** This allocate storage for m_UpdateBuffer, m_UpdateBuffer1 */
  void
  AllocateUpdateBuffer();

  /** Implement hysteresis thresholding */
  void
  HysteresisThresholding();

  /** Edge linking funciton */
  void
  FollowEdge(IndexType index);

  /** Check if the index is in bounds or not */
  bool
  InBounds(IndexType index);


  /** Calculate the second derivative of the smoothed image, it writes the
   *  result to m_UpdateBuffer using the ThreadedCompute2ndDerivative() method
   *  and multithreading mechanism.   */
  void
  Compute2ndDerivative();

  /**
   * Split the input into "num" pieces, returning region "i" as
   * "splitRegion". This method is called "num" times to return non-overlapping
   * regions. The method returns the number of pieces that the input
   * can be split into by the routine. i.e. return value is less than or equal
   * to "num".
   * \sa ImageSource
   */
  //  virtual
  //  int SplitUpdateContainer(int i, int num, ThreadRegionType& splitRegion);

  /** Does the actual work of calculating of the 2nd derivative over a region
   *  supplied by the multithreading mechanism.
   *
   *  \sa Compute2ndDerivative
   *  \sa Compute2ndDerivativeThreaderCallBack   */
  void
  ThreadedCompute2ndDerivative(const OutputImageRegionType & outputRegionForThread, int threadId);

  /** This callback method uses ImageSource::SplitRequestedRegion to acquire an
   * output region that it passes to ThreadedCompute2ndDerivative for
   * processing.  */
  static ITK_THREAD_RETURN_TYPE
  Compute2ndDerivativeThreaderCallback(void * arg);

  /** This methos is used to calculate the 2nd derivative for
   * non-boundary pixels. It is called by the ThreadedCompute2ndDerivative
   * method. */
  OutputImagePixelType
  ComputeCannyEdge(const NeighborhoodType & it, void * globalData);

  /** Calculate the gradient of the second derivative of the smoothed image,
   *  it writes the result to m_UpdateBuffer1 using the
   *  ThreadedCompute2ndDerivativePos() method and multithreading mechanism.
   */
  void
  Compute2ndDerivativePos();

  /** Does the actual work of calculating of the 2nd derivative over a region
   *  supplied by the multithreading mechanism.
   *
   *  \sa Compute2ndDerivativePos
   *  \sa Compute3ndDerivativePosThreaderCallBack   */
  void
  ThreadedCompute2ndDerivativePos(const OutputImageRegionType & outputRegionForThread, int threadId);

  /**This callback method uses ImageSource::SplitRequestedRegion to acquire an
   * output region that it passes to ThreadedCompute2ndDerivative for
   * processing.   */
  static ITK_THREAD_RETURN_TYPE
  Compute2ndDerivativePosThreaderCallback(void * arg);

  /** Standard deviation of the gaussian used for smoothing */
  SigmaArrayType m_Sigma;

  /** The maximum error of the gaussian blurring kernel in each dimensional
   * direction.  */
  ArrayType m_MaximumError;

  /** Upper threshold value for identifying edges. */
  OutputImagePixelType m_UpperThreshold; // should be float here?

  /** Lower threshold value for identifying edges. */
  OutputImagePixelType m_LowerThreshold; // should be float here?

  /** Threshold value for identifying edges. */
  OutputImagePixelType m_Threshold;

  /** "Background" value for use in thresholding. */
  OutputImagePixelType m_OutsideValue;

  /** Update buffers used during calculation of multiple steps */
  typename OutputImageType::Pointer m_UpdateBuffer1;

  /** Gaussian filter to smooth the input image  */
  typename GaussianImageFilterType::Pointer m_GaussianFilter;

  /** Multiply image filter to multiply with the zero crossings of the second
   *  derivative.  */
  typename MultiplyImageFilterType::Pointer m_MultiplyImageFilter;

  /** Function objects that are used in the inner loops of derivatiVex
      calculations. */
  DerivativeOperator<OutputImagePixelType, Self::ImageDimension> m_ComputeCannyEdge1stDerivativeOper;
  DerivativeOperator<OutputImagePixelType, Self::ImageDimension> m_ComputeCannyEdge2ndDerivativeOper;

  std::slice m_ComputeCannyEdgeSlice[ImageDimension];

  unsigned long m_Stride[ImageDimension];
  unsigned long m_Center;

  typename ListNodeStorageType::Pointer m_NodeStore;
  ListPointerType                       m_NodeList;
};

} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkCannyEdgeDetectionRecursiveGaussianImageFilter.hxx"
#endif

#endif
