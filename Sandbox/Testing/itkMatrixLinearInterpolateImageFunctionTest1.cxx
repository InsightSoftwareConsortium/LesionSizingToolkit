/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkMatrixLinearInterpolateImageFunctionTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkMatrixLinearInterpolateImageFunction.h"
#include "itkImage.h"
#include "itkMatrix.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 2;

  typedef itk::Matrix< char,   NumberOfPhases, Dimension >   MatrixType1;
  typedef itk::Matrix< float,  NumberOfPhases, Dimension >   MatrixType2;
  typedef itk::Matrix< double, NumberOfPhases, Dimension >   MatrixType3;

  typedef itk::Image< MatrixType1, Dimension >   ImageType1;
  typedef itk::Image< MatrixType2, Dimension >   ImageType2;
  typedef itk::Image< MatrixType3, Dimension >   ImageType3;

  typedef itk::MatrixLinearInterpolateImageFunction< ImageType1 >  InterpolatorType1;
  typedef itk::MatrixLinearInterpolateImageFunction< ImageType2 >  InterpolatorType2;
  typedef itk::MatrixLinearInterpolateImageFunction< ImageType3 >  InterpolatorType3;

  InterpolatorType1::Pointer interpolator1 = InterpolatorType1::New();
  InterpolatorType2::Pointer interpolator2 = InterpolatorType2::New();
  InterpolatorType3::Pointer interpolator3 = InterpolatorType3::New();

  return EXIT_SUCCESS;
}

