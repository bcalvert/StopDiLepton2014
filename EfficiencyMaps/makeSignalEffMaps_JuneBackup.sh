#!/bin/sh
doLocal=${1}
whichMacro=${2}
whichT2=${3} #0 for T2bw, 1 for T2tt, 2 for T2tt Tight Bin, 3 for T2tb
numDims=${4} #numDims -- should be 1, 2, or 3
whichFullSel=${5} #which full selection to apply -- should be 0, 1, 2, or 3
doVerbosity=${6}

if [ ${numDims} -gt 0 ]
    then
    typeDilep=0
else
    typeDilep=1    
fi

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
if [ ${whichT2} -eq 2 ]
    then
    arrMacroToUse[1]="MakeCoarseEffMap_TightBin"
fi
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
arrMT2ll0D=(0 80 90 100 110 120)
arrMT2lblb0D=(0)
arrMT2bb_ZMET0D=(-1)

arrMT2ll1D=(0 80 85 90 95 100 105 110 115 120 200)
arrMT2lblb1D=(0)
arrMT2bb_ZMET1D=(-1)

arrMT2ll2D=(0 80 100 150)
arrMT2lblb2D=(0 150 175 200 400)
arrMT2bb_ZMET2D=(-1)

arrMT2ll3D=(0 80 100 150)
arrMT2lblb3D=(0 150 200 400)
arrMT2bb_ZMET3D=(70 170 250 450)

arrToUseMT2ll=(${arrMT2ll0D[@]})
arrToUseMT2lblb=(${arrMT2lblb0D[@]})
arrToUseMT2bb_ZMET=(${arrMT2bb_ZMET0D[@]})

if [ ${numDims} -eq 1 ]
    then
    arrToUseMT2ll=(${arrMT2ll1D[@]})
    arrToUseMT2lblb=(${arrMT2lblb1D[@]})
    arrToUseMT2bb_ZMET=(${arrMT2bb_ZMET1D[@]})
elif [ ${numDims} -eq 2 ]
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

############################################
##Set up which dilepton types to run over ##
############################################

arrDilep=(-1 0 1 2)

############################################
##Set up which dilepton types to run over ##
############################################


#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################

arrIndexPolT2bw=(0 2 4 6 8)
arrCharFracT2bw=(25 50 75)

arrIndexPolT2tt=(-1 0 10)
#arrIndexPolT2tt=(-1) #0 5 10)
arrCharFracT2tt=(0)

arrIndexPolT2tb=(-1)
arrCharFracT2tb=(0)

arrToUseIndexPol=(${arrIndexPolT2bw[@]})
arrToUseCharFrac=(${arrCharFracT2bw[@]})
if [ ${whichT2} -gt 0 ]
    then
    arrToUseIndexPol=(${arrIndexPolT2tt[@]})
    arrToUseCharFrac=(${arrCharFracT2tt[@]})
    if [ ${whichT2} -gt 2 ]
	then
	arrToUseIndexPol=(${arrIndexPolT2tb[@]})
	arrToUseCharFrac=(${arrCharFracT2tb[@]})
    fi
fi
#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################

for chanDilep in ${arrDilep[@]}
do
  for charFrac in "${arrToUseCharFrac[@]}"
    do
    for indexPol in "${arrToUseIndexPol[@]}"
      do
      T2String="T2SMS ${whichT2} indexPol ${indexPol} charFrac ${charFrac} -b -q -wDilep ${chanDilep} -tDilep ${typeDilep}"
      if [ ${whichMacro} -gt 3 ]
	  then
	  if [ ${doVerbosity} -gt 0 ]
	      then
	      echo "${strMacroCommand} ${T2String} doSC"
	  else
	      ${strMacroCommand} ${T2String} doSC
	  fi
      fi
      if [ ${numDims} -lt 2 ]
	  then
	  echo generating control stuff 
	  echo indexPol ${indexPol}
	  if [ ${whichT2} -lt 1 ]
	      then
	      echo T2bw charFrac ${charFrac}
	  elif [ ${whichT2} -lt 3 ]
	      then
	      echo T2tt
	  else
	      echo T2tb
	  fi	
	  if [ ${whichMacro} -lt 4 ]
	      then		  
	      if [ ${doVerbosity} -gt 0 ]
		  then
		  echo "${strMacroCommand} ${T2String} doControl controlCuts 80 0 -1"
	      else
		  ${strMacroCommand} ${T2String} doControl controlCuts 80 0 -1
	      fi
	      if [ ${whichMacro} -gt 0 ]
		  then
		  if [ ${doVerbosity} -gt 0 ]
		      then
		      echo "${strMacroCommand} ${T2String} doControl controlCuts 80 150 170"
		  else
		      ${strMacroCommand} ${T2String} doControl controlCuts 80 150 170
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
		    if [ ${whichT2} -lt 1 ]
			then
			echo T2bw charFrac ${charFrac}
		    elif [ ${whichT2} -lt 3 ]
			then
			echo T2tt
		    else
			echo T2tb
		    fi
		    echo indexPol ${indexPol}
		echo MT2llCut ${MT2llCut}
		echo MT2lblbCut ${MT2lblbCut}
		echo MT2bb_ZMETCut ${MT2bb_ZMETCut}
		if [ ${whichMacro} -lt 4 ]
		    then
		    if [ ${doVerbosity} -gt 0 ]
			then
			echo "${strMacroCommand} ${T2String} cuts ${MT2llCut} ${MT2lblbCut} ${MT2bb_ZMETCut}"
		    else
			${strMacroCommand} ${T2String} cuts ${MT2llCut} ${MT2lblbCut} ${MT2bb_ZMETCut}
		    fi
		fi
		  done
		done
	      done
	  fi
      fi
    done
  done
done