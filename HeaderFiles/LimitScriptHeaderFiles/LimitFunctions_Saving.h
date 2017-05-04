#include "TString.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

#include "BasicStrings.h"
#include "LimitScripts_SUSYStruct.h"

using namespace std;


TString CondorScript(bool doLocal) {
    TString arrCondorScript[2] = {"runCondorTest.sh", "runCondorLocal.sh"};
    TString strPath = BaseWorkDirString() + "GeneralScripts/";
    
    return strPath + arrCondorScript[doLocal];
}

TString LimitSettingArea() {
    TString strBWD = BaseWorkDirString();
    TString strPath = strBWD + "LimitSetting/";
    return strPath;
}

TString NameLimitRunUmbrellaScript() {
    TString pathLimit = LimitSettingArea() + TString("LimitRunning/");
    TString nameUmbrellaScript = pathLimit + "runExpLimit_InputFile.sh";
    return nameUmbrellaScript;
}

TString NameSignifRunUmbrellaScript() {
    TString pathLimit = LimitSettingArea() + TString("LimitRunning/");
    TString nameUmbrellaScript = pathLimit + "runExpSignif_InputFile.sh";
    return nameUmbrellaScript;
}

TString NameLimitRunScript() {
    TString pathLimit = LimitSettingArea() + TString("LimitRunning/");;
    TString nameLimitScript = pathLimit + "runLimit_IndividualCard.sh";
    return nameLimitScript;
}

TString FullCardName_DilepCC(TString baseCardPath, TString baseCardName, MT2LimitParams * inMT2LP) {
    TString outNameCard = baseCardPath + baseCardName + inMT2LP->strMT2Append + TString(".txt");
    return outNameCard;
}

TString AppendStringParameters(int whichLepAnaType, int whichFullSel, int isShape, bool doSC, SUSYT2LimitParams * inST2LPs, SUSYMassLimitParams * inSMLPs) {
    
    //Get the strings for the input parameters
    TString strAnaType   = SavingString_WhichLepAnaType(whichLepAnaType);
    TString strFullSel   = SavingString_WhichFullSel(whichFullSel);
    TString strShape     = SavingString_ShapeOrCC(isShape);
    TString strSigContam = SavingString_DoSigContam(doSC);
    TString strSUSYT2SMS = inST2LPs->SavingName(false);
    TString strSUSYMass  = inSMLPs->SavingName(false);
    
    TString strAppend = strFullSel + TString("_") + strSigContam + TString("_") + strSUSYT2SMS + + strSUSYMass + strShape;
    
    return strAppend;
}