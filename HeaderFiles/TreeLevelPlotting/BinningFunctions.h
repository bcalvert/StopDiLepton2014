void DoOverflow(TH1 * inputHist) {
    int nBinsX = inputHist->GetNbinsX();
    int nBinsY = inputHist->GetNbinsY();
    int nBinsZ = inputHist->GetNbinsZ();
    
    float currBinContent, currBinContentToAddTo;
    
    int currBinToAddToX, currBinToAddToY, currBinToAddToZ;
    bool isOverflowX = false;
    bool isOverflowY = false;
    bool isOverflowZ = false;
    for (int iBinX = 0; iBinX <= nBinsX + 1; ++iBinX) {
        isOverflowX = false;
        currBinToAddToX = iBinX;
        if (currBinToAddToX == 0) {
            isOverflowX = true;
            currBinToAddToX = 1;
        }
        else if (currBinToAddToX > nBinsX) {
            isOverflowX = true;
            currBinToAddToX = nBinsX;
        }
        for (int iBinY = 0; iBinY <= nBinsY + 1; ++iBinY) {
            isOverflowY = false;
            currBinToAddToY = iBinY;
            if (currBinToAddToY == 0) {
                isOverflowY = true;
                currBinToAddToY = 1;
            }
            else if (currBinToAddToY > nBinsY) {
                isOverflowY = true;
                currBinToAddToY = nBinsY;
            }
            for (int iBinZ = 0; iBinZ <= nBinsZ + 1; ++iBinZ) {
                isOverflowZ = false;
                currBinToAddToZ = iBinZ;
                if (currBinToAddToZ == 0) {
                    isOverflowZ = true;
                    currBinToAddToZ = 1;
                }
                else if (currBinToAddToZ > nBinsZ) {
                    isOverflowZ = true;
                    currBinToAddToZ = nBinsZ;
                }
                if (!(isOverflowX || isOverflowY || isOverflowZ)) continue;
                currBinContent = inputHist->GetBinContent(iBinX, iBinY, iBinZ);
                currBinContentToAddTo = inputHist->GetBinContent(currBinToAddToX, currBinToAddToY, currBinToAddToZ);

                inputHist->SetBinContent(iBinX, iBinY, iBinZ, 0);
                inputHist->SetBinContent(currBinToAddToX, currBinToAddToY, currBinToAddToZ, currBinContentToAddTo + currBinContent);
            }
        }
    }
}
void CopyToBetterBinnedHist(TH3F * inputHistToCopyFrom, TH3F * inputHistToCopyTo, int doVerb = 0) {
    int nBinsX = inputHistToCopyFrom->GetNbinsX();
    int nBinsY = inputHistToCopyFrom->GetNbinsY();
    int nBinsZ = inputHistToCopyFrom->GetNbinsZ();
    TAxis * xAxis = inputHistToCopyFrom->GetXaxis();
    TAxis * yAxis = inputHistToCopyFrom->GetYaxis();
    TAxis * zAxis = inputHistToCopyFrom->GetZaxis();
    
    TAxis * xAxisProjHist = inputHistToCopyTo->GetXaxis();
    TAxis * yAxisProjHist = inputHistToCopyTo->GetYaxis();
    TAxis * zAxisProjHist = inputHistToCopyTo->GetZaxis();
    
    float currX, currY, currZ;
    int currProjHistBinX, currProjHistBinY, currProjHistBinZ;
    float currBinContent, currBinContentProjHist;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currX = xAxis->GetBinCenter(iBinX);
        if (doVerb > 1) {
            cout << "currX " << currX << endl;
        }
        currProjHistBinX = xAxisProjHist->FindBin(currX);
        if (doVerb > 1) {
            cout << "currProjHistBinX " << currProjHistBinX << endl;
        }
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currY = yAxis->GetBinCenter(iBinY);
            currProjHistBinY = yAxisProjHist->FindBin(currY);
            for (int iBinZ = 1; iBinZ <= nBinsZ; ++iBinZ) {
                currZ = zAxis->GetBinCenter(iBinZ);
                currProjHistBinZ = zAxisProjHist->FindBin(currZ);
                currBinContent = inputHistToCopyFrom->GetBinContent(iBinX, iBinY, iBinZ);
                currBinContentProjHist = inputHistToCopyTo->GetBinContent(currProjHistBinX, currProjHistBinY, currProjHistBinZ);
                if (doVerb > 1) {
                    cout << "iBinX:iBinY:iBinZ " << iBinX << ":" << iBinY << ":" << iBinZ << endl;
                    cout << "currBinContent " << currBinContent << endl;
                    cout << "currBinContentProjHist " << currBinContentProjHist << endl;
                }
                inputHistToCopyTo->SetBinContent(currProjHistBinX, currProjHistBinY, currProjHistBinZ, currBinContentProjHist + currBinContent);
            }
        }
    }
}
void AddInFullAxis(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     40 bins from 0 to 400 -- 40 bins
     */
    const int numBinsTotal = 50;
    
    vector<Double_t> vecFullAxis(numBinsTotal + 1);
    vecFullAxis[0] = 0;
    
    const int numRanges = 1;
    int binRangeIndexEPs[numRanges] = {501};
    int stepSize[numRanges] = {10};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecFullAxis[binIndex] = vecFullAxis[binIndex - 1] + stepSizeToUse;
        //cout << "vecFullAxis at binIndex " << binIndex << " is " << vecFullAxis[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecFullAxis);
    vecNumBins->push_back(numBinsTotal);
}

void AddInFullAxisMT2bb_ZMET(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     42 bins from 80 to 500 -- 42 bins
     */
    const int numBinsTotal = 42;
    
    vector<Double_t> vecFullAxis(numBinsTotal + 1);
    vecFullAxis[0] = 80;
    
    const int numRanges = 1;
    int binRangeIndexEPs[numRanges] = {43};
    int stepSize[numRanges] = {10};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecFullAxis[binIndex] = vecFullAxis[binIndex - 1] + stepSizeToUse;
        //cout << "vecFullAxis at binIndex " << binIndex << " is " << vecFullAxis[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecFullAxis);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInMT2llBounds3DShapeTwo(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     80 GeV bin from 0 to 80 -- 1 bin
     20 GeV bins 80 to 120 -- 2 bins
     50 GeV Signal bin after 120 -- 1 bin
     */
    const int numBinsTotal = 4;
    vector<Double_t> vecMT2ll(numBinsTotal + 1);
    vecMT2ll[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 4, 5};
    int stepSize[numRanges] = {80, 20, 50};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecMT2ll[binIndex] = vecMT2ll[binIndex - 1] + stepSizeToUse;
        //cout << "vecMT2ll at binIndex " << binIndex << " is " << vecMT2ll[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecMT2ll);
    vecNumBins->push_back(numBinsTotal);
}

inline void AddInMT2llBounds3DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     80 GeV bin from 0 to 80 -- 1 bin
     40 GeV bins 80 to 120 -- 1 bins
     50 GeV Signal bin after 120 -- 1 bin
     */
    const int numBinsTotal = 3;
    vector<Double_t> vecMT2ll(numBinsTotal + 1);
    vecMT2ll[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 3, 4};
    int stepSize[numRanges] = {80, 40, 50};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecMT2ll[binIndex] = vecMT2ll[binIndex - 1] + stepSizeToUse;
        //cout << "vecMT2ll at binIndex " << binIndex << " is " << vecMT2ll[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecMT2ll);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInMT2lblbBounds3DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     170 GeV bin from 0 to 170 -- 1 bin
     30 GeV bin from 170 to 200 -- 1 bins
     200 GeV bin from 200 to 400 -- 1 bin
     */
    const int numBinsTotal = 3;
    
    vector<Double_t> vecMT2lblb(numBinsTotal + 1);
    vecMT2lblb[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 3, 4};
    int stepSize[numRanges] = {170, 30, 200};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecMT2lblb[binIndex] = vecMT2lblb[binIndex - 1] + stepSizeToUse;
        //cout << "vecMT2lblb at binIndex " << binIndex << " is " << vecMT2lblb[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecMT2lblb);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInMT2lblbBounds3DShapeTwo(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     170 GeV bin from 0 to 170 -- 1 bin
     230 GeV bin from 200 to 400 -- 1 bin
     */
    const int numBinsTotal = 2;
    
    vector<Double_t> vecMT2lblb(numBinsTotal + 1);
    vecMT2lblb[0] = 0.0;
    const int numRanges = 2;
    int binRangeIndexEPs[numRanges] = {2, 3};
    int stepSize[numRanges] = {170, 230};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecMT2lblb[binIndex] = vecMT2lblb[binIndex - 1] + stepSizeToUse;
        //cout << "vecMT2lblb at binIndex " << binIndex << " is " << vecMT2lblb[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecMT2lblb);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInMT2lblbBounds3DShapeThree(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
  /*                                                                                                                                                                       
     170 GeV bin from 0 to 170 -- 1 bin                                                                                                                                      
     30 GeV bin from 170 to 200 -- 1 bin
     50 GeV bin from 200 to 250 -- 1 bin                                                                                                                                    
     150 GeV bin from 250 to 400 -- 1 bin                                                                                                                                    
  */
  const int numBinsTotal = 4;

  vector<Double_t> vecMT2lblb(numBinsTotal + 1);
  vecMT2lblb[0] = 0.0;
  const int numRanges = 4;
  int binRangeIndexEPs[numRanges] = {2, 3, 4, 5};
  int stepSize[numRanges] = {170, 30, 50, 150};

  int stepSizeToUse = -1;

  for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
    for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
      if (binIndex < binRangeIndexEPs[iBRIndex]) {
	stepSizeToUse = stepSize[iBRIndex];
	break;
      }
    }
    vecMT2lblb[binIndex] = vecMT2lblb[binIndex - 1] + stepSizeToUse;
    //cout << "vecMT2lblb at binIndex " << binIndex << " is " << vecMT2lblb[binIndex] << endl;                                                                           
  }
  inVecVecBinEdges->push_back(vecMT2lblb);
  vecNumBins->push_back(numBinsTotal);
}

inline void AddInMT2bb_ZMETBounds3DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     100 GeV bin from 70 to 170 -- 1 bin
     30 GeV bin from 170 to 200 -- 1 bin
     50 GeV bins from 200 to 250 -- 1 bins
     200 GeV bins from 250 to 450 -- 1 bin
     */
    const int numBinsTotal = 4;
    
    vector<Double_t> vecMT2bb_ZMET(numBinsTotal + 1);
    vecMT2bb_ZMET[0] = 70.0;
    
    const int numRanges = 4;
    int binRangeIndexEPs[numRanges] = {2, 3, 4, 5};
    int stepSize[numRanges] = {100, 30, 50, 200};
    
    int stepSizeToUse = -1;
    
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecMT2bb_ZMET[binIndex] = vecMT2bb_ZMET[binIndex - 1] + stepSizeToUse;
        //cout << "vecMT2bb_ZMET at binIndex " << binIndex << " is " << vecMT2bb_ZMET[binIndex] << endl;
    }
    
    inVecVecBinEdges->push_back(vecMT2bb_ZMET);
    vecNumBins->push_back(numBinsTotal);
}
