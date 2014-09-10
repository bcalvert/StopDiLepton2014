// Functions that operate on either the HistogramDisplayStructs Struct or the IndSamplePlotInfo Struct
// Used as part of prepping the plots to show

using namespace std;
typedef pair<int, int> intBounds;
typedef pair<TString, intBounds> indMCParams;


void SumIndSamps_PointerVersion(vector<IndSamplePlotInfo> * vecInISPI, IndSamplePlotInfo * outISPI, TString nameOutISPI, int initialIndex = 0, int finalIndex = -1, bool useNameOutISPI = true, bool useSampTypeOutISPI = true, bool checkSampleTypeConsistency = false, bool doVerbosity = false) {
    outISPI->SetName(nameOutISPI);
    if (doVerbosity) cout << "name of outISPI " << outISPI->nameISPI << endl;
    int startIndex = initialIndex;
    int endIndex = finalIndex > -1 ? finalIndex : (int) vecInISPI->size() - 1;
    for (int iISPI = startIndex; iISPI <= endIndex; ++iISPI) {
        if (doVerbosity) cout << "iISPI " << iISPI << endl;
        if (iISPI == startIndex) {
            outISPI->CloneHists(&vecInISPI->at(iISPI), useNameOutISPI, useSampTypeOutISPI, doVerbosity);
        }
        else {
            outISPI->AddHists(&vecInISPI->at(iISPI), checkSampleTypeConsistency, doVerbosity);
        }
    }
}
IndSamplePlotInfo SumIndSamps(vector<IndSamplePlotInfo> * vecInISPI, TString nameOutISPI, int initialIndex = 0, int finalIndex = -1, bool useNameOutISPI = true, bool checkSampleTypeConsistency = false, bool doVerbosity = false) {
    IndSamplePlotInfo outISPI;
    outISPI.SetName(nameOutISPI);
    if (doVerbosity) cout << "name of outISPI " << outISPI.nameISPI << endl;
    int startIndex = initialIndex;
    int endIndex = finalIndex > -1 ? finalIndex + 1 : (int) vecInISPI->size();
    for (int iISPI = startIndex; iISPI < endIndex; ++iISPI) {
        if (iISPI == startIndex) {
            outISPI.CloneHists(&vecInISPI->at(iISPI), useNameOutISPI, doVerbosity);
        }
        else {
            outISPI.AddHists(&vecInISPI->at(iISPI), checkSampleTypeConsistency, doVerbosity);
        }
    }
    return outISPI;
}
void HistogramAdder(vector<IndSamplePlotInfo> * vecISPI_asLoaded, IndSamplePlotInfo * compISPI, TString compName, bool doVerbosity = false, vector<SampDisplay> * vecIndMC_SampDisplays = 0, vector<indMCParams> * vecIndMCParams = 0) {
    
    bool useNameOutISPIComp = false, checkCompSampTypeConsistency = false;
    bool useSampTypeOutISPIComp = false;
    
    bool useNameOutISPIIndMC = false, checkIndSampTypeConsistency = true;
    bool useSampTypeOutISPIIndMC = true;
    
    int begIndex = 0;
    int endIndex = (int) vecISPI_asLoaded->size() - 1;
    TString baseName = vecISPI_asLoaded->at(0).grabbedHist->GetName();
    TString addName = compName;
    if (doVerbosity) {
        cout << "about to sum together vecISPI_asLoaded " << vecISPI_asLoaded->size() << endl;
    }
    SumIndSamps_PointerVersion(vecISPI_asLoaded, compISPI, baseName + addName, begIndex, endIndex, useNameOutISPIComp, useSampTypeOutISPIComp, checkCompSampTypeConsistency, doVerbosity);
    if (doVerbosity) {
        cout << "going to do the CompHist projection now " << endl;
    }
    if (vecIndMC_SampDisplays != NULL) {
        if (doVerbosity) {
            cout << "vecIndMC_SampDisplays size " << vecIndMC_SampDisplays->size() << endl;
        }
        for (unsigned int iIndMC = 0; iIndMC < vecIndMCParams->size(); ++iIndMC) {
            IndSamplePlotInfo currISPI;
            begIndex = vecIndMCParams->at(iIndMC).second.first;
            endIndex = vecIndMCParams->at(iIndMC).second.second;
            addName = vecIndMCParams->at(iIndMC).first;
            if (doVerbosity) {
                cout << "iIndMC " << iIndMC << endl;
                cout << "begIndex " << begIndex << endl;
                cout << "endIndex " << endIndex << endl;
                cout << "addName " << addName << endl;
            }
            SumIndSamps_PointerVersion(vecISPI_asLoaded, &currISPI, baseName + addName, begIndex, endIndex, useNameOutISPIIndMC, useSampTypeOutISPIIndMC, checkIndSampTypeConsistency, doVerbosity);
            
            vecIndMC_SampDisplays->at(iIndMC).first.CloneHists(&currISPI, true, true);
            //            currISPI.SetTH1F_Projection(inAPI, inHDP);
        }
    }
}
void HistogramAdderProjector(AxisProjInfo * inAPI, HistDisplayParams * inHDP, vector<IndSamplePlotInfo> * vecISPI_asLoaded, IndSamplePlotInfo * compISPI, TString compName, bool doProjection, bool doVerbosity = false, vector<SampDisplay> * vecIndMC_SampDisplays = 0, vector<indMCParams> * vecIndMCParams = 0) {
    
    bool useNameOutISPIComp = false, checkCompSampTypeConsistency = false;
    bool useSampTypeOutISPIComp = false;
    
    bool useNameOutISPIIndMC = false, checkIndSampTypeConsistency = true;
    bool useSampTypeOutISPIIndMC = true;
    
    int begIndex = 0;
    int endIndex = (int) vecISPI_asLoaded->size() - 1;
    TString baseName = vecISPI_asLoaded->at(0).grabbedHist->GetName();
    TString addName = compName;
    if (doVerbosity) {
        cout << "about to sum together vecISPI_asLoaded " << vecISPI_asLoaded->size() << endl;
    }
    SumIndSamps_PointerVersion(vecISPI_asLoaded, compISPI, baseName + addName, begIndex, endIndex, useNameOutISPIComp, useSampTypeOutISPIComp, checkCompSampTypeConsistency, doVerbosity);
    if (doVerbosity) {
        cout << "going to do the CompHist projection now " << endl;
    }
    if (doProjection) {
        compISPI->SetTH1F_Projection(inAPI, inHDP, doVerbosity);
    }
    if (vecIndMC_SampDisplays != NULL) {
        if (doVerbosity) {
            cout << "vecIndMC_SampDisplays size " << vecIndMC_SampDisplays->size() << endl;
        }
        for (unsigned int iIndMC = 0; iIndMC < vecIndMCParams->size(); ++iIndMC) {
            IndSamplePlotInfo currISPI;
            begIndex = vecIndMCParams->at(iIndMC).second.first;
            endIndex = vecIndMCParams->at(iIndMC).second.second;
            addName = vecIndMCParams->at(iIndMC).first;
            if (doVerbosity) {
                cout << "iIndMC " << iIndMC << endl;
                cout << "begIndex " << begIndex << endl;
                cout << "endIndex " << endIndex << endl;
                cout << "addName " << addName << endl;
            }
            SumIndSamps_PointerVersion(vecISPI_asLoaded, &currISPI, baseName + addName, begIndex, endIndex, useNameOutISPIIndMC, useSampTypeOutISPIIndMC, checkIndSampTypeConsistency, doVerbosity);
            
            vecIndMC_SampDisplays->at(iIndMC).first.CloneHists(&currISPI, true, true);
            if (doProjection) {
                vecIndMC_SampDisplays->at(iIndMC).first.SetTH1F_Projection(inAPI, inHDP);
            }
//            currISPI.SetTH1F_Projection(inAPI, inHDP);
        }
    }
}