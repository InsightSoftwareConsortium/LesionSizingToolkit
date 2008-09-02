/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorSparseFieldCityBlockNeighborListTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkVectorSparseFieldLevelSetImageFilter.h"
#include "itkImage.h"
#include "itkVector.h"

int main( int argc, char * argv [] )
{

  const unsigned int Dimension = 2;
  const unsigned int NumberOfPhases = 3;

  typedef itk::Vector< float, NumberOfPhases >           LevelSetPixelType;

  typedef itk::Image< LevelSetPixelType, Dimension >     LevelSetImageType;

  typedef itk::NeighborhoodIterator<LevelSetImageType>   NeighborhoodType;

  typedef itk::SparseFieldCityBlockNeighborList< NeighborhoodType > NeighborhoodListType;


  return EXIT_SUCCESS;
}
