#!/bin/sh
whichT2=${1}
doSC=${2}
doLocal=${3}

minDim=1
maxDim=3

for (( dim=${minDim}; dim<=${maxDim}; dim++ )); do
    ./runSignalMT2Maker_AllT2SMS.sh ${dim} ${doSC} ${whichT2} ${doLocal}
done
