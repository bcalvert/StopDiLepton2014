// Structs used as part of plotting the final sets of histograms
using namespace std;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;
typedef pair<float, float> valPlusErr;
inline TString SystStringForPlot(int whichSyst = 0) {
    TString suffixSyst[10] = {"", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "JetSmear", "UncES", "LepEffSF", "genRecoilRW", "genStopXSec"};
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
void MakeSystVec(vector<TString> * vecSystString, bool isSmear = false) {
    const int numSysts = 8;
    int indexSystJetSmear = 5;
    int indexSystUncES = 6;
    for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
        if (!isSmear && (iSyst == indexSystJetSmear || iSyst == indexSystUncES)) continue;
        vecSystString->push_back(SystStringForPlot(iSyst));
    }
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
    float GetSF(int whichSyst = 0) {
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
        outSF = histToGetSFFrom->GetBinContent(1);
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
        weightHist_SystVarUp.resize(numSysts);
        weightHist_SystVarDown.resize(numSysts);
        
        for (int iSyst = 0; iSyst < numSysts; ++iSyst) {
            grabName = baseHistName + vecSystString->at(iSyst) + TString("ShiftUp");
            weightHist_SystVarUp[iSyst] = (TH1F *) outputFile->Get(grabName);
            grabName = baseHistName + vecSystString->at(iSyst) + TString("ShiftDown");
            weightHist_SystVarDown[iSyst] = (TH1F *) outputFile->Get(grabName);
        }
    }
    void PrintWeights() {
        TH1F * histToPrint = weightHistCV;
        cout << "Weight for Hist " << histToPrint->GetName() << " is " << GetSF(0) << endl;
        for (int iSyst = 1; iSyst <= (int) weightHist_SystVarUp.size(); ++iSyst) {
            histToPrint = weightHist_SystVarUp[iSyst - 1];
            cout << "Weight for Hist " << histToPrint->GetName() << " is " << GetSF(iSyst) << endl;
            
            histToPrint = weightHist_SystVarDown[iSyst - 1];
            cout << "Weight for Hist " << histToPrint->GetName() << " is " << GetSF(-1 * iSyst) << endl;
        }
    }
} SpecificWeight;
typedef struct WeightCalculators {
    float SFLumi;
    int intLumi;
    
    vector<TString> vecChanNames;
    vector<TString> vecTTBarNames, vecDYNames;
    
    vector<SpecificWeight> vecWeightTTBar_Dilep;
    vector<SpecificWeight> vecWeightDY_Dilep;
    
    SpecificWeight weightTTBar_DDNormEE, weightTTBar_DDNormEMu, weightTTBar_DDNormMuMu;
    SpecificWeight weightDY_DDNormEE, weightDY_DDNormEMu, weightDY_DDNormMuMu;
    
    void SetTTBarNames() {
        vecTTBarNames.resize(0);
        vecTTBarNames.push_back("_FullCut");
    }
    void SetDYNames() {
        vecDYNames.resize(0);
        vecDYNames.push_back("_Dilep");
        vecDYNames.push_back("_Jet2");
        vecDYNames.push_back("_Jet2MET");
        vecDYNames.push_back("_Jet2BJet1");
        vecDYNames.push_back("_Jet2BJet1MET");
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

        TString weightNameSF = GetWeightName(whichWeightType, versNumber, true);
        TString weightNameOF = GetWeightName(whichWeightType, versNumber, false);
        
        TString addString = inSLS->TTBarString();
        addString += inSLS->SampleStringScaleFacSaving();
        
        TString nameFileSF = "ScaleFactors";
        nameFileSF += weightNameSF;
        nameFileSF += addString;

        TString nameFileOF = "ScaleFactors";
        nameFileOF += weightNameOF;
        nameFileOF += addString;
        
        cout << "going to try and grab SF File: " << nameFileSF << endl;
        cout << "going to try and grab OF File: " << nameFileOF << endl;
        
        TFile * inFileSF = TFile::Open(nameFileSF);
        TFile * inFileOF = TFile::Open(nameFileOF);
        
        vector<SpecificWeight> * vecSWToUse = GetVecSW(whichWeightType);
        vecSWToUse->resize(vecChanNames.size());
        
        TFile * fileToGrabFrom;
        TString weightNameToUse;
        
        for (unsigned int iChan = 0; iChan < vecSWToUse->size(); ++iChan) {
            //set which file to grab from based on dilepton type
            fileToGrabFrom = inFileSF;
            weightNameToUse = weightNameSF;
            if (iChan == 2) {
                fileToGrabFrom = inFileOF;
                weightNameToUse = weightNameOF;
            }
            vecSWToUse->at(iChan).SetHistsFromFile(fileToGrabFrom, weightNameToUse + vecChanNames[iChan], vecSystString);
            cout << "printing " << vecChanNames[iChan] << " weights " << endl;
            vecSWToUse->at(iChan).PrintWeights();
        }
    }
    void DefaultVarVals() {
        SFLumi = 1.0;
        SetTTBarNames();
        SetDYNames();
        SetChanNames();
    }
    void SetIntLumi() {
        float indLumiOvi[4] = {892, 4404, 7032, 7274};
        intLumi = 0.0;
        for (int iLumi = 0; iLumi < 4; ++iLumi) {
            intLumi += indLumiOvi[iLumi];
        }
    }
    void SetUniWeights(SampLoadSettings * inSLS) {
        SetIntLumi();
    }
    float ScaleBackCalc(TFile * inputFile, bool doVerbosity = false, int whichSyst = 0, vector<TString> * vecSystString = 0) {
        TString mcplot = "h_nVtx_inclusive";
        TString mcplot_preRW = "h_nVtx_preRW_inclusive";
        TString badSysts[3] = {"JetSmear", "UncES", "genStop"};
        TString fileName = inputFile->GetName();
        bool isBadSyst = false;
        if (whichSyst != 0) {
            for (int iBadSyst = 0; iBadSyst < 3; ++iBadSyst) {
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
        TString badSysts[2] = {"JetSmear", "UncES"};
        bool isBadSyst = false;
        if (whichSyst != 0) {
            for (int iBadSyst = 0; iBadSyst < 3; ++iBadSyst) {
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
            inISPI->weight_CentVal *= SWtoUse->GetSF(0);
            if (doVerbosity) {
                cout << "base " << nameWeight << " weight " << SWtoUse->GetSF(0) << endl;
            }
            for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
                if (doVerbosity) {
                    cout << " for iSyst " << iSyst << endl;
                    cout << "weight " << nameWeight << " Syst Var Up " << SWtoUse->GetSF(iSyst) << endl;
                    cout << "weight " << nameWeight << " Syst Var Down " << SWtoUse->GetSF(- 1 * iSyst) << endl;
                }
                inISPI->weight_SystVarUp[iSyst - 1] *= SWtoUse->GetSF(iSyst);
                inISPI->weight_SystVarDown[iSyst - 1] *= SWtoUse->GetSF(- 1 * iSyst);
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
    /*
    ////Set of functions
    void SetFileNameVector(vector<TFile *> * inVecTFile, int index, int doNonSig, bool doSig) {
        TString dataName = "Data.root";
        TString mcName1, mcName2;
        if (doSig && !doNonSig) {
            mcName1
        }
        TString mcName1 = "TTBar";
        if (doSig) mc
        if (doNonSig) {
            if (index == 0) {
                inVecTFile->at(0) = new TFile("Data.root", "RECREATE");
            }
            else {
                if (index == 1) {
                    if (
                }h
            }
        }
    }
    
    void SetUpTFiles(bool doNonSig, bool doSig) {
        if (doNonSig) {
            vecVecTFiles.resize(2);
            vecVecTFiles[0].resize(1); // one output data file
            vecVecTFiles[1].resize(2); // Two output MC files
        }
        else if (doSig) {
            vecVecTFiles.resize(1);
        }
        else {
            cout << "not saving any TFiles because you've specified both no 'non-signal' and no 'signal'!"
            vecVecTFiles.resize(0);
        }
    }
    */
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
    
    void SetVecIndMCParams(SampLoadSettings * inSLS) {
        vecIndMCParams.resize(0);
        vecMCColors.resize(0);
        vecMCLegends.resize(0);
        vecMCSortParams.resize(0);
        SampleStartPositionsNames(&vecMCSortParams, &vecMCLegends, &vecMCColors, &vecIndMCParams, inSLS);
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
        Int_t wtopy = 100;
        
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
    
    void SetStructs(SampLoadSettings * inSLS) {
        int whichSSType = inSLS->doSignal ? 2 : -1;
        subSampVec = new vector<SampleT>;
        SetSubSampVec(subSampVec, whichSSType);
        SetHistTVecs(inSLS);
        SetCanvasParams();
        SetVecSystNames(inSLS->SmearedPlots);
        SetVecIndMCParams(inSLS);
    }
    
} PlotMakingStructs;



