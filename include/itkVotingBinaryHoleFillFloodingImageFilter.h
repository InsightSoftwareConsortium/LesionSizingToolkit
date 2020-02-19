/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkVotingBinaryHoleFillFloodingImageFilter_h
#define itkVotingBinaryHoleFillFloodingImageFilter_h

#include "itkImage.h"
#include "itkVotingBinaryImageFilter.h"

#include <vector>

namespace itk
{

/** \class VotingBinaryHoleFillFloodingImageFilter
 *
 * \brief Perform front-propagation under a quorum sensing (voting) algorithm
 * for filling holes in a binary mask.
 *
 * This is an alternative implementation of the
 * VotingBinaryIterativeHoleFillingImageFilter.
 *
 * \ingroup RegionGrowingSegmentation
 * \ingroup LesionSizingToolkit
 */
template <typename TInputImage, typename TOutputImage>
class ITK_TEMPLATE_EXPORT VotingBinaryHoleFillFloodingImageFilter
  : public VotingBinaryImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(VotingBinaryHoleFillFloodingImageFilter);

  /** Standard class type alias. */
  using Self = VotingBinaryHoleFillFloodingImageFilter;
  using Superclass = VotingBinaryImageFilter<TInputImage, TOutputImage>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).  */
  itkTypeMacro(VotingBinaryHoleFillFloodingImageFilter, VotingBinaryImageFilter);

  using InputImageType = typename Superclass::InputImageType;
  using InputImagePointer = typename InputImageType::Pointer;
  using InputImageConstPointer = typename InputImageType::ConstPointer;
  using InputImageRegionType = typename InputImageType::RegionType;
  using InputImagePixelType = typename InputImageType::PixelType;
  using IndexType = typename InputImageType::IndexType;
  using OffsetValueType = typename InputImageType::OffsetValueType;

  using OutputImageType = typename Superclass::OutputImageType;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using OutputImagePixelType = typename OutputImageType::PixelType;

  using InputSizeType = typename Superclass::InputSizeType;

  /** Image dimension constants */
  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  /** Majority threshold. It is the number of pixels over 50% that will decide
   * whether an OFF pixel will become ON or not. For example, if the
   * neighborhood of a pixel has 124 pixels (excluding itself), the 50% will be
   * 62, and if you set upd a Majority threshold of 5, that means that the
   * filter will require 67 or more neighbor pixels to be ON in order to switch
   * the current OFF pixel to ON. The default value is 1. */
  itkGetConstReferenceMacro(MajorityThreshold, unsigned int);
  itkSetMacro(MajorityThreshold, unsigned int);

  /** Set/Get the maximum number of iterations that will be applied to the
   * propagating front */
  itkSetMacro(MaximumNumberOfIterations, unsigned int);
  itkGetMacro(MaximumNumberOfIterations, unsigned int);

  /** Returned the number of iterations used so far. */
  itkGetMacro(CurrentIterationNumber, unsigned int);

  /** Returned the number of pixels changed in total. */
  itkGetMacro(TotalNumberOfPixelsChanged, unsigned int);


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(OutputEqualityComparableCheck, (Concept::EqualityComparable<OutputImagePixelType>));
  itkConceptMacro(InputEqualityComparableCheck, (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(InputConvertibleToOutputCheck, (Concept::Convertible<InputImagePixelType, OutputImagePixelType>));
  itkConceptMacro(SameDimensionCheck, (Concept::SameDimension<InputImageDimension, OutputImageDimension>));
  itkConceptMacro(IntConvertibleToInputCheck, (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(OutputOStreamWritableCheck, (Concept::OStreamWritable<OutputImagePixelType>));
  /** End concept checking */
#endif

protected:
  VotingBinaryHoleFillFloodingImageFilter();
  ~VotingBinaryHoleFillFloodingImageFilter() override;

  void
  GenerateData() override;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

private:
  void
  AllocateOutputImageWorkingMemory();

  void
  InitializeNeighborhood();

  void
  FindAllPixelsInTheBoundaryAndAddThemAsSeeds();

  void
  IterateFrontPropagations();

  void
  VisitAllSeedsAndTransitionTheirState();

  void
  PasteNewSeedValuesToOutputImage();

  void
  SwapSeedArrays();

  void
  ClearSecondSeedArray();

  bool
  TestForQuorumAtCurrentPixel() const;

  void
  PutCurrentPixelNeighborsIntoSeedArray();

  void
  ComputeArrayOfNeighborhoodBufferOffsets();

  void
  ComputeBirthThreshold();

  unsigned int
  GetNeighborhoodSize() const;

  itkSetMacro(CurrentPixelIndex, IndexType);
  itkGetConstReferenceMacro(CurrentPixelIndex, IndexType);

  unsigned int m_MajorityThreshold;

  using SeedArrayType = std::vector<IndexType>;

  SeedArrayType * m_SeedArray1;
  SeedArrayType * m_SeedArray2;

  InputImageRegionType m_InternalRegion;

  using SeedNewValuesArrayType = std::vector<OutputImagePixelType>;

  SeedNewValuesArrayType m_SeedsNewValues;

  unsigned int m_CurrentIterationNumber;
  unsigned int m_MaximumNumberOfIterations;
  unsigned int m_NumberOfPixelsChangedInLastIteration;
  unsigned int m_TotalNumberOfPixelsChanged;

  IndexType m_CurrentPixelIndex;

  //
  // Variables used for addressing the Neighbors.
  // This could be factorized into a helper class.
  //
  OffsetValueType m_OffsetTable[InputImageDimension + 1];

  using NeighborOffsetArrayType = std::vector<OffsetValueType>;

  NeighborOffsetArrayType m_NeighborBufferOffset;


  //
  // Helper cache variables
  //
  const InputImageType * m_InputImage;
  OutputImageType *      m_OutputImage;

  using SeedMaskImageType = itk::Image<unsigned char, InputImageDimension>;
  using SeedMaskImagePointer = typename SeedMaskImageType::Pointer;

  SeedMaskImagePointer m_SeedsMask;

  using NeighborhoodType = itk::Neighborhood<InputImagePixelType, InputImageDimension>;

  NeighborhoodType m_Neighborhood;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkVotingBinaryHoleFillFloodingImageFilter.hxx"
#endif

#endif
