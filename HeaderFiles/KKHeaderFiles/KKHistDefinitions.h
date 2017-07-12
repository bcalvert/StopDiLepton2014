#ifndef KK_HIST_DEFINITIONS_H
#define KK_HIST_DEFINITIONS_H

#include <vector>
#include <map>

#include "HistogramT.h"
#include "StopHistBinBounds.h"



typedef std::map<TString, TString> labelMap;

void OneDeeHistTVec_AddBasicMETHists(std::vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB,  labelMap * mapVartoLabel) {
    
    bool bDoSyst(false); //set to false for now

    TString stringSmear = ""; //hard-coded for now because it's not illustrative -- normally set to GetStringSmear(isSmear, false);
    TString stringMET = "MET";
    
    HistogramT H_MET; H_MET.DefaultVarVals();
    H_MET.SetName(stringSmear, stringMET, "");
    H_MET.xAxis.SetAxis("patsy", stringMET, &inSHBB->m_histBounds_EnergyPtMET, bDoSyst);
    H_MET.SetIndAxisLabel(stringSmear + stringMET, mapVartoLabel, 1);
    H_MET.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_METTightBin; H_METTightBin.DefaultVarVals();
    H_METTightBin.SetName(stringSmear, stringMET, "TightBin");
    H_METTightBin.xAxis.SetAxis("patsy", stringMET, 8, 0., 400., bDoSyst);
    H_METTightBin.SetIndAxisLabel(stringSmear + stringMET, mapVartoLabel, 1);
    H_METTightBin.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_METPhi; H_METPhi.DefaultVarVals();
    H_METPhi.SetName(stringSmear, stringMET + "Phi", "");
    H_METPhi.xAxis.SetAxis("patsy", stringMET + "Phi", &inSHBB->m_histBounds_Phi, bDoSyst);
    H_METPhi.SetIndAxisLabel(stringSmear + stringMET + "Phi", mapVartoLabel, 1);
    H_METPhi.yAxis.SetAxisAsDefaultCountAxis("radians");    
    
    HistogramT H_METX; H_METX.DefaultVarVals();
    H_METX.SetName(stringSmear, stringMET + "X", "");
    H_METX.xAxis.SetAxis("patsy", stringMET + "X", &inSHBB->m_histBounds_PxPy, bDoSyst);
    H_METX.SetIndAxisLabel(stringSmear + stringMET + "X", mapVartoLabel, 1);
    H_METX.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_METY; H_METY.DefaultVarVals();
    H_METY.SetName(stringSmear, stringMET + "Y", "");
    H_METY.xAxis.SetAxis("patsy", stringMET + "Y", &inSHBB->m_histBounds_PxPy, bDoSyst);
    H_METY.SetIndAxisLabel(stringSmear + stringMET + "Y", mapVartoLabel, 1);
    H_METY.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    HistogramT H_TTBarPt; H_TTBarPt.DefaultVarVals();
    H_TTBarPt.SetName(stringSmear, "TTBarPt", "");
    H_TTBarPt.xAxis.SetAxis("patsy", "TTBarPt", &inSHBB->m_histBounds_EnergyPtMET, bDoSyst);
    H_TTBarPt.SetIndAxisLabel(stringSmear + "TTBarPt", mapVartoLabel, 1);
    H_TTBarPt.yAxis.SetAxisAsDefaultCountAxis("GeV");
    
    inHistTVec->push_back(H_MET);
    //inHistTVec->push_back(H_METTightBin);
    inHistTVec->push_back(H_METPhi);
    inHistTVec->push_back(H_METX);
    inHistTVec->push_back(H_METY);    
    
    inHistTVec->push_back(H_TTBarPt);
}


void SetHistTVec_Inclusive(std::vector<HistogramT> * inHistTVec, StopHistBinBounds * inSHBB, labelMap * mapVartoLabel, int iNDims) {
    //bool noSmear = false;
    
    switch (iNDims) {
        case 1:
            OneDeeHistTVec_AddBasicMETHists(inHistTVec, inSHBB, mapVartoLabel);
            break;
            /*
        case 2:
            TwoDeeHistTVec_Inclusive(inHistTVec, inSHBB, mapVartoLabel, cutPlotLevel);            
            break;
        case 3:
            ThreeDeeHistTVec_Inclusive(inHistTVec, mapVartoLabel, cutPlotLevel);
            break;
            */
        default:
            std::cout << "num of Dims should be 1, 2, or 3 -- it is " << iNDims << std::endl;
            break;
    }
}
#endif // KK_HIST_DEFINITIONS_H