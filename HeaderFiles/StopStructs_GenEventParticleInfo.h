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
    int TypeT2Decay() {
        int indexCharg = 2;
        bool realChargino_0 = vecVecGenSUSYPSt3I[indexCharg][0].GP_Index > 0;
        bool realChargino_1 = vecVecGenSUSYPSt3I[indexCharg][1].GP_Index > 0;
        
        int typeDecay = -1;
        if (realChargino_0 && realChargino_1) {
            typeDecay = 2; //T2bw -- stop -> b + chargino both sides
        }
        else if (!realChargino_0 && !realChargino_1) {
            typeDecay = 0; //T2tt -- stop -> top + neutralino both sides, so no charginos
        }
        else {
            typeDecay = 1; //T2tb -- asymmetric decay, technically T2tb also has T2tt and T2bw like modes
        }
        return typeDecay;
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
    
    bool PassMassCutPlotMaker(int grabStopMass, int grabChi0Mass, float grabCharginoMassFrac, float massDiffThresh, float CharginoMassFracDiffThresh, bool isT2tt, bool doVerb = false) {
        bool passesCut = true;
        //        cout << "vecVecGenSUSYRoundMass[0][0] " << vecVecGenSUSYRoundMass[0][0] << endl;
        //        cout << "vecVecGenSUSYRoundMass[1][0] " << vecVecGenSUSYRoundMass[1][0] << endl;
        for (int iIndSUSY = 0; iIndSUSY < 2; ++iIndSUSY) {
            if (doVerb) {
                cout << "vecVecGenSUSYRoundMass[0][iIndSUSY] " << vecVecGenSUSYRoundMass[0][iIndSUSY] << endl;
                cout << "grabStopMass " << grabStopMass << endl;
                cout << "vecVecGenSUSYRoundMass[1][iIndSUSY] " << vecVecGenSUSYRoundMass[1][iIndSUSY] << endl;
                cout << "grabChi0Mass " << grabChi0Mass << endl;
                cout << "isT2tt " << isT2tt << endl;
            }
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
        if (StopRightPolPercent >= 0) {
            return GSEPs.genStopPolarizationWeights->at(StopRightPolPercent/10);
        }
        return 1.;
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
    void PrintInfo(int whichLepType, int whichLepPos, int whichLepStat) {
        GenParticleSt1Info * GPSt1toPrint;
        GenParticleSt3Info * GPSt3toPrint;
        if (whichLepStat == 1) {
            GPSt1toPrint = &vecVecGenLepPSt1I[whichLepType][whichLepPos];
            GPSt1toPrint->PrintInfo();
        }
        else if (whichLepStat == 3) {
            GPSt3toPrint = &vecVecGenLepPSt3I[whichLepType][whichLepPos];
            GPSt3toPrint->PrintInfo();
        }
        else {
            cout << "whichLepStat needs to be either 1 or 3 -- it is " << whichLepStat << endl;
        }
    }
    void SetLepPointers(GenParticle * &inGPLep0, GenParticle * &inGPLep1) {
        //try electrons first
        inGPLep0 = vecVecGenLepPSt1I[0][0].GetLepFromW();
        inGPLep1 = vecVecGenLepPSt1I[0][1].GetLepFromW();
        if (inGPLep0 == NULL) {
            if (inGPLep1 == NULL) {
                // try muons
                inGPLep0 = vecVecGenLepPSt1I[1][0].GetLepFromW();
                inGPLep1 = vecVecGenLepPSt1I[1][1].GetLepFromW();
            }
            else {
                //vecVecGenLepPSt1I[0][0] is NULL and inGPLep1 = vecVecGenLepPSt1I[0][1];
                //try one muon
                inGPLep0 = vecVecGenLepPSt1I[1][0].GetLepFromW();
                if (inGPLep0 == NULL) {
                    //try other muon
                    inGPLep0 = vecVecGenLepPSt1I[1][1].GetLepFromW();
                }
            }
        }
        else {
            //inGPLep0 = vecVecGenLepPSt1I[0][0];
            if (inGPLep1 == NULL) {
                //try one muon
                inGPLep1 = vecVecGenLepPSt1I[1][0].GetLepFromW();
                if (inGPLep1 == NULL) {
                    //try other muon
                    inGPLep1 = vecVecGenLepPSt1I[1][1].GetLepFromW();
                }
            }
        }
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
    void PrintInfo(int whichQuarkType, int whichQuarkPos, int whichQuarkStat = 1) {
        GenParticleSt1Info * GPSt1toPrint;
        GenParticleSt3Info * GPSt3toPrint;
        if (whichQuarkType == 0) {
            GPSt3toPrint = whichQuarkPos == 0 ? &genTopSt3_0 : &genTopSt3_1;
            GPSt3toPrint->PrintInfo();
        }
        else if (whichQuarkType == 1) {
            if (whichQuarkStat == 1) {
                GPSt1toPrint = whichQuarkPos == 0 ? &genBSt1_0: &genBSt1_1;
                GPSt1toPrint->PrintInfo();
            }
            else {
                GPSt3toPrint = whichQuarkPos == 0 ? &genBSt3_0: &genBSt3_1;
                GPSt3toPrint->PrintInfo();
            }
        }
        else {
            cout << "whichQuarkType must be either 0 or 1 -- it is " << whichQuarkType << endl;
        }
    }
    void SetGenBPointers(GenParticle * &inGPB0, GenParticle * &inGPB1) {
        inGPB0 = genBSt1_0.GetBQuarkFromTopOrChargino();
        inGPB1 = genBSt1_1.GetBQuarkFromTopOrChargino();
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

struct EventGenMT2Info {
    float genMT2ll;
    float genMT2bb, genMT2bb_ZMET, genMT2bb_ZMET_RecoB;
    vector<float> vecGenMT2lblb;
    vector<float> vecDPhiGenBLeps;
    float TotBLepMass;
    vector< vector<float> > vecVecGenBLepMass;
    vector< vector<float> > vecVecGenBLepDPhi;
    vector< vector<float> > vecVecGenBLepDEta;
    void InitializeVecs() {
        vecGenMT2lblb.resize(2);
        vecDPhiGenBLeps.resize(2);
        vecVecGenBLepMass.resize(2);
        vecVecGenBLepDPhi.resize(2);
        vecVecGenBLepDEta.resize(2);
        for (int iCorr = 0; iCorr < 2; ++iCorr) {
            vecVecGenBLepMass[iCorr].resize(2);
            vecVecGenBLepDPhi[iCorr].resize(2);
            vecVecGenBLepDEta[iCorr].resize(2);
        }
    }
    void SetBadMT2ll() {
        genMT2ll = -9999.;
    }
    void SetBadMT2bb() {
        genMT2bb = -9999.;
        genMT2bb_ZMET = -9999.;
        genMT2bb_ZMET_RecoB = -9999.;
    }
    void SetBadMT2lblb() {
        TotBLepMass = -9999.;
        for (int iCorr = 0; iCorr < 2; ++iCorr) {
            vecGenMT2lblb[iCorr] = -9999.;
            vecDPhiGenBLeps[iCorr] = -9999.;
            for (int iPair = 0; iPair < 2; ++iPair) {
                vecVecGenBLepMass[iCorr][iPair] = -9999.;
                vecVecGenBLepDPhi[iCorr][iPair] = -9999.;
                vecVecGenBLepDEta[iCorr][iPair] = -9999.;
            }
        }
    }
    void DefaultVarVals() {
        SetBadMT2ll();
        SetBadMT2lblb();
    }
    bool checkOppositePDGIDPair(GenParticle * inGP0, GenParticle * inGP1) {
        int signPair = TMath::Sign(1, inGP0->PDGID * inGP1->PDGID);
        return (signPair < 0);
    }
    void SetGenMT2ll(EventGenLeptonParticleInfo * inEGLPI, float genMET, float genMETPhi) {
        GenParticle * GPLep0, * GPLep1;
        inEGLPI->SetLepPointers(GPLep0, GPLep1);
        if (GPLep0 == NULL || GPLep1 == NULL) {
            SetBadMT2ll();
        }
        else if (!checkOppositePDGIDPair(GPLep0, GPLep1)){
            // not opposite sign dilepton
            SetBadMT2ll();
        }
        else {
            genMT2ll = MT2::getMT2(GPLep0->P4, GPLep0->P4, genMET, genMETPhi, false);
            /*
             cout << "genMT2ll " << genMT2ll << endl;
             */
        }
    }
    void SetGenMT2bb(EventGenLeptonParticleInfo * inEGLPI, EventGenQuarkParticleInfo * inEGQPI, float genMET, float genMETPhi, vector<TLorentzVector>  * inVecJetsMT2bb) {

        GenParticle * GPLep0, * GPLep1;
        GenParticle * GPB0, * GPB1;
        inEGLPI->SetLepPointers(GPLep0, GPLep1);
        inEGQPI->SetGenBPointers(GPB0, GPB1);
        
        TLorentzVector ZMET;
        if (GPLep0 == NULL || GPLep1 == NULL || GPB0 == NULL || GPB1 == NULL) {
            SetBadMT2bb();
        }
        else if (!checkOppositePDGIDPair(GPLep0, GPLep1)) {
            //not opposite sign dilepton
            SetBadMT2bb();
        }
        else if (!checkOppositePDGIDPair(GPB0, GPB1)) {
            // not opposite sign b quarks
            SetBadMT2bb();
        }
        else {
            float testMass = 80.4;
            TLorentzVector MET; MET.SetPtEtaPhiM(genMET, 0., genMETPhi, 0);
            ZMET = MET + GPLep0->P4 + GPLep1->P4;
            genMT2bb = MT2::getMT2(GPB0->P4, GPB1->P4, genMET, genMETPhi, false);
            genMT2bb_ZMET = MT2::getMT2(GPB0->P4, GPB1->P4, ZMET.Pt(), ZMET.Phi(), false, testMass);
	    if (inVecJetsMT2bb->size() < 2) {
	      genMT2bb_ZMET_RecoB = -9999;
	    }
	    else {
	      genMT2bb_ZMET_RecoB = MT2::getMT2(inVecJetsMT2bb->at(0), inVecJetsMT2bb->at(1), ZMET.Pt(), ZMET.Phi(), false, testMass);
	    }
            /*
             cout << "genMT2ll " << genMT2ll << endl;
             */
        }
    }

    void SetGenMT2lblb(EventGenLeptonParticleInfo * inEGLPI, EventGenQuarkParticleInfo * inEGQPI, float genMET, float genMETPhi) {
        GenParticle * GPLep0, * GPLep1;
        GenParticle * GPB0, * GPB1;
        TLorentzVector BLep0_Corr, BLep1_Corr;
        TLorentzVector BLep0_Incorr, BLep1_Incorr;
        inEGLPI->SetLepPointers(GPLep0, GPLep1);
        inEGQPI->SetGenBPointers(GPB0, GPB1);
        if (GPLep0 == NULL || GPLep1 == NULL || GPB0 == NULL || GPB1 == NULL) {
            SetBadMT2lblb();
        }
        else if (!checkOppositePDGIDPair(GPLep0, GPLep1)) {
            //not opposite sign dilepton
        }
        else if (!checkOppositePDGIDPair(GPB0, GPB1)) {
            // not opposite sign b quarks
        }
        else {
            //Try pairing one
            if (checkOppositePDGIDPair(GPLep0, GPB0)) {
                //pairing of leads is good
                if (!checkOppositePDGIDPair(GPLep1, GPB1)) {
                    cout << "something weird with checkOppositePDGIDPair for second pairing, it's bad when it should be good " << endl;
                }
                else {
                    //pairings of 0 with 0 and 1 with 1 are the correct ones
                    BLep0_Incorr = GPLep0->P4 + GPB1->P4;
                    BLep1_Incorr = GPLep1->P4 + GPB0->P4;
                    vecVecGenBLepDPhi[0][0] = dPhi(&GPLep0->P4, &GPB1->P4);
                    vecVecGenBLepDPhi[0][1] = dPhi(&GPLep1->P4, &GPB0->P4);
                    vecVecGenBLepDEta[0][0] = GPLep0->P4.Eta() - GPB1->P4.Eta();
                    vecVecGenBLepDEta[0][1] = GPLep1->P4.Eta() - GPB0->P4.Eta();
                    
                    BLep0_Corr = GPLep0->P4 + GPB0->P4;
                    BLep1_Corr = GPLep1->P4 + GPB1->P4;
                    vecVecGenBLepDPhi[1][0] = dPhi(&GPLep0->P4, &GPB0->P4);
                    vecVecGenBLepDPhi[1][1] = dPhi(&GPLep1->P4, &GPB1->P4);
                    vecVecGenBLepDEta[1][0] = GPLep0->P4.Eta() - GPB0->P4.Eta();
                    vecVecGenBLepDEta[1][1] = GPLep1->P4.Eta() - GPB1->P4.Eta();
                }
            }
            else {
                //pairing of leads is not good
                if (checkOppositePDGIDPair(GPLep1, GPB1)) {
                    cout << "something weird with checkOppositePDGIDPair for second pairing, it's good when it should be bad " << endl;
                }
                else {
                    //pairings of 0 with 1 and 1 with 0 are the correct ones
                    BLep0_Incorr = GPLep0->P4 + GPB0->P4;
                    BLep1_Incorr = GPLep1->P4 + GPB1->P4;
                    vecVecGenBLepDPhi[0][0] = dPhi(&GPLep0->P4, &GPB0->P4);
                    vecVecGenBLepDPhi[0][1] = dPhi(&GPLep1->P4, &GPB1->P4);
                    vecVecGenBLepDEta[0][0] = GPLep0->P4.Eta() - GPB0->P4.Eta();
                    vecVecGenBLepDEta[0][1] = GPLep1->P4.Eta() - GPB1->P4.Eta();
                    
                    BLep0_Corr = GPLep0->P4 + GPB1->P4;
                    BLep1_Corr = GPLep1->P4 + GPB0->P4;
                    vecVecGenBLepDPhi[1][0] = dPhi(&GPLep0->P4, &GPB1->P4);
                    vecVecGenBLepDPhi[1][1] = dPhi(&GPLep1->P4, &GPB0->P4);
                    vecVecGenBLepDEta[1][0] = GPLep0->P4.Eta() - GPB1->P4.Eta();
                    vecVecGenBLepDEta[1][1] = GPLep1->P4.Eta() - GPB0->P4.Eta();
                }
            }
            vecVecGenBLepMass[0][0] = BLep0_Incorr.M();
            vecVecGenBLepMass[0][1] = BLep1_Incorr.M();
            vecVecGenBLepMass[1][0] = BLep0_Corr.M();
            vecVecGenBLepMass[1][1] = BLep1_Corr.M();
            vecDPhiGenBLeps[0] = dPhi(BLep0_Incorr.Phi(), BLep1_Incorr.Phi());
            vecDPhiGenBLeps[1] = dPhi(BLep0_Corr.Phi(), BLep1_Corr.Phi());
            
            TotBLepMass = (BLep0_Corr + BLep1_Corr).M();
            vecGenMT2lblb[0] = MT2::getMT2(BLep0_Incorr, BLep1_Incorr, genMET, genMETPhi, false);
            vecGenMT2lblb[1] = MT2::getMT2(BLep0_Corr, BLep1_Corr, genMET, genMETPhi, false);
        }
    }
};
