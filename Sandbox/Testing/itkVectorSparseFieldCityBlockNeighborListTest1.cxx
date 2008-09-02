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

  NeighborhoodListType neighborList;

  NeighborhoodListType::RadiusType radius = neighborList.GetRadius();

  std::cout << "Radius = " << radius << std::endl;

  unsigned int size = neighborList.GetSize();

  std::cout << "Size = " << size << std::endl;

  std::cout << std::endl;

  std::cout << "Index      Offset" << std::endl;

  for( unsigned int i=0; i < size; i++ )
    {
    std::cout << neighborList.GetArrayIndex( i ) << "      ";
    std::cout << neighborList.GetNeighborhoodOffset( i ) << std::endl;
    }

  return EXIT_SUCCESS;
}
