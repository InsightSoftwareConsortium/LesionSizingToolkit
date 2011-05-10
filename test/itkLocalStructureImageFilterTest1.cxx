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

  typedef itk::ImageFileReader< InputImageType >     ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >    WriterType;

  HessianFilterType::Pointer hessian = HessianFilterType::New();
  EigenAnalysisFilterType::Pointer eigen = EigenAnalysisFilterType::New();
  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  typedef itk::LocalStructureImageFilter< EigenValueImageType, OutputImageType >   LocalStructureFilterType;

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
