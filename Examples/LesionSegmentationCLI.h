#ifndef __LesionSegmentationCLI_h
#define __LesionSegmentationCLI_h

#include "metaCommand.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "itkFixedArray.h"
#include "itkLandmarkSpatialObject.h"
#include <fstream> 

class LesionSegmentationCLI : public MetaCommand
{

public:
  typedef itk::LandmarkSpatialObject< 3 >    SeedSpatialObjectType;
  typedef SeedSpatialObjectType::PointListType   PointListType;

  LesionSegmentationCLI( int argc, char *argv[] ) : MetaCommand()
  {
    this->DisableDeprecatedWarnings();

    this->AddArgument("InputImage",false,"Input image to be segmented.");
    this->AddArgument("InputDICOMDir",false,"DICOM directory containing series of the Input image to be segmented.");
    this->AddArgument("OutputImage", false, "Output segmented image");
    this->AddArgument("OutputMesh", true, "Output segmented surface (STL filename expected)");
    this->AddArgument("OutputROI", false, "Write the ROI within which the segmentation will be confined to (for debugging purposes)");
    this->AddArgument("Visualize", false, "Visualize the input image and the segmented surface.", MetaCommand::BOOL, "0");
    this->AddArgument("Wireframe", false, "Visualize the input image and the segmented surface as a wireframe. Only valid if the Visualize flag is also enabled.", MetaCommand::BOOL, "0");
    this->AddArgument("IgnoreDirection", false, "Ignore the direction of the DICOM image", MetaCommand::BOOL, "0");
     this->AddArgument("PartSolid", false, "Default is to assume parameters for a solid lesion. Specify this if the lesion is part-solid.", MetaCommand::BOOL, "0");
    this->AddArgument("ROI", false,
                    "Bounds of the ROI if any, 6 parameters", MetaCommand::LIST);
    this->AddArgument("Sigma", false,
      "Manually specify sigma. This is an array with 3 values in physical units. This defaults to the maximum spacing in the dataset, if unspecified",
      MetaCommand::LIST);
    this->AddArgument("Seeds", true,
      "Manually specify seeds. At least one must be specified",
      MetaCommand::LIST);
    this->AddArgument("MaximumRadius", false, "Maximum radius of the lesion in mm. This can be used as alternate way of specifying the bounds. You specify a seed and a value of say 20mm, if you know the lesion is smaller than 20mm..", MetaCommand::FLOAT, "30");
    
    
    if(!this->Parse(argc, argv))
      {
      // We can't invoke errors from constructors..
      exit(-1);
      }  
    }

  double *GetROI()
    {
    if (this->GetOptionWasSet("ROI"))
      {
      std::list< std::string > bounds = this->GetValueAsList("ROI");
      std::list< std::string >::const_iterator fit = bounds.begin();
      for (unsigned int i = 0; fit != bounds.end(); ++fit, ++i)
        {
        this->ROI[i] = (float)atof((*fit).c_str());
        }
      }
    else
      {
      PointListType seeds = this->GetSeeds();
      seeds[0];
      for (int i = 0; i < 3; i++)
        {
        this->ROI[2*i] = seeds[0].GetPosition()[i] - this->GetValueAsFloat("MaximumRadius");
        this->ROI[2*i+1] = seeds[0].GetPosition()[i] + this->GetValueAsFloat("MaximumRadius");
        }
      }
    return this->ROI;
    }
  
  itk::FixedArray< double, 3 > GetSigmas()
    {
    itk::FixedArray< double, 3 > s;
    std::list< std::string > bounds = this->GetValueAsList("ROI");
    std::list< std::string >::const_iterator fit = bounds.begin();
    for (unsigned int i = 0; fit != bounds.end(); ++fit, ++i)
      {
      s[i] = (double)atof((*fit).c_str());
      }
    return s;
    }

  PointListType GetSeeds()
    {
    std::list< std::string > seedsString = this->GetValueAsList("Seeds");
    std::list< std::string >::const_iterator fit = seedsString.begin();
    const int nb_of_markers = seedsString.size() / 3;
    PointListType seeds(nb_of_markers);
    for (unsigned int i = 0; i < nb_of_markers; i++)
      {
      const double sx = (double)atof((*fit).c_str());
      ++fit;
      const double sy = (double)atof((*fit).c_str());
      ++fit;
      const double sz = (double)atof((*fit).c_str());
      ++fit;
      seeds[i].SetPosition(sx,sy,sz);
      }
    return seeds;
    }


protected:

  void AddArgument( std::string name, 
                    bool required,
                    std::string description,
                    TypeEnumType type = MetaCommand::STRING,
                    std::string defVal = "" )
    {
    this->SetOption(name,name,required,description);
    this->SetOptionLongTag(name,name);
    this->AddOptionField(name,name,type,true, defVal);
    } 

  double ROI[6];
};

#endif
