/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ImageReadRegionOfInterestWrite.cxx
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
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkImage.h"

#include <string>


int main( int argc, char ** argv )
{

  RegisterRequiredFactories();

  // Verify the number of parameters in the command line
  if( argc < 9 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " startX startY startZ sizeX sizeY sizeZ" << std::endl;
    return EXIT_FAILURE;
    }


  using InputPixelType = signed short;
  using OutputPixelType = signed short;
  constexpr unsigned int Dimension = 3;

  using InputImageType = itk::Image< InputPixelType,  Dimension >;
  using OutputImageType = itk::Image< OutputPixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< InputImageType  >;
  using WriterType = itk::ImageFileWriter< OutputImageType >;

  using FilterType = itk::RegionOfInterestImageFilter< InputImageType, 
                                            OutputImageType >;

  FilterType::Pointer filter = FilterType::New();

  OutputImageType::IndexType start;
  start[0] = std::stoi( argv[3] );
  start[1] = std::atoi( argv[4] );
  start[2] = std::atoi( argv[5] );

  OutputImageType::SizeType size;
  size[0] = std::atoi( argv[6] );
  size[1] = std::atoi( argv[7] );
  size[2] = std::atoi( argv[8] );


  OutputImageType::RegionType desiredRegion;
  desiredRegion.SetSize(  size  );
  desiredRegion.SetIndex( start );

  filter->SetRegionOfInterest( desiredRegion );

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName( inputFilename  );
  writer->SetFileName( outputFilename );

  filter->SetInput( reader->GetOutput() );
  writer->SetInput( filter->GetOutput() );
  writer->UseCompressionOn();

  try 
    { 
    writer->Update(); 
    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cerr << "ExceptionObject caught !" << std::endl; 
    std::cerr << err << std::endl; 
    return EXIT_FAILURE;
    } 

  return EXIT_SUCCESS;
}
