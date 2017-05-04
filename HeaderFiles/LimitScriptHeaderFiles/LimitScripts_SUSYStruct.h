#ifndef LimitScripts_SUSYStructs
#define LimitScripts_SUSYStructs

#include "TString.h"

struct MT2LimitParams {
    int cutMT2ll, cutMT2lblb;
    
    int whichMT2Bounds, minMT2ll, maxMT2ll, minMT2lblb, maxMT2lblb;
    int smallStepSizeMT2ll, bigStepSizeMT2ll;
    int smallStepSizeMT2lblb, bigStepSizeMT2lblb;
    
    int contMT2ll, contMT2lblb;
    
    TString strMT2LimitSave, strMT2Append;
    
    void SetMT2AppendString(bool isControl) {
        strMT2Append = "MT2ll";
        strMT2Append += isControl ? contMT2ll : cutMT2ll;
        strMT2Append += "_MT2lblb";
        strMT2Append += isControl ? contMT2lblb : cutMT2lblb;
    }
    
    void DefaultVals() {
        contMT2ll = 80;
        contMT2lblb = 0;
        
        smallStepSizeMT2ll = 10;
        bigStepSizeMT2ll = 50;
        
        smallStepSizeMT2lblb = 10;
        bigStepSizeMT2lblb = 50;
        minMT2ll = 80;
        maxMT2ll = 120;
        minMT2lblb = 0;
        maxMT2lblb = 0;
        
        cutMT2ll = 120;
        cutMT2lblb = 0;
        strMT2LimitSave = "/MT2SpaceScan/";
    }
};

struct SUSYMassLimitParams {
    int minStopMass, maxStopMass;
    int minLSPMass, maxLSPMass;
    int massStop, massLSP;
    
    int binSize;
    
    TString strStop, strLSP;
    
    int GetDM() {
      vector<int> vecDMValues{100, 200, 300, 400, 500};
        
        int closeDM = -1;
        float currMinDiff = 9999;
        float currDM = massStop - massLSP;
        float currDiff_DM;
        for (int iDM = 0; iDM < (int) vecDMValues.size(); ++iDM) {
            currDiff_DM = TMath::Abs(currDM - vecDMValues[iDM]);
            if (currDiff_DM < currMinDiff) {
                currMinDiff = currDiff_DM;
                closeDM = iDM;
            }
        }
        return closeDM;
    }
    
    void DefaultVals() {
        minStopMass = 100;
        maxStopMass = 800;
        
        minLSPMass = 0;
        maxLSPMass = 400;
        
        binSize = 25;
        
        massStop = 500;
        massLSP = 100;
    }
    
    void PrintVals() {
        cout << "minStopMass " << minStopMass << endl;
        cout << "maxStopMass " << maxStopMass << endl;
        cout << "minLSPMass " << minLSPMass << endl;
        cout << "maxLSPMass " << maxLSPMass << endl;
        cout << "binSize " << binSize << endl;
    }
    
    void PrintCurrMass() {
        cout << "massStop:massLSP " << massStop << ":" << massLSP << endl;
    }
    
    void SetMassStrings() {
        strStop = "Stop";
        strStop += massStop;
        strLSP = "LSP";
        strLSP += massLSP;
    }
    void SetParams(int inMassStop, int inMassLSP) {
        massStop = inMassStop;
        massLSP = inMassLSP;
        
        SetMassStrings();
    }
    
    TString InputSingLepCardName() {
        TString baseNameSingLep = "datacard-S";
        baseNameSingLep += massStop;
        baseNameSingLep += "-N";
        baseNameSingLep += massLSP;
        baseNameSingLep += ".txt";
        return baseNameSingLep;
    }
    
    TString SavingName(bool isDir) {
        TString outStringSaveName = "";
        
        TString addString = isDir ? "/" : "_";
        
        outStringSaveName = strStop + addString + strLSP + addString;
        return outStringSaveName;
    }
    
    void SetParamsFromCommandLine(int argc, char* argv[]) {
        for (int iSUSYMassParam = 0; iSUSYMassParam < argc; ++iSUSYMassParam) {
            if (strncmp (argv[iSUSYMassParam],"minStop", 7) == 0) {
                minStopMass = strtol(argv[iSUSYMassParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSUSYMassParam],"maxStop", 7) == 0) {
                maxStopMass = strtol(argv[iSUSYMassParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSUSYMassParam],"minLSP", 6) == 0) {
                minLSPMass = strtol(argv[iSUSYMassParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSUSYMassParam],"maxLSP", 6) == 0) {
                maxLSPMass = strtol(argv[iSUSYMassParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSUSYMassParam],"binSize", 7) == 0) {
                binSize = strtol(argv[iSUSYMassParam + 1], NULL, 10);
            }
            else if (strncmp (argv[iSUSYMassParam],"massSUSY", 8) == 0) {
                massStop = strtol(argv[iSUSYMassParam + 1], NULL, 10);
                massLSP = strtol(argv[iSUSYMassParam + 2], NULL, 10);
            }
        }
    }
};
#endif
