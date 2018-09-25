/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkFeatureGenerator.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkFeatureGenerator_h
#define itkFeatureGenerator_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkDataObjectDecorator.h"
#include "itkSpatialObject.h"

namespace itk
{

/** \class FeatureGenerator
 * \brief Class that generates features (typically images) used as input for a segmentation method.
 *
 * The typical use of this class would be to generate the edge-map needed by a
 * Level Set filter to internally compute its speed image.
 *
 * SpatialObjects are used as inputs and outputs of this class.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT FeatureGenerator : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_ASSIGN(FeatureGenerator);

  /** Standard class type alias. */
  using Self = FeatureGenerator;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FeatureGenerator, ProcessObject);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = SpatialObject< NDimension >;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;

  /** Input data that will be used for generating the feature. */
  using ProcessObject::SetInput;
  void SetInput( const SpatialObjectType * input );
  const SpatialObjectType * GetInput() const;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType * GetFeature() const;


protected:
  FeatureGenerator();
  ~FeatureGenerator() override;
  void PrintSelf(std::ostream& os, Indent indent) const override;

  /** Derived classes must implement the "void  GenerateData()" method  */

  /** non-const version of the method intended to be used in derived classes. */
  SpatialObjectType * GetInternalFeature();
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
# include "itkFeatureGenerator.hxx"
#endif

#endif
