/*=========================================================================
const 
  Program:   Lesion Sizing Toolkit
  Module:    itkRegionCompetitionImageFilterTest1.cxx

  Copyright (c) Kitware Inc. 
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#include "itkBinaryThresholdImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkRegionCompetitionImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"


int itkRegionCompetitionImageFilterTest1( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{
  const unsigned int Dimension = 3;
  typedef signed short    InputPixelType;
  typedef unsigned char   BinaryPixelType;
  typedef unsigned short   LabelPixelType;

  typedef itk::Image< InputPixelType,  Dimension >      InputImageType;
  typedef itk::Image< BinaryPixelType, Dimension >      BinaryImageType;
  typedef itk::Image< LabelPixelType,  Dimension >      LabelImageType;

  typedef itk::BinaryThresholdImageFilter< 
    InputImageType, BinaryImageType >  ThresholdFilterType;

  typedef itk::ConnectedComponentImageFilter<
    BinaryImageType, LabelImageType > ComponentsFilterType;

  typedef itk::RelabelComponentImageFilter<
    LabelImageType, LabelImageType > RelabelFilterType;

  typedef itk::RegionCompetitionImageFilter< 
    InputImageType, LabelImageType >    CompetitionFilterType;


  ThresholdFilterType::Pointer thresholderFilter = ThresholdFilterType::New();

  ComponentsFilterType::Pointer componentsFilter = ComponentsFilterType::New();

  RelabelFilterType::Pointer relabelerFilter = RelabelFilterType::New();
  
  CompetitionFilterType::Pointer competitionFilter = CompetitionFilterType::New();
 

  //
  // Create an input image
  //
  InputImageType::Pointer  inputImage = InputImageType::New();

  InputImageType::RegionType itkregion;
  InputImageType::SizeType   itksize;
  InputImageType::IndexType  itkindex;

  itksize[0] = 21;
  itksize[1] = 21;
  itksize[2] = 42;

  itkindex[0] = 0;
  itkindex[1] = 0;
  itkindex[2] = 0;

  itkregion.SetIndex( itkindex );
  itkregion.SetSize( itksize );

  inputImage->SetRegions( itkregion );
  inputImage->Allocate();

  InputImageType::SpacingType itkspacing;

  itkspacing[0] = 0.7;
  itkspacing[1] = 0.9;
  itkspacing[2] = 1.5;

  inputImage->SetSpacing( itkspacing );

  InputImageType::PointType  itkorigin;

  itkorigin[0] = 129.5;
  itkorigin[1] = 137.5;
  itkorigin[2] = 159.5;

  inputImage->SetOrigin( itkorigin );

  //
  // Populate the pixel values in an asymmetric way to ensure that one
  // component is larger than the other.
  //
  InputImageType::IndexType index1;
  InputImageType::IndexType index2;

  index1[0] = 10;
  index1[1] = 10;
  index1[2] = 10;

  index2[0] = 10;
  index2[1] = 10;
  index2[2] = 31;

  InputImageType::PointType point1;
  InputImageType::PointType point2;

  inputImage->TransformIndexToPhysicalPoint( index1, point1 );
  inputImage->TransformIndexToPhysicalPoint( index2, point2 );

 
  typedef itk::ImageRegionIteratorWithIndex< InputImageType > IteratorType;

  IteratorType itr( inputImage, itkregion );

  itr.GoToBegin();

  InputImageType::PointType point;

  while ( !itr.IsAtEnd() )
    {
    const InputImageType::IndexType & index = itr.GetIndex();
    inputImage->TransformIndexToPhysicalPoint( index, point );
    const double distance1 = point1.EuclideanDistanceTo( point );
    const double distance2 = point2.EuclideanDistanceTo( point );
    const double value1 = 1000.0 - distance1 * 50;
    const double value2 = 1000.0 - distance2 * 50;
    if ( value1 > value2 )
      {
      itr.Set( static_cast< InputPixelType >( value1 ) );
      }
    else
      {
      itr.Set( static_cast< InputPixelType >( value2 ) );
      }
    ++itr;
    }


  // Just for debugging, save input image
  typedef itk::ImageFileWriter< InputImageType >   WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( inputImage );
  writer->SetFileName("inputCompetitionImage.mha");
  writer->Update();


  thresholderFilter->SetInput( inputImage );
  thresholderFilter->SetUpperThreshold( 2000 );
  thresholderFilter->SetLowerThreshold(  400 );
  thresholderFilter->Update();
  componentsFilter->SetInput( thresholderFilter->GetOutput() );
  relabelerFilter->SetInput( componentsFilter->GetOutput() );

  relabelerFilter->Update();
  LabelImageType * labelImagePt =  relabelerFilter->GetOutput();
  // Just for debugging, save input image of labels
  typedef itk::ImageFileWriter< LabelImageType >   LabelWriterType;
  LabelWriterType::Pointer labelWriter = LabelWriterType::New();
  labelWriter->SetInput( relabelerFilter->GetOutput() );
  labelWriter->SetFileName("labeledImage.mha");
  labelWriter->Update();

  competitionFilter->SetInput( inputImage );
  competitionFilter->SetInputLabels( labelImagePt);
  competitionFilter->Update();

  // Write the output image
  labelWriter->SetInput( competitionFilter->GetOutput() );
  labelWriter->SetFileName("labeledSegmentedImage.mha");
  labelWriter->Update();


  return EXIT_SUCCESS;
}
