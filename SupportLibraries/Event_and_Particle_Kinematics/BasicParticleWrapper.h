#include "TLorentzVector.h"
#include "BasicVecComponents.h"

class BasicParticleWrapper {
public:
	BasicParticleWrapper();
private:
	TLorentzVector m_VecP4;
	BasicVecComponents m_BVC; ///< 
};

using namespace std;
typedef struct GenJet{
    ///struct to contain relevant info for generator level jets
    TLorentzVector  P4;
    BasicVecComponents BVC;
    int             seedPDGID;
    float           invisE;
    bool            isBJet;
    void ClearVars() {
        DefaultLorentzVector(&P4);
        BVC.ExtractParams(&P4);
        seedPDGID = 0;
        invisE = 0.0;
        isBJet = false;
    }
} GenJet;
inline bool operator<(const GenJet &a, const GenJet &b)
{
    return (a.BVC.Vec_Pt < b.BVC.Vec_Pt);
}

inline bool operator>(const GenJet &a, const GenJet &b)
{
    return (a.BVC.Vec_Pt > b.BVC.Vec_Pt);
}