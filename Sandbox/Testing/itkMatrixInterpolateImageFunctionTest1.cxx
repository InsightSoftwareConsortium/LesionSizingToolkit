/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkMatrixInterpolateImageFunctionTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkMatrixInterpolateImageFunction.h"
#include "itkImage.h"
#include "itkMatrix.h"

namespace itk
{
template <class TInputImage, class TCoordRep = double>
class MatrixInterpolateImageFunctionTestHelper : 
  public MatrixInterpolateImageFunction<TInputImage,TCoordRep> 
{
public:
  /** Standard class typedefs. */
  typedef MatrixInterpolateImageFunctionTestHelper Self;
  typedef MatrixInterpolateImageFunction<TInputImage,TCoordRep> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Run-time type information (and related methods). */
  itkTypeMacro(MatrixInterpolateImageFunctionTestHelper, 
    MatrixInterpolateImageFunction);

  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);
  itkStaticConstMacro(RowDimensions, unsigned int,Superclass::RowDimensions);
  itkStaticConstMacro(ColumnDimensions, unsigned int,Superclass::ColumnDimensions);

  typedef typename Superclass::PixelType              PixelType;
  typedef typename Superclass::OutputType             OutputType;
  typedef typename Superclass::IndexType              IndexType;
  typedef typename Superclass::ContinuousIndexType    ContinuousIndexType;

  virtual OutputType EvaluateAtContinuousIndex( 
    const ContinuousIndexType & cindex ) const
    {
    IndexType index;

    for(unsigned int dim = 0; dim < ImageDimension; dim++ )
      {
      index[dim] = (long) vcl_floor(cindex[dim] );
      }

    PixelType pixelValue = this->m_Image->GetPixel( index );

    OutputType outputValue;

    for(unsigned int row=0; row < RowDimensions; row++ )
      {
      for(unsigned int col=0; col < ColumnDimensions; col++ )
        {
        outputValue(row,col) = pixelValue(row,col);
        }
      }
    return outputValue;
    }
};


}

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 3;

  typedef itk::Matrix< char,   NumberOfPhases, Dimension >   MatrixType1;
  typedef itk::Matrix< float,  NumberOfPhases, Dimension >   MatrixType2;
  typedef itk::Matrix< double, NumberOfPhases, Dimension >   MatrixType3;

  typedef itk::Image< MatrixType1, Dimension >   ImageType1;
  typedef itk::Image< MatrixType2, Dimension >   ImageType2;
  typedef itk::Image< MatrixType3, Dimension >   ImageType3;

  typedef itk::MatrixInterpolateImageFunctionTestHelper< ImageType1 >  InterpolatorType1;
  typedef itk::MatrixInterpolateImageFunctionTestHelper< ImageType2 >  InterpolatorType2;
  typedef itk::MatrixInterpolateImageFunctionTestHelper< ImageType3 >  InterpolatorType3;

  InterpolatorType1::Pointer interpolator1 = InterpolatorType1::New();
  InterpolatorType2::Pointer interpolator2 = InterpolatorType2::New();
  InterpolatorType3::Pointer interpolator3 = InterpolatorType3::New();

  ImageType1::RegionType region;
  ImageType1::SizeType   size;
  ImageType1::IndexType  start;

  start.Fill( 0 );
  size.Fill( 10 );

  region.SetIndex( start );
  region.SetSize( size );

  ImageType1::Pointer image1 = ImageType1::New();
  image1->SetRegions( region );
  image1->Allocate();

  ImageType1::PixelType pixel1;

  pixel1(0,0) =  7;
  pixel1(0,1) =  9;
  pixel1(1,0) = 11;
  pixel1(1,1) = 13;

  image1->FillBuffer( pixel1 );

  interpolator1->SetInputImage( image1 );

  InterpolatorType1::ContinuousIndexType cindex;

  cindex[0] = 4.5;
  cindex[1] = 4.5;

  InterpolatorType1::OutputType value1 = interpolator1->EvaluateAtContinuousIndex( cindex );

  for( unsigned row=0; row < NumberOfPhases; row++ )
    {
    for( unsigned col=0; col < Dimension; col++ )
      {
      if( value1(row,col) != pixel1(row,col) )
        {
        std::cerr << "Error in EvaluateAtContinuousIndex()" << std::endl;
        return EXIT_FAILURE;
        }
      }
    } 

  InterpolatorType1::PointType point1;

  point1[0] = 4.5;
  point1[1] = 4.5;

  value1 = interpolator1->Evaluate( point1 );

  for( unsigned row=0; row < NumberOfPhases; row++ )
    {
    for( unsigned col=0; col < Dimension; col++ )
      {
      if( value1(row,col) != pixel1(row,col) )
        {
        std::cerr << "Error in EvaluateAtContinuousIndex()" << std::endl;
        return EXIT_FAILURE;
        }
      }
    } 

  InterpolatorType1::IndexType index;

  index[0] = 4;
  index[1] = 4;

  value1 = interpolator1->EvaluateAtIndex( index );

  for( unsigned row=0; row < NumberOfPhases; row++ )
    {
    for( unsigned col=0; col < Dimension; col++ )
      {
      if( value1(row,col) != pixel1(row,col) )
        {
        std::cerr << "Error in EvaluateAtContinuousIndex()" << std::endl;
        return EXIT_FAILURE;
        }
      }
    } 


  interpolator1->Print( std::cout );

  std::cout << "Name of Class " << interpolator1->GetNameOfClass() << std::endl;
  std::cout << "Name of SuperClass " << interpolator1->Superclass::GetNameOfClass() << std::endl;

  return EXIT_SUCCESS;
}

