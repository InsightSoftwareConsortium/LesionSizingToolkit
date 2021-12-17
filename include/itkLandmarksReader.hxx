/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkLandmarksReader_hxx
#define itkLandmarksReader_hxx

#include "itkSpatialObjectReader.h"


namespace itk
{

/**
 * Constructor
 */
template <unsigned int NDimension>
LandmarksReader<NDimension>::LandmarksReader()
{
  this->SetNumberOfRequiredOutputs(1);

  this->m_SpatialObjectReader = SpatialObjectReaderType::New();

  typename SpatialObjectType::Pointer outputObject = SpatialObjectType::New();

  this->ProcessObject::SetNthOutput(0, outputObject);
}


/**
 * Destructor
 */
template <unsigned int NDimension>
LandmarksReader<NDimension>::~LandmarksReader() = default;

template <unsigned int NDimension>
const typename LandmarksReader<NDimension>::SpatialObjectType *
LandmarksReader<NDimension>::GetOutput() const
{
  return static_cast<const SpatialObjectType *>(this->ProcessObject::GetOutput(0));
}

template <unsigned int NDimension>
void
LandmarksReader<NDimension>::GenerateData()
{
  this->m_SpatialObjectReader->SetFileName(this->GetFileName());

  this->m_SpatialObjectReader->Update();

  typename SpatialObjectReaderType::GroupPointer group = this->m_SpatialObjectReader->GetGroup();

  if (!group)
  {
    itkExceptionMacro("Couldn't fine a group in file" << this->GetFileName());
  }

  ObjectListType * groupChildren = group->GetChildren(999999);

  typename ObjectListType::const_iterator spatialObjectItr = groupChildren->begin();

  typename SpatialObjectType::Pointer landmarkSpatialObject;

  while (spatialObjectItr != groupChildren->end())
  {
    std::string objectName = (*spatialObjectItr)->GetTypeName();
    if (objectName == "LandmarkSpatialObject")
    {
      const auto * landmarkSpatialObjectRawPointer =
        dynamic_cast<const SpatialObjectType *>(spatialObjectItr->GetPointer());
      landmarkSpatialObject = const_cast<SpatialObjectType *>(landmarkSpatialObjectRawPointer);
      break;
    }
    spatialObjectItr++;
  }

  if (landmarkSpatialObject.IsNull())
  {
    itkExceptionMacro("Input file does not contain landmarks");
  }

  landmarkSpatialObject->DisconnectPipeline();

  auto * outputObject = dynamic_cast<SpatialObjectType *>(this->ProcessObject::GetOutput(0));

  outputObject->SetPoints(landmarkSpatialObject->GetPoints());

  delete groupChildren;
}

/*
 * PrintSelf
 */
template <unsigned int NDimension>
void
LandmarksReader<NDimension>::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << this->m_FileName << std::endl;
}


} // end namespace itk

#endif
