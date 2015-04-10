// Contains relevant header files to include for the purposes
// of the Legacy nTuple skimmer

#include "TChain.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TMath.h"
#include "TRandom.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TF1.h"
#include "TTree.h"
#include "TString.h"
#include "TRegexp.h"
#include "TProfile.h"
#include <sys/stat.h>
#include "TCut.h"

#include "./Hasher.h"
#include "./StopSpecialFunctions.h"
#include "./BasicFunctions.h"
#include "./StopStructs_BasicParticleStructs.h"
#include "./StopStructs_BasicParticleFunctions.h"
#include "./StopStructs_ParticlePointers.h"
#include "./StopStructs_GenEventParticleInfo.h"
#include "./StopFunctions_HadronicResolution.h"
#include "./StopStructs_RecoEventParticleInfo.h"
#include "./StopStructs_RecoEventParticleFunctions.h"
#include "./Typedefs.h"
#include "./StopStructDefinitions.h"
#include "./StopTreeManipulations/StopTreeManipulations.h"
#include "./StopFunctionDefinitions_v2.h"
#include "./StopFunctionDefinitions_Plots.h"
#include "./StopFillFunctions.h"
#include "./StopPlotStructs_PlotMakingMacro.h"
#include "./StopFakeLeptonFunctions.h"
#include "./StopPlotHaddInfo.h"
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>
