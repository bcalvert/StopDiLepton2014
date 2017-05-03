#!/bin/sh
whichMacro=${1}
whichT2=${2}
runAllPol=${3}
#################################################################################
##Set up which polarization indices and chargino mass fractions to iterate over##
#################################################################################
if [ ${runAllPol} -gt 0 ]
    then
    arrIndexPolT2bw=(0 2 4 6 8)
    arrIndexPolT2tt=(-1 0 10)
else
    arrIndexPolT2bw=(4)
    arrIndexPolT2tt=(-1)
fi

arrCharFracT2bw=(25 50 75)

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
#######################################################################################
##Done: Set up which polarization indices and chargino mass fractions to iterate over##
#######################################################################################

cutMT2ll=80
cutMT2lblb=0
cutMT2bb=-1

for charFrac in "${arrToUseCharFrac[@]}"
  do
  for indexPol in "${arrToUseIndexPol[@]}"
    do
    strSyst=""
    if (( ${whichT2} < 1 &&  ${indexPol} == 4 )) || (( ${whichT2} > 0 &&  ${indexPol} == -1 ))
	then
	strSyst="doSyst"
    else
	strSyst=""
    fi
    echo "./DrawSignalStuff -wpt ${whichMacro} T2SMS ${whichT2} charFrac ${charFrac} indexPol ${indexPol} cuts ${cutMT2ll} ${cutMT2lblb} ${cutMT2bb} ${strSyst} -b -q noDraw callLatex"
    ./DrawSignalStuff -wpt ${whichMacro} T2SMS ${whichT2} charFrac ${charFrac} indexPol ${indexPol} cuts ${cutMT2ll} ${cutMT2lblb} ${cutMT2bb} ${strSyst} -b -q noDraw callLatex
    if [ ${whichMacro} -gt 2 ]
	then
	echo "./DrawSignalStuff -wpt ${whichMacro} T2SMS ${whichT2} charFrac ${charFrac} indexPol ${indexPol} cuts 80 0 -1 doControl controlCuts 80 0 -1 doSC -b -q noDraw callLatex"
	./DrawSignalStuff -wpt ${whichMacro} T2SMS ${whichT2} charFrac ${charFrac} indexPol ${indexPol} cuts 80 0 -1 doControl controlCuts 80 0 -1 doSC -b -q noDraw callLatex
    fi
  done
done