// Structs used as part of plotting the final sets of histograms
using namespace std;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;
typedef pair<float, float> valPlusErr;
inline TString SystStringForPlotShape(int whichSyst = 0) {
    TString suffixSyst[8] = {"", "LepES", "JetES", "BTagSF", "JetSmear", "UncES", "LepEffSF", "genRecoilRW"};
    ////    cout << "suffixSyst[7] " << suffixSyst[7] << endl;
    //    cout << "suffixSyst[8] " << suffixSyst[8] << endl;
    //    cout << "suffixSyst[9] " << suffixSyst[9] << endl;
    TString outString = "";
    if (whichSyst == 0) {
        return outString;
    }
    else {
        outString = "_";
        outString += suffixSyst[abs(whichSyst)];
    }
    return outString;
}
inline TString SystStringForPlot(int whichSyst = 0) {
    TString suffixSyst[9] = {"", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "JetSmear", "UncES", "LepEffSF", "genRecoilRW"};
    ////    cout << "suffixSyst[7] " << suffixSyst[7] << endl;
    //    cout << "suffixSyst[8] " << suffixSyst[8] << endl;
    //    cout << "suffixSyst[9] " << suffixSyst[9] << endl;
    TString outString = "";
    if (whichSyst == 0) {
        return outString;
    }
    else {
        outString = "_";
        outString += suffixSyst[abs(whichSyst)];       
    }
    return outString;
}

inline void AddFakeLepSysts(vector<TString> * vecSystString) {
    TString suffixFakeLepSyst[3] = {"_FakeLepStat", "_FakeLepFakeRateSyst", "_FakeLepPromptRateSyst"};
    for (int iSystFake = 0; iSystFake < 3; ++iSystFake) {
        vecSystString->push_back(suffixFakeLepSyst[iSystFake]);
    }
}
inline void AddNormSysts(vector<TString> * vecSystString) {
    TString suffixNormSyst[3] = {"_TTBarNorm", "_DYNorm", "_EWKNorm"};
    for (int iSystNorm = 0; iSystNorm < 3; ++iSystNorm) {
        vecSystString->push_back(suffixNormSyst[iSystNorm]);
    }
}
void MakeSystVec(vector<TString> * vecSystString, bool isSmear = false) {
    const int numSysts = 8;
    int indexSystJetSmear = 5;
    int indexSystUncES = 6;
    for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
        if (!isSmear && (iSyst == indexSystJetSmear || iSyst == indexSystUncES)) continue;
        vecSystString->push_back(SystStringForPlot(iSyst));
    }
    AddFakeLepSysts(vecSystString);
    AddNormSysts(vecSystString);
}

void MakeSystVecShape(vector<TString> * vecSystString, bool isSmear = false) {
    const int numSysts = 7;
    int indexSystJetSmear = 4;
    int indexSystUncES = 5;
    for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
        if (!isSmear && (iSyst == indexSystJetSmear || iSyst == indexSystUncES)) continue;
        vecSystString->push_back(SystStringForPlotShape(iSyst));
    }
    AddFakeLepSysts(vecSystString);
    AddNormSysts(vecSystString);
}

typedef struct SpecificWeight {
    TH1F * weightHistCV;
    vector<TH1F *> weightHist_SystVarUp;
    vector<TH1F *> weightHist_SystVarDown;
    void SetHistDefault(TString weightName, vector<TString> * vecSystString) {
        TString baseHistName = "h_Weight";
        baseHistName += weightName;
        TString xAxisName = "patsy";
        TString yAxisName = "Scale Factor";
        TString axisTitle = "; ";
        axisTitle += xAxisName;
        axisTitle += "; ";
        axisTitle += yAxisName;
        int nBins = 1;
        float xLB = 0.5;
        float xUB = 1.5;
        weightHistCV = new TH1F(baseHistName + TString("_CV"), axisTitle, nBins, xLB, xUB);
        
        
        int numSysts = (int) vecSystString->size();
        weightHist_SystVarUp.resize(numSysts);
        weightHist_SystVarDown.resize(numSysts);
        
        for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
            weightHist_SystVarUp[iSyst] = new TH1F(baseHistName + vecSystString->at(iSyst) + TString("ShiftUp"), axisTitle, nBins, xLB, xUB);
            weightHist_SystVarDown[iSyst] = new TH1F(baseHistName + vecSystString->at(iSyst) + TString("ShiftDown"), axisTitle, nBins, xLB, xUB);
        }
    }
    void SetSF(float SF, int whichSyst = 0) {
        TH1F * histToSet;
        if (whichSyst == 0) {
            histToSet = weightHistCV;
        }
        else if (whichSyst > 0) {
            histToSet = weightHist_SystVarUp[abs(whichSyst) - 1];
        }
        else if (whichSyst < 0) {
            histToSet = weightHist_SystVarDown[abs(whichSyst) - 1];
        }
        histToSet->SetBinContent(1, SF);
    }
    void SetSF(valPlusErr * inVPE, int whichSyst = 0) {
        TH1F * histToSet;
        if (whichSyst == 0) {
            histToSet = weightHistCV;
        }
        else if (whichSyst > 0) {
            histToSet = weightHist_SystVarUp[abs(whichSyst) - 1];
        }
        else if (whichSyst < 0) {
            histToSet = weightHist_SystVarDown[abs(whichSyst) - 1];
        }
        histToSet->SetBinContent(1, inVPE->first);
        histToSet->SetBinError(1, inVPE->second);
    }
    void SetSF(ValError * inVE, int whichSyst = 0) {
        TH1F * histToSet;
        if (whichSyst == 0) {
            histToSet = weightHistCV;
        }
        else if (whichSyst > 0) {
            histToSet = weightHist_SystVarUp[abs(whichSyst) - 1];
        }
        else if (whichSyst < 0) {
            histToSet = weightHist_SystVarDown[abs(whichSyst) - 1];
        }
        histToSet->SetBinContent(1, inVE->centVal);
        histToSet->SetBinError(1, inVE->upError);
    }
    float GetSF(int whichSyst = 0, bool doVerbosity = false) {
        float outSF;
        TH1F * histToGetSFFrom;
        if (whichSyst == 0) {
            histToGetSFFrom = weightHistCV;
        }
        else {
            if (abs(whichSyst) - 1 >= (int) weightHist_SystVarUp.size()) {
                if (doVerbosity) {
                    cout << "continuing for whichSyst " << whichSyst << " because it's as big as the size of the weightVec " << weightHist_SystVarUp.size() << endl;
                }
                return 1;
            }
            if (whichSyst > 0) {
                histToGetSFFrom = weightHist_SystVarUp[abs(whichSyst) - 1];
            }
            if (whichSyst < 0) {
                histToGetSFFrom = weightHist_SystVarDown[abs(whichSyst) - 1];
            }
        }
        outSF = histToGetSFFrom->GetBinContent(1);
        return outSF;
    }
    float GetSFError(int whichSyst = 0) {
        float outSF;
        TH1F * histToGetSFFrom;
        if (whichSyst == 0) {
            histToGetSFFrom = weightHistCV;
        }
        else if (whichSyst > 0) {
            histToGetSFFrom = weightHist_SystVarUp[abs(whichSyst) - 1];
        }
        else if (whichSyst < 0) {
            histToGetSFFrom = weightHist_SystVarDown[abs(whichSyst) - 1];
        }
        outSF = histToGetSFFrom->GetBinError(1);
        return outSF;
    }
    void WriteToFile(TFile * outputFile) {
        outputFile->cd();
        weightHistCV->Write();
        for (unsigned int iSyst = 0; iSyst < weightHist_SystVarUp.size(); ++iSyst) {
            weightHist_SystVarUp[iSyst]->Write();
            weightHist_SystVarDown[iSyst]->Write();
        }
    }
    void SetHistsFromFile(TFile * outputFile, TString weightName, vector<TString> * vecSystString) {
        TString baseHistName = "h_Weight";
        baseHistName += weightName;
        
        TString grabName = baseHistName + "_CV";
        
        weightHistCV = (TH1F *) outputFile->Get(grabName);
        
        int numSysts = (int) vecSystString->size();
//        weightHist_SystVarUp.resize(numSysts);
//        weightHist_SystVarDown.resize(numSysts);
        weightHist_SystVarUp.resize(0);
        weightHist_SystVarDown.resize(0);
        
        for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
            if (vecSystString->at(iSyst).Contains("Norm")) {
                continue;
            }
            grabName = baseHistName + vecSystString->at(iSyst) + TString("ShiftUp");
//            weightHist_SystVarUp[iSyst] = (TH1F *) outputFile->Get(grabName);
            weightHist_SystVarUp.push_back((TH1F *) outputFile->Get(grabName));
            grabName = baseHistName + vecSystString->at(iSyst) + TString("ShiftDown");
//            weightHist_SystVarDown[iSyst] = (TH1F *) outputFile->Get(grabName);
            weightHist_SystVarDown.push_back((TH1F *) outputFile->Get(grabName));
        }
    }
    void PrintWeights() {
        TH1F * histToPrint = weightHistCV;
        cout << "Weight for Hist " << histToPrint->GetName() << " is " << GetSF(0) << " with a stat error of " << GetSFError(0) << endl;
        for (int iSyst = 1; iSyst <= (int) weightHist_SystVarUp.size(); ++iSyst) {
            histToPrint = weightHist_SystVarUp[iSyst - 1];
            cout << "Weight for Hist " << histToPrint->GetName() << " is " << GetSF(iSyst) << " with a stat error of " << GetSFError(iSyst) << endl;
            
            histToPrint = weightHist_SystVarDown[iSyst - 1];
            cout << "Weight for Hist " << histToPrint->GetName() << " is " << GetSF(-1 * iSyst) << " with a stat error of " << GetSFError(-1 * iSyst) << endl;
        }
    }
} SpecificWeight;
typedef struct WeightCalculators {
    float SFLumi;
    int intLumi;
    
    float xSecUncertEWK;
    
    vector<TString> vecChanNames;
    vector<TString> vecTTBarNames, vecDYNames;
    
    vector<SpecificWeight> vecWeightTTBar_Dilep;
    vector<SpecificWeight> vecWeightDY_Dilep;
    
    TString ChanName(int whichTTBarDDEst) {
        const int numChans = 10;
        TString arrChanName[numChans] = {"_FullCut", "_FullCutTTBarControl", "_FullCut", "_FullCut_LowBLepMass", "_FullCut2BJets_LowBLepMass", "_FullCutMETSig1D_LowBLepMass", "_FullCutMETSigTrue_LowBLepMass", "_FullCutMETSig2D_LowBLepMass", "_FullCut_0BJets_LowBLepMass", "_FullCut2BJets"};
        return arrChanName[whichTTBarDDEst];
    }
    void SetTTBarNames() {
        vecTTBarNames.resize(0);
        vecTTBarNames.push_back("_FullCut");
        vecTTBarNames.push_back("_FullCutTTBarControl");
        vecTTBarNames.push_back("_TheoryNorm");
        vecTTBarNames.push_back("_FullCut_LowBLepMass");
        vecTTBarNames.push_back("_FullCut2BJets_LowBLepMass");
        vecTTBarNames.push_back("_FullCutMETSig1D_LowBLepMass");
        vecTTBarNames.push_back("_FullCutMETSigTrue_LowBLepMass");
        vecTTBarNames.push_back("_FullCutMETSig2D_LowBLepMass");
        vecTTBarNames.push_back("_FullCut_0BJets_LowBLepMass");
        vecTTBarNames.push_back("_FullCut2BJets");
    }
    void SetDYNames() {
        vecDYNames.resize(0);
        vecDYNames.push_back("_Dilep");
        vecDYNames.push_back("_Jet2");
        vecDYNames.push_back("_Jet2MET");
        vecDYNames.push_back("_Jet2BJet1");
        vecDYNames.push_back("_Jet2BJet1MET");
        vecDYNames.push_back("_ZCR");
        vecDYNames.push_back("_Jet2BJet2");
        vecDYNames.push_back("_Jet2BJet2MET");
    }
    void SetChanNames() {
        vecChanNames.resize(0);
        vecChanNames.push_back("_MuMu");
        vecChanNames.push_back("_EE");
        vecChanNames.push_back("_EMu");
    }
    
    TString GetWeightName(int whichWeightType, int versNumber, bool useMET = true) {
        vector< vector<TString> * > vecVecToUse(0);
        vecVecToUse.push_back(&vecTTBarNames);
        vecVecToUse.push_back(&vecDYNames);
        
        TString arrBaseName[2] = {"_TTBarMT2ControlNormalization", "DY_ROutIn"};
        
        if (whichWeightType == 1 && !useMET && (versNumber == 2 || versNumber == 4)) {
            cout << "rolling back to a non-MET cut SF " << endl;
            versNumber -= 1;
        }
        TString weightName = arrBaseName[whichWeightType] + vecVecToUse[whichWeightType]->at(versNumber);
        return weightName;
    }
    vector<SpecificWeight> * GetVecSW(int whichWeightType) {
        if (whichWeightType == 0) {
            return &vecWeightTTBar_Dilep;
        }
        else if (whichWeightType == 1) {
            return &vecWeightDY_Dilep;
        }
        return NULL;
    }
    void LoadWeights(SampLoadSettings * inSLS, vector<TString> * vecSystString, int whichWeightType, int versNumber) {
        //debug statements
        vector<int> vecDebugInts(0);
        vecDebugInts.push_back(0);
        vecDebugInts.push_back(1);
        DebugStatement(whichWeightType, &vecDebugInts, "whichWeightType", "LoadWeights");

        
        TString weightNameMuMu = GetWeightName(whichWeightType, versNumber, true);
        TString weightNameEE = GetWeightName(whichWeightType, versNumber, true);
        TString weightNameEMu = GetWeightName(whichWeightType, versNumber, false);
        
        TString arrNames[3] = {"_MuMu", "_EE", "_EMu"};
//        TString arrNames[3] = {"All", "All", "All"};
        TString addString = "";
        if (whichWeightType == 0) {
            addString = inSLS->TTBarString();
        }
    
        addString += inSLS->SampleStringScaleFacSaving();
        
        TString nameFileMuMu = "ScaleFactors";
        nameFileMuMu += weightNameMuMu;
        if (whichWeightType == 0) {
            nameFileMuMu += arrNames[0];
        }
        nameFileMuMu += addString;
        
        TString nameFileEE = "ScaleFactors";
        nameFileEE += weightNameEE;
        if (whichWeightType == 0) {
            nameFileEE += arrNames[1];
        }
        nameFileEE += addString;
        
        TString nameFileEMu = "ScaleFactors";
        nameFileEMu += weightNameEMu;
        if (whichWeightType == 0) {
            nameFileEMu += arrNames[2];
        }
        nameFileEMu += addString;
        
        cout << "going to try and grab MuMu File: " << nameFileMuMu << endl;
        cout << "going to try and grab EE File: " << nameFileEE << endl;
        cout << "going to try and grab EMu File: " << nameFileEMu << endl;
        
        TFile * inFileMuMu = TFile::Open(nameFileMuMu);
        TFile * inFileEE = TFile::Open(nameFileEE);
        TFile * inFileEMu = TFile::Open(nameFileEMu);
        
        vector<SpecificWeight> * vecSWToUse = GetVecSW(whichWeightType);
        vecSWToUse->resize(vecChanNames.size());
        
        TFile * fileToGrabFrom;
        TString weightNameToUse;
        
        for (unsigned int iChan = 0; iChan < vecSWToUse->size(); ++iChan) {
            //set which file to grab from based on dilepton type
            switch (iChan) {
                case 0:
                    fileToGrabFrom = inFileMuMu;
                    weightNameToUse = weightNameMuMu;
                    break;
                case 1:
                    fileToGrabFrom = inFileEE;
                    weightNameToUse = weightNameEE;
                    break;
                case 2:
                    fileToGrabFrom = inFileEMu;
                    weightNameToUse = weightNameEMu;
                    break;
            }
            /*
            if (whichWeightType == 0) {
                cout << "hacking that ish for ttbar " << endl;
                vecSWToUse->at(iChan).SetHistsFromFile(fileToGrabFrom, weightNameToUse + "All", vecSystString);
            }
            else {
                vecSWToUse->at(iChan).SetHistsFromFile(fileToGrabFrom, weightNameToUse + vecChanNames[iChan], vecSystString);
            }
            */
            vecSWToUse->at(iChan).SetHistsFromFile(fileToGrabFrom, weightNameToUse + vecChanNames[iChan], vecSystString);
            cout << "printing " << vecChanNames[iChan] << " weights " << endl;
            vecSWToUse->at(iChan).PrintWeights();
        }
    }
    void DefaultVarVals() {
        SFLumi = 1.0;
        xSecUncertEWK = 0.5; //50% xsec uncertainty on EWK backgrounds
        SetTTBarNames();
        SetDYNames();
        SetChanNames();
    }
    void SetIntLumi() {
        float indLumiOvi[4] = {876, 4404, 7016, 7360};
        intLumi = 0.0;
        for (int iLumi = 0; iLumi < 4; ++iLumi) {
            intLumi += indLumiOvi[iLumi];
        }
    }
    void SetUniWeights(SampLoadSettings * inSLS) {
        SetIntLumi();
    }
    float ScaleBackCalc(TFile * inputFile, bool doVerbosity = false, int whichSyst = 0, vector<TString> * vecSystString = 0) {
        const int numBadSysts = 5;
        TString mcplot = "h_nVtx_inclusive";
        TString mcplot_preRW = "h_nVtx_preRW_inclusive";
        TString badSysts[numBadSysts] = {"JetSmear", "UncES", "genStop", "Norm", "Fake"};
        TString fileName = inputFile->GetName();
        bool isBadSyst = false;
        if (whichSyst != 0) {
            if (doVerbosity) {
                cout << "name of syst: " << vecSystString->at(abs(whichSyst) - 1) << endl;
            }
            for (int iBadSyst = 0; iBadSyst < numBadSysts; ++iBadSyst) {
                if (vecSystString->at(abs(whichSyst) - 1).Contains(badSysts[iBadSyst])) {
                    isBadSyst = true;
                }
            }
            if (!isBadSyst) {
                mcplot += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                mcplot_preRW += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                if (whichSyst > 0) {
                    mcplot += "Up";
                    mcplot_preRW += "Up";
                }
                else {
                    mcplot += "Down";
                    mcplot_preRW += "Down";
                }
            }
        }
        if (doVerbosity) {
            cout << "from file " << fileName << endl;
            cout << "mcplot to grab " << mcplot << endl;
            cout << "mcplot_preRW to grab " << mcplot_preRW << endl;
        }
        
        TH1F * nVtxOrigHist;
        TH1F * nVtxNewHist;
        int NBinsX;
        float scaleBack;
        
        nVtxOrigHist = (TH1F*) inputFile->Get(mcplot_preRW);
        nVtxNewHist = (TH1F*) inputFile->Get(mcplot);
        NBinsX = nVtxOrigHist->GetNbinsX();
        scaleBack = (float) nVtxOrigHist->Integral(1, NBinsX + 1) / nVtxNewHist->Integral(1, NBinsX + 1);
        return scaleBack;
    }
    float ScaleBackCalcBasic(TFile * inputFile, bool doVerbosity = false, int whichSyst = 0, vector<TString> * vecSystString = 0) {
        TString mcplot = "h_nVtx_inclusive";
        TString mcplot_preRW = "h_BasicWeightIntegral_inclusive";
        
        TString fileName = inputFile->GetName();
        const int numBadSysts = 4;
        TString badSysts[numBadSysts] = {"JetSmear", "UncES", "Norm", "Fake"};
        bool isBadSyst = false;
        if (whichSyst != 0) {
            for (int iBadSyst = 0; iBadSyst < numBadSysts; ++iBadSyst) {
                if (vecSystString->at(abs(whichSyst) - 1).Contains(badSysts[iBadSyst])) {
                    isBadSyst = true;
                }
            }
            if (vecSystString->at(abs(whichSyst) - 1).Contains("genStop")) {
                if ((fileName.Contains("T2tt") || fileName.Contains("T2bw"))) {
                    mcplot += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                    mcplot_preRW += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                    if (whichSyst > 0) {
                        mcplot += "Up";
                        mcplot_preRW += "Up";
                    }
                    else {
                        mcplot += "Down";
                        mcplot_preRW += "Down";
                    }
                }
            }
            else {
                if (!isBadSyst) {
                    mcplot += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                    mcplot_preRW += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                    if (whichSyst > 0) {
                        mcplot += "Up";
                        mcplot_preRW += "Up";
                    }
                    else {
                        mcplot += "Down";
                        mcplot_preRW += "Down";
                    }
                }
            }
        }
        if (doVerbosity) {
            cout << "from file " << fileName << endl;
            cout << "mcplot to grab " << mcplot << endl;
            cout << "mcplot_preRW to grab " << mcplot_preRW << endl;
        }
        TH1F * nVtxOrigHist;
        TH1F * nVtxNewHist;
        int NBinsX, NBinsXNew;
        float scaleBack;
        nVtxOrigHist = (TH1F*) inputFile->Get(mcplot_preRW);
        nVtxNewHist = (TH1F*) inputFile->Get(mcplot);
        NBinsX = nVtxOrigHist->GetNbinsX();
        NBinsXNew = nVtxNewHist->GetNbinsX();
        scaleBack = (float) nVtxOrigHist->Integral(1, NBinsX + 1) / nVtxNewHist->Integral(1, NBinsXNew + 1);
        return scaleBack;
    }
    float ScaleBackCalc_Recoil(TFile * inputFile, bool doVerbosity = false, int whichSyst = 0, vector<TString> * vecSystString = 0) {
        TString mcplot = "h_nVtx_inclusive";        
        TString fileName = inputFile->GetName();
        if (whichSyst != 0) {
            if (vecSystString->at(abs(whichSyst) - 1).Contains("genRecoil")) {
                mcplot += vecSystString->at(abs(whichSyst) - 1) + TString("Shift");
                if (whichSyst > 0) {
                    mcplot += "Up";
                }
                else {
                    mcplot += "Down";
                }
            }
        }
        TString mcplot_preRW = "h_nVtx_inclusive_genRecoilRWShiftUp";
        if (doVerbosity) {
            cout << "from file " << fileName << endl;
            cout << "mcplot to grab " << mcplot << endl;
            cout << "mcplot_preRW to grab " << mcplot_preRW << endl;
        }
        TH1F * RecoilOrigHist;
        TH1F * RecoilNewHist;
        int NBinsX;
        float scaleBack;
        RecoilOrigHist = (TH1F*) inputFile->Get(mcplot_preRW);
        RecoilNewHist = (TH1F*) inputFile->Get(mcplot);
        NBinsX = RecoilOrigHist->GetNbinsX();
        scaleBack = (float) RecoilOrigHist->Integral(1, NBinsX + 1) / RecoilNewHist->Integral(1, NBinsX + 1);
        return scaleBack;
    }
    SpecificWeight * PickSW(IndSamplePlotInfo * inISPI, SampLoadSettings * inSLS, vector<SpecificWeight> * vecSW) {
        int indexMuMu = 0;
        int indexEE = 1;
        int indexEMu = 2;
        if (inSLS->whichDilepType == 0 || inISPI->nameISPI.Contains("_MuMu")) {
            return &vecSW->at(indexMuMu);
        }
        else if (inSLS->whichDilepType == 1 || inISPI->nameISPI.Contains("_EE")) {
            return &vecSW->at(indexEE);
        }
        else if (inSLS->whichDilepType == 2 || inISPI->nameISPI.Contains("_EMu")) {
            return &vecSW->at(indexEMu);
        }
        else {
            cout << "inSLS->whichDilepType " << inSLS->whichDilepType << endl;
            cout << "inISPI->nameISPI " << inISPI->nameISPI << endl;
            cout << "returning NULL! in PickSW Function " << endl;
        }
        return NULL;
    }
    void SetISPIDDWeight(IndSamplePlotInfo * inISPI, SampLoadSettings * inSLS, bool doTTBar = false, bool doDY = false, bool doVerbosity = false) {
        SpecificWeight * SWtoUse;
        TString nameWeight;
        if ((doTTBar && inISPI->sampleType == 1) || (doDY && inISPI->sampleType == 2)) {
            if (doTTBar && inISPI->sampleType == 1) {
                nameWeight = "TTBar";
                SWtoUse = PickSW(inISPI, inSLS, &vecWeightTTBar_Dilep);
            }
            else if (doDY && inISPI->sampleType == 2) {
                nameWeight = "DY";
                SWtoUse = PickSW(inISPI, inSLS, &vecWeightDY_Dilep);
            }
            inISPI->weight_CentVal *= SWtoUse->GetSF(0, doVerbosity);
            inISPI->weight_CentVal_StatErr = SWtoUse->GetSFError(0);
            if (doVerbosity) {
                cout << "base " << nameWeight << " weight " << SWtoUse->GetSF(0, doVerbosity) << endl;
            }
            for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
                if (doVerbosity) {
                    cout << " for iSyst " << iSyst << endl;
                    cout << "weight " << nameWeight << " Syst Var Up " << SWtoUse->GetSF(iSyst, doVerbosity) << endl;
                    cout << "weight " << nameWeight << " Syst Var Down " << SWtoUse->GetSF(- 1 * iSyst, doVerbosity) << endl;
                }
                inISPI->weight_SystVarUp[iSyst - 1] *= SWtoUse->GetSF(iSyst, doVerbosity);
                inISPI->weight_SystVarDown[iSyst - 1] *= SWtoUse->GetSF(- 1 * iSyst, doVerbosity);
            }
        }
    }
    void SetISPIWeightSignal(IndSamplePlotInfo * inISPI, vector<float> * inVecSignalSkimEfficiencyCalc = 0, int whichSigIndex = 0) {
        inISPI->weight_CentVal *= inVecSignalSkimEfficiencyCalc->at(whichSigIndex);
        for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
            inISPI->weight_SystVarUp[iSyst - 1] *= inVecSignalSkimEfficiencyCalc->at(whichSigIndex);
            inISPI->weight_SystVarDown[iSyst - 1] *= inVecSignalSkimEfficiencyCalc->at(whichSigIndex);
        }
    }
    void SetISPIWeightNVtxRecoil(IndSamplePlotInfo * inISPI, bool doVerbosity = false, vector<TString> * vecSystString = 0) {
        inISPI->weight_CentVal *= ScaleBackCalc(inISPI->inputFile, doVerbosity);
        inISPI->weight_CentVal *= ScaleBackCalc_Recoil(inISPI->inputFile, doVerbosity);
        for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
            inISPI->weight_SystVarUp[iSyst - 1] *= ScaleBackCalc(inISPI->inputFile, doVerbosity, iSyst, vecSystString);
            inISPI->weight_SystVarUp[iSyst - 1] *= ScaleBackCalc_Recoil(inISPI->inputFile, doVerbosity, -1 * iSyst, vecSystString);
            
            inISPI->weight_SystVarDown[iSyst - 1] *= ScaleBackCalc(inISPI->inputFile, doVerbosity, iSyst, vecSystString);
            inISPI->weight_SystVarDown[iSyst - 1] *= ScaleBackCalc_Recoil(inISPI->inputFile, doVerbosity, - 1 * iSyst, vecSystString);
        }
    }
    void SetISPIWeightBasicWeight(IndSamplePlotInfo * inISPI, bool doVerbosity = false, vector<TString> * vecSystString = 0) {
        inISPI->weight_CentVal *= ScaleBackCalcBasic(inISPI->inputFile, doVerbosity);
        for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
            inISPI->weight_SystVarUp[iSyst - 1] *= ScaleBackCalcBasic(inISPI->inputFile, doVerbosity, iSyst, vecSystString);
            
            inISPI->weight_SystVarDown[iSyst - 1] *= ScaleBackCalcBasic(inISPI->inputFile, doVerbosity, -1 * iSyst, vecSystString);
        }
    }
    void SetISPIWeight(IndSamplePlotInfo * inISPI, SampLoadSettings * inSLS, bool useDDEstTTBar, bool useDDEstDY, bool doVerbosity = false, int numSysts = 0, vector<TString> * vecSystString = 0, vector<float> * inVecSignalSkimEfficiencyCalc = 0, int whichSigIndex = 0) {
        bool useDDEst = (useDDEstDY || useDDEstTTBar);
        inISPI->DefaultWeights(numSysts);
        if (inISPI->sampleType != 0 && inISPI->sampleType != 100) {
            if (useDDEst) {
                SetISPIDDWeight(inISPI, inSLS, useDDEstTTBar, useDDEstDY, doVerbosity);
            }
            if (inISPI->sampleType < 0) {
                SetISPIWeightSignal(inISPI, inVecSignalSkimEfficiencyCalc, whichSigIndex);
                SetISPIWeightBasicWeight(inISPI, doVerbosity, vecSystString);
            }
            else {
                SetISPIWeightNVtxRecoil(inISPI, doVerbosity, vecSystString);
            }
            if (inISPI->sampleType == 3) {
                cout << "for inISPI " << inISPI->nameISPI << " setting stat uncert. on weight to be " << xSecUncertEWK << endl;
                inISPI->weight_CentVal_StatErr = xSecUncertEWK;
            }
        }
    }    
} WeightCalculators;

typedef struct HistoStrings {
    TString plotVarName;
    TString nameToGrabData, nameToGrabMC;
    TString canvName, mcStackName;
    vector<TString> vecMultiChannelAddStrings_Data, vecMultiChannelAddStrings_MC;
    bool doSystCurrPlot;
    void PrintInfo() {
        cout << "plotVarName " << plotVarName << endl;
        cout << "nameToGrabData " << nameToGrabData << endl;
        cout << "nameToGrabMC " << nameToGrabMC << endl;
        cout << "canvName " << canvName << endl;
    }
    void SetBaseStuff(HistogramT * inHistT, SampLoadSettings * inSLS) {
        plotVarName = "";
        plotVarName += inHistT->name;
        plotVarName.Replace(0, 2, "");
        
        nameToGrabData = inHistT->name;
        nameToGrabMC = inHistT->name;
        if (inSLS->SmearedPlots) {
            nameToGrabData.Replace(nameToGrabData.Index("Smear"), 5,"");
        }
        
        canvName = "c_";
        canvName += plotVarName;
        mcStackName = "mcStack_";
        mcStackName += plotVarName;
    }
    void SetBaseStuff(TString histName, SampLoadSettings * inSLS) {
        plotVarName = "";
        plotVarName += histName;
        plotVarName.Replace(0, 2, "");
        
        nameToGrabData = histName;
        nameToGrabMC = histName;
        if (inSLS->SmearedPlots) {
            nameToGrabData.Replace(nameToGrabData.Index("Smear"), 5,"");
        }
        
        canvName = "c_";
        canvName += plotVarName;
        mcStackName = "mcStack_";
        mcStackName += plotVarName;
    }
    void AddToNames(TString subSampName) {
        vecMultiChannelAddStrings_Data.resize(0);
        vecMultiChannelAddStrings_MC.resize(0);
        
        nameToGrabData += subSampName;
        nameToGrabMC += subSampName;
        canvName += subSampName;
        mcStackName += subSampName;        
    }
    void AddToNames(SampleT * inSampT) {
        vecMultiChannelAddStrings_Data.resize(0);
        vecMultiChannelAddStrings_MC.resize(0);
        
        TString subSampName = "";
        subSampName += inSampT->histNameSuffix;
        
        nameToGrabData += subSampName;
        nameToGrabMC += subSampName;
        canvName += subSampName;
        mcStackName += subSampName;
    }
    
    void AddToNames(TString multiChannelCompOutputName, vector<SampleT> * inVecSampT, vector<int> * vecMultiChannelIDs) {
        vecMultiChannelAddStrings_Data.resize(0);
        vecMultiChannelAddStrings_MC.resize(0);
        
        canvName += multiChannelCompOutputName;
        mcStackName += multiChannelCompOutputName;
        
        SetMultiChannelStrings(&vecMultiChannelAddStrings_Data, inVecSampT, vecMultiChannelIDs, nameToGrabData);
        SetMultiChannelStrings(&vecMultiChannelAddStrings_MC, inVecSampT, vecMultiChannelIDs, nameToGrabMC);
    }

} HistoStrings;


typedef struct PlotMakingStructs {
    vector< vector<HistogramT> *> vecVecHistTtoUse;
    vector<SampleT> * subSampVec;
    
    vector<int> multiChannelIDs;
    vector<int> multiHistListIDs;
    
    vector<TString> vecSystNames;
    int numSysts;
    
    vector<Color_t> vecMCColors;
    vector<TString> vecMCLegends;
    vector<int> vecMCSortParams;
    vector<indMCParams> vecIndMCParams;
    
    vector<vector<TFile *> > vecVecTFiles;
    
    THStack * indMCStack;
    
    vector<float> vecCanvasCornerParams, vecCanvasSizeParams;
    TCanvas * plottingCanvas;
    
    
    //structs for MT2CutYieldCalc.C
    vector<float> vecXaxisCut, vecYaxisCut;
    vector<TString> vecHistNamesForYieldCalc;
    ////Set of functions
    void SetFileNameVector(TString saveNameDim, vector<TFile *> * inVecTFile, int index, bool doNonSig, bool doSig) {
        TString strAppend = saveNameDim + ".root";
        if (doNonSig) {
            if (index == 0) {
                inVecTFile->at(0) = new TFile("Data" + strAppend, "RECREATE");
            }
            else if (index == 1) {
                inVecTFile->at(0) = new TFile("TTBar" + strAppend, "RECREATE");
                inVecTFile->at(1) = new TFile("NonTTBar" + strAppend, "RECREATE");
                if (doSig) {
                    inVecTFile->at(2) = new TFile("Signal" + strAppend, "RECREATE");
                }
            }
        }
        else if (doSig) {
            inVecTFile->at(0) = new TFile("Signal" + strAppend, "RECREATE");
        }
        else {
            cout << "not saving any TFiles because you've specified both no 'non-signal' and no 'signal'!" << endl;
        }
    }
    
    void CloseFiles() {
        for (unsigned int iVec = 0; iVec < vecVecTFiles.size(); ++iVec) {
            for (unsigned int iVec2= 0; iVec2 < vecVecTFiles[iVec].size(); ++iVec2) {
                vecVecTFiles[iVec][iVec2]->Write();
                vecVecTFiles[iVec][iVec2]->Close();
            }
        }
    }
    
    void SetUpTFiles(TString saveNameDim, bool doNonSig, bool doSig) {
        if (doNonSig) {
            vecVecTFiles.resize(2);
            vecVecTFiles[0].resize(1); // one output data file
            SetFileNameVector(saveNameDim, &vecVecTFiles[0], 0, doNonSig, doSig);
            if (doSig) {
                vecVecTFiles[1].resize(3); // Three output MC files
            }
            else {
                vecVecTFiles[1].resize(2); // Two output MC files
            }
            SetFileNameVector(saveNameDim, &vecVecTFiles[1], 1, doNonSig, doSig);
        }
        else if (doSig) {
            vecVecTFiles.resize(2);
            vecVecTFiles[1].resize(1); // Two output MC files
            SetFileNameVector(saveNameDim, &vecVecTFiles[1], 0, doNonSig, doSig);
        }
        else {
            cout << "not saving any TFiles because you've specified both no 'non-signal' and no 'signal'!" << endl;
            vecVecTFiles.resize(0);
        }
    }
    //Functions for Yield calculation
    void SetCutValues(HistPlotMaking * inHPM, int levelVerbosity = 0) {
        vecXaxisCut.resize(0);
        vecYaxisCut.resize(0);
        inHPM->SetCutValues(&vecXaxisCut, &vecYaxisCut, levelVerbosity);
    }
    void SetYieldHistNames(SampLoadSettings * inSLS, HistPlotMaking * inHPM, int levelVerbosity = 0) {
        vecHistNamesForYieldCalc.resize(0);
        SetCutValues(inHPM, levelVerbosity);
        inSLS->SetupMultiHistList_MultiChanList_MT2CutYield(&vecHistNamesForYieldCalc, inHPM->tryCalcPassByHand, &vecXaxisCut, &vecYaxisCut, levelVerbosity);
    }
    
    void DoPassCut(HistogramDisplayStructs * inHDS, TString compName, vector<indMCParams> * vecIndMCParams, int whichIntType, int whichSet = 0, bool doSyst = true, int levelVerbosity = 0) {
        vector<int> vecCutValues;
        vector<TString> vecCutVarNames;
        vecCutValues.push_back((int) vecXaxisCut[whichSet]);
        vecCutVarNames.push_back("MT2ll");
        if (vecYaxisCut.size() > 0) {
            vecCutValues.push_back((int) vecYaxisCut[whichSet]);
            vecCutVarNames.push_back("MT2lblb");
        }        
        inHDS->DoPassCutHisto(&vecCutValues, &vecCutVarNames, whichIntType, compName, vecIndMCParams, &vecSystNames, doSyst, levelVerbosity);
    }
    ///
    
    void SetVecIndMCParams(SampLoadSettings * inSLS, int whichIndMCSort, bool isShape = false) {
        vecIndMCParams.resize(0);
        vecMCColors.resize(0);
        vecMCLegends.resize(0);
        vecMCSortParams.resize(0);
        SampleStartPositionsNames(&vecMCSortParams, &vecMCLegends, &vecMCColors, &vecIndMCParams, inSLS, whichIndMCSort, isShape);
    }
    
    void DefaultMultiChanHistVecs() {
        multiChannelIDs.resize(0);
        multiHistListIDs.resize(0);
    }
    
    void SetVecSystNames(bool isSmear = false) {
        vecSystNames.resize(0);
        MakeSystVec(&vecSystNames, isSmear);
        numSysts = (int) vecSystNames.size();
    }
    
    void SetVecSystNamesShape(bool isSmear = false) {
        vecSystNames.resize(0);
        MakeSystVecShape(&vecSystNames, isSmear);
        numSysts = (int) vecSystNames.size();
    }
    
    void SetCanvasParams() {
        float m = 1.3;
        int n_ = 1;
        Double_t e_ = 15*m; //15
        Double_t k_ = 7*m; //15
        Double_t t_ = 30*m; //30
        Double_t b_ = 50*m; //80
        Double_t g_ = 87*m; //87
        Double_t d_ = 30*m; //30
        Double_t h_ = 350*m; //400
        Double_t w_ = 350*m; //350
        Double_t hl_ = 70*m;
        
        //    Double_t ww_ = g_ + w_ + e_ ;
        Double_t W_ = g_ + n_*w_ + 2*(n_-1)*e_ + d_;
        Double_t H_ = t_ + h_ + b_ + hl_ +2*k_ ;
        Int_t wtopx = 100;
        Int_t wtopy = 150;
        
        vecCanvasCornerParams.resize(0);
        vecCanvasCornerParams.push_back(wtopx);
        vecCanvasCornerParams.push_back(wtopy);
        
        vecCanvasSizeParams.resize(0);
        vecCanvasSizeParams.push_back(W_);
        vecCanvasSizeParams.push_back(H_);
    }
    void NewCanvas(TString canvName) {
        plottingCanvas = new TCanvas(canvName, canvName, vecCanvasCornerParams[0], vecCanvasCornerParams[1], vecCanvasSizeParams[0], vecCanvasSizeParams[1]);
    }
    void NewMCStack(TString stackName) {
        indMCStack = new THStack(stackName, "");
    }
    void SetHistTVecs(SampLoadSettings * inSLS) {
        //Set up the mapping of input axis label variables to full out labels;
        labelMap mapVartoLabel;
        SetStringKey_StSMap_Composite(mapVartoLabel);
        //Set up the Default Stop Hist Binnings and Bounds
        StopHistBinBounds basicSHBB; basicSHBB.DefaultVarVals();
        
        int numSpaceDimensions = 3;    
        vector< vector<HistogramT> *> vecVecHistT_Inclusive; vecVecHistT_Inclusive.resize(numSpaceDimensions);
        vector< vector<HistogramT> *> vecVecHistT_MET_noType0; vecVecHistT_MET_noType0.resize(numSpaceDimensions);
        for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
            vecVecHistT_Inclusive[iDim] = new vector<HistogramT>;
            SetHistTVec_Inclusive(vecVecHistT_Inclusive[iDim], &basicSHBB, &mapVartoLabel, iDim + 1, inSLS->doSignal);
            vecVecHistT_MET_noType0[iDim] = new vector<HistogramT>;
            //SetHistTVec_MET_noType0(vecVecHistT_MET_noType0[iDim], &basicSHBB, &mapVartoLabel, iDim + 1);
        }
        
        vector< vector<HistogramT> *> vecVecHistT_Inclusive_Smear;
        vector< vector<HistogramT> *> vecVecHistT_MET_noType0_Smear;
        
        vecVecHistT_Inclusive_Smear.resize(numSpaceDimensions);
        vecVecHistT_MET_noType0_Smear.resize(numSpaceDimensions);
        
        for (int iDim = 0; iDim < numSpaceDimensions; ++iDim) {
            vecVecHistT_Inclusive_Smear[iDim] = new vector<HistogramT>;
            SetHistTVec_Inclusive_Smeared(vecVecHistT_Inclusive_Smear[iDim], &basicSHBB, &mapVartoLabel, iDim + 1, inSLS->doSignal);
            
            vecVecHistT_MET_noType0_Smear[iDim] = new vector<HistogramT>;
            //SetHistTVec_MET_noType0_Smeared(vecVecHistT_MET_noType0_Smear[iDim], &basicSHBB, &mapVartoLabel, iDim + 1);
        }
        
        if (inSLS->SmearedPlots) {
            if (inSLS->noType0) {
                vecVecHistTtoUse = vecVecHistT_MET_noType0_Smear;
            }
            else {
                vecVecHistTtoUse = vecVecHistT_Inclusive_Smear;
            }
        }
        else {
            if (inSLS->noType0) {
                vecVecHistTtoUse = vecVecHistT_MET_noType0;
            }
            else {
                vecVecHistTtoUse = vecVecHistT_Inclusive;
            }
        }
    }
    
    void SetStructs(SampLoadSettings * inSLS, int whichIndMCSort, bool isShape = false) {
        int whichSSType = inSLS->doSignal ? 2 : -1;
        subSampVec = new vector<SampleT>;
        SetSubSampVec(subSampVec, whichSSType);
        SetHistTVecs(inSLS);
        SetCanvasParams();
        if (isShape) {
            SetVecSystNamesShape(inSLS->SmearedPlots);
        }
        else {
            SetVecSystNames(inSLS->SmearedPlots);
        }
        SetVecIndMCParams(inSLS, whichIndMCSort, isShape);
    }
    
} PlotMakingStructs;



