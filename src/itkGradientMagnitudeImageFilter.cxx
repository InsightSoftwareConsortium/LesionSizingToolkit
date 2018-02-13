/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    GradientMagnitudeImageFilter.cxx
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

#ifdef __BORLANDC__
#define ITK_LEAN_AND_MEAN
#endif

#include "itkIncludeRequiredIOFactories.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkGradientMagnitudeImageFilter.h"

int main( int argc, char * argv[] )
{

  RegisterRequiredFactories();

  if( argc < 3 ) 
    { 
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
    }

  
  using InputPixelType = float;
  using OutputPixelType = float;

  const unsigned int  Dimension = 3;

  using InputImageType = itk::Image< InputPixelType,  Dimension >;
  using OutputImageType = itk::Image< OutputPixelType, Dimension >;


  using ReaderType = itk::ImageFileReader< InputImageType >;

  using FilterType = itk::GradientMagnitudeImageFilter< InputImageType, OutputImageType >;


  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );

  using WriterType = itk::ImageFileWriter< OutputImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( argv[2] );
 
  writer->SetInput( filter->GetOutput() );

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "ExceptionObject caught !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}
