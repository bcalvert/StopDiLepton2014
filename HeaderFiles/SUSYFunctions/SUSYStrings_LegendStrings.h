TString LegendString_T2tt(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strComma = ", ";
    
    TString strStopDecayToTop = AddTitle_Top(whichPol, isRealLatex);
    TString outLegend_T2tt = strStopDecayToTop;
    
    TString strHashSin = isRealLatex ? strHash : "";
    if (!(whichPol == -1 || whichPol == 0 || whichPol == 10)) {
        outLegend_T2tt += strComma  + strHashSin + TString("sin") + strHash + TString("left(") + TString("theta_{eff}") + strHash + TString("right) = 0.");
        outLegend_T2tt += whichPol;
    }
    return outLegend_T2tt;
}

TString LegendString_T2bw(int whichPol, bool isRealLatex) {
    TString strHash = GetHashString(isRealLatex);
    TString strComma = ", ";
    TString strStopDecayToBChargino = AddTitle_BChargino(whichPol, isRealLatex, true);
    
    TString outLegend_T2bw = strStopDecayToBChargino;
    
    return outLegend_T2bw;
}