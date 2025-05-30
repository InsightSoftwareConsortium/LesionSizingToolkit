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
#ifndef itkVesselEnhancingDiffusion3DImageFilter_h
#define itkVesselEnhancingDiffusion3DImageFilter_h

#include "itkImageToImageFilter.h"
#include <vector>

namespace itk
{

/** \class VesselEnhancingDiffusion3DImageFilter
 *
 * Complete rewrite of previous versions, only using itk/vnl routines
 * for derivatives, eigensystem calculations and diffusion. Internally,
 * the input image image is converted to internalprecision (float) for
 * calculation, and converted back when returning the results.
 *
 * Uses simple forward Euler scheme (explicit) with 3x3x3 stencil,
 * see eg phd of Joachim Weickert for theory and implementation regarding
 * the construction of this discretization scheme. See 'Vessel Enhancing
 * Diffusion', Manniesing, media 2006, for information regarding the
 * construction of the diffusion tensor.
 *
 * - Stores all elements of the Hessian of the complete image during
 *   diffusion. An alternative implementation is to only store the
 *   scale for which the vesselness has maximum response, and to
 *   recalculate the hessian (locally) during diffusion. Also stores
 *   the current image, ie at iteration i + temp image, therefore the complete
 *   memory consumption approximately peaks at 8 times the input image
 *   (input image in float)
 * - The hessian is stored as six individual images, an alternative
 *   implementation is to use the itk symmetric second rank tensor
 *   as pixeltype (and eg using the class SymmetricEigenAnalysisImage
 *   Filter). However, we are lazy, and using this since we rely
 *   on vnl datatypes and its eigensystem calculations
 * - note: most of computation time is spent at calculation of vesselness
 *   response
 *
 *   9 feb 2009
 *      changed imagetype to precisionimage type of function call,
 *      thanks to Mark Rabotnikov
 *
 * - PixelType      short, 3D
 *   Precision      float, 3D
 *
 *
 * - todo
 *   - using parallelism/threading eg over scales
 *   - completely itk-fying, eg eigenvalues calculation
 *   - possibly embedding within itk-diffusion framework
 *   - itk expert to have a look at use of iterators
 *     (there must be a potential gain there)
 *
 * email: r.manniesing@erasmusmc.nl
 *
 * \ingroup LesionSizingToolkit
 */
template <typename PixelType = short int, unsigned int NDimension = 3>
class ITK_TEMPLATE_EXPORT VesselEnhancingDiffusion3DImageFilter
  : public ImageToImageFilter<Image<PixelType, NDimension>, Image<PixelType, NDimension>>
{

public:
  ITK_DISALLOW_COPY_AND_MOVE(VesselEnhancingDiffusion3DImageFilter);

  using Precision = float;
  using ImageType = Image<PixelType, NDimension>;
  using PrecisionImageType = Image<Precision, NDimension>;

  using Self = VesselEnhancingDiffusion3DImageFilter;
  using Superclass = ImageToImageFilter<ImageType, ImageType>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  itkNewMacro(Self);
  itkOverrideGetNameOfClassMacro(VesselEnhancingDiffusion3DImageFilter);

  itkSetMacro(TimeStep, Precision);
  itkSetMacro(Iterations, unsigned int);
  itkSetMacro(RecalculateVesselness, unsigned int);

  itkSetMacro(Alpha, Precision);
  itkSetMacro(Beta, Precision);
  itkSetMacro(Gamma, Precision);

  itkSetMacro(Epsilon, Precision);
  itkSetMacro(Omega, Precision);
  itkSetMacro(Sensitivity, Precision);

  void
  SetScales(const std::vector<Precision> scales)
  {
    m_Scales = scales;
  }
  itkBooleanMacro(DarkObjectLightBackground);
  itkSetMacro(DarkObjectLightBackground, bool);
  itkBooleanMacro(Verbose);
  itkSetMacro(Verbose, bool);

  // some defaults for lowdose example
  // used in the paper
  void
  SetDefaultPars()
  {
    m_TimeStep = 0.001;
    m_Iterations = 30;
    m_RecalculateVesselness = 100;
    m_Alpha = 0.5;
    m_Beta = 0.5;
    m_Gamma = 5.0;
    m_Epsilon = 0.01;
    m_Omega = 25.0;
    m_Sensitivity = 5.0;

    m_Scales.resize(5);
    m_Scales[0] = 0.300;
    m_Scales[1] = 0.482;
    m_Scales[2] = 0.775;
    m_Scales[3] = 1.245;
    m_Scales[4] = 2.000;

    m_DarkObjectLightBackground = false;
    m_Verbose = true;
  }

protected:
  VesselEnhancingDiffusion3DImageFilter();
  ~VesselEnhancingDiffusion3DImageFilter() override = default;
  void
  PrintSelf(std::ostream & os, Indent indent) const override;
  void
  GenerateData() override;

private:
  Precision              m_TimeStep;
  unsigned int           m_Iterations{ 0 };
  unsigned int           m_RecalculateVesselness{ 0 };
  Precision              m_Alpha;
  Precision              m_Beta;
  Precision              m_Gamma;
  Precision              m_Epsilon{ 0.0 };
  Precision              m_Omega{ 0.0 };
  Precision              m_Sensitivity{ 0.0 };
  std::vector<Precision> m_Scales;
  bool                   m_DarkObjectLightBackground{ false };
  bool                   m_Verbose;
  unsigned int           m_CurrentIteration;

  // current hessian for which we have max vesselresponse
  typename PrecisionImageType::Pointer m_Dxx;
  typename PrecisionImageType::Pointer m_Dxy;
  typename PrecisionImageType::Pointer m_Dxz;
  typename PrecisionImageType::Pointer m_Dyy;
  typename PrecisionImageType::Pointer m_Dyz;
  typename PrecisionImageType::Pointer m_Dzz;

  void VED3DSingleIteration(typename PrecisionImageType::Pointer);

  // Calculates maxvessel response of the range
  // of scales and stores the hessian of each voxel
  // into the member images m_Dij.
  void
  MaxVesselResponse(const typename PrecisionImageType::Pointer);

  // calculates diffusion tensor
  // based on current values of hessian (for which we have
  // maximim vessel response).
  void
  DiffusionTensor();

  // Sorted magnitude increasing
  inline Precision VesselnessFunction3D(Precision, Precision, Precision);
};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkVesselEnhancingDiffusion3DImageFilter.hxx"
#endif

#endif
