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
    string stringVecHT = "VecHT";
    
    inStVM[stringNJets]     = inEJI->EventNJets;
    inStVM[stringNBJets]    = inEJI->EventNBtagJets;
    inStVM[stringHT]        = inEJI->EventHT;
    inStVM[stringVecHT]     = inEJI->BVC_SumJet.Vec_Pt;
    
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
    TString stringUPerp      = "UPerp";
    
    inStVM[stringUPar]        = inERI->recoilUPar;
    inStVM[stringUParPlusqT]  = inERI->recoilUPar + inEDSI->DP_DiLepton.BVC.Vec_Pt;  
    inStVM[stringUPerp]       = inERI->recoilUPerp;
}

inline void SetStringKey_StFMap_AddMT2(StV_Map &inStVM, EventMT2Info * inEMT2I, int numJets) {
    TString stringMT2ll = "MT2ll";
    TString stringMT2lblb = "MT2lblb";
    TString stringMT2bb_ZMET = "MT2bb_ZMET";
    TString stringKT2_Top = "KT2_Top";
    TString baseMT2lblbToTString = "typeMT2lblb";
    
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
        inStVM[stringMT2bb_ZMET]                = inEMT2I->EventMT2bb_ZMET;
        inStVM[stringKT2_Top]                   = inEMT2I->EventMT2bb_ZMET - inEMT2I->EventMT2lblb;
        inStVM[baseMT2lblbToTString]            = inEMT2I->caseMT2lblb;
    }     
}

inline void SetStringKey_StFMap_AddMET(StV_Map &inStVM, EventMETInfo * inEMI, EventDiStructureInfo * inEDSI, int numJets) {
    SetStringKey_StFMap_AddBasicMET(inStVM, inEMI);
    SetStringKey_StFMap_AddMETPerformance(inStVM, &inEMI->MET_ERI, inEDSI);
    SetStringKey_StFMap_AddMT2(inStVM, &inEMI->MET_EMT2I, numJets);
}

inline void SetStringKey_StFMap_AddDiStructureKinInfo(StV_Map &inStVM, EventDiStructureInfo * inEDSI, EventJetInfo * inEJI) {
    
    TString stringObj;
    TString stringELepMinEJet = "ELepEJet";
    TString stringDi = "Di";
    int iLep = 0;
    int iJet = 1;
    int iBJet = 2;
    
    int numObjs = 3;
    
    // have to hack shit by hand as the DP_Di structures are saved in a non-conducive way to streamlining it
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
    
    SetStringKey_StFMap_AddMET(inStVM, inESPI->addEMI, inESPI->addEDSI, inESPI->addEJI->EventNJets);    
    SetStringKey_StFMap_IndLepton(inStVM, inESPI->addELI);
    SetStringKey_StFMap_IndJets(inStVM, inESPI->addEJI);    
    SetStringKey_StFMap_Basic(inStVM, inESPI, inEPI);
    SetStringKey_StFMap_AddDiStructureKinInfo(inStVM, inESPI->addEDSI, inESPI->addEJI);
    SetStringKey_StFMap_AddDiStructureAngleInfo(inStVM, inESPI->addEDSI, inESPI->addEJI);
    SetStringKey_StFMap_AddBasicMETSig(inStVM, RawMETSig);
}
