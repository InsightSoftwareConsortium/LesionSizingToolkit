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

namespace itk 
{
template< unsigned int NDimension>
class FeatureAggregatorSurrogate : public FeatureAggregator< NDimension >
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
  FeatureAggregatorSurrogate() {};
  ~FeatureAggregatorSurrogate() override {};
  void PrintSelf(std::ostream& os, Indent indent) const override 
    {
    this->Superclass::PrintSelf( os, indent );
    }

private:
  FeatureAggregatorSurrogate(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  using FeatureGeneratorType = typename Superclass::FeatureGeneratorType;
  using FeatureGeneratorPointer = typename FeatureGeneratorType::Pointer;
  using FeatureGeneratorArrayType = std::vector< FeatureGeneratorPointer >;
  using FeatureGeneratorIterator = typename FeatureGeneratorArrayType::iterator;
  using FeatureGeneratorConstIterator = typename FeatureGeneratorArrayType::const_iterator;

  void ConsolidateFeatures() override
    {
    using FeaturePixelType = float;
    using FeatureImageType = Image< FeaturePixelType, NDimension >;
    using FeatureSpatialObjectType = ImageSpatialObject< NDimension, FeaturePixelType >;

    const auto * firstFeatureObject = dynamic_cast< const FeatureSpatialObjectType * >( this->GetInputFeature(0) );

    const FeatureImageType * firstFeatureImage = firstFeatureObject->GetImage();

    typename FeatureImageType::Pointer consolidatedFeatureImage = FeatureImageType::New();

    consolidatedFeatureImage->CopyInformation( firstFeatureImage );
    consolidatedFeatureImage->SetRegions( firstFeatureImage->GetBufferedRegion() );
    consolidatedFeatureImage->Allocate();
    consolidatedFeatureImage->FillBuffer( NumericTraits< FeaturePixelType >::max() );

    const unsigned int numberOfFeatures = this->GetNumberOfInputFeatures();

    for( unsigned int i = 0; i < numberOfFeatures; i++ )
      {
      const auto * featureObject = dynamic_cast< const FeatureSpatialObjectType * >( this->GetInputFeature(i) );

      const FeatureImageType * featureImage = featureObject->GetImage();

      using FeatureIterator = ImageRegionIterator< FeatureImageType >;
      using FeatureConstIterator = ImageRegionConstIterator< FeatureImageType >;

      FeatureIterator       dstitr( consolidatedFeatureImage, consolidatedFeatureImage->GetBufferedRegion() );
      FeatureConstIterator  srcitr( featureImage, featureImage->GetBufferedRegion() );

      dstitr.GoToBegin();
      srcitr.GoToBegin();
     
      while( !srcitr.IsAtEnd() )
        {
        if( dstitr.Get() > srcitr.Get() )
          {
          dstitr.Set( srcitr.Get() );
          }
        ++srcitr;
        ++dstitr;
        }
      }

    auto * outputObject = dynamic_cast< FeatureSpatialObjectType * >(this->ProcessObject::GetOutput(0));

    outputObject->SetImage( consolidatedFeatureImage );
    }

};

} // end of itk namespace


int itkFeatureAggregatorTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " landmarksFile inputImage outputImage ";
    std::cerr << " [lowerThreshold upperThreshold] " << std::endl;
    return EXIT_FAILURE;
    }


  constexpr unsigned int Dimension = 3;
  using InputPixelType = signed short;

  using InputImageType = itk::Image< InputPixelType, Dimension >;

  using InputImageReaderType = itk::ImageFileReader< InputImageType >;
  InputImageReaderType::Pointer inputImageReader = InputImageReaderType::New();

  inputImageReader->SetFileName( argv[2] );

  try 
    {
    inputImageReader->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }


  using AggregatorType = itk::FeatureAggregatorSurrogate< Dimension >;

  AggregatorType::Pointer  featureAggregator = AggregatorType::New();
  
  using VesselnessGeneratorType = itk::SatoVesselnessSigmoidFeatureGenerator< Dimension >;
  VesselnessGeneratorType::Pointer vesselnessGenerator = VesselnessGeneratorType::New();

  using LungWallGeneratorType = itk::LungWallFeatureGenerator< Dimension >;
  LungWallGeneratorType::Pointer lungWallGenerator = LungWallGeneratorType::New();

  using SigmoidFeatureGeneratorType = itk::SigmoidFeatureGenerator< Dimension >;
  SigmoidFeatureGeneratorType::Pointer  sigmoidGenerator = SigmoidFeatureGeneratorType::New();
 
  featureAggregator->AddFeatureGenerator( lungWallGenerator );
  featureAggregator->AddFeatureGenerator( vesselnessGenerator );
  featureAggregator->AddFeatureGenerator( sigmoidGenerator );


  using SpatialObjectType = AggregatorType::SpatialObjectType;

  using InputImageSpatialObjectType = itk::ImageSpatialObject< Dimension, InputPixelType  >;
  InputImageSpatialObjectType::Pointer inputObject = InputImageSpatialObjectType::New();

  InputImageType::Pointer inputImage = inputImageReader->GetOutput();

  inputImage->DisconnectPipeline();

  inputObject->SetImage( inputImage );

  lungWallGenerator->SetInput( inputObject );
  vesselnessGenerator->SetInput( inputObject );
  sigmoidGenerator->SetInput( inputObject );

  lungWallGenerator->SetLungThreshold( -400 );

  vesselnessGenerator->SetSigma( 1.0 );
  vesselnessGenerator->SetAlpha1( 0.5 );
  vesselnessGenerator->SetAlpha2( 2.0 );
 
  sigmoidGenerator->SetAlpha(  1.0  );
  sigmoidGenerator->SetBeta( -200.0 );
 
  using SegmentationModuleType = itk::ConnectedThresholdSegmentationModule< Dimension >;
  
  SegmentationModuleType::Pointer  segmentationModule = SegmentationModuleType::New();


  double lowerThreshold = 0.5;
  double upperThreshold = 1.0;

  if( argc > 4 )
    {
    lowerThreshold = atof( argv[4] );
    }

  if( argc > 5 )
    {
    upperThreshold = atof( argv[5] );
    }

  segmentationModule->SetLowerThreshold( lowerThreshold );
  segmentationModule->SetUpperThreshold( upperThreshold );

  featureAggregator->Update();

  
  using SpatialObjectType = SegmentationModuleType::SpatialObjectType;
  using OutputSpatialObjectType = SegmentationModuleType::OutputSpatialObjectType;
  using OutputImageType = SegmentationModuleType::OutputImageType;

  SpatialObjectType::ConstPointer segmentation = featureAggregator->GetFeature();

  OutputSpatialObjectType::ConstPointer outputObject = 
    dynamic_cast< const OutputSpatialObjectType * >( segmentation.GetPointer() );

  OutputImageType::ConstPointer outputImage = outputObject->GetImage();

  using OutputWriterType = itk::ImageFileWriter< OutputImageType >;
  OutputWriterType::Pointer writer = OutputWriterType::New();

  writer->SetFileName( argv[3] );
  writer->SetInput( outputImage );
  writer->UseCompressionOn();


  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  featureAggregator->Print( std::cout );

  return EXIT_SUCCESS;
}
