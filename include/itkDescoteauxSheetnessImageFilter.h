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
#ifndef itkDescoteauxSheetnessImageFilter_h
#define itkDescoteauxSheetnessImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "itkMath.h"

namespace itk
{

/** \class DescoteauxSheetnessImageFilter
 *
 * \brief Computes a measure of Sheetness from the Hessian Eigenvalues
 *
 * Based on the "Sheetness" measure proposed by Decouteaux et. al.
 *
 * M.Descoteaux, M.Audette, K.Chinzei, el al.:
 * "Bone enhancement filtering: Application to sinus bone segmentation
 *  and simulation of pituitary surgery."
 *  In: MICCAI.  (2005) 9-16
 *
 * \ingroup IntensityImageFilters  Multithreaded
 * \ingroup LesionSizingToolkit
 */
namespace Function
{

template <typename TInput, typename TOutput>
class Sheetness
{
public:
  Sheetness()
  {
    m_Alpha = 0.5; // suggested value in the paper
    m_Gamma = 0.5; // suggested value in the paper;
    m_C = 1.0;
    m_DetectBrightSheets = true;
  }
  ~Sheetness() = default;
  bool
  operator!=(const Sheetness &) const
  {
    return false;
  }
  bool
  operator==(const Sheetness & other) const
  {
    return !(*this != other);
  }
  inline TOutput
  operator()(const TInput & A)
  {
    double sheetness = 0.0;

    auto a1 = static_cast<double>(A[0]);
    auto a2 = static_cast<double>(A[1]);
    auto a3 = static_cast<double>(A[2]);

    double l1 = itk::Math::abs(a1);
    double l2 = itk::Math::abs(a2);
    double l3 = itk::Math::abs(a3);

    //
    // Sort the values by their absolute value.
    // At the end of the sorting we should have
    //
    //          l1 <= l2 <= l3
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

    if (this->m_DetectBrightSheets)
    {
      if (a3 > 0.0)
      {
        return static_cast<TOutput>(sheetness);
      }
    }
    else
    {
      if (a3 < 0.0)
      {
        return static_cast<TOutput>(sheetness);
      }
    }


    //
    // Avoid divisions by zero (or close to zero)
    //
    if (static_cast<double>(l3) < itk::Math::eps)
    {
      return static_cast<TOutput>(sheetness);
    }

    const double Rs = l2 / l3;
    const double Rb = itk::Math::abs(l3 + l3 - l2 - l1) / l3;
    const double Rn = std::sqrt(l3 * l3 + l2 * l2 + l1 * l1);

    sheetness = std::exp(-(Rs * Rs) / (2.0 * m_Alpha * m_Alpha));
    sheetness *= (1.0 - std::exp(-(Rb * Rb) / (2.0 * m_Gamma * m_Gamma)));
    sheetness *= (1.0 - std::exp(-(Rn * Rn) / (2.0 * m_C * m_C)));

    return static_cast<TOutput>(sheetness);
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
  void
  SetC(double value)
  {
    this->m_C = value;
  }
  void
  SetDetectBrightSheets(bool value)
  {
    this->m_DetectBrightSheets = value;
  }
  void
  SetDetectDarkSheets(bool value)
  {
    this->m_DetectBrightSheets = !value;
  }

private:
  double m_Alpha;
  double m_Gamma;
  double m_C;
  bool   m_DetectBrightSheets;
};
} // namespace Function

template <typename TInputImage, typename TOutputImage>
class ITK_TEMPLATE_EXPORT DescoteauxSheetnessImageFilter
  : public UnaryFunctorImageFilter<
      TInputImage,
      TOutputImage,
      Function::Sheetness<typename TInputImage::PixelType, typename TOutputImage::PixelType>>
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(DescoteauxSheetnessImageFilter);

  /** Standard class type alias. */
  using Self = DescoteauxSheetnessImageFilter;
  using Superclass =
    UnaryFunctorImageFilter<TInputImage,
                            TOutputImage,
                            Function::Sheetness<typename TInputImage::PixelType, typename TOutputImage::PixelType>>;
  using Pointer = SmartPointer<Self>;
  using ConstPointer = SmartPointer<const Self>;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(DescoteauxSheetnessImageFilter, UnaryFunctorImageFilter);

  /** Set the normalization term for sheetness */
  void
  SetSheetnessNormalization(double value)
  {
    this->GetFunctor().SetAlpha(value);
  }

  /** Set the normalization term for bloobiness. */
  void
  SetBloobinessNormalization(double value)
  {
    this->GetFunctor().SetGamma(value);
  }

  /** Set the normalization term for noise. */
  void
  SetNoiseNormalization(double value)
  {
    this->GetFunctor().SetC(value);
  }
  void
  SetDetectBrightSheets(bool value)
  {
    this->GetFunctor().SetDetectBrightSheets(value);
  }
  void
  SetDetectDarkSheets(bool value)
  {
    this->GetFunctor().SetDetectDarkSheets(value);
  }

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  using InputPixelType = typename TInputImage::PixelType;
  itkConceptMacro(BracketOperatorsCheck, (Concept::BracketOperator<InputPixelType, unsigned int, double>));
  itkConceptMacro(DoubleConvertibleToOutputCheck, (Concept::Convertible<double, typename TOutputImage::PixelType>));
  /** End concept checking */
#endif

protected:
  DescoteauxSheetnessImageFilter() = default;
  ~DescoteauxSheetnessImageFilter() override = default;
};

} // end namespace itk


#endif
