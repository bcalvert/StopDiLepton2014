// function used for calculating MT2 with given input TLorentzVectors, and a MET value
#include "TLorentzVector.h"
#include "../PlotMakingCode/mt2bisect.h"

float    getMT2(TLorentzVector, TLorentzVector, float, float, float);


inline float getMT2(TLorentzVector lept1, TLorentzVector lept2, float theMET, float theMETphi, bool useMassless, float inputTestMass = 0){
    // Calculate MT2 variable for two leptons and missing energy, assuming zero testmass
    double pa[3];
    double pb[3];
    double pmiss[3];
    
    TLorentzVector pmet;
    pmet.SetPtEtaPhiM(theMET, 0., theMETphi, 0.);
    pmiss[0] = 0.; // irrelevant
    pmiss[1] = pmet.Px();
    pmiss[2] = pmet.Py();
    
    pa[1] = lept1.Px();
    pa[2] = lept1.Py();
    
    pb[1] = lept2.Px();
    pb[2] = lept2.Py();
    
    
    if (useMassless) {
        pa[0] = 0.;
        pb[0] = 0;
    }
    else {
        pa[0] = lept1.M();
        pb[0] = lept2.M();
    }
    
    mt2bisect* MT2bisect = new mt2bisect();
    MT2bisect->set_verbose(0);
    MT2bisect->set_momenta(pa, pb, pmiss);
    MT2bisect->set_mn(inputTestMass); // testmass
    double MT2 = MT2bisect->get_mt2();
    delete MT2bisect;
    return MT2;
}

inline float getMT2MN(TLorentzVector lept1, TLorentzVector lept2, float theMET, float theMETphi, bool useMassless, float inputMass){
    // Calculate MT2 variable for two leptons and missing energy, assuming zero testmass
    double pa[3];
    double pb[3];
    double pmiss[3];
    
    TLorentzVector pmet;
    pmet.SetPtEtaPhiM(theMET, 0., theMETphi, 0.);
    pmiss[0] = 0.; // irrelevant
    pmiss[1] = pmet.Px();
    pmiss[2] = pmet.Py();
    
    pa[1] = lept1.Px();
    pa[2] = lept1.Py();
    
    pb[1] = lept2.Px();
    pb[2] = lept2.Py();
    
    if (useMassless) {
        pa[0] = 0.;
        pb[0] = 0;
    }
    else {
        pa[0] = lept1.M();
        pb[0] = lept2.M();
    }
    
    mt2bisect* MT2bisect = new mt2bisect();
    MT2bisect->set_verbose(0);
    MT2bisect->set_momenta(pa, pb, pmiss);
    MT2bisect->set_mn(inputMass); // testmass
    double MT2 = MT2bisect->get_mt2();
    delete MT2bisect;
    return MT2;
}
