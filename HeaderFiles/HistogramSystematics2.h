#include <vector>
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TString.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
using namespace std;

int SystHistFinderOneDee(vector<TH1F *> * vecSystHists, TString searchString, bool doVerbosity = false) {
    TString name;
    int whichIndex = -1;
    if (doVerbosity) {
        cout << "searchString " << searchString << endl;
    }
    for (unsigned int iSyst = 0; iSyst < vecSystHists->size(); ++iSyst) {
        name = vecSystHists->at(iSyst)->GetName();
        if (doVerbosity) {
            cout << "name " << name << endl;
        }
        if (name.Contains(searchString)) {
            whichIndex = iSyst;
            if (doVerbosity) {
                cout << "found it! " << whichIndex << endl;
            }
        }        
    }
    return whichIndex;
}

int SystHistFinderOneDee(vector<TH1 *> * vecSystHists, TString searchString) {
    TString name;
    int whichIndex = -1;
    for (unsigned int iSyst = 0; iSyst < vecSystHists->size(); ++iSyst) {
        name = vecSystHists->at(iSyst)->GetName();
        if (name.Contains(searchString)) whichIndex = iSyst;
    }
    return whichIndex;
}
/*
TH1F * SystHistFinderOneDee(vector<TH1 *> * vecSystHists, TString searchString) {
    TString name;
    int whichIndex = -1;
    for (unsigned int iSyst = 0; iSyst < vecSystHists->size(); ++iSyst) {
        name = vecSystHists->at(iSyst)->GetName();
        if (name.Contains(searchString)) whichIndex = iSyst;
    }
    return (TH1F *) vecSystHists->at(whichIndex);
}
*/
inline void pointSystErr(Double_t centVal, Double_t shiftUp, Double_t shiftDown, Double_t &errUp, Double_t &errDown, bool doSymError = false, bool doVerbosity = false) {
    Double_t diffShiftUp, diffShiftDown;
    Double_t errSum;
    int whichErrCase = 0; 
    // +(-) 3 for if shift up is greater (smaller) than cent Val; likewise for shift down but with +(-) 1 instead;
    // +4 (-4): both more (less) than cent val
    // +2 (-2): upErr more, downErr less (upErr less, downErr more)
    if (doVerbosity) {
        cout << "shiftUp " << shiftUp << endl;
        cout << "shiftDown " << shiftDown << endl;
    }
    whichErrCase += (shiftUp > centVal) ? 3 : 0;
    whichErrCase += (shiftUp < centVal) ? -3 : 0;
    whichErrCase += (shiftDown > centVal) ? 1 : 0;
    whichErrCase += (shiftDown < centVal) ? -1 : 0;
    diffShiftUp = centVal - shiftUp;
    diffShiftDown = centVal - shiftDown;
//    cout << "justOneSideErr " << justOneSideErr << endl;
//    cout << "whichErrCase " << whichErrCase << endl;
    errSum = TMath::Sqrt(0.5*(diffShiftDown*diffShiftDown + diffShiftUp*diffShiftUp));
    if (doSymError) {
        errUp = errSum;
        errDown = errSum;
    }
    else {
        switch (whichErrCase) {
            case -4:
                errUp = 0;
                errDown = errSum;
                break;
            case -3:
                errUp = 0;
                errDown = abs(diffShiftUp);
                break;                
            case -2:
                errUp = abs(diffShiftDown);
                errDown = abs(diffShiftUp);
                break;
            case -1:
                errUp = 0;
                errDown = abs(shiftDown);
            case 0:
                errUp = 0;
                errDown = 0;
                break;
            case 1:
                errUp = abs(shiftDown);
                errDown = 0;
            case 2:
                errUp = abs(diffShiftUp);
                errDown = abs(diffShiftDown);
                break;
            case 3:
                errUp = abs(diffShiftUp);
                errDown = 0;
                break;
            case 4:
                errUp = errSum;
                errDown = 0;
                break;
            default:
                break;
        }
    }
}

TGraphAsymmErrors * GraphSystErrorSet_SingleSource(TH1 * HistCentralVal, TH1 * HistShiftUp, TH1 * HistShiftDown, TString systString, bool doSymError = false, int doVerbosity = -1) {
    // overloaded version of the graph error set used for grabbing specific systematic
    int nBinsX = HistCentralVal->GetNbinsX();
    Double_t errUp, errDown;
    Double_t centVal, shiftUp, shiftDown;
    Double_t binWidth;
    Double_t x;
    bool     filledSystUpHist, filledSystDownHist;
    TString name = HistCentralVal->GetName();
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(nBinsX+2); 
    outGraph->SetName(name + systString);// "_ErrSingleSource");
    TString systHistName;
    //    cout << "HACK JOB " << endl;
    filledSystUpHist = (HistShiftUp->Integral() > 0) ? true : false;
    filledSystDownHist = (HistShiftDown->Integral() > 0) ? true : false;
    for (int iPoint = 0; iPoint < nBinsX + 2; ++iPoint) { 
        
        //coordinate prep
        x = (Double_t) HistCentralVal->GetBinCenter(iPoint);
        //        if (i == 1) x = (Double_t) HistCentralVal->GetBinLowEdge(i);
        //        if (i == nBinsX) x = (Double_t) HistCentralVal->GetBinLowEdge(i+1);
        binWidth = (Double_t) HistCentralVal->GetXaxis()->GetBinWidth(iPoint);
        //initial point sets -- y-axis info will contain relevant uncertainties
        if (iPoint >= 1 && iPoint <= nBinsX && doVerbosity > 0) {
            cout << "HistCentralVal->GetBinContent(i) for i: " << iPoint << " is " << HistCentralVal->GetBinContent(iPoint) << endl;
            cout << "HistShiftUp->GetBinContent(iPoint) for i: " << iPoint << " is " << HistShiftUp->GetBinContent(iPoint) << endl;
            cout << "HistShiftDown->GetBinContent(iPoint) for i: " << iPoint << " is " << HistShiftDown->GetBinContent(iPoint) << endl;
            cout << "diffShift Up " << HistShiftUp->GetBinContent(iPoint) - HistCentralVal->GetBinContent(iPoint) << endl;
            cout << "diffShift Down " << HistShiftDown->GetBinContent(iPoint) - HistCentralVal->GetBinContent(iPoint) << endl;
        }
        outGraph->SetPoint(iPoint, x, HistCentralVal->GetBinContent(iPoint));
        outGraph->SetPointEXlow(iPoint, binWidth/2.);
        outGraph->SetPointEXhigh(iPoint, binWidth/2.);
        
        //get info for setting y-axis
        centVal = (Double_t) HistCentralVal->GetBinContent(iPoint);
        shiftUp = (Double_t) HistShiftUp->GetBinContent(iPoint);
        shiftDown = (Double_t) HistShiftDown->GetBinContent(iPoint);
        pointSystErr(centVal, shiftUp, shiftDown, errUp, errDown, doSymError, doVerbosity);
        if (iPoint > 0 && iPoint < nBinsX + 1 && doVerbosity > 0) {
            cout << " for i = " << iPoint << " in syst " << systString << " errors " << endl;
            cout << "upErr: " << errUp << endl;
            cout << "downErr: " << errDown << endl;            
            cout << endl;
        }
        outGraph->SetPointEYhigh(iPoint, errUp);
        outGraph->SetPointEYlow(iPoint, errDown);
    }
    return outGraph;
}

TGraphAsymmErrors * GraphSystErrorSet_SingleSource(TH1 * HistCentralVal, vector<TH1F *> * systHistVec, TString systString, bool doSymError, int justOneSideErr, int doVerbosity = -1) {
    // overloaded version of the graph error set used for grabbing specific systematic
    int nBinsX = HistCentralVal->GetNbinsX();
    Double_t errUp, errDown;
    Double_t centVal, shiftUp, shiftDown;
    Double_t binWidth;
    Double_t x;
    bool     filledSystUpHist, filledSystDownHist;
    TString name = HistCentralVal->GetName();
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(nBinsX+2); 
    outGraph->SetName(name + systString);// "_ErrSingleSource");
    TH1F * HistShiftUp, * HistShiftDown;
    TString systHistName;
    //picking out correct histogram
//    cout << "systVec size " << systHistVec->size() << endl;
    bool foundUp = 0;
    bool foundDown = 0;
    int singleShiftIndex = -99;
    for (unsigned int k = 0; k < systHistVec->size(); ++k) {
        systHistName = systHistVec->at(k)->GetName();
//        cout << "histName " << systHistName << endl;
//        cout << "systHistName.Contains(systString) for systString " << systString << " is " <<systHistName.Contains(systString) << endl;
        if (!systHistName.Contains(systString)) {
            continue;   
        }
        else {
            singleShiftIndex = k;
        }
//        cout << "systHistName.Contains(up) " << systHistName.Contains("Up") << endl;
//        cout << "systHistName.Contains(down) " << systHistName.Contains("Down") << endl;
        if (systHistName.Contains("Up")) {
            HistShiftUp = systHistVec->at(k);   
            foundUp = 1;
        }
        else if (systHistName.Contains("Down")) {
            HistShiftDown = systHistVec->at(k);
            foundDown = 1;
        }
    }
//    cout << "singleshift index " << singleShiftIndex << endl;
    if (!foundUp && !foundDown) {
        if (singleShiftIndex >= 0) {
        HistShiftUp = systHistVec->at(singleShiftIndex);
        }
        else {
            HistShiftUp = (TH1F *) HistCentralVal;
        }
        HistShiftDown = (TH1F *) HistCentralVal;//->Clone(HistCentralVal->GetName() + systString + TString("Down"));
    }
    else if (!foundUp) {
        HistShiftUp = (TH1F *) HistCentralVal;//->Clone(HistCentralVal->GetName() + systString + TString("Up"));
    }
    else if (!foundDown) {
        HistShiftDown = (TH1F *) HistCentralVal;//->Clone(HistCentralVal->GetName() + systString + TString("Down"));
    }
//    cout << "HACK JOB " << endl;
    filledSystUpHist = (HistShiftUp->Integral() > 0) ? true : false;
    filledSystDownHist = (HistShiftDown->Integral() > 0) ? true : false;
    for (int i = 0; i < nBinsX+2; ++i) { 

        //coordinate prep
        x = (Double_t) HistCentralVal->GetBinCenter(i);
//        if (i == 1) x = (Double_t) HistCentralVal->GetBinLowEdge(i);
//        if (i == nBinsX) x = (Double_t) HistCentralVal->GetBinLowEdge(i+1);
        binWidth = (Double_t) HistCentralVal->GetXaxis()->GetBinWidth(i);
        //initial point sets -- y-axis info will contain relevant uncertainties
        if (i > 0 && i < nBinsX + 1 && doVerbosity > 0) {
            cout << "HistCentralVal->GetBinContent(i) for i: " << i << " is " << HistCentralVal->GetBinContent(i) << endl;
            cout << "HistShiftUp->GetBinContent(i) for i: " << i << " is " << HistShiftUp->GetBinContent(i) << endl;
            cout << "HistShiftDown->GetBinContent(i) for i: " << i << " is " << HistShiftDown->GetBinContent(i) << endl;
            cout << "diffShift Up " << HistShiftUp->GetBinContent(i) - HistCentralVal->GetBinContent(i) << endl;
            cout << "diffShift Down " << HistShiftDown->GetBinContent(i) - HistCentralVal->GetBinContent(i) << endl;
        }
        outGraph->SetPointEXlow(i, binWidth/2.);
        outGraph->SetPointEXhigh(i, binWidth/2.);
        outGraph->SetPoint(i, x, HistCentralVal->GetBinContent(i));
        outGraph->SetPointEXlow(i, binWidth/2.);

        //get info for setting y-axis
        centVal = (Double_t) HistCentralVal->GetBinContent(i);
        shiftUp = (Double_t) HistShiftUp->GetBinContent(i);
        shiftDown = (Double_t) HistShiftDown->GetBinContent(i);
//        cout << "within GraphSyst line 150" << endl;
        pointSystErr(centVal, shiftUp, shiftDown, errUp, errDown, doSymError, doVerbosity);
        if (i > 0 && i < nBinsX + 1 && doVerbosity > 0) {
            cout << " for i = " << i << " in syst " << systString << " errors " << endl;
            cout << "upErr: " << errUp << endl;
            cout << "downErr: " << errDown << endl;            
            cout << endl;
        }
        if (filledSystUpHist) {
            outGraph->SetPointEYhigh(i, errUp);
            outGraph->SetPointEYlow(i, errDown);
        }
        else {
            outGraph->SetPointEYhigh(i, 0);
            outGraph->SetPointEYlow(i, 0);
        }
//        cout << "chose to fill1? " << outGraph->GetErrorYhigh(i) << endl;        
//        cout << "chose to fill2? " << outGraph->GetErrorYlow(i) << endl;
    }
    return outGraph;
}

TGraphAsymmErrors * GraphSystErrorSumErrors(TGraphAsymmErrors * centValGraph, TString addName, vector<TGraphAsymmErrors *> * systGraphVec, bool addStatErr, TH1 * inputHist, int doVerbosity = -1) {
    int nPoints = centValGraph->GetN();
    Double_t upErrSum, downErrSum;
    Double_t upErr, downErr;
    Double_t x, y;
    TString name = centValGraph->GetName();
    TGraphAsymmErrors * outGraph = (TGraphAsymmErrors *) centValGraph->Clone(name + addName + TString("_ErrSummed"));
    for (int iPoint = 0; iPoint < nPoints; ++iPoint) {
        upErrSum = 0;
        downErrSum = 0;
        x = (Double_t) inputHist->GetBinCenter(iPoint);
        y = (Double_t) inputHist->GetBinContent(iPoint);
        if (addStatErr) {            
            upErr = (Double_t) centValGraph->GetErrorYhigh(iPoint);
            downErr = (Double_t) centValGraph->GetErrorYlow(iPoint);
            upErrSum += upErr*upErr;
            downErrSum += downErr*downErr;
        }
        for (unsigned int k = 0; k < systGraphVec->size(); ++k) {
            if (doVerbosity > 0) {
                cout << "systGraphVec->at(k) has a name? " << endl;
                cout << systGraphVec->at(k)->GetName() << endl;
            }
            upErr = (Double_t) systGraphVec->at(k)->GetErrorYhigh(iPoint);
            downErr = (Double_t) systGraphVec->at(k)->GetErrorYlow(iPoint);
            if (iPoint == 2 && doVerbosity > 0) {
                cout << "systGraph Name " << systGraphVec->at(k)->GetName() << endl;
                cout << "upErr " << upErr << endl;
                cout << "downErr " << downErr << endl;
            }
            upErrSum += upErr*upErr;
            downErrSum += downErr*downErr;
        }
        outGraph->SetPoint(iPoint, x, y);
        outGraph->SetPointEYlow(iPoint, sqrt(downErrSum));
        outGraph->SetPointEYhigh(iPoint, sqrt(upErrSum));
        outGraph->SetPointEXlow(iPoint, systGraphVec->at(0)->GetErrorXlow(iPoint));
        outGraph->SetPointEXhigh(iPoint, systGraphVec->at(0)->GetErrorXhigh(iPoint));
        if (doVerbosity) {
            cout << "in quad sum: up error at iPoint " << iPoint << "is " << outGraph->GetErrorYhigh(iPoint) << endl;
        }
    }
    return outGraph;
}

TGraphAsymmErrors * GraphSystErrorSumErrors(TGraphAsymmErrors * centValGraph, TString addName, TGraphAsymmErrors * systGraph, TH1 * inputHist, bool doVerbosity = false) {
    int nPoints = centValGraph->GetN();
    Double_t upErrSum, downErrSum;
    Double_t upErr, downErr;
    Double_t x, y;
    TString name = centValGraph->GetName();
    TGraphAsymmErrors * outGraph = (TGraphAsymmErrors *) centValGraph->Clone(name + addName + TString("_ErrSummed"));
    for (int iPoint = 0; iPoint < nPoints; ++iPoint) {
        upErrSum = 0;
        downErrSum = 0;
        x = (Double_t) inputHist->GetBinCenter(iPoint);
        y = (Double_t) inputHist->GetBinContent(iPoint);  
        upErr = (Double_t) centValGraph->GetErrorYhigh(iPoint);
        downErr = (Double_t) centValGraph->GetErrorYlow(iPoint);
        upErrSum += upErr*upErr;
        downErrSum += downErr*downErr;
        upErr = (Double_t) systGraph->GetErrorYhigh(iPoint);
        downErr = (Double_t) systGraph->GetErrorYlow(iPoint);
        upErrSum += upErr*upErr;
        downErrSum += downErr*downErr;
        outGraph->SetPoint(iPoint, x, y);
        outGraph->SetPointEYlow(iPoint, sqrt(downErrSum));
        outGraph->SetPointEYhigh(iPoint, sqrt(upErrSum));
        outGraph->SetPointEXlow(iPoint, systGraph->GetErrorXlow(iPoint));
        outGraph->SetPointEXhigh(iPoint, systGraph->GetErrorXhigh(iPoint));
        if (doVerbosity) {
            cout << "in quad sum: up error at iPoint " << iPoint << " is " << outGraph->GetErrorYhigh(iPoint) << endl;
        }
    }
    return outGraph;
}
TGraphAsymmErrors * ClonePoints(TH1 * inputHist, bool cloneZero = true) {
    int NBinsX = inputHist->GetNbinsX();
    double x, y;
    Double_t binWidth;
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(NBinsX + 2);
    outGraph->SetName(inputHist->GetName() + TString("_Graph"));
    for (int i = 0; i < NBinsX + 2; ++i) {
        x = (double) inputHist->GetBinCenter(i);
        y = (double) inputHist->GetBinContent(i);
        if (!cloneZero && y == 0) continue;
        binWidth = (Double_t) inputHist->GetXaxis()->GetBinWidth(i);
        outGraph->SetPoint(i, x, y);
        outGraph->SetPointEYlow(i, inputHist->GetBinError(i));
        outGraph->SetPointEYhigh(i, inputHist->GetBinError(i));
        outGraph->SetPointEXlow(i, binWidth/2.);
        outGraph->SetPointEXhigh(i, binWidth/2.);
    }
    return outGraph;
}

TGraphAsymmErrors * NormalizationSystGraph(TH1 * centValMCHist, float normFactor = 0.0) {
    double x, y, yErr;
    Double_t binWidth;
    int nBins = centValMCHist->GetNbinsX();
    TString name = centValMCHist->GetName();
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(nBins+2); 
    outGraph->SetName(name + "_XSecUncert");// "_ErrSingleSource");
    for (int i = 0; i < nBins + 2; ++i) {
        x = centValMCHist->GetBinCenter(i);
        y = centValMCHist->GetBinContent(i);
        yErr = normFactor * y;
        outGraph->SetPoint(i, x, y);
        binWidth = (Double_t) centValMCHist->GetXaxis()->GetBinWidth(i);
        outGraph->SetPointEYhigh(i, yErr);
        outGraph->SetPointEYlow(i, yErr);
        outGraph->SetPointEXlow(i, binWidth/2.);
        outGraph->SetPointEXhigh(i, binWidth/2.);        
    }
    return outGraph;
}


inline TGraphAsymmErrors * FracGraph(TH1 * centValMCHist, TGraphAsymmErrors * errGraphSystPlusStat, bool doAbsRatio, float yAxisRange) {
    double x, y;
    double rel, relUpErr, relDownErr;
    Double_t binWidth;
    int nBins = centValMCHist->GetNbinsX();
    TGraphAsymmErrors * ratioGraph = new TGraphAsymmErrors(nBins + 2);    
    GraphMainAttSet(ratioGraph, kGray+1, 3001, kGray+1, 2, kWhite, 0, 0); 
    if (doAbsRatio) {
        rel = 1;
        HistAxisAttSet(ratioGraph->GetYaxis(), TString("Data/MC"), .15, .54, .14, .011, 1.0 - yAxisRange, 1.0 + yAxisRange); 
    }
    else {
        rel = 0;
        HistAxisAttSet(ratioGraph->GetYaxis(), TString("(MC-Data)/Data"), .15, .54, .14, .011, -1.0 * yAxisRange, 1.0 * yAxisRange);
    }
    for (int i = 0; i < nBins + 2; ++i) {
        x = centValMCHist->GetBinCenter(i);
        y = centValMCHist->GetBinContent(i);
        ratioGraph->SetPoint(i, x, rel);        
        binWidth = (Double_t) centValMCHist->GetXaxis()->GetBinWidth(i);
        
        //initial point sets -- y-axis info will contain relevant uncertainties
        
        if (!(y > 0.)) {
            relUpErr = 0;
            relDownErr = 0;
        }
        else {
//            relUpErr = (doAbsRatio) ? errGraphSystPlusStat->GetErrorYlow(i) :  errGraphSystPlusStat->GetErrorYhigh(i);
            relUpErr = (!doAbsRatio) ? errGraphSystPlusStat->GetErrorYlow(i) :  errGraphSystPlusStat->GetErrorYhigh(i);
            relUpErr /= centValMCHist->GetBinContent(i);
//            relDownErr = (doAbsRatio) ? errGraphSystPlusStat->GetErrorYhigh(i) :  errGraphSystPlusStat->GetErrorYlow(i);
            relDownErr = (!doAbsRatio) ? errGraphSystPlusStat->GetErrorYhigh(i) :  errGraphSystPlusStat->GetErrorYlow(i);
            relDownErr /= centValMCHist->GetBinContent(i);
        }
        ratioGraph->SetPointEYhigh(i, relUpErr);
        ratioGraph->SetPointEYlow(i, relDownErr);
        ratioGraph->SetPointEXlow(i, binWidth/2.);
        ratioGraph->SetPointEXhigh(i, binWidth/2.);
        
    }
    return ratioGraph;
}


inline TGraphAsymmErrors * FracGraph(TH1 * centValMCHist, TGraphAsymmErrors * errGraphSystPlusStat, bool doAbsRatio, float yAxisLB, float yAxisUB) {
    double x, y;
    double rel, relUpErr, relDownErr;
    Double_t binWidth;
    int nBins = centValMCHist->GetNbinsX();
    TGraphAsymmErrors * ratioGraph = new TGraphAsymmErrors(nBins + 2);    
    GraphMainAttSet(ratioGraph, kGray+1, 3001, kGray+1, 2, kWhite, 0, 0);
    TString axisTitle;
    if (doAbsRatio) {
        rel = 1;
        axisTitle = "Data/MC";
    }
    else {
        rel = 0;
        axisTitle = "(MC-Data)/Data";
    }
    HistAxisAttSet(ratioGraph->GetYaxis(), axisTitle, .15, .54, .14, .011, yAxisLB, yAxisUB);
    for (int i = 0; i < nBins + 2; ++i) {
        x = centValMCHist->GetBinCenter(i);
        y = centValMCHist->GetBinContent(i);
        ratioGraph->SetPoint(i, x, rel);        
        binWidth = (Double_t) centValMCHist->GetXaxis()->GetBinWidth(i);
        
        //initial point sets -- y-axis info will contain relevant uncertainties
        
        if (!(y > 0.)) {
            relUpErr = 0;
            relDownErr = 0;
        }
        else {
            //            relUpErr = (doAbsRatio) ? errGraphSystPlusStat->GetErrorYlow(i) :  errGraphSystPlusStat->GetErrorYhigh(i);
            relUpErr = (!doAbsRatio) ? errGraphSystPlusStat->GetErrorYlow(i) :  errGraphSystPlusStat->GetErrorYhigh(i);
            relUpErr /= centValMCHist->GetBinContent(i);
            //            relDownErr = (doAbsRatio) ? errGraphSystPlusStat->GetErrorYhigh(i) :  errGraphSystPlusStat->GetErrorYlow(i);
            relDownErr = (!doAbsRatio) ? errGraphSystPlusStat->GetErrorYhigh(i) :  errGraphSystPlusStat->GetErrorYlow(i);
            relDownErr /= centValMCHist->GetBinContent(i);
        }
        ratioGraph->SetPointEYhigh(i, relUpErr);
        ratioGraph->SetPointEYlow(i, relDownErr);
        ratioGraph->SetPointEXlow(i, binWidth/2.);
        ratioGraph->SetPointEXhigh(i, binWidth/2.);
        
    }
    return ratioGraph;
}


void SetPointsAsymmGraphfromHists(TGraphAsymmErrors * inputTGraph, TH1F * inputUpErrHist, TH1F * inputDownErrHist) {
    int NGraphPoints = inputTGraph->GetN();
    for (unsigned int iPoint = 0; iPoint < NGraphPoints; ++iPoint) {
        inputTGraph->SetPointEYhigh(iPoint, inputUpErrHist->GetBinContent(iPoint + 1));
        inputTGraph->SetPointEYlow(iPoint, inputDownErrHist->GetBinContent(iPoint + 1));
    }
}
void SetPointsHistsfromAsymmGraph(TGraphAsymmErrors * inputTGraph, TH1F * inputUpErrHist, TH1F * inputDownErrHist) {
    int NGraphPoints = inputTGraph->GetN();
    for (unsigned int iPoint = 0; iPoint < NGraphPoints; ++iPoint) {
        inputUpErrHist->SetBinContent(iPoint + 1, inputTGraph->GetErrorYhigh(iPoint));
        inputDownErrHist->SetBinContent(iPoint + 1, inputTGraph->GetErrorYlow(iPoint));
    }    
}

void SmoothedTGraph(TGraphAsymmErrors * inputTGraph) {
    int NSmooth = 3;
    int NGraphPoints = inputTGraph->GetN();
    TString GraphName = inputTGraph->GetName();
    Double_t xMin, xMax, yMin, yMax;
    Double_t xCurr, yCurr;
    inputTGraph->GetPoint(0, xMin, yMin); 
    inputTGraph->GetPoint(1, xCurr, yCurr); 
    Double_t xBinWidth = xCurr - xMin;
    inputTGraph->GetPoint(NGraphPoints - 1, xMax, yMax);
    TH1F * UpErr = new TH1F(GraphName + TString("UpErrSmooth"), "", NGraphPoints, xMin - (0.5 * xBinWidth), xMax + (0.5 * xBinWidth));
    TH1F * DownErr = new TH1F(GraphName + TString("DownErrSmooth"), "", NGraphPoints, xMin - (0.5 * xBinWidth), xMax + (0.5 * xBinWidth));
    SetPointsHistsfromAsymmGraph(inputTGraph, UpErr, DownErr);
    UpErr->Smooth(NSmooth);
    DownErr->Smooth(NSmooth);
    SetPointsAsymmGraphfromHists(inputTGraph, UpErr, DownErr);
}
