#ifndef SignalYieldMaker_cc
#define SignalYieldMaker_cc

void SignalYieldMaker::DefaultVals() {
    lumiData = 19656;
    
    doSigContam = false;
    
    loadXSec = true;
    
    //        xsecFileName = "../NonDataRootFiles/SignalXSection/referenceXSecs.root";
    //        xsecHistName = "stop8TeV_NLONLL";
    xsecFileName = "../NonDataRootFiles/SignalXSection/Stop8TeVXSecs.root";
    xsecHistName = "StopXSec_5GeV";
}

float SignalYieldMaker::FindXSec(int massStop) {
    int xBin = crossSectionHist->FindBin(massStop);
    return crossSectionHist->GetBinContent(xBin);
}
float SignalYieldMaker::FindXSec(float massStop) {
    int xBin = crossSectionHist->FindBin(massStop);
    return crossSectionHist->GetBinContent(xBin);
}
void SignalYieldMaker::LoadXSec() {
    crossSectionFile = TFile::Open(xsecFileName);
    crossSectionHist = (TH1F *) crossSectionFile->Get(xsecHistName);
}
void SignalYieldMaker::InitializeVecs() {
    vecSystNames.clear();
    SetSystVec(&vecSystNames);
    
    
    vecSystFinalNames.clear();
    SetSystVecFinal(&vecSystFinalNames);
    
    vecSystFinalNames_TTBar.clear();
    SetSystVecFinal_TTBar(&vecSystFinalNames_TTBar);
    
    unsigned int numSysts = vecSystNames.size();
    unsigned int numSystsFinal = vecSystFinalNames.size();
    unsigned int numSystsFinal_TTBar = vecSystFinalNames_TTBar.size();
    
    vecInSmoothHist_SystVarUp.resize(numSysts);
    vecInSmoothHist_SystVarDown.resize(numSysts);
    
    vecOutYieldHist_SystVarUp.resize(numSystsFinal);
    vecOutYieldHist_SystVarDown.resize(numSystsFinal);
    
    vecYieldTTBarControl_SystVarUp.resize(numSystsFinal_TTBar);
    vecYieldTTBarControl_SystVarDown.resize(numSystsFinal_TTBar);
    
    vecYieldTTBarSignal_SystVarUp.resize(numSystsFinal_TTBar);
    vecYieldTTBarSignal_SystVarDown.resize(numSystsFinal_TTBar);
}
void SignalYieldMaker::NormalizeSigYieldByTTBar() {
    cout << "outYieldHist_CV Name " << outYieldHist_CV->GetName() << endl;
    cout << "yieldTTBarControl " << yieldTTBarControl << endl;
    outYieldHist_CV->Scale(1./yieldTTBarControl);
    for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
        vecOutYieldHist_SystVarUp[iSyst]->Scale(vecYieldTTBarControl_SystVarUp[iSyst]);
        vecOutYieldHist_SystVarDown[iSyst]->Scale(vecYieldTTBarControl_SystVarDown[iSyst]);
    }
}
void SignalYieldMaker::SetSigContamYields(TString nameTempFile, bool doVerb = 0) {
    
    System_RemoveFile(nameTempFile);
    
    stringstream cmdGrabTTBarControlYield;
    cmdGrabTTBarControlYield << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Yield | grep -v Total | awk '{print $3}' >> " << nameTempFile;
    if (doVerb) {
        cout << cmdGrabTTBarControlYield.str() << endl;
    }
    system(cmdGrabTTBarControlYield.str().c_str());
    
    stringstream cmdGrabTTBarControlStat;
    cmdGrabTTBarControlStat << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Stat | grep -v Total | awk '{print $4}' >> " << nameTempFile;
    if (doVerb) {
        cout << cmdGrabTTBarControlStat.str() << endl;
    }
    system(cmdGrabTTBarControlStat.str().c_str());
    
    stringstream cmdGrabTTBarSignalYield;
    cmdGrabTTBarSignalYield << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Yield | grep -v Total | awk '{print $3}' >> " << nameTempFile;
    if (doVerb) {
        cout << cmdGrabTTBarSignalYield.str() << endl;
    }
    system(cmdGrabTTBarSignalYield.str().c_str());
    
    stringstream cmdGrabTTBarSignalStat;
    cmdGrabTTBarSignalStat << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Stat | grep -v Total | awk '{print $4}' >> " << nameTempFile;
    if (doVerb) {
        cout << cmdGrabTTBarSignalStat.str() << endl;
    }
    system(cmdGrabTTBarSignalStat.str().c_str());
    
    for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
        
        TString nameSyst = vecSystFinalNames_TTBar[iSyst];
        nameSyst.Replace(nameSyst.Index("_"), 1, "");
        
        if (doVerb) {
            cout << "iSyst " << iSyst << endl;
            cout << "nameSyst " << nameSyst << endl;
        }
        
        stringstream cmdGrabTTBarControlYield_SystVarUp;
        cmdGrabTTBarControlYield_SystVarUp << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Up | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
        system(cmdGrabTTBarControlYield_SystVarUp.str().c_str());
        stringstream cmdGrabTTBarControlYield_SystVarDown;
        cmdGrabTTBarControlYield_SystVarDown << "cat " << nameFileTTBarYieldControl << " | grep TTBar | grep Down | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
        system(cmdGrabTTBarControlYield_SystVarDown.str().c_str());
        
        if (doVerb) {
            cout << "Control Syst Up " << cmdGrabTTBarControlYield_SystVarUp.str() << endl;
            cout << "Control Syst Down " << cmdGrabTTBarControlYield_SystVarDown.str() << endl;
        }
    }
    
    for (unsigned int iSyst = 0; iSyst < vecYieldTTBarSignal_SystVarUp.size(); ++iSyst) {
        
        TString nameSyst = vecSystFinalNames_TTBar[iSyst];
        nameSyst.Replace(nameSyst.Index("_"), 1, "");
        
        stringstream cmdGrabTTBarSignalYield_SystVarUp;
        cmdGrabTTBarSignalYield_SystVarUp << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Up | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
        system(cmdGrabTTBarSignalYield_SystVarUp.str().c_str());
        stringstream cmdGrabTTBarSignalYield_SystVarDown;
        cmdGrabTTBarSignalYield_SystVarDown << "cat " << nameFileTTBarYieldSignal << " | grep TTBar | grep Down | grep Shift | grep -v Total | grep " << nameSyst << " | awk '{print $7}' >>" << nameTempFile;
        system(cmdGrabTTBarSignalYield_SystVarDown.str().c_str());
        
        if (doVerb) {
            cout << "Signal Syst Up " << cmdGrabTTBarSignalYield_SystVarUp.str() << endl;
            cout << "Signal Syst Down " << cmdGrabTTBarSignalYield_SystVarDown.str() << endl;
        }
    }
    
    vecVals.clear();
    vecVals = ReadSigContamFileFloats(nameTempFile);
    
    if (doVerb) {
        for (unsigned int iVecVal = 0; iVecVal < vecVals.size(); ++iVecVal) {
            cout << "iVecVal " << iVecVal << endl;
            cout << "vecVals[iVecVal] " << vecVals[iVecVal] << endl;
        }
    }
    yieldTTBarControl = vecVals[0];
    statTTBarControl = (vecVals[1] - 1) * yieldTTBarControl;
    yieldTTBarSignal = vecVals[2];
    statTTBarSignal = (vecVals[3] - 1) * yieldTTBarSignal;
    
    unsigned int numPreSystVals = 4;
    unsigned int numBkgSystVals = vecYieldTTBarControl_SystVarUp.size();
    unsigned int numSigSystVals = vecYieldTTBarSignal_SystVarUp.size();
    
    for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
        vecYieldTTBarControl_SystVarUp[iSyst] = vecVals[numPreSystVals + (2* iSyst)] * yieldTTBarControl;
        vecYieldTTBarControl_SystVarDown[iSyst] = vecVals[numPreSystVals + (2* iSyst) + 1] * yieldTTBarControl;
    }
    
    for (unsigned int iSyst = 0; iSyst < vecYieldTTBarSignal_SystVarUp.size(); ++iSyst) {
        vecYieldTTBarSignal_SystVarUp[iSyst] = vecVals[numPreSystVals + 2 * numBkgSystVals + (2* iSyst)] * yieldTTBarSignal;
        vecYieldTTBarSignal_SystVarDown[iSyst] = vecVals[numPreSystVals + 2 * numBkgSystVals + (2* iSyst) + 1] * yieldTTBarSignal;
    }
    
    if (doVerb) {
        cout << "yieldTTBarControl " << yieldTTBarControl << endl;
        cout << "statTTBarControl " << statTTBarControl << endl;
        cout << "yieldTTBarSignal " << yieldTTBarSignal << endl;
        cout << "statTTBarSignal " << statTTBarSignal << endl;
        
        for (unsigned int iSyst = 0; iSyst < vecYieldTTBarControl_SystVarUp.size(); ++iSyst) {
            cout << "for Syst " << vecSystFinalNames_TTBar[iSyst] << endl;
            cout << "vecYieldTTBarControl_SystVarUp[iSyst] " << vecYieldTTBarControl_SystVarUp[iSyst] << endl;
            cout << "vecYieldTTBarControl_SystVarDown[iSyst] " << vecYieldTTBarControl_SystVarDown[iSyst] << endl;
            cout << "vecYieldTTBarSignal_SystVarUp[iSyst] " << vecYieldTTBarSignal_SystVarUp[iSyst] << endl;
            cout << "vecYieldTTBarSignal_SystVarDown[iSyst] " << vecYieldTTBarSignal_SystVarDown[iSyst] << endl;
        }
    }
    
    
}

void SignalYieldMaker::PrintYieldCV(float yieldCV, float yieldStatErr, TString printString) {
    char buf[300];
    if (yieldCV > 0) {
        sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", yieldCV, 1 + (yieldStatErr/yieldCV));
    }
    else {
        sprintf(buf,"CentVal: %0.4f, StatErr: %0.4f", 0.00, 1.00);
    }
    cout << "Name: " << printString << ", " << buf << endl;
}
void SignalYieldMaker::PrintYieldSyst(float yieldCV, float yieldSystUp, float yieldSystDown, TString printString, TString systString, bool doAverage) {
    char buf[300];
    if (doAverage) {
        if (yieldCV > 0) {
            sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", yieldCV, 1 + ((yieldSystUp + yieldSystDown)/(2 * yieldCV)));
        }
        else {
            sprintf(buf,"CentVal: %0.4f, AverageErr: %0.4f", 0.00, 1.00);
        }
    }
    else {
        if (yieldCV > 0) {
            sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", yieldCV, 1 + (yieldSystUp/yieldCV), 1 + (yieldSystDown/yieldCV));
        }
        else {
            sprintf(buf,"CentVal: %0.4f, UpErr: %0.4f, DownErr: %0.4f", 0.00, 1.00, 1.00);
        }
    }
    cout << "Name: " << printString << ", Syst: " << systString << ", " << buf << endl;
}

void SignalYieldMaker::PrintYieldMassPoint(massCombo * inMassCombo, bool doAverage, bool doVerb = 0, SignalYieldMaker * inSigYieldControl = 0) {
    ValError currValErr;
    ValError currBTagValErr;
    
    ValError currValErrControl, currBTagValErrControl;
    
    unsigned int indexStatErr = -1;
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        if (vecSystNames[iSyst].Contains("Stat")) {
            indexStatErr = iSyst;
        }
    }
    
    cout << "running at mass point {MassStop:MassLSP} = {" << inMassCombo->first << ":" << inMassCombo->second << "}" << endl;
    TString basicPrintString = outYieldHist_CV->GetName();
    TString addStringSyst;
    int binX = outYieldHist_CV->GetXaxis()->FindBin(inMassCombo->first);
    int binY = outYieldHist_CV->GetYaxis()->FindBin(inMassCombo->second);
    
    float yieldCV = outYieldHist_CV->GetBinContent(binX, binY);
    float yieldStatErr = abs(vecOutYieldHist_SystVarUp[indexStatErr]->GetBinContent(binX, binY) - yieldCV);
    
    float yieldCVUncorr = yieldCV;
    float yieldStatErrUncorr = yieldStatErr;
    
    float yieldCVControl, yieldStatErrControl;
    float yieldSystUpControl, yieldSystDownControl;
    float yieldContam_SigErr_SigContStat;
    float yieldContam_SigErr_TTContStat;
    float yieldContam_SigErr_TTSigStat;
    
    if (doSigContam) {
        yieldCVControl = inSigYieldControl->outYieldHist_CV->GetBinContent(binX, binY);
        yieldStatErrControl = abs(inSigYieldControl->vecOutYieldHist_SystVarUp[indexStatErr]->GetBinContent(binX, binY) - yieldCVControl);
        
        yieldCV = CalculateNewYield(yieldCV, yieldCVControl, yieldTTBarSignal, yieldTTBarControl, doVerb);
        yieldContam_SigErr_SigContStat = CalculateSigContamErr_SigContStat(yieldStatErrControl, yieldTTBarSignal, yieldTTBarControl, doVerb);
        yieldContam_SigErr_TTContStat = CalculateSigContamErr_TTContStat(yieldCVControl, yieldTTBarSignal, statTTBarControl, yieldTTBarControl, doVerb);
        yieldContam_SigErr_TTSigStat = CalculateSigContamErr_TTSigStat(yieldCVControl, statTTBarSignal, yieldTTBarControl, doVerb);
        
        currValErr.SetStatError(yieldCV, yieldContam_SigErr_SigContStat);
        cout << "Signal Stat Error TTBar Control Region Printout ";
        currValErr.PrintOut(basicPrintString, true, "", true, false);
        
        currValErr.SetStatError(yieldCV, yieldContam_SigErr_TTSigStat);
        cout << "TTBar Stat Error Signal Region Printout ";
        currValErr.PrintOut(basicPrintString, true, "", true, false);
        
        currValErr.SetStatError(yieldCV, yieldContam_SigErr_TTContStat);
        cout << "TTBar Stat Error TTBar Control Region Printout ";
        currValErr.PrintOut(basicPrintString, true, "", true, false);
        
        if (doVerb) {
            cout << "basicStat error " << yieldStatErr << endl;
            
            currValErr.SetStatError(yieldCVUncorr, yieldStatErrUncorr);
            cout << "No Sig Contam: Basic Stat Error Printout ";
            currValErr.PrintOut(basicPrintString, true, "", true, false);
        }
    }
    
    float yieldSystUp, yieldSystDown;
    currValErr.SetStatError(yieldCV, yieldStatErr);
    cout << "Basic Stat Error Printout ";
    currValErr.PrintOut(basicPrintString, true, "", true, false);
    //        PrintYieldCV(yieldCV, yieldStatErr, basicPrintString);
    
    //        ValError Syst
    
    int indexOrigSyst;
    const int numBTagSysts = 2;
    //TString nameBTagSysts[numBTagSysts] = {"_BTagEffSF", "_BMisTagSF"};
    
    //cout << "size vecSystFinalNames.size " << vecSystFinalNames.size() << endl;
    if (doVerb) {
        cout << "vecSystFinalNames.size() " << vecSystFinalNames.size() << endl;
        cout << "vecOutYieldHist_SystVarUp size " << vecOutYieldHist_SystVarUp.size() << endl;
        cout << "vecOutYieldHist_SystVarDown size " << vecOutYieldHist_SystVarDown.size() << endl;
    }
    for (unsigned int iSyst = 0; iSyst <= vecSystFinalNames.size(); ++iSyst) {
        if (iSyst < vecSystFinalNames.size() && vecSystFinalNames[iSyst].Contains("Stat")) continue;
        if (doVerb) {
            cout << "on iSyst " << iSyst << endl;
        }
        indexOrigSyst = -1;
        currValErr.DefaultVals();
        currValErr.centVal = yieldCV;
        if (iSyst != vecSystFinalNames.size()) {
            for (unsigned int iSyst2 = 0; iSyst2 < vecSystFinalNames_TTBar.size(); ++iSyst2) {
                if (vecSystFinalNames_TTBar[iSyst2].Contains(vecSystFinalNames[iSyst])) {
                    indexOrigSyst = iSyst2;
                    //cout << "iSyst " << iSyst << " indexOrigSyst " << indexOrigSyst << endl;
                }
            }
        }
        if (iSyst == vecSystFinalNames.size()) {
            addStringSyst = "PDF";
        }
        else {
            addStringSyst = vecSystFinalNames[iSyst];
            addStringSyst.Replace(addStringSyst.Index("_"), 1, "");
        }
        /*
         yieldSystUp = abs(vecOutYieldHist_SystVarUp[indexOrigSyst]->GetBinContent(binX, binY));
         yieldSystDown = abs(vecOutYieldHist_SystVarDown[indexOrigSyst]->GetBinContent(binX, binY));
         */
        if (doVerb) {
            cout << "num Out Yield Up hists " << vecOutYieldHist_SystVarUp.size() << endl;
            cout << "num Out Yield Down hists " << vecOutYieldHist_SystVarDown.size() << endl;
            cout << "num Out Control Yield Up hists " << inSigYieldControl->vecOutYieldHist_SystVarUp.size() << endl;
            cout << "num Out Control Yield Down hists " << inSigYieldControl->vecOutYieldHist_SystVarDown.size() << endl;
        }
        yieldSystUp = abs(vecOutYieldHist_SystVarUp[iSyst]->GetBinContent(binX, binY));
        yieldSystDown = abs(vecOutYieldHist_SystVarDown[iSyst]->GetBinContent(binX, binY));
        
        if (doSigContam) {
            /*
             yieldSystUpControl = abs(inSigYieldControl->vecOutYieldHist_SystVarUp[indexOrigSyst]->GetBinContent(binX, binY));
             yieldSystDownControl = abs(inSigYieldControl->vecOutYieldHist_SystVarDown[indexOrigSyst]->GetBinContent(binX, binY));
             */
            yieldSystUpControl = abs(inSigYieldControl->vecOutYieldHist_SystVarUp[iSyst]->GetBinContent(binX, binY));
            yieldSystDownControl = abs(inSigYieldControl->vecOutYieldHist_SystVarDown[iSyst]->GetBinContent(binX, binY));
            float corrTermSystUp, corrTermSystDown;
            if (indexOrigSyst != -1) {
                corrTermSystUp = (yieldSystUpControl * vecYieldTTBarSignal_SystVarUp[indexOrigSyst] / vecYieldTTBarControl_SystVarUp[indexOrigSyst]);
                corrTermSystDown = (yieldSystDownControl * vecYieldTTBarSignal_SystVarDown[indexOrigSyst] / vecYieldTTBarControl_SystVarDown[indexOrigSyst]);
            }
            else {
                corrTermSystUp = (yieldSystUpControl * yieldTTBarSignal / yieldTTBarControl);
                corrTermSystUp = (yieldSystDownControl * yieldTTBarSignal / yieldTTBarControl);
            }
            if (doVerb) {
                //cout << "indexOrigSyst " << indexOrigSyst << endl;
                cout << "iSyst " << iSyst << endl;
                cout << "yieldSystUpControl " << yieldSystUpControl << endl;
                cout << "yieldSystDownControl " << yieldSystDownControl << endl;
                if (indexOrigSyst != -1 ) {
                    cout << "indexOrigSyst " << indexOrigSyst << endl;
                    cout << "vecYieldTTBarSignal_SystVarUp[indexOrigSyst] " << vecYieldTTBarSignal_SystVarUp[indexOrigSyst] << endl;
                    cout << "vecYieldTTBarSignal_SystVarDown[indexOrigSyst] " << vecYieldTTBarSignal_SystVarDown[indexOrigSyst] << endl;
                    
                    cout << "vecYieldTTBarControl_SystVarUp[indexOrigSyst] " << vecYieldTTBarControl_SystVarUp[indexOrigSyst] << endl;
                    cout << "vecYieldTTBarControl_SystVarDown[indexOrigSyst] " << vecYieldTTBarControl_SystVarDown[indexOrigSyst] << endl;
                }
                else {
                    cout << "used base TTBar CV yields since not a TTBar systematic " << endl;
                }
                cout << "corrTermSystUp " << corrTermSystUp << endl;
                cout << "corrTermSystDown " << corrTermSystDown << endl;
                
                cout << "pre-corr yieldSystUp " << yieldSystUp << endl;
                cout << "pre-corr yieldSystDown " << yieldSystDown << endl;
            }
            
            yieldSystUp -= corrTermSystUp;
            yieldSystDown -= corrTermSystDown;
            
            if (yieldSystUp < 0) yieldSystUp = 0.0;
            if (yieldSystDown < 0) yieldSystDown = 0.0;
        }
        //cout << "name for up shift " << vecOutYieldHist_SystVarUp[indexOrigSyst]->GetName() << endl;
        //cout << "name for down shift " << vecOutYieldHist_SystVarDown[indexOrigSyst]->GetName() << endl;
        //cout << "up yield " << yieldSystUp << endl;
        //cout << "down yield " << yieldSystDown << endl;
        //cout << "yieldCV " << yieldCV << endl;
        currValErr.SetSystError(yieldCV, yieldSystUp, yieldSystDown, false, false);
        currValErr.PrintOut(basicPrintString, false, addStringSyst, true, doAverage);
    }
}

void SignalYieldMaker::PrintYields(bool doAverage, bool doVerb = 0, SignalYieldMaker * inSigYieldControl = 0) {
    int minDiff = 100;
    int currDiff;
    
    int nBinsX = outYieldHist_CV->GetNbinsX();
    int nBinsY = outYieldHist_CV->GetNbinsY();
    
    int currMassStop, currMassLSP;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currMassStop = ModFive(outYieldHist_CV->GetXaxis()->GetBinCenter(iBinX));
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMassLSP = outYieldHist_CV->GetYaxis()->GetBinCenter(iBinY);
            if (currMassLSP % 5 == 4) {
                currMassLSP += 1;
            }
            else if (currMassLSP % 5 == 1) {
                currMassLSP -= 1;
            }
            currDiff = currMassStop - currMassLSP;
            if (currDiff < minDiff) continue;
            massCombo currCombo(currMassStop, currMassLSP);
            PrintYieldMassPoint(&currCombo, doAverage, doVerb, inSigYieldControl);
        }
    }
}

void SignalYieldMaker::MakeYieldHists(bool doVerb = 0) {
    int nBinsX = outYieldHist_CV->GetNbinsX();
    int nBinsY = outYieldHist_CV->GetNbinsY();
    
    float currMassStop, currMassLSP;
    int currBinStop_Smooth, currBinLSP_Smooth;
    float currSigEff, currSigEff_SystUp, currSigEff_SystDown;
    
    float currXSec;
    float currYield, currYield_SystUp, currYield_SystDown;
    
    int indexCT10 = -1, indexMSTW = -1, indexNNPDF = -1;
    
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currMassStop = outYieldHist_CV->GetXaxis()->GetBinCenter(iBinX);
        currBinStop_Smooth = inSmoothHist_CV->GetXaxis()->FindBin(currMassStop);
        
        //	    currXSec = FindXSec(currMassStop + 0.5);
        currXSec = FindXSec(currMassStop);
        
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currMassLSP = outYieldHist_CV->GetYaxis()->GetBinCenter(iBinY);
            currBinLSP_Smooth = inSmoothHist_CV->GetYaxis()->FindBin(currMassLSP);
            
            currSigEff = inSmoothHist_CV->GetBinContent(currBinStop_Smooth, currBinLSP_Smooth);
            currYield = currSigEff * lumiData * currXSec;
            if (doVerb) {
                cout << "currMassStop " << currMassStop << endl;
                cout << "currMassLSP " << currMassLSP << endl;
                cout << "for CV, currSigEff is " << currSigEff << endl;
                cout << "and yield is lumiData: " << lumiData << " times currXSec: " << currXSec << " times currSigEff to equal " << currYield << endl;
            }
            outYieldHist_CV->SetBinContent(iBinX, iBinY, currYield);
            
            for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
                currSigEff_SystUp = vecInSmoothHist_SystVarUp[iSyst]->GetBinContent(currBinStop_Smooth, currBinLSP_Smooth);
                currYield_SystUp = currSigEff_SystUp * lumiData * currXSec;
                if (doVerb) {
                    cout << "for iSyst = " << iSyst << " currSigEff_SystVarUp is " << currSigEff_SystUp << endl;
                    cout << "and yield is lumiData: " << lumiData << " times currXSec: " << currXSec << " times currSigEff_SystUp to equal " << currYield_SystUp << endl;
                }
                vecOutYieldHist_SystVarUp[iSyst]->SetBinContent(iBinX, iBinY, currYield_SystUp);
                
                currSigEff_SystDown = vecInSmoothHist_SystVarDown[iSyst]->GetBinContent(currBinStop_Smooth, currBinLSP_Smooth);
                currYield_SystDown = currSigEff_SystDown * lumiData * currXSec;
                if (doVerb) {
                    cout << "for iSyst = " << iSyst << " currSigEff_SystVarDown is " << currSigEff_SystDown << endl;
                    cout << "and yield is lumiData: " << lumiData << " times currXSec: " << currXSec << " times currSigEff_SystDown to equal " << currYield_SystDown << endl;
                }
                vecOutYieldHist_SystVarDown[iSyst]->SetBinContent(iBinX, iBinY, currYield_SystDown);
            }
        }
    }
    for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
        if (vecSystNames[iSyst].Contains("CT10")) {
            indexCT10 = iSyst;
        }
        else if (vecSystNames[iSyst].Contains("MSTW")) {
            indexMSTW = iSyst;
        }
        else if (vecSystNames[iSyst].Contains("NNPDF")) {
            indexNNPDF = iSyst;
        }
    }
    
    vector<TH2F *> vecHists_PDFShiftUp, vecHists_PDFShiftDown;
    vecHists_PDFShiftUp.push_back(vecOutYieldHist_SystVarUp[indexCT10]);
    vecHists_PDFShiftUp.push_back(vecOutYieldHist_SystVarUp[indexMSTW]);
    //vecHists_PDFShiftUp.push_back(vecOutYieldHist_SystVarUp[indexNNPDF]);
    
    vecHists_PDFShiftDown.push_back(vecOutYieldHist_SystVarDown[indexCT10]);
    vecHists_PDFShiftDown.push_back(vecOutYieldHist_SystVarDown[indexMSTW]);
    //vecHists_PDFShiftDown.push_back(vecOutYieldHist_SystVarDown[indexNNPDF]);
    
    if (doVerb) {
        cout << "going to set envelope histograms " << endl;
    }
    SetEnvelopeHistogram(&vecHists_PDFShiftUp, vecOutYieldHist_SystVarUp[vecSystNames.size()], vecOutYieldHist_SystVarUp[vecSystNames.size() + 1], 1, doVerb);
    SetEnvelopeHistogram(&vecHists_PDFShiftDown, vecOutYieldHist_SystVarDown[vecSystNames.size()], vecOutYieldHist_SystVarDown[vecSystNames.size() + 1], -1, doVerb);
}


void SignalYieldMaker::ReadSmoothMaps(SmoothMapMaker * inSMM, SmoothMapMaker * inSMM_T2ttHack = NULL, bool doVerb = 0) {
    if (doVerb) {
        cout << "inSMM file " << inSMM->outFile->GetName() << endl;
    }
    
    inSmoothHist_CV = inSMM->histCV;
    for (unsigned int iSyst = 0; iSyst < vecInSmoothHist_SystVarUp.size(); ++iSyst) {
        if (inSMM_T2ttHack && IsPDF(vecSystNames[iSyst])) {
            TH2F * tempHist = (TH2F *) inSMM_T2ttHack->histCV->Clone(inSMM_T2ttHack->histCV->GetName() + TString("_Hack_") + vecSystNames[iSyst] + TString("_Up"));
            tempHist->Divide(tempHist, inSMM_T2ttHack->vecHist_SystVarDown[iSyst]);
            
            vecInSmoothHist_SystVarUp[iSyst] = (TH2F *) inSmoothHist_CV->Clone(inSmoothHist_CV->GetName() + vecSystNames[iSyst] + TString("Up"));
            vecInSmoothHist_SystVarUp[iSyst]->Divide(vecInSmoothHist_SystVarUp[iSyst], tempHist);
            
            tempHist = (TH2F *) inSMM_T2ttHack->histCV->Clone(inSMM_T2ttHack->histCV->GetName() + TString("_Hack_") + vecSystNames[iSyst] + TString("_Down"));
            tempHist->Divide(tempHist, inSMM_T2ttHack->vecHist_SystVarDown[iSyst]);
            
            vecInSmoothHist_SystVarDown[iSyst] = (TH2F *) inSmoothHist_CV->Clone(inSmoothHist_CV->GetName() + vecSystNames[iSyst] + TString("Down"));
            vecInSmoothHist_SystVarDown[iSyst]->Divide(vecInSmoothHist_SystVarDown[iSyst], tempHist);
        }
        else {
            vecInSmoothHist_SystVarUp[iSyst] = inSMM->vecHist_SystVarUp[iSyst];
            vecInSmoothHist_SystVarDown[iSyst] = inSMM->vecHist_SystVarDown[iSyst];
        }
    }
}

void SignalYieldMaker::InitializeHistsandOutfile(SUSYT2LimitParams * inSUSYT2LP, CoarseMapMaker * inCMM, bool readFile, bool isControl, bool doVerb = 0) {
    
    InitializeVecs();
    TString outHistName = "h_SigYield_";
    
    if (inCMM->doControl && isControl) {
        cout << "what's going on, both shouldn't be turned on!" << endl;
    }
    
    if (inCMM->doControl || isControl) {
        outHistName += "Control";
    }
    
    outHistName += inSUSYT2LP->SavingName(false);
    if (inCMM->LPC->typeDilepChannel > -1) outHistName += inCMM->LPC->strDilepChannel;
    outHistName += inCMM->LPC->strFullSel; //Handle this in better fashion -- it's copied code...also want to add in more verbosity statements
    outHistName += inCMM->GetMT2String(isControl);
    
    int indexForSMSDir = inCMM->doControl || isControl ? -3 : 3;
    
    TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(indexForSMSDir);
    //        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
    TString baseDirOutFile = strSMSDir;
    
    System_MakeDir(baseDirOutFile);
    
    TString outFileName = baseDirOutFile + outHistName;
    outFileName += ".root";
    
    if (doVerb) {
        cout << "outFileName " << outFileName << endl;
    }
    
    if (readFile) {
        outFile = TFile::Open(outFileName);
        outYieldHist_CV = (TH2F *) outFile->Get(outHistName);
        if (doVerb) {
            cout << "grabbed hist? " << outHistName << "? " << outYieldHist_CV << endl;
        }
        
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            vecOutYieldHist_SystVarUp[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Up"));
            vecOutYieldHist_SystVarDown[iSyst] = (TH2F *) outFile->Get(outHistName + vecSystNames[iSyst] + TString("Down"));
        }
        
        vecOutYieldHist_SystVarUp.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFUp")));
        vecOutYieldHist_SystVarUp.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFEnvelopeUp")));
        vecOutYieldHist_SystVarDown.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFDown")));
        vecOutYieldHist_SystVarDown.push_back((TH2F *) outFile->Get(outHistName + TString("_PDFEnvelopeDown")));
    }
    else {
        if (doVerb) {
            cout << "making output file " << endl;
        }
        outFile = new TFile(outFileName, "RECREATE");
        if (doVerb) {
            cout << "setting central value histogram " << endl;
        }
        SetHistogram(outYieldHist_CV, inSUSYT2LP->typeT2, outHistName, 3, "");
        cout << "outYieldHist_CV " << outYieldHist_CV << endl;
        for (unsigned int iSyst = 0; iSyst < vecSystNames.size(); ++iSyst) {
            if (doVerb) {
                cout << "in iSyst = " << iSyst << endl;
                cout << "setting systematic histogram shift up " << endl;
            }
            SetHistogram(vecOutYieldHist_SystVarUp[iSyst], inSUSYT2LP->typeT2, outHistName, 3, vecSystNames[iSyst] + TString("Up"));
            if (doVerb) {
                cout << "in iSyst = " << iSyst << endl;
                cout << "setting systematic histogram shift down " << endl;
            }
            
            SetHistogram(vecOutYieldHist_SystVarDown[iSyst], inSUSYT2LP->typeT2, outHistName, 3, vecSystNames[iSyst] + TString("Down"));
        }
        
        TH2F * histPDFEnvelope_ShiftUp;
        TH2F * histPDFEnvelope_ShiftDown;
        
        TH2F * histPDFEnvelopeBookKeep_ShiftUp;
        TH2F * histPDFEnvelopeBookKeep_ShiftDown;
        
        if (doVerb) {
            cout << "setting the envelope " << endl;
        }
        SetHistogram(histPDFEnvelope_ShiftUp, inSUSYT2LP->typeT2, outHistName, 3, TString("_PDFUp"));
        SetHistogram(histPDFEnvelope_ShiftDown, inSUSYT2LP->typeT2, outHistName, 3, TString("_PDFDown"));
        
        if (doVerb) {
            cout << "setting the envelope book-keeper " << endl;
        }
        SetHistogram(histPDFEnvelopeBookKeep_ShiftUp, inSUSYT2LP->typeT2, outHistName, 3, TString("_PDFEnvelopeUp"));
        SetHistogram(histPDFEnvelopeBookKeep_ShiftDown, inSUSYT2LP->typeT2, outHistName, 3, TString("_PDFEnvelopeDown"));
        
        if (doVerb) {
            cout << "pushing the PDF envelope hists into the vecSyst " << endl;
        }
        vecOutYieldHist_SystVarUp.push_back(histPDFEnvelope_ShiftUp);
        vecOutYieldHist_SystVarUp.push_back(histPDFEnvelopeBookKeep_ShiftUp);
        
        vecOutYieldHist_SystVarDown.push_back(histPDFEnvelope_ShiftDown);
        vecOutYieldHist_SystVarDown.push_back(histPDFEnvelopeBookKeep_ShiftDown);
    }
}
void SignalYieldMaker::WriteFile() {
    cout << "going to write to " << outFile->GetName() << endl;
    outFile->Write();
    outFile->Close();
}
void SignalYieldMaker::SetParamsFromCommandLine(int argc, char* argv[]) {
    for (int iSigYieldParam = 0; iSigYieldParam < argc; ++iSigYieldParam) {
        if (strncmp (argv[iSigYieldParam],"lumi", 4) == 0) {
            lumiData = strtol(argv[iSigYieldParam + 1], NULL, 10);
        }
        else if (strncmp (argv[iSigYieldParam],"doSC", 6) == 0) {
            doSigContam = 1;
        }
    }
}
#endif
