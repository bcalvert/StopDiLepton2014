#!/bin/sh


arrDirT2SMS=("T2tt/PerPol0/" "T2bw/CharFrac0.75/SCharSW/")

arrDirEffMap=("CoarseMaps/" "SmoothMaps/" "SignalYields/")

arrDirSubDir=("SignalRegion/" "ControlRegion/")

for dirT2SMS in ${arrDirT2SMS[@]}
  do
  for dirEffMap in ${arrDirEffMap[@]}
    do
    for dirSubDir in ${arrDirSubDir[@]}
      do
      fullDir=${dirT2SMS}${dirEffMap}${dirSubDir}
      echo fullDir ${fullDir}
      ls ${fullDir}/h_*EMu_*_MT2ll80AND_MT2lblb170AND_MT2bb_ZMET170.root | sed "s/\(.*\)\(LowBLepMass\)/\2/"
    done
  done
done