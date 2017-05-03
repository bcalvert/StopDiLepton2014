#!/bin/sh
T2SMS=${1}
charFrac=${2}
indexPol=${3}
selFull=${4}
chan=${5}

arrSelFullNames=("BaseFullSel" "BaseFullSel_2BJets" "BaseFullSel_METSig1D" "BaseFullSel_METSigTrue")
nameFullSel=${arrSelFullNames[${selFull}]}
arrNameT2=("T2bw" "T2tt" "" "T2tb")
nameT2=${arrNameT2[${T2SMS}]}

nameSMS="${nameT2}_charFrac${charFrac}_indexPol${indexPol}_${nameFullSel}"

arrName=("_MuMu" "_EE" "_EMu" "_Comp")
chanName=${arrName[${chan}]}



iDM=0
nameFile1="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${iDM}_selFull${selFull}${chanName}List.txt"
echo nameFile1 ${nameFile1}
for iDM in 1 2 3
  do
  nameFile2="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${iDM}_selFull${selFull}${chanName}List.txt"
  firstDiff=$(diff ${nameFile1} ${nameFile2} | grep "<" | wc -l)
  echo "the diff between iDM 0 and iDM ${iDM} for ${nameSMS} is ${firstDiff}"
done 

iDM=1
nameFile1="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${iDM}_selFull${selFull}${chanName}List.txt"
for iDM in 2 3
  do
  nameFile2="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${iDM}_selFull${selFull}${chanName}List.txt"
  firstDiff=$(diff ${nameFile1} ${nameFile2} | grep "<" | wc -l)
  echo "the diff between iDM 1 and iDM ${iDM} for ${nameSMS} is ${firstDiff}"
done


iDM=2
nameFile1="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${iDM}_selFull${selFull}${chanName}List.txt"
for iDM in 3
  do
  nameFile2="MapInfo_T2SMS${T2SMS}_charFrac${charFrac}_indexPol${indexPol}_dM${iDM}_selFull${selFull}${chanName}List.txt"
  firstDiff=$(diff ${nameFile1} ${nameFile2} | grep "<" | wc -l)
  echo "the diff between iDM 2 and iDM ${iDM} for ${nameSMS} is ${firstDiff}"
done