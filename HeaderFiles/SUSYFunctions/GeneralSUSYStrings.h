#include "TString.h"

TString GetHashString(bool isRealLatex) {
    TString strHash = "#";
    if (isRealLatex) strHash += "#";
    return strHash;
}

TString EncapsulateWithTilde(TString inputString) {
    TString strTilde = "tilde{";
    strTilde += inputString;
    strTilde += "}";
    return strTilde;
}

TString GetStop(bool isRealLatex, bool isAnti = false) {
    TString strHash = GetHashString(isRealLatex);
    TString strStop = strHash + EncapsulateWithTilde("t");
    strStop += "_{1}";
    if (isAnti) {
        strStop += "*";
    }
    return strStop;
}

TString GetLSP(bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strLSP = strHash + EncapsulateWithTilde(strHash + TString("chi")) + TString("^{0}_{1}");
    return strLSP;
}

TString GetChargino(int whichPol, bool isRealLatex, int isSpecCharge = 0) {
    TString strHash = GetHashString(isRealLatex);
    TString strChargino = strHash + EncapsulateWithTilde(strHash + TString("chi"));
    
    TString addStringCharginoChirality = "_{1}";;
    if (whichPol < 3) {
        addStringCharginoChirality = "_{1, L}";
    }
    else if (whichPol > 5) {
        addStringCharginoChirality = "_{1, R}";
    }
    strChargino += addStringCharginoChirality;
    strChargino += TString("^{");
    if (isSpecCharge != 0) {
        strChargino += isSpecCharge > 0 ? "+}" : "-}";
    }
    else {
        strChargino +=  + strHash + TString("pm}");
    }
    return strChargino;
}

TString GetTop(int whichPol) {
    TString strTop = "t";
    if (whichPol == 0 || whichPol == 10) {
        if (whichPol == 0) {
            strTop += "_{R}";
        }
        else if (whichPol == 10) {
            strTop += "_{L}";
        }
    }
    return strTop;
}

TString GetW(int whichPol, bool isRealLatex, int isSpecCharge = 0) {
    TString strHash = GetHashString(isRealLatex);
    TString strW = TString("W^{");
    
    if (isSpecCharge != 0) {
        strW += isSpecCharge > 0 ? "+}" : "-}";
    }
    else {
        strW +=  + strHash + TString("pm}");
    }
    
    if (whichPol % 3 == 0) {
        strW += "_{L}";
    }
    else if (whichPol % 3 == 2){
        strW += "_{R}";
    }
    return strW;
}

TString GetSUSYMass(int whichSUSYParticle, bool isRealLatex) {
    //0 is stop
    //1 is LSP
    //2 is chargino
    TString outMass = "m_{";
    
    if (whichSUSYParticle == 0) {
        outMass += GetStop(isRealLatex);
    }
    else if (whichSUSYParticle == 1) {
        outMass += GetLSP(isRealLatex);
    }
    else if (whichSUSYParticle == 2) {
        outMass += GetChargino(4, isRealLatex);
    }
    outMass += "}";
    return outMass;
}


TString GetDeltaM(int whichDeltaM, bool isRealLatex) {
    //0 is delta M between stop and LSP
    //1 is deltaM between Stop and chargino
    //2 is delta M between chargino and LSP
    TString strHash = GetHashString(isRealLatex);
    TString outDeltaM = strHash + TString("Delta M = ");
    if (whichDeltaM < 2) {
        outDeltaM += GetSUSYMass(0, isRealLatex);
    }
    else {
        outDeltaM += GetSUSYMass(2, isRealLatex);
    }
    outDeltaM += " - ";
    outDeltaM += whichDeltaM != 1 ? GetSUSYMass(1, isRealLatex) : GetSUSYMass(2, isRealLatex);
    return outDeltaM;
}

TString GeneralTitle(bool isRealLatex) {
    
    TString strHash = GetHashString(isRealLatex);
    
    TString strPP = "pp ";
    TString strRightArrow = "rightarrow ";
    TString strStop = GetStop(isRealLatex);
    TString strStopAnti = GetStop(isRealLatex, true);
    TString outTitle = strPP + strHash + strRightArrow + strStop + TString(" ") + strStopAnti;
    
    return outTitle;
}

TString AddTitle_Top(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strRightArrow = strHash + "rightarrow ";
    
    TString strStop = GetStop(isRealLatex);
    TString strTop = GetTop(whichPol);
    TString strLSP = GetLSP(isRealLatex);

    TString strStopDecayToTop = strStop + strRightArrow + strTop + TString(" ") + strLSP;

    return strStopDecayToTop;
}

TString AddTitle_BChargino(int whichPol, bool isRealLatex, bool dropStopDecay = false) {
    TString strHash = GetHashString(isRealLatex);
    TString strRightArrow = strHash + "rightarrow ";
    
    TString strStop = GetStop(isRealLatex);
    TString strChargino = GetChargino(whichPol, isRealLatex, 1);
    TString strB = "b";
    TString strW = GetW(whichPol, isRealLatex, 1);
    TString strLSP = GetLSP(isRealLatex);
    
    TString strCharginoDecayToW = strChargino + strRightArrow + strW + TString(" ") + strLSP;
    TString strStopDecayToBChargino = strStop + strRightArrow + strB + TString(" ") + strChargino + TString(", ") + strCharginoDecayToW;
    if (dropStopDecay) {
        strStopDecayToBChargino = strChargino + TString(", ") + strCharginoDecayToW;
    }
    return strStopDecayToBChargino;
}

