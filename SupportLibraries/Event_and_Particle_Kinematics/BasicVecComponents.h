#ifndef BASIC_VEC_COMPONENTS_H
#define BASIC_VEC_COMPONENTS_H
class TLorentzVector;

class BasicVecComponents {
public:
    BasicVecComponents();
    BasicVecComponents(TLorentzVector * pVecP4);
    void PrintVals(bool bPrintCartesian = true);
    void SetP4Vals(TLorentzVector * pVecP4);

    float GetPt() const;
    float GetEta() const;
    float GetPhi() const;
    float GetMass() const;
    float GetPx() const;
    float GetPy() const;
    float GetPz() const;
    float GetEn() const;
private:
    float m_fPt, m_fEta, m_fPhi, m_fMass;
    float m_fPx, m_fPy, m_fPz, m_fEn;

    void UpdateParams(TLorentzVector * pVecP4);
};


#endif // BASIC_VEC_COMPONENTS_H