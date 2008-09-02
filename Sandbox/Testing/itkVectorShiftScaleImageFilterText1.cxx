/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorShiftScaleImageFilterText1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorShiftScaleImageFilter.h"
#include "itkImage.h"
#include "itkVector.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 3;

  typedef char  PixelComponentType;

  typedef itk::Vector< PixelComponentType, NumberOfPhases >     LevelSetPixelType;
  typedef itk::Image< LevelSetPixelType, Dimension >            LevelSetImageType;

  typedef itk::VectorShiftScaleImageFilter< 
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

  FilterType::RealType scale;

  scale[0] = 1.0;
  scale[1] = 2.0;
  scale[2] = 3.0;

  filter->SetScale( scale );
  if( scale != filter->GetScale() )
    {
    std::cerr << "Set/GetScale() failed " << std::endl;
    return EXIT_FAILURE;
    }


  FilterType::RealType shift;

  shift[0] = 0.0;
  shift[1] = 1.0;
  shift[2] = 2.0;


  filter->SetShift( shift );
  if( shift != filter->GetShift() )
    {
    std::cerr << "Set/GetShift() failed " << std::endl;
    return EXIT_FAILURE;
    }

  // Exercise the Print method 
  filter->Print( std::cout );

  try
    {
    filter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    }

  
  LevelSetImageType::Pointer outputImage = filter->GetOutput();

  IteratorType otr( outputImage, outputImage->GetBufferedRegion() );

  itr.GoToBegin();
  otr.GoToBegin();

  typedef itk::NumericTraits< PixelComponentType >::RealType PixelComponentRealType;

  const double tolerance = 1e-6;

  unsigned long overflowCount  = 0L;
  unsigned long underflowCount = 0L;
 
  while( !itr.IsAtEnd() )
    {
    for( unsigned int i=0; i < NumberOfPhases; i++ )
      {
      PixelComponentRealType componentValue = itr.Get()[i];
      componentValue += shift[i];
      componentValue *= scale[i];
      if( componentValue < itk::NumericTraits< PixelComponentType >::NonpositiveMin() )
        {
        componentValue = itk::NumericTraits< PixelComponentType >::NonpositiveMin();
        underflowCount++;
        }
      if( componentValue > itk::NumericTraits< PixelComponentType >::max() )
        {
        componentValue = itk::NumericTraits< PixelComponentType >::max();
        overflowCount++;
        }
      if( vcl_abs( componentValue - otr.Get()[i] ) > tolerance )
        {
        std::cerr << "Error in pixel = " << itr.GetIndex() << std::endl;
        return EXIT_FAILURE;
        }
      } 
    ++itr;
    ++otr;
    }

  if( overflowCount != filter->GetOverflowCount() )
    {
    std::cerr << "Overflow count error = " << std::endl;
    std::cerr << "expected = "  << overflowCount << std::endl;
    std::cerr << "but received = "  << filter->GetOverflowCount()  << std::endl;
    }

  if( underflowCount != filter->GetUnderflowCount() )
    {
    std::cerr << "Underflow count error = " << std::endl;
    std::cerr << "expected = "  << underflowCount << std::endl;
    std::cerr << "but received = "  << filter->GetUnderflowCount()  << std::endl;
    }

  return EXIT_SUCCESS;
}
