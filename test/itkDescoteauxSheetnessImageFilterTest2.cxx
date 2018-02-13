/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkDescoteauxSheetnessImageFilterTest2.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkDescoteauxSheetnessImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"

int itkDescoteauxSheetnessImageFilterTest2( int argc, char * argv [] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " outputImage [(bright1/dark:0) sigma sheetness bloobiness noise]" << std::endl;
    return EXIT_FAILURE;
    }

  constexpr unsigned int Dimension = 3;

  using InputPixelType = signed short;
  using OutputPixelType = float;

  using InputImageType = itk::Image< InputPixelType,  Dimension >;
  using OutputImageType = itk::Image< OutputPixelType, Dimension >;

  using HessianFilterType = itk::HessianRecursiveGaussianImageFilter< InputImageType >;
  using HessianImageType = HessianFilterType::OutputImageType;
  using HessianPixelType = HessianImageType::PixelType;

  using EigenValueArrayType = itk::FixedArray< double, HessianPixelType::Dimension >;
  using EigenValueImageType = itk::Image< EigenValueArrayType, Dimension >;

  using EigenAnalysisFilterType = itk::SymmetricEigenAnalysisImageFilter< 
    HessianImageType, EigenValueImageType >;

  using WriterType = itk::ImageFileWriter< OutputImageType >;

  HessianFilterType::Pointer hessian = HessianFilterType::New();
  EigenAnalysisFilterType::Pointer eigen = EigenAnalysisFilterType::New();

  InputImageType::Pointer inputImage = InputImageType::New();

  InputImageType::SpacingType spacing;

  spacing[0] = 0.5;
  spacing[1] = 0.5;
  spacing[2] = 0.5;
  
  inputImage->SetSpacing( spacing );


  InputImageType::SizeType  size;
  size[0] = 101;
  size[1] = 101;
  size[2] = 101;

  InputImageType::RegionType region;
  region.SetSize( size );

  inputImage->SetRegions( region );
  inputImage->Allocate();


  using IteratorType = itk::ImageRegionIterator< InputImageType >;

  InputImageType::SizeType  planeSize;
  planeSize = size;
  planeSize[2] = 1;

  InputImageType::IndexType planeIndex;
  planeIndex[0] = 0;
  planeIndex[1] = 0;
  planeIndex[2] = size[2] / 2;

  InputImageType::RegionType planeRegion;
  planeRegion.SetSize( planeSize );
  planeRegion.SetIndex( planeIndex );

  IteratorType itr( inputImage, planeRegion );

  itr.GoToBegin();

  InputPixelType brightValue = 500;
  InputPixelType darkValue = 0;

  inputImage->FillBuffer( darkValue );

  InputImageType::IndexType firstIndex;
  firstIndex.Fill(0);
  std::cout << "First pixel value = " << inputImage->GetPixel(firstIndex) << std::endl; 
  //
  // Populate the inputImage with a bright XY plane  
  //
  while( !itr.IsAtEnd() )
    {
    itr.Set( brightValue );
    ++itr;
    }


  using FilterType = itk::DescoteauxSheetnessImageFilter< EigenValueImageType, OutputImageType >;

  FilterType::Pointer sheetnessFilter = FilterType::New();
  
  hessian->SetInput( inputImage );
  eigen->SetInput( hessian->GetOutput() );
  sheetnessFilter->SetInput( eigen->GetOutput() );

  sheetnessFilter->SetDetectBrightSheets(true);

  if( argc > 2 )
    {
    sheetnessFilter->SetDetectBrightSheets( atoi( argv[2] ) );
    }

  if( argc > 3 )
    {
    hessian->SetSigma( atof( argv[3] ) );
    }

  if( argc > 4 )
    {
    sheetnessFilter->SetSheetnessNormalization( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    sheetnessFilter->SetBloobinessNormalization( atof( argv[5] ) );
    }

  if( argc > 6 )
    {
    sheetnessFilter->SetNoiseNormalization( atof( argv[6] ) );
    }


  eigen->SetDimension( Dimension );

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[1] );
  writer->SetInput( sheetnessFilter->GetOutput() );

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Name Of Class = " << sheetnessFilter->GetNameOfClass() << std::endl;

  sheetnessFilter->Print( std::cout );

  OutputImageType::ConstPointer outputImage = sheetnessFilter->GetOutput();

  std::cout << "Output Image = " << std::endl;
  outputImage->Print( std::cout );

  using ConstIterator = itk::ImageRegionConstIterator< OutputImageType >;

  ConstIterator citr( outputImage, outputImage->GetBufferedRegion() );
  IteratorType  iitr( inputImage, inputImage->GetBufferedRegion() );

  citr.GoToBegin();
  iitr.GoToBegin();

  bool failed = false;

  InputPixelType meanValue = ( brightValue + darkValue ) / 2.0;

  std::cout << "Mean Input Value = " << meanValue << std::endl;

  while( !citr.IsAtEnd() )
    {

    bool closeToSheet = ( vnl_math_abs( iitr.GetIndex()[2] - planeIndex[2] ) < 2 );

    if( iitr.Get() > meanValue || closeToSheet )
      {
      if( citr.Get() < 0.9 )
        {
        std::cerr << "Detection bright failure at index " << citr.GetIndex();
        std::cerr << " Input = " << iitr.Get();
        std::cerr << " Sheetness = " << citr.Get() << std::endl;
        failed = true;
        }
      }
    else
      {
      if( citr.Get() > 0.1 )
        {
        std::cerr << "Detection dark failure at index " << citr.GetIndex();
        std::cerr << " Input = " << iitr.Get();
        std::cerr << " Sheetness = " << citr.Get() << std::endl;
        failed = true;
        }
      }

    ++citr;
    ++iitr;
    }

  if( failed )
    {
    std::cerr << "The output values didn't match expected values" << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
