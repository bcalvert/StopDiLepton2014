// Key set of functions utilized as part of automatically grabbing the right of reco particle info when filling histograms
// The crux of is setting up a boolean map which specifies for a given systematic, whether or not to grab central value version of the 
// Reco level Event Info Struct -- MET, Leptons, etc. -- or whether to grab the appropriate systematic variation


using namespace std;

inline void SetMapIntBoolFakeLep(mIB &inMapIntBool, int typeMapIntBool) {
    int numSysts = 3;
    /*
    int systFakeStat = 1;
    int systFakeSystFakeRate = 2;
    int systFakeSystPromptRate = 3;
    */
    inMapIntBool[0] = false; //central value should never grab syst
    mIB::iterator x;
    for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
        switch (typeMapIntBool) {
            case 0:
                inMapIntBool[iSyst] = true;
                break;
            default:
                inMapIntBool[iSyst] = false;
                break;
        }
    }
}

inline void SetMapIntBoolPDF(mIB &inMapIntBool) {
    int numNormSysts = 1;
    int numSysts_CT10 = 23;
    int numSysts_MSTW = 25;
    int numSysts_NNPDF = 50;
    int numSystsTotal = numNormSysts + numSysts_CT10 + numSysts_MSTW + numSysts_NNPDF;

    inMapIntBool[0] = false; //central value should never grab syst
    for (int iSyst = 1; iSyst <= numSystsTotal; ++iSyst) {
        inMapIntBool[iSyst] = true;
    }
}

inline void SetMapIntBool(mIB &inMapIntBool, int typeMapIntBool, bool isSmear = false) {
    // Set up the map of systematic integers to booleans
    
    
    // typeMapIntBool: 0 BEI, 1:ELI, 2: EJI, 3: EMI, 4:EDSI
    
    // keep the below list consistent!!!! I know I know, I should have a better way of doing this
    const int numSysts = 9;    
    int systLepES = 1;
    int systJetES = 2;
    int systBTagEffSF = 3;
    int systBMisTagSF = 4;
    int systJetSmear = 5;
    int systUncES = 6;
    int systLepEff = 7;
    int systGenRecoilRW = 8;
    int systStopXSec = 9;
    inMapIntBool[0] = false; //central value should never grab syst
    mIB::iterator x;
    for (int iSyst = 1; iSyst <= numSysts; ++iSyst) {
        switch (typeMapIntBool) {
            case 0:
                //BEI
                
                // Basic Event Info only grabs either shifts in weight (lepton, gen Recoil, stop xSection) 
                // or possibly different lepton scale factors given a different dilepton set (which can happen with lepton ES shifts)
                if (iSyst == systLepES || iSyst == systLepEff || iSyst == systGenRecoilRW || iSyst == systStopXSec) {
                    inMapIntBool[iSyst] = true;
                }
                else {
                    inMapIntBool[iSyst] = false;
                }
                break;
            case 1:
                //ELI
                
                // Event Lepton Info only cares about shifts in lepton energy scale
                if (iSyst == systLepES) {
                    inMapIntBool[iSyst] = true;
                }
                else {
                    inMapIntBool[iSyst] = false;
                }
                break;
            case 2:
                //EJI
                
                // Unsmeared Event Jet Info only cares about b-tagging efficiencies and jet energy scale
                // When you start adding in smearing you also care about jet smearing systematics
                if (iSyst == systJetES || iSyst == systBTagEffSF || iSyst == systBMisTagSF) {
                    inMapIntBool[iSyst] = true;
                }
                else if (iSyst == systJetSmear) {
                    inMapIntBool[iSyst] = isSmear;
                }
                else {
                    inMapIntBool[iSyst] = false;
                }
                break;
            case 3:
                //EMI
                
                // Event MET info does not care about shifts in weights and for the most part doesn't care about b-tagging efficiencies
                // If we start harping on type of MT2lblb -- i.e. whether or not input "B" jets are actually tagged as b-jets, then we have to switch this
                if (iSyst == systJetSmear || iSyst == systUncES) {
                    inMapIntBool[iSyst] = isSmear;
                }
                else if (iSyst == systBMisTagSF || iSyst == systBTagEffSF || iSyst == systLepEff || iSyst == systGenRecoilRW || iSyst == systStopXSec) {
                    inMapIntBool[iSyst] = false;
                }
                else {
                    inMapIntBool[iSyst] = true;
                }
                break;
            case 4:
                //EDSI
                
                // Event Distructure info doesn't care about event weights, and only cares about jet smearing and unclustered energy scale if there is smearing
                // nominally could add in unclustered energy scale systematic using Matthieu's nTuples based on unsmeared MET but haven't bothered yet
                if (iSyst == systJetSmear || iSyst == systUncES) {
                    inMapIntBool[iSyst] = isSmear;
                }
                else if (iSyst == systLepEff || iSyst == systGenRecoilRW || iSyst == systStopXSec) {
                    inMapIntBool[iSyst] = false;
                }
                else {
                    inMapIntBool[iSyst] = true;
                }
                break;
            default:
                cout << "don't know which type of map you wanna set buddy! " << endl;
                break;
        }
    }
}

/************************************************************************************/
// Set the mapping of systematic designating integer to the appropriate struct pointer
/************************************************************************************/

inline void SetBEIMapPDF(mapIntBEI &inMapIntBEI, BasicEventInfo * inBEICentVal, vector<BasicEventInfo> * inBEISystVecShiftUp, vector<BasicEventInfo> * inBEISystVecShiftDown, int numSysts, mIB * useSystMapThisBEI) {
    bool useSyst = false;
    mIB::iterator xIter;
    for (int iSyst = 0; iSyst <= numSysts; ++iSyst) {
        //        cout << "iSyst " << iSyst << endl;
        //        cout << "test " << useSystMapThisBEI->size() << endl;
        xIter = useSystMapThisBEI->find(iSyst);
        //        cout << "test 2 " << endl;
        if (xIter != useSystMapThisBEI->end()) {
            useSyst = xIter->second;
            //            cout << "found it! " << iSyst << endl;
            //            cout << "useSyst value is " << useSyst << endl;
        }
        else {
            // not in the map so assume central value
            useSyst = false;
        }
        if (useSyst) {
            inMapIntBEI[iSyst] = &inBEISystVecShiftUp->at(iSyst);
            inMapIntBEI[-1 * iSyst] = &inBEISystVecShiftDown->at(iSyst);
        }
        else {
            inMapIntBEI[iSyst] = inBEICentVal;
            if (iSyst != 0) {
                inMapIntBEI[-1 * iSyst] = inBEICentVal;
            }
        }
    }
}

inline void SetBEIMap(mapIntBEI &inMapIntBEI, BasicEventInfo * inBEICentVal, vector<BasicEventInfo> * inBEISystVecShiftUp, vector<BasicEventInfo> * inBEISystVecShiftDown, int numSysts, mIB * useSystMapThisBEI) {
    bool useSyst = false;   
    mIB::iterator xIter;
    for (int iSyst = 0; iSyst <= numSysts; ++iSyst) {
//        cout << "iSyst " << iSyst << endl;
//        cout << "test " << useSystMapThisBEI->size() << endl;
        xIter = useSystMapThisBEI->find(iSyst);
//        cout << "test 2 " << endl;
        if (xIter != useSystMapThisBEI->end()) {
            useSyst = xIter->second;
//            cout << "found it! " << iSyst << endl;
//            cout << "useSyst value is " << useSyst << endl;
        }
        else {
            // not in the map so assume central value
            useSyst = false;
        }
        if (useSyst) {
            inMapIntBEI[iSyst] = &inBEISystVecShiftUp->at(iSyst);
            inMapIntBEI[-1 * iSyst] = &inBEISystVecShiftDown->at(iSyst);
        }
        else {
            inMapIntBEI[iSyst] = inBEICentVal;
            if (iSyst != 0) {
                inMapIntBEI[-1 * iSyst] = inBEICentVal;
            }
        }
    }
}

inline void SetEMIMap(mapIntEMI &inMapIntEMI, EventMETInfo * inEMICentVal, vector<EventMETInfo> * inEMISystVecShiftUp, vector<EventMETInfo> * inEMISystVecShiftDown, int numSysts, mIB * useSystMapThisEMI) {
    bool useSyst = false;
    mIB::iterator xIter;
    for (int iSyst = 0; iSyst <= numSysts; ++iSyst) {
        xIter = useSystMapThisEMI->find(iSyst);
        if (xIter != useSystMapThisEMI->end()) {
            useSyst = xIter->second;
        }
        else {
            // not in the map so assume central value
            useSyst = false;
        }
        if (useSyst) {
            inMapIntEMI[iSyst] = &inEMISystVecShiftUp->at(iSyst);
            inMapIntEMI[-1 * iSyst] = &inEMISystVecShiftDown->at(iSyst);
        }
        else {
            inMapIntEMI[iSyst] = inEMICentVal;
            if (iSyst != 0) {
                inMapIntEMI[-1 * iSyst] = inEMICentVal;
            }
        }
    }
}

inline void SetEJIMap(mapIntEJI &inMapIntEJI, EventJetInfo * inEJICentVal, vector<EventJetInfo> * inEJISystVecShiftUp, vector<EventJetInfo> * inEJISystVecShiftDown, int numSysts, mIB * useSystMapThisEJI) {
  bool useSyst = false;
  mIB::iterator xIter;
  for (int iSyst = 0; iSyst <= numSysts; ++iSyst) {
    xIter = useSystMapThisEJI->find(iSyst);
    if (xIter != useSystMapThisEJI->end()) {
      useSyst = xIter->second;
    }
    else {
      // not in the map so assume central value
      useSyst = false;
    }
    if (useSyst) {
      inMapIntEJI[iSyst] = &inEJISystVecShiftUp->at(iSyst);
      inMapIntEJI[-1 * iSyst] = &inEJISystVecShiftDown->at(iSyst);
    }
    else {
      inMapIntEJI[iSyst] = inEJICentVal;
      if (iSyst != 0) {
	inMapIntEJI[-1 * iSyst] = inEJICentVal;
      }
    }
  }
}


inline void SetELIMap(mapIntELI &inMapIntELI, EventLepInfo * inELICentVal, vector<EventLepInfo> * inELISystVecShiftUp, vector<EventLepInfo> * inELISystVecShiftDown, int numSysts, mIB * useSystMapThisELI) {
    bool useSyst = false;
    mIB::iterator xIter;
    for (int iSyst = 0; iSyst <= numSysts; ++iSyst) {
        xIter = useSystMapThisELI->find(iSyst);
        if (xIter != useSystMapThisELI->end()) {
            useSyst = xIter->second;
        }
        else {
            // not in the map so assume central value
            useSyst = false;
        }
        if (useSyst) {
            inMapIntELI[iSyst] = &inELISystVecShiftUp->at(iSyst);
            inMapIntELI[-1 * iSyst] = &inELISystVecShiftDown->at(iSyst);
        }
        else {
            inMapIntELI[iSyst] = inELICentVal;
            if (iSyst != 0) {
                inMapIntELI[-1 * iSyst] = inELICentVal;
            }
        }
    }
}

inline void SetEDSIMap(mapIntEDSI &inMapIntEDSI, EventDiStructureInfo * inEDSICentVal, vector<EventDiStructureInfo> * inEDSISystVecShiftUp, vector<EventDiStructureInfo> * inEDSISystVecShiftDown, int numSysts, mIB * useSystMapThisEDSI) {
    bool useSyst = false;
    mIB::iterator xIter;
    for (int iSyst = 0; iSyst <= numSysts; ++iSyst) {
        xIter = useSystMapThisEDSI->find(iSyst);
        if (xIter != useSystMapThisEDSI->end()) {
            useSyst = xIter->second;
        }
        else {
            // not in the map so assume central value
            useSyst = false;
        }
        if (useSyst) {
            inMapIntEDSI[iSyst] = &inEDSISystVecShiftUp->at(iSyst);
            inMapIntEDSI[-1 * iSyst] = &inEDSISystVecShiftDown->at(iSyst);
        }
        else {
            inMapIntEDSI[iSyst] = inEDSICentVal;
            if (iSyst != 0) {
                inMapIntEDSI[-1 * iSyst] = inEDSICentVal;
            }
        }
    }
}
/************************************************************************************/
// Set the mapping of systematic designating integer to the appropriate struct pointer
/************************************************************************************/
