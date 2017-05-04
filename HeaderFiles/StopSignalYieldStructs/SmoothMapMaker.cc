#ifndef SmoothMapMaker_cc
#define SmoothMapMaker_cc
void SmoothMapMaker::DefaultVals() {
    dirToRW = "SW";
    numRebins = 1;
    //numRebins = 0;
    addName = "_Rebin";
    addName += numRebins;
    addNameSmooth = "_2DGaussSmooth_OptimalKernel";
    DefaultSmootherValues();
}


void CombineSmoothEffs(SmoothMapMaker * inSMMNormal, SmoothMapMaker * inSMMTightBin, int doVerb = 0) {
    vector<TH2F *> vecAverHist_CV;
    vector<TH2F *> vecAverHist_SystVarUp, vecAverHist_SystVarDown;
    
    vecAverHist_CV.push_back(inSMMTightBin->histCV);
    
    float minAxisDiff = 99, maxAxisDiff = -1;
    
    int numSystsNorm = inSMMNormal->vecHist_SystVarUp.size();
    int numSystsTight = inSMMTightBin->vecHist_SystVarUp.size();

    if (doVerb) {
        cout << "numSystsNorm " << numSystsNorm << endl;
        cout << "numSystsTight " << numSystsTight << endl;
    }
    
    int maxIndex = TMath::Min(numSystsNorm, numSystsTight);
    int maxMaxIndex = TMath::Max(numSystsNorm, maxIndex);

    for (int iSyst = 0; iSyst < maxIndex; ++iSyst) {
        if (doVerb) {
            cout << "about to run the averaging for Shift Up for iSyst = " << iSyst << endl;
        }
        vecAverHist_SystVarUp.resize(0);
        vecAverHist_SystVarUp.push_back(inSMMTightBin->vecHist_SystVarUp[iSyst]);
        VarianceWeightedAverageHist(inSMMNormal->vecHist_SystVarUp[iSyst], &vecAverHist_SystVarUp, minAxisDiff, maxAxisDiff, doVerb);
        if (doVerb) {
            cout << "about to run the averaging for Shift Down for iSyst = " << iSyst << endl;
        }
        vecAverHist_SystVarDown.resize(0);
        vecAverHist_SystVarDown.push_back(inSMMTightBin->vecHist_SystVarDown[iSyst]);
        VarianceWeightedAverageHist(inSMMNormal->vecHist_SystVarDown[iSyst], &vecAverHist_SystVarDown, minAxisDiff, maxAxisDiff, doVerb);
    }
    
    for (int iSyst = maxIndex; iSyst < maxMaxIndex; ++iSyst) {
        inSMMNormal->vecHist_SystVarUp[iSyst]->Divide(inSMMNormal->histCV, inSMMNormal->vecHist_SystVarUp[iSyst]);
        inSMMNormal->vecHist_SystVarDown[iSyst]->Divide(inSMMNormal->histCV, inSMMNormal->vecHist_SystVarDown[iSyst]);
    }
    VarianceWeightedAverageHist(inSMMNormal->histCV, &vecAverHist_CV, minAxisDiff, maxAxisDiff, doVerb);
    
    for (int iSyst = maxIndex; iSyst < maxMaxIndex; ++iSyst) {
        inSMMNormal->vecHist_SystVarUp[iSyst]->Divide(inSMMNormal->histCV, inSMMNormal->vecHist_SystVarUp[iSyst]);
        inSMMNormal->vecHist_SystVarDown[iSyst]->Divide(inSMMNormal->histCV, inSMMNormal->vecHist_SystVarDown[iSyst]);
    }
}

void SmoothMapMaker::PrintBasic() {
    cout << "running interpolation " << numRebins << " times in the " << dirToRW << "direction; ";
    cout << "will be saving histograms with the added name " << addName << endl;
}

void SmoothMapMaker::SetFile(CoarseMapMaker * inCMM) {
    inFile = inCMM->outFile;
    vecHist_SystVarUp.resize(inCMM->vecOutHist_SystVarUp.size());
    vecHist_SystVarDown.resize(inCMM->vecOutHist_SystVarDown.size());
}

void SmoothMapMaker::DefaultSmootherValues() {
    scenarioSmoother.DefaultVarVals_EfficiencySmoothing();
}

typedef pair<int, int> axisRange;

void SmoothHist(TH2F * &inputHist, Smoother * inputSmoother, axisRange * inputAxRange_XAxis, axisRange * inputAxRange_YAxis, TString addName, bool doVerb = false) {
    
    TH2F * currConstDeltaMHist = OutputHistDeltaMChanges(inputHist, 0, 1, inputAxRange_YAxis, inputAxRange_XAxis, addName);
    
    TH2F * currSmoothHist = inputSmoother->GenerateOptimalSmoothedMap(currConstDeltaMHist, doVerb);
    
    TH2F * currSmoothedConstDeltaMHist_ReverseDeltaM = OutputHistDeltaMChanges(currSmoothHist, 0, -1, inputAxRange_YAxis, inputAxRange_XAxis, addName);
    
    TString originalName = inputHist->GetName() + addName;
    
    inputHist->Delete();
    inputHist = (TH2F *) currSmoothedConstDeltaMHist_ReverseDeltaM->Clone(originalName);
}

void SmoothMapMaker::SetHists(CoarseMapMaker * inCMM) {
    histCV = inCMM->outHist_CV;
    unsigned int numSysts = inCMM->vecSystNames.size();
    vecHist_SystVarUp.resize(numSysts);
    vecHist_SystVarDown.resize(numSysts);
    cout << "setting up " << numSysts << " systematics" <<endl;
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        vecHist_SystVarUp[iSyst] = inCMM->vecOutHist_SystVarUp[iSyst];
        vecHist_SystVarDown[iSyst] = inCMM->vecOutHist_SystVarDown[iSyst];
    }
    
}
void SmoothMapMaker::DoSmoothing(CoarseMapMaker * inCMM, bool doVerb = false) {
    SetHists(inCMM);
    
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(histCV, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    int minLSP = (int) (vecInputHistAxes[1]->GetBinCenter(1) + 0.5);
    int maxLSP = (int) (vecInputHistAxes[1]->GetBinCenter(vecInputHistAxisNBins[1]) + 0.5);
    
    int minStop = (int) (vecInputHistAxes[0]->GetBinCenter(1) + 0.5);
    int maxStop = (int) (vecInputHistAxes[0]->GetBinCenter(vecInputHistAxisNBins[0]) + 0.5);
    
    cout << "for hist " << histName << endl;
    cout << "minLSP " << minLSP << endl;
    cout << "maxLSP " << maxLSP << endl;
    cout << "minStop " << minStop << endl;
    cout << "maxStop " << maxStop << endl;
    
    
    axisRange axLSP(minLSP, maxLSP);
    axisRange axStop(minStop, maxStop);
    ///Smooth Central Value
    cout << "about to smooth central value " << endl;
    SmoothHist(histCV, &scenarioSmoother, &axStop, &axLSP, addNameSmooth, doVerb);
    
    cout << "histCV name " << histCV->GetName() << endl;
    unsigned int numSysts = vecHist_SystVarUp.size();
    
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        cout << "for iSyst = " << iSyst << ", i.e. " << inCMM->vecSystNames[iSyst] << endl;
        cout << "about to smooth syst shift up " << endl;
        SmoothHist(vecHist_SystVarUp[iSyst], &scenarioSmoother, &axStop, &axLSP, addNameSmooth, doVerb);
        cout << "about to smooth syst shift down " << endl;
        SmoothHist(vecHist_SystVarDown[iSyst], &scenarioSmoother, &axStop, &axLSP, addNameSmooth, doVerb);
        cout << "about to re-calculate systematic uncertainties from new smoothed maps" << endl;
        ResetSystHists(histCV, vecHist_SystVarUp[iSyst], vecHist_SystVarDown[iSyst], true, 3.0, doVerb);
    }
}
void SmoothMapMaker::DoInterpolation(bool doVerb = 0) {
    
    vector<TH2F *> vecCurrInterpHist;
    TString currHistName;
    TH2F * currHistToInterpolate;
    
    if (doVerb) {
        cout << "about to run interpolation on central value" << endl;
    }
    currHistToInterpolate = histCV;
    currHistName = currHistToInterpolate->GetName();
    SetInterpolatedHistVec(currHistToInterpolate, &vecCurrInterpHist, dirToRW, numRebins);
    if (numRebins > 0) {
        histCV = (TH2F *) vecCurrInterpHist[numRebins - 1]->Clone(currHistName);
    }
    unsigned int numSysts = vecHist_SystVarUp.size();
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        currHistToInterpolate = vecHist_SystVarUp[iSyst];
        currHistName = currHistToInterpolate->GetName();
        SetInterpolatedHistVec(currHistToInterpolate, &vecCurrInterpHist, dirToRW, numRebins);
        if (numRebins > 0) {
            vecHist_SystVarUp[iSyst] = (TH2F *) vecCurrInterpHist[numRebins - 1]->Clone(currHistName);
        }
        
        currHistToInterpolate = vecHist_SystVarDown[iSyst];
        currHistName = currHistToInterpolate->GetName();
        SetInterpolatedHistVec(currHistToInterpolate, &vecCurrInterpHist, dirToRW, numRebins);
        if (numRebins > 0) {
            vecHist_SystVarDown[iSyst] = (TH2F *) vecCurrInterpHist[numRebins - 1]->Clone(currHistName);
        }
    }
}

void SmoothMapMaker::DoInterpolation_Old(bool doVerb = 0) {
    if (doVerb) {
        cout << "about to run interpolation on " << inFile->GetName() << endl;
    }
    vecInterpHist = InterpolateCoarseMaps(inFile, dirToRW, numRebins, addName, doVerb);
    histCV = (TH2F *) vecInterpHist[0]->Clone();
    if (doVerb) {
        cout << "vecInterpHist.size() " << vecInterpHist.size() << endl;
    }
    /*
     cout << "vecHist_SystVarUp " << vecHist_SystVarUp.size() << endl;
     cout << "vecHist_SystVarDown " << vecHist_SystVarDown.size() << endl;
     */
    int checkSize = (vecInterpHist.size() - 1)/2;
    if ((int) vecHist_SystVarDown.size() > checkSize) {
        vecHist_SystVarUp.resize(checkSize);
        vecHist_SystVarDown.resize(checkSize);
    }
    for (unsigned int iVec = 1; iVec < vecInterpHist.size(); iVec+=2) {
        //cout << "test " << iVec << endl;
        vecHist_SystVarUp[(iVec - 1)/2] = (TH2F *) vecInterpHist[iVec]->Clone();
        //cout << "test " << iVec + 1 << endl;
        vecHist_SystVarDown[(iVec - 1)/2] = (TH2F *) vecInterpHist[iVec + 1]->Clone();
    }
}

void SmoothMapMaker::InitializeHistsandOutfile(SUSYT2LimitParams * inSUSYT2LP, CoarseMapMaker * inCMM, bool readFile, bool doVerb = 0) {
    
    TString outFileBaseName = "h_SmoothSigEff_";
    
    if (inCMM->doControl) {
        outFileBaseName += "Control";
    }
    
    outFileBaseName += inSUSYT2LP->SavingName(false, (inSUSYT2LP->typeT2 == 2));
    if (inCMM->LPC->typeDilepChannel > -1) outFileBaseName += inCMM->LPC->strDilepChannel;
    outFileBaseName += inCMM->LPC->strFullSel;
    outFileBaseName += inCMM->GetMT2String();
    
    int indexForSMSDir = inCMM->doControl ? -2 : 2;
    
    TString strSMSDir = inSUSYT2LP->SavingName(true) + SetSMSStringAppend(indexForSMSDir);
    //        TString baseDirOutFile = "EfficiencyMaps/" + strSMSDir;
    TString baseDirOutFile = strSMSDir;
    
    System_MakeDir(baseDirOutFile);
    
    TString outFileName = baseDirOutFile + outFileBaseName;
    outFileName += ".root";
    
    if (readFile) {
        outFileBaseName = outFileBaseName.Replace(outFileBaseName.Index("Smooth"), 6, "Coarse");
        outFile = TFile::Open(outFileName);
        TString grabName = outFileBaseName + addNameSmooth;
        if (doVerb) {
            cout << "trying to grab " << grabName << endl;
        }
        histCV = (TH2F *) outFile->Get(grabName);
        vecHist_SystVarUp.resize(inCMM->vecSystNames.size());
        vecHist_SystVarDown.resize(inCMM->vecSystNames.size());
        for (unsigned int iSyst = 0; iSyst < inCMM->vecSystNames.size(); ++iSyst) {
            TString grabNameSystUp = outFileBaseName + inCMM->vecSystNames[iSyst] + TString("Up") + addNameSmooth;;
            TString grabNameSystDown = outFileBaseName + inCMM->vecSystNames[iSyst] + TString("Down") + addNameSmooth;;
            if (doVerb) {
                cout << "trying to grab " << grabNameSystUp << " from the file " << outFile->GetName() << endl;
                cout << "size vec " << vecHist_SystVarUp.size() << endl;
            }
            vecHist_SystVarUp[iSyst] = (TH2F *) outFile->Get(grabNameSystUp);
            vecHist_SystVarDown[iSyst] = (TH2F *) outFile->Get(grabNameSystDown);
        }
    }
    else {
        outFile = new TFile(outFileName, "RECREATE");
    }
}
void SmoothMapMaker::WriteFile(bool closeFile = true) {
    cout << "I will be saving everything to " << outFile->GetName() << endl;
    outFile->cd();
    outFile->Write();
    histCV->Write();
    for (unsigned int iSyst = 0; iSyst < vecHist_SystVarUp.size(); ++iSyst) {
        vecHist_SystVarUp[iSyst]->Write();
        vecHist_SystVarDown[iSyst]->Write();
    }
    if (closeFile) {
        outFile->Close();
    }
}

void SmoothMapMaker::SetParamsFromCommandLine(int argc, char* argv[]) {
    for (int iSmoothMapParam = 0; iSmoothMapParam < argc; ++iSmoothMapParam) {
        if (strncmp (argv[iSmoothMapParam],"-n", 2) == 0) {
            numRebins = strtol(argv[iSmoothMapParam + 1], NULL, 10);
        }
        else if (strncmp (argv[iSmoothMapParam],"addName", 7) == 0) {
            addName = TString(argv[iSmoothMapParam + 1]);
        }
    }
    scenarioSmoother.SetParamsFromCommandLine(argc, argv);
    scenarioSmoother.SetKernelParams();
    scenarioSmoother.SetVecSizes();
}

void SmoothMapMaker::CombineSmoothEffs(SmoothMapMaker * inSMMTightBin, int doVerb = 0) {
    vector<TH2F *> vecAverHist_CV;
    vector<TH2F *> vecAverHist_SystVarUp, vecAverHist_SystVarDown;
    
    vecAverHist_CV.push_back(inSMMTightBin->histCV);
    
//    float minAxisDiff = 99, maxAxisDiff = -1;
    float minAxisDiff = 149, maxAxisDiff = 199;
    
    unsigned int numSystsNorm = vecHist_SystVarUp.size();
    unsigned int numSystsTight = inSMMTightBin->vecHist_SystVarUp.size();
    
    unsigned int maxIndex = TMath::Min(numSystsNorm, numSystsTight);
    unsigned int maxMaxIndex = TMath::Max(numSystsNorm, maxIndex);
    
    for (unsigned int iSyst = 0; iSyst < maxIndex; ++iSyst) {
        if (doVerb) {
            cout << "about to run the averaging for Shift Up for iSyst = " << iSyst << endl;
        }
        vecAverHist_SystVarUp.resize(0);
        vecAverHist_SystVarUp.push_back(inSMMTightBin->vecHist_SystVarUp[iSyst]);
        VarianceWeightedAverageHist(vecHist_SystVarUp[iSyst], &vecAverHist_SystVarUp, minAxisDiff, maxAxisDiff, doVerb);
        if (doVerb) {
            cout << "about to run the averaging for Shift Down for iSyst = " << iSyst << endl;
        }
        vecAverHist_SystVarDown.resize(0);
        vecAverHist_SystVarDown.push_back(inSMMTightBin->vecHist_SystVarDown[iSyst]);
        VarianceWeightedAverageHist(vecHist_SystVarDown[iSyst], &vecAverHist_SystVarDown, minAxisDiff, maxAxisDiff, doVerb);
    }
    
    
    //Need to hack the systematics which don't exist, namely PDF uncertainties in tight bin
    for (unsigned int iSyst = maxIndex; iSyst < maxMaxIndex; ++iSyst) {
        vecHist_SystVarUp[iSyst]->Divide(histCV, vecHist_SystVarUp[iSyst]);
        vecHist_SystVarDown[iSyst]->Divide(histCV, vecHist_SystVarDown[iSyst]);
    }
    //then you can do the variance weighted average hist
    VarianceWeightedAverageHist(histCV, &vecAverHist_CV, minAxisDiff, maxAxisDiff, doVerb);
    
    for (unsigned int iSyst = maxIndex; iSyst < maxMaxIndex; ++iSyst) {
        vecHist_SystVarUp[iSyst]->Divide(histCV, vecHist_SystVarUp[iSyst]);
        vecHist_SystVarDown[iSyst]->Divide(histCV, vecHist_SystVarDown[iSyst]);
    }
}







#endif