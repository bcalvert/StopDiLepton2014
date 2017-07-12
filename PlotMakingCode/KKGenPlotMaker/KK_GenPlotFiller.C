#include "../../HeaderFiles/KKHeaderFiles/PlotMakingRunParams_KK.h"
#include "../../HeaderFiles/KKHeaderFiles/EventMETInfo.h"
#include "../../HeaderFiles/KKHeaderFiles/METLabelStrings.h"
#include "../../HeaderFiles/KKHeaderFiles/HistogramT.h"
#include "../../HeaderFiles/KKHeaderFiles/SampleT.h"
#include "../../HeaderFiles/KKHeaderFiles/StopHistBinBounds.h"
#include "../../HeaderFiles/KKHeaderFiles/KKHistDefinitions.h"
#include "../../HeaderFiles/KKHeaderFiles/Hasher.h"
#include "../../HeaderFiles/KKHeaderFiles/HasherPart2.h"
#include "../../HeaderFiles/KKHeaderFiles/StopFunctions_PlotFillingShowing_HistogramBooking.h"
//#include "../../HeaderFiles/KKHeaderFiles/HistogramFilling.h"

#include "TString.h"
#include "TFile.h"
#include "TChain.h"
#include "TRegexp.h"

#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>

//different nVtx regions for the plots
int main( int argc, char* argv[] ) {
    /////////Variable initializations/////////////////
    /////Organization Variables//////////////
    

    
    PlotMakingRunParamsKK paramsPlotMaking_KK;
    paramsPlotMaking_KK.SetVals(argc, argv);
    paramsPlotMaking_KK.SetStrings();

    /*
    char Buffer[500];
    std::ifstream * outDirFile;
    TRegexp fCutSlash("[^/]+$");
    */
    TString tsOutFileName = paramsPlotMaking_KK.GetOutputString();

    std::cout << "saving to " << tsOutFileName << std::endl;
    TFile * tfOutputFile = new TFile(tsOutFileName,"RECREATE");

    TString tsFileTreeName("OviSkimTree");
    TChain tcFileTree(tsFileTreeName);
    /////Set up the tree////////
    tcFileTree.Add(paramsPlotMaking_KK.GetFileName() + TString(".root"));
    
    EventMETInfo infoMETPF(0, 0, false);
    infoMETPF.SetStrings();
    infoMETPF.SetTreeBranchInfo(&tcFileTree);

    EventMETInfo infoMETSmearPF(0, 0, true);
    infoMETSmearPF.SetStrings();
    infoMETSmearPF.SetTreeBranchInfo(&tcFileTree);

    EventMETInfo infoMETCalo(4, 0, false);
    infoMETCalo.SetStrings();
    infoMETCalo.SetTreeBranchInfo(&tcFileTree);


    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  BEGIN: Deal with booking of histograms
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ////Book histograms and histogram names
    tfOutputFile->cd();

    //Set up the Default Stop Hist Binnings and Bounds
    StopHistBinBounds basicSHBB; basicSHBB.DefaultVarVals();

    //Set up the mapping of input axis label variables to full out labels;
    labelMap mapVartoLabel;
    SetStringKey_StSMap_Composite(mapVartoLabel);

    // Set up the SubSampleVector (i.e. the different classes of events)
    std::vector<SampleT> * subSampVec = new std::vector<SampleT>;
    SetSubSampVec(subSampVec);

    /////initialize the vectors of HistogramTs and then create them
    int iNumDims(1);
    std::vector< std::vector<HistogramT> *> vvHistT(iNumDims);

    for (int iDim = 0; iDim < iNumDims; ++iDim) {
        vvHistT[iDim] = new std::vector<HistogramT>;
        SetHistTVec_Inclusive(vvHistT[iDim], &basicSHBB, &mapVartoLabel, iDim + 1);
        AddPatsyName(vvHistT[iDim]);
    }


       /******************************************************************************************************************************/
    
    /////Declare the Histogram Maps that will be used for determining how to fill the histograms
    typedef std::unordered_map<histKeyString, TH1 *, Hasher2, EqualFn2>      HMap_1D;
    typedef std::unordered_map<histKeyString, TH2 *, Hasher2, EqualFn2>      HMap_2D;
    typedef std::unordered_map<histKeyString, TH3 *, Hasher2, EqualFn2>      HMap_3D;

    ///Basic 1D HistMaps
    HMap_1D histMap_1D;

    ///Basic 2D HistMaps
    HMap_2D histMap_2D;

    ///Basic 3D HistMaps
    HMap_3D histMap_3D;

    /////Declare the Histogram Maps that will be used for determining how to fill the histograms
    /////Book the Histograms
    for (unsigned int iSubSamp = 0; iSubSamp < subSampVec->size(); ++iSubSamp) {
        for (int iDim = 0; iDim < iNumDims; ++iDim) {
            BookHists(vvHistT[iDim], histMap_1D, histMap_2D, histMap_3D, &subSampVec->at(iSubSamp), &basicSHBB.SAB, iDim + 1);            
        }
    }
    /////Book the Histograms
    
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    ///////////////////////  END: Deal with booking of histograms
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/
    /******************************************************************************************************************************/

    typedef std::unordered_map<TString, float, Hasher, EqualFn> StV_Map;
    /////declare the maps that will contain the values of event variables (using strings as keys)
    StV_Map StVM_Basic;


    //Need to declare the maps that will contain the values of event variables (using strings as keys)
    //c.f. https://github.com/bcalvert/StopDiLepton2014/blob/master/PlotMakingCode/NewOviStopPlotFillerRunOnSkim_wSyst.C#L784-L875
    //for the code -- yes there's a lot and yes I'm working on cleaning it up
    
    /////Iterate over events
    int const iTotalFileEvents = tcFileTree.GetEntries();
    int const iNEvents = paramsPlotMaking_KK.GetNEvents();
    std::cout << "--- Total Events in file: " << iTotalFileEvents << " events" << std::endl;

    if (iNEvents < 0) {
        std::cout << "running over all events " << std::endl;
        paramsPlotMaking_KK.SetNEvents(iTotalFileEvents);
    }
    else {
        if (iNEvents > iTotalFileEvents) paramsPlotMaking_KK.SetNEvents(iTotalFileEvents);
        std::cout << "running on just " << iNEvents << " events " << std::endl;
    }
    
    size_t const szModFactor = 1E3;
    size_t const szStartPoint = paramsPlotMaking_KK.GetStartPoint();
    size_t const szEndPoint = szStartPoint + static_cast<size_t>(iNEvents);
    for (size_t szEventIndex = szStartPoint; szEventIndex < paramsPlotMaking_KK.GetNEvents(); ++szEventIndex) {
        //    for (Long64_t szEventIndex=0; szEventIndex<1000;szEventIndex++)
        if (szStartPoint > 0 ) {
            if ((szEventIndex / szStartPoint == 1) && (szEventIndex % szStartPoint == 0)) std::cout << "event index at start point: " << szEventIndex << std::endl;
        }
        if (szEventIndex % szModFactor == 0) std::cout << szEventIndex << std::endl;
        if (szEventIndex == szEndPoint) {
            std::cout << "event index at end point (note, not running on this event!):" << szEventIndex << std::endl;
            break;
        }
        
        tcFileTree.GetEntry(szEventIndex);

        /// Clear the Maps    
        StVM_Basic.clear();

        //Need to add in the code that sets up all the event variable mapping
        //c.f. https://github.com/bcalvert/StopDiLepton2014/blob/master/PlotMakingCode/NewOviStopPlotFillerRunOnSkim_wSyst.C#L972-L1460
        //for the code -- yes there's a lot and yes I'm working on cleaning it up
        //
        //
        
        //Need to add in the code that fills the histograms given maps of variables
        //c.f. https://github.com/bcalvert/StopDiLepton2014/blob/master/PlotMakingCode/NewOviStopPlotFillerRunOnSkim_wSyst.C#L1464-L1550
        //for the code -- yes there's a lot and yes I'm working on cleaning it up
        //It's going to involve this line below
        //SetupMapsAndFillHistograms(StVM_Basic, subSampBool, subSampVec, &ESPI, &EPI, &vecVecHistT_Inclusive, &histMap_1D, &histMap_2D, &histMap_3D, METSig_Raw, iNumDims, systStopBook, 0, PMRP.SMV.doData, false, PMRP.SMV.whichDiLepType, PMRP.PSIV.doVerbosity_Plots);
    }
    std::cout << "All events done" << std::endl;
    tfOutputFile->cd();
    std::cout << "cd-ing to output directory" << std::endl;
    tfOutputFile->Write();
    std::cout << "Writing of output file done" << std::endl;
    tfOutputFile->Close();
    std::cout << "end of code" << std::endl;
}
