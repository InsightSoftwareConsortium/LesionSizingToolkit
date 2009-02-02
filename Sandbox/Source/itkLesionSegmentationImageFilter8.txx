/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationImageFilter8.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLesionSegmentationImageFilter8_txx
#define __itkLesionSegmentationImageFilter8_txx
#include "itkLesionSegmentationImageFilter8.h"

#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

namespace itk
{
  
template <class TInputImage, class TOutputImage>
LesionSegmentationImageFilter8<TInputImage, TOutputImage>::
LesionSegmentationImageFilter8()
{
  m_CannyEdgesFeatureGenerator = CannyEdgesFeatureGeneratorType::New();
  m_LesionSegmentationMethod = LesionSegmentationMethodType::New();
  m_LungWallFeatureGenerator = LungWallGeneratorType::New();
  m_VesselnessFeatureGenerator = VesselnessGeneratorType::New();
  m_SigmoidFeatureGenerator = SigmoidFeatureGeneratorType::New();
  m_FeatureAggregator = FeatureAggregatorType::New();
  m_SegmentationModule = SegmentationModuleType::New();
  m_CropFilter = CropFilterType::New();
  m_IsotropicResampler = IsotropicResamplerType::New();
  m_InputSpatialObject = InputImageSpatialObjectType::New();

  // Report progress.
  m_CommandObserver    = CommandType::New();
  m_CommandObserver->SetCallbackFunction(
    this, &Self::ProgressUpdate );
  m_LungWallFeatureGenerator->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_SigmoidFeatureGenerator->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_VesselnessFeatureGenerator->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_CannyEdgesFeatureGenerator->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_SegmentationModule->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_CropFilter->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );
  m_IsotropicResampler->AddObserver( 
      itk::ProgressEvent(), m_CommandObserver );

  // Connect pipeline
  m_LungWallFeatureGenerator->SetInput( m_InputSpatialObject );
  m_SigmoidFeatureGenerator->SetInput( m_InputSpatialObject );
  m_VesselnessFeatureGenerator->SetInput( m_InputSpatialObject );
  m_CannyEdgesFeatureGenerator->SetInput( m_InputSpatialObject );
  m_FeatureAggregator->AddFeatureGenerator( m_LungWallFeatureGenerator );
  m_FeatureAggregator->AddFeatureGenerator( m_VesselnessFeatureGenerator );
  m_FeatureAggregator->AddFeatureGenerator( m_SigmoidFeatureGenerator );
  m_FeatureAggregator->AddFeatureGenerator( m_CannyEdgesFeatureGenerator );
  m_LesionSegmentationMethod->AddFeatureGenerator( m_FeatureAggregator );
  m_LesionSegmentationMethod->SetSegmentationModule( m_SegmentationModule );

  // Populate some parameters
  m_LungWallFeatureGenerator->SetLungThreshold( -400 );
  m_VesselnessFeatureGenerator->SetSigma( 1.0 );
  m_VesselnessFeatureGenerator->SetAlpha1( 0.1 );
  m_VesselnessFeatureGenerator->SetAlpha2( 2.0 );
  m_VesselnessFeatureGenerator->SetSigmoidAlpha( -10.0 );
  m_VesselnessFeatureGenerator->SetSigmoidBeta( 80.0 );
  m_SigmoidFeatureGenerator->SetAlpha( 100.0 );
  m_SigmoidFeatureGenerator->SetBeta( -500.0 );
  m_CannyEdgesFeatureGenerator->SetSigma(1.0);
  m_CannyEdgesFeatureGenerator->SetUpperThreshold( 150.0 );
  m_CannyEdgesFeatureGenerator->SetLowerThreshold( 75.0 );
  m_FastMarchingStoppingTime = 5.0;
  m_FastMarchingDistanceFromSeeds = 0.5;  
  m_SigmoidBeta = -500.0;
  m_StatusMessage = "";
  m_SegmentationModule->SetCurvatureScaling(1.0);
  m_SegmentationModule->SetAdvectionScaling(0.0);
  m_SegmentationModule->SetPropagationScaling(500.0);
  m_SegmentationModule->SetMaximumRMSError(0.0002);
  m_SegmentationModule->SetMaximumNumberOfIterations(300);
  m_UseIsotropicResampling = true;
}
 
template <class TInputImage, class TOutputImage>
void 
LesionSegmentationImageFilter8<TInputImage,TOutputImage>
::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  if ( !this->GetInput() )
    {
    typename InputImageType::Pointer inputPtr  = 
      const_cast< TInputImage *>( this->GetInput() );

    // Request the entire input image
    inputPtr->SetRequestedRegion(inputPtr->GetLargestPossibleRegion());  
    }
}

template <class TInputImage, class TOutputImage>
void 
LesionSegmentationImageFilter8<TInputImage,TOutputImage>
::GenerateOutputInformation() 
{
  typedef typename SizeType::SizeValueType SizeValueType;

  // get pointers to the input and output
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();

  if ( !outputPtr || !inputPtr)
    {
    return;
    }

  // Compute the spacing after isotropic resampling.
  double minSpacing = NumericTraits< double >::max();
  for (int i = 0; i < ImageDimension; i++)
    {
    minSpacing = (minSpacing > inputPtr->GetSpacing()[i] ? 
                  inputPtr->GetSpacing()[i] : minSpacing);
    }
  
  // Compute the new size due to isotropic upresampling of the cropped region
  SizeType resampledSize;
  for (int i = 0; i < ImageDimension; i++)
    {
    const double d = m_RegionOfInterest.GetSize()[i] 
                     * inputPtr->GetSpacing()[i] / minSpacing;
    resampledSize[i] = static_cast<SizeValueType>( d );
    }  

  // Copy Information without modification.
  outputPtr->CopyInformation( inputPtr );

  // Compute the regions due to resampling

  IndexType start;
  start.Fill(0);
  // RegionType region( start, resampledSize ); // Does not work ??
  RegionType region( start, m_RegionOfInterest.GetSize() );
  m_ResampledSize = resampledSize;

  // Adjust output region
  outputPtr->SetLargestPossibleRegion(region);

  // Correct origin of the extracted region.
  IndexType roiStart( m_RegionOfInterest.GetIndex() );
  typename Superclass::OutputImageType::PointType  outputOrigin;
  typedef Image< ITK_TYPENAME TInputImage::PixelType,
    Superclass::InputImageDimension > ImageType;
  typename ImageType::ConstPointer imagePtr =
    dynamic_cast< const ImageType * >( inputPtr.GetPointer() );
  if ( imagePtr )
    {
    // Input image supports TransformIndexToContinuousPoint
    inputPtr->TransformIndexToPhysicalPoint( roiStart, outputOrigin);
    }
  else
    {
    // Generic type of image
    const typename Superclass::InputImageType::PointType&
      inputOrigin = inputPtr->GetOrigin();

    const typename Superclass::InputImageType::SpacingType&
      spacing = inputPtr->GetSpacing() ;
 
    for( unsigned int i=0; i<ImageDimension; i++)
      {
      outputOrigin[i] = inputOrigin[i] + roiStart[i] * spacing[i];
      }
    }
  
  outputPtr->SetOrigin( outputOrigin );  

  if (m_UseIsotropicResampling)
    {
    // Adjust the spacing to the isotropic spacing
    typename Superclass::InputImageType::SpacingType outputSpacing;
    outputSpacing.Fill( minSpacing );
    outputPtr->SetSpacing( outputSpacing );
    }
}

  
template< class TInputImage, class TOutputImage >
void
LesionSegmentationImageFilter8< TInputImage, TOutputImage >
::GenerateData()
{
  m_SigmoidFeatureGenerator->SetBeta( m_SigmoidBeta );
  m_SegmentationModule->SetDistanceFromSeeds(m_FastMarchingDistanceFromSeeds);
  m_SegmentationModule->SetStoppingValue(m_FastMarchingStoppingTime);

  // Allocate the output
  if (!this->m_UseIsotropicResampling)
    {
    this->GetOutput()->SetBufferedRegion( this->GetOutput()->GetRequestedRegion() );
    }
  else
    {
    this->GetOutput()->SetLargestPossibleRegion( m_ResampledSize );
    this->GetOutput()->SetBufferedRegion( m_ResampledSize );
    }
  this->GetOutput()->Allocate();
 
  // Get the input image
  typename  InputImageType::ConstPointer  input  = this->GetInput();

  // Crop
  m_CropFilter->SetInput(input);
  m_CropFilter->SetRegionOfInterest(m_RegionOfInterest);
  m_CropFilter->Update(); 
  typename InputImageType::Pointer inputImage = m_CropFilter->GetOutput();

  if (m_UseIsotropicResampling)
    { 
    // Resample to isotropic dimensions. We will resample to the min(Spacing).

    m_IsotropicResampler->SetInput( m_CropFilter->GetOutput() );
    double minSpacing = NumericTraits< double >::max();
    for (int i = 0; i < ImageDimension; i++)
      {
      minSpacing = (minSpacing > input->GetSpacing()[i] ? 
                    input->GetSpacing()[i] : minSpacing);
      }
    m_IsotropicResampler->SetOutputSpacing( minSpacing );
    m_IsotropicResampler->Update();
    inputImage = m_IsotropicResampler->GetOutput();
    }


  // Convert the output of resampling (or cropping based on 
  // m_UseIsotropicResampling) to a spatial object that can be fed into
  // the lesion segmentation method

  inputImage->DisconnectPipeline();
  m_InputSpatialObject->SetImage(inputImage);
  
  // Sigma for the canny is the max spacing of the original input (before 
  // resampling)

  double maxSpacing = NumericTraits< double >::min();  
  for (int i = 0; i < ImageDimension; i++)
    {
    maxSpacing = (maxSpacing < input->GetSpacing()[i] ? 
                    input->GetSpacing()[i] : maxSpacing);
    }
  m_CannyEdgesFeatureGenerator->SetSigma( maxSpacing );

  // Seeds

  typename SeedSpatialObjectType::Pointer seedSpatialObject =
    SeedSpatialObjectType::New();
  seedSpatialObject->SetPoints(m_Seeds);
  m_LesionSegmentationMethod->SetInitialSegmentation(seedSpatialObject);
  
  // Do the actual segmentation.
  m_LesionSegmentationMethod->Update();
  
  // Graft the output.
  typename SpatialObjectType::Pointer segmentation = 
    const_cast< SpatialObjectType * >(m_SegmentationModule->GetOutput());
  typename OutputSpatialObjectType::Pointer outputObject = 
    dynamic_cast< OutputSpatialObjectType * >( segmentation.GetPointer() );
  typename OutputImageType::Pointer outputImage = 
    const_cast< OutputImageType * >(outputObject->GetImage());
  outputImage->DisconnectPipeline();
  this->GraftOutput(outputImage);

  /*typedef ImageFileWriter< OutputImageType > WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName("output.mha");
  writer->SetInput(outputImage);
  writer->Write();*/
}


template <class TInputImage, class TOutputImage>
void LesionSegmentationImageFilter8< TInputImage,TOutputImage >
::ProgressUpdate( Object * caller, 
                  const EventObject & e )
{
  if( typeid( itk::ProgressEvent ) == typeid( e ) )
    {  
    if (dynamic_cast< CropFilterType * >(caller))
      {
      this->m_StatusMessage = "Cropping data..";
      this->UpdateProgress( m_CropFilter->GetProgress() );
      }

    if (dynamic_cast< IsotropicResamplerType * >(caller))
      {
      this->m_StatusMessage = "Isotropic resampling of data using BSpline interpolation..";
      this->UpdateProgress( m_IsotropicResampler->GetProgress() );
      }
    
    else if (dynamic_cast< LungWallGeneratorType * >(caller))
      {
      // Given its iterative nature.. a cranky heuristic here.
      this->m_StatusMessage = "Generating lung wall feature by front propagation..";
      this->UpdateProgress( ((double)(((int)(
        m_LungWallFeatureGenerator->GetProgress()*500))%100))/100.0 );
      }

    else if (dynamic_cast< SigmoidFeatureGeneratorType * >(caller))
      {
      this->m_StatusMessage = "Generating intensity feature..";
      this->UpdateProgress( m_SigmoidFeatureGenerator->GetProgress() );
      }

    else if (dynamic_cast< CannyEdgesFeatureGeneratorType * >(caller))
      {
      m_StatusMessage = "Generating canny edge feature..";
      this->UpdateProgress( m_CannyEdgesFeatureGenerator->GetProgress());
      }

    else if (dynamic_cast< VesselnessGeneratorType * >(caller))
      {
      m_StatusMessage = "Generating vesselness feature (Sato et al.)..";
      this->UpdateProgress( m_LungWallFeatureGenerator->GetProgress() );
      }

    else if (dynamic_cast< SegmentationModuleType * >(caller))
      {
      m_StatusMessage = "Segmenting using level sets..";
      this->UpdateProgress( m_SegmentationModule->GetProgress() );
      }
    }
}

template <class TInputImage, class TOutputImage>
void LesionSegmentationImageFilter8< TInputImage,TOutputImage >
::SetAbortGenerateData( bool abort )
{
  this->Superclass::SetAbortGenerateData(abort);
  this->m_CropFilter->SetAbortGenerateData(abort);
  this->m_LesionSegmentationMethod->SetAbortGenerateData(abort);
}


template <class TInputImage, class TOutputImage>
void 
LesionSegmentationImageFilter8<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

}//end of itk namespace
#endif

