/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLocalStructureImageFilterTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLocalStructureImageFilter.h"
#include "itkHessianRecursiveGaussianImageFilter.h"
#include "itkSymmetricEigenAnalysisImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

int itkLocalStructureImageFilterTest1( int argc, char * argv [] )
{

  if( argc < 3 )
    {
    std::cerr << "Missing Arguments" << std::endl;
    std::cerr << argv[0] << " inputImage outputImage [sigma alpha gamma]" << std::endl;
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

  using ReaderType = itk::ImageFileReader< InputImageType >;
  using WriterType = itk::ImageFileWriter< OutputImageType >;

  HessianFilterType::Pointer hessian = HessianFilterType::New();
  EigenAnalysisFilterType::Pointer eigen = EigenAnalysisFilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  using LocalStructureFilterType = itk::LocalStructureImageFilter< EigenValueImageType, OutputImageType >;

  LocalStructureFilterType::Pointer localStructure = LocalStructureFilterType::New();
  
  hessian->SetInput( reader->GetOutput() );
  eigen->SetInput( hessian->GetOutput() );
  localStructure->SetInput( eigen->GetOutput() );

  if( argc > 3 )
    {
    hessian->SetSigma( atof( argv[3] ) );
    }

  if( argc > 4 )
    {
    localStructure->SetAlpha( atof( argv[4] ) );
    }

  if( argc > 5 )
    {
    localStructure->SetGamma( atof( argv[5] ) );
    }

  eigen->SetDimension( Dimension );

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( localStructure->GetOutput() );

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "Name Of Class = " << localStructure->GetNameOfClass() << std::endl;

  localStructure->Print( std::cout );

  return EXIT_SUCCESS;
}
