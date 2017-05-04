#ifndef ScenarioDrawingParams_cc
#define ScenarioDrawingParams_cc

typedef pair<float, float> pairFloats;


void ScenarioDrawingParams::SetVectorSize() {
    vecKinematicLines.resize(2);
}

TString ScenarioDrawingParams::SavingName(bool isDir, int isTightBin = false) {
    TString addString = isDir ? "/" : "_";
    
    TString addStringTightBin = isTightBin ? "_TightBin" : "";
    
    TString outStringSaveName = strT2SMS + addStringTightBin + addString;
    if (strT2SMS.Contains("T2bw")) {
        outStringSaveName += strCharFracDir + addString;
    }
    outStringSaveName += strPol;
    if (isDir) outStringSaveName += addString;
    return outStringSaveName;
}
void ScenarioDrawingParams::Print() {
    cout << "strT2SMS " << strT2SMS << endl;
    cout << "strPol " << strPol << endl;
    cout << "strCharFracName " << strCharFracName << endl;
}

void ScenarioDrawingParams::DrawOnCanvas(TCanvas * inputCanvas, bool includeKinematicLines, bool doVerb = false) {
    if (includeKinematicLines) {
        int numKin = (int) vecKinematicLines.size();
        for (int iKin = 0; iKin < numKin; ++iKin) {
            vecKinematicLines[iKin].DrawOnCanvas(inputCanvas, 1, doVerb);
        }
    }
    if (!isHack) {
        latexTitleString_TLatex.DrawCanvas(inputCanvas, 1, doVerb);
    }
    latexSecondTitleString_TLatex.DrawCanvas(inputCanvas, 1, doVerb);
}

/*
 no idea why this is here...
TString GetStringPunziOneBin(int whichVersSig, int whichVersPunzi) {
    int nSigmaA = whichVersPunzi ? 2 : 5;
    int nSigmaB = whichVersPunzi ? 2 : 2;
    vector<TString> vecCheckAddName{"", "Sqrt", "CubeRoot", "TwoFifths"};
    TString outString = "PunziSigmaA";
    outString += nSigmaA;
    outString += "_SigmaB";
    outString += pc_OneBin.nSigmaB;
    outString += "_";
    outString += vecCheckAddName[whichVersSig];
    
    return outString;
}
*/

void ScenarioDrawingParams::SetAsT2tt(int whichPol, float endX = -1, float endY = 300) {
    TString strTitle = TitleString_T2tt(whichPol, false);
    TString strSecondTitle = SecondTitleString_T2tt(whichPol, false);
    strLegend = LegendString_T2tt(whichPol, false);
    
    SetTitleLatexString_LimitPlot(&latexTitleString_TLatex, strTitle);
    SetSecondTitleLatexString_LimitPlot(&latexSecondTitleString_TLatex, strSecondTitle);
    
    latexTitleString_RealLatex = TitleString_T2tt(whichPol, true);
    latexSecondTitleString_RealLatex = SecondTitleString_T2tt(whichPol, true);
    
    strT2SMS = "T2tt";
    TString strT2ttPerPol = "PerPol";
    strT2ttPerPol += whichPol * 10;
    strPol = strT2ttPerPol;
    
    SetKinematicLine_Onshell_Top(&vecKinematicLines[0], endX, endY, isHack);
    SetKinematicLine_Onshell_W_b(&vecKinematicLines[1], binWidth, endX, endY, isHack);
    
    labelXaxis = GetSUSYMass(0, false) + TString(" (GeV)");
    labelYaxis = GetSUSYMass(1, false) + TString(" (GeV)");
    
    hackDeltaMRegion = 175;
}

void ScenarioDrawingParams::SetAsT2bw(int whichPol, int charFrac, float endX = -1, float endY = 300) {
    TString strTitle = TitleString_T2bw(whichPol, false);
    TString strSecondTitle = SecondTitleString_T2bw(charFrac, false);
    strLegend = LegendString_T2bw(whichPol, false);

    SetTitleLatexString_LimitPlot(&latexTitleString_TLatex, strTitle);
    SetSecondTitleLatexString_LimitPlot(&latexSecondTitleString_TLatex, strSecondTitle);
    
    latexTitleString_RealLatex = TitleString_T2bw(whichPol, true);
    latexSecondTitleString_RealLatex = SecondTitleString_T2bw(charFrac, true);
    
    strT2SMS = "T2bw";
    
    TString arrT2bwChirCoup[9] = {"LCharLW", "LCharSW", "LCharRW", "SCharLW", "SCharSW", "SCharRW", "RCharLW", "RCharSW", "RCharRW"};
    
    strPol = arrT2bwChirCoup[whichPol];
    
    strCharFracName = "CharFrac0p";
    strCharFracName += charFrac;
    
    strCharFracDir = "CharFrac0.";
    strCharFracDir += charFrac;
    
    SetKinematicLine_Onshell_W_CharginoDecay(&vecKinematicLines[0], charFrac, endX, endY, isHack);
    SetKinematicLine_Onshell_W_b(&vecKinematicLines[1], binWidth, endX, endY, isHack);
    
    labelXaxis = GetSUSYMass(0, false) + TString(" (GeV)");
    labelYaxis = GetSUSYMass(1, false) + TString(" (GeV)");
    
    hackDeltaMRegion = -1;
}

void ScenarioDrawingParams::SetAsT2tb(int whichPol) {
    TString strTitle = TitleString_T2tb(whichPol, false);
    TString strSecondTitle = SecondTitleString_T2tb(false);
    SetTitleLatexString_LimitPlot(&latexTitleString_TLatex, strTitle);
    SetSecondTitleLatexString_LimitPlot(&latexSecondTitleString_TLatex, strSecondTitle);
    
    latexTitleString_RealLatex = TitleString_T2tb(whichPol, true);
    latexSecondTitleString_RealLatex = SecondTitleString_T2tb(true);
    
    strT2SMS = "T2tb";
    TString strT2tbPerPol = "PerPol";
    strT2tbPerPol += whichPol * 10;
    strPol = strT2tbPerPol;
    
    ///setting this back down to 0 for now
    vecKinematicLines.resize(0);
    
    labelXaxis = GetSUSYMass(0, false) + TString(" (GeV)");
    labelYaxis = GetSUSYMass(1, false) + TString(" (GeV)");
    
    hackDeltaMRegion = -1;
}

#endif