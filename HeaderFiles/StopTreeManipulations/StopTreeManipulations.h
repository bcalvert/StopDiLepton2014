// Set of functions used as part of setting up input and output branch addresses 
// for the various TTrees used as part of making plots

inline TString SystString(int whichSyst = 0) {
    TString suffixSyst[7] = {"", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "JetSmear", "UncES"};
    TString outString = "";
    if (whichSyst == 0) {
        return outString;
    }
    else {
        outString = "_";
        outString += suffixSyst[abs(whichSyst)];
        if (whichSyst < 0) outString += "Down";
        else if (whichSyst > 0) outString += "Up";
    }
    return outString;
}

inline TString SystString_v2(int whichSyst = 0) {
    TString suffixSyst[8] = {"", "ElectronEn", "JetEn", "BTagEffSF", "BMisTagSF", "JetRes", "UnclusteredEn", "MuonEn"};
    TString outString = "";
    if (whichSyst == 0) {
        return outString;
    }
    else {
        outString = "";
        outString += suffixSyst[abs(whichSyst)];
        if (whichSyst < 0) outString += "Down";
        else if (whichSyst > 0) outString += "Up";
    }
    return outString;
}
inline TString SystString_v3(int whichSyst = 0) {
  TString suffixSyst[10] = {"", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "JetSmear", "UncES", "LepEffSF", "genRecoilRW", "genStopXSec"};
  TString outString = "";
  if (whichSyst == 0) {
    return outString;
  }
  else {
    outString = "_";
    outString += suffixSyst[abs(whichSyst)];
    outString += "Shift";
    if (whichSyst < 0) outString += "Down";
    else if (whichSyst > 0) outString += "Up";
  }
  return outString;
}

inline TString SystString_v4(int whichSyst = 0) {
  TString suffixSyst[10] = {"", "LepES", "JetES", "BTagEffSF", "BMisTagSF", "JetSmear", "UncES", "LepEffSF", "genRecoilRW", "genStopXSec"};
  ////    cout << "suffixSyst[7] " << suffixSyst[7] << endl;
  //    cout << "suffixSyst[8] " << suffixSyst[8] << endl;
  //    cout << "suffixSyst[9] " << suffixSyst[9] << endl;
  TString outString = "";
  if (whichSyst == 0) {
    return outString;
  }
  else {
    outString = "_";
    outString += suffixSyst[abs(whichSyst)];
    outString += "Shift";
    if (whichSyst < 0) outString += "Down";
    else if (whichSyst > 0) outString += "Up";
  }
  return outString;
}


inline void SetDiStructureBranch(TTree * outTree, DiParticle * DP_ToUse, TString prefixBranchName, TString appendStringSyst, bool doKinVars = true) {
  if (doKinVars) {
    outTree->Branch(prefixBranchName + TString("Pt")   + appendStringSyst, &DP_ToUse->BVC.Vec_Pt);
    outTree->Branch(prefixBranchName + TString("Eta")  + appendStringSyst, &DP_ToUse->BVC.Vec_Eta);
    outTree->Branch(prefixBranchName + TString("Mass") + appendStringSyst, &DP_ToUse->BVC.Vec_Mass);
    outTree->Branch(prefixBranchName + TString("Phi")  + appendStringSyst, &DP_ToUse->BVC.Vec_Phi);
  }
  outTree->Branch(prefixBranchName + TString("_IndObjDPhi")  + appendStringSyst, &DP_ToUse->DPhiInputObjects);    
}

inline void SetDiStructureBranchAddress(TTree * outTree, DiParticle * DP_ToUse, TString prefixBranchName, TString appendStringSyst, bool doKinVars = true) {
  if (doKinVars) {
    outTree->SetBranchAddress(prefixBranchName + TString("Pt")   + appendStringSyst, &DP_ToUse->BVC.Vec_Pt);
    outTree->SetBranchAddress(prefixBranchName + TString("Eta")  + appendStringSyst, &DP_ToUse->BVC.Vec_Eta);
    outTree->SetBranchAddress(prefixBranchName + TString("Mass") + appendStringSyst, &DP_ToUse->BVC.Vec_Mass);
    outTree->SetBranchAddress(prefixBranchName + TString("Phi")  + appendStringSyst, &DP_ToUse->BVC.Vec_Phi);
  }
  outTree->SetBranchAddress(prefixBranchName + TString("_IndObjDPhi")  + appendStringSyst, &DP_ToUse->DPhiInputObjects);    
}


#include "StopOutTreeManipulations.h"
#include "StopInTreeManipulations_Skimmer.h"
#include "StopInTreeManipulations_PlotMaker.h"
