#include <iostream>
#include <vector>
#include "TString.h"

using namespace std;
typedef pair<float, float> knobPunzi;

#define numDraw 31


TString PostPunziName(knobPunzi * inKP) {
    TString outPostPunziName = "";
    outPostPunziName += "SigmaA";
    outPostPunziName += inKP->first;
    outPostPunziName += "SigmaB";
    outPostPunziName += inKP->second;
    return outPostPunziName;
}
int NumBJetsCut(int whichDraw) {
    vector<int> vec0BJet{0};
    vector<int> vec1BJet{1, 2, 3, 4, 9, 11, 13, 14, 15, 16, 17, 18, 19, 20, 21};
    vector<int> vec2BJet{5, 6, 7, 8, 10, 12, 22, 23, 24};
    
    for (unsigned int iB0 = 0; iB0 < vec0BJet.size(); ++iB0) {
        if (whichDraw == vec0BJet[iB0]) return 0;
    }

    for (unsigned int iB1 = 0; iB1 < vec1BJet.size(); ++iB1) {
        if (whichDraw == vec1BJet[iB1]) return 1;
    }
    for (unsigned int iB2 = 0; iB2 < vec2BJet.size(); ++iB2) {
        if (whichDraw == vec2BJet[iB2]) return 2;
    }
    return 1;
}
void FillLatexCutString(int whichDraw, vector<TString> * inVecLatexStrings) {
    inVecLatexStrings->resize(0);
    
    if (whichDraw == 0) {
        inVecLatexStrings->push_back("Dilep. Selection");
    }
    else if (whichDraw == 1) {
        inVecLatexStrings->push_back("Jet Selection");
    }
    else if (whichDraw >= 2) {
        inVecLatexStrings->push_back("Preselection");
    }
    vector<int> vecNoZMass{5, 11, 12};
    vector<int> vec2BJets{5, 6, 7, 8, 10, 12, 22, 23, 24, 27, 28};
    vector<int> vecLowBLepMass{19, 20, 21, 22, 23, 24, 25, 26, 27, 28};
    vector<int> vecMETSig1D{13, 14, 15, 25, 26};
    vector<int> vecMETSigTrue{16, 17, 18, 27, 28};
    
    vector<int> vecMT2llTail{3, 7, 14, 17, 20, 23};
    vector<int> vecMT2llControl{4, 8, 15, 18, 21, 24};
    vector<int> vecMT2Tail{9, 11};
    vector<int> vecMT2Control{10, 12};
    
    
    for (unsigned int iNoZ = 0; iNoZ < vecNoZMass.size(); ++iNoZ) {
        if (whichDraw == vecNoZMass[iNoZ]) inVecLatexStrings->push_back("No SF DY Cuts");
    }
    
    for (unsigned int i2B = 0; i2B < vec2BJets.size(); ++i2B) {
        if (whichDraw == vec2BJets[i2B]) inVecLatexStrings->push_back("2+ b-jets");
    }
    
    for (unsigned int iLowBLep = 0; iLowBLep < vecLowBLepMass.size(); ++iLowBLep) {
        if (whichDraw == vecLowBLepMass[iLowBLep]) inVecLatexStrings->push_back("m_{(bl)} < 200 GeV");
    }
    
    for (unsigned int iMETSig1D = 0; iMETSig1D < vecMETSig1D.size(); ++iMETSig1D) {
        if (whichDraw == vecMETSig1D[iMETSig1D]) inVecLatexStrings->push_back("SF: #frac{#slash{E}_{T}^{2}}{H_{T}} > 80");
    }
    
    for (unsigned int iMETSigTrue = 0; iMETSigTrue < vecMETSigTrue.size(); ++iMETSigTrue) {
        if (whichDraw == vecMETSigTrue[iMETSigTrue]) inVecLatexStrings->push_back("SF: #slash{E}_{T} Sig. > 160");
    }
    
    for (unsigned int iMT2llTail = 0; iMT2llTail < vecMT2llTail.size(); ++iMT2llTail) {
        if (whichDraw == vecMT2llTail[iMT2llTail]) inVecLatexStrings->push_back("M_{T2}(ll) > 80 GeV");
    }
    
    for (unsigned int iMT2llControl = 0; iMT2llControl < vecMT2llControl.size(); ++iMT2llControl) {
        if (whichDraw == vecMT2llControl[iMT2llControl]) inVecLatexStrings->push_back("M_{T2}(ll) < 80 GeV");
    }
    
    for (unsigned int iMT2Tail = 0; iMT2Tail < vecMT2Tail.size(); ++iMT2Tail) {
        if (whichDraw == vecMT2Tail[iMT2Tail]) inVecLatexStrings->push_back("M_{T2} Control");
    }
    
    for (unsigned int iMT2Control = 0; iMT2Control < vecMT2Control.size(); ++iMT2Control) {
        if (whichDraw == vecMT2Control[iMT2Control]) inVecLatexStrings->push_back("M_{T2} Tail");
    }
}

TString ROCDrawString(int whichDraw) {
    TString arrStrDraw[numDraw];
    
    TString strDilep = "Dilep";
    TString stringJet = "Jet";
    TString stringJet2 = "Jet2BJet";
    TString stringJet0 = "Jet0BJet";
    TString stringLowBLepMass = "LowBLepMass";
    TString stringZMass = "ZMass";
    TString stringZMassMETSig1D = "ZMassMETSig1D";
    TString stringZMassMETSig2D = "ZMassMETSig2D";
    TString stringZCR = "ZCR";
    TString strMT2Tail = "MT2Tail";
    TString strMT2Control = "MT2Control";
    TString strMT2llTail = "MT2llTail";
    TString strMT2llControl = "MT2llControl";
    
    arrStrDraw[0] = strDilep;
    arrStrDraw[1] = strDilep + stringJet;
    arrStrDraw[2] = strDilep + stringJet + stringZMass;
    arrStrDraw[3] = strDilep + stringJet + stringZMass + strMT2llTail;
    arrStrDraw[4] = strDilep + stringJet + stringZMass + strMT2llControl;
    arrStrDraw[5] = strDilep + stringJet2;
    arrStrDraw[6] = strDilep + stringJet2 + stringZMass;
    arrStrDraw[7] = strDilep + stringJet2 + stringZMass + strMT2llTail;
    arrStrDraw[8] = strDilep + stringJet2 + stringZMass + strMT2llControl;
    arrStrDraw[9] = strDilep + stringJet + stringZMass + strMT2Tail;
    arrStrDraw[10] = strDilep + stringJet2 + stringZMass + strMT2Tail;
    arrStrDraw[11] = strDilep + stringJet + strMT2Tail;
    arrStrDraw[12] = strDilep + stringJet2 + strMT2Tail;
    arrStrDraw[13] = strDilep + stringJet + stringZMassMETSig1D;
    arrStrDraw[14] = strDilep + stringJet + stringZMassMETSig1D + strMT2llTail;
    arrStrDraw[15] = strDilep + stringJet + stringZMassMETSig1D + strMT2llControl;
    arrStrDraw[16] = strDilep + stringJet + stringZMassMETSig2D;
    arrStrDraw[17] = strDilep + stringJet + stringZMassMETSig2D + strMT2llTail;
    arrStrDraw[18] = strDilep + stringJet + stringZMassMETSig2D + strMT2llControl;
    arrStrDraw[19] = strDilep + stringJet + stringZMass + stringLowBLepMass;
    arrStrDraw[20] = strDilep + stringJet + stringZMass + stringLowBLepMass + strMT2llTail;
    arrStrDraw[21] = strDilep + stringJet + stringZMass + stringLowBLepMass + strMT2llControl;
    arrStrDraw[22] = strDilep + stringJet2 + stringZMass + stringLowBLepMass;
    arrStrDraw[23] = strDilep + stringJet2 + stringZMass + stringLowBLepMass + strMT2llTail;
    arrStrDraw[24] = strDilep + stringJet2 + stringZMass + stringLowBLepMass + strMT2llControl;    
    arrStrDraw[25] = strDilep + stringJet + stringZMassMETSig1D + stringLowBLepMass;
    arrStrDraw[26] = strDilep + stringJet + stringZMassMETSig2D + stringLowBLepMass;
    arrStrDraw[27] = strDilep + stringJet2 + stringZMassMETSig1D + stringLowBLepMass;
    arrStrDraw[28] = strDilep + stringJet2 + stringZMassMETSig2D + stringLowBLepMass;
    arrStrDraw[29] = strDilep + stringZCR + stringLowBLepMass;
    arrStrDraw[30] = strDilep + stringJet0 + stringZMass + stringLowBLepMass;
    
    TString strDraw = arrStrDraw[whichDraw];
    return strDraw;
}


TString ROCCutString(bool isSim, int whichDraw) {
    TString selDilep = "TDoEvent && ((TPassDoubleMu && TChannel == 0) || (TPassDoubleEl && TChannel == 1) || (TPassElMu && TChannel == 2))";
    TString diLepZMassCut = "(TChannel == 2 || (!TDiLepInZ && ";
    diLepZMassCut += isSim ? "TSmearMET_v2" : "TPFMET";
    diLepZMassCut += " >= 40 && (TChannel == 0 || TChannel == 1)))";
    
    TString diLepZCRCut = isSim ? "(TSmearMET_v2" : "TPFMET";
    diLepZCRCut += " >= 40)";
    TString diLepZMassMETSig1DCut = "(TChannel == 2 || (!TDiLepInZ && ";
    diLepZMassMETSig1DCut += isSim ? "TMath::Power(TSmearMET_v2/TMath::Sqrt(THT_AllJets), 2)" : "TMath::Power(TPFMET/TMath::Sqrt(THT_AllJets), 2)";
    diLepZMassMETSig1DCut += " >= 80 && (TChannel == 0 || TChannel == 1)))";
    TString diLepZMassMETSig2DCut = "(TChannel == 2 || (!TDiLepInZ && ";
    diLepZMassMETSig2DCut += isSim ? "TSmearMET_SigTrue_v2" : "TPFMET_SigTrue";
    diLepZMassMETSig2DCut += " >= 160 && (TChannel == 0 || TChannel == 1)))";
    TString jetCut = "TNJets > 1 && TNJetsBtag > 0";
    TString jetCut2 = "TNJets > 1 && TNJetsBtag > 1";
    TString jetCut0 = "TNJets > 1 && TNJetsBtag == 0";
    TString lowBLepMassCut = "TMath::Max(TSmearMassBLep0_BLepsUsed_v2, TSmearMassBLep1_BLepsUsed_v2) < 200";
    if (!isSim) lowBLepMassCut = "TMath::Max(TPFMassBLep0_BLepsUsed, TPFMassBLep1_BLepsUsed) < 200";
    TString cutStringMT2Tail = "(TSmearMT2ll_v2 > 80 || TSmearMT2lblb_v2 > 170 || TSmearMT2bb_ZMET_v2 > 170)";
    TString cutStringMT2llTail = "(TSmearMT2ll_v2 > 80)";
    TString cutStringMT2llControl = "(TSmearMT2ll_v2 < 80)";
    if (!isSim) {
        cutStringMT2Tail = "(TPFMT2ll > 80 || TPFMT2lblb > 170 || TPFMT2bb_ZMET > 170)";
        cutStringMT2llTail = "(TPFMT2ll > 80)";
        cutStringMT2llControl = "(TPFMT2ll < 80)";
    }
    TString andCut = " && ";
    
    
    TString arrStrCut[numDraw];
    arrStrCut[0] = selDilep;
    arrStrCut[1] = selDilep + andCut + jetCut;
    arrStrCut[2] = selDilep + andCut + jetCut + andCut + diLepZMassCut;
    arrStrCut[3] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + cutStringMT2llTail;
    arrStrCut[4] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + cutStringMT2llControl;
    arrStrCut[5] = selDilep + andCut + jetCut2;
    arrStrCut[6] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut;
    arrStrCut[7] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + cutStringMT2llTail;
    arrStrCut[8] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + cutStringMT2llControl;
    arrStrCut[9] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + cutStringMT2Tail;
    arrStrCut[10] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + cutStringMT2Tail;
    
    arrStrCut[11] = selDilep + andCut + jetCut + andCut + cutStringMT2Tail;
    arrStrCut[12] = selDilep + andCut + jetCut2 + andCut + cutStringMT2Tail;
    arrStrCut[13] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut;
    arrStrCut[14] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut + andCut + cutStringMT2llTail;
    arrStrCut[15] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut + andCut + cutStringMT2llControl;
    arrStrCut[16] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut;
    arrStrCut[17] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut + andCut + cutStringMT2llTail;
    arrStrCut[18] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut + andCut + cutStringMT2llControl;
    arrStrCut[19] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + lowBLepMassCut;
    arrStrCut[20] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llTail;
    arrStrCut[21] = selDilep + andCut + jetCut + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llControl;
    arrStrCut[22] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + lowBLepMassCut;
    arrStrCut[23] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llTail;
    arrStrCut[24] = selDilep + andCut + jetCut2 + andCut + diLepZMassCut + andCut + lowBLepMassCut + andCut + cutStringMT2llControl;
    arrStrCut[25] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig1DCut + andCut + lowBLepMassCut;
    arrStrCut[26] = selDilep + andCut + jetCut + andCut + diLepZMassMETSig2DCut + andCut + lowBLepMassCut;
    arrStrCut[27] = selDilep + andCut + jetCut2 + andCut + diLepZMassMETSig1DCut + andCut + lowBLepMassCut;
    arrStrCut[28] = selDilep + andCut + jetCut2 + andCut + diLepZMassMETSig2DCut + andCut + lowBLepMassCut;
    arrStrCut[29] = selDilep + andCut + jetCut0 + andCut + diLepZCRCut + andCut + lowBLepMassCut;
    arrStrCut[30] = selDilep + andCut + jetCut0 + andCut + diLepZMassCut + andCut + lowBLepMassCut;

    
    TString strCutCommand = arrStrCut[whichDraw];
    return strCutCommand;
}