/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorSegmentationLevelSetImageFilterTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorSegmentationLevelSetImageFilter.h"
#include "itkVectorFiniteDifferenceFunction.h"
#include "itkImage.h"
#include "itkVector.h"

namespace itk {

template <class TInputImage, class TFeatureImage, class TOutputImage >
class HelperVectorSegmentationLevelSetImageFilter
  : public VectorSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputImage >
{
public:
  typedef HelperVectorSegmentationLevelSetImageFilter    Self;
  typedef VectorSegmentationLevelSetImageFilter<
    TInputImage, TFeatureImage, TOutputImage>            Superclass;
  typedef SmartPointer<Self>                             Pointer;
  typedef SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(HelperVectorSegmentationLevelSetImageFilter, 
     VectorSegmentationLevelSetImageFilter);
};

} // end namespace itk


int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 2;

  typedef itk::Vector< float, NumberOfPhases >           LevelSetPixelType;
  typedef itk::Vector< float, NumberOfPhases >           FeaturePixelType;

  typedef itk::Image< LevelSetPixelType, Dimension >     LevelSetImageType;
  typedef itk::Image< FeaturePixelType, Dimension >      FeatureImageType;

  typedef itk::HelperVectorSegmentationLevelSetImageFilter< 
    LevelSetImageType, FeatureImageType, LevelSetImageType >      FilterType;

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

  filter->SetNumberOfIterations( 5 );

  filter->SetDifferenceFunction( differenceFunction );

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
