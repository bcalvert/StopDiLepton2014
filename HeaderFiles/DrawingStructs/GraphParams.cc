

#ifndef GraphParams_cc
#define GraphParams_cc
typedef pair<float, float> pairFloats;

void GraphParams::DefaultVals() {
    colorLine = kBlack;
    styleLine = 1;
    widthLine = 1;
    
    colorFill = kWhite;
    styleFill = 1001;
    
    styleMarker = 0;
    
    baseName = "graph";
    baseTitle = "graph";
}

/*
void GraphParams::SetInputTGraphForKinematicDiagonal(TGraph &inputTGraph, TString addName, float diagX, float diagY) {
    inputTGraph = TGraph(3, diagX, diagY);
    SetInputTGraph(inputTGraph);
    inputTGraph->SetName(TString("kinematic_diagonal_") + addName);
}
*/

void GraphParams::SetInputTGraphForLegendHack(TGraph * inputTGraph, pairFloats * baseStartValsXY, pairFloats * xPair, pairFloats * yPair, bool isObs, int whichExp, bool doVerb = false) {
    
    TString baseNameHack = "LGraph_";
    baseNameHack += isObs ? "Obs" : "Exp";
    baseNameHack += whichExp;
    
    SetInputTGraph(inputTGraph);
    inputTGraph->Set(2);
    inputTGraph->SetName(baseNameHack);
    inputTGraph->SetTitle(baseNameHack);
    
    float xVal_Point0 = xPair->first + (baseStartValsXY->first - 8) * (xPair->second/100);
    float xVal_Point1 = xPair->first + (baseStartValsXY->first - 1) * (xPair->second/100);
    
    float baseUnitMove = 0.05;
    
    vector<float> vecFloatWhichExp{0.0, -1 * baseUnitMove, baseUnitMove};
    
    float baseVal = (baseStartValsXY->second - baseUnitMove);
    cout << "baseVal " << baseVal << endl;
    //- 8) isObs ? (0.5 - baseUnitMove) : (0.25 - baseUnitMove);
    cout << " vecFloatWhichExp[whichExp] " <<  vecFloatWhichExp[whichExp] << endl;
    cout << "yPair->second " << yPair->second << endl;
    cout << "yPair->first " << yPair->first << endl;
    cout << "(baseVal + vecFloatWhichExp[whichExp]) * (yPair->second - yPair->first) " << (baseVal + vecFloatWhichExp[whichExp]) * (yPair->second - yPair->first) << endl;
    
    float yVal = yPair->first + (baseVal + vecFloatWhichExp[whichExp]) * (yPair->second - yPair->first);
    cout << "yVal " << yVal << endl;
    /*
     float yVal_Point0 = yPair->first - vecYCentToUse->at(whichExp) * (yPair->second / 10);
     float yVal_Point1 = yPair->first - vecYCentToUse->at(whichExp) * (yPair->second / 10);
     */
    
    if (doVerb) {
        cout << "Point 0 -- (x:y) = (" << xVal_Point0 << ":" << yVal << ")" << endl;
        cout << "Point 1 -- (x:y) = (" << xVal_Point1 << ":" << yVal << ")" << endl;
    }
    
    inputTGraph->SetPoint(0, xVal_Point0, yVal);
    inputTGraph->SetPoint(1, xVal_Point1, yVal);
}

/*
void GraphParams::SetInputTGraphForLegendHack(TGraph * inputTGraph, pairFloats * xPair, pairFloats * yPair, bool isObs, int whichExp, bool doVerb = false) {
    
    TString baseNameHack = "LGraph_";
    baseNameHack += isObs ? "Obs" : "Exp";
    baseNameHack += whichExp;
    
    SetInputTGraph(inputTGraph);
    inputTGraph->Set(2);
    inputTGraph->SetName(baseNameHack);
    inputTGraph->SetTitle(baseNameHack);
    
    float xVal_Point0 = xPair->first + 3 * (xPair->second/100);
    float xVal_Point1 = xPair->first + 10 * (xPair->second/100);

    float baseUnitMove = 0.05;
    
    vector<float> vecFloatWhichExp{0.0, -1 * baseUnitMove, baseUnitMove};
    
    float baseVal = isObs ? (0.5 - baseUnitMove) : (0.25 - baseUnitMove);
    
    
    
    float yVal = yPair->first + (baseVal + vecFloatWhichExp[whichExp]) * (yPair->second - yPair->first);
    cout << "yVal " << yVal << endl;

    
    if (doVerb) {
        cout << "Point 0 -- (x:y) = (" << xVal_Point0 << ":" << yVal << ")" << endl;
        cout << "Point 1 -- (x:y) = (" << xVal_Point1 << ":" << yVal << ")" << endl;
    }
    
    inputTGraph->SetPoint(0, xVal_Point0, yVal);
    inputTGraph->SetPoint(1, xVal_Point1, yVal);
}
*/

void GraphParams::SetInputTGraph(TGraph * inputTGraph) {
    inputTGraph->SetLineColor(colorLine);
    inputTGraph->SetLineStyle(styleLine);
    inputTGraph->SetLineWidth(widthLine);
    
    inputTGraph->SetMarkerStyle(styleMarker);
    
    inputTGraph->SetFillColor(colorFill);
    inputTGraph->SetFillStyle(styleFill);
    
    inputTGraph->SetName(baseName);
    inputTGraph->SetTitle(baseTitle);
}
void GraphParams::SetAsAdditionalLimitContour(int whichIndex) {
    styleLine = 1;
    widthLine = 4;
    
    styleMarker = 20;
    colorFill = kWhite;
    
    vector<Color_t> vecColorLine{kRed, kBlue, kMagenta, kGreen};
    colorLine = vecColorLine[whichIndex];
}
void GraphParams::SetAsBasicSignifContour(int whichExpSig, int whichExp) {
    vector<Color_t> vecColorLine{kRed, kBlue, kMagenta, kGreen};
    colorLine = vecColorLine[whichExpSig];
    
    styleLine = whichExp > 0 ? 7 : 1;
    
    widthLine = whichExp > 0 ? 2 : 4;
    styleMarker = 20;
    
    colorFill = kWhite;
}
void GraphParams::SetAsBasicLimitContour(bool isObs, int whichExp) {
    colorLine = isObs ? kBlack : kRed;

    styleLine = whichExp > 0 ? 7 : 1;
    
    widthLine = whichExp > 0 ? 2 : 4;
    
//    styleMarker = isObs && whichExp == 0 ? 20 : 0;
    styleMarker = 20;
    
    colorFill = kWhite;
    /*
    cout << "colorLine " << colorLine << endl;
    cout << "styleLine " << styleLine << endl;
    cout << "widthLine " << widthLine << endl;
    cout << "styleMarker " << styleMarker << endl;
    cout << "colorFill " << colorFill << endl;
    */
}

void GraphParams::SetAsKinematicDiagonalContour() {
    colorLine = kGray;
    styleLine = 2;
    colorFill = kWhite;
}

void GraphParams::SetAsWhiteBackgroundGraphParams() {
    colorLine = kBlack;
    styleLine = 1;
    widthLine = 3;
    colorFill = kWhite;
    styleFill = 1001;
    
    baseName = "white";
    baseTitle = "white";
}


#endif