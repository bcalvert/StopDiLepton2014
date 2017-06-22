#ifndef PARTICLE_UTILITY_FUNCTIONS_H
#define PARTICLE_UTILITY_FUNCTIONS_H

class TLorentzVector;
namespace ParticleUtility {
	float dPhi(float fPhi1, float fPhi2);
	float dPhi(TLorentzVector * p4VecA, TLorentzVector * p4VecB);

	float dR(float fEta1, float fPhi1, float fEta2, float fPhi2);
	float dR(TLorentzVector * p4VecA, TLorentzVector * p4VecB);


	void DefaultLorentzVector(TLorentzVector * p4Vec);

	void PrintLorentzVector(TLorentzVector * p4Vec);
} // ParticleUtility

/// \todo Add in Float_t instead of float
#endif // PARTICLE_UTILITY_FUNCTIONS_H