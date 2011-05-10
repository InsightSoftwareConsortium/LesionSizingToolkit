/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVotingBinaryHoleFillFloodingImageFilterTest1.cxx
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


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkVotingBinaryHoleFillFloodingImageFilter.h"

int itkVotingBinaryHoleFillFloodingImageFilterTest1( int argc, char * argv[] )
{
  if( argc < 7 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile outputImageFile inputThreshold radius majority maxNumberOfIterations" << std::endl;
    return EXIT_FAILURE;
    }


  typedef signed short   InputPixelType;
  typedef unsigned char  OutputPixelType;

  const unsigned int Dimension = 3;

  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;

  typedef itk::ImageFileReader< InputImageType >   ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( argv[1] );
  writer->SetFileName( argv[2] );

  typedef itk::BinaryThresholdImageFilter< 
    InputImageType, OutputImageType > ThresholderType;

  ThresholderType::Pointer thresholder = ThresholderType::New();

  thresholder->SetLowerThreshold( atoi( argv[3] ) );
  thresholder->SetUpperThreshold( itk::NumericTraits< InputPixelType >::max() );

  thresholder->SetOutsideValue(  0 );
  thresholder->SetInsideValue( 255 );
 
  typedef itk::VotingBinaryHoleFillFloodingImageFilter< 
    OutputImageType, OutputImageType >  FilterType;

  FilterType::Pointer filter = FilterType::New();

  const unsigned int radius = atoi( argv[4] );

  OutputImageType::SizeType indexRadius;
  
  indexRadius[0] = radius; // radius along x
  indexRadius[1] = radius; // radius along y
  indexRadius[2] = radius; // radius along z

  filter->SetRadius( indexRadius );

  filter->SetBackgroundValue(   0 );
  filter->SetForegroundValue( 255 );

  const unsigned int majorityThreshold = atoi( argv[5] );

  filter->SetMajorityThreshold( majorityThreshold  );

  const unsigned int maximumNumberOfIterations = atoi( argv[6] );

  filter->SetMaximumNumberOfIterations( maximumNumberOfIterations  );

  if( filter->GetMaximumNumberOfIterations() != maximumNumberOfIterations )
    {
    std::cerr << "Error in Set/GetMaximumNumberOfIterations() " << std::endl;
    return EXIT_FAILURE;
    }

  thresholder->SetInput( reader->GetOutput() );
  filter->SetInput( thresholder->GetOutput() );
  writer->SetInput( filter->GetOutput() );
  writer->Update();

  std::cout << "Class name " << filter->GetNameOfClass() << std::endl;
  
  filter->Print( std::cout );

  std::cout << "Iteration used = " << filter->GetCurrentIterationNumber()     << std::endl;
  std::cout << "Pixels changes = " << filter->GetTotalNumberOfPixelsChanged() << std::endl;

  return EXIT_SUCCESS;
}

