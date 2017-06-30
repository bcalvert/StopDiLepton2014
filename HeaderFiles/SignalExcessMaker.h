#include "TRandom3.h"

void RandomizeHist(TH1F * inputHist) {
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    TRandom3 rand(0);
    double currBinContent, newBinContent;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        currBinContent = inputHist->GetBinContent(iBinX);
        newBinContent = rand.PoissonD(currBinContent);
        inputHist->SetBinContent(iBinX, newBinContent);
    }
}


TH1F * MakeSignalHist(TH1F * inputHistData, TH1F * inputHistNonTTBar, TH1F * inputHistTTBar, bool doPheno, int numPoissonTrials = 0, TString addName = "") {
    TString outName = "signal" + addName;
    TH1F * outHist = NULL;
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHistData, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    vector<TH1F *> vecRandHistData(numPoissonTrials + 1), vecRandHistNonTTBar(numPoissonTrials + 1), vecRandHistTTBar(numPoissonTrials + 1);
    vecRandHistData[0] = (TH1F *) inputHistData->Clone("data_Rand0");
    vecRandHistNonTTBar[0] = (TH1F *) inputHistNonTTBar->Clone("NonTTBar_Rand0");
    vecRandHistTTBar[0] = (TH1F *) inputHistTTBar->Clone("TTBar_Rand0");
    TString currHistName;
    TString addRandName;
    for (int iHist = 1; iHist <= numPoissonTrials; ++iHist) {
        addRandName = "_Rand";
        addRandName += iHist;
        currHistName = inputHistData->GetName();
        currHistName += addRandName;
        vecRandHistData[iHist] = (TH1F *) inputHistData->Clone(currHistName);
        RandomizeHist(vecRandHistData[iHist]);
        currHistName = inputHistNonTTBar->GetName();
        currHistName += addRandName;
        vecRandHistNonTTBar[iHist] = (TH1F *) inputHistNonTTBar->Clone(currHistName);
        RandomizeHist(vecRandHistNonTTBar[iHist]);
        
        currHistName = inputHistTTBar->GetName();
        currHistName += addRandName;
        vecRandHistTTBar[iHist] = (TH1F *) inputHistTTBar->Clone(currHistName);
        RandomizeHist(vecRandHistTTBar[iHist]);
    }
    
    
    for (int iHist = 0; iHist <= numPoissonTrials; ++iHist) {
        vecRandHistData[iHist]->Add(vecRandHistNonTTBar[iHist], -1);
        if (doPheno) {
            ZeroOutNegHistBins(vecRandHistData[iHist]);
            vecRandHistData[iHist]->Divide(vecRandHistData[iHist], vecRandHistTTBar[iHist]);
            TString tempPhenoName = "histPheno";
            tempPhenoName += addRandName;
            TH1F * tempPheno = (TH1F *) vecRandHistTTBar[iHist]->Clone(tempPhenoName);
            tempPheno->Multiply(vecRandHistData[iHist]);
            vecRandHistData[iHist] = tempPheno;
        }
        else {
            vecRandHistData[iHist]->Add(vecRandHistTTBar[iHist], -1);
            ZeroOutNegHistBins(vecRandHistData[iHist]);
        }
        if (outHist == NULL) {
            outHist = (TH1F *) vecRandHistData[iHist]->Clone(outName);
        }
        else {
            outHist->Add(vecRandHistData[iHist]);
        }
    }
    outHist->Scale(1./(numPoissonTrials + 1));
    if (outName.Contains("Up")) {
        outHist->SetLineColor(kRed);
    }
    else if (outName.Contains("Down")) {
        outHist->SetLineColor(kBlue);
    }
    else {
        outHist->SetFillColor(kGreen);
        outHist->SetLineColor(kGreen);
    }
    return outHist;
}

vector<TString> MakeSystVec(int whichSystType) {
    vector<TString> vecSystNamesFullAnti{"TTBarNorm", "TTBarMCStat", "NonTTBarMCStat", "LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "DYNorm", "EWKNorm", "FakeLepStat", "FakeLepFakeRateSyst"};
    vector<TString> vecSystNamesFullSimilar{"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "FakeLepStat", "FakeLepFakeRateSyst"};
    if (whichSystType == 0) {
        return vecSystNamesFullAnti;
    }
    else {
        return vecSystNamesFullSimilar;
    }
    return vecSystNamesFullSimilar;
}


TFile * MakeOutFile(TFile * inputFileData, TFile * inputFileTTBar, TFile * inputFileNonTTBar, TString outFileName, int whichSystType, int whichHistType, int numPoissonTrials = 0) {
    TString addName = whichSystType == 0 ? "AntiCorrSyst" : "CorrSyst";
    addName += whichHistType == 0 ? "JustExcess" : "MorePheno";
    outFileName += addName;
    outFileName += ".root";
    
    vector<TString> vecSystNames = MakeSystVec(whichSystType);
    
    vector<TString> vecAddNames{"", "_FullBin"};
    
    TH1F * outHistCV, * outHistCV_FullBin;
    vector<TH1F *> vecOutHist_SystUp, vecOutHist_SystUp_FullBin;
    vector<TH1F *> vecOutHist_SystDown, vecOutHist_SystDown_FullBin;
    
    TH1F * currHistData, * currHistTTBar, * currHistNonTTBar;
    TString baseNameData = "data_obs";
    TString baseNameTTBar = "TTBar";
    TString baseNameNonTTBar = "NonTTBar";
    
    TString currAddName = "";
    
    for (unsigned int iName = 0; iName < vecAddNames.size(); ++iName) {
        currHistData = (TH1F*) inputFileData->Get(baseNameData + vecAddNames[iName]);
        currHistTTBar = (TH1F*) inputFileTTBar->Get(baseNameTTBar + vecAddNames[iName]);
        currHistNonTTBar = (TH1F*) inputFileNonTTBar->Get(baseNameNonTTBar + vecAddNames[iName]);
        if (vecAddNames[iName].Contains("FullBin")) {
            currAddName = "_FullBin";
            outHistCV_FullBin = MakeSignalHist(currHistData, currHistNonTTBar, currHistTTBar, whichHistType, numPoissonTrials, currAddName);
        }
        else {
            currAddName = "";
            outHistCV = MakeSignalHist(currHistData, currHistNonTTBar, currHistTTBar, whichHistType, numPoissonTrials, currAddName);
        }
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            currAddName = TString("_") + vecSystNames[iSyst] + TString("Up");
            TString currSystGrabName = currAddName;
            if (whichSystType == 0) {
                currSystGrabName.Replace(currSystGrabName.Index("Up"), 2, "Down");
            }
            currHistTTBar = (TH1F*) inputFileTTBar->Get(baseNameTTBar + currSystGrabName + vecAddNames[iName]);
            currHistNonTTBar = (TH1F*) inputFileNonTTBar->Get(baseNameNonTTBar + currSystGrabName + vecAddNames[iName]);
            if (!currHistTTBar) {
                cout << "re-grabbing TTBar for currSystGrabName " << currSystGrabName << endl;
                currHistTTBar = (TH1F*) inputFileTTBar->Get(baseNameTTBar + vecAddNames[iName]);
            }
            if (!currHistNonTTBar) {
                cout << "re-grabbing NonTTBar for currSystGrabName " << currSystGrabName << endl;
                currHistNonTTBar = (TH1F*) inputFileNonTTBar->Get(baseNameNonTTBar + vecAddNames[iName]);
            }
            if (vecAddNames[iName].Contains("FullBin")) {
                currAddName += "_FullBin";
                vecOutHist_SystUp_FullBin.push_back(MakeSignalHist(currHistData, currHistNonTTBar, currHistTTBar, whichHistType, numPoissonTrials, currAddName));
            }
            else {
                currAddName += "";
                vecOutHist_SystUp.push_back(MakeSignalHist(currHistData, currHistNonTTBar, currHistTTBar, whichHistType, numPoissonTrials, currAddName));
            }
            
            currAddName = TString("_") + vecSystNames[iSyst] + TString("Down");
            currSystGrabName = currAddName;
            if (whichSystType == 0) {
                currSystGrabName.Replace(currSystGrabName.Index("Down"), 4, "Up");
            }
            currHistTTBar = (TH1F*) inputFileTTBar->Get(baseNameTTBar + currSystGrabName + vecAddNames[iName]);
            currHistNonTTBar = (TH1F*) inputFileNonTTBar->Get(baseNameNonTTBar + currSystGrabName + vecAddNames[iName]);
            if (!currHistTTBar) {
                cout << "re-grabbing TTBar for currSystGrabName " << currSystGrabName << endl;
                currHistTTBar = (TH1F*) inputFileTTBar->Get(baseNameTTBar + vecAddNames[iName]);
            }
            if (!currHistNonTTBar) {
                cout << "re-grabbing NonTTBar for currSystGrabName " << currSystGrabName << endl;
                currHistNonTTBar = (TH1F*) inputFileNonTTBar->Get(baseNameNonTTBar + vecAddNames[iName]);
            }
            if (vecAddNames[iName].Contains("FullBin")) {
                currAddName += "_FullBin";
                vecOutHist_SystDown_FullBin.push_back(MakeSignalHist(currHistData, currHistNonTTBar, currHistTTBar, whichHistType, numPoissonTrials, currAddName));
            }
            else {
                currAddName += "";
                vecOutHist_SystDown.push_back(MakeSignalHist(currHistData, currHistNonTTBar, currHistTTBar, whichHistType, numPoissonTrials, currAddName));
            }
        }
    }
    
    TFile * outFile = new TFile(outFileName, "RECREATE");
    outFile->cd();
    outHistCV->Write();
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        vecOutHist_SystUp[iSyst]->Write();
        vecOutHist_SystDown[iSyst]->Write();
    }
    outHistCV_FullBin->Write();
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        vecOutHist_SystUp_FullBin[iSyst]->Write();
        vecOutHist_SystDown_FullBin[iSyst]->Write();
    }
    outFile->Write();
    outFile->Close();
    
    return outFile;
}

