#! /bin/bash

baseNamePunziResult="Punzi"

minSig=0
maxSig=4

minPunzi=0
maxPunzi=1

for (( iSig=${minSig}; iSig<=${maxSig}; iSig++ )); do
    for (( iPunzi=${minPunzi}; iPunzi<=${maxPunzi}; iPunzi++ )); do
	fullPunziName="${baseNamePunziResult}_wSig${iSig}_Punzi${iPunzi}.txt"
	rm -rf ${fullPunziName}
	./OneBinPunziTest -b -q wSig ${iSig} wPunzi ${iPunzi} >> ${fullPunziName}
	./OneBinPunziTest -b -q wSig ${iSig} wPunzi ${iPunzi} doRun
    done
done