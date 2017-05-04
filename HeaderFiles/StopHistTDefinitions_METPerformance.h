void TwoDeeHistTVec_AddMET_vs_qTHists(vector<HistogramT> * inHistTVec,  StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear, TString addString = "") {
    // Histograms used as part of calculating MET Phi corrections
    
    bool doSyst = true;
    bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringUPerp = "UPerp";
    TString stringUPar  = "UPar";
    TString stringqT    = "qT";
    TString stringSumET = "SumEt";
    
    TString strPlus = "Plus";
    TString strDiv = "Div";
    TString strSub = "Sub";
    
    TString strVS = "_vs_";
    
    HistogramT H_UPerp_vs_qT; H_UPerp_vs_qT.DefaultVarVals();
    H_UPerp_vs_qT.SetName(stringSmear + stringUPerp, strVS, stringqT + addString);
    H_UPerp_vs_qT.xAxis.SetAxisSpecial("patsy", stringqT, 17, doSyst);
    H_UPerp_vs_qT.SetIndAxisLabel(stringqT, mapVartoLabel, 1);
    H_UPerp_vs_qT.yAxis.SetAxisSpecial("patsy", stringUPerp, 18, doSyst);
    H_UPerp_vs_qT.SetIndAxisLabel(stringSmear + stringUPerp, mapVartoLabel, 2);
    
    HistogramT H_UParPlusqT_vs_qT; H_UParPlusqT_vs_qT.DefaultVarVals();
    H_UParPlusqT_vs_qT.SetName(stringSmear + stringUPar + strPlus + stringqT, strVS, stringqT + addString);
    H_UParPlusqT_vs_qT.xAxis.SetAxisSpecial("patsy", stringqT, 17, doSyst);
    H_UParPlusqT_vs_qT.SetIndAxisLabel(stringqT, mapVartoLabel, 1);
    H_UParPlusqT_vs_qT.yAxis.SetAxisSpecial("patsy", stringUPar + strPlus + stringqT, 18, doSyst);
    H_UParPlusqT_vs_qT.SetIndAxisLabel(stringSmear + stringUPar + strPlus + stringqT, mapVartoLabel, 2);
    
    HistogramT H_UParDivqT_vs_qT; H_UParDivqT_vs_qT.DefaultVarVals();
    H_UParDivqT_vs_qT.SetName(stringSmear + stringUPar + strDiv + stringqT, strVS, stringqT + addString);
    H_UParDivqT_vs_qT.xAxis.SetAxisSpecial("patsy", stringqT, 17, doSyst);
    H_UParDivqT_vs_qT.SetIndAxisLabel(stringqT, mapVartoLabel, 1);
    H_UParDivqT_vs_qT.yAxis.SetAxisSpecial("patsy", stringUPar + strDiv + stringqT, 19, doSyst);
    H_UParDivqT_vs_qT.SetIndAxisLabel(stringSmear + stringUPar + strDiv + stringqT, mapVartoLabel, 2);

    inHistTVec->push_back(H_UPerp_vs_qT);
    inHistTVec->push_back(H_UParPlusqT_vs_qT);
    inHistTVec->push_back(H_UParDivqT_vs_qT);
}

void TwoDeeHistTVec_AddMET_vs_nVtxHists(vector<HistogramT> * inHistTVec,  StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear, TString addString = "") {
    // Histograms used as part of calculating MET Phi corrections
    
    bool doSyst = true;
    //    bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMET = "MET";
    TString stringUPerp = "UPerp";
    TString stringUPar  = "UPar";
    TString stringqT    = "qT";
    TString stringSumET = "SumEt";
    TString stringnVtx  = "nVtx";
    
    TString strPlus = "Plus";
    TString strDiv = "Div";
    TString strSub = "Sub";
    
    TString strVS = "_vs_";
    
    HistogramT H_UPerp_vs_nVtx; H_UPerp_vs_nVtx.DefaultVarVals();
    H_UPerp_vs_nVtx.SetName(stringSmear + stringUPerp, strVS, stringnVtx + addString);
    H_UPerp_vs_nVtx.xAxis.SetAxisSpecial("patsy", stringnVtx, 20, doSyst);
    H_UPerp_vs_nVtx.SetIndAxisLabel(stringqT, mapVartoLabel, 1);
    H_UPerp_vs_nVtx.yAxis.SetAxisSpecial("patsy", stringUPerp, 18, doSyst);
    H_UPerp_vs_nVtx.SetIndAxisLabel(stringSmear + stringUPerp, mapVartoLabel, 2);
    
    HistogramT H_UParPlusqT_vs_nVtx; H_UParPlusqT_vs_nVtx.DefaultVarVals();
    H_UParPlusqT_vs_nVtx.SetName(stringSmear + stringUPar + strPlus + stringqT, strVS, stringnVtx + addString);
    H_UParPlusqT_vs_nVtx.xAxis.SetAxisSpecial("patsy", stringnVtx, 20, doSyst);
    H_UParPlusqT_vs_nVtx.SetIndAxisLabel(stringqT, mapVartoLabel, 1);
    H_UParPlusqT_vs_nVtx.yAxis.SetAxisSpecial("patsy", stringUPar + strPlus + stringqT, 18, doSyst);
    H_UParPlusqT_vs_nVtx.SetIndAxisLabel(stringSmear + stringUPar + strPlus + stringqT, mapVartoLabel, 2);
    
    inHistTVec->push_back(H_UPerp_vs_nVtx);
    inHistTVec->push_back(H_UParPlusqT_vs_nVtx);
}



void TwoDeeHistTVec_METPerformance(vector<HistogramT> * inHistTVec,  StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, TString addString = "") {
    bool doSmear = true;
    TwoDeeHistTVec_AddMET_vs_qTHists(inHistTVec, inSHBB, mapVartoLabel, doSmear, addString);
    TwoDeeHistTVec_AddMET_vs_nVtxHists(inHistTVec, inSHBB, mapVartoLabel, doSmear, addString);
}

void TwoDeeHistTVec_METPerformance_NoSmear(vector<HistogramT> * inHistTVec,  StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, TString addString = "") {
    bool doSmear = false;
    TwoDeeHistTVec_AddMET_vs_qTHists(inHistTVec, inSHBB, mapVartoLabel, doSmear, addString);
    TwoDeeHistTVec_AddMET_vs_nVtxHists(inHistTVec, inSHBB, mapVartoLabel, doSmear, addString);
}
