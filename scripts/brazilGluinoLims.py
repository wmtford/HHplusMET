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
gROOT.SetBatch(True)

import tdrstyle
tdrstyle.setTDRStyle()

idir = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/myVersion/CMSSW_10_2_13/src/boostedHiggsPlusMET/datacards/";
odir = "../output/";

def columnToList(fn,col):
	f = open(fn,'r');
	olist = [];
	for line in f:
		linelist = line.strip().split()
		olist.append( linelist[col] );
	return olist

def ExtractFile(iname, m1):
	f = ROOT.TFile(iname);
	t = f.Get("limit");
	lims = [];
	lims.append(m1);
	for i in range(6):
		t.GetEntry(i);
		lims.append( t.limit )
	return lims;

def makeBrazilFlag(compareLimits,saveRootFile):
	results = []; results2 = []; results3 = []; results4 = [];

	#Combo, ResolvedOnly+BoostedVeto
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1000_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1000') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1100_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1100') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1200_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1200') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1300_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1300') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1400_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1400') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1500_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1500') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1600_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1600') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1700_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1700') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1800_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1800') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH1900_LSP1_Data_Combo.AsymptoticLimits.mH120.root','1900') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH2000_LSP1_Data_Combo.AsymptoticLimits.mH120.root','2000') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH2100_LSP1_Data_Combo.AsymptoticLimits.mH120.root','2100') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH2200_LSP1_Data_Combo.AsymptoticLimits.mH120.root','2200') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH2300_LSP1_Data_Combo.AsymptoticLimits.mH120.root','2300') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH2400_LSP1_Data_Combo.AsymptoticLimits.mH120.root','2400') );
	results.append( ExtractFile(idir+'higgsCombine1DT5HH2500_LSP1_Data_Combo.AsymptoticLimits.mH120.root','2500') );

	#BoostedOnly
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1000_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1000') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1100_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1100') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1200_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1200') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1300_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1300') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1400_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1400') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1500_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1500') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1600_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1600') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1700_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1700') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1800_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1800') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH1900_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','1900') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH2000_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','2000') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH2100_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','2100') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH2200_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','2200') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH2300_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','2300') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH2400_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','2400') );
	results2.append( ExtractFile(idir+'higgsCombine1DT5HH2500_LSP1_BothBoostedH_Data.AsymptoticLimits.mH120.root','2500') );

	#ResolvedOnly
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1000_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1000') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1100_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1100') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1200_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1200') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1300_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1300') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1400_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1400') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1500_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1500') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1600_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1600') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1700_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1700') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1800_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1800') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH1900_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','1900') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH2000_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','2000') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH2100_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','2100') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH2200_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','2200') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH2300_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','2300') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH2400_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','2400') );
	results3.append( ExtractFile(idir+'higgsCombine1DT5HH2500_LSP1_Data_ResOnly.AsymptoticLimits.mH120.root','2500') );

	#Starting at 1 TeV
	xsecs=[0.325388, 0.163491, 0.0856418, 0.0460525, 0.0252977, 0.0141903, 0.00810078, 0.00470323, 0.00276133, 0.00163547, 0.000981077, 0.000591918,0.000359318,0.000219049,0.000133965,8.20068E-05]

	names   = []; names2   = []; names3   = []; names4   = [];
	l_obs   = []; l_obs2   = []; l_obs3   = []; l_obs4   = [];
	l_m2sig = []; l_m2sig2 = []; l_m2sig3 = []; l_m2sig4 = [];
	l_m1sig = []; l_m1sig2 = []; l_m1sig3 = []; l_m1sig4 = [];
	l_exp   = []; l_exp2   = []; l_exp3   = []; l_exp4   = [];
	l_p1sig = []; l_p1sig2 = []; l_p1sig3 = []; l_p1sig4 = [];
	l_p2sig = []; l_p2sig2 = []; l_p2sig3 = []; l_p2sig4 = [];
	count=0; count2=0; count3=0; count4=0;

	for r in results:
		thisxsec = xsecs[count]*1000.0
		names.append(r[0]);
		l_m2sig.append(r[1]*thisxsec);
		l_m1sig.append(r[2]*thisxsec);
		l_exp.append(r[3]*thisxsec);
		l_p1sig.append(r[4]*thisxsec);
		l_p2sig.append(r[5]*thisxsec);
		l_obs.append(r[6]*thisxsec);
		count=count+1

	for r in results2:
		thisxsec = xsecs[count2]*1000.0
		names2.append(r[0]);
		l_m2sig2.append(r[1]*thisxsec);
		l_m1sig2.append(r[2]*thisxsec);
		l_exp2.append(r[3]*thisxsec);
		l_p1sig2.append(r[4]*thisxsec);
		l_p2sig2.append(r[5]*thisxsec);
		l_obs2.append(r[6]*thisxsec);
		count2=count2+1

	for r in results3:
		thisxsec = xsecs[count3]*1000.0
		names3.append(r[0]);
		l_m2sig3.append(r[1]*thisxsec);
		l_m1sig3.append(r[2]*thisxsec);
		l_exp3.append(r[3]*thisxsec);
		l_p1sig3.append(r[4]*thisxsec);
		l_p2sig3.append(r[5]*thisxsec);
		l_obs3.append(r[6]*thisxsec);
		count3=count3+1

	a_xax = array('d', []); a_xax2 = array('d', []); a_xax3 = array('d', []); a_xax4 = array('d', []);
	a2_xax = array('d', []);
	a_exp = array('d', []);a_exp2 = array('d', []); a_exp3 = array('d', []); a_exp4 = array('d', []);
	a_obs = array('d', []); a_obs2 = array('d', []); a_obs3 = array('d', []);

	a_1sig = array('d', []); a_1sig2 = array('d', []); a_1sig3 = array('d', []);
	a_1sigup = array('d', []); a_1sig2up = array('d', []); a_1sig3up = array('d', []);
	a_1sigdown = array('d', []); a_1sig2down = array('d', []); a_1sig3down = array('d', []);

	a_2sig = array('d', []); a_2sig2 = array('d', []); a_2sig3 = array('d', []);
	a_2sigup = array('d', []); a_2sig2up = array('d', []); a_2sig3up = array('d', []);
	a_2sigdown = array('d', []); a_2sig2down = array('d', []); a_2sig3down = array('d', []);


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

	for i in range(len(l_m2sig3)): a_2sig3.append( float(l_m2sig3[i]) );
	for i in range(len(l_p2sig3)-1,-1,-1): a_2sig3.append( float(l_p2sig3[i]) );
	for i in range(len(l_m1sig3)): a_1sig3.append( float(l_m1sig3[i]) );
	for i in range(len(l_p1sig3)-1,-1,-1): a_1sig3.append( float(l_p1sig3[i]) );

	for i in range(len(l_m2sig3)): a_2sig3down.append( float(l_m2sig3[i]) );
	for i in range(len(l_p2sig3)): a_2sig3up.append( float(l_p2sig3[i]) );
	for i in range(len(l_m1sig3)): a_1sig3down.append( float(l_m1sig3[i]) );
	for i in range(len(l_p1sig3)): a_1sig3up.append( float(l_p1sig3[i]) );

	a_2sig.append(results[0][6])
	a_2sig2.append(results2[0][6])
	a_2sig3.append(results3[0][6])
	a2_xax.append(0.5)

	g_exp = ROOT.TGraph(len(a_xax), a_xax, a_exp)
	g_obs = ROOT.TGraph(len(a_xax), a_xax, a_obs)
	g_1sig = ROOT.TGraph(len(2*a_xax), a2_xax, a_1sig)
	g_2sig = ROOT.TGraph(len(2*a_xax), a2_xax, a_2sig)

	g_1sigup = ROOT.TGraph(len(a_xax), a_xax, a_1sigup)
	g_1sigdown = ROOT.TGraph(len(a_xax), a_xax, a_1sigdown)
	g_2sigup = ROOT.TGraph(len(a_xax), a_xax, a_2sigup)
	g_2sigdown = ROOT.TGraph(len(a_xax), a_xax, a_2sigdown)

	g_exp2 = ROOT.TGraph(len(a_xax2), a_xax2, a_exp2)
	g_obs2 = ROOT.TGraph(len(a_xax2), a_xax2, a_obs2)
	g_1sig2 = ROOT.TGraph(len(2*a_xax2), a2_xax, a_1sig2)
	g_2sig2 = ROOT.TGraph(len(2*a_xax2), a2_xax, a_2sig2)

	g_1sig2up = ROOT.TGraph(len(a_xax2), a_xax, a_1sig2up)
	g_1sig2down = ROOT.TGraph(len(a_xax2), a_xax, a_1sig2down)
	g_2sig2up = ROOT.TGraph(len(a_xax2), a_xax, a_2sig2up)
	g_2sig2down = ROOT.TGraph(len(a_xax2), a_xax, a_2sig2down)

	g_exp3 = ROOT.TGraph(len(a_xax3), a_xax3, a_exp3)
	g_obs3 = ROOT.TGraph(len(a_xax3), a_xax3, a_obs3)
	g_1sig3 = ROOT.TGraph(len(2*a_xax3), a2_xax, a_1sig3)
	g_2sig3 = ROOT.TGraph(len(2*a_xax3), a2_xax, a_2sig3)

	g_1sig3up = ROOT.TGraph(len(a_xax3), a_xax, a_1sig3up)
	g_1sig3down = ROOT.TGraph(len(a_xax3), a_xax, a_1sig3down)
	g_2sig3up = ROOT.TGraph(len(a_xax3), a_xax, a_2sig3up)
	g_2sig3down = ROOT.TGraph(len(a_xax3), a_xax, a_2sig3down)

	can = ROOT.TCanvas("can","can",1600,1200);
	hrl = ROOT.TH1F("hrl","hrl",36,1000,2500);
	hrl.GetXaxis().SetTitle("m(#tilde{g}#scale[0.55]{_{ }}) [GeV]");
	hrl.GetXaxis().SetRangeUser(750,2600);
	hrl.GetXaxis().SetLabelSize(0.04)
	hrl.GetXaxis().SetTitleSize(0.053)
	hrl.GetXaxis().SetTitleOffset(1.0)
	hrl.GetYaxis().SetTitle("Cross section [fb] ");
	hrl.GetYaxis().SetLabelSize(0.04)
	hrl.GetYaxis().SetTitleSize(0.053)
	hrl.GetYaxis().SetTitleOffset(0.90)
	hrl.Draw();

	hrl.GetYaxis().SetRangeUser(0.01,100000.);
	can.SetLeftMargin( 0.1 ); can.SetRightMargin( 0.04 );
	can.SetTopMargin( 0.06 ); can.SetBottomMargin( 0.13 );
	can.SetLogy();

	chi02 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.9]{#scale[0.85]{_{2}}}";
	chi01 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0 }}}#kern[-1.2]{#scale[0.85]{_{1 }}}";
	chi01nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
	chipm1nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{#pm}}}#kern[-1.3]{#scale[0.85]{_{1}}}";


	txta = ROOT.TLatex(0.16,0.86,"CMS");
	txta.SetNDC();  txta.SetTextSize(0.07);
	txtb = ROOT.TLatex(0.16,0.82,"Preliminary");
	txtb.SetNDC(); txtb.SetTextFont(52); txtb.SetTextSize(0.042);
	txtc = ROOT.TLatex(0.7,0.95,"137 fb^{-1} (13 TeV)");
	txtc.SetNDC(); txtc.SetTextFont(42); txtc.SetTextSize(0.05);
	txtd = ROOT.TLatex(0.41,0.85,"pp #rightarrow #tilde{g} #tilde{g},  #tilde{g} #rightarrow q #bar{q} "+chi02+",  "+chi02+" #rightarrow H "+chi01);
	txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.05);
	if compareLimits:
		txtf = ROOT.TLatex(0.16,0.805,"Supplementary");
		txtf.SetNDC(); txtf.SetTextFont(52); txtf.SetTextSize(0.042);

	a_stop = array('d', []);
	a_xsec = array('d', []);
	count=0;
	for x in xsecs:
		a_stop.append(float(names[count]));
		a_xsec.append(x*1000.0)
		count=count+1
	g_xsec=ROOT.TGraph(len(a_stop), a_stop, a_xsec)

	leg = ROOT.TLegend(0.4,0.53,0.65,0.83);
	leg.SetFillStyle(4050);leg.SetFillColor(0);
	leg.SetTextFont(42);leg.SetBorderSize(0);
	leg.SetTextSize(0.045);

	if compareLimits:
		leg.SetHeader("95% CL upper limit")
		leg.AddEntry(g_obs2, "Observed (boosted)","l")
		leg.AddEntry(g_obs3,"Observed (resolved)","l")
		leg.AddEntry(g_exp2, "Expected (boosted)","l")
		leg.AddEntry(g_exp3,"Expected (resolved)","l")
	else:
		leg.SetHeader("95% CL upper limit")
		leg.AddEntry(g_obs,"Observed","l")
		leg.AddEntry(g_exp,"Expected","l")
		leg.AddEntry(g_2sig,"#pm2 #sigma_{experiment}","f")
		leg.AddEntry(g_1sig,"#pm1 #sigma_{experiment}","f")


	leg2 = ROOT.TLegend(0.15,0.2,0.4,0.3);
	leg2.SetFillStyle(4050); leg2.SetFillColor(0);
	leg2.SetTextFont(42); leg2.SetBorderSize(0);
	leg2.SetTextSize(0.045);
	leg2.AddEntry(g_xsec, "Theory", "l")

	g_1sig.SetFillColor(ROOT.kGreen+1); g_1sig.SetFillStyle(3244);
	g_2sig.SetFillColor(ROOT.kOrange);g_2sig.SetFillStyle(3244);
	g_exp.SetLineStyle(2); g_exp.SetLineColor(ROOT.kBlack);
	g_exp.SetLineWidth(3); g_obs.SetLineWidth(2);

	g_exp.SetLineColor(ROOT.kBlack); g_exp.SetLineWidth(2); g_exp.SetLineStyle(2);
	g_obs.SetLineColor(ROOT.kBlack); g_obs.SetLineWidth(2); g_obs.SetLineStyle(1);
	g_exp2.SetLineColor(ROOT.kRed); g_exp2.SetLineWidth(2); g_exp2.SetLineStyle(2);
	g_obs2.SetLineColor(ROOT.kRed); g_obs2.SetLineWidth(2); g_obs2.SetLineStyle(1);
	g_exp3.SetLineColor(ROOT.kBlue); g_exp3.SetLineWidth(2); g_exp3.SetLineStyle(2);
	g_obs3.SetLineColor(ROOT.kBlue); g_obs3.SetLineWidth(2); g_obs3.SetLineStyle(1);


	if compareLimits:
		g_exp2.Draw('l');
		g_exp3.Draw('lsames');
		g_obs2.Draw('lsames');
		g_obs3.Draw('lsames');

	else:
		g_2sig.Draw('f');
		g_1sig.Draw('fsames');
		g_obs.Draw('lsames');
		g_exp.Draw('lsames');

	txta.Draw(); txtc.Draw(); txtd.Draw();
	leg.Draw(); leg2.Draw();
	g_xsec.SetLineStyle(2); g_xsec.SetLineWidth(2);
	g_xsec.SetLineColor(ROOT.kRed);

	if compareLimits:
		g_xsec.SetLineColor(ROOT.kBlack);
		txtf.Draw();
	g_xsec.Draw("lsame")

	for i in range(0,500):
		if (g_xsec.Eval(2000+i)-g_obs.Eval(2000+i)>0):
			print "AllH: Mass %d  Obs Excl %g " %(2000+i,g_obs.Eval(2000+i))

	if compareLimits: can.SaveAs(odir+'CMS-SUS-20-004_Figure-aux_001.pdf');
	else:
		can.SaveAs(odir+'Figure_014.pdf');
		# txtb.Draw(); #Preliminary
		# can.SaveAs(odir+'T5HHResults_ComboOnly_Data_prelim.pdf');


	####### Save ROOT file with all limits
	if saveRootFile:
		fNEW=ROOT.TFile(odir+"CMS-SUS-20-004_Figure_014.root", "RECREATE")
		can2 = ROOT.TCanvas("can2","can2",1600,1200);

		g_xsec.Write("TheoryXSec");
		g_exp3.Write("ExpectedLimit_Resolved");
		g_exp2.Write("ExpectedLimit_Boosted");
		g_exp.Write("ExpectedLimit_Combo");

		g_1sig3up.Write("ExpectedLimit1SigmaUp_Resolved");
		g_1sig3down.Write("ExpectedLimit1SigmaDown_Resolved");
		g_1sig2up.Write("ExpectedLimit1SigmaUp_Boosted");
		g_1sig2down.Write("ExpectedLimit1SigmaDown_Boosted");
		g_1sigup.Write("ExpectedLimit1SigmaUp_Combo");
		g_1sigdown.Write("ExpectedLimit1SigmaDown_Combo");

		g_2sig3up.Write("ExpectedLimit2SigmaUp_Resolved");
		g_2sig3down.Write("ExpectedLimit2SigmaDown_Resolved");
		g_2sig2up.Write("ExpectedLimit2SigmaUp_Boosted");
		g_2sig2down.Write("ExpectedLimit2SigmaDown_Boosted");
		g_2sigup.Write("ExpectedLimit2SigmaUp_Combo");
		g_2sigdown.Write("ExpectedLimit2SigmaDown_Combo");

		g_obs3.Write("ObservedLimit_Resolved");
		g_obs2.Write("ObservedLimit_Boosted");
		g_obs.Write("ObservedLimit_Combo");

		fNEW.Close();

def main():
	#First argument is compareLimits
	#Second is saveRootFile, does the same thing regardless of first argument
	makeBrazilFlag(True,True) #boostedOnly vs resolvedOnly
	makeBrazilFlag(False,False) #Combination with 1- and 2-sigma bands


if __name__ == "__main__":
    main()
