// Structs used in very general fashions as part of drawing plots

#include "TLatex.h"
#include "TLegend.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TAxis.h"
typedef struct XYBounds {
    float xLB, xUB;
    float yLB, yUB;
    void SetBounds(float x1, float y1, float x2, float y2) {
        xLB = x1;
        xUB = x2;
        yLB = y1;
        yUB = y2;
    }
    void PrintVals() {
        cout << "xLB " << xLB << endl;
        cout << "xUB " << xUB << endl;
        cout << "yLB " << yLB << endl;
        cout << "yUB " << yUB << endl;
    }
} XYBounds;

typedef struct GhettoLatexParams {
    bool NDC;
    float textSize;
    int textAlign;    
    Color_t textColor;
    void SetGhettoLatex(bool useNDC = true, float inTextSize = 0.03, int inTextAlign = 12, Color_t inTextColor = kBlack) {
        NDC = useNDC;
        textSize = inTextSize;
        textAlign = inTextAlign;
        textColor = inTextColor;
    }
    void SetLatexParams(TLatex * inTLatex) {
        inTLatex->SetNDC(NDC);
        inTLatex->SetTextAlign(textAlign);
        inTLatex->SetTextSize(textSize);
        inTLatex->SetTextColor(textColor);
    }
} GhettoLatexParams;

typedef struct LatexString {
    TString text;
    XYBounds position;
    GhettoLatexParams straightGhetto;
    //stuff for style?
    void SetLatex(bool useNDC = true, float inTextSize = 0.03, int inTextAlign = 12, float xLB = 0.0, float yLB = 0.0, TString inText = "", Color_t inTextColor = kBlack) {
        straightGhetto.SetGhettoLatex(useNDC, inTextSize, inTextAlign, inTextColor);
        position.SetBounds(xLB, yLB, xLB, yLB);
        text = inText;
    }
    
    void SetTLatex(TLatex * inTLatex, bool doVerbosity = false) {
        straightGhetto.SetLatexParams(inTLatex);
        if (doVerbosity) {
            cout << "position x " << position.xLB << endl;
            cout << "position y " << position.yLB << endl;
            cout << "text " << text << endl;
        }
        inTLatex->SetText(position.xLB, position.yLB, text);
    }
    void DrawCanvas(TCanvas * inputCanvas, int whichPad = 1, bool doVerbosity = false) {
        TPad * padToDrawOn = (TPad *) inputCanvas->cd(whichPad);
        TLatex outLatex;
        SetTLatex(&outLatex, doVerbosity);
        if (doVerbosity) {
            cout << "title of outLatex " << outLatex.GetTitle() << endl;   
        }
        outLatex.DrawLatex(position.xLB, position.yLB, text);
        padToDrawOn->Update();
        padToDrawOn->Modified();
    }
} LatexString;
typedef struct AxisDisplayParams {
    TString axisStringPart1, axisStringPart2;
    float axisRangePart1, axisRangePart2;
    vector<Size_t> vecAxisSizes; // 0 Title Size, 1 Label Size
    vector<double> vecAxisOffsets; // 0 Title Offsets, 1 Label Offsets
    bool doSmartAxis;
    void DefaultVarVals() {
        vecAxisOffsets.resize(2);
        vecAxisSizes.resize(2);
        doSmartAxis = false;
        axisStringPart1 = "";
        axisStringPart2 = "";
        axisRangePart1 = 0.0;
        axisRangePart2 = 0.0;
    }
    void SetStrings(TString input1, TString input2) {
        axisStringPart1 = input1;
        axisStringPart2 = input2;
    }
    void SetRanges(float input1, float input2) {
        axisRangePart1 = input1;
        axisRangePart2 = input2;
    }
    void SetOffsets(double inTitleOffset, double inLabelOffset) {
        vecAxisOffsets[0] = inTitleOffset;
        vecAxisOffsets[1] = inLabelOffset;
    }
    void SetSizes(Size_t inTitleSize, Size_t inLabelSize) {
        vecAxisSizes[0] = inTitleSize;
        vecAxisSizes[1] = inLabelSize;
    }    
    TString FullAxisTitle() {
        return axisStringPart1 + axisStringPart2;
    }
    void SetAxisParams(TAxis * inAxis) {
        HistAxisAttSet(inAxis, FullAxisTitle(), vecAxisSizes[0], vecAxisOffsets[0], vecAxisSizes[1], vecAxisOffsets[1], axisRangePart1, axisRangePart2);
    }
} AxisDisplayParams;

typedef struct AncillaryDrawingVariables {
    TLegend topLeg, botLeg;
    THStack * mcStack;
    TGraphAsymmErrors * errGraph;
    TH1F * fracRatioHist;
    TGraphAsymmErrors * fracRatioErrGraphSyst;
    bool doLogYTopPad;
    
    XYBounds BaseBounds;
    vector<AxisDisplayParams> yADPs, xADPs;
    void DefaultLegParams() {
        topLeg.SetFillStyle(0);
        topLeg.SetLineColor(kWhite);
        topLeg.SetLineWidth(0);
        topLeg.SetTextFont(62);
        botLeg.SetFillStyle(0);
        botLeg.SetLineColor(kWhite);
        botLeg.SetLineWidth(0);
        botLeg.SetTextFont(62);
    }
    void SetLegBounds(XYBounds * inXYB, bool setTopLeg = true, bool doVerbosity = false) {
        if (doVerbosity) {
            inXYB->PrintVals();
        }
        TLegend * whichLegToSet = setTopLeg ? &topLeg : &botLeg;
        whichLegToSet->SetX1NDC(inXYB->xLB);
        whichLegToSet->SetX2NDC(inXYB->xUB);
        whichLegToSet->SetY1NDC(inXYB->yLB);
        whichLegToSet->SetY2NDC(inXYB->yUB);
    }
    void SetFracRatioHist(TH1F * inFracRatioHist) {
        fracRatioHist = inFracRatioHist;
        HistMainAttSet(fracRatioHist, kWhite, 0, kBlack, 1, kBlack, 21, 0.0);
    }
    void SetFracRatioGraph(TGraphAsymmErrors * inFracRatioErrGraphSyst, Color_t fillColor = kYellow) {
        fracRatioErrGraphSyst = inFracRatioErrGraphSyst;
        GraphMainAttSet(fracRatioErrGraphSyst, fillColor, 3001, kGray + 1, 2, kWhite, 0, 0);
    }
    void SetErrGraph(TH1F * inMCCompHist = 0, TGraphAsymmErrors * inErrGraph = 0) {
        if (inErrGraph != 0) {
            errGraph = inErrGraph;
        }
        else if (inMCCompHist != 0) {            
            errGraph = ClonePoints(inMCCompHist);
        }
        else {
            cout << "something wiggedy hombre! One of the Two needs to be non NULL! " << endl;
        }
        GraphMainAttSet(errGraph, kGray+1, 3001, kGray+1, 2, kWhite, 0, 0);
    }
    void SetPlots(THStack * inMCStack, TH1F * inFracRatioHist, TH1F * inMCCompHist, TGraphAsymmErrors * inErrGraph = 0) {
        mcStack = inMCStack;
        SetFracRatioHist(inFracRatioHist);
        SetErrGraph(inMCCompHist, inErrGraph);
    }
    
    void SetDefaultAxisSizeParams() {
        yADPs.resize(2);
        xADPs.resize(2);
        for (int iPad = 0; iPad < 2; ++iPad) {
            yADPs[iPad].DefaultVarVals();
            xADPs[iPad].DefaultVarVals();
        }
        
        Size_t topXTitleSize = 0.06;
        double topXTitleOffset = 999;
        Size_t topXLabelSize = 0.05;
        double topXLabelOffset = 999;
        
        Size_t topYTitleSize = 0.06;
        double topYTitleOffset = 1.5;
        Size_t topYLabelSize = 0.05;
        double topYLabelOffset = 0.007;
        
        Size_t botXTitleSize = 0.17;
        double botXTitleOffset = 1.03;
        Size_t botXLabelSize = 0.12;
        double botXLabelOffset = 0.07;
        
        Size_t botYTitleSize = 0.0;
        double botYTitleOffset = 0.0;        
        Size_t botYLabelSize = 0.0;
        double botYLabelOffset = 0.0;
        
        yADPs[0].SetOffsets(topYTitleOffset, topYLabelOffset);
        yADPs[0].SetSizes(topYTitleSize, topYLabelSize);
        yADPs[1].SetOffsets(botYTitleOffset, botYLabelOffset);
        yADPs[1].SetSizes(botYTitleSize, botYLabelSize);
        
        xADPs[0].SetOffsets(topXTitleOffset, topXLabelOffset);
        xADPs[0].SetSizes(topXTitleSize, topXLabelSize);
        xADPs[1].SetOffsets(botXTitleOffset, botXLabelOffset);
        xADPs[1].SetSizes(botXTitleSize, botXLabelSize);
    }
    
} AncillaryDrawingVariables;
