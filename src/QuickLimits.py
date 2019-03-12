from ROOT import *
import os
Bkgs=["QCD","ZJets", "WJets", "TT"]

#Files
fboost=TFile("newMETbin/Boost/Bkg.root", "READ");
fDiAk8=TFile("newMETbin/DiAK8/Bkg.root", "READ");
fSemiRes=TFile("newMETbin/SemiResolved/Bkg.root", "READ");
fSingleAk8=TFile("newMETbin/SingleAK8/Bkg.root", "READ");
fRes4b=TFile("newMETbin/Res4b/Bkg.root", "READ");
fRes3b=TFile("newMETbin/Res3b/Bkg.root", "READ");

Res3b=fRes3b.Get("%s_MET" %Bkgs[0]) #initialize to get the number of MET bins
Res4b=fRes4b.Get("%s_MET" %Bkgs[0]) #initialize to get the number of MET bins
BoostedBins=fboost.Get("%s_MET" %Bkgs[0])
SemiRes=fSemiRes.Get("%s_MET" %Bkgs[0])
SingleAK8=fSingleAk8.Get("%s_MET" %Bkgs[0])
DiAk8=fDiAk8.Get("%s_MET" %Bkgs[0])
FullBkgYieldsSingleAk8=[[]]
FullBkgYieldsDiAk8=[[]]
FullBkgYieldsRes3b=[[]]
FullBkgYieldsRes4b=[[]]
FullBkgYieldsBoostH=[[]]
FullBkgYieldsSemi=[[]]
for b in Bkgs:
	Res4b=fRes4b.Get("%s_MET" %b)
	Res3b=fRes3b.Get("%s_MET" %b)
	#if b is "QCD":
	#	Res3b.Scale(3.2/Res4b.Integral())
	#	Res4b.Scale(3.2/Res4b.Integral())
	BoostedBins=fboost.Get("%s_MET" %b)
	SemiRes=fSemiRes.Get("%s_MET" %b)
	SingleAK8=fSingleAk8.Get("%s_MET" %b)
	DiAk8=fDiAk8.Get("%s_MET" %b)
	METYields=[]
	for m in range(1,Res4b.GetNbinsX()+1):
		METYields.append(Res4b.GetBinContent(m));
	FullBkgYieldsRes4b.append(METYields)
	METYields=[]
	for m in range(1,Res3b.GetNbinsX()+1):
		METYields.append(Res3b.GetBinContent(m));
	FullBkgYieldsRes3b.append(METYields)
        METYields=[]
	for m in range(1, SemiRes.GetNbinsX()+1):
		METYields.append(SemiRes.GetBinContent(m))
	FullBkgYieldsSemi.append(METYields)
        METYields=[]
	for m in range(1, BoostedBins.GetNbinsX()+1):
		METYields.append(BoostedBins.GetBinContent(m))
	FullBkgYieldsBoostH.append(METYields)
        METYields=[]
	for m in range(1, SingleAK8.GetNbinsX()+1):
		METYields.append(SingleAK8.GetBinContent(m))
	FullBkgYieldsSingleAk8.append(METYields)
        METYields=[]
	for m in range(1, DiAk8.GetNbinsX()+1):
		METYields.append(DiAk8.GetBinContent(m))
	FullBkgYieldsDiAk8.append(METYields)

print FullBkgYieldsRes4b,FullBkgYieldsRes3b
fboostSig=TFile("newMETbin/Boost/TChiHH.root", "READ");
fDiAk8Sig=TFile("newMETbin/DiAK8/TChiHH.root", "READ");
fSemiResSig=TFile("newMETbin/SemiResolved/TChiHH.root", "READ");
fSingleAk8Sig=TFile("newMETbin/SingleAK8/TChiHH.root", "READ");
fResSig4b=TFile("newMETbin/Res4b/TChiHH.root", "READ");
fResSig3b=TFile("newMETbin/Res3b/TChiHH.root", "READ");
mSig=[127, 150, 175,200,225, 250, 275, 300, 325,350, 375, 400, 425,450,475,500, 525, 550,575, 600, 625, 650, 675, 700,725, 750,775, 800,825, 850, 875, 900, 925, 950, 975, 1000]
#mSig=[600,625, 650, 675, 700,725, 750,775, 800,825, 850, 875, 900, 925, 975, 1000]
#mSig=[1000]
for s in mSig:
	SigHisto3b=fResSig3b.Get("TChiHH%d_MET" %s)
	SigHisto4b=fResSig4b.Get("TChiHH%d_MET" %s)
	SigHistoSemiRes=fSemiResSig.Get("TChiHH%d_MET" %s)
	SigHistoBoost=fboostSig.Get("TChiHH%d_MET" %s)
	SigHisto1Ak8=fSingleAk8Sig.Get("TChiHH%d_MET" %s)
	SigHisto2Ak8=fDiAk8Sig.Get("TChiHH%d_MET" %s)
	ResolvedCards3b=""
	ResolvedCards4b=""
	CardsBoost=""
	SingleJetCardsBoost=""
	DiJetCardsBoost=""
	SemiResolvedCards=""
	for m in range(0,SigHisto3b.GetNbinsX()):
		fout=open("HiggsinoRes3b_MET%d.txt" %m,'w')
		fout.seek(0)
		fout.write("# Simple counting experiment, with higgsino signal and a few background processes\n")
		fout.write("imax 1  number of channels\n") #one .txt card for each MET bin
		fout.write("jmax %d  number of backgrounds\n" %len(Bkgs)) #total bkg processes
		fout.write("kmax *  number of nuisance parameters (sources of systematical uncertainties)\n")
		fout.write("bin bin%d\n" %m)
		Obs=0;
		for i in range(1, len(Bkgs)+1):Obs=Obs+FullBkgYieldsRes3b[i][m]
		fout.write("observation %g\n" %Obs)
		fout.write("bin ");
		for i in range(0, len(Bkgs)+1):fout.write("bin%d " %m)
		fout.write("\nprocess sig");
		for b in Bkgs: fout.write(" %s" %b)
		fout.write("\nprocess")
		for i in range(0, len(Bkgs)+1):fout.write(" %d" %i)
		Sig=SigHisto3b.GetBinContent(m+1)
	 	if(m==SigHisto3b.GetNbinsX()-1):Sig=Sig+SigHisto3b.GetBinContent(m+2)#ADD OVERFLOW Bin
		fout.write("\nrate %g " %(Sig));
		for i in range(1, len(Bkgs)+1):
			#fout.write(" %g " %FullBkgYieldsRes3b[i][m])
			if FullBkgYieldsRes3b[i][m]>0:  fout.write(" %g " %FullBkgYieldsRes3b[i][m])
			else:fout.write(" 0.00001 ")
		fout.close()
		ResolvedCards3b=ResolvedCards3b+" HiggsinoRes3b_MET%d.txt" %m
	for m in range(0,SigHisto4b.GetNbinsX()):
		fout=open("HiggsinoRes4b_MET%d.txt" %m,'w')
		fout.seek(0)
		fout.write("# Simple counting experiment, with higgsino signal and a few background processes\n")
		fout.write("imax 1  number of channels\n") #one .txt card for each MET bin
		fout.write("jmax %d  number of backgrounds\n" %len(Bkgs)) #total bkg processes
		fout.write("kmax *  number of nuisance parameters (sources of systematical uncertainties)\n")
		fout.write("bin bin%d\n" %m)
		Obs=0;
		for i in range(1, len(Bkgs)+1):Obs=Obs+FullBkgYieldsRes4b[i][m]
		fout.write("observation %g\n" %Obs)
		fout.write("bin ");
		for i in range(0, len(Bkgs)+1):fout.write("bin%d " %m)
		fout.write("\nprocess sig");
		for b in Bkgs: fout.write(" %s" %b)
		fout.write("\nprocess")
		for i in range(0, len(Bkgs)+1):fout.write(" %d" %i)
		Sig=SigHisto4b.GetBinContent(m+1)
	 	if(m==SigHisto4b.GetNbinsX()-1):Sig=Sig+SigHisto4b.GetBinContent(m+2)#ADD OVERFLOW Bin
		fout.write("\nrate %g " %(Sig));
		for i in range(1, len(Bkgs)+1):
			#fout.write(" %g " %FullBkgYieldsRes4b[i][m])
			if FullBkgYieldsRes4b[i][m]>0:  fout.write(" %g " %FullBkgYieldsRes4b[i][m])
			else:fout.write(" 0.00001 ")
		fout.close()
		ResolvedCards4b=ResolvedCards4b+" HiggsinoRes4b_MET%d.txt" %m

	for m in range(0,SigHistoBoost.GetNbinsX()):
		fout=open("HiggsinoBoosted_MET%d.txt" %m,'w')
		fout.seek(0)
		fout.write("# Simple counting experiment, with higgsino signal and a few background processes\n")
		fout.write("imax 1  number of channels\n") #one .txt card for each MET bin
		fout.write("jmax %d  number of backgrounds\n" %len(Bkgs)) #total bkg processes
		fout.write("kmax *  number of nuisance parameters (sources of systematical uncertainties)\n")
		fout.write("bin bin%d\n" %m)
		Obs=0;
		for i in range(1, len(Bkgs)+1):Obs=Obs+FullBkgYieldsBoostH[i][m]
		fout.write("observation %g\n" %Obs)
		fout.write("bin ");
		for i in range(0, len(Bkgs)+1):fout.write("bin%d " %m)
		fout.write("\nprocess sig");
		for b in Bkgs: fout.write(" %s" %b)
		fout.write("\nprocess")
		for i in range(0, len(Bkgs)+1):fout.write(" %d" %i)
		Sig=SigHistoBoost.GetBinContent(m+1)
	 	if(m==SigHistoBoost.GetNbinsX()-1):Sig=Sig+SigHistoBoost.GetBinContent(m+2)#ADD OVERFLOW Bin
		fout.write("\nrate %g " %(Sig));
		for i in range(1, len(Bkgs)+1):
			if FullBkgYieldsBoostH[i][m]>0:  fout.write(" %g " %FullBkgYieldsBoostH[i][m])
			else:fout.write(" 0.0 ")
		fout.close()
		CardsBoost=CardsBoost+" HiggsinoBoosted_MET%d.txt" %m
	for m in range(0,SigHisto1Ak8.GetNbinsX()):
		fout=open("HiggsinoSingleAk8_MET%d.txt" %m,'w')
		fout.seek(0)
		fout.write("# Simple counting experiment, with higgsino signal and a few background processes\n")
		fout.write("imax 1  number of channels\n") #one .txt card for each MET bin
		fout.write("jmax %d  number of backgrounds\n" %len(Bkgs)) #total bkg processes
		fout.write("kmax *  number of nuisance parameters (sources of systematical uncertainties)\n")
		fout.write("bin bin%d\n" %m)
		Obs=0;
		for i in range(1, len(Bkgs)+1):Obs=Obs+FullBkgYieldsSingleAk8[i][m]
		fout.write("observation %g\n" %Obs)
		fout.write("bin ");
		for i in range(0, len(Bkgs)+1):fout.write("bin%d " %m)
		fout.write("\nprocess sig");
		for b in Bkgs: fout.write(" %s" %b)
		fout.write("\nprocess")
		for i in range(0, len(Bkgs)+1):fout.write(" %d" %i)
		Sig=SigHisto1Ak8.GetBinContent(m+1)
	 	if(m==SigHisto1Ak8.GetNbinsX()-1):Sig=Sig+SigHisto1Ak8.GetBinContent(m+2)#ADD OVERFLOW Bin
		fout.write("\nrate %g " %(Sig));
		for i in range(1, len(Bkgs)+1):
			if FullBkgYieldsSingleAk8[i][m]>0:  fout.write(" %g " %FullBkgYieldsSingleAk8[i][m])
			else:fout.write(" 0.0 ")
		fout.close()
		SingleJetCardsBoost=SingleJetCardsBoost+" HiggsinoSingleAk8_MET%d.txt" %m
	for m in range(0,SigHisto2Ak8.GetNbinsX()):
		fout=open("HiggsinoDiAk8_MET%d.txt" %m,'w')
		fout.seek(0)
		fout.write("# Simple counting experiment, with higgsino signal and a few background processes\n")
		fout.write("imax 1  number of channels\n") #one .txt card for each MET bin
		fout.write("jmax %d  number of backgrounds\n" %len(Bkgs)) #total bkg processes
		fout.write("kmax *  number of nuisance parameters (sources of systematical uncertainties)\n")
		fout.write("bin bin%d\n" %m)
		Obs=0;
		for i in range(1, len(Bkgs)+1):Obs=Obs+FullBkgYieldsDiAk8[i][m]
		fout.write("observation %g\n" %Obs)
		fout.write("bin ");
		for i in range(0, len(Bkgs)+1):fout.write("bin%d " %m)
		fout.write("\nprocess sig");
		for b in Bkgs: fout.write(" %s" %b)
		fout.write("\nprocess")
		for i in range(0, len(Bkgs)+1):fout.write(" %d" %i)
		Sig=SigHisto2Ak8.GetBinContent(m+1)
	 	if(m==SigHisto2Ak8.GetNbinsX()-1):Sig=Sig+SigHisto2Ak8.GetBinContent(m+2)#ADD OVERFLOW Bin
		fout.write("\nrate %g " %(Sig));
		for i in range(1, len(Bkgs)+1):
			if FullBkgYieldsDiAk8[i][m]>0:  fout.write(" %g " %FullBkgYieldsDiAk8[i][m])
			else:fout.write(" 0.0 ")
		fout.close()
		DiJetCardsBoost=DiJetCardsBoost+" HiggsinoDiAk8_MET%d.txt" %m
	for m in range(0,SigHistoSemiRes.GetNbinsX()):
		fout=open("HiggsinoSemiResolved_MET%d.txt" %m,'w')
		fout.seek(0)
		fout.write("# Simple counting experiment, with higgsino signal and a few background processes\n")
		fout.write("imax 1  number of channels\n") #one .txt card for each MET bin
		fout.write("jmax %d  number of backgrounds\n" %len(Bkgs)) #total bkg processes
		fout.write("kmax *  number of nuisance parameters (sources of systematical uncertainties)\n")
		fout.write("bin bin%d\n" %m)
		Obs=0;
		for i in range(1, len(Bkgs)+1):Obs=Obs+FullBkgYieldsSemi[i][m]
		fout.write("observation %g\n" %Obs)
		fout.write("bin ");
		for i in range(0, len(Bkgs)+1):fout.write("bin%d " %m)
		fout.write("\nprocess sig");
		for b in Bkgs: fout.write(" %s" %b)
		fout.write("\nprocess")
		for i in range(0, len(Bkgs)+1):fout.write(" %d" %i)
		Sig=SigHistoSemiRes.GetBinContent(m+1)
	 	if(m==SigHistoSemiRes.GetNbinsX()-1):Sig=Sig+SigHistoSemiRes.GetBinContent(m+2)#ADD OVERFLOW Bin
		fout.write("\nrate %g " %(Sig));
		for i in range(1, len(Bkgs)+1):
			if FullBkgYieldsSemi[i][m]>0:  fout.write(" %g " %FullBkgYieldsSemi[i][m])
			else:fout.write(" 0.0 ")
		fout.close()
		SemiResolvedCards=SemiResolvedCards+" HiggsinoSemiResolved_MET%d.txt" %m
	print "signal: TChiHH%s " %s
	os.system("combineCards.py %s %s>HiggsinoBoostedOnlyTChiHH%s.txt" %(DiJetCardsBoost,CardsBoost,s))
	os.system("combineCards.py %s %s>HiggsinoResolvedOnlyTChiHH%s.txt" %(ResolvedCards3b,ResolvedCards4b,s))#this one
	os.system("combineCards.py %s %s HiggsinoBoostedOnlyTChiHH%s.txt > ResolvedBoostCombinationTChiHH%s.txt" %(ResolvedCards3b,ResolvedCards4b,s,s))
	os.system("combine -M AsymptoticLimits ResolvedBoostCombinationTChiHH%s.txt -n ResolvedBoostCombinationTChiHH%s -m %s" %(s,s,s))
	os.system("combineCards.py %s %s HiggsinoBoostedOnlyTChiHH%s.txt %s %s > ResolvedBoostFullCombinationTChiHH%s.txt " %(ResolvedCards3b,ResolvedCards4b,s,SemiResolvedCards,SingleJetCardsBoost,s))
	os.system("combine -M AsymptoticLimits ResolvedBoostFullCombinationTChiHH%s.txt -n FullCombinationTChiHH%s -m %s" %(s,s,s))#this one
