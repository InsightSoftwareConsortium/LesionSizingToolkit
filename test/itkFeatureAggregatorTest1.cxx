/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkFeatureAggregatorTest1.cxx

  Copyright (c) Kitware Inc.
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkFeatureAggregator.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSpatialObject.h"
#include "itkSpatialObjectReader.h"
#include "itkImageMaskSpatialObject.h"
#include "itkLungWallFeatureGenerator.h"
#include "itkSatoVesselnessSigmoidFeatureGenerator.h"
#include "itkSigmoidFeatureGenerator.h"
#include "itkConnectedThresholdSegmentationModule.h"
#include "itkTestingMacros.h"

namespace itk
{
template <unsigned int NDimension>
class FeatureAggregatorSurrogate : public FeatureAggregator<NDimension>
{
public:
  /** Standard class type alias. */
  using Self = FeatureAggregatorSurrogate;
  using Superclass = FeatureAggregator<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FeatureAggregatorSurrogate, FeatureAggregator);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;


protected:
  FeatureAggregatorSurrogate(){};
  ~FeatureAggregatorSurrogate() override{};
  void
  PrintSelf(std::ostream & os, Indent indent) const override
  {
    this->Superclass::PrintSelf(os, indent);
  }

private:
  FeatureAggregatorSurrogate(const Self &) = delete; // purposely not implemented
  void
  operator=(const Self &) = delete; // purposely not implemented

  using FeatureGeneratorType = typename Superclass::FeatureGeneratorType;
  using FeatureGeneratorPointer = typename FeatureGeneratorType::Pointer;
  using FeatureGeneratorArrayType = std::vector<FeatureGeneratorPointer>;
  using FeatureGeneratorIterator = typename FeatureGeneratorArrayType::iterator;
  using FeatureGeneratorConstIterator = typename FeatureGeneratorArrayType::const_iterator;

  void
  ConsolidateFeatures() override
  {
    using FeaturePixelType = float;
    using FeatureImageType = Image<FeaturePixelType, NDimension>;
    using FeatureSpatialObjectType = ImageSpatialObject<NDimension, FeaturePixelType>;

    const auto * firstFeatureObject = dynamic_cast<const FeatureSpatialObjectType *>(this->GetInputFeature(0));

    const FeatureImageType * firstFeatureImage = firstFeatureObject->GetImage();

    typename FeatureImageType::Pointer consolidatedFeatureImage = FeatureImageType::New();

    consolidatedFeatureImage->CopyInformation(firstFeatureImage);
    consolidatedFeatureImage->SetRegions(firstFeatureImage->GetBufferedRegion());
    consolidatedFeatureImage->Allocate();
    consolidatedFeatureImage->FillBuffer(NumericTraits<FeaturePixelType>::max());

    const unsigned int numberOfFeatures = this->GetNumberOfInputFeatures();

    for (unsigned int i = 0; i < numberOfFeatures; i++)
    {
      const auto * featureObject = dynamic_cast<const FeatureSpatialObjectType *>(this->GetInputFeature(i));

      const FeatureImageType * featureImage = featureObject->GetImage();

      using FeatureIterator = ImageRegionIterator<FeatureImageType>;
      using FeatureConstIterator = ImageRegionConstIterator<FeatureImageType>;

      FeatureIterator      dstitr(consolidatedFeatureImage, consolidatedFeatureImage->GetBufferedRegion());
      FeatureConstIterator srcitr(featureImage, featureImage->GetBufferedRegion());

      dstitr.GoToBegin();
      srcitr.GoToBegin();

      while (!srcitr.IsAtEnd())
      {
        if (dstitr.Get() > srcitr.Get())
        {
          dstitr.Set(srcitr.Get());
        }
        ++srcitr;
        ++dstitr;
      }
    }

    auto * outputObject = dynamic_cast<FeatureSpatialObjectType *>(this->ProcessObject::GetOutput(0));

    outputObject->SetImage(consolidatedFeatureImage);
  }
};

} // namespace itk


int
itkFeatureAggregatorTest1(int argc, char * argv[])
{
  if (argc < 3)
  {
    std::cerr << "Missing parameters." << std::endl;
    std::cerr << "Usage: " << argv[0];
    std::cerr << " landmarksFile inputImage outputImage ";
    std::cerr << " [lowerThreshold upperThreshold] " << std::endl;
    return EXIT_FAILURE;
  }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image<InputPixelType, Dimension>;

  using InputImageReaderType = itk::ImageFileReader<InputImageType>;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName(argv[2]);

  TRY_EXPECT_NO_EXCEPTION(inputImageReader->Update());


  using AggregatorType = itk::FeatureAggregatorSurrogate<Dimension>;

  AggregatorType::Pointer featureAggregator = AggregatorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(featureAggregator, FeatureAggregatorSurrogate, FeatureAggregator);

  using VesselnessGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator<Dimension>;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(vesselnessGenerator, SatoVesselnessSigmoidFeatureGenerator, FeatureAggregator);

  using LungWallGeneratorType = itk::LungWallFeatureGenerator<Dimension>;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(lungWallGenerator, LungWallFeatureGenerator, FeatureAggregator);


  using SigmoidFeatureGeneratorType = itk::SigmoidFeatureGenerator<Dimension>;
  SigmoidFeatureGeneratorType::Pointer sigmoidGenerator = SigmoidFeatureGeneratorType::New();

  EXERCISE_BASIC_OBJECT_METHODS(sigmoidGenerator, SigmoidFeatureGenerator, FeatureAggregator);

  featureAggregator->AddFeatureGenerator(lungWallGenerator);
  featureAggregator->AddFeatureGenerator(vesselnessGenerator);
  featureAggregator->AddFeatureGenerator(sigmoidGenerator);


  using SpatialObjectType = AggregatorType::SpatialObjectType;

  using InputImageSpatialObjectType = itk::ImageSpatialObject<Dimension, InputPixelType>;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage(inputImage);

  lungWallGenerator->SetInput(inputObject);
  vesselnessGenerator->SetInput(inputObject);
  sigmoidGenerator->SetInput(inputObject);

  LungWallGeneratorType::InputPixelType lungThreshold = -400;
  lungWallGenerator->SetLungThreshold(lungThreshold);
  TEST_SET_GET_VALUE(lungThreshold, lungWallGenerator->GetLungThreshold());


  double sigma = 1.0;
  vesselnessGenerator->SetSigma(sigma);
  TEST_SET_GET_VALUE(sigma, vesselnessGenerator->GetSigma());

  double alpha1 = 0.5;
  vesselnessGenerator->SetAlpha1(alpha1);
  TEST_SET_GET_VALUE(alpha1, vesselnessGenerator->GetAlpha1());

  double alpha2 = 2.0;
  vesselnessGenerator->SetAlpha2(alpha2);
  TEST_SET_GET_VALUE(alpha2, vesselnessGenerator->GetAlpha2());


  double alpha = 1.0;
  sigmoidGenerator->SetAlpha(alpha);
  TEST_SET_GET_VALUE(alpha, sigmoidGenerator->GetAlpha());

  double beta = -200.0;
  sigmoidGenerator->SetBeta(beta);
  TEST_SET_GET_VALUE(beta, sigmoidGenerator->GetBeta());

  using SegmentationModuleType = itk::ConnectedThresholdSegmentationModule<Dimension>;

  SegmentationModuleType::Pointer segmentationModule = SegmentationModuleType::New();


  double lowerThreshold = 0.5;
  if (argc > 4)
  {
    lowerThreshold = std::stod(argv[4]);
  }
  segmentationModule->SetLowerThreshold(lowerThreshold);
  TEST_SET_GET_VALUE(lowerThreshold, segmentationModule->GetLowerThreshold());

  double upperThreshold = 1.0;
  if (argc > 5)
  {
    upperThreshold = std::stod(argv[5]);
  }
  segmentationModule->SetUpperThreshold(upperThreshold);
  TEST_SET_GET_VALUE(upperThreshold, segmentationModule->GetUpperThreshold());


  TRY_EXPECT_NO_EXCEPTION(featureAggregator->Update());


  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  SpatialObjectType::ConstPointer segmentation = featureAggregator->GetFeature();

  OutputSpatialObjectType::ConstPointer outputObject =
    dynamic_cast<const OutputSpatialObjectType *>(segmentation.GetPointer());

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter<OutputImageType>;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName(argv[3]);
  writer->SetInput(outputImage);
  writer->UseCompressionOn();

  TRY_EXPECT_NO_EXCEPTION(writer->Update());

  std::cout << "Test finished." << std::endl;
  return EXIT_SUCCESS;
}
