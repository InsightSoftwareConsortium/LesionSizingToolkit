#ifndef __LesionSegmentationCLI_h
#define __LesionSegmentationCLI_h

#include "metaCommand.h"
#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "itkFixedArray.h"
#include "itkLandmarkSpatialObject.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkMetaDataDictionary.h"
#include "itkMetaDataObject.h"
#include <vtksys/SystemTools.hxx>
#include <fstream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

typedef short PixelType;
const unsigned int ImageDimension = 3;
typedef itk::Image< PixelType, ImageDimension > InputImageType;
typedef itk::Image< float, ImageDimension > RealImageType;

class LesionSegmentationCLI : public MetaCommand
{

public:
  typedef itk::LandmarkSpatialObject< 3 >    SeedSpatialObjectType;
  typedef SeedSpatialObjectType::PointListType   PointListType;

  LesionSegmentationCLI( int argc, char *argv[] ) : MetaCommand()
  {
    m_Image = NULL;
    this->DisableDeprecatedWarnings();

    this->AddArgument("InputImage",false,"Input image to be segmented.");
    this->AddArgument("InputDICOMDir",false,"DICOM directory containing series of the Input image to be segmented.");
    this->AddArgument("OutputImage", false, "Output segmented image");
    this->AddArgument("OutputMesh", false, "Output segmented surface (STL filename expected)");
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
    this->AddArgument("Seeds", false,
      "Manually specify seeds in physical coordinates. At least one seed must be specified using for a segmentation to be generated. Usage is of the form --Seeds 3 X1 Y1 Z1 (for 1 seed) or --Seeds 6 X1 Y1 Z1 X2 Y2 Z2 (for 2 seeds) etc..",
      MetaCommand::LIST);
    this->AddArgument("SeedUnitsInPixels", false,
      "Are the seeds specified in pixel coordinates ? (Note that pixel coords start at 0 index). If so, use this flag. By default seeds are assumed to be in physical coordinates.", MetaCommand::BOOL, "0");
    this->AddArgument("MaximumRadius", false, "Maximum radius of the lesion in mm. This can be used as alternate way of specifying the bounds. You specify a seed and a value of say 20mm, if you know the lesion is smaller than 20mm..", MetaCommand::FLOAT, "30");
    this->AddArgument("Screenshot",false,"Screenshot PNG file of the final segmented surface (requires \"Visualize\" to be ON.");
    this->AddArgument("ShowBoundingBox", false,
      "Show the ROI used for the segmentation as a bounding box on the visualization.", MetaCommand::BOOL, "0");
    this->AddArgument("GetZSpacingFromSliceNameRegex",false,
      "This option was added for the NIST Biochange challenge where the Z seed index was specified by providing the filename of the DICOM slice where the seed resides. Hence if this option is specified, the Z value of the seed is ignored.");


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
    std::list< std::string > bounds = this->GetValueAsList("Sigma");
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
    const unsigned int nb_of_markers = seedsString.size() / 3;
    PointListType seeds(nb_of_markers);
    for (unsigned int i = 0; i < nb_of_markers; i++)
      {
      double sx = (double)atof((*fit).c_str());
      ++fit;
      double sy = (double)atof((*fit).c_str());
      ++fit;
      double sz = (double)atof((*fit).c_str());
      ++fit;

      if (this->GetOptionWasSet("SeedUnitsInPixels"))
        {
        // Convert seeds from pixel units to physical units
        IndexType index = {{
          static_cast< IndexValueType >(sx),
          static_cast< IndexValueType >(sy),
          static_cast< IndexValueType >(sz) }};

        InputImageType::PointType point;
        m_Image->TransformIndexToPhysicalPoint(index, point);
        sx = point[0];
        sy = point[1];
        sz = point[2];

        // Get the z spacing from the slice name regex..
        if (this->GetOptionWasSet("GetZSpacingFromSliceNameRegex"))
          {
          std::string substring =
            this->GetValueAsString("GetZSpacingFromSliceNameRegex");
          std::vector< std::string > filesInDir =
            this->GetFilesInDirectory(this->GetValueAsString("InputDICOMDir"));
          bool found = false;
          for (std::vector< std::string >::iterator it = filesInDir.begin();
              it != filesInDir.end(); ++it)
            {
            if (it->find(substring) != std::string::npos &&
                it->find("vvi") == std::string::npos)
              {
              std::string file = this->GetValueAsString("InputDICOMDir");
              file += "/";
              file += (*it);
              if (vtksys::SystemTools::FileExists(file.c_str(), true))
                {
                found = true;
                std::string sopInstanceUID;
                sz = this->GetZPositionFromFile(file, sopInstanceUID);
                std::cout << "Found matching filename: " << file << "\n  with SOPInstanceUID " << sopInstanceUID << "\n  matching string " << substring << std::endl;
                break;
                }
              }
            }
          if (!found)
            {
            // Loop now and check the SOP instance UID. Some datasets in the
            // biochange challenge rely on the filename, yet others rely on
            // the SOP instance UID present in the file.
            
            for (std::vector< std::string >::iterator it = filesInDir.begin();
                it != filesInDir.end(); ++it)
              {
              if (it->find("vvi") == std::string::npos)
                {
                std::string file = this->GetValueAsString("InputDICOMDir");
                file += "/";
                file += (*it);
                if (vtksys::SystemTools::FileExists(file.c_str(), true))
                  {
                  std::string sopInstanceUID;
                  sz = this->GetZPositionFromFile(file, sopInstanceUID);
                  if (sopInstanceUID.find(substring) != std::string::npos)
                    {
                    std::cout << "Found file: " << file << "\n  with matching SOPInstanceUID " << sopInstanceUID << "\n  matching string " << substring << std::endl;
                    found = true;
                    break;
                    }
                  }
                }
              }

            if (!found)
              {
              std::cerr << "Could not find a file with matching SOP "
                        << substring << std::endl;
              exit(-1);
              }
            }
          }
        }

      // Sanity check      
      std::cout << "Seed position in physical units: (" << sx << ","
                << sy << "," << sz << ")" << std::endl;
      InputImageType::PointType pointSeed;
      pointSeed[0] = sx;
      pointSeed[1] = sy;
      pointSeed[2] = sz;
      IndexType indexSeed;
      m_Image->TransformPhysicalPointToIndex(pointSeed, indexSeed);
      if (!this->m_Image->GetBufferedRegion().IsInside(indexSeed))
        {
        std::cerr << "Seed with pixel units of index: " <<
            indexSeed << " does not lie within the image. The images extents are"
          << this->m_Image->GetBufferedRegion() << std::endl;
        exit(-1);
        }      

      seeds[i].SetPosition(sx,sy,sz);
      }
    return seeds;
    }

  void SetImage( InputImageType * image )
    {
    this->m_Image = image;
    }

  typedef InputImageType::IndexType IndexType;
  typedef IndexType::IndexValueType IndexValueType;

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


  // get files in dir
  std::vector< std::string > GetFilesInDirectory(std::string dir)
    {
    std::vector< std::string > files;
    DIR *dp;
    struct dirent *dirp;
    if((dp = opendir(dir.c_str())) == NULL)
      {
      std::cerr << "Error(" << errno << ") opening " << dir << std::endl;
      return files;
      }

    while ((dirp = readdir(dp)) != NULL)
      {
      files.push_back(std::string(dirp->d_name));
      }

    closedir(dp);
    return files;
    }


  double GetZPositionFromFile( std::string file, std::string &sopInstanceUID )
    {
    typedef itk::ImageFileReader< InputImageType > ReaderType;
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( file );
    reader->Update();

    itk::MetaDataDictionary dict = reader->GetMetaDataDictionary();
    itk::ExposeMetaData<std::string>( dict, "0008|0018", sopInstanceUID );

    return reader->GetOutput()->GetOrigin()[2];
    }



  double ROI[6];
  InputImageType * m_Image;
};

#endif
