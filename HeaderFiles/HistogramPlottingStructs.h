// Structs used as part of booking/showing histograms

#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include <vector>
using namespace std;


/// utilize the idea here for setting custom labels for plots
// like a vector of pairs of tstring with floats where you do find bin and set the axis label for said value
typedef struct {
    TString HistTXLabel;
    TString HistTYLabel;
    TString HistTZLabel;
    TString SampTLabel;
    
    int newXBinN;
    float newXMin, newXMax;
    
    int newYBinN;
    float newYMin, newYMax;
    
    int newZBinN;
    float newZMin, newZMax;    
    
} SpecHistBinT;


typedef struct SystT {
    TString name;
    float   fVar;
    double  dVar;
    TString  systVarKey;
    int     whichSystType;   // 0 = universal systematic, 1 = lepton systematic, 2 = jet systematic, 3 = other systematic
    
    //Copied over from StopFunctionDefinitions_v2.h
    // int whichSystType is used to demarcate between different systematics, with positive values designating a "shift up" systematic and negative values designating a "shift down" systematic -- note that systematics with no "shift" to them, i.e. GenTopRW which is unidirectional, are marked with a positive shift type by default
    // whichSystType number meanings:
    // MC based systematics
    // 1: Lepton Energy Scale
    // 2: Jet Energy Scale
    // 3: B RealTag Eff SF Shift
    // 4: B MisTag Eff SF Shift
    // 5: Jet Resolution (Smearing SF) Shift
    // 6: Unclustered Energy Scale
    // 7: Lepton Efficiency SF Shift
    // 8: Gen Recoil RW
    // 9: Stop XSec Shift
    
    // DD based systematics -- just Fake Lep for now
    // 1: Fake Lepton Estimate systematic
    
    void SetParams(vector<TString> * vecSystNames, vector<int> * vecWST, int index) {
        
        TString outString = "";
        systVarKey = "_";
        systVarKey += vecSystNames->at(abs(index));
        systVarKey += "Shift";
        if (index < 0) systVarKey += "Down";
        else if (index > 0) systVarKey += "Up";
        whichSystType = TMath::Sign(1, index) * vecWST->at(abs(index));
        name = systVarKey;
    }    
} SystT;

struct SpecialAxisBounds {
    vector< vector<Double_t> > vecVecBinEdges;
    vector<int> vecNumBins;
    void DefaultVarVals() {
        vecVecBinEdges.resize(0);
        vecNumBins.resize(0);
        

        AddInMT2llBounds_NonControl(&vecVecBinEdges, &vecNumBins); //1
        AddInMT2llBounds_Control(&vecVecBinEdges, &vecNumBins); //2

        AddInMT2lblbBounds_NonControl(&vecVecBinEdges, &vecNumBins); //3
        AddInMT2lblbBounds_Control(&vecVecBinEdges, &vecNumBins); //4

        AddInMT2bb_ZMETBounds_NonControl(&vecVecBinEdges, &vecNumBins); //5
        AddInMT2bb_ZMETBounds_Control(&vecVecBinEdges, &vecNumBins); //6
        
        AddInKT2_TopBounds(&vecVecBinEdges, &vecNumBins); //7

        AddInDilepPtBounds(&vecVecBinEdges, &vecNumBins); //8
        
        
        AddInMT2llBounds2DShape(&vecVecBinEdges, &vecNumBins); //9
        AddInMT2llBounds3DShape(&vecVecBinEdges, &vecNumBins); //10
        
        AddInMT2lblbBounds2DShape(&vecVecBinEdges, &vecNumBins); //11
        AddInMT2lblbBounds3DShape(&vecVecBinEdges, &vecNumBins); //12
        
        AddInMT2bb_ZMETBounds2DShape(&vecVecBinEdges, &vecNumBins); //13
        AddInMT2bb_ZMETBounds3DShape(&vecVecBinEdges, &vecNumBins); //14
        
        AddInKT2_TopBounds2DShape(&vecVecBinEdges, &vecNumBins); //15
        AddInKT2_TopBounds3DShape(&vecVecBinEdges, &vecNumBins); //16
        
        AddInQtBounds(&vecVecBinEdges, &vecNumBins); //17
        AddInUperpUparBounds(&vecVecBinEdges, &vecNumBins); //18
        AddInUparDivQtBounds(&vecVecBinEdges, &vecNumBins); //19
        AddInNVtxBounds(&vecVecBinEdges, &vecNumBins); //20
    }
    
    void PrintVals() {
        for (unsigned int iSpecAxis = 0; iSpecAxis < vecVecBinEdges.size(); ++iSpecAxis) {
            cout << "iSpecAxis " << iSpecAxis << endl;
            for (int iBin = 0; iBin <= vecNumBins[iSpecAxis]; ++iBin) {
                cout << "iBin " << iBin << endl;
                cout << "value is " << vecVecBinEdges[iSpecAxis][iBin] << endl;
            }
        }
    }
};

typedef struct InputHistBinBounds {
    int nBins;
    float axisLB, axisUB;
    void SetVals(int inNBins, float inAxisMin, float inAxisMax) {
        nBins = inNBins;
        axisLB = inAxisMin;
        axisUB = inAxisMax;
    }
} InputHistBinBounds;

typedef struct AxisT {
    TString axisLabel;
    int numBins;
    int rebinNum;
    float axisMin, axisMax;
    TString axisVarKey;
    bool doAxisSyst;
    float axisLB, axisUB;
    int axisSpecialType;
    int axisVarMapType; // 0 NonMET; 1 MET
    int axisVarCode;

    void SetAxis(TString inLabel, TString inAxisVarKey, int inNBins, float inAxisMin, float inAxisMax, bool inDoAxisSyst) {
        axisLabel = inLabel;
        axisVarKey = inAxisVarKey;
        numBins = inNBins;
        axisMin = inAxisMin;
        axisMax = inAxisMax;
        doAxisSyst = inDoAxisSyst;
    }
    void SetAxis(TString inLabel, TString inAxisVarKey, InputHistBinBounds * inHBB, bool inDoAxisSyst, float nBinsScale = 1) {
        axisLabel = inLabel;
        axisVarKey = inAxisVarKey;        
        numBins = (int) (inHBB->nBins * nBinsScale);
        axisMin = inHBB->axisLB;
        axisMax = inHBB->axisUB;
        doAxisSyst = inDoAxisSyst;
    }
    void SetAxisSpecial(TString inLabel, TString inAxisVarKey, int inAxisSpecialType, bool inDoAxisSyst) {
        axisLabel = inLabel;
        axisVarKey = inAxisVarKey;
        numBins = -1;
        axisSpecialType = inAxisSpecialType;
        doAxisSyst = inDoAxisSyst;
    }
    void SetAxisAsDefaultCountAxis(TString unitString, TString addString = "NUM ") {
        axisLabel = "Number of Events / ";
        axisLabel += addString;
        axisLabel += unitString;
    }
} AxisT;

typedef struct HistogramT {
    TString name;
    TString stringAxis;
    
    AxisT xAxis;
    AxisT yAxis;
    AxisT zAxis;
    
    bool xAxisAppend;
    
    bool yAxisAppend;
    
    bool zAxisAppend;
    
    SampCut specialHistCutFloat;
    SampCutInt specialHistCutInt;
    TString specialHistCutFloatVar, specialHistCutIntVar;
    
    bool logY1D;
    void DefaultVarVals() {
        specialHistCutFloat.doCut = false;
        specialHistCutInt.doCut = false;
    }
    AxisT * WhichAxis(int whichDim) {
        AxisT * AxisTToLook = 0;
        switch (whichDim) {
            case 1:
                AxisTToLook = &xAxis;
                break;
            case 2:
                AxisTToLook = &yAxis;
                break;
            case 3:
                AxisTToLook = &zAxis;
                break;
            default:                
                cout << "whichDim should be 1, 2, or 3 -- it is " << whichDim << endl;
                break;
        }
        return AxisTToLook;
    }
    void SetAxisString(TString titleString = "") {
        stringAxis = titleString;
        stringAxis += ";";
        stringAxis += xAxis.axisLabel;
        stringAxis += ";";
        stringAxis += yAxis.axisLabel;
        stringAxis += ";";
        stringAxis += zAxis.axisLabel;
    }
    void SetName(TString string1 = "", TString string2 = "", TString string3 = "") {
        name = "h_";
        name += string1;
        name += string2;
        name += string3;
    }
    void SetName(TString AppendString = "", int numDims = 1) {
        name = "h_";
        if (numDims > 0) {
            name += TString(xAxis.axisVarKey);
            if (numDims > 1) {
                name += "_vs_";
                name += TString(yAxis.axisVarKey);
                if (numDims > 2) {
                    name += "_vs_";
                    name += TString(zAxis.axisVarKey);
                }
            }
        }
        name += AppendString;
    }
    void SetIndAxisLabel(TString inputString, labelMap * mapVartoLabel, int whichDim = 1) {
        labelMap::iterator xIter;
        xIter = mapVartoLabel->find(inputString);
        TString label = "";
        if (xIter != mapVartoLabel->end()) {
            label = xIter->second;
            if (whichDim == 1) xAxis.axisLabel = label;
            else if (whichDim == 2) yAxis.axisLabel = label;
            else if (whichDim == 3) zAxis.axisLabel = label;           
        }
    }
    TH1F * BookOneDeeHist(SampleT * inSampleT, SpecialAxisBounds * inSAB) {
        TString histName = name;
        histName.Replace(histName.Index("PATSY"), 5, inSampleT->histNameSuffix);
        //        cout << "inHistSuffix " << inSampleT->histNameSuffix << endl;
        //        cout << "histName " << histName << endl;
        TH1F * outHist1D = 0;
        int nXBins;
        Double_t * xBinEdges;
        if (xAxis.numBins < 1) {
            nXBins = inSAB->vecNumBins[xAxis.axisSpecialType - 1];
            xBinEdges = &inSAB->vecVecBinEdges[xAxis.axisSpecialType - 1][0];
            outHist1D = new TH1F(histName, stringAxis, nXBins, xBinEdges);
        }
        else {
            outHist1D = new TH1F(histName, stringAxis, xAxis.numBins, xAxis.axisMin, xAxis.axisMax);
        }
        outHist1D->Sumw2();
        return outHist1D;
    }
    TH2F * BookTwoDeeHist(SampleT * inSampleT, SpecialAxisBounds * inSAB) {
        TString histName = name;
        histName.Replace(histName.Index("PATSY"), 5, inSampleT->histNameSuffix);
        //        cout << "histName " << histName << endl;
        TH2F * outHist2D = 0;
        int nXBins, nYBins;
        Double_t * xBinEdges, * yBinEdges;
        if (xAxis.numBins < 1) {
            nXBins = inSAB->vecNumBins[xAxis.axisSpecialType - 1];
            xBinEdges = &inSAB->vecVecBinEdges[xAxis.axisSpecialType - 1 ][0];
            if (yAxis.numBins < 1) {
                nYBins = inSAB->vecNumBins[yAxis.axisSpecialType - 1];
                yBinEdges = &inSAB->vecVecBinEdges[yAxis.axisSpecialType - 1][0];
                outHist2D = new TH2F(histName, stringAxis, nXBins, xBinEdges, nYBins, yBinEdges);
            }
            else {
                outHist2D = new TH2F(histName, stringAxis, nXBins, xBinEdges, yAxis.numBins, yAxis.axisMin, yAxis.axisMax);
            }
        }
        else {
            if (yAxis.numBins < 1) {
                nYBins = inSAB->vecNumBins[yAxis.axisSpecialType - 1];
                yBinEdges = &inSAB->vecVecBinEdges[yAxis.axisSpecialType - 1][0];
                outHist2D = new TH2F(histName, stringAxis, xAxis.numBins, xAxis.axisMin, xAxis.axisMax, nYBins, yBinEdges);
            }
            else {
                outHist2D = new TH2F(histName, stringAxis, xAxis.numBins, xAxis.axisMin, xAxis.axisMax, yAxis.numBins, yAxis.axisMin, yAxis.axisMax);
            }
        }
        outHist2D->Sumw2();
        return outHist2D;
    }
    TH3F * BookThreeDeeHist(SampleT * inSampleT, SpecialAxisBounds * inSAB) {
        TString histName = name;
        histName.Replace(histName.Index("PATSY"), 5, inSampleT->histNameSuffix);
        //        cout << "histName " << histName << endl;
        TH3F * outHist3D = 0;
        int nXBins, nYBins, nZBins;
        Double_t * xBinEdges, * yBinEdges, * zBinEdges;
        if (xAxis.numBins < 1 || yAxis.numBins < 1 || zAxis.numBins < 1) {
            if (xAxis.numBins > 0 || yAxis.numBins > 0 || zAxis.numBins > 0) {
                cout << "Given that one of the axes has a special axis setting, all of them have to for 3D hists!! " << endl;
                cout << "For debugging purposes, this issue ^ happened with the hist " << name << endl;
            }
            else {
                nXBins = inSAB->vecNumBins[xAxis.axisSpecialType - 1];
                xBinEdges = &inSAB->vecVecBinEdges[xAxis.axisSpecialType - 1][0];
                nYBins = inSAB->vecNumBins[yAxis.axisSpecialType - 1];
                yBinEdges = &inSAB->vecVecBinEdges[yAxis.axisSpecialType - 1][0];
                nZBins = inSAB->vecNumBins[zAxis.axisSpecialType - 1];
                zBinEdges = &inSAB->vecVecBinEdges[zAxis.axisSpecialType - 1][0];
                outHist3D = new TH3F(histName, stringAxis, nXBins, xBinEdges, nYBins, yBinEdges, nZBins, zBinEdges);
            }
        }
        else {
            outHist3D = new TH3F(histName, stringAxis, xAxis.numBins, xAxis.axisMin, xAxis.axisMax, yAxis.numBins, yAxis.axisMin, yAxis.axisMax, zAxis.numBins, zAxis.axisMin, zAxis.axisMax);
        }
        outHist3D->Sumw2();
        return outHist3D;
    }
    
    bool EventPassesHistSpecificCut_Float(StV_Map * inStVM, bool doVerbosity = false) {
        bool passesCut = true;
        StV_Map::iterator xIter;
        if (specialHistCutFloat.doCut) {
            if (TString(specialHistCutFloatVar).Length() == 0) {
                cout << "something wrong with trying to cut on the float, length is 0!" << endl;
                passesCut = false;
            }
            else {
                xIter = inStVM->find(specialHistCutFloatVar);
                if (xIter != inStVM->end()) {
                    if (doVerbosity) {
                        cout << "in the Histogram Internal Cut " << endl;
                        cout << "xIter first " << xIter->first << endl;
                        cout << "xIter second " << xIter->second << endl;
                    }
                    passesCut = (specialHistCutFloat.PassCut(xIter->second, doVerbosity));
                }
                else {
                    cout << "For the Histogram Integral Cut, I couldn't find the key: " << specialHistCutFloatVar << " in the map! " << endl;
                    passesCut = false;
                }
            }
        }
        return passesCut;
    }
    bool EventPassesHistSpecificCut(StV_Map * inStVM, bool doVerbosity = false) {
        bool passesFloatCut = EventPassesHistSpecificCut_Float(inStVM, doVerbosity);
        bool passesIntCut = true;
//        bool passesIntCut = EventPassesHistSpecificCut_Int(inStVM);
        return passesFloatCut && passesIntCut;       
    }
    bool HistContainsStringAsAxisVar(TString inString, int numDims) {
        bool containsString = false;
        if (numDims < 1 || numDims > 3) {
            cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << endl;
        }
        for (int iAxis = 1; iAxis <= numDims; ++iAxis) {
            if (TString(WhichAxis(iAxis)->axisVarKey).Contains(inString)) {
                containsString = true;
            }
        }
        return containsString;
    }    
} HistogramT;
inline bool operator<(const HistogramT &a, const HistogramT &b)
{
    return (a.name < b.name) || (a.xAxis.axisLabel < b.xAxis.axisLabel);
}

typedef std::pair<HistogramT, TString> histKeyString;

inline bool operator<(const histKeyString &a, const histKeyString &b)
{
    return (a.first.name < b.first.name) || (a.first.name == b.first.name && a.second < b.second);
}
inline bool operator<(const SampleT &a, const SampleT &b)
{
    return (a.histNameSuffix < b.histNameSuffix) || (a.histXaxisSuffix < b.histXaxisSuffix && a.histNameSuffix == b.histNameSuffix);
}
inline SampleT operator+(const SampleT &a, const SampleT &b)
{
    SampleT outSampleT;
    outSampleT.histNameSuffix = a.histNameSuffix + b.histNameSuffix;
    outSampleT.histXaxisSuffix = a.histXaxisSuffix + b.histXaxisSuffix;
    outSampleT.histYaxisSuffix = a.histYaxisSuffix + b.histYaxisSuffix;
    outSampleT.histZaxisSuffix = a.histZaxisSuffix + b.histZaxisSuffix;
    return outSampleT;
}


inline int HistID(vector<HistogramT> * inputHistTVec, TString inputSearchString) {
    if (inputSearchString.Contains(" ")) {
        inputSearchString.Replace(0, string(inputSearchString).find_first_not_of(" "), "");
        inputSearchString.Replace(string(inputSearchString).find_last_not_of(" ")+ 1, inputSearchString.Length(), "");
    }
    int counter = 0;
    int histID = -1;
    TString currHistName;
    for (unsigned int iHist = 0; iHist < inputHistTVec->size(); ++iHist) {
        currHistName = "";
        currHistName += inputHistTVec->at(iHist).name;
        if (currHistName.EqualTo(inputSearchString)) {
            histID = counter;
            cout << "found the Hist! ID is " << histID << endl;
        }
        else {
            ++counter;
        }
    }
    return histID;
}
inline int ChannelID(vector<SampleT> * inputSubSampVec, TString inputSearchString) {
    if (inputSearchString.Contains(" ")) {
        inputSearchString.Replace(0, string(inputSearchString).find_first_not_of(" "), "");
        inputSearchString.Replace(string(inputSearchString).find_last_not_of(" ")+ 1, inputSearchString.Length(), "");
    }
    int counter = 0;
    int chanID = -1;
    TString currSubSampName;
    for (unsigned int iSamp = 0; iSamp < inputSubSampVec->size(); ++iSamp) {
        currSubSampName = "";
        currSubSampName += inputSubSampVec->at(iSamp).histNameSuffix;
        if (currSubSampName.EqualTo(inputSearchString)) {
            chanID = counter;
            cout << "found the Chan! ID is " << chanID << endl;
        }
        else {
            ++counter;
        }
    }
    return chanID;
}
inline void SetMultiChannelStrings(vector<TString> * vecMultChanString, vector<SampleT> * subSampVec, vector<int> * multiChannelIDs, TString histPlotName) {
    TString currHistGrabName;
    for (unsigned int iChan = 0; iChan < multiChannelIDs->size(); ++iChan) {
        currHistGrabName = histPlotName;
        currHistGrabName += subSampVec->at(multiChannelIDs->at(iChan)).histNameSuffix;
        vecMultChanString->push_back(currHistGrabName);
    }
}



