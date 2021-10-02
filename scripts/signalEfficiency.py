import os
from ROOT import *
import sys
import array
from array import array
from ROOT import gROOT
gROOT.SetBatch(True)


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


signalSkimsT5HHDIR = "/eos/uscms/store/user/emacdona/Skims/Run2ProductionV18/scan/tree_signal_METVars_FullSIM/"
signalSkimsTChiHHDIR = "/eos/uscms/store/user/kaulmer/Skims/Run2ProductionV18/scan/tree_signal_METVars/"
datacardsDIR = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/forGithub/CMSSW_10_2_13/src/boostedHiggsPlusMET/datacards/"
outDIR = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/forGithub/CMSSW_10_2_13/src/boostedHiggsPlusMET/output/"

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

def higgsinoCrossSection2D( hig_mass):
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
    datacardName = "1DT5HH1000_LSP1_Data_Combo.txt"
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
    effFile=open(datacardsDIR+"efficiency_"+model+".txt", 'w');
    NLSPmass = []; LSPmass = [];
    if model=="N1N2":
        effFile.write("#NLSPmass LSPmass boostEff resEff totalEff\n")
        fNames=open(datacardsDIR+"higgsino2DFileNames.txt", 'r');
        for line in fNames:
            x = line.split('_')
            if (int(x[5])>800): continue;
            NLSPmass.append(x[5]); LSPmass.append(x[6]);
        fNames.close();
        for h in range(0, 26):
            thisMass = 175+h*25;
            NLSPmass.append(str(thisMass)); LSPmass.append("1");
    elif model=="TChiHH":
        effFile.write("#LSPmass Gmass boostEff resEff totalEff\n")
        for h in range(0, 42):
            thisMass = 175+h*25;
            if thisMass==1025 or thisMass==1075 or thisMass==1125 or thisMass==1175: continue;
            NLSPmass.append(str(thisMass)); LSPmass.append("1");
    elif model=="T5HH":
        effFile.write("#gluinoMass LSPmass boostEff resEff totalEff\n")
        for g in range(0, 16):
            thisMass = 1000+g*100;
            NLSPmass.append(str(thisMass)); LSPmass.append("1");

    for i in range(len(NLSPmass)):
        print("For %s, mNLSP=%s and mLSP=%s" %(model, NLSPmass[i],LSPmass[i]))
        BR = 0.5823329; convertXsec = 1.0; signalFileName = ""; modelName = ""
        if model=="N1N2":
            BR = 0.5823329; modelName="2DTChiHH"
            if (LSPmass[i]=="1"):
                signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_"+NLSPmass[i]+"_1_MC2016_fast.root"
                convertXsec = higgsinoCrossSection1D(NLSPmass[i])/higgsinoCrossSection2D(NLSPmass[i])
            else:
                signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_2D_"+NLSPmass[i]+"_"+LSPmass[i]+"_MC2016_fast.root"
        elif model=="TChiHH":
            BR = 0.5823329; modelName="1DTChiHH"
            signalFileName = signalSkimsTChiHHDIR+"tree_TChiHH_HToBB_HToBB_"+NLSPmass[i]+"_1_MC2016_fast.root"
        elif model=="T5HH":
            BR = 1.0; modelName="1DT5HH"
            signalFileName = signalSkimsT5HHDIR+"tree_T5qqqqZH-mGluino-1000to2500_"+NLSPmass[i]+"_1_MC2016.root"

        signalFile = TFile(signalFileName, "r")
        tree = signalFile.Get("tree"); tree.GetEntry(0);
        weight=getattr(tree,"Weight");
        if weight==0:
            tree.GetEntry(3642); weight=getattr(tree,"Weight");
            if weight==0:
                tree.GetEntry(4477); weight=getattr(tree,"Weight");
                if weight==0:
                    tree.GetEntry(4483); weight=getattr(tree,"Weight");
                    if weight==0:
                        tree.GetEntry(6160); weight=getattr(tree,"Weight");
                        if weight==0: print("Weight is zero")

        #OK have weight now
        if weight==0: print("Weight is zero")
        else: signalFile.Close()
        totalEvt1 = weight*35922.0*BR*BR/convertXsec
        totalEvt2 = weight*41529.0*BR*BR/convertXsec
        totalEvt3 = weight*59740.0*BR*BR/convertXsec
        totalEvt = totalEvt1+totalEvt2+totalEvt3

        #open datacard
        datacardName = modelName+NLSPmass[i]+"_LSP"+LSPmass[i]+"_Data_Combo.txt"
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
        line = "%s %s %.7f %.7f %.7f\n" %(NLSPmass[i], LSPmass[i], boostEff, resEff, totalEff)
        effFile.write(line)
    effFile.close()


def readInValues(model, which):
    vmx=array('d',[]); vmy=array('d',[]); veff=array('d',[]);
    fcard=open(datacardsDIR+"efficiency_"+model+".txt", 'r');
    for line in fcard:
        if "#" in line: continue;
        thisLine = line.split()
        vmx.append(Double(thisLine[0])); vmy.append(Double(thisLine[1]));
        if which=="boost": veff.append(Double(thisLine[2]));
        elif which=="res": veff.append(Double(thisLine[3]));
        elif which=="comb": veff.append(Double(thisLine[4]));
    if model=="N1N2":
        histo = TGraph2D("", "", len(veff), vmx, vmy, veff);
        return(histo)
    elif model=="TChiHH" or model=="T5HH":
        histo = TGraph(len(veff), vmx, veff);
        return(histo)

def makeCanvas(model, which):
    SignifScan2 = TGraph2D()
    SignifScan = TGraph(); SignifScanBoost = TGraph(); SignifScanRes = TGraph();
    canvName = model+"_"+which
    if model=="N1N2":
        SignifScan2 = readInValues(model, which);
        SignifScan2.SetNpx(300); SignifScan2.SetNpy(300);
        SignifScan = SignifScan2.GetHistogram(); SignifScan.SetStats(0);
        canv = TCanvas(canvName,"",1200,1050)
        canv.SetRightMargin(0.18);
    elif model=="TChiHH" or model=="T5HH":
        if (which=="all"):
            SignifScan = readInValues(model, "comb");
            SignifScanBoost = readInValues(model, "boost");
            SignifScanRes = readInValues(model, "res");
        else: SignifScan = readInValues(model, which);
        canv = TCanvas(canvName,"",1200,1200)
        canv.SetRightMargin(0.04);
    canv.cd()
    canv.SetLeftMargin(0.13); canv.SetBottomMargin(0.11);canv.SetTopMargin(0.08)
    SignifScan.SetMinimum(0.000005); SignifScan.SetMaximum(1.0);

    zAxisTitle = "Signal Efficiency" #although for the 1D plots this is y-axis title
    if which=="boost": zAxisTitle = "Boosted Signal Efficiency"
    elif which=="res": zAxisTitle = "Resolved Signal Efficiency"
    elif which=="comb": zAxisTitle = "Combined Signal Efficiency"

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

    ltitle = TLatex(0.17, 0.87,"#font[62]{CMS}#scale[0.76]{#font[52]{ Simulation Supplementary}}")
    ltitle.SetNDC(); ltitle.SetTextAlign(12);

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
        SignifScan.SetContour(150)
        SignifScan.Draw("colz")
    elif model=="TChiHH" or model=="T5HH":
        canv.SetLogy()
        if model=="TChiHH":
            SignifScan.SetMaximum(2.5); SignifScan.SetMinimum(0.001)
            SignifScan.GetXaxis().SetRangeUser(180.0,1220.0);
        elif model=="T5HH":
            if (which=="boost" or which=="comb"): SignifScan.SetMaximum(1.0); SignifScan.SetMinimum(0.01)
            elif (which=="all"): SignifScan.SetMaximum(1.0); SignifScan.SetMinimum(0.0001)
            elif (which=="res"): SignifScan.SetMaximum(1.0); SignifScan.SetMinimum(0.0001)
            SignifScan.GetXaxis().SetRangeUser(950.0,2550.0);
        SignifScan.SetMarkerStyle(20); SignifScan.SetMarkerSize(1.8); SignifScan.SetMarkerColor(kBlack);
        SignifScan.Draw("APE")

        if (which=="all"):
            SignifScanBoost.SetMarkerStyle(20); SignifScanBoost.SetMarkerSize(1.8); SignifScanBoost.SetMarkerColor(kRed); SignifScanBoost.SetLineColor(kRed);
            SignifScanRes.SetMarkerStyle(20); SignifScanRes.SetMarkerSize(1.8); SignifScanRes.SetMarkerColor(kBlue); SignifScanRes.SetLineColor(kBlue);
            SignifScanBoost.Draw("PE same"); SignifScanRes.Draw("PE same");
            if model=="TChiHH": leg = TLegend(0.6,0.17,0.85,0.43);
            elif model=="T5HH": leg = TLegend(0.2,0.17,0.45,0.43);
            leg.SetFillStyle(4050); leg.SetFillColor(0);
            leg.SetTextFont(42); leg.SetBorderSize(0);
            leg.SetTextSize(0.045);
            leg.AddEntry(SignifScanRes, "Resolved","lp")
            leg.AddEntry(SignifScanBoost, "Boosted","lp")
            leg.AddEntry(SignifScan, "Combination","lp")
            leg.Draw("same")

    rtitle.SetTextFont(42); rtitle.SetNDC(); rtitle.SetTextAlign(32)
    ltitle.Draw("same"); rtitle.Draw("same"); txtd.Draw("same")
    if model=="TChiHH": txte.Draw("same")

    canv.SaveAs(outDIR+model+"_signalEfficiency_"+which+".pdf", "PDF")

def saveSigEffRootFile():
    '''
    For HEPData, saves all signal efficiency plots as root files
    '''
    SignifScan2DG = TGraph2D(); SignifScan2DGBoost = TGraph2D(); SignifScan2DGRes = TGraph2D();
    SignifScan2D = TGraph(); SignifScan2DBoost = TGraph(); SignifScan2DRes = TGraph();
    SignifScan1D = TGraph(); SignifScan1DBoost = TGraph(); SignifScan1DRes = TGraph();
    SignifScan1Dglu = TGraph(); SignifScan1DgluBoost = TGraph(); SignifScan1DgluRes = TGraph();
    SignifScan2DG = readInValues("N1N2", "comb")
    SignifScan2DGBoost = readInValues("N1N2", "boost")
    SignifScan2DGRes = readInValues("N1N2", "res")
    SignifScan1D = readInValues("TChiHH", "comb")
    SignifScan1DBoost = readInValues("TChiHH", "boost")
    SignifScan1DRes = readInValues("TChiHH", "res")
    SignifScan1Dglu = readInValues("T5HH", "comb")
    SignifScan1DgluBoost = readInValues("T5HH", "boost")
    SignifScan1DgluRes = readInValues("T5HH", "res")

    SignifScan2DG.SetNpx(300); SignifScan2DG.SetNpy(300);
    SignifScan2DGBoost.SetNpx(300); SignifScan2DGBoost.SetNpy(300);
    SignifScan2DGRes.SetNpx(300); SignifScan2DGRes.SetNpy(300);
    SignifScan2D = SignifScan2DG.GetHistogram(); SignifScan2DBoost = SignifScan2DGBoost.GetHistogram(); SignifScan2DRes = SignifScan2DGRes.GetHistogram();

    #N1N2
    SignifScan2D.SetTitle(";m("+chi02+") [GeV]; m("+chi01+") [GeV];Combined Signal Efficiency")
    SignifScan2DBoost.SetTitle(";m("+chi02+") [GeV]; m("+chi01+") [GeV];Boosted Signal Efficiency")
    SignifScan2DRes.SetTitle(";m("+chi02+") [GeV]; m("+chi01+") [GeV];Resolved Signal Efficiency")

    #TChiHH
    SignifScan1D.SetTitle(";m("+chi01nospace+") [GeV];Combined Signal Efficiency")
    SignifScan1DBoost.SetTitle(";m("+chi01nospace+") [GeV];Boosted Signal Efficiency")
    SignifScan1DRes.SetTitle(";m("+chi01nospace+") [GeV];Resolved Signal Efficiency")

    #T5HH
    SignifScan1Dglu.SetTitle(";m("+glu+") [GeV];Combined Signal Efficiency")
    SignifScan1DgluBoost.SetTitle(";m("+glu+") [GeV];Boosted Signal Efficiency")
    SignifScan1DgluRes.SetTitle(";m("+glu+") [GeV];Resolved Signal Efficiency")

    canv = TCanvas("canv","",1200,1050)
    SignifScan2D.SetContour(150); SignifScan2DBoost.SetContour(150); SignifScan2DRes.SetContour(150);
    SignifScan2D.Draw("colz"); SignifScan2DBoost.Draw("colz"); SignifScan2DRes.Draw("colz");
    SignifScan1D.Draw(); SignifScan1DBoost.Draw(); SignifScan1DRes.Draw();
    SignifScan1Dglu.Draw(); SignifScan1DgluBoost.Draw(); SignifScan1DgluRes.Draw();


    fNEW = TFile(outDIR+"CMS-SUS-20-004_aux_SignalEfficiency.root", "recreate")
    SignifScan2D.Write("CombinedSignalEfficiency_N1N2"); SignifScan2DBoost.Write("BoostedSignalEfficiency_N1N2"); SignifScan2DRes.Write("ResolvedSignalEfficiency_N1N2");
    SignifScan1D.Write("CombinedSignalEfficiency_TChiHH"); SignifScan1DBoost.Write("BoostedSignalEfficiency_TChiHH"); SignifScan1DRes.Write("ResolvedSignalEfficiency_TChiHH");
    SignifScan1Dglu.Write("CombinedSignalEfficiency_T5HH"); SignifScan1DgluBoost.Write("BoostedSignalEfficiency_T5HH"); SignifScan1DgluRes.Write("ResolvedSignalEfficiency_T5HH");
    fNEW.Close()

def saveSigEffMassRootFile(model, mass, printEff):
    '''
    For HEPData, saves a root file with one TH2F for the signal efficiency of the
    chosen signal model and mass point.
    y-axis is binned in MET (integrated, 300-500, 500-700, 700+)
    x-axis is the analysis region bins (Baseline, 0HSB, 0HSR, 1HSB, 1HSR, 2HSB, 2HSR)
    Note: this signal efficiency ignores the fastSIM MET treatment (for TChiHH only), only using the yields from the recoMET
    '''
    #Denominator of efficiency is all H decays for T5HH and only H->bb decays for TChiHH
    #First, define TH2D to fill, set bin labels, too
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
    tree = signalFile.Get("tree"); tree.GetEntry(0);
    weight=getattr(tree,"Weight");
    if weight==0:
        tree.GetEntry(3642); weight=getattr(tree,"Weight");
        if weight==0:
            tree.GetEntry(4477); weight=getattr(tree,"Weight");
            if weight==0: print("Weight is zero")

    #OK have weight now
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

def main():
    # findWhichBins() # prints out indices of rate vector when reading from a datacard

    saveEff("N1N2") # saves the efficiencies into a text file
    saveEff("TChiHH")
    saveEff("T5HH")

    #makeCanvas(arg1, arg2),
    #arg1, model: "N1N2", "TChiHH", "T5HH"
    #arg2, plotting: "comb", "boost", "res", "all" - "all" plots all three lines on the same canvas so doesn't work for 2D

    makeCanvas("TChiHH", "all")
    makeCanvas("T5HH", "all")
    makeCanvas("N1N2", "boost")
    makeCanvas("N1N2", "res")
    makeCanvas("N1N2", "comb")

    #save root file for HEPData (not setup for 2D models), 3rd argument is printing efficiencies to screen
    saveSigEffMassRootFile("T5HH", "2200", False)

    saveSigEffRootFile()

if __name__ == "__main__":
    main()
