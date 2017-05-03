#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TString.h"
#include "TGraphAsymmErrors.h"

using namespace std;

typedef pair<float, float> valPlusError;

//List of functions for creating histograms/TGraphs
//that are used in multiple different aspects of the analysis

inline void VarianceWeightedAverageHist(TH2F * inputHistToAverageInto, vector<TH2F *> * vecInputHistsToAverageWith, float minAxisDiff = -1, float maxAxisDiff = -1, bool doVerb = 0) {
    int nBinsX = inputHistToAverageInto->GetNbinsX();
    int nBinsY = inputHistToAverageInto->GetNbinsY();
    
    TAxis * xAxis = inputHistToAverageInto->GetXaxis();
    TAxis * yAxis = inputHistToAverageInto->GetYaxis();
    
    float currXVal, currYVal;
    
    unsigned int numAddtlHists = vecInputHistsToAverageWith->size();
    
    if (numAddtlHists == 0) {
        if (doVerb) {
            cout << "returning because no input hists to average " << endl;
        }
        return; //no point averaging if there's no input hists!
    }
    
    int iBinX_CurrAddtlHist, iBinY_CurrAddtlHist;
    
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        currXVal = xAxis->GetBinCenter(iBinX);
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            currYVal = yAxis->GetBinCenter(iBinY);
            if (maxAxisDiff > -1 && currXVal - currYVal > maxAxisDiff) continue;
            if (minAxisDiff > -1 && currXVal - currYVal < minAxisDiff) continue;
            if (doVerb) {
                cout << "pre-combination val " << inputHistToAverageInto->GetBinContent(iBinX, iBinY) << " +- " << inputHistToAverageInto->GetBinError(iBinX, iBinY) << endl;
            }
            
            vector<valPlusError> vecVPE(0);
            vecVPE.push_back(valPlusError(inputHistToAverageInto->GetBinContent(iBinX, iBinY), inputHistToAverageInto->GetBinError(iBinX, iBinY)));
            if (doVerb) {
                cout << "hist I'm looking at: " << inputHistToAverageInto->GetName() << endl;
            }
            for (unsigned int iHist = 0; iHist < numAddtlHists; ++iHist) {
                if (doVerb) {
                    cout << "currXVal " << currXVal << endl;
                    cout << "currYVal " << currYVal << endl;
                    cout << "vec hist I'm looking at: " << vecInputHistsToAverageWith->at(iHist)->GetName() << endl;
                }
                /*
                cout << "vec hist I'm looking at: " << vecInputHistsToAverageWith->at(iHist)->GetName() << endl;
                cout << "iBinX " << iBinX << endl;
                cout << "iBinY " << iBinY << endl;
                cout << "iBinX_CurrAddtlHist " << iBinX_CurrAddtlHist << endl;
                cout << "iBinY_CurrAddtlHist " << iBinY_CurrAddtlHist << endl;
                cout << "currXVal " << currXVal << endl;
                cout << "currYVal " << currYVal << endl;
                 */
                iBinX_CurrAddtlHist = vecInputHistsToAverageWith->at(iHist)->GetXaxis()->FindBin(currXVal);
                iBinY_CurrAddtlHist = vecInputHistsToAverageWith->at(iHist)->GetYaxis()->FindBin(currYVal);
                vecVPE.push_back(valPlusError(vecInputHistsToAverageWith->at(iHist)->GetBinContent(iBinX_CurrAddtlHist, iBinY_CurrAddtlHist), vecInputHistsToAverageWith->at(iHist)->GetBinError(iBinX_CurrAddtlHist, iBinY_CurrAddtlHist)));
            }
            valPlusError weightAveragedVPE = VarianceWeightedMean(&vecVPE, doVerb);
            
            inputHistToAverageInto->SetBinContent(iBinX, iBinY, weightAveragedVPE.first);
            inputHistToAverageInto->SetBinError(iBinX, iBinY, weightAveragedVPE.second);
            
            if (doVerb) {
                cout << "post-combination val " << inputHistToAverageInto->GetBinContent(iBinX, iBinY) << " +- " << inputHistToAverageInto->GetBinError(iBinX, iBinY) << endl;
            }
        }
    }
}

inline TH2F * TwoDeeRatio(TH2F * inputHistNum, TH2F * inputHistDenom, TString addHistName = "", bool doConsistency = false, TH2F * inputHist_UncertUp = 0, TH2F * inputHist_UncertDown = 0) {
    //Histogram that generates a 2D plot showing the ratio of inputHistNum/inputHistDenom
    //If the "doConsistency" boolean is turned on, then
    //the function assumes one is asking how consistent the ratio is with 1.0 and will do
    //(Ratio - 1.0)/SigmaRatio
    //If two additional histograms are included -- inputHist_UncertUp/inputHist_UncertDown --
    //then the function treats those as the uncertainty on the ratio, as opposed to the
    //standard uncertainties on the actual input histograms. Including these two histograms is
    //for the purpose of doing the consistency check;
    //if the ratio is < 1.0, the sigma ratio used will be the
    //up shift (as that would bring the ratio closer to one);
    //if the ratio is > 1.0, the sigma ratio used will be the
    //down shift (for similar reasons as down shift)
    
    bool useSyst = inputHist_UncertUp && inputHist_UncertDown && doConsistency;
    
    int nBinsXNum = inputHistNum->GetNbinsX();
    int nBinsYNum = inputHistNum->GetNbinsY();
    
    int nBinsXDenom = inputHistDenom->GetNbinsX();
    int nBinsYDenom = inputHistDenom->GetNbinsY();
    
    int nBinsXUncertUp, nBinsXUncertDown;
    int nBinsYUncertUp, nBinsYUncertDown;
    
    if (nBinsXNum != nBinsXDenom) {
        cout << "Something wrong with input histograms, nBinsX don't match" << endl;
        cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsXNum " << nBinsXNum << endl;
        cout << "histDenom Name: " << inputHistDenom->GetName() << ", nBinsXDenom " << nBinsXDenom << endl;
        return NULL;
    }
    if (nBinsYNum != nBinsYDenom) {
        cout << "Something wrong with input histograms, nBinsY don't match" << endl;
        cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsYNum " << nBinsYNum << endl;
        cout << "histDenom Name: " << inputHistDenom->GetName() << ", nBinsYDenom " << nBinsYDenom << endl;
        return NULL;
    }
    
    if (useSyst) {
        nBinsXUncertUp = inputHist_UncertUp->GetNbinsX();
        nBinsXUncertDown = inputHist_UncertDown->GetNbinsX();
        
        nBinsYUncertUp = inputHist_UncertUp->GetNbinsY();
        nBinsYUncertDown = inputHist_UncertDown->GetNbinsY();
        if (nBinsXNum != nBinsXUncertUp || nBinsXNum != nBinsXUncertDown) {
            cout << "Something wrong with input histograms, nBinsX don't match" << endl;
            cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsXNum " << nBinsXNum << endl;
            cout << "histUncertUp Name: " << inputHist_UncertUp->GetName() << ", nBinsXUncertUp " << nBinsXUncertUp << endl;
            cout << "histUncertDown Name: " << inputHist_UncertDown->GetName() << ", nBinsXUncertDown " << nBinsXUncertDown << endl;
            return NULL;
        }
        
        if (nBinsYNum != nBinsYUncertUp || nBinsYNum != nBinsYUncertDown) {
            cout << "Something wrong with input histograms, nBinsY don't match" << endl;
            cout << "histNum Name: " << inputHistNum->GetName() << ", nBinsYNum " << nBinsYNum << endl;
            cout << "histUncertUp Name: " << inputHist_UncertUp->GetName() << ", nBinsYUncertUp " << nBinsYUncertUp << endl;
            cout << "histUncertDown Name: " << inputHist_UncertDown->GetName() << ", nBinsYUncertDown " << nBinsYUncertDown << endl;
            return NULL;
        }
    }
    
    TString outHistName = inputHistNum->GetName() + TString("Div") + inputHistDenom->GetName() + addHistName;
    if (doConsistency) {
        outHistName += "_Consistency";
    }
    TH2F * outRatioHist = (TH2F *) inputHistNum->Clone(outHistName);
    
    valPlusError currVPENum, currVPEDenom;
    valPlusError currVPEUncertUp, currVPEUncertDown;
    float currBinContentNum, currBinContentDenom;
    float currBinErrNum, currBinErrDenom;
    
    valPlusError currBinRatioVPE(0,0);
    for (int iBinX = 1; iBinX <= nBinsXNum; ++iBinX) {
        for (int iBinY = 1; iBinY <= nBinsYNum; ++iBinY) {
            currBinContentNum = inputHistNum->GetBinContent(iBinX, iBinY);
            currBinErrNum = inputHistNum->GetBinError(iBinX, iBinY);
            
            currBinContentDenom = inputHistDenom->GetBinContent(iBinX, iBinY);
            currBinErrDenom = inputHistDenom->GetBinError(iBinX, iBinY);
            
            currVPENum = valPlusError(currBinContentNum, currBinErrNum);
            currVPEDenom = valPlusError(currBinContentDenom, currBinErrDenom);
            
            if (useSyst) {
                currVPEUncertUp = valPlusError(inputHist_UncertUp->GetBinContent(iBinX, iBinY), inputHist_UncertUp->GetBinError(iBinX, iBinY));
                currVPEUncertDown = valPlusError(inputHist_UncertDown->GetBinContent(iBinX, iBinY), inputHist_UncertDown->GetBinError(iBinX, iBinY));
                currBinRatioVPE = GetRatio(&currVPENum, &currVPEDenom, doConsistency, &currVPEUncertUp, &currVPEUncertDown);
            }
            else {
                currBinRatioVPE = GetRatio(&currVPENum, &currVPEDenom, doConsistency);
            }
            outRatioHist->SetBinContent(iBinX, iBinY, currBinRatioVPE.first);
            outRatioHist->SetBinError(iBinX, iBinY, currBinRatioVPE.second);
        }
    }
    return outRatioHist;
}

TH1F * CDFRight(TH1F * inputHist, bool makeCDF = true) {
  int NBinsX = inputHist->GetNbinsX();
  double currIntegral, currIntegralError;
  float totalIntegral = inputHist->Integral(0, NBinsX+1);

  TString histName = inputHist->GetName();
  histName += "_CDFRight";
  TH1F * outCDF = (TH1F *) inputHist->Clone(histName);

  for (int i = 0; i < NBinsX + 2; ++i) {
    currIntegral = inputHist->IntegralAndError(i, NBinsX+1, currIntegralError);
    currIntegral /= makeCDF ? totalIntegral : 1.0;
    outCDF->SetBinContent(i, currIntegral);
    outCDF->SetBinError(i, currIntegralError);
    if (makeCDF) {
      cout << "error is slightly wrong -- should make it binominal " << endl;
    }
  }
  return outCDF;
}



TH1F * ThreeDeeProject(TH3F * inputHist, TString name, int whichAxis, vector<int> * vecBounds) {
    //Function to project an input 3D histogram into a 1D histogram given an input vector of bounds for the axes
    
    TString optProj = name + "_";
    switch (whichAxis) {
        case 1:
            optProj += "x";
            inputHist->GetYaxis()->SetRange(vecBounds->at(0), vecBounds->at(1));
            inputHist->GetZaxis()->SetRange(vecBounds->at(2), vecBounds->at(3));
            break;
        case 2:
            optProj += "y";
            inputHist->GetXaxis()->SetRange(vecBounds->at(0), vecBounds->at(1));
            inputHist->GetZaxis()->SetRange(vecBounds->at(2), vecBounds->at(3));
            break;
        case 3:
            optProj += "z";
            inputHist->GetXaxis()->SetRange(vecBounds->at(0), vecBounds->at(1));
            inputHist->GetYaxis()->SetRange(vecBounds->at(2), vecBounds->at(3));
            break;
        default:
            break;
    }
    return (TH1F *) inputHist->Project3D(optProj);
}

inline TH2F * ThreeDeeToTwoDeeStatProjection(TH3F * inputHist, int whichAxis, int whichPercentile, bool doVerb = 0) {
    //Function that takes an input 3D histogram and, in the 2D plane of the 2 axes not projected with, projects the remaining axis
    //and calculates some statistical quantity with this projection, the idea being that you can use this to help visualize 3D distributions
    //still a little bit of a work in progress
    
    float currStatBinVal;
    TH1F * tempDist;
    
    
    int nBinsX3D = inputHist->GetNbinsX();
    int nBinsY3D = inputHist->GetNbinsY();
    int nBinsZ3D = inputHist->GetNbinsZ();
    
    inputHist->GetXaxis()->SetRange(1, nBinsX3D);
    inputHist->GetYaxis()->SetRange(1, nBinsY3D);
    inputHist->GetZaxis()->SetRange(1, nBinsZ3D);
    
    /*
    int nBins_Loop1 = whichAxis > 1 ? nBinsX3D : nBinsY3D;
    int nBins_Loop2 = whichAxis != 3 ? nBinsZ3D : nBinsY3D;
    */
    
    TString optProjString = "p_";
    switch (whichAxis) {
        case 1:
            optProjString += "zy";
            break;
        case 2:
            optProjString += "zx";
            break;
        case 3:
            optProjString += "yx";
            break;
        default:
            break;
    }
    if (doVerb) {
        cout << "inputHist name " << inputHist->GetName() << endl;
        cout << "optProjString " << optProjString << endl;
    }
    TH2F * outHist = (TH2F *) inputHist->Project3D(optProjString);
    int nBinsX = outHist->GetNbinsX();
    int nBinsY = outHist->GetNbinsY();
    if (doVerb) {
        cout << "outHist name " << outHist->GetName() << endl;
    }
    
    vector<int> vecBounds(4);
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        vecBounds[0] = iBinX;
        vecBounds[1] = iBinX;
        for (int iBinY = 1; iBinY <= nBinsY; ++iBinY) {
            vecBounds[2] = iBinY;
            vecBounds[3] = iBinY;
            if (doVerb) {
                cout << "iBinX " << iBinX << endl;
                cout << "iBinY " << iBinY << endl;
            }
            TString name = "p1_";
            name += iBinX;
            name += "_";
            name += iBinY;
            tempDist = ThreeDeeProject(inputHist, name, whichAxis, &vecBounds);
            if (doVerb) {
                cout << "tempDist name " << tempDist->GetName() << endl;
            }
            currStatBinVal = GetStatValue(tempDist, whichPercentile, doVerb);
            if (doVerb) {
                cout << "currStatBinVal " << currStatBinVal << endl;
            }
            outHist->SetBinContent(iBinX, iBinY, currStatBinVal);
        }
    }
    
    return outHist;
    /*
    vector<int> vecBounds(4);
    for (int iBinLoop1 = 1; iBinLoop1 <= nBins_Loop1; ++iBinLoop1) {
        vecBounds[0] = iBinLoop1;
        vecBounds[1] = iBinLoop1;
        for (int iBinLoop2 = 1; iBinLoop2 <= nBins_Loop2; ++iBinLoop2) {
            vecBounds[2] = iBinLoop2;
            vecBounds[3] = iBinLoop2;
            
            tempDist = ThreeDeeProject(inputHist, whichAxis, &vecBounds);
        }
    }
    */
}

int GetBinCounter(vector<int> * inputVecNBins, int whichBinX, int whichBinY, int whichBinZ) {
    int binCounter = 0;
    for (int iBinX = 1; iBinX <= inputVecNBins->at(0); ++iBinX) {
        for (int iBinY = 1; iBinY <= inputVecNBins->at(1); ++iBinY) {
            for (int iBinZ = 1; iBinZ <= inputVecNBins->at(2); ++iBinZ) {
                binCounter++;
                if (iBinX != whichBinX || iBinY != whichBinY || iBinZ != whichBinZ) {
                    continue;
                }
                else {
                    return binCounter;
                }
            }
        }
    }
    return binCounter;
}

TH3F * MakeSignificanceShape(TH1F * inputRatioHist, TH1 * inputMap, int whichSystDir = 0, TGraphAsymmErrors * inputFullSystGraph = NULL) {
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputMap, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    TString inputRatioHistName = inputRatioHist->GetName();
    TString addName = "";
    if (whichSystDir != 0) {
        addName += "_SystErrShift";
        addName += whichSystDir > 0 ? "Up" : "Down";
    }
    
    TH3F * outHist = (TH3F *) inputMap->Clone("Rebuilt3DSignificanceShape_" + inputRatioHistName);
    cout << "outHist x axis name " << outHist->GetXaxis()->GetName() << endl;
    cout << "outHist y axis name " << outHist->GetYaxis()->GetName() << endl;
    cout << "outHist z axis name " << outHist->GetZaxis()->GetName() << endl;
    
    float currBinContent, currBinErr;
    float currSystErrUp, currSystErrDown;
    
    int currMapBinX, currMapBinY, currMapBinZ;
    int currMapBin,  currGlobalBin;
    int whichBinToGrab;
    
    int binCounter = 0;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[0]; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= vecInputHistAxisNBins[0]; ++iBinZ) {
                binCounter++;
                
                currGlobalBin = inputMap->GetBin(iBinX, iBinY, iBinZ);
                currMapBin = (int) (0.5 + inputMap->GetBinContent(iBinX, iBinY, iBinZ));
                inputMap->GetBinXYZ(currMapBin, currMapBinX, currMapBinY, currMapBinZ);
                
                if (currGlobalBin == currMapBin) {
                    whichBinToGrab = binCounter;
                }
                else {
                    whichBinToGrab = GetBinCounter(&vecInputHistAxisNBins, currMapBinX, currMapBinY, currMapBinZ);
                }
                
                currBinContent = inputRatioHist->GetBinContent(whichBinToGrab);
                currBinErr = inputRatioHist->GetBinError(whichBinToGrab);
                
                if (whichSystDir != 0 && inputFullSystGraph != NULL) {
                    currSystErrUp = inputFullSystGraph->GetErrorYhigh(whichBinToGrab);
                    currSystErrDown = inputFullSystGraph->GetErrorYlow(whichBinToGrab);
                    cout << "currSystErrUp " << currSystErrUp << endl;
                    cout << "currSystErrDown " << currSystErrDown << endl;
                    cout << "currBinErr " << currBinErr << endl;
                    if (whichSystDir > 0) {
                        currBinErr = TMath::Sqrt(TMath::Power(currBinErr, 2) + TMath::Power(currSystErrUp, 2));
                    }
                    else {
                        currBinErr = TMath::Sqrt(TMath::Power(currBinErr, 2) + TMath::Power(currSystErrDown, 2));
                    }
                    cout << "currBinErr " << currBinErr << endl;
                }
                if (currBinContent == 0) {
                    outHist->SetBinContent(currGlobalBin, 0);
                    outHist->SetBinError(currGlobalBin, currBinErr);
                }
                else {
                    cout << "binCounter " << binCounter << endl;
                    cout << "currBinContent - 1 " << currBinContent - 1 << endl;
                    outHist->SetBinContent(currGlobalBin, (currBinContent - 1)/currBinErr);
                    outHist->SetBinError(currGlobalBin, currBinErr);
                }
            }
        }
    }
    return outHist;
}



typedef pair<int, int> axisRange;


TH2F * OutputHistDeltaMChanges(TH2F * inputHistToChange,  int whichVersDeltaM, int whichChange, axisRange * axisLSP, axisRange * axisStop, TString addName = "") {
    if (whichChange == 0) {
        cout << "need to choose a direction to change!" << endl;
        return NULL;
    }
    TAxis * xAxis = inputHistToChange->GetXaxis();
    int nBinsX = inputHistToChange->GetNbinsX();
    
    TAxis * yAxis = inputHistToChange->GetYaxis();
    int nBinsY = inputHistToChange->GetNbinsY();
    
    float xAxisBinWidth = xAxis->GetBinWidth(1);
    float yAxisBinWidth = yAxis->GetBinWidth(1);
    
    TString axisString = inputHistToChange->GetTitle();
    
    axisString += ";";
    if (whichVersDeltaM == 0) {
        axisString += xAxis->GetTitle();
        axisString += ";";
        axisString += whichChange > 0 ? "#Delta M [GeV]" : "M_{#tilde{#chi}_{0}} [GeV]";
    }
    else {
        axisString += whichChange > 0 ? "#Delta M [GeV]" : "M_{#tilde{t}} [GeV]";
        axisString += ";";
        axisString += yAxis->GetTitle();
    }
    
    TString newName = inputHistToChange->GetName();
    
    TString nominalReplaceName = addName + TString("_ConstDeltaM_vers");
    nominalReplaceName += whichVersDeltaM;
    
    if (whichChange > 0) {
        newName += nominalReplaceName;
    }
    else {
        newName.Replace(newName.Index(nominalReplaceName), nominalReplaceName.Length(), "");
    }
    
    
    
    int minDeltM = xAxis->GetBinCenter(1) - yAxis->GetBinCenter(1);
    int maxDeltM = xAxis->GetBinCenter(nBinsX) - yAxis->GetBinCenter(1);

    TH2F * outHist = NULL;
    if (whichChange > 0) {
        if (whichVersDeltaM == 0) {
            outHist = new TH2F(newName, axisString, nBinsX, xAxis->GetBinLowEdge(1), xAxis->GetBinUpEdge(nBinsX), nBinsX, minDeltM - xAxisBinWidth/2, maxDeltM + xAxisBinWidth/2);
        }
        else {
            outHist = new TH2F(newName, axisString, nBinsX, minDeltM - xAxisBinWidth/2, maxDeltM + xAxisBinWidth/2, nBinsY, yAxis->GetBinLowEdge(1), yAxis->GetBinUpEdge(nBinsY));
        }
    }
    else {
        int outputNBinsX = 1 + (axisStop->second - axisStop->first) / xAxisBinWidth;
        int outputNBinsY = 1 + (axisLSP->second - axisLSP->first) / yAxisBinWidth;
        outHist = new TH2F(newName, axisString, outputNBinsX, axisStop->first - xAxisBinWidth/2, axisStop->second + xAxisBinWidth/2, outputNBinsY, axisLSP->first - yAxisBinWidth/2, axisLSP->second + yAxisBinWidth/2);
    }
    float currBinContent, currBinErr;
    float currDeltaM, currStopMass, currLSPMass;
    int currNewHistBin;
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        if (whichChange > 0 || whichVersDeltaM == 0) {
            currStopMass = xAxis->GetBinCenter(iBinX);
        }
        else {
            currLSPMass = xAxis->GetBinCenter(iBinX);
        }
        for (int iBinY = 1; iBinY <= TMath::Min(iBinX, nBinsX); ++iBinY) {
            if (whichChange > 0) {
                currLSPMass = yAxis->GetBinCenter(iBinY);
                currDeltaM = currStopMass - currLSPMass;
            }
            else {
                currDeltaM = yAxis->GetBinCenter(iBinY);
                if (whichVersDeltaM == 0) {
                    currLSPMass = currStopMass - currDeltaM;
                }
                else {
                    currStopMass = currLSPMass + currDeltaM;
                }
            }
            currBinContent = inputHistToChange->GetBinContent(iBinX, iBinY);
            currBinErr = inputHistToChange->GetBinError(iBinX, iBinY);
            if (currBinContent == 0) {
                continue;
            }
            if (whichVersDeltaM == 0) {
                if (whichChange > 0) {
                    currNewHistBin = outHist->GetYaxis()->FindBin(currDeltaM);
                }
                else {
                    currNewHistBin = outHist->GetYaxis()->FindBin(currLSPMass);
                }
                outHist->SetBinContent(iBinX, currNewHistBin, currBinContent);
                outHist->SetBinError(iBinX, currNewHistBin, currBinErr);
            }
            else {
                cout << "not supported for now!" << endl;
            }
        }
    }
    return outHist;
}