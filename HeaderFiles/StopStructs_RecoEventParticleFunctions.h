/*
inline void CalcMT2(EventLepInfo * inELI, EventJetInfo * inEJI, EventMETInfo &inputEMI) {
  vector<TLorentzVector> vecLepMT2lb(2), vecJetMT2lb(2);
    
  // Set the events M_{T2}(ll)
  inputEMI.MET_EMT2I.EventMT2ll = getMT2(inELI->Lep0.P4, inELI->Lep1.P4, inputEMI.EventMET, inputEMI.EventMETPhi);
  // Set the events M_{T2}(lb)(lb)
  if (inEJI->EventNJets > 1) {
    vecLepMT2lb[0] = inELI->Lep0.P4;
    vecLepMT2lb[1] = inELI->Lep1.P4;
    if (inEJI->EventNBtagJets > 1) {
      vecJetMT2lb[0] = inEJI->BtagJet0.P4;
      vecJetMT2lb[1] = inEJI->BtagJet1.P4;
      inputEMI.MET_EMT2I.caseMT2lb = 0;
    }
    else if (inEJI->EventNBtagJets == 1) {
      vecJetMT2lb[0] = inEJI->BtagJet0.P4;
      if (inEJI->EventBtagJet0Index == 0) {
	vecJetMT2lb[1] = inEJI->Jet1.P4;
	inputEMI.MET_EMT2I.caseMT2lb = 1;
      }
      else {
	vecJetMT2lb[1] = inEJI->Jet0.P4;
	inputEMI.MET_EMT2I.caseMT2lb = 2;
      }            
    }
    else {
      vecJetMT2lb[0] = inEJI->Jet0.P4;
      vecJetMT2lb[1] = inEJI->Jet1.P4;
      inputEMI.MET_EMT2I.caseMT2lb = 3;
    }
    inputEMI.MET_EMT2I.EventMT2lb = MT2lbCalculator(&vecLepMT2lb, &vecJetMT2lb, inputEMI.EventMET, inputEMI.EventMETPhi, inputEMI.MET_EMT2I.EventVecBLepsMT2lb);
    inputEMI.MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed = dPhi(vecJetMT2lb[0].Phi(), vecJetMT2lb[1].Phi());
    inputEMI.MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed = dPhi(inputEMI.MET_EMT2I.EventVecBLepsMT2lb[0].Phi(), inputEMI.MET_EMT2I.EventVecBLepsMT2lb[1].Phi());
  }
  else {
    inputEMI.MET_EMT2I.EventMT2lb = -99.;
    inputEMI.MET_EMT2I.EventDeltaPhiMT2lb_JetsUsed = -99.;
    inputEMI.MET_EMT2I.EventDeltaPhiMT2lb_BLepsUsed = -99.;
  }
}
*/