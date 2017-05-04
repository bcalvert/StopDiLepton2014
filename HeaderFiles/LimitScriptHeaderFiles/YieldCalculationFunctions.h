TString PathYield_FullSel(int whichFullSel) {
    TString arrNameFullSel[2] = {"NormalFullSelection/", ""};
    
    return arrNameFullSel[whichFullSel];
}

TString PathYield_ControlSig(bool doControl) {
    TString arrControlName [2] = {"SignalRegion/", "ControlRegion/"};
    
    return arrControlName[doControl];
}

TString PathYield_ANDOR(bool useOR) {
    TString arrANDOR [2] = {"ANDCUT/", "ORCUT/"};
    
    return arrANDOR[useOR];
}

TString PathYield(bool isSig, int whichFullSel, bool doControl, bool useOR = 0) {
    TString strPath = LimitSettingArea();
    strPath += "DileptonYields/";
    strPath += isSig ? "SignalSamples/" : "Backgrounds/";
    strPath += PathYield_FullSel(whichFullSel);
    strPath += PathYield_ControlSig(doControl);
    strPath += PathYield_ANDOR(useOR);
    
    return strPath;
}


TString PathYield_SignalEfficiency(SUSYT2LimitParams * inSUSYT2LP, bool doControl) {
    TString strBWD = BaseWorkDirString();
    TString strPath = strBWD + "EfficiencyMaps/";
    strPath += inSUSYT2LP->SavingName(true);
    strPath += "/SignalYieldText/";
    strPath += doControl ? "/ControlRegion/" : "/SignalRegion/";

    return strPath;
}

TString NameYield_Signal(bool doSC, bool doControl, LimitParametersContainer * inLPC, MT2LimitParams * inMT2LP, SUSYT2LimitParams * inSUSYT2LP) {
    TString baseName = "yieldSig";
    baseName += doControl ? "Control" : "";
    baseName += doSC ? "_wSC_" : "_woSC_";
    
    baseName += inLPC->strFullSel + TString("_");
    
    baseName += inSUSYT2LP->SavingName(false);
    baseName += TString("_");
    baseName += inMT2LP->strMT2Append + TString(".txt");

    return baseName;
}

TString NameYield(MT2LimitParams * inMT2LP, bool isSig = 0, int whichPrefix = 1, bool useOR = 0) {
    TString arrNamePrefix[2] = {"Base", "TrimOut"};
    TString arrNameIntType[2] = {"ANDCut", "ORCut"};
    TString arrNameYieldType[2] = {"Backgrounds", "Signal"};
    
    TString spaceString = "_";
    
    TString outString = arrNamePrefix[whichPrefix] + arrNameYieldType[isSig] + spaceString + arrNameIntType[useOR] + spaceString + inMT2LP->strMT2Append + TString(".txt");
    return outString;
}

