/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorZeroCrossingImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkVectorZeroCrossingImageFilter_txx
#define _itkVectorZeroCrossingImageFilter_txx

#include "itkConstNeighborhoodIterator.h"
#include "itkVectorZeroCrossingImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkFixedArray.h"
#include "itkProgressReporter.h"
#include "itkMeasurementVectorTraits.h"


namespace itk
{

template <class TInputImage, class TOutputImage>
void 
VectorZeroCrossingImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr = 
    const_cast<TInputImage *>( this->GetInput() );
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // Build an operator so that we can determine the kernel size
  unsigned long radius = 1;
  
  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius( radius );

  // crop the input requested region at the input's largest possible region
  if ( inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()) )
    {
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion( inputRequestedRegion );
    
    // build an exception
    InvalidRequestedRegionError e(__FILE__, __LINE__);
    e.SetLocation(ITK_LOCATION);
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    
    }
}

template< class TInputImage, class TOutputImage >
void
VectorZeroCrossingImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  unsigned int i;
  ZeroFluxNeumannBoundaryCondition<TInputImage> nbc;

  ConstNeighborhoodIterator<TInputImage> bit;
  ImageRegionIterator<TOutputImage> it;
  
  typename OutputImageType::Pointer      output = this->GetOutput();
  typename  InputImageType::ConstPointer input  = this->GetInput();
  
  // Calculate iterator radius
  Size<ImageDimension> radius;
  radius.Fill(1);
  
  // Find the data-set boundary "faces"
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::
    FaceListType faceList;
  NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage> bC;
  faceList = bC(input, outputRegionForThread, radius);
  
  typename NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>::
    FaceListType::iterator fit;
  
  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
    
  InputImagePixelType this_one;
  InputImagePixelType that;
  InputImagePixelType abs_this_one;
  InputImagePixelType abs_that;
  InputImagePixelType zero = NumericTraits<InputImagePixelType>::Zero;

  FixedArray<long, 2 * ImageDimension> offset;

  bit = ConstNeighborhoodIterator<InputImageType>(radius,
                                                  input,
                                                  *faceList.begin());
  //Set the offset of the neighbors to the center pixel.
  for ( i = 0 ; i < ImageDimension; i++)
    {
    offset[i] = -1 * static_cast<long>( bit.GetStride(i));
    offset[i+ImageDimension] =  bit.GetStride(i);
    }

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit=faceList.begin(); fit != faceList.end(); ++fit)
    { 
    bit = ConstNeighborhoodIterator<InputImageType>(radius,
                                                    input, *fit);
    it = ImageRegionIterator<OutputImageType>(output, *fit);
    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();
    
    InputImagePixelType outputPixel;

    const unsigned long center = bit.Size()/2;
    while ( ! bit.IsAtEnd() )
      {
      this_one = bit.GetPixel(center);
      it.Set(m_BackgroundValue);
      for( i = 0; i< ImageDimension * 2; i++)
        {
        that = bit.GetPixel(center + offset[i]);

        const unsigned int numberOfComponents = itk::MeasurementVectorTraits::GetLength( that );

        for( unsigned int k=0; k < numberOfComponents; k++ )
          {
          const InputImagePixelComponentType this_one_k = this_one[k];
          const InputImagePixelComponentType that_k     = that[k];
          const InputImagePixelComponentType zero_k     = zero[k];

          if(    ((this_one_k  < zero_k) && (that_k >  zero_k))
              || ((this_one_k  > zero_k) && (that_k <  zero_k)) 
              || ((this_one_k == zero_k) && (that_k != zero_k))
              || ((this_one_k != zero_k) && (that_k == zero_k))  )
            {
            const InputImagePixelComponentType abs_this_one_k =  vnl_math_abs(this_one_k);
            const InputImagePixelComponentType abs_that_k = vnl_math_abs(that_k);
            if(abs_this_one_k < abs_that_k)
              {
              outputPixel[k] = m_ForegroundValue;
              break;
              }
            else if(abs_this_one_k == abs_that_k && i >= ImageDimension)
              {
              outputPixel[k] = m_ForegroundValue;
              break;
              }
            }
          it.Set( outputPixel );
          }
        }
      ++bit;
      ++it;
      progress.CompletedPixel();
      }    
    }
}

template< class TInputImage, class TOutputImage >
void
VectorZeroCrossingImageFilter< TInputImage, TOutputImage >
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ForegroundValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ForegroundValue)
     << std::endl;
  os << indent << "BackgroundValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue)
     << std::endl;
}

}//end of itk namespace



#endif
