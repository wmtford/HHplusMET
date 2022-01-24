#!/bin/env python
import os
import sys
import multiprocessing
import glob
import subprocess
from time import time
import argparse

'''
Written mostly by Jaebak, modified by me
Runs very quickly since multi-threaded or something
makeMergeHiggsinoCommands() or makeMergeGluinoCommands(): Can combine datacards into the final ones used for running limits (need the ones from makeDatacards.py first)
makeCombineCommands(): Runs combine for the chosen model and type
Running options in main at the bottom
'''


def getTChiHHMassPoints():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    mass_points = []
    #G rab the mass points from the NAMES file - just need the mass points
    with open(script_dir+"/../src/higgsino2DFileNames.txt", 'r') as namesFile:
        for line in namesFile:
            x = line.split('_')
            hino_mass = int(x[5])
            LSP_mass = int(x[6])
            if hino_mass>810: break;
            mass_points.append([hino_mass, LSP_mass])
    # Add 1D scan
    for i in range(0, 27):
        hino_mass=150+i*25
        mass_points.append([hino_mass, 1])
    return mass_points


def runCommandForMultiProcessing(command):
    # Jaebak's magic
    print(command)
    process = subprocess.Popen(command[1], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    out, err = process.communicate()
    return out.decode("utf-8")+'\n'+err.decode("utf-8")


def runCommands(commands):
    # Jaebak's magic
    commands_info = [(index, commands[index]) for index in range(len(commands))]
    pool = multiprocessing.Pool()
    pool.map(runCommandForMultiProcessing, commands_info)


def makeMergeHiggsinoCommands(whichDString,signal,hino,LSP,dataMCString, phase_space):
    # Merges the datacards into the "final" ones combine runs over
    hino = int(hino); finalLSP = int(LSP)
    # resolved and boosted have different mass point names
    # Make them consistent and set final datacards to have the good names (For example, mLSP=225 instead of mLSP=223)
    if (finalLSP==1): finalLSP=1;
    elif (finalLSP%5!=0): finalLSP=finalLSP+2;
    resLSP = finalLSP
    if (resLSP==1): resLSP=0;

    # Make merge cards commands
    combineCommands = []
    # Boosted only
    if 'boosted' in phase_space:
        combineCommands.append("combineCards.py SRMerge_%s%s%d_LSP%d_%s.txt CRBMerge_%s%s%d_LSP%d_%s.txt CRCMerge_%s%s%d_LSP%d_%s.txt CRDMerge_%s%s%d_LSP%d_%s.txt CREMerge_%s%s%d_LSP%d_%s.txt > %s%s%d_LSP%d_BothBoostedH_%s.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
    # Resolved only
    if 'resolved' in phase_space:
        combineCommands.append("combineCards.py resData/%sTChiHH_%s_btagfix/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_ResOnly.txt "%(whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,finalLSP,dataMCString))
    # BoostedOnlyVeto and Combination
    if 'combined' in phase_space:
        combineCommands.append("combineCards.py SRMerge_%s%s%d_LSP%d_%s_veto.txt CRBMerge_%s%s%d_LSP%d_%s_veto.txt CRCMerge_%s%s%d_LSP%d_%s_veto.txt CRDMerge_%s%s%d_LSP%d_%s_veto.txt CREMerge_%s%s%d_LSP%d_%s_veto.txt > %s%s%d_LSP%d_BothBoostedH_%s_veto.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
        combineCommands.append("combineCards.py %s%s%d_LSP%d_BothBoostedH_%s_veto.txt resData/%sTChiHH_%s_btagfix/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_Combo.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,finalLSP,dataMCString))
    return combineCommands


def makeMergeGluinoCommands(whichDString,signal,hino,LSP,dataMCString, phase_space):
    # Merges the datacards into the "final" ones combine runs over
    hino = int(hino); finalLSP = int(LSP)
    if (finalLSP==1): finalLSP=1; #some resolved datacards have mLSP=0
    elif (finalLSP%5!=0): finalLSP=finalLSP+2;

    # Make marge datacards commands
    combineCommands = []
    # Boosted only
    if 'boosted' in phase_space:
        combineCommands.append("combineCards.py SRMerge_%s%s%d_LSP%d_%s.txt CRBMerge_%s%s%d_LSP%d_%s.txt CRCMerge_%s%s%d_LSP%d_%s.txt CRDMerge_%s%s%d_LSP%d_%s.txt CREMerge_%s%s%d_LSP%d_%s.txt > %s%s%d_LSP%d_BothBoostedH_%s.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
    # Resolved only
    if 'resolved' in phase_space:
        combineCommands.append("combineCards.py resData/%sT5HH_%s_btagfix/datacard-%s_mGluino-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_ResOnly.txt "%(whichDString,dataMCString,signal,hino,finalLSP,whichDString,signal,hino,finalLSP,dataMCString))
    # BoostedOnlyVeto and Combination
    if 'combined' in phase_space:
        combineCommands.append("combineCards.py SRMerge_%s%s%d_LSP%d_%s_veto.txt CRBMerge_%s%s%d_LSP%d_%s_veto.txt CRCMerge_%s%s%d_LSP%d_%s_veto.txt CRDMerge_%s%s%d_LSP%d_%s_veto.txt CREMerge_%s%s%d_LSP%d_%s_veto.txt > %s%s%d_LSP%d_BothBoostedH_%s_veto.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
        combineCommands.append("combineCards.py %s%s%d_LSP%d_BothBoostedH_%s_veto.txt resData/%sT5HH_%s_btagfix/datacard-%s_mGluino-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_Combo.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,dataMCString,signal,hino,finalLSP,whichDString,signal,hino,finalLSP,dataMCString))
    return combineCommands


def makeCombineCommands(whichDString,signal,hino,LSP,dataMCString, phase_space):
    # Makes the combine limit commands
    # Make sure LSP mass is consistent with datacards
    hino = int(hino); finalLSP = int(LSP)
    if (finalLSP==1): finalLSP=1;
    elif (finalLSP%5!=0): finalLSP=finalLSP+2;

    # Make combine commands
    combineCommands = []
    # Boosted only
    if 'boosted' in phase_space:
        combineCommands.append("combine -M AsymptoticLimits -n %s%s%d_LSP%d_BothBoostedH_%s %s%s%d_LSP%d_BothBoostedH_%s.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
    # Resolved only
    if 'resolved' in phase_space:
        combineCommands.append("combine -M AsymptoticLimits -n %s%s%d_LSP%d_%s_ResOnly %s%s%d_LSP%d_%s_ResOnly.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
    # Combination
    if 'combined' in phase_space:
        combineCommands.append("combine -M AsymptoticLimits -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
    return combineCommands


def makeSignificanceCommands(whichDString,signal,hino,LSP,dataMCString, phase_space):
    # Make significance commands, only for combined
    hino = int(hino); finalLSP = int(LSP)
    if (finalLSP==1): finalLSP=1;
    elif (finalLSP%5!=0): finalLSP=finalLSP+2;
    significanceCommands = []
    if 'combined' in phase_space:
        significanceCommands.append("combine -M Significance --rMin=-10 --uncapped yes -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
    return significanceCommands



if __name__ == '__main__':
    # Setup arguments
    parser = argparse.ArgumentParser(description='''\
        Combines resolved and boosted datacards.
        Requires boosted datacards to be in current working directory and
        resolved datacards in one of the below directories:
        - resData/1DTChiHH_Data_btagfix
        - resData/2DTChiHH_Data_btagfix
        - resData/1DT5HH_Data_btagfix
        ''', formatter_class=argparse.RawTextHelpFormatter)
    parser.add_argument('-s','--signal_model', required=True, help='Signal model: TChiHH-G or TChiHH or T5HH')
    parser.add_argument('-p','--mass_points', required=True, help='Signal mass points in following format: 500_100,500_1. Can also use "all"')
    parser.add_argument('-f','--fake_run', action="store_true", help='Does not run commands. Only print commands')
    parser.add_argument('-m','--only_merge', action="store_true", help='Do only merge datacards')
    parser.add_argument('-c','--only_combine', action="store_true", help='Do only combine')
    parser.add_argument('-sig','--only_sig', action="store_true", help='Run significance only')
    parser.add_argument('--phase_space', help='Phase space to process. Use following format: resolved,boosted,combined. Can also use "all"', default='all')
    args = parser.parse_args()

    # Set variables from arguments
    if args.signal_model == "TChiHH-G":
        whichDString = "1D"
        signal = "TChiHH"
    elif args.signal_model == "TChiHH":
        whichDString = "2D"
        signal = "TChiHH"
    elif args.signal_model == "T5HH":
        whichDString = "1D"
        signal = "T5HH"
    else:
        print("[Error] Undefined signal model: "+args.signalModel+". Exiting")
        sys.exit()

    # Grab mass points
    # mass_points = [(NLSP, LSP)]
    mass_points = []
    if args.mass_points == "all":
        if args.signal_model == "TChiHH-G": mass_points = [[150+item*25, 1]for item in range(0, 55)]
        elif args.signal_model == "TChiHH": mass_points = getTChiHHMassPoints()
        elif args.signal_model == "T5HH": mass_points = [[1000+item*100,1] for item in range(0,16)]
    else:
        raw_mass_points = args.mass_points.split(',')
        mass_points = [item.split('_') for item in raw_mass_points]

    #Get phase space we're running over
    # phase_space = [resolved, boosted, combined]
    if args.phase_space == "all": phase_space = ['resolved', 'boosted', 'combined']
    else: phase_space = args.phase_space.split(',')

    print('Mass points: '+str(mass_points))
    t0 = time()

    # Merge datacards
    merge_commands = []
    for mass_point in mass_points:
        hino=mass_point[0]
        LSP=mass_point[1]
        if signal == "TChiHH": merge_commands.extend(makeMergeHiggsinoCommands(whichDString,signal,hino,LSP,"Data", phase_space))
        elif signal == "T5HH": merge_commands.extend(makeMergeGluinoCommands(whichDString,signal,hino,LSP,"Data", phase_space))
    if not args.only_combine and not args.only_sig:
        if args.fake_run: print('Commands to run:'+str(merge_commands))
        else: runCommands(merge_commands)


    # Run combine on datacards
    combine_commands = []
    for mass_point in mass_points:
        hino=mass_point[0]
        LSP=mass_point[1]
        combine_commands.extend(makeCombineCommands(whichDString,signal,hino,LSP,"Data", phase_space))
    if not args.only_merge and not args.only_sig:
        if args.fake_run: print('Commands to run:'+str(combine_commands))
        else: runCommands(combine_commands)


    # Run significance on datacards
    significance_commands = []
    for mass_point in mass_points:
        hino=mass_point[0]
        LSP=mass_point[1]
        significance_commands.extend(makeSignificanceCommands(whichDString,signal,hino,LSP,"Data", phase_space))
    if args.only_sig:
        if args.fake_run: print('Commands to run:'+str(significance_commands))
        else: runCommands(significance_commands)

    print('\nProgram took %.0fm %.0fs.' % ((time()-t0)/60,(time()-t0)%60))
