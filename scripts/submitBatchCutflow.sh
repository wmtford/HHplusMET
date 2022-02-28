#!/bin/bash
submitDir=$PWD
branch=$1

outputDir=cutflow_noVeto

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

#NLSP mass doesn't matter for bkg, set to 0
i=0
condor_submit batchExecute.jdl exec_name="cutflow_ZJets_MC2016" arguments="${outputDir} cutflowFromNtuples 0 ZJets MC2016 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_ZJets_MC2017" arguments="${outputDir} cutflowFromNtuples 0 ZJets MC2017 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_ZJets_MC2018" arguments="${outputDir} cutflowFromNtuples 0 ZJets MC2018 $i 0"

condor_submit batchExecute.jdl exec_name="cutflow_WJets_MC2016" arguments="${outputDir} cutflowFromNtuples 0 WJets MC2016 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_WJets_MC2017" arguments="${outputDir} cutflowFromNtuples 0 WJets MC2017 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_WJets_MC2018" arguments="${outputDir} cutflowFromNtuples 0 WJets MC2018 $i 0"

condor_submit batchExecute.jdl exec_name="cutflow_TT_MC2016" arguments="${outputDir} cutflowFromNtuples 0 TT MC2016 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_TT_MC2017" arguments="${outputDir} cutflowFromNtuples 0 TT MC2017 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_TT_MC2018" arguments="${outputDir} cutflowFromNtuples 0 TT MC2018 $i 0"

condor_submit batchExecute.jdl exec_name="cutflow_SnglT_MC2016" arguments="${outputDir} cutflowFromNtuples 0 SnglT MC2016 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_SnglT_MC2017" arguments="${outputDir} cutflowFromNtuples 0 SnglT MC2017 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_SnglT_MC2018" arguments="${outputDir} cutflowFromNtuples 0 SnglT MC2018 $i 0"

condor_submit batchExecute.jdl exec_name="cutflow_QCD_MC2016" arguments="${outputDir} cutflowFromNtuples 0 QCD MC2016 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_QCD_MC2017" arguments="${outputDir} cutflowFromNtuples 0 QCD MC2017 $i 0"
condor_submit batchExecute.jdl exec_name="cutflow_QCD_MC2018" arguments="${outputDir} cutflowFromNtuples 0 QCD MC2018 $i 0"

#Run signal cutflow that goes in supplementary materials, year doesn't matter for signal
for i in {1000,1600,2200}
  do
    condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_allYears_$i" arguments="${outputDir} cutflowFromNtuples 1 T5HH MC2016 $i 0"
  done
