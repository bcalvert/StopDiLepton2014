#ifndef LEPTON_H
#define LEPTON_H

#include "BasicParticleWrapper.h"

class Lepton : public BasicParticleWrapper {
private:
    GenParticle m_GP; ///< Internal generator-level particle 
    bool m_bIsGenMatched;
    bool m_bIsPromptLepton;
    int m_iLepQual;
    int m_iPDGID;
    float m_fRelPFLepIso;

public:
    void IsBadLep();
    void ClearVars();

    bool IsElec() {
        return (abs(m_iPDGID) == 11);
    }

    bool IsMuon() {
        return (abs(m_iPDGID) == 13);
    }
};

void Lepton::ClearVars()
{
    this->BasicParticleWrapper::ClearVars();
    m_fRelPFLepIso = -1;
    m_iLepQual = -1;
}
void Lepton::IsBadLep()
{
    ClearVars();
    m_fRelPFLepIso = -99999;
}

typedef struct Lepton {
    TLorentzVector  P4;
    BasicVecComponents BVC;
    GenParticle GP;
    bool m_fIsGenMatched;
    bool m_fIsPromptLepton;
    int  m_iLepQual;
    int             m_iPDGID;
    float           m_fRelPFLepIso;
    void isBadLep() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        m_iPDGID = 0;
        m_fRelPFLepIso = -99999;
        m_iLepQual = -1;
    }
    bool isElec() {
        return (abs(m_iPDGID) == 11);
    }
    bool isMuon() {
        return (abs(m_iPDGID) == 13);
    }
    void SetPromptLeptonStatus() {
        m_fIsPromptLepton = m_fIsGenMatched;
        m_fIsPromptLepton &= (abs(GP.Momm_iPDGID) == 15 || abs(GP.Momm_iPDGID) == 23 || abs(GP.Momm_iPDGID) == 24);
    }
    void PrintVals() {
        cout << "Lepton Vec Components: " << endl;
        BVC.PrintVals(1);
        cout << "Lepton m_iPDGID " << m_iPDGID << endl;
        cout << "Gen Particle Matched? " << m_fIsGenMatched << endl;
        if (m_fIsGenMatched) {
            cout << "printing matched genLep info " << endl;
            GP.BVC.PrintVals(1);
        }
        else {
            cout << "not Gen Matched" << endl;
        }
        cout << "m_iLepQual " << m_iLepQual << endl;
    }
    float TempEtaFixElectron() {
        float EtaEBEnd = 1.4442;
        float EtaEEBegin = 1.566;
        float EtaEEEnd = 2.5;
        float safetyFactor = 1E-3;

        //add safetyFactors to the various eta endpoints for safety purposes
        EtaEBEnd -= safetyFactor;
        EtaEEBegin += safetyFactor;
        EtaEEEnd -= safetyFactor;
        
        float absLepEta = fabs(BVC.Vec_Eta);
        if (absLepEta > EtaEEEnd) {
            return BVC.Vec_Eta < 0 ? -1 * EtaEEEnd : EtaEEEnd;
        }
        return BVC.Vec_Eta > 0 ? TruncateNum(BVC.Vec_Eta, EtaEBEnd, EtaEEBegin, safetyFactor) : TruncateNum(BVC.Vec_Eta, -1 * EtaEEBegin, -1 * EtaEBEnd, safetyFactor);
    }
} Lepton;

inline bool operator<(const Lepton &a, const Lepton &b)
{
    return (a.BVC.Vec_Pt < b.BVC.Vec_Pt);
}

inline bool operator>(const Lepton &a, const Lepton &b)
{
    return (a.BVC.Vec_Pt > b.BVC.Vec_Pt);
}

#endif // LEPTON_H
