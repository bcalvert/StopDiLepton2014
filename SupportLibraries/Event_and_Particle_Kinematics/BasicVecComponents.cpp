

#include "TLorentzVector.h"

#include <iostream>

#include "BasicVecComponents.h"

BasicVecComponents::BasicVecComponents()
: m_fPt(0)
, m_fEta(0)
, m_fPhi(0)
, m_fMass(0)
, m_fPx(0)
, m_fPy(0)
, m_fPz(0)
, m_fEn(0)
{}

BasicVecComponents::BasicVecComponents(TLorentzVector * pVecP4) 
: BasicVecComponents()
{
	this->UpdateParams(pVecP4);
}

void BasicVecComponents::UpdateParams(TLorentzVector * pVecP4) {
	m_fPt    = pVecP4->Pt();
	if (m_fPt == 0) {
		m_fEta = 1E9;
	}
	else {
		m_fEta   = pVecP4->Eta();   
	}
	m_fPhi   = pVecP4->Phi();
	m_fMass  = pVecP4->M();
	m_fPx    = pVecP4->Px();
	m_fPy    = pVecP4->Py();
	m_fPz    = pVecP4->Pz();
	m_fEn    = pVecP4->E();
}

void BasicVecComponents::PrintVals(bool bPrintCartesian) {
	if (bPrintCartesian) {
		std::cout << "m_fPt " << m_fPt << std::endl;
		std::cout << "m_fEta " << m_fEta << std::endl;
		std::cout << "m_fPhi " << m_fPhi << std::endl;
		std::cout << "m_fMass " << m_fMass << std::endl;
	}
	else {
		std::cout << "m_fPx " << m_fPx << std::endl;
		std::cout << "m_fPy " << m_fPy << std::endl;
		std::cout << "m_fPz " << m_fPz << std::endl;
		std::cout << "m_fEn " << m_fEn << std::endl;
	}
}
void BasicVecComponents::SetP4Vals(TLorentzVector * pVecP4) {
	pVecP4->SetPxPyPzE(m_fPx, m_fPy, m_fPz, m_fEn);
}

float BasicVecComponents::GetPt() const
{
	return m_fPt;
}
float BasicVecComponents::GetEta() const
{
	return m_fEta;
}
float BasicVecComponents::GetPhi() const
{
	return m_fPhi;
}
float BasicVecComponents::GetMass() const
{
	return m_fMass;
}
float BasicVecComponents::GetPx() const
{
	return m_fPx;
}
float BasicVecComponents::GetPy() const
{
	return m_fPy;
}
float BasicVecComponents::GetPz() const
{
	return m_fPz;
}
float BasicVecComponents::GetEn() const
{
	return m_fEn;
}

