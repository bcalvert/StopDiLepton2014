
#include "ParticleUtilityFunctions.h"
#include "TMath.h"
#include "TLorentzVector.h"

float ParticleUtility::dPhi(float fPhi1, float fPhi2)
{
    float fDPhi = fPhi1-fPhi2;
    while (fDPhi >= TMath::Pi()) fDPhi -= 2*TMath::Pi();
    while (fDPhi < -1*TMath::Pi()) fDPhi += 2*TMath::Pi();
    return fabs(fDPhi);
}

float ParticleUtility::dPhi(TLorentzVector * p4VecA, TLorentzVector * p4VecB)
{
	return dPhi(p4VecA->Phi(), p4VecB->Phi());
}

float ParticleUtility::dR(float fEta1, float fPhi1, float fEta2, float fPhi2)
{
    float fDPhi = dPhi(fPhi1, fPhi2);
    float fDEta = fEta1 - fEta2;
    float fResult = TMath::Power(fDPhi, 2) + TMath::Power(fDEta, 2); // check if this is wildly inefficient over simple multiplication
    fResult = TMath::Sqrt(fResult);
    return fResult;
}

float ParticleUtility::dR(TLorentzVector * p4VecA, TLorentzVector * p4VecB)
{
	Float_t fDPhi = dPhi(p4VecA, p4VecB);
	Float_t fDEta = p4VecA->Eta() - p4VecB->Eta();

	Float_t fResult = TMath::Power(fDPhi, 2) + TMath::Power(fDEta, 2); // check if this is wildly inefficient over simple multiplication
    fResult = TMath::Sqrt(fResult);
    return fResult;
}