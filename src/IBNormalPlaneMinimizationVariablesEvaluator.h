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



#ifndef IBNORMALPLANEMINIMIZATIONVARIABLESEVALUATOR_H
#define IBNORMALPLANEMINIMIZATIONVARIABLESEVALUATOR_H

#include "Core/Object.h"
#include "Core/ObjectProps.h"

#include "IBMinimizationVariablesEvaluator.h"

using namespace uLib;


class IBNormalPlaneMinimizationVariablesEvaluator : public IBMinimizationVariablesEvaluator
{
public:
    uLibTypeMacro(IBNormalPlaneMinimizationVariablesEvaluator,IBMinimizationVariablesEvaluator)

    ULIB_props() {
        bool    use_free_rotation;
        Scalarf alphaXZ;
    };

    bool m_scatterOnly, m_displacementOnly, m_oneD;
    
    IBNormalPlaneMinimizationVariablesEvaluator();
    ~IBNormalPlaneMinimizationVariablesEvaluator();

    bool evaluate(MuonScatterData muon);

    Vector4f getDataVector();
    Scalarf  getDataVector(int i);
    Matrix4f getCovarianceMatrix();
    Scalarf  getCovarianceMatrix(int i, int j);
    void setRaytracer(IBVoxRaytracer *tracer);
    void setDisplacementScatterOnly(bool,bool,bool);
    // virtual void setConfiguration();
private:
    friend class IBNormalPlaneMinimizationVariablesEvaluatorPimpl;
    class IBNormalPlaneMinimizationVariablesEvaluatorPimpl *d;
};

inline void IBNormalPlaneMinimizationVariablesEvaluator::init_properties() {
    $_init();
    $$.use_free_rotation = 0;
    $$.alphaXZ = 1; // 0 = X ---> 1 = Z
}


#endif // IBNORMALPLANEMINIMIZATIONVARIABLESEVALUATOR_H
