Repo for studying boosted Higgs tools with the RA2b trees.  This package relies on other packages,
depending on what you want to do

#General Stuff:

#### Setting up the code:
<pre>
cmsrel CMSSW_10_2_13
cd CMSSW_10_2_13/src/
cmsenv
git clone git@github.com:emacdonald16/HHplusMET.git
</pre>

NEEDS UPDATING

#### Overview:
There are (unfortunately) lots of steps.

If you want to make your own skims, follow Kevin's instructions: https://github.com/kpedro88/Analysis/tree/SUSY2018
You can also use the skims at /eos/uscms/store/user/emacdona/Skims/Run2ProductionV18 and /eos/uscms/store/user/kaulmer/Skims/ (for the TChiHH skims).

Once you have skims, you'll need to run ALPHABET to save the histograms in the analysis regions.

After, you can run the datacards.

Finally, you can make plots.


#### Batch submission:

Batch submissions are done through `submitBatch.sh` and `submitBatchCutflow.sh` in the `scripts` directory.  Code is tar'ed sent to worker nodes and outputs are transferd to EOS.  
You will need to have a directory in your eos area called boostedHiggsPlusMET.

First we tar
<pre>
bash makeTar.sh        
</pre>

`submitBatch.sh` runs the ALPHABET code and a cutflow (off of skims only). Modify the script to run what you want, and change the outgoing directory name.
`submitBatchCutflow.sh` runs cutflowFromNtuples for signal samples.

#### Region descriptions/definitions
`src/definitions.cc` contains the region definitions, baseline selections, useful functions, and the veto option that removes events that overlap with the resolved method.

Baseline selections are defined for the signal region and for each of the validation regions. Broadly, they require two AK8 jets with transverse momentum above 300 GeV and mass between 60 and 260 GeV, MET>300 GeV, HT>600 GeV, deltaPhi cuts, and event filters.  

There are two validation regions: single photon -- proxy for Z(nn), and a single lepton control region -- W/top enriched. These have slightly different selections than the signal samples.

#### Using ALPHABET

The ALPHABET code (`scripts/ALPHABET.cc`) creates histograms of different variables in different regions. These output histograms are used to both
make datacards and for plots of baseline and region variables.

To compile:

``` bash
g++ -o ALPHABET ALPHABET.cc `root-config --cflags --libs`
```

To run:
``` bash
./ALPHABET 0 MC2016 0 0
```

The first argument is the region (0=SM background), second is the year, third is a bool to remove events that overlap with the resolved method, and fourth is the NLSP mass for signal
samples (which doesn't matter for anything else).

You can also batch-submit the ALPHABET code.

#### Distributions from ALPHABET

Put in here stuff for ABCD.C


#### Cut flow table

Verify the below is true!

To produce a cut flow table, you run `cutFlow.cc`.  This will print a latex-formatted table to the standard output.  To run:

```bash
g++ -o cutflow cutflow.cc `root-config --cflags --libs`
./cutflow 0 MC2016 0 0
```
with the same argument definition as ALPHABET. Similarly, you can also batch-submit.

If you want to run the cutflow from the ntuples (so without skim-level cuts):
```bash
g++ -o cutflowFromNtuples cutflowFromNtuples.cc `root-config --cflags --libs`
./cutflowFromNtuples 0 600 0
```
THIS ONLY WORKS ON 1D SIGNAL SAMPLES

The first argument is the region (0=TChiHH, 1=T5HH), second is the NLSP mass, third is a bool for a more detailed cutflow (breaking down filters etc).

#### Making datacards

Put stuff here



#### Making final brazil flag plots

Words here



#### Plots and tables for auxilary materials

More words here
