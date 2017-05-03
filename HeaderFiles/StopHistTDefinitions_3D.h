// Set of functions that set up the Three-Dimensional histograms that will be automatically booked and filled by the plot-making macro

inline void ThreeDeeHistTVec_AddMT2CorrelationHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    // Histograms used for checking correlations of the various MT2s with other variables
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMET = "MET";
    TString stringMT2ll = "MT2ll";
    TString stringMT2lblb = "MT2lblb";
    TString stringMT2bb_ZMET = "MT2bb_ZMET";
    TString stringKT2_Top = "KT2_Top";
    TString stringShape = "SignalShape";
    
    HistogramT H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET; H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.DefaultVarVals();
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.SetName(stringSmear + stringMT2ll, "_vs_", stringMT2lblb + "_vs_" + stringMT2bb_ZMET);
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.xAxis.SetAxisSpecial("patsy", stringMT2ll, 10, doSyst);
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.yAxis.SetAxisSpecial("patsy", stringMT2lblb, 12, doSyst);
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 2);
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.zAxis.SetAxisSpecial("patsy", stringMT2bb_ZMET, 14, doSyst);
    H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET.SetIndAxisLabel(stringSmear + stringMT2bb_ZMET, mapVartoLabel, 3);
    
    HistogramT H_MT2ll_vs_MT2lblb_vs_KT2_Top; H_MT2ll_vs_MT2lblb_vs_KT2_Top.DefaultVarVals();
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.SetName(stringSmear + stringMT2ll, "_vs_", stringMT2lblb + "_vs_" + stringKT2_Top);
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.xAxis.SetAxisSpecial("patsy", stringMT2ll, 10, doSyst);
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.yAxis.SetAxisSpecial("patsy", stringMT2lblb, 12, doSyst);
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 2);
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.zAxis.SetAxisSpecial("patsy", stringKT2_Top, 16, doSyst);
    H_MT2ll_vs_MT2lblb_vs_KT2_Top.SetIndAxisLabel(stringSmear + stringKT2_Top, mapVartoLabel, 3);
    
    inHistTVec->push_back(H_MT2ll_vs_MT2lblb_vs_MT2bb_ZMET);
    //inHistTVec->push_back(H_MT2ll_vs_MT2lblb_vs_KT2_Top);
}



inline void ThreeDeeHistTVec_Inclusive(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int cutPlotLevel) {
    bool noSmear = false;
    if (cutPlotLevel > 0) return;
    ThreeDeeHistTVec_AddMT2CorrelationHists(inHistTVec, inSHBB, mapVartoLabel, noSmear);
}
inline void ThreeDeeHistTVec_Inclusive_Smeared(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int cutPlotLevel) {
    bool doSmear = true;
    if (cutPlotLevel > 0 && cutPlotLevel < 2) return;
    ThreeDeeHistTVec_AddMT2CorrelationHists(inHistTVec, inSHBB, mapVartoLabel, doSmear);
}







/*
inline vector<HistogramT> * ThreeDeeHistTVec() {

    
    ///////analogous case for 2D histograms///////
    
    HistogramT H_MT2ll_vs_DeltaPhiZMET_vs_nVtx; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.name = "h_MT2ll_vs_DeltaPhiZMET_vs_nVtx"; 
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.xLabel = "MT2_{ll} [GeV]"; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.xBinN = METBinN; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.xMin = METBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.xMax = METBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.yLabel = "#Delta #phi_{Z, #slash{E}_{T}}"; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.yBinN = PhiBinN; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.yMin = 0.; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.yMax = PI;
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.zLabel = "N_{vtx}^{reco}"; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.zBinN = nVtxBinN; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.zMin = nVtxBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.zMax = nVtxBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.xVarKey = "MT2ll";
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.yVarKey = "DPhiZMET";
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.zVarKey = "nVtx";
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.doXSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_nVtx.doYSyst = true;
    
    HistogramT H_MT2ll_vs_DeltaPhiZMET_vs_NJets; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.name = "h_MT2ll_vs_DeltaPhiZMET_vs_NJets"; 
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.xLabel = "MT2_{ll} [GeV]"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.xBinN = METBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.xMin = METBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.xMax = METBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.yLabel = "#Delta #phi_{Z, #slash{E}_{T}}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.yBinN = PhiBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.yMin = 0.; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.yMax = PI;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.zLabel = "N_{jets}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.zBinN = NJetsBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.zMin = NJetsBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets.zMax = NJetsBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.xVarKey = "MT2ll";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.yVarKey = "DPhiZMET";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.zVarKey = "NJets";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.doXSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.doYSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets.doZSyst = true;
    
    HistogramT H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.name = "h_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10"; 
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.xLabel = "MT2_{ll} [GeV]"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.xBinN = METBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.xMin = METBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.xMax = METBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.yLabel = "#Delta #phi_{Z, #slash{E}_{T}}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.yBinN = PhiBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.yMin = 0.; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.yMax = PI;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.zLabel = "N_{jets}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.zBinN = NJetsBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.zMin = NJetsBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.zMax = NJetsBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.xVarKey = "MT2ll";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.yVarKey = "DPhiZMET";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.zVarKey = "NJets";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.doXSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.doYSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx1to10.doZSyst = true;
    
    HistogramT H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.name = "h_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20"; 
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.xLabel = "MT2_{ll} [GeV]"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.xBinN = METBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.xMin = METBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.xMax = METBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.yLabel = "#Delta #phi_{Z, #slash{E}_{T}}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.yBinN = PhiBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.yMin = 0.; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.yMax = PI;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.zLabel = "N_{jets}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.zBinN = NJetsBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.zMin = NJetsBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.zMax = NJetsBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.xVarKey = "MT2ll";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.yVarKey = "DPhiZMET";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.zVarKey = "NJets";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.doXSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.doYSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx11to20.doZSyst = true;
    
    HistogramT H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.name = "h_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30"; 
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.xLabel = "MT2_{ll} [GeV]"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.xBinN = METBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.xMin = METBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.xMax = METBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.yLabel = "#Delta #phi_{Z, #slash{E}_{T}}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.yBinN = PhiBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.yMin = 0.; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.yMax = PI;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.zLabel = "N_{jets}"; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.zBinN = NJetsBinN; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.zMin = NJetsBinLB; H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.zMax = NJetsBinUB;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.xVarKey = "MT2ll";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.yVarKey = "DPhiZMET";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.zVarKey = "NJets";
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.doXSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.doYSyst = true;
    H_MT2ll_vs_DeltaPhiZMET_vs_NJets_nVtx21to30.doZSyst = true;

    return histVec_3D;
}
*/