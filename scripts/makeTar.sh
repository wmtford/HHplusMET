#!/bin/bash

cd $CMSSW_BASE/../
tar -cf workingArea.tar CMSSW_10_2_13 --exclude='*.dag.*' --exclude='*tar' --exclude='CMSSW_10_2_13/*/src/*root' --exclude='*eps' --exclude='*png' --exclude='*pdf' --exclude='*stdout' --exclude='*stderr' --exclude='*condor' --exclude='CMSSW_10_2_13/src/HHplusMET/datacards/*' --exclude='CMSSW_10_2_13/src/HHplusMET/output/*' --exclude='CMSSW_10_2_13/src/HHplusMET/scripts/*root'
mv workingArea.tar CMSSW_10_2_13/src/HHplusMET/scripts/.
