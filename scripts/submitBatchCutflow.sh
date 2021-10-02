#!/bin/bash
submitDir=$PWD
branch=$1

#outputDir=extraPlots
outputDir=cutflow_veto
vetoOpt=1

if [ ! -e /eos/uscms/store/user/${USER}/boostedHiggsPlusMET/ ]
    then
    echo "I need /eos/uscms/store/user/${USER}/boostedHiggsPlusMET/"
    exit
fi

echo "outputDir ${outputDir}"    #Output directory name
source makeTar.sh                # package up code to transfer to worker nodes
cd $submitDir


#Run detailed comparison between samples (4b events only, only works for TChiHH950 and T5HH1000)
#cutflow arguments: region, mass, detailedCutflow?, region: 0=1DTChiHH, 1=1DT5HH

#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH950_allYears_4b" arguments="${outputDir} cutflowFromNtuples 0 950 1"
#condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH1000_allYears_4b" arguments="${outputDir} cutflowFromNtuples 1 1000 1"

#Run signal cutflow that goes in supplementary materials
for i in {1000,1600,2200}
  do
    condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_allYears_$i_1H" arguments="${outputDir} cutflowFromNtuples 1 $i 0"
  done
