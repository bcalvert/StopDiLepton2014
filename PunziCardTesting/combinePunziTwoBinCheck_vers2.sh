#! /bin/bash
arrName=("Low" "Med" "High" "SuperHigh")

minName=0
maxName=3

baseName="Results/limitOut_"

punziFile="PunziTwoBinCheck.txt"

punziComboFile="combinedPunzi_TwoBinCheck.txt"
punziComboFile2="combinedPunzi2_TwoBinCheck.txt"
outLimitFile="combinedLimits_TwoBinCheck.txt"
rm -rf ${outLimitFile}
rm -rf ${punziComboFile}
rm -rf ${punziComboFile2}


limitScriptPath="/data/users/bcalvert/StopDiLepton2014/GeneralScripts/LimitScripts/printLimsToFileFreq.sh"

currPermute=0

for (( iSigName1=${minName}; iSigName1<=${maxName}; iSigName1++ )); do
    nameSig1="${arrName[${iSigName1}]}Sig"
    for (( iSigName2=${minName}; iSigName2<=${maxName}; iSigName2++ )); do
	nameSig2="${arrName[${iSigName2}]}Sig"
	for (( iSigName3=${minName}; iSigName3<=${maxName}; iSigName3++ )); do
	    nameSig3="${arrName[${iSigName3}]}Sig"
	    for (( iBkgName1=${minName}; iBkgName1<=${maxName}; iBkgName1++ )); do
		nameBkg1="${arrName[${iBkgName1}]}Bkg"
		for (( iBkgName2=${minName}; iBkgName2<=${maxName}; iBkgName2++ )); do
		    nameBkg2="${arrName[${iBkgName2}]}Bkg"
		    for (( iBkgName3=${minName}; iBkgName3<=${maxName}; iBkgName3++ )); do
			let "currPermute = ${currPermute} + 1"
			echo currPermute ${currPermute}
			nameBkg3="${arrName[${iBkgName3}]}Bkg"
			fullName="${nameSig1}${nameBkg1}_Bin1${nameSig2}${nameBkg2}_Bin2${nameSig3}${nameBkg3}_Bin3"
			fullResultsName1="${baseName}${fullName}_TwoBin1.txt"
			fullResultsName2="${baseName}${fullName}_TwoBin2.txt"
			currPunziComboVers0=$(cat ${punziFile} | grep -v Super${fullName} | grep -A4 "\(.*\)iVers 0\(.*\)${fullName}" | grep currPunziComboVal | grep -v Super${fullName} | awk '{print $5}')
			currPunziComboVers1=$(cat ${punziFile} | grep -v Super${fullName} | grep -A4 "\(.*\)iVers 1\(.*\)${fullName}" | grep currPunziComboVal | grep -v Super${fullName} | awk '{print $5}')
			echo "${currPermute}:${currPunziComboVers0}:::${fullName}" >> ${punziComboFile}
			echo "${currPermute}:${currPunziComboVers1}:::${fullName}" >> ${punziComboFile}

			currPunziCombo2Vers0=$(cat ${punziFile} | grep -v Super${fullName} | grep -A4 "\(.*\)iVers 0\(.*\)${fullName}" | grep currPunziEllipseVal | grep -v Super${fullName} | awk '{print $5}')
			currPunziCombo2Vers1=$(cat ${punziFile} | grep -v Super${fullName} | grep -A4 "\(.*\)iVers 1\(.*\)${fullName}" | grep currPunziEllipseVal | grep -v Super${fullName} | awk '{print $5}')

			echo "${currPermute}:${currPunziCombo2Vers0}:::${fullName}" >> ${punziComboFile2}
			echo "${currPermute}:${currPunziCombo2Vers1}:::${fullName}" >> ${punziComboFile2}

			${limitScriptPath} ${fullResultsName1} ${outLimitFile} 0 ${currPermute} 0 ${fullName} -1 0
			${limitScriptPath} ${fullResultsName2} ${outLimitFile} 0 ${currPermute} 0 ${fullName} -1 0
		    done
		done
	    done
	done
    done
done