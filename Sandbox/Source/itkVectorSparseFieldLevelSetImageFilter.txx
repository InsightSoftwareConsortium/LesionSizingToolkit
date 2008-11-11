/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVectorSparseFieldLevelSetImageFilter.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVectorSparseFieldLevelSetImageFilter_txx
#define __itkVectorSparseFieldLevelSetImageFilter_txx

#include "itkVectorSparseFieldLevelSetImageFilter.h"
#include "itkVectorZeroCrossingImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkVectorShiftScaleImageFilter.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkMeasurementVectorTraits.h"

namespace itk {

template <class TNeighborhoodType>
SparseFieldCityBlockNeighborList<TNeighborhoodType>
::SparseFieldCityBlockNeighborList()
{
  typedef typename NeighborhoodType::ImageType ImageType;
  typename ImageType::Pointer dummy_image = ImageType::New();

  unsigned int i, nCenter;
  int d;
  OffsetType zero_offset;

  for (i = 0; i < Dimension; ++i)
    {
    m_Radius[i] = 1;
    zero_offset[i] = 0;
    }
  NeighborhoodType it(m_Radius, dummy_image, dummy_image->GetRequestedRegion());
  nCenter = it.Size() / 2;

  m_Size = 2 * Dimension;
  m_ArrayIndex.reserve(m_Size);
  m_NeighborhoodOffset.reserve(m_Size);

  for (i = 0; i < m_Size; ++i)
    {
    m_NeighborhoodOffset.push_back(zero_offset);
    }

  for (d = Dimension - 1, i = 0; d >= 0; --d, ++i)
    {
    m_ArrayIndex.push_back( nCenter - it.GetStride(d) );
    m_NeighborhoodOffset[i][d] = -1;
    }
  for (d = 0; d < static_cast<int>(Dimension); ++d, ++i)
    {
    m_ArrayIndex.push_back( nCenter + it.GetStride(d) );
    m_NeighborhoodOffset[i][d] = 1;
    }

  for (i = 0; i < Dimension; ++i)
    {
    m_StrideTable[i] = it.GetStride(i);
    }
}

template <class TNeighborhoodType>
void
SparseFieldCityBlockNeighborList<TNeighborhoodType>
::Print(std::ostream &os) const
{
  os << "SparseFieldCityBlockNeighborList: " << std::endl;
  for (unsigned i = 0; i < this->GetSize(); ++i)
    {
    os << "m_ArrayIndex[" << i << "]: " << m_ArrayIndex[i] << std::endl;
    os << "m_NeighborhoodOffset[" << i << "]: " << m_NeighborhoodOffset[i]
       << std::endl;
    }
}

//template<class TInputImage, class TOutputImage>
//double VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
//::m_ConstantGradientValue = 1.0;

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::ScalarValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_ScalarValueOne = NumericTraits<ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage,
                                                             TOutputImage>::ScalarValueType >::One;

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::ScalarValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_ScalarValueZero = NumericTraits<ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage,
                                                             TOutputImage>::ScalarValueType >::Zero;

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::StatusValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_StatusNullValue = NumericTraits<ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage,
                                                 TOutputImage>::StatusValueType >::NonpositiveMin();

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::StatusValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_StatusChangingValue = -1;

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::StatusValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_StatusActiveChangingUpValue = -2;

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::StatusValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_StatusActiveChangingDownValue = -3;

template<class TInputImage, class TOutputImage>
ITK_TYPENAME VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::StatusValueType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::m_StatusBoundaryPixelValue = -4;

template<class TInputImage, class TOutputImage>
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::VectorSparseFieldLevelSetImageFilter()
{
  m_IsoSurfaceValue = m_ValueZero;
  m_NumberOfLayers = ImageDimension;
  m_LayerNodeStore = LayerNodeStorageType::New();
  m_LayerNodeStore->SetGrowthStrategyToExponential();
  this->SetRMSChange(static_cast<double>(m_ScalarValueZero));
  m_InterpolateSurfaceLocation = true;
  m_BoundsCheckingActive = false;
  m_ConstantGradientValue = 1.0;
}

template<class TInputImage, class TOutputImage>
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::~VectorSparseFieldLevelSetImageFilter()
{}

template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::ApplyUpdate(TimeStepType dt)
{

  for( unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
    {
    unsigned int j;
    unsigned int k;
    unsigned int t;

    StatusValueType up_to;
    StatusValueType up_search;
    StatusValueType down_to;
    StatusValueType down_search;

    LayerPointerType UpList[2];
    LayerPointerType DownList[2];

    for (unsigned int i = 0; i < 2; ++i)
      {
      UpList[i]   = LayerType::New();
      DownList[i] = LayerType::New();
      }

    // Process the active layer.  This step will update the values in the active
    // layer as well as the values at indicies that *will* become part of the
    // active layer when they are promoted/demoted.  Also records promotions,
    // demotions in the m_StatusLayer for current active layer indicies
    // (i.e. those indicies which will move inside or outside the active
    // layers).
    this->UpdateActiveLayerValues(dt, UpList[0], DownList[0], phase);

    // Process the status up/down lists.  This is an iterative process which
    // proceeds outwards from the active layer.  Each iteration generates the
    // list for the next iteration.

    // First process the status lists generated on the active layer.
    this->ProcessStatusList(UpList[0], UpList[1], 2, 1, phase);
    this->ProcessStatusList(DownList[0], DownList[1], 1, 2, phase);

    down_to = up_to = 0;
    up_search       = 3;
    down_search     = 4;
    j = 1;
    k = 0;

    LayerListType & layers = this->m_LayersPhases[phase];

    while( down_search < static_cast<StatusValueType>( layers.size() ) )
      {
      this->ProcessStatusList(UpList[j], UpList[k], up_to, up_search, phase);
      this->ProcessStatusList(DownList[j], DownList[k], down_to, down_search, phase);

      if (up_to == 0) up_to += 1;
      else            up_to += 2;
      down_to += 2;

      up_search += 2;
      down_search += 2;

      // Swap the lists so we can re-use the empty one.
      t = j;
      j = k;
      k = t;
      }

    // Process the outermost inside/outside layers in the sparse field.
    this->ProcessStatusList(UpList[j], UpList[k], up_to, m_StatusNullValue, phase);
    this->ProcessStatusList(DownList[j], DownList[k], down_to, m_StatusNullValue, phase);

    // Now we are left with the lists of indicies which must be
    // brought into the outermost layers.  Bring UpList into last inside layer
    // and DownList into last outside layer.
    this->ProcessOutsideList(UpList[k], static_cast<int>( layers.size()) -2, phase);
    this->ProcessOutsideList(DownList[k], static_cast<int>( layers.size()) -1, phase);
    }

  // Finally, we update all of the layer values (excluding the active layer,
  // which has already been updated).
  this->PropagateAllLayerValues();
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::ProcessOutsideList(LayerType *OutsideList, StatusValueType ChangeToStatus, unsigned int phase)
{
  LayerNodeType *node;

  LayerListType & layers = this->m_LayersPhases[phase];

  // Push each index in the input list into its appropriate status layer
  // (ChangeToStatus) and update the status image value at that index.
  while ( ! OutsideList->Empty() )
    {
    m_StatusImage->SetPixel( OutsideList->Front()->m_Value, ChangeToStatus );
    node = OutsideList->Front();
    OutsideList->PopFront();
    layers[ChangeToStatus]->PushFront(node);
    }
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::ProcessStatusList(LayerType *InputList, LayerType *OutputList,
                    StatusValueType ChangeToStatus, StatusValueType SearchForStatus,
                    unsigned int phase)
{
  unsigned int i;
  bool bounds_status;
  LayerNodeType *node;
  StatusValueType neighbor_status;
  NeighborhoodIterator<StatusImageType>
    statusIt(m_NeighborList.GetRadius(), m_StatusImage,
             this->GetOutput()->GetRequestedRegion());

  if (m_BoundsCheckingActive == false )
    {
    statusIt.NeedToUseBoundaryConditionOff();
    }

  LayerListType & layers = this->m_LayersPhases[phase];

  // Push each index in the input list into its appropriate status layer
  // (ChangeToStatus) and update the status image value at that index.
  // Also examine the neighbors of the index to determine which need to go onto
  // the output list (search for SearchForStatus).
  while ( ! InputList->Empty() )
    {
    statusIt.SetLocation(InputList->Front()->m_Value);
    statusIt.SetCenterPixel(ChangeToStatus);

    node = InputList->Front();  // Must unlink from the input list
    InputList->PopFront();      // _before_ transferring to another list.
    layers[ChangeToStatus]->PushFront(node);

    for (i = 0; i < m_NeighborList.GetSize(); ++i)
      {
      neighbor_status = statusIt.GetPixel(m_NeighborList.GetArrayIndex(i))[phase];

      // Have we bumped up against the boundary?  If so, turn on bounds
      // checking.
      if ( neighbor_status == m_StatusBoundaryPixel[phase] )
        {
        m_BoundsCheckingActive = true;
        }

      if (neighbor_status == SearchForStatus)
        { // mark this pixel so we don't add it twice.
        statusIt.SetPixel(m_NeighborList.GetArrayIndex(i),
                          m_StatusChanging, bounds_status);
        if (bounds_status == true)
          {
          node = m_LayerNodeStore->Borrow();
          node->m_Value = statusIt.GetIndex() +
            m_NeighborList.GetNeighborhoodOffset(i);
          OutputList->PushFront( node );
          } // else this index was out of bounds.
        }
      }
    }
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::UpdateActiveLayerValues(TimeStepType dt,
                          LayerType *UpList, LayerType *DownList, unsigned int phase)
{
  // This method scales the update buffer values by the time step and adds
  // them to the active layer pixels.  New values at an index which fall
  // outside of the active layer range trigger that index to be placed on the
  // "up" or "down" status list.  The neighbors of any such index are then
  // assigned new values if they are determined to be part of the active list
  // for the next iteration (i.e. their values will be raised or lowered into
  // the active range).
  const ScalarValueType LOWER_ACTIVE_THRESHOLD = - (m_ConstantGradientValue / 2.0);
  const ScalarValueType UPPER_ACTIVE_THRESHOLD =    m_ConstantGradientValue / 2.0;

  ScalarValueType new_value;
  ScalarValueType temp_value;
  ScalarValueType rms_change_accumulator;

  LayerNodeType *node;
  LayerNodeType *release_node;

  StatusValueType neighbor_status;

  unsigned int i;
  unsigned int idx;
  unsigned int counter;

  bool bounds_status;
  bool flag;

  typename LayerType::Iterator         layerIt;
  typename UpdateBufferType::const_iterator updateIt;

  NeighborhoodIterator<OutputImageType>
    outputIt(m_NeighborList.GetRadius(), this->GetOutput(),
             this->GetOutput()->GetRequestedRegion());

  NeighborhoodIterator<StatusImageType>
    statusIt(m_NeighborList.GetRadius(), m_StatusImage,
             this->GetOutput()->GetRequestedRegion());

  if ( m_BoundsCheckingActive == false )
    {
    outputIt.NeedToUseBoundaryConditionOff();
    statusIt.NeedToUseBoundaryConditionOff();
    }

  LayerListType & layers = this->m_LayersPhases[phase];

  counter =0;
  rms_change_accumulator = m_ScalarValueZero;
  layerIt = layers[0]->Begin();
  updateIt = m_UpdateBuffer.begin();
  while (layerIt != layers[0]->End() )
    {
    outputIt.SetLocation(layerIt->m_Value);
    statusIt.SetLocation(layerIt->m_Value);

    new_value = this->CalculateUpdateValue(layerIt->m_Value,
                                           dt,
                                           outputIt.GetCenterPixel()[phase],
                                           *updateIt);

    // If this index needs to be moved to another layer, then search its
    // neighborhood for indicies that need to be pulled up/down into the
    // active layer. Set those new active layer values appropriately,
    // checking first to make sure they have not been set by a more
    // influential neighbor.

    //   ...But first make sure any neighbors in the active layer are not
    // moving to a layer in the opposite direction.  This step is necessary
    // to avoid the creation of holes in the active layer.  The fix is simply
    // to not change this value and leave the index in the active set.

    if( new_value >= UPPER_ACTIVE_THRESHOLD)
      { // This index will move UP into a positive (outside) layer.

      // First check for active layer neighbors moving in the opposite
      // direction.
      flag = false;
      for (i = 0; i < m_NeighborList.GetSize(); ++i)
        {
        if (statusIt.GetPixel(m_NeighborList.GetArrayIndex(i))
            == m_StatusActiveChangingDown)
          {
          flag = true;
          break;
          }
        }
      if (flag == true)
        {
        ++layerIt;
        ++updateIt;
        continue;
        }

      rms_change_accumulator += vnl_math_sqr( new_value - outputIt.GetCenterPixel()[phase] );

      // Search the neighborhood for inside indicies.
      temp_value = new_value - m_ConstantGradientValue;
      for (i = 0; i < m_NeighborList.GetSize(); ++i)
        {
        idx = m_NeighborList.GetArrayIndex(i);
        neighbor_status = statusIt.GetPixel( idx )[phase];
        if (neighbor_status == 1)
          {
          // Keep the smallest possible value for the new active node.  This
          // places the new active layer node closest to the zero level-set.
          if ( outputIt.GetPixel(idx)[phase] < LOWER_ACTIVE_THRESHOLD ||
               ::vnl_math_abs(temp_value) < ::vnl_math_abs(outputIt.GetPixel(idx)[phase]) )
            {
            typename OutputImageType::PixelType o = outputIt.GetPixel(idx);
            o[phase] = temp_value;
            outputIt.SetPixel(idx, o, bounds_status);
            }
          }
        }
      node = m_LayerNodeStore->Borrow();
      node->m_Value = layerIt->m_Value;
      UpList->PushFront(node);
      statusIt.SetCenterPixel(m_StatusActiveChangingUp);

      // Now remove this index from the active list.
      release_node = layerIt.GetPointer();
      ++layerIt;
      layers[0]->Unlink(release_node);
      m_LayerNodeStore->Return( release_node );
      }

    else if( new_value < LOWER_ACTIVE_THRESHOLD)
      { // This index will move DOWN into a negative (inside) layer.

      // First check for active layer neighbors moving in the opposite
      // direction.
      flag = false;
      for (i = 0; i < m_NeighborList.GetSize(); ++i)
        {
        if (statusIt.GetPixel(m_NeighborList.GetArrayIndex(i))
            == m_StatusActiveChangingUp)
          {
          flag = true;
          break;
          }
        }
      if (flag == true)
        {
        ++layerIt;
        ++updateIt;
        continue;
        }

      rms_change_accumulator += vnl_math_sqr( new_value - outputIt.GetCenterPixel()[phase] );


      // Search the neighborhood for outside indicies.
      temp_value = new_value + m_ConstantGradientValue;
      for (i = 0; i < m_NeighborList.GetSize(); ++i)
        {
        idx = m_NeighborList.GetArrayIndex(i);
        neighbor_status = statusIt.GetPixel( idx )[phase];
        if (neighbor_status == 2)
          {
          // Keep the smallest magnitude value for this active set node.  This
          // places the node closest to the active layer.
          if ( outputIt.GetPixel(idx)[phase] >= UPPER_ACTIVE_THRESHOLD ||
               ::vnl_math_abs(temp_value) < ::vnl_math_abs(outputIt.GetPixel(idx)[phase]) )
            {
            typename OutputImageType::PixelType o = outputIt.GetPixel(idx);
            o[phase] = temp_value;
            outputIt.SetPixel(idx, temp_value, bounds_status);
            }
          }
        }
      node = m_LayerNodeStore->Borrow();
      node->m_Value = layerIt->m_Value;
      DownList->PushFront(node);
      statusIt.SetCenterPixel(m_StatusActiveChangingDown);

      // Now remove this index from the active list.
      release_node = layerIt.GetPointer();
      ++layerIt;
      layers[0]->Unlink(release_node);
      m_LayerNodeStore->Return( release_node );
      }
    else
      {
      rms_change_accumulator += vnl_math_sqr( new_value - outputIt.GetCenterPixel()[phase] );

      outputIt.SetCenterPixel( new_value );
      ++layerIt;
      }
    ++updateIt;
    ++counter;
    }

  // Determine the average change during this iteration.
  if (counter == 0)
    {
    this->SetRMSChange(static_cast<double>(m_ScalarValueZero));
    }
  else
    {
    double rmsTotal =
      vcl_sqrt((double)(rms_change_accumulator / static_cast<ScalarValueType>(counter)) );

    this->SetRMSChange( rmsTotal );
    }
}

template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::CopyInputToOutput()
{
  // This method is the first step in initializing the level-set image, which
  // is also the output of the filter.  The input is passed through a
  // zero crossing filter, which produces zero's at pixels closest to the zero
  // level set and one's elsewhere.  The actual zero level set values will be
  // adjusted in the Initialize() step to more accurately represent the
  // position of the zero level set.

  // First need to subtract the iso-surface value from the input image.

  //
  //  Use the index cast filter to extract a phase, process it and push it back.
  //
  typedef VectorShiftScaleImageFilter<InputImageType, OutputImageType> ShiftScaleFilterType;
  typename ShiftScaleFilterType::Pointer shiftScaleFilter = ShiftScaleFilterType::New();
  shiftScaleFilter->SetInput( this->GetInput()  );
  shiftScaleFilter->SetShift( - this->m_IsoSurfaceValue );
  // keep a handle to the shifted output
  this->m_ShiftedImage = shiftScaleFilter->GetOutput();

  typename VectorZeroCrossingImageFilter<OutputImageType, OutputImageType>::Pointer
    zeroCrossingFilter = VectorZeroCrossingImageFilter<OutputImageType,
    OutputImageType>::New();
  zeroCrossingFilter->SetInput(m_ShiftedImage);
  zeroCrossingFilter->GraftOutput(this->GetOutput());
  zeroCrossingFilter->SetBackgroundValue(m_ScalarValueOne);
  zeroCrossingFilter->SetForegroundValue(m_ScalarValueZero);

  zeroCrossingFilter->Update();

  this->GraftOutput(zeroCrossingFilter->GetOutput());
}

template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::InitializeStatusImage()
{
  MeasurementVectorTraits::SetLength( m_ValueZero, this->m_NumberOfPhases );
  MeasurementVectorTraits::SetLength( m_ValueOne, this->m_NumberOfPhases );
  MeasurementVectorTraits::SetLength( m_StatusNull, this->m_NumberOfPhases );
  MeasurementVectorTraits::SetLength( m_StatusChanging, this->m_NumberOfPhases );
  MeasurementVectorTraits::SetLength( m_StatusActiveChangingUp, this->m_NumberOfPhases );
  MeasurementVectorTraits::SetLength( m_StatusActiveChangingDown, this->m_NumberOfPhases );
  this->m_ValueZero.Fill( NumericTraits< ScalarValueType >::Zero );
  this->m_ValueOne.Fill( NumericTraits< ScalarValueType >::OneValue() );
  this->m_StatusNull.Fill( this->m_StatusNullValue );
  this->m_StatusChanging.Fill( this->m_StatusChangingValue );
  this->m_StatusActiveChangingUp.Fill( this->m_StatusActiveChangingUpValue );
  this->m_StatusActiveChangingDown.Fill( this->m_StatusActiveChangingDownValue );
  this->m_StatusBoundaryPixel.Fill( this->m_StatusBoundaryPixelValue );

  // Allocate the status image.
  m_StatusImage = StatusImageType::New();
  m_StatusImage->SetVectorLength( this->m_NumberOfPhases );
  m_StatusImage->SetRegions(this->GetOutput()->GetRequestedRegion());
  m_StatusImage->Allocate();

  // Initialize the status image to contain all m_StatusNull values.
  ImageRegionIterator<StatusImageType>
    statusIt(m_StatusImage, m_StatusImage->GetRequestedRegion());
  for (statusIt.GoToBegin(); ! statusIt.IsAtEnd(); ++statusIt)
    {
    statusIt.Set( m_StatusNull );
    }

  // Initialize the boundary pixels in the status image to
  // m_StatusBoundaryPixel values.  Uses the face calculator to find all of the
  // region faces.
  typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<StatusImageType>
    BFCType;

  BFCType faceCalculator;
  typename BFCType::FaceListType faceList;
  typename BFCType::SizeType sz;
  typename BFCType::FaceListType::iterator fit;

  sz.Fill(1);
  faceList = faceCalculator(m_StatusImage, m_StatusImage->GetRequestedRegion(), sz);
  fit = faceList.begin();

  for (++fit; fit != faceList.end(); ++fit) // skip the first (nonboundary) region
    {
    statusIt = ImageRegionIterator<StatusImageType>(m_StatusImage, *fit);
    for (statusIt.GoToBegin(); ! statusIt.IsAtEnd(); ++statusIt)
      {
      statusIt.Set( m_StatusBoundaryPixel );
      }
    }
}

template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::Initialize()
{
  unsigned int i;

  if (this->GetUseImageSpacing())
    {
    double minSpacing = NumericTraits<double>::max();
    for (i=0; i<ImageDimension; i++)
      {
      minSpacing = vnl_math_min(minSpacing,this->GetInput()->GetSpacing()[i]);
      }
    m_ConstantGradientValue = minSpacing;
    }
  else
    {
    m_ConstantGradientValue = 1.0;
    }


  this->m_LayersPhases.resize( this->m_NumberOfPhases );

  // Allocate and initialize the status image and the status vectors
  this->InitializeStatusImage();

  for( unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
    {
    LayerListType & layers = this->m_LayersPhases[phase];

    // Erase all existing layer lists.
    for( unsigned int k = 0; k < layers.size(); ++k )
      {
      while (! layers[k]->Empty() )
        {
        m_LayerNodeStore->Return(layers[k]->Front());
        layers[k]->PopFront();
        }
      }

    // Allocate the layers for the sparse field.
    layers.clear();
    layers.reserve( 2 * m_NumberOfLayers + 1 );

    while ( layers.size() < ( 2 * m_NumberOfLayers + 1 ) )
      {
      layers.push_back( LayerType::New() );
      }

    // Throw an exception if we don't have enough layers.
    if (layers.size() < 3)
      {
      itkExceptionMacro( << "Not enough layers have been allocated for the sparse field.  Requires at least one layer.");
      }

    // Construct the active layer and initialize the first layers inside and
    // outside of the active layer.
    this->ConstructActiveLayer(phase);

    // Construct the rest of the non-active set layers using the first two
    // layers. Inside layers are odd numbers, outside layers are even numbers.
    for(unsigned int k = 1; k < layers.size() - 2; ++k)
      {
      this->ConstructLayer(k, k+2, phase);
      }

    }

  // Set the values in the output image for the active layer.
  this->InitializeActiveLayerValues();

  // Initialize layer values using the active layer as seeds.
  this->PropagateAllLayerValues();

  // Initialize pixels inside and outside the sparse field layers to positive
  // and negative values, respectively.  This is not necessary for the
  // calculations, but is useful for presenting a more intuitive output to the
  // filter.  See PostProcessOutput method for more information.
  this->InitializeBackgroundPixels();
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::InitializeBackgroundPixels()
{
  // Assign background pixels OUTSIDE the sparse field layers to a new level set
  // with value greater than the outermost layer.  Assign background pixels
  // INSIDE the sparse field layers to a new level set with value less than
  // the innermost layer.
  const ScalarValueType max_layer = static_cast<ScalarValueType>( this->m_NumberOfLayers );

  const ScalarValueType outside_value =  (max_layer+1) * m_ConstantGradientValue;
  const ScalarValueType inside_value  = -(max_layer+1) * m_ConstantGradientValue;

  ImageRegionConstIterator<StatusImageType> statusIt(m_StatusImage,
                                                     this->GetOutput()->GetRequestedRegion());

  ImageRegionIterator<OutputImageType> outputIt(this->GetOutput(),
                                                this->GetOutput()->GetRequestedRegion());

  ImageRegionConstIterator<OutputImageType> shiftedIt(m_ShiftedImage,
                                                      this->GetOutput()->GetRequestedRegion());

  typename OutputImageType::PixelType outputValue;
  typename OutputImageType::PixelType pixelValue;

  for (outputIt = outputIt.Begin(), statusIt = statusIt.Begin(),
         shiftedIt = shiftedIt.Begin();
       ! outputIt.IsAtEnd(); ++outputIt, ++statusIt, ++shiftedIt)
    {
    for( unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
      {
      pixelValue = shiftedIt.Get();
      if (statusIt.Get()[phase] == m_StatusNullValue || statusIt.Get()[phase] == m_StatusBoundaryPixelValue)
        {
        if( pixelValue[phase] > this->m_ScalarValueZero )
          {
          outputValue[phase] = outside_value;
          }
        else
          {
          outputValue[phase] = inside_value;
          }
        }
      outputIt.Set( outputValue );
      }
    }
}


template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::ConstructActiveLayers()
{
  //
  // The layers at every phase are independent. Therefore they must be
  // constructed on a separate pass by phase.
  //
  typedef ImageRegionIterator< OutputImageType > IteratorType;

  IteratorType itr( this->m_ShiftedImage, this->m_ShiftedImage->GetBufferedRegion() );

  itr.GoToBegin();

  const unsigned int numberOfPhases = MeasurementVectorTraits::GetLength( itr.Get() );

  for( unsigned int phase = 0; phase < numberOfPhases; phase++ )
    {
    this->ConstructActiveLayer( phase );
    }
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::ConstructActiveLayer(unsigned int phase)
{
  //
  // We find the active layer by searching for 0's in the zero crossing image
  // (output image).  The first inside and outside layers are also constructed
  // by searching the neighbors of the active layer in the (shifted) input image.
  // Negative neighbors not in the active set are assigned to the inside,
  // positive neighbors are assigned to the outside.
  //
  // During construction we also check whether any of the layers of the active
  // set (or the active set itself) is sitting on a boundary pixel location. If
  // this is the case, then we need to do active bounds checking in the solver.
  //

  LayerListType & layers = this->m_LayersPhases[phase];

  NeighborhoodIterator<OutputImageType>
    shiftedIt(m_NeighborList.GetRadius(), m_ShiftedImage,
              this->GetOutput()->GetRequestedRegion());
  NeighborhoodIterator<OutputImageType>
    outputIt(m_NeighborList.GetRadius(), this->GetOutput(),
             this->GetOutput()->GetRequestedRegion());
  NeighborhoodIterator<StatusImageType>
    statusIt(m_NeighborList.GetRadius(), m_StatusImage,
             this->GetOutput()->GetRequestedRegion());

  IndexType center_index, offset_index;
  LayerNodeType *node;
  bool bounds_status;
  ValueType value;
  StatusValueType layer_number;

  typename OutputImageType::IndexType upperBounds, lowerBounds;
  lowerBounds = this->GetOutput()->GetRequestedRegion().GetIndex();
  upperBounds = this->GetOutput()->GetRequestedRegion().GetIndex()
    + this->GetOutput()->GetRequestedRegion().GetSize();

  for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
    {
    if ( outputIt.GetCenterPixel()[phase] == m_ScalarValueZero )
      {
      // Grab the neighborhood in the status image.
      center_index = outputIt.GetIndex();
      statusIt.SetLocation( center_index );

      // Check to see if any of the sparse field touches a boundary.  If so,
      // then activate bounds checking.
      for( unsigned int i = 0; i < ImageDimension; i++ )
        {
        if (center_index[i] + static_cast<long>(m_NumberOfLayers) >= (upperBounds[i] - 1)
            || center_index[i] - static_cast<long>(m_NumberOfLayers) <= lowerBounds[i])
          {
          m_BoundsCheckingActive = true;
          }
        }

      // Borrow a node from the store and set its value.
      node = m_LayerNodeStore->Borrow();
      node->m_Value = center_index;

      // Add the node to the active list and set the status in the status
      // image.
      layers[0]->PushFront( node );
      statusIt.SetCenterPixel( 0 );

      // Grab the neighborhood in the image of shifted input values.
      shiftedIt.SetLocation( center_index );

      // Search the neighborhood pixels for first inside & outside layer
      // members.  Construct these lists and set status list values.
      for( unsigned int i = 0; i < m_NeighborList.GetSize(); ++i )
        {
        offset_index = center_index
          + m_NeighborList.GetNeighborhoodOffset(i);

        if( outputIt.GetPixel(m_NeighborList.GetArrayIndex(i))[phase] != m_ScalarValueZero )
          {
          value = shiftedIt.GetPixel(m_NeighborList.GetArrayIndex(i));

          if( value[phase] < m_ScalarValueZero ) // Assign to first inside layer.
            {
            layer_number = 1;
            }
          else // Assign to first outside layer
            {
            layer_number = 2;
            }

          StatusType s = statusIt.GetPixel(m_NeighborList.GetArrayIndex(i));
          s[phase] = layer_number; 
          statusIt.SetPixel( m_NeighborList.GetArrayIndex(i),
                             s, bounds_status );
          if ( bounds_status == true ) // In bounds.
            {
            node = m_LayerNodeStore->Borrow();
            node->m_Value = offset_index;
            layers[layer_number]->PushFront( node );
            } // else do nothing.
          }
        }
      }
    }
}

template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::ConstructLayer( StatusType from, StatusType to, unsigned int phase)
{
  unsigned int i;
  LayerNodeType *node;
  bool boundary_status;

  typename LayerType::ConstIterator fromIt;

  NeighborhoodIterator<StatusImageType>
    statusIt(m_NeighborList.GetRadius(), m_StatusImage,
             this->GetOutput()->GetRequestedRegion() );

  LayerListType & layers = this->m_LayersPhases[phase];

  // For all indicies in the "from" layer...
  for (fromIt = layers[from[phase]]->Begin();
       fromIt != layers[from[phase]]->End();  ++fromIt)
    {
    // Search the neighborhood of this index in the status image for
    // unassigned indicies. Push those indicies onto the "to" layer and
    // assign them values in the status image.  Status pixels outside the
    // boundary will be ignored.
    statusIt.SetLocation( fromIt->m_Value );
    for (i = 0; i < m_NeighborList.GetSize(); ++i)
      {
      StatusType s = statusIt.GetPixel( m_NeighborList.GetArrayIndex(i) );
      if ( s[phase] == m_StatusNullValue )
        {
        s[phase] = to[phase];
        statusIt.SetPixel(m_NeighborList.GetArrayIndex(i), s,
                          boundary_status);
        if (boundary_status == true) // in bounds
          {
          node = m_LayerNodeStore->Borrow();
          node->m_Value = statusIt.GetIndex()
            + m_NeighborList.GetNeighborhoodOffset(i);
          layers[to[phase]]->PushFront( node );
          }
        }
      }
    }
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::InitializeActiveLayerValues()
{
  const ScalarValueType CHANGE_FACTOR = m_ConstantGradientValue / 2.0;
  ScalarValueType MIN_NORM      = 1.0e-6;

  if (this->GetUseImageSpacing())
    {
    double minSpacing = NumericTraits<double>::max();
    for (unsigned int i=0; i<ImageDimension; i++)
      {
      minSpacing = vnl_math_min(minSpacing,this->GetInput()->GetSpacing()[i]);
      }
    MIN_NORM *= minSpacing;
    }

  unsigned int i, center;

  typename LayerType::ConstIterator activeIt;
  ConstNeighborhoodIterator<OutputImageType>
    shiftedIt( m_NeighborList.GetRadius(), m_ShiftedImage,
               this->GetOutput()->GetRequestedRegion() );

  center = shiftedIt.Size() /2;
  typename OutputImageType::Pointer output = this->GetOutput();

  const NeighborhoodScalesType neighborhoodScales = this->GetDifferenceFunction()->ComputeNeighborhoodScales();

  ScalarValueType dx_forward;
  ScalarValueType dx_backward;
  ScalarValueType length;
  ScalarValueType distance;

  for( unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
    {
    LayerListType & layers = this->m_LayersPhases[phase];

    // For all indicies in the active layer...
    for (activeIt = layers[0]->Begin();
         activeIt != layers[0]->End(); ++activeIt)
      {
      // Interpolate on the (shifted) input image values at this index to
      // assign an active layer value in the output image.
      shiftedIt.SetLocation( activeIt->m_Value );

      const ValueType centerPixelValue   = shiftedIt.GetCenterPixel();
      const ScalarValueType centerPixelValuePhase   = centerPixelValue[phase];

      length = m_ScalarValueZero;
      for (i = 0; i < ImageDimension; ++i)
        {
        const ValueType forwardPixelValue  = shiftedIt.GetPixel(center + m_NeighborList.GetStride(i));
        const ValueType backwardPixelValue = shiftedIt.GetPixel(center - m_NeighborList.GetStride(i));

        const ScalarValueType forwardPixelValuePhase  = forwardPixelValue[phase];
        const ScalarValueType backwardPixelValuePhase = backwardPixelValue[phase];

        dx_forward  = ( forwardPixelValuePhase - centerPixelValuePhase   ) * neighborhoodScales[i];
        dx_backward = ( centerPixelValuePhase  - backwardPixelValuePhase ) * neighborhoodScales[i];

        if ( vnl_math_abs(dx_forward) > vnl_math_abs(dx_backward) )
          {
          length += dx_forward * dx_forward;
          }
        else
          {
          length += dx_backward * dx_backward;
          }
        }
      length = vcl_sqrt((double)length) + MIN_NORM;
      distance = centerPixelValuePhase / length;

      ValueType outpuPixel = output->GetPixel( activeIt->m_Value );

      outpuPixel[phase] = vnl_math_min(vnl_math_max(-CHANGE_FACTOR, distance), CHANGE_FACTOR);

      output->SetPixel( activeIt->m_Value , outpuPixel );
      }
    }

}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::AllocateUpdateBuffer()
{
  // Preallocate the update buffer.  NOTE: There is currently no way to
  // downsize a std::vector. This means that the update buffer will grow
  // dynamically but not shrink.  In newer implementations there may be a
  // squeeze method which can do this.  Alternately, we can implement our own
  // strategy for downsizing.
  m_UpdateBuffer.clear();

  // Use the maximum of the sizes from all phase layers.
  LayerListType & layers = this->m_LayersPhases[0];
  m_UpdateBuffer.reserve( layers[0]->Size() );
}


template <class TInputImage, class TOutputImage>
typename
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>::TimeStepType
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::CalculateChange()
{
  // the time step will be computed as the smallest of all the time steps
  // computed across all image phases.
  TimeStepType timeStep = NumericTraits<TimeStepType>::max();

  for(unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
    {
    VectorDifferenceFunctionPointer df = this->m_DifferenceFunctions[phase];

    if( df.IsNull() )
      {
      continue;
      }

    typename VectorDifferenceFunctionType::FloatOffsetType offset;

    ScalarValueType norm_grad_phi_squared;
    ScalarValueType dx_forward;
    ScalarValueType dx_backward;
    ScalarValueType forwardValue;
    ScalarValueType backwardValue;
    ScalarValueType centerValue;

    unsigned i;
    ScalarValueType MIN_NORM      = 1.0e-6;

    if( this->GetUseImageSpacing() )
      {
      double minSpacing = NumericTraits<double>::max();
      for (i=0; i<ImageDimension; i++)
        {
        minSpacing = vnl_math_min(minSpacing,this->GetInput()->GetSpacing()[i]);
        }
      MIN_NORM *= minSpacing;
      }

    void *globalData = df->GetGlobalDataPointer();

    typename LayerType::ConstIterator layerIt;
    NeighborhoodIterator<OutputImageType> outputIt(df->GetRadius(),
                      this->GetOutput(), this->GetOutput()->GetRequestedRegion());

    const NeighborhoodScalesType neighborhoodScales = this->GetDifferenceFunction()->ComputeNeighborhoodScales();

    if ( m_BoundsCheckingActive == false )
      {
      outputIt.NeedToUseBoundaryConditionOff();
      }


    for( unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
      {
      LayerListType & layers = this->m_LayersPhases[phase];

      m_UpdateBuffer.clear();
      m_UpdateBuffer.reserve(layers[0]->Size());

      // Calculates the update values for the active layer indicies in this
      // iteration.  Iterates through the active layer index list, applying
      // the level set function to the output image (level set image) at each
      // index.  Update values are stored in the update buffer.
      for (layerIt = layers[0]->Begin(); layerIt != layers[0]->End(); ++layerIt)
        {
        outputIt.SetLocation(layerIt->m_Value);

        // Calculate the offset to the surface from the center of this
        // neighborhood.  This is used by some level set functions in sampling a
        // speed, advection, or curvature term.
        if (this->GetInterpolateSurfaceLocation()
                       && (centerValue = outputIt.GetCenterPixel()[phase]) != 0.0 )
          {
          // Surface is at the zero crossing, so distance to surface is:
          // phi(x) / norm(grad(phi)), where phi(x) is the center of the
          // neighborhood.  The location is therefore
          // (i,j,k) - ( phi(x) * grad(phi(x)) ) / norm(grad(phi))^2
          norm_grad_phi_squared = 0.0;
          for (i = 0; i < ImageDimension; ++i)
            {
            forwardValue  = outputIt.GetNext(i)[phase];
            backwardValue = outputIt.GetPrevious(i)[phase];

            if (forwardValue * backwardValue >= 0)
              { //  Neighbors are same sign OR at least one neighbor is zero.
              dx_forward  = forwardValue - centerValue;
              dx_backward = centerValue - backwardValue;

              // Pick the larger magnitude derivative.
              if (::vnl_math_abs(dx_forward) > ::vnl_math_abs(dx_backward) )
                {
                offset[i] = dx_forward;
                }
              else
                {
                offset[i] = dx_backward;
                }
              }
            else //Neighbors are opposite sign, pick the direction of the 0 surface.
              {
              if (forwardValue * centerValue < 0)
                {
                offset[i] = forwardValue - centerValue;
                }
              else
                {
                offset[i] = centerValue - backwardValue;
                }
              }

            norm_grad_phi_squared += offset[i] * offset[i];
            }

          for (i = 0; i < ImageDimension; ++i)
            {
            offset[i] = (offset[i] * centerValue) / (norm_grad_phi_squared + MIN_NORM);
            }

          //  FIXME: Change GetDifferenceFunction() to GetComponentDifferenceFunction() ??
          //  FIXME: OR... have multiple DifferenceFunctions() one per phase...?
          //  FIXME: KK Check with Luis.. Looks like an old comment that we can ignore.
          //
          m_UpdateBuffer.push_back( df->ComputeUpdate(outputIt, globalData, phase, offset ) );
          }
        else // Don't do interpolation
          {
          m_UpdateBuffer.push_back( df->ComputeUpdate(outputIt, globalData, phase, 0.0 ) );
          }
        }
      }

    // Ask the finite difference function to compute the time step for
    // this iteration.  We give it the global data pointer to use, then
    // ask it to free the global data memory.
    TimeStepType partialTimeStep = df->ComputeGlobalTimeStep( globalData );

    if( partialTimeStep < timeStep )
      {
      timeStep = partialTimeStep;
      }

    df->ReleaseGlobalDataPointer(globalData);
    }

  return timeStep;
}


template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::PropagateAllLayerValues()
{
  for( unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++ )
    {
    // Update values in the first inside and first outside layers using the
    // active layer as a seed. Inside layers are odd numbers, outside layers are
    // even numbers.
    this->PropagateLayerValues(0, 1, 3, 1, phase); // first inside
    this->PropagateLayerValues(0, 2, 4, 2, phase); // first outside

    LayerListType & layers = this->m_LayersPhases[phase];

    // Update the rest of the layers.
    for( unsigned int i = 1; i < layers.size() - 2; ++i )
      {
      this->PropagateLayerValues(i, i+2, i+4, (i+2)%2, phase);
      }
    }
}

template <class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::PropagateLayerValues(StatusType from, StatusType to,
                       StatusType promote, int InOrOut,unsigned int phase)
{
  unsigned int i;
  ScalarValueType value, value_temp, delta;
  value = NumericTraits<ScalarValueType>::Zero;
  bool found_neighbor_flag;
  typename LayerType::Iterator toIt;
  LayerNodeType *node;

  LayerListType & layers = this->m_LayersPhases[phase];

  StatusValueType past_end = static_cast<StatusValueType>( layers.size() ) - 1;

  // Are we propagating values inward (more negative) or outward (more
  // positive)?
  if (InOrOut == 1)
    {
    delta = - m_ConstantGradientValue;
    }
  else
    {
    delta = m_ConstantGradientValue;
    }

  NeighborhoodIterator<OutputImageType>
    outputIt(m_NeighborList.GetRadius(), this->GetOutput(),
             this->GetOutput()->GetRequestedRegion() );
  NeighborhoodIterator<StatusImageType>
    statusIt(m_NeighborList.GetRadius(), m_StatusImage,
             this->GetOutput()->GetRequestedRegion() );

  if ( m_BoundsCheckingActive == false )
    {
    outputIt.NeedToUseBoundaryConditionOff();
    statusIt.NeedToUseBoundaryConditionOff();
    }

  toIt  = layers[to[phase]]->Begin();
  while ( toIt != layers[to[phase]]->End() )
    {
    statusIt.SetLocation( toIt->m_Value );

    // Is this index marked for deletion? If the status image has
    // been marked with another layer's value, we need to delete this node
    // from the current list then skip to the next iteration.
    if (statusIt.GetCenterPixel()[phase] != to[phase])
      {
      node = toIt.GetPointer();
      ++toIt;
      layers[to[phase]]->Unlink( node );
      m_LayerNodeStore->Return( node );
      continue;
      }

    outputIt.SetLocation( toIt->m_Value );

    found_neighbor_flag = false;
    for (i = 0; i < m_NeighborList.GetSize(); ++i)
      {
      // If this neighbor is in the "from" list, compare its absolute value
      // to to any previous values found in the "from" list.  Keep the value
      // that will cause the next layer to be closest to the zero level set.
      if ( statusIt.GetPixel( m_NeighborList.GetArrayIndex(i) )[phase] == from[phase] )
        {
        value_temp = outputIt.GetPixel( m_NeighborList.GetArrayIndex(i) )[phase];

        if (found_neighbor_flag == false)
          {
          value = value_temp;
          }
        else
          {
          if (InOrOut == 1)
            {
            // Find the largest (least negative) neighbor
            if ( value_temp > value )
             {
             value = value_temp;
             }
            }
          else
            {
            // Find the smallest (least positive) neighbor
            if (value_temp < value)
              {
              value = value_temp;
              }
            }
          }
        found_neighbor_flag = true;
        }
      }
    if (found_neighbor_flag == true)
      {
      // Set the new value using the smallest distance
      // found in our "from" neighbors.
      outputIt.SetCenterPixel( value + delta );
      ++toIt;
      }
    else
      {
      // Did not find any neighbors on the "from" list, then promote this
      // node.  A "promote" value past the end of my sparse field size
      // means delete the node instead.  Change the status value in the
      // status image accordingly.
      node  = toIt.GetPointer();
      ++toIt;
      layers[to[phase]]->Unlink( node );
      StatusType s = statusIt.GetCenterPixel();
      if ( promote[phase] > past_end )
        {
        m_LayerNodeStore->Return( node );
        s[phase] = m_StatusNullValue;
        statusIt.SetCenterPixel(s);
        }
      else
        {
        layers[promote[phase]]->PushFront( node );
        s[phase] = promote[phase];
        statusIt.SetCenterPixel(s);
        }
      }
    }
}


template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::PostProcessOutput()
{
  // Assign background pixels INSIDE the sparse field layers to a new level set
  // with value less than the innermost layer.  Assign background pixels
  // OUTSIDE the sparse field layers to a new level set with value greater than
  // the outermost layer.
  const ScalarValueType max_layer = static_cast<ScalarValueType>(m_NumberOfLayers);

  const ScalarValueType inside_value  = (max_layer+1) * m_ConstantGradientValue;
  const ScalarValueType outside_value = -(max_layer+1) * m_ConstantGradientValue;

  ImageRegionConstIterator<StatusImageType> statusIt(m_StatusImage,
                                                     this->GetOutput()->GetRequestedRegion());

  ImageRegionIterator<OutputImageType> outputIt(this->GetOutput(),
                                                this->GetOutput()->GetRequestedRegion());

  for (outputIt = outputIt.Begin(), statusIt = statusIt.Begin();
       ! outputIt.IsAtEnd(); ++outputIt, ++statusIt)
    {
    StatusType s = statusIt.Get();
    typename OutputImageType::PixelType outputPixel = outputIt.Get();
    for (unsigned int phase = 0; phase < this->m_NumberOfPhases; phase++)
      {
      if (s[phase] == m_StatusNullValue)
        {
        if (outputPixel[phase] > NumericTraits< ScalarValueType >::Zero )
          {
          outputPixel[phase] = inside_value;
          }
        else
          {
          outputPixel[phase] = outside_value;
          }
        }
      }
    outputIt.Set( outputPixel );
    }
}

template<class TInputImage, class TOutputImage>
void
VectorSparseFieldLevelSetImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "m_IsoSurfaceValue: " << m_IsoSurfaceValue << std::endl;
  os << indent << "m_LayerNodeStore: " << std::endl;
    m_LayerNodeStore->Print(os,indent.GetNextIndent());
  os << indent << "m_BoundsCheckingActive: " << m_BoundsCheckingActive;

  for( unsigned int c = 0; c < this->m_NumberOfComponents; c++ )
    {
    const LayerListType & layers = this->m_LayersPhases[c];

    for( unsigned int i=0; i < layers.size(); i++)
      {
      os << indent << "layers[" << i << "]: size="
         << layers[i]->Size() << std::endl;
      os << indent << layers[i];
      }
    }
  os << indent << "m_UpdateBuffer: size=" << static_cast<unsigned long>(m_UpdateBuffer.size())
     << " capacity=" << static_cast<unsigned long>( m_UpdateBuffer.capacity()) << std::endl;
}

} // end namespace itk

#endif
