#include "TString.h"
#include "TCanvas.h"
#include "TLatex.h"

#include <iostream>

using namespace std;

typedef pair<int, int> vPP;

int DeltaMValue(int whichDeltaM) {
    int baseVal = 100;
    return baseVal + whichDeltaM * 100;
}
int DeltaMPrecision(int whichDeltaMPrecision) {
    return whichDeltaMPrecision ? 25 : 15;
}
TString MStopCutString(vPP * massStop) {
    TString baseStringCutMStop = " && (TMath::Abs(TGenStop_Mass_0 - ";
    baseStringCutMStop += massStop->first;
    baseStringCutMStop += ") < ";
    baseStringCutMStop += massStop->second;
    baseStringCutMStop += ")";
    return baseStringCutMStop;
}
TString MStopPlotString(vPP * massStop, bool isLatex = false) {
    TString baseStringMStop = isLatex ? "$##msTop$ = " : "m_{#tilde{t}_{1}} = ";
    TString midStringMStop = isLatex ? " $##pm$ " : " #pm ";
    TString endStringMStop = isLatex ? " $##GeV$" : " GeV";
    
    TString outString = baseStringMStop;
    outString += massStop->first;
    outString += midStringMStop;
    outString += massStop->second;
    outString += endStringMStop;
    
    return outString;
}
TString DeltaMCutString(vPP * DeltaM) {
    TString baseStringCutDeltaM = " && (TMath::Abs(TGenStop_Mass_0-TGenChi0_Mass_0 - ";
    baseStringCutDeltaM += DeltaM->first;
    baseStringCutDeltaM += ") < ";
    baseStringCutDeltaM += DeltaM->second;
    baseStringCutDeltaM += ")";
    return baseStringCutDeltaM;
}
TString DeltaMPlotString(vPP * DeltaM, bool isLatex = false) {
    TString baseStringDeltaM = isLatex ? "$##dm$ = " : "#DeltaM = ";
    TString midStringDeltaM = isLatex ? " $##pm$ " : " #pm ";
    TString endStringDeltaM = isLatex ? " $##GeV$" : " GeV";
    
    TString outString = baseStringDeltaM;
    outString += DeltaM->first;
    outString += midStringDeltaM;
    outString += DeltaM->second;
    outString += endStringDeltaM;
    
    return outString;
}
TString axisStringMT2Var(int whichMT2Var) {
    const int numMT2Vars = 4;
    TString arrMT2Names[numMT2Vars] = {"M_{T2}(ll) [GeV]   ", "M_{T2}(lb)(lb) [GeV]   ", "M_{T2}^{#slash{W}}(bb) [GeV]   ", "#kappa_{T2} [GeV]   "};
    return arrMT2Names[whichMT2Var];
}

TString T2String(int whichT2SMS, int addtlParam = -1) {
    TString baseString = "";
    
    if (whichT2SMS == 0) {        
//        baseString = "#tilde{t}_{1}#tilde{t}_{1}*, #tilde{t}_{1} #rightarrow b + #chi_{#pm}, x = 0.";
        baseString = "#tilde{t}_{1} #rightarrow b + #chi^{+}_{1}, x = 0.";
        baseString += addtlParam;
    }
    else if (whichT2SMS > 0 && whichT2SMS < 3) {
        baseString = "#tilde{t}_{1} #rightarrow t + #chi^{0}_{1}";
    }
    else if (whichT2SMS == 3) {
        baseString = "#tilde{t}_{1} #rightarrow t + #chi^{0}_{1}, #tilde{t}_{1} #rightarrow b + #chi_{1}^{+}";
    }
    else {
        baseString = "t#bar{t}";
    }
    return baseString;
}

Double_t LatexXVal(int whichString, int whichCanvType) {
    vector<Double_t> vecXVals_SigString{0.04, 0.04};
    vector<Double_t> vecXVals_DeltaMStopString{0.02, 0.02};
    vector<Double_t> vecXVals_CMSSimulation{0.65, 0.6375};
    vector<vector<Double_t> > vecVecXVals;
    vecVecXVals.push_back(vecXVals_SigString);
    vecVecXVals.push_back(vecXVals_DeltaMStopString);
    vecVecXVals.push_back(vecXVals_CMSSimulation);
    return vecVecXVals[whichString][whichCanvType];
}

Double_t LatexYVal(int whichString, int whichCanvType) {
    vector<Double_t> vecYVals_SigString{0.925, 0.925};
    vector<Double_t> vecYVals_DeltaMStopString{0.1, 0.1};
    vector<Double_t> vecYVals_CMSSimulation{0.025, 0.92};
    vector<vector<Double_t> > vecVecYVals;
    vecVecYVals.push_back(vecYVals_SigString);
    vecVecYVals.push_back(vecYVals_DeltaMStopString);
    vecVecYVals.push_back(vecYVals_CMSSimulation);
    return vecVecYVals[whichString][whichCanvType];
}

void SigStringDraw(TCanvas * inputCanv, TString drawString, int whichCanvType) {
    inputCanv->cd();
    float size = 0.05;
    TLatex textSigString; textSigString.SetNDC();
    textSigString.SetTextSize(size);
    textSigString.DrawLatex(LatexXVal(0, whichCanvType), LatexYVal(0, whichCanvType), drawString);
    inputCanv->Update();
    inputCanv->Modified();
}

void DrawStrings(TCanvas * inputCanv, int whichCanvType, TString drawString, vPP * deltaM = NULL) {
    inputCanv->cd();
    SigStringDraw(inputCanv, drawString, whichCanvType);
    float size = 0.05;
    TLatex textCMSPrelim; textCMSPrelim.SetNDC();
    textCMSPrelim.SetTextSize(size);
    textCMSPrelim.SetTextFont(42);
    cout << "whichCanvType " << whichCanvType << endl;
    cout << "LatexXVal(2, whichCanvType) " << LatexXVal(2, whichCanvType) << endl;
    cout << "LatexYVal(2, whichCanvType) " << LatexYVal(2, whichCanvType) << endl;
    textCMSPrelim.DrawLatex(LatexXVal(2, whichCanvType), LatexYVal(2, whichCanvType), "#bf{CMS} #it{Simulation}");
    if (deltaM) {
        TLatex deltaMLatex; deltaMLatex.SetNDC();
        deltaMLatex.SetTextSize(size);
        deltaMLatex.DrawLatex(LatexXVal(1, whichCanvType), LatexYVal(1, whichCanvType), "#DeltaM = m_{#tilde{t}_{1}} - m_{#chi_{0}}");
        deltaMLatex.DrawLatex(LatexXVal(1, whichCanvType), LatexYVal(1, whichCanvType) - 0.07, DeltaMPlotString(deltaM));

    }
    inputCanv->Update();
    inputCanv->Modified();
}

/*
TString T2String(int whichT2SMS, int addtlParam = -1) {
    TString baseString = "";
    
    if (whichT2SMS == 0) {        
        baseString = "#tilde{t}_{1}#tilde{t}_{1}*, #tilde{t}_{1} #rightarrow b + #chi_{#pm}, x = 0.";
        baseString += addtlParam;
    }
    else if (whichT2SMS < 3) {
        baseString = "#tilde{t}_{1}#tilde{t}_{1}*, #tilde{t}_{1} #rightarrow t + #chi_{0}";
    }
    else {
        baseString = "#tilde{t}_{1}#tilde{t}_{1}*, #tilde{t}_{1} #rightarrow t + #chi_{0}, #tilde{t}_{1} #rightarrow b + #chi_{#pm}";
    }
    return baseString;
}
*/
