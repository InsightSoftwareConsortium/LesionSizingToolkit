/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorFiniteDifferenceImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorFiniteDifferenceImageFilter_h_
#define __itkVectorFiniteDifferenceImageFilter_h_

#include "itkFiniteDifferenceImageFilter.h"

namespace itk {

/**
 *  \class VectorFiniteDifferenceImageFilter
 *
 *  \brief This class implements a finite difference partial differential
 *  equation solver for evolving surfaces embedded in volumes as level-sets.
 *
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT VectorFiniteDifferenceImageFilter :
  public FiniteDifferenceImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef VectorFiniteDifferenceImageFilter                       Self;
  typedef FiniteDifferenceImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef SmartPointer<Self>                                      Pointer;
  typedef SmartPointer<const Self>                                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VectorFiniteDifferenceImageFilter, FiniteDifferenceImageFilter);

protected:
  VectorFiniteDifferenceImageFilter();
  ~VectorFiniteDifferenceImageFilter();

  virtual void PrintSelf(std::ostream& os, Indent indent) const;

  /** This method allocates a temporary update container in the subclass. */
  virtual void AllocateUpdateBuffer();

  typedef typename Superclass::TimeStepType TimeStepType;

  /** This method is defined by a subclass to apply changes to the output
   * from an update buffer and a time step value "dt".
   * \param dt Time step value. */
  virtual void ApplyUpdate(TimeStepType dt);
  
  /** This method is defined by a subclass to populate an update buffer
   * with changes for the pixels in the output.  It returns a time
   * step value to be used for the update.
   * \returns A time step to use in updating the output with the changes
   * calculated from this method. */
  virtual TimeStepType CalculateChange();

  /** This method can be defined in subclasses as needed to copy the input
   * to the output. See DenseFiniteDifferenceImageFilter for an
   * implementation. */
  virtual void CopyInputToOutput();

private:

  VectorFiniteDifferenceImageFilter(const Self&);//purposely not implemented
  void operator=(const Self&);      //purposely not implemented

};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorFiniteDifferenceImageFilter.txx"
#endif

#endif
