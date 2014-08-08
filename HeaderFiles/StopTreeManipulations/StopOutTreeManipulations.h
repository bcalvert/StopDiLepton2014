// Functions used as part of the nTuple skimming macro
// Set up the output branch address names and values for the skim output tree


inline void SetOutTreeBranchPassesCutInfo(TTree * outTree, bool passesFullCut, int whichSyst = 0) {
    TString appendStringSyst = SystString(whichSyst);
    outTree->Branch("TEventPassesFullSelection" + appendStringSyst, &passesFullCut);
}
/*
 int systLepES = 1;
 int systJetES = 2;
 int systBTagEffSF = 3;
 int systBMisTagSF = 4;
 int systJetSmear = 5;
 int systUncES = 6;
 */
inline void SetOutTreeBranchLeptonInfo(TTree * outTree, EventLepInfo * inELI, int whichSyst = 0, bool isData = false, bool keepLooseLeps = false) {
    TString appendStringSyst = SystString(whichSyst);
    TString numString;
    /*
    int systLepES = 1;
    int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    int systJetSmear = 5;
    int systUncES = 6;
    */
    
    TString lepString = "TLep";
    outTree->Branch("TDoEvent" + appendStringSyst,                     &inELI->doEvent);
    outTree->Branch("TChannel" + appendStringSyst,                     &inELI->EventDiLepType);
    outTree->Branch("TDiLepInZ" + appendStringSyst,                    &inELI->EventDiLepinZMass);
    outTree->Branch("TNIsoElecs_pT20" + appendStringSyst,              &inELI->ELI_LC.EventNIsoElecs_pT20);
    outTree->Branch("TNIsoMuons_pT20" + appendStringSyst,              &inELI->ELI_LC.EventNIsoMuons_pT20);
    outTree->Branch("TNIsoPosits_pT20" + appendStringSyst,             &inELI->ELI_LC.EventNIsoPosits_pT20);
    outTree->Branch("TNIsoMubars_pT20" + appendStringSyst,             &inELI->ELI_LC.EventNIsoMubars_pT20);
    outTree->Branch("TNIsoElecs_pT10to20" + appendStringSyst,          &inELI->ELI_LC.EventNIsoElecs_pT10to20);
    outTree->Branch("TNIsoMuons_pT10to20" + appendStringSyst,          &inELI->ELI_LC.EventNIsoMuons_pT10to20);
    outTree->Branch("TNIsoPosits_pT10to20" + appendStringSyst,         &inELI->ELI_LC.EventNIsoPosits_pT10to20);
    outTree->Branch("TNIsoMubars_pT10to20" + appendStringSyst,         &inELI->ELI_LC.EventNIsoMubars_pT10to20);
    outTree->Branch("TNViableLepPairsPreMassCut" + appendStringSyst,   &inELI->ELI_LC.EventNViableLepPairsPreMassCut);
    outTree->Branch("TNViableLepPairsPostMassCut" + appendStringSyst,  &inELI->ELI_LC.EventNViableLepPairsPostMassCut);
    
    outTree->Branch("TDiLepMass" + appendStringSyst,                   &inELI->EventDiLepMass);    
    
    for (int iLep = 0; iLep < inELI->numSavedLeps; ++iLep) {
        numString = ""; numString += iLep;
        outTree->Branch(lepString + numString + TString("Pt") + appendStringSyst,                      &inELI->vecEventLeps[iLep].BVC.Vec_Pt); 
        outTree->Branch(lepString + numString + TString("Eta") + appendStringSyst,                     &inELI->vecEventLeps[iLep].BVC.Vec_Eta); 
        outTree->Branch(lepString + numString + TString("Phi") + appendStringSyst,                     &inELI->vecEventLeps[iLep].BVC.Vec_Phi); 
        outTree->Branch(lepString + numString + TString("E") + appendStringSyst,                       &inELI->vecEventLeps[iLep].BVC.Vec_En);
        outTree->Branch(lepString + numString + TString("PDGID") + appendStringSyst,                   &inELI->vecEventLeps[iLep].PDGID);
        outTree->Branch(lepString + numString + TString("RelPFIso") + appendStringSyst,                &inELI->vecEventLeps[iLep].relPFLepIso);
        
        if (keepLooseLeps) {
            outTree->Branch(lepString + numString + TString("LepQual") + appendStringSyst,                &inELI->vecEventLeps[iLep].lepQuality);
        }
        
        if (!isData) {
            outTree->Branch(lepString + numString + TString("GenMatched") + appendStringSyst,      &inELI->vecEventLeps[iLep].isGenMatched);
            outTree->Branch(lepString + numString + TString("GM_PromptLepton") + appendStringSyst, &inELI->vecEventLeps[iLep].isPromptLepton);
            outTree->Branch(lepString + numString + TString("GM_Pt") + appendStringSyst,           &inELI->vecEventLeps[iLep].GP.BVC.Vec_Pt);
            outTree->Branch(lepString + numString + TString("GM_Eta") + appendStringSyst,          &inELI->vecEventLeps[iLep].GP.BVC.Vec_Eta);
            outTree->Branch(lepString + numString + TString("GM_Phi") + appendStringSyst,          &inELI->vecEventLeps[iLep].GP.BVC.Vec_Phi);
            outTree->Branch(lepString + numString + TString("GM_E") + appendStringSyst,            &inELI->vecEventLeps[iLep].GP.BVC.Vec_En);
            outTree->Branch(lepString + numString + TString("GM_PDGID") + appendStringSyst,        &inELI->vecEventLeps[iLep].PDGID);
            outTree->Branch(lepString + numString + TString("GM_MomPDGID") + appendStringSyst,     &inELI->vecEventLeps[iLep].GP.MomPDGID);
            outTree->Branch(lepString + numString + TString("GM_MomStatus") + appendStringSyst,    &inELI->vecEventLeps[iLep].GP.MomPDGStatus);
        }
    }
}

inline void SetOutTreeBranchJetInfo(TTree * outTree, EventJetInfo * inEJI, int whichSyst = 0, bool isSmear = false) {
    TString appendStringSyst = SystString(whichSyst);
    TString numString;
    TString stringJet = "Jet";
    TString stringBJet = "BTagJet";
    TString stringSmear = isSmear ? "Smear" : "";
    
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    
    outTree->Branch(TString("TN") + stringSmear + TString("Jets") + appendStringSyst,           &inEJI->EventNJets);   
    outTree->Branch(TString("THT") + stringSmear + appendStringSyst,                            &inEJI->EventHT);
    outTree->Branch(TString("TSumJetPt") + stringSmear + appendStringSyst,                      &inEJI->BVC_SumJet.Vec_Pt);
    outTree->Branch(TString("TSumJetPhi") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumJet.Vec_Phi);
    outTree->Branch(TString("TSumJetEta") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumJet.Vec_Eta);
    outTree->Branch(TString("TSumJetEn") + stringSmear + appendStringSyst,                      &inEJI->BVC_SumJet.Vec_En);
    outTree->Branch(TString("TSumBJetPt") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumBJet.Vec_Pt);
    outTree->Branch(TString("TSumBJetPhi") + stringSmear + appendStringSyst,                    &inEJI->BVC_SumBJet.Vec_Phi);
    outTree->Branch(TString("TSumBJetEta") + stringSmear + appendStringSyst,                    &inEJI->BVC_SumBJet.Vec_Eta);
    outTree->Branch(TString("TSumBJetEn") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumBJet.Vec_En);
    if (whichSyst != systBMisTagSF && whichSyst != systBTagEffSF) {
        for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
            numString = "";
            numString += iJet;
            outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("Pt") + appendStringSyst,          &inEJI->vecEventJets[iJet].BVC.Vec_Pt); 
            outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("Eta") + appendStringSyst,         &inEJI->vecEventJets[iJet].BVC.Vec_Eta); 
            outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("Phi") + appendStringSyst,         &inEJI->vecEventJets[iJet].BVC.Vec_Phi); 
            outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("En") + appendStringSyst,          &inEJI->vecEventJets[iJet].BVC.Vec_En); 
            outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("BTagDisc") + appendStringSyst,    &inEJI->vecEventJets[iJet].valBTagDisc);
            if (isSmear) {
                outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("isGenMatched") + appendStringSyst,        &inEJI->vecEventJets[iJet].isGenJetMatched);
                outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("DeltaEnRecoGenMatch") + appendStringSyst,        &inEJI->vecEventJets[iJet].dEnRecoGen);
            }        
        }
    }
    outTree->Branch(TString("TN") + stringSmear + TString("JetsBtag") + appendStringSyst,       &inEJI->EventNBtagJets);
    
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        numString = "";
        numString += iJet;
        outTree->Branch(TString("T") + stringSmear + stringJet + numString + TString("isBTagged") + appendStringSyst,   &inEJI->vecEventJets[iJet].isBJet);
    }
    
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        numString = "";
        numString += iBJet;
        outTree->Branch(TString("T") + stringSmear + stringBJet + numString + TString("Pt") + appendStringSyst,          &inEJI->vecEventBTagJets[iBJet].BVC.Vec_Pt); 
        outTree->Branch(TString("T") + stringSmear + stringBJet + numString + TString("Eta") + appendStringSyst,         &inEJI->vecEventBTagJets[iBJet].BVC.Vec_Eta); 
        outTree->Branch(TString("T") + stringSmear + stringBJet + numString + TString("Phi") + appendStringSyst,         &inEJI->vecEventBTagJets[iBJet].BVC.Vec_Phi); 
        outTree->Branch(TString("T") + stringSmear + stringBJet + numString + TString("En") + appendStringSyst,          &inEJI->vecEventBTagJets[iBJet].BVC.Vec_En);
        outTree->Branch(TString("T") + stringSmear + stringBJet + numString + TString("Index") + appendStringSyst,       &inEJI->vecEventBTagJets_Index[iBJet]);
    } 
}


inline void SetOutTreeBranchMETInfo(TTree * outTree, EventMETInfo * inEMI, int whichSyst = 0, bool isSmear = false, bool isPhiCorr = true, TString appendString = "") {
    TString appendStringSyst = SystString(whichSyst);
    TString noPhiCorrString = isPhiCorr ? "" : "_noPhiCorr";
    TString stringSmear = isSmear ? "Smear" : "";
    TString stringMETType[5] = {"PF", "", "", "", "Calo"};
    
    noPhiCorrString += appendString;
    
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET") + appendStringSyst + noPhiCorrString,         &inEMI->EventMET); 
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETPhi);
    //    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_Sig") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETSig);
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_SumET") + appendStringSyst + noPhiCorrString,     &inEMI->EventSumET);
    
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2ll") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll);  
    if (whichSyst != systBTagEffSF && whichSyst != systBMisTagSF) {
        if (isPhiCorr) {
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT_Lep0") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep0);
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT_Lep1") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep1);
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT_HTVec") + appendStringSyst + noPhiCorrString,    &inEMI->MET_EMT2I.MT_HTVec);
            
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("METdivMeff") + appendStringSyst + noPhiCorrString,  &inEMI->EventMETdivMeff);
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("METdivHT") + appendStringSyst + noPhiCorrString,    &inEMI->EventMETdivHT);
            
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("UPar") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPar);        
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("UPerp") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPerp);
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2lb") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2lb);                        
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("DPhiMT2lb_JetsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed);
            outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("DPhiMT2lb_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed);
        }
    }
    if (isPhiCorr) {
        outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2lb_ToT") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.caseMT2lb);       
    }
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("BMET") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMET);
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("BMET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMETPhi);
    outTree->Branch(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2ll_BMET") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll_BMET);
}

inline void SetOutTreeBranchSpecialMETInfo(TTree * outTree, EventMETInfo * inEMI, int whichSyst = 0, bool isSmear = false, bool isPhiCorr = true, TString appendString = "") {
    TString appendStringSyst = SystString(whichSyst);
    TString noPhiCorrString = isPhiCorr ? "" : "_noPhiCorr";
    noPhiCorrString += "_v2";
    noPhiCorrString += appendString;
    
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    
    TString stringSmear = isSmear ? "Smear" : "";
    outTree->Branch(TString("T") + stringSmear + TString("MET") + appendStringSyst + noPhiCorrString,         &inEMI->EventMET); 
    outTree->Branch(TString("T") + stringSmear + TString("MET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETPhi);
    //    outTree->Branch(TString("T") + stringSmear + TString("MET_Sig") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETSig);
    //    outTree->Branch(TString("T") + stringSmear + TString("MET_SumET") + appendStringSyst + noPhiCorrString,     &inEMI->EventSumET);
    
    outTree->Branch(TString("T") + stringSmear + TString("MT2ll") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll);                    
    if (whichSyst != systBTagEffSF && whichSyst != systBMisTagSF) {
        if (isPhiCorr) {
            outTree->Branch(TString("T") + stringSmear + TString("MT_Lep0") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep0);
            outTree->Branch(TString("T") + stringSmear + TString("MT_Lep1") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep1);
            outTree->Branch(TString("T") + stringSmear + TString("MT_HTVec") + appendStringSyst + noPhiCorrString,    &inEMI->MET_EMT2I.MT_HTVec);
            
            outTree->Branch(TString("T") + stringSmear + TString("METdivMeff") + appendStringSyst + noPhiCorrString,  &inEMI->EventMETdivMeff);
            outTree->Branch(TString("T") + stringSmear + TString("METdivHT") + appendStringSyst + noPhiCorrString,    &inEMI->EventMETdivHT);
            
            outTree->Branch(TString("T") + stringSmear + TString("UPar") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPar);        
            outTree->Branch(TString("T") + stringSmear + TString("UPerp") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPerp);
            
            outTree->Branch(TString("T") + stringSmear + TString("MT2lb") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2lb);                        
            outTree->Branch(TString("T") + stringSmear + TString("DPhiMT2lb_JetsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed);
            outTree->Branch(TString("T") + stringSmear + TString("DPhiMT2lb_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed);
        }
    }
    if (isPhiCorr) {
        outTree->Branch(TString("T") + stringSmear + TString("MT2lb_ToT") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.caseMT2lb);       
    }
    outTree->Branch(TString("T") + stringSmear + TString("BMET") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMET);
    outTree->Branch(TString("T") + stringSmear + TString("BMET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMETPhi);
    outTree->Branch(TString("T") + stringSmear + TString("MT2ll_BMET") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll_BMET);
}

inline void SetOutTreeBranchDiStructureInfo(TTree * outTree, EventDiStructureInfo * inEDSI, int whichSyst = 0, bool isSmear = false, bool isPhiCorr = true) {
    TString appendStringSyst = SystString(whichSyst);
    TString noPhiCorrString = isPhiCorr ? "" : "_noPhiCorr";
    TString stringSmear = isSmear ? "Smear" : "";
    DiParticle * DP_ToUse = 0;
    
    int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    int systUncES = 6;
    
    const int numDPs = 3;
    int startDPIndex = isSmear ? 1 : 0;
    for (int iDP = startDPIndex; iDP < numDPs; ++iDP) {
        switch (iDP) {
            case 0:
                DP_ToUse = &inEDSI->DP_DiLepton;
                break;                
            case 1:
                DP_ToUse = &inEDSI->DP_DiJet;
                break;            
            case 2:
                DP_ToUse = &inEDSI->DP_DiBJet;
                break;
            default:
                break;
        }
        if (iDP == 0) stringSmear = "";
        else stringSmear = isSmear ? "Smear" : "";   
        
        SetDiStructureBranch(outTree, DP_ToUse, TString("T") + stringSmear + DP_ToUse->Name, appendStringSyst, true);
        /*
        outTree->Branch(TString("T") + stringSmear + DP_ToUse->Name + TString("Pt")   + appendStringSyst, &DP_ToUse->BVC.Vec_Pt);
        outTree->Branch(TString("T") + stringSmear + DP_ToUse->Name + TString("Eta")  + appendStringSyst, &DP_ToUse->BVC.Vec_Eta);
        outTree->Branch(TString("T") + stringSmear + DP_ToUse->Name + TString("Mass") + appendStringSyst, &DP_ToUse->BVC.Vec_Mass);
        outTree->Branch(TString("T") + stringSmear + DP_ToUse->Name + TString("Phi")  + appendStringSyst, &DP_ToUse->BVC.Vec_Phi);
        outTree->Branch(TString("T") + stringSmear + DP_ToUse->Name + TString("_IndObjDPhi")  + appendStringSyst, &DP_ToUse->DPhiInputObjects);
         */
    }
    stringSmear = isSmear ? "Smear" : "";
    if (whichSyst != systBMisTagSF && whichSyst != systBTagEffSF) {
        SetDiStructureBranch(outTree, &inEDSI->DP_ZMET, TString("T") + stringSmear + inEDSI->DP_ZMET.Name, appendStringSyst, false);
        SetDiStructureBranch(outTree, &inEDSI->DP_DiBLeps, TString("T") + stringSmear + inEDSI->DP_DiBLeps.Name, appendStringSyst, false);
    }
    
    for (int iLep = 0; iLep < inEDSI->numLeps; ++iLep) {
        if (whichSyst != systBMisTagSF && whichSyst != systBTagEffSF) {
            SetDiStructureBranch(outTree, &inEDSI->vecDP_LepMET[iLep], TString("T") + stringSmear + inEDSI->vecDP_LepMET[iLep].Name, appendStringSyst, false);
            if (whichSyst != systUncES) {
                for (int iJet = 0; iJet < inEDSI->numJets; ++iJet) {
                    SetDiStructureBranch(outTree, &inEDSI->vecVecDP_LepJet[iLep][iJet], TString("T") + stringSmear + inEDSI->vecVecDP_LepJet[iLep][iJet].Name, appendStringSyst, false);
                }
            }
        }
        if (whichSyst != systUncES) {
            for (int iBJet = 0; iBJet < inEDSI->numBJets; ++iBJet) {
                SetDiStructureBranch(outTree, &inEDSI->vecVecDP_LepBJet[iLep][iBJet], TString("T") + stringSmear + inEDSI->vecVecDP_LepBJet[iLep][iBJet].Name, appendStringSyst, false);
            }
        }
    }     
    if (whichSyst != systLepES && whichSyst != systUncES) {
        for (int iJet = 0; iJet < inEDSI->numJets; ++iJet) {
            for (int iBJet = 0; iBJet < inEDSI->numBJets; ++iBJet) {
                SetDiStructureBranch(outTree, &inEDSI->vecVecDP_JetBJet[iJet][iBJet], TString("T") + stringSmear + inEDSI->vecVecDP_JetBJet[iJet][iBJet].Name, appendStringSyst, false);
            }
        }
    }
}

inline void SetOutTreeBranchGenLepton(TTree * outTree, EventGenLeptonParticleInfo * inEGLPI) {

    TString prefixGenLep = "TGen";
    TString prefixBranchName;
    for (int iLepSt3 = 0; iLepSt3 < inEGLPI->numSt3Leps; ++iLepSt3) {
        for (int iIndLep = 0; iIndLep < inEGLPI->numIndLeps; ++iIndLep) {
            prefixBranchName = prefixGenLep + inEGLPI->nameLeps[iLepSt3] + TString("_St3_");
            prefixBranchName += iIndLep;
            inEGLPI->vecVecGenLepPSt3I[iLepSt3][iIndLep].SetOutBranches(outTree, prefixBranchName);            
        }
    }
    for (int iLepSt1 = 0; iLepSt1 < inEGLPI->numSt1Leps; ++iLepSt1) {
        for (int iIndLep = 0; iIndLep < inEGLPI->numIndLeps; ++iIndLep) {
            prefixBranchName = prefixGenLep + inEGLPI->nameLeps[iLepSt1] + TString("_St1_");
            prefixBranchName += iIndLep;
            inEGLPI->vecVecGenLepPSt1I[iLepSt1][iIndLep].SetOutBranches(outTree, prefixBranchName);            
        }
    }
    /*
    GenParticleSt3Info * GPISt3_0_ToUse = 0, * GPISt3_1_ToUse = 0;
    GenParticleSt1Info * GPISt1_0_ToUse = 0, * GPISt1_1_ToUse = 0;
    for (int iLep = 0; iLep < numLep; ++iLep) {
        switch (iLep) {
            case 0:
                GPISt1_0_ToUse = &inEGLPI->genElecSt1_0;
                GPISt1_1_ToUse = &inEGLPI->genElecSt1_1;
                GPISt3_0_ToUse = &inEGLPI->genElecSt3_0;
                GPISt3_1_ToUse = &inEGLPI->genElecSt3_1;
                break;
            case 1:
                GPISt1_0_ToUse = &inEGLPI->genMuonSt1_0;
                GPISt1_1_ToUse = &inEGLPI->genMuonSt1_1;
                GPISt3_0_ToUse = &inEGLPI->genMuonSt3_0;
                GPISt3_1_ToUse = &inEGLPI->genMuonSt3_1;
                break;                
            default:
                break;
        }
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_PDGID"),      &GPISt3_0_ToUse->GP.PDGID );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_FirstMom"),   &GPISt3_0_ToUse->GP_FirstMom );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_Index"),      &GPISt3_0_ToUse->GP_Index );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_Energy"),     &GPISt3_0_ToUse->GP.BVC.Vec_En );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_Pt"),         &GPISt3_0_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_Eta"),        &GPISt3_0_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_0_Phi"),        &GPISt3_0_ToUse->GP.BVC.Vec_Phi );
        
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_PDGID"),      &GPISt3_1_ToUse->GP.PDGID );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_FirstMom"),   &GPISt3_1_ToUse->GP_FirstMom );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_Index"),      &GPISt3_1_ToUse->GP_Index );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_Energy"),     &GPISt3_1_ToUse->GP.BVC.Vec_En );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_Pt"),         &GPISt3_1_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_Eta"),        &GPISt3_1_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch(prefixGenLep + nameLep[iLep] + TString("St3_1_Phi"),        &GPISt3_1_ToUse->GP.BVC.Vec_Phi );
        
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_PID"),       &GPISt1_0_ToUse->GP.PDGID );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_Pt"),        &GPISt1_0_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_Eta"),       &GPISt1_0_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_Phi"),       &GPISt1_0_ToUse->GP.BVC.Vec_Phi );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_Energy"),    &GPISt1_0_ToUse->GP.BVC.Vec_En );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_MomPID"),    &GPISt1_0_ToUse->GP_Mom.PDGID );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_MomPt"),     &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Pt );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_MomEta"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Eta );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_MomPhi"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Phi );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_MomEnergy"), &GPISt1_0_ToUse->GP_Mom.BVC.Vec_En );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_0_MomStat"),   &GPISt1_0_ToUse->GP_Mom.PDGStatus );
        
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_PID"),       &GPISt1_1_ToUse->GP.PDGID );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_Pt"),        &GPISt1_1_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_Eta"),       &GPISt1_1_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_Phi"),       &GPISt1_1_ToUse->GP.BVC.Vec_Phi );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_Energy"),    &GPISt1_1_ToUse->GP.BVC.Vec_En );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_MomPID"),    &GPISt1_1_ToUse->GP_Mom.PDGID );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_MomPt"),     &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Pt );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_MomEta"),    &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Eta );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_MomPhi"),     &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Phi );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_MomEnergy"), &GPISt1_1_ToUse->GP_Mom.BVC.Vec_En );
        outTree->Branch( prefixGenLep + nameLep[iLep] + TString("St1_1_MomStat"),   &GPISt1_1_ToUse->GP_Mom.PDGStatus );
        
    }
     */
}



inline void SetOutTreeBranchGenQuark(TTree * outTree, EventGenQuarkParticleInfo * inEGQPI) {
    const int numQuark = 2;
    TString nameQuark[numQuark] = {"Top", "B"};
    TString prefixGenQuark = "TGen";
    GenParticleSt3Info * GPISt3_0_ToUse = 0, * GPISt3_1_ToUse = 0;
    GenParticleSt1Info * GPISt1_0_ToUse = 0, * GPISt1_1_ToUse = 0;
    for (int iQuark = 0; iQuark < numQuark; ++iQuark) {
        switch (iQuark) {
            case 0:
                //GPISt1_0_ToUse = &inEGQPI->genTopSt1_0;
                //GPISt1_1_ToUse = &inEGQPI->genTopSt1_0;
                GPISt3_0_ToUse = &inEGQPI->genTopSt3_0;
                GPISt3_1_ToUse = &inEGQPI->genTopSt3_1;
                break;
            case 1:
                GPISt1_0_ToUse = &inEGQPI->genBSt1_0;
                GPISt1_1_ToUse = &inEGQPI->genBSt1_1;
                GPISt3_0_ToUse = &inEGQPI->genBSt3_0;
                GPISt3_1_ToUse = &inEGQPI->genBSt3_1;
                break;                
            default:
                break;
        }
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_PDGID"),      &GPISt3_0_ToUse->GP.PDGID );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_FirstMom"),   &GPISt3_0_ToUse->GP_FirstMom );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Index"),      &GPISt3_0_ToUse->GP_Index );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Energy"),     &GPISt3_0_ToUse->GP.BVC.Vec_En );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Pt"),         &GPISt3_0_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Eta"),        &GPISt3_0_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Phi"),        &GPISt3_0_ToUse->GP.BVC.Vec_Phi );
        
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_PDGID"),      &GPISt3_1_ToUse->GP.PDGID );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_FirstMom"),   &GPISt3_1_ToUse->GP_FirstMom );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Index"),      &GPISt3_1_ToUse->GP_Index );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Energy"),     &GPISt3_1_ToUse->GP.BVC.Vec_En );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Pt"),         &GPISt3_1_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Eta"),        &GPISt3_1_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Phi"),        &GPISt3_1_ToUse->GP.BVC.Vec_Phi );
        if (iQuark > 0) {
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_PID"),       &GPISt1_0_ToUse->GP.PDGID );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Pt"),        &GPISt1_0_ToUse->GP.BVC.Vec_Pt );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Eta"),       &GPISt1_0_ToUse->GP.BVC.Vec_Eta );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Phi"),       &GPISt1_0_ToUse->GP.BVC.Vec_Phi );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Energy"),    &GPISt1_0_ToUse->GP.BVC.Vec_En );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomPID"),    &GPISt1_0_ToUse->GP_Mom.PDGID );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomPt"),     &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Pt );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomEta"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Eta );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomPhi"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Phi );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomEnergy"), &GPISt1_0_ToUse->GP_Mom.BVC.Vec_En );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomStat"),   &GPISt1_0_ToUse->GP_Mom.PDGStatus );
            
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_PID"),       &GPISt1_1_ToUse->GP.PDGID );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Pt"),        &GPISt1_1_ToUse->GP.BVC.Vec_Pt );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Eta"),       &GPISt1_1_ToUse->GP.BVC.Vec_Eta );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Phi"),       &GPISt1_1_ToUse->GP.BVC.Vec_Phi );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Energy"),    &GPISt1_1_ToUse->GP.BVC.Vec_En );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomPID"),    &GPISt1_1_ToUse->GP_Mom.PDGID );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomPt"),     &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Pt );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomEta"),    &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Eta );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomPhi"),    &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Phi );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomEnergy"), &GPISt1_1_ToUse->GP_Mom.BVC.Vec_En );
            outTree->Branch( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomStat"),   &GPISt1_1_ToUse->GP_Mom.PDGStatus );
        }
    }
}



inline void SetOutTreeBranchSUSYInfo(TTree * outTree, EventGenSUSYParticleInfo * inEGSPI) {
    int numSUSYParts = inEGSPI->numSavedSUSYParts;
    int numIndSUSYParts = inEGSPI->numSavedIndSUSYParts;
    vector<TString> vecSUSYNames;
    vecSUSYNames.push_back("Stop");
    vecSUSYNames.push_back("Chi0");
    vecSUSYNames.push_back("Chargino");
    TString prefixSUSY = "TGen";
    TString numString;
    TString prefixBranchName;
    
    outTree->Branch(prefixSUSY + TString("StopPolWeights"),    &inEGSPI->GSEPs.genStopPolarizationWeights);
    
    for (int iSUSY = 0; iSUSY < numSUSYParts; ++iSUSY) {
        for (int iIndSUSY = 0; iIndSUSY < numIndSUSYParts; ++iIndSUSY) {
            numString = "_"; numString += iIndSUSY;
            prefixBranchName = prefixSUSY + vecSUSYNames[iSUSY];
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_Mass") + numString, &inEGSPI->vecVecGenSUSYRoundMass[iSUSY][iIndSUSY]);
            if (iSUSY == 2) {
                outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_MassFrac") + numString, &inEGSPI->vecGenCharginoRoundMassFrac[iIndSUSY]);
            }
            prefixBranchName += numString;
            inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].SetOutBranches(outTree, prefixBranchName);
            /*
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_PDGID"),           &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.PDGID );
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_FirstMom"),        &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP_FirstMom );
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Index"),           &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP_Index );
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Energy"),          &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_En );
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Pt"),              &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_Pt );
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Eta"),             &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_Eta );
            outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Phi"),             &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_Phi ); 
             */            
        }
    }
    /*
    outTree->Branch(prefixSUSY + TString("StopMass0"),         &inEGSPI->genStopMass0);
    outTree->Branch(prefixSUSY + TString("StopMass1"),         &inEGSPI->genStopMass1);
    outTree->Branch(prefixSUSY + TString("Chi0Mass0"),         &inEGSPI->genChi0Mass0);
    outTree->Branch(prefixSUSY + TString("Chi0Mass1"),         &inEGSPI->genChi0Mass1);
    outTree->Branch(prefixSUSY + TString("CharginoMass0"),     &inEGSPI->genCharginoMass0);
    outTree->Branch(prefixSUSY + TString("CharginoMass1"),     &inEGSPI->genCharginoMass1);
    outTree->Branch(prefixSUSY + TString("CharginoMassFrac0"), &inEGSPI->MassFrac_Chargino0);
    outTree->Branch(prefixSUSY + TString("CharginoMassFrac1"), &inEGSPI->MassFrac_Chargino1);
    */
    /*
    GenParticleSt3Info * GPISt3_0_ToUse = 0, * GPISt3_1_ToUse = 0;
    for (int iSUSY = 0; iSUSY < numSUSY; ++iSUSY) {
            switch (iSUSY) {
                case 0:
                    GPISt3_0_ToUse = &inEGSPI->genStop0;
                    GPISt3_1_ToUse = &inEGSPI->genStop1;
                    break;
                case 1:
                    GPISt3_0_ToUse = &inEGSPI->genChiZero0;
                    GPISt3_1_ToUse = &inEGSPI->genChiZero1;
                    break;
                case 2:
                    GPISt3_0_ToUse = &inEGSPI->genChargino0;
                    GPISt3_1_ToUse = &inEGSPI->genChargino1;
                    break;
                default:
                    break;
            }
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_PDGID"),           &GPISt3_0_ToUse->GP.PDGID );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_FirstMom"),        &GPISt3_0_ToUse->GP_FirstMom );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Index"),           &GPISt3_0_ToUse->GP_Index );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Energy"),          &GPISt3_0_ToUse->GP.BVC.Vec_En );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Pt"),              &GPISt3_0_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Eta"),             &GPISt3_0_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Phi"),             &GPISt3_0_ToUse->GP.BVC.Vec_Phi );
        
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_PDGID"),           &GPISt3_1_ToUse->GP.PDGID );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_FirstMom"),        &GPISt3_1_ToUse->GP_FirstMom );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Index"),           &GPISt3_1_ToUse->GP_Index );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Energy"),          &GPISt3_1_ToUse->GP.BVC.Vec_En );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Pt"),              &GPISt3_1_ToUse->GP.BVC.Vec_Pt );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Eta"),             &GPISt3_1_ToUse->GP.BVC.Vec_Eta );
        outTree->Branch(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Phi"),             &GPISt3_1_ToUse->GP.BVC.Vec_Phi );  
    }
    */
}

inline void SetOutTreeBranchBasicInfo(TTree * outTree, FilterTriggerInfo * inFTI, EventPileUpInfo * inEPI, BasicEventInfo * inBEI, EventGenWeight * inEGW) {
    outTree->Branch("TPassDoubleEl", &inFTI->passTrigDoubleEl);
    outTree->Branch("TPassDoubleMu", &inFTI->passTrigDoubleMu);
    outTree->Branch("TPassElMu",     &inFTI->passTrigElMu);
    
    outTree->Branch("TPUWeight",     &inEPI->nPUWeight1DTrue);
    outTree->Branch("TPUWeight1D",   &inEPI->nPUWeight1D);
    outTree->Branch("TPUWeight3D",   &inEPI->nPUWeight3D);
    outTree->Branch("T_nPUTrue ",    &inEPI->nPUTrue);        
    outTree->Branch("TNPV",          &inEPI->nVtx);
    outTree->Branch("TNPV_True",     &inEPI->nVtxTrue);
    
    outTree->Branch("TRunNum",       &inBEI->runNumber);
    outTree->Branch("TEventNum",     &inBEI->eventNumber);
    outTree->Branch("TLumiBlock",    &inBEI->lumiBlock);
    
    if (!inBEI->doData) {
        outTree->Branch("TGenWeight",            &inEGW->vecGenWeight[0]);
        outTree->Branch("TGenWeight_NoShift",    &inEGW->vecGenWeight[1]);
        outTree->Branch("TGenWeight_ExtraShift", &inEGW->vecGenWeight[2]);
    }
}

