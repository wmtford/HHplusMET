import os
from ROOT import *
import sys

def binContent(histo,binNum):
	thisBinContent = histo.GetBinContent(binNum);
	if (thisBinContent>0): return thisBinContent;
  	else: return 0.000001;

def systBinContent(histoNom,histoVar,binNum):
	nominalBinContent = histoNom.GetBinContent(binNum);
	variationBinContent = histoVar.GetBinContent(binNum);
	if binNum==3:
		nominalBinContent = histoNom.GetBinContent(binNum)+histoNom.GetBinContent(binNum+1);
		variationBinContent = histoVar.GetBinContent(binNum)+histoVar.GetBinContent(binNum+1);
	if (nominalBinContent>0):
		syst = (variationBinContent-nominalBinContent)/nominalBinContent+1.0
		if syst==0: syst = 2.00
		if syst<0:  syst=syst+2.0
	 	return syst;
  	else: return 2.0;


signal = "TChiHH"
runVeto = True
runResAlone = True
useData = True
run2D = False
dataMCString ="MC"
if useData==True: dataMCString ="Data"
whichDString ="1D"
if run2D==True: whichDString ="2D"


f=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_Final/ALPHABET_0l.root", "READ");
fData=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_Final/ALPHABET_0lData.root", "READ");
fVetoData=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0lData.root", "READ");
fVeto=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0l.root", "READ");

if useData==True:
	sumRegionA2=fData.Get("MET_doubletagSR_data"); sumRegionA1=fData.Get("MET_tagSR_data");
	sumRegionB2=fData.Get("MET_doubletagSB_data").Integral(1,5); sumRegionB1=fData.Get("MET_tagSB_data").Integral(1,5);
	sumRegionC=fData.Get("MET_antitagSR_data").Integral(1,5); sumRegionD=fData.Get("MET_antitagSB_data").Integral(1,5);
	bkgFrac_C=fData.Get("MET_antitagOpt1_data"); bkgFrac_C.SetBinContent(3,bkgFrac_C.GetBinContent(3)+bkgFrac_C.GetBinContent(4)); bkgFrac_C.SetBinContent(4,0.0);

else:
	sumRegionA2=f.Get("MET_doubletagSR_sum"); sumRegionA1=f.Get("MET_tagSR_sum");
	sumRegionB2=f.Get("MET_doubletagSB_sum").Integral(1,5); sumRegionB1=f.Get("MET_tagSB_sum").Integral(1,5);
	sumRegionC=f.Get("MET_antitagSR_sum").Integral(1,5); sumRegionD=f.Get("MET_antitagSB_sum").Integral(1,5);
	bkgFrac_C=f.Get("MET_antitagOpt1_sum");  bkgFrac_C.SetBinContent(3,bkgFrac_C.GetBinContent(3)+bkgFrac_C.GetBinContent(4)); bkgFrac_C.SetBinContent(4,0.0);

bkgFracHisto = bkgFrac_C.Clone("bkgFracHisto");
bkgFracHisto.Scale(1/bkgFracHisto.Integral(1,5))
Predictions2H = f.Get("MET_doubletagSB_sum"); Predictions1H = f.Get("MET_tagSB_sum") #just to set bin numbers

#for vetoed events
if runVeto==True:
	if useData==True:
		sumRegionA2Veto=fVetoData.Get("MET_doubletagSR_data"); sumRegionA1Veto=fVetoData.Get("MET_tagSR_data");
		sumRegionB2Veto=fVetoData.Get("MET_doubletagSB_data").Integral(1,5); sumRegionB1Veto=fVetoData.Get("MET_tagSB_data").Integral(1,5);
		sumRegionCVeto=fVetoData.Get("MET_antitagSR_data").Integral(1,5); sumRegionDVeto=fVetoData.Get("MET_antitagSB_data").Integral(1,5);
		bkgFrac_CVeto=fVetoData.Get("MET_antitagOpt1_data"); bkgFrac_CVeto.SetBinContent(3,bkgFrac_CVeto.GetBinContent(3)+bkgFrac_CVeto.GetBinContent(4)); bkgFrac_CVeto.SetBinContent(4,0.0);
		Predictions2HVeto=fVetoData.Get("MET_doubletagSB_data") #just to set bin numbers
		Predictions1HVeto=fVetoData.Get("MET_tagSB_data") #just to set bin numbers
	else:
		sumRegionA2Veto=fVeto.Get("MET_doubletagSR_sum"); sumRegionA1Veto=fVeto.Get("MET_tagSR_sum");
		sumRegionB2Veto=fVeto.Get("MET_doubletagSB_sum").Integral(1,5); sumRegionB1Veto=fVeto.Get("MET_tagSB_sum").Integral(1,5);
		sumRegionCVeto=fVeto.Get("MET_antitagSR_sum").Integral(1,5); sumRegionDVeto=fVeto.Get("MET_antitagSB_sum").Integral(1,5);
		bkgFrac_CVeto=fVeto.Get("MET_antitagOpt1_sum"); bkgFrac_CVeto.SetBinContent(3,bkgFrac_CVeto.GetBinContent(3)+bkgFrac_CVeto.GetBinContent(4)); bkgFrac_CVeto.SetBinContent(4,0.0);
		Predictions2HVeto=fVeto.Get("MET_doubletagSB_sum") #just to set bin numbers
		Predictions1HVeto=fVeto.Get("MET_tagSB_sum") #just to set bin numbers

	bkgFracHistoVeto = bkgFrac_CVeto.Clone("bkgFracHistoVeto");
	bkgFracHistoVeto.Scale(1/bkgFracHistoVeto.Integral(1,5))



hino=int(sys.argv[1]); LSP=int(sys.argv[2])
print("\n\n\n-----------------------------------------------------")
print("Signal: %s, looping over mass=%d, mLSP=%d now" %(signal,hino,LSP))
print("-----------------------------------------------------\n")

#2D
if run2D:
	f2=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_FastSIMSFs/ALPHABET_2DSignal.root", "READ")
	f2_genMET=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_FastSIMSFs_genMET/ALPHABET_2DSignal.root", "READ")
	if runVeto==True:
		f2Veto=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_FastSIMSFs/ALPHABET_2DSignal.root", "READ")
		f2Veto_genMET=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_FastSIMSFs_genMET/ALPHABET_2DSignal.root", "READ")

else:
	f2=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_FastSIMSFs/ALPHABET_1DSignal.root", "READ")
	f2_genMET=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_FastSIMSFs_genMET/ALPHABET_1DSignal.root", "READ")
	if runVeto==True:
		f2Veto=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_FastSIMSFs/ALPHABET_1DSignal.root", "READ")
		f2Veto_genMET=TFile("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_FastSIMSFs_genMET/ALPHABET_1DSignal.root", "READ")


SignalA2=f2.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalA1=f2.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP));
SignalA2.SetBinContent(3,SignalA2.GetBinContent(3)+SignalA2.GetBinContent(4)); SignalA2.SetBinContent(4,0.0);
SignalA1.SetBinContent(3,SignalA1.GetBinContent(3)+SignalA1.GetBinContent(4)); SignalA1.SetBinContent(4,0.0);
SignalB2=f2.Get("MET_doubletagSB_%s%d_LSP%d" %(signal,hino,LSP)); SignalB1=f2.Get("MET_tagSB_%s%d_LSP%d" %(signal,hino,LSP));
SignalC=f2.Get("MET_antitagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalD=f2.Get("MET_antitagSB_%s%d_LSP%d" %(signal,hino,LSP));
SignalMET=f2.Get("MET_antitagOpt1_%s%d_LSP%d" %(signal,hino,LSP));
SignalMET.SetBinContent(3,SignalMET.GetBinContent(3)+SignalMET.GetBinContent(4)); SignalMET.SetBinContent(4,0.0);

#genMET
SignalA2_gen=f2_genMET.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalA1_gen=f2_genMET.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP));
SignalA2_gen.SetBinContent(3,SignalA2_gen.GetBinContent(3)+SignalA2_gen.GetBinContent(4)); SignalA2_gen.SetBinContent(4,0.0);
SignalA1_gen.SetBinContent(3,SignalA1_gen.GetBinContent(3)+SignalA1_gen.GetBinContent(4)); SignalA1_gen.SetBinContent(4,0.0);
SignalB2_gen=f2_genMET.Get("MET_doubletagSB_%s%d_LSP%d" %(signal,hino,LSP)); SignalB1_gen=f2_genMET.Get("MET_tagSB_%s%d_LSP%d" %(signal,hino,LSP));
SignalC_gen=f2_genMET.Get("MET_antitagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalD_gen=f2_genMET.Get("MET_antitagSB_%s%d_LSP%d" %(signal,hino,LSP));
SignalMET_gen=f2_genMET.Get("MET_antitagOpt1_%s%d_LSP%d" %(signal,hino,LSP));
SignalMET_gen.SetBinContent(3,SignalMET_gen.GetBinContent(3)+SignalMET_gen.GetBinContent(4)); SignalMET_gen.SetBinContent(4,0.0);

if runVeto==True:
	SignalA2Veto=f2Veto.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalA1Veto=f2Veto.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP));
	SignalA2Veto.SetBinContent(3,SignalA2Veto.GetBinContent(3)+SignalA2Veto.GetBinContent(4)); SignalA2Veto.SetBinContent(4,0.0);
	SignalA1Veto.SetBinContent(3,SignalA1Veto.GetBinContent(3)+SignalA1Veto.GetBinContent(4)); SignalA1Veto.SetBinContent(4,0.0);
	SignalB2Veto=f2Veto.Get("MET_doubletagSB_%s%d_LSP%d" %(signal,hino,LSP)); SignalB1Veto=f2Veto.Get("MET_tagSB_%s%d_LSP%d" %(signal,hino,LSP));
	SignalCVeto=f2Veto.Get("MET_antitagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalDVeto=f2Veto.Get("MET_antitagSB_%s%d_LSP%d" %(signal,hino,LSP));
	SignalMETVeto=f2Veto.Get("MET_antitagOpt1_%s%d_LSP%d" %(signal,hino,LSP));
	SignalMETVeto.SetBinContent(3,SignalMETVeto.GetBinContent(3)+SignalMETVeto.GetBinContent(4)); SignalMETVeto.SetBinContent(4,0.0);


	#genMET, vetoed
	SignalA2Veto_gen=f2Veto_genMET.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalA1Veto_gen=f2Veto_genMET.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP));
	SignalA2Veto_gen.SetBinContent(3,SignalA2Veto_gen.GetBinContent(3)+SignalA2Veto_gen.GetBinContent(4)); SignalA2Veto_gen.SetBinContent(4,0.0);
	SignalA1Veto_gen.SetBinContent(3,SignalA1Veto_gen.GetBinContent(3)+SignalA1Veto_gen.GetBinContent(4)); SignalA1Veto_gen.SetBinContent(4,0.0);
	SignalB2Veto_gen=f2Veto_genMET.Get("MET_doubletagSB_%s%d_LSP%d" %(signal,hino,LSP)); SignalB1Veto_gen=f2Veto_genMET.Get("MET_tagSB_%s%d_LSP%d" %(signal,hino,LSP));
	SignalCVeto_gen=f2Veto_genMET.Get("MET_antitagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalDVeto_gen=f2Veto_genMET.Get("MET_antitagSB_%s%d_LSP%d" %(signal,hino,LSP));
	SignalMETVeto_gen=f2Veto.Get("MET_antitagOpt1_%s%d_LSP%d" %(signal,hino,LSP));
	SignalMETVeto_gen.SetBinContent(3,SignalMETVeto_gen.GetBinContent(3)+SignalMETVeto_gen.GetBinContent(4)); SignalMETVeto_gen.SetBinContent(4,0.0);

#Background prediction
norm2H=sumRegionB2*sumRegionC/sumRegionD
norm1H=sumRegionB1*sumRegionC/sumRegionD

if runVeto==True:
	norm2HVeto=sumRegionB2Veto*sumRegionCVeto/sumRegionDVeto
	norm1HVeto=sumRegionB1Veto*sumRegionCVeto/sumRegionDVeto

for i in range(1,4):
	Predictions2H.SetBinContent(i, norm2H*bkgFracHisto.GetBinContent(i))
	Predictions1H.SetBinContent(i, norm1H*bkgFracHisto.GetBinContent(i))
	if runVeto==True:
		Predictions2HVeto.SetBinContent(i, norm2HVeto*bkgFracHistoVeto.GetBinContent(i))
		Predictions1HVeto.SetBinContent(i, norm1HVeto*bkgFracHistoVeto.GetBinContent(i))

#Start systematics
baseDir = "/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/";
sysDir = "boosted_noVeto_FinalSyst";

if run2D==False:
	fNominal=TFile(baseDir+"/boosted_noVeto_FastSIMSFs/ALPHABET_1DSignal.root", "READ");
	fNominal_gen=TFile(baseDir+"/boosted_noVeto_FastSIMSFs_genMET/ALPHABET_1DSignal.root", "READ");

	fISRUp=TFile(baseDir+sysDir+"/ISRup/ALPHABET_1DSignal.root", "READ");
	fISRDown=TFile(baseDir+sysDir+"/ISRdown/ALPHABET_1DSignal.root", "READ");
	fSmear=TFile(baseDir+sysDir+"/massSmear/ALPHABET_1DSignal.root", "READ");
	fFastMassUp=TFile(baseDir+sysDir+"/massFastSIMup/ALPHABET_1DSignal.root", "READ");
	fFastMassDown=TFile(baseDir+sysDir+"/massFastSIMdown/ALPHABET_1DSignal.root", "READ");
	fFastBBUp=TFile(baseDir+sysDir+"/bbFastSIMup/ALPHABET_1DSignal.root", "READ");
	fFastBBDown=TFile(baseDir+sysDir+"/bbFastSIMdown/ALPHABET_1DSignal.root", "READ");
	ftrigUp=TFile(baseDir+sysDir+"/trigUp/ALPHABET_1DSignal.root", "READ");
	ftrigDown=TFile(baseDir+sysDir+"/trigDown/ALPHABET_1DSignal.root", "READ");
	fpreDown=TFile(baseDir+sysDir+"/prefireDown/ALPHABET_1DSignal.root", "READ");
	fpreUp=TFile(baseDir+sysDir+"/prefireUp/ALPHABET_1DSignal.root", "READ");
	fpuUp=TFile(baseDir+sysDir+"/PUup/ALPHABET_1DSignal.root", "READ");
	fpuDown=TFile(baseDir+sysDir+"/PUdown/ALPHABET_1DSignal.root", "READ");
	fbbtagUp=TFile(baseDir+sysDir+"/bbTagUp/ALPHABET_1DSignal.root", "READ");
	fbbtagDown=TFile(baseDir+sysDir+"/bbTagDown/ALPHABET_1DSignal.root", "READ");
	fjecUp=TFile(baseDir+sysDir+"/JECup/ALPHABET_1DSignal.root", "READ");
	fjecDown=TFile(baseDir+sysDir+"/JECdown/ALPHABET_1DSignal.root", "READ");
	fjerUp=TFile(baseDir+sysDir+"/JERup/ALPHABET_1DSignal.root", "READ");
	fjerDown=TFile(baseDir+sysDir+"/JERdown/ALPHABET_1DSignal.root", "READ");
	fScaleUp=TFile(baseDir+sysDir+"/scalesUp/ALPHABET_1DSignal.root", "READ");
	fScaleDown=TFile(baseDir+sysDir+"/scalesDown/ALPHABET_1DSignal.root", "READ");

else:
	fNominal=TFile(baseDir+"/boosted_noVeto_FastSIMSFs/ALPHABET_2DSignal.root", "READ");
	fNominal_gen=TFile(baseDir+"/boosted_noVeto_FastSIMSFs_genMET/ALPHABET_2DSignal.root", "READ");
	fISRUp=TFile(baseDir+sysDir+"/ISRup/ALPHABET_2DSignal.root", "READ");
	fISRDown=TFile(baseDir+sysDir+"/ISRdown/ALPHABET_2DSignal.root", "READ");
	fSmear=TFile(baseDir+sysDir+"/massSmear/ALPHABET_2DSignal.root", "READ");
	fFastMassUp=TFile(baseDir+sysDir+"/massFastSIMup/ALPHABET_2DSignal.root", "READ");
	fFastMassDown=TFile(baseDir+sysDir+"/massFastSIMdown/ALPHABET_2DSignal.root", "READ");
	fFastBBUp=TFile(baseDir+sysDir+"/bbFastSIMup/ALPHABET_2DSignal.root", "READ");
	fFastBBDown=TFile(baseDir+sysDir+"/bbFastSIMdown/ALPHABET_2DSignal.root", "READ");
	ftrigUp=TFile(baseDir+sysDir+"/trigUp/ALPHABET_2DSignal.root", "READ");
	ftrigDown=TFile(baseDir+sysDir+"/trigDown/ALPHABET_2DSignal.root", "READ");
	fpreDown=TFile(baseDir+sysDir+"/prefireDown/ALPHABET_2DSignal.root", "READ");
	fpreUp=TFile(baseDir+sysDir+"/prefireUp/ALPHABET_2DSignal.root", "READ");
	fpuUp=TFile(baseDir+sysDir+"/PUup/ALPHABET_2DSignal.root", "READ");
	fpuDown=TFile(baseDir+sysDir+"/PUdown/ALPHABET_2DSignal.root", "READ");
	fbbtagUp=TFile(baseDir+sysDir+"/bbTagUp/ALPHABET_2DSignal.root", "READ");
	fbbtagDown=TFile(baseDir+sysDir+"/bbTagDown/ALPHABET_2DSignal.root", "READ");
	fjecUp=TFile(baseDir+sysDir+"/JECup/ALPHABET_2DSignal.root", "READ");
	fjecDown=TFile(baseDir+sysDir+"/JECdown/ALPHABET_2DSignal.root", "READ");
	fjerUp=TFile(baseDir+sysDir+"/JERup/ALPHABET_2DSignal.root", "READ");
	fjerDown=TFile(baseDir+sysDir+"/JERdown/ALPHABET_2DSignal.root", "READ");
	fScaleUp=TFile(baseDir+sysDir+"/scalesUp/ALPHABET_2DSignal.root", "READ");
	fScaleDown=TFile(baseDir+sysDir+"/scalesDown/ALPHABET_2DSignal.root", "READ");

Nom1H=fNominal.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); Nom2H=fNominal.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
NomGen_1H=fNominal_gen.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); NomGen_2H=fNominal_gen.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
PreUp1H=fpreUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); PreUp2H=fpreUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
PreDown1H=fpreDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); PreDown2H=fpreDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
PUUp1H=fpuUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); PUUp2H=fpuUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
PUDown1H=fpuDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); PUDown2H=fpuDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
ISRUp1H=fISRUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); ISRUp2H=fISRUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
ISRDown1H=fISRDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); ISRDown2H=fISRDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
JECUp1H=fjecUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); JECUp2H=fjecUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
JECDown1H=fjecDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); JECDown2H=fjecDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
JERUp1H=fjerUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); JERUp2H=fjerUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
JERDown1H=fjerDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); JERDown2H=fjerDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
BBTagUp1H=fbbtagUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); BBTagUp2H=fbbtagUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
BBTagDown1H=fbbtagDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); BBTagDown2H=fbbtagDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
massSmear1H=fSmear.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); massSmear2H=fSmear.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
FastMassUp1H=fFastMassUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastMassUp2H=fFastMassUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
FastMassDown1H=fFastMassDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastMassDown2H=fFastMassDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
FastBBUp1H=fFastBBUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastBBUp2H=fFastBBUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
FastBBDown1H=fFastBBDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastBBDown2H=fFastBBDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
trigUp1H=ftrigUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); trigUp2H=ftrigUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
trigDown1H=ftrigDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); trigDown2H=ftrigDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
ScaleUp1H=fScaleUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); ScaleUp2H=fScaleUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
ScaleDown1H=fScaleDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); ScaleDown2H=fScaleDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))


MCStat2H=Nom2H.Clone("MCStat2H");MCStat2H.Reset();
METFastSyst2H=Nom2H.Clone("METFastSyst2H");METFastSyst2H.Reset();
for i in range(1, Nom2H.GetNbinsX()+1):
	StatErr=Nom2H.GetBinError(i);
	if StatErr<=0 or Nom2H.GetBinContent(i)<=0:StatErr=1.0; #changed these for the range plot
	else: StatErr=(StatErr/Nom2H.GetBinContent(i)+1.0) #changed these for the range plot
	MCStat2H.SetBinContent(i, StatErr);
	yAvg = (Nom2H.GetBinContent(i)+NomGen_2H.GetBinContent(i))/2
	if yAvg==0.0: fastMETBin=1.0
	else: fastMETBin = abs(Nom2H.GetBinContent(i)-NomGen_2H.GetBinContent(i))/(2*yAvg)
	METFastSyst2H.SetBinContent(i,fastMETBin+1.0)


MCStat1H=Nom1H.Clone("MCStat1H");MCStat1H.Reset();
METFastSyst1H=Nom1H.Clone("METFastSyst1H");METFastSyst1H.Reset();
for i in range(1, Nom1H.GetNbinsX()+1):
	StatErr=Nom1H.GetBinError(i);
	if StatErr<=0 or Nom1H.GetBinContent(i)<=0:StatErr=1.0;
	else: StatErr=(StatErr/Nom1H.GetBinContent(i)+1.0)
	MCStat1H.SetBinContent(i, StatErr);
	yAvg = (Nom1H.GetBinContent(i)+NomGen_1H.GetBinContent(i))/2
	if yAvg==0.0: fastMETBin=1.0
	else: fastMETBin = abs(Nom1H.GetBinContent(i)-NomGen_1H.GetBinContent(i))/(2*yAvg)
	METFastSyst1H.SetBinContent(i,1.0+fastMETBin)


#BoostedOnly
fcard=open("SingleDatacardTemplate.txt", 'r');
fcardout=open("SRMerge_%s%s%d_LSP%d_%s.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
for line in fcard:
	if "observation" in line:
		newline="observation %.4f %.4f %.4f %.4f %.4f %.4f" %(sumRegionA2.GetBinContent(1),sumRegionA2.GetBinContent(2),sumRegionA2.GetBinContent(3)+sumRegionA2.GetBinContent(4),sumRegionA1.GetBinContent(1),sumRegionA1.GetBinContent(2),sumRegionA1.GetBinContent(3)+sumRegionA1.GetBinContent(4))
		fcardout.write(newline);
	elif "rate" in line and not "rateParam" in line:
		newline="rate "
		newline=newline+" %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 \n"  %((SignalA2.GetBinContent(1)+SignalA2_gen.GetBinContent(1))/2,(SignalA2.GetBinContent(2)+SignalA2_gen.GetBinContent(2))/2, (SignalA2.GetBinContent(3)+SignalA2_gen.GetBinContent(3))/2,(SignalA1.GetBinContent(1)+SignalA1_gen.GetBinContent(1))/2,(SignalA1.GetBinContent(2)+SignalA1_gen.GetBinContent(2))/2, (SignalA1.GetBinContent(3)+SignalA1_gen.GetBinContent(3))/2)
		fcardout.write(newline);
	elif "TrigSyst" in line:
		newline="TrigSyst lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,trigDown2H,1),systBinContent(Nom2H,trigUp2H,1), systBinContent(Nom2H,trigDown2H,2),systBinContent(Nom2H,trigUp2H,2),systBinContent(Nom2H,trigDown2H,3),systBinContent(Nom2H,trigUp2H,3), systBinContent(Nom1H,trigDown1H,1),systBinContent(Nom1H,trigUp1H,1), systBinContent(Nom1H,trigDown1H,2),systBinContent(Nom1H,trigUp1H,2),systBinContent(Nom1H,trigDown1H,3),systBinContent(Nom1H,trigUp1H,3))
		fcardout.write(newline);
	elif "ISRSyst" in line:
		newline="ISRSyst lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ISRDown2H,1),systBinContent(Nom2H,ISRUp2H,1), systBinContent(Nom2H,ISRDown2H,2),systBinContent(Nom2H,ISRUp2H,2),systBinContent(Nom2H,ISRDown2H,3),systBinContent(Nom2H,ISRUp2H,3), systBinContent(Nom1H,ISRDown1H,1),systBinContent(Nom1H,ISRUp1H,1), systBinContent(Nom1H,ISRDown1H,2),systBinContent(Nom1H,ISRUp1H,2),systBinContent(Nom1H,ISRDown1H,3),systBinContent(Nom1H,ISRUp1H,3))
		fcardout.write(newline);
	elif "SignalStat2H" in line:
		line="SignalStat1_2H lnN %.4f - - - - - - - - - - - \n" %(MCStat2H.GetBinContent(1))
		fcardout.write(line)
		line="SignalStat2_2H lnN - - %.4f - - - - - - - - - \n" %(MCStat2H.GetBinContent(2))
		fcardout.write(line)
		line="SignalStat3_2H lnN - - - - %.4f - - - - - - - \n" %(MCStat2H.GetBinContent(3))
		fcardout.write(line)
		continue
	elif "SignalStat1H" in line:
		line="SignalStat1_1H lnN - - - - - - %.4f - - - - -\n" %(MCStat1H.GetBinContent(1))
		fcardout.write(line)
		line="SignalStat2_1H lnN - - - - - - - - %.4f - - -\n" %(MCStat1H.GetBinContent(2))
		fcardout.write(line)
		line="SignalStat3_1H lnN - - - - - - - - - - %.4f -\n" %(MCStat1H.GetBinContent(3))
		fcardout.write(line)
		continue
	elif "SignalJEC" in line:
		newline="SignalJEC lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JECDown2H,1),systBinContent(Nom2H,JECUp2H,1), systBinContent(Nom2H,JECDown2H,2),systBinContent(Nom2H,JECUp2H,2),systBinContent(Nom2H,JECDown2H,3),systBinContent(Nom2H,JECUp2H,3), systBinContent(Nom1H,JECDown1H,1),systBinContent(Nom1H,JECUp1H,1), systBinContent(Nom1H,JECDown1H,2),systBinContent(Nom1H,JECUp1H,2),systBinContent(Nom1H,JECDown1H,3),systBinContent(Nom1H,JECUp1H,3))
		fcardout.write(newline);
	elif "SignalJER" in line:
		newline="SignalJER lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JERDown2H,1),systBinContent(Nom2H,JERUp2H,1), systBinContent(Nom2H,JERDown2H,2),systBinContent(Nom2H,JERUp2H,2),systBinContent(Nom2H,JERDown2H,3),systBinContent(Nom2H,JERUp2H,3), systBinContent(Nom1H,JERDown1H,1),systBinContent(Nom1H,JERUp1H,1), systBinContent(Nom1H,JERDown1H,2),systBinContent(Nom1H,JERUp1H,2),systBinContent(Nom1H,JERDown1H,3),systBinContent(Nom1H,JERUp1H,3))
		fcardout.write(newline);
	elif "SignalRes" in line:
		newline="SignalRes lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(systBinContent(Nom2H,massSmear2H,1),systBinContent(Nom2H,massSmear2H,2),systBinContent(Nom2H,massSmear2H,3), systBinContent(Nom1H,massSmear1H,1),systBinContent(Nom1H,massSmear1H,2),systBinContent(Nom1H,massSmear1H,3))
		fcardout.write(newline);
	elif "SignalPrefire" in line:
		newline="SignalPrefire lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PreDown2H,1),systBinContent(Nom2H,PreUp2H,1), systBinContent(Nom2H,PreDown2H,2),systBinContent(Nom2H,PreUp2H,2),systBinContent(Nom2H,PreDown2H,3),systBinContent(Nom2H,PreUp2H,3), systBinContent(Nom1H,PreDown1H,1),systBinContent(Nom1H,PreUp1H,1), systBinContent(Nom1H,PreDown1H,2),systBinContent(Nom1H,PreUp1H,2),systBinContent(Nom1H,PreDown1H,3),systBinContent(Nom1H,PreUp1H,3))
		fcardout.write(newline);
	elif "SignalPU" in line:
		newline="SignalPU lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PUDown2H,1),systBinContent(Nom2H,PUUp2H,1), systBinContent(Nom2H,PUDown2H,2),systBinContent(Nom2H,PUUp2H,2),systBinContent(Nom2H,PUDown2H,3),systBinContent(Nom2H,PUUp2H,3), systBinContent(Nom1H,PUDown1H,1),systBinContent(Nom1H,PUUp1H,1), systBinContent(Nom1H,PUDown1H,2),systBinContent(Nom1H,PUUp1H,2),systBinContent(Nom1H,PUDown1H,3),systBinContent(Nom1H,PUUp1H,3))
		fcardout.write(newline);
	elif "SignalScale" in line:
		newline="SignalScale lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ScaleDown2H,1),systBinContent(Nom2H,ScaleUp2H,1), systBinContent(Nom2H,ScaleDown2H,2),systBinContent(Nom2H,ScaleUp2H,2),systBinContent(Nom2H,ScaleDown2H,3),systBinContent(Nom2H,ScaleUp2H,3), systBinContent(Nom1H,ScaleDown1H,1),systBinContent(Nom1H,ScaleUp1H,1), systBinContent(Nom1H,ScaleDown1H,2), systBinContent(Nom1H,ScaleUp1H,2), systBinContent(Nom1H,ScaleDown1H,3), systBinContent(Nom1H,ScaleUp1H,3))
		fcardout.write(newline);
	elif "SignalBBTag" in line:
		newline="SignalBBTag lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,BBTagDown2H,1), systBinContent(Nom2H,BBTagUp2H,1), systBinContent(Nom2H,BBTagDown2H,2), systBinContent(Nom2H,BBTagUp2H,2),systBinContent(Nom2H,BBTagDown2H,3),systBinContent(Nom2H,BBTagUp2H,3), systBinContent(Nom1H,BBTagDown1H,1),systBinContent(Nom1H,BBTagUp1H,1), systBinContent(Nom1H,BBTagDown1H,2), systBinContent(Nom1H,BBTagUp1H,2), systBinContent(Nom1H,BBTagDown1H,3), systBinContent(Nom1H,BBTagUp1H,3))
		fcardout.write(newline);
	elif "SignalFastBBTag" in line:
		newline="SignalFastBBTag lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastBBDown2H,1),systBinContent(Nom2H,FastBBUp2H,1), systBinContent(Nom2H,FastBBDown2H,2),systBinContent(Nom2H,FastBBUp2H,2),systBinContent(Nom2H,FastBBDown2H,3),systBinContent(Nom2H,FastBBUp2H,3), systBinContent(Nom1H,FastBBDown1H,1),systBinContent(Nom1H,FastBBUp1H,1), systBinContent(Nom1H,FastBBDown1H,2),systBinContent(Nom1H,FastBBUp1H,2),systBinContent(Nom1H,FastBBDown1H,3),systBinContent(Nom1H,FastBBUp1H,3))
		fcardout.write(newline);
	elif "SignalFastMass" in line:
		newline="SignalFastMass lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastMassDown2H,1),systBinContent(Nom2H,FastMassUp2H,1), systBinContent(Nom2H,FastMassDown2H,2), systBinContent(Nom2H,FastMassUp2H,2), systBinContent(Nom2H,FastMassDown2H,3),systBinContent(Nom2H,FastMassUp2H,3), systBinContent(Nom1H,FastBBDown1H,1),systBinContent(Nom1H,FastBBUp1H,1), systBinContent(Nom1H,FastBBDown1H,2), systBinContent(Nom1H,FastBBUp1H,2), systBinContent(Nom1H,FastBBDown1H,3), systBinContent(Nom1H,FastBBUp1H,3))
		fcardout.write(newline);
	elif "SignalMETFastSIM" in line:
		newline="SignalMETFastSIM lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(METFastSyst2H.GetBinContent(1),METFastSyst2H.GetBinContent(2),METFastSyst2H.GetBinContent(3), METFastSyst1H.GetBinContent(1),METFastSyst1H.GetBinContent(2),METFastSyst1H.GetBinContent(3))
		fcardout.write(newline);
	else: fcardout.write(line);
fcardout.close()
fcard.close()

fcardout=open("CRBMerge_%s%s%d_LSP%d_%s.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
fcard=open("SingleBTemplate.txt", 'r');
for line in fcard:
	if "observation" in line:
		newline="observation %.4f %.4f " %(sumRegionB2, sumRegionB1)
		fcardout.write(newline);
	elif "rate" in line and not "rateParam" in line:
		newline="rate "
		newline=newline +"%.4f 1.0 %.4f 1.0 \n" %((SignalB2.Integral(1,5)+SignalB2_gen.Integral(1,5))/2, (SignalB1.Integral(1,5)+SignalB1_gen.Integral(1,5))/2)
		fcardout.write(newline);
	elif "Beta_2H rateParam" in line:
		newline="Beta_2H rateParam SidebandRegion2H bkg %.4f [0.000,%.4f] \n" %(sumRegionB2, sumRegionB2*10.0)
		fcardout.write(newline);
	elif "Beta_1H rateParam" in line:
		newline="Beta_1H rateParam SidebandRegion1H bkg %.4f [0.000,%.4f] \n" %(sumRegionB1, sumRegionB1*10.0)
		fcardout.write(newline);
	else: fcardout.write(line);
fcardout.close()

fcardout=open("CRCMerge_%s%s%d_LSP%d_%s.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
fcard=open("RegionCTemplateScaleMergeBkg.txt", 'r');
for line in fcard:
	if "observation" in line:
		newline="observation %.4f " %(sumRegionC)
		fcardout.write(newline);
	elif "rate" in line and not "rateParam" in line:
		newline="rate "
		newline=newline +"%.4f 1.0  \n" %((SignalC.Integral(1,5)+SignalC_gen.Integral(1,5))/2)
		fcardout.write(newline);
	elif "rateParam" in line:
		newline="Gamma rateParam AntitagRegion0 bkg %.4f [0.000,%.4f] \n" %(sumRegionC, sumRegionC*10.0)
		fcardout.write(newline);
	else: fcardout.write(line);
fcardout.close()

fcardout=open("CRDMerge_%s%s%d_LSP%d_%s.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
fcard=open("RegionDTemplateScaleMergeBkg.txt", 'r');
for line in fcard:
	if "observation" in line:
		newline="observation %.4f " %(sumRegionD)
		fcardout.write(newline);
	elif "rate" in line and not "rateParam" in line:
		newline="rate "
		newline=newline +"%.4f 1.0 \n" %((SignalD.Integral(1,5)+SignalD_gen.Integral(1,5))/2)
		fcardout.write(newline);
	elif "rateParam" in line:
		newline="Delta rateParam AntiSidebandRegion0 bkg %.4f [0.000,%.4f] \n" %(sumRegionD, sumRegionD*10.0)
		fcardout.write(newline);
	else: fcardout.write(line);
fcardout.close()

fcardout=open("CREMerge_%s%s%d_LSP%d_%s.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
fcard=open("METShapeTemplate.txt", 'r');
for line in fcard:
	if "observation" in line:
		newline="observation %.4f %.4f %.4f " %(bkgFrac_C.GetBinContent(1),bkgFrac_C.GetBinContent(2),bkgFrac_C.GetBinContent(3))
		fcardout.write(newline);
	elif "rate" in line and not "rateParam" in line:
		newline="rate "
		newline=newline +"%.4f 1.0 %.4f 1.0 %.4f 1.0 \n" %((SignalMET.GetBinContent(1)+SignalMET_gen.GetBinContent(1))/2,(SignalMET.GetBinContent(2)+SignalMET_gen.GetBinContent(2))/2,(SignalMET.GetBinContent(3)+SignalMET_gen.GetBinContent(3))/2)
		fcardout.write(newline);
	elif "Epsilon1 rateParam" in line:
		newline="Epsilon1 rateParam METShape_1 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_C.GetBinContent(1), bkgFrac_C.GetBinContent(1)*10.0)
		fcardout.write(newline);
	elif "Epsilon2 rateParam" in line:
		newline="Epsilon2 rateParam METShape_2 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_C.GetBinContent(2), bkgFrac_C.GetBinContent(2)*10.0)
		fcardout.write(newline);
	elif "Epsilon3 rateParam" in line:
		newline="Epsilon3 rateParam METShape_3 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_C.GetBinContent(3), bkgFrac_C.GetBinContent(3)*10.0)
		fcardout.write(newline);
	else: fcardout.write(line);
fcardout.close()

##########################################
#Boosted w/ veto
##########################################
if runVeto==True:
	fcard=open("SingleDatacardTemplate.txt", 'r');
	fcardout=open("SRMerge_%s%s%d_LSP%d_%s_veto.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
	for line in fcard:
		if "observation" in line:
			newline="observation %.4f %.4f %.4f %.4f %.4f %.4f" %(sumRegionA2Veto.GetBinContent(1),sumRegionA2Veto.GetBinContent(2),sumRegionA2Veto.GetBinContent(3)+sumRegionA2Veto.GetBinContent(4),sumRegionA1Veto.GetBinContent(1),sumRegionA1Veto.GetBinContent(2),sumRegionA1Veto.GetBinContent(3)+sumRegionA1Veto.GetBinContent(4))
			fcardout.write(newline);
		elif "rate" in line and not "rateParam" in line:
			newline="rate "
			newline=newline+" %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 \n" %((SignalA2Veto.GetBinContent(1)+SignalA2Veto_gen.GetBinContent(1))/2,(SignalA2Veto.GetBinContent(2)+SignalA2Veto_gen.GetBinContent(2))/2,(SignalA2Veto.GetBinContent(3)+SignalA2Veto_gen.GetBinContent(3))/2,(SignalA1Veto.GetBinContent(1)+SignalA1Veto_gen.GetBinContent(1))/2, (SignalA1Veto.GetBinContent(2)+SignalA1Veto_gen.GetBinContent(2))/2, (SignalA1Veto.GetBinContent(3)+SignalA1Veto_gen.GetBinContent(3))/2)
			fcardout.write(newline);
		elif "TrigSyst" in line:
			newline="TrigSyst lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,trigDown2H,1),systBinContent(Nom2H,trigUp2H,1), systBinContent(Nom2H,trigDown2H,2),systBinContent(Nom2H,trigUp2H,2),systBinContent(Nom2H,trigDown2H,3),systBinContent(Nom2H,trigUp2H,3), systBinContent(Nom1H,trigDown1H,1),systBinContent(Nom1H,trigUp1H,1), systBinContent(Nom1H,trigDown1H,2),systBinContent(Nom1H,trigUp1H,2),systBinContent(Nom1H,trigDown1H,3),systBinContent(Nom1H,trigUp1H,3))
			fcardout.write(newline);
		elif "ISRSyst" in line:
			newline="ISRSyst lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ISRDown2H,1),systBinContent(Nom2H,ISRUp2H,1), systBinContent(Nom2H,ISRDown2H,2),systBinContent(Nom2H,ISRUp2H,2),systBinContent(Nom2H,ISRDown2H,3),systBinContent(Nom2H,ISRUp2H,3), systBinContent(Nom1H,ISRDown1H,1),systBinContent(Nom1H,ISRUp1H,1), systBinContent(Nom1H,ISRDown1H,2),systBinContent(Nom1H,ISRUp1H,2),systBinContent(Nom1H,ISRDown1H,3),systBinContent(Nom1H,ISRUp1H,3))
			fcardout.write(newline);
		elif "SignalStat2H" in line:
			line="SignalStat1_2H lnN %.4f - - - - - - - - - - - \n" %(MCStat2H.GetBinContent(1))
			fcardout.write(line)
			line="SignalStat2_2H lnN - - %.4f - - - - - - - - - \n" %(MCStat2H.GetBinContent(2))
			fcardout.write(line)
			line="SignalStat3_2H lnN - - - - %.4f - - - - - - - \n" %(MCStat2H.GetBinContent(3))
			fcardout.write(line)
			continue
		elif "SignalStat1H" in line:
			line="SignalStat1_1H lnN - - - - - - %.4f - - - - -\n" %(MCStat1H.GetBinContent(1))
			fcardout.write(line)
			line="SignalStat2_1H lnN - - - - - - - - %.4f - - -\n" %(MCStat1H.GetBinContent(2))
			fcardout.write(line)
			line="SignalStat3_1H lnN - - - - - - - - - - %.4f -\n" %(MCStat1H.GetBinContent(3))
			fcardout.write(line)
			continue
		elif "SignalJEC" in line:
			newline="SignalJEC lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JECDown2H,1),systBinContent(Nom2H,JECUp2H,1), systBinContent(Nom2H,JECDown2H,2),systBinContent(Nom2H,JECUp2H,2),systBinContent(Nom2H,JECDown2H,3),systBinContent(Nom2H,JECUp2H,3), systBinContent(Nom1H,JECDown1H,1),systBinContent(Nom1H,JECUp1H,1), systBinContent(Nom1H,JECDown1H,2),systBinContent(Nom1H,JECUp1H,2),systBinContent(Nom1H,JECDown1H,3),systBinContent(Nom1H,JECUp1H,3))
			fcardout.write(newline);
		elif "SignalJER" in line:
			newline="SignalJER lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JERDown2H,1),systBinContent(Nom2H,JERUp2H,1), systBinContent(Nom2H,JERDown2H,2),systBinContent(Nom2H,JERUp2H,2),systBinContent(Nom2H,JERDown2H,3),systBinContent(Nom2H,JERUp2H,3), systBinContent(Nom1H,JERDown1H,1),systBinContent(Nom1H,JERUp1H,1), systBinContent(Nom1H,JERDown1H,2),systBinContent(Nom1H,JERUp1H,2),systBinContent(Nom1H,JERDown1H,3),systBinContent(Nom1H,JERUp1H,3))
			fcardout.write(newline);
		elif "SignalRes" in line:
			newline="SignalRes lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(systBinContent(Nom2H,massSmear2H,1),systBinContent(Nom2H,massSmear2H,2),systBinContent(Nom2H,massSmear2H,3), systBinContent(Nom1H,massSmear1H,1),systBinContent(Nom1H,massSmear1H,2),systBinContent(Nom1H,massSmear1H,3))
			fcardout.write(newline);
		elif "SignalPrefire" in line:
			newline="SignalPrefire lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PreDown2H,1),systBinContent(Nom2H,PreUp2H,1), systBinContent(Nom2H,PreDown2H,2),systBinContent(Nom2H,PreUp2H,2),systBinContent(Nom2H,PreDown2H,3),systBinContent(Nom2H,PreUp2H,3), systBinContent(Nom1H,PreDown1H,1),systBinContent(Nom1H,PreUp1H,1), systBinContent(Nom1H,PreDown1H,2),systBinContent(Nom1H,PreUp1H,2),systBinContent(Nom1H,PreDown1H,3),systBinContent(Nom1H,PreUp1H,3))
			fcardout.write(newline);
		elif "SignalPU" in line:
			newline="SignalPU lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PUDown2H,1),systBinContent(Nom2H,PUUp2H,1), systBinContent(Nom2H,PUDown2H,2),systBinContent(Nom2H,PUUp2H,2),systBinContent(Nom2H,PUDown2H,3),systBinContent(Nom2H,PUUp2H,3), systBinContent(Nom1H,PUDown1H,1),systBinContent(Nom1H,PUUp1H,1), systBinContent(Nom1H,PUDown1H,2),systBinContent(Nom1H,PUUp1H,2),systBinContent(Nom1H,PUDown1H,3),systBinContent(Nom1H,PUUp1H,3))
			fcardout.write(newline);
		elif "SignalScale" in line:
			newline="SignalScale lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ScaleDown2H,1),systBinContent(Nom2H,ScaleUp2H,1), systBinContent(Nom2H,ScaleDown2H,2),systBinContent(Nom2H,ScaleUp2H,2),systBinContent(Nom2H,ScaleDown2H,3),systBinContent(Nom2H,ScaleUp2H,3), systBinContent(Nom1H,ScaleDown1H,1),systBinContent(Nom1H,ScaleUp1H,1), systBinContent(Nom1H,ScaleDown1H,2), systBinContent(Nom1H,ScaleUp1H,2), systBinContent(Nom1H,ScaleDown1H,3), systBinContent(Nom1H,ScaleUp1H,3))
			fcardout.write(newline);
		elif "SignalBBTag" in line:
			newline="SignalBBTag lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,BBTagDown2H,1), systBinContent(Nom2H,BBTagUp2H,1), systBinContent(Nom2H,BBTagDown2H,2), systBinContent(Nom2H,BBTagUp2H,2),systBinContent(Nom2H,BBTagDown2H,3),systBinContent(Nom2H,BBTagUp2H,3), systBinContent(Nom1H,BBTagDown1H,1),systBinContent(Nom1H,BBTagUp1H,1), systBinContent(Nom1H,BBTagDown1H,2), systBinContent(Nom1H,BBTagUp1H,2), systBinContent(Nom1H,BBTagDown1H,3), systBinContent(Nom1H,BBTagUp1H,3))
			fcardout.write(newline);
		elif "SignalFastBBTag" in line:
			newline="SignalFastBBTag lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastBBDown2H,1),systBinContent(Nom2H,FastBBUp2H,1), systBinContent(Nom2H,FastBBDown2H,2),systBinContent(Nom2H,FastBBUp2H,2),systBinContent(Nom2H,FastBBDown2H,3),systBinContent(Nom2H,FastBBUp2H,3), systBinContent(Nom1H,FastBBDown1H,1),systBinContent(Nom1H,FastBBUp1H,1), systBinContent(Nom1H,FastBBDown1H,2),systBinContent(Nom1H,FastBBUp1H,2),systBinContent(Nom1H,FastBBDown1H,3),systBinContent(Nom1H,FastBBUp1H,3))
			fcardout.write(newline);
		elif "SignalFastMass" in line:
			newline="SignalFastMass lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastMassDown2H,1),systBinContent(Nom2H,FastMassUp2H,1), systBinContent(Nom2H,FastMassDown2H,2), systBinContent(Nom2H,FastMassUp2H,2), systBinContent(Nom2H,FastMassDown2H,3),systBinContent(Nom2H,FastMassUp2H,3), systBinContent(Nom1H,FastBBDown1H,1),systBinContent(Nom1H,FastBBUp1H,1), systBinContent(Nom1H,FastBBDown1H,2), systBinContent(Nom1H,FastBBUp1H,2), systBinContent(Nom1H,FastBBDown1H,3), systBinContent(Nom1H,FastBBUp1H,3))
			fcardout.write(newline);
		elif "SignalMETFastSIM" in line:
			newline="SignalMETFastSIM lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(METFastSyst2H.GetBinContent(1),METFastSyst2H.GetBinContent(2),METFastSyst2H.GetBinContent(3), METFastSyst1H.GetBinContent(1),METFastSyst1H.GetBinContent(2),METFastSyst1H.GetBinContent(3))
			fcardout.write(newline);
		else: fcardout.write(line);
	fcardout.close()
	fcard.close()

	fcardout=open("CRBMerge_%s%s%d_LSP%d_%s_veto.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
	fcard=open("SingleBTemplate.txt", 'r');
	for line in fcard:
		if "observation" in line:
			newline="observation %.4f %.4f " %(sumRegionB2Veto, sumRegionB1Veto)
			fcardout.write(newline);
		elif "rate" in line and not "rateParam" in line:
			newline="rate "
			newline=newline +"%.4f 1.0 %.4f 1.0 \n" %((SignalB2Veto.Integral(1,5)+SignalB2Veto_gen.Integral(1,5))/2, (SignalB1Veto.Integral(1,5)+SignalB1Veto_gen.Integral(1,5))/2)
			fcardout.write(newline);
		elif "Beta_2H rateParam" in line:
			newline="Beta_2H rateParam SidebandRegion2H bkg %.4f [0.000,%.4f] \n" %(sumRegionB2Veto, sumRegionB2Veto*10.0)
			fcardout.write(newline);
		elif "Beta_1H rateParam" in line:
			newline="Beta_1H rateParam SidebandRegion1H bkg %.4f [0.000,%.4f] \n" %(sumRegionB1Veto, sumRegionB1Veto*10.0)
			fcardout.write(newline);
		else: fcardout.write(line);
	fcardout.close()

	fcardout=open("CRCMerge_%s%s%d_LSP%d_%s_veto.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
	fcard=open("RegionCTemplateScaleMergeBkg.txt", 'r');
	for line in fcard:
		if "observation" in line:
			newline="observation %.4f " %(sumRegionCVeto)
			fcardout.write(newline);
		elif "rate" in line and not "rateParam" in line:
			newline="rate "
			newline=newline +"%.4f 1.0  \n" %((SignalCVeto.Integral(1,5)+SignalCVeto_gen.Integral(1,5))/2)
			fcardout.write(newline);
		elif "rateParam" in line:
			newline="Gamma rateParam AntitagRegion0 bkg %.4f [0.000,%.4f] \n" %(sumRegionCVeto, sumRegionCVeto*10.0)
			fcardout.write(newline);
		else:
			fcardout.write(line);
	fcardout.close()

	fcardout=open("CRDMerge_%s%s%d_LSP%d_%s_veto.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
	fcard=open("RegionDTemplateScaleMergeBkg.txt", 'r');
	for line in fcard:
		if "observation" in line:
			newline="observation %.4f " %(sumRegionDVeto)
			fcardout.write(newline);
		elif "rate" in line and not "rateParam" in line:
			newline="rate "
			newline=newline +"%.4f 1.0 \n" %((SignalDVeto.Integral(1,5)+SignalDVeto_gen.Integral(1,5))/2)
			fcardout.write(newline);
		elif "rateParam" in line:
			newline="Delta rateParam AntiSidebandRegion0 bkg %.4f [0.000,%.4f] \n" %(sumRegionDVeto, sumRegionDVeto*10.0)
			fcardout.write(newline);
		else: fcardout.write(line);
	fcardout.close()

	fcardout=open("CREMerge_%s%s%d_LSP%d_%s_veto.txt" %(whichDString,signal,hino,LSP,dataMCString) , 'w')
	fcard=open("METShapeTemplate.txt", 'r');
	for line in fcard:
		if "observation" in line:
			newline="observation %.4f %.4f %.4f " %(bkgFrac_CVeto.GetBinContent(1),bkgFrac_CVeto.GetBinContent(2),bkgFrac_CVeto.GetBinContent(3))
			fcardout.write(newline);
		elif "rate" in line and not "rateParam" in line:
			newline="rate "
			newline=newline +"%.4f 1.0 %.4f 1.0 %.4f 1.0 \n" %((SignalMETVeto.GetBinContent(1)+SignalMETVeto_gen.GetBinContent(1))/2,(SignalMETVeto.GetBinContent(2)+SignalMETVeto_gen.GetBinContent(2))/2,(SignalMETVeto.GetBinContent(3)+SignalMETVeto_gen.GetBinContent(3))/2)
			fcardout.write(newline);
		elif "Epsilon1 rateParam" in line:
			newline="Epsilon1 rateParam METShape_1 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_CVeto.GetBinContent(1), bkgFrac_CVeto.GetBinContent(1)*10.0)
			fcardout.write(newline);
		elif "Epsilon2 rateParam" in line:
			newline="Epsilon2 rateParam METShape_2 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_CVeto.GetBinContent(2), bkgFrac_CVeto.GetBinContent(2)*10.0)
			fcardout.write(newline);
		elif "Epsilon3 rateParam" in line:
			newline="Epsilon3 rateParam METShape_3 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_CVeto.GetBinContent(3), bkgFrac_CVeto.GetBinContent(3)*10.0)
			fcardout.write(newline);
		else: fcardout.write(line);
	fcardout.close()
########################################################################

#Boosted only
print("----------------BoostedOnly----------------")
os.system("combineCards.py SRMerge_%s%s%d_LSP%d_%s.txt CRBMerge_%s%s%d_LSP%d_%s.txt CRCMerge_%s%s%d_LSP%d_%s.txt CRDMerge_%s%s%d_LSP%d_%s.txt CREMerge_%s%s%d_LSP%d_%s.txt > %s%s%d_LSP%d_BothBoostedH_%s.txt "%(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString))
os.system("combine -M AsymptoticLimits -n %s%s%d_LSP%d_BothBoostedH_%s %s%s%d_LSP%d_BothBoostedH_%s.txt " %(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString))

#Resolved only
if runResAlone==True:
	print("----------------ResOnly----------------")
	os.system("combineCards.py resData/%sTChiHH_%s/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_ResOnly.txt "%(whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,LSP,dataMCString))
	os.system("combine -M AsymptoticLimits -n %s%s%d_LSP%d_%s_ResOnly %s%s%d_LSP%d_%s_ResOnly.txt " %(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString))


#BoostedOnlyVeto
if runVeto==True:
	os.system("combineCards.py SRMerge_%s%s%d_LSP%d_%s_veto.txt CRBMerge_%s%s%d_LSP%d_%s_veto.txt CRCMerge_%s%s%d_LSP%d_%s_veto.txt CRDMerge_%s%s%d_LSP%d_%s_veto.txt CREMerge_%s%s%d_LSP%d_%s_veto.txt > %s%s%d_LSP%d_BothBoostedH_%s_veto.txt "%(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString))

	#resolved data cards don't end in "3" like ours do... which is good
	resLSP = LSP;
	if (resLSP==1): resLSP=0;
	if (resLSP%5!=0): resLSP=resLSP+2;
	os.system("combineCards.py %s%s%d_LSP%d_BothBoostedH_%s_veto.txt resData/%sTChiHH_%s/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_Combo.txt "%(whichDString,signal,hino,LSP,dataMCString,whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,LSP,dataMCString))

	print("----------------Combination----------------")
	os.system("combine -M AsymptoticLimits -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt " %(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString))


	# os.system("combine -M Significance --rMin=-10 --uncapped yes -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt " %(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString))
	# seed = int(TRandom3(0).Rndm()*100000.0)
	# print(seed)
	# os.system("combine -M GoodnessOfFit -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt  --algo=saturated --toysFreq -t 500 -s %i --fixedSignalStrength=1" %(whichDString,signal,hino,LSP,dataMCString,whichDString,signal,hino,LSP,dataMCString, seed))
