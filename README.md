Repo for studying boosted Higgs tools with the RA2b trees.  This package relies on other packages,
depending on what you want to do

## General Stuff:

### Setting up the code:
<pre>
cmsrel CMSSW_10_2_13
cd CMSSW_10_2_13/src/
cmsenv
git clone git@github.com:emacdonald16/HHplusMET.git
</pre>

### Other package dependencies you might want
- https://github.com/cms-analysis/CombineHarvester
- https://github.com/kpedro88/Analysis/tree/SUSY2018
- https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/
- https://github.com/TreeMaker/TreeMaker

## Overview:
There are (unfortunately) lots of steps.
1. First, you'll need skims
  - If you want to make your own skims, follow Kevin's instructions: https://github.com/kpedro88/Analysis/tree/SUSY2018
  - You can also use the skims at /eos/uscms/store/user/emacdona/Skims/Run2ProductionV18 and /eos/uscms/store/user/kaulmer/Skims/ (for the TChiHH skims).
2. Once you have skims, you'll need to run ALPHABET to save histograms of the analysis regions
3. After, you can run the datacards
4. Finally, you can make plots

To make all the paper plots from pre-existing datacards in Emily's area, you can run:
<pre>
cd scripts/
bash runPaperPlots.sh        
</pre>
This creates all the paper plots (and a few more) and writes them to the *output* directory. If you need plots that are in the supplementary materials,
see the bottom of this README. If you need plots that are in the AN, I hope you have loads of time and patience since I did NOT
set that up in an intuitive way (they'll be in `ABCD.C`).

## Batch submission:

Batch submissions are done through `submitBatch.sh` and `submitBatchCutflow.sh` in the *scripts* directory.  Code is tar'ed, sent to the worker nodes, and final output
root files are transferd to EOS. You will need to have a directory in your EOS area called boostedHiggsPlusMET, or you'll get a cryptic warning.

First we compile whatever code we're batch submitting, since we don't trust the script to do it itself:
``` bash
g++ -o ALPHABET ALPHABET.cc `root-config --cflags --libs`
```
Then we tar:
<pre>
bash makeTar.sh        
</pre>
Then we make sure we have a valid grid certificate:
``` bash
voms-proxy-init -voms cms -valid 192:00
```
And THEN we submit whatever batch code we want:
```
./submitBatch.sh
```

- `submitBatch.sh` runs the ALPHABET code or a cutflow (off of skims only). Modify the script to run what you want, and change the outgoing directory name.
- `submitBatchCutflow.sh` runs cutflowFromNtuples.cc for signal samples only, from the ntuples.

The processed files are moved to /eos/uscms/store/user/${USER}/boostedHiggsPlusMET/, so make sure you have a *boostedHiggsPlusMET* directory in your eos area.

## Region descriptions/definitions
`src/definitions.cc` contains the region definitions, baseline selections, useful functions, and the veto option that removes events that overlap with the resolved method.

Baseline selections are defined for the signal region and for each of the validation regions. Broadly, they require two AK8 jets with transverse momentum above 300 GeV and mass between 60 and 260 GeV, MET>300 GeV, HT>600 GeV, deltaPhi cuts, and event filters. There are two validation regions: single photon (proxy for Z->nunu), and a single lepton control region (W/top enriched). These have slightly different selections than the signal samples.

I tried to clean this code up, but it's not the best. So, sorry if you need to change something.

## Using ALPHABET
The ALPHABET code (`scripts/ALPHABET.cc`) creates histograms of different variables in different signal and control regions. These output histograms are used to both
make datacards and for plots of baseline and region variables.

To compile:
``` bash
cd scripts/
g++ -o ALPHABET ALPHABET.cc `root-config --cflags --libs`
```

To run:
``` bash
./ALPHABET arg1 arg2 arg3 arg4
```

The arguments are as follows:
- arg1: This is the region definition (0 = SM background, 1 = 1D signal, 2 = 2D signal, 3 = single muon, 4 = single electron, 5 = single photon)
- arg2: Year, setup as "MC2016", "MC2017", or "MC2018"
- arg3: Resolved veto, 0 means do not remove any events, 1 means remove events that overlap with the veto
- arg4: NLSP mass. This is used for the signal models, so I just put a 0 for anything else

Example running "./ALPHABET 1 MC2016 1 600" - runs the 1D TChiHH(600,1) mass point for 2016, removing the events that overlap with the resolved.

If you wish to run the 1D T5HH FullSIM samples, you will need to change the bool in `skimSamples.cc` (https://github.com/emacdonald16/HHplusMET/blob/v2021/src/skimSamples.cc#L81-L82) and the bool in `ALPHABET.cc` (https://github.com/emacdonald16/HHplusMET/blob/v2021/scripts/ALPHABET.cc#L27).



You can also batch-submit the ALPHABET code, see above.

Included in `ALPHABET` are a variety of bools and functions to run the signal systematics. These are best run using batch submission, changing the
necessary bools and function arguments for every individual systematic and variation (yes there are a lot). The batch code is setup to easily change the
name of the output directory (example, "JECUp") to keep track of all of these.

If you re-run the signal samples, you will need to hadd them to create a single file for the datacards and the plotting scripts to find.
I have been naming these `ALPHABET_1DSignal.root`, but you can name it what you like as long as you change the input for the datacards and/or
`ABCD.C` which is the plotting script for distributions.

For the FastSIM samples, you will also need to run ALPHABET using genMET instead of recoMET (the MET is averaged for regular running).
The bool is set in `src/definitions.cc`: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/src/definitions.cc#L412


#### If you need to change/add signal samples
The samples that are run are included in `src/skimSamples.cc`. There are bools that define which samples are run, depending on the region
that is passed from `ALPHABET.cc`. The 1D samples are run using region==1, the 2D samples with region==2. The bools are set further down: https://github.com/emacdonald16/HHplusMET/blob/v2021/src/skimSamples.cc#L81-L82

If you wish to add/change signal samples, this is where you do it. It is not conveniently set up. The directory of the skims is defined at the top,
and you can change/add signal samples closer to the bottom: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/src/skimSamples.cc#L476-L485

`ALPHABET.cc` runs the signal samples per NLSP mass and year.

#### To run boosted event counts (for WX overlap studies)
If you need to run boosted event counts for overlap studies, there is a bool at the top of `ALPHABET.cc` that does just that "saveBoostedEvt".
The text files are created for each analysis region, and for the same arguments you pass `ALPHABET.cc` (region, year, resVeto, NLSP mass).
- If you wish to run on data, set the bool "runData" at the top of the ALPHABET code to true, and run with the argument region=0. This
is currently setup so that if runData=true, the MC will not run.
- I also did not set this up to include multiple signal mass points in the same text file!
- These text files are also not transfered to an eos area, so I would suggest running locally.
- The files are output into *src/evtCount/boost/*

## Plotting distributions from ALPHABET
`ABCD.C` contains loads of functions that make different plots. It is currently setup to only run the plots that are put into the published paper, but you can easily change it
to run the plots that go into the supplementary materials and the AN.
IT DOES NOT RUN THE LIMIT PLOTS.
To run:
``` bash
root -l -b ABCD.C
```
This will create an output root file with all the plots saved into it, and will save the paper plots as PDFs.

## Cut flow table
To produce a cut flow table, you run `cutFlow.cc`.  This will print a latex-formatted table to the standard output. It will also make a root file with the cut flow histograms saved.

To run:
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
- THIS ONLY WORKS ON 1D SIGNAL SAMPLES
- The first argument is the region (0=TChiHH, 1=T5HH), second is the NLSP mass, third is a bool for a more detailed cutflow (breaking down filters etc).

## Making datacards
You will need the combine package in your CMSSW_10_2_13/src/ area. Follow the instructions here: https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/

Before making the datacards, you'll need to run `ALPHABET.cc` to create the necessary histograms, see above.
You will also need to run all of the necessary signal systematic variations, or switch the bool at the top "runSysts" to False.

The main codes for making the datacards are `datacards/QuickDataCardsABCDNorm_Higgsino.py` and `datacards/QuickDataCardsABCDNorm_Gluino.py`. The first runs rather the 1D or 2D TChiHH models
(you need to change the bool "run2D").
There are also configurable options to run with or without the overlap events ("runVeto"), whether data or MC should be used for the backgrounds
("useData") and if combine should be run for the resolved method alone ("runResAlone"). These bools are toward the top of `datacards/QuickDataCardsABCDNorm_Higgsino.py`.
Also at the top are the paths to wherever the ALPHABET files are stored. If you make new/different signal samples, you will need to change
these paths: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/datacards/QuickDataCardsABCDNorm_Higgsino.py#L89-L101


These codes create the datacards from the output of ALPHABET, including all the signal systematics that would need to be run. After creating the datacards, they also run combine to
produce the limit root files (at the very bottom of the code).

These can be run for a single mass point from the *datacards* directory:
```bash
python -b QuickDataCardsABCDNorm_Higgsino.py 600 1
```
or you can run them all at once from the *scripts* directory:
```bash
python -b runABCDNormHiggsino.py
python -b runABCDNormGluino.py
```
The first has a configurable option to run the 1D or 2D scan. If you change to run over the 2D TChiHH, you will also need to change the bool
in `datacards/QuickDataCardsABCDNorm_Higgsino.py`.


The output datacards and combine root files (with the limits) will be saved in the *datacards* directory.

## Making limits plots
After running the datacards, you can make the final limits plots from the *scripts* directory. If you wish to run over different datacards, you'll need
to change the directory.


For the 1D TChiHH model, this currently makes both the combined limit plot and the resolved vs boosted overlay plot:
```bash
python -b brazilHiggsinoLims.py
```
To change the directory where the datacards are stored: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/scripts/brazilHiggsinoLims.py#L17

This currently also saves the root file for HEPData, with all limits included.
You can change what you want to run and save at the bottom of the code.




For the 1D T5HH model, this currently makes only the combined limit plot (but is configurable to make the resolved vs boosted overlay plot, too):
```bash
python -b brazilGluinoLims.py
```
To change the directory where the datacards are stored: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/scripts/brazilGluinoLims.py#L16

This currently also saves the root file for HEPData, with all limits included.
You can change what you want to run and save at the bottom of the code.



Finally, for the 2D TChiHH model, this currently makes the combined 2D limit plot:
```bash
root -l -b limit_scan.cxx
```
The code is setup to also run for boosted only and resolved only, you just need to change the strings "which" at the time.
I've written this to run over the 2D gluino model as well, but I haven't tested it in awhile since we didn't have samples.
Currently, the production of the root file for HEPData is commented out.


If you haven't, you'll need to run `scan_point.cxx` before running the 2D limits plot. You'll get a warning that tells you you need to if you haven't.
```bash
root -l 'scan_point.cxx("N1N2","comb")'
```
The first argument is the model (N1N2 for 2D TChiHH, and Gluino for 2D T5HH). The second argument is the type you want:
"comb" for the combination, "res" for resolved only, and "boost" for boosted only.
If you wish to change the directory of the datacards, you can do that here: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/scripts/scan_point.cxx#L23

You will need to re-run `scan_point.cxx`, or delete the saved files in the *src* directory and run `runPaperPlots.sh`, which will automatically
run scan_point if the files don't exist.


All of the final plots are in the *output* directory.



## Plots and tables for auxilary materials
The following code pieces in the *scripts* directory create different plots and tables that go into auxilary materials:
- `grabMatrices.py`: Opens the root files automagically made from the combine checks from pre-approval, pulls out the correlation matrix and saves it to a new root file.
Additionally, pulls out the covariance matrix, takes only the signal region bins (from boosted and resolved), and saves that new matrix to the same root file.
No idea if this is the correct thing to do right now! Currently, this input is in the *src* directory (fitDiagnostics_t0.root).
Output root file is in the *output* directory.
- `signalEfficiency.py`: Creates the signal efficiency plots from the datacards. Creates for resolved only, boosted only, and combination for all 3 signal models
(1DTChiHH, 2DTChiHH, 1DT5HH). The denominator of the T5HH is all H decays (but HH events only), whereas the TChiHH denominator is only H->bb events,
since that's how the signal samples were generated.
Can also creates a root file with all of these histograms saved into it for HEPData (commented out right now). Also for
HEPData, can produce a root file with the signal efficiency/contamination in all boosted regions for a single mass point of T5HH (commented out right now). The directory for the datacards is at the top of the code. Output PDF and root files are in the *output* directory.
- `significance.py`: Creates the significance plot for the combination only for TChiHH and T5HH. Also creates the root file for HEPData (currently commented out). Would need to
modify `QuickDataCardsABCDNorm_Higgsino.py` and `QuickDataCardsABCDNorm_Gluino.py` to have combine run the significance (commented out at the bottom)
if these don't exist yet. You can change the directory of the datacards at the top of the code. Output PDF and root files are in the *output* directory.
- `writeCutflow.py`: Creates a txt file with Latex-style output used in the auxiliary material for the cutflow table. Can also be modified to write out the
more-detailed cutflow. You need to first run `cutflowFromNtuples.cc` for this to work, but it's setup to use ones I've already ran.
- `ABCD.C`: The main plotting script for the boosted part of the analysis, making a lot of the plots and tables in the paper, AN, and the auxilary materials.
It is currently setup to only run the plots in the paper.
There are bools at the top that you can change to run other plots.
