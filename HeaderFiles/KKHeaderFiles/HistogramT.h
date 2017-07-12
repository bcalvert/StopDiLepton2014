#ifndef HISTOGRAM_T_H
#define HISTOGRAM_T_H

#include "TString.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include <vector>
#include <iostream>

#include "AxisT.h"
#include "SampleCut.h"
#include "SampleT.h"
#include "SpecialAxisBounds.h"
// Structs used as part of booking/showing histograms
struct HistogramT {
    TString name;
    TString stringAxis;
    
    AxisT xAxis;
    AxisT yAxis;
    AxisT zAxis;
    
    bool xAxisAppend;
    
    bool yAxisAppend;
    
    bool zAxisAppend;
    
    SampleCut<float> specialHistCutFloat;
    SampleCut<int> specialHistCutInt;
    TString specialHistCutFloatVar, specialHistCutIntVar;
    
    bool logY1D;
    void DefaultVarVals() {
        specialHistCutFloat.m_bDoCut = false;
        specialHistCutInt.m_bDoCut = false;
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
                std::cout << "whichDim should be 1, 2, or 3 -- it is " << whichDim << std::endl;
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
        //        std::cout << "inHistSuffix " << inSampleT->histNameSuffix << std::endl;
        //        std::cout << "histName " << histName << std::endl;
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
        //        std::cout << "histName " << histName << std::endl;
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
        //        std::cout << "histName " << histName << std::endl;
        TH3F * outHist3D = 0;
        int nXBins, nYBins, nZBins;
        Double_t * xBinEdges, * yBinEdges, * zBinEdges;
        if (xAxis.numBins < 1 || yAxis.numBins < 1 || zAxis.numBins < 1) {
            if (xAxis.numBins > 0 || yAxis.numBins > 0 || zAxis.numBins > 0) {
                std::cout << "Given that one of the axes has a special axis setting, all of them have to for 3D hists!! " << std::endl;
                std::cout << "For debugging purposes, this issue ^ happened with the hist " << name << std::endl;
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
    
    /*
    bool EventPassesHistSpecificCut_Float(StV_Map * inStVM, bool doVerbosity = false) {
        bool passesCut = true;
        StV_Map::iterator xIter;
        if (specialHistCutFloat.m_bDoCut) {
            if (TString(specialHistCutFloatVar).Length() == 0) {
                std::cout << "something wrong with trying to cut on the float, length is 0!" << std::endl;
                passesCut = false;
            }
            else {
                xIter = inStVM->find(specialHistCutFloatVar);
                if (xIter != inStVM->end()) {
                    if (doVerbosity) {
                        std::cout << "in the Histogram Internal Cut " << std::endl;
                        std::cout << "xIter first " << xIter->first << std::endl;
                        std::cout << "xIter second " << xIter->second << std::endl;
                    }
                    passesCut = (specialHistCutFloat.PassCut(xIter->second, doVerbosity));
                }
                else {
                    std::cout << "For the Histogram Integral Cut, I couldn't find the key: " << specialHistCutFloatVar << " in the map! " << std::endl;
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
    */
    bool HistContainsStringAsAxisVar(TString inString, int numDims) {
        bool containsString = false;
        if (numDims < 1 || numDims > 3) {
            std::cout << "num of Dims should be 1, 2, or 3 -- it is " << numDims << std::endl;
        }
        for (int iAxis = 1; iAxis <= numDims; ++iAxis) {
            if (TString(WhichAxis(iAxis)->axisVarKey).Contains(inString)) {
                containsString = true;
            }
        }
        return containsString;
    }    
};
inline bool operator<(const HistogramT &a, const HistogramT &b)
{
    return (a.name < b.name) || (a.xAxis.axisLabel < b.xAxis.axisLabel);
}

typedef std::pair<HistogramT, TString> histKeyString;
#endif // HISTOGRAM_T_H
