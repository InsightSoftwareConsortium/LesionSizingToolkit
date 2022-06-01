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
#include "itkImage.h"

int
main(int argc, char ** argv)
{

  RegisterRequiredFactories();

  // Verify the number of parameters in the command line
  if (argc < 3)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile  outputImageFile " << std::endl;
    return EXIT_FAILURE;
  }

  using PixelType = unsigned char;
  constexpr unsigned int Dimension = 3;

  using ImageType = itk::Image<PixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<ImageType>;
  using WriterType = itk::ImageFileWriter<ImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  const char * inputFilename = argv[1];
  const char * outputFilename = argv[2];

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);

  writer->SetInput(reader->GetOutput());
  writer->UseCompressionOn();

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
