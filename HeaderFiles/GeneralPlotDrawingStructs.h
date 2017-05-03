// Structs used in very general fashions as part of drawing plots

#include "TLatex.h"
#include "TLegend.h"
#include "THStack.h"
#include "TGraphAsymmErrors.h"
#include "TH1F.h"
#include "TAxis.h"
#include "TLine.h"
typedef struct XYBounds {
    float xLB, xUB;
    float yLB, yUB;
    float GetXRange() {
        return xUB - xLB;
    }
    float GetYRange() {
        return yUB - yLB;
    }
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
    int textFont;
    float textAngle;
    void SetGhettoLatex(bool useNDC = true, float inTextSize = 0.03, int inTextAlign = 12, Color_t inTextColor = kBlack) {
        NDC = useNDC;
        textSize = inTextSize;
        textAlign = inTextAlign;
        textColor = inTextColor;
        textAngle = 0;
        textFont = 62;
    }
    void SetLatexParams(TLatex * inTLatex) {
        inTLatex->SetNDC(NDC);
        inTLatex->SetTextAlign(textAlign);
        inTLatex->SetTextSize(textSize);
        inTLatex->SetTextColor(textColor);
        inTLatex->SetTextFont(textFont);
        inTLatex->SetTextAngle(textAngle);
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
//        outLatex.DrawLatex(position.xLB, position.yLB, text);
        outLatex.DrawLatex(position.xLB, position.yLB, outLatex.GetTitle());
        padToDrawOn->Update();
        padToDrawOn->Modified();
    }
} LatexString;


struct KinematicLine {
    XYBounds bounds_Line;
    TLine drawLine;
    LatexString drawString;
    
    void SetLatexString(bool useNDC = true, float inTextSize = 0.03, int inTextAlign = 12, float xLB = 0.0, float yLB = 0.0, TString inText = "", Color_t inTextColor = kBlack) {
        drawString.SetLatex(useNDC, inTextSize, inTextAlign, xLB, yLB, inText, inTextColor);
    }
    void DrawOnCanvas(TCanvas * inputCanvas, int whichPad = 1, bool doVerbosity = false) {
        drawString.DrawCanvas(inputCanvas,whichPad, doVerbosity);
        TPad * padToDrawOn = (TPad *) inputCanvas->cd(whichPad);
        drawLine.DrawLine(bounds_Line.xLB, bounds_Line.yLB, bounds_Line.xUB, bounds_Line.yUB);
        padToDrawOn->Update();
        padToDrawOn->Modified();
    }
};

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
    TH1F * signifHist;
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
        HistMainAttSet(fracRatioHist, kWhite, 0, kBlack, 1, kBlack, 20, 0.9);
    }
    void SetFracRatioGraph(TGraphAsymmErrors * inFracRatioErrGraphSyst, Color_t fillColor = kRed - 7) {
        fracRatioErrGraphSyst = inFracRatioErrGraphSyst;
        GraphMainAttSet(fracRatioErrGraphSyst, fillColor, 3001, kGray + 1, 2, kWhite, 0, 0);
	//	GraphMainAttSet(fracRatioErrGraphSyst, fillColor, 3001, kRed - 4, 2, kWhite, 0, 0);
    }
    void SetSignifHist(TH1F * inSignifHist) {
        Color_t fillColorSignif = kBlue;
        Color_t lineColorSignif = kRed;
        Style_t styleMarker = 0;
        
        signifHist = inSignifHist;
        HistMainAttSet(inSignifHist, fillColorSignif, 3001, lineColorSignif, 2, kBlack, 20, styleMarker);
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
    void NoXaxisLabel() {
        Size_t botXTitleSize = 0.17;
        double botXTitleOffset = 1.03;
        Size_t botXLabelSize = 0.12;
        double botXLabelOffset = 1;
        xADPs[1].SetOffsets(botXTitleOffset, botXLabelOffset);
        xADPs[1].SetSizes(botXTitleSize, botXLabelSize);
    }
} AncillaryDrawingVariables;


struct IndivSystBand {
    vector<int> vecSystStyle, vecSystColor, vecSystLineStyle;
    bool doCumulativeSyst;
    
    TH1F * inHist;
    vector<TGraphAsymmErrors *> vecSystBands;
    
    //variables for controlling which systs to look for and which get paired up
    vector<TString> vecSystNames_In, vecSystNames_Out;
    vector<TString> vecLegendName;
    vector< vector<int> > vecVecIndSystCombine;
    
    void DefaultVarVals() {
        const int numPossibleSysts = 4;

//        Style_t arrSystStyle[numPossibleSysts] = {0, 0, 3002, 3001};
//        Style_t arrSystStyle[numPossibleSysts] = {3001, 3002, 3008, 3010};
        Style_t arrSystStyle[numPossibleSysts] = {1001, 1001, 1001, 1001};
        vecSystStyle.assign(&arrSystStyle[0], &arrSystStyle[0] + numPossibleSysts);
        
//        Color_t arrSystColor[numPossibleSysts] = {kViolet+2, kOrange+7, kAzure+2, kGreen+1};
        Color_t arrSystColor[numPossibleSysts] = {kGreen+1, kAzure+2, kOrange+7, kViolet+2};
        vecSystColor.assign(&arrSystColor[0], &arrSystColor[0] + numPossibleSysts);
        
//        Style_t arrSystLStyle[numPossibleSysts] = {1, 2, 2, 2};
        Style_t arrSystLStyle[numPossibleSysts] = {1, 1, 1, 1};
        vecSystLineStyle.assign(&arrSystLStyle[0], &arrSystLStyle[0] + numPossibleSysts);
        
        doCumulativeSyst = true;
    }
    void SetBandStyle(unsigned int iBand) {
        vecSystBands[iBand]->SetFillStyle(vecSystStyle[iBand]);
        vecSystBands[iBand]->SetFillColor(vecSystColor[iBand]);
        vecSystBands[iBand]->SetLineStyle(vecSystLineStyle[iBand]);
        vecSystBands[iBand]->SetLineColor(vecSystColor[iBand]);
        vecSystBands[iBand]->SetLineWidth(2);
    }
    
    //For next two functions, check the VecNameSysts function in HistogramSystematics2.h
    //"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "TTBarNorm", "DYNorm", "EWKNorm", "FakeLepSyst"
    void SetNamesObjES(bool isSmear) {
        //Combining the object energy scale/resolution systematics
        const int numEnergySysts_In = 4;
        const int numEnergySysts_Out = 4;
        
        const int numEnergySysts_NoSmear = 2;
        
        TString arrCumulSystNames_ES_In[numEnergySysts_In] = {"JetES", "LepES", "JetSmear", "UncES"};
        TString arrCumulSystNames_ES_Out[numEnergySysts_Out] = {"JetES", "JetES_LepES", "JetES_LepES_JetSmear", "JetES_LepES_JetSmear_UncES_ObjES"};
        TString arrSystLegNames[numEnergySysts_Out] = {"Jet ES", "Lep. ES", "Jet ER", "Uncl. ES"};
        
        vecVecIndSystCombine.resize(0);
        //vector<int> vecJetES = {0};
        vector<int> vecJetES;
        vecJetES.push_back(0);
        vecVecIndSystCombine.push_back(vecJetES);
        //vector<int> vecLepES = {1};
        vector<int> vecLepES; vecLepES.push_back(1);
        vecVecIndSystCombine.push_back(vecLepES);
        if (isSmear) {
            //vector<int> vecJetER = {2};
            vector<int> vecJetER; vecJetER.push_back(2);
            vecVecIndSystCombine.push_back(vecJetER);
            //vector<int> vecUnclES = {3};
            vector<int> vecUnclES; vecUnclES.push_back(3);
            vecVecIndSystCombine.push_back(vecUnclES);
        }
        
        if (!isSmear) {
            arrCumulSystNames_ES_Out[1] += "_ObjES";
            vecSystNames_In.assign(&arrCumulSystNames_ES_In[0], &arrCumulSystNames_ES_In[0] + numEnergySysts_NoSmear);
            vecSystNames_Out.assign(&arrCumulSystNames_ES_Out[0], &arrCumulSystNames_ES_Out[0] + numEnergySysts_NoSmear);
            vecLegendName.assign(&arrSystLegNames[0], &arrSystLegNames[0] + numEnergySysts_NoSmear);
            
            vecSystBands.resize(numEnergySysts_NoSmear);
        }
        else {
            vecSystNames_In.assign(&arrCumulSystNames_ES_In[0], &arrCumulSystNames_ES_In[0] + numEnergySysts_In);
            vecSystNames_Out.assign(&arrCumulSystNames_ES_Out[0], &arrCumulSystNames_ES_Out[0] + numEnergySysts_Out);
            vecLegendName.assign(&arrSystLegNames[0], &arrSystLegNames[0] + numEnergySysts_Out);
            
            vecSystBands.resize(numEnergySysts_Out);
        }
    }
    void SetNamesNonObjES() {
        //Combining the non-object energy scale/resolution systematics
        const int numNonEnergySysts_In = 7;
        const int numNonEnergySysts_Out = 4;
        TString arrCumulSystNames_NonES_In[numNonEnergySysts_In] = {"LepEffSF", "BTagSF", "genRecoilRW", "TTBarNorm", "DYNorm", "EWKNorm", "FakeLepSyst"};
        TString arrCumulSystNames_NonES_Out[numNonEnergySysts_Out] = {"ObjEff", "ObjEff_GenRecoil", "ObjEff_GenRecoil_SampNorm", "ObjEff_GenRecoil_SampNorm_FakeLepSyst_NonES"};
        TString arrSystLegNames[numNonEnergySysts_Out] = {"Obj. Eff. SF", "Gen. Recoil", "Sample Norm.", "Fake Lep."};
        
        vecVecIndSystCombine.resize(0);
        //vector<int> vecObjEff = {0, 1};
        vector<int> vecObjEff;
        vecObjEff.push_back(0); vecObjEff.push_back(1);
        vecVecIndSystCombine.push_back(vecObjEff);
        //vector<int> vecGenRecoil = {2};
        vector<int> vecGenRecoil; vecGenRecoil.push_back(2);
        vecVecIndSystCombine.push_back(vecGenRecoil);
        //vector<int> vecObjNorm = {3, 5};
        vector<int> vecObjNorm; vecObjNorm.push_back(3); vecObjNorm.push_back(5);
        vecVecIndSystCombine.push_back(vecObjNorm);
        //vector<int> vecFakeLep = {6};
        vector<int> vecFakeLep; vecFakeLep.push_back(6);
        vecVecIndSystCombine.push_back(vecFakeLep);
        
        vecSystNames_In.assign(&arrCumulSystNames_NonES_In[0], &arrCumulSystNames_NonES_In[0] + numNonEnergySysts_In);
        vecSystNames_Out.assign(&arrCumulSystNames_NonES_Out[0], &arrCumulSystNames_NonES_Out[0] + numNonEnergySysts_Out);
        vecLegendName.assign(&arrSystLegNames[0], &arrSystLegNames[0] + numNonEnergySysts_Out);
        
        vecSystBands.resize(numNonEnergySysts_Out);
    }
    
    void SetNamesBigPair() {
        //Combining the Obj. ES and Non Obj. ES scale/resolution systematics
        const int numBigPairing = 2;
        TString arrCumulSystNames_BigPairing_In[numBigPairing] = {"ObjES", "NonES"};
        TString arrCumulSystNames_BigPairing_Out[numBigPairing] = {"ObjES", "ObjES_NonES_FullSyst"};
        TString arrSystLegNames[numBigPairing] = {"Energy Systs", "Non-Energy Systs"};
        
        vecVecIndSystCombine.resize(0);
        //vector<int> vecObjES = {0};
        vector<int> vecObjES;
        vecObjES.push_back(0);
        vecVecIndSystCombine.push_back(vecObjES);
        //vector<int> vecNonObjES = {1};
        vector<int> vecNonObjES;
        vecNonObjES.push_back(1);
        vecVecIndSystCombine.push_back(vecNonObjES);
        
        vecSystNames_In.assign(&arrCumulSystNames_BigPairing_In[0], &arrCumulSystNames_BigPairing_In[0] + numBigPairing);
        vecSystNames_Out.assign(&arrCumulSystNames_BigPairing_Out[0], &arrCumulSystNames_BigPairing_Out[0] + numBigPairing);
        vecLegendName.assign(&arrSystLegNames[0], &arrSystLegNames[0] + numBigPairing);
        
        vecSystBands.resize(numBigPairing);
    }
    
    void SetSystBands(AncillaryDrawingVariables * inADV, vector<TGraphAsymmErrors *> * inVecSysts, bool doVerbosity = 0) {
        //Function assumes you've set the various systematics names properly
        //(c.f. functions SetNamesObjES, SetNamesNonObjES, SetNamesBigPair above)
        vector<TGraphAsymmErrors *> tempVecSystBandsAll; //temp vector to containing all the systematics I care about for this instance of the struct
        vector<TGraphAsymmErrors *> tempVecSystBandsToCombine; //temp vector to contain systematics to combine into one
        TGraphAsymmErrors * tempCentVal; //patsy systematic band to work with the function

        
        inHist = (TH1F *) inADV->fracRatioHist;
        TAxis * YAxis = inHist->GetYaxis();
        TAxis * XAxis = inHist->GetXaxis();
        TString titleXAxis = XAxis->GetTitle();
        inADV->xADPs[1].SetStrings(titleXAxis, "");
        inADV->xADPs[1].SetAxisParams(XAxis);
        YAxis->SetNdivisions(3,5,0);
        XAxis->SetNdivisions(6,5,0);
        
        
        TH1F * proxyRatioHist = (TH1F *) inHist->Clone("Proxy Ratio");

        ResetRatioHist(proxyRatioHist);

        unsigned int numBands = vecSystBands.size();

        TString strBand;
        
        unsigned int currNumSystsToCombine;
        
        
        tempVecSystBandsAll = VecGraphs(inVecSysts, &vecSystNames_In);
        /*
        cout << "tempVecSystBandsAll size " << tempVecSystBandsAll.size() << endl;
        cout << "vecSystNames_Out size " << vecSystNames_Out.size() << endl;
         */
        for (unsigned int iBand = 0; iBand < numBands; ++iBand) {
            tempVecSystBandsToCombine.clear();

            currNumSystsToCombine = vecVecIndSystCombine[iBand].size();
            //cout << "currNumSystsToCombine " << currNumSystsToCombine << endl;
            for (unsigned int iSystCombine = 0; iSystCombine < currNumSystsToCombine; ++iSystCombine) {
                tempVecSystBandsToCombine.push_back(tempVecSystBandsAll[vecVecIndSystCombine[iBand][iSystCombine]]);
            }
            /*
            cout << "tempVecSystBandsToCombine size " << tempVecSystBandsToCombine.size() << endl;
            cout << "vecVecIndSystCombine[iBand][0] " << vecVecIndSystCombine[iBand][0] << endl;
            cout << "tempVecSystBandsToCombine[0] " << tempVecSystBandsToCombine[0] << endl;
            cout << "tempVecSystBandsToCombine[0] " << tempVecSystBandsToCombine[0]->GetName() << endl;
            cout << "vecSystNames_Out[iBand] " << vecSystNames_Out[iBand] << endl;
            */
            tempCentVal = (TGraphAsymmErrors *) tempVecSystBandsToCombine[0]->Clone(vecSystNames_Out[iBand] + TString("_CVPatsy"));
            
            if (doCumulativeSyst && iBand != 0) {
                tempVecSystBandsToCombine.push_back(vecSystBands[iBand - 1]);
            }
            vecSystBands[iBand] = GraphSystErrorSumErrors(tempCentVal, vecSystNames_Out[iBand], &tempVecSystBandsToCombine, false, proxyRatioHist, doVerbosity);
            //vecSystBands[iBand] = GraphSystErrorSumErrors(tempCentVal, vecSystNames_Out[iBand], &tempVecSystBandsToCombine, false, proxyRatioHist, true);
            SetBandStyle(iBand);
        }
    }
    void DrawBands(TVirtualPad * padToDrawOn, bool isBotPad, bool isRatio = true) {
        padToDrawOn->cd();

        TH1F * tempDrawHist;
        if (isBotPad) {
            tempDrawHist = (TH1F *) inHist->Clone("bottom Draw hist");
        }
        else {
            tempDrawHist = inHist;
        }
        
        
        TGraphAsymmErrors * fracRatioDrawGraph = ClonePoints(tempDrawHist, false, true);
        tempDrawHist->SetLineColor(kBlack);
        HistToGraphCopyAttributes(tempDrawHist, fracRatioDrawGraph);
        TH1F * patsy = (TH1F*) tempDrawHist->Clone("frac_patsy");
        patsy->SetLineColor(kWhite);
        patsy->SetMarkerColor(kWhite);
        patsy->Draw("e1");
        
        //tempDrawHist->Draw("e1");

        //TAxis * XAxis = tempDrawHist->GetXaxis();
        TAxis * XAxis = patsy->GetXaxis();
        /*
        float xMin = XAxis->GetXmin();
        float xMax = XAxis->GetXmax();
         */
        float xMin = XAxis->GetBinLowEdge(XAxis->GetFirst());
        float xMax = XAxis->GetBinUpEdge(XAxis->GetLast());
        
        if (isBotPad) {
            if (XAxis->GetLabelOffset() < 1) {
                XAxis->SetLabelOffset(0.02);
            }
            XAxis->SetTitleOffset(0.75);
            TString title = XAxis->GetTitle();
            title += "     ";
            XAxis->SetTitle(title);
        }
        else {
            XAxis->SetLabelOffset(5);
            XAxis->SetTitleOffset(5);
            XAxis->SetTitleSize(0.12);
        }
        
        
        for (unsigned int iSyst = vecSystBands.size(); iSyst > 0; --iSyst) {
//            vecSystBands[iSyst]->Draw("F same");
            TPolyLine * tempDrawBand = SystBand(tempDrawHist, vecSystBands[iSyst - 1]);
            tempDrawBand->Draw("f same");
            //vecSystBands[iSyst - 1]->Draw("3 same");
        }
        
        
        if (isRatio) {
            TLine * line = new TLine(xMin, 1, xMax, 1);
            line->SetLineColor(kRed+1);
            line->SetLineStyle(7);
            line->SetLineWidth(2);
            line->Draw("same");
        }
        
        TLegend * ratioLeg = new TLegend(0.13, 0.67, 0.45, 0.9);
        ratioLeg->SetNColumns(2);
        ratioLeg->AddEntry(tempDrawHist, tempDrawHist->GetYaxis()->GetTitle(), "p");
        for (unsigned int iSyst = vecSystBands.size(); iSyst > 0; --iSyst) {
            ratioLeg->AddEntry(vecSystBands[iSyst -1], vecLegendName[iSyst -1], "f");
//            vecSystBands[iSyst - 1]->Draw("3 same");
            

            //vecSystBands[iSyst - 1]->Draw("3 same");
        }
        
        fracRatioDrawGraph->SetMarkerStyle(20);
        fracRatioDrawGraph->Draw("p0 same");
        //tempDrawHist->Draw("e1 same");
        
        ratioLeg->Draw("same");
        
        padToDrawOn->Update();
        padToDrawOn->Modified();
    }
};




