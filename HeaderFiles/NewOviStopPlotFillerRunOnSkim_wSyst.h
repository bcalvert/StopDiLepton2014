// Contains relevant header files for the purposes of the Stop Plot Filler Macro
#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TMath.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TTree.h"
#include "TString.h"
#include "TRegexp.h"
#include "TProfile.h"
#include <fstream>
#include <string>
#include <iostream>
//#include <exception>                                                                                                      
#include <sys/stat.h>

#include "TCut.h"

#include "./Hasher.h"
#include "./StopSpecialFunctions.h"
#include "./BasicFunctions.h"
#include "./StopStructs_BasicParticleStructs.h"
#include "./StopStructs_BasicParticleFunctions.h"
#include "./StopStructs_ParticlePointers.h"
#include "./StopStructs_GenEventParticleInfo.h"
#include "./StopStructs_RecoEventParticleInfo.h"
#include "./Typedefs.h"
#include "./StopStructDefinitions.h"
#include "./HistogramPlottingStructs_SampleCut.h"
#include "./HistogramPlottingStructs.h"
#include "./HistogramPlottingStructs_StopSpecific.h"
#include "./HasherPart2.h"
#include "./TypedefsPart2.h"
#include "./StopFillFunctions.h"
#include "./StopPlotStructs_PlotMakingMacro.h"
#include "./StopFunctions_PlotFilling.h"
#include "./StopFunctionDefinitions_Plots.h"
#include "./StopFunctionDefinitions_v2.h"
#include "./StopTreeManipulations/StopTreeManipulations.h"
#include "./StopFunctions_MapShenanigans.h"
#include "./StopFakeLeptonFunctions.h"
