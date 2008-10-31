/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConnectedThresholdSegmentationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConnectedThresholdSegmentationModule_h
#define __itkConnectedThresholdSegmentationModule_h

#include "itkSegmentationModule.h"

namespace itk
{

/** \class ConnectedThresholdSegmentationModule
 * \brief Class applies a region growing segmentation method
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 */
template <unsigned int NDimension>
class ITK_EXPORT ConnectedThresholdSegmentationModule : public SegmentationModule<NDimension>
{
public:
  /** Standard class typedefs. */
  typedef ConnectedThresholdSegmentationModule       Self;
  typedef SegmentationModule<NDimension>        Superclass;
  typedef SmartPointer<Self>                    Pointer;
  typedef SmartPointer<const Self>              ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConnectedThresholdSegmentationModule, SegmentationModule);

  /** Dimension of the space */
  itkStaticConstMacro(Dimension, unsigned int, NDimension);

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  typedef typename Superclass::SpatialObjectType         SpatialObjectType;
  typedef typename Superclass::SpatialObjectPointer      SpatialObjectPointer;

protected:
  ConnectedThresholdSegmentationModule();
  virtual ~ConnectedThresholdSegmentationModule();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData ();

private:
  ConnectedThresholdSegmentationModule(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkConnectedThresholdSegmentationModule.txx"
#endif

#endif
