#include <iostream>
#include <vector>
#include <map>
#include "TString.h"

using namespace std;

#define numMT2 4
#define numDYKillers 5
#define numROCTypes 2




TString ROCSaveString(int whichROCType, int whichROC) {
    vector<TString> vecROCSaveStrings_MT2{"MT2ll", "MT2lblb", "MT2bb_ZMET", "MT2lblb_ZeroBLepMass"};
    vector<TString> vecROCSaveStrings_DY{"SFMETCut", "SFMETSig1DCut", "SFMETSig2DCut", "SFMETSig2DFullCut", "SFZMassCut"};
    
    vector< vector<TString> > vecVecROCSaveStrings(numROCTypes);
    
    vecVecROCSaveStrings[0].insert(vecVecROCSaveStrings[0].begin(), vecROCSaveStrings_MT2.begin(), vecROCSaveStrings_MT2.end());
    vecVecROCSaveStrings[1].insert(vecVecROCSaveStrings[1].begin(), vecROCSaveStrings_DY.begin(), vecROCSaveStrings_DY.end());
    
    TString strROCSave = vecVecROCSaveStrings[whichROCType][whichROC];
    return strROCSave;
}

TString PunziAxisString(int whichROCType, int whichROC) {
    vector<TString> vecPunziAxisSaveString_MT2{"M_{T2}(ll) Cut [GeV]   ", "M_{T2}(lb)(lb) Cut [GeV]   ", "M_{T2}^{#slash{W}}(bb) Cut [GeV]   ", "M_{T2}^{0lb}(lb)(lb) Cut [GeV]   "};
    vector<TString> vecPunziAxisSaveString_DY{"SF #slash{E}_{T} cut", "SF #slash{E}^{2}_{T, 1D}/#sigma^{2}_{#slash{E}_{T, 1D}} cut", "SF #slash{E}^{2}_{T, 2D}/#sigma^{2}_{#slash{E}_{T, 2D}} cut", "SF #slash{E}_{T} significance cut", "SF M_{ll} cut"};
    
    vector< vector<TString> > vecVecPunziAxisStrings(numROCTypes);

    vecVecPunziAxisStrings[0].insert(vecVecPunziAxisStrings[0].begin(), vecPunziAxisSaveString_MT2.begin(), vecPunziAxisSaveString_MT2.end());
    vecVecPunziAxisStrings[1].insert(vecVecPunziAxisStrings[1].begin(), vecPunziAxisSaveString_DY.begin(), vecPunziAxisSaveString_DY.end());

    TString strPunziAxis = vecVecPunziAxisStrings[whichROCType][whichROC];
    
    return strPunziAxis;
}

TString ROCMainVarCutString(bool isSim, int whichROCType, int whichROC, bool doCut = false) {
    
    vector<TString> vecROCMainVarCutString_MT2{"TSmearMT2ll_v2", "TSmearMT2lblb_v2", "TSmearMT2bb_ZMET_v2", "TSmearMT2lblb_ZeroBLepMass_v2"};
    vector<TString> vecROCMainVarCutString_DY{"TSmearMET_v2", "TMath::Power(TSmearMET_v2/TMath::Sqrt(THT_AllJets), 2)", "TMath::Power(TSmearUPerp_v2/THadResPerpGauss_AllJets,2)+TMath::Power((TSmearUPar_v2+TDiLepPt)/THadResParGauss_AllJets, 2)", "TSmearMET_SigTrue_v2", "TMath::Abs(TDiLepMass - ZMass)"};
    
    vector< vector<TString> > vecVecROCMainVarCutStrings(numROCTypes);
    
    vecVecROCMainVarCutStrings[0].insert(vecVecROCMainVarCutStrings[0].begin(), vecROCMainVarCutString_MT2.begin(), vecROCMainVarCutString_MT2.end());
    vecVecROCMainVarCutStrings[1].insert(vecVecROCMainVarCutStrings[1].begin(), vecROCMainVarCutString_DY.begin(), vecROCMainVarCutString_DY.end());
    
    
    TString baseCutString = "";
    if (whichROCType == 1 && doCut) {
        baseCutString = "(TChannel == 2 || ((TChannel == 0 || TChannel == 1) && ";
    }
    
    
    TString strVarCut = baseCutString + vecVecROCMainVarCutStrings[whichROCType][whichROC];
    if (doCut) strVarCut += TString (" >= ");
    if (!isSim) {
        int index = strVarCut.Index("Smear");
        strVarCut.Replace(index, 5, "PF");
        index = strVarCut.Index("_v2");
        strVarCut.Replace(index, 3, "");
    }
    
    return strVarCut;
}

void SetMainVarCutValsVec(vector<int> * inVecCutVals, int whichROCType, int whichROC) {
    vector<int> vecMainVarMinCut_MT2{0, 0, 80, 0};
    vector<int> vecMainVarMaxCut_MT2{200, 300, 380, 300};
    vector<int> vecMainVarCutStepSize_MT2{10, 10, 10, 10};
    
    vector<int> vecMainVarMinCut_DY{0, 0, 0, 0, 0};
    vector<int> vecMainVarMaxCut_DY{200, 200, 200, 200, 30};
    vector<int> vecMainVarCutStepSize_DY{5, 2, 2, 2, 2};
    
    vector< vector<int> > vecVecMainVarMinCut(numROCTypes), vecVecMainVarMaxCut(numROCTypes), vecVecMainVarCutStepSize(numROCTypes);
    
    vecVecMainVarMinCut[0].insert(vecVecMainVarMinCut[0].begin(), vecMainVarMinCut_MT2.begin(), vecMainVarMinCut_MT2.end());
    vecVecMainVarMinCut[1].insert(vecVecMainVarMinCut[1].begin(), vecMainVarMinCut_DY.begin(), vecMainVarMinCut_DY.end());
    
    vecVecMainVarMaxCut[0].insert(vecVecMainVarMaxCut[0].begin(), vecMainVarMaxCut_MT2.begin(), vecMainVarMaxCut_MT2.end());
    vecVecMainVarMaxCut[1].insert(vecVecMainVarMaxCut[1].begin(), vecMainVarMaxCut_DY.begin(), vecMainVarMaxCut_DY.end());
    
    vecVecMainVarCutStepSize[0].insert(vecVecMainVarCutStepSize[0].begin(), vecMainVarCutStepSize_MT2.begin(), vecMainVarCutStepSize_MT2.end());
    vecVecMainVarCutStepSize[1].insert(vecVecMainVarCutStepSize[1].begin(), vecMainVarCutStepSize_DY.begin(), vecMainVarCutStepSize_DY.end());
    
    int minCut = vecVecMainVarMinCut[whichROCType][whichROC];
    int maxCut = vecVecMainVarMaxCut[whichROCType][whichROC];
    int cutStepSize = vecVecMainVarCutStepSize[whichROCType][whichROC];
    
    inVecCutVals->resize(0);
    for (int iCut = minCut; iCut <= maxCut; iCut += cutStepSize) {
        inVecCutVals->push_back(iCut);
    }
}



