#!/bin/sh
scaleGen="2.0"
sigma="2.0"
baseCommand="./testGenerators scaleGen ${scaleGen} sigma ${sigma} -b -q"
for whichDL in 0 1 2 3
  do
  echo "${baseCommand} whichChan ${whichDL} secondTTbarGen 1 firstTTbarGen 2"
  #${baseCommand} whichChan ${whichDL} secondTTbarGen 1 firstTTbarGen 2
  for firstGen in 0 1 3
    do
    echo "${baseCommand} whichChan ${whichDL} secondTTbarGen 2 firstTTbarGen ${firstGen}"
    #${baseCommand} whichChan ${whichDL} secondTTbarGen 2 firstTTbarGen ${firstGen}
  done
done
