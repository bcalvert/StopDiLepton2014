// See HistogramStyleFunctions.h, but utilizing some more specialized structs that aren't defined before that header file

inline void GraphMainAttSet(TGraph * inputGraph, IndSampleDisplayParams * inISDP) {
    if (inISDP->fillDS.color != -1) inputGraph->SetFillColor(inISDP->fillDS.color);
    if (inISDP->fillDS.style != -1) inputGraph->SetFillStyle(inISDP->fillDS.style);
    if (inISDP->lineDS.color != -1) inputGraph->SetLineColor(inISDP->lineDS.color);
    if (inISDP->lineDS.width != -1) inputGraph->SetLineWidth(inISDP->lineDS.width);
    if (inISDP->markerDS.color != -1) inputGraph->SetMarkerColor(inISDP->markerDS.color);
    if (inISDP->markerDS.style != -1) inputGraph->SetMarkerStyle(inISDP->markerDS.style);
    if (inISDP->markerDS.size != -1) inputGraph->SetMarkerSize(inISDP->markerDS.size);
}

inline void HistMainAttSet(TH1 * inputHist, IndSampleDisplayParams * inISDP) {
    if (inISDP->fillDS.color != -1) inputHist->SetFillColor(inISDP->fillDS.color);
    if (inISDP->fillDS.style != -1) inputHist->SetFillStyle(inISDP->fillDS.style);
    if (inISDP->lineDS.color != -1) inputHist->SetLineColor(inISDP->lineDS.color);
    if (inISDP->lineDS.width != -1) inputHist->SetLineWidth(inISDP->lineDS.width);
    if (inISDP->markerDS.color != -1) inputHist->SetMarkerColor(inISDP->markerDS.color);
    if (inISDP->markerDS.style != -1) inputHist->SetMarkerStyle(inISDP->markerDS.style);
    if (inISDP->markerDS.size != -1) inputHist->SetMarkerSize(inISDP->markerDS.size);
    /*
  inputHist->SetFillColor(inISDP->fillDS.color);
  inputHist->SetFillStyle(inISDP->fillDS.style);
  inputHist->SetLineColor(inISDP->lineDS.color);
  inputHist->SetLineWidth(inISDP->lineDS.width);
  inputHist->SetLineStyle(inISDP->lineDS.style);
  inputHist->SetMarkerColor(inISDP->markerDS.color);
  inputHist->SetMarkerStyle(inISDP->markerDS.style);
  inputHist->SetMarkerSize(inISDP->markerDS.size);
    */
}
