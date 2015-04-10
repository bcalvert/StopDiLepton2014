#include <sstream>

// Set of very general functions used as part of making plots -- a lot of these are called in other functions/header files

void SetProjectionLatex(LatexString * inLS, int whichProjection, TString cutUsed = "") {
    bool useNDC = true;
    float textSize = 0.04;
    int alignText = 11;
    float xLB = 0.22;
    float yLB = whichProjection ? 0.76 : 0.86;
    TString textToShow = cutUsed;
    Color_t latexColor = kBlack;
    
    inLS->SetLatex(useNDC, textSize, alignText, xLB, yLB, textToShow, latexColor);
}

void SetGenCutLatex(LatexString * inLS, TString cutUsed = "") {
    bool useNDC = true;
    float textSize = 0.04;
    int alignText = 11;
    float xLB = 0.22;
    float yLB = 0.86;
    TString textToShow = cutUsed;
    Color_t latexColor = kBlack;
    
    inLS->SetLatex(useNDC, textSize, alignText, xLB, yLB, textToShow, latexColor);
}
void SetCMSPreliminaryLatex(LatexString * inLS, bool isPrelim = true) {
    bool useNDC = true;
    float textSize = 0.04;
    int alignText = 11;
    float xLB = 0.6;
    float yLB = 0.96;
    TString textToShow = isPrelim ? "CMS preliminary 2012" : "";
    Color_t latexColor = kBlack;

    inLS->SetLatex(useNDC, textSize, alignText, xLB, yLB, textToShow, latexColor);
}
void SetLumiLatex(LatexString * inLS, float intLumi) {
    bool useNDC = true;
    float textSize = 0.04;
    int alignText = 31;
    float xLB = 0.886;
    float yLB = 0.862;
    TString textToShow = Form(" %.1f fb^{-1} at #sqrt{s} = 8 TeV", intLumi/1000.);
    Color_t latexColor = kBlack;
    
    inLS->SetLatex(useNDC, textSize, alignText, xLB, yLB, textToShow, latexColor);
}
void SetMETLatexString(LatexString * inLS, int typeMET = 0, TString textExtra = "") {
    const int numMET = 5;
    TString stringMET[numMET] = {"PF", "NoPU PF", "MVA PF", "MVA Unity PF" , "Calo"};
    TString textToShow = stringMET[typeMET];
    textToShow += " #slash{E}_{T}";
    textToShow += textExtra;
    bool useNDC = true;
    float textSize = 0.04;
    int alignText = 11;
    float xLB = 0.2;
    float yLB = 0.96;
    Color_t latexColor = kBlack;    
    inLS->SetLatex(useNDC, textSize, alignText, xLB, yLB, textToShow, latexColor);
}

void SetDilepLatexString(LatexString * inLS, int typeDilep = -1) {
    const TString nameDilep[3] = {"#mu#mu", "ee", "e#mu"};
    TString stringDilep;
    if (typeDilep > -1) {
        stringDilep = nameDilep[typeDilep];
    }
    bool useNDC = true;
    float textSize = 0.104;
    int alignText = 11;
    float xLB = 0.22;
    float yLB = 0.86;
    Color_t latexColor = kBlack;
    inLS->SetLatex(useNDC, textSize, alignText, xLB, yLB, stringDilep, latexColor);
}



void BaseCanvasSetup(TCanvas * InputCanvas, bool logYPad1) {
    InputCanvas->Divide(1, 2);
    TPad * Pad1 = (TPad *) InputCanvas->cd(1);
    FixPad(Pad1, 1, InputCanvas);    
    Pad1->SetLogy(logYPad1);
    TPad * botPad = (TPad *) InputCanvas->cd(2);
    FixPad(botPad, 2, InputCanvas);
    botPad->SetGridy(1);    
}
void BaseSpectrumDraw(TCanvas * InputCanvas, TH1F * Hist1, AncillaryDrawingVariables * inADV) {
    TPad * topPad = (TPad *) InputCanvas->cd(1);
    Hist1->Draw("e1");
    inADV->mcStack->Draw("hist same");
    inADV->errGraph->Draw("2 same");
    Hist1->Draw("e1 same");
    Hist1->Draw("axis same");
    topPad->Update();
    topPad->Modified();
}

void BaseSpectrumDraw(TCanvas * InputCanvas, TH1F * Hist1, TGraphAsymmErrors * errGraph, THStack * MCStack) {
    TPad * topPad = (TPad *) InputCanvas->cd(1);
    Hist1->Draw("e1");
    MCStack->Draw("hist same");
    errGraph->Draw("2 same");
    Hist1->Draw("axis same");
    topPad->Update();
    topPad->Modified();
}

void BaseSpectrumDraw(TCanvas * InputCanvas, TH1F * Hist1, TH1F * errHist, THStack * MCStack) {
    TPad * topPad = (TPad *) InputCanvas->cd(1);
    Hist1->Draw("e1");
    MCStack->Draw("hist same");
    errHist->Draw("e2 same");    
    Hist1->Draw("axis same");
    topPad->Update();
    topPad->Modified();
}
void DrawBottomRatio(TCanvas * InputCanvas, AncillaryDrawingVariables * inADV, bool doSFR, bool doSyst = false, bool doSmoothSyst = false) {
    TPad * botPad = (TPad *) InputCanvas->cd(2);
    TH1F * fracRatioHist = inADV->fracRatioHist;
    TAxis * YAxis = fracRatioHist->GetYaxis();
    TAxis * XAxis = fracRatioHist->GetXaxis();
    TString titleXAxis = XAxis->GetTitle();
    inADV->xADPs[1].SetStrings(titleXAxis, "");
    inADV->xADPs[1].SetAxisParams(XAxis);
    YAxis->SetNdivisions(3,5,0);
    XAxis->SetNdivisions(6,5,0);
    float RatioMax;
    float RatioMin;  
    if (doSFR) {
        RatioMax = fracRatioHist->GetBinContent(fracRatioHist->GetMaximumBin());
        RatioMin = fracRatioHist->GetBinContent(fracRatioHist->GetMinimumBin());
        if (abs(RatioMin - 0) < 1E-3) RatioMin = fracRatioHist->GetMinimum(1E-3);
        YAxis->SetRangeUser(RatioMin - 0.1, RatioMax + 0.1);
    }
    TGraphAsymmErrors * fracRatioDrawGraph = ClonePoints(fracRatioHist, false);
    fracRatioHist->SetLineColor(kBlack);
    HistToGraphCopyAttributes(fracRatioHist, fracRatioDrawGraph);
    TH1F * patsy = (TH1F*) fracRatioHist->Clone("frac_patsy");
    patsy->SetLineColor(kWhite);
    patsy->SetMarkerColor(kWhite);
    patsy->Draw("e1");
    
    if (doSyst) {
        if (doSmoothSyst && inADV->fracRatioErrGraphSyst->GetN() > 10) {
            SmoothedTGraph(inADV->fracRatioErrGraphSyst);
        }
        inADV->fracRatioErrGraphSyst->Draw("2 same");
    }
    fracRatioDrawGraph->Draw("p0 same");
    botPad->Update();
    botPad->Modified();
}

void SetStyle_Text_TopPad(TCanvas * InputCanvas, TH1F * Hist1, AncillaryDrawingVariables * inADV) {
    TPad * topPad = (TPad *) InputCanvas->cd(1);
    int NBins = Hist1->GetNbinsX();    
    TAxis * XAxis = Hist1->GetXaxis();
    float XBinUB = XAxis->GetXmax();
    float XBinLB = XAxis->GetXmin();
    float BinWidthGeVInit = (XBinUB - XBinLB)/NBins;
    float BinWidthGeV = nDigits(BinWidthGeVInit, 3);
    stringstream ss (stringstream::in | stringstream::out);
    ss << BinWidthGeV;   
    string attempt = ss.str();
    TString BinWidthString;
    BinWidthString += attempt;
    
    TAxis * YAxis = Hist1->GetYaxis();
    TString YAxisTitle = YAxis->GetTitle(); 
    int StrPos = YAxisTitle.Index("NUM", 3, TString::kExact);
    if (StrPos != -1) YAxisTitle.Replace(StrPos, 3, BinWidthString);
    inADV->yADPs[0].SetStrings(YAxisTitle, "");
    inADV->yADPs[0].SetAxisParams(YAxis);
    
    TString XAxisTitle = XAxis->GetTitle();
    inADV->xADPs[0].SetStrings(XAxisTitle, "");
    inADV->xADPs[0].SetAxisParams(XAxis);
    YAxis->SetNdivisions(5,5,0);
    topPad->Update();
    topPad->Modified();
}

void DrawStats(TCanvas * InputCanvas, TH1F * Hist1, TString textHist1, TH1F * Hist2, TString textHist2, XYBounds * inXYB) {
    TPad * topPad = (TPad *) InputCanvas->cd(1);
    
    LatexString plotLatex;
    plotLatex.SetLatex();
    TLatex plotStatLatex;
    plotLatex.straightGhetto.SetLatexParams(&plotStatLatex);
    char buf[99];
    float MeanXstart = inXYB->xLB;
    float MeanYstart = inXYB->yLB;
    if (inXYB->xLB < 0.5) {
        MeanXstart += 0.01;
        MeanYstart -= 0.02;
    }
    string textH1 = string(textHist1);
    string textH2 = string(textHist2);
    char array1[textH1.size() + 1];
    strcpy(array1, textH1.c_str());
    char array2[textH2.size() + 1];
    strcpy(array2, textH2.c_str());
    
    plotStatLatex.SetTextColor(kBlack);
    sprintf(buf,"%s Mean = %0.2f #pm %0.2f", array1, Hist1->GetMean(), Hist1->GetMeanError());
    plotStatLatex.DrawLatex(MeanXstart,MeanYstart,buf);
    sprintf(buf,"%s RMS = %0.2f #pm %0.2f", array1, Hist1->GetRMS(), Hist1->GetRMSError());
    plotStatLatex.DrawLatex(MeanXstart,MeanYstart-0.05,buf);
    plotStatLatex.SetTextColor(kRed);
    sprintf(buf,"%s Mean = %0.2f #pm %0.2f", array2, Hist2->GetMean(),Hist2->GetMeanError());
    plotStatLatex.DrawLatex(MeanXstart,MeanYstart-0.1,buf);
    sprintf(buf,"%s RMS = %0.2f #pm %0.2f", array2, Hist2->GetRMS(),Hist2->GetRMSError());
    plotStatLatex.DrawLatex(MeanXstart,MeanYstart-0.15,buf);
    
    topPad->Update();
    topPad->Modified();
}