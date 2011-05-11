/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    LaplacianRecursiveGaussianImageFilter.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkIncludeRequiredIOFactories.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLaplacianRecursiveGaussianImageFilter.h"


int main( int argc, char * argv[] )
{

  RegisterRequiredFactories();

  if( argc < 4 ) 
    { 
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile  sigma " << std::endl;
    return EXIT_FAILURE;
    }

  typedef    float    InputPixelType;
  typedef    float    OutputPixelType;

  const unsigned int Dimension = 3;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType >  ReaderType;

  typedef itk::LaplacianRecursiveGaussianImageFilter<
                        InputImageType, OutputImageType >  FilterType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  FilterType::Pointer laplacian = FilterType::New();

  laplacian->SetNormalizeAcrossScale( false );

  laplacian->SetInput( reader->GetOutput() );

  const double sigma = atof( argv[3] );

  laplacian->SetSigma( sigma );

  try
    {
    laplacian->Update();
    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput( laplacian->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return EXIT_SUCCESS;
}

