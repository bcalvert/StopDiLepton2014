#! /bin/bash
cp StopTreeManipulations/StopOutTreeManipulations.h StopTreeManipulations/StopInTreeManipulations_PlotMaker.h
sed -i.bak "s/outTree/inTree/g" StopTreeManipulations/StopInTreeManipulations_PlotMaker.h
sed -i.bak "s/OutTreeBranch/InTreeBranch_PlotMaker_/g" StopTreeManipulations/StopInTreeManipulations_PlotMaker.h
sed -i.bak "s/->Branch/->SetBranchAddress/g" StopTreeManipulations/StopInTreeManipulations_PlotMaker.h
sed -i.bak "s/SetDiStructureBranch/SetDiStructureBranchAddress/g" StopTreeManipulations/StopInTreeManipulations_PlotMaker.h
sed -i.bak "s/SetOutBranches/SetInBranches/g" StopTreeManipulations/StopInTreeManipulations_PlotMaker.h