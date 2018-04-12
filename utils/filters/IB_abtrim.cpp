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

#include <IBVoxFilters.h>
#include "IBVoxCollection.h"

#include <Vtk/vtkVoxImage.h>

#include <Core/Options.h>
#include <Core/Archives.h>

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


struct Params {
    std::string file;
    std::string size;
    std::string abtrim;
    Scalarf scale;
    Scalarf sigma;
} p = {
    std::string("source_image.vtk"),
    std::string("5 5 5"), // default size
    std::string("0 1"),   // default ab
    1,  // default scaling factor
    1 // kernel rms voxels
};

int main(int argc, char *argv[])
{

    if(argc < 2)
    {
        std::cerr << "No input filename given ..\n" <<
                     "use: abtrim filename.vtk voxel_size a_trim b_trim scale\n";
        exit(1);
    }
    else {
        p.file = std::string(argv[1]);
    }

    if(argc == 3)
    {
        p.scale = atof(argv[2]);
    }

    //    if(argc == 6)
    //    {
    //        p.size = atoi(argv[2]);
    //        p.atrim = atoi(argv[3]);
    //        p.btrim = atoi(argv[4]);
    //        p.scale = atof(argv[5]);
    //    }

    Options opt;

    opt.add_options()
            ("help", "get thi help message")
            ("size",&p.size, "Vector3i for kernel mask size [x y z]")
            ("abtrim",&p.abtrim,"Vector2i for a and b values")
            ("scale",&p.scale,"Scaling factor")
            ("sigma", &p.sigma,"kernel rms in voxels");

    opt.parse_command_line(argc,argv);

    // convert string parameters //

    Vector3i size;
    p.size >> size;
    Vector2i abtrim;
    p.abtrim >> abtrim;

    std::cout << "// --------- [abtrim] ----------------- //\n"
              << "size [ " << p.size << "] "
              << " a b = " << p.abtrim << "\n"
              << " scale factor = " << p.scale << "\n"
              << "// ------------------------------------ //\n";


    IBVoxCollection image(Vector3i(0,0,0));

    char filename[150];
    sprintf(filename,"%s",p.file.c_str());

    if(!image.ImportFromVtk(filename))
    {
        std::cerr << "Error: Could not open file\n";
        exit(1);
    }

    IBVoxFilter_Abtrim trim(size);
    IBFilterGaussShape shape(p.sigma);
    trim.SetKernelWeightFunction(shape);
    trim.SetABTrim(abtrim(0), abtrim(1));
    trim.SetImage(&image);
    trim.Run();

    image *= p.scale;

    sprintf(filename,"%s_trim%d%d%d%d%d_scale%.2f_sigma%.1f.vtk",
            FileNameRemoveExtension(p.file.c_str()).c_str(),
            size(0),size(1),size(2), abtrim(0), abtrim(1),
            p.scale,p.sigma);
    image.ExportToVtk(filename,0);


    return 0;
}
