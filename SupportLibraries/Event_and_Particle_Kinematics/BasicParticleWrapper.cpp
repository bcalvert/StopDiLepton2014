#include "BasicParticleWrapper.h"

#include "ParticleUtilityFunctions.h"

#include "TLorentzVector.h"
void BasicParticleWrapper::ClearVars()
{
	TLorentzVector vecTemp; /// \todo fix this
	ParticleUtility::DefaultLorentzVector(&vecTemp);
	m_iPDGID = 0;
	m_VecP4 = vecTemp;
	m_BVC.SetP4Vals(&vecTemp);
}

bool BasicParticleWrapper::CheckPDGID(PDGID const &specPDGID)
{
	return abs(m_iPDGID) == static_cast<int>(specPDGID); ///\todo fix this
}

bool operator< (const BasicParticleWrapper &a, const BasicParticleWrapper &b)
{
    return a.m_BVC.GetPt() < b.m_BVC.GetPt();
}