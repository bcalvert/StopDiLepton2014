// Structs for plotting/booking of histograms that are specific to the Stop analysis
using namespace std;
typedef struct StopHistBinBounds {
    // Contains the general set of InputHistBinBounds that the stop analysis uses for various
    // types of histograms -- most names should be fairly self-explanatory
    
    SpecialAxisBounds SAB;
    
    InputHistBinBounds IHBB_Phi;
    InputHistBinBounds IHBB_DPhi;
    InputHistBinBounds IHBB_Eta;
    InputHistBinBounds IHBB_EnergyPtMET;
    InputHistBinBounds IHBB_EnergyPtMET_SR;
    InputHistBinBounds IHBB_KT2;
    InputHistBinBounds IHBB_PxPy;
    InputHistBinBounds IHBB_PxPy_SR;
    
    InputHistBinBounds IHBB_Mass;
    InputHistBinBounds IHBB_ZMass;
    
    InputHistBinBounds IHBB_NJets;
    InputHistBinBounds IHBB_NVtx;
    InputHistBinBounds IHBB_NVtx_SR;
    void DefaultVarVals() {
        SAB.DefaultVarVals();
        /*
        cout << "about to print vals " << endl;
        SAB.PrintVals();
        cout << "just printed vals " << endl;
        */
        
        const float PI = 3.14159265;

        int PhiBinN    = 50;
        float PhiBinLB = -PI;
        float PhiBinUB = PI;

        IHBB_Phi.SetVals(PhiBinN, PhiBinLB, PhiBinUB);
        
        int DPhiBinN    = 50;
        float DPhiBinLB = 0.0;
        float DPhiBinUB = PI;
        
        IHBB_DPhi.SetVals(DPhiBinN, DPhiBinLB, DPhiBinUB);
        
        int EtaBinN    = 40;
        float EtaBinLB = -5.;
        float EtaBinUB =  5.;
        IHBB_Eta.SetVals(EtaBinN, EtaBinLB, EtaBinUB);
            
        int EnergyPtMassBinN    = 40;
        int MassBinN            = 100;
        float EnergyPtMassBinLB = 0;
        float EnergyPtMassBinUB = 400;
        
        IHBB_EnergyPtMET.SetVals(EnergyPtMassBinN, EnergyPtMassBinLB, EnergyPtMassBinUB);
        IHBB_Mass.SetVals(MassBinN, EnergyPtMassBinLB, EnergyPtMassBinUB);
        
        int EnergyPtMassBinN_SR    = 40;
        float EnergyPtMassBinLB_SR = 0;
        float EnergyPtMassBinUB_SR = 200;
        
        IHBB_EnergyPtMET_SR.SetVals(EnergyPtMassBinN_SR, EnergyPtMassBinLB_SR, EnergyPtMassBinUB_SR);
        
        int ZMassBinN    = 60;
        float ZMassBinLB = 60;
        float ZMassBinUB = 120;
        IHBB_ZMass.SetVals(ZMassBinN, ZMassBinLB, ZMassBinUB);
        
        int METXYBinN       = 50;
        float METXYBinLB    = -200;
        float METXYBinUB    = 200;
        IHBB_PxPy.SetVals(METXYBinN, METXYBinLB, METXYBinUB);
        
        int METXYBinN_SR       = 16;
        float METXYBinLB_SR    = -200;
        float METXYBinUB_SR   = 200;
        IHBB_PxPy_SR.SetVals(METXYBinN_SR, METXYBinLB_SR, METXYBinUB_SR);
        
        int NJetsBinN    = 11;
        float NJetsBinLB    = -0.5;
        float NJetsBinUB    = 10.5;
        IHBB_NJets.SetVals(NJetsBinN, NJetsBinLB, NJetsBinUB);
        
        int nVtxBinN     = 60;        
        float nVtxBinLB     = 0.5;
        float nVtxBinUB     = 60.5;
        IHBB_NVtx.SetVals(nVtxBinN, nVtxBinLB, nVtxBinUB);
        
        int nVtxBinN_SR     = 35;        
        float nVtxBinLB_SR     = 0.5;
        float nVtxBinUB_SR     = 35.5;
        IHBB_NVtx_SR.SetVals(nVtxBinN_SR, nVtxBinLB_SR, nVtxBinUB_SR);
        
        int KT2BinN = 40;
        float KT2BinLB = -100;
        float KT2BinUB = 300;
        IHBB_KT2.SetVals(KT2BinN, KT2BinLB, KT2BinUB);
    }
} StopHistBinBounds;