#!/bin/sh
doLocal=${1}
whichMacro=${2}
whichT2=${3}
whichFullSel=${4}
doVerbosity=${5}


minDim=1
maxDim=3

for ((dim=${minDim};dim<=${maxDim};dim++));
do
    ./makeSignalEffMaps.sh ${doLocal} ${whichMacro} ${whichT2} ${dim} ${whichFullSel} ${doVerbosity}    
done