#ifndef CoarseMapMaker_cc
#define CoarseMapMaker_cc
void CoarseMapMaker::PrintBasic_Part1(bool isT2tt, int paramSMS) {
    TString strSMS = isT2tt ? "T2tt" : "T2bw";
    cout << "Running on " << strSMS << " with paramSMS = " << paramSMS << endl;
    if (doControl) {
        cout << "making the control region map" << endl;
        cout << "Cutting on MT2ll = " << cutMT2ll_Control << " and MT2lblb = " << cutMT2lblb_Control << " and MT2bb_ZMET = " << cutMT2bb_ZMET_Control << endl;
    }
    else {
        cout << "making the signal region map" << endl;
        cout << "Cutting on MT2ll = " << MT2llCut << " and MT2lblb = " << MT2lblbCut << " and MT2bb_ZMET = " << MT2bb_ZMETCut  << endl;
    }
}
void CoarseMapMaker::PrintBasic(bool isT2tt, int paramSMS) {
    TString strSMS = isT2tt ? "T2tt" : "T2bw";
    cout << "Running on " << strSMS << " with paramSMS = " << paramSMS << endl;
    cout << "Cutting on MT2ll = " << MT2llCut << " and MT2lblb = " << MT2lblbCut << " and MT2bb_ZMET = " << MT2bb_ZMETCut  << endl;
    if (!loadXSec) {
        cout << "note, making a xsection map " << endl;
    }
    cout << "Saving to " << outFile->GetName() << endl;
}
float CoarseMapMaker::FindXSec(int massStop) {
    int xBin = crossSectionHist->FindBin(massStop);
    return crossSectionHist->GetBinContent(xBin);
}
void CoarseMapMaker::LoadXSec() {
    crossSectionFile = TFile::Open(xsecFileName);
    crossSectionHist = (TH1F *) crossSectionFile->Get(xsecHistName);
}
void CoarseMapMaker::DeleteHists() {
    delete outHist_CV;
    
    for (unsigned int iSyst = 0; iSyst < vecOutHist_SystVarUp.size(); ++iSyst) {
        delete vecOutHist_SystVarUp[iSyst];
        delete vecOutHist_SystVarDown[iSyst];
    }
}
void CoarseMapMaker::CloseFiles() {
    for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
        if (vecSignalFiles[iChan]) {
            cout << "going to close " << vecSignalFiles[iChan]->GetName() << endl;
            vecSignalFiles[iChan]->Close("R");
        }
        
    }
    if (inNormFile) {
        cout << "going to close " << inNormFile->GetName() << endl;
        inNormFile->Close("R");
    }
}

void CoarseMapMaker::DefaultCuts() {
    MT2llCut = 0;
    MT2lblbCut = 0;
    MT2bb_ZMETCut = -1;
    whichIntType = 0;
    
    doControl = 0;
    cutMT2ll_Control = 80;
    cutMT2lblb_Control = 0;
    cutMT2bb_ZMET_Control = -1;
}
void CoarseMapMaker::DefaultVals(LeptFiltSignalAssociator * inLFSC, LimitParametersContainer * inLPC) {
    baseDirNormFile = inLFSC->baseDirNormFile;
    baseDirFile = inLFSC->baseDirFile;
    baseNameFile = inLFSC->baseNameFile;
    midNameFile = inLFSC->midNameFile;
    
    LPC = inLPC;
    vecDilepChannelName = LPC->GetNamesDilepChannels();
    
    strStop = inLFSC->strStop;
    strChi0 = inLFSC->strChi0;
    strChargino = inLFSC->strChargino;
    strPerPol = inLFSC->strPerPol;
    endNameFile = inLFSC->endNameFile;
    
    strFullSel = "";
    //        loadXSec = true;
    loadXSec = false;
    
    xsecFileName = "../NonDataRootFiles/SignalXSection/Stop8TeVXSecs.root";
    xsecHistName = "StopXSec_5GeV";
    //xsecFileName = "../NonDataRootFiles/SignalXSection/referenceXSecs.root";
    //xsecHistName = "stop8TeV_NLONLL";
}
void CoarseMapMaker::DoOverflow(TH2F * inHist) {
    int nBinsX = inHist->GetNbinsX();
    float eff;
    for (int iBinY = 1; iBinY <= inHist->GetNbinsY(); ++iBinY) {
        eff = inHist->GetBinContent(nBinsX, iBinY);
        if (iBinY == 1) {
            inHist->SetBinContent(nBinsX, 0, eff);
            inHist->SetBinContent(nBinsX + 1, 0, eff);
        }
        inHist->SetBinContent(nBinsX + 1, iBinY, eff);
    }
}
void CoarseMapMaker::DoOverflow() {
    DoOverflow(outHist_CV);
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        DoOverflow(vecOutHist_SystVarUp[iSyst]);
        DoOverflow(vecOutHist_SystVarDown[iSyst]);
    }
}
void CoarseMapMaker::CheckCombineHistBin(double inputScaledNumPass, double inputScaledNumPassErr, double inputTotalNum) {
    double sumNumPass = inputScaledNumPass + numPassCutScaled;
    double sumNumTotal = inputTotalNum + numTotal;
    double errSumNumPass = TMath::Sqrt(TMath::Power(inputScaledNumPassErr, 2) + TMath::Power(numPassCutScaledErr, 2));
    
    double estEff = sumNumPass / sumNumTotal;
    double estEffErr = errSumNumPass / sumNumTotal;
    cout << "inputScaledNumPass: " << inputScaledNumPass << " +- " << inputScaledNumPassErr << endl;
    cout << "numPassCutScaled " << numPassCutScaled << " +- " << numPassCutScaledErr << endl;
    cout << "sumNumPass " << sumNumPass << endl;
    
    cout << "inputTotalNum " << inputTotalNum << endl;
    cout << "numTotal " << numTotal << endl;
    
    cout << "eff is " << estEff << " +- " << estEffErr << endl;
}
void CoarseMapMaker::CombineHistBin(int massStop, int massLSP, int whichSyst, TH2F * inputHist, bool doVerb = false) {
    //Used for combining the measured efficiencies from two different efficiency measurements
    int binStop = outHist_CV->GetXaxis()->FindBin(massStop);
    int binLSP = outHist_CV->GetYaxis()->FindBin(massLSP);
    
    TH2F * histToUse = NULL;
    if (whichSyst > 0) {
        histToUse = vecOutHist_SystVarUp[abs(whichSyst) - 1];
    }
    else if (whichSyst < 0) {
        histToUse = vecOutHist_SystVarDown[abs(whichSyst) - 1];
    }
    else {
        histToUse = outHist_CV;
    }
    
    if (doVerb) {
        cout << "massStop " << massStop << endl;
        cout << "massLSP " << massLSP << endl;
        cout << "histToUse: " << histToUse->GetName() << endl;
        cout << "inputHist: " << inputHist->GetName() << endl;
    }
    
    vector<valPlusError> vecVPE(0);
    vecVPE.push_back(valPlusError(histToUse->GetBinContent(binStop, binLSP), histToUse->GetBinError(binStop, binLSP)));
    vecVPE.push_back(valPlusError(inputHist->GetBinContent(binStop, binLSP), inputHist->GetBinError(binStop, binLSP)));
    
    valPlusError weightAveragedVPE = VarianceWeightedMean(&vecVPE, doVerb);
    
    if (doVerb) {
        cout << "pre-combination" << endl;
        cout << "eff: " << histToUse->GetBinContent(binStop, binLSP) << endl;
        cout << "eff err: " << histToUse->GetBinError(binStop, binLSP) << endl;
    }
    
    histToUse->SetBinContent(binStop, binLSP, weightAveragedVPE.first);
    histToUse->SetBinError(binStop, binLSP, weightAveragedVPE.second);
    
    if (whichSyst == 0) {
        effCV = weightAveragedVPE.first;
        errEffCV = weightAveragedVPE.second;
    }
    
    if (doVerb) {
        cout << "post-combination" << endl;
        cout << "eff: " << histToUse->GetBinContent(binStop, binLSP) << endl;
        cout << "eff err: " << histToUse->GetBinError(binStop, binLSP) << endl;
    }
}
void CoarseMapMaker::SetBin(int massStop, int massLSP) {
    int binStop = outHist_CV->GetXaxis()->FindBin(massStop);
    int binLSP = outHist_CV->GetYaxis()->FindBin(massLSP);
    
    /*
     bool doOverflowStop = binStop == outHist_CV->GetNbinsX();
     bool doOverflowLSP = binLSP == outHist_CV->GetNbinsY();
     */
    outHist_CV->SetBinContent(binStop, binLSP, effCV);
    outHist_CV->SetBinError(binStop, binLSP,  errEffCV);
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        vecOutHist_SystVarUp[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarUp[iSyst]);
        vecOutHist_SystVarUp[iSyst]->SetBinError(binStop, binLSP, vecErrEff_SystVarUp[iSyst]);
        
        vecOutHist_SystVarDown[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarDown[iSyst]);
        vecOutHist_SystVarDown[iSyst]->SetBinError(binStop, binLSP, vecErrEff_SystVarDown[iSyst]);
    }
    /*
     if (doOverflowStop || doOverflowLSP) {
     binStop += doOverflowStop;
     binLSP += doOverflowLSP;
     outHist_CV->SetBinContent(binStop, binLSP, effCV);
     for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
     vecOutHist_SystVarUp[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarUp[iSyst]);
     vecOutHist_SystVarDown[iSyst]->SetBinContent(binStop, binLSP, vecEff_SystVarDown[iSyst]);
     }
     }
     */
}
TString CoarseMapMaker::FullPathFile(TString strSMS, int massStop, int massLSP, int indexPol, int charFrac = -1) {
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
void CoarseMapMaker::SetFiles(int massStop, int massLSP, SUSYT2LimitParams * inSUSYT2LP, massMap * inMassMap, bool doVerb = 0) {
    TString strSMS = "";
    
    massCombo currCombo(massStop, massLSP);
    
    massMap::iterator xIterMassMap;
    xIterMassMap = inMassMap->find(currCombo);
    if (xIterMassMap != inMassMap->end()) {
        strSMS = xIterMassMap->second;
    }
    else {
        cout << "something wiggedy: Stop:LSP " << currCombo.first << ":" << currCombo.second << " is not in the map, hombre!!" << endl;
    }
    if (strSMS.Contains("0.50") && !strSMS.Contains("LeptonFilter")) {
        strSMS.Replace(strSMS.Index("0.50"), 4, "0.5");
    }
    
    TString fullPathFile = FullPathFile(strSMS, massStop, massLSP, inSUSYT2LP->indexPol, inSUSYT2LP->charFrac);
    cout << "fullPathFile " << fullPathFile << endl;
    TString fullPathNormFile = baseDirNormFile + strSMS + TString(".root");
    
    for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
        TString currPathOpenFile = fullPathFile;
        currPathOpenFile = ConditionalStringReplacement(fullPathFile, "PATSY", vecDilepChannelName[iChan]);
        vecSignalFiles[iChan] = TFile::Open(currPathOpenFile);
    }
    inNormFile = TFile::Open(fullPathNormFile);
}

void CoarseMapMaker::CalculatePDFUncert(vector<TFile *> * vecSignalFilesToUse, TString systPDFName, int massStop, float baseEff, float LeptFiltSF, int * binAddreses, double &totalPDFRelSystErrUp, double &totalPDFRelSystErrDown, double &currRelErrSystVarUp, double &currRelErrSystVarDown, bool doVerb = false) {
    //This function calculates the PDF uncertainty for a given input file, which actually corresponds more explicitly
    //to a specific mass point in the 2D SUSY mass plane
    //To explain some of the not so obvious input variables
    //baseEff: normalization to apply to the signal to account for total number of signal MC points run at the mass point in question
    //LeptFiltSF: 1.0 for non-lepton filtered datasets, approximately 0.53 for lepton filtered datasets (accounts for effective cross section change from the lepton filter
    //binAddress: an array of size 3 containing the bins to cut on to calculate the overall efficiency (whether for CVal or syst. shift)
    //totalPDFRelSystErrUp/Down -- contain the quadrature sum of the relative PDF uncertainty for a specific PDF set for a specific mass point across all the eigenvector shifts
    
    int numPDFCT10 = 25;
    int numPDFMSTW = 23;
    //int numPDFNNPDF = 50;
    int numPDFNNPDF = 0;
    
    int numPDFToUse = 0;
    if (systPDFName.Contains("CT10")) {
        numPDFToUse = numPDFCT10;
    }
    else if (systPDFName.Contains("MSTW")) {
        numPDFToUse = numPDFMSTW;
    }
    else if (systPDFName.Contains("NNPDF")) {
        numPDFToUse = numPDFNNPDF;
    }
    
    
    ///Replace the names
    unsigned int numFiles = vecSignalFilesToUse->size();
    
    Size_t lengthString = TString("_SkimOutput").Length();
    Size_t lengthString2 = TString("_PDFWeights_SkimOutput").Length();
    vector<TString> vecNameFiles(numFiles);
    vector<TFile *> vecSignalFilesToUsePDF(numFiles);
    
    for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
        vecNameFiles[iChan] = vecSignalFilesToUse->at(iChan)->GetName();
        vecNameFiles[iChan].Replace(vecNameFiles[iChan].Index("_SkimOutput"), lengthString, "_PDFWeights_SkimOutput");
        
        vecSignalFilesToUsePDF[iChan] = TFile::Open(vecNameFiles[iChan]);
    }
    
    float scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUsePDF, doVerb);
    
    float estDilepXSec = baseEff * scaleBack * LeptFiltSF;
    
    if (loadXSec) estDilepXSec /= FindXSec(massStop);
    
    if (doVerb) {
        cout << "baseEff " << baseEff << endl;
        cout << "scaleBack for PDF weight file " << scaleBack << endl;
        cout << "LeptFiltSF " << LeptFiltSF << endl;
        cout << "estDilepXSec " << estDilepXSec << endl;
    }
    
    //Hack to fix NNPDF weights that are ridiculous
    float estDilepXSecCV = estDilepXSec;
    float threshDiffDilepXsec = 0.25;
    
    int minIndex = LPC->typeDilepChannel;
    int maxIndex = minIndex + 1;
    if (LPC->typeDilepChannel < 0) {
        minIndex = 0;
        maxIndex = numDilepChannels;
    }
    TH1 * inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUsePDF, histBaseName, minIndex, maxIndex, "_PDFComp");
    
    int binToGet = doControl ? 0 : 1;
    
    int whichIntTypeToUse = doControl && cutMT2lblb_Control != 0 ? 1 : whichIntType;
    
    vector<double> vecIntegrals;
    vector<double> vecIntegralErrors;
    
    PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
    double effCVPDF = vecIntegrals[binToGet] * estDilepXSec;
    double errEffCVPDF = vecIntegralErrors[binToGet] * estDilepXSec;
    if (doVerb) {
        cout << "For PDF weights file " << endl;
        cout << "PDF weights: vecIntegrals at index " << binToGet << " is " << vecIntegrals[binToGet] << endl;
        cout << "PDF weights: vecIntegralErrors at index " << binToGet << " is " << vecIntegralErrors[binToGet] << endl;
        
        cout << "PDF weights: effCV " << effCVPDF << endl;
        cout << "PDF weights: errEffCV " << errEffCVPDF << endl;
    }
    
    double currSystErrUp, currSystErrDown;
    double currEffSystUp, currEffSystDown;
    double currPDFSystErrUpSum = 0.0, currPDFSystErrDownSum = 0.0;
    
    currRelErrSystVarUp = 0.0;
    currRelErrSystVarDown = 0.0;
    
    
    TString systName;
    for (int iSystPDF = 1; iSystPDF <= numPDFToUse; ++iSystPDF) {
        systName = systPDFName;
        systName += iSystPDF;
        
        vecIntegrals.clear();
        vecIntegralErrors.clear();
        scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUsePDF, doVerb, systName + TString("ShiftUp"));
        estDilepXSec = baseEff * scaleBack * LeptFiltSF;
        if (loadXSec) estDilepXSec /= FindXSec(massStop);
        if (doVerb) {
            cout << "for Shift Up for Syst = " << systName << " estDilepXSec is " << estDilepXSec << endl;
            cout << "estDilepXSecCV/estDilepXSec " << estDilepXSecCV/estDilepXSec << endl;
            cout << "estDilepXSec/baseEstDilepXSec " << estDilepXSec/estDilepXSecCV << endl;
        }
        if (TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) > threshDiffDilepXsec) {
            if (doVerb) {
                cout << "changing the estDilepXSec because otherwise it's ridic" << endl;
                cout << "relative difference of dilep xsecs " << TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) << endl;
            }
            estDilepXSec = estDilepXSecCV;
        }
        inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUsePDF, histBaseName + systName + TString("ShiftUp"), minIndex, maxIndex, "_PDFComp");
        PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
        if (doVerb) {
            cout << "running on hist " << inputHist->GetName() << endl;
            cout << "for Shift Up for Syst = " << systName << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
            cout << "current efficiency for syst shift up " << vecIntegrals[binToGet] * estDilepXSec << endl;
        }
        currEffSystUp = vecIntegrals[binToGet] * estDilepXSec;
        currRelErrSystVarUp = TMath::Max(currRelErrSystVarUp, vecIntegralErrors[binToGet] * estDilepXSec); //Not 100% correct...but conservative I think
        
        vecIntegrals.clear();
        vecIntegralErrors.clear();
        scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUsePDF, doVerb, systName + TString("ShiftDown"));
        estDilepXSec = baseEff * scaleBack * LeptFiltSF;
        //baseEff is 1 / N_events total
        //scaleBack is h_nVtx_inclusive_SystShift / h_BasicWeightIntegral_inclusive
        if (loadXSec) estDilepXSec /= FindXSec(massStop);
        if (doVerb) {
            cout << "for Shift Down for Syst = " << systName << " estDilepXSec is " << estDilepXSec << endl;
            cout << "estDilepXSecCV/estDilepXSec " << estDilepXSecCV/estDilepXSec << endl;
            cout << "estDilepXSec/estDilepXSecCV " << estDilepXSec/estDilepXSecCV << endl;
        }
        if (TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) > threshDiffDilepXsec) {
            if (doVerb) {
                cout << "changing the estDilepXSec because otherwise it's ridic" << endl;
                cout << "relative difference of dilep xsecs " << TMath::Abs((estDilepXSecCV - estDilepXSec)/estDilepXSecCV) << endl;
            }
            estDilepXSec = estDilepXSecCV;
        }
        inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUsePDF, histBaseName + systName + TString("ShiftDown"), minIndex, maxIndex, "_PDFComp");
        PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);
        if (doVerb) {
            cout << "running on hist " << inputHist->GetName() << endl;
            cout << "for Shift Down for Syst = " << systName << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
            cout << "current efficiency for syst shift up " << vecIntegrals[binToGet] * estDilepXSec << endl;
        }
        currEffSystDown = vecIntegrals[binToGet] * estDilepXSec;
        currRelErrSystVarDown = TMath::Max(currRelErrSystVarDown, vecIntegralErrors[binToGet] * estDilepXSec); //Not 100% correct...but conservative I think
        
        pointSystErr(effCVPDF, currEffSystUp, currEffSystDown, currSystErrUp, currSystErrDown, false, doVerb);
        
        if (doVerb) {
            //            if (true) {
            if (effCVPDF < 1E-8) {
                cout << "relErr Up for iSystPDF " << iSystPDF << " is uncalculable, effCVPDF is 0 " << endl;
                cout << "relErr Down for iSystPDF " << iSystPDF << " is uncalculable, effCVPDF is 0 " << endl;
            }
            else {
                cout << "relErr Up for iSystPDF " << iSystPDF << " is " << currSystErrUp / effCVPDF << endl;
                cout << "relErr Down for iSystPDF " << iSystPDF << " is " << currSystErrDown / effCVPDF << endl;
            }
        }
        
        currPDFSystErrUpSum += TMath::Power(currSystErrUp, 2);
        currPDFSystErrDownSum += TMath::Power(currSystErrDown, 2);
    }
    currRelErrSystVarUp /= effCVPDF;
    currRelErrSystVarDown /= effCVPDF;
    totalPDFRelSystErrUp = TMath::Sqrt(currPDFSystErrUpSum) / effCVPDF;
    totalPDFRelSystErrDown = TMath::Sqrt(currPDFSystErrDownSum) / effCVPDF;
    if (doVerb) {
        cout << "for PDF " << systPDFName << endl;
        cout << "totalPDFRelSystErrUp " << totalPDFRelSystErrUp << endl;
        cout << "totalPDFRelSystErrDown " << totalPDFRelSystErrDown << endl;
    }
    
    
    for (unsigned int iFile = 0; iFile < numDilepChannels; ++iFile) {
        if (vecSignalFilesToUsePDF[iFile]) {
            vecSignalFilesToUsePDF[iFile]->Close("R");
        }
    }
    
    CloseMultipleFiles(&vecSignalFilesToUsePDF);
}

void CoarseMapMaker::SetEff(LeptFiltSignalAssociator * inLFSC, int massStop, int massLSP, float paramSMS, bool isT2tt, bool hackPDF = false, int doVerb = 0) {
    int numDims = isT2tt ? 2 : 3;
    
    vector<TFile *> vecSignalFilesToUse(numDilepChannels);
    vector<TString> vecNameFiles(numDilepChannels);
    for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
        vecSignalFilesToUse[iChan] = vecSignalFiles[iChan];
        vecNameFiles[iChan] = vecSignalFilesToUse[iChan]->GetName();
        isLeptFilt = vecNameFiles[iChan].Contains("LeptonFilter");
        if (doVerb) {
            cout << "vecNameFiles[iChan] " << vecNameFiles[iChan] << endl;
            cout << "isLeptFilt? " << isLeptFilt << endl;
        }
    }
    
    TFile * normFileToUse = inNormFile;
    
    int binStop = outHist_CV->GetXaxis()->FindBin(massStop);
    int binLSP = outHist_CV->GetYaxis()->FindBin(massLSP);
    float LeptFiltSF = isLeptFilt ? inLFSC->outHist->GetBinContent(binStop, binLSP) : 1.0;
    if (doVerb) {
        cout << "LeptFiltSF from the file " << inLFSC->outHist->GetName() << " is " << LeptFiltSF << endl;
        cout << "binStop " << binStop << endl;
        cout << "massStop " << massStop << endl;
        cout << "binLSP " << binLSP << endl;
        cout << "massLSP " << massLSP << endl;
    }
    if (isLeptFilt && (LeptFiltSF == 0 || LeptFiltSF > 1)) {
        for (int iChan = 0; iChan < numDilepChannels; ++iChan) {
            vecSignalFilesToUse[iChan] = inLFSC->vecFileNonLeptFilter[iChan];
        }
        normFileToUse = inLFSC->inNormFileNonLeptFilt;
        LeptFiltSF = 1.0;
        isLeptFilt = false;
    }
    
    float baseEff = SignalSkimEfficiencyCalc_SinglePoint(normFileToUse, massStop, massLSP, paramSMS, numDims, doVerb);
    float scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUse, doVerb);
    
    float estDilepXSec = baseEff * scaleBack * LeptFiltSF;
    
    if (loadXSec) estDilepXSec /= FindXSec(massStop);
    
    if (doVerb) {
        cout << "baseEff " << baseEff << endl;
        cout << "scaleBack " << scaleBack << endl;
        cout << "LeptFiltSF " << LeptFiltSF << endl;
        cout << "estDilepXSec " << estDilepXSec << endl;
    }
    
    int binAddreses[3] = {1, 1, 1};
    vector<double> vecIntegrals;
    vector<double> vecIntegralErrors;
    
    const int numBTagSysts = 2;
    TString nameBTagSysts[numBTagSysts] = {"_BTagEffSF", "_BMisTagSF"};
    

    
    
    int minIndex = LPC->typeDilepChannel;
    int maxIndex = minIndex + 1;
    if (LPC->typeDilepChannel < 0) {
        minIndex = 0;
        maxIndex = numDilepChannels;
    }
    
    
    if (doVerb) {
        for (int iChan = minIndex; iChan < maxIndex; ++iChan) {
            cout << "going to try from file " << vecSignalFilesToUse[iChan]->GetName() << endl;
        }
        cout << "trying to grab " << histBaseName << endl;
    }
    TH1 * inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUse, histBaseName, minIndex, maxIndex, "_Comp");
    
    int binToGet = doControl ? 0 : 1;
    
    int whichIntTypeToUse = doControl && cutMT2lblb_Control != 0 ? 1 : whichIntType;
    
    if (doControl) {
        if (cutMT2bb_ZMET_Control == -1) {
            BinFinder(binAddreses, inputHist, cutMT2ll_Control, cutMT2lblb_Control, 1e99, false, doVerb);
        }
        else {
            BinFinder(binAddreses, inputHist, cutMT2ll_Control, cutMT2lblb_Control, cutMT2bb_ZMET_Control, false, doVerb);
        }
    }
    else {
        if (MT2bb_ZMETCut == -1) {
            BinFinder(binAddreses, inputHist, MT2llCut, MT2lblbCut, 1e99, false, doVerb);
        }
        else {
            BinFinder(binAddreses, inputHist, MT2llCut, MT2lblbCut, MT2bb_ZMETCut, false, doVerb);
        }
    }
    
    if (doVerb) {
        cout << endl;
        cout << "binAddress for x " << binAddreses[0] << endl;
        cout << "binAddress for y " << binAddreses[1] << endl;
        if (MT2bb_ZMETCut != -1 || cutMT2bb_ZMET_Control != -1) {
            cout << "binAddress for z " << binAddreses[2] << endl;
        }
    }
    
    PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerb);

    
    effCV = vecIntegrals[binToGet] * estDilepXSec;
    errEffCV = vecIntegralErrors[binToGet] * estDilepXSec;
    
    cout << "effCV " << effCV << endl;
    if (effCV > 0) {
        cout << "relErr * 100 " << 100 * errEffCV / effCV << endl;
    }
    
    numPassCutScaled = vecIntegrals[binToGet] * scaleBack;
    numPassCutScaledErr = vecIntegralErrors[binToGet] * scaleBack;
    numTotal = (1./estDilepXSec) * scaleBack;
    
    //if (doVerb) {
    if (true) {
        cout << "vecIntegrals at index " << binToGet << " is " << vecIntegrals[binToGet] << endl;
        cout << "vecIntegralErrors at index " << binToGet << " is " << vecIntegralErrors[binToGet] << endl;
        
        cout << "effCV " << effCV << endl;
        if (effCV > 0) {
            cout << "relErr * 100 " << 100 * errEffCV / effCV << endl;
        }
    }
    vecEff_SystVarUp.resize(0);
    vecEff_SystVarDown.resize(0);
    
    vecErrEff_SystVarUp.resize(0);
    vecErrEff_SystVarDown.resize(0);
    
    double currEffSystUp, currEffSystDown;
    double currErrEffSystUp, currErrEffSystDown;
    double currSystErrUp, currSystErrDown;
    
    float estDilepXSecCV = estDilepXSec;
    
    bool doVerbSyst = doVerb > 1;
    
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        if (doVerbSyst) {
            cout << "iSyst " << iSyst << endl;
            cout << "vecSystNames[iSyst] " << vecSystNames[iSyst] << endl;
        }
        if (vecSystNames[iSyst].Contains("BTagSF")) {
            double currBTagSFErrUpSum = 0.0, currBTagSFErrDownSum = 0.0;
            currErrEffSystUp = 0.0;
            currErrEffSystDown = 0.0;
            for (int iBTag = 0; iBTag < 2; ++iBTag) {
                //do shift up first
                vecIntegrals.clear();
                vecIntegralErrors.clear();
                scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUse, doVerbSyst, nameBTagSysts[iBTag] + TString("ShiftUp"));
                estDilepXSec = baseEff * scaleBack * LeptFiltSF;
                if (loadXSec) estDilepXSec /= FindXSec(massStop);
                if (doVerbSyst) {
                    cout << "for Shift Up for BTag = " << nameBTagSysts[iBTag] << " estDilepXSec is " << estDilepXSec << endl;
                }
                inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUse, histBaseName + nameBTagSysts[iBTag] + TString("ShiftUp"), minIndex, maxIndex, "_Comp");
                PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerbSyst);
                if (doVerbSyst) {
                    cout << "running on hist " << inputHist->GetName() << endl;
                    cout << "for Shift Up for BTag = " << nameBTagSysts[iBTag] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                    cout << "current efficiency for syst shift up " << (vecIntegrals[binToGet] * estDilepXSec) << endl;
                }
                currEffSystUp = vecIntegrals[binToGet] * estDilepXSec;
                currErrEffSystUp = TMath::Max(currErrEffSystUp, vecIntegralErrors[binToGet] * estDilepXSec); //Not 100% correct...but conservative I think
                if (doVerbSyst) {
                    cout << "iBTag " << iBTag << " currEffSystUp " << currEffSystUp << endl;
                }
                //do shift up first
                vecIntegrals.clear();
                vecIntegralErrors.clear();
                scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUse, doVerbSyst, nameBTagSysts[iBTag] + TString("ShiftDown"));
                estDilepXSec = baseEff * scaleBack * LeptFiltSF;
                if (loadXSec) estDilepXSec /= FindXSec(massStop);
                if (doVerbSyst) {
                    cout << "for Shift Down for BTag = " << nameBTagSysts[iBTag] << " estDilepXSec is " << estDilepXSec << endl;
                }
                inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUse, histBaseName + nameBTagSysts[iBTag] + TString("ShiftDown"), minIndex, maxIndex, "_Comp");
                PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerbSyst);
                if (doVerbSyst) {
                    cout << "running on hist " << inputHist->GetName() << endl;
                    cout << "for Shift Down for BTag = " << nameBTagSysts[iBTag] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                    cout << "current efficiency for syst shift down " << (vecIntegrals[binToGet] * estDilepXSec) << endl;
                }
                currEffSystDown = vecIntegrals[binToGet] * estDilepXSec;
                currErrEffSystDown = TMath::Max(currErrEffSystDown, vecIntegralErrors[binToGet] * estDilepXSec); //Not 100% correct...but conservative I think
                if (doVerbSyst) {
                    cout << "iBTag " << iBTag << " currEffSystDown " << currEffSystDown << endl;
                }
                pointSystErr(effCV, currEffSystUp, currEffSystDown, currSystErrUp, currSystErrDown, false, doVerbSyst);
                
                currBTagSFErrUpSum += TMath::Power(currSystErrUp, 2);
                currBTagSFErrDownSum += TMath::Power(currSystErrDown, 2);
            }
            if (doVerbSyst) {
                cout << "TMath::Sqrt(currBTagSFErrUpSum) " << TMath::Sqrt(currBTagSFErrUpSum) << endl;
                cout << "TMath::Sqrt(currBTagSFErrDownSum) " << TMath::Sqrt(currBTagSFErrDownSum) << endl;
            }
            vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV + TMath::Sqrt(currBTagSFErrUpSum)));
            vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV - TMath::Sqrt(currBTagSFErrDownSum)));
            
            vecErrEff_SystVarUp.push_back(TMath::Max(errEffCV, currErrEffSystUp));
            vecErrEff_SystVarDown.push_back(TMath::Max(errEffCV, currErrEffSystDown));
        }
        else if (vecSystNames[iSyst].Contains("SignalMCStat")) {
            vecEff_SystVarUp.push_back(effCV + errEffCV);
            vecEff_SystVarDown.push_back(effCV - errEffCV);
            
            vecErrEff_SystVarUp.push_back(errEffCV);
            vecErrEff_SystVarDown.push_back(errEffCV);
        }
        else if (vecSystNames[iSyst].Contains("CT10") || vecSystNames[iSyst].Contains("MSTW") || vecSystNames[iSyst].Contains("NNPDF")) {
            double currPDFSystErrUp, currPDFSystErrDown;
            if (!hackPDF) {
                if (massStop < massStopThresh) {
                    CalculatePDFUncert(&vecSignalFiles, vecSystNames[iSyst], massStop, baseEff, LeptFiltSF, binAddreses, currPDFSystErrUp, currPDFSystErrDown, currErrEffSystUp, currErrEffSystDown, doVerbSyst);
                    //vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV + currPDFSystErrUp));
                    //vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV - currPDFSystErrDown));
                    vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV * (1 + currPDFSystErrUp)));
                    vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV * (1 - currPDFSystErrDown)));
                    
                    /*
                     vecErrEff_SystVarUp.push_back(TMath::Max(errEffCV, currErrEffSystUp));
                     vecErrEff_SystVarDown.push_back(TMath::Max(errEffCV, currErrEffSystDown));
                     */
                    vecErrEff_SystVarUp.push_back(TMath::Max(errEffCV, effCV * currErrEffSystUp));
                    vecErrEff_SystVarDown.push_back(TMath::Max(errEffCV, effCV * currErrEffSystDown));
                    
                }
                else {
                    int binThreshX = vecOutHist_SystVarUp[iSyst]->GetXaxis()->FindBin(massStopThresh);
                    int binThreshY = vecOutHist_SystVarUp[iSyst]->GetYaxis()->FindBin(massLSP);
                    vecEff_SystVarUp.push_back(vecOutHist_SystVarUp[iSyst]->GetBinContent(binThreshX, binThreshY));
                    vecEff_SystVarDown.push_back(vecOutHist_SystVarDown[iSyst]->GetBinContent(binThreshX, binThreshY));
                    
                    vecErrEff_SystVarUp.push_back(vecOutHist_SystVarUp[iSyst]->GetBinError(binThreshX, binThreshY));
                    vecErrEff_SystVarDown.push_back(vecOutHist_SystVarDown[iSyst]->GetBinError(binThreshX, binThreshY));
                }
            }
            else {
                vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV));
                vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV));
                vecErrEff_SystVarUp.push_back(errEffCV);
                vecErrEff_SystVarDown.push_back(errEffCV);
            }
        }
        else {
            //do shift up first
            vecIntegrals.clear();
            vecIntegralErrors.clear();
            scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUse, doVerbSyst, vecSystNames[iSyst] + TString("ShiftUp"));
            estDilepXSec = baseEff * scaleBack * LeptFiltSF;
            if (loadXSec) estDilepXSec /= FindXSec(massStop);
            if (doVerbSyst) {
                cout << "for Shift Up for Syst = " << vecSystNames[iSyst] << " estDilepXSec is " << estDilepXSec << endl;
            }
            inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUse, histBaseName + vecSystNames[iSyst] + TString("ShiftUp"), minIndex, maxIndex, "_Comp");
            PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerbSyst);
            if (doVerbSyst) {
                cout << "running on hist " << inputHist->GetName() << endl;
                cout << "for Shift Up for Syst = " << vecSystNames[iSyst] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                cout << "current efficiency for syst shift up " << vecIntegrals[binToGet] * estDilepXSec << endl;
            }
            currEffSystUp = vecIntegrals[binToGet] * estDilepXSec;
            currErrEffSystUp = vecIntegralErrors[binToGet] * estDilepXSec;
            
            //do shift down next
            vecIntegrals.clear();
            vecIntegralErrors.clear();
            scaleBack = ScaleBackCalcBasic2(&vecSignalFilesToUse, doVerbSyst, vecSystNames[iSyst] + TString("ShiftDown"));
            estDilepXSec = baseEff * scaleBack * LeptFiltSF;
            if (loadXSec) estDilepXSec /= FindXSec(massStop);
            if (doVerbSyst) {
                cout << "for Shift Down for Syst = " << vecSystNames[iSyst] << " estDilepXSec is " << estDilepXSec << endl;
            }
            inputHist = (TH1 *) LoadMultipleHistogram(&vecSignalFilesToUse, histBaseName + vecSystNames[iSyst] + TString("ShiftDown"), minIndex, maxIndex, "_Comp");
            PassFailCuts(&vecIntegrals, &vecIntegralErrors, binAddreses, inputHist, whichIntTypeToUse, doVerbSyst);
            if (doVerbSyst) {
                cout << "running on hist " << inputHist->GetName() << endl;
                cout << "for Shift Down for Syst = " << vecSystNames[iSyst] << " vecIntegrals[" << binToGet << "] is " << vecIntegrals[binToGet] << endl;
                cout << "current efficiency for syst shift down " << vecIntegrals[binToGet] * estDilepXSec << endl;
            }
            currEffSystDown = vecIntegrals[binToGet] * estDilepXSec;
            currErrEffSystDown = vecIntegralErrors[binToGet] * estDilepXSec;
            
            
            pointSystErr(effCV, currEffSystUp, currEffSystDown, currSystErrUp, currSystErrDown, false, doVerbSyst);
            
            if (doVerbSyst) {
                if (effCV < 1E-8) {
                    cout << "relErr Up for iSyst " << iSyst << " is non-calculable: effCV = 0" << endl;
                    cout << "relErr Down for iSyst " << iSyst << " is non-calculable: effCV = 0" << endl;
                }
                else {
                    cout << "relErr Up for iSyst " << iSyst << " is " << currSystErrUp / effCV << endl;
                    cout << "relErr Down for iSyst " << iSyst << " is " << currSystErrDown / effCV << endl;
                }
            }
            
            vecEff_SystVarUp.push_back(TMath::Max(0.0, effCV + currSystErrUp));
            vecEff_SystVarDown.push_back(TMath::Max(0.0, effCV - currSystErrDown));
            
            vecErrEff_SystVarUp.push_back(TMath::Max(errEffCV, currErrEffSystUp));
            vecErrEff_SystVarDown.push_back(TMath::Max(errEffCV, currErrEffSystDown));
        }
    }
}
void CoarseMapMaker::SetHistString() {
    histBaseName = "h_SmearMT2ll_vs_MT2lblbSignalShape";
    if ((!doControl && MT2bb_ZMETCut == -1) || (doControl && cutMT2bb_ZMET_Control == -1)) {
        histBaseName = "h_SmearMT2ll_vs_MT2lblbSignalShape";
    }
    else {
        histBaseName = "h_SmearMT2ll_vs_MT2lblb_vs_MT2bb_ZMET";
    }
    histBaseName += strFullSel;
}
void CoarseMapMaker::InitializeVecs(bool includePDF = true) {
    vecSystNames.clear();
    SetSystVec(&vecSystNames, includePDF);
    unsigned int numSysts = vecSystNames.size();
    vecOutHist_SystVarUp.resize(numSysts);
    vecOutHist_SystVarDown.resize(numSysts);
    vecSignalFiles.resize(numDilepChannels);
}

TString CoarseMapMaker::GetMT2String(bool isControl = false) {
    TString outString = "";
    if (doControl || isControl) {
        outString += "_MT2ll";
        outString += cutMT2ll_Control;
        outString += "AND";
        outString += "_MT2lblb";
        outString += cutMT2lblb_Control;
        if (cutMT2bb_ZMET_Control > -1) {
            outString += "AND";
            outString += "_MT2bb_ZMET";
            outString += cutMT2bb_ZMET_Control;
        }
    }
    else {
        outString += "_MT2ll";
        outString += MT2llCut;
        cout << "MT2llCut " << MT2llCut << endl;
        if (MT2lblbCut > -1) {
            outString += whichIntType ? "OR" : "AND";
            outString += "_MT2lblb";
            outString += MT2lblbCut;
            cout << "MT2lblbCut " << MT2lblbCut << endl;
        }
        if (MT2bb_ZMETCut > -1) {
            outString += whichIntType ? "OR" : "AND";
            outString += "_MT2bb_ZMET";
            outString += MT2bb_ZMETCut;
            cout << "MT2bb_ZMETCut " << MT2bb_ZMETCut << endl;
        }
    }
    cout << "outString " << outString << endl;
    return outString;
}
TString CoarseMapMaker::GetMT2LatexString(bool justMT2ll = true) {
    bool nonTrivMT2bbCut = (cutMT2bb_ZMET_Control != -1 || MT2bb_ZMETCut != -1);
    bool nonTrivMT2lblbCut = (cutMT2lblb_Control != 0 || MT2lblbCut != 0);
    if (nonTrivMT2bbCut || nonTrivMT2lblbCut) justMT2ll = false;
    
    TString outString = "##MTTwoll";
    TString jointString, strMT2ll, strMT2lblb, strMT2bb_ZMET;
    TString strAnd = whichIntType ? "##,##text{or}##," : "##,##text{and}##,";
    TString strGeV = "##,##GeV";
    if (doControl) {
        jointString = " < ";
        strMT2ll = ""; strMT2ll += cutMT2ll_Control;
        strMT2lblb = ""; strMT2lblb += cutMT2lblb_Control;
        strMT2bb_ZMET = ""; strMT2bb_ZMET += cutMT2bb_ZMET_Control;
    }
    else {
        jointString = " > ";
        strMT2ll = ""; strMT2ll += MT2llCut;
        strMT2lblb = ""; strMT2lblb += MT2lblbCut;
        strMT2bb_ZMET = ""; strMT2bb_ZMET += MT2bb_ZMETCut;
    }
    
    outString += jointString;
    outString += strMT2ll;
    outString += strGeV;
    if (!justMT2ll) {
        if (!(doControl && cutMT2lblb_Control == 0)) {
            outString += strAnd;
            outString += "##MTTwolblb";
            outString += jointString;
            outString += strMT2lblb;
            outString += strGeV;
            if (!doControl && cutMT2bb_ZMET_Control <= 0) {
                outString += strAnd;
                outString += "##MTTwobb_ZMET";
                outString += jointString;
                outString += strMT2lblb;
                outString += strGeV;
            }
        }
    }
    return outString;
}
TString CoarseMapMaker::GetMT2TitleString(bool justMT2ll = true) {
    bool nonTrivMT2bbCut = (cutMT2bb_ZMET_Control != -1 || MT2bb_ZMETCut != -1);
    bool nonTrivMT2lblbCut = (cutMT2lblb_Control != 0 || MT2lblbCut != 0);
    if (nonTrivMT2bbCut || nonTrivMT2lblbCut) justMT2ll = false;
    
    TString outString = "M_{T2}(ll)";
    TString jointString, strMT2ll, strMT2lblb, strMT2bb_ZMET;
    TString strAnd = whichIntType ? " or " : " and ";
    TString strGeV = " GeV";
    if (doControl) {
        jointString = " < ";
        strMT2ll = ""; strMT2ll += cutMT2ll_Control;
        strMT2lblb = ""; strMT2lblb += cutMT2lblb_Control;
        strMT2bb_ZMET = ""; strMT2bb_ZMET += cutMT2bb_ZMET_Control;
    }
    else {
        jointString = " > ";
        strMT2ll = ""; strMT2ll += MT2llCut;
        strMT2lblb = ""; strMT2lblb += MT2lblbCut;
        strMT2bb_ZMET = ""; strMT2bb_ZMET += MT2bb_ZMETCut;
    }
    
    outString += jointString;
    outString += strMT2ll;
    outString += strGeV;
    if (!justMT2ll) {
        if (!(doControl && cutMT2lblb_Control == 0)) {
            outString += strAnd;
            outString += "M_{T2}(lb)(lb)";
            outString += jointString;
            outString += strMT2lblb;
            outString += strGeV;
            if (!(doControl && cutMT2bb_ZMET_Control <= 0)) {
                outString += strAnd;
                outString += "M_{T2}^{#slash{W}}(bb)";
                outString += jointString;
                outString += strMT2lblb;
                outString += strGeV;
            }
        }
    }
    return outString;
}

void CoarseMapMaker::InitializeHistsandOutfile(SUSYT2LimitParams * inSUSYT2LP, int readFile, bool includePDF = true) {
    
    massStopThresh = 801;
    
    cout << "not going to calculate PDF weights higher than " << massStopThresh << endl;
    
    InitializeVecs(includePDF);
    
    TString outHistName = "h_CoarseSigEff_";
    
    if (doControl) {
        outHistName += "Control";
    }
    outHistName += inSUSYT2LP->SavingName(false, (inSUSYT2LP->typeT2 == 2));
    outHistName += LPC->strDilepChannel;
    
    strFullSel = LPC->SetHistAppendString_WhichFullSel();
    outHistName += LPC->strFullSel;
    outHistName += GetMT2String();
    
    SetHistString();
    int indexForSMSDir = doControl ? -1 : 1;
    
    TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(indexForSMSDir);
    //        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
    TString baseDirOutFile = strSMSDir;
    
    System_MakeDir(baseDirOutFile);
    
    TString outFileName = baseDirOutFile + outHistName;
    outFileName += ".root";
    
    cout << "Saving to " << outFileName << endl;
    
    if (readFile == 1) {
        outFile = TFile::Open(outFileName);
        outHist_CV = (TH2F *) outFile->Get(outHistName);
        
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            vecOutHist_SystVarUp[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Up"));
            vecOutHist_SystVarDown[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Down"));
        }
    }
    else if (readFile == -1) {
        SetHistogram(outHist_CV, inSUSYT2LP->typeT2, outHistName, 1, "_NonLeptFilter");
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            SetHistogram(vecOutHist_SystVarUp[iSyst], inSUSYT2LP->typeT2, outHistName, 1, "_NonLeptFilter" + vecSystNames[iSyst] + TString("Up"));
            SetHistogram(vecOutHist_SystVarDown[iSyst], inSUSYT2LP->typeT2, outHistName, 1, "_NonLeptFilter" + vecSystNames[iSyst] + TString("Down"));
        }
    }
    else {
        outFile = new TFile(outFileName, "RECREATE");
        SetHistogram(outHist_CV, inSUSYT2LP->typeT2, outHistName, 1, "");
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            SetHistogram(vecOutHist_SystVarUp[iSyst], inSUSYT2LP->typeT2, outHistName, 1, vecSystNames[iSyst] + TString("Up"));
            SetHistogram(vecOutHist_SystVarDown[iSyst], inSUSYT2LP->typeT2, outHistName, 1, vecSystNames[iSyst] + TString("Down"));
        }
    }
}
void CoarseMapMaker::WriteFile() {
    cout << "going to write " << outFile->GetName() << endl;
    outFile->Write();
    outFile->Close();
}

void CoarseMapMaker::SetMCStatEff(bool doVerb = false) {
    int indexStat = -1;
    TString shiftNameUp, shiftNameDown;
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        shiftNameUp = vecOutHist_SystVarUp[iSyst]->GetName();
        if (shiftNameUp.Contains("MCStat")) {
            indexStat = iSyst;
        }
    }
    
    if (indexStat > -1) {
        shiftNameUp = vecOutHist_SystVarUp[indexStat]->GetName();
        shiftNameDown = vecOutHist_SystVarDown[indexStat]->GetName();
        
        vecOutHist_SystVarUp[indexStat] = (TH2F *) outHist_CV->Clone(shiftNameUp);
        SetMCStatSystHist(vecOutHist_SystVarUp[indexStat], 1, doVerb);
        
        vecOutHist_SystVarDown[indexStat] = (TH2F *) outHist_CV->Clone(shiftNameDown);
        SetMCStatSystHist(vecOutHist_SystVarDown[indexStat], -1, doVerb);
    }
}

void CoarseMapMaker::SetParamsFromCommandLine(int argc, char* argv[]) {
    for (int iCoarseMapParam = 0; iCoarseMapParam < argc; ++iCoarseMapParam) {
        if (strncmp (argv[iCoarseMapParam],"makeXSecMap", 11) == 0) {
            loadXSec = 0;
        }
        else if (strncmp (argv[iCoarseMapParam],"cuts", 4) == 0) {
            MT2llCut = strtol(argv[iCoarseMapParam + 1], NULL, 10);
            MT2lblbCut = strtol(argv[iCoarseMapParam + 2], NULL, 10);
            MT2bb_ZMETCut = strtol(argv[iCoarseMapParam + 3], NULL, 10);
            cout << "MT2llCut " << MT2llCut << endl;
            cout << "MT2lblbCut " << MT2lblbCut << endl;
            cout << "MT2bb_ZMETCut " << MT2bb_ZMETCut << endl;
        }
        else if (strncmp (argv[iCoarseMapParam],"doControl", 9) == 0) {
            doControl = 1;
        }
        else if (strncmp (argv[iCoarseMapParam],"wInTy", 5) == 0) {
            whichIntType = strtol(argv[iCoarseMapParam + 1], NULL, 10 );
        }
        else if (strncmp (argv[iCoarseMapParam],"controlCuts", 11) == 0) {
            cutMT2ll_Control = strtol(argv[iCoarseMapParam + 1], NULL, 10);
            cutMT2lblb_Control = strtol(argv[iCoarseMapParam + 2], NULL, 10);
            cutMT2bb_ZMET_Control = strtol(argv[iCoarseMapParam + 3], NULL, 10);
            cout << "cutMT2ll_Control " << cutMT2ll_Control << endl;
            cout << "cutMT2lblb_Control " << cutMT2lblb_Control << endl;
            cout << "cutMT2bb_ZMET_Control " << cutMT2bb_ZMET_Control << endl;
        }
    }
}

void CoarseMapMaker::CombineCoarseEffs(int massStop, int massLSP, CoarseMapMaker * inCMMNonLeptFilt, int doVerb = 0) {
    CombineHistBin(massStop, massLSP, 0, inCMMNonLeptFilt->outHist_CV, doVerb);
    CheckCombineHistBin(inCMMNonLeptFilt->numPassCutScaled, inCMMNonLeptFilt->numPassCutScaledErr, inCMMNonLeptFilt->numTotal);
    
    for (unsigned int iSyst = 1; iSyst <= vecSystNames.size(); ++iSyst) {
        CombineHistBin(massStop, massLSP, iSyst, inCMMNonLeptFilt->vecOutHist_SystVarUp[abs(iSyst) - 1], doVerb > 1);
        CombineHistBin(massStop, massLSP, -1 * iSyst, inCMMNonLeptFilt->vecOutHist_SystVarDown[abs(iSyst) - 1], doVerb > 1);
    }
}
#endif