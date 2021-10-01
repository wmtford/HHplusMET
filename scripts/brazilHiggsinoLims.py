#! /usr/bin/env python
import os
import glob
import math
import array
import sys
import time
import ROOT
from array import array
from ROOT import gROOT
from ROOT import gStyle
gROOT.SetBatch(True)

import tdrstyle
tdrstyle.setTDRStyle()

runBoostedOnly = False #just boosted limits
compareLimits = True #boostedOnly vs resolvedOnly
saveRootFile = False #For HEPdata

idir = "../datacards/";
odir = "../output/";

def columnToList(fn,col):
	f = open(fn,'r');
	olist = [];
	for line in f:
		linelist = line.strip().split()
		olist.append( linelist[col] );
	return olist

def ExtractFile(iname, m1, m2):
	f = ROOT.TFile(iname);
	t = f.Get("limit");
	lims = [];
	lims.append(m1);
	lims.append(m2);
	for i in range(6):
		t.GetEntry(i);
		lims.append( t.limit )
	return lims;

BR = 1.0 # BR = 0.5824
def higgsinoCrossSection(hig_mass):
	if (hig_mass=="127"): xsec = BR*BR*7.6022
	elif (hig_mass =="150"): xsec = BR*BR*3.83231
	elif (hig_mass =="175"): xsec = BR*BR*2.26794
	elif (hig_mass =="200"): xsec = BR*BR*1.33562
	elif (hig_mass =="225"): xsec = BR*BR*0.860597
	elif (hig_mass =="250"): xsec = BR*BR*0.577314
	elif (hig_mass =="275"): xsec = BR*BR*0.400107
	elif (hig_mass =="300"): xsec = BR*BR*0.284855
	elif (hig_mass =="325"): xsec = BR*BR*0.20736
	elif (hig_mass =="350"): xsec = BR*BR*0.153841
	elif (hig_mass =="375"): xsec = BR*BR*0.116006
	elif (hig_mass =="400"): xsec = BR*BR*0.0887325
	elif (hig_mass =="425"): xsec = BR*BR*0.0686963
	elif (hig_mass =="450"): xsec = BR*BR*0.0537702
	elif (hig_mass =="475"): xsec = BR*BR*0.0424699
	elif (hig_mass =="500"): xsec = BR*BR*0.0338387
	elif (hig_mass =="525"): xsec = BR*BR*0.0271867
	elif (hig_mass =="550"): xsec = BR*BR*0.0219868
	elif (hig_mass =="575"): xsec = BR*BR*0.0179062
	elif (hig_mass =="600"): xsec = BR*BR*0.0146677
	elif (hig_mass =="625"): xsec = BR*BR*0.012062
	elif (hig_mass =="650"): xsec = BR*BR*0.00996406
	elif (hig_mass =="675"): xsec = BR*BR*0.00828246
	elif (hig_mass =="700"): xsec = BR*BR*0.00689981
	elif (hig_mass =="725"): xsec = BR*BR*0.00578355
	elif (hig_mass =="750"): xsec = BR*BR*0.0048731
	elif (hig_mass =="775"): xsec = BR*BR*0.00409781
	elif (hig_mass =="800"): xsec = BR*BR*0.00346143
	elif (hig_mass =="825"): xsec = BR*BR*0.0029337
	elif (hig_mass =="850"): xsec = BR*BR*0.0024923
	elif (hig_mass =="875"): xsec = BR*BR*0.00213679
	elif (hig_mass =="900"): xsec = BR*BR*0.00180616
	elif (hig_mass =="925"): xsec = BR*BR*0.00155453
	elif (hig_mass =="950"): xsec = BR*BR*0.00132692
	elif (hig_mass =="975"): xsec = BR*BR*0.00112975
	elif (hig_mass =="1000"): xsec = BR*BR*0.000968853
	elif (hig_mass =="1025"): xsec = BR*BR*0.000840602
	elif (hig_mass =="1050"): xsec = BR*BR*0.000731306
	elif (hig_mass =="1075"): xsec = BR*BR*0.000627083
	elif (hig_mass =="1100"): xsec = BR*BR*0.000538005
	elif (hig_mass =="1125"): xsec = BR*BR*0.00046747
	elif (hig_mass =="1150"): xsec = BR*BR*0.000405108
	elif (hig_mass =="1175"): xsec = BR*BR*0.000348261
	elif (hig_mass =="1200"): xsec = BR*BR*0.000299347
	elif (hig_mass =="1225"): xsec = BR*BR*0.000265935
	elif (hig_mass =="1250"): xsec = BR*BR*0.000240471
	elif (hig_mass =="1275"): xsec = BR*BR*0.000190411
	elif (hig_mass =="1300"): xsec = BR*BR*0.000160765
	elif (hig_mass =="1325"): xsec = BR*BR*0.000136272
	elif (hig_mass =="1350"): xsec = BR*BR*0.000111174
	elif (hig_mass =="1375"): xsec = BR*BR*9.74728e-05
	elif (hig_mass =="1400"): xsec = BR*BR*7.80263e-05
	elif (hig_mass =="1425"): xsec = BR*BR*6.96843e-05
	elif (hig_mass =="1450"): xsec = BR*BR*6.96962e-05
	elif (hig_mass =="1475"): xsec = BR*BR*4.98006e-05
	else:
		xsec = 0
		print("Cross section not found for mass of %s" %(hig_mass))
	return xsec*1000.0

if __name__ == '__main__':
	results = []; results2 = []; results3 = []; results_hino = [];

	#BoostedOnly
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH150_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','150', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH175_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','175', '1') );
	# results.append( ExtractFile(idir+'higgsCombine1DTChiHH200_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','200', '1') );
	# results.append( ExtractFile(idir+'higgsCombine1DTChiHH225_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','225', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH250_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','250', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH275_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','275', '1') );
	# results.append( ExtractFile(idir+'higgsCombine1DTChiHH300_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','300', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH325_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','325', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH350_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','350', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH375_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','375', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH400_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','400', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH425_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','425', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH450_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','450', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH475_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','475', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH500_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','500', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH525_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','525', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH550_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','550', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH575_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','575', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH600_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','600', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH625_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','625', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH650_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','650', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH675_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','675', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH700_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','700', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH725_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','725', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH750_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','750', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH775_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','775', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH800_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','800', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH825_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','825', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH850_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','850', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH875_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','875', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH900_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','900', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH925_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','925', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH950_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','950', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH975_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','975', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH1000_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1000', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH1100_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1100', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH1200_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1200', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH1300_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1300', '1') );
	results.append( ExtractFile(idir+'higgsCombine1DTChiHH1400_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1400', '1') );

	# ResolvedOnly
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH150_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','150', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH175_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','175', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH200_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','200', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH225_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','225', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH250_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','250', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH275_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','275', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH300_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','300', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH325_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','325', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH350_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','350', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH375_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','375', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH400_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','400', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH425_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','425', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH450_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','450', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH475_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','475', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH500_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','500', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH525_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','525', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH550_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','550', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH575_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','575', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH600_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','600', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH625_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','625', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH650_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','650', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH675_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','675', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH700_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','700', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH725_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','725', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH750_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','750', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH775_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','775', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH800_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','800', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH825_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','825', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH850_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','850', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH875_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','875', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH900_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','900', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH925_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','925', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH950_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','950', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH975_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','975', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH1000_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1000', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH1100_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1100', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH1200_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1200', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH1300_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1300', '1') );
	results2.append( ExtractFile(idir+'higgsCombine1DTChiHH1400_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1400', '1') );


	# Combo (ResolvedOnly+BoostedVeto)
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH150_LSP1_Data_Combo.AsymptoticLimits.mH120.root','150', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH175_LSP1_Data_Combo.AsymptoticLimits.mH120.root','175', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH200_LSP1_Data_Combo.AsymptoticLimits.mH120.root','200', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH225_LSP1_Data_Combo.AsymptoticLimits.mH120.root','225', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH250_LSP1_Data_Combo.AsymptoticLimits.mH120.root','250', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH275_LSP1_Data_Combo.AsymptoticLimits.mH120.root','275', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH300_LSP1_Data_Combo.AsymptoticLimits.mH120.root','300', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH325_LSP1_Data_Combo.AsymptoticLimits.mH120.root','325', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH350_LSP1_Data_Combo.AsymptoticLimits.mH120.root','350', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH375_LSP1_Data_Combo.AsymptoticLimits.mH120.root','375', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH400_LSP1_Data_Combo.AsymptoticLimits.mH120.root','400', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH425_LSP1_Data_Combo.AsymptoticLimits.mH120.root','425', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH450_LSP1_Data_Combo.AsymptoticLimits.mH120.root','450', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH475_LSP1_Data_Combo.AsymptoticLimits.mH120.root','475', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH500_LSP1_Data_Combo.AsymptoticLimits.mH120.root','500', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH525_LSP1_Data_Combo.AsymptoticLimits.mH120.root','525', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH550_LSP1_Data_Combo.AsymptoticLimits.mH120.root','550', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH575_LSP1_Data_Combo.AsymptoticLimits.mH120.root','575', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH600_LSP1_Data_Combo.AsymptoticLimits.mH120.root','600', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH625_LSP1_Data_Combo.AsymptoticLimits.mH120.root','625', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH650_LSP1_Data_Combo.AsymptoticLimits.mH120.root','650', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH675_LSP1_Data_Combo.AsymptoticLimits.mH120.root','675', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH700_LSP1_Data_Combo.AsymptoticLimits.mH120.root','700', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH725_LSP1_Data_Combo.AsymptoticLimits.mH120.root','725', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH750_LSP1_Data_Combo.AsymptoticLimits.mH120.root','750', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH775_LSP1_Data_Combo.AsymptoticLimits.mH120.root','775', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH800_LSP1_Data_Combo.AsymptoticLimits.mH120.root','800', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH825_LSP1_Data_Combo.AsymptoticLimits.mH120.root','825', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH850_LSP1_Data_Combo.AsymptoticLimits.mH120.root','850', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH875_LSP1_Data_Combo.AsymptoticLimits.mH120.root','875', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH900_LSP1_Data_Combo.AsymptoticLimits.mH120.root','900', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH925_LSP1_Data_Combo.AsymptoticLimits.mH120.root','925', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH950_LSP1_Data_Combo.AsymptoticLimits.mH120.root','950', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH975_LSP1_Data_Combo.AsymptoticLimits.mH120.root','975', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH1000_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1000', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH1100_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1100', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH1200_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1200', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH1300_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1300', '1') );
	results3.append( ExtractFile(idir+'higgsCombine1DTChiHH1400_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1400', '1') );


	# results_hino=results3
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH150_LSP1_Data_Combo.AsymptoticLimits.mH120.root','150', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH200_LSP1_Data_Combo.AsymptoticLimits.mH120.root','200', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH250_LSP1_Data_Combo.AsymptoticLimits.mH120.root','250', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH300_LSP1_Data_Combo.AsymptoticLimits.mH120.root','300', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH350_LSP1_Data_Combo.AsymptoticLimits.mH120.root','350', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH400_LSP1_Data_Combo.AsymptoticLimits.mH120.root','400', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH450_LSP1_Data_Combo.AsymptoticLimits.mH120.root','450', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH500_LSP1_Data_Combo.AsymptoticLimits.mH120.root','500', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH550_LSP1_Data_Combo.AsymptoticLimits.mH120.root','550', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH600_LSP1_Data_Combo.AsymptoticLimits.mH120.root','600', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH650_LSP1_Data_Combo.AsymptoticLimits.mH120.root','650', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH700_LSP1_Data_Combo.AsymptoticLimits.mH120.root','700', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH750_LSP1_Data_Combo.AsymptoticLimits.mH120.root','750', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH800_LSP1_Data_Combo.AsymptoticLimits.mH120.root','800', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH850_LSP1_Data_Combo.AsymptoticLimits.mH120.root','850', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH900_LSP1_Data_Combo.AsymptoticLimits.mH120.root','900', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH950_LSP1_Data_Combo.AsymptoticLimits.mH120.root','950', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH1000_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1000', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH1100_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1100', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH1200_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1200', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH1300_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1300', '1') );
	results_hino.append( ExtractFile(idir+'higgsCombine1DTChiHH1400_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1400', '1') );

	#N2N1, 200-1200 (every 50 until 1000, then 1100,1200,1300,1400)
	xsec_hino=[715.14,244.213,104.252, 50.9994, 27.3286, 15.6691, 9.44017, 5.90757, 3.8167, 2.53015, 1.71418, 1.18113, 0.826366, 0.586211, 0.420556, 0.305935, 0.22285, 0.16428,0.0912469,0.0516263,0.0299353,0.0175031] #this doesn't include the 25 GeV and 75 GeV mass points

	names=[]; l_obs=[]; l_m2sig=[]; l_m1sig=[]; l_exp=[]; l_p1sig=[]; l_p2sig=[]; count=0;
	names2=[]; l_obs2=[]; l_m2sig2=[]; l_m1sig2=[]; l_exp2=[]; l_p1sig2=[]; l_p2sig2=[]; count2=0;
	names3=[]; l_obs3=[]; l_m2sig3=[]; l_m1sig3=[]; l_exp3=[]; l_p1sig3=[]; l_p2sig3=[]; count3=0;
	names4=[]; l_obs4=[]; l_m2sig4=[]; l_m1sig4=[]; l_exp4=[]; l_p1sig4=[]; l_p2sig4=[]; count4=0;
	LSP_m=[];

	names_hino=[]; count_hino=0; xsecs = []
	# BR=1.0 #BR set in ALPHABET

	for r in results:
		names.append(r[0]);#chi20 mass
		LSP_m.append(r[1]);#LSP mass
		this_xsec = higgsinoCrossSection(r[0]);
		xsecs.append(this_xsec);
		l_m2sig.append(r[2]*this_xsec);
		l_m1sig.append(r[3]*this_xsec);
		l_exp.append(r[4]*this_xsec);
		l_p1sig.append(r[5]*this_xsec);
		l_p2sig.append(r[6]*this_xsec);
		l_obs.append(r[7]*this_xsec);
		count=count+1

	for r in results2:
		names2.append(r[0]);#chi20 mass
		this_xsec = higgsinoCrossSection(r[0]);
		l_m2sig2.append(r[2]*this_xsec);
		l_m1sig2.append(r[3]*this_xsec);
		l_exp2.append(r[4]*this_xsec);
		l_p1sig2.append(r[5]*this_xsec);
		l_p2sig2.append(r[6]*this_xsec);
		l_obs2.append(r[7]*this_xsec);
		count2=count2+1

	for r in results3:
		names3.append(r[0]);#chi20 mass
		this_xsec = higgsinoCrossSection(r[0]);
		l_m2sig3.append(r[2]*this_xsec);
		l_m1sig3.append(r[3]*this_xsec);
		l_exp3.append(r[4]*this_xsec);
		l_p1sig3.append(r[5]*this_xsec);
		l_p2sig3.append(r[6]*this_xsec);
		l_obs3.append(r[7]*this_xsec);
		count3=count3+1

	for r in results_hino:
		names_hino.append(r[0]);
		count_hino=count_hino+1

	a_xax = array('d', []); a2_xax = array('d', []);
	a_exp = array('d', []); a_obs = array('d', []);
	a_1sig = array('d', []); a_2sig = array('d', []);

	a_xax2 = array('d', []); a_exp2 = array('d', []); a_obs2 = array('d', []); a_1sig2 = array('d', []); a_2sig2 = array('d', []);
	a_xax3 = array('d', []); a_exp3 = array('d', []); a_obs3 = array('d', []); a_1sig3 = array('d', []); a_2sig3 = array('d', []);
	aComb_1sig = array('d', []); aComb_2sig = array('d', []); a2Comb_xax = array('d', []);

	a_1sigup = array('d', []); a_1sigdown = array('d', []); a_2sigup = array('d', []); a_2sigdown = array('d', []);
	a_1sig2up = array('d', []); a_1sig2down = array('d', []); a_2sig2up = array('d', []); a_2sig2down = array('d', []);
	a_1sig3up = array('d', []); a_1sig3down = array('d', []); a_2sig3up = array('d', []); a_2sig3down = array('d', []);
	aComb_1sigup = array('d', []); aComb_1sigdown = array('d', []); aComb_2sigup = array('d', []); aComb_2sigdown = array('d', []);

	for i in range(len(names)): a_xax.append( float(names[i]) );
	for i in range(len(names)): a2_xax.append( float(names[i]) );
	for i in range(len(names)-1,-1,-1): a2_xax.append( float(names[i]));
	for i in range(len(l_obs)): a_obs.append( float(l_obs[i]) );
	for i in range(len(l_exp)): a_exp.append( float(l_exp[i]) );
	for i in range(len(l_m2sig)): a_2sig.append( float(l_m2sig[i]) );
	for i in range(len(l_p2sig)-1,-1,-1): a_2sig.append( float(l_p2sig[i]) );
	for i in range(len(l_m1sig)): a_1sig.append( float(l_m1sig[i]) );
	for i in range(len(l_p1sig)-1,-1,-1): a_1sig.append( float(l_p1sig[i]) );

	for i in range(len(l_m2sig)): a_2sigdown.append( float(l_m2sig[i]) );
	for i in range(len(l_p2sig)): a_2sigup.append( float(l_p2sig[i]) );
	for i in range(len(l_m1sig)): a_1sigdown.append( float(l_m1sig[i]) );
	for i in range(len(l_p1sig)): a_1sigup.append( float(l_p1sig[i]) );

	for i in range(len(names2)): a_xax2.append( float(names2[i]) );
	for i in range(len(l_exp2)): a_exp2.append( float(l_exp2[i]) );
	for i in range(len(l_obs2)): a_obs2.append( float(l_obs2[i]) );

	for i in range(len(l_m2sig2)): a_2sig2.append( float(l_m2sig2[i]) );
	for i in range(len(l_p2sig2)-1,-1,-1): a_2sig2.append( float(l_p2sig2[i]) );
	for i in range(len(l_m1sig2)): a_1sig2.append( float(l_m1sig2[i]) );
	for i in range(len(l_p1sig2)-1,-1,-1): a_1sig2.append( float(l_p1sig2[i]) );

	for i in range(len(l_m2sig2)): a_2sig2down.append( float(l_m2sig2[i]) );
	for i in range(len(l_p2sig2)): a_2sig2up.append( float(l_p2sig2[i]) );
	for i in range(len(l_m1sig2)): a_1sig2down.append( float(l_m1sig2[i]) );
	for i in range(len(l_p1sig2)): a_1sig2up.append( float(l_p1sig2[i]) );

	for i in range(len(names3)): a_xax3.append( float(names3[i]) );
	for i in range(len(l_exp3)): a_exp3.append( float(l_exp3[i]) );
	for i in range(len(l_obs3)): a_obs3.append( float(l_obs3[i]) );

	for i in range(len(l_m2sig3)): aComb_2sig.append( float(l_m2sig3[i]) );
	for i in range(len(l_p2sig2)-1,-1,-1): aComb_2sig.append( float(l_p2sig3[i]) );
	for i in range(len(l_m1sig3)): aComb_1sig.append( float(l_m1sig3[i]) );
	for i in range(len(l_p1sig3)-1,-1,-1): aComb_1sig.append( float(l_p1sig3[i]) );
	for i in range(len(names3)): a2Comb_xax.append( float(names3[i]) );
	for i in range(len(names3)-1,-1,-1): a2Comb_xax.append( float(names3[i]));

	for i in range(len(l_m2sig3)): aComb_2sigdown.append( float(l_m2sig3[i]) );
	for i in range(len(l_p2sig2)): aComb_2sigup.append( float(l_p2sig3[i]) );
	for i in range(len(l_m1sig3)): aComb_1sigdown.append( float(l_m1sig3[i]) );
	for i in range(len(l_p1sig3)): aComb_1sigup.append( float(l_p1sig3[i]) );

	a_2sig.append(results[0][6])
	a2_xax.append(0.5)

	g_exp = ROOT.TGraph(len(a_xax), a_xax, a_exp)
	g_obs = ROOT.TGraph(len(a_xax), a_xax, a_obs)
	g_1sig = ROOT.TGraph(len(2*a_xax), a2_xax, a_1sig)
	g_2sig = ROOT.TGraph(len(2*a_xax), a2_xax, a_2sig)
	g_1sigup = ROOT.TGraph(len(a_xax), a_xax, a_1sigup)
	g_1sigdown = ROOT.TGraph(len(a_xax), a_xax, a_1sigdown)
	g_2sigup = ROOT.TGraph(len(a_xax), a_xax, a_2sigup)
	g_2sigdown = ROOT.TGraph(len(a_xax), a_xax, a_2sigdown)

	g_exp2 = ROOT.TGraph(len(a_xax2), a_xax2, a_exp2);
	g_obs2 = ROOT.TGraph(len(a_xax2), a_xax2, a_obs2)
	g_1sig2 = ROOT.TGraph(len(2*a_xax2), a2_xax, a_1sig2)
	g_2sig2 = ROOT.TGraph(len(2*a_xax2), a2_xax, a_2sig2)
	g_1sig2up = ROOT.TGraph(len(a_xax2), a_xax, a_1sig2up)
	g_1sig2down = ROOT.TGraph(len(a_xax2), a_xax, a_1sig2down)
	g_2sig2up = ROOT.TGraph(len(a_xax2), a_xax, a_2sig2up)
	g_2sig2down = ROOT.TGraph(len(a_xax2), a_xax, a_2sig2down)

	g_exp3 = ROOT.TGraph(len(a_xax3), a_xax3, a_exp3);
	g_obs3 = ROOT.TGraph(len(a_xax3), a_xax3, a_obs3)
	gComb_1sig = ROOT.TGraph(len(2*a_xax3), a2Comb_xax, aComb_1sig)
	gComb_2sig = ROOT.TGraph(len(2*a_xax3), a2Comb_xax, aComb_2sig)
	gComb_1sigup = ROOT.TGraph(len(a_xax3), a_xax3, aComb_1sigup)
	gComb_1sigdown = ROOT.TGraph(len(a_xax3), a_xax3, aComb_1sigdown)
	gComb_2sigup = ROOT.TGraph(len(a_xax3), a_xax3, aComb_2sigup)
	gComb_2sigdown = ROOT.TGraph(len(a_xax3), a_xax3, aComb_2sigdown)


	can = ROOT.TCanvas("can","can",1600,1200);
	hrl = ROOT.TH1F("hrl","hrl",25,100,1200);
	chi02 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0 }}}#kern[-1.0]{#scale[0.85]{_{2 }}}";
	chi01 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0 }}}#kern[-1.15]{#scale[0.85]{_{1 }}}";
	chi02nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.9]{#scale[0.85]{_{2}}}";
	chi01nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0#scale[0.2]{ }}}}#kern[-1.25]{#scale[0.85]{_{1#scale[0.2]{ }}}}";
	chipm1nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.05]{#scale[0.85]{^{#pm}}}#kern[-1.4]{#scale[0.85]{_{1}}}";
	chipm1 = "#lower[-0.12]{#tilde{#chi}}#lower[0.00]{#scale[0.85]{^{#pm }}}#kern[-1.3]{#scale[0.85]{_{1 }}}";
	chimp1 = "#lower[-0.12]{#tilde{#chi}}#lower[0.00]{#scale[0.85]{^{#mp }}}#kern[-1.3]{#scale[0.85]{_{1 }}}";
	chi0pmi = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0,#pm  }}}#kern[-1.1]{#scale[0.85]{_{ i    }}}";
	chi0mpj = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0,#mp }}}#kern[-1.1]{#scale[0.85]{_{j   }}}";

	hrl.GetXaxis().SetTitle("m("+chi01nospace+") [GeV]");
	hrl.GetXaxis().SetRangeUser(150,1400);
	hrl.GetXaxis().SetLabelSize(0.04)
	hrl.GetXaxis().SetTitleSize(0.053)
	hrl.GetXaxis().SetTitleOffset(1.0)
	hrl.GetYaxis().SetTitle("Cross section [fb] ");
	hrl.GetYaxis().SetLabelSize(0.04)
	hrl.GetYaxis().SetTitleSize(0.053)
	hrl.GetYaxis().SetTitleOffset(0.90)

	hrl.Draw();
	hrl.GetYaxis().SetRangeUser(0.01,1000000);
	if compareLimits: hrl.GetYaxis().SetRangeUser(0.01,10000000);
	can.SetLeftMargin( 0.1 ); can.SetRightMargin( 0.04 );
	can.SetTopMargin( 0.06 ); can.SetBottomMargin( 0.13 );
	can.SetLogy();

	txta = ROOT.TLatex(0.16,0.86,"CMS");
	txta.SetNDC(); txta.SetTextSize(0.07);
	if compareLimits: txtb = ROOT.TLatex(0.16,0.805,"Preliminary");
	else: txtb = ROOT.TLatex(0.16,0.82,"Preliminary");
	txtb.SetNDC(); txtb.SetTextFont(52); txtb.SetTextSize(0.042);
	txtc = ROOT.TLatex(0.7,0.95,"137 fb^{-1} (13 TeV)");
	txtc.SetNDC(); txtc.SetTextFont(42); txtc.SetTextSize(0.05);
	txtd = ROOT.TLatex(0.31,0.85,"pp #rightarrow "+chi0pmi+chi0mpj+"#rightarrow "+chi01+chi01+" + X_{soft} #rightarrow HH#tilde{G}#tilde{G} + X_{soft}");
	txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.046);
	txte = ROOT.TLatex(0.395,0.79,"m("+chi02nospace+") = m("+chipm1nospace+") = m("+chi01nospace+"), m(#tilde{G}) = 1 GeV");
	txte.SetNDC(); txte.SetTextFont(42); txte.SetTextSize(0.046);

	a_stop = array('d', []); a_xsec = array('d', []);
	count=0;
	for x in xsecs:
		a_stop.append(float(names[count]));
		a_xsec.append(x)
		count=count+1
	g_xsec=ROOT.TGraph(len(a_stop), a_stop, a_xsec)

	a_stop_hino = array('d', []); a_xsec_hino = array('d', []);
	count_hino=0;
	for x in xsec_hino:
		a_stop_hino.append(float(names_hino[count_hino]));
		a_xsec_hino.append(x*BR*BR)
		count_hino=count_hino+1
	g_xsec_hino=ROOT.TGraph(len(a_stop_hino), a_stop_hino, a_xsec_hino)

	if compareLimits: leg = ROOT.TLegend(0.55,0.47,0.8,0.73);
	else: leg = ROOT.TLegend(0.6,0.47,0.85,0.73);
	leg.SetFillStyle(4050); leg.SetFillColor(0);
	leg.SetTextFont(42); leg.SetBorderSize(0);
	leg.SetTextSize(0.045);

	#Boosted only
	if runBoostedOnly:
		leg.AddEntry(g_exp, "Expected","l")
		leg.AddEntry(g_obs,"Observed","l")
		leg.AddEntry(g_2sig,"Expected 2#sigma","f")
		leg.AddEntry(g_1sig,"Expected 1#sigma","f")

	#Compare limits
	elif compareLimits:
		leg.SetHeader("95% CL Upper Limit")
		leg.AddEntry(g_obs, "Observed (boosted)","l")
		leg.AddEntry(g_obs2,"Observed (resolved)","l")
		leg.AddEntry(g_exp,"Expected (boosted)","l")
		leg.AddEntry(g_exp2,"Expected (resolved)","l")

	#Combo
	else:
		leg.SetHeader("95% CL Upper Limit")
		leg.AddEntry(g_obs3,"Observed","l")
		leg.AddEntry(g_exp3, "Expected","l")
		leg.AddEntry(gComb_2sig,"#pm2 #sigma_{experiment}","f")
		leg.AddEntry(gComb_1sig,"#pm1 #sigma_{experiment}","f")


	leg2 = ROOT.TLegend(0.15,0.16,0.4,0.3);
	leg2.SetFillStyle(4050); leg2.SetFillColor(0);
	leg2.SetTextFont(42); leg2.SetBorderSize(0);
	leg2.SetTextSize(0.045);

	plusString = "#scale[0.1]{ }+#scale[0.5]{ }"
  	leg2.AddEntry(g_xsec, "Theory ("+chi01+chi02+plusString+chi01+chipm1+plusString+chi02+chipm1+plusString+chipm1+chimp1+")", "l")
	if not compareLimits: leg2.AddEntry(g_xsec_hino, "Theory ("+chi01+chi02nospace+")", "l")

	g_1sig.SetFillColor(ROOT.kGreen+1); g_1sig.SetFillStyle(3244);
	g_2sig.SetFillColor(ROOT.kOrange); g_2sig.SetFillStyle(3244);
	gComb_1sig.SetFillColor(ROOT.kGreen+1); gComb_1sig.SetFillStyle(3244);
	gComb_2sig.SetFillColor(ROOT.kOrange); gComb_2sig.SetFillStyle(3244);

	g_exp.SetLineStyle(2); g_exp.SetLineColor(ROOT.kBlack);
	g_exp.SetLineWidth(3); g_obs.SetLineWidth(2);
	g_exp3.SetLineStyle(2); g_exp3.SetLineColor(ROOT.kBlack);
	g_exp3.SetLineWidth(3); g_obs3.SetLineWidth(2);

	if runBoostedOnly:
		g_2sig.Draw('f');
		g_1sig.Draw('fsames');
		g_obs.Draw('lsames');
		g_exp.Draw('lsames');

	elif compareLimits:
		g_obs.SetLineColor(ROOT.kRed); g_obs.SetLineWidth(2);
		g_obs2.SetLineColor(ROOT.kBlue); g_obs2.SetLineWidth(2);
		g_obs3.SetLineColor(ROOT.kBlack); g_obs3.SetLineWidth(3);
		g_obs.Draw('l');
		g_obs2.Draw('lsames');
		g_exp.SetLineColor(ROOT.kRed); g_exp.SetLineWidth(2); #g_exp.SetLineStyle(1);
		g_exp2.SetLineColor(ROOT.kBlue); g_exp2.SetLineWidth(2); g_exp2.SetLineStyle(2);
		g_exp3.SetLineColor(ROOT.kBlack); g_exp3.SetLineWidth(2); g_exp3.SetLineStyle(2);
		g_exp.Draw('lsames');
		g_exp2.Draw('lsames');

	# For one line with 1- and 2-sigma bands, combo
	else:
		gComb_2sig.Draw('f');
		gComb_1sig.Draw('fsames');
		g_obs3.Draw('lsames');
		g_exp3.Draw('lsames');

	txta.Draw(); txtc.Draw();
	txtd.Draw(); txte.Draw();
	leg.Draw(); leg2.Draw();
	g_xsec.SetLineStyle(2); g_xsec.SetLineWidth(3);
	g_xsec.SetLineColor(ROOT.kRed);
	if compareLimits and not runBoostedOnly: g_xsec.SetLineColor(ROOT.kBlack);
	g_xsec.Draw("lsame")

	#Second xsec (https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SUSYCrossSections13TeVn1n2hino)
	g_xsec_hino.SetLineStyle(3);
	g_xsec_hino.SetLineWidth(3);
	g_xsec_hino.SetLineColor(ROOT.kMagenta);
	if not compareLimits: g_xsec_hino.Draw("lsame")

	if runBoostedOnly: can.SaveAs(odir+'TChiHHResults_V18_BoostedOnly_Data.pdf');
	elif compareLimits:
		can.SaveAs(odir+'TChiHHResults_V18_Combo_CompareObsExpAll.pdf');
		txtb.Draw();
		can.SaveAs(odir+'TChiHHResults_V18_Combo_CompareObsExpAll_prelim.pdf');
	else:
		can.SaveAs(odir+'TChiHHResults_V18_Combo_ObsExp.pdf');
		txtb.Draw();
		can.SaveAs(odir+'TChiHHResults_V18_Combo_ObsExp_prelim.pdf');


	####### Save ROOT file with all limits
	if saveRootFile:
		fNEW=ROOT.TFile(odir+"CMS-SUS-20-004_TChiHH1DLimits.root", "RECREATE")
		can2 = ROOT.TCanvas("can2","can2",1600,1200);
		g_xsec.Write("TheoryXSec");
		g_xsec_hino.Write("TheoryXSec_N1N2");
		g_exp2.Write("ExpectedLimit_Resolved");
		g_exp.Write("ExpectedLimit_Boosted");
		g_exp3.Write("ExpectedLimit_Combo");

		g_1sig2up.Write("ExpectedLimit1SigmaUp_Resolved");
		g_1sig2down.Write("ExpectedLimit1SigmaDown_Resolved");
		g_1sigup.Write("ExpectedLimit1SigmaUp_Boosted");
		g_1sigdown.Write("ExpectedLimit1SigmaDown_Boosted");
		gComb_1sigup.Write("ExpectedLimit1SigmaUp_Combo");
		gComb_1sigdown.Write("ExpectedLimit1SigmaDown_Combo");

		g_2sig2up.Write("ExpectedLimit2SigmaUp_Resolved");
		g_2sig2down.Write("ExpectedLimit2SigmaDown_Resolved");
		g_2sigup.Write("ExpectedLimit2SigmaUp_Boosted");
		g_2sigdown.Write("ExpectedLimit2SigmaDown_Boosted");
		gComb_2sigup.Write("ExpectedLimit2SigmaUp_Combo");
		gComb_2sigdown.Write("ExpectedLimit2SigmaDown_Combo");

		g_obs2.Write("ObservedLimit_Resolved");
		g_obs.Write("ObservedLimit_Boosted");
		g_obs3.Write("ObservedLimit_Combo");

		fNEW.Close();
