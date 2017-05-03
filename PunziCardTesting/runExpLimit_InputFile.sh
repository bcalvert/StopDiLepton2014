#! /bin/bash
#source ../../GeneralShellScriptFunctions/LimitScriptFunctions/MT2Bounds.sh
#source ../../GeneralShellScriptFunctions/LimitScriptFunctions/LimitSettingFunctions.sh
#source /data/users/bcalvert/StopDiLepton2014/GeneralShellScriptFunctions/LimitScriptFunctions/LimitSettingFunctions.sh
source /data/users/bcalvert/StopDiLepton2014/GeneralShellScriptFunctions/LimitScriptFunctions/LimitCalculationFunctions.sh

inFile=${1}
outFile=${2}
doAsymptotic=${3}
isExpLim=${4}

commandString=$(LimitCalcStrings_Command ${doAsymptotic} ${isExpLim})

rm -rf ${outFile}


if [[ ${inFile} == *"Combo"* ]]
then
    runNoSigYield=0
else
    runNoSigYield=1
fi

#sed -i "s/\([0-9].*\),/\1/" ${inFile}
sigYield=$(cat ${inFile} | grep -A1 process |  grep rate | awk '{ print $2}')

echo sigYield ${sigYield}

thresh=0.0001
if [ ${runNoSigYield} -gt 0 ]
    then
    if (( $(bc <<< "${sigYield} < ${thresh}") == 1 ))
	then
	/data/users/bcalvert/StopDiLepton2014/GeneralScripts/LimitScripts/noSigYield.sh >> ${outFile}
    else
	${commandString} ${inFile} >> ${outFile}
#    ${commandString} ${inFile}
    fi
else
    ${commandString} ${inFile} >> ${outFile}
fi
echo ${commandString} >> ${outFile}





#combine -M Asymptotic -H ProfileLikelihood LimitCards/ExpectedLimits/T2bw/ChargFrac0p5/StopMass425/Chi025/expCard_Stop425_Chi025_CharginoFrac0p5_MT2ll0_MT2lblb0.txt
#combine -M HybridNew --frequentist --testStat LHC -H ProfileLikelihood LimitCards/ExpectedLimits/T2bw/ChargFrac0p5/StopMass425/Chi025/expCard_Stop425_Chi025_CharginoFrac0p5_MT2ll0_MT2lblb0.txt
