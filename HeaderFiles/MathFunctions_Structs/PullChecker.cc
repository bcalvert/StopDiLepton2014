#ifndef PullChecker_cc
#define PullChecker_cc
void PullChecker::DefaultVarVals() {
    nBinsToCheckOver_X = 2;
    nBinsToCheckOver_Y = 2;
    
    nBinsExtraToSync_X = 1;
    nBinsExtraToSync_Y = 1;
}


TH1F * PullChecker::GetPullDistribution(TH2F * originalHist, TH2F * smoothedHist, TString addName, float errMultiplier, bool doVerbosity) {
    ///Assumes the edge of the scan is iBinY <= iBinX
    int nBinsX = originalHist->GetNbinsX();
    int nBinsY = originalHist->GetNbinsY();
    
    TString histName = originalHist->GetName();
    histName += "_Pull_XCheck";
    histName += currCheckBinX;
    histName += "_YCheck";
    histName += currCheckBinY;
    histName += addName;
    
    TH1F * pullHist = new TH1F(histName, "; #frac{x - x'}{#sigma_{x}};number of entries", 2000, -50, 50);
    pullHist->Sumw2();
    
    float currBinContent_Orig, currBinContent_Smooth, currBinError_Orig;
    //float currBinError_Smooth;
    
    if (doVerbosity) {
        cout << "currCheckBinX " << currCheckBinX << endl;
        cout << "nBinsToCheckOver_X " << nBinsToCheckOver_X << endl;
        cout << "currCheckBinY " << currCheckBinY << endl;
        cout << "nBinsToCheckOver_Y " << nBinsToCheckOver_Y << endl;
    }
    
    
    float currPull;
    for (int iBinX = TMath::Max(1, currCheckBinX - nBinsToCheckOver_X); iBinX <= TMath::Min(nBinsX, currCheckBinX + nBinsToCheckOver_X); ++iBinX) {
        for (int iBinY = TMath::Max(1, currCheckBinY - nBinsToCheckOver_Y); iBinY <= TMath::Min(nBinsY, currCheckBinY + nBinsToCheckOver_Y); ++iBinY) {
            if (iBinY > iBinX) continue;
            currBinContent_Orig = originalHist->GetBinContent(iBinX, iBinY);
            currBinContent_Smooth = smoothedHist->GetBinContent(iBinX, iBinY);
            currBinError_Orig = originalHist->GetBinError(iBinX, iBinY);
            if (currBinContent_Orig == 0 && currBinError_Orig == 0) continue;
            //currBinError_Smooth = smoothedHist->GetBinError(iBinX, iBinY);
            currPull = (currBinContent_Orig - currBinContent_Smooth)/(errMultiplier * currBinError_Orig);
            if (doVerbosity) {
                cout << "iBinX:iBinY " << iBinX << ":" << iBinY << endl;
                cout << "currBinContent_Orig " << currBinContent_Orig << endl;
                cout << "currBinContent_Smooth " << currBinContent_Smooth << endl;
                cout << "currBinError_Orig " << currBinError_Orig << endl;
                cout << "currPull " << currPull << endl;
            }
            pullHist->Fill(currPull);
        }
    }
    return pullHist;
}

void PullChecker::DeleteCurrPullHistsVecs() {
    unsigned int numPulls = vecPullHists.size();
    
    for (unsigned int iPull = 0; iPull < numPulls; ++iPull) {
        vecPullHists[iPull]->Delete();
    }
    vecPullHists.resize(0);
}

void PullChecker::MakePullVec(TH2F * originalHist, vector<TH2F *> * vecSmoothedHist, float errMultiplier, bool doVerbosity) {
    DeleteCurrPullHistsVecs();
    
    unsigned int numSmoothedHists = vecSmoothedHist->size();
    TString currAddName;
    
    vecPullHists.resize(numSmoothedHists);
    for (unsigned int iSmooth = 0; iSmooth < numSmoothedHists; ++iSmooth) {
        currAddName = "_smoothing";
        currAddName += iSmooth;
        if (doVerbosity) {
            cout << "iSmooth " << iSmooth << endl;
        }
        vecPullHists[iSmooth] = GetPullDistribution(originalHist, vecSmoothedHist->at(iSmooth), currAddName, errMultiplier, doVerbosity);
    }
}


void PullChecker::SmoothWhichOptimalKernel(TH2F * newHist, vector< vector<int> > * inVecVecOptIntegers) {
    //"Smooth" out the optimal kernel widths
    
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(newHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    int currOptKernel;
    for (int iBinX = 1 + nBinsExtraToSync_X; iBinX <= vecInputHistAxisNBins[0] + nBinsExtraToSync_X; iBinX += (1 + 2 *nBinsExtraToSync_X)) {
        if (iBinX - nBinsExtraToSync_X > vecInputHistAxisNBins[0]) continue;
        for (int iBinY = 1 + nBinsExtraToSync_Y; iBinY <= vecInputHistAxisNBins[1] + nBinsExtraToSync_Y; iBinY += (1 + 2 *nBinsExtraToSync_Y)) {
            if (iBinY - nBinsExtraToSync_Y > vecInputHistAxisNBins[1]) continue;
            currOptKernel = inVecVecOptIntegers->at(TMath::Min(iBinX, vecInputHistAxisNBins[0]))[TMath::Min(iBinY, vecInputHistAxisNBins[1])];
            for (int resetBinX = iBinX - nBinsExtraToSync_X; resetBinX <= TMath::Min(iBinX + nBinsExtraToSync_X, vecInputHistAxisNBins[0]); ++resetBinX) {
                for (int resetBinY = iBinY - nBinsExtraToSync_Y; resetBinY <= TMath::Min(iBinY + nBinsExtraToSync_Y, vecInputHistAxisNBins[1]); ++resetBinY) {
                    /*
                    cout << "vecInputHistAxisNBins[0] " << vecInputHistAxisNBins[0] << endl;
                    cout << "vecInputHistAxisNBins[1] " << vecInputHistAxisNBins[1] << endl;
                    cout << "iBinX " << iBinX << endl;
                    cout << "iBinY " << iBinY << endl;
                    cout << "resetBinX " << resetBinX << endl;
                    cout << "resetBinY " << resetBinY << endl;
                    */
                    inVecVecOptIntegers->at(resetBinX)[resetBinY] = currOptKernel;
                }
            }
        }
    }
}


void PullChecker::SetParamsFromCommandLine(int argc, char* argv[]) {
    for (int iPullCheckerParam = 0; iPullCheckerParam < argc; ++iPullCheckerParam) {
        if (strncmp (argv[iPullCheckerParam],"setCheckBins", 12) == 0) {
            nBinsToCheckOver_X = strtol(argv[iPullCheckerParam + 1], NULL, 10);
            nBinsToCheckOver_Y = strtol(argv[iPullCheckerParam + 2], NULL, 10);
        }
        else if (strncmp (argv[iPullCheckerParam],"setSyncBins", 11) == 0) {
            nBinsExtraToSync_X = strtol(argv[iPullCheckerParam + 1], NULL, 10);
            nBinsExtraToSync_Y = strtol(argv[iPullCheckerParam + 2], NULL, 10);
        }
    }
}

int PullChecker::ComparePulls(bool doVerbosity) {
    unsigned int numPull = vecPullHists.size();
    
    vector<float> vecPullMeans(numPull), vecPullRMSDiff(numPull);
    vector<float> vecPullMeanErrors(numPull), vecPullRMSErrors(numPull);
    
    ///Find the kernel that yielded a pull distribution with RMS closest to 1.
    float currMinRMSDiff = 1E6;
    int indexMinRMSDiff = -1;
    
    bool foundZeroSmoothingCase = false;
    
    for (unsigned int iPull = 0; iPull < numPull; ++iPull) {
        
        vecPullMeans[iPull] = TMath::Abs(vecPullHists[iPull]->GetMean());
        vecPullRMSDiff[iPull] = TMath::Abs(vecPullHists[iPull]->GetRMS() - 1);
        vecPullMeanErrors[iPull] = TMath::Abs(vecPullHists[iPull]->GetMeanError());
        vecPullRMSErrors[iPull] = TMath::Abs(vecPullHists[iPull]->GetRMSError());
        
        /*
        if (vecPullMeans[iPull] == 0 && vecPullRMSDiff[iPull] == 1) {
            if (vecPullRMSErrors[iPull] == 0 && vecPullMeanErrors[iPull] == 0) {
                foundZeroSmoothingCase = true;
                continue;
            }
        }
        */
        
        if (doVerbosity) {
            cout << "iPull " << iPull << endl;
            cout << "vecPullMeans[iPull] " << vecPullMeans[iPull] << endl;
            cout << "vecPullRMSDiff[iPull] " << vecPullRMSDiff[iPull] << endl;
            cout << "vecPullMeanErrors[iPull] " << vecPullMeanErrors[iPull] << endl;
            cout << " vecPullRMSErrors[iPull] " <<  vecPullRMSErrors[iPull] << endl;
        }
        
        if (vecPullRMSDiff[iPull] < currMinRMSDiff) {
            indexMinRMSDiff = iPull;
            currMinRMSDiff = vecPullRMSDiff[iPull];
        }
    }
    
    if (indexMinRMSDiff == -1 && foundZeroSmoothingCase) {
        if (doVerbosity) {
            
        }
        cout << "none of the kernels did any smoothing so you should reset to first kernel " << endl;
        indexMinRMSDiff = 0;
        currMinRMSDiff = vecPullRMSDiff[indexMinRMSDiff];
    }
    
    ///For all kernels that yielded RMSes "close" to the aforementioned kernel ("close" being RMS_i - RMS_j = 0 within uncertainties)
    ///Minimize the bias of the pull distribution
    int currIndexMinBias_CloseRMS = indexMinRMSDiff;
    
    float currMinBias_CloseRMSes = 1E6;
    
    float currNetError_RMSDiff;
    //vector<pair<float, int>> vecBias_CloseRMSes(0);
    
    for (unsigned int iPull2 = 0; iPull2 < numPull; ++iPull2) {
        currNetError_RMSDiff = TMath::Sqrt(TMath::Power(vecPullMeanErrors[iPull2], 2) + TMath::Power(vecPullMeanErrors[indexMinRMSDiff], 2));
        if ((int) iPull2 == indexMinRMSDiff) currNetError_RMSDiff = 0;
        if (doVerbosity) {
            cout << "iPull2 " << iPull2 << endl;
            cout << "currNetError_RMSDiff " << currNetError_RMSDiff << endl;
        }
        if (TMath::Abs(vecPullRMSDiff[iPull2] - vecPullRMSDiff[indexMinRMSDiff]) < currNetError_RMSDiff) {
            if (doVerbosity) {
                cout << "iPull2 " << iPull2 << endl;
                cout << "vecPullMeans[iPull2] " << vecPullMeans[iPull2] << endl;
                cout << "vecPullRMSDiff[iPull2] " << vecPullRMSDiff[iPull2] << endl;
                cout << "vecPullMeanErrors[iPull2] " << vecPullMeanErrors[iPull2] << endl;
                cout << "vecPullRMSErrors[iPull2] " <<  vecPullRMSErrors[iPull2] << endl;
            }
            if (vecPullMeans[iPull2] < currMinBias_CloseRMSes) {
                currIndexMinBias_CloseRMS = iPull2;
                currMinBias_CloseRMSes = vecPullMeans[iPull2];
            }
        }
    }
    return currIndexMinBias_CloseRMS;
}
#endif