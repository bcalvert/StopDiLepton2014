// Set of functions utilized as part of filling plots and showing them
// These functions handle setting up strings, whether for systematics' names or for axis labels, among other things


using namespace std;

#include "./Typedefs.h"

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


inline TString GetStringLead(int whichPos, bool isLabelString) {
    if (whichPos < 0 || whichPos > 2) {
        cout << "something wrong with whichPOS " << endl;
        return "BADBADBAD";
    }
    TString stringLead[3] = {"Lead", "Sub", "SubSub"};
    TString stringLeadLabel[3] = {"Lead ", "Sub-Lead ", "Sub-Sub-Lead "};
    
    if (isLabelString) {
        return stringLeadLabel[whichPos];
    }
    return stringLead[whichPos];
}
inline TString GetStringSmear(bool isSmear, bool isLabelString) {
    TString stringSmear = isSmear ? "Smear" : "";
    TString stringSmearLabel = isSmear ? "Smeared " : "";
    if (isLabelString) {
        return stringSmearLabel;
    }
    return stringSmear;
}

inline TString GetStringIndObj(int typeObj, bool isLabelString) {
    TString stringObj, stringObjLabel;   
    switch (typeObj) {
        case 0:
            //lepton
            stringObj = "Lep";
            stringObjLabel = "Lepton ";
            break;
        case 1:
            //jet
            stringObj = "Jet";
            stringObjLabel = "Jet ";
            break;
        case 2:
            //bjet
            stringObj = "BJet";
            stringObjLabel = "B-Jet ";            
            break;
        case 3:
            //blep
            stringObj = "BLep";
            stringObjLabel = "'B+Lepton' ";            
            break;
        case 4:
            //Z
            stringObj = "Z";
            stringObjLabel = "'Z' ";            
            break;
        case 5:
            //MET
            stringObj = "MET";
            stringObjLabel = "#slash{E}_{T} ";
            break;
        default:
            cout << "something weird with picking object type " << typeObj << endl;
            break;
    }
    
    if (isLabelString) {
        return stringObjLabel;
    }
    return stringObj;
}
inline TString DPhiVarString(int typeObj1, int typeObj2, int posObj1 = -1, int posObj2 = -1) {
    const int numObjs = 6;
    if (typeObj1 < 0 || typeObj1 >= numObjs) {
        cout << "something wrong when trying to set DPhiLabelString for first object -- type is " << typeObj1 << endl;
    }
    if (typeObj1 < 0 || typeObj2 >= numObjs) {
        cout << "something wrong when trying to set DPhiLabelString for second object -- type is " << typeObj2 << endl;
    }
    
    TString baseDPhiString = "DPhi";
    TString stringDPhiZMETBaseString = baseDPhiString + "ZMET";
    TString stringDPhiLep0Lep1BaseString = baseDPhiString + "Lep0Lep1";
    TString stringDPhiBLep0BLep1BaseString = baseDPhiString + "BLep0BLep1";
    TString stringDPhiJet0Jet1BaseString = baseDPhiString + "Jet0Jet1";
    TString stringDPhiBJet0BJet1BaseString = baseDPhiString + "BJet0BJet1";
    
    TString stringDiObj[4];
    stringDiObj[0] = stringDPhiLep0Lep1BaseString;
    stringDiObj[1] = stringDPhiJet0Jet1BaseString;
    stringDiObj[2] = stringDPhiBJet0BJet1BaseString;
    stringDiObj[3] = stringDPhiBLep0BLep1BaseString;
    
    if (typeObj1 == typeObj2) {
        if (typeObj1 > 3) cout << "doesn't make sense to have same object here! typeObj is " << typeObj1 << endl;
        return stringDiObj[typeObj1];
    }
    if (typeObj1 + typeObj2 == 9) {
        //is ZMET
        return stringDPhiZMETBaseString;
    }    
    TString nameObj1 = GetStringIndObj(typeObj1, false);
    if (posObj1 > -1) {
        nameObj1 += posObj1;
    }
    TString nameObj2 = GetStringIndObj(typeObj2, false);
    if (posObj2 > -1) {
        nameObj2 += posObj2;
    }    
    return baseDPhiString + nameObj1 + nameObj2;
}
inline TString DPhiLabelString(int typeObj1, int typeObj2, int posObj1 = -1, int posObj2 = -1) {
    const int numObjs = 6;
    if (typeObj1 < 0 || typeObj1 >= numObjs) {
        cout << "something wrong when trying to set DPhiLabelString for first object -- type is " << typeObj1 << endl;
    }
    if (typeObj1 < 0 || typeObj2 >= numObjs) {
        cout << "something wrong when trying to set DPhiLabelString for second object -- type is " << typeObj2 << endl;
    }
    TString labelObj[numObjs] = {"l", "j", "b", "(b+l)", "Z", "#slash{E}_{T}"};
    
    TString basicDPhi = "#Delta #phi_{";
    
    TString objDPhi = labelObj[typeObj1];
    if (posObj1 > -1) {        
        objDPhi += "_{";
        objDPhi += posObj1;
        objDPhi += "}";
    }
    objDPhi += ", ";
    objDPhi += labelObj[typeObj2];
    if (posObj2 > -1) {        
        objDPhi += "_{";
        objDPhi += posObj2;
        objDPhi += "}";
    }
    basicDPhi += objDPhi;
    basicDPhi += "}";
    
    return basicDPhi;
}
inline TString DPhiString(int typeObj1, int typeObj2, int posObj1 = -1, int posObj2 = -1, bool returnVarString = true) {
    return returnVarString ? DPhiVarString(typeObj1, typeObj2, posObj1, posObj2) : DPhiLabelString(typeObj1, typeObj2, posObj1, posObj2);
}


inline void SetStringKey_StSMap_Misc(labelMap &mapVartoLabel) {
    mapVartoLabel["CS"] = "Cut Stage";
}

inline void SetStringKey_StSMap_Leptons(labelMap &mapVartoLabel, int whichPos) {
    TString stringLead = GetStringLead(whichPos, false);
    TString stringLeadLabel = GetStringLead(whichPos, true);
    
    mapVartoLabel[stringLead + "LepIso"] = stringLeadLabel + "Lepton Relative PF Isolation";
}

inline void SetStringKey_StSMap_IndObjects(labelMap &mapVartoLabel, int typeObj, int whichPos, bool isSmear) {
    TString stringLead = GetStringLead(whichPos, false);
    TString stringLeadLabel = GetStringLead(whichPos, true);    
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringSmearLabel = GetStringSmear(isSmear, true);
    TString stringObj = GetStringIndObj(typeObj, false);
    TString stringObjLabel = GetStringIndObj(typeObj, true);
    
    mapVartoLabel[stringSmear + stringLead + stringObj + "Pt"] = stringLeadLabel + stringSmearLabel + stringObjLabel + TString("p_{T} [GeV]");
    mapVartoLabel[stringSmear + stringLead + stringObj + "En"] = stringLeadLabel + stringSmearLabel + stringObjLabel + TString("Energy [GeV]");
    mapVartoLabel[stringSmear + stringLead + stringObj + "Eta"] = stringLeadLabel + stringSmearLabel + stringObjLabel + TString("#eta");
}

inline void SetStringKey_StSMap_DiObjects(labelMap &mapVartoLabel, int typeObj, bool isSmear) {
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringSmearLabel = GetStringSmear(isSmear, true);
    TString stringObj = GetStringIndObj(typeObj, false);
    TString stringObjLabel = GetStringIndObj(typeObj, true);
    
    mapVartoLabel[stringSmear + "Di" + stringObj + "Pt"] =  stringSmearLabel + "Di" + stringObjLabel + TString("p_{T} [GeV]");
    mapVartoLabel[stringSmear + "Di" + stringObj + "InvMass"] =  stringSmearLabel + "Di" + stringObjLabel + TString("Inv. Mass [GeV]");
    mapVartoLabel[stringSmear + "Di" + stringObj + "Eta"] =  stringSmearLabel + "Di" + stringObjLabel + TString("#eta");
    mapVartoLabel[stringSmear + "Di" + stringObj + "Phi"] =  stringSmearLabel + "Di" + stringObjLabel + TString("#phi");
}

inline void SetStringKey_StSMap_MET(labelMap &mapVartoLabel, bool isSmear) {
    TString stringSmear = GetStringSmear(isSmear, false);
    TString stringSmearLabel = GetStringSmear(isSmear, true);
    
    TString stringMT2ll = "M_{T2}(ll)";
    TString stringMT2lblb = "M_{T2}(lb)(lb)";
    TString stringMT2bb_ZMET = "M_{T2}^{#slash{W}}(bb)";
    TString stringKT2_Top = "#kappa_{T2}^{top}";
    
    mapVartoLabel["RawMETSig"]    = "RawMET Significance";
    
    mapVartoLabel[stringSmear + "MT2ll"]        = stringSmearLabel + stringMT2ll + " [GeV]";
    mapVartoLabel[stringSmear + "MT2ll80"]      = stringSmearLabel + stringMT2ll + " > 80 GeV";
    mapVartoLabel[stringSmear + "MT2ll90"]      = stringSmearLabel + stringMT2ll + " > 90 GeV";
    mapVartoLabel[stringSmear + "MT2ll100"]     = stringSmearLabel + stringMT2ll + " > 100 GeV";
    mapVartoLabel[stringSmear + "MT2ll110"]     = stringSmearLabel + stringMT2ll + " > 110 GeV";
    mapVartoLabel[stringSmear + "MT2ll120"]     = stringSmearLabel + stringMT2ll + " > 120 GeV";
    mapVartoLabel[stringSmear + "MT2lblb"]      = stringSmearLabel + stringMT2lblb + " [GeV]";
    mapVartoLabel[stringSmear + "MT2bb_ZMET"]   = stringSmearLabel + stringMT2bb_ZMET + " [GeV]";
    mapVartoLabel[stringSmear + "KT2_Top"]   = stringSmearLabel + stringKT2_Top + " [GeV]";
    
    mapVartoLabel[stringSmear + "MET"]          = stringSmearLabel + "#slash{E}_{T} [GeV]";
    mapVartoLabel[stringSmear + "METX"]         = stringSmearLabel + "#slash{E}_{x} [GeV]";
    mapVartoLabel[stringSmear + "METY"]         = stringSmearLabel + "#slash{E}_{y} [GeV]";
    mapVartoLabel[stringSmear + "METPhi"]       = stringSmearLabel + "#slash{E}_{T} #phi [rad]";
    
    mapVartoLabel[stringSmear + "UPar"]         = stringSmearLabel + "u_{#parallel} [GeV]";
    mapVartoLabel[stringSmear + "UParPlusqT"]   = stringSmearLabel + "u_{#parallel} + q_{T}^{Z} [GeV]";
    mapVartoLabel[stringSmear + "UPerp"]        = stringSmearLabel + "u_{#perp}  [GeV]";
    mapVartoLabel["SumEt"] = "#Sigma E_{T} [GeV]";
    mapVartoLabel["SumEtSubqT"] = "#Sigma E_{T} - q_{T}^{Z} [GeV]";
}

inline void SetStringKey_StSMap_Basic(labelMap &mapVartoLabel, bool isSmear) {
    TString stringSmear = GetStringSmear(isSmear, false);    
    TString stringSmearLabel = GetStringSmear(isSmear, true);
    mapVartoLabel["nVtx"] = "N_{vtx}^{reco}";
    mapVartoLabel[stringSmear + "NJets"] = TString("N_{") + stringSmearLabel + TString("jets}");
    mapVartoLabel[stringSmear + "NBJets"] = TString("N_{") + stringSmearLabel + TString("b-jets}");
    mapVartoLabel[stringSmear + "HT"] = "H_{T} [GeV]";
    
    mapVartoLabel[stringSmear + "VecHT"] = "#vec{H}_{T} [GeV]";
}
inline void SetStringKey_StSMap_DPhis(labelMap &mapVartoLabel) {
    
    int numSavedLeps = 2;    
    int numSavedJets = 2;    
    int numSavedBJets = 2;   
    
    int intLep  = 0;
    int intJet  = 1;
    int intBJet = 2;
    int intBLep = 3;
    int intZ    = 4;
    int intMET  = 5;
    
    // from function, DPhiLabelString(int typeObj1, int typeObj2, int posObj1 = -1, int posObj2 = -1),  
    // TString labelObj[numObjs] = {"l", "j", "b", "(b+l)", "Z", "#slash{E}_{T}"};
    
    mapVartoLabel[DPhiVarString(intZ, intMET)] = DPhiLabelString(intZ, intMET);
    mapVartoLabel[DPhiVarString(intLep, intLep)] = DPhiLabelString(intLep, intLep, 0, 1);
    mapVartoLabel[DPhiVarString(intBLep, intBLep)] = DPhiLabelString(intBLep, intBLep, 0, 1);
    
    mapVartoLabel[DPhiVarString(intJet, intJet)] = DPhiLabelString(intJet, intJet, 0, 1);
    mapVartoLabel[DPhiVarString(intBJet, intBJet)] = DPhiLabelString(intBJet, intBJet, 0, 1); 
    
    for (int iLep = 0; iLep < numSavedLeps; ++iLep) {
        mapVartoLabel[DPhiVarString(intLep, intMET, iLep, -1)] = DPhiLabelString(intLep, intMET, iLep, -1);
        for (int iJet = 0; iJet < numSavedJets; ++iJet) {
            mapVartoLabel[DPhiVarString(intLep, intJet, iLep, iJet)] = DPhiLabelString(intLep, intJet, iLep, iJet);
        }
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            mapVartoLabel[DPhiVarString(intLep, intBJet, iLep, iBJet)] = DPhiLabelString(intLep, intBJet, iLep, iBJet);
        }
    }    
    for (int iJet = 0; iJet < numSavedJets; ++iJet) {
        for (int iBJet = 0; iBJet < numSavedBJets; ++iBJet) {
            mapVartoLabel[DPhiVarString(intJet, intBJet, iJet, iBJet)] = DPhiLabelString(intJet, intBJet, iJet, iBJet);
        }
    }    
}
inline void SetStringKey_StSMap_Composite(labelMap &mapVartoLabel) {
    SetStringKey_StSMap_Misc(mapVartoLabel);
    SetStringKey_StSMap_DPhis(mapVartoLabel);
    int numObjs = 3;        
    SetStringKey_StSMap_Leptons(mapVartoLabel, true);
    SetStringKey_StSMap_Leptons(mapVartoLabel, false);
    for (int iSmear = 0; iSmear < 2; ++iSmear) {
        SetStringKey_StSMap_MET(mapVartoLabel, iSmear);
        SetStringKey_StSMap_Basic(mapVartoLabel, iSmear);
        for (int iObj = 0; iObj < numObjs; ++iObj) {
            SetStringKey_StSMap_DiObjects(mapVartoLabel, iObj, iSmear);
            for (int iLead = 0; iLead < 2; ++iLead) {    
                SetStringKey_StSMap_IndObjects(mapVartoLabel, iObj, iLead, iSmear);
            }
        }                
    }
}
