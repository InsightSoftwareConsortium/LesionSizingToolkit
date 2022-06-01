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

#include "itkIncludeRequiredIOFactories.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLaplacianRecursiveGaussianImageFilter.h"

#include <string>


int
main(int argc, char * argv[])
{

  RegisterRequiredFactories();

  if (argc < 4)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile  sigma " << std::endl;
    return EXIT_FAILURE;
  }

  using InputPixelType = float;
  using OutputPixelType = float;

  constexpr unsigned int Dimension = 3;

  using InputImageType = itk::Image<InputPixelType, Dimension>;
  using OutputImageType = itk::Image<OutputPixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<InputImageType>;

  using FilterType = itk::LaplacianRecursiveGaussianImageFilter<InputImageType, OutputImageType>;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  FilterType::Pointer laplacian = FilterType::New();

  laplacian->SetNormalizeAcrossScale(false);

  laplacian->SetInput(reader->GetOutput());

  const double sigma = std::stod(argv[3]);

  laplacian->SetSigma(sigma);

  try
  {
    laplacian->Update();
  }
  catch (itk::ExceptionObject & err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  using WriterType = itk::ImageFileWriter<OutputImageType>;

  WriterType::Pointer writer = WriterType::New();

  writer->SetInput(laplacian->GetOutput());
  writer->SetFileName(argv[2]);
  writer->Update();

  return EXIT_SUCCESS;
}
