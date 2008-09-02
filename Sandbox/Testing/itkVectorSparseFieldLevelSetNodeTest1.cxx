/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkVectorSparseFieldLevelSetNodeTest1.cxx

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

  typedef LevelSetImageType::IndexType                   IndexType;

  typedef itk::SparseFieldLevelSetNode< IndexType >      NodeType;

  IndexType index1;

  NodeType node1;
  NodeType node2;
  NodeType node3;

  node1.Next = &node2;
  node1.Program = &node3;
  node3.m_Value = index1;
 
  return EXIT_SUCCESS;
}
