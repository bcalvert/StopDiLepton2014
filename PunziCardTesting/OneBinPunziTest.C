#include "TRint.h"
#include <algorithm>    // std::next_permutation, std::sort

#include "TMath.h"
#include "TString.h"
#include "TH1F.h"
#include "TGraphErrors.h"
#include <iostream>     // std::cout
#include <fstream>     // std::cout
#include <sstream>     // std::cout
#include <map>
#include "TGraphErrors.h"
using namespace std;

#include "../HeaderFiles/BasicFunctions_SystemCommands.h"
#include "./PunziTestingFunctions.h"

typedef map<TString, float> mapStrToF;

using namespace std;

int main( int argc, char* argv[]) {
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    int numBins = 1;
    bool doVerb = 0;
    
    bool runCards = false;
    int whichVersSigEff = 0;
    int whichVersPunzi = 0;
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
        }
        else if (strncmp (argv[k],"wSig", 4) == 0) {
            whichVersSigEff = strtol(argv[k + 1], NULL, 10 );
        }
        else if (strncmp (argv[k],"wPunzi", 6) == 0) {
            whichVersPunzi = strtol(argv[k + 1], NULL, 10 );
        }
        else if (strncmp (argv[k],"doRun", 5) == 0) {
            runCards = true;
        }
    }
    
    
    TString patsyCardName = "Cards/punziTestCard_PATSY_OneBin.txt";
    TString patsyOutName = "Results/limitOut_PATSY_OneBin.txt";
    
    if (numBins == 2) {
        patsyOutName = "Cards/limitOut_PATSY_TwoBin.txt";
    }
    TString currOutputCardName, currOutputLimitName;

    PunziCheck pc_OneBin;
    pc_OneBin.DefaultVarVals();
    
    
    float sigXsec = 100;
    
    
    int numChecks = 250;
    
    float minSigEff = 0.005;
    float maxSigEff = 0.505;
    float stepSizeSigEff = (maxSigEff - minSigEff)/numChecks;
    
    float minBackground = 1;
    float maxBackground = 501;
    float stepSizeBackground = (maxBackground - minBackground)/numChecks;
 
    TString currReplaceString;
    
    vector<TString> vecCheckAddName{"", "TwoThirds", "Sqrt", "CubeRoot", "TwoFifths"};
    vector<float> vecPower{1, 2./3, 1./2, 1./3, 2./5};
    pc_OneBin.nSigmaA = 5;
    pc_OneBin.nSigmaB = 2;
    
    if (whichVersPunzi) {
        pc_OneBin.nSigmaA = 2;
        pc_OneBin.nSigmaB = 2;
    }
    
    vector<PunziCheck> vecPCs;
    cout << "vecPower[whichVersSigEff] " << vecPower[whichVersSigEff] << endl;
    for (int iCheck = 0; iCheck <= numChecks; ++iCheck) {
        currReplaceString = GetStringPunziOneBin(whichVersSigEff, whichVersPunzi);
        currReplaceString += "Check";
        currReplaceString += iCheck;
//        pc_OneBin.sigEff = minSigEff + TMath::Power(iCheck * stepSizeSigEff, 0.5);
//        pc_OneBin.sigEff = minSigEff + TMath::Power(iCheck * stepSizeSigEff, vecPower[whichVersSigEff]);
        pc_OneBin.sigEff = minSigEff + (TMath::Power(iCheck * stepSizeBackground, vecPower[whichVersSigEff])/stepSizeBackground) * stepSizeSigEff;
        pc_OneBin.backYield = minBackground + iCheck * stepSizeBackground;
        pc_OneBin.SetPunziParam(doVerb);
        
        currOutputCardName = patsyCardName;
        currOutputCardName.Replace(currOutputCardName.Index("PATSY"), 5, currReplaceString);
        
        currOutputLimitName = patsyOutName;
        currOutputLimitName.Replace(currOutputLimitName.Index("PATSY"), 5, currReplaceString);
        
        vecPCs.resize(0);
        vecPCs.push_back(pc_OneBin);
        
        GenerateCard(patsyCardName, currOutputCardName, &vecPCs, sigXsec, numBins, 0, doVerb);
        if (runCards) {
            RunCard(currOutputLimitName, currOutputCardName, doVerb);
        }
        else {
            cout << vecPCs[0].backYield << ":" << vecPCs[0].punziParam << endl;
            //"Printing for iCheck " << iCheck << " (SigEff:BackYield:PunziParam) = (" << vecPCs[0].sigEff << ":" << vecPCs[0].backYield << ":" << vecPCs[0].punziParam << ")" << endl;
        }
    }
    theApp.Run(retVal);
}