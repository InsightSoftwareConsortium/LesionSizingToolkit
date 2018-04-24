/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkConfidenceConnectedSegmentationModule.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkConfidenceConnectedSegmentationModule_h
#define itkConfidenceConnectedSegmentationModule_h

#include "itkRegionGrowingSegmentationModule.h"
#include "itkConfidenceConnectedImageFilter.h"

namespace itk
{

/** \class ConfidenceConnectedSegmentationModule
 * \brief This class applies the connected threshold region growing
 * segmentation method.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_EXPORT ConfidenceConnectedSegmentationModule :
  public RegionGrowingSegmentationModule<NDimension>
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(ConfidenceConnectedSegmentationModule);

  /** Standard class type alias. */
  using Self = ConfidenceConnectedSegmentationModule;
  using Superclass = RegionGrowingSegmentationModule<NDimension>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ConfidenceConnectedSegmentationModule, RegionGrowingSegmentationModule);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using FeatureImageType = typename Superclass::FeatureImageType;
  using OutputImageType = typename Superclass::OutputImageType;
  using InputSpatialObjectType = typename Superclass::InputSpatialObjectType;

  /** Factor that will be applied to the standard deviation in order to compute
   * the intensity range from which pixel will be included in the region. */
  itkSetMacro( SigmaMultiplier, double );
  itkGetMacro( SigmaMultiplier, double );

protected:
  ConfidenceConnectedSegmentationModule();
  ~ConfidenceConnectedSegmentationModule() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Method invoked by the pipeline in order to trigger the computation of
   * the segmentation. */
  void  GenerateData () override;

private:
  double        m_SigmaMultiplier;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkConfidenceConnectedSegmentationModule.hxx"
#endif

#endif
