/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorLevelSetFunctionTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorLevelSetFunction.h"
#include "itkImage.h"
#include "itkVector.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 2;

  typedef itk::Vector< float, NumberOfPhases >           LevelSetPixelType;

  typedef itk::Image< LevelSetPixelType, Dimension >     LevelSetImageType;

  typedef itk::VectorLevelSetFunction< LevelSetImageType >  FunctionType;

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

  FunctionType::ScalarValueType curvatureWeight   = 10.0;
  FunctionType::ScalarValueType propagationWeight = 10.0;
  FunctionType::ScalarValueType advectionWeight   = 10.0;

  //
  // Test Set/GetCurvatureWeight()
  //
  // First set a trivial number
  function->SetCurvatureWeight( 1.0 );
  // Then set the real value
  function->SetCurvatureWeight( curvatureWeight );
  // Then check if the value was stored correctly
  if( function->GetCurvatureWeight() != curvatureWeight )
    {
    std::cerr << "Error in SetCurvatureWeight()/GetCurvatureWeight() " << std::endl;
    return EXIT_FAILURE;
    }


  //
  // Test Set/GetPropagationWeight()
  //
  // First set a trivial number
  function->SetPropagationWeight( 1.0 );
  // Then set the real value
  function->SetPropagationWeight( propagationWeight );
  // Then check if the value was stored correctly
  if( function->GetPropagationWeight() != propagationWeight )
    {
    std::cerr << "Error in SetPropagationWeight()/GetPropagationWeight() " << std::endl;
    return EXIT_FAILURE;
    }


  //
  // Test Set/GetAdvectionWeight()
  //
  // First set a trivial number
  function->SetAdvectionWeight( 1.0 );
  // Then set the real value
  function->SetAdvectionWeight( advectionWeight );
  // Then check if the value was stored correctly
  if( function->GetAdvectionWeight() != advectionWeight )
    {
    std::cerr << "Error in SetAdvectionWeight()/GetAdvectionWeight() " << std::endl;
    return EXIT_FAILURE;
    }


  // Exercise the Print method 
  function->Print( std::cout );

  typedef FunctionType::GlobalDataStruct    GlobalDataStruct;
  typedef FunctionType::VectorType          VectorType;

  GlobalDataStruct * gds = static_cast< GlobalDataStruct * >( function->GetGlobalDataPointer() );
   
  FunctionType::RadiusType radius;

  radius.Fill( 3 );

  FunctionType::NeighborhoodType neigborhood(radius, inputLevelSet, region);

  FunctionType::TimeStepType timeStep = function->ComputeGlobalTimeStep( gds );
  
  std::cout << "Time Step: " << timeStep << std::endl;
  
  FunctionType::FloatOffsetType offset;

  // Set the 2D components
  offset[0] = 0.0;
  offset[1] = 0.0;

  for( unsigned int component = 0; component < NumberOfPhases; component++ )
    {
    double update        = function->ComputeUpdate( neigborhood, gds, component, offset );
    VectorType advection = function->AdvectionField( neigborhood, offset, component, gds );
    double speed = function->PropagationSpeed( neigborhood, offset, component, gds );
    double curvature = function->CurvatureSpeed( neigborhood, offset, component, gds );
    double laplacian = function->LaplacianSmoothingSpeed( neigborhood, offset, component, gds );
    std::cout << component << " : " << update << " : " << advection << " : ";
    std::cout << speed << " : " << curvature <<  " : " << laplacian << std::endl;
    }

  // Now exercise the options of Minimal curvature.
  function->UseMinimalCurvatureOn();

  for( unsigned int component = 0; component < NumberOfPhases; component++ )
    {
    double update        = function->ComputeUpdate( neigborhood, gds, component, offset );
    VectorType advection = function->AdvectionField( neigborhood, offset, component, gds );
    double speed = function->PropagationSpeed( neigborhood, offset, component, gds );
    double curvature = function->CurvatureSpeed( neigborhood, offset, component, gds );
    double laplacian = function->LaplacianSmoothingSpeed( neigborhood, offset, component, gds );
    std::cout << component << " : " << update << " : " << advection << " : ";
    std::cout << speed << " : " << curvature <<  " : " << laplacian << std::endl;
    }

  //
  // Exercise combinations of all weights being zero or non-zero
  //
  for(unsigned int weightCombination = 0; weightCombination < 8; weightCombination++ )
    {
    curvatureWeight   = 10.0 * ( weightCombination & 1 );
    propagationWeight = 10.0 * ( weightCombination & 2 );
    advectionWeight   = 10.0 * ( weightCombination & 4 );
    function->SetCurvatureWeight( curvatureWeight );
    function->SetPropagationWeight( propagationWeight );
    function->SetAdvectionWeight( advectionWeight );
    for( unsigned int component = 0; component < NumberOfPhases; component++ )
      {
      function->ComputeUpdate( neigborhood, gds, component, offset );
      }
    }

  function->ReleaseGlobalDataPointer( gds );

  return EXIT_SUCCESS;
}
