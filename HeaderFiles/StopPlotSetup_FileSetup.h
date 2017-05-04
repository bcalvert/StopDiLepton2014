/// Functions and typedefs used as part of setting up the stop input files

using namespace std;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;


//would like this to be in better spot

struct CompIndMCParams {
    TString compName;
    TString legEntry;
    Color_t histColor;
    int numIndMC;
    intBounds indMCIB;
    int paramSort;
    void SetAsFake(int numMC, int startPos, bool isFake, bool tightEnumerate) {
        compName  = isFake ? "_Fake" : "_WJ";
        legEntry  = isFake ? "Fake Lep." : "W + Jets";
        histColor = kGreen + 2;
        paramSort = tightEnumerate ? 1 : 2;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsTTBar(int numMC, int startPos, int tightEnumerate) {
        compName  = "_TTBar";
        legEntry  = "t#bar{t}";
        histColor = kRed;
        if (tightEnumerate == 1) {
            paramSort = 3;
        }
        else if (tightEnumerate == 2) {
            paramSort = 2;
        }
        else {
            paramSort = 7;
        }
//        cout << "paramSort " << paramSort << endl;
//        paramSort = tightEnumerate ? 3 : 7;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsZDY(int numMC, int startPos, int tightEnumerate) {
        compName  = "_ZDY";
        legEntry  = "Z/#gamma* #rightarrow l^{+}l^{-}";
        histColor = kBlue;
        if (tightEnumerate == 1) {
            paramSort = 2;
        }
        else if (tightEnumerate == 2) {
            paramSort = 3;
        }
        else {
            paramSort = 5;
        }
//        cout << "paramSort " << paramSort << endl;
//        paramSort = tightEnumerate ? 2 : 5;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsSingTop(int numMC, int startPos) {
        compName  = "_SingTop";
        legEntry  = "Single Top";
        histColor = kRed - 10;
        paramSort = 6;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsVV(int numMC, int startPos) {
        compName  = "_VV";
        legEntry  = "VV";
        histColor = kOrange + 2;
        paramSort = 4;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsVG(int numMC, int startPos) {
        compName  = "_VG";
        legEntry  = "VG";
        histColor = kOrange - 5;
        paramSort = 3;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsHiggs(int numMC, int startPos) {
        compName  = "_Higgs";
        legEntry  = "Higgs";
        histColor = kCyan - 2;
        paramSort = 1;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    void SetAsRare(int numMC, int startPos, bool tightEnumerate) {
        compName  = "_Rare";
        legEntry  = tightEnumerate ? "Others" : "Rare";
        histColor = kGreen + 3;
        paramSort = tightEnumerate ? 0 : 0;
        numIndMC = numMC;
        indMCIB = make_pair(startPos, startPos + numMC - 1);
    }
    
    void SetVecs(vector<int> * vecMCSortParams, vector<TString> * vecMCLegends, vector<Color_t> * vecMCColors, vector<indMCParams> * vecIndMCParams) {
        vecMCSortParams->push_back(paramSort);
        vecMCLegends->push_back(legEntry);
        vecMCColors->push_back(histColor);
        vecIndMCParams->push_back(make_pair(compName, indMCIB));
    }
};

void StopBaseFileNames_Data(vector<TString> * vecBaseFileNames_Data, SampLoadSettings * inSLS) {
    // sets up the input data files
    TString baseName = "Data";
    TString endName = inSLS->doReReco ? "_ReReco" : "";
    TString diLep[3] = {"MuMu", "EE", "EMu"};
    if (inSLS->whichDilepType > -1) {
        vecBaseFileNames_Data->push_back(baseName + diLep[inSLS->whichDilepType] + endName);
    }
    else {
        for (int iDiLep = 0; iDiLep < 3; ++iDiLep) {
            vecBaseFileNames_Data->push_back(baseName + diLep[iDiLep] + endName);
        }
    }
}
void StopBaseFileNames_MC_TTBar(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for TTBar processes
    const int numOviTTBar = 1;
    TString fileInNameTTBar[numOviTTBar] = {"TTBarComp"};
    
    const int numOviTTBarIndivDilep = 3;
    TString fileInNameTTBarIndivDilep[numOviTTBarIndivDilep] = {"TTBarComp_MuMu", "TTBarComp_EE", "TTBarComp_EMu"};
    
    //    int numTTBarToUse = numOviTTBar;
    //    TString * arrToUseTTBar = fileInNameTTBar;
    int numTTBarToUse = inSLS->whichDilepType > -1 ? numOviTTBar : numOviTTBarIndivDilep;
    TString * arrToUseTTBar = inSLS->whichDilepType > -1 ? fileInNameTTBar : fileInNameTTBarIndivDilep;
    
    for (int iIndTTBar = 0; iIndTTBar < numTTBarToUse; ++iIndTTBar) {
        vecBaseFileNames_MC->push_back(arrToUseTTBar[iIndTTBar]);
    }
}

void StopBaseFileNames_MC_ZDY(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for DY processes
    const int numOviDY = 1;
    TString fileInNameDY[numOviDY] = {"ZDY"};
    
    const int numOviDYIndivDilep = 3;
    TString fileInNameDYIndivDilep[numOviDYIndivDilep] = {"ZDY_MuMu", "ZDY_EE", "ZDY_EMu"};
    
    int numDYToUse = inSLS->whichDilepType > -1 ? numOviDY: numOviDYIndivDilep;
    TString * arrToUseDY = inSLS->whichDilepType > -1 ? fileInNameDY : fileInNameDYIndivDilep;
    
    for (int iIndDY = 0; iIndDY < numDYToUse; ++iIndDY) {
        vecBaseFileNames_MC->push_back(arrToUseDY[iIndDY]);
    }
}

void StopBaseFileNames_MC_Other(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for Other processes
    const int numOviOther = 1;
    TString fileInNameOther[numOviOther] = {"SingleTop"};
    
    int numOtherToUse = numOviOther;
    TString * arrToUseOther = fileInNameOther;
    
    for (int iIndOther = 0; iIndOther < numOtherToUse; ++iIndOther) {
        vecBaseFileNames_MC->push_back(arrToUseOther[iIndOther]);
    }
}

void StopBaseFileNames_MC_Fake(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for FakeLep processes
    const int numOviFake = 1;
    TString fileInNameFake[numOviFake] = {"WToLNu"};
    const int numOviFakeDDIndivDilep = 3;
    TString fileInNameFakeDDIndivDilep[numOviFakeDDIndivDilep] = {"FakeLep_MuMu", "FakeLep_EE", "FakeLep_EMu"};
    
    const int numOviFakeDD = 1;
    TString fileInNameFakeDD[numOviFakeDD] = {"FakeLep"};
    
    int numFakeToUse;
    TString * arrToUseFake;
    
    if (!inSLS->doDropFakes) {
        numFakeToUse = numOviFake;
        arrToUseFake = fileInNameFake;
    }
    else {
        if (inSLS->whichDilepType != -1) {
            numFakeToUse = numOviFakeDD;
            arrToUseFake = fileInNameFakeDD;
        }
        else {
            numFakeToUse = numOviFakeDDIndivDilep;
            arrToUseFake = fileInNameFakeDDIndivDilep;
        }
    }
    
    for (int iIndFake = 0; iIndFake < numFakeToUse; ++iIndFake) {
        vecBaseFileNames_MC->push_back(arrToUseFake[iIndFake]);
    }
}

void StopBaseFileNames_MC_VV(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for VV processes
    const int numOviVV = 3;
    TString fileInNameVV[numOviVV] = {"WW", "WZ", "ZZ"};
    
    int numVVToUse = numOviVV;
    TString * arrToUseVV = fileInNameVV;
    
    for (int iIndVV = 0; iIndVV < numVVToUse; ++iIndVV) {
        vecBaseFileNames_MC->push_back(arrToUseVV[iIndVV]);
    }
}

void StopBaseFileNames_MC_VG(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for VG processes
    const int numOviVG = 2;
    TString fileInNameVG[numOviVG] = {"WG", "ZG"};
    
    int numVGToUse = numOviVG;
    TString * arrToUseVG = fileInNameVG;
    
    for (int iIndVG = 0; iIndVG < numVGToUse; ++iIndVG) {
        vecBaseFileNames_MC->push_back(arrToUseVG[iIndVG]);
    }
}

void StopBaseFileNames_MC_Higgs(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for Higgs processes
    const int numOviHiggs = 3;
    TString fileInNameHiggs[numOviHiggs] = {"HiggsWW", "HiggsVBF", "HiggsZZ4L"};
    
    int numHiggsToUse = numOviHiggs;
    TString * arrToUseHiggs = fileInNameHiggs;
    
    for (int iIndHiggs = 0; iIndHiggs < numHiggsToUse; ++iIndHiggs) {
        vecBaseFileNames_MC->push_back(arrToUseHiggs[iIndHiggs]);
    }
}

void StopBaseFileNames_MC_Rare(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    //Names for Rare processes
    const int numOviRare = 2;
    TString fileInNameRare[numOviRare] = {"TripVecBoson", "TTBarVecBoson"};
    
    int numRareToUse = numOviRare;
    TString * arrToUseRare = fileInNameRare;
    
    for (int iIndRare = 0; iIndRare < numRareToUse; ++iIndRare) {
        vecBaseFileNames_MC->push_back(arrToUseRare[iIndRare]);
    }
}

void StopBaseFileNames_MC(vector<TString> * vecBaseFileNames_MC, SampLoadSettings * inSLS) {
    // sets up the input MC SM background files
    
    StopBaseFileNames_MC_TTBar(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_ZDY(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_Fake(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_Other(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_VV(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_VG(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_Higgs(vecBaseFileNames_MC, inSLS);
    StopBaseFileNames_MC_Rare(vecBaseFileNames_MC, inSLS);
    /*
     for (int iVec = 0; iVec < vecBaseFileNames_MC->size(); ++iVec) {
     cout << "name " << vecBaseFileNames_MC->at(iVec) << endl;
     }
     */
}


void StopBaseFileNames_DataShape(vector<TString> * vecBaseFileNames_Data) {
    vecBaseFileNames_Data->push_back("Data");
}

void StopBaseFileNames_MCShape(vector<TString> * vecBaseFileNames_MC) {
    vecBaseFileNames_MC->push_back("TTBar");
    /*
    vecBaseFileNames_MC->push_back("NonTTBar");
    vecBaseFileNames_MC->push_back("NonTTBar");
    vecBaseFileNames_MC->push_back("NonTTBar");
    vecBaseFileNames_MC->push_back("NonTTBar");
    vecBaseFileNames_MC->push_back("NonTTBar");
    vecBaseFileNames_MC->push_back("NonTTBar");
    vecBaseFileNames_MC->push_back("NonTTBar");
    */
    vecBaseFileNames_MC->push_back("ZDY");
    vecBaseFileNames_MC->push_back("Fake");
    vecBaseFileNames_MC->push_back("SingTop");
    vecBaseFileNames_MC->push_back("VV");
    vecBaseFileNames_MC->push_back("VG");
    vecBaseFileNames_MC->push_back("Higgs");
    vecBaseFileNames_MC->push_back("Rare");
    
}

void StopBaseFileNames_SignalShape(vector<TString> * vecBaseFileNames_Signal, SampLoadSettings * inSLS, bool doVerb = false) {
    // sets up the input MC SIgnal files
    if (doVerb) {
        cout << "inSLS Signal information:" << endl;
        inSLS->PrintSignalInfo();
    }
    for (unsigned int iSig = 0; iSig < inSLS->vecStopMassGrab.size(); ++iSig) {
        if (doVerb) {
            cout << "for iSig = " << iSig << ", name to push back " << inSLS->NameSigFile(iSig) << endl;
        }
        vecBaseFileNames_Signal->push_back(inSLS->NameSigFile(iSig));
    }
}

void StopBaseFileNames_Signal(vector<TString> * vecBaseFileNames_Signal, SampLoadSettings * inSLS) {
    // sets up the input MC SIgnal files
    for (unsigned int iSig = 0; iSig < inSLS->vecStopMassGrab.size(); ++iSig) {
        vecBaseFileNames_Signal->push_back(inSLS->SignalString(iSig));
    }
}

void SampleStartPositionsNames(vector<int> * vecMCSortParams, vector<TString> * vecMCLegends, vector<Color_t> * vecMCColors, vector<indMCParams> * vecIndMCParams, SampLoadSettings * inSLS, int whichIndMCSort, bool isShape = false) {
    // Dictates the bounds to determine which individual MC files get grouped into what composite backgrounds
    // along with their resultant names
    
    int numTTbar            = inSLS->whichDilepType > -1 ? 1 : 3; // (inSLS->doExcSamps && inSLS->whichTTbarGen == 0) ? 2 : 1;
    int numVV               = 3;
    int numSingTop          = 1;
    int numFake             = inSLS->doDropFakes ? (inSLS->whichDilepType > -1 ? 1 : 3) : 1;
    
    int numZDY              = inSLS->whichDilepType > -1 ? 1 : 3;
    int numVG               = 2;
    int numHiggs            = 3;
    int numRare             = 2;
    
    if (isShape) {
        numTTbar = 1;
        numVV = 1;
        numSingTop = 1;
        numFake = 1;
        numZDY = 1;
        numVG = 1;
        numHiggs = 1;
        numRare = 1;
    }
    
    int non_TTBar_DY_Fake = numVV + numSingTop + numVG + numHiggs + numRare;
    
    int TTbarStart          = 0;
    int ZDYStart            = TTbarStart + numTTbar;
    int FakeStart           = ZDYStart + numZDY;
    int SingTopStart        = FakeStart + numFake;
    int VVStart             = SingTopStart + numSingTop;
    int VGStart             = VVStart + numVV;
    int HiggsStart          = VGStart + numVG;
    int RareStart           = HiggsStart + numHiggs;
    
    int tightEnumerate = whichIndMCSort;
    //== 1;
    
    if (tightEnumerate >= 1) {
        TTbarStart = 0;
        ZDYStart            = TTbarStart + numTTbar;
        FakeStart           = ZDYStart + numZDY;
        RareStart           = FakeStart + numFake;
        numRare = non_TTBar_DY_Fake;
    }

    CompIndMCParams CompTTBar;
    CompTTBar.SetAsTTBar(numTTbar, TTbarStart, tightEnumerate);
    CompTTBar.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
    
    CompIndMCParams CompZDY;
    CompZDY.SetAsZDY(numZDY, ZDYStart, tightEnumerate);
    CompZDY.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
    
    CompIndMCParams CompFake;
    CompFake.SetAsFake(numFake, FakeStart, inSLS->doDropFakes, tightEnumerate);
    CompFake.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
    
    CompIndMCParams CompRare;
    CompRare.SetAsRare(numRare, RareStart, tightEnumerate);
    CompRare.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
    if (!tightEnumerate) {
        CompIndMCParams CompSingTop;
        CompSingTop.SetAsSingTop(numSingTop, SingTopStart);
        CompSingTop.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
        
        CompIndMCParams CompVV;
        CompVV.SetAsVV(numVV, VVStart);
        CompVV.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
        
        CompIndMCParams CompVG;
        CompVG.SetAsVG(numVG, VGStart);
        CompVG.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
        
        CompIndMCParams CompHiggs;
        CompHiggs.SetAsHiggs(numHiggs, HiggsStart);
        CompHiggs.SetVecs(vecMCSortParams, vecMCLegends, vecMCColors, vecIndMCParams);
    }
}
