#! /bin/bash
arrNamePunzi=("SigmaA5_SigmaB2" "SigmaA2_SigmaB2")
arrNameSig=("" "TwoThirds" "Sqrt" "CubeRoot" "TwoFifths")

minSig=0
maxSig=4

minPunzi=0
maxPunzi=1

baseNamePunziParamFile="Punzi"
baseNameLimitFile="Results/limitOut_"
baseNameOutFile="combinedLimits_Punzi";
baseNameOutPunzi="combinedParams_Punzi";

limitScriptPath="/data/users/bcalvert/StopDiLepton2014/GeneralScripts/LimitScripts/printLimsToFileFreq.sh"

outFile="limits_PunziOneBinCheck.txt"
rm -rf ${outFile}


minCheck=0
#maxCheck=100
maxCheck=250

for (( iSig=${minSig}; iSig<=${maxSig}; iSig++ )); do
    for (( iPunzi=${minPunzi}; iPunzi<=${maxPunzi}; iPunzi++ )); do
	addName="${arrNamePunzi[${iPunzi}]}_${arrNameSig[${iSig}]}"
	fullNameOutFile="${baseNameOutFile}${addName}_OneBin.txt"
	fullNameOutPunzi="${baseNameOutPunzi}${addName}_OneBin.txt"
	rm -rf ${fullNameOutFile}
	rm -rf ${fullNameOutPunzi}

	fullNamePunziParamIn="${baseNamePunziParamFile}_wSig${iSig}_Punzi${iPunzi}.txt"
#	echo fullNamePunziParamIn ${fullNamePunziParamIn}
	cat ${fullNamePunziParamIn} | grep ":" | grep -v ROOT | grep -v root >> ${fullNameOutPunzi}
	for (( iCheck=${minCheck}; iCheck<=${maxCheck}; iCheck++ )); do
	    echo iSig ${iSig}
	    echo iPunzi ${iPunzi}
	    echo iCheck ${iCheck}
	    currLimitFileName="${baseNameLimitFile}${addName}Check${iCheck}_OneBin.txt"
#	    echo fullNameOutFile ${fullNameOutFile}
	  #currLimitFileName="${baseNameFile}${iCheck}${endNameFile}"
	    ${limitScriptPath} ${currLimitFileName} ${fullNameOutFile} 0 ${iCheck} 0 -1 -1 0
	done
    done
done