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
            if (vecISPI->at(iSamp).sampleType == 0) {
                baseName += "Data/";
            }
            else if (vecISPI->at(iSamp).sampleType == 100) {
                baseName += "FakeLep/";
            }
            else {
                baseName += "MC/";
            }
        }
//        vecISPI->at(iSamp).SetInputFile(baseName + vecBaseFileNames->at(iSamp) + inSLS->SampleStringEnd(&vecISPI->at(iSamp)));
        vecISPI->at(iSamp).SetInputFile(baseName + inSLS->SampleStringEnd(&vecISPI->at(iSamp)));
    }
}


void SetWeights(HistogramDisplayStructs * inHDS, WeightCalculators * inWC, SampLoadSettings * inSLS, bool useDDEstTTBar, bool useDDEstDY, bool doVerbosity = false, vector<TString> * vecSystString = 0, vector<float> * inVecSignalSkimEfficiencyCalc = 0) {
    int numSysts = vecSystString != 0 ? (int) vecSystString->size() : 0;
    for (unsigned int iISPI = 0; iISPI < inHDS->vecISPI_asLoaded.size(); ++iISPI) {
        if (inVecSignalSkimEfficiencyCalc != NULL) {
            for (unsigned int iSig = 0; iSig < inVecSignalSkimEfficiencyCalc->size(); ++iSig) {
                inWC->SetISPIWeight(&inHDS->vecISPI_asLoaded[iISPI], inSLS, useDDEstTTBar, useDDEstDY, doVerbosity, numSysts, vecSystString, inVecSignalSkimEfficiencyCalc, iSig);
            }
        }
        else {
            inWC->SetISPIWeight(&inHDS->vecISPI_asLoaded[iISPI], inSLS, useDDEstTTBar, useDDEstDY, doVerbosity, numSysts, vecSystString);
        }
    }
}

void SetSystBasics(HistogramDisplayStructs * inHDS, vector<TString> * vecSystNames, bool doSmear, bool isSignalHDS, bool doVerbosity = false) {
    unsigned int numSysts = vecSystNames->size();
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        for (unsigned int iISPI = 0; iISPI < inHDS->vecISPI_asLoaded.size(); ++iISPI) {
            if (doVerbosity) {
                cout << "For Syst " << vecSystNames->at(iSyst) << endl;
                cout << "inHDS->vecISPI_asLoaded[iISPI].nameISPI " << inHDS->vecISPI_asLoaded[iISPI].nameISPI << endl;
            }
            if (vecSystNames->at(iSyst).Contains("Fake")) {
                if (doVerbosity) {
                    cout << "syst is a fake syst " << endl;
                }
                if (!inHDS->vecISPI_asLoaded[iISPI].nameISPI.Contains("FakeLep")) {
                    //not on a sample that is a fake lepton sample so nuke that ish
                    inHDS->SetBoolSysts(iSyst, iISPI, false);
                }
                else {
                    inHDS->SetBoolSysts(iSyst, iISPI, true);
                }
            }
            else {
                //Not a fake lepton systematic
                if (inHDS->vecISPI_asLoaded[iISPI].nameISPI.Contains("FakeLep")) {
                    //on a sample that is a fake lepton sample so nuke that ish
                    inHDS->SetBoolSysts(iSyst, iISPI, false);
                }
                else {
                    if (vecSystNames->at(iSyst).Contains("genStopXSec")) {
                        inHDS->SetBoolSysts(iSyst, iISPI, isSignalHDS);
                        //                inHDS->SetBoolSysts(iSyst, iISPI, false);
                    }
                    else if (vecSystNames->at(iSyst).Contains("JetSmear") || vecSystNames->at(iSyst).Contains("UncES")){
                        inHDS->SetBoolSysts(iSyst, iISPI, doSmear);
                        //                inHDS->SetBoolSysts(iSyst, iISPI, false);
                    }
                    else if (vecSystNames->at(iSyst).Contains("Norm")) {
                        inHDS->SetBoolSysts(iSyst, iISPI, false);
                    }
                    else {
                        inHDS->SetBoolSysts(iSyst, iISPI, true);
                        //                inHDS->SetBoolSysts(iSyst, iISPI, false);
                    }
                }
            }
        }
    }
}

void SetSystBasicsShape(HistogramDisplayStructs * inHDS, vector<TString> * vecSystNames) {
    unsigned int numSysts = vecSystNames->size();
    for (unsigned int iSyst = 0; iSyst < numSysts; ++iSyst) {
        for (unsigned int iISPI = 0; iISPI < inHDS->vecISPI_asLoaded.size(); ++iISPI) {
            if (inHDS->vecISPI_asLoaded[iISPI].nameISPI.Contains("T2tt") || inHDS->vecISPI_asLoaded[iISPI].nameISPI.Contains("T2bw")) {
                if (vecSystNames->at(iSyst).Contains("Norm") || vecSystNames->at(iSyst).Contains("Fake")) {
                    //signal files don't have Norm nor Fake systs saved
                    inHDS->SetBoolSysts(iSyst, iISPI, false);
                }
                else {
                    inHDS->SetBoolSysts(iSyst, iISPI, true);
                }
            }
            else {
                inHDS->SetBoolSysts(iSyst, iISPI, true);
            }
        }
    }
}


