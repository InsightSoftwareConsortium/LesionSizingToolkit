/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkMimimumFeatureAggregator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimumFeatureAggregator_h
#define __itkMinimumFeatureAggregator_h

#include "itkFeatureGenerator.h"
#include "itkImage.h"
#include "itkImageSpatialObject.h"

namespace itk
{

/** \class MinimumFeatureAggregator
 * \brief Class for combining multiple features into a single one by computing
 * the pixel-wise minimum. 
 *
 * This class generates a new feature object containing an image that is
 * computed as the pixel-wise minimum of all the input feature images.
 *
 * \warning This class assumes that all the images have the same: origin,
 * spacing, orientation, and that they are represented in the same image grid.
 * mixing strategies.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 */
template <unsigned int NDimension>
class ITK_EXPORT MinimumFeatureAggregator : public FeatureAggregator<NDimension>
{
public:
  /** Standard class typedefs. */
  typedef MinimumFeatureAggregator      Self;
  typedef FeatureAggregator             Superclass;
  typedef SmartPointer<Self>            Pointer;
  typedef SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MinimumFeatureAggregator, FeatureAggregator);

  /** Dimension of the space */
  itkStaticConstMacro(Dimension, unsigned int, NDimension);


protected:
  MinimumFeatureAggregator();
  virtual ~MinimumFeatureAggregator();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData();


private:
  MinimumFeatureAggregator(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  typedef typename Superclass::FeatureGeneratorType             FeatureGeneratorType;
  typedef typename FeatureGeneratorType::Pointer                FeatureGeneratorPointer;
  typedef std::vector< FeatureGeneratorPointer >                FeatureGeneratorArrayType;
  typedef typename FeatureGeneratorArrayType::iterator          FeatureGeneratorIterator;
  typedef typename FeatureGeneratorArrayType::const_iterator    FeatureGeneratorConstIterator;

  void ConsolidateFeatures();

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkMinimumFeatureAggregator.txx"
#endif

#endif
