import os
from ROOT import *
import sys
import array
from array import array
from ROOT import gROOT
gROOT.SetBatch(True)

file_DIR = "../datacards/";
out_DIR = "../output/";

def returnCovarHisto(which,covarNew):
    '''
    Returns a new covariance matrix with just the signal bins
    '''
    fileName=""; histoName=""
    if which=="pre":
        fileName = "fitDiagnostics_t0.root"
        histoName = "shapes_prefit/overall_total_covar"
    elif which=="b":
        fileName = "fitDiagnostics_t0.root"
        histoName = "shapes_fit_b/overall_total_covar"
    elif which=="sb":
        fileName = "fitDiagnostics_t1.root"
        histoName = "shapes_fit_s/overall_total_covar"
    thisFile=TFile(file_DIR+fileName, 'r');
    cover_b = thisFile.Get(histoName);
    xLabels = []; yLabels = []; xLabelsIndex = []; yLabelsIndex = [];
    nbinsx = cover_b.GetNbinsX(); nbinsy = cover_b.GetNbinsY();

    for x in range(nbinsx+1):
        if x==0: continue
        xlabel = cover_b.GetXaxis().GetBinLabel(x)
        keepBin=False
        if (xlabel.find('ch1_ch1_') != -1):
            keepBin=True
        elif (xlabel.find('ch2_xsig0_ysig_') != -1):
            keepBin=True
        elif (xlabel.find('ch2_xsig1_ysig_') != -1):
            keepBin=True
        if keepBin:
            xLabels.append(xlabel)
            xLabelsIndex.append(x)

    for y in range(nbinsy+1):
        if y==0: continue
        ylabel = cover_b.GetYaxis().GetBinLabel(y)
        keepBin=False
        if (ylabel.find('ch1_ch1_') != -1):
            keepBin=True
        elif (ylabel.find('ch2_xsig0_ysig_') != -1):
            keepBin=True
        elif (ylabel.find('ch2_xsig1_ysig_') != -1):
            keepBin=True
        if keepBin:
            yLabels.append(ylabel)
            yLabelsIndex.append(y)


    countX = 0;
    for xIndex in xLabelsIndex:
        countX=countX+1
        covarNew.GetXaxis().SetBinLabel(countX,xLabels[countX-1])
        countY = 0;
        for yIndex in yLabelsIndex:
            countY=countY+1
            thisBinContent = cover_b.GetBinContent(xIndex,yIndex)
            covarNew.SetBinContent(countX,countY,thisBinContent)

    for y in range(len(yLabelsIndex)):
        covarNew.GetYaxis().SetBinLabel(y+1,yLabels[y])

    return(covarNew)

def returnCorrelHisto(which,correlation_full_new):
    '''
    Returns the full correlation matrix for now
    '''

    correlation_full = TH2D()
    fileName=""; histoName=""
    if which=="b":
        fileName = "fitDiagnostics_t0.root"
        histoName = "covariance_fit_b"
    elif which=="sb":
        fileName = "fitDiagnostics_t1.root"
        histoName = "covariance_fit_s"

    thisFile=TFile(file_DIR+fileName, 'r');
    correlation_full = thisFile.Get(histoName)

    xLabels = []; yLabels = []; xLabelsIndex = []; yLabelsIndex = [];
    nbinsx = correlation_full.GetNbinsX(); nbinsy = correlation_full.GetNbinsY();

    for x in range(nbinsx+1):
        if x==0: continue
        xlabel = correlation_full.GetXaxis().GetBinLabel(x)
        xLabels.append(xlabel); xLabelsIndex.append(x)

    for y in range(nbinsy+1):
        if y==0: continue
        ylabel = correlation_full.GetYaxis().GetBinLabel(y)
        yLabels.append(ylabel); yLabelsIndex.append(y)


    countX = 0;
    for xIndex in xLabelsIndex:
        countX=countX+1
        correlation_full_new.GetXaxis().SetBinLabel(countX,xLabels[countX-1])
        countY = 0;
        for yIndex in yLabelsIndex:
            countY=countY+1
            thisBinContent = correlation_full.GetBinContent(xIndex,yIndex)
            correlation_full_new.SetBinContent(countX,countY,thisBinContent)

    for y in range(len(yLabelsIndex)):
        if y==0: continue
        correlation_full_new.GetYaxis().SetBinLabel(y,yLabels[y-1])

    return(correlation_full_new)



def main():
    covarNew_pre = TH2D("covariance_SRs_prefit","", 22,0,22,22,0,22)
    covarNew_pre = returnCovarHisto("pre",covarNew_pre)
    covarNew_b = TH2D("covariance_SRs_b","", 22,0,22,22,0,22)
    covarNew_b = returnCovarHisto("b",covarNew_b)
    covarNew_sb = TH2D("covariance_SRs_sb","", 22,0,22,22,0,22)
    covarNew_sb = returnCovarHisto("sb",covarNew_sb)


    correlation_full_b = TH2D("correlation_full_b","", 143,0,143,143,0,143)
    correlation_full_b = returnCorrelHisto("b",correlation_full_b)
    correlation_full_sb = TH2D("correlation_full_sb","", 144,0,144,144,0,144)
    correlation_full_sb = returnCorrelHisto("sb",correlation_full_sb)

    fNEW = TFile(out_DIR+"CMS-SUS-20-004_aux_Matrices.root", "recreate")


    covarNew_pre.Write()
    covarNew_b.Write()
    covarNew_sb.Write()

    correlation_full_b.Write()
    correlation_full_sb.Write()

    fNEW.Close()


if __name__ == "__main__":
    main()
