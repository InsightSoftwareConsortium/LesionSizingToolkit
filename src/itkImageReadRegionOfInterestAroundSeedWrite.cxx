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
#if defined(_MSC_VER)
#  pragma warning(disable : 4786)
#endif

#ifdef __BORLANDC__
#  define ITK_LEAN_AND_MEAN
#endif


#include "itkIncludeRequiredIOFactories.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkLandmarksReader.h"
#include "itkEllipseSpatialObject.h"
#include "itkImage.h"

#include <string>


int
main(int argc, char ** argv)
{

  RegisterRequiredFactories();

  // Verify the number of parameters in the command line
  if (argc < 5)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    std::cerr << " landmarksFile radius" << std::endl;
    return EXIT_FAILURE;
  }


  using InputPixelType = signed short;
  using OutputPixelType = signed short;
  constexpr unsigned int Dimension = 3;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<InputImageType>;
  using WriterType = itk::ImageFileWriter<OutputImageType>;

  using FilterType = itk::RegionOfInterestImageFilter<InputImageType, OutputImageType>;

  using LandmarksReaderType = itk::LandmarksReader<Dimension>;

  LandmarksReaderType::Pointer landmarksReader = LandmarksReaderType::New();

  landmarksReader->SetFileName(argv[3]);
  landmarksReader->Update();

  using InputSpatialObjectType = itk::LandmarkSpatialObject<Dimension>;
  const InputSpatialObjectType * inputSeeds = landmarksReader->GetOutput();
  const unsigned int             numberOfPoints = inputSeeds->GetNumberOfPoints();

  if (numberOfPoints < 1)
  {
    std::cerr << "Seed points file is empty !" << std::endl;
    return EXIT_FAILURE;
  }

  using LandmarkPointListType = InputSpatialObjectType::LandmarkPointListType;

  const LandmarkPointListType & points = inputSeeds->GetPoints();

  InputImageType::PointType seedPoint = points[0].GetPositionInObjectSpace();

  FilterType::Pointer filter = FilterType::New();

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  filter->SetInput(reader->GetOutput());
  writer->SetInput(filter->GetOutput());
  writer->UseCompressionOn();

  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }

  const InputImageType * inputImage = reader->GetOutput();

  InputImageType::IndexType centralIndex = inputImage->TransformPhysicalPointToIndex(seedPoint);

  InputImageType::SpacingType spacing = inputImage->GetSpacing();

  InputImageType::IndexType originIndex;

  const double radius = std::stod(argv[4]);

  originIndex[0] = centralIndex[0] - radius / spacing[0];
  originIndex[1] = centralIndex[1] - radius / spacing[1];
  originIndex[2] = centralIndex[2] - radius / spacing[2];

  InputImageType::SizeType regionSize;

  regionSize[0] = 2.0 * radius / spacing[0];
  regionSize[1] = 2.0 * radius / spacing[1];
  regionSize[2] = 2.0 * radius / spacing[2];

  OutputImageType::RegionType desiredRegion;

  desiredRegion.SetIndex(originIndex);
  desiredRegion.SetSize(regionSize);

  desiredRegion.PadByRadius(2);

  desiredRegion.Crop(inputImage->GetBufferedRegion());

  filter->SetRegionOfInterest(desiredRegion);
  std::cout << "Desired region: " << desiredRegion << std::endl;
  std::cout << "ImageLargestPossibleRegion: " << inputImage->GetLargestPossibleRegion() << std::endl;
  std::cout << "Seed is at : " << seedPoint << " Index: " << centralIndex << std::endl;

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    std::cerr << "ExceptionObject caught !" << std::endl;
    std::cerr << err << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
