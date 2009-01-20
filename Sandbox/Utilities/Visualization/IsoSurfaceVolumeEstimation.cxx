/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    IsoSurfaceVolumeEstimation.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "vtkMetaImageReader.h"
#include "vtkContourFilter.h"
#include "vtkImageData.h"
#include "vtkPolyDataWriter.h"
#include "vtkSTLWriter.h"
#include "vtkSmartPointer.h"
#include "vtksys/SystemTools.hxx"
#include "vtkMassProperties.h"
#include "vnl/vnl_math.h"


#define VTK_CREATE(type, name) \
  vtkSmartPointer<type> name = vtkSmartPointer<type>::New()


int main(int argc, char * argv [] )
{  

  if( argc < 7 )
    {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << "Usage: " << argv[0] << " imageFileName isoValue ";
    std::cerr << " MethodID DatasetID ExpectedVolume ouputTextFile " << std::endl;
    std::cerr << std::endl;
    return 1;
    }
  
  VTK_CREATE( vtkMetaImageReader, imageReader );

  imageReader->SetFileName( argv[1] );
  imageReader->Update();
  
  float isoValue = atof( argv[2] );

  VTK_CREATE( vtkContourFilter, contourFilter );

  contourFilter->SetValue( 0, isoValue ); 
  contourFilter->SetInput( imageReader->GetOutput() );


  VTK_CREATE( vtkMassProperties, massProperties );
  massProperties->SetInput( contourFilter->GetOutput() );
  double volume = massProperties->GetVolume();

  std::cout << "Volume = " << volume << " mm3" << std::endl;

  //
  // Compute the radius of the equivalent-volume sphere
  //
  const double radius3 = ( ( volume * 3.0 ) / ( 4.0 * vnl_math::pi ) );
  const double radius = vnl_math_cuberoot( radius3 );


  const std::string segmentationMethodID = argv[3];
  const std::string datasetID = argv[4];
  const double  expectedVolume = atof( argv[5] );
  const std::string outpuFileName = argv[6];

  const double volumeDifference = expectedVolume - volume;

  const double errorPercent = volumeDifference / expectedVolume * 100.0;

  //
  // Append the value to the file containing estimations for this dataset.
  //
  std::ofstream ouputFile;

  ouputFile.open( outpuFileName.c_str(), std::ios_base::app );

  ouputFile << segmentationMethodID << "  ";
  ouputFile << datasetID << "  ";
  ouputFile << expectedVolume << "   ";
  ouputFile << volume << "   ";
  ouputFile << errorPercent << "   ";
  ouputFile << radius << std::endl;

  ouputFile.close();

  return EXIT_SUCCESS;
}
