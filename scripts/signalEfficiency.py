import os
from ROOT import *
import sys
import array
from array import array
from ROOT import gROOT
import os.path
gROOT.SetBatch(True)


signalSkimsT5HHDIR = "/eos/uscms/store/user/emacdona/Skims/Run2ProductionV18/scan/tree_signal_METVars_FullSIM/"
signalSkimsTChiHHDIR = "/eos/uscms/store/user/kaulmer/Skims/Run2ProductionV18/scan/tree_signal_METVars/"
datacardsDIR = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/forGithub/CMSSW_10_2_13/src/HHplusMET/datacards/"
srcDIR = "../src/"
outDIR = "../output/"


chi03 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.9]{#scale[0.85]{_{3}}}";
chi02 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0 }}}#kern[-1.0]{#scale[0.85]{_{2 }}}";
chi01 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0 }}}#kern[-1.15]{#scale[0.85]{_{1 }}}";
chi02nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.9]{#scale[0.85]{_{2}}}";
chi01nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0#scale[0.2]{ }}}}#kern[-1.25]{#scale[0.85]{_{1#scale[0.2]{ }}}}";
chipm1nospace = "#lower[-0.12]{#tilde{#chi}}#lower[0.05]{#scale[0.85]{^{#pm}}}#kern[-1.4]{#scale[0.85]{_{1}}}";
chipm1 = "#lower[-0.12]{#tilde{#chi}}#lower[0.00]{#scale[0.85]{^{#pm }}}#kern[-1.3]{#scale[0.85]{_{1 }}}";
chimp1 = "#lower[-0.12]{#tilde{#chi}}#lower[0.00]{#scale[0.85]{^{#mp }}}#kern[-1.3]{#scale[0.85]{_{1 }}}";
chi0pmi = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0,#pm  }}}#kern[-1.1]{#scale[0.85]{_{ i    }}}";
chi0mpj = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0,#mp }}}#kern[-1.1]{#scale[0.85]{_{j   }}}";
glu = "#tilde{g}#scale[0.55]{_{ }}"


def higgsinoCrossSection1D(hig_mass):
    xsec = 1.0;
    if (hig_mass=="127"): xsec = 7.6022;
    elif (hig_mass =="150"): xsec = 3.83231;
    elif (hig_mass =="175"): xsec = 2.26794;
    elif (hig_mass =="200"): xsec = 1.33562;
    elif (hig_mass =="225"): xsec = 0.860597;
    elif (hig_mass =="250"): xsec = 0.577314;
    elif (hig_mass =="275"): xsec = 0.400107;
    elif (hig_mass =="300"): xsec = 0.284855;
    elif (hig_mass =="325"): xsec = 0.20736;
    elif (hig_mass =="350"): xsec = 0.153841;
    elif (hig_mass =="375"): xsec = 0.116006;
    elif (hig_mass =="400"): xsec = 0.0887325;
    elif (hig_mass =="425"): xsec = 0.0686963;
    elif (hig_mass =="450"): xsec = 0.0537702;
    elif (hig_mass =="475"): xsec = 0.0424699;
    elif (hig_mass =="500"): xsec = 0.0338387;
    elif (hig_mass =="525"): xsec = 0.0271867;
    elif (hig_mass =="550"): xsec = 0.0219868;
    elif (hig_mass =="575"): xsec = 0.0179062;
    elif (hig_mass =="600"): xsec = 0.0146677;
    elif (hig_mass =="625"): xsec = 0.012062;
    elif (hig_mass =="650"): xsec = 0.00996406;
    elif (hig_mass =="675"): xsec = 0.00828246;
    elif (hig_mass =="700"): xsec = 0.00689981;
    elif (hig_mass =="725"): xsec = 0.00578355;
    elif (hig_mass =="750"): xsec = 0.0048731;
    elif (hig_mass =="775"): xsec = 0.00409781;
    elif (hig_mass =="800"): xsec = 0.00346143;
    elif (hig_mass =="825"): xsec = 0.0029337;
    elif (hig_mass =="850"): xsec = 0.0024923;
    elif (hig_mass =="875"): xsec = 0.00213679;
    elif (hig_mass =="900"): xsec = 0.00180616;
    elif (hig_mass =="925"): xsec = 0.00155453;
    elif (hig_mass =="950"): xsec = 0.00132692;
    elif (hig_mass =="975"): xsec = 0.00112975;
    elif (hig_mass =="1000"): xsec = 0.000968853;
    elif (hig_mass =="1025"): xsec = 0.000840602;
    elif (hig_mass =="1050"): xsec = 0.000731306;
    elif (hig_mass =="1075"): xsec = 0.000627083;
    elif (hig_mass =="1100"): xsec = 0.000538005;
    elif (hig_mass =="1125"): xsec = 0.00046747;
    elif (hig_mass =="1150"): xsec = 0.000405108;
    elif (hig_mass =="1175"): xsec = 0.000348261;
    elif (hig_mass =="1200"): xsec = 0.000299347;
    elif (hig_mass =="1225"): xsec = 0.000265935;
    elif (hig_mass =="1250"): xsec = 0.000240471;
    elif (hig_mass =="1275"): xsec = 0.000190411;
    elif (hig_mass =="1300"): xsec = 0.000160765;
    elif (hig_mass =="1325"): xsec = 0.000136272;
    elif (hig_mass =="1350"): xsec = 0.000111174;
    elif (hig_mass =="1375"): xsec = 9.74728e-05;
    elif (hig_mass =="1400"): xsec = 7.80263e-05;
    elif (hig_mass =="1425"): xsec = 6.96843e-05;
    elif (hig_mass =="1450"): xsec = 6.96962e-05;
    elif (hig_mass =="1475"): xsec = 4.98006e-05;
    else: xsec = 0;
    return xsec*1000.0;

def higgsinoCrossSection2D(hig_mass):
    xsec = 1.0;
    if (hig_mass =="127"): xsec = 1.44725;
    elif (hig_mass =="150"): xsec = 0.71514;
    elif (hig_mass =="175"): xsec = 0.419059;
    elif (hig_mass =="200"): xsec = 0.244213;
    elif (hig_mass =="225"): xsec = 0.156286;
    elif (hig_mass =="250"): xsec = 0.104252;
    elif (hig_mass =="275"): xsec = 0.0719125;
    elif (hig_mass =="300"): xsec = 0.0509994;
    elif (hig_mass =="325"): xsec = 0.0369715;
    elif (hig_mass =="350"): xsec = 0.0273286;
    elif (hig_mass =="375"): xsec = 0.0205429;
    elif (hig_mass =="400"): xsec = 0.0156691;
    elif (hig_mass =="425"): xsec = 0.0120965;
    elif (hig_mass =="450"): xsec = 0.00944017;
    elif (hig_mass =="475"): xsec = 0.00743587;
    elif (hig_mass =="500"): xsec = 0.00590757;
    elif (hig_mass =="525"): xsec = 0.00469101;
    elif (hig_mass =="550"): xsec = 0.0038167;
    elif (hig_mass =="575"): xsec = 0.003073;
    elif (hig_mass =="600"): xsec = 0.00253015;
    elif (hig_mass =="625"): xsec = 0.00206136;
    elif (hig_mass =="650"): xsec = 0.00171418;
    elif (hig_mass =="675"): xsec = 0.00140934;
    elif (hig_mass =="700"): xsec = 0.00118113;
    elif (hig_mass =="725"): xsec = 0.000979349;
    elif (hig_mass =="750"): xsec = 0.000826366;
    elif (hig_mass =="775"): xsec = 0.000690208;
    elif (hig_mass =="800"): xsec = 0.000586211;
    elif (hig_mass =="825"): xsec = 0.00049277;
    elif (hig_mass =="850"): xsec = 0.000420556;
    elif (hig_mass =="875"): xsec = 0.000358734;
    elif (hig_mass =="900"): xsec = 0.000305935;
    elif (hig_mass =="925"): xsec = 0.000260948;
    elif (hig_mass =="950"): xsec = 0.00022285;
    elif (hig_mass =="975"): xsec = 0.000189681;
    elif (hig_mass =="1000"): xsec = 0.00016428;
    elif (hig_mass =="1025"): xsec = 0.000142206;
    elif (hig_mass =="1050"): xsec = 0.000120971;
    elif (hig_mass =="1075"): xsec = 0.000105301;
    elif (hig_mass =="1100"): xsec = 9.12469e-05;
    elif (hig_mass =="1125"): xsec = 7.9765e-05;
    elif (hig_mass =="1150"): xsec = 6.78234e-05;
    elif (hig_mass =="1175"): xsec = 5.9016e-05;
    elif (hig_mass =="1200"): xsec = 5.16263e-05;
    elif (hig_mass =="1225"): xsec = 4.5147e-05;
    elif (hig_mass =="1250"): xsec = 3.88343e-05;
    elif (hig_mass =="1275"): xsec = 3.41304e-05;
    elif (hig_mass =="1300"): xsec = 2.99353e-05;
    elif (hig_mass =="1325"): xsec = 2.63637e-05;
    elif (hig_mass =="1350"): xsec = 2.26779e-05;
    elif (hig_mass =="1375"): xsec = 1.99318e-05;
    elif (hig_mass =="1400"): xsec = 1.75031e-05;
    elif (hig_mass =="1425"): xsec = 1.53974e-05;
    elif (hig_mass =="1455"): xsec = 1.3245e-05;
    elif (hig_mass =="1475"): xsec = 1.16416e-05;
    else: xsec = 0;
    return xsec*1000.0;

def findWhichBins():
    '''
    For double-checking
    This will print out the signal bin indices in the datacards
    You can also print out the bin labels and/or the rate
    '''
    # datacardName = "1DT5HH1000_LSP1_Data_Combo.txt"
    datacardName = "1DTChiHH150_LSP1_Data_Combo.txt"

    datacardFile=open(datacardsDIR+datacardName, 'r');
    rateVec = [];
    binVec1 = []; binVec2= []; binVecFinal = []
    for line in datacardFile:
        if line.startswith("rate"):
            rateVec = line.split()
        elif line.startswith("bin"):
            if len(binVec1)==0:
                binVec1 = line.split()
            else: binVec2 = line.split()

    if len(binVec1)>len(binVec2): binVecFinal = binVec1
    else: binVecFinal = binVec2
    for i in range(len(rateVec)):
        # print(binVecFinal[i]+", "+rateVec[i])
        if ("xsig0_ysig" in binVecFinal[i]) and rateVec[i]!="1":
            print("res3b, "+str(i))
        elif ("xsig1_ysig" in binVecFinal[i]) and rateVec[i]!="1":
            print("res4b, "+str(i))
        elif ("Bin1H" in binVecFinal[i]) and rateVec[i]!="1":
            print("boost1H, "+str(i))
        elif ("Bin2H" in binVecFinal[i]) and rateVec[i]!="1":
            print("boost2H, "+str(i))

def saveEff(model):
    '''
    Saves efficiency for each model by mass point in a txt file.
    Saves the boosted efficiency, resolved efficiency, and combined efficiency
    Numerator is the sum of the signal regions from the datacards
    Denominator is all H decays for T5HH, and only H->bb decays for TChiHH/N1N2
    '''
    effFile=open(srcDIR+"efficiency_"+model+".txt", 'w');
    NLSPmass = []; LSPmass_final = []; LSPmass_skims = [];
    if model=="N1N2":
        effFile.write("#mNLSP mLSP boostEff resEff totalEff   bin1Eff   bin2Eff   bin3Eff   bin4Eff   bin5Eff   bin6Eff   bin7Eff   bin8Eff   bin9Eff  bin10Eff  bin11Eff  bin12Eff  bin13Eff  bin14Eff  bin15Eff  bin16Eff  bin17Eff  bin18Eff  bin19Eff  bin20Eff  bin21Eff  bin22Eff\n")
        fNames=open(srcDIR+"higgsino2DFileNames.txt", 'r');
        for line in fNames:
            x = line.split('_')
            if (int(x[5])>800): continue;
            LSP_mass = int(x[6])
            if (LSP_mass==1): LSP_mass=1;
            elif (LSP_mass%5!=0): LSP_mass=LSP_mass+2;
            NLSPmass.append(x[5]); LSPmass_final.append(str(LSP_mass)); LSPmass_skims.append(x[6])
        fNames.close();
        for h in range(0, 27):
            thisMass = 150+h*25;
            NLSPmass.append(str(thisMass)); LSPmass_final.append("1"); LSPmass_skims.append("1")
    elif model=="TChiHH":
        effFile.write("#mChi01 mG boostEff resEff totalEff   bin1Eff   bin2Eff   bin3Eff   bin4Eff   bin5Eff   bin6Eff   bin7Eff   bin8Eff   bin9Eff  bin10Eff  bin11Eff  bin12Eff  bin13Eff  bin14Eff  bin15Eff  bin16Eff  bin17Eff  bin18Eff  bin19Eff  bin20Eff  bin21Eff  bin22Eff\n")
        for h in range(0, 43):
            thisMass = 150+h*25;
            if thisMass==1025 or thisMass==1075 or thisMass==1125 or thisMass==1175: continue;
            NLSPmass.append(str(thisMass)); LSPmass_final.append("1"); LSPmass_skims.append("1")
    elif model=="T5HH":
        effFile.write("#mGluino mLSP boostEff resEff totalEff bin1Eff   bin2Eff   bin3Eff   bin4Eff   bin5Eff   bin6Eff   bin7Eff   bin8Eff   bin9Eff  bin10Eff  bin11Eff  bin12Eff  bin13Eff  bin14Eff  bin15Eff  bin16Eff  bin17Eff  bin18Eff  bin19Eff  bin20Eff  bin21Eff  bin22Eff\n")
        for g in range(0, 16):
            thisMass = 1000+g*100;
            NLSPmass.append(str(thisMass)); LSPmass_final.append("1"); LSPmass_skims.append("1")

    for i in range(len(NLSPmass)):
        print("For %s, mNLSP=%s and mLSP=%s" %(model, NLSPmass[i],LSPmass_final[i]))
        BR = 0.5823329; convertXsec = 1.0; signalFileName = ""; modelName = ""
        if model=="N1N2":
            BR = 0.5823329; modelName="2DTChiHH"
            if (LSPmass_final[i]=="1"):
                modelName="1DTChiHH"  # Resolved veto was missed for this LSP mass row
                signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_"+NLSPmass[i]+"_1_MC2016_fast.root"
                # convertXsec = higgsinoCrossSection1D(NLSPmass[i])/higgsinoCrossSection2D(NLSPmass[i])
            else:
                signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_2D_"+NLSPmass[i]+"_"+LSPmass_skims[i]+"_MC2016_fast.root"
        elif model=="TChiHH":
            BR = 0.5823329; modelName="1DTChiHH"
            signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_"+NLSPmass[i]+"_1_MC2016_fast.root"
        elif model=="T5HH":
            BR = 1.0; modelName="1DT5HH"
            signalFileName = signalSkimsT5HHDIR+"tree_T5qqqqZH-mGluino-1000to2500_"+NLSPmass[i]+"_1_MC2016.root"

        signalFile = TFile(signalFileName, "r")
        tree = signalFile.Get("tree"); weight=tree.GetMaximum("Weight")
        if weight==0: print("Weight is zero")
        else: signalFile.Close()
        totalEvt1 = weight*35922.0*BR*BR/convertXsec
        totalEvt2 = weight*41529.0*BR*BR/convertXsec
        totalEvt3 = weight*59740.0*BR*BR/convertXsec
        totalEvt = totalEvt1+totalEvt2+totalEvt3

        #open datacard
        datacardName = modelName+NLSPmass[i]+"_LSP"+LSPmass_final[i]+"_Data_Combo.txt"
        # print "For model "+model+", datacardFile is "+datacardName
        datacardFile=open(datacardsDIR+datacardName, 'r');
        for line in datacardFile:
            if line.startswith("rate"):
                x = line.split()
                res3b = float(x[35]) + float(x[47]) + float(x[59]) + float(x[71]) + float(x[83]) + float(x[95]) + float(x[107]) + float(x[119])
                res4b = float(x[37]) + float(x[49]) + float(x[61]) + float(x[73]) + float(x[85]) + float(x[97]) + float(x[109]) + float(x[121])
                boost2H = float(x[1]) + float(x[3]) + float(x[5])
                boost1H = float(x[7]) + float(x[9]) + float(x[11])
                break;
        passCuts = res3b+res4b+boost1H+boost2H
        datacardFile.close()
        boostEff = (boost1H+boost2H)/totalEvt; resEff = (res3b+res4b)/totalEvt; totalEff = passCuts/totalEvt
        # print("boostEff: %.4f, resEff: %.4f, totalEff: %.4f" %(boostEff, resEff, totalEff))
        # line = "%s %s %.7f %.7f %.7f\n" %(NLSPmass[i], LSPmass_final[i], boostEff, resEff, totalEff)
        line = [] ; line.append("%s" % NLSPmass[i]) ; line.append("%s" % LSPmass_final[i])
        effs = [] ; effs.extend([boostEff, resEff, totalEff])
        bins = [
            47, 71, 95, 119, # 3b, drmax>1.1, low to high met
            49, 73, 97, 121, # 4b, drmax>1.1, low to high met
            35, 59, 83, 107, # 3b, drmax<1.1, low to high met
            37, 61, 85, 109, # 4b, drmax<1.1, low to high met
            7, 9, 11, # 1H, low to high met
            1, 3, 5]  # 2H, low to high met
        for bin in bins: effs.append(float(x[bin])/totalEvt)
        for eff in effs: line.append("%.7f" % eff)
        effFile.write(' '.join(line)); effFile.write('\n')
    effFile.close()


def readInValues(model, which):
    vmx=array('d',[]); vmy=array('d',[]); veff=array('d',[]);
    effFileName = srcDIR+"efficiency_"+model+".txt"
    file_exists = os.path.exists(effFileName)
    if not file_exists:
        print("File not find. Trying to make...")
        saveEff(model)
        file_exists2 = os.path.exists(effFileName)
        if file_exists2: print("Made the file!")
        else:
            print("OK couldn't make it I guess. Exiting...")
            sys.exit(0)

    fcard=open(effFileName, 'r');
    for line in fcard:
        if "#" in line: continue;
        thisLine = line.split()
        vmx.append(Double(thisLine[0])); vmy.append(Double(thisLine[1]));
        if vmy[-1] == 1: vmy[-1] = -1  # A kludge to make histo binning work
        if which=="boost": veff.append(Double(thisLine[2]));
        elif which=="res": veff.append(Double(thisLine[3]));
        elif which=="comb": veff.append(Double(thisLine[4]));
        else: 
            try:
                bin = int(which)
            except ValueError:
                print "readInValues: Invalid value of which:  "+which
                sys.exit(0)
            veff.append(Double(thisLine[4+int(which)]))
    if model=="N1N2":
        histo = TGraph2D("", "", len(veff), vmx, vmy, veff);
        return(histo)
    elif model=="TChiHH" or model=="T5HH":
        histo = TGraph(len(veff), vmx, veff);
        return(histo)

def makeCanvas(model, binsToPlot, figlabel = None):

    outFn = ''
    if figlabel is None:
        outFn = "signalEff_"+model+"_"+which
    else:
        outFn = "CMS-SUS-20-004_Figure-"+figlabel
    pdfFn = outDIR+outFn+".pdf"
    outRootFile = TFile(outDIR+outFn+".root", "recreate")

    if type(binsToPlot) is str:
        which = binsToPlot
        bins = []
    else:
        which = 'bins'
        bins = binsToPlot
    if (which=="all" or which=="bins") and model=="N2N2":
        print("Can't make signal efficiency plot for 2D TChiHH with multiple planes! Run separately. Exiting now...")
        sys.exit(0)

    SignifScan2 = TGraph2D()
    SignifScan = TGraph(); SignifScanBoost = TGraph(); SignifScanRes = TGraph();
    canvName = model+"_"+which
    if which == 'bins':
        for bin in bins:
            canvName += '-'+bin

    # Set up canvas and fill graph(s) to be drawn and written
    canv = TCanvas(canvName)
    if model=="N1N2":
        SignifScan2 = readInValues(model, which);
        hForG2d = TH2F("hForG2d", "", 27, 137.5, 812.5, 28, -12.5, 687.5);
        hForG2d.GetXaxis().SetNdivisions(207)
        hForG2d.GetYaxis().SetNdivisions(207)
        # hForG2d.SetStats(0)
        # npts = SignifScan2.GetN()
        # xvals = SignifScan2.GetX()
        # yvals = SignifScan2.GetY()
        # zvals = SignifScan2.GetZ()
        # for pt in range(0, npts):
        #     hForG2d.Fill(xvals[pt], yvals[pt], zvals[pt])
        # SignifScan = hForG2d
        SignifScan2.SetHistogram(hForG2d);
        SignifScan = SignifScan2.GetHistogram()
        SignifScan.SetStats(0);
        canv.SetCanvasSize(1200,1050)
        canv.SetRightMargin(0.18);
    elif model=="TChiHH" or model=="T5HH":
        if (which=="all"):
            SignifScan = readInValues(model, "comb");
            SignifScanBoost = readInValues(model, "boost");
            SignifScanRes = readInValues(model, "res");
        elif which == 'bins' and len(bins) > 0: SignifScan = readInValues(model, bins[0]) 
        else: SignifScan = readInValues(model, which);
        canv.SetCanvasSize(1200,1200)
        canv.SetRightMargin(0.04);
    canv.cd()
    canv.SetLeftMargin(0.13); canv.SetBottomMargin(0.11);canv.SetTopMargin(0.08)

    bin = -1
    try:
        bin = int(which)
    except ValueError:
        pass

    signatureDict = {'res':'Resolved', 'boost':'Boosted', 'comb':'Combined', 'all':'', 'bins':''}
    zAxisTitle = "Signal Efficiency" #although for the 1D plots this is y-axis title
    if bin > 0: zAxisTitle = "Bin "+which+" Signal Efficiency"
    else:  zAxisTitle = signatureDict[which]+" Signal Efficiency"

    colors = [kSpring+7, kTeal+9, kAzure+1, kAzure-1, kRed+2, kOrange+7, kOrange-3, kYellow+1]
    colorOffset = 0 if len(bins) == 4 else 4  # A kludge to choose between resolved & boosted when which = 'bins'

    # Set titles
    if model=="N1N2":
        SignifScan.SetTitle(";m("+chi02+") [GeV]; m("+chi01+") [GeV];"+zAxisTitle)
        SignifScan.GetZaxis().SetTitleOffset(1.2); SignifScan.GetZaxis().SetTitleSize(0.047)
    elif model=="TChiHH":
        if (which=="all"): SignifScan.SetTitle(";m("+chi01nospace+") [GeV];Signal Efficiency")
        else: SignifScan.SetTitle(";m("+chi01nospace+") [GeV];"+zAxisTitle)
    elif model=="T5HH":
        if (which=="all"): SignifScan.SetTitle(";m("+glu+") [GeV];Signal Efficiency")
        else: SignifScan.SetTitle(";m("+glu+") [GeV];"+zAxisTitle)

    SignifScan.GetYaxis().SetTitleOffset(1.2); SignifScan.GetYaxis().SetTitleSize(0.047)
    SignifScan.GetXaxis().SetTitleOffset(1.0); SignifScan.GetXaxis().SetTitleSize(0.047)
    # ltitle = TLatex(0.17, 0.87,"#font[62]{CMS}#scale[0.76]{#font[52]{ Simulation Supplementary}}")
    CMStitleStr = "#font[62]{CMS}#scale[0.76]{#font[52]{ Simulation Supplementary}}"
    subfig = figlabel.split('-')[1]
    if not (subfig == 'a' or subfig == 'b' or (model == "N1N2" and subfig == 'c')):
        CMStitleStr += "#font[42]{     ("+subfig+")}"
    ltitle = TLatex(0.17, 0.87, CMStitleStr)
    ltitle.SetNDC(); ltitle.SetTextAlign(12);

    # Set up text blocks
    if model=="N1N2":
        rtitle = TLatex(0.82, 0.945,"#scale[0.75]{137 fb^{-1} (13 TeV)}")
        txtd = TLatex(0.19,0.75,"pp#rightarrow "+chi03+" "+chi02+" #rightarrow HH "+chi01+" "+chi01);
        txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.045);
    elif model=="TChiHH":
        rtitle = TLatex(0.96, 0.943,"#scale[0.75]{137 fb^{-1} (13 TeV)}")
        txtd = TLatex(0.17,0.78,"pp #rightarrow "+chi0pmi+chi0mpj+"#rightarrow "+chi01+chi01+" + X_{soft} #rightarrow HH#tilde{G}#tilde{G} + X_{soft}");
        txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.045);
        txte = TLatex(0.17,0.72,"m("+chi02nospace+") = m("+chipm1nospace+") = m("+chi01nospace+"), m(#tilde{G}) = 1 GeV");
        txte.SetNDC(); txte.SetTextFont(42); txte.SetTextSize(0.045);
    elif model=="T5HH":
        rtitle = TLatex(0.96, 0.943,"#scale[0.75]{137 fb^{-1} (13 TeV)}")
        txtd = TLatex(0.17,0.78,"pp #rightarrow #tilde{g} #tilde{g},  #tilde{g} #rightarrow q #bar{q} "+chi02+",  "+chi02+" #rightarrow H "+chi01);
        txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.045);

    if model=="N1N2":
        canv.SetLogz()
        SignifScan.SetMaximum(1.0);
        SignifScan.SetContour(150)
        if bin > 0:
            SignifScan.SetMinimum(0.0000005)
            SignifScan.Write("Bin"+str(bin)+"SignalEfficiency_"+model)
        else:
            SignifScan.SetMinimum(0.000005)
            SignifScan.Write(signatureDict[which]+"SignalEfficiency_"+model)
        SignifScan.Draw("colz")
    elif model=="TChiHH" or model=="T5HH":
        canv.SetLogy()
        if model=="TChiHH":
            SignifScan.SetMaximum(2.5)
            if which == "bins" or bin > 0:
                SignifScan.SetMinimum(0.0000005)
                SignifScan.SetMaximum(2.0);
            else: SignifScan.SetMinimum(0.001)
            SignifScan.GetXaxis().SetRangeUser(180.0,1220.0);
        elif model=="T5HH":
            SignifScan.SetMaximum(1.0)
            if which == "bins" or bin > 0:
                min = 0.00001 if colorOffset == 4 else 0.00000001
                SignifScan.SetMinimum(min)
                if colorOffset == 0: SignifScan.SetMaximum(0.2)
            elif (which=="boost" or which=="comb"): SignifScan.SetMinimum(0.01)
            elif (which=="all" or which=="res"): SignifScan.SetMinimum(0.0001)
            SignifScan.GetXaxis().SetRangeUser(950.0,2550.0);
        SignifScan.SetMarkerStyle(20); SignifScan.SetMarkerSize(1.8)
        SignifScan.SetMarkerColor(kBlack); SignifScan.SetLineColor(kBlack)
        if which == "bins":
            SignifScan.SetMarkerColor(colors[colorOffset]); SignifScan.SetLineColor(colors[colorOffset])
            SignifScan.Write("Bin"+bins[0]+"SignalEfficiency_"+model)
        else:
            SignifScan.Write("Combined"+"SignalEfficiency_"+model)
        SignifScan.Draw("APE")

        if (which=="all"):
            SignifScanBoost.SetMarkerStyle(20); SignifScanBoost.SetMarkerSize(1.8)
            SignifScanBoost.SetMarkerColor(kRed); SignifScanBoost.SetLineColor(kRed);
            SignifScanRes.SetMarkerStyle(20); SignifScanRes.SetMarkerSize(1.8)
            SignifScanRes.SetMarkerColor(kBlue); SignifScanRes.SetLineColor(kBlue);
            SignifScanBoost.Draw("PE same")
            SignifScanBoost.Write("BoostedSignalEfficiency_"+model)
            SignifScanRes.Draw("PE same");
            SignifScanRes.Write("ResolvedSignalEfficiency_"+model)
            if model=="TChiHH": leg = TLegend(0.6,0.17,0.85,0.43);
            elif model=="T5HH": leg = TLegend(0.2,0.17,0.45,0.43);
            leg.SetFillStyle(4050); leg.SetFillColor(0);
            leg.SetTextFont(42); leg.SetBorderSize(0);
            leg.SetTextSize(0.045);
            leg.AddEntry(SignifScanRes, "Resolved","lp")
            leg.AddEntry(SignifScanBoost, "Boosted","lp")
            leg.AddEntry(SignifScan, "Combination","lp")
            leg.Draw("same")
        elif (which=="bins"):
            if model=="TChiHH": leg = TLegend(0.6,0.17,0.85,0.43);
            elif model=="T5HH": leg = TLegend(0.2,0.17,0.45,0.43);
            leg.SetFillStyle(4050); leg.SetFillColor(0);
            leg.SetTextFont(42); leg.SetBorderSize(0);
            leg.SetTextSize(0.045);
            scan = []
            for ib in range(0, len(bins)):
                scan.append(readInValues(model, bins[ib]))
                scan[ib].SetMarkerStyle(20); scan[ib].SetMarkerSize(1.8)
                color = colors[colorOffset+ib]
                scan[ib].SetMarkerColor(color); scan[ib].SetLineColor(color)
                if ib > 0:
                    scan[ib].Draw("PE same")  # The first bin was drawn above
                    scan[ib].Write("Bin"+bins[ib]+"SignalEfficiency_"+model)
                leg.AddEntry(scan[ib], "Bin "+bins[ib], "lp")
            leg.Draw("same")

    rtitle.SetTextFont(42); rtitle.SetNDC(); rtitle.SetTextAlign(32)
    ltitle.Draw("same"); rtitle.Draw("same"); txtd.Draw("same")
    if model=="TChiHH": txte.Draw("same")

    #save canvas
    canv.SaveAs(pdfFn, "PDF")
    outRootFile.Close()

def saveSigEffMassRootFile(model, mass, printEff):
    '''
    For HEPData, saves a root file with one TH2F for the signal efficiency of the
    chosen 1D signal model and mass point.
    y-axis is binned in MET (integrated, 300-500, 500-700, 700+)
    x-axis is the analysis region bins (Baseline, 0HSB, 0HSR, 1HSB, 1HSR, 2HSB, 2HSR)
    Note: this signal efficiency ignores the fastSIM MET treatment (for TChiHH only), only using the yields from the recoMET
    '''
    #Denominator of efficiency is all H decays for T5HH and only H->bb decays for TChiHH
    #First, define TH2D to fill and set bin labels
    h_sigEff = TH2F("sigEff","",7,0,7,4,0,4)
    binLabelsX = ["Baseline", "N_{H} = 0 CSB", "N_{H} = 0 CSR", "N_{H} = 1 SB", "N_{H} = 1 SR", "N_{H} = 2 SB", "N_{H} = 2 SR"]
    binLabelsY = [">300", "300-500", "500-700", ">700"]

    #Determine denominator based on xsec and lumi only (no other weights)
    BR2 = 1.0; signalFileName = ""

    if model=="TChiHH":
        BR2 = 0.5823329*0.5823329
        signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_"+mass+"_1_MC2016_fast.root"
    elif model=="T5HH":
        BR2 = 1.0
        signalFileName = signalSkimsT5HHDIR+"tree_T5qqqqZH-mGluino-1000to2500_"+mass+"_1_MC2016.root"

    signalFile = TFile(signalFileName, "r")
    tree = signalFile.Get("tree"); weight=tree.GetMaximum("Weight")
    if weight==0:
        print("Weight is zero")
        return;
    else: signalFile.Close()
    denom = weight*(35922.0+41529.0+59740.0)*BR2

    #Then, open ALPHABET output file
    subDir = "T5HH1D_FullSIM_veto/"
    if (model=="TChiHH"): subDir = "boosted_veto_FastSIMSFs/"
    ABCD_FileName = "/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/"+subDir+"ALPHABET_1DSignal.root"
    ABCD_File = TFile(ABCD_FileName, "r")
    baseline=ABCD_File.Get("MET_baseline_%s%s_LSP1" %(model,mass));
    zeroSB=ABCD_File.Get("MET_antitagSB_%s%s_LSP1" %(model,mass));
    zeroSR=ABCD_File.Get("MET_antitagSR_%s%s_LSP1" %(model,mass));
    singleSB=ABCD_File.Get("MET_tagSB_%s%s_LSP1" %(model,mass));
    singleSR=ABCD_File.Get("MET_tagSR_%s%s_LSP1" %(model,mass));
    doubleSB=ABCD_File.Get("MET_doubletagSB_%s%s_LSP1" %(model,mass));
    doubleSR=ABCD_File.Get("MET_doubletagSR_%s%s_LSP1" %(model,mass));
    vecHists = [baseline,zeroSB,zeroSR,singleSB,singleSR,doubleSB,doubleSR]

    for xbin in range(len(vecHists)):
        h_sigEff.GetXaxis().SetBinLabel(xbin+1, binLabelsX[xbin])
        thisHist = vecHists[xbin]
        for ybin in range(4):
            if ybin==0: num = thisHist.Integral(0,5)
            elif ybin==3: num = thisHist.GetBinContent(ybin)+thisHist.GetBinContent(ybin+1)
            else: num = thisHist.GetBinContent(ybin)
            sigEff = num/denom
            if printEff: print("Efficiency for %s, %s: %.4f%%" %(binLabelsX[xbin],binLabelsY[ybin],sigEff*100.0))
            h_sigEff.SetBinContent(xbin+1,ybin+1,sigEff)

    for ybin in range(4): h_sigEff.GetYaxis().SetBinLabel(ybin+1, binLabelsY[ybin])
    h_sigEff.SetStats(0)
    h_sigEff.SaveAs(outDIR+"CMS-SUS-20-004_aux_Table_002.root")

def main(args=None):
    if args is None:
        args = sys.argv[1:]

    doSaveEff = True
    if(len(args)) > 0 and args[0] == 'False':
        doSaveEff = False

    # findWhichBins() # prints out indices of rate vector when reading from a datacard - for debugging
    '''
    Saves the efficiencies into a text file (boosted only, resolved only, combined)
    Will run automatically if the needed txt file can't be found
    arg1, model: "N1N2", "TChiHH", "T5HH"
    '''
    if doSaveEff:
        saveEff("TChiHH")
        saveEff("N1N2")
        saveEff("T5HH")


    '''
    Creates and saves the signal efficiency plots in the paper
    makeCanvas(arg1, arg2),
    arg1, model: "N1N2", "TChiHH", "T5HH"
    arg2, plotting: "comb", "boost", "res", "all" - "all" plots all three lines on the same canvas so doesn't work for 2D
    '''
    makeCanvas("TChiHH", "all", "aux_003-a")
    makeCanvas("TChiHH", ['1', '2', '3', '4'], "aux_003-c")
    makeCanvas("TChiHH", ['5', '6', '7', '8'], "aux_003-d")
    makeCanvas("TChiHH", ['9', '10', '11', '12'], "aux_003-g")
    makeCanvas("TChiHH", ['13', '14', '15', '16'], "aux_003-h")
    makeCanvas("TChiHH", ['17', '18', '19'], "aux_003-k")
    makeCanvas("TChiHH", ['20', '21', '22'], "aux_003-l")
    makeCanvas("T5HH", "all", "aux_003-b")
    makeCanvas("T5HH", ['1', '2', '3', '4'], "aux_003-e")
    makeCanvas("T5HH", ['5', '6', '7', '8'], "aux_003-f")
    makeCanvas("T5HH", ['9', '10', '11', '12'], "aux_003-i")
    makeCanvas("T5HH", ['13', '14', '15', '16'], "aux_003-j")
    makeCanvas("T5HH", ['17', '18', '19'], "aux_003-m")
    makeCanvas("T5HH", ['20', '21', '22'], "aux_003-n")
    makeCanvas("N1N2", "res", "aux_004-a")
    makeCanvas("N1N2", "boost", "aux_004-b")
    makeCanvas("N1N2", "comb", "aux_004-c")
    for i in range(1, 23):
        makeCanvas("N1N2", str(i), "aux_004-"+chr(96+3+i))


    '''
    Save the root file for HEPData (not setup for 2D models)
    This is the signal efficiency for all boosted bins, Table 2 in the auxilary material
    saveSigEffMassRootFile(arg1, arg2, arg3)
    arg1, model: "TChiHH" or "T5HH"
    arg2, NLSP/gluino mass point (LSP=1)
    arg3, prints the efficiency for each bin to the screen
    '''
    #saveSigEffMassRootFile("T5HH", "2200", False)

if __name__ == "__main__":
    main()
