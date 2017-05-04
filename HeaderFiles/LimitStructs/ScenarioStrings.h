#include "TString.h"

TString GetStop(int whichPol) {
    TString strStop;
    strStop = "#tilde{t}";
    if (whichPol == 0) {
        strStop += "_{R}";
    }
    else if (whichPol == 10) {
        strStop += "_{L}";
    }
    return strStop;
}

TString GetLSP() {
    TString strLSP = "#tilde{#chi}^{0}_{1}";
    return strLSP;
}

TString GetChargino(int whichPol) {
    TString arrChargino[9] = {"#tilde{#chi_{1, L}^{#pm}}", "#tilde{#chi_{1, L}^{#pm}}", "#tilde{#chi_{1, L}^{#pm}}", "#tilde{#chi_{1}^{#pm}}", "#tilde{#chi_{1}^{#pm}}", "#tilde{#chi_{1}^{#pm}}", "#tilde{#chi_{1, R}^{#pm}}", "#tilde{#chi_{1, R}^{#pm}}", "#tilde{#chi_{1, R}^{#pm}}"};
    
    TString strChargino = arrChargino[whichPol];
    return strChargino;
}

TString GetDeltaM(int whichDeltaM) {
    //0 is delta M between stop and LSP
    //1 is deltaM between Stop and chargino
    //2 is delta M between chargino and LSP
    TString outDeltaM = "#Delta M = ";
    if (whichDeltaM < 2) {
        outDeltaM += GetSUSYMass(0);
    }
    else {
        outDeltaM += GetSUSYMass(2);
    }
    outDeltaM += " - ";
    outDeltaM += whichDeltaM != 1 ? GetSUSYMass(1) : GetSUSYMass(2);
    return outDeltaM;
}


TString GetSUSYMass(int whichSUSYParticle) {
    //0 is stop
    //1 is LSP
    //2 is chargino
    TString outMass = "m_{";
    
    if (whichSUSYParticle == 0) {
        outMass += GetStop(-1);
    }
    else if (whichSUSYParticle == 1) {
        outMass += GetLSP();
    }
    else if (whichSUSYParticle == 2) {
        outMass += GetChargino(4);
    }
    outMass += "}";
    return outMass
    return "m_{#tilde{t}}";
}

TString TitleString_T2tt(int whichPol) {
    TString outTitle;
    if (whichPol == -1 || whichPol == 0 || whichPol == 10) {
        outTitle = "#tilde{t} #rightarrow t";

        if (whichPol == 0) {
            outTitle += "^{R}";
        }
        else if (whichPol == 10) {
            outTitle += "^{L}";
        }
        outTitle += " + #tilde{#chi}^{0}_{1}";
    }
    else {
        outTitle = "#tilde{t} #rightarrow t + #tilde{#chi}^{0}_{1}, sin#theta_{eff} = 0.";
        outTitle += whichPol;
    }
    return outTitle;
}

TString TitleString_T2tb() {
    TString outTitle = "#tilde{t} #rightarrow t + #tilde{#chi}^{0}_{1}, #tilde{t} #rightarrow b + #tilde{ #chi}^{#pm}_{1}";
    return outTitle;
}

TString TitleString_T2bw(int whichPol) {
    TString arrT2bwChirCoup[9] = {"#chi_{L}^{#pm}#rightarrow W^{#pm}_{L}", "#chi_{L}^{#pm}#rightarrow W^{#pm}", "#chi_{L}^{#pm}#rightarrow W^{#pm}_{R}", "#chi^{#pm}#rightarrow W^{#pm}_{L}", "#chi^{#pm}#rightarrow W^{#pm}", "#chi^{#pm}#rightarrow W^{#pm}_{R}", "#chi_{R}^{#pm}#rightarrow W^{#pm}_{L}", "#chi_{R}^{#pm}#rightarrow W^{#pm}", "#chi_{R}^{#pm}#rightarrow W^{#pm}_{R}"};
    
    TString outTitle = "#tilde{t} #rightarrow b + #tilde{ #chi}^{#pm}_{1}, ";
    outTitle += arrT2bwChirCoup[whichPol];
    return outTitle;
}

TString T2SMSTitleString(int whichPol, int typeT2) {
    if (typeT2 == 0) {
        return TitleString_T2bw(whichPol);
    }
    else if (typeT2 > 0 && typeT2 < 3) {
        return TitleString_T2tt(whichPol);
    }
    else {
        return TitleString_T2tb();
    }
    return "";
}