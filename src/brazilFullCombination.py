#! /usr/bin/env python

import os
import glob
import math
import array
import sys
import time
import ROOT
from array import array


import tdrstyle
tdrstyle.setTDRStyle()
# ROOT.gROOT.ProcessLine(".L ~/tdrstyle.C");
# ROOT.setTDRStyle();
# ROOT.gStyle.SetPadLeftMargin(0.16);
# ROOT.gStyle.SetPadRightMargin(0.10);
# ROOT.gStyle.SetPadTopMargin(0.10);
# ROOT.gStyle.SetPalette(1);

## ===========================================================================================
## ===========================================================================================
## ===========================================================================================

def columnToList(fn,col):
	f = open(fn,'r');

	olist = [];
	for line in f: 
		linelist = line.strip().split()
		olist.append( linelist[col] );
	return olist

def ExtractFile(iname, tag):
	f = ROOT.TFile(iname);
	t = f.Get("limit");
	lims = [];
	lims.append(tag);
	for i in range(6):
		t.GetEntry(i);
		lims.append( t.limit )
	return lims;

if __name__ == '__main__':

	#idir = "/eos/uscms/store/user/ntran/SUSY/statInterp/scanOutput/Dec6";
	#idir = "/uscms_data/d2/rgp230/BoostedHPush/NewCommit/CMSSW_7_4_2/src/boostedHiggsPlusMET/datacardsRateParamTest/";
	idir = "./";
	resultsCombFull = [];
	resultsComb = [];
	resultsBoost=[];
	resultsResolv=[]
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH200.Asymptotic.mH200.root','200') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH225.Asymptotic.mH225.root','225') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH250.Asymptotic.mH250.root','250') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH275.Asymptotic.mH275.root','275') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH300.Asymptotic.mH300.root','300') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH325.Asymptotic.mH325.root','325') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH350.Asymptotic.mH350.root','350') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH375.Asymptotic.mH375.root','375') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH400.Asymptotic.mH400.root','400') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH425.Asymptotic.mH425.root','425') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH450.Asymptotic.mH450.root','450') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH475.Asymptotic.mH475.root','475') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH500.Asymptotic.mH500.root','500') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH525.Asymptotic.mH525.root','525') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH550.Asymptotic.mH550.root','550') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH575.Asymptotic.mH575.root','575') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH600.Asymptotic.mH600.root','600') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH625.Asymptotic.mH625.root','625') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH650.Asymptotic.mH650.root','650') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH675.Asymptotic.mH675.root','675') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH700.Asymptotic.mH700.root','700') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH725.Asymptotic.mH725.root','725') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH750.Asymptotic.mH750.root','750') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH775.Asymptotic.mH775.root','775') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH800.Asymptotic.mH800.root','800') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH825.Asymptotic.mH825.root','825') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH850.Asymptotic.mH850.root','850') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH875.Asymptotic.mH875.root','875') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH900.Asymptotic.mH900.root','900') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH925.Asymptotic.mH925.root','925') );
	#resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH950.Asymptotic.mH120.root','950') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH975.Asymptotic.mH975.root','975') );
	resultsResolv.append( ExtractFile(idir+'/higgsCombineHiggsinoResolved4b_METOnlyTChiHH1000.Asymptotic.mH1000.root','1000') );

	#results.append( ExtractFile(idir+'/results_T2tt_175_1_mu0.0.root','175') );
	#results.append( ExtractFile(idir+'/higgsCombinetestCards-allBkgs-TChiHH_127_1-35.9-mu0.0.Asymptotic.mH120.root','127') );
	#results.append( ExtractFile(idir+'/higgsCombinetestCards-allBkgs-TChiHH_150_1-35.9-mu0.0.Asymptotic.mH120.root','150') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH200.Asymptotic.mH200.root','200') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH225.Asymptotic.mH225.root','225') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH250.Asymptotic.mH250.root','250') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH275.Asymptotic.mH275.root','275') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH300.Asymptotic.mH300.root','300') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH325.Asymptotic.mH325.root','325') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH350.Asymptotic.mH350.root','350') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH375.Asymptotic.mH375.root','375') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH400.Asymptotic.mH400.root','400') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH425.Asymptotic.mH425.root','425') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH450.Asymptotic.mH450.root','450') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH475.Asymptotic.mH475.root','475') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH500.Asymptotic.mH500.root','500') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH525.Asymptotic.mH525.root','525') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH550.Asymptotic.mH550.root','550') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH575.Asymptotic.mH575.root','575') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH600.Asymptotic.mH600.root','600') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH625.Asymptotic.mH625.root','625') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH650.Asymptotic.mH650.root','650') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH675.Asymptotic.mH675.root','675') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH700.Asymptotic.mH700.root','700') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH725.Asymptotic.mH725.root','725') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH750.Asymptotic.mH750.root','750') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH775.Asymptotic.mH775.root','775') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH800.Asymptotic.mH800.root','800') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH825.Asymptotic.mH825.root','825') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH850.Asymptotic.mH850.root','850') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH875.Asymptotic.mH875.root','875') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH900.Asymptotic.mH900.root','900') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH925.Asymptotic.mH925.root','925') );
	#resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH950.Asymptotic.mH120.root','950') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH975.Asymptotic.mH975.root','975') );
	resultsBoost.append( ExtractFile(idir+'/higgsCombineHiggsinoBoosted_METOnly_TChiHH1000.Asymptotic.mH1000.root','1000') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH200.Asymptotic.mH200.root','200') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH225.Asymptotic.mH225.root','225') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH250.Asymptotic.mH250.root','250') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH275.Asymptotic.mH275.root','275') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH300.Asymptotic.mH300.root','300') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH325.Asymptotic.mH325.root','325') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH350.Asymptotic.mH350.root','350') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH375.Asymptotic.mH375.root','375') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH400.Asymptotic.mH400.root','400') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH425.Asymptotic.mH425.root','425') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH450.Asymptotic.mH450.root','450') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH475.Asymptotic.mH475.root','475') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH500.Asymptotic.mH500.root','500') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH525.Asymptotic.mH525.root','525') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH550.Asymptotic.mH550.root','550') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH575.Asymptotic.mH575.root','575') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH600.Asymptotic.mH600.root','600') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH625.Asymptotic.mH625.root','625') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH650.Asymptotic.mH650.root','650') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH675.Asymptotic.mH675.root','675') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH700.Asymptotic.mH700.root','700') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH725.Asymptotic.mH725.root','725') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH750.Asymptotic.mH750.root','750') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH775.Asymptotic.mH775.root','775') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH800.Asymptotic.mH800.root','800') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH825.Asymptotic.mH825.root','825') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH850.Asymptotic.mH850.root','850') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH875.Asymptotic.mH875.root','875') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH900.Asymptotic.mH900.root','900') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH925.Asymptotic.mH925.root','925') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH975.Asymptotic.mH975.root','975') );
	resultsComb.append( ExtractFile(idir+'/higgsCombineResolvedBoostCombinationTChiHH1000.Asymptotic.mH1000.root','1000') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH200.Asymptotic.mH200.root','200') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH225.Asymptotic.mH225.root','225') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH250.Asymptotic.mH250.root','250') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH275.Asymptotic.mH275.root','275') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH300.Asymptotic.mH300.root','300') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH325.Asymptotic.mH325.root','325') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH350.Asymptotic.mH350.root','350') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH375.Asymptotic.mH375.root','375') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH400.Asymptotic.mH400.root','400') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH425.Asymptotic.mH425.root','425') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH450.Asymptotic.mH450.root','450') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH475.Asymptotic.mH475.root','475') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH500.Asymptotic.mH500.root','500') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH525.Asymptotic.mH525.root','525') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH550.Asymptotic.mH550.root','550') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH575.Asymptotic.mH575.root','575') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH600.Asymptotic.mH600.root','600') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH625.Asymptotic.mH625.root','625') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH650.Asymptotic.mH650.root','650') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH675.Asymptotic.mH675.root','675') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH700.Asymptotic.mH700.root','700') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH725.Asymptotic.mH725.root','725') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH750.Asymptotic.mH750.root','750') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH775.Asymptotic.mH775.root','775') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH800.Asymptotic.mH800.root','800') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH825.Asymptotic.mH825.root','825') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH850.Asymptotic.mH850.root','850') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH875.Asymptotic.mH875.root','875') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH900.Asymptotic.mH900.root','900') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH925.Asymptotic.mH925.root','925') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH975.Asymptotic.mH975.root','975') );
	resultsCombFull.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH1000.Asymptotic.mH1000.root','1000') );
	xsecs=[1335.62,860.597,577.314,400.107,284.855,207.36,153.841,116.006,88.7325,68.6963,53.7702,42.4699,33.8387,27.1867,21.9868,17.9062,14.6677,12.062,9.96406,8.28246,6.89981,5.78355,4.8731,4.09781,3.46143,2.9337,2.4923,2.13679,1.80616,1.55453,1.12975,0.968853]
	names   = [];
	l_expFull   = [];
	l_exp   = [];
	l_expB   = [];
	l_expR   = [];
	count=0;
	BR=0.57*0.57
	for r in resultsCombFull:
		l_expFull.append(r[3]*xsecs[count]*BR);
		count=count+1
		
	count=0;
	for r in resultsComb:
		names.append(r[0]);
		l_exp.append(r[3]*xsecs[count]*BR);
		count=count+1
		
	count=0;
	for r in resultsBoost:
		l_expB.append(r[3]*xsecs[count]*BR);
		count=count+1
	count=0;
	for r in resultsResolv:
		l_expR.append(r[3]*xsecs[count]*BR);
		count=count+1

	a_xax = array('d', []);
	a_expFull = array('d', []);
	a_exp = array('d', []);
	a_expB = array('d', []);
	a_expR = array('d', []);
	#Need to do this a bit more clever
	for i in range(len(names)): a_xax.append( float(names[i]) );
	#for i in range(len(names)): a2_xax.append( float(names[i]) );
	#for i in range(len(names)-1,-1,-1): a2_xax.append( float(names[i]));
	#for i in range(len(l_obs)): a_obs.append( float(l_obs[i]) );
	for i in range(len(l_exp)): a_expFull.append( float(l_expFull[i]) );
	for i in range(len(l_exp)): a_exp.append( float(l_exp[i]) );
	for i in range(len(l_expB)): a_expB.append( float(l_expB[i]) );
	for i in range(len(l_expR)): a_expR.append( float(l_expR[i]) );
	

	g_expFull = ROOT.TGraph(len(a_xax), a_xax, a_expFull)
	g_exp = ROOT.TGraph(len(a_xax), a_xax, a_exp)
	g_expR = ROOT.TGraph(len(a_xax), a_xax, a_expR)
	g_expB = ROOT.TGraph(len(a_xax), a_xax, a_expB)
	
	can = ROOT.TCanvas("can","can",1800,1200);
	hrl = ROOT.TH1F("hrl","hrl",36,100,1000);
	
	# hrl = can.DrawFrame(0,0,6,15);
	hrl.GetXaxis().SetTitle("Higgsino mass m_{#tilde{#chi}^{0}_{1}} [GeV]");
	hrl.GetXaxis().SetRangeUser(600,1000);
	hrl.GetXaxis().SetLabelSize(0.035)
	hrl.GetXaxis().SetTitleSize(0.04)
	hrl.GetXaxis().SetTitleOffset(1.2)
	hrl.GetYaxis().SetTitle("#sigma_{95% CL} x BR(hh#rightarrow bbbb) [fb] ");
	#hrl.GetYaxis().SetTitleOffset(0.2)
	hrl.GetYaxis().SetTitleSize(0.04)

	#for i in range(0,15):
	#	if i%3==0:
	#		hrl.GetXaxis().SetBinLabel(i+1,names[i])
	#	if i==14:hrl.GetXaxis().SetBinLabel(i+1, names[i])
	#hrl.GetXaxis().SetBinLabel(2,names[1])
	#hrl.GetXaxis().SetBinLabel(3,names[2])
	#hrl.GetXaxis().SetBinLabel(4,names[3])
	#hrl.GetXaxis().SetBinLabel(5,names[4])
	#hrl.GetXaxis().SetBinLabel(6,names[5])
	#hrl.SetMaximum(1000.);
	hrl.Draw();
	#hrl.GetYaxis().SetRangeUser(0.1,1000000);
	hrl.GetYaxis().SetRangeUser(0.1,10);

	#hrl.GetYaxis().SetTitle("UL on #sigma_{95\% CL} ")#x BR(hh#rightarrow bbbb) [fb] ");
	can.SetGrid(); 
	can.SetLogy();
	txta = ROOT.TLatex(0.2,0.95,"CMS");
	txta.SetNDC();
	txtb = ROOT.TLatex(0.27,0.95,"Preliminary");
	txtb.SetNDC(); txtb.SetTextFont(52); txtb.SetTextSize(0.042);
	txtc = ROOT.TLatex(0.75,0.96,"35.9 fb^{-1} (13 TeV)");
	txtc.SetNDC(); txtc.SetTextFont(42); txtc.SetTextSize(0.04);
	txtd = ROOT.TLatex(0.65,0.88,"pp#rightarrow #tilde{#chi}^{1}_{0} #tilde{#chi}^{1}_{0} #rightarrow hh #tilde{G} #tilde{G}");
	txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.05);
	f=open("LatestXsecGluGlu.txt", 'r')
	a_stop = array('d', []);
	a_xsec = array('d', []);
	count=0;
	for x in xsecs:
		a_stop.append(float(names[count]));
		a_xsec.append(x*0.57*0.57)
		count=count+1
	g_xsec=ROOT.TGraph(len(a_stop), a_stop, a_xsec)
	leg = ROOT.TLegend(0.17,0.20,0.6,0.5);
	leg.SetFillStyle(1001);
	leg.SetFillColor(0);    
	leg.SetBorderSize(1);  
  	leg.AddEntry(g_xsec, "Theory cross-section", "l") 
	leg.AddEntry(g_expB,"Exp UL: 2 Boosted H","l")
	leg.AddEntry(g_expR,"Exp UL: 2 Resolv H","l")
	leg.AddEntry(g_exp,"Exp UL: Boost+Resolv","l")
	leg.AddEntry(g_expFull,"Exp UL: Full Combination","l")
	g_expR.SetLineColor(ROOT.kCyan+2);
	g_expB.SetLineColor(ROOT.kRed);
	g_expFull.SetLineStyle(1);
	g_expFull.SetLineWidth(3);
	g_exp.SetLineStyle(2);
	g_exp.SetLineWidth(2);
	g_expR.SetLineStyle(2);
	g_expR.SetLineWidth(2);
	g_expB.SetLineStyle(2);
	g_expB.SetLineWidth(2);
	g_exp.Draw('lsames');
	g_expR.Draw('lsames');
	g_expB.Draw('lsames');
	for i in range(0,100):
		if(g_xsec.Eval(700+i)-g_expR.Eval(700+i)>0):
			print "Mass %d  Exp Excl %g " %(700+i,g_expR.Eval(700+i))
			print "Theory Xsec %g " %g_xsec.Eval(700+i)
	#oneLine.Draw("LSAMES");
	txta.Draw();
	txtb.Draw();
	txtc.Draw();
	txtd.Draw();	
	leg.Draw();
	g_xsec.SetLineStyle(2);
	g_xsec.SetLineWidth(2);
	g_xsec.SetLineColor(ROOT.kBlue);
	g_xsec.Draw("lsame")
        g_expFull.Draw('lsames');
	#can.SaveAs('brazilTChiHHResultsSummary.C');
	can.SaveAs('brazilTChiHHResultsSummary.pdf');
