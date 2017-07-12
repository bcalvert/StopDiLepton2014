#ifndef SPECIAL_AXIS_BOUNDS_H
#define SPECIAL_AXIS_BOUNDS_H


#include "HistogramPlottingFunctions.h"
#include "HistogramPlottingFunctions_METPerformance.h"
#include <vector>

struct SpecialAxisBounds {
    std::vector< std::vector<Double_t> > vecVecBinEdges;
    std::vector<int> vecNumBins;
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
            std::cout << "iSpecAxis " << iSpecAxis << std::endl;
            for (int iBin = 0; iBin <= vecNumBins[iSpecAxis]; ++iBin) {
                std::cout << "iBin " << iBin << std::endl;
                std::cout << "value is " << vecVecBinEdges[iSpecAxis][iBin] << std::endl;
            }
        }
    }
};
#endif // SPECIAL_AXIS_BOUNDS_H
