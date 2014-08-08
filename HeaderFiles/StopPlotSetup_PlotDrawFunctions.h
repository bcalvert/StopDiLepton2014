

void SpectrumDraw_AddSignal(TCanvas * InputCanvas, HistogramDisplayStructs * inHDP_Signal, TLegend * inputLeg, bool showLegend) {
    
    TPad * Pad1 = (TPad *) InputCanvas->cd(1);
    TH1F * currSigHist;
    TH1F * h_SigErr;
    
    currSigHist = inHDP_Signal->compSamp.first.grabbedHist_TH1F;
    TString legHist1 = inHDP_Signal->compSamp.second.legendEntry;
    h_SigErr = (TH1F *) currSigHist->Clone();
    
    HistMainAttSet(h_SigErr, h_SigErr->GetLineColor(), 3001, h_SigErr->GetLineColor(), 2, kWhite, 0, 0);
    h_SigErr->Draw("e2 same");
    currSigHist->Draw("hist same");
    inputLeg->AddEntry(currSigHist, legHist1, "l");
    if (showLegend) {
        inputLeg->Draw("same");
    }
    Pad1->Update();
    Pad1->Modified();
}