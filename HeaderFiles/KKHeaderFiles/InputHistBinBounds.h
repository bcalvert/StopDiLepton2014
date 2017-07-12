#ifndef INPUT_HIST_BIN_BOUNDS_H
#define INPUT_HIST_BIN_BOUNDS_H

struct InputHistBinBounds {
    int nBins;
    float axisLB, axisUB;
    void SetVals(int inNBins, float inAxisMin, float inAxisMax) {
        nBins = inNBins;
        axisLB = inAxisMin;
        axisUB = inAxisMax;
    }
};

#endif // INPUT_HIST_BIN_BOUNDS_H

