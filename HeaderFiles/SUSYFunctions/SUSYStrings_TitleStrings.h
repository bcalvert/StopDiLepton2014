TString TitleString_T2tt(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strComma = ", ";
    
    TString strStopDecayToTop = AddTitle_Top(whichPol, isRealLatex);
    TString outTitle_T2tt = GeneralTitle(isRealLatex) + strComma + strStopDecayToTop;
    
    TString strHashSin = isRealLatex ? strHash : "";
    if (!(whichPol == -1 || whichPol == 0 || whichPol == 10)) {
        outTitle_T2tt += strComma  + strHashSin + TString("sin") + strHash + TString("left(") + TString("theta_{eff}") + strHash + TString("right) = 0.");
        outTitle_T2tt += whichPol;
    }
    return outTitle_T2tt;
}

TString TitleString_T2bw(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strComma = ", ";
    TString strStopDecayToBChargino = AddTitle_BChargino(whichPol, isRealLatex);
    
    TString outTitle_T2bw = GeneralTitle(isRealLatex) + strComma + strStopDecayToBChargino;
    
    return outTitle_T2bw;
}

TString TitleString_T2tb(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strComma = ", ";
    TString strStopDecayToTop = AddTitle_Top(whichPol, isRealLatex);
    TString strStopDecayToBChargino = AddTitle_BChargino(whichPol, isRealLatex);
    
    TString outTitle_T2tb = GeneralTitle(isRealLatex) + strComma + strStopDecayToTop + strComma + strStopDecayToBChargino;
    
    return outTitle_T2tb;
}


void SetTitleLatexString_LimitPlot(LatexString * inLS, TString inputTitleString) {
    float sizeModel = 0.040;
    int alignModel = 13;
    float posX_Model = 0.16;
    float posY_Model = 0.90;
    int fontModel = 42;
    
    inLS->SetLatex(true, sizeModel, alignModel, posX_Model, posY_Model, inputTitleString);
    inLS->straightGhetto.textFont = fontModel;
}