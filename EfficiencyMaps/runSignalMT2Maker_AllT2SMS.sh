#!/bin/sh
numDims=${1}
doSC=${2}
whichT2=${3}
doLocal=${4}
minStop=100
maxStop=800

maxLSPMassFix=400

#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################
arrIndexPolT2tt=(-1 0 10)
#arrIndexPolT2tt=(0 10)
arrCharFracT2tt=(0)

arrIndexPolT2bw=(0 2 4 6 8)
#arrIndexPolT2bw=(0 2 6 8)
arrCharFracT2bw=(25 50 75)

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

############################################
##Set up which dilepton types to run over ##
############################################

arrDilepFull=(-1 0 1 2)
arrDilepJustInd=(0 1 2)
#arrDilepJustInd=(-1)
############################################
##Set up which dilepton types to run over ##
############################################

minSel=8
maxSel=8

for (( wSel=${minSel}; wSel<=${maxSel}; wSel++ )); do
    arrDilepToUse=(${arrDilepFull[@]})
    if [ ${wSel} -gt 0 ]
	then
#	arrDilepToUse=(${arrDilepFull[@]})
	arrDilepToUse=(${arrDilepJustInd[@]})
	#only using the composite dilepton as an example for basic full selection
	#all others will be done with individual dileptons
    fi
    for chanDilep in ${arrDilepToUse[@]}
      do
      for charFrac in "${arrToUseCharFrac[@]}"
	do
	for indexPol in "${arrToUseIndexPol[@]}"
	  do
	  echo "./makeSignalMT2Distributions_FullShape.sh ${minStop} ${maxStop} ${maxLSPMassFix} ${whichT2} ${indexPol} ${charFrac} ${numDims} ${doSC} ${doLocal} ${chanDilep} ${wSel}"
	  ./makeSignalMT2Distributions_FullShape.sh ${minStop} ${maxStop} ${maxLSPMassFix} ${whichT2} ${indexPol} ${charFrac} ${numDims} ${doSC} ${doLocal} ${chanDilep} ${wSel}
	done
      done
    done
done
