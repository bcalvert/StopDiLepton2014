// hack set of functions to correct not saving redundant information in the skimming process 
// (for example, I don't bother saving the lead jet pT for BTag SF systematic shifts because it won't change)
void CorrectEJI(EventJetInfo * inEJICV, EventJetInfo * inEJIShiftUp, EventJetInfo * inEJIShiftDown, int whichSyst = 0, bool isSmear = false) {
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    
    if (whichSyst == systBMisTagSF || whichSyst == systBTagEffSF) {
        for (int iJet = 0; iJet < inEJICV->numSavedJets; ++iJet) {
            inEJIShiftUp->vecEventJets[iJet].BVC.Vec_Pt = inEJICV->vecEventJets[iJet].BVC.Vec_Pt;
            inEJIShiftUp->vecEventJets[iJet].BVC.Vec_Eta = inEJICV->vecEventJets[iJet].BVC.Vec_Eta;
            inEJIShiftUp->vecEventJets[iJet].BVC.Vec_Phi = inEJICV->vecEventJets[iJet].BVC.Vec_Phi;
            inEJIShiftUp->vecEventJets[iJet].BVC.Vec_En = inEJICV->vecEventJets[iJet].BVC.Vec_En;
            inEJIShiftUp->vecEventJets[iJet].valBTagDisc = inEJICV->vecEventJets[iJet].valBTagDisc;
            
            inEJIShiftDown->vecEventJets[iJet].BVC.Vec_Pt = inEJICV->vecEventJets[iJet].BVC.Vec_Pt;
            inEJIShiftDown->vecEventJets[iJet].BVC.Vec_Eta = inEJICV->vecEventJets[iJet].BVC.Vec_Eta;
            inEJIShiftDown->vecEventJets[iJet].BVC.Vec_Phi = inEJICV->vecEventJets[iJet].BVC.Vec_Phi;
            inEJIShiftDown->vecEventJets[iJet].BVC.Vec_En = inEJICV->vecEventJets[iJet].BVC.Vec_En;
            inEJIShiftDown->vecEventJets[iJet].valBTagDisc = inEJICV->vecEventJets[iJet].valBTagDisc;
            
            if (isSmear) {
                inEJIShiftUp->vecEventJets[iJet].isGenJetMatched = inEJICV->vecEventJets[iJet].isGenJetMatched;
                inEJIShiftUp->vecEventJets[iJet].dEnRecoGen = inEJICV->vecEventJets[iJet].dEnRecoGen;
                
                inEJIShiftDown->vecEventJets[iJet].isGenJetMatched = inEJICV->vecEventJets[iJet].isGenJetMatched;
                inEJIShiftDown->vecEventJets[iJet].dEnRecoGen = inEJICV->vecEventJets[iJet].dEnRecoGen;
            }
        }
    }
}

void CorrectEDSI(EventDiStructureInfo * inEDSICV, EventDiStructureInfo * inEDSIShiftUp, EventDiStructureInfo * inEDSIShiftDown, int whichSyst = 0) {
    
    
    int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    int systUncES = 6;
    if (whichSyst == systBMisTagSF || whichSyst == systBTagEffSF) {
        inEDSIShiftUp->DP_ZMET.DPhiInputObjects = inEDSICV->DP_ZMET.DPhiInputObjects;
        inEDSIShiftUp->DP_DiBLeps.DPhiInputObjects = inEDSICV->DP_DiBLeps.DPhiInputObjects;
        
        inEDSIShiftDown->DP_ZMET.DPhiInputObjects = inEDSICV->DP_ZMET.DPhiInputObjects;
        inEDSIShiftDown->DP_DiBLeps.DPhiInputObjects = inEDSICV->DP_DiBLeps.DPhiInputObjects;
    }
    
    for (int iLep = 0; iLep < inEDSICV->numLeps; ++iLep) {
        if (whichSyst == systBMisTagSF || whichSyst == systBTagEffSF) {
            inEDSIShiftUp->vecDP_LepMET[iLep].DPhiInputObjects = inEDSICV->vecDP_LepMET[iLep].DPhiInputObjects;
            inEDSIShiftDown->vecDP_LepMET[iLep].DPhiInputObjects = inEDSICV->vecDP_LepMET[iLep].DPhiInputObjects;
            
            if (whichSyst == systUncES) {
                for (int iJet = 0; iJet < inEDSICV->numJets; ++iJet) {
                    inEDSIShiftUp->vecVecDP_LepJet[iLep][iJet].DPhiInputObjects = inEDSICV->vecVecDP_LepJet[iLep][iJet].DPhiInputObjects;
                    inEDSIShiftDown->vecVecDP_LepJet[iLep][iJet].DPhiInputObjects = inEDSICV->vecVecDP_LepJet[iLep][iJet].DPhiInputObjects;
                }
            }
        }
        if (whichSyst == systUncES) {            
            for (int iBJet = 0; iBJet < inEDSICV->numBJets; ++iBJet) {
                inEDSIShiftUp->vecVecDP_LepBJet[iLep][iBJet].DPhiInputObjects = inEDSICV->vecVecDP_LepBJet[iLep][iBJet].DPhiInputObjects;
                inEDSIShiftDown->vecVecDP_LepBJet[iLep][iBJet].DPhiInputObjects = inEDSICV->vecVecDP_LepBJet[iLep][iBJet].DPhiInputObjects;
            }
        }
    }     
    if (whichSyst == systLepES || whichSyst == systUncES) {
        for (int iJet = 0; iJet < inEDSICV->numJets; ++iJet) {
            for (int iBJet = 0; iBJet < inEDSICV->numBJets; ++iBJet) {
                inEDSIShiftUp->vecVecDP_JetBJet[iJet][iBJet].DPhiInputObjects = inEDSICV->vecVecDP_JetBJet[iJet][iBJet].DPhiInputObjects;
                inEDSIShiftDown->vecVecDP_JetBJet[iJet][iBJet].DPhiInputObjects = inEDSICV->vecVecDP_JetBJet[iJet][iBJet].DPhiInputObjects;
            }
        }
    }
}

void CorrectEMI(EventMETInfo * inEMICV, EventMETInfo * inEMIShiftUp, EventMETInfo * inEMIShiftDown, int whichSyst = 0) {
    //int systLepES = 1;
    //int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    //int systJetSmear = 5;
    //int systUncES = 6;
    if (whichSyst == systBTagEffSF || whichSyst == systBMisTagSF) {
        inEMIShiftUp->MET_EMT2I.MT_Lep0 = inEMICV->MET_EMT2I.MT_Lep0;
        inEMIShiftUp->MET_EMT2I.MT_Lep1 = inEMICV->MET_EMT2I.MT_Lep1;
        inEMIShiftUp->MET_EMT2I.MT_HTVec = inEMICV->MET_EMT2I.MT_HTVec;
        
        inEMIShiftUp->EventMETdivMeff = inEMICV->EventMETdivMeff;
        inEMIShiftUp->EventMETdivHT = inEMICV->EventMETdivHT;
        
        inEMIShiftUp->MET_ERI.recoilUPar = inEMICV->MET_ERI.recoilUPar;
        inEMIShiftUp->MET_ERI.recoilUPerp = inEMICV->MET_ERI.recoilUPerp;
        
        inEMIShiftUp->MET_EMT2I.EventMT2lb = inEMICV->MET_EMT2I.EventMT2lb;
        
        inEMIShiftUp->MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed = inEMICV->MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed;
        inEMIShiftUp->MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed = inEMICV->MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed;
        
        
        
        
        inEMIShiftDown->MET_EMT2I.MT_Lep0 = inEMICV->MET_EMT2I.MT_Lep0;
        inEMIShiftDown->MET_EMT2I.MT_Lep1 = inEMICV->MET_EMT2I.MT_Lep1;
        inEMIShiftDown->MET_EMT2I.MT_HTVec = inEMICV->MET_EMT2I.MT_HTVec;
        
        inEMIShiftDown->EventMETdivMeff = inEMICV->EventMETdivMeff;
        inEMIShiftDown->EventMETdivHT = inEMICV->EventMETdivHT;
        
        inEMIShiftDown->MET_ERI.recoilUPar = inEMICV->MET_ERI.recoilUPar;
        inEMIShiftDown->MET_ERI.recoilUPerp = inEMICV->MET_ERI.recoilUPerp;
        
        inEMIShiftDown->MET_EMT2I.EventMT2lb = inEMICV->MET_EMT2I.EventMT2lb;
        
        inEMIShiftDown->MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed = inEMICV->MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed;
        inEMIShiftDown->MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed = inEMICV->MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed;
    }
}

void CorrectThingsEMI(EventMETInfo * inEMICV, vector<EventMETInfo> * inVecEMIShiftUp, vector<EventMETInfo> * inVecEMIShiftDown) {
    for (unsigned int iSyst = 0; iSyst < inVecEMIShiftUp->size(); ++iSyst) {
        CorrectEMI(inEMICV, &inVecEMIShiftUp->at(iSyst), &inVecEMIShiftDown->at(iSyst), iSyst);
    }
}

void CorrectThingsEJI(EventJetInfo * inEJICV, vector<EventJetInfo> * inVecEJIShiftUp, vector<EventJetInfo> * inVecEJIShiftDown, bool isSmear = false) {
    for (unsigned int iSyst = 0; iSyst < inVecEJIShiftUp->size(); ++iSyst) {
        CorrectEJI(inEJICV, &inVecEJIShiftUp->at(iSyst), &inVecEJIShiftDown->at(iSyst), iSyst, isSmear);
    }
}

void CorrectThingsEDSI(EventDiStructureInfo * inEDSICV, vector<EventDiStructureInfo> * inVecEDSIShiftUp, vector<EventDiStructureInfo> * inVecEDSIShiftDown) {
    for (unsigned int iSyst = 0; iSyst < inVecEDSIShiftUp->size(); ++iSyst) {
        CorrectEDSI(inEDSICV, &inVecEDSIShiftUp->at(iSyst), &inVecEDSIShiftDown->at(iSyst), iSyst);
    }
}

