inline void AddInQtBounds(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    //Original Photon MET bins
    //    const int      qTbinsN   = 15;
    //    Double_t qTbins[qTbinsN + 1] = {40.,60.,90.,100.,120.,150.,165.,200.,250.,300.,400., 500., 650., 800., 1000., 10000.};
  //const int      qTbinsN   = 10;
    //Double_t qTbins[qTbinsN + 1] = {40.,60.,90.,100.,120.,150.,165.,200.,250.,300.,400.};
    const int qTbinsN = 18;
    double qTbins[qTbinsN + 1] = {0,5,10,15,20,25,30,40,60,85,100,120,145,165,200,250,300,400,600};
    
    vector<Double_t> vecQt;
    vecQt.assign(&qTbins[0], &qTbins[0] + qTbinsN + 1);
    
    inVecVecBinEdges->push_back(vecQt);
    vecNumBins->push_back(qTbinsN);
}

inline void AddInUperpUparBounds(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    int i1start = 0;
    int i1end = i1start + 5;
    int i2start = 0;
    int i2end = i2start + 15;
    int i3start = 0;
    int i3end = i3start + 75;
    int i4start = 0;
    int i4end = i4start + 15;
    int i5start = 0;
    int i5end = i5start + 6;
    
    int uprp_uparNBins = 115;
    Double_t uprp_uparBinVals[uprp_uparNBins+1];
    for (int i1 = i1start; i1 < i1end; ++i1) {
        uprp_uparBinVals[i1] = -200 + 10 * i1;
        //        cout << "i1 " << i1 << endl;
    }
    for (int i2 = i2start; i2 < i2end; ++i2) {
        uprp_uparBinVals[i1end + i2] = -200 + (10 * i1end) + (5 * i2);
        //        cout << "i2 " << i2 << endl;
    }
    for (int i3 = i3start; i3 < i3end; ++i3) {
        uprp_uparBinVals[i1end + i2end + i3] = -200 + (10 * i1end) + (5 * i2end) + 2 * i3;
        //        cout << "i3 " << i3 << endl;
    }
    for (int i4 = i4start; i4 < i4end; ++i4) {
        uprp_uparBinVals[i1end + i2end + i3end + i4] = -200 + (10 * i1end) + (5 * i2end) + (2 * i3end) + 5 * i4;
        //        cout << "i4 " << i4 << endl;
    }
    for (int i5 = i5start; i5 < i5end; ++i5) {
        uprp_uparBinVals[i1end + i2end + i3end + i4end + i5] = -200 + (10 * i1end) + (5 * i2end) + (2 * i3end) + (5 * i4end) + 10 * i5;
        //        cout << "i5 " << i5 << endl;
    }
    
    
    vector<Double_t> vecUPerpUPar;
    vecUPerpUPar.assign(&uprp_uparBinVals[0], &uprp_uparBinVals[0] + uprp_uparNBins + 1);
    
    inVecVecBinEdges->push_back(vecUPerpUPar);
    vecNumBins->push_back(uprp_uparNBins);
}

inline void AddInUparDivQtBounds(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    int uparDivqTNBins = 166;
    Double_t uparDivqTBinVals[uparDivqTNBins+1];
    uparDivqTBinVals[0] = -7.;
    uparDivqTBinVals[1] = -4.;
    uparDivqTBinVals[2] = -2.;
    uparDivqTBinVals[3] = -1.;
    for (unsigned int i = 0; i < 5; ++i) {
        uparDivqTBinVals[i + 4] = -0.9 + i * 0.1;
        //        cout << "test i = " << i + 4 << " we get " << -0.9 + i * 0.1 << endl;
        uparDivqTBinVals[uparDivqTNBins - i - 4] = 2.9 - i * 0.1;
        //        cout << "test 2 i = " << uparDivqTNBins - i - 4 << " we get " << 2.9 - i * 0.1 << endl;
    }
    for (unsigned int i = 0; i < 75; ++i) {
        //        cout << " for i = " << i + 9 << " we get " << -0.48 + i * 0.02 << endl;
        uparDivqTBinVals[i + 9] = -0.48 + i * 0.02;
        //        cout << " for i2 = " << uparDivqTNBins - i - 9 << " we get " << 2.48 - i * 0.02 << endl;
        uparDivqTBinVals[uparDivqTNBins - i - 9] = 2.48 - i * 0.02;
    }
    uparDivqTBinVals[uparDivqTNBins - 3] = 3.;
    uparDivqTBinVals[uparDivqTNBins - 2] = 4.;
    uparDivqTBinVals[uparDivqTNBins - 1] = 6.;
    uparDivqTBinVals[uparDivqTNBins] = 9.;
    
    vector<Double_t> vecUParDivQt;
    vecUParDivQt.assign(&uparDivqTBinVals[0], &uparDivqTBinVals[0] + uparDivqTNBins + 1);
    
    inVecVecBinEdges->push_back(vecUParDivQt);
    vecNumBins->push_back(uparDivqTNBins);
}

inline void AddInNVtxBounds(vector< vector<Double_t> > * inVecVecBinEdges, vector<int> * vecNumBins) {
    int nVtxBins = 35;
    Double_t nVtxBinVals[nVtxBins+1];
    for (int k = 0; k < nVtxBins+1; ++k) {
        nVtxBinVals[k] = (Double_t) k+0.5;
    }
    
    vector<Double_t> vecNVtx;
    vecNVtx.assign(&nVtxBinVals[0], &nVtxBinVals[0] + nVtxBins + 1);
    
    inVecVecBinEdges->push_back(vecNVtx);
    vecNumBins->push_back(nVtxBins);
}





