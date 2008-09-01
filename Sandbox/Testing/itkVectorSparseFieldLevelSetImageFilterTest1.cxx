/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorSparseFieldLevelSetImageFilterTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorSparseFieldLevelSetImageFilter.h"
#include "itkVectorFiniteDifferenceFunction.h"
#include "itkImage.h"
#include "itkVector.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 2;

  typedef itk::Vector< float, NumberOfPhases >           LevelSetPixelType;
  typedef itk::Vector< float, NumberOfPhases >           FeaturePixelType;

  typedef itk::Image< LevelSetPixelType, Dimension >     LevelSetImageType;
  typedef itk::Image< FeaturePixelType, Dimension >      FeatureImageType;

  typedef itk::VectorSparseFieldLevelSetImageFilter< 
    LevelSetImageType, LevelSetImageType >               FilterType;

  typedef itk::VectorFiniteDifferenceFunction<LevelSetImageType>  FunctionType;

  FilterType::Pointer filter = FilterType::New();

  FunctionType::Pointer differenceFunction = FunctionType::New();

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

  filter->SetInput( inputLevelSet );

  const unsigned int numberOfLayers = 5;

  filter->SetNumberOfLayers( numberOfLayers );
  if( numberOfLayers != filter->GetNumberOfLayers() )
    {
    std::cerr << "Error: Set/Get NumberOfLayers " << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int numberOfIterations = 5;

  filter->SetNumberOfIterations( numberOfIterations );
  if( numberOfIterations != filter->GetNumberOfIterations() )
    {
    std::cerr << "Error: Set/Get NumberOfIterations " << std::endl;
    return EXIT_FAILURE;
    }

  filter->SetDifferenceFunction( differenceFunction );

  FilterType::ValueType isoSurfaceValue;
  isoSurfaceValue[0] = 0.0;
  isoSurfaceValue[1] = 0.0;

  filter->SetIsoSurfaceValue( isoSurfaceValue );
  if( isoSurfaceValue != filter->GetIsoSurfaceValue() )
    {
    std::cerr << "Error: Set/Get IsoSurfaceValue " << std::endl;
    return EXIT_FAILURE;
    }

  bool interpolateSurfaceLocation = true;
  filter->SetInterpolateSurfaceLocation( interpolateSurfaceLocation );
  if( interpolateSurfaceLocation != filter->GetInterpolateSurfaceLocation() )
    {
    std::cerr << "Error: Set/Get InterpolateSurfaceLocation " << std::endl;
    return EXIT_FAILURE;
    }

  interpolateSurfaceLocation = false;
  filter->SetInterpolateSurfaceLocation( interpolateSurfaceLocation );
  if( interpolateSurfaceLocation != filter->GetInterpolateSurfaceLocation() )
    {
    std::cerr << "Error: Set/Get InterpolateSurfaceLocation " << std::endl;
    return EXIT_FAILURE;
    }

  filter->InterpolateSurfaceLocationOff();
  filter->InterpolateSurfaceLocationOn();



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

  std::cout << "Number of elapsed iterations = " << filter->GetElapsedIterations() << std::endl;

  return EXIT_SUCCESS;
}
