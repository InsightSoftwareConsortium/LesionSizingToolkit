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
#ifndef itkLocalStructureImageFilter_h
#define itkLocalStructureImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkMath.h"

namespace itk
{

/** \class LocalStructureImageFilter
 *
 * \brief Computes local similarity to geometrical structures using second
 * derivative operations.
 *
 * Based on the paper
 *
 *      "Tissue Classification Based on 3D Local Intensity Structures for
 *      Volume Rendering".
 *
 *  by
 *
 *      Y. Sato, C-F. Westin, A. Bhalerao, S. Nakajima,
 *      N. Shiraga, S. Tamura, R. Kikinis.
 *
 * IEEE Transactions on Visualization and Computer Graphics
 * Vol 6. No. 2. April-June 2000.
 *
 * \ingroup IntensityImageFilters  Multithreaded
 * \ingroup LesionSizingToolkit
 */
namespace Function
{

template <typename TInput, typename TOutput>
class LocalStructure
{
public:
  LocalStructure()
  {
    m_Alpha = 0.25; // suggested value in the paper
    m_Gamma = 0.50; // suggested value in the paper;
  }
  ~LocalStructure() = default;
  bool
  operator!=(const LocalStructure &) const
  {
    return false;
  }
  bool
  operator==(const LocalStructure & other) const
  {
    return !(*this != other);
  }
  inline TOutput
  operator()(const TInput & A)
  {
    auto a1 = static_cast<double>(A[0]);
    auto a2 = static_cast<double>(A[1]);
    auto a3 = static_cast<double>(A[2]);

    double l1 = itk::Math::abs(a1);
    double l2 = itk::Math::abs(a2);
    double l3 = itk::Math::abs(a3);

    //
    // Sort the values by their absolute value.
    //
    if (l2 > l3)
    {
      double tmpl = l3;
      l3 = l2;
      l2 = tmpl;
      double tmpa = a3;
      a3 = a2;
      a2 = tmpa;
    }

    if (l1 > l2)
    {
      double tmp = l1;
      l1 = l2;
      l2 = tmp;
      double tmpa = a1;
      a1 = a2;
      a2 = tmpa;
    }

    if (l2 > l3)
    {
      double tmp = l3;
      l3 = l2;
      l2 = tmp;
      double tmpa = a3;
      a3 = a2;
      a2 = tmpa;
    }

    //
    // Avoid divisions by zero.
    //
    if (l3 < itk::Math::eps)
    {
      return 0.0;
    }

    const double L3 = itk::Math::abs(static_cast<double>(a3));
    const double W = WeightFunctionOmega(a2, a3);
    const double F = WeightFunctionOmega(a1, a3);

    const double sheetness = L3 * W * F;

    return static_cast<TOutput>(sheetness);
  }
  inline double
  WeightFunctionOmega(double ls, double lt) const
  {
    if (ls <= 0.0 && lt <= ls)
    {
      return (1 + std::pow(ls / itk::Math::abs(lt), m_Gamma));
    }
    const double abslt = itk::Math::abs(lt);
    if (ls > 0.0 && abslt / m_Gamma > ls)
    {
      return std::pow(1 - m_Alpha * ls / itk::Math::abs(lt), m_Gamma);
    }
    return 0.0;
  }
  inline double
  WeightFunctionPhi(double ls, double lt) const
  {
    if (ls < 0.0 && lt <= ls)
    {
      return std::pow((ls / lt), m_Gamma);
    }
    return 0.0;
  }

  void
  SetAlpha(double value)
  {
    this->m_Alpha = value;
  }

  void
  SetGamma(double value)
  {
    this->m_Gamma = value;
  }

private:
  double m_Alpha;
  double m_Gamma;
};
} // namespace Function

template <typename TInputImage, typename TOutputImage>
class ITK_TEMPLATE_EXPORT LocalStructureImageFilter
  : public UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Function::LocalStructure<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(LocalStructureImageFilter);

  /** Standard class type alias. */
  using Self = LocalStructureImageFilter;
  using Superclass = UnaryFunctorImageFilter<
    TInputImage,
    TOutputImage,
    Function::LocalStructure<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(LocalStructureImageFilter, UnaryFunctorImageFilter);

  /** Set the normalization term for sheetness */
  void
  SetAlpha(double value)
  {
    this->GetFunctor().SetAlpha(value);
  }

  /** Set the normalization term for bloobiness. */
  void
  SetGamma(double value)
  {
    this->GetFunctor().SetGamma(value);
  }


#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  using InputPixelType = typename TInputImage::PixelType;
  itkConceptMacro(BracketOperatorsCheck, (Concept::BracketOperator<InputPixelType, unsigned int, double>));
  itkConceptMacro(DoubleConvertibleToOutputCheck, (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  LocalStructureImageFilter() = default;
  ~LocalStructureImageFilter() override = default;
};

} // end namespace itk


#endif
