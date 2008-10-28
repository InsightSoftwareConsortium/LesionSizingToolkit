/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkLesionSegmentationMethod.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLesionSegmentationMethod_h
#define __itkLesionSegmentationMethod_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkDataObjectDecorator.h"

namespace itk
{

/** \class LesionSegmentationMethod
 * \brief Class for coordinating components of a segmentation framework.
 *
 * This class was designed to facilitate the segmentation of lung lesions, but
 * yet to be useful for other segmentation tasks.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 */
class ITK_EXPORT LesionSegmentationMethod : public ProcessObject 
{
public:
  /** Standard class typedefs. */
  typedef LesionSegmentationMethod      Self;
  typedef ProcessObject                 Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(LesionSegmentationMethod, ProcessObject);

protected:
  LesionSegmentationMethod();
  virtual ~LesionSegmentationMethod();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Method invoked by the pipeline in order to trigger the computation of 
   * the segmentation. */
  void  GenerateData ();

private:
  LesionSegmentationMethod(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


} // end namespace itk

#endif
