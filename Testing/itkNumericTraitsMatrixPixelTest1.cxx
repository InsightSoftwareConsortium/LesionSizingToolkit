/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkNumericTraitsMatrixPixelTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkNumericTraitsMatrixPixel.h"
#include "itkMatrix.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 2;

  typedef itk::Matrix< char,   NumberOfPhases, Dimension >   MatrixType1;
  typedef itk::Matrix< float,  NumberOfPhases, Dimension >   MatrixType2;
  typedef itk::Matrix< double, NumberOfPhases, Dimension >   MatrixType3;

  typedef itk::NumericTraits< MatrixType1 >::RealType    RealMatrixType1;
  typedef itk::NumericTraits< MatrixType2 >::RealType    RealMatrixType2;
  typedef itk::NumericTraits< MatrixType3 >::RealType    RealMatrixType3;

  RealMatrixType1 realTypeInstance1;
  RealMatrixType2 realTypeInstance2;
  RealMatrixType3 realTypeInstance3;

  return EXIT_SUCCESS;
}
