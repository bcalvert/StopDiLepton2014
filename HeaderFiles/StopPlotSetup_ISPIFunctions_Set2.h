// Functions that operate on either the HistogramDisplayStructs Struct or the IndSamplePlotInfo Struct
// Used as part of prepping the plots to show

using namespace std;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;

void SetFileNames(vector<IndSamplePlotInfo> * vecISPI, vector<TString> * vecBaseFileNames, SampLoadSettings * inSLS, bool isSignal = false, bool doVerbosity = false) {
    TString path = inSLS->customPath.Length() != 0 ? inSLS->customPath : "../RootFiles/";  
    if (isSignal) path = inSLS->customPathSignalFile;
    TString baseName; // = path;
    for (unsigned int iSamp = 0; iSamp < vecISPI->size(); ++iSamp) {
        vecISPI->at(iSamp).SetName(vecBaseFileNames->at(iSamp));
        vecISPI->at(iSamp).SetSampleType(doVerbosity);
        baseName = path;
        if (!isSignal) {
            baseName += vecISPI->at(iSamp).sampleType == 0 ? "Data/" : "MC/";
        }
//        vecISPI->at(iSamp).SetInputFile(baseName + vecBaseFileNames->at(iSamp) + inSLS->SampleStringEnd(&vecISPI->at(iSamp)));
        vecISPI->at(iSamp).SetInputFile(baseName + inSLS->SampleStringEnd(&vecISPI->at(iSamp)));
    }
}


void SetWeights(HistogramDisplayStructs * inHDS, WeightCalculators * inWC, bool useDDEstTTBar, bool useDDEstDY, bool doVerbosity = false, vector<TString> * vecSystString = 0, vector<float> * inVecSignalSkimEfficiencyCalc = 0) {
    int numSysts = vecSystString != 0 ? (int) vecSystString->size() : 0;
    for (unsigned int iISPI = 0; iISPI < inHDS->vecISPI_asLoaded.size(); ++iISPI) {
        if (inVecSignalSkimEfficiencyCalc != NULL) {
            for (unsigned int iSig = 0; iSig < inVecSignalSkimEfficiencyCalc->size(); ++iSig) {
                inWC->SetISPIWeight(&inHDS->vecISPI_asLoaded[iISPI], useDDEstTTBar, useDDEstDY, doVerbosity, numSysts, vecSystString, inVecSignalSkimEfficiencyCalc, iSig);   
            }
        }
        else {
            inWC->SetISPIWeight(&inHDS->vecISPI_asLoaded[iISPI], useDDEstTTBar, useDDEstDY, doVerbosity, numSysts, vecSystString);
        }
    }
}

void SetSystBasics(HistogramDisplayStructs * inHDS, vector<TString> * vecSystNames, bool doSmear, bool isSignalHDS) {
    unsigned int numSysts = vecSystNames->size();
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        for (unsigned int iISPI = 0; iISPI < inHDS->vecISPI_asLoaded.size(); ++iISPI) {            
            if (vecSystNames->at(iSyst).Contains("genStopXSec")) {
                inHDS->SetBoolSysts(iSyst, iISPI, isSignalHDS);
//                inHDS->SetBoolSysts(iSyst, iISPI, false);
            }
            else if (vecSystNames->at(iSyst).Contains("JetSmear") || vecSystNames->at(iSyst).Contains("UncES")){
                inHDS->SetBoolSysts(iSyst, iISPI, doSmear);
//                inHDS->SetBoolSysts(iSyst, iISPI, false);
            }
            else {
                inHDS->SetBoolSysts(iSyst, iISPI, true);
//                inHDS->SetBoolSysts(iSyst, iISPI, false);
            }
        }
    }
}