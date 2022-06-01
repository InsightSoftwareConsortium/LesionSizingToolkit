/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkLandmarksReader_h
#define itkLandmarksReader_h

#include "itkProcessObject.h"
#include "itkImage.h"
#include "itkLandmarkSpatialObject.h"
#include "itkSpatialObjectReader.h"

namespace itk
{

/** \class LandmarksReader
 * \brief Class that reads a file containing spatial object landmarks.
 *
 * A LandmarkSpatialObject is produced as output.
 *
 * \ingroup SpatialObjectFilters
 * \ingroup LesionSizingToolkit
 */
template <unsigned int NDimension>
class ITK_TEMPLATE_EXPORT LandmarksReader : public ProcessObject
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(LandmarksReader);

  /** Standard class type alias. */
  using Self = LandmarksReader;
  using Superclass = ProcessObject;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LandmarksReader, ProcessObject);

  /** Dimension of the space */
  static constexpr unsigned int Dimension = NDimension;

  /** Type of spatialObject that will be passed as input and output of this
   * segmentation method. */
  using SpatialObjectType = LandmarkSpatialObject<NDimension>;
  using SpatialObjectPointer = typename SpatialObjectType::Pointer;

  /** Output data that carries the feature in the form of a
   * SpatialObject. */
  const SpatialObjectType *
  GetOutput() const;

  /** Set / Get the input filename */
  itkSetStringMacro(FileName);
  itkGetStringMacro(FileName);

protected:
  LandmarksReader();
  ~LandmarksReader() override;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;

  void
  GenerateData() override;

private:
  using SpatialObjectReaderType = SpatialObjectReader<NDimension, unsigned short>;
  using SpatialObjectReaderPointer = typename SpatialObjectReaderType::Pointer;
  using GroupType = typename SpatialObjectReaderType::GroupType;
  using ObjectListType = typename GroupType::ObjectListType;

  std::string                m_FileName;
  SpatialObjectReaderPointer m_SpatialObjectReader;
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkLandmarksReader.hxx"
#endif

#endif
