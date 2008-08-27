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

  typedef itk::Matrix< float, NumberOfPhases, Dimension >   MatrixType;

  typedef itk::NumericTraits< MatrixType >::RealType    RealMatrixType;


  return EXIT_SUCCESS;
}
