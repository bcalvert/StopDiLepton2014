#ifndef MT2CALCULATIONS_H
#define MT2CALCULATIONS_H

////////////////////////////////////////                                                                                                                                     
/// \file <MT2Calculations.h>
/// Synopsis: Contains function used for calcClass for Merge-sort algorithm
////////////////////////////////////////  

#include "TLorentzVector.h"
#include "./mt2bisect.h"

namespace MT2 {
    float getMT2(TLorentzVector const &lv_Lept1, TLorentzVector const &lv_Lept2, float const &fMET, float const &fMETPhi, float const &fInputTestMass = 0);
} // MT2

#endif // MT2CALCULATIONS_H