#!/bin/sh
wSel=${1}

minMacro=1
maxMacro=3

for (( whichMacro=${minMacro}; whichMacro<=${maxMacro}; whichMacro++ )); do
    echo "./DrawSignalStuff -wpt ${whichMacro} T2SMS 1 charFrac indexPol -1 cuts 80 170 170 wInTy 1 doSyst -tDilep 1 numDims 3 -tSel ${wSel} -b -q"
    ./DrawSignalStuff -wpt ${whichMacro} T2SMS 1 charFrac indexPol -1 cuts 80 170 170 wInTy 1 doSyst -tDilep 1 numDims 3 -tSel ${wSel} -b -q

    for charFrac in 25 50 75
      do
      echo "./DrawSignalStuff -wpt ${whichMacro} T2SMS 0 charFrac ${charFrac} indexPol 4 cuts 80 170 170 wInTy 1 doSyst -tDilep 1 numDims 3 -tSel ${wSel} -b -q"
      ./DrawSignalStuff -wpt ${whichMacro} T2SMS 0 charFrac ${charFrac} indexPol 4 cuts 80 170 170 wInTy 1 doSyst -tDilep 1 numDims 3 -tSel ${wSel} -b -q
    done

    if [ ${whichMacro} -gt 2 ]
	then
	echo "./DrawSignalStuff -wpt ${whichMacro} T2SMS 1 charFrac indexPol -1 cuts 80 170 170 doControl controlCuts 80 170 170 doSC -tDilep 1 numDims 3 -tSel ${wSel} -b -q"
	./DrawSignalStuff -wpt ${whichMacro} T2SMS 1 charFrac indexPol -1 cuts 80 170 170 doControl controlCuts 80 170 170 doSC -tDilep 1 numDims 3 -tSel ${wSel} -b -q 
	for charFrac in 25 50 75
	  do
	  echo "./DrawSignalStuff -wpt ${whichMacro} T2SMS 0 charFrac ${charFrac} indexPol 4 cuts 80 170 170 doControl controlCuts 80 170 170 doSC -tDilep 1 numDims 3 -tSel ${wSel} -b -q"
	  ./DrawSignalStuff -wpt ${whichMacro} T2SMS 0 charFrac ${charFrac} indexPol 4 cuts 80 170 170 doControl controlCuts 80 170 170 doSC -tDilep 1 numDims 3 -tSel ${wSel} -b -q 
	done

    fi
done