class TLorentzVector;

class BasicVecComponents {
public:
    BasicVecComponents();
    BasicVecComponents(TLorentzVector * pVecP4);
    void PrintVals(bool bPrintCartesian = true);
    void SetP4Vals(TLorentzVector * pVecP4);
private:
    float m_fPt, m_fEta, m_fPhi, m_fMass;
    float m_fPx, m_fPy, m_fPz, m_fEn;

    void UpdateParams(TLorentzVector * pVecP4);
};