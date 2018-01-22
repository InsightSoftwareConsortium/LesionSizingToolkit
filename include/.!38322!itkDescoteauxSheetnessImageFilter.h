/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkDescoteauxSheetnessImageFilter.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef itkDescoteauxSheetnessImageFilter_h
#define itkDescoteauxSheetnessImageFilter_h

#include "itkUnaryFunctorImageFilter.h"
#include "vnl/vnl_math.h"

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
