#ifndef STOP_HIST_BIN_BOUNDS_H
#define STOP_HIST_BIN_BOUNDS_H
// Structs for plotting/booking of histograms that are specific to the Stop analysis

#include "TMath.h"
#include "SpecialAxisBounds.h"

struct StopHistBinBounds {
	///\todo implement with static pointer
	///\todo probably also implement with text file reader?
    // Contains the general set of InputHistBinBounds that the stop analysis uses for various
    // types of histograms -- most names should be fairly self-explanatory    
    SpecialAxisBounds SAB;
    InputHistBinBounds m_histBounds_Phi;
    InputHistBinBounds m_histBounds_DPhi;
    InputHistBinBounds m_histBounds_Eta;
    InputHistBinBounds m_histBounds_EnergyPtMET;
    InputHistBinBounds m_histBounds_EnergyPtMET_SR;
    InputHistBinBounds m_histBounds_RootEnergyPtMET;
    InputHistBinBounds m_histBounds_KT2;
    InputHistBinBounds m_histBounds_PxPy;
    InputHistBinBounds m_histBounds_RootPxPy;
    InputHistBinBounds m_histBounds_PxPy_SR;
    
    InputHistBinBounds m_histBounds_Mass;
    InputHistBinBounds m_histBounds_ZMass;
    
    InputHistBinBounds m_histBounds_NJets;
    InputHistBinBounds m_histBounds_NVtx;
    InputHistBinBounds m_histBounds_NVtx_SR;
    void DefaultVarVals() {
        SAB.DefaultVarVals();
        double const dPI(TMath::Pi());

        int PhiBinN    = 50;
        float PhiBinLB = -dPI;
        float PhiBinUB = dPI;

        m_histBounds_Phi.SetVals(PhiBinN, PhiBinLB, PhiBinUB);
        
        int DPhiBinN    = 50;
        float DPhiBinLB = 0.0;
        float DPhiBinUB = dPI;
        
        m_histBounds_DPhi.SetVals(DPhiBinN, DPhiBinLB, DPhiBinUB);
        
        int EtaBinN    = 40;
        float EtaBinLB = -5.;
        float EtaBinUB =  5.;
        m_histBounds_Eta.SetVals(EtaBinN, EtaBinLB, EtaBinUB);

//        int EnergyPtMassBinN    = 40;
        int EnergyPtMassBinN    = 80;
//        int MassBinN            = 100;
        int MassBinN            = 200;
        float EnergyPtMassBinLB = 0;
//        float EnergyPtMassBinUB = 400;
        float EnergyPtMassBinUB = 800;
        
        m_histBounds_EnergyPtMET.SetVals(EnergyPtMassBinN, EnergyPtMassBinLB, EnergyPtMassBinUB);
        m_histBounds_Mass.SetVals(MassBinN, EnergyPtMassBinLB, EnergyPtMassBinUB);
        
        int EnergyPtMassBinN_SR    = 40;
        float EnergyPtMassBinLB_SR = 0;
        float EnergyPtMassBinUB_SR = 200;
        
        m_histBounds_EnergyPtMET_SR.SetVals(EnergyPtMassBinN_SR, EnergyPtMassBinLB_SR, EnergyPtMassBinUB_SR);
        
        int RootEnergyPtMassBinN    = 40;
        float RootEnergyPtMassBinLB = 0;
        float RootEnergyPtMassBinUB = TMath::Sqrt(400);
        m_histBounds_RootEnergyPtMET.SetVals(RootEnergyPtMassBinN, RootEnergyPtMassBinLB, RootEnergyPtMassBinUB);
        
        int ZMassBinN    = 60;
        float ZMassBinLB = 60;
        float ZMassBinUB = 120;
        m_histBounds_ZMass.SetVals(ZMassBinN, ZMassBinLB, ZMassBinUB);
        
        int METXYBinN       = 50;
        float METXYBinLB    = -200;
        float METXYBinUB    = 200;
        m_histBounds_PxPy.SetVals(METXYBinN, METXYBinLB, METXYBinUB);
        
        int METXYBinN_SR       = 16;
        float METXYBinLB_SR    = -200;
        float METXYBinUB_SR   = 200;
        m_histBounds_PxPy_SR.SetVals(METXYBinN_SR, METXYBinLB_SR, METXYBinUB_SR);
        
        int RootMETXYBinN       = 50;
        float RootMETXYBinLB    = -TMath::Sqrt(400);
        float RootMETXYBinUB    = TMath::Sqrt(400);
        m_histBounds_RootPxPy.SetVals(RootMETXYBinN, RootMETXYBinLB, RootMETXYBinUB);
        
        int NJetsBinN    = 11;
        float NJetsBinLB    = -0.5;
        float NJetsBinUB    = 10.5;
        m_histBounds_NJets.SetVals(NJetsBinN, NJetsBinLB, NJetsBinUB);
        
        int nVtxBinN     = 60;        
        float nVtxBinLB     = 0.5;
        float nVtxBinUB     = 60.5;
        m_histBounds_NVtx.SetVals(nVtxBinN, nVtxBinLB, nVtxBinUB);
        
        int nVtxBinN_SR     = 35;        
        float nVtxBinLB_SR     = 0.5;
        float nVtxBinUB_SR     = 35.5;
        m_histBounds_NVtx_SR.SetVals(nVtxBinN_SR, nVtxBinLB_SR, nVtxBinUB_SR);
        
        int KT2BinN = 40;
        float KT2BinLB = -200;
        float KT2BinUB = 200;
        m_histBounds_KT2.SetVals(KT2BinN, KT2BinLB, KT2BinUB);
    }
};

#endif // STOP_HIST_BIN_BOUNDS_H
