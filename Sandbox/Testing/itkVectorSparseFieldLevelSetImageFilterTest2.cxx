/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorSparseFieldLevelSetImageFilterTest2.cxx

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
#include "itkImageRegionIteratorWithIndex.h"

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

  virtual ScalarValueType CalculateUpdateValue(
    const IndexType &idx,
    const TimeStepType &dt,
    const ScalarValueType &value,
    const ScalarValueType &change)
  { 
  return this->Superclass::CalculateUpdateValue(idx,dt,value,change);
  }

  void ProcessOutsideList(LayerType *OutsideList, StatusType ChangeToStatus, unsigned int component)
    {
    this->Superclass::ProcessOutsideList( OutsideList, ChangeToStatus, component );
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

  LevelSetImageType::SpacingType    spacing;
  LevelSetImageType::PointType      origin;

  start.Fill( 0 );
  size.Fill( 100 );

  spacing.Fill(  2.0 );  // Purposely not using 1.0; better for testing
  origin.Fill( 150.0 );  // Purposely not using 0.0; better for testing

  region.SetSize( size );
  region.SetIndex( start );

  inputLevelSet->SetRegions( region );
  inputLevelSet->Allocate();

  filter->SetInput( inputLevelSet );

  FeatureImageType::Pointer featureImage = FeatureImageType::New();
  featureImage->SetRegions( region );
  featureImage->Allocate();


  //
  // Initializing the Feature image
  //
  typedef itk::ImageRegionIteratorWithIndex< FeatureImageType > FeatureIteratorType;

  FeatureImageType::PointType    point;
  FeatureImageType::PointType    center1;

  center1[0] = size[0] * spacing[0] / 2.0 + origin[0];
  center1[1] = size[1] * spacing[1] / 2.0 + origin[1];

  const double radius1 = ( spacing[0] * size[0] / 2.0 ) * 0.95; // 95% of half the image
  const double squaredRadius1 = radius1 * radius1;

  const float ZeroValue = 0.0;
  const float UnitValue = 1.0;

  FeaturePixelType unitVector;
  FeaturePixelType zeroVector;

  zeroVector.Fill( ZeroValue );
  unitVector.Fill( UnitValue );

  FeatureIteratorType ftr( featureImage, region );

  ftr.GoToBegin();

  while( !ftr.IsAtEnd() )
    {
    featureImage->TransformIndexToPhysicalPoint( ftr.GetIndex(), point );
    if( point.SquaredEuclideanDistanceTo( center1 ) > squaredRadius1 )
      {
      ftr.Set( zeroVector );
      }
    else
      {
      ftr.Set( unitVector );
      }
    ++ftr;
    }

  //
  // Initializing the Feature image
  //
  typedef itk::ImageRegionIteratorWithIndex< LevelSetImageType > LevelSetIteratorType;

  LevelSetImageType::PointType  center2;

  center2[0] = size[0] * spacing[0] * 0.25 + origin[0];  // circle on the left side of the image
  center2[1] = size[1] * spacing[1] * 0.50 + origin[1];

  const double radius2 = ( spacing[0] * size[0] / 4.0 ) * 0.95; // 95% of a quarter of the image
  const double squaredRadius2 = radius2 * radius2;

  LevelSetImageType::PointType  center3;

  center3[0] = size[0] * spacing[0] * 0.75 + origin[0];  // circle on the left side of the image
  center3[1] = size[1] * spacing[1] * 0.50 + origin[1];

  const double radius3 = ( spacing[0] * size[0] / 4.0 ) * 0.95; // 95% of a quarter of the image
  const double squaredRadius3 = radius3 * radius3;

  LevelSetIteratorType ltr( inputLevelSet, region );

  ltr.GoToBegin();

  LevelSetPixelType pixelVector;

  while( !ltr.IsAtEnd() )
    {
    inputLevelSet->TransformIndexToPhysicalPoint( ltr.GetIndex(), point );

    if( point.SquaredEuclideanDistanceTo( center2 ) > squaredRadius2 )
      {
      pixelVector[0] = ZeroValue;
      }
    else
      {
      pixelVector[0] = UnitValue;
      }

    if( point.SquaredEuclideanDistanceTo( center3 ) > squaredRadius3 )
      {
      pixelVector[1] = ZeroValue;
      }
    else
      {
      pixelVector[1] = UnitValue;
      }

    ltr.Set( pixelVector );
    ++ltr;
    }

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

  HelperFilterType::ScalarValueType updatedValue; 

  const double tolerance = 1e-5;

  for( unsigned int f = 0; f < NumberOfPhases; f++ )
    {
    updatedValue = helperFilter->CalculateUpdateValue( idx, timeDelta, value[f], change[f] );
    double newValue = value[f] + timeDelta * change[f];
    if( vcl_abs( updatedValue - newValue ) > tolerance )
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

   
  typedef HelperFilterType::LayerType      LayerType;
  typedef HelperFilterType::LayerNodeType  LayerNodeType;

  HelperFilterType::IndexType index1;
  HelperFilterType::IndexType index2;
  HelperFilterType::IndexType index3;
  HelperFilterType::IndexType index4;
  HelperFilterType::IndexType index5;

  index1[0] = 5;  index1[1] = 2;
  index2[0] = 5;  index2[1] = 3;
  index3[0] = 5;  index3[1] = 4;
  index4[0] = 5;  index4[1] = 5;
  index5[0] = 5;  index5[1] = 6;

  LayerNodeType node1;
  LayerNodeType node2;
  LayerNodeType node3;
  LayerNodeType node4;
  LayerNodeType node5;

  node1.m_Value  = index1;
  node1.Previous = NULL;
  node1.Next     = &node2;

  node2.m_Value  = index2;
  node2.Previous = &node1;
  node2.Next     = &node3;

  node3.m_Value  = index3;
  node3.Previous = &node2;
  node3.Next     = &node4;

  node4.m_Value  = index4;
  node4.Previous = &node3;
  node4.Next     = &node5;

  node5.m_Value  = index5;
  node5.Previous = &node4;
  node5.Next     = NULL;

  LayerType::Pointer outsideList = LayerType::New();

  outsideList->PushFront( &node1 );
  outsideList->PushFront( &node2 );
  outsideList->PushFront( &node3 );
  outsideList->PushFront( &node4 );
  outsideList->PushFront( &node5 );

  HelperFilterType::StatusType changeToStatus = 0;;

  helperFilter->SetInput( inputLevelSet );

  FunctionType::Pointer helperDifferenceFunction = FunctionType::New();

  helperFilter->SetDifferenceFunction( helperDifferenceFunction );

  helperFilter->CopyInputToOutput();
  helperFilter->Initialize();

  for(unsigned int component = 0; component < NumberOfPhases; component++ )
    {
    helperFilter->ProcessOutsideList( outsideList, changeToStatus, component );
    }

  try
    {
    helperFilter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << excp << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

