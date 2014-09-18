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



#include <Core/Vector.h>
#include "IBVoxCollectionCap.h"
#include "IBAnalyzerTrackLengths.h"
#include "IBVoxRaytracer.h"
#include "IBPocaEvaluator.h"

using namespace uLib;


///// PIMPL ////////////////////////////////////////////////////////////////////


class IBAnalyzerTrackLengthsPimpl {
public:
    struct Event {
        struct Element {
            Matrix4f Wij;
            IBVoxel *voxel;
        };
        Vector<Element> elements;
    };

    IBAnalyzerTrackLengthsPimpl() :
        m_RayAlgorithm(NULL),
        m_PocaAlgorithm(NULL)
    {}

    void Project(Event *evc) {
        IBVoxel *vox;
        for (unsigned int j = 0; j < evc->elements.size(); ++j) {
            vox = evc->elements[j].voxel;
            vox->Value += evc->elements[j].Wij(0,0);
            vox->Count++;
        }
    }

    // members //
    Vector<Event> m_Events;
    VoxRaytracer *m_RayAlgorithm;
    IBPocaEvaluator *m_PocaAlgorithm;
};




IBAnalyzerTrackLengths::IBAnalyzerTrackLengths() :
    d(new IBAnalyzerTrackLengthsPimpl)
{}

IBAnalyzerTrackLengths::~IBAnalyzerTrackLengths()
{
    delete d;
}

bool IBAnalyzerTrackLengths::AddMuon(const MuonScatterData &muon)
{
    if(!d->m_RayAlgorithm || !d->m_PocaAlgorithm) return false;
    IBAnalyzerTrackLengthsPimpl::Event evc;

    IBVoxRaytracer::RayData ray;
    { // Get RayTrace RayData //
        HPoint3f entry_pt,poca,exit_pt;
        if( !d->m_RayAlgorithm->GetEntryPoint(muon.LineIn(),entry_pt) ||
                !d->m_RayAlgorithm->GetExitPoint(muon.LineOut(),exit_pt) )
            return false;
        bool test = d->m_PocaAlgorithm->evaluate(muon);
        poca = d->m_PocaAlgorithm->getPoca();
        if(test && this->GetVoxCollection()->IsInsideBounds(poca)) {
            poca = d->m_PocaAlgorithm->getPoca();
            ray = d->m_RayAlgorithm->TraceBetweenPoints(entry_pt,poca);
            ray.AppendRay( d->m_RayAlgorithm->TraceBetweenPoints(poca,exit_pt) );
        }
        else {
            ray = d->m_RayAlgorithm->TraceBetweenPoints(entry_pt,exit_pt);
        }
    }

    IBAnalyzerTrackLengthsPimpl::Event::Element elc;
    Scalarf T = ray.TotalLength();
    for(int i=0; i<ray.Data().size(); ++i)
    {
        const IBVoxRaytracer::RayData::Element *el = &ray.Data().at(i);
        elc.voxel = &this->GetVoxCollection()->operator [](el->vox_id);
        Scalarf L = el->L;
        T -= L;

        Matrix2f wij_block;
        wij_block << L ,          L*L/2 + L*T,
                     L*L/2 + L*T, L*L*L/3 + L*L*T + L*T*T;
        elc.Wij = Matrix4f::Zero();
        elc.Wij.block<2,2>(0,0) = wij_block;
        elc.Wij.block<2,2>(2,2) = wij_block;

        evc.elements.push_back(elc);
    }
    d->m_Events.push_back(evc);
    return true;
}

void IBAnalyzerTrackLengths::Run(unsigned int iterations, float muons_ratio)
{
    for(int i=0; i<d->m_Events.size(); ++i)
        d->Project(&d->m_Events[i]);
}

void IBAnalyzerTrackLengths::SetRayAlgorithm(IBVoxRaytracer *raytracer)
{
    d->m_RayAlgorithm = raytracer;
}

void IBAnalyzerTrackLengths::SetPocaAlgorithm(IBPocaEvaluator *evaluator)
{
    d->m_PocaAlgorithm = evaluator;
}
