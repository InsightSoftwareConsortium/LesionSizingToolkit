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
#ifndef itkRegionCompetitionImageFilter_h
#define itkRegionCompetitionImageFilter_h

#include "itkImage.h"
#include "itkImageToImageFilter.h"

#include <vector>

namespace itk
{

/** \class RegionCompetitionImageFilter
 *
 * \brief Perform front-propagation from different starting labeled regions.
 *
 * The filter expects two inputs: One gray-scale image and a labeled image.
 * The labels will be used as initial regions from which the fronts will be
 * propagated until they collide with other labeled regions. Each labeled front
 * will compete for pixels against other labels.
 *
 * \ingroup RegionGrowingSegmentation
 * \ingroup LesionSizingToolkit
 */
template <typename TInputImage, typename TOutputImage>
class RegionCompetitionImageFilter : public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(RegionCompetitionImageFilter);

  /** Standard class type alias. */
  using Self = RegionCompetitionImageFilter;
  using Superclass = ImageToImageFilter<TInputImage, TOutputImage>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods).  */
  itkTypeMacro(RegionCompetitionImageFilter, ImageToImageFilter);

  using InputImageType = TInputImage;
  using InputImagePointer = typename InputImageType::Pointer;
  using InputImageConstPointer = typename InputImageType::ConstPointer;
  using InputImageRegionType = typename InputImageType::RegionType;
  using InputSizeType = typename InputImageType::SizeType;
  using InputImagePixelType = typename InputImageType::PixelType;
  using IndexType = typename InputImageType::IndexType;
  using OffsetValueType = typename InputImageType::OffsetValueType;

  using OutputImageType = TOutputImage;
  using OutputImagePointer = typename OutputImageType::Pointer;
  using OutputImageRegionType = typename OutputImageType::RegionType;
  using OutputImagePixelType = typename OutputImageType::PixelType;


  /** Image dimension constants */
  static constexpr unsigned int InputImageDimension = TInputImage::ImageDimension;
  static constexpr unsigned int OutputImageDimension = TOutputImage::ImageDimension;

  /** Set/Get the maximum number of iterations that will be applied to the
   * propagating front */
  itkSetMacro(MaximumNumberOfIterations, unsigned int);
  itkGetMacro(MaximumNumberOfIterations, unsigned int);

  /** Returned the number of iterations used so far. */
  itkGetMacro(CurrentIterationNumber, unsigned int);

  /** Returned the number of pixels changed in total. */
  itkGetMacro(TotalNumberOfPixelsChanged, unsigned int);

  /** Input Labels */
  void
  SetInputLabels(const TOutputImage * inputLabelImage);

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
  RegionCompetitionImageFilter();
  ~RegionCompetitionImageFilter() override;

  void
  GenerateData() override;

  void
  PrintSelf(std::ostream & os, Indent indent) const override;

private:
  void
  AllocateOutputImageWorkingMemory();

  void
  AllocateFrontsWorkingMemory();

  void
  ComputeNumberOfInputLabels();

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
  TestForAvailabilityAtCurrentPixel() const;

  void
  PutCurrentPixelNeighborsIntoSeedArray();

  void
  ComputeArrayOfNeighborhoodBufferOffsets();

  void
  ComputeBirthThreshold();

  itkSetMacro(CurrentPixelIndex, IndexType);
  itkGetConstReferenceMacro(CurrentPixelIndex, IndexType);

  using SeedArrayType = std::vector<IndexType>;

  SeedArrayType * m_SeedArray1;
  SeedArrayType * m_SeedArray2;

  InputImageRegionType m_InternalRegion;

  using SeedNewValuesArrayType = std::vector<OutputImagePixelType>;

  SeedNewValuesArrayType * m_SeedsNewValues;

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
  const InputImageType *  m_InputImage;
  const OutputImageType * m_inputLabelsImage;
  OutputImageType *       m_OutputImage;

  using SeedMaskImageType = itk::Image<unsigned char, InputImageDimension>;
  using SeedMaskImagePointer = typename SeedMaskImageType::Pointer;

  SeedMaskImagePointer m_SeedsMask;

  using NeighborhoodType = itk::Neighborhood<InputImagePixelType, InputImageDimension>;

  NeighborhoodType m_Neighborhood;

  mutable unsigned int m_NumberOfLabels;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkRegionCompetitionImageFilter.hxx"
#endif

#endif
