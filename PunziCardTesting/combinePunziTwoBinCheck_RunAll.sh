#! /bin/bash

minName=0
maxName=3
for (( iSigName3=${minName}; iSigName3<=${maxName}; iSigName3++ )); do
    for (( iBkgName3=${minName}; iBkgName3<=${maxName}; iBkgName3++ )); do
	./combinePunziTwoBinCheck.sh ${iSigName3} ${iBkgName3}
    done
done