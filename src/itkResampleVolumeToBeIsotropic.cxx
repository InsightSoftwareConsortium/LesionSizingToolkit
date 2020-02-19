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
#if defined(_MSC_VER)
#  pragma warning(disable : 4786)
#endif


#include "itkIncludeRequiredIOFactories.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkIdentityTransform.h"

#include "itkBSplineInterpolateImageFunction.h"
#include "itkWindowedSincInterpolateImageFunction.h"

#include <string>


int
main(int argc, char * argv[])
{

  RegisterRequiredFactories();

  if (argc < 5)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile finalSpacing ";
    std::cerr << " interpolatorType(0:BSpline 1:WindowedSinc 2:Linear)" << std::endl;
    return EXIT_FAILURE;
  }


  using PixelType = signed short;
  constexpr unsigned int Dimension = 3;

  using ImageType = itk::Image<PixelType, Dimension>;

  using ReaderType = itk::ImageFileReader<ImageType>;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  try
  {
    reader->Update();
  }
  catch (itk::ExceptionObject & excep)
  {
    std::cerr << "Exception caught!" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
  }


  ImageType::ConstPointer inputImage = reader->GetOutput();

  using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType>;

  ResampleFilterType::Pointer resampler = ResampleFilterType::New();

  using TransformType = itk::IdentityTransform<double, Dimension>;

  TransformType::Pointer transform = TransformType::New();
  transform->SetIdentity();

  resampler->SetTransform(transform);


  using BSplineInterpolatorType = itk::BSplineInterpolateImageFunction<ImageType, double>;

  BSplineInterpolatorType::Pointer bsplineInterpolator = BSplineInterpolatorType::New();

#ifdef ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE
  bsplineInterpolator->UseImageDirectionOn();
#endif
  bsplineInterpolator->SetSplineOrder(3);


  using WindowedSincInterpolatorType = itk::WindowedSincInterpolateImageFunction<ImageType, 3>;

  WindowedSincInterpolatorType::Pointer windowedSincInterpolator = WindowedSincInterpolatorType::New();

  using LinearInterpolatorType = itk::LinearInterpolateImageFunction<ImageType, double>;

  LinearInterpolatorType::Pointer linearInterpolator = LinearInterpolatorType::New();


  switch (std::stoi(argv[4]))
  {
    case 0:
      resampler->SetInterpolator(bsplineInterpolator);
      break;
    case 1:
      resampler->SetInterpolator(windowedSincInterpolator);
      break;
    case 2:
      resampler->SetInterpolator(linearInterpolator);
      break;
  }


  resampler->SetDefaultPixelValue(-1024); // Hounsfield Units for Air

  const ImageType::SpacingType & inputSpacing = inputImage->GetSpacing();

  double minSpacing = itk::NumericTraits<double>::max();
  for (int i = 0; i < 3; i++)
  {
    minSpacing = (minSpacing > inputSpacing[i] ? inputSpacing[i] : minSpacing);
  }

  ImageType::SpacingType outputSpacing;

  const double finalSpacing = (strcmp(argv[3], "-minspacing") == 0) ? minSpacing : std::stod(argv[3]);

  outputSpacing[0] = finalSpacing;
  outputSpacing[1] = finalSpacing;
  outputSpacing[2] = finalSpacing;

  resampler->SetOutputSpacing(outputSpacing);


  resampler->SetOutputOrigin(inputImage->GetOrigin());
  resampler->SetOutputDirection(inputImage->GetDirection());

  ImageType::SizeType inputSize = inputImage->GetLargestPossibleRegion().GetSize();

  using SizeValueType = ImageType::SizeType::SizeValueType;

  const double dx = inputSize[0] * inputSpacing[0] / outputSpacing[0];
  const double dy = inputSize[1] * inputSpacing[1] / outputSpacing[1];
  const double dz = inputSize[2] * inputSpacing[2] / outputSpacing[2];

  ImageType::SizeType finalSize;

  finalSize[0] = static_cast<SizeValueType>(dx);
  finalSize[1] = static_cast<SizeValueType>(dy);
  finalSize[2] = static_cast<SizeValueType>(dz);

  resampler->SetSize(finalSize);

  resampler->SetInput(inputImage);

  using WriterType = itk::ImageFileWriter<ImageType>;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[2]);
  writer->SetInput(resampler->GetOutput());
  writer->UseCompressionOn();

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject & excep)
  {
    std::cerr << "Exception caught !" << std::endl;
    std::cerr << excep << std::endl;
  }

  return EXIT_SUCCESS;
}
