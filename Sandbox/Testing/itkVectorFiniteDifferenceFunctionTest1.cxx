/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorFiniteDifferenceFunctionTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

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

  typedef itk::VectorFiniteDifferenceFunction< LevelSetImageType >  FunctionType;

  FunctionType::Pointer function = FunctionType::New();

  std::cout << function->GetNameOfClass() << std::endl;

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

  // Exercise the Print method 
  function->Print( std::cout );

  typedef FunctionType::GlobalDataStruct GlobalDataStruct;

  GlobalDataStruct * gds = static_cast< GlobalDataStruct * >( function->GetGlobalDataPointer() );
   
  FunctionType::RadiusType radius;

  radius.Fill( 3 );

  FunctionType::NeighborhoodType neigborhood(radius, inputLevelSet, region);

  FunctionType::TimeStepType timeStep = function->ComputeGlobalTimeStep( gds );
  
  std::cout << "Time Step: " << timeStep << std::endl;
  
  FunctionType::FloatOffsetType offset;

  offset[0] = 0.0;
  offset[1] = 0.0;

  for( unsigned int component = 0; component < NumberOfPhases; component++ )
    {
    double update = function->ComputeUpdate( neigborhood, gds, component, offset );
    std::cout << "Component : " << component << "  update = " << update << std::endl;
    }

  function->ReleaseGlobalDataPointer( gds );

  return EXIT_SUCCESS;
}
