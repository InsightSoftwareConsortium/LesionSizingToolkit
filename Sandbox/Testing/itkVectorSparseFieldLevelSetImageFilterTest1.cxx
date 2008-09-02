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

namespace itk {

template <class TInputImage, class TOutputImage >
class HelperVectorSparseFieldLevelSetImageFilter
  : public VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage >
{
public:
  typedef HelperVectorSparseFieldLevelSetImageFilter    Self;
  typedef VectorSparseFieldLevelSetImageFilter<
    TInputImage, TOutputImage>                          Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(HelperVectorSparseFieldLevelSetImageFilter, 
     VectorSparseFieldLevelSetImageFilter);

  typedef typename Superclass::ValueType            ValueType;
  typedef typename Superclass::ScalarValueType      ScalarValueType;
  typedef typename Superclass::IndexType            IndexType;
  typedef typename Superclass::TimeStepType         TimeStepType;
  typedef typename Superclass::LayerType            LayerType;
  typedef typename Superclass::StatusType           StatusType;

  ValueType GetValueZero() const 
    {
    return this->Superclass::GetValueZero(); 
    }

  ValueType GetValueOne() const 
    {
    return this->Superclass::GetValueOne(); 
    }

  ScalarValueType GetScalarValueZero() const
    {
    return this->Superclass::GetScalarValueZero(); 
    }

  virtual ValueType CalculateUpdateValue(
    const IndexType &idx,
    const TimeStepType &dt,
    const ValueType &value,
    const ValueType &change)
  { 
  return this->Superclass::CalculateUpdateValue(idx,dt,value,change);
  }

  void ProcessOutsideList(LayerType *OutsideList, StatusType ChangeToStatus)
    {
    this->Superclass::ProcessOutsideList( OutsideList, ChangeToStatus );
    };

  void Initialize()
    {
    this->Superclass::Initialize();
    }

  void CopyInputToOutput()
    {
    this->Superclass::CopyInputToOutput();
    }
};

} // end namespace itk


int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 3;

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
  isoSurfaceValue[2] = 0.0;

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

 
  typedef itk::HelperVectorSparseFieldLevelSetImageFilter< 
    LevelSetImageType, LevelSetImageType >               HelperFilterType;

  HelperFilterType::Pointer helperFilter = HelperFilterType::New();

  HelperFilterType::ValueType   valueZero = helperFilter->GetValueZero();
  HelperFilterType::ValueType   valueOne  = helperFilter->GetValueOne();

  for(unsigned int k=0; k < NumberOfPhases; k++)
    {
    if( valueOne[k] != 1 )
      {
      std::cerr << "Error in GetValueOne()" << std::endl;
      return EXIT_FAILURE;
      }
    if( valueZero[k] != 0 )
      {
      std::cerr << "Error in GetValueZero()" << std::endl;
      return EXIT_FAILURE;
      }
    }

  FilterType::ScalarValueType scalarValueZero = helperFilter->GetScalarValueZero();
 
  if( scalarValueZero != 0 )
    {
    std::cerr << "Error in GetScalarValueZero()" << std::endl;
    return EXIT_FAILURE;
    }

  const HelperFilterType::TimeStepType  timeDelta = 0.1;

  HelperFilterType::IndexType       idx;
  HelperFilterType::ValueType       value;
  HelperFilterType::ValueType       change;
  
  value[0] = 13;
  value[1] = 17;
  value[2] = 19;

  change[0] = 2;
  change[1] = 3;
  change[2] = 4;

  HelperFilterType::ValueType updatedValue = 
    helperFilter->CalculateUpdateValue( idx, timeDelta, value, change );

  const double tolerance = 1e-5;

  for( unsigned int f = 0; f < NumberOfPhases; f++ )
    {
    double newValue = value[f] + timeDelta * change[f];
    if( vcl_abs( updatedValue[f] - newValue ) > tolerance )
      {
      std::cerr << "Error in CalculateUpdateValue() " << std::endl;
      return EXIT_FAILURE;
      }
    }

  filter->SetNumberOfLayers(0);
  
  try
    {
    filter->Update();
    std::cerr << "Error: Failure to catch an Expected Exception" << std::endl;
    return EXIT_FAILURE;
    }
  catch( itk::ExceptionObject & )
    {
    std::cout << "Catched EXPECTED Exception" << std::endl;
    }

   
  typedef HelperFilterType::LayerType  LayerType;

  LayerType::Pointer outsideList = LayerType::New();

  HelperFilterType::StatusType changeToStatus;

  helperFilter->SetInput( inputLevelSet );

  FunctionType::Pointer helperDifferenceFunction = FunctionType::New();

  helperFilter->SetDifferenceFunction( helperDifferenceFunction );

  helperFilter->CopyInputToOutput();
  helperFilter->Initialize();

  helperFilter->ProcessOutsideList( outsideList, changeToStatus );

}
