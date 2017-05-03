#!/bin/sh
source ../GeneralShellScriptFunctions/BaseWorkDir.sh


minStopMass=${1}
maxStopMass=${2}
maxLSPMassFix=${3}
whichT2=${4}
indexPol=${5}
charFrac=${6}
numDims=${7}
doSC=${8}
doLocal=${9}
whichDilep=${10}
whichSel=${11}


minLSPMass=0

if [ ${doLocal} -gt 0 ]
    then
    condorScript=$(BaseCondorScriptName 1)
else
    condorScript=$(BaseCondorScriptName 2)
fi

stepSizeStopMass=25
stepSizeLSPMass=25

strSC="noSC"
if [ ${doSC} -gt 0 ]
    then
    strSC="doSC"
fi


baseCommand="${condorScript} ./MakeSignalYieldMT2_IndivPoint -b -q T2SMS ${whichT2} indexPol ${indexPol} charFrac ${charFrac} numDims ${numDims} ${strSC} -tDilep 1 -wDilep ${whichDilep} -tSel ${whichSel}"

massString="minStop ${minStopMass} maxStop ${maxStopMass} minLSP ${minLSPMass} maxLSP ${maxLSPMassFix}"

echo "${baseCommand} ${massString}"
${baseCommand} ${massString}