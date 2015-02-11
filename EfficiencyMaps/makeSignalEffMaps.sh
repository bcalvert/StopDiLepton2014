#!/bin/sh
doLocal=${1}
whichMacro=${2}
isT2tt=${3} #0 for T2bw, 1 for T2tt
numDims=${4} #numDims -- should be 1, 2, or 3
source ../GeneralShellScriptFunctions/LimitScriptFunctions/MT2Bounds.sh
source ../GeneralShellScriptFunctions/BaseWorkDir.sh

if [ ${doLocal} -gt 0 ]
    then
    condorScript=$(BaseCondorScriptName 1)
else
    condorScript=$(BaseCondorScriptName 2)
fi

#############################
##Set up which macro to run##
#############################
arrMacroToUse=("MakeLeptFiltSF" "MakeCoarseEffMap" "MakeSmoothedEffMap" "MakeSignalYield" "MakeSignalYieldText")
strMacroCommand="./${arrMacroToUse[${whichMacro}]}"
if [ ${whichMacro} -gt 0 ]
    then
    strMacroCommand="${condorScript} ${strMacroCommand}"
fi
#############################
##Set up which macro to run##
#############################

################################
##Set up which MT2 cuts to use##
################################
arrMT2ll1D=(0 80 85 90 95 100 105 110 115 120 200)
arrMT2lblb1D=(0)
arrMT2bb_ZMET1D=(-1)

arrMT2ll2D=(0 80 100 150)
arrMT2lblb2D=(0 150 200 400)
arrMT2bb_ZMET2D=(-1)

arrMT2ll3D=(0 80 100 150)
arrMT2lblb3D=(0 150 200 400)
arrMT2bb_ZMET3D=(70 170 250 450)

arrToUseMT2ll=(${arrMT2ll1D[@]})
arrToUseMT2lblb=(${arrMT2lblb1D[@]})
arrToUseMT2bb_ZMET=(${arrMT2bb_ZMET1D[@]})

if [ ${numDims} -eq 2 ]
    then
    arrToUseMT2ll=(${arrMT2ll2D[@]})
    arrToUseMT2lblb=(${arrMT2lblb2D[@]})
    arrToUseMT2bb_ZMET=(${arrMT2bb_ZMET2D[@]})
elif [ ${numDims} -eq 3 ]
    then
    arrToUseMT2ll=(${arrMT2ll3D[@]})
    arrToUseMT2lblb=(${arrMT2lblb3D[@]})
    arrToUseMT2bb_ZMET=(${arrMT2bb_ZMET3D[@]})
fi
################################
##Set up which MT2 cuts to use##
################################

#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################

arrIndexPolT2tt=(0 5 10)
arrCharFracT2tt=(0)

arrIndexPolT2bw=(0 2 4 6 8)
arrCharFracT2bw=(25 50 75)

arrToUseIndexPol=(${arrIndexPolT2bw[@]})
arrToUseCharFrac=(${arrCharFracT2bw[@]})
if [ ${isT2tt} -gt 0 ]
    then
    arrToUseIndexPol=(${arrIndexPolT2tt[@]})
    arrToUseCharFrac=(${arrCharFracT2tt[@]})
fi
#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################


for charFrac in "${arrToUseCharFrac[@]}"
  do
  for indexPol in "${arrToUseIndexPol[@]}"
    do
    if [ ${whichMacro} -gt 3 ]
	then
	${strMacroCommand} T2SMS ${isT2tt} indexPol ${indexPol} charFrac ${charFrac} -b -q
	${strMacroCommand} T2SMS ${isT2tt} indexPol ${indexPol} charFrac ${charFrac} -b -q doSC
    fi
    if [ ${numDims} -eq 1 ]
	then
	echo generating control stuff 
	echo indexPol ${indexPol}
	if [ ${isT2tt} -lt 1 ]
	    then
	    echo T2bw charFrac ${charFrac}
	else
	    echo T2tt
	fi	
	if [ ${whichMacro} -lt 4 ]
	    then
	    ${strMacroCommand} T2SMS ${isT2tt} indexPol ${indexPol} charFrac ${charFrac} doControl controlCuts 80 0 -1 -b -q
	    if [ ${whichMacro} -gt 0 ]
		then
		${strMacroCommand} T2SMS ${isT2tt} indexPol ${indexPol} charFrac ${charFrac} doControl controlCuts 80 150 170 -b -q
	    fi
	fi
    fi
    if [ ${whichMacro} -gt 0 ]
	then
	for MT2llCut in "${arrToUseMT2ll[@]}"
	  do
	  for MT2lblbCut in "${arrToUseMT2lblb[@]}"
	    do
	    for MT2bb_ZMETCut in "${arrToUseMT2bb_ZMET[@]}"
	      do
	      if [ ${isT2tt} -lt 1 ]
		  then
		  echo T2bw charFrac ${charFrac}
	      else
		  echo T2tt
	      fi
	      echo indexPol ${indexPol}
	      echo MT2llCut ${MT2llCut}
	      echo MT2lblbCut ${MT2lblbCut}
	      echo MT2bb_ZMETCut ${MT2bb_ZMETCut}
	      if [ ${whichMacro} -lt 4 ]
		  then
		  ${strMacroCommand} T2SMS ${isT2tt} indexPol ${indexPol} charFrac ${charFrac} cuts ${MT2llCut} ${MT2lblbCut} ${MT2bb_ZMETCut} -b -q
	      fi
	    done
	  done
	done
    fi
  done
done