//Set of functions used for drawing various parts of spectra and/or resolution/mean graphs

void LegendTextDrawTopPad(TCanvas * InputCanvas, TH1F * Hist1, TString legHist1, AncillaryDrawingVariables * inADV, bool doMean, HistogramDisplayStructs * inHDP_MC, bool justStat = true, bool showLegend = true, bool doVerbosity = false) {
    
    // function that draws the legend on the top part of a typical two pad canvas
    // legend is contained within the AncillaryDrawingVariables struct
    
    TPad * topPad = (TPad *) InputCanvas->cd(1);
            
    int numLegendEntries = 1 + (int) inHDP_MC->vecSampDisplay_IndMC.size();
    float legXstart = inADV->BaseBounds.xLB;
    float legYstart = inADV->BaseBounds.yLB;
    if (doMean) {
        legYstart = legYstart - 0.25;
    }
        
    XYBounds legXYBs; legXYBs.SetBounds(legXstart - 0.05, legYstart - 0.05 * (numLegendEntries), legXstart + 0.40, legYstart);
    if (doVerbosity) {
        cout << "legXYB bounds " << endl;
        legXYBs.PrintVals();
    }
    inADV->SetLegBounds(&legXYBs, true, doVerbosity);
    inADV->topLeg.Clear();
    inADV->topLeg.AddEntry(Hist1, legHist1, "pl");    
    for (unsigned int iIndMC = 0; iIndMC < inHDP_MC->vecSampDisplay_IndMC.size(); ++iIndMC) {
        inADV->topLeg.AddEntry(inHDP_MC->vecSampDisplay_IndMC[inHDP_MC->vecSampDisplay_IndMC.size() - (1 + iIndMC)].first.grabbedHist_TH1F, inHDP_MC->vecSampDisplay_IndMC[inHDP_MC->vecSampDisplay_IndMC.size() - (1 + iIndMC)].second.legendEntry, "f");
    }
    TString errLegEntry = justStat ? "Stat. Unc." : "Stat #oplus syst" ;
    inADV->topLeg.AddEntry(inADV->errGraph, errLegEntry, "f");
    if (showLegend) {
        inADV->topLeg.Draw("same");
    }
    
    if (doVerbosity) {
        cout << "printing legend contents" << endl;
        inADV->topLeg.Print();
    }
    
    topPad->Update();
    topPad->Modified();
}

void LegendTextDrawBotPad(TCanvas * InputCanvas, AncillaryDrawingVariables * inADV, bool doVerbosity = false) {
    
    // function that draws the legend for the bottom part of a typical two pad canvas
    // legend is contained within the AncillaryDrawingVariables struct
    
    TPad * botPad = (TPad *) InputCanvas->cd(2);
    float ratioXLB = 0.21;
    float ratioXUB = 0.36;
    float ratioYLB = 0.78;
    float ratioYUB = 0.93;
    XYBounds legXYBs; legXYBs.SetBounds(ratioXLB, ratioYLB, ratioXUB, ratioYUB);
    if (doVerbosity) {
        cout << "about to set bottom legend bounds " << endl;
    }
    inADV->SetLegBounds(&legXYBs, false, doVerbosity);
    inADV->botLeg.Clear();
    inADV->botLeg.AddEntry(inADV->fracRatioErrGraphSyst, "Syst. Uncert.", "f");
    inADV->botLeg.Draw("same");
    
    botPad->Update();
    botPad->Modified();
}

void SpectrumDraw(TCanvas * InputCanvas, HistogramDisplayStructs * inHDP_Data, HistogramDisplayStructs * inHDP_MC, AncillaryDrawingVariables * inADV, bool doMean, GlobalHistPlotInfo * inGHPI, bool doSFR, bool showLegend, bool doVerbosity = false) {
    
    // function that draws the spectra (Data and MC) for the top part of a typical two pad canvas
                
    TH1F * Hist1 = inHDP_Data->compSamp.first.grabbedHist_TH1F;
    TString legHist1 = inHDP_Data->compSamp.second.legendEntry;
    
    TH1F * Hist2 = inHDP_MC->compSamp.first.grabbedHist_TH1F;
    TString legHist2 = inHDP_MC->compSamp.second.legendEntry;
    
    BaseCanvasSetup(InputCanvas, inADV->doLogYTopPad);
    BaseSpectrumDraw(InputCanvas, Hist1, inADV);
    SetStyle_Text_TopPad(InputCanvas, Hist1, inADV);
    if (doMean) {
        DrawStats(InputCanvas, Hist1, "Data", Hist2, "MC", &inADV->BaseBounds);
    }
    if (doVerbosity) {
        cout << "about to draw latex strings " << endl;
    }
    inGHPI->DrawLatexStrings(InputCanvas, 1, doVerbosity);
    if (doVerbosity) {
        cout << "about to draw legend " << endl;
    }
    LegendTextDrawTopPad(InputCanvas, Hist1, legHist1, inADV, doMean, inHDP_MC, true, showLegend, doVerbosity);

    DrawBottomRatio(InputCanvas->cd(2), inADV, doSFR);
}

void SpectrumDrawSyst(TCanvas * InputCanvas, HistogramDisplayStructs * inHDP_Data, HistogramDisplayStructs * inHDP_MC, AncillaryDrawingVariables * inADV, bool doMean, GlobalHistPlotInfo * inGHPI, bool doSFR, bool showLegend, bool doSmoothSyst, bool doVerbosity = false, bool doSignif = false) {
    
    // function that draws the MC systematics for the top part of a typical two pad canvas (that is, draws them overlaying the spectra
    // and draws the bottom part systematics (i.e. the systematics band on the ratio
    // note this function also draws the legends
    
    TH1F * Hist1 = inHDP_Data->compSamp.first.grabbedHist_TH1F;
    TString legHist1 = inHDP_Data->compSamp.second.legendEntry;
    
    TH1F * Hist2 = inHDP_MC->compSamp.first.grabbedHist_TH1F;
    TString legHist2 = inHDP_MC->compSamp.second.legendEntry;
    
    BaseCanvasSetup(InputCanvas, inADV->doLogYTopPad);
    BaseSpectrumDraw(InputCanvas, Hist1, inADV);
    SetStyle_Text_TopPad(InputCanvas, Hist1, inADV);
    if (doMean) {
        DrawStats(InputCanvas, Hist1, "Data", Hist2, "MC", &inADV->BaseBounds);
    }
    if (doVerbosity) {
        cout << "about to draw latex strings " << endl;
    }
    inGHPI->DrawLatexStrings(InputCanvas, 1, doVerbosity);
    if (doVerbosity) {
        cout << "about to draw legend " << endl;
    }
    LegendTextDrawTopPad(InputCanvas, Hist1, legHist1, inADV, doMean, inHDP_MC, false, showLegend, doVerbosity);
    
    DrawBottomRatio(InputCanvas->cd(2), inADV, doSFR, true, doSmoothSyst, doSignif);
    if (!doSignif) {
        LegendTextDrawBotPad(InputCanvas, inADV, doVerbosity);
    }
}




void SpectrumDrawSplitSyst(TCanvas * InputCanvas, AncillaryDrawingVariables * inADV, GlobalHistPlotInfo * inGHPI, vector<IndivSystBand> * inVecISB, bool doSFR, bool doSmoothSyst, bool doVerbosity = false) {
    
    // function that draws the split MC systematics for the three pad canvas
    
    inGHPI->DrawLatexStrings(InputCanvas, 1, doVerbosity);
    
    unsigned int numISBs = inVecISB->size();
    SplitSystCanvasSetup(InputCanvas, numISBs);
    bool isBotPad = false;
    for (unsigned int iISB = 0; iISB < numISBs; ++iISB) {
        DrawBottomRatio(InputCanvas->cd(numISBs - iISB), inADV, doSFR, false, doSmoothSyst);
        isBotPad = (iISB == 0);
        inVecISB->at(iISB).DrawBands(InputCanvas->cd(numISBs - iISB), isBotPad);
    }
}


void SpectrumDraw_AddSignal(TCanvas * InputCanvas, HistogramDisplayStructs * inHDS_Signal, TLegend * inputLeg, bool showLegend, int whichSyst = -1) {
    
    TPad * Pad1 = (TPad *) InputCanvas->cd(1);
    TH1F * currSigHist;
    TH1F * h_SigErr;
    
    currSigHist = inHDS_Signal->compSamp.first.grabbedHist_TH1F;
    TString legHist1 = inHDS_Signal->compSamp.second.legendEntry;
    
    if (whichSyst < 0) {
        h_SigErr = (TH1F *) currSigHist->Clone();    
        HistMainAttSet(h_SigErr, h_SigErr->GetLineColor(), 3001, h_SigErr->GetLineColor(), 2, kWhite, 0, 0);
        h_SigErr->Draw("e2 same");
    }
    else {
        inHDS_Signal->compSamp.first.indSSI.SystPlusStatErrorGraph[whichSyst]->Draw("e2 same");
    }
    currSigHist->Draw("hist same");
        
    inputLeg->AddEntry(currSigHist, legHist1, "l");
    if (showLegend) {
        inputLeg->Draw("same");
    }
    Pad1->Update();
    Pad1->Modified();
}
