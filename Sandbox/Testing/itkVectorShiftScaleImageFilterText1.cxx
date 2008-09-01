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

  typedef itk::Vector< float, NumberOfPhases >           LevelSetPixelType;
  typedef itk::Vector< float, NumberOfPhases >           FeaturePixelType;

  typedef itk::Image< LevelSetPixelType, Dimension >     LevelSetImageType;
  typedef itk::Image< FeaturePixelType, Dimension >      FeatureImageType;

  typedef itk::VectorShiftScaleImageFilter< 
    LevelSetImageType, LevelSetImageType >               FilterType;

  FilterType::Pointer filter = FilterType::New();

  std::cout << filter->GetNameOfClass() << std::endl;

  LevelSetImageType::Pointer inputLevelSet = LevelSetImageType::New();

  LevelSetImageType::RegionType region;
  LevelSetImageType::SizeType   size;
  LevelSetImageType::IndexType  start;

  start.Fill( 0 );
  size.Fill( 10 );

  region.SetSize( size );
  region.SetIndex( start );

  inputLevelSet->SetRegions( region );
  inputLevelSet->Allocate();

  LevelSetImageType::PixelType pixel1;

  pixel1[0] =  7;
  pixel1[1] =  9;
  pixel1[2] = 11;

  inputLevelSet->FillBuffer( pixel1 );

  filter->SetInput( inputLevelSet );

  FilterType::RealType scale;

  scale[0] = 2.0;
  scale[1] = 3.0;
  scale[2] = 4.0;

  filter->SetScale( scale );
  if( scale != filter->GetScale() )
    {
    std::cerr << "Set/GetScale() failed " << std::endl;
    return EXIT_FAILURE;
    }


  FilterType::RealType shift;

  shift[0] = 2.0;
  shift[1] = 3.0;
  shift[2] = 4.0;


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

  return EXIT_SUCCESS;
}
