#!/bin/sh

nameBackup=${1}

mkdir ${nameBackup}
for T2SMS in T2bw # T2tt T2bw
  do
  for addDir in `/bin/ls -F ${T2SMS}/* | grep ":" |sed "s/://"`
    do
    echo ${T2SMS}/${addDir}
    for addSDir in `/bin/ls -F ${addDir}/* | grep ":" |sed "s/://"`
      do
#      cp -r ./${T2tSMS}/${addDir}/${addSDir}/SignalYieldText/SignalRegion/ ${nameBackup}
      cp -r ./${addSDir}/SignalYieldText/SignalRegion/ ${nameBackup}
    done
  done
done
