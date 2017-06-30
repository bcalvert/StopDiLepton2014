// Set of functions that handle setting up the unordered_map<TString, float> that the plot filler uses
// to automatically fill histograms with the correct event variables

inline void SetStringKey_StFMap_Basic(StV_Map &inStVM, EventStructPointerInfo * inESPI, EventPileUpInfo * inEPI) {
    inStVM["BasicWeight"] = 1.0;
    inStVM["CutFlowEntry"] = 4.0;
    inStVM["nVtx"] = inEPI->nVtx;
}
inline void SetStringKey_StFMap_IndLepton(StV_Map &inStVM, EventLepInfo * inELI) {
    int iNameLep = 0;
    TString leadString;
    TString stringLep = GetStringIndObj(iNameLep, false);
    for (int iLep = 0; iLep < inELI->numSavedLeps; ++iLep) {
        leadString = GetStringLead(iLep, false);
        leadString += stringLep;
        inStVM[leadString + TString("Pt")]  = inELI->vecEventLeps[iLep].BVC.Vec_Pt;
        inStVM[leadString + TString("Eta")] = inELI->vecEventLeps[iLep].BVC.Vec_Eta;
        inStVM[leadString + TString("En")] = inELI->vecEventLeps[iLep].BVC.Vec_En;
        inStVM[leadString + TString("Iso")] = inELI->vecEventLeps[iLep].relPFLepIso;
    }
}
inline void SetStringKey_StFMap_IndJets(StV_Map &inStVM, EventJetInfo * inEJI) {
    int iNameJet = 1;
    TString leadString;
    TString stringJetObj = GetStringIndObj(iNameJet, false);
    int iNameBJet = 2;
    TString stringBJetObj = GetStringIndObj(iNameBJet, false);
    
    string stringNJets = "NJets";
    string stringNBJets = "NBJets";
    string stringHT = "HT";
    string stringHT_AllJets = "HT_AllJets";
    string stringVecHT = "VecHT";
    
    inStVM[stringNJets]         = inEJI->EventNJets;
    inStVM[stringNBJets]        = inEJI->EventNBtagJets;
    inStVM[stringHT]            = inEJI->EventHT;
    inStVM[stringHT_AllJets]    = inEJI->EventHT_AllJets;
    inStVM[stringVecHT]         = inEJI->BVC_SumJet.Vec_Pt;
    
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        leadString = GetStringLead(iJet, false);
        leadString += stringJetObj;
        if (inEJI->EventNJets > iJet) {
            inStVM[leadString + TString("Pt")]  = inEJI->vecEventJets[iJet].BVC.Vec_Pt;
            inStVM[leadString + TString("Eta")] = inEJI->vecEventJets[iJet].BVC.Vec_Eta;
            inStVM[leadString + TString("En")] = inEJI->vecEventJets[iJet].BVC.Vec_En;
        }
    }
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        leadString = GetStringLead(iBJet, false);
        leadString += stringBJetObj;
        if (inEJI->EventNBtagJets > iBJet) {
            inStVM[leadString + TString("Pt")]  = inEJI->vecEventBTagJets[iBJet].BVC.Vec_Pt;
            inStVM[leadString + TString("Eta")] = inEJI->vecEventBTagJets[iBJet].BVC.Vec_Eta;
            inStVM[leadString + TString("En")] = inEJI->vecEventBTagJets[iBJet].BVC.Vec_En;
        }
    }
}

inline void SetStringKey_StFMap_HadrRes(StV_Map &inStVM, EventJetInfo * inEJI) {
    TString stringHadResParGauss = "HadResParGauss";
    TString stringHadResPerpGauss = "HadResPerpGauss";
    TString stringHadResParGauss_AllJets = "HadResParGauss_AllJets";
    TString stringHadResPerpGauss_AllJets = "HadResPerpGauss_AllJets";
    
    TString stringHadResParFull = "HadResParFull";
    TString stringHadResPerpFull = "HadResPerpFull";
    
    
    inStVM[stringHadResParGauss]          = inEJI->EventHadResParGauss;
    inStVM[stringHadResPerpGauss]         = inEJI->EventHadResPerpGauss;
    
    inStVM[stringHadResParGauss_AllJets]  = inEJI->EventHadResParGauss_AllJets;
    inStVM[stringHadResPerpGauss_AllJets] = inEJI->EventHadResPerpGauss_AllJets;
    
    inStVM[stringHadResParFull]  = inEJI->EventHadResParFull;
    inStVM[stringHadResPerpFull] = inEJI->EventHadResPerpFull;
    
}

inline void SetStringKey_StFMap_AddMETSigHack(StV_Map &inStVM, EventMETInfo * inEMI, EventJetInfo * inEJI, EventDiStructureInfo * inEDSI) {
    TString stringMETSig1DHack = "METSig1DHack";
    TString stringMETSig1DHack_AllJets = "METSig1DHack_AllJets";
    TString stringMETSig2DHack = "METSig2DHack";
    TString stringMETSig2DHack_Par = "METSig2DHack_Par";
    TString stringMETSig2DHack_Perp = "METSig2DHack_Perp";
    TString stringMETSig2DHack_AllJets = "METSig2DHack_AllJets";
    TString stringMETSig2DHack_Par_AllJets = "METSig2DHack_Par_AllJets";
    TString stringMETSig2DHack_Perp_AllJets = "METSig2DHack_Perp_AllJets";
    
    TString stringMETSig2DTrue = "METSig2DTrue";
    TString stringMETSig2DHackFull = "METSig2DHackFull";
    TString stringMETSig2DHack_ParFull = "METSig2DHack_ParFull";
    TString stringMETSig2DHack_PerpFull = "METSig2DHack_PerpFull";
    
    
    float resoMET1DHack = TMath::Sqrt(inEJI->EventHT);
    float resoMET2DHack_Par = inEJI->EventHadResParGauss;
    float resoMET2DHack_Perp = inEJI->EventHadResPerpGauss;
    float resoMET1DHack_AllJets = TMath::Sqrt(inEJI->EventHT_AllJets);
    float resoMET2DHack_Par_AllJets = inEJI->EventHadResParGauss_AllJets;
    float resoMET2DHack_Perp_AllJets = inEJI->EventHadResPerpGauss_AllJets;
    float resoMET2DHack_ParFull = inEJI->EventHadResParFull;
    float resoMET2DHack_PerpFull = inEJI->EventHadResPerpFull;
    
    
    float METSig1DHack = 0.0;
    float METSig1DHack_AllJets = 0.0;
    float METSig2DHack_Par = 0.0;
    float METSig2DHack_Perp = 0.0;
    float METSig2DHack = 0.0;
    float METSig2DHack_Par_AllJets = 0.0;
    float METSig2DHack_Perp_AllJets = 0.0;
    float METSig2DHack_AllJets = 0.0;
    
    float METSig2DHack_ParFull = 0.0;
    float METSig2DHack_PerpFull = 0.0;
    float METSig2DHackFull = 0.0;
    
    float METSig2DTrue = 0.0;
    
    if (inEJI->EventNJets > 0) {
        
        METSig1DHack = TMath::Power(inEMI->EventMET / resoMET1DHack, 2);
        
        METSig2DHack_Par = (inEMI->MET_ERI.recoilUPar + inEDSI->DP_DiLepton.BVC.Vec_Pt) / resoMET2DHack_Par;
        METSig2DHack_Perp = inEMI->MET_ERI.recoilUPerp / resoMET2DHack_Perp;
        
        METSig2DHack = TMath::Power(METSig2DHack_Perp, 2) + TMath::Power(METSig2DHack_Par, 2);
    }
    METSig1DHack_AllJets = TMath::Power(inEMI->EventMET / resoMET1DHack_AllJets, 2);
    METSig2DHack_Par_AllJets = (inEMI->MET_ERI.recoilUPar + inEDSI->DP_DiLepton.BVC.Vec_Pt) / resoMET2DHack_Par_AllJets;
    METSig2DHack_Perp_AllJets = inEMI->MET_ERI.recoilUPerp / resoMET2DHack_Perp_AllJets;
    METSig2DHack_ParFull = (inEMI->MET_ERI.recoilUPar + inEDSI->DP_DiLepton.BVC.Vec_Pt) / resoMET2DHack_ParFull;
    METSig2DHack_PerpFull = inEMI->MET_ERI.recoilUPerp / resoMET2DHack_PerpFull;
    METSig2DHack_AllJets = TMath::Power(METSig2DHack_Perp_AllJets, 2) + TMath::Power(METSig2DHack_Par_AllJets, 2);
    METSig2DHackFull = TMath::Power(METSig2DHack_PerpFull, 2) + TMath::Power(METSig2DHack_ParFull, 2);
    METSig2DTrue = inEMI->EventMETSigTrue;
    
    
    inStVM[stringMETSig1DHack]              = METSig1DHack;
    inStVM[stringMETSig2DHack]              = METSig2DHack;
    inStVM[stringMETSig2DHack_Par]          = METSig2DHack_Par;
    inStVM[stringMETSig2DHack_Perp]         = METSig2DHack_Perp;
    inStVM[stringMETSig1DHack_AllJets]      = METSig1DHack_AllJets;
    inStVM[stringMETSig2DHack_AllJets]      = METSig2DHack_AllJets;
    inStVM[stringMETSig2DHack_Par_AllJets]  = METSig2DHack_Par_AllJets;
    inStVM[stringMETSig2DHack_Perp_AllJets] = METSig2DHack_Perp_AllJets;
    
    inStVM[stringMETSig2DHackFull]          = METSig2DHackFull;
    inStVM[stringMETSig2DHack_ParFull]      = METSig2DHack_ParFull;
    inStVM[stringMETSig2DHack_PerpFull]     = METSig2DHack_PerpFull;
    
    inStVM[stringMETSig2DTrue]              = METSig2DTrue;
    
}

inline void SetStringKey_StFMap_AddBasicMET(StV_Map &inStVM, EventMETInfo * inEMI) {
    TString stringMET = "MET";
    
    inStVM[stringMET]                      = inEMI->EventMET;
    inStVM[stringMET + TString("Phi")]     = inEMI->EventMETPhi;
    inStVM[stringMET + TString("X")]       = inEMI->EventMET * TMath::Cos(inEMI->EventMETPhi);
    inStVM[stringMET + TString("Y")]       = inEMI->EventMET * TMath::Sin(inEMI->EventMETPhi);
    inStVM[stringMET + TString("divMeff")] = inEMI->EventMETdivMeff;
}

inline void SetStringKey_StFMap_AddMETPerformance(StV_Map &inStVM, EventRecoilInfo * inERI, EventDiStructureInfo * inEDSI) {
    TString stringUPar       = "UPar";
    TString stringUParPlusqT = "UParPlusqT";
    TString stringUParDivqT  = "UParDivqT";
    TString stringUPerp      = "UPerp";
    
    inStVM[stringUPar]        = inERI->recoilUPar;
    inStVM[stringUParPlusqT]  = inERI->recoilUPar + inEDSI->DP_DiLepton.BVC.Vec_Pt;
    inStVM[stringUParDivqT]   = -1 * inERI->recoilUPar / inEDSI->DP_DiLepton.BVC.Vec_Pt;
    inStVM[stringUPerp]       = inERI->recoilUPerp;
}

inline void SetStringKey_StFMap_AddMT2(StV_Map &inStVM, EventMT2Info * inEMT2I, int numJets) {
    TString stringMT2ll = "MT2ll";
    TString stringMT2lblb = "MT2lblb";
    TString stringMT2lblb_ZeroBLepMass = "MT2lblb_ZeroBLepMass";
    TString stringMT2bb_ZMET = "MT2bb_ZMET";
    TString stringKT2_Top = "KT2_Top";
    TString baseMT2lblbToTString = "typeMT2lblb";
    
    
    int iNameBLep = 3;
    TString stringBLepObj = GetStringIndObj(iNameBLep, false);
    
    inStVM[stringMT2ll]                   = inEMT2I->EventMT2ll;
    const int numCuts = 5;
    int cutVals[5] = {80, 90, 100, 110, 120};
    TString stringCutVal;
    for (int iCut = 0; iCut < numCuts; ++iCut) {
        stringCutVal = stringMT2ll;
        stringCutVal += cutVals[iCut];
        inStVM[stringCutVal]              = inEMT2I->EventMT2ll > cutVals[iCut];
    }
    if (numJets > 1) {
        inStVM[stringMT2lblb]                   = inEMT2I->EventMT2lblb;
        inStVM[stringMT2lblb_ZeroBLepMass]      = inEMT2I->EventMT2lblb_ZeroBLepMass;
        inStVM[stringMT2bb_ZMET]                = inEMT2I->EventMT2bb_ZMET;
        inStVM[stringKT2_Top]                   = inEMT2I->EventMT2bb_ZMET - inEMT2I->EventMT2lblb;
        inStVM[baseMT2lblbToTString]            = inEMT2I->caseMT2lblb;
        
        TString stringLead = GetStringLead(true, false);
        TString stringSubLead = GetStringLead(false, false);
        inStVM[stringLead + stringBLepObj + TString("InvMass")] = inEMT2I->EventMassBLep0_BLepsUsed;
        inStVM[stringSubLead + stringBLepObj + TString("InvMass")] = inEMT2I->EventMassBLep1_BLepsUsed;
        inStVM["Max" + stringBLepObj + TString("InvMass")] = TMath::Max(inEMT2I->EventMassBLep0_BLepsUsed, inEMT2I->EventMassBLep1_BLepsUsed);
    }
}

inline void SetStringKey_StFMap_AddTTBarPt(StV_Map &inStVM, EventMETInfo * inEMI, EventDiStructureInfo * inEDSI, EventJetInfo * inEJI) {
    int numJets = inEJI->EventNJets;
    TString stringTTBarPt = "TTBarPt";
    
    vector<TLorentzVector> vecJetMT2lblb(2);
    vector<int> vecJetPartFlavor(2);
    float currTTBarPx = 0.0, currTTBarPy = 0.0;
    
    bool doVerbosity = false;
    
    if (numJets > 1) {
        int numBJets = inEJI->EventNBtagJets;
        if (numBJets > 1) {
            currTTBarPx += inEJI->vecEventBTagJets[0].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventBTagJets[0].BVC.Vec_Phi);
            currTTBarPx += inEJI->vecEventBTagJets[1].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventBTagJets[1].BVC.Vec_Phi);
            currTTBarPy += inEJI->vecEventBTagJets[0].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventBTagJets[0].BVC.Vec_Phi);
            currTTBarPy += inEJI->vecEventBTagJets[1].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventBTagJets[1].BVC.Vec_Phi);
        }
        else if (numBJets == 1) {
            currTTBarPx += inEJI->vecEventBTagJets[0].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventBTagJets[0].BVC.Vec_Phi);
            currTTBarPy += inEJI->vecEventBTagJets[0].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventBTagJets[0].BVC.Vec_Phi);
            if (inEJI->vecEventJets[0].isBJet) {
                currTTBarPx += inEJI->vecEventJets[1].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventJets[1].BVC.Vec_Phi);
                currTTBarPy += inEJI->vecEventJets[1].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventJets[1].BVC.Vec_Phi);
            }
            else {
                currTTBarPx += inEJI->vecEventJets[0].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventJets[0].BVC.Vec_Phi);
                currTTBarPy += inEJI->vecEventJets[0].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventJets[0].BVC.Vec_Phi);
            }
        }
        else {
            currTTBarPx += inEJI->vecEventJets[0].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventJets[0].BVC.Vec_Phi);
            currTTBarPx += inEJI->vecEventJets[1].BVC.Vec_Pt * TMath::Cos(inEJI->vecEventJets[1].BVC.Vec_Phi);
            currTTBarPy += inEJI->vecEventJets[0].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventJets[0].BVC.Vec_Phi);
            currTTBarPy += inEJI->vecEventJets[1].BVC.Vec_Pt * TMath::Sin(inEJI->vecEventJets[1].BVC.Vec_Phi);
        }
        
        if (doVerbosity) {
            cout << "currTTBarPx Post Jets " << currTTBarPx << endl;
            cout << "currTTBarPy Post Jets " << currTTBarPy << endl;
        }
        
        currTTBarPx += inEDSI->DP_DiLepton.BVC.Vec_Pt * TMath::Cos(inEDSI->DP_DiLepton.BVC.Vec_Phi);
        currTTBarPy += inEDSI->DP_DiLepton.BVC.Vec_Pt * TMath::Sin(inEDSI->DP_DiLepton.BVC.Vec_Phi);        
        
        if (doVerbosity) {
            cout << "currTTBarPx Post Leps " << currTTBarPx << endl;
            cout << "currTTBarPy Post Leps " << currTTBarPy << endl;
        }
        
        currTTBarPx += inEMI->EventMET * TMath::Cos(inEMI->EventMETPhi);
        currTTBarPy += inEMI->EventMET * TMath::Sin(inEMI->EventMETPhi);
        
        if (doVerbosity) {
            cout << "currTTBarPx Post MET " << currTTBarPx << endl;
            cout << "currTTBarPy Post MET " << currTTBarPy << endl;
        }
        inStVM[stringTTBarPt] = TMath::Sqrt(TMath::Power(currTTBarPx, 2) + TMath::Power(currTTBarPy, 2));
        if (doVerbosity) {
            cout << "currTTBarPt " << TMath::Sqrt(TMath::Power(currTTBarPx, 2) + TMath::Power(currTTBarPy, 2)) << endl;
        }
    }
    else {
        inStVM[stringTTBarPt] = -1;
    }
}

inline void SetStringKey_StFMap_AddMET(StV_Map &inStVM, EventMETInfo * inEMI, EventDiStructureInfo * inEDSI, EventJetInfo * inEJI) {
    int numJets = inEJI->EventNJets;
    SetStringKey_StFMap_AddBasicMET(inStVM, inEMI);
    SetStringKey_StFMap_AddMETPerformance(inStVM, &inEMI->MET_ERI, inEDSI);
    SetStringKey_StFMap_AddMT2(inStVM, &inEMI->MET_EMT2I, numJets);
    SetStringKey_StFMap_AddMETSigHack(inStVM, inEMI, inEJI, inEDSI);
    SetStringKey_StFMap_AddTTBarPt(inStVM, inEMI, inEDSI, inEJI);
}

inline void SetStringKey_StFMap_AddDiStructureKinInfo(StV_Map &inStVM, EventDiStructureInfo * inEDSI, EventJetInfo * inEJI) {
    
    TString stringObj;
    TString stringELepMinEJet = "ELepEJet";
    TString stringDi = "Di";
    int iLep = 0;
    int iJet = 1;
    int iBJet = 2;
    int iBLep = 3;
    
    // have to hack shit by hand as the DP_Di structures are saved in a non-conducive way to streamlining it
    
    inStVM["qT"]          = inEDSI->DP_DiLepton.BVC.Vec_Pt;
    
    stringObj = GetStringIndObj(iLep, false);
    inStVM[stringDi + stringObj + TString("Pt")]          = inEDSI->DP_DiLepton.BVC.Vec_Pt;
    inStVM[stringDi + stringObj + TString("InvMass")]     = inEDSI->DP_DiLepton.BVC.Vec_Mass;
    inStVM[stringDi + stringObj + TString("Eta")]         = inEDSI->DP_DiLepton.BVC.Vec_Eta;
    inStVM[stringDi + stringObj + TString("Phi")]         = inEDSI->DP_DiLepton.BVC.Vec_Phi;
    
    if (inEJI->EventNJets > 1) {
        stringObj = GetStringIndObj(iJet, false);
        inStVM[stringDi + stringObj + TString("Pt")]      = inEDSI->DP_DiJet.BVC.Vec_Pt;
        inStVM[stringDi + stringObj + TString("InvMass")] = inEDSI->DP_DiJet.BVC.Vec_Mass;
        inStVM[stringDi + stringObj + TString("Eta")]     = inEDSI->DP_DiJet.BVC.Vec_Eta;
        inStVM[stringDi + stringObj + TString("Phi")]     = inEDSI->DP_DiJet.BVC.Vec_Phi;
        inStVM[stringELepMinEJet]              = inEDSI->ELepMinEJet;
        
        stringObj = GetStringIndObj(iBLep, false);
        inStVM[stringDi + stringObj + TString("Pt")]      = inEDSI->DP_DiBLeps.BVC.Vec_Pt;
        inStVM[stringDi + stringObj + TString("InvMass")] = inEDSI->DP_DiBLeps.BVC.Vec_Mass;
        inStVM[stringDi + stringObj + TString("Eta")]     = inEDSI->DP_DiBLeps.BVC.Vec_Eta;
        inStVM[stringDi + stringObj + TString("Phi")]     = inEDSI->DP_DiBLeps.BVC.Vec_Phi;
    }
    
    if (inEJI->EventNBtagJets > 1) {
        stringObj = GetStringIndObj(iBJet, false);
        inStVM[stringDi + stringObj + TString("Pt")]      = inEDSI->DP_DiBJet.BVC.Vec_Pt;
        inStVM[stringDi + stringObj + TString("InvMass")] = inEDSI->DP_DiBJet.BVC.Vec_Mass;
        inStVM[stringDi + stringObj + TString("Eta")]     = inEDSI->DP_DiBJet.BVC.Vec_Eta;
        inStVM[stringDi + stringObj + TString("Phi")]     = inEDSI->DP_DiBJet.BVC.Vec_Phi;
    }
    
}

inline void SetStringKey_StFMap_AddDiStructureAngleInfo(StV_Map &inStVM, EventDiStructureInfo * inEDSI, EventJetInfo * inEJI) {
    int numSavedLeps = inEDSI->numLeps;
    
    int numSavedJets = inEDSI->numJets;
    int numJetsPresent = inEJI->EventNJets;
    
    int numSavedBJets = inEDSI->numBJets;
    int numBJetsPresent = inEJI->EventNBtagJets;
    
    int intLep  = 0;
    int intJet  = 1;
    int intBJet = 2;
    int intBLep = 3;
    int intZ    = 4;
    int intMET  = 5;
    
    
    inStVM[DPhiVarString(intLep, intLep)] = inEDSI->DP_DiLepton.DPhiInputObjects;
    inStVM[DPhiVarString(intZ, intMET)]   = inEDSI->DP_ZMET.DPhiInputObjects;
    
    for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
        inStVM[DPhiVarString(intLep, intMET, iLep, -1)] = inEDSI->vecDP_LepMET[iLep].DPhiInputObjects;
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            if (iJet >= numJetsPresent) continue;
            inStVM[DPhiVarString(intLep, intJet, iLep, iJet)] = inEDSI->vecVecDP_LepJet[iLep][iJet].DPhiInputObjects;
        }
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            if (iBJet >= numBJetsPresent) continue;
            inStVM[DPhiVarString(intLep, intBJet, iLep, iBJet)] = inEDSI->vecVecDP_LepBJet[iLep][iBJet].DPhiInputObjects;
        }
    }
    for (int iJet = 0; iJet < numSavedJets; ++iJet) {
        if (iJet >= numJetsPresent) continue;
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            if (iBJet >= numBJetsPresent) continue;
            inStVM[DPhiVarString(intJet, intBJet, iJet, iBJet)] = inEDSI->vecVecDP_JetBJet[iJet][iBJet].DPhiInputObjects;
        }
    }
    if (inEJI->EventNJets > 1) {
        inStVM[DPhiVarString(intJet, intJet)] = inEDSI->DP_DiJet.DPhiInputObjects;
        inStVM[DPhiVarString(intBLep, intBLep)] = inEDSI->DP_DiBLeps.DPhiInputObjects;
        if (inEJI->EventNBtagJets > 1) {
            inStVM[DPhiVarString(intBJet, intBJet)] = inEDSI->DP_DiBJet.DPhiInputObjects;
        }
    }
}

inline void SetStringKey_StFMap_AddBasicMETSig(StV_Map &inStVM, float RawMETSig) {
    TString stringMETSigRaw = "METSigRaw";
    
    inStVM[stringMETSigRaw] = RawMETSig;
}

inline void SetStringKey_StFMap(StV_Map &inStVM, EventStructPointerInfo * inESPI, EventPileUpInfo * inEPI, float RawMETSig) {
    ///Set up the mapping of string keys to the appropriate event variables
    
    SetStringKey_StFMap_AddMET(inStVM, inESPI->addEMI, inESPI->addEDSI, inESPI->addEJI);
    SetStringKey_StFMap_IndLepton(inStVM, inESPI->addELI);
    SetStringKey_StFMap_IndJets(inStVM, inESPI->addEJI);
    SetStringKey_StFMap_HadrRes(inStVM, inESPI->addEJI);
    SetStringKey_StFMap_Basic(inStVM, inESPI, inEPI);
    SetStringKey_StFMap_AddDiStructureKinInfo(inStVM, inESPI->addEDSI, inESPI->addEJI);
    SetStringKey_StFMap_AddDiStructureAngleInfo(inStVM, inESPI->addEDSI, inESPI->addEJI);
    SetStringKey_StFMap_AddBasicMETSig(inStVM, RawMETSig);
}
