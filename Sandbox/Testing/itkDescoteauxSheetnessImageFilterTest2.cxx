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

int main( int argc, char * argv [] )
{

  if( argc < 2 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " outputImage [sigma sheetness bloobiness noise]" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 3;

  typedef signed short    InputPixelType;
  typedef float           OutputPixelType;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

  typedef itk::HessianRecursiveGaussianImageFilter< InputImageType >  HessianFilterType;
  typedef HessianFilterType::OutputImageType                          HessianImageType;
  typedef HessianImageType::PixelType                                 HessianPixelType;

  typedef  itk::FixedArray< double, HessianPixelType::Dimension >     EigenValueArrayType;
  typedef  itk::Image< EigenValueArrayType, Dimension >               EigenValueImageType;

  typedef  itk::SymmetricEigenAnalysisImageFilter< 
    HessianImageType, EigenValueImageType >     EigenAnalysisFilterType;

  typedef itk::ImageFileWriter< OutputImageType >    WriterType;

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

  inputImage->FillBuffer( 0 );

  typedef itk::ImageRegionIterator< InputImageType > IteratorType;

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

  //
  // Populate the inputImage with a bright XY plane  
  //
  while( !itr.IsAtEnd() )
    {
    itr.Set( 500 );
    ++itr;
    }


  typedef itk::DescoteauxSheetnessImageFilter< EigenValueImageType, OutputImageType >   FilterType;

  FilterType::Pointer sheetnessFilter = FilterType::New();
  
  hessian->SetInput( inputImage );
  eigen->SetInput( hessian->GetOutput() );
  sheetnessFilter->SetInput( eigen->GetOutput() );

  if( argc > 3 )
    {
    hessian->SetSigma( atof( argv[2] ) );
    }

  if( argc > 4 )
    {
    sheetnessFilter->SetSheetnessNormalization( atof( argv[3] ) );
    }

  if( argc > 5 )
    {
    sheetnessFilter->SetBloobinessNormalization( atof( argv[4] ) );
    }

  if( argc > 6 )
    {
    sheetnessFilter->SetNoiseNormalization( atof( argv[5] ) );
    }

  sheetnessFilter->SetDetectBrightSheets(true);

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

  return EXIT_SUCCESS;
}
