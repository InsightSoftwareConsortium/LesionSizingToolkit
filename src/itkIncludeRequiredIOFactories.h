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

#ifndef itkIncludeRequiredIOFactories_h
#define itkIncludeRequiredIOFactories_h
#include "itkGDCMImageIOFactory.h"
#include "itkMetaImageIOFactory.h"
#include "itkPNGImageIOFactory.h"
#include "itkObjectFactoryBase.h"

void
RegisterRequiredFactories()
{
  itk::ObjectFactoryBase::RegisterFactory(itk::MetaImageIOFactory::New());
  itk::ObjectFactoryBase::RegisterFactory(itk::GDCMImageIOFactory::New());
  itk::ObjectFactoryBase::RegisterFactory(itk::PNGImageIOFactory::New());
}

#endif
