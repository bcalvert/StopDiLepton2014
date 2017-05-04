void SetContourVecVec(vector< vector<TGraph *> > * inVecVecContours, vector<TH2F *> * inVecSigStrengthHists, vector<GraphParams> * inVecGPs, int minNumPoints, float desiredExclusion, int hackDeltaMRegion = -1, bool doVerb = false) {

  //inVecSigStrengthHists is the input vector of signal strength histograms
  //inVecVecContours is a vector of vectors of TGraph pointers, used to contain the output exclusion contours for each of the histograms in inVecSigStrengthHists
  //inVecGPs is a custom structure of mine used to set style for the output exclusion contours -- it's not important for the grabbing of contours
  //minNumPoints is the minimum number of points a contour must have before it is considered a legitimate contour (i.e. don't pick up single point contours and think that they're exclusion regions
  //hackDeltaMRegion is the deltaM line that one thinks might have little to no sensitivity -- i.e. deltaM = top mass for T2tt
  //doVerb is a debugstatement option
  
  unsigned int numHists = inVecSigStrengthHists->size();
  unsigned int currNumContours;
  TH2F * currHist; //the hist corresponding to the signal strength map
  TString currHistName;
  vector<TH2F *> vecOfCurrHist; //used for splitting along diagonals of no sensitivity -- i.e. deltaM = top mass for T2tt                            

  bool shouldSplit;

  for (unsigned int iHist = 0; iHist < numHists; ++iHist) {
    currHist = inVecSigStrengthHists->at(iHist);
    currHistName = currHist->GetName();
    shouldSplit = hackDeltaMRegion > -1;
    if (shouldSplit) {
      TH2F * test = (TH2F *) currHist->Clone(currHistName + "_TestSplit");
      shouldSplit = SplitHistAlongDeltaM(test, hackDeltaMRegion, 1);
    }
    if (shouldSplit) {
      vecOfCurrHist.clear();
      vecOfCurrHist.push_back((TH2F *) currHist->Clone(currHistName + "_ForwardDeltaMSplit"));
      vecOfCurrHist.push_back((TH2F *) currHist->Clone(currHistName + "_BackwardDeltaMSplit"));

      SplitHistAlongDeltaM(vecOfCurrHist[0], hackDeltaMRegion, 1);
      SplitHistAlongDeltaM(vecOfCurrHist[1], hackDeltaMRegion, -1);
      ReturnTGraphVector(&inVecVecContours->at(iHist), vecOfCurrHist[0], minNumPoints, desiredExclusion, true, doVerb);
      ReturnTGraphVector(&inVecVecContours->at(iHist), vecOfCurrHist[1], minNumPoints, desiredExclusion, true, doVerb);
    }
    else {
      ReturnTGraphVector(&inVecVecContours->at(iHist), inVecSigStrengthHists->at(iHist), minNumPoints, desiredExclusion, false, doVerb);
    }
  }
