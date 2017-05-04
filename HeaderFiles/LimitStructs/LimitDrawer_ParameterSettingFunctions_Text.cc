void LimitDrawer::SetText_LegHack(bool isObs) {
    TString strLegHack = isObs ? "Observed #pm 1 #sigma_{theory}" : "Expected #pm 1 #sigma_{experiment}";
    if (isObs) {
        latexObsHack = TLatex(latexObsHack_X, latexObsHack_Y, strLegHack);
    }
    else {
        latexExpHack = TLatex(latexExpHack_X, latexExpHack_Y, strLegHack);
    }
}


void LimitDrawer::SetText_CMS() {
    char * buf[300];
    
    sprintf(buf," %0.4f fb^{-1}, #sqrt{s} = %0.4f TeV", intLumi, beamEnergy);
    TString strCMS = "CMS";
    strCMS += textPrelim;
    strCMS += ", " + TString(buf.c_str());
    
    latexCMS = TLatex(latexCMS_X, latexCMS_Y, strCMS);
    latexCMS.SetNDC()
    latexCMS.SetTextAlign(13)
    latexCMS.SetTextFont(42)
    latexCMS.SetTextSize(0.038)
}

void LimitDrawer::SetText_Model() {
    char * buf[300];
    
    TString strModel = labelModel + TString(" #scale[0.7]{ }");
    latexModel = TLatex(latexModel_X, latexModel_Y, strModel);
    latexModel.SetNDC()
    latexModel.SetTextAlign(13)
    latexModel.SetTextFont(42)
    latexModel.SetTextSize(0.040)
}

void LimitDrawer::SetText_WhiteBackground() {
    
    params_WhiteBackgroundGraph.SetAsWhiteBackgroundGraphParams();
    graph_WhiteBackground = TGraph(5);
    
    params_WhiteBackgroundGraph.SetInputTGraph(&graph_WhiteBackground);
    graph_WhiteBackground.SetPoint(0, Xmin, Ymax);
    graph_WhiteBackground.SetPoint(0, Xmax, Ymax);
    graph_WhiteBackground.SetPoint(0, Xmax, Ymax * 0.75);
    graph_WhiteBackground.SetPoint(0, Xmin, Ymax * 0.75);
    graph_WhiteBackground.SetPoint(0, Xmin, Ymax);
}

void LimitDrawer::SetText_All() {
    SetText_WhiteBackground();
    SetText_CMS();
    SetText_Model();
    
    SetText_LegHack(true);
    SetText_LegHack(false);
}
