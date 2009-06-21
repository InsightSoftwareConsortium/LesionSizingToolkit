/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    ResampleVolumeToBeIsotropic.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif


#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkIdentityTransform.h"

#include "itkBSplineInterpolateImageFunction.h"
#include "itkWindowedSincInterpolateImageFunction.h"


int main( int argc, char * argv[] )
{
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile finalSpacing ";
    std::cerr << " interpolatorType(0:BSpline 1:WindowedSinc 2:Linear)" << std::endl;
    return EXIT_FAILURE;
    }


  typedef   signed short    PixelType;
  const     unsigned int    Dimension = 3;

  typedef itk::Image< PixelType, Dimension >       ImageType;

  typedef itk::ImageFileReader< ImageType  >       ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( argv[1] );

  try 
    {
    reader->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught!" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }


  ImageType::ConstPointer inputImage = reader->GetOutput();

  typedef itk::ResampleImageFilter< ImageType, ImageType >  ResampleFilterType;

  ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  typedef itk::IdentityTransform< double, Dimension >  TransformType;

  TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();

  resampler->SetTransform( transform );


  typedef itk::BSplineInterpolateImageFunction< ImageType, double >  BSplineInterpolatorType;

  BSplineInterpolatorType::Pointer bsplineInterpolator = BSplineInterpolatorType::New();

  bsplineInterpolator->UseImageDirectionOn();
  bsplineInterpolator->SetSplineOrder( 3 );


  typedef itk::WindowedSincInterpolateImageFunction< ImageType, 3 >  WindowedSincInterpolatorType;

  WindowedSincInterpolatorType::Pointer windowedSincInterpolator = WindowedSincInterpolatorType::New();

  typedef itk::LinearInterpolateImageFunction< ImageType, double >  LinearInterpolatorType;

  LinearInterpolatorType::Pointer linearInterpolator = LinearInterpolatorType::New();
  

  switch( atoi( argv[4] ) )
    {
    case 0:
      resampler->SetInterpolator( bsplineInterpolator );
      break;
    case 1:
      resampler->SetInterpolator( windowedSincInterpolator );
      break;
    case 2:
      resampler->SetInterpolator( linearInterpolator );
      break;
    }


  resampler->SetDefaultPixelValue( -1024 ); // Hounsfield Units for Air

  const ImageType::SpacingType & inputSpacing = inputImage->GetSpacing();

  double minSpacing = itk::NumericTraits< double >::max();
  for (int i = 0; i < 3; i++)
    {
    minSpacing = (minSpacing > inputSpacing[i] ? inputSpacing[i] : minSpacing);
    }
  
  ImageType::SpacingType outputSpacing;

  const double finalSpacing = (strcmp( argv[3], "-minspacing" ) == 0) 
                                        ? minSpacing : atof( argv[3] );

  outputSpacing[0] = finalSpacing;
  outputSpacing[1] = finalSpacing;
  outputSpacing[2] = finalSpacing;

  resampler->SetOutputSpacing( outputSpacing );


  resampler->SetOutputOrigin( inputImage->GetOrigin() );
  resampler->SetOutputDirection( inputImage->GetDirection() );

  ImageType::SizeType   inputSize = inputImage->GetLargestPossibleRegion().GetSize();
  
  typedef ImageType::SizeType::SizeValueType SizeValueType;

  const double dx = inputSize[0] * inputSpacing[0] / outputSpacing[0];
  const double dy = inputSize[1] * inputSpacing[1] / outputSpacing[1];
  const double dz = inputSize[2] * inputSpacing[2] / outputSpacing[2];

  ImageType::SizeType   finalSize;

  finalSize[0] = static_cast<SizeValueType>( dx );
  finalSize[1] = static_cast<SizeValueType>( dy );
  finalSize[2] = static_cast<SizeValueType>( dz );

  resampler->SetSize( finalSize );

  resampler->SetInput( inputImage );

  typedef itk::ImageFileWriter< ImageType >  WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName( argv[2] );
  writer->SetInput( resampler->GetOutput() );
  writer->UseCompressionOn();

  try 
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
    }

  return EXIT_SUCCESS;
}

