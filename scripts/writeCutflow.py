import os
from ROOT import *
import sys
import array
from array import array
from ROOT import gROOT
gROOT.SetBatch(True)

'''
Make sure you run cutflowFromNtuples.cc (signal only, run directly from ntuples) to produce the root files
There are only a few samples this was run on, so I am assuming you are aware of that
'''
fileDir = "/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/cutflow_veto/"
outDir = "../output/"

def writeCutflow(doDetailed):
    '''
    !doDetailed is the cutflow in auxilary material, T5HH1000 vs T5HH1600 vs T5HH2200
    doDetailed compares TChiHH950 to T5HH1000, 4b events only, with a lot more lines. Used for internal studies
    '''
    #Step 1) Read in cutflow root files
    signalNames = []; vecFiles = []
    if doDetailed:
        file1 = "cutflow_allYears_TChiHH950_detailed.root"
        file2 = "cutflow_allYears_T5HH1000_detailed.root"
        signalNames = ["TChiHH-G(950,1)", "T5HH(1000,1)"]
        vecFiles = [file1,file2]
    else:
        file1 = "cutflow_allYears_T5HH1000.root"
        file2 = "cutflow_allYears_T5HH1600.root"
        file3 = "cutflow_allYears_T5HH2200.root"
        signalNames = ["T5HH(1000,1)", "T5HH(1600,1)", "T5HH(2200,1)"]
        vecFiles = [file1,file2,file3]

    vecFiles_1H = []
    file1 = "cutflow_allYears_T5HH1000_1H.root"
    file2 = "cutflow_allYears_T5HH1600_1H.root"
    file3 = "cutflow_allYears_T5HH2200_1H.root"
    vecFiles_1H = [file1,file2,file3]

    #Step 2) Grab histograms and store info
    fancyCutNames = ["All events", "Hadronic baseline", "$N_{\mathrm{AK8}}>$1, jet $p_{\mathrm{T}}>$300 GeV","m$_{\mathrm{J1\&2}}$ [60,260] GeV"]
    fancyCutNames.append("Resolved event veto");
    fancyCutNames.append("$D_{bb,J1\&2}>$0.7"); fancyCutNames.append("m$_{\mathrm{J1\&2}}$ [95,145] GeV");
    fancyCutNames.append("$D_{bb,J1or2}>$0.7"); fancyCutNames.append("m$_{\mathrm{J1\&2}}$ [95,145] GeV");
    cutNames = []; vecEventCounts = [] #cutNames is for detailed only
    vecRelEff = []; vecTotEff = []
    doFillCuts = True
    whichFile = 0
    for fileName in vecFiles:
        eventCounts = []; relEff = []; totEff = [];
        file = TFile(fileDir+fileName, "r")
        listOfHistos = TIter(file.GetListOfKeys())
        histo = TH1F()
        i = 0
        for key in listOfHistos:
            histo = key.ReadObj()
            eventCounts.append(histo.Integral(0,21))
            if doFillCuts: cutNames.append(histo.GetName())
            if i==0:
                relEff.append(1.0); totEff.append(1.0);
            else:
                relEff.append(eventCounts[i]/eventCounts[i-1])
                if doDetailed and i==1: totEff.append(1.0)
                elif doDetailed: totEff.append(eventCounts[i]/eventCounts[1])
                else: totEff.append(eventCounts[i]/eventCounts[0])
            i=i+1
        if not doDetailed:
            fileName1H = vecFiles_1H[whichFile]
            file_1H = TFile(fileDir+fileName1H, "r")
            listOfHistos_1H = TIter(file_1H.GetListOfKeys())
            histo = TH1F()
            i = 0
            for key in listOfHistos_1H:
                i=i+1
                if (i<6): continue #don't need to save the other ones, just the final two lines for 1H
                histo = key.ReadObj()
                eventCounts.append(histo.Integral(0,21))
                if doFillCuts: cutNames.append(histo.GetName())
        vecEventCounts.append(eventCounts); vecRelEff.append(relEff); vecTotEff.append(totEff)
        doFillCuts = False
        file.Close(); whichFile=whichFile+1
        if not doDetailed: file_1H.Close();

    #Step 3) Write out latex file
    outFileName = "latexCutflow_supp.txt"
    if doDetailed: outFileName = "latexCutflow_compSignals.txt"
    outFile = open(outDir+outFileName, 'w');
    outFile.write("\\documentclass{standalone}\n\\begin{document}\n")
    if doDetailed:
        outFile.write("\\begin{tabular}{l|cc|cc|cc}\n")
        line = "Selection requirement ";
        for sample in signalNames: line = line+" & "+sample
        line = line+" & TChiHH Rel Eff & T5HH Rel Eff & TChiHH Tot Eff & T5HH Tot Eff \\\\ \\hline\n"
        outFile.write(line)
    else:
        outFile.write("\\begin{tabular}{l|ccc}\n")
        line = " ";
        for sample in signalNames: line = line+" & "+sample
        line = line + " \\\\ \n"
        outFile.write(line)
        outFile.write("Selection requirement & \\multicolumn{3}{c}{\\large{[Events]}}  \\\\ \\hline\n")
    line = ""
    for i in range(len(cutNames)):
        if doDetailed:
            outFile.write(cutNames[i])
            for j in range(len(vecEventCounts)): outFile.write(" & %.4f" %vecEventCounts[j][i])
            outFile.write(" & %.4f & %.4f & %.4f & %.4f" %(vecRelEff[0][i], vecRelEff[1][i], vecTotEff[0][i], vecTotEff[1][i]))
        else:
            outFile.write(fancyCutNames[i])
            for j in range(len(vecEventCounts)): outFile.write(" & %.1f" %vecEventCounts[j][i])
        outFile.write("  \\\\ \n")
        if not doDetailed and (i==4): outFile.write("\\hline \n")
    outFile.write("\\end{tabular}\n\\end{document}\n")
    outFile.close()

def makeCutflowRootFile():
    '''
    For HEPData, saves a root file with one TH2F with the same cutflow as in the auxilary material
    y-axis is the different samples (T5HH1000, T5HH1600, T5HH2200)
    x-axis is the cutflow
    '''
    h_cutflow = TH2F("cutflow","",9,0,9,3,0,3)
    binLabelsX = ["All events", "Hadronic baseline", "N_{AK8}>1, jet p_{T}>300 GeV","m_{J} [60,260] GeV"]
    binLabelsX.append("Resolved event veto");
    binLabelsX.append("D_{bb,J1&2}>0.7"); binLabelsX.append("m_{J1&2} [95,145] GeV");
    binLabelsX.append("D_{bb,J1or2}>0.7"); binLabelsX.append("m_{J1&2} [95,145] GeV");
    binLabelsY = ["T5HH(1000,1)", "T5HH(1600,1)", "T5HH(2200,1)"]
    for xbin in range(len(binLabelsX)): h_cutflow.GetXaxis().SetBinLabel(xbin+1, binLabelsX[xbin])
    for ybin in range(len(binLabelsY)): h_cutflow.GetYaxis().SetBinLabel(ybin+1, binLabelsY[ybin])

    file1 = "cutflow_allYears_T5HH1000.root"
    file2 = "cutflow_allYears_T5HH1600.root"
    file3 = "cutflow_allYears_T5HH2200.root"
    vecFiles = [file1,file2,file3]

    vecFiles_1H = []
    file1 = "cutflow_allYears_T5HH1000_1H.root"
    file2 = "cutflow_allYears_T5HH1600_1H.root"
    file3 = "cutflow_allYears_T5HH2200_1H.root"
    vecFiles_1H = [file1,file2,file3]

    whichFile = 0
    for fileName in vecFiles:
        # print("For file: "+fileName)
        file = TFile(fileDir+fileName, "r")
        listOfHistos = TIter(file.GetListOfKeys())
        histo = TH1F()
        i = 0
        eventCounts = []
        for key in listOfHistos:
            i=i+1
            # if (i==4 or i==5): continue #Only need last year of res veto
            histo = key.ReadObj()
            eventCounts.append(histo.Integral(0,21))
        fileName1H = vecFiles_1H[whichFile]
        file_1H = TFile(fileDir+fileName1H, "r")
        listOfHistos_1H = TIter(file_1H.GetListOfKeys())
        histo = TH1F()
        i = 0
        for key in listOfHistos_1H:
            i=i+1
            histo = key.ReadObj()
            if (i<6): continue #don't need to save the other ones, just the final two lines for 1H
            eventCounts.append(histo.Integral(0,21))
        for i in range(len(eventCounts)):
            h_cutflow.SetBinContent(i+1, whichFile+1, eventCounts[i])
        file.Close(); file_1H.Close(); whichFile = whichFile+1

    h_cutflow.SetStats(0)
    h_cutflow.SaveAs(outDir+"CMS-SUS-20-004_aux_Table_006.root")


def main():
    writeCutflow(False)
    writeCutflow(True)
    makeCutflowRootFile() #Only prints the root file for the cutflow in supplementary material
if __name__ == "__main__":
    main()
