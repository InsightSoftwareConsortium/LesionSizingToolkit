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
  const unsigned int NumberOfComponents = 2;

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

  // Set curvature weights.
  FunctionType::MatrixValueType curvatureWeights( NumberOfPhases, NumberOfPhases );
  curvatureWeights.SetIdentity();
  curvatureWeights *= 10.0;

  // Set Propagation weights
  FunctionType::MatrixValueType propagationWeights( NumberOfPhases, NumberOfComponents );
  propagationWeights.SetIdentity();
  propagationWeights *= 10.0;

  // Set Propagation weights
  FunctionType::MatrixValueType advectionWeights( NumberOfPhases, NumberOfComponents );
  advectionWeights.SetIdentity();
  advectionWeights *= 10.0;
  
  //
  // Test Set/GetCurvatureWeight()
  function->SetCurvatureWeights( curvatureWeights );

  // Then check if the value was stored correctly
  if( function->GetCurvatureWeights() != curvatureWeights )
    {
    std::cerr << "Error in SetCurvatureWeight()/GetCurvatureWeight() " << std::endl;
    return EXIT_FAILURE;
    }


  //
  // Test Set/GetPropagationWeight()
  //
  function->SetPropagationWeights( propagationWeights );
  // Then check if the value was stored correctly
  if( function->GetPropagationWeights() != propagationWeights )
    {
    std::cerr << "Error in SetPropagationWeight()/GetPropagationWeight() " << std::endl;
    return EXIT_FAILURE;
    }


  //
  // Test Set/GetAdvectionWeight()
  //
  function->SetAdvectionWeights( advectionWeights );
  // Then check if the value was stored correctly
  if( function->GetAdvectionWeights() != advectionWeights )
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

  for( unsigned int phase = 0; phase < NumberOfPhases; phase++ )
    {
    double update        = function->ComputeUpdate( neigborhood, gds, phase, offset );
    double advection = function->ComputeAdvectionTerms( neigborhood, offset, phase, gds );
    double speed = function->ComputePropagationTerms( neigborhood, offset, phase, gds );
    double curvature = function->ComputeCurvatureTerms( neigborhood, offset, phase, gds );
    double laplacian = function->ComputeLaplacianTerms( neigborhood, offset, phase, gds );
    std::cout << phase << " : " << update << " : " << advection << " : ";
    std::cout << speed << " : " << curvature <<  " : " << laplacian << std::endl;
    }

  // Now exercise the options of Minimal curvature.
  function->UseMinimalCurvatureOn();

  for( unsigned int phase = 0; phase < NumberOfPhases; phase++ )
    {
    double update        = function->ComputeUpdate( neigborhood, gds, phase, offset );
    double advection = function->ComputeAdvectionTerms( neigborhood, offset, phase, gds );
    double speed = function->ComputePropagationTerms( neigborhood, offset, phase, gds );
    double curvature = function->ComputeCurvatureTerms( neigborhood, offset, phase, gds );
    double laplacian = function->ComputeLaplacianTerms( neigborhood, offset, phase, gds );
    std::cout << phase << " : " << update << " : " << advection << " : ";
    std::cout << speed << " : " << curvature <<  " : " << laplacian << std::endl;
    }

  function->ReleaseGlobalDataPointer( gds );

  return EXIT_SUCCESS;
}

