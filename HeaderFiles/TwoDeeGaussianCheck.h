struct PullCheckParams {
    int currCheckBinX, currCheckBinY;
    int nBinsToCheckOver_X, nBinsToCheckOver_Y;
    //How many bins to look left of right of along axis;
    void DefaultVals() {
        nBinsToCheckOver_X = 2;
        nBinsToCheckOver_Y = 2;
    }
};


int ComparePulls(vector<TH1F *> * inVecPullDistributions, bool doVerbosity) {
    unsigned int numPull = inVecPullDistributions->size();
    
    vector<float> vecPullMeans(numPull), vecPullRMSDiff(numPull);
    vector<float> vecPullMeanErrors(numPull), vecPullRMSErrors(numPull);
    
    ///Find the kernel that yielded a pull distribution with RMS closest to 1.
    float currMinRMSDiff = 1E6;
    int indexMinRMSDiff = -1;
    for (unsigned int iPull = 0; iPull < numPull; ++iPull) {
        
        vecPullMeans[iPull] = TMath::Abs(inVecPullDistributions->at(iPull)->GetMean());
        vecPullRMSDiff[iPull] = TMath::Abs(inVecPullDistributions->at(iPull)->GetRMS() - 1);
        vecPullMeanErrors[iPull] = TMath::Abs(inVecPullDistributions->at(iPull)->GetMeanError());
        vecPullRMSErrors[iPull] = TMath::Abs(inVecPullDistributions->at(iPull)->GetRMSError());
        
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
                cout << " vecPullRMSErrors[iPull2] " <<  vecPullRMSErrors[iPull2] << endl;
            }
            if (vecPullMeans[iPull2] < currMinBias_CloseRMSes) {
                currIndexMinBias_CloseRMS = iPull2;
                currMinBias_CloseRMSes = vecPullMeans[iPull2];
            }
        }
    }
    return currIndexMinBias_CloseRMS;
}

TH1F * GetPullDistribution(TH2F * originalHist, TH2F * smoothedHist, PullCheckParams * inPCP, TString addName, float errMultiplier, bool doVerbosity) {
    ///Assumes the edge of the scan is iBinY <= iBinX
    int nBinsX = originalHist->GetNbinsX();
    int nBinsY = originalHist->GetNbinsY();
    
    TString histName = originalHist->GetName();
    histName += "_Pull_XCheck";
    histName += inPCP->currCheckBinX;
    histName += "_YCheck";
    histName += inPCP->currCheckBinY;
    histName += addName;
    
    TH1F * pullHist = new TH1F(histName, "; #frac{x - x'}{#sigma_{x}};number of entries", 2000, -50, 50);
    
    float currBinContent_Orig, currBinContent_Smooth, currBinError_Orig;
    //float currBinError_Smooth;
    
    if (doVerbosity) {
        cout << "currCheckBinX " << inPCP->currCheckBinX << endl;
        cout << "nBinsToCheckOver_X " << inPCP->nBinsToCheckOver_X << endl;
        cout << "currCheckBinY " << inPCP->currCheckBinY << endl;
        cout << "nBinsToCheckOver_Y " << inPCP->nBinsToCheckOver_Y << endl;
    }
    
    
    float currPull;
    for (int iBinX = TMath::Max(1, inPCP->currCheckBinX - inPCP->nBinsToCheckOver_X); iBinX <= TMath::Min(nBinsX, inPCP->currCheckBinX + inPCP->nBinsToCheckOver_X); ++iBinX) {
        for (int iBinY = TMath::Max(1, inPCP->currCheckBinY - inPCP->nBinsToCheckOver_Y); iBinY <= TMath::Min(nBinsY, inPCP->currCheckBinY + inPCP->nBinsToCheckOver_Y); ++iBinY) {
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

void GetPullVec(vector<TH1F *> * inVecPullVec, TH2F * originalHist, vector<TH2F *> * vecSmoothedHist, PullCheckParams * inPCP, float errMultiplier, bool doVerbosity) {
    
    inVecPullVec->resize(0);
    
    unsigned int numSmoothedHists = vecSmoothedHist->size();
    
    TString currAddName;
    
    
    for (unsigned int iSmooth = 0; iSmooth < numSmoothedHists; ++iSmooth) {
        currAddName = "_smoothing";
        currAddName += iSmooth;
        if (doVerbosity) {
            cout << "iSmooth " << iSmooth << endl;
        }
        inVecPullVec->push_back(GetPullDistribution(originalHist, vecSmoothedHist->at(iSmooth), inPCP, currAddName, errMultiplier, doVerbosity));
    }
}

void SetSmoothMapFromOptimalKernel(TH2F * newHist, vector< vector<int> > * inVecVecOptIntegers, vector<TH2F *> * inVecSmoothedHists_OptKernel, float maxRelErrChange) {
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(newHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    float currBinContent_Orig, currBinError_Orig;
    float currBinContent_Smooth, currBinError_Smooth;
    
    float currRelDiff;
    
    int currIndexOptKernel;
    TH2F * currOptHist;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            currIndexOptKernel = inVecVecOptIntegers->at(iBinX)[iBinY];
            currOptHist = inVecSmoothedHists_OptKernel->at(currIndexOptKernel);
            
            currBinContent_Orig = newHist->GetBinContent(iBinX, iBinY);
            currBinError_Orig = newHist->GetBinError(iBinX, iBinY);
            
            currBinContent_Smooth = currOptHist->GetBinContent(iBinX, iBinY);
            currBinError_Smooth = currOptHist->GetBinError(iBinX, iBinY);
            
            currRelDiff = (currBinContent_Smooth - currBinContent_Orig)/currBinError_Orig;
            /*
            if (TMath::Abs(currRelDiff) > maxRelErrChange) {
                currBinContent_Smooth = currBinContent_Orig * (1 + (currBinContent_Smooth - currBinContent_Orig)/(maxRelErrChange * currBinError_Orig));
            }
            */
            
            newHist->SetBinContent(iBinX, iBinY, currBinContent_Smooth);
            newHist->SetBinError(iBinX, iBinY, currBinError_Smooth);
        }
    }
}

TH2F * GenerateOptimalKernelMap(TH2F * originalHist, vector<kernelParams> * inVecKPs, vector<TH2F *> * inVecOptKernelValHists, PullCheckParams * inPCP, bool doLogNormal, float errMultiplier, float maxRelErrChangeThresh, bool doVerbosity) {
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(originalHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    //resize vectors for number of kernels
    unsigned int numKernels = inVecKPs->size();
    vector<TF2 *> vecKernels(numKernels);
    vector<TH2F *> vecSmoothedHist(numKernels);
    
    TString addName = "_2DGaussSmoothed";
    float thresh = 0.0001;
    
    vector< vector<int> > vecVecIndex_OptHist(vecInputHistAxisNBins[0] + 1);
    for (int iBinX = 1; iBinX < vecInputHistAxisNBins[0] + 1; ++iBinX) {
        vecVecIndex_OptHist[iBinX].resize(vecInputHistAxisNBins[1] + 1);
    }

    
    TH2F * outOptimalHist = (TH2F *) originalHist->Clone(histName + addName + TString("_Optimal"));
    
    for (unsigned int iKernel = 0; iKernel < numKernels; ++iKernel) {
        if (doVerbosity) {
            cout << "on kernel " << iKernel + 1 << endl;
            cout << "about to generate kernels " << endl;
        }
        //Generate the kernels
        vecKernels[iKernel] = TwoDeeGaussianKernel(&inVecKPs->at(iKernel));
        if (doVerbosity) {
            cout << "about to generate smoothed histogram " << endl;
        }
        //Generate the smoothed histograms using the smoothing kernels
        vecSmoothedHist[iKernel] = KernelAveragedHistogram(originalHist, vecKernels[iKernel], doLogNormal, addName, thresh, doVerbosity);
    }
    
    vector<TString> vecNames{"_OptSigX", "_OptSigY"};
    int numOpt = 2;
    inVecOptKernelValHists->resize(numOpt);
    for (int iOpt = 0; iOpt < numOpt; ++iOpt) {
        inVecOptKernelValHists->at(iOpt) = (TH2F *) originalHist->Clone(histName + vecNames[iOpt]);
    }
    
    vector<TH1F *> currPullVec(0);
    
    int currIndexOptKernel;
    
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= TMath::Min(iBinX, vecInputHistAxisNBins[1]); ++iBinY) {
            inPCP->currCheckBinX = iBinX;
            inPCP->currCheckBinY = iBinY;
            
            if (doVerbosity) {
                cout << "iBinX:iBinY " << iBinX << ":" << iBinY << endl;
                cout << "about to generate the Pull vectors " << endl;
            }
            currPullVec.resize(0);
            GetPullVec(&currPullVec, originalHist, &vecSmoothedHist, inPCP, errMultiplier, doVerbosity);
            
            currIndexOptKernel = ComparePulls(&currPullVec, doVerbosity);
            vecVecIndex_OptHist[iBinX][iBinY] = currIndexOptKernel;
            if (doVerbosity) {
                cout << "iBinX:iBinY " << iBinX << ":" << iBinY << endl;
                cout << "currIndexOptKernel " << currIndexOptKernel << endl;
            }
            inVecOptKernelValHists->at(0)->SetBinContent(iBinX, iBinY, inVecKPs->at(currIndexOptKernel).first);
            inVecOptKernelValHists->at(1)->SetBinContent(iBinX, iBinY, inVecKPs->at(currIndexOptKernel).second);
        }
    }
    SetSmoothMapFromOptimalKernel(outOptimalHist, &vecVecIndex_OptHist, &vecSmoothedHist, maxRelErrChangeThresh * errMultiplier);
    
    return outOptimalHist;
}

