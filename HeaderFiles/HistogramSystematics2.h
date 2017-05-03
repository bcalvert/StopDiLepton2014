#include <vector>
#include "TH1F.h"
#include "TH1D.h"
#include "TF1.h"
#include "TString.h"
#include "TMath.h"
#include "TGraphAsymmErrors.h"
#include "TPolyLine.h"
#include "Math/Math.h"
#include "Math/ProbFuncMathCore.h"
#include "Math/SpecFuncMathCore.h"
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
    if (doVerbosity) {
        cout << "whichErrCase " << whichErrCase << endl;
        cout << "errUp " << errUp << endl;
        cout << "errDown " << errDown << endl;
    }
}

TPolyLine * GetSystBand(vector<float> xs, vector<float> yl, vector<float> yh, float xmin, float xmax) {
    
    
    int nbin = xs.size()-1;
    
    vector< float > vecx_(8*nbin,-100000);
    vector< float > vecyl_(8*nbin,-100000);
    vector< float > vecy_(8*nbin,-100000);
    //float lsyst;
    
    bool findM=false,findm=false;
    
    for( int ibin=0; ibin<nbin; ibin++ )
    {
        vecx_[4*ibin]   = (xs[ibin]*3+xs[ibin+1])/4.;
        vecx_[4*ibin+1]   = (xs[ibin]+xs[ibin+1])/2.;
        vecx_[4*ibin+2]   = (xs[ibin]+xs[ibin+1]*3)/4.;
        vecx_[4*ibin+3]   = xs[ibin+1];
        
        if(findM) {
            vecx_[ibin] = vecx_[ibin-1];
            findM=false;
        }
        if(findm && ibin!=0) {
            vecx_[ibin] = vecx_[0];
            findm=false;
        }
        
        vecy_[4*ibin]   = yh[ibin];
        vecy_[4*ibin+1]  = (yh[ibin]+yh[ibin+1])/2.;
        vecy_[4*ibin+2]  = yh[ibin+1];
        vecy_[4*ibin+3]  = yh[ibin+1];
        vecyl_[4*ibin]  = yl[ibin];
        vecyl_[4*ibin+1]  = (yl[ibin]+yl[ibin+1])/2.;
        vecyl_[4*ibin+2]  = yl[ibin+1];
        vecyl_[4*ibin+3]  = yl[ibin+1];
        
        vecx_[8*nbin-4*ibin-1] = vecx_[4*ibin];
        vecx_[8*nbin-4*ibin-2] = vecx_[4*ibin+1];
        vecx_[8*nbin-4*ibin-3] = vecx_[4*ibin+2];
        vecx_[8*nbin-4*ibin-4] = vecx_[4*ibin+3];
        vecy_[8*nbin-4*ibin-1] = -vecyl_[4*ibin];
        vecy_[8*nbin-4*ibin-2] = -vecyl_[4*ibin+1];
        vecy_[8*nbin-4*ibin-3] = -vecyl_[4*ibin+2];
        vecy_[8*nbin-4*ibin-4] = -vecyl_[4*ibin+3];
    }
    
    int n=0;
    for( size_t ipt=0; ipt<vecx_.size(); ipt++ )
    {
        if(vecx_[ipt]>=xmin && vecx_[ipt]<=xmax)
            n++;
    }
    TPolyLine* band = new TPolyLine(n);
    int cnt=0;
    
    for( size_t ipt=0; ipt<vecx_.size(); ipt++ )
    {
        if(vecx_[ipt]>=xmin && vecx_[ipt]<=xmax) {
            band->SetPoint(cnt, vecx_[ipt], (1+vecy_[ipt])>1.6?1.6:((1+vecy_[ipt])<0.4?0.4:1+vecy_[ipt]) );
            cnt++;
        }
    }
    
    band->SetLineColor(kGray+1);
    band->SetFillColor(kGray+1);
    band->SetFillStyle(3002);
    
    return band;
}
/*
 inline TPolyLine * SystBand(TH1F * inputHist, TGraphAsymmErrors * inputSystBand) {
 //Function to take an input systematics band and create a "smoothed" systematics band
 //where there's linear interpolation between points.
 
 double currX, currY;
 double currErrYHigh, currErrYLow;
 
 vector<float> vecXPos, vecYUpErr, vecYDownErr;
 
 int nPoints = inputSystBand->GetN();
 
 int nBinsX = inputHist->GetNbinsX();
 TAxis * xAxis = inputHist->GetXaxis();
 double xMin = xAxis->GetBinLowEdge(1);
 double xMax = xAxis->GetBinUpEdge(nBinsX);
 
 for (int iPoint = 0; iPoint < nPoints; ++iPoint) {
 inputSystBand->GetPoint(iPoint, currX, currY);
 if (currX < xMin || currX > xMax) continue;
 currErrYHigh = inputSystBand->GetErrorYhigh(iPoint);
 currErrYLow = inputSystBand->GetErrorYlow(iPoint);
 
 cout << "currX " << currX << endl;
 cout << "currY + currErrYHigh " << currY + currErrYHigh << endl;
 cout << "currY - currErrYLow " << currY - currErrYLow << endl;
 
 vecXPos.push_back(currX);
 vecYUpErr.push_back(currY + currErrYHigh);
 vecYDownErr.push_back(currY - currErrYLow);
 }
 
 TPolyLine * outPolyLine = GetSystBand(vecXPos, vecYDownErr, vecYUpErr, xMin, xMax);
 
 outPolyLine->SetLineColor(inputSystBand->GetLineColor());
 outPolyLine->SetLineStyle(inputSystBand->GetLineStyle());
 outPolyLine->SetFillColor(inputSystBand->GetFillColor());
 outPolyLine->SetFillStyle(inputSystBand->GetFillStyle());
 
 return outPolyLine;
 }
 */

inline TPolyLine * SystBand(TH1F * inputHist, TGraphAsymmErrors * inputSystBand) {
    //Function to take an input systematics band and create a "smoothed" systematics band
    //where there's linear interpolation between points.
    
    int nBinsX = inputHist->GetNbinsX();
    
    TAxis * xAxis = inputHist->GetXaxis();
    double xMin = xAxis->GetBinLowEdge(1);
    double xMax = xAxis->GetBinUpEdge(nBinsX);
    
    /*
    double yMin = 0.5;
    double yMax = 1.5;
    */
    double yMin = inputHist->GetMinimum();
    double yMax = inputHist->GetMaximum();
    
    double currX, currY;
    double currBinXWidth;
    inputSystBand->GetPoint(1, currX, currY);
    
    double currXLow, currXHigh;
    double currYLow, currYHigh;
    
    int numPolyPoints = (4 * nBinsX) + 2;
    int subtract = 0;
    TPolyLine * outPolyLine = new TPolyLine(numPolyPoints);
    outPolyLine->SetPoint(0, xMin, currY);
    outPolyLine->SetPoint(numPolyPoints / 2, xMax, currY);
    //outPolyLine->SetPoint(numPolyPoints - subtract, xMin, currY);
    
    for (int iPoint = 1; iPoint <= nBinsX; ++iPoint) {
        inputSystBand->GetPoint(iPoint, currX, currY);
        currBinXWidth = xAxis->GetBinWidth(iPoint);
        
        currXLow = currX - currBinXWidth / 4;
        currXHigh = currX + currBinXWidth / 4;
        
        currYHigh = currY + inputSystBand->GetErrorYhigh(iPoint);
        currYLow = currY - inputSystBand->GetErrorYlow(iPoint);
        
        if (currYHigh > yMax) currYHigh = yMax;
        if (currYLow < yMin) currYLow = yMin;
        /*
        cout << "iPoint " << iPoint << endl;
        cout << "currX " << currX << endl;
        cout << "currY " << currY << endl;
        cout << "currXLow " << currXLow << endl;
        cout << "currXHigh " << currXHigh << endl;
        cout << "currYHigh " << currYHigh << endl;
        cout << "currYLow " << currYLow << endl;
        */
        outPolyLine->SetPoint((2 * iPoint) - 1, currXLow, currYHigh);
        outPolyLine->SetPoint((2 * iPoint), currXHigh, currYHigh);
        
        outPolyLine->SetPoint(numPolyPoints - subtract - ((2 * iPoint) - 1), currXLow, currYLow);
        outPolyLine->SetPoint(numPolyPoints - subtract - (2 * iPoint), currXHigh, currYLow);
    }
    outPolyLine->SetLineColor(inputSystBand->GetLineColor());
    outPolyLine->SetLineStyle(inputSystBand->GetLineStyle());
    outPolyLine->SetFillColor(inputSystBand->GetFillColor());
    outPolyLine->SetFillStyle(inputSystBand->GetFillStyle());
    
    return outPolyLine;
}

/*
inline TPolyLine * SystBand(TH1F * inputHist, TGraphAsymmErrors * inputSystBand) {
    //Function to take an input systematics band and create a "smoothed" systematics band
    //where there's linear interpolation between points.
    vector<double> vecXPos, vecYPos;
    
    int nPoints = inputSystBand->GetN();
    int nBinsX = inputHist->GetNbinsX();
    
    TAxis * xAxis = inputHist->GetXaxis();
    double xMin = xAxis->GetBinLowEdge(1);
    double xMax = xAxis->GetBinUpEdge(nPoints);
    
    double currX, currY;
    double currErrY;
    double currBinXWidth;
    inputSystBand->GetPoint(0, currX, currY);
    
    double currXLow, currXHigh;
    double currYLow, currYHigh;
    
    int numPolyPoints = (4 * nBinsX) + 3;
    TPolyLine * outPolyLine = new TPolyLine(numPolyPoints);
    outPolyLine->SetPoint(0, xMin, currY);
    outPolyLine->SetPoint(numPolyPoints - 1, xMin, currY);
    
    for (int iPoint = 1; iPoint <= nBinsX; ++iPoint) {
        inputSystBand->GetPoint(iPoint, currX, currY);
        currBinXWidth = xAxis->GetBinWidth(iPoint);
        currErrY = inputSystBand->GetErrorYhigh(iPoint);
        currXLow = currX - currBinXWidth / 2;
        currXHigh = currX + currBinXWidth / 2;
    }
    
    vecXPos.push_back(xMin);
    vecYPos.push_back(currY);
    //Iterate through up errors
    for (int iPoint = 1; iPoint <= nPoints; ++iPoint) {


        

        vecXPos.push_back(currXLow);
        vecYPos.push_back(currY + currErrY);
        vecXPos.push_back(currXHigh);
        vecYPos.push_back(currY + currErrY);
    }
    vecXPos.push_back(xMax);
    vecYPos.push_back(currY);
    
    //Iterate through down errors
    for (int iPoint = nPoints - 2; iPoint > 0; --iPoint) {
        inputSystBand->GetPoint(iPoint, currX, currY);
        
        cout << "iPoint " << iPoint << endl;
        cout << "currX " << currX << endl;
        cout << "currY " << currY << endl;
        cout << "currBin Center " << xAxis->GetBinCenter(iPoint) << endl;
        currBinXWidth = xAxis->GetBinWidth(iPoint);
        currErrY = inputSystBand->GetErrorYlow(iPoint);
        
        currXLow = currX - currBinXWidth / 2;
        currXHigh = currX + currBinXWidth / 2;
        vecXPos.push_back(currXHigh);
        vecYPos.push_back(currY - currErrY);
        vecXPos.push_back(currXLow);
        vecYPos.push_back(currY - currErrY);
    }
    vecXPos.push_back(xMin);
    vecYPos.push_back(currY);
    
    TPolyLine * outPolyLine = new TPolyLine(vecXPos.size(), &vecXPos[0], &vecYPos[0]);
    TString name = inputSystBand->GetName();
    
    outPolyLine->SetLineColor(inputSystBand->GetLineColor());
    outPolyLine->SetLineStyle(inputSystBand->GetLineStyle());
    outPolyLine->SetFillColor(inputSystBand->GetFillColor());
    outPolyLine->SetFillStyle(inputSystBand->GetFillStyle());
    
    return outPolyLine;
}
*/

TGraphAsymmErrors * GraphSystErrorSet_SingleSource(TH1 * HistCentralVal, TH1 * HistShiftUp, TH1 * HistShiftDown, TString systString, bool doSymError = false, int doVerbosity = -1) {
    // overloaded version of the graph error set used for grabbing specific systematic
    int nBinsX = HistCentralVal->GetNbinsX();
    Double_t errUp, errDown;
    Double_t centVal, shiftUp, shiftDown;
    Double_t binWidth;
    Double_t x;
    //bool filledSystUpHist, filledSystDownHist;
    TString name = HistCentralVal->GetName();
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(nBinsX+2);
    outGraph->SetName(name + systString);// "_ErrSingleSource");
    TString systHistName;
    //    cout << "HACK JOB " << endl;
    /*
     filledSystUpHist = (HistShiftUp->Integral() > 0) ? true : false;
     filledSystDownHist = (HistShiftDown->Integral() > 0) ? true : false;
     */
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
    bool filledSystUpHist;
    //bool filledSystDownHist;
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
    //filledSystDownHist = (HistShiftDown->Integral() > 0) ? true : false;
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
TGraphAsymmErrors * ClonePoints(TH1 * inputHist, bool cloneZero = true, bool zeroErrX = false) {
    int NBinsX = inputHist->GetNbinsX();
    double x, y;
    Double_t binWidth;
    TGraphAsymmErrors * outGraph = new TGraphAsymmErrors(NBinsX + 2);
    outGraph->SetMarkerStyle(20);
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
        if (zeroErrX) {
            outGraph->SetPointEXlow(i, 0.);
            outGraph->SetPointEXhigh(i, 0.);
        }
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
        /*
         outGraph->SetPointEXlow(i, 0.0);
         outGraph->SetPointEXhigh(i, 0.0);
         */
    }
    return outGraph;
}
/*
inline TGraph * FracGraphOutline(TH1 * centValMCHist, TGraphAsymmErrors * ratioSystGraph) {
    double x, y;
    int nBins = centValMCHist->GetNbinsX();
    TAxis * xAxis = centValMCHist->GetXaxis();
    
    TGraph outGraph = new TGraph(2 + (2 * nBins));
    outGraph->SetPoint(0, xAxis->GetXmin());
    for (int iPoint = 1; iPoint < outGraph->GetN(); ++iPoint) {
        
    }

}
*/

/*
inline TGraphAsymmErrors * SignifGraph(TH1F * ratioHist, TGraphAsymmErrors * ratioGraph = NULL) {
    double x, y;
    int nBins = ratioHist->GetNbinsX();
    TGraphAsymmErrors * signifGraph = new TGraphAsymmErrors(nBins + 2);
}
 */
inline TH1F * CorrectedRatioHist(TH1F * ratioHist, TH1F * inputSignifHist, double sigma = 1) {
    //function to make a "corrected" ratio hist where the input is the original ratio hist,
    //a significance hist detailing the statistical significance of the differences
    //between the ratio hist and the expected ratio,
    //and finally an input value of sigma for the gaussian weighted correction
    //currently assumes the desired ratio is 1
    
    float idealRatio = 1;
    
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(ratioHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    TH1F * outHist = (TH1F *) ratioHist->Clone(histName + TString("_Corrected"));
    
    float currBinRatio, currSignif;
    float currCorrFactor, currDiffFromIdeal;
    
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        currBinRatio = ratioHist->GetBinContent(iBinX);
        currSignif = inputSignifHist->GetBinContent(iBinX);
        if (currBinRatio > 1E-9 && (currBinRatio > 5 || 1./currBinRatio > 5) && TMath::Abs(currSignif) < 1) {
            outHist->SetBinContent(iBinX, 1);
            continue;
        }
        currCorrFactor = ROOT::Math::normal_cdf(TMath::Abs(currSignif), sigma);
        currDiffFromIdeal = TMath::Abs(currBinRatio - idealRatio);
        if (currBinRatio <= 1E-9) {
            outHist->SetBinContent(iBinX, 0);
        }
        else {
            outHist->SetBinContent(iBinX, currDiffFromIdeal * currCorrFactor);
        }
    }
    return outHist;
}
inline TH1F * SignifHist(TH1F * ratioHist, TGraphAsymmErrors * ratioGraph = NULL, TH1F * additionalCorrHist = NULL) {
    
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(ratioHist, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    
    TH1F * outHist = (TH1F *) ratioHist->Clone(histName + TString("_Signif"));
    outHist->GetYaxis()->SetTitle("Significance");
    outHist->GetYaxis()->SetRangeUser(-3, 3.4);
    
    bool doVerbosity = true;
    
    float currBinRatio, currBinRatioErr;
    float currRatioGraphErrUp, currRatioGraphErrDown, currRatioGraphErrAve;
    float currRatioTotalErr;
    float currSignif;
    float currAddtlCorr;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        currBinRatio = ratioHist->GetBinContent(iBinX);
        currBinRatioErr = ratioHist->GetBinError(iBinX);
        
        currRatioGraphErrUp = ratioGraph->GetErrorYhigh(iBinX);
        currRatioGraphErrDown = ratioGraph->GetErrorYlow(iBinX);
        currRatioGraphErrAve = TMath::Sqrt(0.5 * (TMath::Power(currRatioGraphErrUp, 2) + TMath::Power(currRatioGraphErrDown, 2)));
        
        currRatioTotalErr = TMath::Power(currBinRatioErr, 2) + TMath::Power(currRatioGraphErrAve, 2);
        if (additionalCorrHist) {
            currAddtlCorr = additionalCorrHist->GetBinContent(iBinX);
            currRatioTotalErr += TMath::Power(currAddtlCorr, 2);
        }
        currRatioTotalErr = TMath::Sqrt(currRatioTotalErr);
        if (currRatioTotalErr < 1E-6 && currBinRatio < 1E-6) {
            currSignif = 0;
        }
        else if (currBinRatio < 1E-6) {
            currSignif = 0;
        }
        else {
            currSignif = (currBinRatio - 1)/currRatioTotalErr;
        }
        
        if (doVerbosity) {
            cout << "currX " << ratioHist->GetXaxis()->GetBinCenter(iBinX) << endl;
            cout << "currBinRatio " << currBinRatio << endl;
            cout << "currBinRatioErr " << currBinRatioErr << endl;
            cout << "currRatioGraphErrUp " << currRatioGraphErrUp << endl;
            cout << "currRatioGraphErrDown " << currRatioGraphErrDown << endl;
            cout << "currSignif " << currSignif << endl;
        }
        
        outHist->SetBinContent(iBinX, currSignif);
        outHist->SetBinError(iBinX, 0);
    }
    return outHist;
}

inline TGraphAsymmErrors * FracGraph(TH1 * centValMCHist, TGraphAsymmErrors * errGraphJustSyst, bool doAbsRatio, float yAxisRange) {
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
            //            relUpErr = (doAbsRatio) ? errGraphJustSyst->GetErrorYlow(i) :  errGraphJustSyst->GetErrorYhigh(i);
            relUpErr = (!doAbsRatio) ? errGraphJustSyst->GetErrorYlow(i) :  errGraphJustSyst->GetErrorYhigh(i);
            relUpErr /= centValMCHist->GetBinContent(i);
            //            relDownErr = (doAbsRatio) ? errGraphJustSyst->GetErrorYhigh(i) :  errGraphJustSyst->GetErrorYlow(i);
            relDownErr = (!doAbsRatio) ? errGraphJustSyst->GetErrorYhigh(i) :  errGraphJustSyst->GetErrorYlow(i);
            relDownErr /= centValMCHist->GetBinContent(i);
        }
        ratioGraph->SetPointEYhigh(i, relUpErr);
        ratioGraph->SetPointEYlow(i, relDownErr);
        
        ratioGraph->SetPointEXlow(i, binWidth/2.);
        ratioGraph->SetPointEXhigh(i, binWidth/2.);
        /*
         ratioGraph->SetPointEXlow(i, 0.0);
         ratioGraph->SetPointEXhigh(i, 0.0);
         */
    }
    return ratioGraph;
}


inline TGraphAsymmErrors * FracGraph(TH1 * centValMCHist, TGraphAsymmErrors * errGraphJustSyst, bool doAbsRatio, float yAxisLB, float yAxisUB) {
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
            //            relUpErr = (doAbsRatio) ? errGraphJustSyst->GetErrorYlow(i) :  errGraphJustSyst->GetErrorYhigh(i);
            relUpErr = (!doAbsRatio) ? errGraphJustSyst->GetErrorYlow(i) :  errGraphJustSyst->GetErrorYhigh(i);
            relUpErr /= centValMCHist->GetBinContent(i);
            //            relDownErr = (doAbsRatio) ? errGraphJustSyst->GetErrorYhigh(i) :  errGraphJustSyst->GetErrorYlow(i);
            relDownErr = (!doAbsRatio) ? errGraphJustSyst->GetErrorYhigh(i) :  errGraphJustSyst->GetErrorYlow(i);
            relDownErr /= centValMCHist->GetBinContent(i);
        }
        ratioGraph->SetPointEYhigh(i, relUpErr);
        ratioGraph->SetPointEYlow(i, relDownErr);
        
        ratioGraph->SetPointEXlow(i, binWidth/2.);
        ratioGraph->SetPointEXhigh(i, binWidth/2.);
        /*
         ratioGraph->SetPointEXlow(i, 0.0);
         ratioGraph->SetPointEXhigh(i, 0.0);
         */
    }
    return ratioGraph;
}


void SetPointsAsymmGraphfromHists(TGraphAsymmErrors * inputTGraph, TH1F * inputUpErrHist, TH1F * inputDownErrHist) {
    int NGraphPoints = inputTGraph->GetN();
    for (int iPoint = 0; iPoint < NGraphPoints; ++iPoint) {
        inputTGraph->SetPointEYhigh(iPoint, inputUpErrHist->GetBinContent(iPoint + 1));
        inputTGraph->SetPointEYlow(iPoint, inputDownErrHist->GetBinContent(iPoint + 1));
    }
}
void SetPointsHistsfromAsymmGraph(TGraphAsymmErrors * inputTGraph, TH1F * inputUpErrHist, TH1F * inputDownErrHist) {
    int NGraphPoints = inputTGraph->GetN();
    for (int iPoint = 0; iPoint < NGraphPoints; ++iPoint) {
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

void CombineSystematics(TH1 * inputHistCV, TH1 * outputHist_SystVarUp, TH1 * outputHist_SystVarDown, vector<TH1 *> * vecInputHist_SystVarUp, vector<TH1 *> * vecInputHist_SystVarDown, bool doVerbosity = false) {
    
    //Function that takes as input central value histogram and vector of systematic varied histograms to combine them into set of two output histograms accounting for full systematic variation
    double currErrSum_SystVarUp, currErrSum_SystVarDown;
    
    int nBinsX = inputHistCV->GetNbinsX();
    int nBinsY = inputHistCV->GetNbinsY();
    int nBinsZ = inputHistCV->GetNbinsZ();
    
    double currYieldCV, currYieldSystUp, currYieldSystDown;
    double currErr_SystUp, currErr_SystDown;
    
    unsigned int nSyst = vecInputHist_SystVarDown->size();
    
    for (int iBinX = 0; iBinX <= nBinsX + 1; ++iBinX) {
        for (int iBinY = 0; iBinY <= nBinsY + 1; ++iBinY) {
            for (int iBinZ = 0; iBinZ <= nBinsZ + 1; ++iBinZ) {
                currErrSum_SystVarUp = 0.0;
                currErrSum_SystVarDown = 0.0;
                
                currYieldCV = inputHistCV->GetBinContent(iBinX, iBinY, iBinZ);
                for (unsigned int iSyst = 0; iSyst < nSyst; ++iSyst) {
                    currYieldSystUp = vecInputHist_SystVarUp->at(iSyst)->GetBinContent(iBinX, iBinY, iBinZ);
                    currYieldSystDown = vecInputHist_SystVarDown->at(iSyst)->GetBinContent(iBinX, iBinY, iBinZ);
                    pointSystErr(currYieldCV, currYieldSystUp, currYieldSystDown, currErr_SystUp, currErr_SystDown, false, doVerbosity);
                    if (doVerbosity) {
                        cout << "iSyst " << iSyst << endl;
                        cout << "iBinX " << iBinX << endl;
                        cout << "iBinY " << iBinY << endl;
                        cout << "iBinZ " << iBinZ << endl;
                        cout << "currYieldCV " << currYieldCV << endl;
                        cout << "currYieldSystUp " << currYieldSystDown << endl;
                        cout << "currErr_SystUp " << currErr_SystUp << endl;
                        cout << "currErr_SystDown " << currErr_SystDown << endl;
                    }
                    currErrSum_SystVarUp += TMath::Power(currErr_SystUp, 2);
                    currErrSum_SystVarDown += TMath::Power(currErr_SystDown, 2);
                }
                outputHist_SystVarUp->SetBinContent(iBinX, iBinY, iBinZ, currYieldCV + TMath::Sqrt(currErrSum_SystVarUp));
                outputHist_SystVarDown->SetBinContent(iBinX, iBinY, iBinZ, currYieldCV - TMath::Sqrt(currErrSum_SystVarDown));
            }
        }
    }
}

vector<bool> VecSmearSysts() {
    const int numSysts = 11;
    bool isSmearSyst[numSysts] = {false, false, false, false, true, true, false, false, false, false, false};
    vector<bool> outVec;
    outVec.assign(&isSmearSyst[0], &isSmearSyst[0] + numSysts);
    
    return outVec;
}

vector<TString> VecNameSysts() {
    const int numSysts = 11;
    TString namesSysts[numSysts] = {"LepEffSF", "LepES", "JetES", "BTagSF", "UncES", "JetSmear", "genRecoilRW", "TTBarNorm", "DYNorm", "EWKNorm", "FakeLepSyst"};
    vector<TString> outVec;
    outVec.assign(&namesSysts[0], &namesSysts[0] + numSysts);
    
    return outVec;
}

vector<TGraphAsymmErrors *> VecGraphs(vector<TGraphAsymmErrors *> * inVecSearchSysts, vector<TString> * inVecSearchNames) {
    unsigned int numSystNames = inVecSearchNames->size();
    unsigned int numSearchSysts = inVecSearchSysts->size();
    
    TGraphAsymmErrors * currSystBand = NULL;
    TString currSystBandName = "";
    TString currSystSearchName = "";
    
    vector<TGraphAsymmErrors *> outVec;
    
    bool foundSystName;
    
    for (unsigned iSystName = 0; iSystName < numSystNames; ++iSystName) {
        foundSystName = false;
        currSystSearchName = inVecSearchNames->at(iSystName);
        for (unsigned int iSyst = 0; iSyst < numSearchSysts; ++iSyst) {
            currSystBand = inVecSearchSysts->at(iSyst);
            currSystBandName = currSystBand->GetName();
            if (currSystBandName.Contains(currSystSearchName)) {
                foundSystName = true;
                /*
                 cout << "currSystSearchName " << currSystSearchName << endl;
                 cout << "currSystBandName " << currSystBandName << endl;
                 cout << "currSystBand " << currSystBand << endl;
                 */
                //outVec.push_back((TGraphAsymmErrors *) currSystBand->Clone(TString("SplitRatioGraph_") + currSystSearchName));
                outVec.push_back(currSystBand);
            }
        }
        if (!foundSystName) {
            cout << "didn't find " << currSystSearchName << " in any of the syst bands!" << endl;
        }
    }
    return outVec;
}

void ResetRatioHist(TH1F * inputRatioHist) {
    int nBinsX = inputRatioHist->GetNbinsX();
    for (int iBinX = 1; iBinX <= nBinsX; ++iBinX) {
        inputRatioHist->SetBinContent(iBinX, 1.0);
    }
}




void ResetSystHists(TH1 * inputHist_CV, TH1 * inputHistSystVarUp, TH1 * inputHistSystVarDown, bool doMaxRelChange, float maxRelChange, bool doVerbosity = false) {
    ////Correct the systematic shift histograms when things have changed from initial creation to desired output
    ///i.e. if non-constant smearing or smoothing is applied to CV, Shift Up, and Shift Down hists separately
    //Set up histogram data
    TString histName;
    vector<int> vecInputHistAxisNBins;
    vector<TAxis *> vecInputHistAxes;
    vector<float> vecInputHistAxisBinWidths;
    SetHistogramData(inputHist_CV, histName, &vecInputHistAxisNBins, &vecInputHistAxes, &vecInputHistAxisBinWidths);
    
    double currYieldCV, currYieldSystUp, currYieldSystDown;
    double currErr_SystUp, currErr_SystDown;
    for (int iBinX = 1; iBinX <= vecInputHistAxisNBins[0]; ++iBinX) {
        for (int iBinY = 1; iBinY <= vecInputHistAxisNBins[1]; ++iBinY) {
            for (int iBinZ = 1; iBinZ <= vecInputHistAxisNBins[2]; ++iBinZ) {
                currYieldCV = inputHist_CV->GetBinContent(iBinX, iBinY, iBinZ);
                currYieldSystUp = inputHistSystVarUp->GetBinContent(iBinX, iBinY, iBinZ);
                currYieldSystDown = inputHistSystVarDown->GetBinContent(iBinX, iBinY, iBinZ);
                
                pointSystErr(currYieldCV, currYieldSystUp, currYieldSystDown, currErr_SystUp, currErr_SystDown, false, doVerbosity);
                if (doMaxRelChange) {
                    currErr_SystUp = TMath::Min(currYieldCV * (maxRelChange - 1), currErr_SystUp); //based on max relative changes
                    currErr_SystDown = TMath::Min(currYieldCV * (maxRelChange - 1), currErr_SystDown);
                }
                if (currErr_SystDown > currYieldCV) currErr_SystDown = currYieldCV; // minimum error down shift of -100%
                inputHistSystVarUp->SetBinContent(iBinX, iBinY, iBinZ, currYieldCV + currErr_SystUp);
                inputHistSystVarDown->SetBinContent(iBinX, iBinY, iBinZ, currYieldCV - currErr_SystDown);
            }
        }
    }
    
}

