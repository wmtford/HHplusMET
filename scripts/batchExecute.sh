#!/bin/sh

echo "Starting job on "`date` # to display the start date
echo "Running on "`uname -a` # to display the machine where the job is running
echo "System release "`cat /etc/redhat-release` # and the system release
# Can also do 'lsb_release -d' or 'lsb_release -r'
echo "CMSSW on Condor"

# check arguments
export USER=$(whoami)
export CMSSWVER="CMSSW_10_2_13"
export CMSSWLOC="slc7_amd64_gcc700"
export CMSSWXRD=""
#export CMSSWXRD="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV16/"

echo ""
echo "parameter set:"
echo "CMSSWVER: $CMSSWVER"
if [ -n "$CMSSWLOC" ]; then
    echo "CMSSWLOC: $CMSSWLOC"
fi
echo ""


# to get condor-chirp from CMSSW
export PATH="/usr/libexec/condor:$PATH"
# environment setup
source /cvmfs/cms.cern.ch/cmsset_default.sh

output_dir=$1
executable=$2
args1=$3
args2=$4
args3=$5
args4=$6
echo "output_dir ${output_dir}"
echo "executable ${executable}"
echo "args ${args}"

#mv ../../workingArea.tar .

tar -xf workingArea.tar
cd CMSSW_10_2_13/src/
eval `scramv1 runtime -sh`
echo "step 2"
cd HHplusMET
export PATH=$PWD/src/:$PATH
export PATH=$PWD/scripts/:$PATH
echo "PWD ${PWD}"
echo "PATH ${PATH}"
cd scripts

echo ""
eval 'echo g++ -o $executable $executable.cc `root-config --cflags --libs` '

eval `echo ./$executable $args1 $args2 $args3 $args4`

echo "PWD ${PWD}"
ls *root

for file in $(ls *root)
do
    xrdcp -f $file root://cmseos.fnal.gov//store/user/${USER}/boostedHiggsPlusMET/$output_dir/$file
done

