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

/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVEDTest.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

=========================================================================*/
// Contributed by  Rashindra Manniesing from Netherlands

#if defined (_MSC_VER)
#pragma warning (disable: 4786)
#endif

#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkVesselEnhancingDiffusion3DImageFilter.h"
#include "itkNumericTraits.h"
#include "itkImage.h"

#include <iostream>

int itkVEDTest(int argc, char * argv [] )
{
  if (argc < 3)
    {
    std::cout << argv[0] << " in out " << std::endl;
    std::cout << "missing filenames " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VesselEnhancingDiffusion3DImageFilter<short> VT;
  typedef VT::ImageType                                     IT;
  typedef itk::ImageFileReader<IT>                          RT;
  typedef itk::ImageFileWriter<IT>                          WT;

  RT::Pointer r = RT::New();
  r->SetFileName(argv[1]);
  r->Update();

  IT::SpacingType spacing = r->GetOutput()->GetSpacing();
  double minSpacing = itk::NumericTraits< double >::max();
  for (unsigned int i = 0; i < IT::ImageDimension; i++)
    {
    if (minSpacing > spacing[i])
      {
      minSpacing = spacing[i];
      }
    }

  // Scales of Sigma. Expressed in terms of the pixel spacing.
  std::vector< VT::Precision > scales(5);
  scales[0] = 1.0    * minSpacing;
  scales[1] = 1.6067 * minSpacing;
  scales[2] = 2.5833 * minSpacing;
  scales[3] = 4.15   * minSpacing;
  scales[4] = 6.66   * minSpacing;

  VT::Pointer v = VT::New();
  v->SetInput(r->GetOutput());
  v->SetDefaultPars();
  v->Update();

  WT::Pointer w = WT::New();
  w->SetInput(v->GetOutput());
  w->SetFileName(argv[2]);
  w->Update();

  return EXIT_SUCCESS;
}
