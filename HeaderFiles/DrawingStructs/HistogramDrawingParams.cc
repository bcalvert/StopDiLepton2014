#ifndef HistogramDrawingParams_cc
#define HistogramDrawingParams_cc

void HistogramDrawingParams::DefaultVals() {
    xAxisParams.DefaultVals();
    yAxisParams.DefaultVals();
    yAxisParams.DefaultVals();
}

void HistogramDrawingParams::SetHistParams(TH1 * inputHist) {
    TAxis * xAxis = inputHist->GetXaxis();
    xAxisParams.SetInputAxisParams(xAxis);
    
    TAxis * yAxis = inputHist->GetYaxis();
    yAxisParams.SetInputAxisParams(yAxis);
    
    TAxis * zAxis = inputHist->GetZaxis();
    zAxisParams.SetInputAxisParams(zAxis);
    
    inputHist->SetTitle(histTitle);
}

void HistogramDrawingParams::SetAxesAsLimitAxes(TString inputTitle_X, TString inputTitle_Y, TString inputTitle_Z) {
    xAxisParams.SetAsLimitAxis(1, inputTitle_X);
    yAxisParams.SetAsLimitAxis(2, inputTitle_Y);
    zAxisParams.SetAsLimitAxis(3, inputTitle_Z);

}

#endif