// Set of functions that set up the One-Dimensional histograms that will be automatically booked and filled by the plot-making macro


inline void OneDeeHistTVec_AddMiscHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel) {
    //Miscellaneous histograms -- weights, cut flow, pileup, etc.
    
    bool doSyst = true;
    bool noSyst = false;
    
    TString stringnVtx = "nVtx";
    
    HistogramT H_BasicWeightIntegral; H_BasicWeightIntegral.DefaultVarVals();
    H_BasicWeightIntegral.name = "h_BasicWeightIntegral";
    H_BasicWeightIntegral.xAxis.SetAxis("patsy", "BasicWeight", 1, 0.5, 1.5, doSyst);
    
    HistogramT H_CutFlow; H_CutFlow.DefaultVarVals();
    H_CutFlow.name = "h_ChannelCutFlow";
    H_CutFlow.xAxis.SetAxis("patsy", "CutFlowEntry", 4, 0.5, 4.5, noSyst);
    H_CutFlow.SetIndAxisLabel("CS", mapVartoLabel, 1);
    H_CutFlow.yAxis.axisLabel = "Number of Events in each cut stage";
    
    HistogramT H_nVtx; H_nVtx.DefaultVarVals();
    H_nVtx.SetName(stringnVtx, "", "");
    H_nVtx.xAxis.SetAxis("patsy", stringnVtx, &inSHBB->IHBB_NVtx, doSyst);
    H_nVtx.SetIndAxisLabel(stringnVtx, mapVartoLabel, 1);
    H_nVtx.yAxis.SetAxisAsDefaultCountAxis("N_{vtx}^{reco}");
    
    HistogramT H_nVtx_preRW; H_nVtx_preRW.DefaultVarVals();
    H_nVtx_preRW.SetName(stringnVtx, "_preRW", "");
    H_nVtx_preRW.xAxis.SetAxis("patsy", stringnVtx, &inSHBB->IHBB_NVtx, doSyst);
    H_nVtx_preRW.SetIndAxisLabel(stringnVtx, mapVartoLabel, 1);
    H_nVtx_preRW.yAxis.SetAxisAsDefaultCountAxis("N_{vtx}^{reco}");
    
    inHistTVec->push_back(H_BasicWeightIntegral);
    inHistTVec->push_back(H_CutFlow);
    inHistTVec->push_back(H_nVtx);
    inHistTVec->push_back(H_nVtx_preRW);
}

inline void OneDeeHistTVec_AddMiscLeptonHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int whichPos) {
    // Relatively unimportant lepton histograms, i.e. stuff we don't check that often
    //bool doSyst = true;
    bool noSyst = false;
    
    int iLep = 0;
    TString lepLeadString = GetStringLead(whichPos, false);
    lepLeadString += GetStringIndObj(iLep, false);
    
    HistogramT H_RelLepPFIso; H_RelLepPFIso.DefaultVarVals();
    H_RelLepPFIso.SetName(lepLeadString, "RelPFIso", "");
    H_RelLepPFIso.xAxis.SetAxis("patsy", lepLeadString + TString("Iso"), 32, 0., 0.16, noSyst);
    H_RelLepPFIso.SetIndAxisLabel(lepLeadString + TString("Iso"), mapVartoLabel, 1);
    H_RelLepPFIso.yAxis.SetAxisAsDefaultCountAxis("Rel Lep Iso");
    
    inHistTVec->push_back(H_RelLepPFIso);
}

inline void OneDeeHistTVec_AddIndKinObjectHists_Check(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int typeObj, int whichPos, bool isSmear) {
    // Kinematic checks for individual visible objects -- leptons, jets, b-jets -- checks being geometry checks
    
    //bool doSyst = true;
    bool noSyst = false;
    
    TString stringLead = GetStringLead(whichPos, false);
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringObj = GetStringIndObj(typeObj, false);
    
    TString objectVarKeyName = stringLead + stringObj;
    TString objectName = stringSmear + stringLead + stringObj;
    
    HistogramT H_ObjEta; H_ObjEta.DefaultVarVals();
    H_ObjEta.SetName(objectName, "Eta", "");
    H_ObjEta.xAxis.SetAxis("patsy", objectVarKeyName + TString("Eta"), &inSHBB->IHBB_Eta, noSyst);
    H_ObjEta.SetIndAxisLabel(objectName + TString("Eta"), mapVartoLabel, 1);
    H_ObjEta.yAxis.SetAxisAsDefaultCountAxis("#eta");
    
    inHistTVec->push_back(H_ObjEta);
}

inline void OneDeeHistTVec_AddDiKinObjectHists_Check(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int typeObj, bool isSmear) {
    // Kinematic checks for di-structure visible objects -- di-lepton system, dijet system, di-b-jet system -- checks being geometry checks
    //bool doSyst = true;
    bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringObj = GetStringIndObj(typeObj, false);
    
    TString objectVarKeyName = "Di" + stringObj;
    TString objectName = stringSmear + "Di" + stringObj;
    
    HistogramT H_ObjEta; H_ObjEta.DefaultVarVals();
    H_ObjEta.SetName(objectName, "Eta", "");
    H_ObjEta.xAxis.SetAxis("patsy", objectVarKeyName + TString("Eta"), &inSHBB->IHBB_Eta, noSyst);
    H_ObjEta.SetIndAxisLabel(objectName + TString("Eta"), mapVartoLabel, 1);
    H_ObjEta.yAxis.SetAxisAsDefaultCountAxis("#eta");
    
    HistogramT H_ObjPhi; H_ObjPhi.DefaultVarVals();
    H_ObjPhi.SetName(objectName, "Phi", "");
    H_ObjPhi.xAxis.SetAxis("patsy", objectVarKeyName + TString("Phi"), &inSHBB->IHBB_Phi, noSyst);
    H_ObjPhi.SetIndAxisLabel(objectName + TString("Phi"), mapVartoLabel, 1);
    H_ObjPhi.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    inHistTVec->push_back(H_ObjEta);
    inHistTVec->push_back(H_ObjPhi);
}

inline void OneDeeHistTVec_AddIndKinObjectHists_Main(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int typeObj, int isLead, bool isSmear) {
    // Main Kinematic distributions for individual visible objects -- leptons, jets, b-jets
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringLead = GetStringLead(isLead, false);
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringObj = GetStringIndObj(typeObj, false);
    
    TString objectName = stringSmear + stringLead + stringObj;
    TString objectMaxName = stringSmear + "Max" + stringObj;
    TString objectVarKeyName = stringLead + stringObj;
    TString objectMaxVarKeyName = "Max" + stringObj;
    
    HistogramT H_ObjPt; H_ObjPt.DefaultVarVals();
    H_ObjPt.SetName(objectName, "Pt", "");
    H_ObjPt.xAxis.SetAxis("patsy", objectVarKeyName + "Pt", &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_ObjPt.SetIndAxisLabel(objectName + "Pt", mapVartoLabel, 1);
    H_ObjPt.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_ObjEn; H_ObjEn.DefaultVarVals();
    H_ObjEn.SetName(objectName, "En", "");
    H_ObjEn.xAxis.SetAxis("patsy", objectVarKeyName + "En", &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_ObjEn.SetIndAxisLabel(objectName + "En", mapVartoLabel, 1);
    H_ObjEn.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_ObjMass; H_ObjMass.DefaultVarVals();
    H_ObjMass.SetName(objectName, "Mass", "");
    H_ObjMass.xAxis.SetAxis("patsy", objectVarKeyName + "InvMass", &inSHBB->IHBB_Mass, doSyst);
    H_ObjMass.SetIndAxisLabel(objectName + "InvMass", mapVartoLabel, 1);
    H_ObjMass.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_MaxObjMass; H_MaxObjMass.DefaultVarVals();
    H_MaxObjMass.SetName(objectMaxName, "Mass", "");
    H_MaxObjMass.xAxis.SetAxis("patsy", objectMaxVarKeyName + "InvMass", &inSHBB->IHBB_Mass, doSyst);
    H_MaxObjMass.SetIndAxisLabel(objectMaxName + "InvMass", mapVartoLabel, 1);
    H_MaxObjMass.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    
    inHistTVec->push_back(H_ObjPt);
    inHistTVec->push_back(H_ObjEn);
    if (typeObj == 3) {
        inHistTVec->push_back(H_ObjMass);
        if (isLead == 0) {
            inHistTVec->push_back(H_MaxObjMass);
        }
    }
}


inline void OneDeeHistTVec_AddDiKinObjectHists_Main(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int typeObj, bool isSmear, bool includeZMassHist = false) {
    
    // Main kinematic distributions for di-structure visible objects -- di-lepton system, dijet system, di-b-jet system
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringObj = GetStringIndObj(typeObj, false);
    
    
    TString objectName = stringSmear + "Di" + stringObj;
    TString objectVarKeyName = "Di" + stringObj;
    
    HistogramT H_ObjPt; H_ObjPt.DefaultVarVals();
    H_ObjPt.SetName(objectName, "Pt", "");
    H_ObjPt.xAxis.SetAxis("patsy", objectVarKeyName + "Pt", &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_ObjPt.SetIndAxisLabel(objectName + "Pt", mapVartoLabel, 1);
    H_ObjPt.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_ObjPtCoarseBin; H_ObjPtCoarseBin.DefaultVarVals();
    H_ObjPtCoarseBin.SetName(objectName, "Pt", "Coarse");
    H_ObjPtCoarseBin.xAxis.SetAxisSpecial("patsy", objectVarKeyName + "Pt", 8, doSyst);
    H_ObjPtCoarseBin.SetIndAxisLabel(objectName + "Pt", mapVartoLabel, 1);
    H_ObjPtCoarseBin.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_ObjMass; H_ObjMass.DefaultVarVals();
    H_ObjMass.SetName(objectName, "Mass", "");
    H_ObjMass.xAxis.SetAxis("patsy", objectVarKeyName + "InvMass", &inSHBB->IHBB_Mass, doSyst);
    H_ObjMass.SetIndAxisLabel(objectName + "InvMass", mapVartoLabel, 1);
    H_ObjMass.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    inHistTVec->push_back(H_ObjPt);
    inHistTVec->push_back(H_ObjPtCoarseBin);
    inHistTVec->push_back(H_ObjMass);
    
    if (includeZMassHist) {
        HistogramT H_ObjZMass; H_ObjZMass.DefaultVarVals();
        H_ObjZMass.SetName(objectName, "MassTight", "");
        H_ObjZMass.xAxis.SetAxis("patsy", objectVarKeyName + "InvMass", &inSHBB->IHBB_ZMass, doSyst);
        H_ObjZMass.SetIndAxisLabel(objectName + "InvMass", mapVartoLabel, 1);
        H_ObjZMass.yAxis.SetAxisAsDefaultCountAxis("GeV");
        inHistTVec->push_back(H_ObjZMass);
    }
}



inline void OneDeeHistTVec_AddDeltaPhi(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    
    // Histograms that contain relative deltaPhi between different visible (or not so "visible" *cough* MET) objects
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    
    
    const int numSavedLeps = 2;
    /*
     const int numSavedJets = 2;
     const int numSavedBJets = 2;
     */
    
    int intLep  = 0;
    int intJet  = 1;
    int intBJet = 2;
    int intBLep = 3;
    int intZ    = 4;
    int intMET  = 5;
    
    HistogramT H_DPhiLep0Lep1; H_DPhiLep0Lep1.DefaultVarVals();
    H_DPhiLep0Lep1.SetName(DPhiVarString(intLep, intLep), "", "");
    H_DPhiLep0Lep1.xAxis.SetAxis("patsy", DPhiVarString(intLep, intLep), &inSHBB->IHBB_DPhi, doSyst);
    H_DPhiLep0Lep1.SetIndAxisLabel(DPhiVarString(intLep, intLep), mapVartoLabel, 1);
    H_DPhiLep0Lep1.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    HistogramT H_DPhiZMET; H_DPhiZMET.DefaultVarVals();
    H_DPhiZMET.SetName(stringSmear + DPhiVarString(intZ, intMET), "", "");
    H_DPhiZMET.xAxis.SetAxis("patsy", DPhiVarString(intZ, intMET), &inSHBB->IHBB_DPhi, doSyst);
    H_DPhiZMET.SetIndAxisLabel(DPhiVarString(intZ, intMET), mapVartoLabel, 1);
    H_DPhiZMET.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    HistogramT H_DPhiJet0Jet1; H_DPhiJet0Jet1.DefaultVarVals();
    H_DPhiJet0Jet1.SetName(stringSmear + DPhiVarString(intJet, intJet), "", "");
    H_DPhiJet0Jet1.xAxis.SetAxis("patsy", DPhiVarString(intJet, intJet), &inSHBB->IHBB_DPhi, doSyst);
    H_DPhiJet0Jet1.SetIndAxisLabel(DPhiVarString(intJet, intJet), mapVartoLabel, 1);
    H_DPhiJet0Jet1.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    HistogramT H_DPhiBJet0BJet1; H_DPhiBJet0BJet1.DefaultVarVals();
    H_DPhiBJet0BJet1.SetName(stringSmear + DPhiVarString(intBJet, intBJet), "", "");
    H_DPhiBJet0BJet1.xAxis.SetAxis("patsy", DPhiVarString(intBJet, intBJet), &inSHBB->IHBB_DPhi, doSyst);
    H_DPhiBJet0BJet1.SetIndAxisLabel(DPhiVarString(intBJet, intBJet), mapVartoLabel, 1);
    H_DPhiBJet0BJet1.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    HistogramT H_DPhiBLep0BLep1; H_DPhiBLep0BLep1.DefaultVarVals();
    H_DPhiBLep0BLep1.SetName(stringSmear + DPhiVarString(intBLep, intBLep), "", "");
    H_DPhiBLep0BLep1.xAxis.SetAxis("patsy", DPhiVarString(intBLep, intBLep), &inSHBB->IHBB_DPhi, doSyst);
    H_DPhiBLep0BLep1.SetIndAxisLabel(DPhiVarString(intBLep, intBLep), mapVartoLabel, 1);
    H_DPhiBLep0BLep1.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    HistogramT H_DPhiIndLepMET[numSavedLeps];
    for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
        H_DPhiIndLepMET[iLep].DefaultVarVals();
        H_DPhiIndLepMET[iLep].SetName(stringSmear + DPhiVarString(intLep, intMET, iLep, -1), "", "");
        H_DPhiIndLepMET[iLep].xAxis.SetAxis("patsy", DPhiVarString(intLep, intMET, iLep, -1), &inSHBB->IHBB_DPhi, doSyst);
        H_DPhiIndLepMET[iLep].SetIndAxisLabel(DPhiVarString(intLep, intMET, iLep, -1), mapVartoLabel, 1);
        H_DPhiIndLepMET[iLep].yAxis.SetAxisAsDefaultCountAxis("radians");
    }
    if (!isSmear) {
        inHistTVec->push_back(H_DPhiLep0Lep1);
    }
    inHistTVec->push_back(H_DPhiZMET);
    inHistTVec->push_back(H_DPhiBLep0BLep1);
    for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
        inHistTVec->push_back(H_DPhiIndLepMET[iLep]);
    }
}





inline void OneDeeHistTVec_AddMT2Hists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear, int cutPlotLevel) {
    // Histograms for MT2ll and MT2lblb
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMT2ll = "MT2ll";
    TString stringMT2lblb = "MT2lblb";
    TString stringMT2lblb_ZeroBLepMass = "MT2lblb_ZeroBLepMass";
    TString stringMT2bb_ZMET = "MT2bb_ZMET";
    TString stringKT2_Top = "KT2_Top";
    
    TString stringShape = "SignalShape";
    TString stringShapeOBC = "SignalShapeOBC";
    
    HistogramT H_MT2ll; H_MT2ll.DefaultVarVals();
    H_MT2ll.SetName(stringSmear, stringMT2ll, "");
    H_MT2ll.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2ll.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    const int numCutVals = 5;
    TString cutVals[numCutVals] = {"80", "90", "100", "110", "120"};
    HistogramT H_MT2llPassCut[numCutVals];
    for (int iMT2Cut = 0; iMT2Cut < numCutVals; ++iMT2Cut) {
        H_MT2llPassCut[iMT2Cut].DefaultVarVals();
        H_MT2llPassCut[iMT2Cut].SetName(stringSmear, stringMT2ll + cutVals[iMT2Cut], "");
        H_MT2llPassCut[iMT2Cut].xAxis.SetAxis("patsy", stringMT2ll + cutVals[iMT2Cut], 2, -0.5, 1.5, doSyst);
        H_MT2llPassCut[iMT2Cut].SetIndAxisLabel(stringSmear + stringMT2ll + cutVals[iMT2Cut], mapVartoLabel, 1);
        H_MT2llPassCut[iMT2Cut].yAxis.axisLabel = "Events Passing/Failing M_{T2}(ll) Cut";
    }
    
    HistogramT H_MT2llZoom; H_MT2llZoom.DefaultVarVals();
    H_MT2llZoom.SetName(stringSmear, stringMT2ll + "Zoom", "");
    H_MT2llZoom.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_MT2llZoom.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2llZoom.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_MT2lblb; H_MT2lblb.DefaultVarVals();
    H_MT2lblb.SetName(stringSmear, stringMT2lblb, "");
    H_MT2lblb.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_MT2lblb_ZeroBLepMass; H_MT2lblb_ZeroBLepMass.DefaultVarVals();
    H_MT2lblb_ZeroBLepMass.SetName(stringSmear, stringMT2lblb_ZeroBLepMass, "");
    H_MT2lblb_ZeroBLepMass.xAxis.SetAxis("patsy", stringMT2lblb_ZeroBLepMass, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_ZeroBLepMass.SetIndAxisLabel(stringSmear + stringMT2lblb_ZeroBLepMass, mapVartoLabel, 1);
    H_MT2lblb_ZeroBLepMass.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    
    HistogramT H_MT2llShape; H_MT2llShape.DefaultVarVals();
    H_MT2llShape.SetName(stringSmear, stringMT2ll, stringShape);
    H_MT2llShape.xAxis.SetAxisSpecial("patsy", stringMT2ll, 1, doSyst);
    H_MT2llShape.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2llShape.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    
    HistogramT H_MT2llShapeOBC; H_MT2llShapeOBC.DefaultVarVals();
    H_MT2llShapeOBC.SetName(stringSmear, stringMT2ll, stringShapeOBC);
    H_MT2llShapeOBC.xAxis.SetAxisSpecial("patsy", stringMT2ll, 2, doSyst);
    H_MT2llShapeOBC.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2llShapeOBC.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    
    HistogramT H_MT2lblbShape; H_MT2lblbShape.DefaultVarVals();
    H_MT2lblbShape.SetName(stringSmear, stringMT2lblb, stringShape);
    H_MT2lblbShape.xAxis.SetAxisSpecial("patsy", stringMT2lblb, 3, doSyst);
    H_MT2lblbShape.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblbShape.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    HistogramT H_MT2lblbShapeOBC; H_MT2lblbShapeOBC.DefaultVarVals();
    H_MT2lblbShapeOBC.SetName(stringSmear, stringMT2lblb, stringShapeOBC);
    H_MT2lblbShapeOBC.xAxis.SetAxisSpecial("patsy", stringMT2lblb, 4, doSyst);
    H_MT2lblbShapeOBC.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblbShapeOBC.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    HistogramT H_MT2bb_ZMET; H_MT2bb_ZMET.DefaultVarVals();
    H_MT2bb_ZMET.SetName(stringSmear, stringMT2bb_ZMET, "");
    H_MT2bb_ZMET.xAxis.SetAxis("patsy", stringMT2bb_ZMET, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2bb_ZMET.SetIndAxisLabel(stringSmear + stringMT2bb_ZMET, mapVartoLabel, 1);
    H_MT2bb_ZMET.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_MT2bb_ZMETShape; H_MT2bb_ZMETShape.DefaultVarVals();
    H_MT2bb_ZMETShape.SetName(stringSmear, stringMT2bb_ZMET, stringShape);
    H_MT2bb_ZMETShape.xAxis.SetAxisSpecial("patsy", stringMT2bb_ZMET, 5, doSyst);
    H_MT2bb_ZMETShape.SetIndAxisLabel(stringSmear + stringMT2bb_ZMET, mapVartoLabel, 1);
    H_MT2bb_ZMETShape.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    HistogramT H_MT2bb_ZMETShapeOBC; H_MT2bb_ZMETShapeOBC.DefaultVarVals();
    H_MT2bb_ZMETShapeOBC.SetName(stringSmear, stringMT2bb_ZMET, stringShapeOBC);
    H_MT2bb_ZMETShapeOBC.xAxis.SetAxisSpecial("patsy", stringMT2bb_ZMET, 6, doSyst);
    H_MT2bb_ZMETShapeOBC.SetIndAxisLabel(stringSmear + stringMT2bb_ZMET, mapVartoLabel, 1);
    H_MT2bb_ZMETShapeOBC.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    
    HistogramT H_KT2_Top; H_KT2_Top.DefaultVarVals();
    H_KT2_Top.SetName(stringSmear, stringKT2_Top, "");
    H_KT2_Top.xAxis.SetAxis("patsy", stringKT2_Top, &inSHBB->IHBB_KT2, doSyst);
    H_KT2_Top.SetIndAxisLabel(stringSmear + stringKT2_Top, mapVartoLabel, 1);
    H_KT2_Top.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    
    HistogramT H_KT2_TopShape; H_KT2_TopShape.DefaultVarVals();
    H_KT2_TopShape.SetName(stringSmear, stringKT2_Top, stringShape);
    H_KT2_TopShape.xAxis.SetAxisSpecial("patsy", stringKT2_Top, 7, doSyst);
    H_KT2_TopShape.SetIndAxisLabel(stringSmear + stringKT2_Top, mapVartoLabel, 1);
    H_KT2_TopShape.yAxis.SetAxisAsDefaultCountAxis("GeV", "");
    
    if (cutPlotLevel != 2) {
        inHistTVec->push_back(H_MT2ll);
        inHistTVec->push_back(H_MT2llZoom);
        for (int iMT2Cut = 0; iMT2Cut < numCutVals; ++iMT2Cut) {
            inHistTVec->push_back(H_MT2llPassCut[iMT2Cut]);
        }
        inHistTVec->push_back(H_MT2lblb);
        inHistTVec->push_back(H_MT2lblb_ZeroBLepMass);
        inHistTVec->push_back(H_MT2bb_ZMET);
        inHistTVec->push_back(H_KT2_Top);
        //inHistTVec->push_back(H_MT2llShape);
        //inHistTVec->push_back(H_MT2lblbShape);
        //inHistTVec->push_back(H_MT2lblbShapeOBC);
        
        //inHistTVec->push_back(H_MT2bb_ZMETShape);
        //inHistTVec->push_back(H_MT2bb_ZMETShapeOBC);
        
        //inHistTVec->push_back(H_KT2_TopShape);
    }
    inHistTVec->push_back(H_MT2llShapeOBC);
}


inline void OneDeeHistTVec_AddMT2Hists_SpecialCuts(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    // Histograms for MT2ll and MT2lblb with special cuts put in
    
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMT2ll = "MT2ll";
    TString stringMT2lblb = "MT2lblb";
    
    const float PI = 3.14159265;
    SampCut SC_DPhi_0_1Div3PI;
    SampCut SC_DPhi_1Div3PI_2Div3PI;
    SampCut SC_DPhi_2Div3PI_PI;
    SC_DPhi_0_1Div3PI.SetParamsWindow("", 0.0, 1./3 * PI, 2);
    SC_DPhi_1Div3PI_2Div3PI.SetParamsWindow("", 1./3 * PI, 2./3 * PI, 2);
    SC_DPhi_2Div3PI_PI.SetParamsWindow("", 2./3 * PI, PI, 2);
    TString baseDPhiString = "DPhi";
    TString stringDPhiZMETBaseString = baseDPhiString + "ZMET";
    TString stringDPhiLep0Lep1BaseString = baseDPhiString + "Lep0Lep1";
    TString stringDPhiBLep0BLep1BaseString = baseDPhiString + "BLep0BLep1";
    TString stringDPhiJet0Jet1BaseString = baseDPhiString + "Jet0Jet1";
    
    TString stringClose = "Close";
    TString stringMid = "Mid";
    TString stringFar = "Far";
    
    HistogramT H_MT2ll_DPhiZMETClose; H_MT2ll_DPhiZMETClose.DefaultVarVals();
    H_MT2ll_DPhiZMETClose.SetName(stringSmear, stringMT2ll, stringDPhiZMETBaseString + stringClose);
    H_MT2ll_DPhiZMETClose.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_MT2ll_DPhiZMETClose.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_DPhiZMETClose.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2ll_DPhiZMETClose.specialHistCutFloat.SetParams(&SC_DPhi_0_1Div3PI);
    H_MT2ll_DPhiZMETClose.specialHistCutFloatVar = stringDPhiZMETBaseString;
    
    HistogramT H_MT2ll_DPhiZMETMid; H_MT2ll_DPhiZMETMid.DefaultVarVals();
    H_MT2ll_DPhiZMETMid.SetName(stringSmear, stringMT2ll, stringDPhiZMETBaseString + stringMid);
    H_MT2ll_DPhiZMETMid.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_MT2ll_DPhiZMETMid.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_DPhiZMETMid.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2ll_DPhiZMETMid.specialHistCutFloat.SetParams(&SC_DPhi_1Div3PI_2Div3PI);
    H_MT2ll_DPhiZMETMid.specialHistCutFloatVar = stringDPhiZMETBaseString;
    
    HistogramT H_MT2ll_DPhiZMETFar; H_MT2ll_DPhiZMETFar.DefaultVarVals();
    H_MT2ll_DPhiZMETFar.SetName(stringSmear, stringMT2ll, stringDPhiZMETBaseString + stringFar);
    H_MT2ll_DPhiZMETFar.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_MT2ll_DPhiZMETFar.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_DPhiZMETFar.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2ll_DPhiZMETFar.specialHistCutFloat.SetParams(&SC_DPhi_2Div3PI_PI);
    H_MT2ll_DPhiZMETFar.specialHistCutFloatVar = stringDPhiZMETBaseString;
    
    
    HistogramT H_MT2ll_DPhiLep0Lep1Close; H_MT2ll_DPhiLep0Lep1Close.DefaultVarVals();
    H_MT2ll_DPhiLep0Lep1Close.SetName(stringSmear, stringMT2ll, stringDPhiLep0Lep1BaseString + stringClose);
    H_MT2ll_DPhiLep0Lep1Close.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2ll_DPhiLep0Lep1Close.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_DPhiLep0Lep1Close.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2ll_DPhiLep0Lep1Close.specialHistCutFloat.SetParams(&SC_DPhi_0_1Div3PI);
    H_MT2ll_DPhiLep0Lep1Close.specialHistCutFloatVar = stringDPhiLep0Lep1BaseString;
    
    HistogramT H_MT2ll_DPhiLep0Lep1Mid; H_MT2ll_DPhiLep0Lep1Mid.DefaultVarVals();
    H_MT2ll_DPhiLep0Lep1Mid.SetName(stringSmear, stringMT2ll, stringDPhiLep0Lep1BaseString + stringMid);
    H_MT2ll_DPhiLep0Lep1Mid.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2ll_DPhiLep0Lep1Mid.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_DPhiLep0Lep1Mid.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2ll_DPhiLep0Lep1Mid.specialHistCutFloat.SetParams(&SC_DPhi_1Div3PI_2Div3PI);
    H_MT2ll_DPhiLep0Lep1Mid.specialHistCutFloatVar = stringDPhiLep0Lep1BaseString;
    
    HistogramT H_MT2ll_DPhiLep0Lep1Far; H_MT2ll_DPhiLep0Lep1Far.DefaultVarVals();
    H_MT2ll_DPhiLep0Lep1Far.SetName(stringSmear, stringMT2ll, stringDPhiLep0Lep1BaseString + stringFar);
    H_MT2ll_DPhiLep0Lep1Far.xAxis.SetAxis("patsy", stringMT2ll, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2ll_DPhiLep0Lep1Far.SetIndAxisLabel(stringSmear + stringMT2ll, mapVartoLabel, 1);
    H_MT2ll_DPhiLep0Lep1Far.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2ll_DPhiLep0Lep1Far.specialHistCutFloat.SetParams(&SC_DPhi_2Div3PI_PI);
    H_MT2ll_DPhiLep0Lep1Far.specialHistCutFloatVar = stringDPhiLep0Lep1BaseString;
    
    
    int intBLep = 3;
    
    HistogramT H_MT2lblb_DPhiBLep0BLep1Close; H_MT2lblb_DPhiBLep0BLep1Close.DefaultVarVals();
    H_MT2lblb_DPhiBLep0BLep1Close.SetName(stringSmear, stringMT2lblb, stringDPhiBLep0BLep1BaseString + stringClose);
    H_MT2lblb_DPhiBLep0BLep1Close.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_DPhiBLep0BLep1Close.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_DPhiBLep0BLep1Close.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_DPhiBLep0BLep1Close.specialHistCutFloat.SetParams(&SC_DPhi_0_1Div3PI);
    H_MT2lblb_DPhiBLep0BLep1Close.specialHistCutFloatVar = DPhiVarString(intBLep, intBLep);
    
    HistogramT H_MT2lblb_DPhiBLep0BLep1Mid; H_MT2lblb_DPhiBLep0BLep1Mid.DefaultVarVals();
    H_MT2lblb_DPhiBLep0BLep1Mid.SetName(stringSmear, stringMT2lblb, stringDPhiBLep0BLep1BaseString + stringMid);
    H_MT2lblb_DPhiBLep0BLep1Mid.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_DPhiBLep0BLep1Mid.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_DPhiBLep0BLep1Mid.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_DPhiBLep0BLep1Mid.specialHistCutFloat.SetParams(&SC_DPhi_1Div3PI_2Div3PI);
    H_MT2lblb_DPhiBLep0BLep1Mid.specialHistCutFloatVar = DPhiVarString(intBLep, intBLep);
    
    HistogramT H_MT2lblb_DPhiBLep0BLep1Far; H_MT2lblb_DPhiBLep0BLep1Far.DefaultVarVals();
    H_MT2lblb_DPhiBLep0BLep1Far.SetName(stringSmear, stringMT2lblb, stringDPhiBLep0BLep1BaseString + stringFar);
    H_MT2lblb_DPhiBLep0BLep1Far.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_DPhiBLep0BLep1Far.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_DPhiBLep0BLep1Far.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_DPhiBLep0BLep1Far.specialHistCutFloat.SetParams(&SC_DPhi_2Div3PI_PI);
    H_MT2lblb_DPhiBLep0BLep1Far.specialHistCutFloatVar = DPhiVarString(intBLep, intBLep);
    
    /*
     HistogramT H_MT2lblb_DPhiJet0Jet1Close; H_MT2lblb_DPhiJet0Jet1Close.DefaultVarVals();
     H_MT2lblb_DPhiJet0Jet1Close.SetName(stringSmear, stringMT2lblb, stringDPhiJet0Jet1BaseString + stringClose);
     H_MT2lblb_DPhiJet0Jet1Close.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
     H_MT2lblb_DPhiJet0Jet1Close.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
     H_MT2lblb_DPhiJet0Jet1Close.yAxis.SetAxisAsDefaultCountAxis("GeV");
     H_MT2lblb_DPhiJet0Jet1Close.specialHistCutFloat.SetParams(&SC_DPhi_0_1Div3PI);
     H_MT2lblb_DPhiJet0Jet1Close.specialHistCutFloatVar = stringDPhiJet0Jet1BaseString;
     
     HistogramT H_MT2lblb_DPhiJet0Jet1Mid; H_MT2lblb_DPhiJet0Jet1Mid.DefaultVarVals();
     H_MT2lblb_DPhiJet0Jet1Mid.SetName(stringSmear, stringMT2lblb, stringDPhiJet0Jet1BaseString + stringMid);
     H_MT2lblb_DPhiJet0Jet1Mid.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
     H_MT2lblb_DPhiJet0Jet1Mid.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
     H_MT2lblb_DPhiJet0Jet1Mid.yAxis.SetAxisAsDefaultCountAxis("GeV");
     H_MT2lblb_DPhiJet0Jet1Mid.specialHistCutFloat.SetParams(&SC_DPhi_1Div3PI_2Div3PI);
     H_MT2lblb_DPhiJet0Jet1Mid.specialHistCutFloatVar = stringDPhiJet0Jet1BaseString;
     
     HistogramT H_MT2lblb_DPhiJet0Jet1Far; H_MT2lblb_DPhiJet0Jet1Far.DefaultVarVals();
     H_MT2lblb_DPhiJet0Jet1Far.SetName(stringSmear, stringMT2lblb, stringDPhiJet0Jet1BaseString + stringFar);
     H_MT2lblb_DPhiJet0Jet1Far.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
     H_MT2lblb_DPhiJet0Jet1Far.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
     H_MT2lblb_DPhiJet0Jet1Far.yAxis.SetAxisAsDefaultCountAxis("GeV");
     H_MT2lblb_DPhiJet0Jet1Far.specialHistCutFloat.SetParams(&SC_DPhi_1Div3PI_2Div3PI);
     H_MT2lblb_DPhiJet0Jet1Far.specialHistCutFloatVar = stringDPhiJet0Jet1BaseString;
     */
    
    
    SampCut SC_MT2lblb_BothB, SC_MT2lblb_LeadB, SC_MT2lblb_SubB, SC_MT2lblb_NoB;
    SC_MT2lblb_BothB.SetParamsWindow("", -0.1, 0.1, 2);
    SC_MT2lblb_LeadB.SetParamsWindow("", 0.9, 1.1, 2);
    SC_MT2lblb_SubB.SetParamsWindow("", 1.9, 2.1, 2);
    SC_MT2lblb_NoB.SetParamsWindow("", 2.9, 3.1, 2);
    
    TString baseMT2lblbToTString = "typeMT2lblb";
    
    TString stringBothB = "BothB";
    TString stringLeadB = "LeadB";
    TString stringSubB = "SubB";
    TString stringNoB = "NoB";
    
    HistogramT H_MT2lblb_BothJetB; H_MT2lblb_BothJetB.DefaultVarVals();
    H_MT2lblb_BothJetB.SetName(stringSmear, stringMT2lblb, baseMT2lblbToTString + stringBothB);
    H_MT2lblb_BothJetB.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_BothJetB.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_BothJetB.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_BothJetB.specialHistCutFloat.SetParams(&SC_MT2lblb_BothB);
    H_MT2lblb_BothJetB.specialHistCutFloatVar = baseMT2lblbToTString;
    
    HistogramT H_MT2lblb_LeadJetB; H_MT2lblb_LeadJetB.DefaultVarVals();
    H_MT2lblb_LeadJetB.SetName(stringSmear, stringMT2lblb, baseMT2lblbToTString + stringLeadB);
    H_MT2lblb_LeadJetB.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_LeadJetB.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_LeadJetB.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_LeadJetB.specialHistCutFloat.SetParams(&SC_MT2lblb_LeadB);
    H_MT2lblb_LeadJetB.specialHistCutFloatVar = baseMT2lblbToTString;
    
    HistogramT H_MT2lblb_SubJetB; H_MT2lblb_SubJetB.DefaultVarVals();
    H_MT2lblb_SubJetB.SetName(stringSmear, stringMT2lblb, baseMT2lblbToTString + stringSubB);
    H_MT2lblb_SubJetB.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_SubJetB.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_SubJetB.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_SubJetB.specialHistCutFloat.SetParams(&SC_MT2lblb_SubB);
    H_MT2lblb_SubJetB.specialHistCutFloatVar = baseMT2lblbToTString;
    
    HistogramT H_MT2lblb_NoJetB; H_MT2lblb_NoJetB.DefaultVarVals();
    H_MT2lblb_NoJetB.SetName(stringSmear, stringMT2lblb, baseMT2lblbToTString + stringNoB);
    H_MT2lblb_NoJetB.xAxis.SetAxis("patsy", stringMT2lblb, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MT2lblb_NoJetB.SetIndAxisLabel(stringSmear + stringMT2lblb, mapVartoLabel, 1);
    H_MT2lblb_NoJetB.yAxis.SetAxisAsDefaultCountAxis("GeV");
    H_MT2lblb_NoJetB.specialHistCutFloat.SetParams(&SC_MT2lblb_NoB);
    H_MT2lblb_NoJetB.specialHistCutFloatVar = baseMT2lblbToTString;
    
    
    inHistTVec->push_back(H_MT2ll_DPhiZMETClose);
    inHistTVec->push_back(H_MT2ll_DPhiZMETMid);
    inHistTVec->push_back(H_MT2ll_DPhiZMETFar);
    
    inHistTVec->push_back(H_MT2lblb_DPhiBLep0BLep1Close);
    inHistTVec->push_back(H_MT2lblb_DPhiBLep0BLep1Mid);
    inHistTVec->push_back(H_MT2lblb_DPhiBLep0BLep1Far);
    
    inHistTVec->push_back(H_MT2lblb_BothJetB);
    inHistTVec->push_back(H_MT2lblb_LeadJetB);
    inHistTVec->push_back(H_MT2lblb_SubJetB);
    inHistTVec->push_back(H_MT2lblb_NoJetB);
    
}


inline void OneDeeHistTVec_AddBasicMETSigHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    // histograms for MET Significance
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMETSigRaw = "METSigRaw";
    SampCut SC_MT2llGeq80;
    SC_MT2llGeq80.SetParamsBound("", 80, 1);
    TString stringMT2ll = "MT2ll";
    
    HistogramT H_METSigRaw; H_METSigRaw.DefaultVarVals();
    H_METSigRaw.SetName(stringSmear, stringMETSigRaw, "");
    H_METSigRaw.xAxis.SetAxis("patsy", stringMETSigRaw, 40, 0., 200., doSyst);
    H_METSigRaw.SetIndAxisLabel(stringSmear + stringMETSigRaw, mapVartoLabel, 1);
    H_METSigRaw.yAxis.SetAxisAsDefaultCountAxis("Sig");
    
    HistogramT H_METSigRaw_MT2llGeq80; H_METSigRaw_MT2llGeq80.DefaultVarVals();
    H_METSigRaw_MT2llGeq80.SetName(stringSmear, stringMETSigRaw + "_", stringMT2ll + "Geq80");
    H_METSigRaw_MT2llGeq80.xAxis.SetAxis("patsy", stringMETSigRaw, 40, 0., 200., doSyst);
    H_METSigRaw_MT2llGeq80.SetIndAxisLabel(stringSmear + stringMETSigRaw, mapVartoLabel, 1);
    H_METSigRaw_MT2llGeq80.yAxis.SetAxisAsDefaultCountAxis("Sig");
    H_METSigRaw_MT2llGeq80.specialHistCutFloat.SetParams(&SC_MT2llGeq80);
    H_METSigRaw_MT2llGeq80.specialHistCutFloatVar = stringMT2ll;
    
    inHistTVec->push_back(H_METSigRaw);
    inHistTVec->push_back(H_METSigRaw_MT2llGeq80);
}


inline void OneDeeHistTVec_AddBasicMETHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    // Basic histograms for MET
    
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMET = "MET";
    TString stringMETSig1DHack = "METSig1DHack";
    TString stringMETSig1DHack_AllJets = "METSig1DHack_AllJets";
    TString stringMETSig2DHack = "METSig2DHack";
    TString stringMETSig2DHack_AllJets = "METSig2DHack_AllJets";
    TString stringMETSig2DHack_Par = "METSig2DHack_Par";
    TString stringMETSig2DHack_Perp = "METSig2DHack_Perp";
    TString stringMETSig2DHack_Par_AllJets = "METSig2DHack_Par_AllJets";
    TString stringMETSig2DHack_Perp_AllJets = "METSig2DHack_Perp_AllJets";
    
    TString stringMETSig2DHackFull = "METSig2DHackFull";
    TString stringMETSig2DHack_ParFull = "METSig2DHack_ParFull";
    TString stringMETSig2DHack_PerpFull = "METSig2DHack_PerpFull";
    
    TString stringMETSig2DTrue = "METSig2DTrue";
    
    HistogramT H_MET; H_MET.DefaultVarVals();
    H_MET.SetName(stringSmear, stringMET, "");
    H_MET.xAxis.SetAxis("patsy", stringMET, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_MET.SetIndAxisLabel(stringSmear + stringMET, mapVartoLabel, 1);
    H_MET.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_METTightBin; H_METTightBin.DefaultVarVals();
    H_METTightBin.SetName(stringSmear, stringMET, "TightBin");
    H_METTightBin.xAxis.SetAxis("patsy", stringMET, 8, 0., 400., doSyst);
    H_METTightBin.SetIndAxisLabel(stringSmear + stringMET, mapVartoLabel, 1);
    H_METTightBin.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_METPhi; H_METPhi.DefaultVarVals();
    H_METPhi.SetName(stringSmear, stringMET + "Phi", "");
    H_METPhi.xAxis.SetAxis("patsy", stringMET + "Phi", &inSHBB->IHBB_Phi, doSyst);
    H_METPhi.SetIndAxisLabel(stringSmear + stringMET + "Phi", mapVartoLabel, 1);
    H_METPhi.yAxis.SetAxisAsDefaultCountAxis("radians");
    
    HistogramT H_METSig1DHack; H_METSig1DHack.DefaultVarVals();
    H_METSig1DHack.SetName(stringSmear, stringMETSig1DHack, "");
    H_METSig1DHack.xAxis.SetAxis("patsy", stringMETSig1DHack, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_METSig1DHack.SetIndAxisLabel(stringSmear + stringMETSig1DHack, mapVartoLabel, 1);
    H_METSig1DHack.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig1DHack_AllJets; H_METSig1DHack_AllJets.DefaultVarVals();
    H_METSig1DHack_AllJets.SetName(stringSmear, stringMETSig1DHack_AllJets, "");
    H_METSig1DHack_AllJets.xAxis.SetAxis("patsy", stringMETSig1DHack_AllJets, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_METSig1DHack_AllJets.SetIndAxisLabel(stringSmear + stringMETSig1DHack_AllJets, mapVartoLabel, 1);
    H_METSig1DHack_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack; H_METSig2DHack.DefaultVarVals();
    H_METSig2DHack.SetName(stringSmear, stringMETSig2DHack, "");
    H_METSig2DHack.xAxis.SetAxis("patsy", stringMETSig2DHack, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_METSig2DHack.SetIndAxisLabel(stringSmear + stringMETSig2DHack, mapVartoLabel, 1);
    H_METSig2DHack.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_AllJets; H_METSig2DHack_AllJets.DefaultVarVals();
    H_METSig2DHack_AllJets.SetName(stringSmear, stringMETSig2DHack_AllJets, "");
    H_METSig2DHack_AllJets.xAxis.SetAxis("patsy", stringMETSig2DHack_AllJets, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_METSig2DHack_AllJets.SetIndAxisLabel(stringSmear + stringMETSig2DHack_AllJets, mapVartoLabel, 1);
    H_METSig2DHack_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DTrue; H_METSig2DTrue.DefaultVarVals();
    H_METSig2DTrue.SetName(stringSmear, stringMETSig2DTrue, "");
    H_METSig2DTrue.xAxis.SetAxis("patsy", stringMETSig2DTrue, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_METSig2DTrue.SetIndAxisLabel(stringSmear + stringMETSig2DTrue, mapVartoLabel, 1);
    H_METSig2DTrue.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHackFull; H_METSig2DHackFull.DefaultVarVals();
    H_METSig2DHackFull.SetName(stringSmear, stringMETSig2DHackFull, "");
    H_METSig2DHackFull.xAxis.SetAxis("patsy", stringMETSig2DHackFull, &inSHBB->IHBB_EnergyPtMET_SR, doSyst);
    H_METSig2DHackFull.SetIndAxisLabel(stringSmear + stringMETSig2DHackFull, mapVartoLabel, 1);
    H_METSig2DHackFull.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Par; H_METSig2DHack_Par.DefaultVarVals();
    H_METSig2DHack_Par.SetName(stringSmear, stringMETSig2DHack_Par, "");
    H_METSig2DHack_Par.xAxis.SetAxis("patsy", stringMETSig2DHack_Par, &inSHBB->IHBB_RootPxPy, doSyst);
    H_METSig2DHack_Par.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Par, mapVartoLabel, 1);
    H_METSig2DHack_Par.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Perp; H_METSig2DHack_Perp.DefaultVarVals();
    H_METSig2DHack_Perp.SetName(stringSmear, stringMETSig2DHack_Perp, "");
    H_METSig2DHack_Perp.xAxis.SetAxis("patsy", stringMETSig2DHack_Perp, &inSHBB->IHBB_RootPxPy, doSyst);
    H_METSig2DHack_Perp.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Perp, mapVartoLabel, 1);
    H_METSig2DHack_Perp.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Par_AllJets; H_METSig2DHack_Par_AllJets.DefaultVarVals();
    H_METSig2DHack_Par_AllJets.SetName(stringSmear, stringMETSig2DHack_Par_AllJets, "");
    H_METSig2DHack_Par_AllJets.xAxis.SetAxis("patsy", stringMETSig2DHack_Par_AllJets, &inSHBB->IHBB_RootPxPy, doSyst);
    H_METSig2DHack_Par_AllJets.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Par_AllJets, mapVartoLabel, 1);
    H_METSig2DHack_Par_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Perp_AllJets; H_METSig2DHack_Perp_AllJets.DefaultVarVals();
    H_METSig2DHack_Perp_AllJets.SetName(stringSmear, stringMETSig2DHack_Perp_AllJets, "");
    H_METSig2DHack_Perp_AllJets.xAxis.SetAxis("patsy", stringMETSig2DHack_Perp_AllJets, &inSHBB->IHBB_RootPxPy, doSyst);
    H_METSig2DHack_Perp_AllJets.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Perp_AllJets, mapVartoLabel, 1);
    H_METSig2DHack_Perp_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_ParFull; H_METSig2DHack_ParFull.DefaultVarVals();
    H_METSig2DHack_ParFull.SetName(stringSmear, stringMETSig2DHack_ParFull, "");
    H_METSig2DHack_ParFull.xAxis.SetAxis("patsy", stringMETSig2DHack_ParFull, &inSHBB->IHBB_RootPxPy, doSyst);
    H_METSig2DHack_ParFull.SetIndAxisLabel(stringSmear + stringMETSig2DHack_ParFull, mapVartoLabel, 1);
    H_METSig2DHack_ParFull.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_PerpFull; H_METSig2DHack_PerpFull.DefaultVarVals();
    H_METSig2DHack_PerpFull.SetName(stringSmear, stringMETSig2DHack_PerpFull, "");
    H_METSig2DHack_PerpFull.xAxis.SetAxis("patsy", stringMETSig2DHack_PerpFull, &inSHBB->IHBB_RootPxPy, doSyst);
    H_METSig2DHack_PerpFull.SetIndAxisLabel(stringSmear + stringMETSig2DHack_PerpFull, mapVartoLabel, 1);
    H_METSig2DHack_PerpFull.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METX; H_METX.DefaultVarVals();
    H_METX.SetName(stringSmear, stringMET + "X", "");
    H_METX.xAxis.SetAxis("patsy", stringMET + "X", &inSHBB->IHBB_PxPy, doSyst);
    H_METX.SetIndAxisLabel(stringSmear + stringMET + "X", mapVartoLabel, 1);
    H_METX.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_METY; H_METY.DefaultVarVals();
    H_METY.SetName(stringSmear, stringMET + "Y", "");
    H_METY.xAxis.SetAxis("patsy", stringMET + "Y", &inSHBB->IHBB_PxPy, doSyst);
    H_METY.SetIndAxisLabel(stringSmear + stringMET + "Y", mapVartoLabel, 1);
    H_METY.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_TTBarPt; H_TTBarPt.DefaultVarVals();
    H_TTBarPt.SetName(stringSmear, "TTBarPt", "");
    H_TTBarPt.xAxis.SetAxis("patsy", "TTBarPt", &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_TTBarPt.SetIndAxisLabel(stringSmear + "TTBarPt", mapVartoLabel, 1);
    H_TTBarPt.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    inHistTVec->push_back(H_MET);
    //inHistTVec->push_back(H_METTightBin);
    inHistTVec->push_back(H_METPhi);
    inHistTVec->push_back(H_METX);
    inHistTVec->push_back(H_METY);
    
    inHistTVec->push_back(H_METSig1DHack);
    inHistTVec->push_back(H_METSig1DHack_AllJets);
    inHistTVec->push_back(H_METSig2DHack);
    inHistTVec->push_back(H_METSig2DHack_AllJets);
    inHistTVec->push_back(H_METSig2DHack_Par);
    inHistTVec->push_back(H_METSig2DHack_Par_AllJets);
    inHistTVec->push_back(H_METSig2DHack_Perp);
    inHistTVec->push_back(H_METSig2DHack_Perp_AllJets);
    inHistTVec->push_back(H_METSig2DHackFull);
    inHistTVec->push_back(H_METSig2DHack_ParFull);
    inHistTVec->push_back(H_METSig2DHack_PerpFull);
    inHistTVec->push_back(H_METSig2DTrue);
    
    inHistTVec->push_back(H_TTBarPt);
}

inline void OneDeeHistTVec_AddMETPerformanceHists_SpecialCuts(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    
    // MET Performance variables -- UPar/UPerp, but with special cuts/requirements
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringUPar = "UPar";
    TString stringUParPlusqT = "UParPlusqT";
    TString stringUPerp = "UPerp";
    TString stringMT2ll = "MT2ll";
    TString stringCut;
    
    const int numCutVals = 5;
    SampCut SC_MT2llGeqCut[numCutVals];
    int cutVal[numCutVals] = {80, 90, 100, 110, 120};
    
    HistogramT H_UParPlusqT_MT2GeqCut[5];
    HistogramT H_UPerp_MT2GeqCut[5];
    for (int iCut = 0; iCut < numCutVals; ++iCut) {
        SC_MT2llGeqCut[iCut].SetParamsBound("", cutVal[iCut], 1);
        stringCut = stringMT2ll;
        stringCut += "Geq";
        stringCut += cutVal[iCut];
        
        H_UParPlusqT_MT2GeqCut[iCut].DefaultVarVals();
        H_UParPlusqT_MT2GeqCut[iCut].SetName(stringSmear, stringUParPlusqT, stringCut);
        H_UParPlusqT_MT2GeqCut[iCut].xAxis.SetAxis("patsy", stringUParPlusqT, &inSHBB->IHBB_PxPy, doSyst);
        H_UParPlusqT_MT2GeqCut[iCut].SetIndAxisLabel(stringSmear + stringUParPlusqT, mapVartoLabel, 1);
        H_UParPlusqT_MT2GeqCut[iCut].yAxis.SetAxisAsDefaultCountAxis("GeV");
        H_UParPlusqT_MT2GeqCut[iCut].specialHistCutFloat.SetParams(&SC_MT2llGeqCut[iCut]);
        H_UParPlusqT_MT2GeqCut[iCut].specialHistCutFloatVar = stringMT2ll;
        
        H_UPerp_MT2GeqCut[iCut].DefaultVarVals();
        H_UPerp_MT2GeqCut[iCut].SetName(stringSmear, stringUPerp, stringCut);
        H_UPerp_MT2GeqCut[iCut].xAxis.SetAxis("patsy", stringUPerp, &inSHBB->IHBB_PxPy, doSyst);
        H_UPerp_MT2GeqCut[iCut].SetIndAxisLabel(stringSmear + stringUPerp, mapVartoLabel, 1);
        H_UPerp_MT2GeqCut[iCut].yAxis.SetAxisAsDefaultCountAxis("GeV");
        H_UPerp_MT2GeqCut[iCut].specialHistCutFloat.SetParams(&SC_MT2llGeqCut[iCut]);
        H_UPerp_MT2GeqCut[iCut].specialHistCutFloatVar = stringMT2ll;
    }
    /*
    for (int iCut = 0; iCut < numCutVals; ++iCut) {
        inHistTVec->push_back(H_UParPlusqT_MT2GeqCut[iCut]);
    }
    for (int iCut = 0; iCut < numCutVals; ++iCut) {
        inHistTVec->push_back(H_UPerp_MT2GeqCut[iCut]);
    }
    */
}


inline void OneDeeHistTVec_AddMETPerformanceHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    bool doSyst = true;
    //bool noSyst = false;
    
    // MET Performance variables -- UPar/UPerp
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringUPar = "UPar";
    TString stringUParPlusqT = "UParPlusqT";
    TString stringUPerp = "UPerp";
    
    HistogramT H_UPar; H_UPar.DefaultVarVals();
    H_UPar.SetName(stringSmear, stringUPar, "");
    H_UPar.xAxis.SetAxis("patsy", stringUPar, 25, -300, 100, doSyst);
    H_UPar.SetIndAxisLabel(stringSmear + stringUPar, mapVartoLabel, 1);
    H_UPar.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_UParPlusqT; H_UParPlusqT.DefaultVarVals();
    H_UParPlusqT.SetName(stringSmear, stringUParPlusqT, "");
    H_UParPlusqT.xAxis.SetAxis("patsy", stringUParPlusqT, &inSHBB->IHBB_PxPy, doSyst);
    H_UParPlusqT.SetIndAxisLabel(stringSmear + stringUParPlusqT, mapVartoLabel, 1);
    H_UParPlusqT.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_UPerp; H_UPerp.DefaultVarVals();
    H_UPerp.SetName(stringSmear, stringUPerp, "");
    H_UPerp.xAxis.SetAxis("patsy", stringUPerp, &inSHBB->IHBB_PxPy, doSyst);
    H_UPerp.SetIndAxisLabel(stringSmear + stringUPerp, mapVartoLabel, 1);
    H_UPerp.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    inHistTVec->push_back(H_UPar);
    inHistTVec->push_back(H_UParPlusqT);
    inHistTVec->push_back(H_UPerp);
}



inline void OneDeeHistTVec_AddCompJetHists(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear) {
    
    // plots relevant to the total behavior of jets in the event
    bool doSyst = true;
    //bool noSyst = false;
    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringMET = "MET";
    
    TString stringNJets = "NJets";
    TString stringNBJets = "NBJets";
    TString stringHT = "HT";
    TString stringVecHT = "VecHT";
    
    TString stringHT_AllJets = "HT_AllJets";
    TString stringHadResParGauss = "HadResParGauss";
    TString stringHadResParGauss_AllJets = "HadResParGauss_AllJets";
    TString stringHadResParFull = "HadResParFull";
    TString stringHadResPerpGauss = "HadResPerpGauss";
    TString stringHadResPerpGauss_AllJets = "HadResPerpGauss_AllJets";
    TString stringHadResPerpFull = "HadResPerpFull";
    
    HistogramT H_NJets; H_NJets.DefaultVarVals();
    H_NJets.SetName(stringSmear, stringNJets, "");
    H_NJets.xAxis.SetAxis("patsy", stringNJets, &inSHBB->IHBB_NJets, doSyst);
    H_NJets.SetIndAxisLabel(stringSmear + stringNJets, mapVartoLabel, 1);
    H_NJets.yAxis.SetAxisAsDefaultCountAxis("N_{jets} Mult.");
    
    HistogramT H_NBJets; H_NBJets.DefaultVarVals();
    H_NBJets.SetName(stringSmear, stringNBJets, "");
    H_NBJets.xAxis.SetAxis("patsy", stringNBJets, &inSHBB->IHBB_NJets, doSyst);
    H_NBJets.SetIndAxisLabel(stringSmear + stringNBJets, mapVartoLabel, 1);
    H_NBJets.yAxis.SetAxisAsDefaultCountAxis("N_{b-jets} Mult.");
    
    HistogramT H_HT; H_HT.DefaultVarVals();
    H_HT.SetName(stringSmear, stringHT, "");
    H_HT.xAxis.SetAxis("patsy", stringHT, 30, 0., 900., doSyst);
    H_HT.SetIndAxisLabel(stringSmear + stringHT, mapVartoLabel, 1);
    H_HT.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_HT_AllJets; H_HT_AllJets.DefaultVarVals();
    H_HT_AllJets.SetName(stringSmear, stringHT_AllJets, "");
    H_HT_AllJets.xAxis.SetAxis("patsy", stringHT_AllJets, 30, 0., 900., doSyst);
    H_HT_AllJets.SetIndAxisLabel(stringSmear + stringHT_AllJets, mapVartoLabel, 1);
    H_HT_AllJets.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_HadResParGauss; H_HadResParGauss.DefaultVarVals();
    H_HadResParGauss.SetName(stringSmear, stringHadResParGauss, "");
    H_HadResParGauss.xAxis.SetAxis("patsy", stringHadResParGauss, &inSHBB->IHBB_RootEnergyPtMET, doSyst);
    H_HadResParGauss.SetIndAxisLabel(stringSmear +  stringHadResParGauss, mapVartoLabel, 1);
    H_HadResParGauss.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_HadResParGauss_AllJets; H_HadResParGauss_AllJets.DefaultVarVals();
    H_HadResParGauss_AllJets.SetName(stringSmear, stringHadResParGauss_AllJets, "");
    H_HadResParGauss_AllJets.xAxis.SetAxis("patsy", stringHadResParGauss_AllJets, &inSHBB->IHBB_RootEnergyPtMET, doSyst);
    H_HadResParGauss_AllJets.SetIndAxisLabel(stringSmear + stringHadResParGauss_AllJets, mapVartoLabel, 1);
    H_HadResParGauss_AllJets.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_HadResParFull; H_HadResParFull.DefaultVarVals();
    H_HadResParFull.SetName(stringSmear, stringHadResParFull, "");
    H_HadResParFull.xAxis.SetAxis("patsy", stringHadResParFull, &inSHBB->IHBB_RootEnergyPtMET, doSyst);
    H_HadResParFull.SetIndAxisLabel(stringSmear + stringHadResParFull, mapVartoLabel, 1);
    H_HadResParFull.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    
    HistogramT H_HadResPerpGauss; H_HadResPerpGauss.DefaultVarVals();
    H_HadResPerpGauss.SetName(stringSmear, stringHadResPerpGauss, "");
    H_HadResPerpGauss.xAxis.SetAxis("patsy", stringHadResPerpGauss, &inSHBB->IHBB_RootEnergyPtMET, doSyst);
    H_HadResPerpGauss.SetIndAxisLabel(stringSmear +  stringHadResPerpGauss, mapVartoLabel, 1);
    H_HadResPerpGauss.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_HadResPerpGauss_AllJets; H_HadResPerpGauss_AllJets.DefaultVarVals();
    H_HadResPerpGauss_AllJets.SetName(stringSmear, stringHadResPerpGauss_AllJets, "");
    H_HadResPerpGauss_AllJets.xAxis.SetAxis("patsy", stringHadResPerpGauss_AllJets, &inSHBB->IHBB_RootEnergyPtMET, doSyst);
    H_HadResPerpGauss_AllJets.SetIndAxisLabel(stringSmear + stringHadResPerpGauss_AllJets, mapVartoLabel, 1);
    H_HadResPerpGauss_AllJets.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_HadResPerpFull; H_HadResPerpFull.DefaultVarVals();
    H_HadResPerpFull.SetName(stringSmear, stringHadResPerpFull, "");
    H_HadResPerpFull.xAxis.SetAxis("patsy", stringHadResPerpFull, &inSHBB->IHBB_RootEnergyPtMET, doSyst);
    H_HadResPerpFull.SetIndAxisLabel(stringSmear + stringHadResPerpFull, mapVartoLabel, 1);
    H_HadResPerpFull.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_VecHT; H_VecHT.DefaultVarVals();
    H_VecHT.SetName(stringSmear, stringVecHT, "");
    H_VecHT.xAxis.SetAxis("patsy", stringVecHT, &inSHBB->IHBB_EnergyPtMET, doSyst);
    H_VecHT.SetIndAxisLabel(stringSmear + stringVecHT, mapVartoLabel, 1);
    H_VecHT.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    inHistTVec->push_back(H_NJets);
    inHistTVec->push_back(H_NBJets);
    inHistTVec->push_back(H_HT);
    inHistTVec->push_back(H_HT_AllJets);
    inHistTVec->push_back(H_HadResParGauss);
    inHistTVec->push_back(H_HadResParGauss_AllJets);
    inHistTVec->push_back(H_HadResParFull);
    inHistTVec->push_back(H_HadResPerpGauss);
    inHistTVec->push_back(H_HadResPerpGauss_AllJets);
    inHistTVec->push_back(H_HadResPerpFull);
    inHistTVec->push_back(H_VecHT);
}



//
/*
 histograms I'm leaving behind for now
 //Angular correlations
 
 //polarization energy "E-malgamation"
 HistogramT H_diLepEMinJetE; H_diLepEMinJetE.name = "h_diLepEMinJetE";
 H_diLepEMinJetE.xLabel = "E(l^{+}) + E(l^{-}) - E(J1) - E(J2) [GeV]"; H_diLepEMinJetE.xBinN = EnergyPtBinN; H_diLepEMinJetE.xMin = -EnergyPtBinUB; H_diLepEMinJetE.xMax = EnergyPtBinUB;
 H_diLepEMinJetE.yAxis.axisLabel = "Number of Events / ";
 numDivs = (H_diLepEMinJetE.xMax - H_diLepEMinJetE.xMin) / (float) H_diLepEMinJetE.xBinN;
 H_diLepEMinJetE.yAxis.axisLabel += "NUM"; H_diLepEMinJetE.yAxis.axisLabel += " GeV";
 H_diLepEMinJetE.xAxis.axisVarKey = "ELepEJet";
 H_diLepEMinJetE.doXSyst = true;
 
 HistogramT H_MET_div_Meff; H_MET_div_Meff.name = "h_MET_div_Meff";
 H_MET_div_Meff.xLabel = "#slash{E}_{T} / M_{eff}"; H_MET_div_Meff.xBinN = 50; H_MET_div_Meff.xMin = 0.; H_MET_div_Meff.xMax = 1.;
 H_MET_div_Meff.yAxis.axisLabel = "Number of Events / "; H_MET_div_Meff.yAxis.axisLabel += "NUM";
 H_MET_div_Meff.xAxis.axisVarKey = "METdivMeff";
 H_MET_div_Meff.doXSyst = true;
 
 HistogramT H_MET_div_Meff_PassMT2llCut80; H_MET_div_Meff_PassMT2llCut80.name = "h_MET_div_Meff_PassMT2llCut80";
 H_MET_div_Meff_PassMT2llCut80.xLabel = "#slash{E}_{T} / M_{eff}"; H_MET_div_Meff_PassMT2llCut80.xBinN = 50; H_MET_div_Meff_PassMT2llCut80.xMin = 0.; H_MET_div_Meff_PassMT2llCut80.xMax = 1.;
 H_MET_div_Meff_PassMT2llCut80.yAxis.axisLabel = "Number of Events / "; H_MET_div_Meff_PassMT2llCut80.yAxis.axisLabel += "NUM";
 H_MET_div_Meff_PassMT2llCut80.xAxis.axisVarKey = "METdivMeff_PassMT2llCut80";
 H_MET_div_Meff_PassMT2llCut80.doXSyst = true;
 
 HistogramT H_MET_div_Meff_PassMT2llCut90; H_MET_div_Meff_PassMT2llCut90.name = "h_MET_div_Meff_PassMT2llCut90";
 H_MET_div_Meff_PassMT2llCut90.xLabel = "#slash{E}_{T} / M_{eff}"; H_MET_div_Meff_PassMT2llCut90.xBinN = 50; H_MET_div_Meff_PassMT2llCut90.xMin = 0.; H_MET_div_Meff_PassMT2llCut90.xMax = 1.;
 H_MET_div_Meff_PassMT2llCut90.yAxis.axisLabel = "Number of Events / "; H_MET_div_Meff_PassMT2llCut90.yAxis.axisLabel += "NUM";
 H_MET_div_Meff_PassMT2llCut90.xAxis.axisVarKey = "METdivMeff_PassMT2llCut90";
 H_MET_div_Meff_PassMT2llCut90.doXSyst = true;
 
 HistogramT H_MET_div_Meff_PassMT2llCut100; H_MET_div_Meff_PassMT2llCut100.name = "h_MET_div_Meff_PassMT2llCut100";
 H_MET_div_Meff_PassMT2llCut100.xLabel = "#slash{E}_{T} / M_{eff}"; H_MET_div_Meff_PassMT2llCut100.xBinN = 50; H_MET_div_Meff_PassMT2llCut100.xMin = 0.; H_MET_div_Meff_PassMT2llCut100.xMax = 1.;
 H_MET_div_Meff_PassMT2llCut100.yAxis.axisLabel = "Number of Events / "; H_MET_div_Meff_PassMT2llCut100.yAxis.axisLabel += "NUM";
 H_MET_div_Meff_PassMT2llCut100.xAxis.axisVarKey = "METdivMeff_PassMT2llCut100";
 H_MET_div_Meff_PassMT2llCut100.doXSyst = true;
 
 HistogramT H_MET_div_Meff_PassMT2llCut110; H_MET_div_Meff_PassMT2llCut110.name = "h_MET_div_Meff_PassMT2llCut110";
 H_MET_div_Meff_PassMT2llCut110.xLabel = "#slash{E}_{T} / M_{eff}"; H_MET_div_Meff_PassMT2llCut110.xBinN = 50; H_MET_div_Meff_PassMT2llCut110.xMin = 0.; H_MET_div_Meff_PassMT2llCut110.xMax = 1.;
 H_MET_div_Meff_PassMT2llCut110.yAxis.axisLabel = "Number of Events / "; H_MET_div_Meff_PassMT2llCut110.yAxis.axisLabel += "NUM";
 H_MET_div_Meff_PassMT2llCut110.xAxis.axisVarKey = "METdivMeff_PassMT2llCut110";
 H_MET_div_Meff_PassMT2llCut110.doXSyst = true;
 
 HistogramT H_MET_div_Meff_PassMT2llCut120; H_MET_div_Meff_PassMT2llCut120.name = "h_MET_div_Meff_PassMT2llCut120";
 H_MET_div_Meff_PassMT2llCut120.xLabel = "#slash{E}_{T} / M_{eff}"; H_MET_div_Meff_PassMT2llCut120.xBinN = 50; H_MET_div_Meff_PassMT2llCut120.xMin = 0.; H_MET_div_Meff_PassMT2llCut120.xMax = 1.;
 H_MET_div_Meff_PassMT2llCut120.yAxis.axisLabel = "Number of Events / "; H_MET_div_Meff_PassMT2llCut120.yAxis.axisLabel += "NUM";
 H_MET_div_Meff_PassMT2llCut120.xAxis.axisVarKey = "METdivMeff_PassMT2llCut120";
 H_MET_div_Meff_PassMT2llCut120.doXSyst = true;
 
 */
inline void OneDeeHistTVec_Basic(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int cutPlotLevel) {
    OneDeeHistTVec_AddMiscHists(inHistTVec, inSHBB, mapVartoLabel);
    if (cutPlotLevel <= 0 || cutPlotLevel > 2) {
        for (int iLep = 0; iLep < 2; ++iLep) {
            OneDeeHistTVec_AddMiscLeptonHists(inHistTVec, inSHBB, mapVartoLabel, iLep);
        }
    }
}
inline void OneDeeHistTVec_IndObjKinematics(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool addCheckHistos = true, bool isSmear = false) {
    //int isLep = 0;
    int numObjs = 4;
    for (int iObj = 0; iObj < numObjs; ++iObj) {
        //if (iObj == isLep && isSmear) continue;
        for (int iLead = 0; iLead < 2; ++iLead) {
            if (addCheckHistos) {
                OneDeeHistTVec_AddIndKinObjectHists_Check(inHistTVec, inSHBB, mapVartoLabel, iObj, iLead, isSmear);
            }
            OneDeeHistTVec_AddIndKinObjectHists_Main(inHistTVec, inSHBB, mapVartoLabel, iObj, iLead, isSmear);
        }
    }
}
inline void OneDeeHistTVec_DiObjKinematics(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool addCheckHistos = true, bool isSmear = false) {
    int isLep = 0;
    int numObjs = 4;
    bool addZMass;
    for (int iObj = 0; iObj < numObjs; ++iObj) {
        if (iObj == isLep) {
            addZMass = true;
        }
        else {
            addZMass = false;
        }
        //if (iObj == isLep && isSmear) continue;
        if (addCheckHistos) {
            OneDeeHistTVec_AddDiKinObjectHists_Check(inHistTVec, inSHBB, mapVartoLabel, iObj, isSmear);
            OneDeeHistTVec_AddDiKinObjectHists_Main(inHistTVec, inSHBB, mapVartoLabel, iObj, isSmear, addZMass);
        }
        else {
            OneDeeHistTVec_AddDiKinObjectHists_Main(inHistTVec, inSHBB, mapVartoLabel, iObj, isSmear, addZMass);
        }
    }
}

inline void OneDeeHistTVec_AddMET_MT2(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear = false, int cutPlotLevel = false) {
    if (cutPlotLevel == 2 && !isSmear) return;
    if (cutPlotLevel != 2) {
        OneDeeHistTVec_AddBasicMETSigHists(inHistTVec, inSHBB, mapVartoLabel, isSmear);
        OneDeeHistTVec_AddBasicMETHists(inHistTVec, inSHBB, mapVartoLabel, isSmear);
        OneDeeHistTVec_AddMETPerformanceHists(inHistTVec, inSHBB, mapVartoLabel, isSmear);
        OneDeeHistTVec_AddMETPerformanceHists_SpecialCuts(inHistTVec, inSHBB, mapVartoLabel, isSmear);
        OneDeeHistTVec_AddMT2Hists_SpecialCuts(inHistTVec, inSHBB, mapVartoLabel, isSmear);
    }
    OneDeeHistTVec_AddMT2Hists(inHistTVec, inSHBB, mapVartoLabel, isSmear, cutPlotLevel);
}

inline void OneDeeHistTVec_AddCompJet(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, bool isSmear = false) {
    OneDeeHistTVec_AddCompJetHists(inHistTVec, inSHBB, mapVartoLabel, isSmear);
}
inline void OneDeeHistTVec_Inclusive(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int cutPlotLevel) {
    bool noSmear = false;
    
    
    OneDeeHistTVec_Basic(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);
    if (cutPlotLevel <= 0 || cutPlotLevel == 3) {
        OneDeeHistTVec_AddCompJetHists(inHistTVec, inSHBB, mapVartoLabel, noSmear);
        OneDeeHistTVec_IndObjKinematics(inHistTVec, inSHBB, mapVartoLabel, true, noSmear);
        OneDeeHistTVec_DiObjKinematics(inHistTVec, inSHBB, mapVartoLabel, true, noSmear);
        OneDeeHistTVec_AddDeltaPhi(inHistTVec, inSHBB, mapVartoLabel, noSmear);
    }
    OneDeeHistTVec_AddMET_MT2(inHistTVec, inSHBB, mapVartoLabel, noSmear, cutPlotLevel);
}
inline void OneDeeHistTVec_Inclusive_Smeared(vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int cutPlotLevel) {
    bool doSmear = true;
    if (cutPlotLevel <= 0 || cutPlotLevel == 3) {
        OneDeeHistTVec_AddCompJetHists(inHistTVec, inSHBB, mapVartoLabel, doSmear);
        OneDeeHistTVec_IndObjKinematics(inHistTVec, inSHBB, mapVartoLabel, false, doSmear);
        OneDeeHistTVec_DiObjKinematics(inHistTVec, inSHBB, mapVartoLabel, false, doSmear);
        OneDeeHistTVec_AddDeltaPhi(inHistTVec, inSHBB, mapVartoLabel, doSmear);
    }
    OneDeeHistTVec_AddMET_MT2(inHistTVec, inSHBB, mapVartoLabel, doSmear, cutPlotLevel);
}
