#ifndef GraphParams_h
#define GraphParams_h

#include "TGraphAsymmErrors.h" //will intrinsically include TGraph and TGraph Errors

typedef pair<float, float> pairFloats;
struct GraphParams {
    Style_t styleLine, styleMarker, styleFill;
    
    Color_t colorLine, colorFill;
    Width_t widthLine;
    
    TString baseName, baseTitle;
    
    void DefaultVals();
    void SetAsBasicLimitContour(bool, int);
    void SetAsBasicSignifContour(int, int);
    void SetAsAdditionalLimitContour(int);
    void SetInputTGraph(TGraph *);
    
    
    void SetInputTGraphForLegendHack(TGraph *, pairFloats *, pairFloats *, pairFloats *, bool, int, bool);
    
    void SetAsKinematicDiagonalContour();
    void SetAsWhiteBackgroundGraphParams();
};



#endif