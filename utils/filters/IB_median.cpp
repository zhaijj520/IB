/*//////////////////////////////////////////////////////////////////////////////
// CMT Cosmic Muon Tomography project //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

  Copyright (c) 2014, Universita' degli Studi di Padova, INFN sez. di Padova

  Coordinators: Prof. Gianni Zumerle < gianni.zumerle@pd.infn.it >
                Paolo Checchia       < paolo.checchia@pd.infn.it >

  Authors: Andrea Rigoni Garola < andrea.rigoni@pd.infn.it >
           Matteo Furlan        < nuright@gmail.com >
           Sara Vanini          < sara.vanini@pd.infn.it >

  All rights reserved
  ------------------------------------------------------------------

  This file can not be copied and/or distributed without the express
  permission of  Prof. Gianni Zumerle  < gianni.zumerle@pd.infn.it >

//////////////////////////////////////////////////////////////////////////////*/



#include <string>

#include <TFile.h>
#include <TH1F.h>
#include <IBVoxFilters.h>
#include "IBVoxCollection.h"
#include <Vtk/vtkVoxImage.h>

using namespace uLib;



std::string GetFileExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(FileName.find_last_of(".")+1);
    return "";
}

std::string FileNameRemoveExtension(const std::string& FileName)
{
    if(FileName.find_last_of(".") != std::string::npos)
        return FileName.substr(0,FileName.find_last_of("."));
    return "";
}


int main(int argc, char *argv[])
{

    if(argc == 1)
    {
        std::cerr << "No input filename given ..\n" <<
                     "use: IB_median filename.vtk kernel_size scale\n";
        exit(1);
    }

    struct Params {
        char *file;
        int size;
        Scalarf scale;
    } parameters = {
        argv[1],
                5, // default size
                1  // default scaling factor
    };

    if(argc == 3)
    {
        parameters.scale = atof(argv[2]);
    }

    if(argc == 6)
    {
        parameters.size = atoi(argv[2]);
        parameters.scale = atof(argv[5]);
    }


    std::cout << "// --------- [abtrim] ----------------- //\n"
              << "size [ " << parameters.size << "] "
              << " scale factor = " << parameters.scale << "\n"
              << "// ------------------------------------ //\n";


    IBVoxCollection image(Vector3i(0,0,0));

    char filename[150];
    sprintf(filename,"%s",parameters.file);

    if(!image.ImportFromVtk(filename))
    {
        std::cerr << "Error: Could not open file\n";
        exit(1);
    }

    int s = parameters.size;
    IBVoxFilter_Median trim(Vector3i(s,s,s));
    //    IBFilterGaussShape shape(0.7);
    //    trim.SetKernelWeightFunction(shape);
    Vector <float> values;
    for (int i=0; i<trim.GetKernelData().GetDims().prod(); ++i) {
        values.push_back(1.);
    }
    trim.SetKernelNumericXZY(values);
    trim.SetImage(&image);
    trim.Run();

    image *= parameters.scale;

    sprintf(filename,"%s_median%d_scale%.2f.vtk",
            FileNameRemoveExtension(parameters.file).c_str(),
            parameters.size,
            parameters.scale);
    image.ExportToVtk(filename,0);


    return 0;
}
