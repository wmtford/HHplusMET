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
	results = [];
	#results.append( ExtractFile(idir+'/results_T2tt_175_1_mu0.0.root','175') );
	#results.append( ExtractFile(idir+'/higgsCombinetestCards-allBkgs-TChiHH_127_1-35.9-mu0.0.AsymptoticLimits.mH120.root','127') );
	#results.append( ExtractFile(idir+'/higgsCombinetestCards-allBkgs-TChiHH_150_1-35.9-mu0.0.AsymptoticLimits.mH120.root','150') );
	#results.append( ExtractFile(idir+'/higgsCombinetestCards-allBkgs-TChiHH_175_1-35.9-mu0.0.AsymptoticLimits.mH120.root','175') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH200.AsymptoticLimits.mH200.root','200') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH225.AsymptoticLimits.mH225.root','225') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH250.AsymptoticLimits.mH250.root','250') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH275.AsymptoticLimits.mH275.root','275') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH300.AsymptoticLimits.mH300.root','300') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH325.AsymptoticLimits.mH325.root','325') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH350.AsymptoticLimits.mH350.root','350') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH375.AsymptoticLimits.mH375.root','375') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH400.AsymptoticLimits.mH400.root','400') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH425.AsymptoticLimits.mH425.root','425') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH450.AsymptoticLimits.mH450.root','450') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH475.AsymptoticLimits.mH475.root','475') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH500.AsymptoticLimits.mH500.root','500') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH525.AsymptoticLimits.mH525.root','525') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH550.AsymptoticLimits.mH550.root','550') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH575.AsymptoticLimits.mH575.root','575') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH600.AsymptoticLimits.mH600.root','600') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH625.AsymptoticLimits.mH625.root','625') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH650.AsymptoticLimits.mH650.root','650') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH675.AsymptoticLimits.mH675.root','675') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH700.AsymptoticLimits.mH700.root','700') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH725.AsymptoticLimits.mH725.root','725') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH750.AsymptoticLimits.mH750.root','750') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH775.AsymptoticLimits.mH775.root','775') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH800.AsymptoticLimits.mH800.root','800') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH825.AsymptoticLimits.mH825.root','825') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH850.AsymptoticLimits.mH850.root','850') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH875.AsymptoticLimits.mH875.root','875') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH900.AsymptoticLimits.mH900.root','900') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH925.AsymptoticLimits.mH925.root','925') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH950.AsymptoticLimits.mH950.root','950') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH975.AsymptoticLimits.mH975.root','975') );
	results.append( ExtractFile(idir+'/higgsCombineFullCombinationTChiHH1000.AsymptoticLimits.mH1000.root','1000') );
	#xsecs=[7602.2,3832.31,2267.94,1335.62, 860.597,577.314,400.107,284.855,207.36,153.841,116.006,88.7325,68.6963,53.7702,42.4699,33.8387,27.1867,21.9868,17.9062,14.6677,12.062,9.96406,8.28246,6.89981,5.78355,4.8731,4.09781,3.46143,2.9337,2.4923,2.13679,1.80616,1.55453,1.32692,1.12975,0.968853]
	#xsecs=[1335.62,860.597,577.314,400.107,284.855,207.36,153.841,116.006,88.7325,68.6963,53.7702,42.4699,33.8387,27.1867,21.9868,17.9062,14.6677,12.062,9.96406,8.28246,6.89981,5.78355,4.8731,4.09781,3.46143,2.9337,2.4923,2.13679,1.80616,1.55453,1.12975,0.968853]
	xsecs=[1335.62,860.597,577.314,400.107,284.855,207.36,153.841,116.006,88.7325,68.6963,53.7702,42.4699,33.8387,27.1867,21.9868,17.9062,14.6677,12.062,9.96406,8.28246,6.89981,5.78355,4.8731,4.09781,3.46143,2.9337,2.4923,2.13679,1.80616,1.55453,1.32692,1.12975,0.968853]
	names   = [];
	l_obs   = [];
	l_m2sig = [];
	l_m1sig = [];
	l_exp   = [];
	l_p1sig = [];
	l_p2sig = [];
	count=0;
	BR=0.57*0.57
	for r in results:
		names.append(r[0]);
		l_m2sig.append(r[1]*xsecs[count]*BR);
		l_m1sig.append(r[2]*xsecs[count]*BR);
		l_exp.append(r[3]*xsecs[count]*BR);
		l_p1sig.append(r[4]*xsecs[count]*BR);
		l_p2sig.append(r[5]*xsecs[count]*BR);
		l_obs.append(r[6]*xsecs[count]*BR);
		count=count+1
	print "l_exp = ", l_exp
	print "l_obs = ", l_obs

	a_xax = array('d', []);
	a2_xax = array('d', []);
	a_exp = array('d', []);
	a_obs = array('d', []);
	a_1sig = array('d', []);
	a_2sig = array('d', []);
	#Need to do this a bit more clever
	for i in range(len(names)): a_xax.append( float(names[i]) );
	for i in range(len(names)): a2_xax.append( float(names[i]) );
	for i in range(len(names)-1,-1,-1): a2_xax.append( float(names[i]));
	for i in range(len(l_obs)): a_obs.append( float(l_obs[i]) );
	for i in range(len(l_exp)): a_exp.append( float(l_exp[i]) );

	for i in range(len(l_m2sig)): a_2sig.append( float(l_m2sig[i]) );
	for i in range(len(l_p2sig)-1,-1,-1): a_2sig.append( float(l_p2sig[i]) );

	for i in range(len(l_m1sig)): a_1sig.append( float(l_m1sig[i]) );
	for i in range(len(l_p1sig)-1,-1,-1): a_1sig.append( float(l_p1sig[i]) );
	#print a_2sig, len(a_2sig)
	#print a2_xax, len(a2_xax)
	a_2sig.append(results[0][6])
	a2_xax.append(0.5)

	g_exp = ROOT.TGraph(len(a_xax), a_xax, a_exp)
	g_obs = ROOT.TGraph(len(a_xax), a_xax, a_obs)
	g_1sig = ROOT.TGraph(len(2*a_xax), a2_xax, a_1sig)
	g_2sig = ROOT.TGraph(len(2*a_xax), a2_xax, a_2sig)

	#print g_2sig;

	can = ROOT.TCanvas("can","can",1800,1200);
	hrl = ROOT.TH1F("hrl","hrl",36,100,1000);

	# hrl = can.DrawFrame(0,0,6,15);
	hrl.GetXaxis().SetTitle("Higgsino mass m_{#tilde{#chi}^{0}_{1}} [GeV]");
	hrl.GetXaxis().SetRangeUser(200,1000);
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
	hrl.GetYaxis().SetRangeUser(0.1,1000000);

	#hrl.GetYaxis().SetTitle("UL on #sigma_{95\% CL} ")#x BR(hh#rightarrow bbbb) [fb] ");
	can.SetGrid();
	can.SetLogy();
	txta = ROOT.TLatex(0.2,0.95,"CMS");
	txta.SetNDC();
	txtb = ROOT.TLatex(0.27,0.95,"Preliminary");
	txtb.SetNDC(); txtb.SetTextFont(52); txtb.SetTextSize(0.042);
	txtc = ROOT.TLatex(0.75,0.96,"35.9 fb^{-1} (13 TeV)");
	#txtc = ROOT.TLatex(0.75,0.96,"150 fb^{-1} (13 TeV)");
	txtc.SetNDC(); txtc.SetTextFont(42); txtc.SetTextSize(0.04);
	txtd = ROOT.TLatex(0.65,0.88,"pp#rightarrow #tilde{#chi}^{1}_{0} #tilde{#chi}^{1}_{0} #rightarrow hh #tilde{G} #tilde{G}");
	txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.05);
	#f=open("LatestXsecGluGlu.txt", 'r')
	a_stop = array('d', []);
	a_xsec = array('d', []);
	count=0;
	for x in xsecs:
		a_stop.append(float(names[count]));
		a_xsec.append(x*0.57*0.57)
		count=count+1
	g_xsec=ROOT.TGraph(len(a_stop), a_stop, a_xsec)
	leg = ROOT.TLegend(0.65,0.60,0.9,0.8);
	leg.SetFillStyle(1001);
	leg.SetFillColor(0);
	leg.SetBorderSize(1);
	# leg.SetNColumns(2);
	leg.AddEntry(g_exp,"expected","l")
	leg.AddEntry(g_obs,"observed","l")
	leg.AddEntry(g_2sig,"expected 2#sigma","f")
	leg.AddEntry(g_1sig,"expected 1#sigma","f")
  	leg.AddEntry(g_xsec, "Theory cross-section", "l")
	#oneLine = ROOT.TF1("oneLine","1",175,550);
	#oneLine.SetLineColor(ROOT.kRed+2);
	#oneLine.SetLineWidth(2);
	#oneLine.SetLineStyle(1);

	g_1sig.SetFillColor(ROOT.kGreen);
	g_1sig.SetFillStyle(3244);
	g_2sig.SetFillColor(ROOT.kYellow);
	g_2sig.SetFillStyle(3244);
	g_exp.SetLineStyle(2);
	g_exp.SetLineWidth(2);
	g_obs.SetLineWidth(2);
	g_2sig.Draw('f');
	g_1sig.Draw('fsames');
	#g_1sig.Draw('f');
	#g_obs.Draw('lsames');
	g_exp.Draw('lsames');
	for i in range(0,100):
		if(g_xsec.Eval(800+i)-g_exp.Eval(800+i)>0):
			print "Mass %d  Exp Excl %g " %(800+i,g_exp.Eval(800+i))
			print "Theory Xsec %g " %g_xsec.Eval(800+i)
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
	can.SaveAs('brazilTChiHHResults.pdf');
