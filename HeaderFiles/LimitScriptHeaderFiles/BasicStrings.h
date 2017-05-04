#include "TString.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

using namespace std;

TString BaseWorkDirString() {
    TString strBWD = "/data/users/bcalvert/StopDiLepton2014/";
    return strBWD;
}

TString SavingString_WhichFullSel(int whichFullSel) {
    TString arrWhichFullSel[2] = {"BaseFullSel", ""};
    
    return arrWhichFullSel[whichFullSel];
}

TString SavingString_ANDOR(int useOR) {
    
    TString arrANDOR[2] = {"CutAnd", "CutOr"};
    
    return arrANDOR[useOR];
}

TString SavingString_ShapeOrCC(int isShape) {
    
    TString arrShapeOrCC[2] = {"CC", "Shape"};
    
    return arrShapeOrCC[isShape];
}

TString SavingString_WhichLepAnaType(int whichLepAnaType) {
    
    TString arrWhichLepAnaType[3] = {"DiLep", "SingLep", "LepCombo"};
    
    return arrWhichLepAnaType[whichLepAnaType];
}

TString SavingString_DoSigContam(bool doSC) {
    
    TString outStringSC = doSC ? "With" : "No";
    outStringSC += "SigContam";
    
    return outStringSC;
}

TString SavingString_ExpObs(bool isExp) {
    
    TString outExpObs = isExp ? "Exp" : "Obs";
    
    return outExpObs;
}

TString SavingString_FreqAsymp(bool isAsymp) {
    
    TString outFreqAsymp = isAsymp ? "Asymp" : "FullFreq";
    
    return outFreqAsymp;
}