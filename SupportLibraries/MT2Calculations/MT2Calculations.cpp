
#include "MT2Calculations.h"


/**
 * @brief      Calculates MT2 for for an input three-body system
 * Assumption is that two of the input Lorentz Vectors are visible while a third represent the invisible (MET)
 *
 * @param      lv_lv_Lept1        Lorentz Vector for first visible particle
 * @param      lv_lv_Lept2        Lorentz Vector for second visible particle
 * @param      fMET            Measured value of the MET
 * @param      fMETPhi         Measured value of the MET Phi
 * @param      fInputTestMass  Test Mass of the invisible particle (neutrino, W-boson, etc.)
 *
 * @return     Calculated MT2 value
 */
float MT2::getMT2(TLorentzVector const &lv_Lept1, TLorentzVector const &lv_Lept2, float const &fMET, float const &fMETPhi, float const &fInputTestMass) {
    double pa[3];
    double pb[3];
    double pmiss[3];

    TLorentzVector pmet;
    pmet.SetPtEtaPhiM(fMET, 0., fMETPhi, 0.);
    pmiss[0] = 0.; // irrelevant                                                                                                                               
    pmiss[1] = pmet.Px();
    pmiss[2] = pmet.Py();
      
    pa[0] = 0.;
    pa[1] = lv_Lept1.Px();
    pa[2] = lv_Lept1.Py();
      
    pb[0] = 0.;
    pb[1] = lv_Lept2.Px();
    pb[2] = lv_Lept2.Py();
      
    mt2bisect* MT2bisect = new mt2bisect();
    MT2bisect->set_verbose(0);
    MT2bisect->set_momenta(pa, pb, pmiss);
    MT2bisect->set_mn(fInputTestMass); // testmass
    double MT2 = MT2bisect->get_mt2();
    delete MT2bisect;
    return MT2;
}