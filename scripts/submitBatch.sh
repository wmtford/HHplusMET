#!/bin/bash
submitDir=$PWD
branch=$1

outputDir=cutflow_veto

# systDir=
#testDir=boosted_noVeto_FinalSyst
#systDir=massFastSIMdown
#outputDir=$testDir/$systDir

vetoOpt=1

if [ ! -e /eos/uscms/store/user/${USER}/boostedHiggsPlusMET/ ]
    then
    echo "I need /eos/uscms/store/user/${USER}/boostedHiggsPlusMET/"
    exit
fi

echo "outputDir ${outputDir}"    #Output directory name
source makeTar.sh                # package up code to transfer to worker nodes
cd $submitDir

#cutflow
#i=500
#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH_2016_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2016 $vetoOpt $i"
#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH_2017_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2017 $vetoOpt $i"
#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH_2018_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2018 $vetoOpt $i"

#i=1000
#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH_2016_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2016 $vetoOpt $i"
#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH_2017_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2017 $vetoOpt $i"
#condor_submit batchExecute.jdl exec_name="cutflow_1DTChiHH_2018_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2018 $vetoOpt $i"

i=1600
condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_2016_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2016 $vetoOpt $i"
condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_2017_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2017 $vetoOpt $i"
condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_2018_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2018 $vetoOpt $i"

i=2200
condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_2016_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2016 $vetoOpt $i"
condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_2017_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2017 $vetoOpt $i"
condor_submit batchExecute.jdl exec_name="cutflow_1DT5HH_2018_$vetoOpt_$i" arguments="${outputDir} cutflow 1 MC2018 $vetoOpt $i"

#0-lepton MC
#condor_submit batchExecute.jdl exec_name="ALPHABET_signal_2016_$vetoOpt" arguments="${outputDir} ALPHABET 0 MC2016 $vetoOpt 0"
#condor_submit batchExecute.jdl exec_name="ALPHABET_signal_2017_$vetoOpt" arguments="${outputDir} ALPHABET 0 MC2017 $vetoOpt 0"
#condor_submit batchExecute.jdl exec_name="ALPHABET_signal_2018_$vetoOpt" arguments="${outputDir} ALPHABET 0 MC2018 $vetoOpt 0"

#1DTChiHH (set in skimSamples)
#for i in {150..1500..25}
#  do
#    condor_submit batchExecute.jdl exec_name="ALPHABET_1DTChiHH_2016_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 1 MC2016 $vetoOpt $i"
#    condor_submit batchExecute.jdl exec_name="ALPHABET_1DTChiHH_2017_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 1 MC2017 $vetoOpt $i"
#    condor_submit batchExecute.jdl exec_name="ALPHABET_1DTChiHH_2018_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 1 MC2018 $vetoOpt $i"
#  done

#1DT5HH FullSIM
#for i in {1000..2600..100}
#  do
#    condor_submit batchExecute.jdl exec_name="ALPHABET_1DT5HH_2016_$vetoOpt_$i" arguments="${outputDir} ALPHABET 1 MC2016 $vetoOpt $i"
#    condor_submit batchExecute.jdl exec_name="ALPHABET_1DT5HH_2017_$vetoOpt_$i" arguments="${outputDir} ALPHABET 1 MC2017 $vetoOpt $i"
#    condor_submit batchExecute.jdl exec_name="ALPHABET_1DT5HH_2018_$vetoOpt_$i" arguments="${outputDir} ALPHABET 1 MC2018 $vetoOpt $i"
#  done

# 2DTChiHH (set in skimSamples)
#for i in {150..800..25}
#  do
#    condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2016_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2016 $vetoOpt $i"
#    condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2017_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2017 $vetoOpt $i"
#    condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2018_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2018 $vetoOpt $i"
#  done

# for i in {825..1200..25}
#   do
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2016_$vetoOpt_$i" arguments="${outputDir} ALPHABET 2 MC2016 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2017_$vetoOpt_$i" arguments="${outputDir} ALPHABET 2 MC2017 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2018_$vetoOpt_$i" arguments="${outputDir} ALPHABET 2 MC2018 $vetoOpt $i"
#   done

# 2DT5HH (set in skimSamples)
# for i in {1000..2600..50}
#   do
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DT5HH_2016_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2016 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DT5HH_2017_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2017 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DT5HH_2018_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2018 $vetoOpt $i"
#   done

#Single-lepton
# condor_submit batchExecute.jdl exec_name="ALPHABET_singleMu_2016" arguments="${outputDir} ALPHABET 3 MC2016 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_singleMu_2017" arguments="${outputDir} ALPHABET 3 MC2017 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_singleMu_2018" arguments="${outputDir} ALPHABET 3 MC2018 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_singleEle_2016" arguments="${outputDir} ALPHABET 4 MC2016 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_singleEle_2017" arguments="${outputDir} ALPHABET 4 MC2017 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_singleEle_2018" arguments="${outputDir} ALPHABET 4 MC2018 0 0"

#Single-photon
# condor_submit batchExecute.jdl exec_name="ALPHABET_photon_2016" arguments="${outputDir} ALPHABET 5 MC2016 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_photon_2017" arguments="${outputDir} ALPHABET 5 MC2017 0 0"
# condor_submit batchExecute.jdl exec_name="ALPHABET_photon_2018" arguments="${outputDir} ALPHABET 5 MC2018 0 0"



#outputDir=boosted_veto_FastSIMSFs
#vetoOpt=1

#0-lepton MC
#condor_submit batchExecute.jdl exec_name="ALPHABET_signal_2016_$vetoOpt" arguments="${outputDir} ALPHABET 0 MC2016 $vetoOpt 0"
#condor_submit batchExecute.jdl exec_name="ALPHABET_signal_2017_$vetoOpt" arguments="${outputDir} ALPHABET 0 MC2017 $vetoOpt 0"
#condor_submit batchExecute.jdl exec_name="ALPHABET_signal_2018_$vetoOpt" arguments="${outputDir} ALPHABET 0 MC2018 $vetoOpt 0"

# #
# echo "outputDir ${outputDir}"    #Output directory name
# #
# # #1DTChiHH (set in skimSamples)
# for i in {150..1500..25}
#   do
#     condor_submit batchExecute.jdl exec_name="ALPHABET_1DTChiHH_2016_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 1 MC2016 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_1DTChiHH_2017_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 1 MC2017 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_1DTChiHH_2018_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 1 MC2018 $vetoOpt $i"
#   done
# #
# # # 2DTChiHH (set in skimSamples)
# for i in {150..800..25}
#   do
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2016_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2016 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2017_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2017 $vetoOpt $i"
#     condor_submit batchExecute.jdl exec_name="ALPHABET_2DTChiHH_2018_$vetoOpt_$i_$systDir" arguments="${outputDir} ALPHABET 2 MC2018 $vetoOpt $i"
#   done
#
