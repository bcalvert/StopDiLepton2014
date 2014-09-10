// Structs that contain information about the event as a whole related to certain classes of generator particles

typedef struct EventGenSUSYParticleInfo {
    // Structs that contain information about the event as a whole related to gen-level SUSY particles
    GenSUSYEventPointers GSEPs;
    void InitializeVecs() {
        GSEPs.InitializeVecs();
    }
    void DeleteVecs() {
        GSEPs.DeleteVecs();
    }
    void SetSize() {
        GSEPs.SetSize();
    }
    vector< vector<GenParticleSt3Info> > vecVecGenSUSYPSt3I;
    vector< vector<int> > vecVecGenSUSYRoundMass;
    
    int numSavedSUSYParts;
    int numSavedIndSUSYParts;
    
    vector<float> vecGenCharginoRoundMassFrac;
    /*
    GenParticleSt3Info genStop0, genStop1;
    GenParticleSt3Info genChiZero0, genChiZero1;
    GenParticleSt3Info genChargino0, genChargino1;
    
    int genStopMass0, genStopMass1;
    int genChi0Mass0, genChi0Mass1;
    int genCharginoMass0, genCharginoMass1;
    float MassFrac_Chargino0, MassFrac_Chargino1;
    */
    float StopXSec, StopXSecErr;
    GenParticle * GrabGP(int whichPartType = 0, int indexPart = 0) {
        return &vecVecGenSUSYPSt3I[whichPartType][indexPart].GP;
    }
    void EGSPIDefaultVarVals() {
        numSavedSUSYParts = 3;
        numSavedIndSUSYParts = 2;
        vecVecGenSUSYPSt3I.resize(numSavedSUSYParts);
        vecVecGenSUSYRoundMass.resize(numSavedSUSYParts);
        for (int iSUSY = 0; iSUSY < numSavedSUSYParts; ++iSUSY) {
            vecVecGenSUSYPSt3I[iSUSY].resize(numSavedIndSUSYParts);
            vecVecGenSUSYRoundMass[iSUSY].resize(numSavedIndSUSYParts);
        }
        vecGenCharginoRoundMassFrac.resize(numSavedIndSUSYParts);
        StopXSec = 0.; StopXSecErr = 0.;        
    }
    void SetSUSYParticles(bool doChargino = false) {
        int numSUSYTypes = doChargino ? 3 : 2;
        for (int iSUSY = 0; iSUSY < numSUSYTypes; ++iSUSY) {
            for (int iIndSUSY = 0; iIndSUSY < numSavedIndSUSYParts; ++iIndSUSY) {
                GSEPs.SetGenParticleInfo_Full(iSUSY, iIndSUSY, &vecVecGenSUSYPSt3I[iSUSY][iIndSUSY]);
            }
        }
        /*
        GSEPs.SetGenParticleInfo_Full(0, 0, &genStop0);
        GSEPs.SetGenParticleInfo_Full(0, 1, &genStop1);
        GSEPs.SetGenParticleInfo_Full(1, 0, &genChiZero0);
        GSEPs.SetGenParticleInfo_Full(1, 1, &genChiZero1);
        if (doChargino) {
            GSEPs.SetGenParticleInfo_Full(2, 0, &genChargino0);
            GSEPs.SetGenParticleInfo_Full(2, 1, &genChargino1);
        }
        */
    }
    float CharginoMassFrac(float genStopMass, float genChi0Mass, float genCharginoMass) {
        //currently rounding to nearest multiple of 0.25
        float estX = (genCharginoMass - genChi0Mass)/(genStopMass - genChi0Mass);
        float returnX = TMath::Nint(estX * 4) * 0.25;
        return returnX;    
    }
    bool PassesStopBugCheck() {
        float genStopMass0 = GSEPs.genStopMass->at(0);
        float genStopMass1 = GSEPs.genStopMass->at(1);
        float genChi0Mass0 = GSEPs.genChi0Mass->at(0);
        float genChi0Mass1 = GSEPs.genChi0Mass->at(1);

        //    return !(fabs(genStopMass0 -genStopMass1) > 0.1 || fabs(genChi0Mass0 - genChi0Mass1) > 0.1); // || (genChi0Mass0 < 0) || (genChi0Mass1< 0));
        return !(fabs(genStopMass0 - genStopMass1) > 0.1 || fabs(genChi0Mass0 - genChi0Mass1) > 0.1 || (genChi0Mass0 < 0) || (genChi0Mass1 < 0));
    }
    void SetMasses(float roundNum, int roundMult = -1) {
        if (GSEPs.genStopMass->size() > 1) {
            for (int iStop = 0; iStop < numSavedIndSUSYParts; ++iStop) {
                vecVecGenSUSYRoundMass[0][iStop] = TMath::Nint(GSEPs.genStopMass->at(iStop)/roundNum);
                if (roundMult > 0) {
                    vecVecGenSUSYRoundMass[0][iStop] *= roundMult;
                }
                else {
                    vecVecGenSUSYRoundMass[0][iStop] *= roundNum;
                }
            }
        }
        if (GSEPs.genChi0Mass->size() > 1) {
            for (int iChi0 = 0; iChi0 < numSavedIndSUSYParts; ++iChi0) {
                vecVecGenSUSYRoundMass[1][iChi0] =  TMath::Nint(GSEPs.genChi0Mass->at(iChi0)/roundNum);
                if (roundMult > 0) {
                    vecVecGenSUSYRoundMass[1][iChi0] *= roundMult;
                }
                else {
                    vecVecGenSUSYRoundMass[1][iChi0] *= roundNum;
                }
            }
        }
        if (GSEPs.genCharginoMass->size() > 1) {
            for (int iChargino = 0; iChargino < numSavedIndSUSYParts; ++iChargino) {
                vecGenCharginoRoundMassFrac[iChargino] = CharginoMassFrac(GSEPs.genStopMass->at(iChargino), GSEPs.genChi0Mass->at(iChargino), GSEPs.genCharginoMass->at(iChargino));
                vecVecGenSUSYRoundMass[2][iChargino] = vecGenCharginoRoundMassFrac[iChargino] * vecVecGenSUSYRoundMass[0][iChargino] + (1 - vecGenCharginoRoundMassFrac[iChargino]) * vecVecGenSUSYRoundMass[1][iChargino];
            }
        }
    }
    
    bool PassMassCut(float genStopMassCut, float genChi0MassCut, float roundNum) {
        bool passCut = true;
        for (int iIndSUSY = 0; iIndSUSY < 1; ++iIndSUSY) {
            passCut &= (vecVecGenSUSYRoundMass[0][iIndSUSY] >= 0 && fabs(genStopMassCut - vecVecGenSUSYRoundMass[0][iIndSUSY]) < (roundNum / 2.));
            passCut &= (vecVecGenSUSYRoundMass[1][iIndSUSY] >= 0 && fabs(genChi0MassCut - vecVecGenSUSYRoundMass[1][iIndSUSY]) < (roundNum / 2.));
        }
//        passCut = (genStopMass0 >=0 && abs(genStopMassCut - genStopMass0) < (roundNum / 2.));
//        passCut &= (genChi0Mass0 >=0 && abs(genChi0MassCut - genChi0Mass0) < (roundNum / 2.));
            //                if (genCharginoMass0 >=0 && abs(genCharginoMassCut - genCharginoMass0) < (roundNum / 2.)) continue;
        return passCut;
    }
    
    bool PassMassCutPlotMaker(int grabStopMass, int grabChi0Mass, float grabCharginoMassFrac, float massDiffThresh, float CharginoMassFracDiffThresh, bool isT2tt) {
        bool passesCut = true;
//        cout << "vecVecGenSUSYRoundMass[0][0] " << vecVecGenSUSYRoundMass[0][0] << endl;
//        cout << "vecVecGenSUSYRoundMass[1][0] " << vecVecGenSUSYRoundMass[1][0] << endl;        
        for (int iIndSUSY = 0; iIndSUSY < 2; ++iIndSUSY) {
            if (fabs(vecVecGenSUSYRoundMass[0][iIndSUSY] - grabStopMass) > massDiffThresh) passesCut = false;
            if (fabs(vecVecGenSUSYRoundMass[1][iIndSUSY] - grabChi0Mass) > massDiffThresh) passesCut = false;
            if (!isT2tt) {
                if (fabs(vecGenCharginoRoundMassFrac[iIndSUSY] - grabCharginoMassFrac) > CharginoMassFracDiffThresh) passesCut = false;
            }
        }        
        return passesCut;
    }
    
    void PrintMassInfo(unsigned int EventNum, bool isT2tt) {
        if (isT2tt) {
            cout << "in format EventNum:genStopMass:genChi0Mass ";
//            cout << EventNum << ":" << genStopMass0 << ":" << genChi0Mass0 << endl;
            cout << EventNum << ":" << vecVecGenSUSYRoundMass[0][0] << ":" << vecVecGenSUSYRoundMass[1][0] << endl;
        }
        else {
            cout << "in format EventNum:genStopMass:genChi0Mass:genCharginoMass:genCharginoMassFrac ";
//            cout << EventNum << ":" << genStopMass0 << ":" << genChi0Mass0 << ":" <<  genCharginoMass0 << ":" << MassFrac_Chargino0 << endl;
            cout << EventNum << ":" << vecVecGenSUSYRoundMass[0][0] << ":" << vecVecGenSUSYRoundMass[1][0] << ":" <<  vecVecGenSUSYRoundMass[2][0] << ":" << vecGenCharginoRoundMassFrac[0] << endl;
        }
    }
    float GetStopXSec(int Systematic = 0) {
        return StopXSec + (Systematic * StopXSecErr * StopXSec);
    }
    float GetStopPolWeight(int StopRightPolPercent) {
        return GSEPs.genStopPolarizationWeights->at(StopRightPolPercent/10);
    }
} EventGenSUSYParticleInfo;


typedef struct EventGenEWKParticleInfo {
        // Structs that contain information about the event as a whole related to gen-level EWK vec boson particles
    GenEWKEventPointers GEWKEPs;
    //0 is W, 1 is Z, 2 is Gamma
    vector< vector<GenParticleSt3Info> > vecVecGenEWKPSt3I;
    int numBosons;
    int numIndBosons;
    GenParticle * GrabGP(int whichPartType = 0, int indexPart = 0) {
        return &vecVecGenEWKPSt3I[whichPartType][indexPart].GP;
    }
    void InitializeVecs() {
        GEWKEPs.InitializeVecs();                
    }
    void DeleteVecs() {
        GEWKEPs.DeleteVecs();                
    }
    void SetSize() {
        GEWKEPs.SetSize();
    }
    void DefaultVarVals() {
        numBosons = 2;
        numIndBosons = 2;
        GEWKEPs.vecGPP_St3_EWKVecBoson.resize(numBosons);
        vecVecGenEWKPSt3I.resize(numBosons);
        for (int iBoson = 0; iBoson < numBosons; ++iBoson) {
            vecVecGenEWKPSt3I[iBoson].resize(numIndBosons);
        }
    }
    void SetParticleInfo() {
        for (int iBoson = 0; iBoson < numBosons; ++iBoson) {
            for (int iIndBoson = 0; iIndBoson < numIndBosons; ++iIndBoson) {
                GEWKEPs.SetGenParticleInfo_Full(iBoson, iIndBoson, &vecVecGenEWKPSt3I[iBoson][iIndBoson]);
            }
        }
    }
} EventGenEWKParticleInfo;

typedef struct EventGenLeptonParticleInfo {
            // Structs that contain information about the event as a whole related to gen-level lepton particles
    GenLeptonEventPointers GLEPs;
    
    vector< vector<GenParticleSt3Info> > vecVecGenLepPSt3I;
    vector< vector<GenParticleSt1Info> > vecVecGenLepPSt1I;
    vector<TString> nameLeps;
    int numSt3Leps;
    int numSt1Leps;
    int numIndLeps;
    /*
    GenParticleSt1Info genElecSt1_0, genElecSt1_1;
    GenParticleSt1Info genMuonSt1_0, genMuonSt1_1;
    
    GenParticleSt3Info genElecSt3_0, genElecSt3_1;
    GenParticleSt3Info genMuonSt3_0, genMuonSt3_1;
    */
    void InitializeVecs() {
        GLEPs.InitializeVecs();                
    }
    void DeleteVecs() {
        GLEPs.DeleteVecs();                
    }
    void SetSize() {
        GLEPs.SetSize();
    }
    void EGLPIDefaultVarVals() {
        numSt3Leps = 2;
        numSt1Leps = 2;
        numIndLeps = 2;
        nameLeps.push_back("Elec");
        nameLeps.push_back("Muon");
        vecVecGenLepPSt3I.resize(numSt3Leps);
        vecVecGenLepPSt1I.resize(numSt1Leps);
        for (int iLepSt3 = 0; iLepSt3 < numSt3Leps; ++iLepSt3) {
            vecVecGenLepPSt3I[iLepSt3].resize(numIndLeps);
        }
        for (int iLepSt1 = 0; iLepSt1 < numSt1Leps; ++iLepSt1) {
            vecVecGenLepPSt1I[iLepSt1].resize(numIndLeps);
        }
    }
    void SetParticleInfo() {
        for (int iLepSt3 = 0; iLepSt3 < numSt3Leps; ++iLepSt3) {
            for (int iIndLep = 0; iIndLep < numIndLeps; ++iIndLep) {
                GLEPs.SetGenParticleInfo_Full(iLepSt3, iIndLep, &vecVecGenLepPSt3I[iLepSt3][iIndLep]);
            }
        }
        
        for (int iLepSt1 = 0; iLepSt1 < numSt1Leps; ++iLepSt1) {
            for (int iIndLep = 0; iIndLep < numIndLeps; ++iIndLep) {
                GLEPs.SetGenParticleInfo_Full(iLepSt1, iIndLep, &vecVecGenLepPSt1I[iLepSt1][iIndLep]);
            }
        }
        /*
        GLEPs.SetGenParticleInfo_Full(0, 0, &genElecSt3_0);
        GLEPs.SetGenParticleInfo_Full(0, 1, &genElecSt3_1);
        GLEPs.SetGenParticleInfo_Full(1, 0, &genMuonSt3_0);
        GLEPs.SetGenParticleInfo_Full(1, 1, &genMuonSt3_1);

        GLEPs.SetGenParticleInfo_Full(0, 0, &genElecSt1_0);
        GLEPs.SetGenParticleInfo_Full(0, 1, &genElecSt1_1);
        GLEPs.SetGenParticleInfo_Full(1, 0, &genMuonSt1_0);
        GLEPs.SetGenParticleInfo_Full(1, 1, &genMuonSt1_1);
         */
    }
} EventGenLeptonParticleInfo;

typedef struct EventGenQuarkParticleInfo {
            // Structs that contain information about the event as a whole related to gen-level quarks
    GenQuarkEventPointers GQEPs;
    
    GenParticleSt3Info genTopSt3_0, genTopSt3_1;
    GenParticleSt3Info genBSt3_0, genBSt3_1;
    GenParticleSt1Info genBSt1_0, genBSt1_1; 
    
    void InitializeVecs() {
        GQEPs.InitializeVecs();
    }    
    void DeleteVecs() {
        GQEPs.DeleteVecs();
    } 
    void SetSize() {
        GQEPs.SetSize();
    }
    void SetParticleInfo() {
//        cout << "test " << endl;
        GQEPs.SetGenParticleInfo_Full(0, 0, &genTopSt3_0);
        GQEPs.SetGenParticleInfo_Full(0, 1, &genTopSt3_1);
//        cout << "test 2" << endl;
        GQEPs.SetGenParticleInfo_Full(1, 0, &genBSt3_0);
        GQEPs.SetGenParticleInfo_Full(1, 1, &genBSt3_1);
//        cout << "test 3" << endl;
        GQEPs.SetGenParticleInfo_Full(1, 0, &genBSt1_0);
//        cout << "test 3a" << endl;
        GQEPs.SetGenParticleInfo_Full(1, 1, &genBSt1_1);
    }    
} EventGenQuarkParticleInfo;

typedef struct EventGenWeight {
    // Structs that contain information about the event as a whole related to gen-level weights calculated based on gen-level information
    std::vector<float> vecGenWeight;
    void SetGenParticleWeightDefault() {
        vecGenWeight.resize(3);
        vecGenWeight[0] = 1.0;
        vecGenWeight[1] = 1.0;
        vecGenWeight[2] = 1.0;
    }
    void SetWeightGenTopPt(GenParticle * GP1, GenParticle * GP2) {
        vecGenWeight.resize(3);
        float expoConst = 0.156;
        float expoSlope = -0.00137;
        //float expoConst = 0.148;
        //float expoSlope = -0.00129;
        float weight0 = TMath::Exp(expoConst);
        float weight1 = TMath::Exp(expoSlope * GP1->P4.Pt());
        float weight2 = TMath::Exp(expoSlope * GP2->P4.Pt());
        vecGenWeight[0] = weight0 * TMath::Sqrt(weight1 * weight2);
        vecGenWeight[1] = 1.0;
        vecGenWeight[2] = vecGenWeight[0]*vecGenWeight[0];
    }
    void SetWeightGenISR(vector<GenParticle *> * vecGP) {
        TLorentzVector ParticleComboVec;
        if (vecGP->size() == 0) {
            cout << "bug: vector size = 0" << endl;
            SetGenParticleWeightDefault();
            return;
        }
        else {
            vecGenWeight.resize(3);
            ParticleComboVec = vecGP->at(0)->P4;   
            for (unsigned int iGP = 1; iGP < vecGP->size(); ++iGP) {
                ParticleComboVec += vecGP->at(iGP)->P4;
            }
        }
        float ParticleComboPt = ParticleComboVec.Pt();
//        cout << "combo pT " << ParticleComboPt << endl;
        if (ParticleComboPt <= 120) {
            vecGenWeight[0] = 1.0;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 1.0;
        }
        else if (ParticleComboPt > 120 && ParticleComboPt <= 150) {
            vecGenWeight[0] = 0.95;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 0.9;
        }
        else if (ParticleComboPt > 150 && ParticleComboPt <= 250) {
            vecGenWeight[0] = 0.9;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 0.8;
        }
        else if (ParticleComboPt > 250) {
            vecGenWeight[0] = 0.8;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 0.6;   
        }
        else {
            cout << "something weird with the particle combo pT " << ParticleComboPt << endl;
        } 
    }
    void SetWeightGenISR(GenParticle * GP1, GenParticle * GP2) {
        vecGenWeight.resize(3);
        TLorentzVector ParticleComboVec = GP1->P4 + GP2->P4;
        float ParticleComboPt = ParticleComboVec.Pt();
        if (ParticleComboPt <= 120) {
            vecGenWeight[0] = 1.0;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 1.0;
        }
        else if (ParticleComboPt > 120 && ParticleComboPt <= 150) {
            vecGenWeight[0] = 0.95;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 0.9;
        }
        else if (ParticleComboPt > 150 && ParticleComboPt <= 250) {
            vecGenWeight[0] = 0.9;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 0.8;
        }
        else if (ParticleComboPt > 250) {
            vecGenWeight[0] = 0.8;
            vecGenWeight[1] = 1.0;
            vecGenWeight[2] = 0.6;   
        }
        else {
            cout << "something weird with the particle combo pT " << ParticleComboPt << endl;
        }
    }
    void SetGenWeight(int genWeightType, EventGenSUSYParticleInfo * inEGSPI, EventGenQuarkParticleInfo * inEGQPI, EventGenEWKParticleInfo * inEGEWKPI) {
//        cout << "genWeight Type " << genWeightType << endl;
        vector<GenParticle* > vecGPs4GenWeight;
        if (genWeightType == 1) {
            SetWeightGenTopPt(&inEGQPI->genTopSt3_0.GP, &inEGQPI->genTopSt3_1.GP);
        }
        else {
            switch (genWeightType) {
                case 0:
                    //SUSY
                    vecGPs4GenWeight.push_back(inEGSPI->GrabGP(0, 0));
                    vecGPs4GenWeight.push_back(inEGSPI->GrabGP(0, 1));
                    //                SetWeightGenISR(inEGSPI->GrabGP(0, 0), inEGSPI->GrabGP(0, 1));                
                    break;
                case 2:
                    // Z+Jets
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(1, 0));
                    break;
                case 3:
                    // WW                    
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(0, 0));
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(0, 1));
                    break;
                case 4:
                    // WZ                    
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(0, 0));
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(1, 0));
                    break;
                case 5:
                    // ZZ                    
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(1, 0));
                    vecGPs4GenWeight.push_back(inEGEWKPI->GrabGP(1, 1));
                    break;
                default:
                    SetGenParticleWeightDefault();
                    return;
                    break;
            }
            SetWeightGenISR(&vecGPs4GenWeight);
        }        
    }
} EventGenWeight;