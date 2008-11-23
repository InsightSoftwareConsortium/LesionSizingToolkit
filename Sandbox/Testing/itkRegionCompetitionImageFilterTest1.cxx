/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkRegionCompetitionImageFilterTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkRegionCompetitionImageFilter.h"
#include "itkImage.h"

int main( int argc, char * argv [] )
{
  const unsigned int Dimension = 3;
  typedef unsigned char  PixelType;

  typedef itk::Image< PixelType, Dimension >      ImageType;

  typedef itk::RegionCompetitionImageFilter< ImageType, ImageType >    FilterType;

  FilterType::Pointer filter = FilterType::New();
 
  return EXIT_SUCCESS;
}
