Repo for studying boosted Higgs tools with the RA2b trees.  This package relies on other packages,
depending on what you want to do.

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

### Running scans and/or skims
Hopefully, you don't have to do this. Scanning and skimming is done using the Analysis package above (by kpedro88). The README there works pretty well. You can find the files I modified inside the *skimming* directory, which is setup in the same way as Analysis. In particular, a lot of the variables and filters were changed to be wrt to MET instead of MHT. The isolated track veto definitions were also changed to be in-line with the resolved analysis. Checkout the Analysis package, replace the files with the changed ones in the *skimming* directory, then follow the Analysis README. The first step is to run TreeMaker ntuples off of the miniAOD datasets. Then, signal files will need to be split by SUSY mass (this is the scanning step). After, the scans will need to be skimmed. MC background and data only needs to be skimmed.

'input/input_selection.txt' determines the selection type (signal is the one you want, you'll be able to see how the different categories depend on each other). If a selection says b:tag[1], that means the branch will be created, but the filter/selection will not be applied to the skims. Down toward the bottom you'll find the variations. The JEC/JER variations are already set (and move jet variables up and down by the uncertainties). I added the METVars variation, which changes all the filters to be wrt to MET, and the CleanMETVars variation (used only for gamma+jets), which does what METVars does, but also cleans the photon from the AK4 jets, HT, MET, and other variables. The PoorClean variation removes any AK8 jet if it has the photon in it.

For scanning, make sure the appropriate input/dict_scan\*.py contains your sample. It will if you're running anything that's been run before. The blocks are broken down in input/input_sets_scan_\*.txt. The tabbing/spacing is special on this file, so be careful if you add anything. Then, batch/exportScan*.sh determines which blocks are actually run. These blocks match the naming in the input_sets_scan txt file. Comment out any files you don't want scanned, or add your own. Again, this is only for signal files. Finally, batch/SCsub.sh determines the input directory (location of TreeMaker files) and output (use your own eos area) for the scans. Which files are to be scanned are set in INPUT (which is input_scan.txt, and defined at the bottom of THAT is the input_sets_scan), and the appropriate export\*.sh file (with the blocks commented in or out) is pulled in: https://github.com/kpedro88/Analysis/blob/e3c81179452cc24b95455e409524f15adbb85a12/batch/SCsub.sh#L32
Then, follow the Analysis README for how to submit the jobs.



For skimming, make sure the appropriate input/dict_skim\*.py contains your sample. It will if you're running anything that's been run before. The blocks are broken down in input/input_sets_skim_\*.txt. The tabbing/spacing is special on this file, so be careful if you add anything. Then, batch/exportSkim*.sh determines which blocks are actually run. These blocks match the naming in the input_sets_skim txt file. Comment out any files you don't want skimmed, or add your own. Finally, batch/SKsub.sh determines the input directory (INDIR, location of TreeMaker files or scanned files for signal) and output (STORE, use your own eos area) for the skims. Which files are to be scanned are set in INPUT (which is input_selection.txt, and defined at the bottom of THAT is the input_sets_skim), and the appropriate export.sh file (with the blocks commented in or out) is pulled in: https://github.com/kpedro88/Analysis/blob/e3c81179452cc24b95455e409524f15adbb85a12/batch/SKsub.sh#L62. The selection type (which skim cuts you want applied, defined in input_selection.txt) are passed as SELTYPE1. I always comment out the other types, since I only run this for a single thing at once. Then follow the Analysis README for how to submit the jobs. Once the jobs finish, you will want to hadd them. Use the hadd_skims.sh script (instructions on Analysis README), changing the top-level directory to your eos area. The script goes through every sub-directory and hadd's anything with "block" in the name, and then removes all "block" files. It's quite nifty!



## Overview:
There are (unfortunately) lots of steps.
1. First, you'll need skims
  - If you want to make your own skims, follow Kevin's instructions: https://github.com/kpedro88/Analysis/tree/SUSY2018, or the instructions detailed above
  - You can also use the skims at /eos/uscms/store/user/emacdona/Skims/Run2ProductionV18 and /eos/uscms/store/user/kaulmer/Skims/ (for the TChiHH skims).
2. Once you have skims, you'll need to run ALPHABET to save histograms of the analysis regions
3. After, you can run the datacards (writing, merging, and running combine)
4. Finally, you can make plots

The pre-existing datacards can be found on LXPLUS (although I'm not sure how long they will available): /afs/cern.ch/user/e/emacdona/public/HHMET_datacards


To make all the paper plots from pre-existing datacards in Emily's area, you can run:
<pre>
cd scripts/
bash runPaperPlots.sh        
</pre>
This creates all the paper plots (and the auxiliary material plots) and writes them to the *output* directory. If you need plots that are in the AN, they'll be in `ABCD.C`. You'll probably have to change the code a bit to get it to run what you want.

## Batch submission:

Batch submissions are done through `submitBatch.sh` and `submitBatchCutflow.sh` in the *scripts* directory.  Code is tar'ed, sent to the worker nodes, and final output
root files are transferd to eos. You will need to have a directory in your eos area called boostedHiggsPlusMET, or you'll get a cryptic warning.

First, we compile whatever code we're batch submitting, since we don't trust the script to do it itself:
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
And THEN we submit whatever batch code we want, modifying it to only submit what we want to run:
```
./submitBatch.sh
```

- `submitBatch.sh` runs the ALPHABET code or a cutflow (off of skims only). Modify the script to run what you want, and change the outgoing directory name.
- `submitBatchCutflow.sh` runs cutflowFromNtuples.cc from the ntuples. This takes a very long time (3ish days) to run for MC background.

The processed files are moved to /eos/uscms/store/user/${USER}/boostedHiggsPlusMET/, so make sure you have a *boostedHiggsPlusMET* directory in your eos area.

## Region descriptions/definitions
`src/definitions.cc` contains the region definitions, baseline selections, useful functions, and the veto option that removes events that overlap with the resolved method.

Baseline selections are defined for the signal region and for each of the validation regions. Broadly, they require two AK8 jets with transverse momentum above 300 GeV and mass between 60 and 260 GeV, MET>300 GeV, HT>600 GeV, deltaPhi cuts, and event filters. There are two validation regions: single photon (proxy for Z->nunu), and a single lepton control region (W/top enriched). These have slightly different selections than the signal samples. The boosted HH+MET analysis using 2016-only data also had a QCD-enrinched control region (called low dphi), so I left the functions for that, in case they're useful in the future.


## Using ALPHABET
The ALPHABET code (`scripts/ALPHABET.cc`) creates histograms of different variables in different signal and control regions. These output histograms are used to both make datacards and for plots of baseline and region variables.

`ALPHABET.cc` runs the samples by type (signal, MC background, single-lepton) and year. It also runs by NLSP mass for the signal samples.

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

Example running "./ALPHABET 1 MC2016 1 600" - runs the 1D TChiHH(600,1) mass point for 2016, removing the events that overlap with the resolved. Example 2: "./ALPHABET 0 MC2016 0 0" runs the MC background without any overlapping events removed.

If you wish to run the 1D T5HH FullSIM samples, you will need to change the bool in `skimSamples.cc` (https://github.com/emacdonald16/HHplusMET/blob/v2021/src/skimSamples.cc#L81-L82) and the bool in `ALPHABET.cc` (https://github.com/emacdonald16/HHplusMET/blob/v2021/scripts/ALPHABET.cc#L27).



You can also batch-submit the ALPHABET code, see above.

Included in `ALPHABET` are a variety of bools and functions to run the signal systematics. These are best run using batch submission, changing the
necessary bools and function arguments for every individual systematic and variation (yes, there are a lot). The batch code is setup to easily change the
name of the output directory (example, "JECUp") to keep track of all of these.

If you re-run the signal samples, you will need to hadd them to create a single file for the datacards and the plotting scripts to find.
I have been naming these `ALPHABET_1DSignal.root`, but you can name it what you like as long as you change the input for the datacards and/or
`ABCD.C`, which is the plotting script for distributions.

For the FastSIM samples, you will also need to run ALPHABET using genMET instead of recoMET (the MET is averaged for regular running for the FastSIM MET systematic).
The bool is set in `src/definitions.cc`: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/src/definitions.cc#L412


#### If you need to change/add signal samples
The samples that are run are included in `src/skimSamples.cc`. There are bools that define which samples are run, depending on the region
that is passed from `ALPHABET.cc`. The 1D samples are run using region==1, the 2D samples with region==2. The bools are set further down: https://github.com/emacdonald16/HHplusMET/blob/v2021/src/skimSamples.cc#L81-L82

If you wish to add/change signal samples, this is where you do it. The directory of the skims is defined at the top,
and you can change/add signal samples closer to the bottom: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/src/skimSamples.cc#L476-L485



#### To run boosted event counts (for WX overlap studies)
If you need to run boosted event counts for overlap studies, there is a bool at the top of `ALPHABET.cc` that does just that "saveBoostedEvt".
The text files are created for each analysis region, and for the same arguments you pass `ALPHABET.cc` (region, year, resVeto, NLSP mass).
- If you wish to run on data, set the bool "runData" at the top of the ALPHABET code to true, and run with the argument region=0. This
is currently setup so that if runData=true, the MC will not run.
- I did not set this up to include multiple signal mass points in the same text file!
- These output text files are not transfered to an eos area once ALPHABET is done running, so I suggest running locally.
- The files are output into *src/evtCount/boost/*


## Plotting distributions from ALPHABET
`ABCD.C` contains loads of functions that make different plots. It is currently setup to only run the plots that are put into the published paper, but you can easily change it to run the plots that go into the supplementary materials and the AN.
IT DOES NOT RUN THE LIMIT PLOTS.
To run:
``` bash
root -l -b ABCD.C
```
This will create an output root file with all the plots saved into it, and will save the paper plots as PDFs. You can toggle the bool, or change which plots are run.


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
./cutflowFromNtuples 0 ZJets MC2016 0 0
```
- This is currently only setup to run on MC background (signal) and the 1D T5HH signal samples. Would need some modifications to run over the other signal samples.
- The first argument is the region (0=MCbkg, 1=T5HH), second is the type (ZJets, WJets, TT, SnglT, QCD, or T5HH), third is the year (MC2016, MC2017, MC2018 - T5HH samples run all three years at once, so only need, say, MC2016 for that), fourth argument is the NLSP mass for the T5HH signal samples (I put 0 for the MC background), and fifth argument is a bool for a more detailed cutflow, breaking down filters, etc. (0 = more basic cutflow).


## Making datacards
You will need the combine package in your CMSSW_10_2_13/src/ area. Follow the instructions here: https://cms-analysis.github.io/HiggsAnalysis-CombinedLimit/

Before making the datacards, you'll need to run `ALPHABET.cc` to create the necessary histograms, see above.
You will also need to run all of the necessary signal systematic variations, or switch the bool at the top "runSysts" to False.

The main code for writing the boosted datacards is `datacards/makeDatacards.py`. This is currently setup to run only the three models we considered.
I would only run the function makeDatacards() with this code (others provided); merging the datacards and running combine is best done using a different script. Choose what you would like to run at the bottom of the script.
At the top are the paths to wherever the ALPHABET files are stored. If you make new/different signal samples, you will need to change
these paths: https://github.com/emacdonald16/HHplusMET/blob/534764a2611a6153ffd1eb583f88c2598dad23a1/datacards/makeDatacards.py#L23
```bash
python -b makeDatacards.py
```

The datacards can be run for a single mass point from the *datacards* directory by modifying the mass points considered:
https://github.com/emacdonald16/HHplusMET/blob/534764a2611a6153ffd1eb583f88c2598dad23a1/datacards/makeDatacards.py#L47


This code create the datacards from the output of ALPHABET, including all the signal systematics that would need to be run. After creating the datacards, it can also merge into the final datacards and then run combine to produce the limit root files, but I would use `combineAndRunDatacards.py` instead, since it uses some multi-threading magic to run things faster.






Use `combineAndRunDatacards.py` in the *datacards* directory to merge the datacards and then run combine. As an example, to run:
```bash
python -b combineAndRunDatacards.py -s TChiHH-G -p all --phase_space combined
```
You can run for a single mass point or all, and for any of the topologies. It is also setup to merge the cards or run combine alone. The resolved datacards need to be in the *datacards/resData/* directory to run either the ResolvedOnly or Combined topologies.
The arguments and available options are described in further detail in the code: https://github.com/emacdonald16/HHplusMET/blob/534764a2611a6153ffd1eb583f88c2598dad23a1/datacards/combineAndRunDatacards.py#L141-L147

The output datacards and combine root files (with the limits) will be saved in the *datacards* directory.


## Making limits plots
After running the datacards, you can make the final limits plots from the *scripts* directory. If you wish to run over different datacards, you'll need
to change the directory.


For the 1D TChiHH model, this currently makes both the combined limit plot and the resolved vs boosted overlay plot:
```bash
python -b brazilHiggsinoLims.py
```
To change the directory where the datacards are stored: https://github.com/emacdonald16/HHplusMET/blob/534764a2611a6153ffd1eb583f88c2598dad23a1/scripts/brazilHiggsinoLims.py#L25
This currently also saves the root file for HEPData, with all limits included. You can change what you want to run and save at the bottom of the code. The resulting limits plots and root file will be in the *output* directory.




For the 1D T5HH model, this currently runs the combined limit plot and the resolved vs boosted overlay plot, too:
```bash
python -b brazilGluinoLims.py
```
To change the directory where the datacards are stored: https://github.com/emacdonald16/HHplusMET/blob/534764a2611a6153ffd1eb583f88c2598dad23a1/scripts/brazilGluinoLims.py#L16

This currently also saves the root file for HEPData, with all limits included. You can change what you want to run and save at the bottom of the code. The resulting limits plots and root file will be in the *output* directory.



Finally, for the 2D TChiHH model, this currently makes the combined 2D limit plot:
```bash
root -l -b limit_scan.cxx
```
The code is setup to also run for boosted only and resolved only, you just need to change the string "which" inside the code.
Currently, the production of the root file for HEPData is commented out. You cannot run saveRootFile() if anything else also runs - it does some weird stuff.


If you haven't, you'll need to run `scan_point.cxx` before running the 2D limits plot. You'll get a warning that tells you you need to if you haven't.
```bash
root -l 'scan_point.cxx("N1N2","comb")'
```
The first argument is the model (N1N2 for 2D TChiHH, and Gluino for 2D T5HH). The second argument is the type you want:
"comb" for the combination, "res" for resolved only, and "boost" for boosted only.
If you wish to change the directory of the datacards, you can do that here: https://github.com/emacdonald16/HHplusMET/blob/f227a4e88a51027040635d4db52db959d0ced69d/scripts/scan_point.cxx#L23

All of the final plots are in the *output* directory.



## Plots and tables for auxilary materials
The following code pieces in the *scripts* directory create different plots and tables that go into auxilary materials:
- `grabMatrices.py`: Opens the root files automagically made from the combine checks from pre-approval, pulls out the correlation matrix and saves it to a new root file.
Additionally, pulls out the covariance matrix, takes only the signal region bins (from boosted and resolved), and saves that new matrix to the same root file. Currently, this input is in the *src* directory (fitDiagnostics_t0.root). Output root file is in the *output* directory. NOTE: this is not what is used for the plots in the supplementary materials! Bill ran those and passed me a root file, that I then made pretty with `ABCD.C` (the makePretty() function).
- `signalEfficiency.py`: Creates the signal efficiency plots from the datacards. Creates for resolved only, boosted only, and combination for all 3 signal models
(1DTChiHH, 2DTChiHH, 1DT5HH). The denominator of the T5HH is all H decays (but HH events only), whereas the TChiHH denominator is only H->bb events,
since that's how the signal samples were generated.
Can also creates a root file with all of these histograms saved into it for HEPData (commented out right now). Also for
HEPData, can produce a root file with the signal efficiency/contamination in all boosted regions for a single mass point of T5HH (commented out right now). The directory for the datacards is at the top of the code. Output PDF and root files are in the *output* directory.
- `significance.py`: Creates the significance plot for the combination only for TChiHH and T5HH. Also creates the root file for HEPData (currently commented out). You can run the significance files using `runAndCombineDatacards.py` and using the argument "-sig".
You can change the directory of the datacards at the top of the code. Output PDF and root files are in the *output* directory.
- `writeCutflow.py`: Creates a txt file with Latex-style output used in the auxiliary material for the cutflow table. Can also be modified to write out the
more-detailed cutflow. You need to first run `cutflowFromNtuples.cc` for this to work, but it's setup to use ones I've already ran.
- `ABCD.C`: The main plotting script for the boosted part of the analysis, making a lot of the plots and tables in the paper, AN, and the auxilary materials.
It is currently setup to only run the plots in the paper and in the auxiliary materials (no tables, nor AN plots).
