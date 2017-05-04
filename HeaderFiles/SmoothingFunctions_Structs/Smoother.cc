#ifndef Smoother_cc
#define Smoother_cc
void Smoother::DefaultVarVals_EfficiencySmoothing() {
    errMultiplier = 1.0;
    maxRelErrChangeThresh = 2.0;
    
    doCheckMaxRelChange = true;
    maxRelChange = 5.0;
    
    doLogNormal = true;
    
    minSigX = 10;
    maxSigX = 30;
    
    minSigY = 5;
    maxSigY = 25;
    
    numSigX = 10;
    numSigY = 10;
    
    smoothingPC.DefaultVarVals();
}
void Smoother::DefaultVarVals() {
    errMultiplier = 1.0;
    maxRelErrChangeThresh = 2.0;
    doCheckMaxRelChange = false;
    doLogNormal = false;
    
    minSigX = 10;
    maxSigX = 50;
    
    minSigY = 10;
    maxSigY = 50;
    
    numSigX = 10;
    numSigY = 10;
    
    smoothingPC.DefaultVarVals();
}

void Smoother::SetName() {
    char buf[300];
    sprintf(buf,"_MinX_%0i_MaxX_%0i_NumX_%0i_MinY_%0i_MaxY_%0i_NumY_%0i_ErrMult_%0.2f", minSigX, maxSigX, numSigX, minSigY, maxSigY, numSigY, errMultiplier);
    
    nameScan = doLogNormal ? "_doLogN" : "";
    
    nameScan += TString(string(buf));
}

void Smoother::SetParamsFromCommandLine(int argc, char* argv[]) {
    smoothingPC.SetParamsFromCommandLine(argc, argv);
    for (int iSmootherParam = 0; iSmootherParam < argc; ++iSmootherParam) {
        if (strncmp (argv[iSmootherParam],"doLogN", 6) == 0) {
            doLogNormal = true;
        }
        else if (strncmp (argv[iSmootherParam],"errMult", 7) == 0) {
            errMultiplier = strtod(argv[iSmootherParam + 1], NULL);
        }
        else if (strncmp (argv[iSmootherParam],"relThresh", 9) == 0) {
            maxRelErrChangeThresh = strtod(argv[iSmootherParam + 1], NULL);
        }
        else if (strncmp (argv[iSmootherParam],"setSigXScan", 11) == 0) {
            numSigX = strtol(argv[iSmootherParam + 1], NULL, 10);
            minSigX = strtol(argv[iSmootherParam + 2], NULL, 10);
            maxSigX = strtol(argv[iSmootherParam + 3], NULL, 10);
        }
        else if (strncmp (argv[iSmootherParam],"setSigYScan", 11) == 0) {
            numSigY = strtol(argv[iSmootherParam + 1], NULL, 10);
            minSigY = strtol(argv[iSmootherParam + 2], NULL, 10);
            maxSigY = strtol(argv[iSmootherParam + 3], NULL, 10);
        }
    }
}

void Smoother::SetKernelParams() {
    vecKPs.resize(0);
    float rangeX = maxSigX - minSigX;
    float stepSizeX = rangeX / numSigX;

    float rangeY = maxSigY - minSigY;
    float stepSizeY = rangeY / numSigY;

    
    float currSigX, currSigY;
    
    for (int iX = 0; iX <= numSigX; ++iX) {
        currSigX = minSigX + iX * stepSizeX;
        for (int iY = 0; iY <= numSigY; ++iY) {
            currSigY = minSigY + iY * stepSizeY;
            if (currSigX < currSigY) continue;
            /*
            cout << "currSigX " << currSigX << endl;
            cout << "currSigY " << currSigY << endl;
             */
            vecKPs.push_back(kernelParams(currSigX, currSigY));
        }
    }
    numKernels = vecKPs.size();
}

void Smoother::DeleteVecs() {
    for (unsigned int iKernel = 0; iKernel < numKernels; ++iKernel) {
        vecKernels[iKernel]->Delete();
        vecSmoothedHists[iKernel]->Delete();
    }
    
    vecKernels.clear();
    vecSmoothedHists.clear();
    
    unsigned int numDims = vecOptimalKernelWidths.size();
    for (unsigned int iDims = 0; iDims < numDims; ++iDims) {
        vecOptimalKernelWidths[iDims]->Delete();
    }
    
    vecOptimalKernelWidths.clear();
}


void Smoother::SetVecSizes() {
    vecKernels.resize(numKernels);
    vecSmoothedHists.resize(numKernels);
    
    int numDims = 2;
    vecOptimalKernelWidths.resize(numDims);
    
    vecVecIndex_OptKernel.resize(0);
}


void Smoother::SetSmoothMapFromOptimalKernel(TH2F * newHist, bool doVerbosity) {
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(newHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    float currBinContent_Orig, currBinError_Orig;
    float currBinContent_Smooth, currBinError_Smooth;
    
    //float currRelDiff;
    
    int currIndexOptKernel;
    TH2F * currOptHist;
    if (doVerbosity) {
        cout << "about to smooth out the optimal kernels " << endl;
        cout << "newHist nBinsX " << newHist->GetNbinsX() << endl;
        cout << "newHist nBinsY " << newHist->GetNbinsY() << endl;
        cout << "vecVecIndex_OptKernel size " << vecVecIndex_OptKernel.size() << endl;
        cout << "vecVecIndex_OptKernel[1] size " << vecVecIndex_OptKernel[1].size() << endl;
    }
    smoothingPC.SmoothWhichOptimalKernel(newHist, &vecVecIndex_OptKernel);
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            currIndexOptKernel = vecVecIndex_OptKernel[iBinX][iBinY];
            
            if (doVerbosity) {
                cout << "iBinX:iBinY " << iBinX << ":" << iBinY << endl;
                cout << "currIndexOptKernel " << currIndexOptKernel << endl;
            }
            vecOptimalKernelWidths[0]->SetBinContent(iBinX, iBinY, vecKPs[currIndexOptKernel].first);
            vecOptimalKernelWidths[1]->SetBinContent(iBinX, iBinY, vecKPs[currIndexOptKernel].second);
            
            currOptHist = vecSmoothedHists[currIndexOptKernel];
            
            currBinContent_Orig = newHist->GetBinContent(iBinX, iBinY);
            currBinError_Orig = newHist->GetBinError(iBinX, iBinY);
            
            currBinContent_Smooth = currOptHist->GetBinContent(iBinX, iBinY);
            currBinError_Smooth = currOptHist->GetBinError(iBinX, iBinY);
            
            if (currBinContent_Smooth == currBinContent_Orig) continue;
            
            if (currBinContent_Orig != 0) {
                if (doCheckMaxRelChange) {
                    if (currBinContent_Smooth/currBinContent_Orig > maxRelChange) {
                        currBinContent_Smooth = maxRelChange * currBinContent_Orig;
                    }
                    else if (currBinContent_Smooth/currBinContent_Orig < 1./maxRelChange) {
                        currBinContent_Smooth = 1./maxRelChange * currBinContent_Orig;
                    }
                }
                    
            }
            /*
            if (currBinError_Orig > 0) {
                currRelDiff = (currBinContent_Smooth - currBinContent_Orig)/(errMultiplier * currBinError_Orig);
            }
            else {
                currRelDiff = 0;
            }
            */
            /*
            cout << "iBinX " << iBinX << endl;
            cout << "iBinY " << iBinY << endl;
            cout << "currBinContent_Smooth " << currBinContent_Smooth << endl;
            cout << "currBinContent_Orig " << currBinContent_Orig << endl;
            cout << "currRelDiff " << currRelDiff << endl;
            cout << "errMultiplier * maxRelErrChangeThresh " << maxRelErrChangeThresh << endl;
            
            if (TMath::Abs(currRelDiff) > (maxRelErrChangeThresh)) {
                float currDiff = currBinContent_Smooth - currBinContent_Orig;
                currDiff *= (errMultiplier * currBinError_Orig)/(maxRelErrChangeThresh);
                cout << "currBinContent_Smooth pre change " << currBinContent_Smooth << endl;
                currBinContent_Smooth = currBinContent_Orig + currDiff;
                cout << "currBinContent_Smooth post change  " << currBinContent_Smooth << endl;
            }
            */
            
            newHist->SetBinContent(iBinX, iBinY, currBinContent_Smooth);
            newHist->SetBinError(iBinX, iBinY, currBinError_Smooth);
        }
    }
}








TH2F * Smoother::GenerateOptimalSmoothedMap(TH2F * inputHist, bool doVerbosity) {
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    TString addName = "_2DGaussSmoothed";
    float thresh = 0.0001;
    
    //Resize the index of optimal kernels to match the 2D hist bounds
    vecVecIndex_OptKernel.resize(vecInputHistAxisNBins[0] + 1);
    for (int iBinX = 1; iBinX < vecInputHistAxisNBins[0] + 1; ++iBinX) {
        vecVecIndex_OptKernel[iBinX].resize(vecInputHistAxisNBins[1] + 1);
    }
    
    //initialize the output optimal smoothed histogram
    TH2F * outOptimalHist = (TH2F *) inputHist->Clone(histName + addName + TString("_Optimal"));
    cout << "numKernels " << numKernels << endl;
    for (unsigned int iKernel = 0; iKernel < numKernels; ++iKernel) {
        if (doVerbosity) {
            cout << "on kernel " << iKernel + 1 << endl;
            cout << "about to generate kernels " << endl;
        }
        //Generate the kernels
        vecKernels[iKernel] = TwoDeeGaussianKernel(&vecKPs[iKernel]);
        if (doVerbosity) {
            cout << "about to generate smoothed histogram " << endl;
        }
        //Generate the smoothed histograms using the smoothing kernels
        vecSmoothedHists[iKernel] = KernelAveragedHistogram(inputHist, vecKernels[iKernel], doLogNormal, addName, thresh, doVerbosity);
    }
    
    //initialize the optimal kernel width information hists
    TString baseName_OptKernelWidth = "_OptKernelWidth_Axis";
    TString currAddName_OptKernelWidth;
    unsigned int numOptAxes = vecOptimalKernelWidths.size();
    for (unsigned int iOptAxis = 0; iOptAxis < numOptAxes; ++iOptAxis) {
        currAddName_OptKernelWidth = baseName_OptKernelWidth;
        currAddName_OptKernelWidth += iOptAxis + 1;
        vecOptimalKernelWidths[iOptAxis] = (TH2F *) inputHist->Clone(histName + currAddName_OptKernelWidth);
    }
    //Find which kernels yielded the optimal smoothing
    int currIndexOptKernel;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= TMath::Min(iBinX, vecInputHistAxisNBins[1]); ++iBinY) {
            smoothingPC.currCheckBinX = iBinX;
            smoothingPC.currCheckBinY = iBinY;
            if (doVerbosity) {
                cout << "Optimal Smoothing Generation iBinX:iBinY " << iBinX << ":" << iBinY << endl;
                cout << "about to generate the Pull vectors " << endl;
            }
            smoothingPC.MakePullVec(inputHist, &vecSmoothedHists, errMultiplier, doVerbosity);
            currIndexOptKernel = smoothingPC.ComparePulls(doVerbosity);
            
            vecVecIndex_OptKernel[iBinX][iBinY] = currIndexOptKernel;
        }
    }
    cout << "about to set smoothed map from optimal kernel " << endl;

    SetSmoothMapFromOptimalKernel(outOptimalHist, doVerbosity);
    
    //return the optimally smoothed hist
    return outOptimalHist;
}
#endif