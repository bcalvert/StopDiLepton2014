#include <iostream>
#include "TTree.h"
#include "TMath.h"
#include "TFile.h"
#include "TH3F.h"
#include "TH2F.h"
#include "TAxis.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TRint.h"
#include <vector>
#include <cmath>

using namespace std;
#include "../../HeaderFiles/StopEfficiencyMapHeaderFiles.h"
/*
#include "../../HeaderFiles/BasicFunctions_SystemCommands.h"
#include "../../HeaderFiles/LimitScriptHeaderFiles/LimitScripts_ParametersStruct.h"
#include "../../HeaderFiles/LimitScriptHeaderFiles/LimitFunctions_Saving.h"
#include "../../HeaderFiles/LimitScriptHeaderFiles/YieldCalculationFunctions.h"
#include "../../HeaderFiles/LimitScriptHeaderFiles/CardCreationFunctions.h"
#include "../../HeaderFiles/LimitScriptHeaderFiles/LimitCombinationFunctions.h"
*/



int main( int argc, char* argv[]) {        
    gROOT->ForceStyle();
    gROOT->ProcessLine("#include <vector>");
    // Style things
    gStyle->SetErrorX(0.5); // set X uncertainty to 1/2 of a bin
    gStyle->SetTitleFont(42);
    
    
    TRint theApp("App", &argc, argv);
    Bool_t retVal = kTRUE;
    
    vector<float> vecTTBarSF_FullCut_LowBLepMass{0.9863, 1.1262, 1.0186, 1.0255};
    vector<float> vecTTBarSF_FullCut_2BJet_LowBLepMass{1.06249, 1.166, 1.06337, 1.0775};
    vector<float> vecTTBarSF_FullCut_METSig1D_LowBLepMass{1.06358, 1.13579, 1.01866, 1.0258};
    vector<float> vecTTBarSF_FullCut_METSigTrue_LowBLepMass{0.871074, 0.9681, 1.01866, 1.0028};
    vector<float> vecTTBarSF_FullCut_0BJets{0.8274, 0.7462, 0.8292, 0.8172};

    
    vector<float> vecTTBarSF_FullCut_LowBLepMass_SimFakes{1.021, 1.111, 1.027, 1.0375};
    vector<float> vecTTBarSF_FullCut_2BJet_LowBLepMass_SimFakes{1.06249, 1.166, 1.06337, 1.0775};
    vector<float> vecTTBarSF_FullCut_METSig1D_LowBLepMass_SimFakes{1.06358, 1.13579, 1.01866, 1.0258};
    vector<float> vecTTBarSF_FullCut_METSigTrue_LowBLepMass_SimFakes{0.871074, 0.9681, 1.01866, 1.0028};
    vector<float> vecTTBarSF_FullCut_0BJets_SimFakes{0.965, 0.829, 0.925, 0.923};
    
    vector< vector<float> > vecVecTTBarSFCuts{vecTTBarSF_FullCut_LowBLepMass, vecTTBarSF_FullCut_2BJet_LowBLepMass, vecTTBarSF_FullCut_METSig1D_LowBLepMass, vecTTBarSF_FullCut_METSigTrue_LowBLepMass, vecTTBarSF_FullCut_0BJets, vecTTBarSF_FullCut_0BJets};
    vector< vector<float> > vecVecTTBarSFCuts_SimFakes{vecTTBarSF_FullCut_LowBLepMass_SimFakes, vecTTBarSF_FullCut_2BJet_LowBLepMass_SimFakes, vecTTBarSF_FullCut_METSig1D_LowBLepMass_SimFakes, vecTTBarSF_FullCut_METSigTrue_LowBLepMass_SimFakes, vecTTBarSF_FullCut_0BJets, vecTTBarSF_FullCut_0BJets_SimFakes};
    
    vector<float> * vecTTBarSFToUse = NULL;
    
    MT2LimitParams MT2LPs;
    MT2LPs.DefaultVals();
    
    SUSYT2LimitParams SUSYT2LPs;
    SUSYT2LPs.DefaultVals();
    SUSYT2LPs.SetParamsFromCommandLine(argc, argv);
    SUSYT2LPs.SetStrings();
    SUSYT2LPs.PrintStrings();
    
    SUSYT2LimitParams SUSYT2LP_Hack;
    SUSYT2LP_Hack.FullSequence(argc, argv);
    if (SUSYT2LP_Hack.useFixParams) {
        SUSYT2LP_Hack.FixTheFixParams();
        SUSYT2LP_Hack.SetStrings();
    }
    
    SUSYMassLimitParams SUSYMLPs;
    SUSYMLPs.DefaultVals();
    SUSYMLPs.PrintVals();
    SUSYMLPs.SetParamsFromCommandLine(argc, argv);
    
    LimitParametersContainer BasicLPs;
    BasicLPs.DefaultVals();
    BasicLPs.SetParamsFromCommandLine(argc, argv);
    BasicLPs.SetStrings();
    BasicLPs.PrintStrings();
    
    TString addName_OptMap;
    
    LimitParametersContainer BasicLPs_AddName_OptMap = BasicLPs;
    BasicLPs_AddName_OptMap.typeLepComboAnaType = 0;
    BasicLPs_AddName_OptMap.fixDMBin = -1;
    BasicLPs_AddName_OptMap.isSignificance = false;
    BasicLPs_AddName_OptMap.SetStrings();
    cout << "debug" << endl;
    BasicLPs_AddName_OptMap.PrintStrings();
    
    if (BasicLPs.isSignificance) {
        if (BasicLPs.simFakes) {
            vecTTBarSFToUse = &vecVecTTBarSFCuts_SimFakes[BasicLPs.typeFullSel];
        }
        else {
            vecTTBarSFToUse = &vecVecTTBarSFCuts[BasicLPs.typeFullSel];
        }
    }
    
    TString baseStringCustPath = "/data/users/bcalvert/StopDiLepton2014/LimitSetting/ShapeAnalysis/LookElsewhere/";
    TString arrCustPath[5] = {"T2ttNoPol/", "T2ttRPol/", "T2bwx0p25NoPol/", "T2ttLPol/", "T2bwx0p25LCharLW/"};
    
    int whichCustPath = -1;
    
    for (int iT2SMSParam = 0; iT2SMSParam < argc; ++iT2SMSParam) {
        if (strncmp (argv[iT2SMSParam],"whichCustPath", 13) == 0) {
            whichCustPath = strtol(argv[iT2SMSParam + 1], NULL, 10);
        }
    }
    
    TString pathCust = whichCustPath > -1 ? baseStringCustPath + arrCustPath[whichCustPath] : "";
    
    
    int maxMassDiff = 100;
    for (int currMassStop = SUSYMLPs.minStopMass; currMassStop <= SUSYMLPs.maxStopMass; currMassStop += SUSYMLPs.binSize) {
        int currMaxMassLSP = currMassStop - maxMassDiff;
        for (int currMassLSP = SUSYMLPs.minLSPMass; currMassLSP <= TMath::Min(SUSYMLPs.maxLSPMass, currMaxMassLSP); currMassLSP += SUSYMLPs.binSize) {
            
            SUSYMLPs.massStop = currMassStop;
            SUSYMLPs.massLSP = currMassLSP;
            SUSYMLPs.SetMassStrings();
            cout << "currMassStop " << currMassStop << endl;
            cout << "currMassLSP " << currMassLSP << endl;
            
            
            addName_OptMap = BaseStringLimit_NameOrDir(&BasicLPs_AddName_OptMap, &SUSYT2LP_Hack, 2);
            addName_OptMap.Replace(addName_OptMap.Index("Exp_"), 4, "");
            addName_OptMap += "_dM";
            if (BasicLPs.fixDMBin > -1) {
                addName_OptMap += BasicLPs.fixDMBin;
            }
            else {
                addName_OptMap += SUSYMLPs.GetDM();
            }
            addName_OptMap += BasicLPs_AddName_OptMap.strDim + TString("ToOneDee");
            int lengthPowShapeStr = BasicLPs_AddName_OptMap.strPowShape.Length();
            if (lengthPowShapeStr > 0) {
                addName_OptMap.Replace(addName_OptMap.Index( BasicLPs_AddName_OptMap.strPowShape), lengthPowShapeStr, 0);
            }
            
            switch (BasicLPs.typeLepComboAnaType) {
                case 0:
                    if (BasicLPs.typeDilepAna == 0) {
                        GenerateCards_CC_Dilep_SpecMassPoint_MT2CutRange(&BasicLPs, &SUSYMLPs, &SUSYT2LPs, &MT2LPs);
                    }
                    else if (BasicLPs.typeFullSel == 6 || BasicLPs.typeFullSel == 7) {
                        GenerateCards_Shape_Full_MultiDilepChannel(&BasicLPs, &SUSYMLPs, &SUSYT2LPs);
                    }
                    else if (!BasicLPs.splitDilepChannel) {
                        GenerateCards_Shape_Full(&BasicLPs, &SUSYMLPs, &SUSYT2LPs, addName_OptMap, vecTTBarSFToUse);
                    }
                    else {
                        GenerateCards_Shape_Full_SplitDilep(&BasicLPs, &SUSYMLPs, &SUSYT2LPs, &BasicLPs_AddName_OptMap, &SUSYT2LP_Hack, vecTTBarSFToUse, pathCust);
                    }
                    break;
                case 1:
                    GenerateCards_CC_SingLep_SpecMassPoint(&BasicLPs, &SUSYMLPs, &SUSYT2LPs);
                    break;
                case 2:
                    if (BasicLPs.typeDilepAna == 0) {
                        CombineCards_DilepCC_SingLep(&BasicLPs, &SUSYMLPs, &SUSYT2LPs, &MT2LPs);
                    }
                    else {
                        GenerateCards_Shape_LepCombo(&BasicLPs, &SUSYMLPs, &SUSYT2LPs);
                    }
                    /*
                    else if (BasicLPs.typeFullSel == 6 || BasicLPs.typeFullSel == 7) {
                        GenerateCards_Shape_Full_MultiDilepChannel(&BasicLPs, &SUSYMLPs, &SUSYT2LPs);
                    }
                    else if (!BasicLPs.splitDilepChannel) {
                        GenerateCards_Shape_Full(&BasicLPs, &SUSYMLPs, &SUSYT2LPs, addName_OptMap, vecTTBarSFToUse);
                    }
                    else {
                        GenerateCards_Shape_Full_SplitDilep(&BasicLPs, &SUSYMLPs, &SUSYT2LPs, &BasicLPs_AddName_OptMap, &SUSYT2LP_Hack, vecTTBarSFToUse, pathCust);
                    }
                    */
                    break;
                default:
                    break;
            }
        }
    }
    
    
    theApp.Run(retVal);
    //    theApp.Terminate(0);
}
