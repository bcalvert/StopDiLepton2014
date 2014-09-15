// General set of definitions of structs (along with some functions....) 
// a lot of these structs have been appropriated into separate files to aid organization
#include "TLorentzVector.h"
#include "TRandom.h"
#include <iostream>
#include <vector>
#include <TROOT.h>
#include <iostream>
#include <fstream>
//#include <vector>
#include <cmath>
#include <sstream>
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"

using namespace std;

typedef struct SampleT {
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
    
} SampleT;




//typedef pair<HistogramT, SampleT> histKey;


inline float DeltaMT2UncEn(std::vector<TH1F *> * vecOneDeeHists, TH2F * TwoDeeHist, float inputMT2Value) {
    //returns float which is a random number drawn from the distribution of MT2 Central Value minus MT2 Unclustered ES shifted version
    
    int whichOneDeeHist = TwoDeeHist->GetXaxis()->FindBin(inputMT2Value) - 1;
    unsigned int numOneDeeHists = vecOneDeeHists->size();
    if (whichOneDeeHist < 0) {
        std::cout << inputMT2Value << std::endl;
        std::cout << "ERROR with which one dee hist!" << std::endl;
        return 0.;
    }
    else if (whichOneDeeHist >= (int) numOneDeeHists) {
        whichOneDeeHist = (int) numOneDeeHists - 1;
    }
    if (vecOneDeeHists->at(whichOneDeeHist)->Integral() == 0) {
        return 0;
    }
    else {
        return vecOneDeeHists->at(whichOneDeeHist)->GetRandom();   
    }
}