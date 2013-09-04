/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkShapeDetectionLevelSetSegmentationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeDetectionLevelSetSegmentationModule_h
#define __itkShapeDetectionLevelSetSegmentationModule_h

#include "itkSinglePhaseLevelSetSegmentationModule.h"
#include "itkShapeDetectionLevelSetImageFilter.h"

namespace itk
{

/** \class ShapeDetectionLevelSetSegmentationModule
 * \brief This class applies the ShapeDetectionLevelSet segmentation
 * method.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT ShapeDetectionLevelSetSegmentationModule : 
  public SinglePhaseLevelSetSegmentationModule<NDimension>
{
public:
  /** Standard class typedefs. */
  typedef ShapeDetectionLevelSetSegmentationModule                Self;
  typedef SinglePhaseLevelSetSegmentationModule<NDimension>       Superclass;
  typedef SmartPointer<Self>                                      Pointer;
  typedef SmartPointer<const Self>                                ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShapeDetectionLevelSetSegmentationModule, SinglePhaseLevelSetSegmentationModule);

  /** Dimension of the space */
  itkStaticConstMacro(Dimension, unsigned int, NDimension);

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  typedef typename Superclass::SpatialObjectType         SpatialObjectType;
  typedef typename Superclass::SpatialObjectPointer      SpatialObjectPointer;

  /** Types of images and spatial objects inherited from the superclass. */
  typedef typename Superclass::OutputPixelType           OutputPixelType;
  typedef typename Superclass::InputImageType            InputImageType;
  typedef typename Superclass::FeatureImageType          FeatureImageType;
  typedef typename Superclass::OutputImageType           OutputImageType;
  typedef typename Superclass::InputSpatialObjectType    InputSpatialObjectType;
  typedef typename Superclass::FeatureSpatialObjectType  FeatureSpatialObjectType;
  typedef typename Superclass::OutputSpatialObjectType   OutputSpatialObjectType;


protected:
  ShapeDetectionLevelSetSegmentationModule();
  virtual ~ShapeDetectionLevelSetSegmentationModule();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData ();

private:
  ShapeDetectionLevelSetSegmentationModule(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkShapeDetectionLevelSetSegmentationModule.hxx"
#endif

#endif
