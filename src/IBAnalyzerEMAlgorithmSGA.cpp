
#include "IBAnalyzerEMAlgorithmSGA.h"





/************************************************
 * Sij 4 HIDDEN DATA IMPLEMENTATION USING EIGEN *
 ************************************************/

void IBAnalyzerEMAlgorithmSGA_PXTZ::evaluate(Matrix4f &Sigma,
                                                IBAnalyzerEMAlgorithm::Event *evc)
{

    Matrix4f iS;
    iS = Sigma.inverse();
    Matrix4f Wij = Matrix4f::Zero();

    Matrix4f Dn = iS * (evc->header.Di * evc->header.Di.transpose());

    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Wij.block<2,2>(0,0) = evc->elements[j].Wij;
        Wij.block<2,2>(2,2) = evc->elements[j].Wij;

        Matrix4f Bn = iS * Wij;
        evc->elements[j].Sij =  ((Bn * Dn).trace() - Bn.trace()) *
                evc->elements[j].lambda * evc->elements[j].lambda *
                evc->elements[j].pw / 2;
    }

}


void IBAnalyzerEMAlgorithmSGA_PXTZ2::evaluate(Matrix4f &Sigma,
                                              IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix4f iS;
    iS = Sigma.inverse();
    Matrix4f Wij = Matrix4f::Zero();

    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Wij.block<2,2>(0,0) = evc->elements[j].Wij;
        Wij.block<2,2>(2,2) = evc->elements[j].Wij;
        Matrix4f iSWij = iS * Wij;

        float DISWISD        = evc->header.Di.transpose() * iSWij * iS * evc->header.Di;
        evc->elements[j].Sij = (DISWISD - iSWij.trace()) * evc->elements[j].lambda *
                evc->elements[j].lambda * evc->elements[j].pw / 2;

    }
}


bool IBAnalyzerEMAlgorithmSGA_PXTZ3::ComputeSigma(Matrix4f &Sigma, IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix2f _Sigma = Matrix2f::Zero();

    if(unlikely(evc->elements.size() < 3))
        return BaseClass::ComputeSigma(Sigma,evc);

    float RLen = 0;
    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        evc->elements[j].lambda = evc->elements[j].voxel->Value;
        float fw = 1 + RLen * m_Factor;
        RLen += evc->elements[j].lambda * evc->elements[j].Wij(0,0);
        _Sigma += fw * evc->elements[j].Wij * evc->elements[j].lambda;

    }

    _Sigma *= evc->header.InitialSqrP;

    Sigma.block<2,2>(0,0) = _Sigma;
    Sigma.block<2,2>(2,2) = _Sigma;
    Sigma += evc->header.E;

    return true;
}



void IBAnalyzerEMAlgorithmSGA_PXTZ4::evaluate(Matrix4f &Sigma, IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix4f iS;
    iS = Sigma.inverse();
    Matrix4f Wij = Matrix4f::Zero();

    Matrix4f Dn = iS * (evc->header.Di * evc->header.Di.transpose());



    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Wij.block<2,2>(0,0) = evc->elements[j].Wij;
        Wij.block<2,2>(2,2) = evc->elements[j].Wij;
        Matrix4f Bn = iS * Wij;
        float Sij = ((Bn * Dn).trace() - Bn.trace()) *
                evc->elements[j].lambda * evc->elements[j].lambda *
                evc->elements[j].pw / 2;
        evc->elements[j].Sij =  Sij;
    }

    Vector<Event::Element> copy = evc->elements;
    const float factor[5] = {2,3,3,3,2};

    float Sij;
    if(evc->elements.size() > 5)
    for(int i=2; i<evc->elements.size()-2; ++i) {
        Sij=0;
        float maxSij = 0;
        for ( int j=-2; j<3; ++j) {
            float s = copy[i+j].Sij * factor[j+2];
            if(s > maxSij) maxSij = s;
            Sij += s;
        }
        Sij -= maxSij;
        Sij /= 10;
        evc->elements[i].Sij = Sij;
    }
}




/************************************************
 * Sij 2 HIDDEN DATA (S,X) EIGEN IMPLEMENTATION *
 ************************************************/

void IBAnalyzerEMAlgorithmSGA_PX::evaluate(Matrix4f &Sigma,
                                              IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix2f iS;
    {
        Matrix2f S;
        S << Sigma(0,0), Sigma(0,1), Sigma(1,0), Sigma(1,1);
        iS = S.inverse();
    }
    Vector2f Di(evc->header.Di(0),evc->header.Di(1));

    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Matrix2f iSWij = iS * evc->elements[j].Wij;
        float DISWISD  = Di.transpose() * iSWij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iSWij.trace()) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}

void IBAnalyzerEMAlgorithmSGA_TZ::evaluate(Matrix4f &Sigma,
                                              IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix2f iS;
    {
        Matrix2f S;
        S << Sigma(2,2), Sigma(2,3), Sigma(3,2), Sigma(3,3);
        iS = S.inverse();
    }
    Vector2f Di(evc->header.Di(2),evc->header.Di(3));

    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Matrix2f iSWij = iS * evc->elements[j].Wij;
        float DISWISD  = Di.transpose() * iSWij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iSWij.trace()) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}




/******************************************
 * Sij 2 HIDDEN DATA (S,S) IMPLEMENTATION *
 ******************************************/

void IBAnalyzerEMAlgorithmSGA_PT::evaluate(Matrix4f &Sigma,
                                              IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix2f iS;
    {
        Matrix2f S;
        S << Sigma(0,0), Sigma(0,2), Sigma(2,0), Sigma(2,2);
        iS = S.inverse();
    }
    Vector2f Di(evc->header.Di(0),evc->header.Di(2));

    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Matrix2f Wij;
        Wij << evc->elements[j].Wij(0,0),0,
               0,evc->elements[j].Wij(0,0);
        Matrix2f iSWij = iS * Wij;
        float DISWISD  = Di.transpose() * iSWij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iSWij.trace()) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}


/******************************************
* Sij 2 HIDDEN DATA (D,D) IMPLEMENTATION *
******************************************/

void IBAnalyzerEMAlgorithmSGA_XZ::evaluate(Matrix4f &Sigma,
                                              IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix2f iS;
    {
        Matrix2f S;
        S << Sigma(1,1), Sigma(1,3), Sigma(3,1), Sigma(3,3);
        iS = S.inverse();
    }
    Vector2f Di(evc->header.Di(1),evc->header.Di(3));

    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Matrix2f Wij;
        Wij << evc->elements[j].Wij(1,1),0,
               0,evc->elements[j].Wij(1,1);
        Matrix2f iSWij = iS * Wij;
        float DISWISD  = Di.transpose() * iSWij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iSWij.trace()) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}


/****************************************
 * Sij 1 HIDDEN DATA (S) IMPLEMENTATION *
 ****************************************/

void IBAnalyzerEMAlgorithmSGA_P::evaluate(Matrix4f &Sigma,
                                             IBAnalyzerEMAlgorithm::Event *evc)
{
    float Di = evc->header.Di(0);
    float iS = 1/Sigma(0,0);
    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        float Wij = evc->elements[j].Wij(0,0);
        float DISWISD = Di * iS * Wij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iS * Wij) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}

void IBAnalyzerEMAlgorithmSGA_T::evaluate(Matrix4f &Sigma,
                                             IBAnalyzerEMAlgorithm::Event *evc)
{
    float Di = evc->header.Di(2);
    float iS = 1/Sigma(2,2);
    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        float Wij = evc->elements[j].Wij(0,0);
        float DISWISD = Di * iS * Wij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iS * Wij) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}





/************************************************
* Sij 1 HIDDEN DATA (D) CLASSIC IMPLEMENTATION *
************************************************/

void IBAnalyzerEMAlgorithmSGA_X::evaluate(Matrix4f &Sigma,
                                             IBAnalyzerEMAlgorithm::Event *evc)
{
    float Di = evc->header.Di(1);
    float iS = 1/Sigma(1,1);
    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        float Wij = evc->elements[j].Wij(1,1);
        float DISWISD = Di * iS * Wij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iS * Wij) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}

void IBAnalyzerEMAlgorithmSGA_Z::evaluate(Matrix4f &Sigma,
                                             IBAnalyzerEMAlgorithm::Event *evc)
{
    float Di = evc->header.Di(3);
    float iS = 1/Sigma(3,3);
    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        float Wij = evc->elements[j].Wij(1,1);
        float DISWISD = Di * iS * Wij * iS * Di;
        evc->elements[j].Sij = (DISWISD - iS * Wij) * evc->elements[j].pw *
                evc->elements[j].lambda * evc->elements[j].lambda / 2.;
    }
}




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// STIMA SU P //


void IBAnalyzerEMAlgorithmSGA_M::evaluate(Matrix4f &Sigma, IBAnalyzerEMAlgorithm::Event *evc)
{
    Matrix4f iS = Sigma.inverse();
    Matrix4f Wij = Matrix4f::Zero();

    Matrix4f Dn = iS * (evc->header.Di * evc->header.Di.transpose());

    Scalarf p = 0;
    for (unsigned int j = 0; j < evc->elements.size(); ++j) {
        Wij.block<2,2>(0,0) = evc->elements[j].Wij;
        Wij.block<2,2>(2,2) = evc->elements[j].Wij;

        Matrix4f Bn = iS * Wij;
        evc->elements[j].Sij = 0;
        p += ((Bn * Dn).trace() - Bn.trace()) * evc->elements[j].lambda *
                pow(evc->elements[j].pw,4) / 2;
    }
    evc->header.InitialSqrP += p / evc->elements.size();

}
