void TestHist(TH1 * inputHist) {
  int currGlobalBin;
  int currBinMapBin;
  int currMapBinX, currMapBinY, currMapBinZ;
  int totalBins = 0, totalMappingBins = 0;
  for (int iBinX = 1; iBinX <= inputHist->GetNbinsX(); ++iBinX) {
    for (int iBinY = 1; iBinY <= inputHist->GetNbinsY(); ++iBinY) {
      for (int iBinZ = 1; iBinZ <= inputHist->GetNbinsZ(); ++iBinZ) {
	totalBins++;
	currGlobalBin = inputHist->GetBin(iBinX, iBinY, iBinZ);
	currBinMapBin = (int) (0.5 + inputHist->GetBinContent(iBinX, iBinY, iBinZ));
	if (currGlobalBin != currBinMapBin) {
	  totalMappingBins++;
	  inputHist->GetBinXYZ(currBinMapBin, currMapBinX, currMapBinY, currMapBinZ);
	  cout << "iBinX:iBinY:iBinZ " << iBinX << ":" << iBinY << ":" << iBinZ << endl;
	  cout << "currMapBinX:currMapBinY:currMapBinZ " << currMapBinX << ":" << currMapBinY << ":" << currMapBinZ << endl;
	}
      }
    }
  }
  cout << "totalBins " << totalBins << endl;
  cout << "totalMappingBins " << totalMappingBins << endl;
}
