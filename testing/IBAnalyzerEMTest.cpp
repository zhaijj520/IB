#include <iostream>

#include <TFile.h>
#include <TTree.h>


#include "IBPocaEvaluator.h"
#include "IBAnalyzerEM.h"
#include "IBVoxRaytracer.h"
#include "IBMinimizationVariablesEvaluator.h"
#include "IBVoxCollectionCap.h"
#include "IBMuonError.h"
#include "Detectors/MuonScatter.h"
#include "IBMuonEventTTreeReader.h"
#include "IBMuonEventTTreeR3DmcReader.h"
#include "IBVoxFilters.h"
#include "IBAnalyzerWPoca.h"
#include "IBMAPUpdateDensityAlgorithms.h"

#include <Vtk/uLibVtkViewer.h>
#include <Vtk/vtkMuonEvent.h>
#include <Vtk/vtkStructuredGrid.h>

using namespace uLib;

int main() {

    // errors //
//    IBMuonError sigma(11.93,2.03, 18.53,2.05);
    IBMuonError sigma(12.24,0, 18.85,0);

    // reader //
    TFile* f = new TFile ("/var/local/data/root/ROC_sets/201212/20121223/muSteel_PDfit_20121223_1_v11.root");
    TTree* t = (TTree*)f->Get("n");
    IBMuonEventTTreeReader* reader = IBMuonEventTTreeReader::New(IBMuonEventTTreeReader::R3D_MC);
    reader->setTTree(t);
    reader->setError(sigma);
    reader->setMomentum(0.7);
    reader->selectionCode(IBMuonEventTTreeR3DmcReader::All);

    // voxels //

    IBVoxel zero = {0.1E-6,0,0};
    IBVoxCollectionCap voxels(Vector3i(140,72,60));
    voxels.SetSpacing (Vector3f(5,5,5));
    voxels.SetPosition(Vector3f(-350,-180,-150));

    voxels.InitLambda(zero);



    // poca //
    IBPocaEvaluator* processor = IBPocaEvaluator::New(IBPocaEvaluator::LineDistance);

    // tracer //
    IBVoxRaytracer* tracer = new IBVoxRaytracer(voxels);

    // variables //
    IBMinimizationVariablesEvaluator* minimizator =
            IBMinimizationVariablesEvaluator::New(IBMinimizationVariablesEvaluator::NormalPlane);
    minimizator->setRaytracer(tracer);

    // analyzer //
    IBAnalyzerEM* aem = new IBAnalyzerEM;
    aem->SetVoxCollection(&voxels);
    aem->SetPocaAlgorithm(processor);
    aem->SetRaytracer(tracer);
    aem->SetVariablesAlgorithm(minimizator);


    std::cout << "There are " << reader->getNumberOfEvents() << " events!\n";
    int tot=0;
    int ev = 1375250;
    for (int i=0; i<ev; i++) {
        MuonScatter mu;
        if(reader->readNext(&mu)) {
            aem->AddMuon(mu);
            tot++;
        }

    }



    char file[100];

    int it   = 10;
    int pwdrop = 10;
    int drop = 100;

    aem->SijCut(60);
    std::cout << "Survived: [" << aem->Size() << "]\n";

    aem->parameters().pweigth = IBAnalyzerEM::PWeigth_pw;

    // SGA //
    std::cout << "SGA PXTZ\n";
    for (int i=1; i<=it; ++i) {
        aem->Run(pwdrop,1);
        sprintf(file, "20121223_PXTZ_SGA_ps1_pw_%i.vtk", i*drop);
        voxels.ExportToVtk(file,0);

        std::cout << "updating pw ... ";
        aem->UpdatePW();
        std::cout << "done! \n";
    }


    delete aem;
    delete minimizator;
    return 0;

}

