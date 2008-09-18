/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorSegmentationLevelSetFunctionTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorSegmentationLevelSetFunction.h"
#include "itkImage.h"
#include "itkVector.h"

namespace itk {

template <class TInputImage, class TFeatureImage>
class HelperVectorSegmentationLevelSetFunction
  : public VectorSegmentationLevelSetFunction<TInputImage, TFeatureImage>
{
public:
  typedef HelperVectorSegmentationLevelSetFunction    Self;
  typedef VectorSegmentationLevelSetFunction<
    TInputImage, TFeatureImage >                      Superclass;
  typedef SmartPointer<Self>                          Pointer;
  typedef SmartPointer<const Self>                    ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(HelperVectorSegmentationLevelSetFunction, 
     VectorSegmentationLevelSetFunction);

  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::NeighborhoodType       NeighborhoodType;
  typedef typename Superclass::FloatOffsetType        FloatOffsetType;
  typedef typename Superclass::ScalarValueType        ScalarValueType;
  typedef typename Superclass::VectorType             VectorType;
  typedef typename Superclass::GlobalDataStruct       GlobalDataStruct;

  virtual ScalarValueType  ComputeUpdate(
    const NeighborhoodType &neighborhood, void *globalData,
    unsigned int component,
    const FloatOffsetType &offset = FloatOffsetType(0.0))
      {
      ScalarValueType output;
      GlobalDataStruct * data = static_cast<GlobalDataStruct *>( globalData );
      ScalarValueType propagationSpeed = this->PropagationSpeed( neighborhood, offset, component, data );
      ScalarValueType curvatureSpeed   = this->CurvatureSpeed(   neighborhood, offset, component, data );
      VectorType      advectionField   = this->AdvectionField(   neighborhood, offset, component, data );
      ScalarValueType laplacian        = this->LaplacianSmoothingSpeed( neighborhood, offset, component, data );

      std::cout << propagationSpeed << " " << curvatureSpeed << " " << advectionField << " " << laplacian << std::endl;

      output = propagationSpeed + curvatureSpeed + laplacian;
      return output;
      }
};

}


int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 2;

  typedef itk::Vector< float, NumberOfPhases >           LevelSetPixelType;
  typedef itk::Vector< float, NumberOfPhases >           FeaturePixelType;

  typedef itk::Image< LevelSetPixelType, Dimension >     LevelSetImageType;
  typedef itk::Image< FeaturePixelType, Dimension >      FeatureImageType;

  typedef itk::HelperVectorSegmentationLevelSetFunction< LevelSetImageType, FeatureImageType >  FunctionType;

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

  FeatureImageType::Pointer featureImage = FeatureImageType::New();

  featureImage->SetRegions( region );
  featureImage->Allocate();

  function->SetFeatureImage( featureImage );

  function->AllocateSpeedImage();
  function->AllocateAdvectionImage();

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

  offset[0] = 0.0;
  offset[1] = 0.0;

  FunctionType::ConstPointer constFunction = function.GetPointer();

  for( unsigned int component = 0; component < NumberOfPhases; component++ )
    {
    double update = function->ComputeUpdate( neigborhood, gds, component, offset );
    std::cout << component << " : " << update << std::endl;
    }

  function->ReleaseGlobalDataPointer( gds );

  return EXIT_SUCCESS;
}
