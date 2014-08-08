// Will eventually contain the struct definition for the SampleT struct specific to the stop analysis

typedef struct SampleT {
    TString histNameSuffix;
    SampCutInt cutDiLepType;
    SampCutInt cutNJets;
    SampCutInt cutNBJets;
    SampCutInt cutZMass;
    
    SampCutInt cutNumLepPairs;
    SampCutInt cutISRSelection;
    
    SampCut cutMT2ll, cutMT2lb;
    SampCut cutMET;
    
    void SetCutSamps_DiLep(SampCutInt * inCutDiLepType, SampCutInt * inCutZMass, ) {
        cutDiLepType.SetParams(inCutDiLepType);
        cutZMass.SetParams(inCutZMass);
    }
    void SetCutSamps_Topology(SampCutInt * inCutNumLepPairs, SampCutInt * inCutISRSelection) {        
        cutNumLepPairs.SetParams(inCutNumLepPairs);
        cutISRSelection.SetParams(inCutISRSelection);
    }    
    void SetCutSamps_NJets(SampCutInt * inCutNJets, SampCutInt * inCutNBJets) {
        cutNJets.SetParams(inCutNJets);
        cutNBJets.SetParams(inCutNBJets);
    }
    void SetCutSamps_MET(SampCut * inCutMET, SampCut * inCutMT2ll, SampCut * inCutMT2lb) {
        cutMET.SetParams(inCutMET);
        cutMT2ll.SetParams(inCutMT2ll);
        cutMT2lb.SetParams(inCutMT2lb);
    }
    bool PassesEvent(EventStructPointerInfo * inESPI, bool doVerbosity = false) {
        bool passesDiLepCut, passesZMassCut;
        bool passesNJetsCut, passesNBJetsCut;
        bool passesNumLepPairsCut, passesISRSelectionCut;        
        bool passesMETCut;        
        bool passesMT2llCut, passesMT2lbCut;
        
        if (doVerbosity) cout << "in sampleT " << histNameSuffix << ", doEvent? " << inESPI->addELI->doEvent << endl;
        bool eventPasses = inESPI->addELI->doEvent;
        if (eventPasses) {
            passesDiLepCut = cutDiLepType.PassCut(inESPI->addELI->EventDiLepType);
            passesZMassCut = cutZMass.PassCut(inESPI->addELI->EventDiLepinZMass);
            if (doVerbosity) {
                cout << "
            }
            if (cutDiLepType.PassCut(
            eventPasses = (cutDiLepType.PassCut(inESPI->addELI->EventDiLepType) && )
            
        }
        return eventPasses;
        eventPasses = (inEPI->EventType == cutTypePhot.cutVarVal);
        switch (cutNJets.cutDir) {
            case -1:
                eventPasses &= (inEJI->EventNJets <= cutNJets.cutVarVal);
                break;
            case 0:
                eventPasses &= (inEJI->EventNJets == cutNJets.cutVarVal);
                break;
            case 1:
                eventPasses &= (inEJI->EventNJets >= cutNJets.cutVarVal);
                break;
            default:
                cout << "issues with checking cutNJets.cutDir" << endl;
                break;
        }
        switch (cutDPhiPhot0Jet0_Angle1.cutDir) {
            case -1:
                eventPasses &= (dPhi(inEPI->vecEventPhotons[0].BVC.Vec_Phi, inEJI->vecEventJets[0].BVC.Vec_Phi) <= cutDPhiPhot0Jet0_Angle1.cutVarVal);
                break;
            case 1:
                eventPasses &= (dPhi(inEPI->vecEventPhotons[0].BVC.Vec_Phi, inEJI->vecEventJets[0].BVC.Vec_Phi) >= cutDPhiPhot0Jet0_Angle1.cutVarVal);
                break;
            default:
                cout << "issues with checking cutDPhiPhot0Jet0_Angle1.cutDir" << endl;
                break;
        }
        switch (cutDPhiPhot0Jet0_Angle2.cutDir) {
            case -1:
                eventPasses &= (dPhi(inEPI->vecEventPhotons[0].BVC.Vec_Phi, inEJI->vecEventJets[0].BVC.Vec_Phi) <= cutDPhiPhot0Jet0_Angle2.cutVarVal);
                break;
            case 1:
                eventPasses &= (dPhi(inEPI->vecEventPhotons[0].BVC.Vec_Phi, inEJI->vecEventJets[0].BVC.Vec_Phi) >= cutDPhiPhot0Jet0_Angle2.cutVarVal);
                break;
            default:
                cout << "issues with checking cutDPhiPhot0Jet0_Angle1.cutDir" << endl;
                break;
        }
        //        cout << "eventPasses " << eventPasses << endl;
        return eventPasses;
    }
} SampleT;
bool operator<(const SampleT &a, const SampleT &b)
{
    return (a.histNameSuffix < b.histNameSuffix);
}