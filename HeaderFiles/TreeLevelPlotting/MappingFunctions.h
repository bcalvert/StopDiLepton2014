#include <iostream>
#include <vector>
#include <map>
#include "TString.h"

typedef map<TString, TString> mapStrToStr;
typedef map<TString, float> mapStrToFloat;
typedef map<TString, bool> mapStrToBool;

void SetIsSimMap(mapStrToBool &inMap_StB_IsSim) {
    //Data File
    inMap_StB_IsSim["Data"] = false;
    
    //Fake Lepton Background File
    inMap_StB_IsSim["Fake"] = false;
    
    //Simulated Background Files
    inMap_StB_IsSim["TTbar"] = true;
    inMap_StB_IsSim["DY"] = true;
    inMap_StB_IsSim["Other"] = true;
    
    //Signal Files
    inMap_StB_IsSim["T2tt"] = true;
    inMap_StB_IsSim["T2tb"] = true;
    inMap_StB_IsSim["T2bwx0p25"] = true;
    inMap_StB_IsSim["T2bwx0p50"] = true;
    inMap_StB_IsSim["T2bwx0p75"] = true;
}

void SetIsSigMap(mapStrToBool &inMap_StB_IsSig) {
    //Data File
    inMap_StB_IsSig["Data"] = false;
    
    //Fake Lepton Background File
    inMap_StB_IsSig["Fake"] = false;
    
    //Simulated Background Files
    inMap_StB_IsSig["TTbar"] = false;
    inMap_StB_IsSig["DY"] = false;
    inMap_StB_IsSig["Other"] = false;
    
    //Signal Files
    inMap_StB_IsSig["T2tt"] = true;
    inMap_StB_IsSig["T2tb"] = true;
    inMap_StB_IsSig["T2bwx0p25"] = true;
    inMap_StB_IsSig["T2bwx0p50"] = true;
    inMap_StB_IsSig["T2bwx0p75"] = true;
}

void SetBareBonesWeightMap(mapStrToStr &inMap_StS_Weights) {
    
    //Data files don't have PU weight
    inMap_StS_Weights["Fake"] = "";
    inMap_StS_Weights["Data"] = "";
    
    //Simulated Background Weights
    inMap_StS_Weights["TTbar"] = "TWeightMC * ";
    inMap_StS_Weights["DY"] = "TWeightMC * ";
    inMap_StS_Weights["Other"] = "TWeightMC * ";
    
    //Signal Weights
    inMap_StS_Weights["T2tt"] = "1 * ";
    inMap_StS_Weights["T2tb"] = "1 * ";
    inMap_StS_Weights["T2bwx0p25"] = "1 * ";
    inMap_StS_Weights["T2bwx0p50"] = "1 * ";
    inMap_StS_Weights["T2bwx0p75"] = "1 * ";
}

void SetWeightMap(mapStrToStr &inMap_StS_Weights, int indexPolT2tt = -1, int indexPolT2bw = 4) {
    //Data weight -- none
    inMap_StS_Weights["Data"] = "";
    
    //Fake Lepton Background Weight
    inMap_StS_Weights["Fake"] = "TEventFakeLepWeight * ";
    
    //Simulated Background Weights
    inMap_StS_Weights["TTbar"] = "TWeightMC * TPUWeight1D * TGenWeight * ";
    inMap_StS_Weights["DY"] = "TWeightMC * TPUWeight1D * ";
    inMap_StS_Weights["Other"] = "TWeightMC * TPUWeight1D * ";
    
    //Signal Weights
    TString baseStrSignal = "TPUWeight1D * ";
    TString strT2tt = baseStrSignal;
    TString strT2bw = baseStrSignal;
    TString strT2tb = baseStrSignal;
    if (indexPolT2tt > -1) {
        strT2tt += " TGenStopPolWeights[";
        strT2tt += indexPolT2tt;
        strT2tt += "] * ";
    }
    if (indexPolT2bw > -1) {
        strT2bw += " TGenStopPolWeights[";
        strT2bw += indexPolT2bw;
        strT2bw += "] * ";
    }
    inMap_StS_Weights["T2tt"] = strT2tt;
    inMap_StS_Weights["T2bwx0p25"] = strT2bw;
    inMap_StS_Weights["T2bwx0p50"] = strT2bw;
    inMap_StS_Weights["T2bwx0p75"] = strT2bw;
    inMap_StS_Weights["T2tb"] = strT2tb;
}
/*
void SetWeightMap_Additional(mapStrToFloat &inMap_StF_Weights) {
    float vecLeptSF[iChannel] = " 0.9 * ";
    float lumi = "19656 * ";
    
    vector<float> vecWeightTTbar1B{"0.99", "1.13", "1.02"};
    vector<float> vecWeightTTbar2B{"1.06", "1.18", "1.07"};
    vector<float> vecWeightDY1B{"1.43", "1.46", "1.44"};
    vector<float> vecWeightDY2B{"1.53", "1.48", "1.50"};
    vector<float> vecWeightOther1B{"1", "1", "1"};
    vector<float> vecWeightOther2B{"1", "1", "1"};
    
    vector< vector<float> > vecVecWeightsTTbar{vecWeightTTbar1B, vecWeightTTbar2B};
    vector< vector<float> > vecVecWeightsDY{vecWeightDY1B, vecWeightDY2B};
    vector< vector<float> > vecVecWeightsOther{vecWeightOther1B, vecWeightOther2B};
    
    int numBJetCuts = 2;
    int numChannels = 3;
    
    vector<TString> vecBkgs{"TTbar", "DY", "Other"};
    
    TString strUnder = "_";
    for (int iChannel = 0; iChannel < numChannels; ++iChannel) {
        for (int iBJetCut = 0; iBJetCut < numBJetCuts; ++iBJetCut) {
            for (int iBkg = 0; iBkg < numBkgs; ++iBkg) {
                
                vector< vector<float> > * vecVecToUse;
                if (iBkg == 0) {
                    vecVecToUse = &vecVecWeightsTTbar;
                }
                else if (iBkg == 1) {
                    vecVecToUse = &vecVecWeightsDY;
                }
                else if (iBkg == 2) {
                    vecVecToUse = &vecVecWeightsOther;
                }
                
                TString strBkg = vecBkgs[iBkg];
                strBkg += strUnder;
                strBkg += iChannel;
                strBkg += strUnder;
                strBkg += iBJetCut + 1;
                inMap_StF_Weights[strBkg] = lumi + vecLeptSF[iChannel] vecVecToUse->at(iBJetCut)[iChannel] + TString(" * ");
                if (iChannel == 2) {
                    TString strBkgComp = vecBkgs[iBkg] + strUnder + TString("Comp") + strUnder;
                    strBkgComp += iBJetCut + 1;
                    inMap_StF_Weights[strBkgComp] = lumi + vecLeptSF[iChannel] vecVecToUse->at(iBJetCut)[iChannel] + TString(" * ");
                }
            }
        }
    }
}
*/

void SetWeightMap_Additional(mapStrToStr &inMap_StS_Weights) {
    vector<TString> vecLeptSF{" 0.9 * 1.01 * ", " 0.9 * 0.963 * ", " 0.9 * 1.016 * "};
    TString lumi = "19656 * ";
    
    vector<TString> vecWeightTTbar1B{"0.99", "1.13", "1.02"};
    vector<TString> vecWeightTTbar2B{"1.06", "1.18", "1.07"};
    vector<TString> vecWeightDY1B{"1.43", "1.46", "1.44"};
    vector<TString> vecWeightDY2B{"1.53", "1.48", "1.50"};
    vector<TString> vecWeightOther1B{"1", "1", "1"};
    vector<TString> vecWeightOther2B{"1", "1", "1"};
    
    vector< vector<TString> > vecVecWeightsTTbar{vecWeightTTbar1B, vecWeightTTbar2B};
    vector< vector<TString> > vecVecWeightsDY{vecWeightDY1B, vecWeightDY2B};
    vector< vector<TString> > vecVecWeightsOther{vecWeightOther1B, vecWeightOther2B};
    
    int numBJetCuts = 2;
    int numChannels = 3;
    int numBkgs = 4;
    
    vector<TString> vecBkgs{"TTbar", "DY", "Other"};
    
    TString strUnder = "_";
    for (int iChannel = 0; iChannel < numChannels; ++iChannel) {
        for (int iBJetCut = 0; iBJetCut < numBJetCuts; ++iBJetCut) {
            for (int iBkg = 0; iBkg < numBkgs; ++iBkg) {
                vector< vector<TString> > * vecVecToUse;
                if (iBkg == 0) {
                    vecVecToUse = &vecVecWeightsTTbar;
                }
                else if (iBkg == 1) {
                    vecVecToUse = &vecVecWeightsDY;
                }
                else if (iBkg == 2) {
                    vecVecToUse = &vecVecWeightsOther;
                }
                
                TString strBkg = vecBkgs[iBkg];
                strBkg += strUnder;
                strBkg += iChannel;
                strBkg += strUnder;
                strBkg += iBJetCut + 1;
                inMap_StS_Weights[strBkg] = lumi + vecLeptSF[iChannel] + vecVecToUse->at(iBJetCut)[iChannel] + TString(" * ");
                if (iChannel == 2) {
                    TString strBkgComp = vecBkgs[iBkg] + strUnder + TString("Comp") + strUnder;
                    strBkgComp += iBJetCut + 1;
                    inMap_StS_Weights[strBkgComp] = lumi + vecLeptSF[iChannel] + vecVecToUse->at(iBJetCut)[iChannel] + TString(" * ");
                }
            }
        }
    }
}


void SetFileNameMap(mapStrToStr &inMap_StS_Files) {
    //Data File
    inMap_StS_Files["Data"] = "Tree_Data_Bare_NoSyst";
    
    //Fake Lepton Background File
    inMap_StS_Files["Fake"] = "Tree_Fake_Bare_NoSyst";
    
    //Simulated Background Files
    inMap_StS_Files["TTbar"] = "Tree_TTbar_Bare_NoSyst";
    inMap_StS_Files["DY"] = "Tree_DY_Bare_NoSyst";
    inMap_StS_Files["Other"] = "Tree_NonDY_NonTTbar_Bare_NoSyst";
    
    //Signal Files
    inMap_StS_Files["T2tt"] = "Tree_T2tt_Bare_NoSyst";
    inMap_StS_Files["T2tb"] = "Tree_T2tb_Bare_NoSyst";
    inMap_StS_Files["T2bwx0p25"] = "Tree_T2bwx0p25_Bare_NoSyst";
    inMap_StS_Files["T2bwx0p50"] = "Tree_T2bwx0p50_Bare_NoSyst";
    inMap_StS_Files["T2bwx0p75"] = "Tree_T2bwx0p75_Bare_NoSyst";
}

TString SetTString(mapStrToStr * inMap_StS, TString inputString) {
    mapStrToStr::iterator xIter;
    xIter = inMap_StS->find(inputString);
    TString outString = "";
    if (xIter != inMap_StS->end()) {
        outString = xIter->second;
    }
    else {
        cout << "couldn't find the string -- " << inputString << " -- in the map!" << endl;
        return "BAD";
    }
    return outString;
}

bool SetBool(mapStrToBool * inMap_StB, TString inputString) {
    mapStrToBool::iterator xIter;
    xIter = inMap_StB->find(inputString);
    bool retValue = false;
    if (xIter != inMap_StB->end()) {
        retValue = xIter->second;
    }
    else {
        cout << "couldn't find the string -- " << inputString << " -- in the map!" << endl;
        return false;
    }
    return retValue;
}

float SetFloat(mapStrToFloat * inMap_StF, TString inputString) {
    mapStrToFloat::iterator xIter;
    xIter = inMap_StF->find(inputString);
    float retValue = 0.0;
    if (xIter != inMap_StF->end()) {
        retValue = xIter->second;
    }
    else {
        cout << "couldn't find the string -- " << inputString << " -- in the map!" << endl;
        return 0.0;
    }
    return retValue;
}
