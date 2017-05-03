#!/bin/sh
doLocal=${1}
whichMacro=${2}
whichT2=${3} #0 for T2bw, 1 for T2tt, 2 for T2tt Tight Bin, 3 for T2tb
doVerbosity=${4}

numDims=3
doControl=1
arrWhichFullSel=(1 2 3)

for wFullSel in ${arrWhichFullSel[@]}
  do
  ./makeSignalEffMaps.sh ${doLocal} ${whichMacro} ${whichT2} ${numDims} ${wFullSel} ${doControl} ${doVerbosity}
done