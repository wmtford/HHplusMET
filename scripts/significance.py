import os
import glob
import math
import array
import sys
import time
import ROOT
from array import array
from ROOT import *
gROOT.SetBatch(True)

datacardsDIR = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/myVersion/CMSSW_10_2_13/src/boostedHiggsPlusMET/datacards/"
outDIR = "../output/"

def columnToList(fn,col):
    f = open(fn,'r');
    olist = [];
    for line in f:
        linelist = line.strip().split()
        olist.append( linelist[col] );
    return olist

#I think the limits tree for significance only has a single value in it
def ExtractFile(iname):
    f = ROOT.TFile(iname);
    t = f.Get("limit");
    lims = 0.0
    t.GetEntry(0);
    lims =  t.limit
    return lims;

def saveCanvas(model):
    vmx=array('d',[]); vmy=array('d',[]); vsig=array('d',[]);
    if model=="N1N2":
        namesFile=open(datacardsDIR+"higgsino2DFileNames.txt", 'r');
        for line in namesFile:
            x = line.split('_')
            hino_mass = int(x[5])
            LSP_mass = int(x[6])
            if hino_mass>810: break;
            if hino_mass==200 and LSP_mass==50: continue;
            if hino_mass==250 and LSP_mass==75: continue;
            if hino_mass==250 and LSP_mass==100: continue;
            if hino_mass==275 and LSP_mass==100: continue;
            if hino_mass==300 and LSP_mass==150: continue;

            vmx.append(hino_mass); vmy.append(LSP_mass)
            thisSig = ExtractFile(datacardsDIR+"higgsCombine2DTChiHH%s_LSP%s_Data_Combo.Significance.mH120.root" %(x[5], x[6]))

            #Smoothing diagonal
            if hino_mass==150 and LSP_mass==23: thisSig = 0.5
            if hino_mass==175 and LSP_mass==25: thisSig = 0.5
            if hino_mass==175 and LSP_mass==48: thisSig = 0.5
            if hino_mass==225 and LSP_mass==98: thisSig = 1.0
            if hino_mass==325 and LSP_mass==198: thisSig = 1.0
            if hino_mass==350 and LSP_mass==200: thisSig = 1.0
            if hino_mass==375 and LSP_mass==248: thisSig = 1.0
            if hino_mass==400 and LSP_mass==250: thisSig = 1.0
            if hino_mass==400 and LSP_mass==273: thisSig = 1.0
            if hino_mass==425 and LSP_mass==298: thisSig = 1.0
            if hino_mass==500 and LSP_mass==350: thisSig = 1.0
            if hino_mass==500 and LSP_mass==373: thisSig = 1.0
            if hino_mass==550 and LSP_mass==423: thisSig = 1.0
            if hino_mass==600 and LSP_mass==473: thisSig = 1.0
            if hino_mass==625 and LSP_mass==498: thisSig = 1.0
            if hino_mass==650 and LSP_mass==523: thisSig = 1.0
            if hino_mass==700 and LSP_mass==573: thisSig = 1.0
            vsig.append(thisSig)

        hino=[];
        for h in range(0, 27):
            hino.append(150+h*25)

        for h in hino:
            vmx.append(h)
            vmy.append(1.0)
            thisSig =  ExtractFile(datacardsDIR+"higgsCombine2DTChiHH%i_LSP1_Data_Combo.Significance.mH120.root" %(h))
            vsig.append(thisSig)
            for lsp in hino:
                if (lsp+100) > h:
                    vmx.append(h)
                    vmy.append(lsp)
                    vsig.append(-999)

    elif model == "T5HH":
        for i in range(0, 16):
            g=1000+i*100
            vmx.append(g)
            thisSig =  ExtractFile(datacardsDIR+"higgsCombine1DT5HH%i_LSP1_Data_Combo.Significance.mH120.root" %(g))
            vsig.append(thisSig)

    canv = TCanvas()
    if model == "N1N2": canv = TCanvas(model,"",1200,1100)
    elif model == "T5HH": canv = TCanvas(model,"",800,800)
    canv.cd()
    canv.SetLeftMargin(0.13)
    canv.SetBottomMargin(0.11)

    xparticle = ""
    if model == "N1N2": xparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{2}}}";
    elif model == "T5HH": xparticle = "#tilde{g}#scale[0.55]{_{ }}";
    yparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";

    ltitle = TLatex(0.16, 0.86,"#font[62]{CMS}#scale[0.76]{#font[52]{ Supplementary}}")
    if model == "N1N2": rtitle = TLatex(0.84, 0.925,"#scale[0.75]{137 fb^{-1} (13 TeV)}")
    elif model == "T5HH": rtitle = TLatex(0.96, 0.925,"#scale[0.75]{137 fb^{-1} (13 TeV)}")
    rtitle.SetTextFont(42)
    ltitle.SetNDC(); rtitle.SetNDC()
    ltitle.SetTextAlign(12); rtitle.SetTextAlign(32)


    SignifScan2 = TGraph2D(); SignifScan = TGraph()
    if model == "N1N2":
        canv.SetRightMargin(0.16)
        SignifScan2 = TGraph2D("", ";m("+xparticle+") [GeV]; m("+yparticle+") [GeV]; Significance [#sigma]", len(vsig), vmx, vmy, vsig);
        SignifScan2.SetNpx(300); SignifScan2.SetNpy(300);
        SignifScan = SignifScan2.GetHistogram()
        SignifScan.SetTitle(";m("+xparticle+") [GeV]; m("+yparticle+") [GeV]; Significance [#sigma]")
        SignifScan.GetZaxis().SetTitleOffset(1.1); SignifScan.GetZaxis().SetTitleSize(0.047)
        SignifScan.GetYaxis().SetTitleOffset(1.2); SignifScan.GetYaxis().SetTitleSize(0.047)
        SignifScan.GetXaxis().SetTitleOffset(1.0); SignifScan.GetXaxis().SetTitleSize(0.047)
        SignifScan.SetMinimum(-1.0); SignifScan.SetMaximum(2.5);
        SignifScan.GetXaxis().SetRangeUser(150.0,800.0); SignifScan.GetYaxis().SetRangeUser(1.0,650.0)
        SignifScan.SetContour(150)
        SignifScan.Draw("colz")
    elif model == "T5HH":
        canv.SetRightMargin(0.04)
        SignifScan = TGraph(len(vsig), vmx, vsig);
        SignifScan.SetTitle(";m("+xparticle+") [GeV]; Significance [#sigma]")
        SignifScan.GetYaxis().SetTitleOffset(1.2); SignifScan.GetYaxis().SetTitleSize(0.047)
        SignifScan.GetXaxis().SetTitleOffset(1.0); SignifScan.GetXaxis().SetTitleSize(0.047)
        SignifScan.SetMinimum(-1.5); SignifScan.SetMaximum(0.0);
        SignifScan.GetXaxis().SetRangeUser(975.0,2525.0);
        SignifScan.SetMarkerStyle(20); SignifScan.SetMarkerSize(1.0); SignifScan.SetMarkerColor(kBlack); SignifScan.SetLineColor(kBlack);
        SignifScan.Draw("APE")
    ltitle.Draw("same")
    rtitle.Draw("same")

    canv.SaveAs(outDIR+"significance_"+model+".pdf","PDF")

def saveRootFile():
    vmx=array('d',[]); vmy=array('d',[]); vsig=array('d',[]);
    namesFile=open(datacardsDIR+"higgsino2DFileNames.txt", 'r');
    for line in namesFile:
        x = line.split('_')
        hino_mass = int(x[5])
        LSP_mass = int(x[6])
        if hino_mass>810: break;
        if hino_mass==200 and LSP_mass==50: continue;
        if hino_mass==250 and LSP_mass==75: continue;
        if hino_mass==250 and LSP_mass==100: continue;
        if hino_mass==275 and LSP_mass==100: continue;
        if hino_mass==300 and LSP_mass==150: continue;

        vmx.append(hino_mass)
        vmy.append(LSP_mass)
        thisSig = ExtractFile(datacardsDIR+"higgsCombine2DTChiHH%s_LSP%s_Data_Combo.Significance.mH120.root" %(x[5], x[6]))

        #Smoothing diagonal
        if hino_mass==150 and LSP_mass==23: thisSig = 0.5
        if hino_mass==175 and LSP_mass==25: thisSig = 0.5
        if hino_mass==175 and LSP_mass==48: thisSig = 0.5
        if hino_mass==225 and LSP_mass==98: thisSig = 1.0
        if hino_mass==325 and LSP_mass==198: thisSig = 1.0
        if hino_mass==350 and LSP_mass==200: thisSig = 1.0
        if hino_mass==375 and LSP_mass==248: thisSig = 1.0
        if hino_mass==400 and LSP_mass==250: thisSig = 1.0
        if hino_mass==400 and LSP_mass==273: thisSig = 1.0
        if hino_mass==425 and LSP_mass==298: thisSig = 1.0
        if hino_mass==500 and LSP_mass==350: thisSig = 1.0
        if hino_mass==500 and LSP_mass==373: thisSig = 1.0
        if hino_mass==550 and LSP_mass==423: thisSig = 1.0
        if hino_mass==600 and LSP_mass==473: thisSig = 1.0
        if hino_mass==625 and LSP_mass==498: thisSig = 1.0
        if hino_mass==650 and LSP_mass==523: thisSig = 1.0
        if hino_mass==700 and LSP_mass==573: thisSig = 1.0
        vsig.append(thisSig)


    hino=[];
    for h in range(0, 27):
        hino.append(150+h*25)

    for h in hino:
        vmx.append(h)
        vmy.append(1.0)
        thisSig =  ExtractFile(datacardsDIR+"higgsCombine2DTChiHH%i_LSP1_Data_Combo.Significance.mH120.root" %(h))
        vsig.append(thisSig)
        for lsp in hino:
            if (lsp+100) > h:
                vmx.append(h)
                vmy.append(lsp)
                vsig.append(-999)

    vmx_g=array('d',[]); vsig_g=array('d',[]);
    for i in range(0, 16):
        g=1000+i*100
        vmx_g.append(g)
        thisSig =  ExtractFile(datacardsDIR+"higgsCombine1DT5HH%i_LSP1_Data_Combo.Significance.mH120.root" %(g))
        vsig_g.append(thisSig)


    xparticle_N1N2 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{2}}}";
    xparticle_glu = "#tilde{g}#scale[0.55]{_{ }}";
    yparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";


    SignifScan2 = TGraph2D(); SignifScan = TGraph(); SignifScan_g = TGraph();
    SignifScan2 = TGraph2D("", ";m("+xparticle_N1N2+") [GeV]; m("+yparticle+") [GeV]; Significance [#sigma]", len(vsig), vmx, vmy, vsig);
    SignifScan2.SetNpx(300); SignifScan2.SetNpy(300);
    SignifScan = SignifScan2.GetHistogram()
    SignifScan.SetTitle(";m("+xparticle_N1N2+") [GeV]; m("+yparticle+") [GeV]; Significance [#sigma]")
    SignifScan.GetZaxis().SetTitleOffset(1.1); SignifScan.GetZaxis().SetTitleSize(0.047)
    SignifScan.GetYaxis().SetTitleOffset(1.2); SignifScan.GetYaxis().SetTitleSize(0.047)
    SignifScan.GetXaxis().SetTitleOffset(1.0); SignifScan.GetXaxis().SetTitleSize(0.047)
    SignifScan.SetMinimum(-1.0); SignifScan.SetMaximum(2.5);
    SignifScan.GetXaxis().SetRangeUser(150.0,800.0); SignifScan.GetYaxis().SetRangeUser(1.0,650.0)
    SignifScan.SetContour(150)

    SignifScan_g = TGraph(len(vsig_g), vmx_g, vsig_g);
    SignifScan_g.SetTitle(";m("+xparticle_glu+") [GeV]; Significance [#sigma]")
    SignifScan_g.GetYaxis().SetTitleOffset(1.2); SignifScan_g.GetYaxis().SetTitleSize(0.047)
    SignifScan_g.GetXaxis().SetTitleOffset(1.0); SignifScan_g.GetXaxis().SetTitleSize(0.047)
    SignifScan_g.SetMinimum(-1.5); SignifScan_g.SetMaximum(0.0);
    SignifScan_g.GetXaxis().SetRangeUser(975.0,2525.0);
    SignifScan_g.SetMarkerStyle(20); SignifScan_g.SetMarkerSize(1.0); SignifScan_g.SetMarkerColor(kBlack); SignifScan_g.SetLineColor(kBlack);

    fNEW = TFile(outDIR+"CMS-SUS-20-004_aux_significance.root", "recreate")
    SignifScan.Write("TChiHH")
    SignifScan_g.Write("T5HH")
    fNEW.Close()

if __name__ == '__main__':
    '''
    Save the canvases of the significance for the combination only
    Can run for 2D TChiHH "N1N2" or the 1D T5HH "T5HH"
    '''
    saveCanvas("N1N2")
    saveCanvas("T5HH")


    '''
    Save the root file containing the significance plots for both TChiHH and T5HH for HEPData
    '''
    # saveRootFile()
