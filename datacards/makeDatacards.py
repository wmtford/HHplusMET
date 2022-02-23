import os
from ROOT import *
import sys

'''
Runs datacards for HH+MET search
Arguments for running all at the bottom
Can run for any model (TChiHH-G (1D), TChiHH (2D), T5HH) - haven't setup to run for 2D T5HH yet
Has capabilities to run combine, too, but I recommend using combineAndRunDatacards.py for that, since it runs faster
Below global arguments:
	inDIR = where the templates for datacards are located (also where this code should be located), also where resData (houses the resolved datacards) is located
	outDIR = where the outgoing datacards should be located
	baseDIR = where the histograms with the yields are located (also where all systematic files are located)
	runSysts = bool for whether or not the signal systematics should be run and included. Turn off if they haven't been made yet
	runMETavg = do the FastSIM recommended treatment for the reco MET (averaging the yields for everything between running with recoMET and running with genMET instead).
		Turn off if you haven't run genMET in ALPHABET
		Automatically doesn't run for the 1D T5HH sample, which is FullSIM
'''


inDIR = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/forGithub/CMSSW_10_2_13/src/HHplusMET/datacards/"
outDIR = inDIR
baseDir = "/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/"
runSysts = True
runMETavg = True

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
		#Set systematic to 100% uncertainty if the bin is empty (shouldn't do anything really, but consistent with resolved)
		if syst==0: syst = 2.00
		if syst<0:  syst=syst+2.0
	 	return syst;
  	else: return 2.0;


def getMassPoints(model):
	mass_points = []
	if model == "TChiHH":
		namesFile=open("../src/higgsino2DFileNames.txt", 'r'); #Just to grab the mass points of the scan
		for line in namesFile:
			x = line.split('_')
			hino_mass = int(x[5])
			LSP_mass = int(x[6])
			if hino_mass>810: break;
			mass_points.append([hino_mass, LSP_mass])
		#Add 1D scan
		for i in range(0, 27):
			hino_mass=150+i*25
			mass_points.append([hino_mass, 1])
	elif model == "TChiHH-G":
		mass_points = [[150+item*25, 1]for item in range(0, 55)]
	elif model == "T5HH":
		mass_points = [[1000+item*100,1] for item in range(0,16)]
	elif model == "T5HHextra":
		mass_points = [[1000,1], [1200,1], [1400,1], [1600,1], [1800,1]]
	return mass_points



#model = TChiHH-G, TChiHH, T5HH
#which = "boost"  "veto"  "only2H"-for EWK combine, runs vetoed boosted for 2H category only
def makeDatacards(model,which,data):
	mass_points = getMassPoints(model)

	#Naming conventions
	signal = model
	dataMCString ="MC"; whichDString ="1D"
	if data==True: dataMCString ="Data"
	if signal=="TChiHH": whichDString ="2D"
	if signal=="TChiHH-G": signal="TChiHH" #Yes, these will have the same name


	#Grab bkg files
	bkgFileName = ""
	if which=="boost":
		if data==True: bkgFileName = baseDir+"boosted_noVeto_Final/ALPHABET_0lData.root"
		else: bkgFileName = baseDir+"boosted_noVeto_Final/ALPHABET_0l.root"
	else:
		if data==True: bkgFileName = baseDir+"boosted_veto_Final/ALPHABET_0lData.root"
		else: bkgFileName = baseDir+"boosted_veto_Final/ALPHABET_0l.root"
	f=TFile(bkgFileName, "READ");
	filestr = "sum"
	if data==True: filestr = "data"
	sumRegionA2=f.Get("MET_doubletagSR_"+filestr); sumRegionA1=f.Get("MET_tagSR_"+filestr);
	sumRegionB2=f.Get("MET_doubletagSB_"+filestr).Integral(1,5); sumRegionB1=f.Get("MET_tagSB_"+filestr).Integral(1,5);
	sumRegionC=f.Get("MET_antitagSR_"+filestr).Integral(1,5); sumRegionD=f.Get("MET_antitagSB_"+filestr).Integral(1,5);
	bkgFrac_C=f.Get("MET_antitagOpt1_"+filestr); bkgFrac_C.SetBinContent(3,bkgFrac_C.GetBinContent(3)+bkgFrac_C.GetBinContent(4)); bkgFrac_C.SetBinContent(4,0.0);


	#Make bkg prediction
	bkgFracHisto = bkgFrac_C.Clone("bkgFracHisto")
	bkgFracHisto.Scale(1/bkgFracHisto.Integral(1,5))
	Predictions2H = f.Get("MET_doubletagSB_"+filestr); Predictions1H = f.Get("MET_tagSB_"+filestr) #just to set bin numbers
	norm2H=sumRegionB2*sumRegionC/sumRegionD
	norm1H=sumRegionB1*sumRegionC/sumRegionD
	for i in range(1,4):
		Predictions2H.SetBinContent(i, norm2H*bkgFracHisto.GetBinContent(i))
		Predictions1H.SetBinContent(i, norm1H*bkgFracHisto.GetBinContent(i))


	#Grab signal files (including gen file where appropriate for the average MET treatment for FastSIM)
	sigFileName = ""; sigGenFileName = ""
	if signal == "TChiHH":
		if which == "boost":
			sigFileName = baseDir+"boosted_noVeto_FastSIMSFs/ALPHABET_"+whichDString+"Signal.root"
			sigGenFileName = baseDir+"boosted_noVeto_FastSIMSFs_genMET/ALPHABET_"+whichDString+"Signal.root"
		else:
			sigFileName = baseDir+"boosted_veto_FastSIMSFs/ALPHABET_"+whichDString+"Signal.root"
			sigGenFileName = baseDir+"boosted_veto_FastSIMSFs_genMET/ALPHABET_"+whichDString+"Signal.root"
	elif signal == "T5HH":
		if which == "boost": sigFileName = baseDir+"T5HH1D_FullSIM/ALPHABET_1DT5HH_FullSIM.root"
		else: sigFileName = baseDir+"T5HH1D_FullSIM_veto/ALPHABET_1DSignal.root"
	if model == "T5HHextra":
		sigFileName = baseDir+"testing_mN2_T5HH/ALPHABET_1DSignalExtra.root"
	# else: print("Something is wrong with your signal...")

	f2=TFile(sigFileName, "READ")
	if signal=="TChiHH" and runMETavg==True: f2_genMET=TFile(sigGenFileName, "READ");


	#Grab systematic files
	if runSysts==True:
		sysDir = "boosted_noVeto_FinalSyst"
		thisFileName = "ALPHABET_"+whichDString+"Signal.root"
		if signal == "T5HH":
			sysDir = "T5HH_FullSIM_Syst"
			thisFileName = "ALPHABET_1DT5HH_FullSIM.root"

		#open nominal (and gen) file
		fNominalName = sigFileName; fNominalGenName=""
		if signal=="TChiHH": fNominalGenName = sigGenFileName
		#need to grab the no-veto version
		if which=="veto" or which=="only2H":
			fNominalName = baseDir+"/boosted_noVeto_FastSIMSFs/ALPHABET_"+whichDString+"Signal.root"
			if signal=="T5HH": fNominalName = baseDir+"/T5HH1D_FullSIM/ALPHABET_1DT5HH_FullSIM.root"
			if signal=="TChiHH": fNominalGenName = baseDir+"boosted_noVeto_FastSIMSFs_genMET/ALPHABET_"+whichDString+"Signal.root"
		fNominal=TFile(fNominalName, "READ");
		if signal=="TChiHH": fNominal_gen=TFile(fNominalGenName, "READ");

		#open syst files
		fISRUp=TFile(baseDir+sysDir+"/ISRUp/"+thisFileName, "READ")
		fISRDown=TFile(baseDir+sysDir+"/ISRDown/"+thisFileName, "READ")
		fSmear=TFile(baseDir+sysDir+"/massSmear/"+thisFileName, "READ")
		ftrigUp=TFile(baseDir+sysDir+"/trigUp/"+thisFileName, "READ")
		ftrigDown=TFile(baseDir+sysDir+"/trigDown/"+thisFileName, "READ")
		fpreUp=TFile(baseDir+sysDir+"/prefireUp/"+thisFileName, "READ")
		fpreDown=TFile(baseDir+sysDir+"/prefireDown/"+thisFileName, "READ")
		fpuUp=TFile(baseDir+sysDir+"/PUUp/"+thisFileName, "READ")
		fpuDown=TFile(baseDir+sysDir+"/PUDown/"+thisFileName, "READ")
		fbbtagUp=TFile(baseDir+sysDir+"/bbTagUp/"+thisFileName, "READ")
		fbbtagDown=TFile(baseDir+sysDir+"/bbTagDown/"+thisFileName, "READ")
		fjecUp=TFile(baseDir+sysDir+"/JECUp/"+thisFileName, "READ")
		fjecDown=TFile(baseDir+sysDir+"/JECDown/"+thisFileName, "READ")
		fjerUp=TFile(baseDir+sysDir+"/JERUp/"+thisFileName, "READ")
		fjerDown=TFile(baseDir+sysDir+"/JERDown/"+thisFileName, "READ")
		fScaleUp=TFile(baseDir+sysDir+"/scalesUp/"+thisFileName, "READ")
		fScaleDown=TFile(baseDir+sysDir+"/scalesDown/"+thisFileName, "READ")
		if signal=="TChiHH":
			fFastMassUp=TFile(baseDir+sysDir+"/massFastSIMUp/"+thisFileName, "READ")
			fFastMassDown=TFile(baseDir+sysDir+"/massFastSIMDown/"+thisFileName, "READ")
			fFastBBUp=TFile(baseDir+sysDir+"/bbFastSIMUp/"+thisFileName, "READ")
			fFastBBDown=TFile(baseDir+sysDir+"/bbFastSIMDown/"+thisFileName, "READ")


	#loop through mass points to write a datacard for each
	for mass_point in mass_points:
		hino=mass_point[0]; LSP=mass_point[1]
		finalLSP=LSP
		if (finalLSP==1): finalLSP=1;
		elif (finalLSP%5!=0): finalLSP=finalLSP+2;
		if model=="T5HHextra": signal="T5HH"

		print("\n\n\n-----------------------------------------------------")
		print("Signal: %s, looping over mass=%d, mLSP=%d now" %(model,hino,finalLSP))
		print("-----------------------------------------------------\n")
		SignalA2=f2.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalA1=f2.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP));
		SignalA2.SetBinContent(3,SignalA2.GetBinContent(3)+SignalA2.GetBinContent(4)); SignalA2.SetBinContent(4,0.0);
		SignalA1.SetBinContent(3,SignalA1.GetBinContent(3)+SignalA1.GetBinContent(4)); SignalA1.SetBinContent(4,0.0);
		SignalB2=f2.Get("MET_doubletagSB_%s%d_LSP%d" %(signal,hino,LSP)); SignalB1=f2.Get("MET_tagSB_%s%d_LSP%d" %(signal,hino,LSP));
		SignalC=f2.Get("MET_antitagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalD=f2.Get("MET_antitagSB_%s%d_LSP%d" %(signal,hino,LSP));
		SignalMET=f2.Get("MET_antitagOpt1_%s%d_LSP%d" %(signal,hino,LSP));
		SignalMET.SetBinContent(3,SignalMET.GetBinContent(3)+SignalMET.GetBinContent(4)); SignalMET.SetBinContent(4,0.0);

		#genMET where necessary
		if runMETavg and signal=="TChiHH":
			SignalA2_gen=f2_genMET.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalA1_gen=f2_genMET.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP));
			SignalA2_gen.SetBinContent(3,SignalA2_gen.GetBinContent(3)+SignalA2_gen.GetBinContent(4)); SignalA2_gen.SetBinContent(4,0.0);
			SignalA1_gen.SetBinContent(3,SignalA1_gen.GetBinContent(3)+SignalA1_gen.GetBinContent(4)); SignalA1_gen.SetBinContent(4,0.0);
			SignalB2_gen=f2_genMET.Get("MET_doubletagSB_%s%d_LSP%d" %(signal,hino,LSP)); SignalB1_gen=f2_genMET.Get("MET_tagSB_%s%d_LSP%d" %(signal,hino,LSP));
			SignalC_gen=f2_genMET.Get("MET_antitagSR_%s%d_LSP%d" %(signal,hino,LSP)); SignalD_gen=f2_genMET.Get("MET_antitagSB_%s%d_LSP%d" %(signal,hino,LSP));
			SignalMET_gen=f2_genMET.Get("MET_antitagOpt1_%s%d_LSP%d" %(signal,hino,LSP));
			SignalMET_gen.SetBinContent(3,SignalMET_gen.GetBinContent(3)+SignalMET_gen.GetBinContent(4)); SignalMET_gen.SetBinContent(4,0.0);


		#grab systematics histos
		if runSysts==True:
			Nom1H=fNominal.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); Nom2H=fNominal.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
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
			trigUp1H=ftrigUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); trigUp2H=ftrigUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
			trigDown1H=ftrigDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); trigDown2H=ftrigDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
			ScaleUp1H=fScaleUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); ScaleUp2H=fScaleUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
			ScaleDown1H=fScaleDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); ScaleDown2H=fScaleDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
			if signal == "TChiHH":
				NomGen_1H=fNominal_gen.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); NomGen_2H=fNominal_gen.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
				FastMassUp1H=fFastMassUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastMassUp2H=fFastMassUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
				FastMassDown1H=fFastMassDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastMassDown2H=fFastMassDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
				FastBBUp1H=fFastBBUp.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastBBUp2H=fFastBBUp.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))
				FastBBDown1H=fFastBBDown.Get("MET_tagSR_%s%d_LSP%d" %(signal,hino,LSP)); FastBBDown2H=fFastBBDown.Get("MET_doubletagSR_%s%d_LSP%d" %(signal,hino,LSP))

			#Make MC stat and MET averaging systematics
			MCStat2H=Nom2H.Clone("MCStat2H"); MCStat2H.Reset();
			if signal=="TChiHH":
				METFastSyst2H=Nom2H.Clone("METFastSyst2H");
				METFastSyst2H.Reset();
			for i in range(1, Nom2H.GetNbinsX()+1):
				StatErr=Nom2H.GetBinError(i);
				if StatErr<=0 or Nom2H.GetBinContent(i)<=0:StatErr=1.0; #changed these for the range plot
				else: StatErr=(StatErr/Nom2H.GetBinContent(i)+1.0) #changed these for the range plot
				MCStat2H.SetBinContent(i, StatErr);
				if signal=="TChiHH":
					yAvg = (Nom2H.GetBinContent(i)+NomGen_2H.GetBinContent(i))/2
					if yAvg==0.0: fastMETBin=1.0
					else: fastMETBin = abs(Nom2H.GetBinContent(i)-NomGen_2H.GetBinContent(i))/(2*yAvg)
					METFastSyst2H.SetBinContent(i,fastMETBin+1.0)

			MCStat1H=Nom1H.Clone("MCStat1H"); MCStat1H.Reset();
			if signal=="TChiHH":
				METFastSyst1H=Nom1H.Clone("METFastSyst1H");
				METFastSyst1H.Reset();
			for i in range(1, Nom1H.GetNbinsX()+1):
				StatErr=Nom1H.GetBinError(i);
				if StatErr<=0 or Nom1H.GetBinContent(i)<=0:StatErr=1.0;
				else: StatErr=(StatErr/Nom1H.GetBinContent(i)+1.0)
				MCStat1H.SetBinContent(i, StatErr);
				if signal=="TChiHH":
					yAvg = (Nom1H.GetBinContent(i)+NomGen_1H.GetBinContent(i))/2
					if yAvg==0.0: fastMETBin=1.0
					else: fastMETBin = abs(Nom1H.GetBinContent(i)-NomGen_1H.GetBinContent(i))/(2*yAvg)
					METFastSyst1H.SetBinContent(i,1.0+fastMETBin)


		#Grab datacard template names and start output names
		tempNameA = inDIR+"SingleDatacardTemplate.txt"
		tempNameB = inDIR+"SingleBTemplate.txt"
		tempNameC = inDIR+"RegionCTemplateScaleMergeBkg.txt"
		tempNameD = inDIR+"RegionDTemplateScaleMergeBkg.txt"
		tempNameE = inDIR+"METShapeTemplate.txt"
		vetoStr = ""
		if which=="veto" or which=="only2H": vetoStr="_veto"
		endCardName ="%s%s%d_LSP%d_%s%s.txt" %(whichDString,signal,hino,finalLSP,dataMCString,vetoStr)
		cardAName = "SRMerge_"+endCardName
		cardBName = "CRBMerge_"+endCardName
		cardCName = "CRCMerge_"+endCardName
		cardDName = "CRDMerge_"+endCardName
		cardEName = "CREMerge_"+endCardName
		if which == "only2H":
			tempNameA = inDIR+"A2DatacardTemplate.txt"
			tempNameB = inDIR+"B2DatacardTemplate.txt"
			cardAName = "SR2Only_"+endCardName
			cardBName = "CRB2Only_"+endCardName


		#start writing datacards - read template and replace lines where necessary
		#Again, only2H is for the EWKCombine only
		if which=="only2H":
			fcard=open(tempNameA, 'r'); fcardout=open(cardAName, 'w')
			for line in fcard:
				if "observation" in line:
					newline="observation %.4f %.4f %.4f" %(sumRegionA2.GetBinContent(1),sumRegionA2.GetBinContent(2),sumRegionA2.GetBinContent(3)+sumRegionA2.GetBinContent(4))
				elif "rate" in line and not "rateParam" in line:
					newline="rate "
					if runMETavg and signal=="TChiHH": newline=newline+" %.4f 1.0 %.4f 1.0 %.4f 1.0 \n"  %((SignalA2.GetBinContent(1)+SignalA2_gen.GetBinContent(1))/2,(SignalA2.GetBinContent(2)+SignalA2_gen.GetBinContent(2))/2, (SignalA2.GetBinContent(3)+SignalA2_gen.GetBinContent(3))/2)
					else: newline=newline+" %.4f 1.0 %.4f 1.0 %.4f 1.0 \n"  %(SignalA2.GetBinContent(1),SignalA2.GetBinContent(2), SignalA2.GetBinContent(3))
				elif "TrigSyst" in line and runSysts==True:
					newline="TrigSyst lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,trigDown2H,1),systBinContent(Nom2H,trigUp2H,1), systBinContent(Nom2H,trigDown2H,2),systBinContent(Nom2H,trigUp2H,2),systBinContent(Nom2H,trigDown2H,3),systBinContent(Nom2H,trigUp2H,3))
				elif "ISRSyst" in line and runSysts==True:
					newline="ISRSyst lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ISRDown2H,1),systBinContent(Nom2H,ISRUp2H,1), systBinContent(Nom2H,ISRDown2H,2),systBinContent(Nom2H,ISRUp2H,2),systBinContent(Nom2H,ISRDown2H,3),systBinContent(Nom2H,ISRUp2H,3))
				elif "SignalStat2H" in line and runSysts==True:
					newline="SignalStat1_2H lnN %.4f - - - - - \n" %(MCStat2H.GetBinContent(1))
					fcardout.write(newline)
					newline="SignalStat2_2H lnN - - %.4f - - - \n" %(MCStat2H.GetBinContent(2))
					fcardout.write(newline)
					newline="SignalStat3_2H lnN - - - - %.4f - \n" %(MCStat2H.GetBinContent(3))
					fcardout.write(newline)
					continue
				elif "SignalJEC" in line and runSysts==True:
					newline="SignalJEC lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JECDown2H,1),systBinContent(Nom2H,JECUp2H,1), systBinContent(Nom2H,JECDown2H,2),systBinContent(Nom2H,JECUp2H,2),systBinContent(Nom2H,JECDown2H,3),systBinContent(Nom2H,JECUp2H,3))
				elif "SignalJER" in line and runSysts==True:
					newline="SignalJER lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JERDown2H,1),systBinContent(Nom2H,JERUp2H,1), systBinContent(Nom2H,JERDown2H,2),systBinContent(Nom2H,JERUp2H,2),systBinContent(Nom2H,JERDown2H,3),systBinContent(Nom2H,JERUp2H,3))
				elif "SignalRes" in line and runSysts==True:
					newline="SignalRes lnN %.4f - %.4f - %.4f -  \n" %(systBinContent(Nom2H,massSmear2H,1),systBinContent(Nom2H,massSmear2H,2),systBinContent(Nom2H,massSmear2H,3))
				elif "SignalPrefire" in line and runSysts==True:
					newline="SignalPrefire lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PreDown2H,1),systBinContent(Nom2H,PreUp2H,1), systBinContent(Nom2H,PreDown2H,2),systBinContent(Nom2H,PreUp2H,2),systBinContent(Nom2H,PreDown2H,3),systBinContent(Nom2H,PreUp2H,3))
				elif "SignalPU" in line and runSysts==True:
					newline="SignalPU lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PUDown2H,1),systBinContent(Nom2H,PUUp2H,1), systBinContent(Nom2H,PUDown2H,2),systBinContent(Nom2H,PUUp2H,2),systBinContent(Nom2H,PUDown2H,3),systBinContent(Nom2H,PUUp2H,3))
				elif "SignalScale" in line and runSysts==True:
					newline="SignalScale lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ScaleDown2H,1),systBinContent(Nom2H,ScaleUp2H,1), systBinContent(Nom2H,ScaleDown2H,2),systBinContent(Nom2H,ScaleUp2H,2),systBinContent(Nom2H,ScaleDown2H,3),systBinContent(Nom2H,ScaleUp2H,3))
				elif "SignalBBTag" in line and runSysts==True:
					newline="SignalBBTag lnN %g/%g - %g/%g - %g/%g -  \n" %(systBinContent(Nom2H,BBTagDown2H,1), systBinContent(Nom2H,BBTagUp2H,1), systBinContent(Nom2H,BBTagDown2H,2), systBinContent(Nom2H,BBTagUp2H,2),systBinContent(Nom2H,BBTagDown2H,3),systBinContent(Nom2H,BBTagUp2H,3))
				elif "SignalFastBBTag" in line and runSysts==True and signal=="TChiHH":
					newline="SignalFastBBTag lnN %g/%g - %g/%g - %g/%g -  \n" %(systBinContent(Nom2H,FastBBDown2H,1),systBinContent(Nom2H,FastBBUp2H,1), systBinContent(Nom2H,FastBBDown2H,2),systBinContent(Nom2H,FastBBUp2H,2),systBinContent(Nom2H,FastBBDown2H,3),systBinContent(Nom2H,FastBBUp2H,3))
					fcardout.write(newline);
					continue
				elif "SignalFastMass" in line and runSysts==True and signal=="TChiHH":
					newline="SignalFastMass lnN %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastMassDown2H,1),systBinContent(Nom2H,FastMassUp2H,1), systBinContent(Nom2H,FastMassDown2H,2), systBinContent(Nom2H,FastMassUp2H,2), systBinContent(Nom2H,FastMassDown2H,3),systBinContent(Nom2H,FastMassUp2H,3))
					fcardout.write(newline);
					continue
				elif "SignalMETFastSIM" in line and runSysts==True and signal=="TChiHH":
					newline="SignalMETFastSIM lnN %.4f - %.4f - %.4f - \n" %(METFastSyst2H.GetBinContent(1),METFastSyst2H.GetBinContent(2),METFastSyst2H.GetBinContent(3))
					fcardout.write(newline);
					continue
				else:
					fcardout.write(line);
					continue
				fcardout.write(newline);
			fcardout.close()
			fcard.close()

			fcard=open(tempNameB, 'r'); fcardout=open(cardBName, 'w')
			for line in fcard:
				if "observation" in line:
					newline="observation %.4f " %(sumRegionB2)
				elif "rate" in line and not "rateParam" in line:
					newline="rate "
					if runMETavg and signal=="TChiHH": newline=newline +"%.4f 1.0  \n" %((SignalB2.Integral(1,5)+SignalB2_gen.Integral(1,5))/2)
					else: newline=newline +"%.4f 1.0  \n" %(SignalB2.Integral(1,5))
				elif "Beta_2H rateParam" in line:
					newline="Beta_2H rateParam SidebandRegion2H bkg %.4f [0.000,%.4f] \n" %(sumRegionB2, sumRegionB2*10.0)
				else:
					fcardout.write(line);
					continue
				fcardout.write(newline);
			fcardout.close();
			fcard.close()

		else:
			fcard=open(tempNameA, 'r'); fcardout=open(cardAName, 'w')
			for line in fcard:
				if "observation" in line:
					newline="observation %.4f %.4f %.4f %.4f %.4f %.4f" %(sumRegionA2.GetBinContent(1),sumRegionA2.GetBinContent(2),sumRegionA2.GetBinContent(3)+sumRegionA2.GetBinContent(4),sumRegionA1.GetBinContent(1),sumRegionA1.GetBinContent(2),sumRegionA1.GetBinContent(3)+sumRegionA1.GetBinContent(4))
				elif "rate" in line and not "rateParam" in line:
					newline="rate "
					if runMETavg and signal=="TChiHH": newline=newline+" %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 \n"  %((SignalA2.GetBinContent(1)+SignalA2_gen.GetBinContent(1))/2,(SignalA2.GetBinContent(2)+SignalA2_gen.GetBinContent(2))/2, (SignalA2.GetBinContent(3)+SignalA2_gen.GetBinContent(3))/2,(SignalA1.GetBinContent(1)+SignalA1_gen.GetBinContent(1))/2,(SignalA1.GetBinContent(2)+SignalA1_gen.GetBinContent(2))/2, (SignalA1.GetBinContent(3)+SignalA1_gen.GetBinContent(3))/2)
					else: newline=newline+" %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 %.4f 1.0 \n"  %(SignalA2.GetBinContent(1),SignalA2.GetBinContent(2), SignalA2.GetBinContent(3), SignalA1.GetBinContent(1), SignalA1.GetBinContent(2), SignalA1.GetBinContent(3))
				elif "TrigSyst" in line and runSysts==True:
					newline="TrigSyst lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,trigDown2H,1),systBinContent(Nom2H,trigUp2H,1), systBinContent(Nom2H,trigDown2H,2),systBinContent(Nom2H,trigUp2H,2),systBinContent(Nom2H,trigDown2H,3),systBinContent(Nom2H,trigUp2H,3), systBinContent(Nom1H,trigDown1H,1),systBinContent(Nom1H,trigUp1H,1), systBinContent(Nom1H,trigDown1H,2),systBinContent(Nom1H,trigUp1H,2),systBinContent(Nom1H,trigDown1H,3),systBinContent(Nom1H,trigUp1H,3))
				elif "ISRSyst" in line and runSysts==True:
					newline="ISRSyst lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ISRDown2H,1),systBinContent(Nom2H,ISRUp2H,1), systBinContent(Nom2H,ISRDown2H,2),systBinContent(Nom2H,ISRUp2H,2),systBinContent(Nom2H,ISRDown2H,3),systBinContent(Nom2H,ISRUp2H,3), systBinContent(Nom1H,ISRDown1H,1),systBinContent(Nom1H,ISRUp1H,1), systBinContent(Nom1H,ISRDown1H,2),systBinContent(Nom1H,ISRUp1H,2),systBinContent(Nom1H,ISRDown1H,3),systBinContent(Nom1H,ISRUp1H,3))
				elif "SignalStat2H" in line and runSysts==True:
					newline="SignalStat1_2H lnN %.4f - - - - - - - - - - - \n" %(MCStat2H.GetBinContent(1))
					fcardout.write(newline)
					newline="SignalStat2_2H lnN - - %.4f - - - - - - - - - \n" %(MCStat2H.GetBinContent(2))
					fcardout.write(newline)
					newline="SignalStat3_2H lnN - - - - %.4f - - - - - - - \n" %(MCStat2H.GetBinContent(3))
					fcardout.write(newline)
					continue
				elif "SignalStat1H" in line and runSysts==True:
					newline="SignalStat1_1H lnN - - - - - - %.4f - - - - -\n" %(MCStat1H.GetBinContent(1))
					fcardout.write(newline)
					newline="SignalStat2_1H lnN - - - - - - - - %.4f - - -\n" %(MCStat1H.GetBinContent(2))
					fcardout.write(newline)
					newline="SignalStat3_1H lnN - - - - - - - - - - %.4f -\n" %(MCStat1H.GetBinContent(3))
					fcardout.write(newline)
					continue
				elif "SignalJEC" in line and runSysts==True:
					newline="SignalJEC lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JECDown2H,1),systBinContent(Nom2H,JECUp2H,1), systBinContent(Nom2H,JECDown2H,2),systBinContent(Nom2H,JECUp2H,2),systBinContent(Nom2H,JECDown2H,3),systBinContent(Nom2H,JECUp2H,3), systBinContent(Nom1H,JECDown1H,1),systBinContent(Nom1H,JECUp1H,1), systBinContent(Nom1H,JECDown1H,2),systBinContent(Nom1H,JECUp1H,2),systBinContent(Nom1H,JECDown1H,3),systBinContent(Nom1H,JECUp1H,3))
				elif "SignalJER" in line and runSysts==True:
					newline="SignalJER lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,JERDown2H,1),systBinContent(Nom2H,JERUp2H,1), systBinContent(Nom2H,JERDown2H,2),systBinContent(Nom2H,JERUp2H,2),systBinContent(Nom2H,JERDown2H,3),systBinContent(Nom2H,JERUp2H,3), systBinContent(Nom1H,JERDown1H,1),systBinContent(Nom1H,JERUp1H,1), systBinContent(Nom1H,JERDown1H,2),systBinContent(Nom1H,JERUp1H,2),systBinContent(Nom1H,JERDown1H,3),systBinContent(Nom1H,JERUp1H,3))
				elif "SignalRes" in line and runSysts==True:
					if signal=="TChiHH": newline="SignalRes lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(systBinContent(Nom2H,massSmear2H,1),systBinContent(Nom2H,massSmear2H,2),systBinContent(Nom2H,massSmear2H,3), systBinContent(Nom1H,massSmear1H,1),systBinContent(Nom1H,massSmear1H,2),systBinContent(Nom1H,massSmear1H,3))
					else: newline="SignalRes lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(1-systBinContent(Nom2H,massSmear2H,1)+1,1-systBinContent(Nom2H,massSmear2H,2)+1,1-systBinContent(Nom2H,massSmear2H,3)+1, 1-systBinContent(Nom1H,massSmear1H,1)+1,1-systBinContent(Nom1H,massSmear1H,2)+1,1-systBinContent(Nom1H,massSmear1H,3)+1)
				elif "SignalPrefire" in line and runSysts==True:
					newline="SignalPrefire lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PreDown2H,1),systBinContent(Nom2H,PreUp2H,1), systBinContent(Nom2H,PreDown2H,2),systBinContent(Nom2H,PreUp2H,2),systBinContent(Nom2H,PreDown2H,3),systBinContent(Nom2H,PreUp2H,3), systBinContent(Nom1H,PreDown1H,1),systBinContent(Nom1H,PreUp1H,1), systBinContent(Nom1H,PreDown1H,2),systBinContent(Nom1H,PreUp1H,2),systBinContent(Nom1H,PreDown1H,3),systBinContent(Nom1H,PreUp1H,3))
				elif "SignalPU" in line and runSysts==True:
					newline="SignalPU lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,PUDown2H,1),systBinContent(Nom2H,PUUp2H,1), systBinContent(Nom2H,PUDown2H,2),systBinContent(Nom2H,PUUp2H,2),systBinContent(Nom2H,PUDown2H,3),systBinContent(Nom2H,PUUp2H,3), systBinContent(Nom1H,PUDown1H,1),systBinContent(Nom1H,PUUp1H,1), systBinContent(Nom1H,PUDown1H,2),systBinContent(Nom1H,PUUp1H,2),systBinContent(Nom1H,PUDown1H,3),systBinContent(Nom1H,PUUp1H,3))
				elif "SignalScale" in line and runSysts==True:
					newline="SignalScale lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,ScaleDown2H,1),systBinContent(Nom2H,ScaleUp2H,1), systBinContent(Nom2H,ScaleDown2H,2),systBinContent(Nom2H,ScaleUp2H,2),systBinContent(Nom2H,ScaleDown2H,3),systBinContent(Nom2H,ScaleUp2H,3), systBinContent(Nom1H,ScaleDown1H,1),systBinContent(Nom1H,ScaleUp1H,1), systBinContent(Nom1H,ScaleDown1H,2), systBinContent(Nom1H,ScaleUp1H,2), systBinContent(Nom1H,ScaleDown1H,3), systBinContent(Nom1H,ScaleUp1H,3))
				elif "SignalBBTag" in line and runSysts==True:
					newline="SignalBBTag lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,BBTagDown2H,1), systBinContent(Nom2H,BBTagUp2H,1), systBinContent(Nom2H,BBTagDown2H,2), systBinContent(Nom2H,BBTagUp2H,2),systBinContent(Nom2H,BBTagDown2H,3),systBinContent(Nom2H,BBTagUp2H,3), systBinContent(Nom1H,BBTagDown1H,1),systBinContent(Nom1H,BBTagUp1H,1), systBinContent(Nom1H,BBTagDown1H,2), systBinContent(Nom1H,BBTagUp1H,2), systBinContent(Nom1H,BBTagDown1H,3), systBinContent(Nom1H,BBTagUp1H,3))
				elif "SignalFastBBTag" in line and runSysts==True and signal=="TChiHH":
					newline="SignalFastBBTag lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastBBDown2H,1),systBinContent(Nom2H,FastBBUp2H,1), systBinContent(Nom2H,FastBBDown2H,2),systBinContent(Nom2H,FastBBUp2H,2),systBinContent(Nom2H,FastBBDown2H,3),systBinContent(Nom2H,FastBBUp2H,3), systBinContent(Nom1H,FastBBDown1H,1),systBinContent(Nom1H,FastBBUp1H,1), systBinContent(Nom1H,FastBBDown1H,2),systBinContent(Nom1H,FastBBUp1H,2),systBinContent(Nom1H,FastBBDown1H,3),systBinContent(Nom1H,FastBBUp1H,3))
					fcardout.write(newline);
					continue
				elif "SignalFastMass" in line and runSysts==True and signal=="TChiHH":
					newline="SignalFastMass lnN %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - %g/%g - \n" %(systBinContent(Nom2H,FastMassDown2H,1),systBinContent(Nom2H,FastMassUp2H,1), systBinContent(Nom2H,FastMassDown2H,2), systBinContent(Nom2H,FastMassUp2H,2), systBinContent(Nom2H,FastMassDown2H,3),systBinContent(Nom2H,FastMassUp2H,3), systBinContent(Nom1H,FastBBDown1H,1),systBinContent(Nom1H,FastBBUp1H,1), systBinContent(Nom1H,FastBBDown1H,2), systBinContent(Nom1H,FastBBUp1H,2), systBinContent(Nom1H,FastBBDown1H,3), systBinContent(Nom1H,FastBBUp1H,3))
					fcardout.write(newline);
					continue
				elif "SignalMETFastSIM" in line and runSysts==True and signal=="TChiHH":
					newline="SignalMETFastSIM lnN %.4f - %.4f - %.4f - %.4f - %.4f - %.4f - \n" %(METFastSyst2H.GetBinContent(1),METFastSyst2H.GetBinContent(2),METFastSyst2H.GetBinContent(3), METFastSyst1H.GetBinContent(1),METFastSyst1H.GetBinContent(2),METFastSyst1H.GetBinContent(3))
					fcardout.write(newline);
					continue
				else:
					fcardout.write(line);
					continue
				fcardout.write(newline);
			fcardout.close()
			fcard.close()

			fcard=open(tempNameB, 'r'); fcardout=open(cardBName, 'w')
			for line in fcard:
				if "observation" in line:
					newline="observation %.4f %.4f " %(sumRegionB2, sumRegionB1)
				elif "rate" in line and not "rateParam" in line:
					newline="rate "
					if runMETavg and signal=="TChiHH": newline=newline +"%.4f 1.0 %.4f 1.0 \n" %((SignalB2.Integral(1,5)+SignalB2_gen.Integral(1,5))/2, (SignalB1.Integral(1,5)+SignalB1_gen.Integral(1,5))/2)
					else: newline=newline +"%.4f 1.0 %.4f 1.0 \n" %(SignalB2.Integral(1,5), SignalB1.Integral(1,5))
				elif "Beta_2H rateParam" in line:
					newline="Beta_2H rateParam SidebandRegion2H bkg %.4f [0.000,%.4f] \n" %(sumRegionB2, sumRegionB2*10.0)
				elif "Beta_1H rateParam" in line:
					newline="Beta_1H rateParam SidebandRegion1H bkg %.4f [0.000,%.4f] \n" %(sumRegionB1, sumRegionB1*10.0)
				else:
					fcardout.write(line);
					continue
				fcardout.write(newline);
			fcardout.close();
			fcard.close()

		#Now fill C, D, and E
		fcard=open(tempNameC, 'r'); fcardout=open(cardCName, 'w')
		for line in fcard:
			if "observation" in line:
				newline="observation %.4f " %(sumRegionC)
			elif "rate" in line and not "rateParam" in line:
				newline="rate "
				if runMETavg and signal=="TChiHH": newline=newline +"%.4f 1.0  \n" %((SignalC.Integral(1,5)+SignalC_gen.Integral(1,5))/2)
				else: newline=newline +"%.4f 1.0  \n" %(SignalC.Integral(1,5))
			elif "rateParam" in line:
				newline="Gamma rateParam AntitagRegion0 bkg %.4f [0.000,%.4f] \n" %(sumRegionC, sumRegionC*10.0)
			else:
				fcardout.write(line);
				continue
			fcardout.write(newline);
		fcardout.close();
		fcard.close()


		fcard=open(tempNameD, 'r'); fcardout=open(cardDName, 'w')
		for line in fcard:
			if "observation" in line:
				newline="observation %.4f " %(sumRegionD)
			elif "rate" in line and not "rateParam" in line:
				newline="rate "
				if runMETavg and signal=="TChiHH": newline=newline +"%.4f 1.0 \n" %((SignalD.Integral(1,5)+SignalD_gen.Integral(1,5))/2)
				else: newline=newline +"%.4f 1.0 \n" %(SignalD.Integral(1,5))
			elif "rateParam" in line:
				newline="Delta rateParam AntiSidebandRegion0 bkg %.4f [0.000,%.4f] \n" %(sumRegionD, sumRegionD*10.0)
			else:
				fcardout.write(line)
				continue
			fcardout.write(newline)
		fcardout.close()
		fcard.close()


		fcard=open(tempNameE, 'r'); fcardout=open(cardEName, 'w')
		for line in fcard:
			if "observation" in line:
				newline="observation %.4f %.4f %.4f " %(bkgFrac_C.GetBinContent(1),bkgFrac_C.GetBinContent(2),bkgFrac_C.GetBinContent(3))
			elif "rate" in line and not "rateParam" in line:
				newline="rate "
				if runMETavg and signal=="TChiHH": newline=newline +"%.4f 1.0 %.4f 1.0 %.4f 1.0 \n" %((SignalMET.GetBinContent(1)+SignalMET_gen.GetBinContent(1))/2,(SignalMET.GetBinContent(2)+SignalMET_gen.GetBinContent(2))/2,(SignalMET.GetBinContent(3)+SignalMET_gen.GetBinContent(3))/2)
				else: newline=newline +"%.4f 1.0 %.4f 1.0 %.4f 1.0 \n" %(SignalMET.GetBinContent(1),SignalMET.GetBinContent(2),SignalMET.GetBinContent(3))
			elif "Epsilon1 rateParam" in line:
				newline="Epsilon1 rateParam METShape_1 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_C.GetBinContent(1), bkgFrac_C.GetBinContent(1)*10.0)
			elif "Epsilon2 rateParam" in line:
				newline="Epsilon2 rateParam METShape_2 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_C.GetBinContent(2), bkgFrac_C.GetBinContent(2)*10.0)
			elif "Epsilon3 rateParam" in line:
				newline="Epsilon3 rateParam METShape_3 bkg %.4f [0.000,%.4f] \n" %(bkgFrac_C.GetBinContent(3), bkgFrac_C.GetBinContent(3)*10.0)
			else:
				fcardout.write(line)
				continue
			fcardout.write(newline)
		fcardout.close()
		fcard.close()
	f.Close(); f2.Close();
	if signal=="TChiHH":
		fNominal_gen.Close()
		f2_genMET.Close()
	if runSysts==True:
		fISRUp.Close()
		fISRDown.Close()
		fSmear.Close()
		ftrigUp.Close()
		ftrigDown.Close()
		fpreUp.Close()
		fpreDown.Close()
		fpuUp.Close()
		fpuDown.Close()
		fbbtagUp.Close()
		fbbtagDown.Close()
		fjecUp.Close()
		fjecDown.Close()
		fjerUp.Close()
		fjerDown.Close()
		fScaleUp.Close()
		fScaleDown.Close()
		if signal=="TChiHH":
			fFastMassUp.Close()
			fFastMassDown.Close()
			fFastBBUp.Close()
			fFastBBDown.Close()


def combineDatacards(model,which,data):
	#This combines all the individual datacards into the cards used for running limits
	#I would recommend using combineAndRunDatacards.py instead of this since it runs much faster
	#Make sure you have datacards first - run makeDatacards()
	#model: TChiHH-G (1D), TChiHH (2D), T5HH (1D)
	#which: boost, res, comb, all (boosted only, resolved only, combination), or only2H
	#data: True or False (which is MC only for bkg)
	signal = model
	dataMCString ="MC"; whichDString ="1D"
	if data==True: dataMCString ="Data"
	if model=="TChiHH": whichDString = "2D"
	if signal=="TChiHH-G": signal="TChiHH" #yes, they'll have the same name

	mass_points = getMassPoints(model)
	#loop through mass points
	for mass_point in mass_points:
		hino=mass_point[0]; finalLSP=mass_point[1]
		if (finalLSP==1): finalLSP=1;
		elif (finalLSP%5!=0): finalLSP=finalLSP+2;
		resLSP = finalLSP;
		if (resLSP==1): resLSP=0;

		if which=="only2H":
			print("Boosted2H: %s%s(%d,%d)" %(whichDString,signal,hino,finalLSP))
			os.system("combineCards.py SR2Only_%s%s%d_LSP%d_%s_veto.txt CRB2Only_%s%s%d_LSP%d_%s_veto.txt CRCMerge_%s%s%d_LSP%d_%s_veto.txt CRDMerge_%s%s%d_LSP%d_%s_veto.txt CREMerge_%s%s%d_LSP%d_%s_veto.txt > %s%s%d_LSP%d_2BoostedH_%s_veto.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino, finalLSP,dataMCString,whichDString,signal, hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP, dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
			os.system("combineCards.py %s%s%d_LSP%d_2BoostedH_%s_veto.txt %sresData/%sTChiHH_%s_btagfix/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_2Honly_Combo.txt "%(whichDString,signal,hino,finalLSP,dataMCString,inDIR,whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,finalLSP,dataMCString))


		if which=="boost" or which=="all":
			print("BoostedOnly: %s%s(%d,%d)" %(whichDString,signal,hino,finalLSP))
			os.system("combineCards.py SRMerge_%s%s%d_LSP%d_%s.txt CRBMerge_%s%s%d_LSP%d_%s.txt CRCMerge_%s%s%d_LSP%d_%s.txt CRDMerge_%s%s%d_LSP%d_%s.txt CREMerge_%s%s%d_LSP%d_%s.txt > %s%s%d_LSP%d_BothBoostedH_%s.txt "%(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))

		if which=="res" or which=="all":
			print("ResOnly: %s%s(%d,%d)" %(whichDString,signal,hino,finalLSP))
			if signal=="TChiHH":
				os.system("combineCards.py %sresData/%sTChiHH_%s_btagfix/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_ResOnly.txt "%(inDIR,whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,finalLSP,dataMCString))
			else:
				os.system("combineCards.py %sresData/%sT5HH_%s_btagfix/datacard-%s_mGluino-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_ResOnly.txt "%(inDIR,whichDString,dataMCString,signal,hino,finalLSP,whichDString,signal,hino,finalLSP,dataMCString))
		if which=="comb" or which=="all" or which=="sig":
			print("Combination: %s%s(%d,%d)" %(whichDString,signal,hino,finalLSP))
			os.system("combineCards.py SRMerge_%s%s%d_LSP%d_%s_veto.txt CRBMerge_%s%s%d_LSP%d_%s_veto.txt CRCMerge_%s%s%d_LSP%d_%s_veto.txt CRDMerge_%s%s%d_LSP%d_%s_veto.txt CREMerge_%s%s%d_LSP%d_%s_veto.txt > %s%s%d_LSP%d_BothBoostedH_%s_veto.txt "%(whichDString,signal,hino,finalLSP, dataMCString,whichDString,signal,hino,finalLSP, dataMCString,whichDString,signal,hino,finalLSP, dataMCString,whichDString,signal,hino,finalLSP, dataMCString,whichDString,signal,hino,finalLSP, dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
			if signal=="TChiHH":
				os.system("combineCards.py %s%s%d_LSP%d_BothBoostedH_%s_veto.txt %sresData/%sTChiHH_%s_btagfix/datacard-%s_mChi-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_Combo.txt "%(whichDString,signal,hino,finalLSP,dataMCString,inDIR,whichDString,dataMCString,signal,hino,resLSP,whichDString,signal,hino,finalLSP,dataMCString))
			else:
				os.system("combineCards.py %s%s%d_LSP%d_BothBoostedH_%s_veto.txt %sresData/%sT5HH_%s_btagfix/datacard-%s_mGluino-%d_mLSP-%d_Tune_2016,2017,2018_priority1_resolved.txt > %s%s%d_LSP%d_%s_Combo.txt "%(whichDString,signal,hino,finalLSP,dataMCString,inDIR,whichDString,dataMCString,signal,hino,finalLSP,whichDString,signal,hino,finalLSP,dataMCString))

def runCombine(model,which,data):
	#Runs combine over the combined datacards to get the limits
	#I would recommend using combineAndRunDatacards.py instead of this since it runs much faster
	#Make sure you have datacards first - run makeDatacards() and runDatacards() first
	#model: TChiHH-G (1D), TChiHH (2D), T5HH (1D)
	#which: boost, res, comb, all, or sig (significance, runs on combination only)
	#Option to run goodness of fit if you uncomment things
	#data: True or False
	signal = model
	dataMCString ="MC"; whichDString ="1D"
	if data==True: dataMCString ="Data"
	if model=="TChiHH": whichDString = "2D"
	if signal=="TChiHH-G": signal="TChiHH" #yes, they'll have the same name

	mass_points = getMassPoints(model)
	#loop through mass points
	for mass_point in mass_points:
		#resolved and boosted have different mass point names
		#Make them consistent and set final datacards to have the good names (For example, mLSP=225 instead of mLSP=223)
		hino=mass_point[0]; finalLSP=mass_point[1]
		if (finalLSP==1): finalLSP=1;
		elif (finalLSP%5!=0): finalLSP=finalLSP+2;
		resLSP = finalLSP;
		if (resLSP==1): resLSP=0;

		if which=="boost" or which=="all":
			print("----------------BoostedOnly----------------")
			os.system("combine -M AsymptoticLimits -n %s%s%d_LSP%d_BothBoostedH_%s %s%s%d_LSP%d_BothBoostedH_%s.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))

		if which=="res" or which=="all":
			print("----------------ResOnly----------------")
			os.system("combine -M AsymptoticLimits -n %s%s%d_LSP%d_%s_ResOnly %s%s%d_LSP%d_%s_ResOnly.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))

		if which=="comb" or which=="all" or which=="sig":

			if which!="sig":
				print("----------------Combination----------------")
				os.system("combine -M AsymptoticLimits -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
			else:
				print("----------------Significance----------------")
				os.system("combine -M Significance --rMin=-10 --uncapped yes -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt " %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString))
			# else:
			#     print("----------------GOF----------------")
			#     seed = int(TRandom3(0).Rndm()*100000.0)
			#     print(seed)
			#     os.system("combine -M GoodnessOfFit -n %s%s%d_LSP%d_%s_Combo %s%s%d_LSP%d_%s_Combo.txt  --algo=saturated --toysFreq -t 500 -s %i --fixedSignalStrength=1" %(whichDString,signal,hino,finalLSP,dataMCString,whichDString,signal,hino,finalLSP,dataMCString, seed))

if __name__ == '__main__':

	#Options for makeDatacards(model,which,data) - runs for BoostOnly and BoostVeto (with overlapping events removed)
	makeDatacards("TChiHH-G","boost",True)
	makeDatacards("TChiHH-G","veto",True)
	makeDatacards("TChiHH","boost",True)
	makeDatacards("TChiHH","veto",True)
	# makeDatacards("TChiHH","only2H",True) #For EWK Combine only
	makeDatacards("T5HH","boost",True)
	makeDatacards("T5HH","veto",True)
	# makeDatacards("T5HHextra","boost",True)


	# I would advise not running the following
	# combineDatacards("TChiHH-G","all",True) #all = runs boost, res, and comb
	# combineDatacards("TChiHH","all",True) #runs boost, res, and comb
	# combineDatacards("TChiHH","only2H",True) #EWK combine only (for higgsino interpretation)
	# combineDatacards("T5HH","all",True)

	# runCombine(model,which,data) - recommend running using combineAndRunDatacards.py
	# runCombine("TChiHH-G","all",True)
	# runCombine("TChiHH","all",True)
	# runCombine("T5HH","all",True)
