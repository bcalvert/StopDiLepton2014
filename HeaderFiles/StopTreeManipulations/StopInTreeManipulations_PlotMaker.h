// Functions used as part of the nTuple skimming macro
// Set up the output branch address names and values for the skim output tree


inline void SetInTreeBranch_PlotMaker_PassesCutInfo(TTree * inTree, bool passesFullCut, int whichSyst = 0) {
    TString appendStringSyst = SystString(whichSyst);
    inTree->SetBranchAddress("TEventPassesFullSelection" + appendStringSyst, &passesFullCut);
}
/*
 int systLepES = 1;
 int systJetES = 2;
 int systBTagEffSF = 3;
 int systBMisTagSF = 4;
 int systJetSmear = 5;
 int systUncES = 6;
 */
inline void SetInTreeBranch_PlotMaker_LeptonInfo(TTree * inTree, EventLepInfo * inELI, int whichSyst = 0, bool isData = false, bool keepLooseLeps = false) {
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
    
    inTree->SetBranchAddress("TDoEvent" + appendStringSyst,                     &inELI->doEvent);
    inTree->SetBranchAddress("TChannel" + appendStringSyst,                     &inELI->EventDiLepType);
    inTree->SetBranchAddress("TDiLepInZ" + appendStringSyst,                    &inELI->EventDiLepinZMass);
    /*
    inTree->SetBranchAddress("TNIsoElecs_pT20" + appendStringSyst,              &inELI->ELI_LC.EventNIsoElecs_pT20);
    inTree->SetBranchAddress("TNIsoMuons_pT20" + appendStringSyst,              &inELI->ELI_LC.EventNIsoMuons_pT20);
    inTree->SetBranchAddress("TNIsoPosits_pT20" + appendStringSyst,             &inELI->ELI_LC.EventNIsoPosits_pT20);
    inTree->SetBranchAddress("TNIsoMubars_pT20" + appendStringSyst,             &inELI->ELI_LC.EventNIsoMubars_pT20);
    inTree->SetBranchAddress("TNIsoElecs_pT10to20" + appendStringSyst,          &inELI->ELI_LC.EventNIsoElecs_pT10to20);
    inTree->SetBranchAddress("TNIsoMuons_pT10to20" + appendStringSyst,          &inELI->ELI_LC.EventNIsoMuons_pT10to20);
    inTree->SetBranchAddress("TNIsoPosits_pT10to20" + appendStringSyst,         &inELI->ELI_LC.EventNIsoPosits_pT10to20);
    inTree->SetBranchAddress("TNIsoMubars_pT10to20" + appendStringSyst,         &inELI->ELI_LC.EventNIsoMubars_pT10to20);
    inTree->SetBranchAddress("TNViableLepPairsPreMassCut" + appendStringSyst,   &inELI->ELI_LC.EventNViableLepPairsPreMassCut);
    inTree->SetBranchAddress("TNViableLepPairsPostMassCut" + appendStringSyst,  &inELI->ELI_LC.EventNViableLepPairsPostMassCut);
    */
    
    inTree->SetBranchAddress("TDiLepMass" + appendStringSyst,                   &inELI->EventDiLepMass);    
    if (keepLooseLeps) {
        inTree->SetBranchAddress("TEventFakeLepWeight" + appendStringSyst,          &inELI->weightFakeLep);
    }
    for (int iLep = 0; iLep < inELI->numSavedLeps; ++iLep) {
        numString = ""; numString += iLep;
        inTree->SetBranchAddress(lepString + numString + TString("Pt") + appendStringSyst,                      &inELI->vecEventLeps[iLep].BVC.Vec_Pt); 
        inTree->SetBranchAddress(lepString + numString + TString("Eta") + appendStringSyst,                     &inELI->vecEventLeps[iLep].BVC.Vec_Eta); 
        inTree->SetBranchAddress(lepString + numString + TString("Phi") + appendStringSyst,                     &inELI->vecEventLeps[iLep].BVC.Vec_Phi); 
        inTree->SetBranchAddress(lepString + numString + TString("E") + appendStringSyst,                       &inELI->vecEventLeps[iLep].BVC.Vec_En);
        inTree->SetBranchAddress(lepString + numString + TString("PDGID") + appendStringSyst,                   &inELI->vecEventLeps[iLep].PDGID);
        inTree->SetBranchAddress(lepString + numString + TString("RelPFIso") + appendStringSyst,                &inELI->vecEventLeps[iLep].relPFLepIso);
        
        if (keepLooseLeps) {
            inTree->SetBranchAddress(lepString + numString + TString("LepQual") + appendStringSyst,             &inELI->vecEventLeps[iLep].lepQuality);
        }
        
        if (!isData) {
            inTree->SetBranchAddress(lepString + numString + TString("GenMatched") + appendStringSyst,      &inELI->vecEventLeps[iLep].isGenMatched);
            inTree->SetBranchAddress(lepString + numString + TString("GM_PromptLepton") + appendStringSyst, &inELI->vecEventLeps[iLep].isPromptLepton);
            inTree->SetBranchAddress(lepString + numString + TString("GM_Pt") + appendStringSyst,           &inELI->vecEventLeps[iLep].GP.BVC.Vec_Pt);
            inTree->SetBranchAddress(lepString + numString + TString("GM_Eta") + appendStringSyst,          &inELI->vecEventLeps[iLep].GP.BVC.Vec_Eta);
            inTree->SetBranchAddress(lepString + numString + TString("GM_Phi") + appendStringSyst,          &inELI->vecEventLeps[iLep].GP.BVC.Vec_Phi);
            inTree->SetBranchAddress(lepString + numString + TString("GM_E") + appendStringSyst,            &inELI->vecEventLeps[iLep].GP.BVC.Vec_En);
            inTree->SetBranchAddress(lepString + numString + TString("GM_PDGID") + appendStringSyst,        &inELI->vecEventLeps[iLep].PDGID);
            inTree->SetBranchAddress(lepString + numString + TString("GM_MomPDGID") + appendStringSyst,     &inELI->vecEventLeps[iLep].GP.MomPDGID);
            inTree->SetBranchAddress(lepString + numString + TString("GM_MomStatus") + appendStringSyst,    &inELI->vecEventLeps[iLep].GP.MomPDGStatus);
        }
    }
}

inline void SetInTreeBranch_PlotMaker_JetInfo(TTree * inTree, EventJetInfo * inEJI, int whichSyst = 0, bool isSmear = false) {
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
    
    inTree->SetBranchAddress(TString("TN") + stringSmear + TString("Jets") + appendStringSyst,           &inEJI->EventNJets);   
    inTree->SetBranchAddress(TString("THT") + stringSmear + appendStringSyst,                            &inEJI->EventHT);
    inTree->SetBranchAddress(TString("THT_AllJets") + stringSmear + appendStringSyst,                    &inEJI->EventHT_AllJets);
    inTree->SetBranchAddress(TString("THTPar") + stringSmear + appendStringSyst,                         &inEJI->EventHTPar);
    inTree->SetBranchAddress(TString("THTPerp") + stringSmear + appendStringSyst,                        &inEJI->EventHTPerp);
    inTree->SetBranchAddress(TString("THadResParGauss") + stringSmear + appendStringSyst,                &inEJI->EventHadResParGauss);
    inTree->SetBranchAddress(TString("THadResParGauss_AllJets") + stringSmear + appendStringSyst,        &inEJI->EventHadResParGauss_AllJets);
    inTree->SetBranchAddress(TString("THadResParFull") + stringSmear + appendStringSyst,                 &inEJI->EventHadResParFull);
    inTree->SetBranchAddress(TString("THadResPerpGauss") + stringSmear + appendStringSyst,               &inEJI->EventHadResPerpGauss);
    inTree->SetBranchAddress(TString("THadResPerpGauss_AllJets") + stringSmear + appendStringSyst,       &inEJI->EventHadResPerpGauss_AllJets);
    inTree->SetBranchAddress(TString("THadResPerpFull") + stringSmear + appendStringSyst,                &inEJI->EventHadResPerpFull);
    inTree->SetBranchAddress(TString("THadVarParFull") + stringSmear + appendStringSyst,                 &inEJI->EventHadVarParFull);
    inTree->SetBranchAddress(TString("THadVarPerpFull") + stringSmear + appendStringSyst,                &inEJI->EventHadVarPerpFull);
    inTree->SetBranchAddress(TString("THadCoVarFull") + stringSmear + appendStringSyst,                  &inEJI->EventHadCoVarFull);
    inTree->SetBranchAddress(TString("TSumJetPt") + stringSmear + appendStringSyst,                      &inEJI->BVC_SumJet.Vec_Pt);
    inTree->SetBranchAddress(TString("TSumJetPhi") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumJet.Vec_Phi);
    inTree->SetBranchAddress(TString("TSumJetEta") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumJet.Vec_Eta);
    inTree->SetBranchAddress(TString("TSumJetEn") + stringSmear + appendStringSyst,                      &inEJI->BVC_SumJet.Vec_En);
    /*
    inTree->SetBranchAddress(TString("TSumBJetPt") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumBJet.Vec_Pt);
    inTree->SetBranchAddress(TString("TSumBJetPhi") + stringSmear + appendStringSyst,                    &inEJI->BVC_SumBJet.Vec_Phi);
    inTree->SetBranchAddress(TString("TSumBJetEta") + stringSmear + appendStringSyst,                    &inEJI->BVC_SumBJet.Vec_Eta);
    inTree->SetBranchAddress(TString("TSumBJetEn") + stringSmear + appendStringSyst,                     &inEJI->BVC_SumBJet.Vec_En);
    */
    if (whichSyst != systBMisTagSF && whichSyst != systBTagEffSF) {
        for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
            numString = "";
            numString += iJet;
            inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("Pt") + appendStringSyst,          &inEJI->vecEventJets[iJet].BVC.Vec_Pt); 
            inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("Eta") + appendStringSyst,         &inEJI->vecEventJets[iJet].BVC.Vec_Eta); 
            inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("Phi") + appendStringSyst,         &inEJI->vecEventJets[iJet].BVC.Vec_Phi); 
            inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("En") + appendStringSyst,          &inEJI->vecEventJets[iJet].BVC.Vec_En); 
            inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("BTagDisc") + appendStringSyst,    &inEJI->vecEventJets[iJet].valBTagDisc);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("PartonFlav") + appendStringSyst,  &inEJI->vecEventJets[iJet].partonFlavor);
            if (isSmear) {
                inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("isGenMatched") + appendStringSyst,        &inEJI->vecEventJets[iJet].isGenJetMatched);
                inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("DeltaEnRecoGenMatch") + appendStringSyst,        &inEJI->vecEventJets[iJet].dEnRecoGen);
            }        
        }
    }
    inTree->SetBranchAddress(TString("TN") + stringSmear + TString("JetsBtag") + appendStringSyst,       &inEJI->EventNBtagJets);
    
    for (int iJet = 0; iJet < inEJI->numSavedJets; ++iJet) {
        numString = "";
        numString += iJet;
        inTree->SetBranchAddress(TString("T") + stringSmear + stringJet + numString + TString("isBTagged") + appendStringSyst,   &inEJI->vecEventJets[iJet].isBJet);
    }
    
    for (int iBJet = 0; iBJet < inEJI->numSavedBJets; ++iBJet) {
        numString = "";
        numString += iBJet;
        inTree->SetBranchAddress(TString("T") + stringSmear + stringBJet + numString + TString("Pt") + appendStringSyst,          &inEJI->vecEventBTagJets[iBJet].BVC.Vec_Pt); 
        inTree->SetBranchAddress(TString("T") + stringSmear + stringBJet + numString + TString("Eta") + appendStringSyst,         &inEJI->vecEventBTagJets[iBJet].BVC.Vec_Eta); 
        inTree->SetBranchAddress(TString("T") + stringSmear + stringBJet + numString + TString("Phi") + appendStringSyst,         &inEJI->vecEventBTagJets[iBJet].BVC.Vec_Phi); 
        inTree->SetBranchAddress(TString("T") + stringSmear + stringBJet + numString + TString("En") + appendStringSyst,          &inEJI->vecEventBTagJets[iBJet].BVC.Vec_En);
        inTree->SetBranchAddress(TString("T") + stringSmear + stringBJet + numString + TString("Index") + appendStringSyst,       &inEJI->vecEventBTagJets_Index[iBJet]);
    } 
}

inline void SetInTreeBranch_PlotMaker_RazorInfo(TTree * inTree, EventRazorInfo * inERazI, int whichSyst = 0, bool isSmear = false, TString appendString = "") {
    TString appendStringSyst = SystString(whichSyst);
    TString noPhiCorrString = "";
    TString stringSmear = isSmear ? "Smear" : "";
    TString stringMETType[5] = {"PF", "", "", "", "Calo"};
    
    int whichMET = 0;
    
    noPhiCorrString += appendString;
    
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("SHat_Razor") + appendStringSyst + noPhiCorrString, &inERazI->SHat_Razor);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("gamma_Razor") + appendStringSyst + noPhiCorrString, &inERazI->gamma_Razor);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("gamma_Top") + appendStringSyst + noPhiCorrString, &inERazI->gamma_Top);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("gamma_WLep0") + appendStringSyst + noPhiCorrString, &inERazI->gamma_W0);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("gamma_WLep1") + appendStringSyst + noPhiCorrString, &inERazI->gamma_W1);
    
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("MR1") + appendStringSyst + noPhiCorrString, &inERazI->mass_Razor1);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("MR2") + appendStringSyst + noPhiCorrString, &inERazI->mass_Razor2);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("MR_Top") + appendStringSyst + noPhiCorrString, &inERazI->mass_RazorTop);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("MR_WLep0") + appendStringSyst + noPhiCorrString, &inERazI->mass_RazorW0);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("MR_WLep1") + appendStringSyst + noPhiCorrString, &inERazI->mass_RazorW1);
    
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("dPhiBLeps_Razor") + appendStringSyst + noPhiCorrString, &inERazI->dPhiBLeps_Razor);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("dPhiLeps_Razor") + appendStringSyst + noPhiCorrString, &inERazI->dPhiLeps_Razor);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("cosThetaLep0_Top") + appendStringSyst + noPhiCorrString, &inERazI->cosThetaLep0_Top);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("cosThetaLep0_W") + appendStringSyst + noPhiCorrString, &inERazI->cosThetaLep0_W);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("cosThetaLep1_Top") + appendStringSyst + noPhiCorrString, &inERazI->cosThetaLep1_Top);
    inTree->SetBranchAddress(TString("TRazor_") + stringSmear + stringMETType[whichMET] + TString("cosThetaLep1_W") + appendStringSyst + noPhiCorrString, &inERazI->cosThetaLep1_W);
}


inline void SetInTreeBranch_PlotMaker_METInfo(TTree * inTree, EventMETInfo * inEMI, int whichSyst = 0, bool isSmear = false, bool isPhiCorr = true, TString appendString = "") {
    TString appendStringSyst = SystString(whichSyst);
    TString noPhiCorrString = isPhiCorr ? "" : "_noPhiCorr";
    TString stringSmear = isSmear ? "Smear" : "";
    TString stringMETType[5] = {"PF", "", "", "", "Calo"};
    
    noPhiCorrString += appendString;
    
    TString stringCorrPairBase = "MT2lblb_CorrPairing_";
    TString stringCorrPair;
    
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET") + appendStringSyst + noPhiCorrString,         &inEMI->EventMET); 
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETPhi);
    //    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_Sig") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETSig);
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_SumET") + appendStringSyst + noPhiCorrString,     &inEMI->EventSumET);
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MET_SigTrue") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETSigTrue);
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2ll") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll);  
    if (whichSyst != systBTagEffSF && whichSyst != systBMisTagSF) {
        if (isPhiCorr) {
            /*
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT_Lep0") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep0);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT_Lep1") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep1);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT_HTVec") + appendStringSyst + noPhiCorrString,    &inEMI->MET_EMT2I.MT_HTVec);
            
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("METdivMeff") + appendStringSyst + noPhiCorrString,  &inEMI->EventMETdivMeff);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("METdivHT") + appendStringSyst + noPhiCorrString,    &inEMI->EventMETdivHT);
            */
            
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("UPar") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPar);        
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("UPerp") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPerp);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2lblb") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2lblb);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2lblb_ZeroBLepMass") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2lblb_ZeroBLepMass);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("DPhiMT2lblb_JetsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lblb_JetsUsed);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("DPhiMT2lblb_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lblb_BLepsUsed);            
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MassBLep0_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMassBLep0_BLepsUsed);
            inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MassBLep1_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMassBLep1_BLepsUsed);
        }
    }
    bool saveCorrPairInfo = false;
    if (isPhiCorr) {
        inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2lblb_ToT") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.caseMT2lblb);
        //        cout << "size " << inEMI->MET_EMT2I.vecCorrPairValMT2lblb.size() << endl;
        
        if (saveCorrPairInfo) {
            for (int iCorrPair = 0; iCorrPair < (int) inEMI->MET_EMT2I.vecCorrPairValMT2lblb.size(); ++iCorrPair) {
                stringCorrPair = stringCorrPairBase;
                stringCorrPair += iCorrPair;
                inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + stringCorrPair + appendStringSyst + noPhiCorrString, &inEMI->MET_EMT2I.vecCorrPairValMT2lblb[iCorrPair]);
            }
        }
    }
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2bb_ZMET") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2bb_ZMET);
    //inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("MT2bb") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2bb);
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("DPhiMT2bb_ZMET_JetsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2bb_ZMET_JetsUsed);
    /*
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("BMET") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMET);
    inTree->SetBranchAddress(TString("T") + stringSmear + stringMETType[inEMI->METType] + TString("BMET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMETPhi);
    */
}

inline void SetInTreeBranch_PlotMaker_SpecialMETInfo(TTree * inTree, EventMETInfo * inEMI, int whichSyst = 0, bool isSmear = false, bool isPhiCorr = true, TString appendString = "") {
    TString appendStringSyst = SystString(whichSyst);
    TString noPhiCorrString = isPhiCorr ? "" : "_noPhiCorr";
    noPhiCorrString += "_v2";
    noPhiCorrString += appendString;
    
    TString stringCorrPairBase = "MT2lblb_CorrPairing_";
    TString stringCorrPair;
    
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    
    TString stringSmear = isSmear ? "Smear" : "";
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MET") + appendStringSyst + noPhiCorrString,         &inEMI->EventMET); 
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETPhi);
    //    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MET_Sig") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETSig);
    //    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MET_SumET") + appendStringSyst + noPhiCorrString,     &inEMI->EventSumET);
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MET_SigTrue") + appendStringSyst + noPhiCorrString,     &inEMI->EventMETSigTrue);
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2ll") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll);
    if (whichSyst != systBTagEffSF && whichSyst != systBMisTagSF) {
        if (isPhiCorr) {
            /*
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT_Lep0") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep0);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT_Lep1") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.MT_Lep1);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT_HTVec") + appendStringSyst + noPhiCorrString,    &inEMI->MET_EMT2I.MT_HTVec);
            
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("METdivMeff") + appendStringSyst + noPhiCorrString,  &inEMI->EventMETdivMeff);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("METdivHT") + appendStringSyst + noPhiCorrString,    &inEMI->EventMETdivHT);
            */
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("UPar") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPar);        
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("UPerp") + appendStringSyst + noPhiCorrString,    &inEMI->MET_ERI.recoilUPerp);
            
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2lblb") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2lblb);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2lblb_ZeroBLepMass") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2lblb_ZeroBLepMass);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("DPhiMT2lblb_JetsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lblb_JetsUsed);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("DPhiMT2lblb_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2lblb_BLepsUsed);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MassBLep0_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMassBLep0_BLepsUsed);
            inTree->SetBranchAddress(TString("T") + stringSmear + TString("MassBLep1_BLepsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMassBLep1_BLepsUsed);
        }
    }
    bool saveCorrPairInfo = false;
    if (isPhiCorr) {
//        cout << "size: " << inEMI->MET_EMT2I.vecCorrPairValMT2lblb.size() << endl;
        inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2lblb_ToT") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.caseMT2lblb); 
        
        if (saveCorrPairInfo) {
            for (int iCorrPair = 0; iCorrPair < (int) inEMI->MET_EMT2I.vecCorrPairValMT2lblb.size(); ++iCorrPair) {
                stringCorrPair = stringCorrPairBase;
                stringCorrPair += iCorrPair;
                inTree->SetBranchAddress(TString("T") + stringSmear + stringCorrPair + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.vecCorrPairValMT2lblb[iCorrPair]);
            }
        }
    }
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2bb_ZMET") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2bb_ZMET);

     //inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2bb") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2bb);
     
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("DPhiMT2bb_ZMET_JetsUsed") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventDeltaPhiMT2bb_ZMET_JetsUsed);
    /*
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("BMET") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMET);
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("BMET_Phi") + appendStringSyst + noPhiCorrString,     &inEMI->EventBMETPhi);
    inTree->SetBranchAddress(TString("T") + stringSmear + TString("MT2ll_BMET") + appendStringSyst + noPhiCorrString,     &inEMI->MET_EMT2I.EventMT2ll_BMET);
    */
}

inline void SetInTreeBranch_PlotMaker_DiStructureInfo(TTree * inTree, EventDiStructureInfo * inEDSI, int whichSyst = 0, bool isSmear = false, bool isPhiCorr = true) {
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
    int systUncESHack = 7;
    
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
        
        SetDiStructureBranchAddress(inTree, DP_ToUse, TString("T") + stringSmear + DP_ToUse->Name, appendStringSyst, true);
        /*
        inTree->SetBranchAddress(TString("T") + stringSmear + DP_ToUse->Name + TString("Pt")   + appendStringSyst, &DP_ToUse->BVC.Vec_Pt);
        inTree->SetBranchAddress(TString("T") + stringSmear + DP_ToUse->Name + TString("Eta")  + appendStringSyst, &DP_ToUse->BVC.Vec_Eta);
        inTree->SetBranchAddress(TString("T") + stringSmear + DP_ToUse->Name + TString("Mass") + appendStringSyst, &DP_ToUse->BVC.Vec_Mass);
        inTree->SetBranchAddress(TString("T") + stringSmear + DP_ToUse->Name + TString("Phi")  + appendStringSyst, &DP_ToUse->BVC.Vec_Phi);
        inTree->SetBranchAddress(TString("T") + stringSmear + DP_ToUse->Name + TString("_IndObjDPhi")  + appendStringSyst, &DP_ToUse->DPhiInputObjects);
         */
    }
    stringSmear = isSmear ? "Smear" : "";
    if (whichSyst != systBMisTagSF && whichSyst != systBTagEffSF) {
        SetDiStructureBranchAddress(inTree, &inEDSI->DP_ZMET, TString("T") + stringSmear + inEDSI->DP_ZMET.Name, appendStringSyst, false);
        SetDiStructureBranchAddress(inTree, &inEDSI->DP_DiBLeps, TString("T") + stringSmear + inEDSI->DP_DiBLeps.Name, appendStringSyst, false);
    }
    
    for (int iLep = 0; iLep < inEDSI->numLeps; ++iLep) {
        if (whichSyst != systBMisTagSF && whichSyst != systBTagEffSF) {
            SetDiStructureBranchAddress(inTree, &inEDSI->vecDP_LepMET[iLep], TString("T") + stringSmear + inEDSI->vecDP_LepMET[iLep].Name, appendStringSyst, false);
            if (whichSyst != systUncES && whichSyst != systUncESHack) {
                for (int iJet = 0; iJet < inEDSI->numJets; ++iJet) {
                    SetDiStructureBranchAddress(inTree, &inEDSI->vecVecDP_LepJet[iLep][iJet], TString("T") + stringSmear + inEDSI->vecVecDP_LepJet[iLep][iJet].Name, appendStringSyst, false);
                }
            }
        }
        if (whichSyst != systUncES && whichSyst != systUncESHack) {
            for (int iBJet = 0; iBJet < inEDSI->numBJets; ++iBJet) {
                SetDiStructureBranchAddress(inTree, &inEDSI->vecVecDP_LepBJet[iLep][iBJet], TString("T") + stringSmear + inEDSI->vecVecDP_LepBJet[iLep][iBJet].Name, appendStringSyst, false);
            }
        }
    }     
    if (whichSyst != systLepES && whichSyst != systUncES && whichSyst != systUncESHack) {
        for (int iJet = 0; iJet < inEDSI->numJets; ++iJet) {
            for (int iBJet = 0; iBJet < inEDSI->numBJets; ++iBJet) {
                SetDiStructureBranchAddress(inTree, &inEDSI->vecVecDP_JetBJet[iJet][iBJet], TString("T") + stringSmear + inEDSI->vecVecDP_JetBJet[iJet][iBJet].Name, appendStringSyst, false);
            }
        }
    }
}

inline void SetInTreeBranch_PlotMaker_GenLepton(TTree * inTree, EventGenLeptonParticleInfo * inEGLPI) {

    TString prefixGenLep = "TGen";
    TString prefixBranchName;
    for (int iLepSt3 = 0; iLepSt3 < inEGLPI->numSt3Leps; ++iLepSt3) {
        for (int iIndLep = 0; iIndLep < inEGLPI->numIndLeps; ++iIndLep) {
            prefixBranchName = prefixGenLep + inEGLPI->nameLeps[iLepSt3] + TString("_St3_");
            prefixBranchName += iIndLep;
            inEGLPI->vecVecGenLepPSt3I[iLepSt3][iIndLep].SetInBranches(inTree, prefixBranchName);            
        }
    }
    for (int iLepSt1 = 0; iLepSt1 < inEGLPI->numSt1Leps; ++iLepSt1) {
        for (int iIndLep = 0; iIndLep < inEGLPI->numIndLeps; ++iIndLep) {
            prefixBranchName = prefixGenLep + inEGLPI->nameLeps[iLepSt1] + TString("_St1_");
            prefixBranchName += iIndLep;
            inEGLPI->vecVecGenLepPSt1I[iLepSt1][iIndLep].SetInBranches(inTree, prefixBranchName);            
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
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_PDGID"),      &GPISt3_0_ToUse->GP.PDGID );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_FirstMom"),   &GPISt3_0_ToUse->GP_FirstMom );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_Index"),      &GPISt3_0_ToUse->GP_Index );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_Energy"),     &GPISt3_0_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_Pt"),         &GPISt3_0_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_Eta"),        &GPISt3_0_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_0_Phi"),        &GPISt3_0_ToUse->GP.BVC.Vec_Phi );
        
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_PDGID"),      &GPISt3_1_ToUse->GP.PDGID );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_FirstMom"),   &GPISt3_1_ToUse->GP_FirstMom );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_Index"),      &GPISt3_1_ToUse->GP_Index );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_Energy"),     &GPISt3_1_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_Pt"),         &GPISt3_1_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_Eta"),        &GPISt3_1_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress(prefixGenLep + nameLep[iLep] + TString("St3_1_Phi"),        &GPISt3_1_ToUse->GP.BVC.Vec_Phi );
        
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_PID"),       &GPISt1_0_ToUse->GP.PDGID );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_Pt"),        &GPISt1_0_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_Eta"),       &GPISt1_0_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_Phi"),       &GPISt1_0_ToUse->GP.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_Energy"),    &GPISt1_0_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_MomPID"),    &GPISt1_0_ToUse->GP_Mom.PDGID );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_MomPt"),     &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_MomEta"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_MomPhi"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_MomEnergy"), &GPISt1_0_ToUse->GP_Mom.BVC.Vec_En );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_0_MomStat"),   &GPISt1_0_ToUse->GP_Mom.PDGStatus );
        
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_PID"),       &GPISt1_1_ToUse->GP.PDGID );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_Pt"),        &GPISt1_1_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_Eta"),       &GPISt1_1_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_Phi"),       &GPISt1_1_ToUse->GP.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_Energy"),    &GPISt1_1_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_MomPID"),    &GPISt1_1_ToUse->GP_Mom.PDGID );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_MomPt"),     &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Pt );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_MomEta"),    &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Eta );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_MomPhi"),     &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Phi );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_MomEnergy"), &GPISt1_1_ToUse->GP_Mom.BVC.Vec_En );
        inTree->SetBranchAddress( prefixGenLep + nameLep[iLep] + TString("St1_1_MomStat"),   &GPISt1_1_ToUse->GP_Mom.PDGStatus );
        
    }
     */
}



inline void SetInTreeBranch_PlotMaker_GenQuark(TTree * inTree, EventGenQuarkParticleInfo * inEGQPI) {
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
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_PDGID"),      &GPISt3_0_ToUse->GP.PDGID );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_FirstMom"),   &GPISt3_0_ToUse->GP_FirstMom );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Index"),      &GPISt3_0_ToUse->GP_Index );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Energy"),     &GPISt3_0_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Pt"),         &GPISt3_0_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Eta"),        &GPISt3_0_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_0_Phi"),        &GPISt3_0_ToUse->GP.BVC.Vec_Phi );
        
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_PDGID"),      &GPISt3_1_ToUse->GP.PDGID );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_FirstMom"),   &GPISt3_1_ToUse->GP_FirstMom );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Index"),      &GPISt3_1_ToUse->GP_Index );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Energy"),     &GPISt3_1_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Pt"),         &GPISt3_1_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Eta"),        &GPISt3_1_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress(prefixGenQuark + nameQuark[iQuark] + TString("St3_1_Phi"),        &GPISt3_1_ToUse->GP.BVC.Vec_Phi );
        if (iQuark > 0) {
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_PID"),       &GPISt1_0_ToUse->GP.PDGID );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Pt"),        &GPISt1_0_ToUse->GP.BVC.Vec_Pt );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Eta"),       &GPISt1_0_ToUse->GP.BVC.Vec_Eta );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Phi"),       &GPISt1_0_ToUse->GP.BVC.Vec_Phi );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_Energy"),    &GPISt1_0_ToUse->GP.BVC.Vec_En );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomPID"),    &GPISt1_0_ToUse->GP_Mom.PDGID );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomPt"),     &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Pt );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomEta"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Eta );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomPhi"),    &GPISt1_0_ToUse->GP_Mom.BVC.Vec_Phi );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomEnergy"), &GPISt1_0_ToUse->GP_Mom.BVC.Vec_En );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_0_MomStat"),   &GPISt1_0_ToUse->GP_Mom.PDGStatus );
            
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_PID"),       &GPISt1_1_ToUse->GP.PDGID );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Pt"),        &GPISt1_1_ToUse->GP.BVC.Vec_Pt );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Eta"),       &GPISt1_1_ToUse->GP.BVC.Vec_Eta );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Phi"),       &GPISt1_1_ToUse->GP.BVC.Vec_Phi );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_Energy"),    &GPISt1_1_ToUse->GP.BVC.Vec_En );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomPID"),    &GPISt1_1_ToUse->GP_Mom.PDGID );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomPt"),     &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Pt );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomEta"),    &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Eta );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomPhi"),    &GPISt1_1_ToUse->GP_Mom.BVC.Vec_Phi );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomEnergy"), &GPISt1_1_ToUse->GP_Mom.BVC.Vec_En );
            inTree->SetBranchAddress( prefixGenQuark + nameQuark[iQuark] + TString("St1_1_MomStat"),   &GPISt1_1_ToUse->GP_Mom.PDGStatus );
        }
    }
}



inline void SetInTreeBranch_PlotMaker_SUSYInfo(TTree * inTree, EventGenSUSYParticleInfo * inEGSPI) {
    int numSUSYParts = inEGSPI->numSavedSUSYParts;
    int numIndSUSYParts = inEGSPI->numSavedIndSUSYParts;
    vector<TString> vecSUSYNames;
    vecSUSYNames.push_back("Stop");
    vecSUSYNames.push_back("Chi0");
    vecSUSYNames.push_back("Chargino");
    TString prefixSUSY = "TGen";
    TString numString;
    TString prefixBranchName;
    
    inTree->SetBranchAddress(prefixSUSY + TString("StopPolWeights"),    &inEGSPI->GSEPs.genStopPolarizationWeights);
    
    for (int iSUSY = 0; iSUSY < numSUSYParts; ++iSUSY) {
        for (int iIndSUSY = 0; iIndSUSY < numIndSUSYParts; ++iIndSUSY) {
            numString = "_"; numString += iIndSUSY;
            prefixBranchName = prefixSUSY + vecSUSYNames[iSUSY];
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_Mass") + numString, &inEGSPI->vecVecGenSUSYRoundMass[iSUSY][iIndSUSY]);
            if (iSUSY == 2) {
                inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_MassFrac") + numString, &inEGSPI->vecGenCharginoRoundMassFrac[iIndSUSY]);
            }
            prefixBranchName += numString;
            inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].SetInBranches(inTree, prefixBranchName);
            /*
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_PDGID"),           &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.PDGID );
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_FirstMom"),        &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP_FirstMom );
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Index"),           &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP_Index );
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Energy"),          &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_En );
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Pt"),              &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_Pt );
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Eta"),             &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_Eta );
            inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + numString + TString("_Phi"),             &inEGSPI->vecVecGenSUSYPSt3I[iSUSY][iIndSUSY].GP.BVC.Vec_Phi ); 
             */            
        }
    }
    /*
    inTree->SetBranchAddress(prefixSUSY + TString("StopMass0"),         &inEGSPI->genStopMass0);
    inTree->SetBranchAddress(prefixSUSY + TString("StopMass1"),         &inEGSPI->genStopMass1);
    inTree->SetBranchAddress(prefixSUSY + TString("Chi0Mass0"),         &inEGSPI->genChi0Mass0);
    inTree->SetBranchAddress(prefixSUSY + TString("Chi0Mass1"),         &inEGSPI->genChi0Mass1);
    inTree->SetBranchAddress(prefixSUSY + TString("CharginoMass0"),     &inEGSPI->genCharginoMass0);
    inTree->SetBranchAddress(prefixSUSY + TString("CharginoMass1"),     &inEGSPI->genCharginoMass1);
    inTree->SetBranchAddress(prefixSUSY + TString("CharginoMassFrac0"), &inEGSPI->MassFrac_Chargino0);
    inTree->SetBranchAddress(prefixSUSY + TString("CharginoMassFrac1"), &inEGSPI->MassFrac_Chargino1);
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
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_PDGID"),           &GPISt3_0_ToUse->GP.PDGID );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_FirstMom"),        &GPISt3_0_ToUse->GP_FirstMom );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Index"),           &GPISt3_0_ToUse->GP_Index );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Energy"),          &GPISt3_0_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Pt"),              &GPISt3_0_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Eta"),             &GPISt3_0_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_0_Phi"),             &GPISt3_0_ToUse->GP.BVC.Vec_Phi );
        
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_PDGID"),           &GPISt3_1_ToUse->GP.PDGID );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_FirstMom"),        &GPISt3_1_ToUse->GP_FirstMom );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Index"),           &GPISt3_1_ToUse->GP_Index );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Energy"),          &GPISt3_1_ToUse->GP.BVC.Vec_En );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Pt"),              &GPISt3_1_ToUse->GP.BVC.Vec_Pt );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Eta"),             &GPISt3_1_ToUse->GP.BVC.Vec_Eta );
        inTree->SetBranchAddress(prefixSUSY + vecSUSYNames[iSUSY] + TString("_1_Phi"),             &GPISt3_1_ToUse->GP.BVC.Vec_Phi );  
    }
    */
}


inline void SetInTreeBranch_PlotMaker_GenMT2Info(TTree * inTree, EventGenMT2Info * inEGMT2I) {
    TString prefixGenMT2 = "TGen";
    TString stringCorr[2] = {"_IncorrPair", "_CorrPair"};
    TString stringPair[2] = {"_0", "_1"};
    inTree->SetBranchAddress(prefixGenMT2 + TString("MT2ll"), &inEGMT2I->genMT2ll);
    /*
    inTree->SetBranchAddress(prefixGenMT2 + TString("MT2bb"), &inEGMT2I->genMT2bb);
    inTree->SetBranchAddress(prefixGenMT2 + TString("MT2bb_ZMET"), &inEGMT2I->genMT2bb_ZMET);
    */
    inTree->SetBranchAddress(prefixGenMT2 + TString("TotBLepMass"), &inEGMT2I->TotBLepMass);
    for (int iCorr = 0; iCorr < 2; ++iCorr) {
        inTree->SetBranchAddress(prefixGenMT2 + TString("MT2lblb") + stringCorr[iCorr], &inEGMT2I->vecGenMT2lblb[iCorr]);
        inTree->SetBranchAddress(prefixGenMT2 + TString("DPhi_BLeps") + stringCorr[iCorr], &inEGMT2I->vecDPhiGenBLeps[iCorr]);
        for (int iPair = 0; iPair < 2; ++iPair) {
            inTree->SetBranchAddress(prefixGenMT2 + TString("BLepMass") + stringCorr[iCorr] + stringPair[iPair], &inEGMT2I->vecVecGenBLepMass[iCorr][iPair]);
            inTree->SetBranchAddress(prefixGenMT2 + TString("BLepDPhi") + stringCorr[iCorr] + stringPair[iPair], &inEGMT2I->vecVecGenBLepDPhi[iCorr][iPair]);
            inTree->SetBranchAddress(prefixGenMT2 + TString("BLepDEta") + stringCorr[iCorr] + stringPair[iPair], &inEGMT2I->vecVecGenBLepDEta[iCorr][iPair]);
        }
    }
}


inline void SetInTreeBranch_PlotMaker_BasicInfo(TTree * inTree, FilterTriggerInfo * inFTI, EventPileUpInfo * inEPI, BasicEventInfo * inBEI, EventGenWeight * inEGW) {
    inTree->SetBranchAddress("TPassDoubleEl", &inFTI->passTrigDoubleEl);
    inTree->SetBranchAddress("TPassDoubleMu", &inFTI->passTrigDoubleMu);
    inTree->SetBranchAddress("TPassElMu",     &inFTI->passTrigElMu);
    
    inTree->SetBranchAddress("TPUWeight",     &inEPI->nPUWeight1DTrue);
    inTree->SetBranchAddress("TPUWeight1D",   &inEPI->nPUWeight1D);
    inTree->SetBranchAddress("TPUWeight3D",   &inEPI->nPUWeight3D);
    inTree->SetBranchAddress("T_nPUTrue ",    &inEPI->nPUTrue);        
    inTree->SetBranchAddress("TNPV",          &inEPI->nVtx);
    inTree->SetBranchAddress("TNPV_True",     &inEPI->nVtxTrue);
    
    inTree->SetBranchAddress("TRunNum",       &inBEI->runNumber);
    inTree->SetBranchAddress("TEventNum",     &inBEI->eventNumber);
    inTree->SetBranchAddress("TLumiBlock",    &inBEI->lumiBlock);
    
    if (!inBEI->doData) {
        inTree->SetBranchAddress("TWeightMC",             &inBEI->weightMC);
        inTree->SetBranchAddress("TGenWeight",            &inEGW->vecGenWeight[0]);
        inTree->SetBranchAddress("TGenWeight_NoShift",    &inEGW->vecGenWeight[1]);
        inTree->SetBranchAddress("TGenWeight_ExtraShift", &inEGW->vecGenWeight[2]);
    }
}

