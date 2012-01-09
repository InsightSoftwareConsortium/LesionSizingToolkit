/*=========================================================================
 *
 *  Copyright Insight Software Consortium
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

#include "itkLesionSegmentationCommandLineProgressReporter.h"
#include <stdio.h>

namespace itk
{

//----------------------------------------------------------------
LesionSegmentationCommandLineProgressReporter::LesionSegmentationCommandLineProgressReporter()
{
  this->ReportProgress = 1;
  this->ProgressValue = 0;
  this->ProgressString = " ";
}

//----------------------------------------------------------------
LesionSegmentationCommandLineProgressReporter::~LesionSegmentationCommandLineProgressReporter()
{
}

//----------------------------------------------------------------
void LesionSegmentationCommandLineProgressReporter
::Execute( Object *caller, const EventObject &event)
{
  this->ExecuteInternal( caller, event );
}

//----------------------------------------------------------------
void LesionSegmentationCommandLineProgressReporter
::Execute( const Object *caller, const EventObject &event)
{
  this->ExecuteInternal( caller, event );
}

//----------------------------------------------------------------
void LesionSegmentationCommandLineProgressReporter
::ExecuteInternal(const Object *caller, const EventObject &event)
{
  bool report = false;
  typedef LesionSegmentationImageFilter8<
        Image< short, 3 >, Image< float, 3 > > FilterType;
  if (const FilterType * filter
       = dynamic_cast< const FilterType *>(caller))
    {
    if ( typeid( event ) == typeid( ProgressEvent ) )
      {
      const int progressValue = static_cast< int >( filter->GetProgress() * 100.0 );
      if (const char *statusText = filter->GetStatusMessage())
        {
        this->ProgressString = statusText;
        }
      if (fabs((double)(progressValue - this->ProgressValue)) >= 1.0)
        {
        this->ProgressValue = static_cast< int >( filter->GetProgress() * 100.0 );
        report = true;
        }
      if (report)
        {
        printf("%s - Progress %d%s",this->ProgressString.c_str(),this->ProgressValue,"%");
        printf("\r");
        fflush(stdout);
        }
      }
    }
}

}
