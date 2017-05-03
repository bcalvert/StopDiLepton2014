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

strSC=""
if [ ${doSC} -gt 0 ]
    then
    strSC="doSC"
fi


baseCommand="${condorScript} ./MakeSignalYieldMT2_IndivPoint -b -q T2SMS ${whichT2} indexPol ${indexPol} charFrac ${charFrac} numDims ${numDims} ${strSC} -tDilep 1 -wDilep ${whichDilep} -tSel ${whichSel}"

massString="minStop ${minStopMass} maxStop ${maxStopMass} minLSP ${minLSPMass} maxLSP ${maxLSPMassFix}"

minStop ${minStopMass} maxStop ${maxStopMass} minLSP ${minLSPMass} maxLSP ${maxLSPMassFix}


for (( massStop=${minStopMass}; massStop<=${maxStopMass}; massStop+=${stepSizeStopMass} ));
  do
    let "maxLSPMass = ${massStop} - 100"
    for (( massLSP=${minLSPMass}; massLSP<=${maxLSPMass}; massLSP+=${stepSizeLSPMass} ));
      do
      if [ ${massLSP} -gt ${maxLSPMassFix} ]
          then
          continue
      fi
      if [ ${whichT2} -gt 0 ]	  
      then
	  if [ ${whichT2} -gt 2 ]
	      then
	      echo T2tb
	  else
	      echo T2tt
	  fi
      else
	  echo T2bw CharFrac ${charFrac}
      fi
      echo indexPol ${indexPol}
      echo massStop ${massStop}
      echo massLSP ${massLSP}
      echo numDims ${numDims}

      ${baseCommand} massSUSY ${massStop} ${massLSP}
#      ${condorScript} ./MakeSignalYieldMT2_IndivPoint T2SMS ${whichT2} indexPol ${indexPol} charFrac ${charFrac} numDims ${numDims} massSUSY ${massStop} ${massLSP} ${strSC}
    done
done