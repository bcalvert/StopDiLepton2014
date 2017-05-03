#!/bin/sh

arrName=("_MuMu" "_EE" "_EMu" "_Comp")

for file in `/bin/ls MapInfo*.txt`
  do
  for chanName in ${arrName[@]}
    do
    #echo ${file}
    newFile=$(echo ${file} | sed "s/.txt/${chanName}List.txt/")
    #echo newFile ${newFile}
    rm -rf ${newFile}
    cat ${file} | grep -A64 "inputHist name h_OptMap${chanName}" | grep currGlobal >> ${newFile}
    done
done