#ifndef CanvasParams_h
#define CanvasParams_h

#include "TCanvas.h"
#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

struct CanvasParams {
    
    bool doLogX, doLogY, doLogZ;
    bool doTickX, doTickY;
    float leftMargin, rightMargin, botMargin, topMargin;
    
    float canvWidth, canvHeight;
    
    TString canvName, canvTitle;
    
    TH1 * emptyHistogram;
    HistogramDrawingParams params_EmptyHistoDraw;
    
    void DefaultVals();
    
    void SetAsLimitCanvas(TString, TString, TString, bool);
    
    void SetEmptyHistogram(TH1 *);
    
    void DrawEmptyHisto(TCanvas *);
    
    void SetCanvas(TCanvas *);
    
    void Print();
};
#endif