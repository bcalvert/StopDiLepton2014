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
    TString stringMETSig1DHack = "METSig1DHack";
    TString stringMETSig1DHack_AllJets = "METSig1DHack_AllJets";
    TString stringMETSig2DHack = "METSig2DHack";
    TString stringMETSig2DHack_AllJets = "METSig2DHack_AllJets";
    TString stringMETSig2DHack_Par = "METSig2DHack_Par";
    TString stringMETSig2DHack_Perp = "METSig2DHack_Perp";
    TString stringMETSig2DHack_Par_AllJets = "METSig2DHack_Par_AllJets";
    TString stringMETSig2DHack_Perp_AllJets = "METSig2DHack_Perp_AllJets";
    
    TString stringMETSig2DHackFull = "METSig2DHackFull";
    TString stringMETSig2DHack_ParFull = "METSig2DHack_ParFull";
    TString stringMETSig2DHack_PerpFull = "METSig2DHack_PerpFull";
    
    TString stringMETSig2DTrue = "METSig2DTrue";
    
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
    
    HistogramT H_METSig1DHack; H_METSig1DHack.DefaultVarVals();
    H_METSig1DHack.SetName(stringSmear, stringMETSig1DHack, "");
    H_METSig1DHack.xAxis.SetAxis("patsy", stringMETSig1DHack, &inSHBB->m_histBounds_EnergyPtMET_SR, bDoSyst);
    H_METSig1DHack.SetIndAxisLabel(stringSmear + stringMETSig1DHack, mapVartoLabel, 1);
    H_METSig1DHack.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig1DHack_AllJets; H_METSig1DHack_AllJets.DefaultVarVals();
    H_METSig1DHack_AllJets.SetName(stringSmear, stringMETSig1DHack_AllJets, "");
    H_METSig1DHack_AllJets.xAxis.SetAxis("patsy", stringMETSig1DHack_AllJets, &inSHBB->m_histBounds_EnergyPtMET_SR, bDoSyst);
    H_METSig1DHack_AllJets.SetIndAxisLabel(stringSmear + stringMETSig1DHack_AllJets, mapVartoLabel, 1);
    H_METSig1DHack_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack; H_METSig2DHack.DefaultVarVals();
    H_METSig2DHack.SetName(stringSmear, stringMETSig2DHack, "");
    H_METSig2DHack.xAxis.SetAxis("patsy", stringMETSig2DHack, &inSHBB->m_histBounds_EnergyPtMET_SR, bDoSyst);
    H_METSig2DHack.SetIndAxisLabel(stringSmear + stringMETSig2DHack, mapVartoLabel, 1);
    H_METSig2DHack.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_AllJets; H_METSig2DHack_AllJets.DefaultVarVals();
    H_METSig2DHack_AllJets.SetName(stringSmear, stringMETSig2DHack_AllJets, "");
    H_METSig2DHack_AllJets.xAxis.SetAxis("patsy", stringMETSig2DHack_AllJets, &inSHBB->m_histBounds_EnergyPtMET_SR, bDoSyst);
    H_METSig2DHack_AllJets.SetIndAxisLabel(stringSmear + stringMETSig2DHack_AllJets, mapVartoLabel, 1);
    H_METSig2DHack_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DTrue; H_METSig2DTrue.DefaultVarVals();
    H_METSig2DTrue.SetName(stringSmear, stringMETSig2DTrue, "");
    H_METSig2DTrue.xAxis.SetAxis("patsy", stringMETSig2DTrue, &inSHBB->m_histBounds_EnergyPtMET_SR, bDoSyst);
    H_METSig2DTrue.SetIndAxisLabel(stringSmear + stringMETSig2DTrue, mapVartoLabel, 1);
    H_METSig2DTrue.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHackFull; H_METSig2DHackFull.DefaultVarVals();
    H_METSig2DHackFull.SetName(stringSmear, stringMETSig2DHackFull, "");
    H_METSig2DHackFull.xAxis.SetAxis("patsy", stringMETSig2DHackFull, &inSHBB->m_histBounds_EnergyPtMET_SR, bDoSyst);
    H_METSig2DHackFull.SetIndAxisLabel(stringSmear + stringMETSig2DHackFull, mapVartoLabel, 1);
    H_METSig2DHackFull.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Par; H_METSig2DHack_Par.DefaultVarVals();
    H_METSig2DHack_Par.SetName(stringSmear, stringMETSig2DHack_Par, "");
    H_METSig2DHack_Par.xAxis.SetAxis("patsy", stringMETSig2DHack_Par, &inSHBB->m_histBounds_RootPxPy, bDoSyst);
    H_METSig2DHack_Par.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Par, mapVartoLabel, 1);
    H_METSig2DHack_Par.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Perp; H_METSig2DHack_Perp.DefaultVarVals();
    H_METSig2DHack_Perp.SetName(stringSmear, stringMETSig2DHack_Perp, "");
    H_METSig2DHack_Perp.xAxis.SetAxis("patsy", stringMETSig2DHack_Perp, &inSHBB->m_histBounds_RootPxPy, bDoSyst);
    H_METSig2DHack_Perp.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Perp, mapVartoLabel, 1);
    H_METSig2DHack_Perp.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Par_AllJets; H_METSig2DHack_Par_AllJets.DefaultVarVals();
    H_METSig2DHack_Par_AllJets.SetName(stringSmear, stringMETSig2DHack_Par_AllJets, "");
    H_METSig2DHack_Par_AllJets.xAxis.SetAxis("patsy", stringMETSig2DHack_Par_AllJets, &inSHBB->m_histBounds_RootPxPy, bDoSyst);
    H_METSig2DHack_Par_AllJets.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Par_AllJets, mapVartoLabel, 1);
    H_METSig2DHack_Par_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_Perp_AllJets; H_METSig2DHack_Perp_AllJets.DefaultVarVals();
    H_METSig2DHack_Perp_AllJets.SetName(stringSmear, stringMETSig2DHack_Perp_AllJets, "");
    H_METSig2DHack_Perp_AllJets.xAxis.SetAxis("patsy", stringMETSig2DHack_Perp_AllJets, &inSHBB->m_histBounds_RootPxPy, bDoSyst);
    H_METSig2DHack_Perp_AllJets.SetIndAxisLabel(stringSmear + stringMETSig2DHack_Perp_AllJets, mapVartoLabel, 1);
    H_METSig2DHack_Perp_AllJets.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_ParFull; H_METSig2DHack_ParFull.DefaultVarVals();
    H_METSig2DHack_ParFull.SetName(stringSmear, stringMETSig2DHack_ParFull, "");
    H_METSig2DHack_ParFull.xAxis.SetAxis("patsy", stringMETSig2DHack_ParFull, &inSHBB->m_histBounds_RootPxPy, bDoSyst);
    H_METSig2DHack_ParFull.SetIndAxisLabel(stringSmear + stringMETSig2DHack_ParFull, mapVartoLabel, 1);
    H_METSig2DHack_ParFull.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
    HistogramT H_METSig2DHack_PerpFull; H_METSig2DHack_PerpFull.DefaultVarVals();
    H_METSig2DHack_PerpFull.SetName(stringSmear, stringMETSig2DHack_PerpFull, "");
    H_METSig2DHack_PerpFull.xAxis.SetAxis("patsy", stringMETSig2DHack_PerpFull, &inSHBB->m_histBounds_RootPxPy, bDoSyst);
    H_METSig2DHack_PerpFull.SetIndAxisLabel(stringSmear + stringMETSig2DHack_PerpFull, mapVartoLabel, 1);
    H_METSig2DHack_PerpFull.yAxis.SetAxisAsDefaultCountAxis("Sig.");
    
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
    
    inHistTVec->push_back(H_METSig1DHack);
    inHistTVec->push_back(H_METSig1DHack_AllJets);
    inHistTVec->push_back(H_METSig2DHack);
    inHistTVec->push_back(H_METSig2DHack_AllJets);
    inHistTVec->push_back(H_METSig2DHack_Par);
    inHistTVec->push_back(H_METSig2DHack_Par_AllJets);
    inHistTVec->push_back(H_METSig2DHack_Perp);
    inHistTVec->push_back(H_METSig2DHack_Perp_AllJets);
    inHistTVec->push_back(H_METSig2DHackFull);
    inHistTVec->push_back(H_METSig2DHack_ParFull);
    inHistTVec->push_back(H_METSig2DHack_PerpFull);
    inHistTVec->push_back(H_METSig2DTrue);
    
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