#include "../../HeaderFiles/KKHeaderFiles/PlotMakingRunParams_KK.h"
#include "../../HeaderFiles/KKHeaderFiles/EventMETInfo.h"

#include "TString.h"
#include "TFile.h"
#include "TChain.h"
#include "TRegexp.h"

#include <fstream>
#include <vector>

#include <iostream>

//different nVtx regions for the plots
int main( int argc, char* argv[] ) {
    /////////Variable initializations/////////////////
    /////Organization Variables//////////////
    

    
    PlotMakingRunParamsKK paramsPlotMaking_KK;
    paramsPlotMaking_KK.SetVals(argc, argv);
    paramsPlotMaking_KK.SetStrings();


    char Buffer[500];
    std::ifstream * outDirFile;
    TRegexp fCutSlash("[^/]+$");

    TString tsOutFileName = paramsPlotMaking_KK.GetOutputString();

    std::cout << "saving to " << tsOutFileName << std::endl;
    TFile * tfOutputFile = new TFile(tsOutFileName,"RECREATE");

    TString tsFileTreeName("Events");
    TChain tcFileTree(tsFileTreeName);
    /////Set up the tree////////
    tcFileTree.Add(paramsPlotMaking_KK.GetFileName() + TString(".root"));
    
    EventMETInfo infoMETPF(0, 0, false);
    infoMETPF.SetStrings();
    infoMETPF.SetTreeBranchInfo(&tcFileTree);

    EventMETInfo infoMETCalo(4, 0, false);
    infoMETCalo.SetStrings();
    infoMETCalo.SetTreeBranchInfo(&tcFileTree);
    
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
    for (Long64_t ievt = szStartPoint; ievt < paramsPlotMaking_KK.GetNEvents(); ++ievt) {
        //    for (Long64_t ievt=0; ievt<1000;ievt++)
        if (szStartPoint > 0 ) {
            if ((ievt / szStartPoint == 1) && (ievt % szStartPoint == 0)) std::cout << "ievt at start point: " << ievt << std::endl;
        }
        if (ievt % szModFactor == 0) std::cout << ievt << std::endl;
        if (ievt == szEndPoint) {
            std::cout << "ievt at end point (note, not running on this event!):" << ievt << std::endl;
            break;
        }
        
        tcFileTree.GetEntry(ievt);
        
    }
    std::cout << "All events done" << std::endl;
    tfOutputFile->cd();
    std::cout << "cd-ing to output directory" << std::endl;
    tfOutputFile->Write();
    std::cout << "Writing of output file done" << std::endl;
    tfOutputFile->Close();
    std::cout << "end of code" << std::endl;
}
