#ifndef CanvasParams_cc
#define CanvasParams_cc
void CanvasParams::SetCanvas(TCanvas * inputCanvas) {
    //inputCanvas->SetCanvasSize(canvWidth, canvHeight);
    
    inputCanvas->SetName(canvName);
    inputCanvas->SetTitle(canvTitle);
    
    inputCanvas->SetLogz(doLogZ);
    inputCanvas->SetLogz(doLogY);
    inputCanvas->SetLogz(doLogZ);
    
    inputCanvas->SetTickx(doTickX);
    inputCanvas->SetTickx(doTickY);
    
    inputCanvas->SetLeftMargin(leftMargin);
    inputCanvas->SetRightMargin(rightMargin);
    inputCanvas->SetTopMargin(topMargin);
    inputCanvas->SetBottomMargin(botMargin);
}

void CanvasParams::SetEmptyHistogram(TH1 * inputHist) {
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    TString className = inputHist->Class_Name();
    if (className.Contains("TH3")) {
        emptyHistogram = new TH3F(histName + TString("_EMPTY"), TString(inputHist->GetTitle()) + TString("_EMPTY"), vecInputHistAxisNBins[0], vecInputHistAxes[0]->GetXmin(), vecInputHistAxes[0]->GetXmax(), vecInputHistAxisNBins[1], vecInputHistAxes[1]->GetXmin(), vecInputHistAxes[1]->GetXmax(), vecInputHistAxisNBins[2], vecInputHistAxes[2]->GetXmin(), vecInputHistAxes[2]->GetXmax());
    }
    else if (className.Contains("TH2")) {
        emptyHistogram = new TH2F(histName + TString("_EMPTY"), TString(inputHist->GetTitle()) + TString("_EMPTY"), vecInputHistAxisNBins[0], vecInputHistAxes[0]->GetXmin(), vecInputHistAxes[0]->GetXmax(), vecInputHistAxisNBins[1], vecInputHistAxes[1]->GetXmin(), vecInputHistAxes[1]->GetXmax());
    }
    else if (className.Contains("TH1")) {
        emptyHistogram = new TH1F(histName + TString("_EMPTY"), TString(inputHist->GetTitle()) + TString("_EMPTY"), vecInputHistAxisNBins[0], vecInputHistAxes[0]->GetXmin(), vecInputHistAxes[0]->GetXmax());
    }
    else {
        cout << "not a 1, 2, or 3-dim histo?!" << endl;
    }
}

void CanvasParams::DrawEmptyHisto(TCanvas * inputCanvas) {
    inputCanvas->cd();
    emptyHistogram->Draw("colz");
}

void CanvasParams::SetAsLimitCanvas(TString inputTitle_X, TString inputTitle_Y, TString inputTitle_Z, bool isSignif = false) {
    doLogZ = true;
    if (isSignif) {
        doLogZ = false;
    }
    doTickX = true;
    doTickY = true;
    
    canvHeight = 800;
    canvWidth = 900;
    
    leftMargin = 0.153;
    rightMargin = 0.19;
    botMargin = 0.14;
    topMargin = 0.08;
    
    params_EmptyHistoDraw.SetAxesAsLimitAxes(inputTitle_X, inputTitle_Y, inputTitle_Z);
}

void CanvasParams::DefaultVals() {
    canvName = "temp";
    canvTitle = "temp";
    
    canvHeight = 800;
    canvWidth = 900;
    
    doLogX = false;
    doLogY = false;
    doLogZ = false;
    doTickX = false;
    doTickY = false;
    
    leftMargin = 0.05;
    rightMargin = 0.05;
    botMargin = 0.05;
    topMargin = 0.05;
    
    params_EmptyHistoDraw.DefaultVals();
}

void CanvasParams::Print() {
    cout << "parameters of the canvas to draw " << endl;
    
    cout << "leftMargin " << leftMargin << endl;
    cout << "rightMargin " << rightMargin << endl;
    cout << "botMargin " << botMargin << endl;
    cout << "topMargin " << topMargin << endl;
    cout << "doLogX " << doLogX << endl;
    cout << "doLogY " << doLogY << endl;
    cout << "doLogZ " << doLogZ << endl;
    cout << "doTickX " << doTickX << endl;
    cout << "doTickY " << doTickY << endl;
    
    cout << "canvName " << canvName << endl;
    cout << "canvTitle " << canvTitle << endl;
}



#endif