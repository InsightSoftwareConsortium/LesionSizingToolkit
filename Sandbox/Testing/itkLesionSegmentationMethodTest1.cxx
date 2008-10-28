/*=========================================================================

  Program:   Lesion Sizing Toolkit
  Module:    itkLesionSegmentationMethodTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkLesionSegmentationMethod.h"
#include "itkImage.h"
#include "itkVector.h"

int main( int argc, char * argv [] )
{
  typedef itk::LesionSegmentationMethod   MethodType;

  MethodType::Pointer  segmentationMethod = MethodType::New();

  segmentationMethod->Print( std::cout );

  return EXIT_SUCCESS;
}
