#ifndef IBPOCAEVALUATOR_H
#define IBPOCAEVALUATOR_H

#include "Core/Macros.h"
#include "Math/Dense.h"
#include "Math/Utils.h"
#include "Detectors/MuonScatter.h"
#include "IBMuonError.h"

using namespace uLib;

class IBTiltedAxisPocaEvaluator;

class IBPocaEvaluator
{
public:
    enum IBPocaEvaluationAlgorithms {
        TiltedAxis
    };

    static IBPocaEvaluator* New(enum IBPocaEvaluationAlgorithms S);

    virtual bool evaluate(MuonScatterData muon) = 0;
    virtual HPoint3f getPoca() = 0;

protected:
    IBPocaEvaluator() {}
    virtual ~IBPocaEvaluator() {}
};

#endif // IBPOCAEVALUATOR_H
