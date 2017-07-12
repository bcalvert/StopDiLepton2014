#ifndef SAMPLE_T_H
#define SAMPLE_T_H

#include <vector>

struct SampleT {
    /// \todo implement as vector of cuts

    //    SampleT() : histNameSuffix(""), whichdiLepType(-1), doZVeto(-1), cutNJets(-1), cutNBJets(-1), cutMET(0) {}
    TString histNameSuffix;
    TString histXaxisSuffix;
    TString histYaxisSuffix;
    TString histZaxisSuffix;
    /// variables to store what kinds of
    int     whichdiLepType; // -1: inclusive, 0: MuMu, 1: EE, 2: EMu
    int     doZVeto;   // -1: inclusive, 0: ZMass window, 1: outside ZMass window;
    int     cutNJets;  // -1: inclusive, # > 0: require NJets >= #
    int     cutNBJets;  // -1: inclusive, # > 0: require NBJets >= #
    float   cutMET; // # > 0: require MET >= #
    bool    blindDataChannel;
    
    void DefaultVarVals() {
        doZVeto = -1;
        cutNJets = -1;
        cutNBJets = -1;
        cutMET = 0;
        whichdiLepType = -1;
        blindDataChannel = 0;
        histNameSuffix = "";
    }
    
    void SetAsFullCut() {
        whichdiLepType = -1;
        doZVeto = 1;
        cutNJets = 2;
        cutNBJets = 1;
        cutMET = 40;
    }
    
};


void SetSubSampVec(std::vector<SampleT> * subSampVec) {   
    SampleT events_METGeq40; events_METGeq40.histNameSuffix = "_METGeq40"; events_METGeq40.histXaxisSuffix = "";
    events_METGeq40.cutMET = 40.;    
    
    //"Inclusive subsample"//
    SampleT allEvents; allEvents.DefaultVarVals();
    allEvents.histNameSuffix = "_inclusive"; 
    
    subSampVec->push_back(events_METGeq40);    
    subSampVec->push_back(allEvents);
}

#endif // SAMPLE_T_H
