#!/bin/sh
wTTbarGen=${1}

source ../GeneralShellScriptFunctions/LimitScriptFunctions/MT2Bounds.sh
source ../GeneralShellScriptFunctions/BaseWorkDir.sh

#condorScript="runCondorLocal.sh"
condorScript="../GeneralScripts/runCondorTestLogArea.sh"

mindM=0
maxdM=4

minSig=1
maxSig=5

minSel=6
maxSel=6

arrT2SMS=(-1 1 0 0 0 3)
arrCharFrac=(-1 -1 25 50 75 -1)

arrMapFullSel=(0 1 2 3 4 5 8)
arrMapFullSelROC=(19 22 25 26 29 30 30)

#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################

arrIndexPolT2bw=(0 2 4 6 8)
arrIndexPolT2tt=(-1 0 10)
arrIndexPolT2tb=(-1)

#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################

baseString="./MakeMT2Binning doSC -tDilep 1 numDims 3 readBkgs -b -q"


for ((wSig=${minSig};wSig<=${maxSig};wSig++));
  do
  for ((wSel=${minSel};wSel<=${maxSel};wSel++));
    do
    selROC=${arrMapFullSelROC[${wSel}]}
    selFull=${arrMapFullSel[${wSel}]}
    strSel=" -wSel ${selROC} -tSel ${selFull}"
    T2SMS=${arrT2SMS[${wSig}]}
    charFrac=${arrCharFrac[${wSig}]}
    strSig=" iSig ${wSig} T2SMS ${T2SMS} charFrac ${charFrac}"

    arrToUseIndexPol=(${arrIndexPolT2bw[@]})
    if [ ${T2SMS} -gt 0 ]
	then
	arrToUseIndexPol=(${arrIndexPolT2tt[@]})
	if [ ${T2SMS} -gt 2 ]
	    then
	    arrToUseIndexPol=(${arrIndexPolT2tb[@]})
	fi
    fi


    for indexPol in "${arrToUseIndexPol[@]}"
      do
      strPol=" indexPol ${indexPol}"
      for ((dM=${mindM};dM<=${maxdM};dM++));
	do
	strDM=" dM ${dM} 0"
	strFile="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${dM}_selFull${selFull}.txt"
###	rm -rf ${strFile}
	strCommand="${condorScript} ${baseString}${strSel}${strSig}${strPol}${strDM} tryHack wTTbarGen ${wTTbarGen} >> ${strFile}"
##	strCommand="${condorScript} ${baseString}${strSel}${strSig}${strPol}${strDM} tryHack simFakes"
	echo ${strCommand}
	${strCommand}
	strCommand="${condorScript} ${baseString}${strSel}${strSig}${strPol}${strDM} tryHack noSC wTTbarGen ${wTTbarGen}"
##	strCommand="${condorScript} ${baseString}${strSel}${strSig}${strPol}${strDM} tryHack noSC simFakes"
###	echo ${strCommand}
###	${strCommand}
      done
    done
  done
done