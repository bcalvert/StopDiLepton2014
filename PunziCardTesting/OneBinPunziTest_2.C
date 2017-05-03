#include "TRint.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::next_permutation, std::sort
#include <map>
#include "TMath.h"
#include "TString.h"
using namespace std;
typedef map<TString, float> mapStrToF;

#include "../HeaderFiles/BasicFunctions_SystemCommands.h"


float GetPunziParam(float effSig, float yieldBkg, float nSigmaA, float nSigmaB, bool doVerb) {
    //equation 7 of http://www-cdf.fnal.gov/physics/statistics/notes/punzi-optimal-search.ps
    
    if (doVerb) {
        cout << "nSigmaA " << nSigmaA << endl;
        cout << "nSigmaB " << nSigmaB << endl;
        cout << "yieldBkg " << yieldBkg << endl;
        cout << "effSig " << effSig << endl;
    }
    
    float sqrtBkg = TMath::Sqrt(yieldBkg);
    
    float sigmaMin_Part1 = pow(nSigmaA, 2) / 8 + (9 * pow(nSigmaB, 2))/13 + nSigmaA * sqrtBkg;
    float sigmaMin_Part2 = (nSigmaB / 2) * TMath::Sqrt(pow(nSigmaB, 2) + 4 * nSigmaA * sqrtBkg + 4 * yieldBkg);
    
    //    float paramPunzi = effSig / (TMath::Sqrt(yieldBkg) + (nSigma/2));
    float paramPunzi = effSig / (sigmaMin_Part1 + sigmaMin_Part2);
    if (doVerb) {
        cout << "Punzi Parameter " << paramPunzi << endl;
    }
    return paramPunzi;
}

void SetMap(mapStrToF &inMap, int whichIndex) {
    TString baseStringSigEff = "SigEff";
    TString baseStringBackYield = "BackYield";
    
    vector<TString> vecString{"Low", "Med", "High", "SuperHigh"};
    vector<float> vecSigEff_Low{0.01, 0.008, 0.02};
    vector<float> vecSigEff_Med{0.05, 0.03, 0.04};
    vector<float> vecSigEff_High{0.2, 0.08, 0.06};
    vector<float> vecSigEff_SuperHigh{0.25, 0.12, 0.08};
    vector<float> vecBackYield_Low{4, 2, 0.2};
    vector<float> vecBackYield_Med{9, 5, 1.0};
    vector<float> vecBackYield_High{20, 8, 1.0};
    vector<float> vecBackYield_SuperHigh{36, 14, 1.8};
    
    int numVals = 4;
    
    vector< vector<float> > vecVecSigEff(numVals);
    vecVecSigEff[0] = vecSigEff_Low;
    vecVecSigEff[1] = vecSigEff_Med;
    vecVecSigEff[2] = vecSigEff_High;
    vecVecSigEff[3] = vecSigEff_SuperHigh;
    
    vector< vector<float> > vecVecBackYield(numVals);
    vecVecBackYield[0] = vecBackYield_Low;
    vecVecBackYield[1] = vecBackYield_Med;
    vecVecBackYield[2] = vecBackYield_High;
    vecVecBackYield[3] = vecBackYield_SuperHigh;
    
    TString currStringSigEff, currStringBackYield;
    
    for (int iVal = 0; iVal < numVals; ++iVal) {
        currStringSigEff = vecString[iVal] + baseStringSigEff;
        currStringSigEff += whichIndex;
        
        currStringBackYield = vecString[iVal] + baseStringBackYield;
        currStringBackYield += whichIndex;
        inMap[currStringSigEff] = vecVecSigEff[iVal][whichIndex];
        inMap[currStringBackYield] = vecVecBackYield[iVal][whichIndex];
    }
}

struct PunziCheck {
    mapStrToF map_InputPunzi;
    
    float sigEff;
    float backYield;
    
    vector<TString> vecString;
    vector<int> vecWhichBin;
    int punziBin;
    
    float nSigmaA, nSigmaB;
    float punziParam;
    void DefaultVarVals() {
        nSigmaA = 5;
        nSigmaB = 2;
        vecString.push_back("Low");
        vecString.push_back("Med");
        vecString.push_back("High");
        vecString.push_back("SuperHigh");
        
        vecWhichBin.push_back(0);
        vecWhichBin.push_back(1);
        vecWhichBin.push_back(2);
        vecWhichBin.push_back(3);
    }
    
    void PrintVec() {
        for (unsigned int iPermute = 0; iPermute < vecWhichBin.size(); ++iPermute) {
            cout << "vecWhichBin[" << iPermute << "]" << vecWhichBin[iPermute] << endl;
        }
    }
    
    void SetPunziMap(int whichIndex) {
        punziBin = whichIndex;
        SetMap(map_InputPunzi, punziBin);
    }
    void SetPunziParam(bool doVerb = false) {
        punziParam = GetPunziParam(sigEff, backYield, nSigmaA, nSigmaB, doVerb);
    }
    void PrintVals(int whichIndex) {
        cout << "For the " << vecString[vecWhichBin[whichIndex]]  << " Bin " << punziBin + 1 << endl;
        cout << "sigEff " << sigEff << endl;
        cout << "backYield " << backYield << endl;
        cout << "punziParam " << punziParam << endl;
    }
    void SetVals(int whichIndex, bool doVerb = false) {
        TString baseStringSigEff = "SigEff";
        TString baseStringBackYield = "BackYield";
        TString stringSig = vecString[vecWhichBin[whichIndex]] + baseStringSigEff;
        stringSig += punziBin;
        
        TString stringBack = vecString[vecWhichBin[whichIndex]] + baseStringBackYield;
        stringBack += punziBin;
        
        mapStrToF::iterator iter_floatVals;
        iter_floatVals = map_InputPunzi.find(stringSig);
        if (iter_floatVals != map_InputPunzi.end()) {
            sigEff = iter_floatVals->second;
        }
        
        iter_floatVals = map_InputPunzi.find(stringBack);
        if (iter_floatVals != map_InputPunzi.end()) {
            backYield = iter_floatVals->second;
        }
        
        SetPunziParam(doVerb);
    }
    
};

TString GetCardPatsyReplace(vector<PunziCheck> * inVecPC, int iPermute, int numBins = 1) {
    TString currPatsyReplace = "";
    for (int iBin = 0; iBin < numBins; ++iBin) {
        currPatsyReplace += inVecPC->at(iBin).vecString[iPermute];
        currPatsyReplace += "Bin";
        currPatsyReplace += iBin + 1;
    }
    return currPatsyReplace;
}

void GenerateCard(TString patsyCardName, TString inputStringCard, vector<PunziCheck> * inVecPC, float sigXsec, int numBins = 1, int whichVers = 0, bool doVerb = false) {
    
    char buf[99];
    
    TString addToName = "AddToBin";
    if (numBins > 1) {
        addToName = whichVers > 0 ? "2" : "1";
        inputStringCard.Replace(inputStringCard.Index(".txt"), 4, addToName);
    }
    System_CopyFile(patsyCardName, inputStringCard, doVerb);
    float currSigYield;
    TString currReplaceString, currSearchString;
    for (int iBin = 0; iBin < numBins; ++iBin) {
        currSigYield = inVecPC->at(iBin).sigEff * sigXsec;
        currReplaceString = "";
        sprintf(buf, "%1.2f", currSigYield);
        currReplaceString += TString(buf);
        currSearchString = "SIGPROXY";
        currSearchString += iBin + 1;
        System_SedInFile(inputStringCard, currSearchString, currReplaceString, doVerb);
        currReplaceString = "";
        currReplaceString += inVecPC->at(iBin).backYield;
        currSearchString = "BKGPROXY";
        currSearchString += iBin + 1;
        System_SedInFile(inputStringCard, currSearchString, currReplaceString, doVerb);
    }
}

void RunCard(TString outLimitName, TString inputCardName, bool doVerb = false) {
    vector<TString> vecComArgs(0);
    bool doAsymptotic = false;
    int isExpLim = 1;
    
    TString strAsymptotic = "";
    strAsymptotic += doAsymptotic;
    
    TString strExpLim = "";
    strExpLim += isExpLim;

    TString nameLimitCommand = "runCondorR510.sh ./runExpLimit_InputFile.sh";
    
    vecComArgs.push_back(inputCardName);
    vecComArgs.push_back(outLimitName);
    vecComArgs.push_back(strAsymptotic);
    vecComArgs.push_back(strExpLim);
    
    System_GeneralCommand(nameLimitCommand, &vecComArgs, doVerb);
}



int main( int argc, char* argv[]) {
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    int numBins = 1;
    bool doVerb = 0;
    
    bool runCards = false;
    for (int k = 0; k < argc; ++k) {
        if (strncmp (argv[k],"doVerb", 6) == 0) {
            doVerb = 1;
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
    
    
    int numChecks = 100;
    
    float minSigEff = 0.005;
    float maxSigEff = 0.505;
    float stepSizeSigEff = (maxSigEff - minSigEff)/numChecks;
    
    float minBackground = 1;
    float maxBackground = 201;
    float stepSizeBackground = (maxBackground - minBackground)/numChecks;
 
    TString currReplaceString;
    
    
    vector<PunziCheck> vecPCs;
    for (int iCheck = 0; iCheck < numChecks; ++iCheck) {
        currReplaceString = "CheckSqrt";
        currReplaceString += iCheck;
        pc_OneBin.sigEff = minSigEff + TMath::Power(iCheck * stepSizeSigEff, 0.5);
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