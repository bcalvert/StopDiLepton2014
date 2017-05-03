#!/bin/sh
for file in `/bin/ls Hist*T2tb*root`
  do
  echo ${file}
  newName=$(echo ${file} | sed "s/Histo/Histos/")
  echo ${newName}
  cp ${file} ${newName}
done