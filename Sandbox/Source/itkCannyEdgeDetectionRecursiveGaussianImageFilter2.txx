/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkCannyEdgeDetectionRecursiveGaussianImageFilter2.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCannyEdgeDetectionRecursiveGaussianImageFilter_txx
#define __itkCannyEdgeDetectionRecursiveGaussianImageFilter_txx

#include "itkCannyEdgeDetectionRecursiveGaussianImageFilter2.h"

#include "itkZeroCrossingImageFilter.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkGradientMagnitudeImageFilter.h"

namespace itk
{
  
template <class TInputImage, class TOutputImage>
CannyEdgeDetectionRecursiveGaussianImageFilter<TInputImage, TOutputImage>::
CannyEdgeDetectionRecursiveGaussianImageFilter()
{
  unsigned int i;

  m_Sigma = 1.0;

  m_OutsideValue = NumericTraits<OutputImagePixelType>::Zero;
  m_Threshold = NumericTraits<OutputImagePixelType>::Zero;
  m_UpperThreshold = NumericTraits<OutputImagePixelType>::Zero;
  m_LowerThreshold = NumericTraits<OutputImagePixelType>::Zero;

  m_GaussianFilter      = GaussianImageFilterType::New();
  m_LaplacianFilter      = LaplacianImageFilterType::New();
  m_MultiplyImageFilter = MultiplyImageFilterType::New();
  m_UpdateBuffer1  = OutputImageType::New();

  // Set up neighborhood slices for all the dimensions.
  typename Neighborhood<OutputImagePixelType, ImageDimension>::RadiusType r;
  r.Fill(1);

  // Dummy neighborhood used to set up the slices.
  Neighborhood<OutputImagePixelType, ImageDimension> it;
  it.SetRadius(r);
  
  // Slice the neighborhood
  m_Center =  it.Size() / 2;

  for (i = 0; i< ImageDimension; ++i)
    {
    m_Stride[i] = it.GetStride(i);
    }

  for (i = 0; i< ImageDimension; ++i)
    {
    m_ComputeCannyEdgeSlice[i]
      = std::slice( m_Center - m_Stride[i], 3, m_Stride[i]);
    }
   
  //Initialize the list
  m_NodeStore = ListNodeStorageType::New();
  m_NodeList = ListType::New();
}
 
template <class TInputImage, class TOutputImage>
void
CannyEdgeDetectionRecursiveGaussianImageFilter<TInputImage, TOutputImage>
::AllocateUpdateBuffer()
{
  // The update buffer looks just like the input.

  typename TInputImage::ConstPointer input = this->GetInput();
  
  m_UpdateBuffer1->CopyInformation( input );
  m_UpdateBuffer1->SetRequestedRegion(input->GetRequestedRegion());
  m_UpdateBuffer1->SetBufferedRegion(input->GetBufferedRegion());
  m_UpdateBuffer1->Allocate();  
}

template <class TInputImage, class TOutputImage>
void 
CannyEdgeDetectionRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::GenerateInputRequestedRegion() throw(InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  return;  
  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr = 
    const_cast< TInputImage * >( this->GetInput());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();
  
  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  //Set the kernel size.
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
    
    itkExceptionMacro("Requested region is (at least partially) outside the largest possible region.");
    }
}

template< class TInputImage, class TOutputImage >
void
CannyEdgeDetectionRecursiveGaussianImageFilter< TInputImage, TOutputImage >
::GenerateData()
{
  // Allocate the output
  this->GetOutput()->SetBufferedRegion( this->GetOutput()->GetRequestedRegion() );
  this->GetOutput()->Allocate();
 
  typename  InputImageType::ConstPointer  input  = this->GetInput();
  
  typename ZeroCrossingImageFilter<TOutputImage, TOutputImage>::Pointer 
    zeroCrossFilter = ZeroCrossingImageFilter<TOutputImage, TOutputImage>::New();

  typename GradientMagnitudeImageFilter<TOutputImage, TOutputImage>::Pointer
    gradMag = GradientMagnitudeImageFilter<TOutputImage, TOutputImage>::New();

  typename MultiplyImageFilter<TOutputImage, TOutputImage,TOutputImage>::Pointer multFilter 
    = MultiplyImageFilter<TOutputImage, TOutputImage,TOutputImage>::New();

  this->AllocateUpdateBuffer();

  // 1.Apply the Gaussian Filter to the input image.-------
  m_GaussianFilter->SetSigma( this->m_Sigma );
  m_GaussianFilter->SetNormalizeAcrossScale( true );
  m_GaussianFilter->SetInput(input);
  m_GaussianFilter->Update();

  //2. Calculate 2nd order directional derivative-------
  m_LaplacianFilter->SetSigma( this->m_Sigma );
  m_LaplacianFilter->SetNormalizeAcrossScale( true );
  m_LaplacianFilter->SetInput(input);
  m_LaplacianFilter->Update();
  

  // 3. Non-maximum suppression----------
  
  // Calculate the zero crossings of the 2nd directional derivative and write 
  // the result to output buffer. 
  zeroCrossFilter->SetInput(this->m_LaplacianFilter->GetOutput());
  zeroCrossFilter->Update();
  
  // 4. Hysteresis Thresholding---------
  
  // First get all the edges corresponding to zerocrossings
  m_MultiplyImageFilter->SetInput1(m_UpdateBuffer1);
  m_MultiplyImageFilter->SetInput2(zeroCrossFilter->GetOutput());
 
  // To save memory, we will graft the output of the m_GaussianFilter, 
  // which is no longer needed, into the m_MultiplyImageFilter.
  m_MultiplyImageFilter->GraftOutput( m_GaussianFilter->GetOutput() );
  m_MultiplyImageFilter->Update();

  //Then do the double threshoulding upon the edge reponses
  this->HysteresisThresholding();
}

template< class TInputImage, class TOutputImage >
void
CannyEdgeDetectionRecursiveGaussianImageFilter< TInputImage, TOutputImage >
::HysteresisThresholding()
{
  // This is the Zero crossings of the Second derivative multiplied with the
  // gradients of the image. HysteresisThresholding of this image should give
  // the Canny output.
  typename OutputImageType::Pointer input = m_MultiplyImageFilter->GetOutput();
  float value;

  ListNodeType *node;

  ImageRegionIterator<TOutputImage> oit( input, input->GetRequestedRegion() );
  
  oit.GoToBegin();
  
  ImageRegionIterator<TOutputImage> uit(this->GetOutput(),
                                    this->GetOutput()->GetRequestedRegion());
  uit.GoToBegin();
  while(!uit.IsAtEnd())
    {
    uit.Value() = 0;
    ++uit;
    }

  while(!oit.IsAtEnd())
    {
    value = oit.Value();

    if(value > m_UpperThreshold)
      {
      node = m_NodeStore->Borrow();
      node->m_Value = oit.GetIndex();
      m_NodeList->PushFront(node);
      FollowEdge(oit.GetIndex());
      }

    ++oit;
    }
}

template< class TInputImage, class TOutputImage >
void
CannyEdgeDetectionRecursiveGaussianImageFilter< TInputImage, TOutputImage >
::FollowEdge(IndexType index)
{
  // This is the Zero crossings of the Second derivative multiplied with the
  // gradients of the image. HysteresisThresholding of this image should give
  // the Canny output.
  typename OutputImageType::Pointer input = m_MultiplyImageFilter->GetOutput();
  
  IndexType nIndex;
  IndexType cIndex;
  ListNodeType * node;

  //assign iterator radius
  Size<ImageDimension> radius; radius.Fill(1);

  ConstNeighborhoodIterator<TOutputImage> oit(radius, 
               input, input->GetRequestedRegion());
  ImageRegionIteratorWithIndex<TOutputImage> uit( this->GetOutput(),
                                             this->GetOutput()->GetRequestedRegion());

  uit.SetIndex(index);
  if(uit.Get() == 1)
    {
    return;
    }
  
  int nSize = m_Center * 2 +1;  
  while(!m_NodeList->Empty())
    {
    // Pop the front node from the list and read its index value.
    node = m_NodeList->Front(); // get a pointer to the first node
    cIndex = node->m_Value;    // read the value of the first node
    m_NodeList->PopFront();    // unlink the front node
    m_NodeStore->Return(node); // return the memory for reuse

    // Move iterators to the correct index position.
    oit.SetLocation(cIndex);
    uit.SetIndex(cIndex);
    uit.Value() = 1;

    // Search the neighbors for new indicies to add to the list.
    for(int i = 0; i < nSize; i++)
      {
      nIndex = oit.GetIndex(i);
      uit.SetIndex(nIndex);
      if(InBounds(nIndex))
        {
        if(oit.GetPixel(i) > m_LowerThreshold && uit.Value() != 1  )
          {
          node = m_NodeStore->Borrow();  // get a new node struct
          node->m_Value = nIndex;        // set its value
          m_NodeList->PushFront(node);   // add the new node to the list
          
          uit.SetIndex(nIndex);
          uit.Value() = 1;
          }
        }
      }
    }
}

template< class TInputImage, class TOutputImage >
bool
CannyEdgeDetectionRecursiveGaussianImageFilter< TInputImage, TOutputImage >
::InBounds(IndexType index)
{
  typename InputImageType::ConstPointer input = this->GetInput();
  typename InputImageType::SizeType sz;
  sz = (input->GetRequestedRegion()).GetSize();
  
  for(unsigned int i = 0; i < ImageDimension; i++)
    {
    if(index[i] < 0 ||
       index[i] >= static_cast<typename IndexType::IndexValueType>(sz[i]))
      {
      return false;
      }
    }
  return true;

}

template <class TInputImage, class TOutputImage>
void 
CannyEdgeDetectionRecursiveGaussianImageFilter<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << "Sigma: " << m_Sigma << std::endl;
  os << indent << "Threshold: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_Threshold)
     << std::endl;
  os << indent << "UpperThreshold: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_UpperThreshold)
     << std::endl;
  os << indent << "LowerThreshold: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_LowerThreshold)
     << std::endl;
  os << indent << "OutsideValue: "
     << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_OutsideValue)
     << std::endl;
  os << "Center: "
     << m_Center << std::endl;
  os << "Stride: "
     << m_Stride << std::endl;
  os << "Gaussian Filter: " << std::endl;
     m_GaussianFilter->Print(os,indent.GetNextIndent());
  os << "Multiply image Filter: " << std::endl;
     m_MultiplyImageFilter->Print(os,indent.GetNextIndent());
  os << "UpdateBuffer1: " << std::endl;
     m_UpdateBuffer1->Print(os,indent.GetNextIndent());
}

}//end of itk namespace
#endif
