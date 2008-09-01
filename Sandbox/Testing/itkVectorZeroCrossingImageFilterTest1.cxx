/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorZeroCrossingImageFilterTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorZeroCrossingImageFilter.h"
#include "itkImage.h"
#include "itkVector.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 3;

  typedef char  PixelComponentType;

  typedef itk::Vector< PixelComponentType, NumberOfPhases >     LevelSetPixelType;
  typedef itk::Image< LevelSetPixelType, Dimension >            LevelSetImageType;

  typedef itk::VectorZeroCrossingImageFilter< 
    LevelSetImageType, LevelSetImageType >                      FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter->GetNameOfClass() << std::endl;

  LevelSetImageType::Pointer inputLevelSet = LevelSetImageType::New();

  LevelSetImageType::RegionType region;
  LevelSetImageType::SizeType   size;
  LevelSetImageType::IndexType  start;

  start.Fill( 0 );
  size.Fill( 256 );

  region.SetSize( size );
  region.SetIndex( start );

  inputLevelSet->SetRegions( region );
  inputLevelSet->Allocate();

  PixelComponentType value;

  PixelComponentType minValue = itk::NumericTraits< PixelComponentType >::NonpositiveMin();

  value = minValue;

  LevelSetPixelType pixel;

  typedef itk::ImageRegionIterator< LevelSetImageType > IteratorType;

  IteratorType itr( inputLevelSet, region );

  itr.GoToBegin();

  while( !itr.IsAtEnd() )
    {
    pixel[0] =  value;
    pixel[1] =  value; 
    pixel[2] =  value;

    itr.Set( pixel );

    ++value;
    ++itr;
    }

  filter->SetInput( inputLevelSet );

  // Exercise the Print method 
  filter->Print( std::cout );

  FilterType::OutputImagePixelComponentType foregroundValue;

  foregroundValue = 235;

  filter->SetForegroundValue( foregroundValue );
  if( foregroundValue != filter->GetForegroundValue() )
    {
    std::cerr << "Set/GetForegroundValue() failed " << std::endl;
    return EXIT_FAILURE;
    }

  foregroundValue = 255;

  filter->SetForegroundValue( foregroundValue );
  if( foregroundValue != filter->GetForegroundValue() )
    {
    std::cerr << "Set/GetForegroundValue() failed " << std::endl;
    return EXIT_FAILURE;
    }

  FilterType::OutputImagePixelComponentType backgroundValue;

  backgroundValue = 35;

  filter->SetBackgroundValue( backgroundValue );
  if( backgroundValue != filter->GetBackgroundValue() )
    {
    std::cerr << "Set/GetBackgroundValue() failed " << std::endl;
    return EXIT_FAILURE;
    }

  backgroundValue = 0;

  filter->SetBackgroundValue( backgroundValue );
  if( backgroundValue != filter->GetBackgroundValue() )
    {
    std::cerr << "Set/GetBackgroundValue() failed " << std::endl;
    return EXIT_FAILURE;
    }



  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }

  return EXIT_SUCCESS;
}
