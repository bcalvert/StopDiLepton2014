// Structs used as part of plotting the final sets of histograms
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;
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
    const int numSysts = 9;
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
        if (whichSyst == 0) {
            weightHistCV->SetBinContent(1, SF);
        }
        else if (whichSyst > 0) {
            weightHist_SystVarUp[abs(whichSyst) - 1]->SetBinContent(1, SF);
        }
        else if (whichSyst < 0) {
            weightHist_SystVarDown[abs(whichSyst) - 1]->SetBinContent(1, SF);
        }
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
    
    SpecificWeight weightTTBar_DDNorm;
    SpecificWeight weightDY_DDNormEE, weightDY_DDNormEMu, weightDY_DDNormMuMu;
    
    void LoadTTBarWeight(SampLoadSettings * inSLS, vector<TString> * vecSystString) {
        TString weightName = "_TTBarFullCutNormalization";
        TString nameTTBarFile = "ScaleFactors";
        nameTTBarFile += weightName;
        nameTTBarFile += inSLS->TTBarString();
        if (!inSLS->doExcSamps) nameTTBarFile += "_noExcSamps";
        nameTTBarFile += ".root";
        TFile * inFile = TFile::Open(nameTTBarFile);
        
        weightTTBar_DDNorm.SetHistsFromFile(inFile, weightName, vecSystString);
        weightTTBar_DDNorm.PrintWeights();
    }
    
    void LoadDYWeight(SampLoadSettings * inSLS, vector<TString> * vecSystString, int versNumber) {
        TString weightNameAppend = versNumber == 1 ? "_ROutIn" : "_ZMassBVetoCR";
        TString weightName = "_DY";
        weightName += weightNameAppend;
        TString weightNameEE = weightName + "_EE";
        TString weightNameEMu = weightName + "_EMu";
        TString weightNameMuMu = weightName + "_MuMu";
        TString nameDYFile = "ScaleFactors";
        nameDYFile += weightName;
        nameDYFile += inSLS->TTBarString();
        if (!inSLS->doExcSamps) nameDYFile += "_noExcSamps";
        nameDYFile += ".root";
        TFile * inFile = TFile::Open(nameDYFile);
        
        weightDY_DDNormEE.SetHistsFromFile(inFile, weightNameEE, vecSystString);
        weightDY_DDNormEMu.SetHistsFromFile(inFile, weightNameEMu, vecSystString);
        weightDY_DDNormMuMu.SetHistsFromFile(inFile, weightNameMuMu, vecSystString);
        cout << "printing EE weights " << endl;
        weightDY_DDNormEE.PrintWeights();
        cout << "printing EMu weights " << endl;
        weightDY_DDNormEMu.PrintWeights();
        cout << "printing MuMu weights " << endl;
        weightDY_DDNormMuMu.PrintWeights();
    }
    
    void DefaultVarVals() {
        SFLumi = 1.0;
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
    float CalcTTBarNormWeight(HistogramDisplayStructs * inHDS_Data, HistogramDisplayStructs * inHDS_MC, int whichSyst = 0, int binStatEnd = -1) {
        
        TH1F * dataHistToIntegrate = inHDS_Data->compSamp.first.grabbedHist_TH1F;
        
        
        int BinMax = binStatEnd < 1 ? dataHistToIntegrate->GetNbinsX() : binStatEnd;
        
        float dataIntegral = dataHistToIntegrate->Integral(1, BinMax);
        float dataIntegralMinNonTTBar = dataIntegral;
        
        
        float mcIntegral_v1 = 0.0, mcIntegral_v2 = 0.0;
        float integralTTBar = 0;
        float currMCIntegral;
        TString mcName;
        
        TH1F * compMCHistToIntegrate, * indMCHistToIntegrate;
        //nominally will put in stuff for systematics in here
        
        /*
        cout << "size of up systs " << inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F.size() << endl;
        cout << "size of down systs " << inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F.size() << endl;
        inHDS_MC->PrintCompSystHistNames();
        */
        if (whichSyst == 0) {
            compMCHistToIntegrate = inHDS_MC->compSamp.first.grabbedHist_TH1F;
        }
        else if (whichSyst > 0) {
            compMCHistToIntegrate = inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F[abs(whichSyst) - 1];
        }
        else if (whichSyst < 0) {
            compMCHistToIntegrate = inHDS_MC->compSamp.first.vecGrabbedHist_SystVarDown_TH1F[abs(whichSyst) - 1];
        }
        /*
        cout << "whichSyst - 1 " << whichSyst - 1 << endl;
        cout << "compMCHistToIntegrate exists? " << compMCHistToIntegrate << endl;
        cout << "compMCHistToIntegrate exists? " << compMCHistToIntegrate->GetName() << endl;
        */
        mcIntegral_v1 = compMCHistToIntegrate->Integral(1, BinMax);
        
        
        for (unsigned int iMC = 0; iMC < inHDS_MC->vecSampDisplay_IndMC.size(); ++iMC) {                        
            mcName = inHDS_MC->vecSampDisplay_IndMC[iMC].first.grabbedHist_TH1F->GetName();
            
            if (whichSyst == 0) {
                indMCHistToIntegrate = inHDS_MC->vecSampDisplay_IndMC[iMC].first.grabbedHist_TH1F;
            }
            else if (whichSyst > 0) {
                indMCHistToIntegrate = inHDS_MC->vecSampDisplay_IndMC[iMC].first.vecGrabbedHist_SystVarUp_TH1F[abs(whichSyst) - 1];
            }
            else if (whichSyst < 0) {
                indMCHistToIntegrate = inHDS_MC->vecSampDisplay_IndMC[iMC].first.vecGrabbedHist_SystVarDown_TH1F[abs(whichSyst) - 1];
            }
            currMCIntegral = indMCHistToIntegrate->Integral(1, BinMax);
            if (!mcName.Contains("VecBoson") && (mcName.Contains("TTbar") || mcName.Contains("TTBar"))) {
                integralTTBar += currMCIntegral;
            }
            else {
                dataIntegralMinNonTTBar -= currMCIntegral;
            }
            mcIntegral_v2 += currMCIntegral;
        }
        if (abs(mcIntegral_v2 - mcIntegral_v1) > 10) {
            cout << "something funky...mc comp integral calculated two different but ostensibly equivalent ways isn't the same..." << endl;
            cout << "mcIntegral_v1 " << mcIntegral_v1 << endl;
            cout << "mcIntegral_v2 " << mcIntegral_v2 << endl;
        }
        cout << "IntegralTTBar " <<  integralTTBar << endl;
        cout << "DataIntegralMinNonTTBar " << dataIntegralMinNonTTBar << endl;
        return dataIntegralMinNonTTBar/integralTTBar;
    }
    
    
    
    
    
    float CalcTTBarNormWeight(vector<TString> * vecStringsToContain, vector<TString> * vecStringsToNotContain, HistogramDisplayStructs * inHDS_Data, HistogramDisplayStructs * inHDS_MC, int whichSyst = 0, int binStatEnd = -1) {
        
        TH1F * dataHistToIntegrate = inHDS_Data->compSamp.first.grabbedHist_TH1F;
        
        
        int BinMax = binStatEnd < 1 ? dataHistToIntegrate->GetNbinsX() : binStatEnd;
        
        float dataIntegral = dataHistToIntegrate->Integral(1, BinMax);
        float dataIntegralMinNonTTBar = dataIntegral;
        
        
        float mcIntegral_v1 = 0.0, mcIntegral_v2 = 0.0;
        float integralTTBar = 0;
        float currMCIntegral;
        TString mcName;
        
        TH1F * compMCHistToIntegrate, * indMCHistToIntegrate;
        //nominally will put in stuff for systematics in here
        
        /*
         cout << "size of up systs " << inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F.size() << endl;
         cout << "size of down systs " << inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F.size() << endl;
         inHDS_MC->PrintCompSystHistNames();
         */
        if (whichSyst == 0) {
            compMCHistToIntegrate = inHDS_MC->compSamp.first.grabbedHist_TH1F;
        }
        else if (whichSyst > 0) {
            compMCHistToIntegrate = inHDS_MC->compSamp.first.vecGrabbedHist_SystVarUp_TH1F[abs(whichSyst) - 1];
        }
        else if (whichSyst < 0) {
            compMCHistToIntegrate = inHDS_MC->compSamp.first.vecGrabbedHist_SystVarDown_TH1F[abs(whichSyst) - 1];
        }
        /*
         cout << "whichSyst - 1 " << whichSyst - 1 << endl;
         cout << "compMCHistToIntegrate exists? " << compMCHistToIntegrate << endl;
         cout << "compMCHistToIntegrate exists? " << compMCHistToIntegrate->GetName() << endl;
         */
        mcIntegral_v1 = compMCHistToIntegrate->Integral(1, BinMax);
        
        
        bool indMCContainsGoodString, indMCContainsBadString;
        
        
        for (unsigned int iMC = 0; iMC < inHDS_MC->vecSampDisplay_IndMC.size(); ++iMC) {    
            indMCContainsBadString = false;
            indMCContainsGoodString = false;
            
            
            mcName = inHDS_MC->vecSampDisplay_IndMC[iMC].first.grabbedHist_TH1F->GetName();
            
            if (whichSyst == 0) {
                indMCHistToIntegrate = inHDS_MC->vecSampDisplay_IndMC[iMC].first.grabbedHist_TH1F;
            }
            else if (whichSyst > 0) {
                indMCHistToIntegrate = inHDS_MC->vecSampDisplay_IndMC[iMC].first.vecGrabbedHist_SystVarUp_TH1F[abs(whichSyst) - 1];
            }
            else if (whichSyst < 0) {
                indMCHistToIntegrate = inHDS_MC->vecSampDisplay_IndMC[iMC].first.vecGrabbedHist_SystVarDown_TH1F[abs(whichSyst) - 1];
            }
            currMCIntegral = indMCHistToIntegrate->Integral(1, BinMax);
            
            for (unsigned int iBadString = 0; iBadString < vecStringsToNotContain->size(); ++iBadString) {
                if (mcName.Contains(vecStringsToNotContain->at(iBadString))) indMCContainsBadString = true;
            }
            if (indMCContainsBadString) {
                dataIntegralMinNonTTBar -= currMCIntegral;
            }
            else {
                for (unsigned int iGoodString = 0; iGoodString < vecStringsToContain->size(); ++iGoodString) {
                    if (mcName.Contains(vecStringsToContain->at(iGoodString))) indMCContainsGoodString = true;
                }
                if (indMCContainsGoodString) {
                    integralTTBar += currMCIntegral;
                }
                else {
                    dataIntegralMinNonTTBar -= currMCIntegral;
                }
            }
            /*
            if (!mcName.Contains("VecBoson") && (mcName.Contains("TTbar") || mcName.Contains("TTBar"))) {
                integralTTBar += currMCIntegral;
            }
            else {
                dataIntegralMinNonTTBar -= currMCIntegral;
            }
            */
            mcIntegral_v2 += currMCIntegral;
        }
        if (abs(mcIntegral_v2 - mcIntegral_v1) > 10) {
            cout << "something funky...mc comp integral calculated two different but ostensibly equivalent ways isn't the same..." << endl;
            cout << "mcIntegral_v1 " << mcIntegral_v1 << endl;
            cout << "mcIntegral_v2 " << mcIntegral_v2 << endl;
        }
        cout << "IntegralTTBar " <<  integralTTBar << endl;
        cout << "DataIntegralMinNonTTBar " << dataIntegralMinNonTTBar << endl;
        return dataIntegralMinNonTTBar/integralTTBar;
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
    void SetISPIDDWeight(IndSamplePlotInfo * inISPI, bool doTTBar = false, bool doDY = false, bool doVerbosity = false) {

        if (doTTBar && inISPI->sampleType == 1) {
            inISPI->weight_CentVal *= weightTTBar_DDNorm.GetSF(0);
            if (doVerbosity) {
                cout << "base TTBar weight " << weightTTBar_DDNorm.GetSF(0) << endl;
            }
            for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
                if (doVerbosity) {
                    cout << " for iSyst " << iSyst << endl;
                    cout << "weight TTBar Syst Var Up " << weightTTBar_DDNorm.GetSF(iSyst) << endl;
                    cout << "weight TTBar Syst Var Down " << weightTTBar_DDNorm.GetSF(- 1 * iSyst) << endl;
                }
                inISPI->weight_SystVarUp[iSyst - 1] *= weightTTBar_DDNorm.GetSF(iSyst);
                inISPI->weight_SystVarDown[iSyst - 1] *= weightTTBar_DDNorm.GetSF(- 1 * iSyst);
            }
        }
        if (doDY && inISPI->sampleType == 2) {
            SpecificWeight * weightDYToUse;
            if (inISPI->nameISPI.Contains("ZDY_MuMu")) {
                weightDYToUse = &weightDY_DDNormMuMu;
            }
            else if (inISPI->nameISPI.Contains("ZDY_EE")) {
                weightDYToUse = &weightDY_DDNormEE;
            }
            else if (inISPI->nameISPI.Contains("ZDY_EMu")) {
                weightDYToUse = &weightDY_DDNormEMu;
            }
            else {
                cout << "not one of the three! " << inISPI->nameISPI << endl;
            }
            inISPI->weight_CentVal *= weightDYToUse->GetSF(0);
            if (doVerbosity) {
                cout << "base DY weight " << weightDYToUse->GetSF(0) << endl;
            }
            for (unsigned int iSyst = 1; iSyst <= inISPI->weight_SystVarUp.size(); ++iSyst) {
                if (doVerbosity) {
                    cout << " for iSyst " << iSyst << endl;
                    cout << "weight DY Syst Var Up " << weightDYToUse->GetSF(iSyst) << endl;
                    cout << "weight DY Syst Var Down " << weightDYToUse->GetSF(- 1 * iSyst) << endl;
                }
                inISPI->weight_SystVarUp[iSyst - 1] *= weightDYToUse->GetSF(iSyst);
                inISPI->weight_SystVarDown[iSyst - 1] *= weightDYToUse->GetSF(- 1 * iSyst);
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
            
            inISPI->weight_SystVarDown[iSyst - 1] *= ScaleBackCalcBasic(inISPI->inputFile, doVerbosity, iSyst, vecSystString);
        }
    }
    void SetISPIWeight(IndSamplePlotInfo * inISPI, bool useDDEstTTBar, bool useDDEstDY, bool doVerbosity = false, int numSysts = 0, vector<TString> * vecSystString = 0, vector<float> * inVecSignalSkimEfficiencyCalc = 0, int whichSigIndex = 0) {
        bool useDDEst = (useDDEstDY || useDDEstTTBar);
        inISPI->DefaultWeights(numSysts);
        if (inISPI->sampleType != 0) {
            if (useDDEst) {
                SetISPIDDWeight(inISPI, useDDEstTTBar, useDDEstDY, doVerbosity);
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
    
    
    vector<indMCParams> vecIndMCParams;
    
    
    THStack * indMCStack;
    
    vector<float> vecCanvasCornerParams, vecCanvasSizeParams;
    TCanvas * plottingCanvas;
    
    
    //structs for MT2CutYieldCalc.C
    vector<float> vecXaxisCut, vecYaxisCut;
    vector<TString> vecHistNamesForYieldCalc;
    
    ////Set of functions
    
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
        SampleStartPositionsNames(&vecIndMCParams, inSLS);
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



