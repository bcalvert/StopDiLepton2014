#!/bin/sh
doLocal=${1}
whichMacro=${2}
whichT2=${3} #0 for T2bw, 1 for T2tt, 2 for T2tt Tight Bin, 3 for T2tb
numDims=${4} #numDims -- should be 1, 2, or 3
whichFullSel=${5} #which full selection to apply -- should be 0, 1, 2, or 3
doControl=${6}
doVerbosity=${7}

if [ ${numDims} -gt 0 ]
    then
    typeDilep=1
else
    typeDilep=0    
fi

source ../GeneralShellScriptFunctions/LimitScriptFunctions/MT2Bounds.sh
source ../GeneralShellScriptFunctions/BaseWorkDir.sh

if [ ${doLocal} -gt 0 ]
    then
    condorScript=$(BaseCondorScriptName 1)
else
    condorScript=$(BaseCondorScriptName 2)
fi

echo "condorScript " ${condorScript}
echo "whichT2 " ${whichT2}
#############################
##Set up which macro to run##
#############################
arrMacroToUse=("MakeLeptFiltSF" "MakeCoarseEffMap" "MakeSmoothedEffMap" "MakeSignalYield" "MakeSignalYieldText")
if [ ${whichT2} -eq 2 ]
    then
    arrMacroToUse[1]="MakeCoarseEffMap_TightBin"
fi
strMacroCommand="./${arrMacroToUse[${whichMacro}]}"
if [ ${whichMacro} -gt -1 ]
    then
    strMacroCommand="${condorScript} ${strMacroCommand}"
fi
#############################
##Set up which macro to run##
#############################

################################
##Set up which MT2 cuts to use##
################################
MT2llCut=80
MT2lblbCut=170
MT2bb_ZMETCut=170
################################
##Set up which MT2 cuts to use##
################################

############################################
##Set up which dilepton types to run over ##
############################################

arrDilepFull=(-1 0 1 2)
arrDilepJustInd=(0 1 2)
arrDilepJustInd=(-1)

#arrDilepToUse=(${arrDilepFull[@]})
if [ ${whichFullSel} -gt 0 ]
    then
#    arrDilepToUse=(${arrDilepFull[@]})
    arrDilepToUse=(${arrDilepJustInd[@]})
    #only using the composite dilepton as an example for basic full selection
    #all others will be done with individual dileptons
else
    arrDilepToUse=(${arrDilepJustInd[@]})
fi
############################################
##Set up which dilepton types to run over ##
############################################


#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################

arrIndexPolT2bw=(0 2 4 6 8)
#arrIndexPolT2bw=(4 6 8)
#arrCharFracT2bw=(50 75)
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

for chanDilep in ${arrDilepToUse[@]}
do
  for charFrac in "${arrToUseCharFrac[@]}"
    do
    for indexPol in "${arrToUseIndexPol[@]}"
      do
      
      numRun=$[`condor_q | grep bcalvert | grep MakeCo | grep " R" | wc -l`]
      thresh=200
      if [ ${whichMacro} -eq 1 ]
	  then
	  thresh=60
      fi
      while [ $numRun -gt $thresh ]
	do
	sleep 30
	numRun=$[`condor_q | grep bcalvert | grep MakeCo | grep " R" | wc -l`]
	echo $numRun
      done


      echo chanDilep ${chanDilep}
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



      T2String="T2SMS ${whichT2} indexPol ${indexPol} charFrac ${charFrac} -b -q -wDilep ${chanDilep} -tDilep ${typeDilep} -tSel ${whichFullSel} wInTy 1"
      if [ ${whichMacro} -gt 3 ]
	  then
	  if [ ${doVerbosity} -gt 0 ]
	      then
	      echo "${strMacroCommand} ${T2String} doSC"
	  else
	      ${strMacroCommand} ${T2String} doSC
	  fi
      fi
      if [ ${doControl} -gt 0 ]
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
		      echo "${strMacroCommand} ${T2String} doControl controlCuts 80 170 170"
		  else
		      ${strMacroCommand} ${T2String} doControl controlCuts 80 170 170
		  fi
	      fi
	  fi      
      else
	  if [ ${whichMacro} -gt 0 ]
	      then

	      if [ ${whichT2} -lt 1 ]
		  then
		  echo T2bw charFrac ${charFrac}
	      elif [ ${whichT2} -lt 3 ]
		  then
		  echo T2tt
	      else
		  echo T2tb
	      fi
	      echo chanDilep ${chanDilep}
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
	  fi
      fi
    done
  done
done