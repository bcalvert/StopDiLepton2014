#ifndef BASIC_PARTICLE_WRAPPER_H
#define BASIC_PARTICLE_WRAPPER_H

#include "TLorentzVector.h"
#include "BasicVecComponents.h"
#include "PDGUtility.h"

class BasicParticleWrapper {
private:
	TLorentzVector m_VecP4; ///< Internal 4-vector representation
	BasicVecComponents m_BVC; ///< Contains the vector components (Used for adding to TTrees)
	int m_iPDGID;            ///< Associated PDGID, in integer form to allow for signage
public:
	BasicParticleWrapper() = default; ///< default constructor
	void ClearVars();
	bool CheckPDGID(PDGID const &specPDGID);
	friend bool operator<(const BasicParticleWrapper &a, const BasicParticleWrapper &b);
};


/*
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
*/


#endif // BASIC_PARTICLE_WRAPPER_H