inline void AddInKT2_TopBounds(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     200 GeV Signal from -300 to -100 -- 1 bin
     35 GeV bins from -100 to 40 -- 4 bins
     10 GeV bins from 40 to 120 -- 8 bins
     25 GeV bins from 120 to 220 -- 2 bins
     40 GeV bins from 220 to 300 -- 2 bins
     200 GeV Signal bins from 300 to 500 -- 1 bin
     */
    const int numBinsTotal = 18;
    
    vector<Double_t> vecKT2_Top(numBinsTotal + 1);
    vecKT2_Top[0] = -300.0;
    
    const int numRanges = 6;
    int binRangeIndexEPs[numRanges] = {2, 6, 14, 16, 18, 19};
    int stepSize[numRanges] = {200, 35, 10, 25, 40, 200};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecKT2_Top[binIndex] = vecKT2_Top[binIndex - 1] + stepSizeToUse;
        //cout << "vecKT2_Top at binIndex " << binIndex << " is " << vecKT2_Top[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecKT2_Top);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInKT2_TopBounds2DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     300 GeV Signal from -300 to 0 -- 1 bin
     100 GeV bins from 0 to 200 -- 2 bins
     300 GeV bin from 200 to 500 -- 1 bin
     */
    const int numBinsTotal = 4;
    
    vector<Double_t> vecKT2_Top(numBinsTotal + 1);
    vecKT2_Top[0] = -300.0;
    
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 4, 5};
    int stepSize[numRanges] = {300, 100, 300};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecKT2_Top[binIndex] = vecKT2_Top[binIndex - 1] + stepSizeToUse;
        //cout << "vecKT2_Top at binIndex " << binIndex << " is " << vecKT2_Top[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecKT2_Top);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInKT2_TopBounds3DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     300 GeV Signal from -300 to 0 -- 1 bin
     100 GeV bins from 0 to 200 -- 2 bins
     300 GeV bin from 200 to 500 -- 1 bin
     */
    const int numBinsTotal = 4;
    
    vector<Double_t> vecKT2_Top(numBinsTotal + 1);
    vecKT2_Top[0] = -300.0;
    
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 4, 5};
    int stepSize[numRanges] = {300, 100, 300};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecKT2_Top[binIndex] = vecKT2_Top[binIndex - 1] + stepSizeToUse;
        //cout << "vecKT2_Top at binIndex " << binIndex << " is " << vecKT2_Top[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecKT2_Top);
    vecNumBins->push_back(numBinsTotal);
}
inline void AddInMT2bb_ZMETBounds2DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     100 GeV bin from 70 to 170 -- 1 bin
     40 GeV bins from 170 to 250 -- 2 bins
     200 GeV bins from 250 to 450 -- 1 bin
     */
    const int numBinsTotal = 4;
    
    vector<Double_t> vecMT2bb_ZMET(numBinsTotal + 1);
    vecMT2bb_ZMET[0] = 70.0;
    
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 4, 5};
    int stepSize[numRanges] = {100, 40, 200};
    
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

inline void AddInMT2bb_ZMETBounds3DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     100 GeV bin from 70 to 170 -- 1 bin
     80 GeV bins from 170 to 250 -- 1 bins
     200 GeV bins from 250 to 450 -- 1 bin
     */
    const int numBinsTotal = 3;
    
    vector<Double_t> vecMT2bb_ZMET(numBinsTotal + 1);
    vecMT2bb_ZMET[0] = 70.0;
    
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 3, 4};
    int stepSize[numRanges] = {100, 80, 200};
    
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

inline void AddInMT2bb_ZMETBounds_NonControl(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     10 GeV bins from 70 to 150 -- 8 bins
     5 GeV bins from 150 to 200 -- 10 bins
     25 GeV bins from 200 to 250 -- 2 bins
     50 GeV bins from 250 to 350 -- 2 bins
     150 GeV Signal bin from 350 to 500 -- 1 bin
     */
    const int numBinsTotal = 23;
    
    vector<Double_t> vecMT2bb_ZMET(numBinsTotal + 1);
    vecMT2bb_ZMET[0] = 70.0;

    const int numRanges = 5;
    int binRangeIndexEPs[numRanges] = {9, 19, 21, 23, 24};
    int stepSize[numRanges] = {10, 5, 25, 50, 150};
    
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

inline void AddInMT2bb_ZMETBounds_Control(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     170 GeV bin from 0 to 170 -- 1 bin
     5 GeV bins from 170 to 200 -- 6 bins
     25 GeV bins from 200 to 250 -- 2 bins
     50 GeV bins from 250 to 350 -- 2 bins
     150 GeV Signal bin from 350 to 500 -- 1 bin
     */
    const int numBinsTotal = 12;
    
    vector<Double_t> vecMT2bb_ZMET(numBinsTotal + 1);
    vecMT2bb_ZMET[0] = 0.0;
    
    const int numRanges = 5;
    int binRangeIndexEPs[numRanges] = {2, 8, 10, 12, 13};
    int stepSize[numRanges] = {170, 5, 25, 50, 150};
    
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

inline void AddInMT2lblbBounds_NonControl(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     1 GeV bin right at 0 -- 1 bin
     9 GeV bin from 1 to 10 -- 1 bin
     10 GeV bins from 10 to 100 -- 9 bins
     10 GeV bins from 100 to 150 -- 5 bins
     10 GeV bins from 150 to 200 -- 5 bins
     100 GeV bin from 200 to 300 -- 1 bin
     200 GeV Signal bin from 300 to 500 -- 1 bin
     */
    const int numBinsTotal = 23;
    
    vector<Double_t> vecMT2lblb(numBinsTotal + 1);
    vecMT2lblb[0] = 0.0;
    const int numRanges = 7;
    int binRangeIndexEPs[numRanges] = {2, 3, 12, 17, 22, 23, 24};
    int stepSize[numRanges] = {1, 9, 10, 10, 10, 100, 200};
    
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
inline void AddInMT2lblbBounds3DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     150 GeV bin from 0 to 150 -- 1 bin
     50 GeV bin from 150 to 200 -- 1 bins
     200 GeV bin from 200 to 400 -- 1 bin
     */
    const int numBinsTotal = 3;
    
    vector<Double_t> vecMT2lblb(numBinsTotal + 1);
    vecMT2lblb[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 3, 4};
    int stepSize[numRanges] = {150, 50, 200};
    
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
inline void AddInMT2lblbBounds2DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     150 GeV bin from 0 to 150 -- 1 bin
     25 GeV bins from 150 to 200 -- 2 bins
     200 GeV bin from 200 to 400 -- 1 bin
     */
    const int numBinsTotal = 4;
    
    vector<Double_t> vecMT2lblb(numBinsTotal + 1);
    vecMT2lblb[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 4, 5};
    int stepSize[numRanges] = {150, 25, 200};
    
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
inline void AddInMT2lblbBounds_Control(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     150 GeV bin from 0 to 150 -- 1 bin
     10 GeV bins from 150 to 200 -- 5 bins
     100 GeV bin from 200 to 300 -- 1 bin
     200 GeV Signal bin from 300 to 500 -- 1 bin
     */
    const int numBinsTotal = 8;
    
    vector<Double_t> vecMT2lblb(numBinsTotal + 1);
    vecMT2lblb[0] = 0.0;
    const int numRanges = 4;
    int binRangeIndexEPs[numRanges] = {2, 7, 8, 9};
    int stepSize[numRanges] = {150, 10, 100, 200};
    
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

inline void AddInMT2llBounds_NonControl(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     1 GeV bin right at 0 -- 1 bin
     9 GeV bin up to 10 -- 1 bin
     10 GeV bins up to 80 -- 7 bins
     5 GeV bins 80 to 120 -- 8 bins
     80 GeV Signal bin after 120 -- 1 bin
     */
    const int numBinsTotal = 18;
    
    vector<Double_t> vecMT2ll(numBinsTotal + 1);
    vecMT2ll[0] = 0.0;
    const int numRanges = 5;
    int binRangeIndexEPs[numRanges] = {2, 3, 10, 18, 19};
    int stepSize[numRanges] = {1, 9, 10, 5, 80};
    
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
inline void AddInMT2llBounds2DShape(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     80 GeV bin from 0 to 80 -- 1 bin
     20 GeV bins 80 to 100 -- 1 bins
     50 GeV Signal bin after 100 -- 1 bin
     */
    const int numBinsTotal = 3;
    vector<Double_t> vecMT2ll(numBinsTotal + 1);
    vecMT2ll[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 3, 4};
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
     20 GeV bins 80 to 100 -- 1 bins
     50 GeV Signal bin after 100 -- 1 bin
     */
    const int numBinsTotal = 3;
    vector<Double_t> vecMT2ll(numBinsTotal + 1);
    vecMT2ll[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 3, 4};
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
inline void AddInMT2llBounds_Control(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     80 GeV bin from 0 to 80 -- 1 bin
     5 GeV bins 80 to 120 -- 8 bins
     80 GeV Signal bin after 120 -- 1 bin
     */
    const int numBinsTotal = 10;
    vector<Double_t> vecMT2ll(numBinsTotal + 1);
    vecMT2ll[0] = 0.0;
    const int numRanges = 3;
    int binRangeIndexEPs[numRanges] = {2, 10, 11};
    int stepSize[numRanges] = {80, 5, 80};
    
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


inline void AddInDilepPtBounds(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    /*
     10 GeV bins from 0 to 120 -- 12 bins
     15 GeV bins from 120 to 150 -- 2 bins
     100 GeV bin from 150 to 250 -- 1 bin
     150 GeV signal bin from 250 to 400 -- 1 bin
     */
    const int numBinsTotal = 16;
    vector<Double_t> vecDiLepPt(numBinsTotal + 1);
    vecDiLepPt[0] = 0.0;
    const int numRanges = 4;
    int binRangeIndexEPs[numRanges] = {13, 15, 16, 17};
    int stepSize[numRanges] = {10, 15, 100, 150};
    
    int stepSizeToUse = -1;
    
    for (int binIndex = 1; binIndex < numBinsTotal + 1; ++binIndex) {
        for (int iBRIndex = 0; iBRIndex < numRanges; ++iBRIndex) {
            if (binIndex < binRangeIndexEPs[iBRIndex]) {
                stepSizeToUse = stepSize[iBRIndex];
                break;
            }
        }
        vecDiLepPt[binIndex] = vecDiLepPt[binIndex - 1] + stepSizeToUse;
        //cout << "vecDiLepPt at binIndex " << binIndex << " is " << vecDiLepPt[binIndex] << endl;
    }
    inVecVecBinEdges->push_back(vecDiLepPt);
    vecNumBins->push_back(numBinsTotal);
}
