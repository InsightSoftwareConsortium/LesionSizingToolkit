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
#ifndef __itkVectorFiniteDifferenceImageFilter_h
#define __itkVectorFiniteDifferenceImageFilter_h

#include "itkFiniteDifferenceImageFilter.h"
#include "itkVectorFiniteDifferenceFunction.h"
#include <vector>

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

  typedef typename Superclass::OutputPixelValueType               OutputPixelValueType;
  typedef typename Superclass::OutputImageType                    OutputImageType;

  /** Type of the function used for computing the updates at a given pixel.
   * Note that this function does not derive from the
   * itk::FiniteDifferenceFunction because the signature of its ComputeUpdate()
   * method is not appropriate for the case of images with multiple components.
   * */
  typedef VectorFiniteDifferenceFunction< TInputImage >        VectorDifferenceFunctionType;
  typedef typename VectorDifferenceFunctionType::Pointer  VectorDifferenceFunctionPointer;

  /** List of difference functions. We expect to have one function per
   * component of the input image */
  typedef std::vector< VectorDifferenceFunctionPointer >       VectorDifferenceFunctionListType;

protected:
  VectorFiniteDifferenceImageFilter();
  ~VectorFiniteDifferenceImageFilter();

  /** This is the high-level algorithm for calculating finite difference
   * solutions. It calls virtual methods in its subclasses to implement the
   * major steps of the algorithm. */
  virtual void GenerateData();

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

  /** The number of components per pixel in the input vector image. This is an
   * auxiliary variable used to cache the number of components. */
  unsigned int m_NumberOfComponents;

  /** List of finite difference functions that will compute the updates at
   * every pixel. There should be one function per component of the input
   * image. For example, if the input image has 5 components, this filter
   * expects to receive from the user, 5 finite difference functions. Each one
   * of the functions will compute the update for its respective pixel
   * component. This couldn't be factorized in a single function that computes
   * the updates in a single step, because this filter uses an internal sparce
   * representation and most of the time the sparce representation of one
   * component will not overlap with the sparce representation of the other,
   * meaning that while one of the components can be updated at a particular
   * pixel, the values of the other components may not be relevant nor needed
   * at that same pixel. */
  VectorDifferenceFunctionListType   m_DifferenceFunctions;

private:
  VectorFiniteDifferenceImageFilter(const Self&);//purposely not implemented
  void operator=(const Self&);      //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVectorFiniteDifferenceImageFilter.txx"
#endif

#endif
