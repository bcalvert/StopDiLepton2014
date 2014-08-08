// Functions utilized to set input branch addresses that the Maryland Skim Code uses
// (4/3/14) Names based on Legacy version of Oviedo nTuples
inline void SetInTreeBranchSUSY(TTree * fileTree, GenSUSYEventPointers * inGSEPs) {
    TString prefixGenPart = "T_Gen_";
    //Signal stuff
    fileTree->SetBranchAddress( "T_Gen_polWeights",   &inGSEPs->genStopPolarizationWeights );
    
    const int numSUSY = 3;
    
    TString nameSUSYParticle[numSUSY] = {"Stop", "Chi0", "ChiPM"};
    vector<float> * genSUSYMassToSet;
    GenParticlePointers_St3 * GenSUSY_PPSt3ToSet;
    TString prefixBranchName;
    for (int iSUSY = 0; iSUSY < numSUSY; ++iSUSY) {
        switch (iSUSY) {
            case 0:
                genSUSYMassToSet = inGSEPs->genStopMass;
                GenSUSY_PPSt3ToSet = &inGSEPs->GPP_St3_Stop;
                break;
            case 1:
                genSUSYMassToSet = inGSEPs->genChi0Mass;
                GenSUSY_PPSt3ToSet = &inGSEPs->GPP_St3_Chi0;
                break;
            case 2:
                genSUSYMassToSet = inGSEPs->genCharginoMass;
                GenSUSY_PPSt3ToSet = &inGSEPs->GPP_St3_Chargino;
                break;                
            default:
                break;
        }
        /*
        if (iSUSY == 2) {
            fileTree->SetBranchAddress(prefixGenPart + "Chargino" + TString("Mass"), &genSUSYMassToSet);
        }
        else {
            fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("Mass"), &genSUSYMassToSet);
        }
*/        
        prefixBranchName = prefixGenPart + nameSUSYParticle[iSUSY];
        
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[0] + TString("Mass"), &inGSEPs->genStopMass);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[1] + TString("Mass"), &inGSEPs->genChi0Mass);
        fileTree->SetBranchAddress(prefixGenPart + "Chargino" + TString("Mass"), &inGSEPs->genCharginoMass);
        
        GenSUSY_PPSt3ToSet->SetBranchAddresses(fileTree, prefixBranchName);
        /*
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_pdgId"), &GenSUSY_PPSt3ToSet->genPartSt3_pdgId);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_firstMother"), &GenSUSY_PPSt3ToSet->genPartSt3_firstMom);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_i"), &GenSUSY_PPSt3ToSet->genPartSt3_i);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_energy"), &GenSUSY_PPSt3ToSet->genPartSt3En);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_pt"), &GenSUSY_PPSt3ToSet->genPartSt3Pt);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_eta"), &GenSUSY_PPSt3ToSet->genPartSt3Eta);
        fileTree->SetBranchAddress(prefixGenPart + nameSUSYParticle[iSUSY] + TString("St3_phi"), &GenSUSY_PPSt3ToSet->genPartSt3Phi);
        */
    }    
}

inline void SetInTreeBranchGenLep(TTree * fileTree, GenLeptonEventPointers * inGLEPs) {
    TString prefixGenPart = "T_Gen_";
    const int numLeps = 2;
    TString nameLep[numLeps] = {"Elec", "Muon"};
    
    GenParticlePointers_St3 * GenLep_PPSt3ToSet;
    GenParticlePointers_St1 * GenLep_PPSt1ToSet;
    TString prefixBranchName;
    for (int iLep = 0; iLep < numLeps; ++iLep) {
        switch (iLep) {
            case 0:
                GenLep_PPSt1ToSet = &inGLEPs->GPP_St1_Elec;
                GenLep_PPSt3ToSet = &inGLEPs->GPP_St3_Elec;
                break;
            case 1:
                GenLep_PPSt1ToSet = &inGLEPs->GPP_St1_Muon;
                GenLep_PPSt3ToSet = &inGLEPs->GPP_St3_Muon;
                break;                
            default:
                break;
        }
        prefixBranchName = prefixGenPart + nameLep[iLep];
        GenLep_PPSt3ToSet->SetBranchAddresses(fileTree, prefixBranchName);
        GenLep_PPSt1ToSet->SetBranchAddresses(fileTree, prefixBranchName);
        /*
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_pdgId"),       &GenLep_PPSt3ToSet->genPartSt3_pdgId );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_firstMother"), &GenLep_PPSt3ToSet->genPartSt3_firstMom );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_i"),           &GenLep_PPSt3ToSet->genPartSt3_i  );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_energy"),      &GenLep_PPSt3ToSet->genPartSt3En );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_pt"),          &GenLep_PPSt3ToSet->genPartSt3Pt );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_eta"),         &GenLep_PPSt3ToSet->genPartSt3Eta );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("St3_phi"),         &GenLep_PPSt3ToSet->genPartSt3Phi );
        
        
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_PID"),     &GenLep_PPSt1ToSet->genPartSt1_pdgId );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_Px"),      &GenLep_PPSt1ToSet->genPartSt1Px );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_Py"),      &GenLep_PPSt1ToSet->genPartSt1Py );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_Pz"),      &GenLep_PPSt1ToSet->genPartSt1Pz );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_Energy"),  &GenLep_PPSt1ToSet->genPartSt1En );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_MPID"),    &GenLep_PPSt1ToSet->genPartSt1MomPID );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_MPx"),     &GenLep_PPSt1ToSet->genPartSt1MomPx );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_MPy"),     &GenLep_PPSt1ToSet->genPartSt1MomPy );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_MPz"),     &GenLep_PPSt1ToSet->genPartSt1MomPz );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_MEnergy"), &GenLep_PPSt1ToSet->genPartSt1MomEn );
        fileTree->SetBranchAddress( prefixGenPart + nameLep[iLep] + TString("_MSt"),     &GenLep_PPSt1ToSet->genPartSt1MomStatus );    
         */
    }    
}

inline void SetInTreeBranchGenQuark(TTree * fileTree, GenQuarkEventPointers * inGQEPs) {
    TString prefixGenPart = "T_Gen_";
    const int numQuarks = 2;
    TString nameQuark[numQuarks] = {"t", "b"};
    
    GenParticlePointers_St3 * GenQuark_PPSt3ToSet = 0;
    GenParticlePointers_St1 * GenQuark_PPSt1ToSet = 0;
    TString prefixBranchName;
    for (int iQuark = 0; iQuark < numQuarks; ++iQuark) {
        switch (iQuark) {
            case 0:
                //GenQuark_PPSt1ToSet = inGQEPs->GPP_St1_Top; //NoGetStatus 1 Top Quark
                GenQuark_PPSt3ToSet = &inGQEPs->GPP_St3_Top;
                break;
            case 1:
                GenQuark_PPSt1ToSet = &inGQEPs->GPP_St1_B;
                GenQuark_PPSt3ToSet = &inGQEPs->GPP_St3_B;
                break;                
            default:
                break;
        }
        prefixBranchName = prefixGenPart + nameQuark[iQuark];
        GenQuark_PPSt3ToSet->SetBranchAddresses(fileTree, prefixBranchName);
        /*
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_pdgId"),       &GenQuark_PPSt3ToSet->genPartSt3_pdgId );
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_firstMother"), &GenQuark_PPSt3ToSet->genPartSt3_firstMom );
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_i"),           &GenQuark_PPSt3ToSet->genPartSt3_i  );
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_energy"),      &GenQuark_PPSt3ToSet->genPartSt3En );
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_pt"),          &GenQuark_PPSt3ToSet->genPartSt3Pt );
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_eta"),         &GenQuark_PPSt3ToSet->genPartSt3Eta );
        fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("St3_phi"),         &GenQuark_PPSt3ToSet->genPartSt3Phi );
         */
        
        if (iQuark > 0) {
            GenQuark_PPSt1ToSet->SetBranchAddresses(fileTree, prefixBranchName);
            /*
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_PID"),     &GenQuark_PPSt1ToSet->genPartSt1_pdgId );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_Px"),      &GenQuark_PPSt1ToSet->genPartSt1Px );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_Py"),      &GenQuark_PPSt1ToSet->genPartSt1Py );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_Pz"),      &GenQuark_PPSt1ToSet->genPartSt1Pz );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_Energy"),  &GenQuark_PPSt1ToSet->genPartSt1En );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_MPID"),    &GenQuark_PPSt1ToSet->genPartSt1MomPID );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_MPx"),     &GenQuark_PPSt1ToSet->genPartSt1MomPx );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_MPy"),     &GenQuark_PPSt1ToSet->genPartSt1MomPy );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_MPz"),     &GenQuark_PPSt1ToSet->genPartSt1MomPz );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_MEnergy"), &GenQuark_PPSt1ToSet->genPartSt1MomEn );
            fileTree->SetBranchAddress( prefixGenPart + nameQuark[iQuark] + TString("_MSt"),     &GenQuark_PPSt1ToSet->genPartSt1MomStatus );
            */
        }
    }    
}

inline void SetInTreeBranchGenEWK(TTree * fileTree, GenEWKEventPointers * inGEWKEPs) {
    TString prefixGenPart = "T_Gen_";
    const int numBosons = 2;
    TString nameBoson[numBosons] = {"W", "Z"};
    TString prefixBranchName;
    for (int iBoson = 0; iBoson < numBosons; ++iBoson) {
        prefixBranchName = prefixGenPart + nameBoson[iBoson];
        inGEWKEPs->vecGPP_St3_EWKVecBoson[iBoson].SetBranchAddresses(fileTree, prefixBranchName);
    }   
}

inline void SetInTreeBranchGenJet(TTree * fileTree, GenJetEventPointers * inGJEPs) {
    TString prefixGenPart = "T_JetAKCHS_";

    /// Gen Jet info
    fileTree->SetBranchAddress(prefixGenPart + TString("GenJet_Px"),           &inGJEPs->genJetPx);
    fileTree->SetBranchAddress(prefixGenPart + TString("GenJet_Py"),           &inGJEPs->genJetPy);
    fileTree->SetBranchAddress(prefixGenPart + TString("GenJet_Pz"),           &inGJEPs->genJetPz);
    fileTree->SetBranchAddress(prefixGenPart + TString("GenJet_Energy"),       &inGJEPs->genJetEn);
    fileTree->SetBranchAddress(prefixGenPart + TString("GenJet_InvisibleE"),   &inGJEPs->genJetInvisE);
    fileTree->SetBranchAddress(prefixGenPart + TString("IsGenJet"),            &inGJEPs->genJetIsGenJet);

}

inline void SetInTreeBranchRecoElec(TTree * fileTree, ElectronEventPointers * inEEPs) {
    
    TString prefixRecoElec = "T_Elec_";
    ////Electron Branches////
    //Electron Kinematic Parameters
    fileTree->SetBranchAddress(prefixRecoElec + TString("PFElecPt"),            &inEEPs->PFElecPt);
    fileTree->SetBranchAddress(prefixRecoElec + TString("Pt"),                  &inEEPs->ElecPt);
    fileTree->SetBranchAddress(prefixRecoElec + TString("Px"),                  &inEEPs->ElecPx);
    fileTree->SetBranchAddress(prefixRecoElec + TString("Py"),                  &inEEPs->ElecPy);
    fileTree->SetBranchAddress(prefixRecoElec + TString("Pz"),                  &inEEPs->ElecPz);
    fileTree->SetBranchAddress(prefixRecoElec + TString("Energy"),              &inEEPs->ElecEn);
    fileTree->SetBranchAddress(prefixRecoElec + TString("Charge"),              &inEEPs->ElecCharge);        
    
    //Electron Supercluster/Shower parameters
    fileTree->SetBranchAddress(prefixRecoElec + TString("SC_Eta"),              &inEEPs->ElecSCEta);
    fileTree->SetBranchAddress(prefixRecoElec + TString("deltaPhiIn"),          &inEEPs->ElecDeltaPhiIn);
    fileTree->SetBranchAddress(prefixRecoElec + TString("deltaEtaIn"),          &inEEPs->ElecDeltaEtaIn);
    fileTree->SetBranchAddress(prefixRecoElec + TString("HtoE"),                &inEEPs->ElecHtoERatio);
    fileTree->SetBranchAddress(prefixRecoElec + TString("sigmaIetaIeta"),       &inEEPs->ElecSigIetaIeta);
    fileTree->SetBranchAddress(prefixRecoElec + TString("eSuperClusterOverP"),  &inEEPs->ElecSCEOverP);
    fileTree->SetBranchAddress(prefixRecoElec + TString("ecalEnergy"),          &inEEPs->ElecECalE);        
    
    //Electron Vertex Geometry
    fileTree->SetBranchAddress(prefixRecoElec + TString("IPwrtPV"),             &inEEPs->ElecIP);
    fileTree->SetBranchAddress(prefixRecoElec + TString("dzwrtPV"),             &inEEPs->ElecDZ);
    fileTree->SetBranchAddress(prefixRecoElec + TString("nHits"),               &inEEPs->ElecNumMissHits);
    
    //Electron Isolation parameters
    fileTree->SetBranchAddress(prefixRecoElec + TString("chargedHadronIso"),    &inEEPs->ElecPFCharHadIso);
    fileTree->SetBranchAddress(prefixRecoElec + TString("neutralHadronIso"),    &inEEPs->ElecPFNeutHadIso);
    fileTree->SetBranchAddress(prefixRecoElec + TString("photonIso"),           &inEEPs->ElecPFPhotIso);
    
    //Electron booleans
    fileTree->SetBranchAddress(prefixRecoElec + TString("passConversionVeto"),  &inEEPs->passConvVeto);
    fileTree->SetBranchAddress(prefixRecoElec + TString("isPF"),                &inEEPs->isPFElectron);
    fileTree->SetBranchAddress(prefixRecoElec + TString("isEB"),                &inEEPs->ElecisEB);
    fileTree->SetBranchAddress(prefixRecoElec + TString("isEE"),                &inEEPs->ElecisEE);
    
}
inline void SetInTreeBranchRecoMuon(TTree * fileTree, MuonEventPointers * inMEPs) {
    
    TString prefixRecoMuon = "T_Muon_";
    ////Muon Branches////
    //Muon Kinematics
    fileTree->SetBranchAddress(prefixRecoMuon + TString("PFMuonPt"),                &inMEPs->PFMuonPt);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("Pt"),                      &inMEPs->MuonPt);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("Px"),                      &inMEPs->MuonPx);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("Py"),                      &inMEPs->MuonPy);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("Pz"),                      &inMEPs->MuonPz);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("Energy"),                  &inMEPs->MuonEn);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("Charge"),                  &inMEPs->MuonCharge); 
    
    //Muon Isolation parameters
    fileTree->SetBranchAddress(prefixRecoMuon + TString("chargedHadronIsoR03"),     &inMEPs->MuonPFCharHadIso);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("neutralHadronIsoR03"),     &inMEPs->MuonPFNeutHadIso);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("photonIsoR03"),            &inMEPs->MuonPFPhotIso);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("sumPUPtR03"),              &inMEPs->MuonSumPUPt);
    
    //Muon vertex geometry 
    fileTree->SetBranchAddress(prefixRecoMuon + TString("IPwrtAveBSInTrack"),       &inMEPs->MuonD0);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("vz"),                      &inMEPs->MuonVertZ);
    
    //Muon track/detector parameters
    fileTree->SetBranchAddress(prefixRecoMuon + TString("NumOfMatchedStations"),    &inMEPs->MuonNumMatchStations);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("NValidPixelHitsInTrk"),    &inMEPs->MuonNumValidPixHitsinTrack);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("NLayers"),                 &inMEPs->MuonNumLayers);
    
    //Muon booleans
    fileTree->SetBranchAddress(prefixRecoMuon + TString("IsGMPTMuons"),             &inMEPs->isGMPTMuon);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("isPFMuon"),                &inMEPs->isPFMuon);
    fileTree->SetBranchAddress(prefixRecoMuon + TString("IsGlobalMuon"),            &inMEPs->isGlobMuon);
    //        fileTree->SetBranchAddress(prefixRecoMuon + TString("IsTrackerMuonArbitrated"), &inMEPs->isTrackArbitMuon);    
}

inline void SetInTreeBranchRecoJet(TTree * fileTree, PFJetEventPointers * inPFJEPs, bool isSmear = false) {
    
    TString prefixRecoJet = isSmear ? "T_JetAKCHSsmeared_" : "T_JetAKCHS_";
    ////Jet Branches
    //        fileTree->SetBranchAddress(prefixRecoJet + TString("Et"),        &inPFJEPs->JetEt);
    //        fileTree->SetBranchAddress(prefixRecoJet + TString("Eta"),       &inPFJEPs->JetEta);  
    fileTree->SetBranchAddress(prefixRecoJet + TString("Px"),                  &inPFJEPs->JetPx);
    fileTree->SetBranchAddress(prefixRecoJet + TString("Py"),                  &inPFJEPs->JetPy);
    fileTree->SetBranchAddress(prefixRecoJet + TString("Pz"),                  &inPFJEPs->JetPz);
    fileTree->SetBranchAddress(prefixRecoJet + TString("Energy"),              &inPFJEPs->JetEn);
    fileTree->SetBranchAddress(prefixRecoJet + TString("NeutHadEnergyFrac"),   &inPFJEPs->JetNHF);
    fileTree->SetBranchAddress(prefixRecoJet + TString("NeutEmEnergyFrac"),    &inPFJEPs->JetNEF);
    fileTree->SetBranchAddress(prefixRecoJet + TString("CharHadEnergyFrac"),   &inPFJEPs->JetCHF);
    fileTree->SetBranchAddress(prefixRecoJet + TString("CharEmEnergyFrac"),    &inPFJEPs->JetCEF);
    fileTree->SetBranchAddress(prefixRecoJet + TString("Tag_CombSVtx"),        &inPFJEPs->JetBTag);
    fileTree->SetBranchAddress(prefixRecoJet + TString("Parton_Flavour"),      &inPFJEPs->JetPartFlav);
    fileTree->SetBranchAddress(prefixRecoJet + TString("nDaughters"),          &inPFJEPs->JetNDaug);
    fileTree->SetBranchAddress(prefixRecoJet + TString("ChargedMultiplicity"), &inPFJEPs->JetCharMult);   
}

inline void SetInTreeBranchMETOld(TTree * fileTree, EventMETInfo * inEMI) {
    TString prefixRecoMET = "T_METPFTypeI_";
    fileTree->SetBranchAddress( prefixRecoMET + TString("ET"),   &inEMI->EventMET );
    fileTree->SetBranchAddress( prefixRecoMET + TString("Phi"),  &inEMI->EventMETPhi );
    fileTree->SetBranchAddress( prefixRecoMET + TString("Sig"),  &inEMI->EventMETSig );
}

inline void SetInTreeBranchMET(TTree * fileTree, EventMETInfo * inEMI) {
    TString prefixRecoMET[5] = {"T_METPFType0I", "", "", "", "T_caloMET"};
    fileTree->SetBranchAddress( prefixRecoMET[inEMI->METType] + TString("_ET"),     &inEMI->EventMET );
    fileTree->SetBranchAddress( prefixRecoMET[inEMI->METType] + TString("_Phi"),    &inEMI->EventMETPhi );
    fileTree->SetBranchAddress( prefixRecoMET[inEMI->METType] + TString("_SumET"),  &inEMI->EventSumET );
}

inline void SetInTreeBranchMETSmeared(TTree * fileTree, EventMETInfo * inEMI, int whichSyst = 0) {
    TString prefixRecoMET = "T_METType1CorrectedPFMet";
    TString prefixBranchName = prefixRecoMET;
    prefixBranchName += SystString_v2(whichSyst);
    prefixBranchName += "Smeared_";
    fileTree->SetBranchAddress( prefixBranchName + TString("ET"),     &inEMI->EventMET );
    fileTree->SetBranchAddress( prefixBranchName + TString("Phi"),    &inEMI->EventMETPhi );
    fileTree->SetBranchAddress( prefixBranchName + TString("SumET"),  &inEMI->EventSumET );    
}

inline void SetInTreeBranchFilterTrigger(TTree * fileTree, FilterTriggerInfo * inFTI) {
    //Trigger information
    fileTree->SetBranchAddress( "T_passTriggerDoubleMu",             &inFTI->passTrigDoubleMu );
    fileTree->SetBranchAddress( "T_passTriggerDoubleEl",             &inFTI->passTrigDoubleEl );
    fileTree->SetBranchAddress( "T_passTriggerElMu",                 &inFTI->passTrigElMu );
    
    //MET Filter information        
    fileTree->SetBranchAddress( "T_EventF_EcalDeadCell",             &inFTI->filterECalDeadCell );
    fileTree->SetBranchAddress( "T_EventF_logErrorTooManyClusters",  &inFTI->filterLogErrorTooManyClusters );
    fileTree->SetBranchAddress( "T_EventF_trackingFailure",          &inFTI->filterTrackingFailure );
    fileTree->SetBranchAddress( "T_EventF_hcalLaser",                &inFTI->filterHCalLaser );
    fileTree->SetBranchAddress( "T_EventF_ecalLaserCorr",            &inFTI->filterECalLaserCorr );
    fileTree->SetBranchAddress( "T_EventF_toomanystripclus",         &inFTI->filterTooManyStripClus );
    fileTree->SetBranchAddress( "T_EventF_manystripclus",            &inFTI->filterManyStripClus );
    fileTree->SetBranchAddress( "T_EventF_eeBadSc",                  &inFTI->filterEEBadSC );
}