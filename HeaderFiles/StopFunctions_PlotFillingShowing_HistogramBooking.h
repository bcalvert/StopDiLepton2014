// Set of functions used for booking of histograms

using namespace std;
inline void Book1DHists(HMap_1D &inHMap_1D, vector<HistogramT> * inHistVec_1D, SampleT * inSampT, vector<int> * inNBinsVec, vector<Double_t *> inBinEdgesVec) {
    TH1F * h_1DCurr;
    for (unsigned int iH1D = 0; iH1D < inHistVec_1D->size(); ++iH1D) {
        inHistVec_1D->at(iH1D).SetAxisString();
        h_1DCurr = inHistVec_1D->at(iH1D).BookOneDeeHist(inSampT, inNBinsVec, inBinEdgesVec);
        inHMap_1D[histKeyString(inHistVec_1D->at(iH1D), inSampT->histNameSuffix)] = h_1DCurr;
    }
}

inline void Book2DHists(HMap_2D &inHMap_2D, vector<HistogramT> * inHistVec_2D, SampleT * inSampT, vector<int> * inNBinsVec, vector<Double_t *> inBinEdgesVec) {
    TH2F * h_2DCurr;
    for (unsigned int iH2D = 0; iH2D < inHistVec_2D->size(); ++iH2D) {
        inHistVec_2D->at(iH2D).SetAxisString();
        h_2DCurr = inHistVec_2D->at(iH2D).BookTwoDeeHist(inSampT, inNBinsVec, inBinEdgesVec);
        inHMap_2D[histKeyString(inHistVec_2D->at(iH2D), inSampT->histNameSuffix)] = h_2DCurr;
    }
}

inline void Book3DHists(HMap_3D &inHMap_3D, vector<HistogramT> * inHistVec_3D, SampleT * inSampT, vector<int> * inNBinsVec, vector<Double_t *> inBinEdgesVec) {
    TH3F * h_3DCurr;
    for (unsigned int iH3D = 0; iH3D < inHistVec_3D->size(); ++iH3D) {
        inHistVec_3D->at(iH3D).SetAxisString();
        h_3DCurr = inHistVec_3D->at(iH3D).BookThreeDeeHist(inSampT, inNBinsVec, inBinEdgesVec);
        inHMap_3D[histKeyString(inHistVec_3D->at(iH3D), inSampT->histNameSuffix)] = h_3DCurr;
    }
}

inline void BookHists(vector<HistogramT> * inHistVec, HMap_1D &inHMap_1D, HMap_2D &inHMap_2D, HMap_3D &inHMap_3D, SampleT * inSampT, vector<int> * inNBinsVec, vector<Double_t *> inBinEdgesVec, int numDims) {
    switch (numDims) {
        case 1:
            Book1DHists(inHMap_1D, inHistVec, inSampT, inNBinsVec, inBinEdgesVec);
            break;
        case 2:
            Book2DHists(inHMap_2D, inHistVec, inSampT, inNBinsVec, inBinEdgesVec);
            break;
        case 3:
            Book3DHists(inHMap_3D, inHistVec, inSampT, inNBinsVec, inBinEdgesVec);
            break;
        default:
            cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << endl;
            break;
    }
}
