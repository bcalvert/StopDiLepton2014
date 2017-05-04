#ifndef LeptFiltSignalAssociator_cc
#define LeptFiltSignalAssociator_cc
void LeptFiltSignalAssociator::FillInGapsTightBin() {
    //Function designed to fill in the gaps left behind from the Tight Bin association to non-tight bin
    TH2F * TempCloneHist = (TH2F*) outHist->Clone("Temp");
    
    int minAxisDiff = 148, maxAxisDiff = 202;
    int maxStopMass = 452;
    int nBinsX = outHist->GetNbinsX();
    int nBinsY = outHist->GetNbinsY();
    
    TAxis * xAxis = outHist->GetXaxis();
    TAxis * yAxis = outHist->GetYaxis();
    
    float currMassStop, currMassLSP, currMassDiff;
    
    float sumLeptFilt;
    int numNonZeroLFs;
    
    float currLeptFilt;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currMassStop = xAxis->GetBinCenter(iBinX);
        if (currMassStop > maxStopMass) continue;
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMassLSP = yAxis->GetBinCenter(iBinY);
            currMassDiff = currMassStop - currMassLSP;
            if (currMassDiff < minAxisDiff || currMassDiff > maxAxisDiff) continue;
            
            sumLeptFilt = 0.0;
            numNonZeroLFs = 0;
            for (int iAveX = TMath::Max(iBinX - 1, 1); iAveX <= TMath::Min(nBinsX, iBinX + 1); ++iAveX) {
                for (int iAveY = TMath::Max(iBinY - 1, 1); iAveY <= TMath::Min(nBinsY, iBinY + 1); ++iAveY) {
                    currLeptFilt = TempCloneHist->GetBinContent(iAveX, iAveY);
                    if (currLeptFilt > 1E-3) {
                        sumLeptFilt += currLeptFilt;
                        numNonZeroLFs++;
                    }
                }
            }
            sumLeptFilt /= numNonZeroLFs;
            outHist->SetBinContent(iBinX, iBinY, sumLeptFilt);
        }
    }
    delete TempCloneHist;
}

void LeptFiltSignalAssociator::CloseFiles() {
    for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
        if (vecFileLeptFilter[iChan]) {
            vecFileLeptFilter[iChan]->Close();
        }
        if (vecFileNonLeptFilter[iChan]) {
            vecFileNonLeptFilter[iChan]->Close();
        }
    }
    if (inNormFileLeptFilt) {
        inNormFileLeptFilt->Close();
    }
    if (inNormFileNonLeptFilt) {
        inNormFileNonLeptFilt->Close();
    }
}
void LeptFiltSignalAssociator::SetVecs() {
    vecFileLeptFilter.resize(numDilepChannels);
    vecFileNonLeptFilter.resize(numDilepChannels);
}
void LeptFiltSignalAssociator::DefaultVals(int isT2tt, LimitParametersContainer * inLPC) {
    
    baseDirNormFile = "/data/users/bcalvert/StopDiLepCode/NonDataRootFiles/SUSYNormFiles/Histos_";
    baseDirFile = "/data/users/bcalvert/new_top/MCSkimOviedo_Old_vers3/Signal/Output_Tree_";
    baseNameFile = "Tree_";
    /*
     if (isT2tt > 1) {
     baseDirFile = "/data/users/bcalvert/new_top/MCSkimOviedo_Old_vers3/Signal/Output_Tree-";
     baseNameFile = "Tree-";
     }
     */
    LPC = inLPC;
    vecDilepChannelName = LPC->GetNamesDilepChannels();
    midNameFile = "_Oviedo_SkimOutput_Oviedo_wSS2PATSY";
    if (isT2tt > 2) {
        midNameFile += "_T2tbDecayType1";
    }
    strStop = "_SignalStop";
    strChi0 = "_Chi0";
    if (isT2tt) {
        strChargino = "";
        strPerPol = "_PercentPol";
    }
    else {
        strChargino = "_Chargino0.";
        strPerPol = "_PercentPol";
    }
    endNameFile = "_Output.root";
}
void LeptFiltSignalAssociator::SetHistAndOutFile(SUSYT2LimitParams * inSUSYT2LP, bool readFile, bool doVerb = 0) {
    
    SetVecs();
    
    TString outHistName = "h_LeptFiltEffSF_";
    outHistName += inSUSYT2LP->SavingName(false, (inSUSYT2LP->typeT2 == 2));
    if (LPC->typeDilepChannel > -1 && !readFile) outHistName += LPC->strDilepChannel;
    
    TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(0);
    //        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
    TString baseDirOutFile = strSMSDir;
    
    System_MakeDir(baseDirOutFile);
    
    TString outFileName = baseDirOutFile + outHistName;
    outFileName += ".root";
    
    if (readFile) {
        outFile = TFile::Open(outFileName);
        outHist = (TH2F *) outFile->Get(outHistName);
        if (doVerb) {
            cout << "trying to get " << outHist->GetName() << " from the file: " << outFile->GetName() << endl;
        }
    }
    else {
        outFile = new TFile(outFileName, "RECREATE");
        SetHistogram(outHist, inSUSYT2LP->typeT2, outHistName, 0, "");
        if (doVerb) {
            cout << "going to save " << outHist->GetName() << " into the file: " << outFile->GetName() << endl;
        }
    }
}
void LeptFiltSignalAssociator::WriteFile() {
    outFile->Write();
    outFile->Close();
}
TString LeptFiltSignalAssociator::FullPathFile(TString strSMS, int massStop, int massLSP, int indexPol, int charFrac = -1) {
    bool isT2bw = strSMS.Contains("T2bw");
    
    TString outPath = baseDirFile + strSMS + "/";
    outPath += baseNameFile;
    outPath += strSMS;
    outPath += midNameFile;
    outPath += strStop;
    outPath += massStop;
    outPath += strChi0;
    outPath += massLSP;
    if (isT2bw) {
        outPath += strChargino;
        if (charFrac == 50) {
            charFrac = 5;
        }
        outPath += charFrac;
    }
    outPath += strPerPol;
    outPath += indexPol * 10;
    outPath += endNameFile;
    return outPath;
}
void LeptFiltSignalAssociator::SetFiles(int massStop, int massLSP, SUSYT2LimitParams * inSUSYT2LP, labelMap * inMap, bool doVerb = 0) {
    TString strSMS = "";
    TString strSMS_NonLeptFilt = "";
    if (inSUSYT2LP->typeT2) {
        if (inSUSYT2LP->typeT2 > 1) {
            strSMS = LeptFiltDataSetT2tt_ver2(massStop, massLSP);
        }
        else {
            strSMS = LeptFiltDataSetT2tt(massStop, massLSP, doVerb);
        }
    }
    else {
        strSMS = LeptFiltDataSetT2bw(massStop, massLSP, inSUSYT2LP->charFrac, doVerb);
    }
    TString pathFullFile = FullPathFile(strSMS, massStop, massLSP, inSUSYT2LP->indexPol, inSUSYT2LP->charFrac);
    
    if (doVerb) {
        cout << "strSMS " << strSMS << endl;
        cout << "pathFullFile " << pathFullFile << endl;
    }
    
    if (!strSMS.Contains("FAIL")) {
        labelMap::iterator xIterLabelMap;
        xIterLabelMap = inMap->find(strSMS);
        if (xIterLabelMap != inMap->end()) {
            strSMS_NonLeptFilt = xIterLabelMap->second;
        }
        else {
            cout << "something wiggedy: " << strSMS << " is not in the map, hombre!!" << endl;
        }
        
        TString pathFullFile_NonLeptFilt = FullPathFile(strSMS_NonLeptFilt, massStop, massLSP, inSUSYT2LP->indexPol, inSUSYT2LP->charFrac);
        TString fullPathNormFile = baseDirNormFile + strSMS + TString(".root");
        TString fullPathNormFile_NonLeptFilt = baseDirNormFile + strSMS_NonLeptFilt + TString(".root");
        
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            TString currPathOpenFile = pathFullFile;
            currPathOpenFile = ConditionalStringReplacement(pathFullFile, "PATSY", vecDilepChannelName[iChan]);
            if (doVerb) {
                cout << "iChan " << iChan << endl;
                cout << "currPathOpenFile " << currPathOpenFile << endl;
            }
            vecFileLeptFilter[iChan] = TFile::Open(currPathOpenFile);
            
            TString currPathOpenFile_NonLeptFilt = pathFullFile_NonLeptFilt;
            currPathOpenFile_NonLeptFilt = ConditionalStringReplacement(pathFullFile_NonLeptFilt, "PATSY", vecDilepChannelName[iChan]);
            vecFileNonLeptFilter[iChan] = TFile::Open(currPathOpenFile_NonLeptFilt);
        }
        
        inNormFileLeptFilt = TFile::Open(fullPathNormFile);
        inNormFileNonLeptFilt = TFile::Open(fullPathNormFile_NonLeptFilt);
    }
    else {
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            vecFileLeptFilter[iChan] = NULL;
            vecFileNonLeptFilter[iChan] = NULL;
        }
        inNormFileLeptFilt = NULL;
        inNormFileNonLeptFilt = NULL;
    }
}
void LeptFiltSignalAssociator::SetBinT2tb(int massStop, int massLSP, bool doVerb = 0) {
    int binX, binY;
    float SF = 1.0; //No lept-filtered dataset for T2tb so set it to be 1.0
    binX = outHist->GetXaxis()->FindBin(massStop);
    binY = outHist->GetYaxis()->FindBin(massLSP);
    if (doVerb) {
        cout << "binX:binY " << binX << ":" << binY << endl;
    }
    outHist->SetBinContent(binX, binY, SF);
}
void LeptFiltSignalAssociator::SetBin(int massStop, int massLSP, float paramSMS, bool isT2tt, bool doVerb = 0) {
    int numDims = isT2tt ? 2 : 3;
    
    float SF;
    float fallbackSF = 0.53; // fall back SF
    float origNumLeptFilt, origNumNonLeptFilt;
    TString grabHistName = "h_nVtx_inclusive";
    float numEventsDilep_LeptFilt, numEventsDilep_NonLeptFilt;
    
    TH1F * histEventsDilep_LeptFilt, * histEventsDilep_NonLeptFilt;
    /*
     cout << "inNormFileLeptFilt " << inNormFileLeptFilt->GetName() << endl;
     cout << "inNormFileNonLeptFilt " << inNormFileNonLeptFilt->GetName() << endl;
     cout << "inFileLeptFilt " << inFileLeptFilt->GetName() << endl;
     cout << "inFileNonLeptFilt " << inFileNonLeptFilt->GetName() << endl;
     */
    bool isFileLeptFilt = true, isFileNonLeptFilt = true;
    for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
        if (!vecFileLeptFilter[iChan]) {
            isFileLeptFilt = false;
        }
        if (!vecFileNonLeptFilter[iChan]) {
            isFileNonLeptFilt = false;
        }
    }
    if (!inNormFileLeptFilt || !inNormFileNonLeptFilt || !isFileNonLeptFilt || !isFileLeptFilt) {
        cout << "inNormFileLeptFilt " << inNormFileLeptFilt << endl;
        cout << "inNormFileNonLeptFilt " << inNormFileNonLeptFilt << endl;
        cout << "isFileNonLeptFilt " << isFileNonLeptFilt << endl;
        cout << "isFileLeptFilt " << isFileLeptFilt << endl;
        SF = 0;
        //not sure what this is for...why did I cut at 150
        //Now I know, there's no non-lepton filtered datasets at that point...
        if (massStop < 150) {
            SF = fallbackSF;
        }
        else {
            SF = 0;
        }
    }
    else {
        float baseEffLeptFilt = SignalSkimEfficiencyCalc_SinglePoint(inNormFileLeptFilt, massStop, massLSP, paramSMS, numDims, doVerb);
        float baseEffNonLeptFilt = SignalSkimEfficiencyCalc_SinglePoint(inNormFileNonLeptFilt, massStop, massLSP, paramSMS, numDims, doVerb);
        
        float scaleBackLeptFilt = ScaleBackCalcBasic2(&vecFileLeptFilter, doVerb);
        float scaleBackNonLeptFilt = ScaleBackCalcBasic2(&vecFileNonLeptFilter, doVerb);
        
        int minIndex = LPC->typeDilepChannel;
        int maxIndex = minIndex + 1;
        if (LPC->typeDilepChannel < 0) {
            minIndex = 0;
            maxIndex = numDilepChannels;
        }
        histEventsDilep_LeptFilt = (TH1F *) LoadMultipleHistogram(&vecFileLeptFilter, grabHistName, minIndex, maxIndex, "_Comp");
        histEventsDilep_NonLeptFilt = (TH1F *) LoadMultipleHistogram(&vecFileNonLeptFilter, grabHistName, minIndex, maxIndex, "_Comp");
        
        numEventsDilep_LeptFilt = histEventsDilep_LeptFilt->Integral(1, histEventsDilep_LeptFilt->GetNbinsX()+1);
        numEventsDilep_NonLeptFilt = histEventsDilep_NonLeptFilt->Integral(1, histEventsDilep_NonLeptFilt->GetNbinsX()+1);
        
        if (doVerb) {
            cout << "baseEffLeptFilt " << baseEffLeptFilt << endl;
            cout << "baseEffNonLeptFilt " << baseEffNonLeptFilt << endl;
            cout << "scaleBackLeptFilt " << scaleBackLeptFilt << endl;
            cout << "scaleBackNonLeptFilt " << scaleBackNonLeptFilt << endl;
            cout << "numEventsDilep_LeptFilt " << numEventsDilep_LeptFilt << endl;
            cout << "numEventsDilep_NonLeptFilt " << numEventsDilep_NonLeptFilt << endl;
        }
        
        
        float estDilepXSecLeptFilt = scaleBackLeptFilt * baseEffLeptFilt * numEventsDilep_LeptFilt;
        float estDilepXSecNonLeptFilt = scaleBackNonLeptFilt * baseEffNonLeptFilt * numEventsDilep_NonLeptFilt;
        
        if (estDilepXSecNonLeptFilt < 1E-20 || estDilepXSecLeptFilt < 1E-20) {
            SF = 0;
        }
        else {
            //                SF = estDilepXSecLeptFilt / estDilepXSecNonLeptFilt;
            SF = estDilepXSecNonLeptFilt / estDilepXSecLeptFilt;
        }
        
        origNumLeptFilt = 1 / baseEffLeptFilt;
        origNumNonLeptFilt = 1 / baseEffNonLeptFilt;
        if (origNumLeptFilt < 1E4) {
            cout << "setting SF to be 0 because original number of Lepton Filtered guys is too small: " << origNumLeptFilt << endl;
            SF = 0;
        }
        else if (origNumNonLeptFilt < 1E4) {
            cout << "setting SF to be fall back SF (0.53) because original number of Non-Lepton Filtered guys is too small: " << origNumNonLeptFilt << endl;
            SF = fallbackSF;
        }
        else if (estDilepXSecNonLeptFilt > estDilepXSecLeptFilt) {
            cout << "setting SF to be 0 because estDilepXSecLeptFilt is less than estDilepXSecNonLeptFilt " << endl;
            cout << "estDilepXSecLeptFilt: " << estDilepXSecLeptFilt << endl;
            cout << "estDilepXSecNonLeptFilt: " << estDilepXSecNonLeptFilt << endl;
            SF = 0;
        }
        if (doVerb) {
            cout << "estDilepXSecLeptFilt " << estDilepXSecLeptFilt << endl;
            cout << "estDilepXSecNonLeptFilt " << estDilepXSecNonLeptFilt << endl;
            cout << "LeptFilt SF " << SF << endl;
        }
    }
    int binX, binY;
    
    binX = outHist->GetXaxis()->FindBin(massStop);
    binY = outHist->GetYaxis()->FindBin(massLSP);
    outHist->SetBinContent(binX, binY, SF);
}



void LeptFiltSignalAssociator::SmoothMap() {
    TString name = outHist->GetName();
    TH2F * tempHist = (TH2F *) outHist->Clone(name + TString("_Smooth"));
    outHist = smooth(tempHist);
    outHist->SetName(name);
}
#endif