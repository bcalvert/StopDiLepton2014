#!/bin/sh
minT2=0
maxT2=2
minMacro=1
maxMacro=3

for (( whichT2=${minT2}; whichT2<=${maxT2}; whichT2++ )); do
  minMacro=1
  if [ ${whichT2} -lt 2 ]
      then
      maxMacro=3
  else
      maxMacro=1
  fi
  for (( whichMacro=${minMacro}; whichMacro<=${maxMacro}; whichMacro++ )); do
      echo "./drawSignalEffMaker.sh ${whichMacro} ${whichT2} 0"
      ./drawSignalEffMaker.sh ${whichMacro} ${whichT2} 0
  done
done