// Set of functions that set up the Two-Dimensional histograms that will be automatically booked and filled by the plot-making macro

inline void TwoDeeHistTVec_AddMETPhiCorrHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    // Histograms used as part of calculating MET Phi corrections
    
    bool doSyst = true;
    bool noSyst = false;
    
    TString stringnVtx = "nVtx";
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMET = "MET";

    HistogramT H_METX_vs_nVtx; H_METX_vs_nVtx.DefaultVarVals();
    H_METX_vs_nVtx.SetName(stringSmear + stringMET + "X", "_vs_", stringnVtx);
    H_METX_vs_nVtx.xAxis.SetAxis("patsy", stringnVtx, &inSHBB->IHBB_NVtx_SR, doSyst);
    H_METX_vs_nVtx.SetIndAxisLabel(stringnVtx, mapVartoLabel, 1);
    H_METX_vs_nVtx.yAxis.SetAxis("patsy", stringMET + "X", &inSHBB->IHBB_PxPy, doSyst);
    H_METX_vs_nVtx.SetIndAxisLabel(stringSmear + stringMET + "X", mapVartoLabel, 2);
    
    HistogramT H_METY_vs_nVtx; H_METY_vs_nVtx.DefaultVarVals();
    H_METY_vs_nVtx.SetName(stringSmear + stringMET + "Y", "_vs_", stringnVtx);
    H_METY_vs_nVtx.xAxis.SetAxis("patsy", stringnVtx, &inSHBB->IHBB_NVtx_SR, doSyst);
    H_METY_vs_nVtx.SetIndAxisLabel(stringnVtx, mapVartoLabel, 1);
    H_METY_vs_nVtx.yAxis.SetAxis("patsy", stringMET + "Y", &inSHBB->IHBB_PxPy, doSyst);
    H_METY_vs_nVtx.SetIndAxisLabel(stringSmear + stringMET + "Y", mapVartoLabel, 2);
    
    inHistTVec->push_back(H_METX_vs_nVtx);
    inHistTVec->push_back(H_METY_vs_nVtx);
}

inline void TwoDeeHistTVec_AddMT2CorrelationHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    // Histograms used for checking correlations of the various MT2s with other variables
    
    bool doSyst = true;
    bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMET = "MET";    
    TString stringMT2ll = "MT2ll";
    TString stringMT2lb = "MT2lb";
    
    HistogramT H_MET_vs_MT2ll; H_MET_vs_MT2ll.DefaultVarVals();
    H_MET_vs_MT2ll.SetName(stringSmear + stringMET, "_vs_", stringMT2ll);
    H_MET_vs_MT2ll.xAxis.SetAxis("patsy", stringMET, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MET_vs_MT2ll.SetIndAxisLabel(stringSmear + stringMET, mapVartoLabel, 1);
    H_MET_vs_MT2ll.yAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MET_vs_MT2ll.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 2);
            
    
    HistogramT H_MT2ll_vs_MT2lb; H_MT2ll_vs_MT2lb.DefaultVarVals();
    H_MT2ll_vs_MT2lb.SetName(stringSmear + stringMT2ll, "_vs_", stringMT2lb);
    H_MT2ll_vs_MT2lb.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_MT2ll_vs_MT2lb.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_vs_MT2lb.yAxis.SetAxis("patsy", stringMT2lb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2ll_vs_MT2lb.SetIndAxisLabel(stringSmear + stringMT2lb, mapVartoLabel, 2);
            
    inHistTVec->push_back(H_MET_vs_MT2ll);
    inHistTVec->push_back(H_MT2ll_vs_MT2lb);
    /*
    HistogramT H_DPhiBLep0BLep1; H_DPhiBLep0BLep1.DefaultVarVals();
    H_DPhiBLep0BLep1.SetName(stringSmear + DPhiVarString(intBLep, intBLep), "", "");
    H_DPhiBLep0BLep1.xAxis.SetAxis("patsy", DPhiVarString(intBLep, intBLep), &inSHBB->IHBB_DPhi, doSyst);
    H_DPhiBLep0BLep1.SetIndAxisLabel(DPhiVarString(intBLep, intBLep), mapVartoLabel, 1);
    H_DPhiBLep0BLep1.yAxis.SetAxisAsDefaultCountAxis("radians");
     
     
     HistogramT H_MT2ll_vs_DeltaPhiLep0Lep1; H_MT2ll_vs_DeltaPhiLep0Lep1.name = "h_MT2ll_vs_DeltaPhiLep0Lep1";
     H_MT2ll_vs_DeltaPhiLep0Lep1.xLabel = "MT2_{ll} [GeV]"; H_MT2ll_vs_DeltaPhiLep0Lep1.xBinN = METBinN; H_MT2ll_vs_DeltaPhiLep0Lep1.xMin = METBinLB; H_MT2ll_vs_DeltaPhiLep0Lep1.xMax = METBinUB;
     H_MT2ll_vs_DeltaPhiLep0Lep1.yLabel = "#Delta #phi_{ll}"; H_MT2ll_vs_DeltaPhiLep0Lep1.yBinN = PhiBinN; H_MT2ll_vs_DeltaPhiLep0Lep1.yMin = 0; H_MT2ll_vs_DeltaPhiLep0Lep1.yMax = PI;
     H_MT2ll_vs_DeltaPhiLep0Lep1.xVarKey = "MT2ll";
     H_MT2ll_vs_DeltaPhiLep0Lep1.yVarKey = "DPhiLep0Lep1";
     H_MT2ll_vs_DeltaPhiLep0Lep1.doXSyst = true;
     H_MT2ll_vs_DeltaPhiLep0Lep1.doYSyst = true;
     
     HistogramT H_MT2lb_vs_DeltaPhiLepB0LepB1; H_MT2lb_vs_DeltaPhiLepB0LepB1.name = "h_MT2lb_vs_DeltaPhiLepB0LepB1";
     H_MT2lb_vs_DeltaPhiLepB0LepB1.xLabel = "MT2lb [GeV]"; H_MT2lb_vs_DeltaPhiLepB0LepB1.xBinN = METBinN; H_MT2lb_vs_DeltaPhiLepB0LepB1.xMin = METBinLB; H_MT2lb_vs_DeltaPhiLepB0LepB1.xMax = METBinUB;
     H_MT2lb_vs_DeltaPhiLepB0LepB1.yLabel = "#Delta #phi_{lb lb}"; H_MT2lb_vs_DeltaPhiLepB0LepB1.yBinN = PhiBinN; H_MT2lb_vs_DeltaPhiLepB0LepB1.yMin = 0; H_MT2lb_vs_DeltaPhiLepB0LepB1.yMax = PI;
     H_MT2lb_vs_DeltaPhiLepB0LepB1.xVarKey = "MT2lb";
     H_MT2lb_vs_DeltaPhiLepB0LepB1.yVarKey = "DPhiLepB0LepB1";
     H_MT2lb_vs_DeltaPhiLepB0LepB1.doXSyst = true;
     H_MT2lb_vs_DeltaPhiLepB0LepB1.doYSyst = true;
     
     */
}


inline void TwoDeeHistTVec_Inclusive(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSignal) {
    bool noSmear = false;
    if (isSignal) return;
    TwoDeeHistTVec_AddMT2CorrelationHists(inHistTVec, inSHBB, mapVartoLabel, noSmear);
    if (!isSignal) {
        TwoDeeHistTVec_AddMETPhiCorrHists(inHistTVec, inSHBB, mapVartoLabel, noSmear);
    }
}
inline void TwoDeeHistTVec_Inclusive_Smeared(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSignal) {
    bool doSmear = true;
    TwoDeeHistTVec_AddMT2CorrelationHists(inHistTVec, inSHBB, mapVartoLabel, doSmear);
    if (!isSignal) {
        TwoDeeHistTVec_AddMETPhiCorrHists(inHistTVec, inSHBB, mapVartoLabel, doSmear);
    }
}

