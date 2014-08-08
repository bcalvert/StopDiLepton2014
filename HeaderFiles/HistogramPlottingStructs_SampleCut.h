#include "TString.h"
using namespace std;

// Structures used as part of setting up sub-sample cuts (events with 2 or more jets, events with MET Geq 40, etc.) for either whole swaths of histograms or as an individual cut for a given histogram

// Are broken down into cutting based on floats or integers

// histNameSuffix is the suffix added to the histogram, if any (used for swaths of histograms
// cutVarVal1 and cutVarVal2 are the two bounds of the cut value to cut on
// doCut is a boolean for whether or not to the cut
// cutDir is whether to consider the second part of the bound, cutVarVal2, and whether cutVarVal1 is a lower bound or not

typedef struct SampCut {
    
    bool doCut;
    TString histNameSuffix;
    float cutVarVal1, cutVarVal2;
    int   cutDir;
    void SetParams(SampCut * inSampCut) {
        doCut = inSampCut->doCut;
        histNameSuffix = inSampCut->histNameSuffix;
        cutVarVal1 = inSampCut->cutVarVal1;
        cutVarVal2 = inSampCut->cutVarVal2;
        cutDir = inSampCut->cutDir;
    }
    void SetParamsWindow(TString inHNS, float inCVV1, float inCVV2, int inCD, bool inDoCut = true) {
        doCut = inDoCut;
        histNameSuffix = inHNS;
        cutVarVal1 = inCVV1;
        cutVarVal2 = inCVV2;
        cutDir = inCD;
    }
    void SetParamsBound(TString inHNS, float inCVV1, int inCD, bool inDoCut = true) {
        doCut = inDoCut;
        histNameSuffix = inHNS;
        cutVarVal1 = inCVV1;
        cutDir = inCD;
    }
    bool PassCut(float checkVal, bool doVerbosity = false) {
        bool passesCut = true;
        if (doVerbosity) {
            cout << "checkVal " << checkVal << endl;
            cout << "cutVarVal1 " << cutVarVal1 << endl;
            cout << "cutVarVal2 " << cutVarVal2 << endl;
            cout << "cutDir " << cutDir << endl;
            cout << "doCut " << doCut << endl;
        }
        if (doCut) {
            switch (cutDir) {
                case -2:
                    passesCut = (checkVal <= cutVarVal1 && checkVal >= cutVarVal2);
                    break;
                case -1:
                    passesCut = checkVal <= cutVarVal1;
                    break;
                case 1:
                    passesCut = checkVal >= cutVarVal1;
                    break;
                case 2:
                    passesCut = (checkVal >= cutVarVal1 && checkVal <= cutVarVal2);
                    break;
                default:
                    cout << "issues with checking based on cutDir " << cutDir << endl;
                    break;
            }
        }
        if (doVerbosity) {
            cout << "passesCut? " << passesCut << endl;
        }
        return passesCut;
    }
} SampCut;

typedef struct SampCutInt {
    bool doCut;
    TString histNameSuffix;    
    int cutVarVal1, cutVarVal2;
    int   cutDir;
    void SetParams(SampCutInt * inSampCut) {        
        doCut = inSampCut->doCut;
        histNameSuffix = inSampCut->histNameSuffix;
        cutVarVal1 = inSampCut->cutVarVal1;
        cutVarVal2 = inSampCut->cutVarVal2;
        cutDir = inSampCut->cutDir;
    }
    void SetParamsWindow(TString inHNS, int inCVV1, int inCVV2, int inCD, bool inDoCut = true) {
        doCut = inDoCut;
        histNameSuffix = inHNS;
        cutVarVal1 = inCVV1;
        cutVarVal2 = inCVV2;
        cutDir = inCD;
    }
    void SetParamsBound(TString inHNS, int inCVV1, int inCD, bool inDoCut = true) {
        doCut = inDoCut;
        histNameSuffix = inHNS;
        cutVarVal1 = inCVV1;
        cutDir = inCD;
    }
    bool PassCut(int checkVal, bool doVerbosity = false) {
        bool passesCut = true;
        if (doVerbosity) {
            cout << "checkVal " << checkVal << endl;
            cout << "cutVarVal1 " << cutVarVal1 << endl;
            cout << "cutVarVal2 " << cutVarVal2 << endl;
            cout << "cutDir " << cutDir << endl;
            cout << "doCut " << doCut << endl;
        }
        if (doCut) {
            switch (cutDir) {
                case -2:
                    passesCut = (checkVal <= cutVarVal1 && checkVal >= cutVarVal2);
                    break;
                case -1:
                    passesCut = checkVal <= cutVarVal1;
                    break;
                case 0:
                    passesCut = checkVal == cutVarVal1;
                    break;
                case 1:
                    passesCut = checkVal >= cutVarVal1;
                    break;
                case 2:
                    passesCut = (checkVal >= cutVarVal1 && checkVal <= cutVarVal2);
                    break;
                default:
                    cout << "issues with checking based on cutDir " << cutDir << endl;
                    break;
            }
        }
        if (doVerbosity) {
            cout << "passesCut? " << passesCut << endl;
        }
        return passesCut;
    }
} SampCutInt;