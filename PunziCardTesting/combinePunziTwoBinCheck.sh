#! /bin/bash

whichSigBin3=${1}
whichBkgBin3=${2}

arrName=("Low" "Med" "High" "SuperHigh")

minName=0
maxName=3

baseName="Results/limitOut_"

punziFile="PunziTwoBinCheck_4.txt"

punziComboFile="combinedPunzi_TwoBinCheck_Bin3Sig${whichSigBin3}Bkg${whichBkgBin3}.txt"
rm -rf ${punziComboFile}

punziComboFile2="combinedPunzi2_TwoBinCheck_Bin3Sig${whichSigBin3}Bkg${whichBkgBin3}.txt"
rm -rf ${punziComboFile2}
punziComboFile3="combinedPunzi3_TwoBinCheck_Bin3Sig${whichSigBin3}Bkg${whichBkgBin3}.txt"
rm -rf ${punziComboFile3}
punziComboFile4="combinedPunzi4_TwoBinCheck_Bin3Sig${whichSigBin3}Bkg${whichBkgBin3}.txt"
rm -rf ${punziComboFile4}
outLimitFile="combinedLimits_TwoBinCheck_Bin3Sig${whichSigBin3}Bkg${whichBkgBin3}.txt"
rm -rf ${outLimitFile}




limitScriptPath="/data/users/bcalvert/StopDiLepton2014/GeneralScripts/LimitScripts/printLimsToFileFreq.sh"

currPermute=0

for (( iSigName1=${minName}; iSigName1<=${maxName}; iSigName1++ )); do
    nameSig1="${arrName[${iSigName1}]}Sig"
    for (( iSigName2=${minName}; iSigName2<=${maxName}; iSigName2++ )); do
	nameSig2="${arrName[${iSigName2}]}Sig"
	for (( iSigName3=${whichSigBin3}; iSigName3<=${whichSigBin3}; iSigName3++ )); do
	    nameSig3="${arrName[${iSigName3}]}Sig"
	    for (( iBkgName1=${minName}; iBkgName1<=${maxName}; iBkgName1++ )); do
		nameBkg1="${arrName[${iBkgName1}]}Bkg"
		for (( iBkgName2=${minName}; iBkgName2<=${maxName}; iBkgName2++ )); do
		    nameBkg2="${arrName[${iBkgName2}]}Bkg"
		    for (( iBkgName3=${whichBkgBin3}; iBkgName3<=${whichBkgBin3}; iBkgName3++ )); do
			let "currPermute = ${currPermute} + 1"
			echo currPermute ${currPermute}
			nameBkg3="${arrName[${iBkgName3}]}Bkg"
			fullName="${nameSig1}${nameBkg1}_Bin1${nameSig2}${nameBkg2}_Bin2${nameSig3}${nameBkg3}_Bin3"
			fullResultsName1="${baseName}${fullName}_TwoBin1.txt"
			fullResultsName2="${baseName}${fullName}_TwoBin2.txt"
			currPunziComboVers0=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 0\(.*\)${fullName}" | grep currPunziComboVal | grep -v Super${fullName} | awk '{print $5}')
			currPunziComboVers1=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 1\(.*\)${fullName}" | grep currPunziComboVal | grep -v Super${fullName} | awk '{print $5}')
			echo "${currPermute}:${currPunziComboVers0}:::${fullName}" >> ${punziComboFile}
			echo "${currPermute}:${currPunziComboVers1}:::${fullName}" >> ${punziComboFile}

			currPunziCombo2Vers0=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 0\(.*\)${fullName}" | grep currPunziNonSquareComboVal | grep -v Super${fullName} | awk '{print $5}')
			currPunziCombo2Vers1=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 1\(.*\)${fullName}" | grep currPunziNonSquareComboVal | grep -v Super${fullName} | awk '{print $5}')

			echo "${currPermute}:${currPunziCombo2Vers0}:::${fullName}" >> ${punziComboFile2}
			echo "${currPermute}:${currPunziCombo2Vers1}:::${fullName}" >> ${punziComboFile2}


			currPunziCombo3Vers0=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 0\(.*\)${fullName}" | grep currPunziEllipseVal | grep -v Super${fullName} | awk '{print $5}')
			currPunziCombo3Vers1=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 1\(.*\)${fullName}" | grep currPunziEllipseVal | grep -v Super${fullName} | awk '{print $5}')

			echo "${currPermute}:${currPunziCombo3Vers0}:::${fullName}" >> ${punziComboFile3}
			echo "${currPermute}:${currPunziCombo3Vers1}:::${fullName}" >> ${punziComboFile3}

			currPunziCombo4Vers0=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 0\(.*\)${fullName}" | grep currPunziVers2EllipseVal | grep -v Super${fullName} | awk '{print $5}')
			currPunziCombo4Vers1=$(cat ${punziFile} | grep -v Super${fullName} | grep -A9 "\(.*\)iVers 1\(.*\)${fullName}" | grep currPunziVers2EllipseVal | grep -v Super${fullName} | awk '{print $5}')

			echo "${currPermute}:${currPunziCombo4Vers0}:::${fullName}" >> ${punziComboFile4}
			echo "${currPermute}:${currPunziCombo4Vers1}:::${fullName}" >> ${punziComboFile4}

			${limitScriptPath} ${fullResultsName1} ${outLimitFile} 0 ${currPermute} 0 ${fullName} -1 0
			${limitScriptPath} ${fullResultsName2} ${outLimitFile} 0 ${currPermute} 0 ${fullName} -1 0
		    done
		done
	    done
	done
    done
done