TString SecondTitleString_T2tt(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strComma = ", ";
    
    TString strSin = isRealLatex ? "##sin" : "sin";
    TString strThetaEff = "theta_{eff} = 0.";
    TString outSecondTitle_T2tt = strSin + strHash + TString("left(") + strThetaEff + strHash + TString("right)");
    outSecondTitle_T2tt += whichPol;
    
    if (whichPol == -1 || whichPol == 0 || whichPol == 10) {
        outSecondTitle_T2tt = "";
    }
    return outSecondTitle_T2tt;
}

TString SecondTitleString_T2bw(int charFrac, bool isRealLatex) {
    TString strCharFrac = "0.";
    TString strOneMinusCharFrac = "0.";
    
    TString strMassStop = GetSUSYMass(0, isRealLatex);
    TString strMassLSP = GetSUSYMass(1, isRealLatex);
    TString strMassChargino = GetSUSYMass(2, isRealLatex);
    
    strCharFrac += charFrac;
    strOneMinusCharFrac += 100 - charFrac;
    
    TString outSecondTitle_T2bw = strMassChargino + TString(" = ") + strCharFrac + + TString(" ") + strMassStop + TString (" + ") + strOneMinusCharFrac + TString(" ") + strMassLSP;
    
    return outSecondTitle_T2bw;
}

TString SecondTitleString_T2tb(bool isRealLatex) {    
    TString strMassStop = GetSUSYMass(0, isRealLatex);
    TString strMassLSP = GetSUSYMass(1, isRealLatex);
    TString strMassChargino = GetSUSYMass(2, isRealLatex);
    
    TString outSecondTitle_T2tb = strMassChargino + TString(" = ") + strMassLSP + TString (" + 5 GeV");
    
    return outSecondTitle_T2tb;
}

void SetSecondTitleLatexString_LimitPlot(LatexString * inLS, TString inputTitleString) {
    float sizeModel = 0.040;
    int alignModel = 13;
    float posX_Model = 0.15;
    float posY_Model = 0.075;
    int fontModel = 42;
    
    inLS->SetLatex(true, sizeModel, alignModel, posX_Model, posY_Model, inputTitleString);
    inLS->straightGhetto.textFont = fontModel;
}