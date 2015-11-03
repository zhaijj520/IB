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



#ifndef IBANALYZEREM_H
#define IBANALYZEREM_H

#include "MuonProjection.hh"
#include "AlphaCalculator.hh"

#include "IBAnalyzer.h"
#include "IBVoxCollection.h"
#include "IBVoxRaytracer.h"
#include "IBVoxel.h"
#include <string>

class IBPocaEvaluator;
class IBMinimizationVariablesEvaluator;
class IBAnalyzerEMAlgorithm;


class IBAnalyzerEM : public IBAnalyzer {
    uLibTypeMacro(IBAnalyzerEM,IBAnalyzer)

public:
    struct Event {
        struct Element {
            Matrix2f Wij;
            union {
                Scalarf lambda;
                Scalarf Sij;
            };
            IBVoxel *voxel;
            Scalarf pw;
        };

        struct {
            Vector4f Di;
            Matrix4f E;
            Scalarf  InitialSqrP;
        } header;
        Vector<Element> elements;
    };


    properties()
    {
        Scalarf nominal_momentum;
        Scalarf SijCutEM;
    };

public:
    IBAnalyzerEM(IBVoxCollection &voxels, int nPath=2, double alpha=0., bool doRecoPath=false,
		 bool scatterOnly=false, bool displacementOnly=false, bool oldTCalculation=false,
		 std::string projectFile="", std::string alphaFile="");
    ~IBAnalyzerEM();

    bool AddMuon(const MuonScatterData &muon){ return false;}
    bool AddMuonFullPath(const MuonScatterData &muon, Vector<HPoint3f>& muonPath);

    void SetMuonCollection(IBMuonCollection *muons);

    unsigned int Size();

    void Run(unsigned int iterations, float muons_ratio);

    void SetMLAlgorithm(IBAnalyzerEMAlgorithm *MLAlgorithm);

    uLibGetSetMacro(PocaAlgorithm,IBPocaEvaluator *)
    uLibGetSetMacro(VarAlgorithm,IBMinimizationVariablesEvaluator *)
    uLibGetSetMacro(RayAlgorithm,IBVoxRaytracer *)
    uLibGetSetMacro(UpdateAlgorithm,IBAbstract::IBVoxCollectionUpdateAlgorithm *)

    void filterEventsVoxelMask();

    void filterEventsLineDistance(float min, float max);

    void dumpEventsTTree(const char *filename);

    void SijCut(float threshold);

    void SijGuess(Vector<Vector2f> tpv);

    void Chi2Cut(float threshold);

    void SetVoxCollection(IBVoxCollection *voxels);

    void SetVoxcollectionShift(Vector3f shift);

    void DumpP(const char *filename, float x0 = 0, float x1 = 10);

    Vector<Event> & Events();    
    
private:    
    IBPocaEvaluator                            *m_PocaAlgorithm;
    IBMinimizationVariablesEvaluator           *m_VarAlgorithm;
    IBVoxRaytracer                             *m_RayAlgorithm;
    IBAbstract::IBVoxCollectionUpdateAlgorithm *m_UpdateAlgorithm;
    friend class IBAnalyzerEMPimpl;    
    class IBAnalyzerEMPimpl *m_d;

    int m_nPath;            //---- Int to indicate whether to build a 1, 2 or 3-path
    double m_alpha;         //---- Relative distance along the trajectory to build the 3-path   
    bool m_useRecoPath;     //---- Use the true muon path from MC
    bool m_scatterOnly;     //---- Only use scattering information to build images
    bool m_displacementOnly;//---- Only use displacement information to build images
    bool m_oldTCalculation; //---- Use the old method of calculating length parameter T
    bool m_project;

    MuonProjection m_projector;
    AlphaCalculator m_alphaCalc;
};

inline void IBAnalyzerEM::init_properties() {
    $_init();
    $$.nominal_momentum = 3;
}


#endif // IBANALYZEREM_H
