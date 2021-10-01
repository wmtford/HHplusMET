#include "TROOT.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TChain.h"
#include "TBranch.h"
#include "TLeaf.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TProfile2D.h"
#include "TMath.h"
#include "TBenchmark.h"
#include <TError.h>
#include "TLorentzVector.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <algorithm>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string>
#include <vector>

#include "../src/definitions.cc"
#include "../src/RA2bTree.cc"
#include "../src/plotterUtils.cc"

using namespace std;

int main(int argc, char** argv) {
  int region(0); region = atoi(argv[1]); // 0=1DTChiHH, 1=1DT5HH
  int NLSPmass(0); NLSPmass = atoi(argv[2]);
  std::string strNLSPmass = std::to_string(NLSPmass);
  int doDetailed(0); doDetailed = atoi(argv[3]);
  //if doDetailed, include all filters and everything before HadronicBaseline. Also only 4b events
  //doDetailed should only be run over TChiHH950 and T5HH1000
  //if not doDetailed, should only be run over T5HH 1000,1600,2200

  TString strLSPmass = "1";
  if (doDetailed==1) {
    if (region==0) { strNLSPmass = "950"; strLSPmass = "0"; }
    else if (region==1) { strNLSPmass = "1000"; }
  }
  setMasses(strNLSPmass,strLSPmass);

  TChain *chain = new TChain("tree");
  if (doDetailed) {
    if (region==0){
      chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/TChiHH_HToBB_HToBB_mMother-950_mLSP-1_MC2016_fast.root");
      chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/TChiHH_HToBB_HToBB_mMother-950_mLSP-1_MC2017_fast.root");
      chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/TChiHH_HToBB_HToBB_mMother-950_mLSP-1_MC2018_fast.root");
    }
    else if (region==1){
    //   chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-1000_mLSP-1_MC2016.root");
    //   chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-1000_mLSP-1_MC2017.root");
    //   chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-1000_mLSP-1_MC2018.root");
    chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-2000_mLSP-1_MC2016.root");
    chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-2000_mLSP-1_MC2017.root");
    chain->Add("root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-2000_mLSP-1_MC2018.root");

    }
  }
  else {
    //From skims
    // TString filename = "root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/scan/tree_signal_METVars_FullSIM/tree_T5qqqqZH-mGluino-1000to2500_"+strNLSPmass+"_1_MC2016.root";
    // chain->Add(filename);
    // filename = "root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/scan/tree_signal_METVars_FullSIM/tree_T5qqqqZH-mGluino-1000to2500_"+strNLSPmass+"_1_MC2017.root";
    // chain->Add(filename);
    // filename = "root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/scan/tree_signal_METVars_FullSIM/tree_T5qqqqZH-mGluino-1000to2500_"+strNLSPmass+"_1_MC2018.root";
    // chain->Add(filename);


    //From ntuples
    TString filename = "root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-"+strNLSPmass+"_mLSP-1_MC2016.root";
    chain->Add(filename);
    filename = "root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-"+strNLSPmass+"_mLSP-1_MC2017.root";
    chain->Add(filename);
    filename = "root://cmsxrootd.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-"+strNLSPmass+"_mLSP-1_MC2018.root";
    chain->Add(filename);
  }

  RA2bTree* ntuple = new RA2bTree(chain);

  bool runVeto = true;
  bool runFullSIM = false;  if (region==1) runFullSIM=true;
  bool runData = false;
  bool printToScreen = true; //Print cutflow in latex format to screen - do NOT use for auxilary material! Just quick checks

  TString thisSample = "";
  if (doDetailed==1) { //strNLSPmass argument doesn't matter, base off regions
    thisSample="TChiHH950";
    // if (region==1) thisSample = "T5HH1000";
    if (region==1) thisSample = "T5HH2200";
  }
  else thisSample="T5HH"+strNLSPmass;

  TString detailString = ""; if (doDetailed==1) detailString = "_detailedMOREPLOTS";
  TString outputFileName = "cutflow_allYears_"+thisSample+detailString+".root";
  TFile* outputFile = new TFile(outputFileName,"RECREATE");

  typedef plot<RA2bTree> plot; vector< plot > plots; vector< vector<plot> > plotsVec;
  typedef bool(*cutFunc)(RA2bTree*);
  vector<cutFunc> cutFlow; vector<TString> cutName;

  cutFlow.push_back(*alwaysTrue<RA2bTree>);
  cutName.push_back("AllEvents");

  if (doDetailed==1) {
    // cutFlow.push_back(*gen4bs<RA2bTree>); cutName.push_back("bbOnly");
    cutFlow.push_back(*cutflowNJets<RA2bTree>);
    cutName.push_back("NJET");
    cutFlow.push_back(*cutflowlooseHT<RA2bTree>);
    cutName.push_back("LooseHT");
    cutFlow.push_back(*cutflowlooseMET<RA2bTree>);
    cutName.push_back("LooseMET");
    cutFlow.push_back(*cutflowMuonVeto<RA2bTree>);
    cutName.push_back("MuonVeto");
    cutFlow.push_back(*cutflowElectronVeto<RA2bTree>);
    cutName.push_back("ElectronVeto");
    cutFlow.push_back(*cutflowIsoMuonTrackVeto<RA2bTree>);
    cutName.push_back("IsoMuonTrackVeto");
    cutFlow.push_back(*cutflowIsoElectronTrackVeto<RA2bTree>);
    cutName.push_back("IsoElectronTrackVeto");
    cutFlow.push_back(*cutflowIsoPionTrackVeto<RA2bTree>);
    cutName.push_back("IsoPionTrackVeto");

    // cutFlow.push_back(*FakeJetFilter<RA2bTree>);
    // cutName.push_back("FakeJetFilter");
    // cutFlow.push_back(*GoodVertexCutflow<RA2bTree>);
    // cutName.push_back("GoodVertex");
    // cutFlow.push_back(*METRatioFilterCut<RA2bTree>);
    // cutName.push_back("METRatioFilter");
    // cutFlow.push_back(*METMHTFilterCut<RA2bTree>);
    // cutName.push_back("METMHTFilter");
    // cutFlow.push_back(*HEMDPhiVetoFilterCutflow<RA2bTree>);
    // cutName.push_back("HEMDPhiVetoFilter");
    // cutFlow.push_back(*globalSuperTightHalo2016FilterCutflow<RA2bTree>);
    // cutName.push_back("globalSuperTightHalo2016Filter");
    // cutFlow.push_back(*HBHENoiseFilterCutflow<RA2bTree>);
    // cutName.push_back("HBHENoiseFilter");
    // cutFlow.push_back(*HBHEIsoNoiseFilterCutflow<RA2bTree>);
    // cutName.push_back("HBHEIsoNoiseFilter");
    // cutFlow.push_back(*EcalDeadCellTriggerPrimitiveFilterCutflow<RA2bTree>);
    // cutName.push_back("EcalDeadCellTriggerPrimitiveFilter");
    // cutFlow.push_back(*BadPFMuonFilterCutflow<RA2bTree>);
    // cutName.push_back("BadPFMuonFilter");
    // cutFlow.push_back(*LowNeutralJetFilter<RA2bTree>);
    // cutName.push_back("LowNeutralJetFilter");
    // cutFlow.push_back(*MuonJetFilter<RA2bTree>);
    // cutName.push_back("MuonJetFilter");
    // cutFlow.push_back(*HTRatioDPhiTightFilter<RA2bTree>);
    // cutName.push_back("HTRatioDPhiTightFilter");
    // cutFlow.push_back(*EcalNoiseJetFilter<RA2bTree>);
    // cutName.push_back("EcalNoiseJetFilter");


    cutFlow.push_back(*FiltersCutflow<RA2bTree>);
    cutName.push_back("Filters");
    cutFlow.push_back(*DeltaPhiCuts<RA2bTree>);
    cutName.push_back("DeltaPhiCuts");
    cutFlow.push_back(*METTightCutflow<RA2bTree>);
    cutName.push_back("MET300");
  }
  else {
    cutFlow.push_back(*HadronicBaseline<RA2bTree>);
    cutName.push_back("HadronicBaseline");
  }
  cutFlow.push_back(*AK8JetPtCut<RA2bTree>);
  cutName.push_back("nAK8"); //Lead and sublead jet, pass pT>300 and jetID each, and HT>600
  cutFlow.push_back(*looseMassCut<RA2bTree>);
  cutName.push_back("LooseJetMass");
  cutFlow.push_back(*ResVetoCutflow<RA2bTree>);
  cutName.push_back("ResVeto");
  cutFlow.push_back(*cutflowBoostBBTag<RA2bTree>);
  cutName.push_back("BB2H");
  //
  // cutFlow.push_back(*cutflowBoostBBTag1H<RA2bTree>);
  // cutName.push_back("BB1H");
  //
  cutFlow.push_back(*doubleMassCut<RA2bTree>);
  cutName.push_back("MassSR");


  for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
    if (!doDetailed) plots.push_back(plot(*fillMET<RA2bTree>,cutName[iCut],"MET [GeV]",20,300.,1300.));
    if (doDetailed) {
      vector< plot > tempPlots;
      tempPlots.push_back(plot(*fillMET<RA2bTree>,cutName[iCut]+"_MET","MET [GeV]",20,300.,1300.));
      tempPlots.push_back(plot(*fillLeadingJetMass<RA2bTree>,cutName[iCut]+"_J1M","Lead m_{J} [GeV]",80,0.,300.));
      tempPlots.push_back(plot(*fillSubLeadingJetMass<RA2bTree>,cutName[iCut]+"_J2M","Sublead m_{J} [GeV]",80,0.,300.));
      tempPlots.push_back(plot(*fillLeadingdeepBBtag<RA2bTree>,cutName[iCut]+"_J1BB","Lead jet deep bb-tag",50,0.0,1.0));
      tempPlots.push_back(plot(*fillSubLeadingdeepBBtag<RA2bTree>,cutName[iCut]+"_J2BB","Sublead jet deep bb-tag",50,0.0,1.0));
      plotsVec.push_back(tempPlots);
    }
  }

  if (runVeto) { // region 0=1DTChiHH, 1=1DT5HH
    if (region==0){readResVeto_Sig1D("TChiHH");}
    else if (region==1){readResVeto_Sig1D("T5HH");}
  }

  if (!doDetailed) {
    for (int iCut = 0; iCut < plots.size(); iCut++) {
      plots[iCut].addNtuple(ntuple,thisSample,false); plots[iCut].dataHist=0;
    }
  }
  else {
    for (int iCut = 0; iCut < plotsVec.size(); iCut++) {
      for (int iPlot = 0; iPlot < plotsVec[iCut].size(); iPlot++) {
        plotsVec[iCut][iPlot].addNtuple(ntuple,thisSample,false);
        plotsVec[iCut][iPlot].dataHist=0;
      }
    }
  }


  int numEvents = ntuple->fChain->GetEntries();
  ntupleBranchStatus<RA2bTree>(ntuple);
  TString Year;

  TString startFilename = ntuple->fChain->GetFile()->GetName();
  double this_lumi = 35922.0;
  if ( startFilename.Contains("2017") ) this_lumi = 41529.0;
  else if ( startFilename.Contains("2018") ) this_lumi = 59740.0;
  TFile *fin = TFile::Open(startFilename);
  TTree *tree = (TTree*)fin->Get("tree");
  int TotalEvents = tree->GetEntries();


  for (int iEvt = 0; iEvt < numEvents; iEvt++) {
    // if (iEvt>5000) break;
    ntuple->GetEntry(iEvt);
    TString thisFileName = ntuple->fChain->GetFile()->GetName();
    double weight = 1.0; double isrweight = 1.0; float trigWeight = 1.0;
    double puweight = 1.0; double prefireWeight = 1.0; float scalesSyst = 1.0;
    float bb_tag = 1.0; float bbtag_FastSIM = 1.0; float sdMass_FastSIM = 1.0;

    setMET(ntuple); setHT(ntuple,"none"); setJetPT(ntuple); setJetMass(ntuple);
    setIsoTracks(ntuple); setDeltaPhis(ntuple);

    if ( thisFileName.Contains("2016") ) {this_lumi = 35922.0; Year = "MC2016";}
    else if ( thisFileName.Contains("2017") ) {this_lumi = 41529.0; Year = "MC2017";}
    else if ( thisFileName.Contains("2018") ) {this_lumi = 59740.0; Year = "MC2018";}

    if (region==1 && getNumGenHiggses(ntuple)!=2) continue;
    double thisMET = fillMET(ntuple); double thisHT = fillHT(ntuple);

    if (startFilename!=thisFileName) {
      std::cout<<"Open file: "<< thisFileName<<std::endl;
      fin = TFile::Open(thisFileName);
      TTree *treeNew = (TTree*)fin->Get("tree");
      TotalEvents = treeNew->GetEntries();
      startFilename=thisFileName;
    }

    trigWeight = trigEff_0l(Year,thisMET,thisHT,"none");
    if (region==1) isrweight = SignalISRCorrection(ntuple,"none");
    else if (region==0) isrweight = SignalEWKISRCorrection(ntuple,"none");
    puweight = ntuple->puWeight;
    prefireWeight = prefireCorr(ntuple,Year,"none");
    weight = ntuple->Weight*this_lumi*trigWeight*isrweight*puweight*prefireWeight;

    if (region==0) weight = weight/TotalEvents*(0.5823329*0.5823329);
    else if (region==1) weight = weight/TotalEvents*4.0;

    sdMass_FastSIM = softdropmassFastSIMSFs(ntuple,"none");
    bb_tag = bbSFs(ntuple,"none");
    bbtag_FastSIM = bbFastSIMSFs(ntuple,"none");
    weight = weight*bb_tag*bbtag_FastSIM*sdMass_FastSIM;

    if (!doDetailed) {
      for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
        if (!cutFlow[iCut](ntuple)) break; plots[iCut].fill(ntuple,weight);
      }
    }
    else {
      for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
        if (! cutFlow[iCut](ntuple)) break;
        for (int iPlot = 0; iPlot < plotsVec[iCut].size(); iPlot++) {
          plotsVec[iCut][iPlot].fill(ntuple,weight);
        }
      }
    }

    clearGlobalConstants();
  } //loop over events

  outputFile->cd();
  if (!doDetailed) for (int iCut = 0; iCut < cutFlow.size(); iCut++) plots[iCut].Write(runData);
  else {
    for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
      for (int iPlot = 0; iPlot < plotsVec[iCut].size(); iPlot++) {
        plotsVec[iCut][iPlot].Write(runData);
      }
    }
  }
  if (printToScreen && !doDetailed) {
    std::cout<<"\n---------------------------------------------------------------\nCutflow for LaTeX:\n"<<std::endl;
    std::cout<<"\\documentclass{standalone}\n\\begin{document}\n\\begin{tabular}{l|c}\n";
    std::cout << "Selection requirement & "<<thisSample << " \\\\ \\hline" << std::endl;
    std::cout << "% ------------------------------------------------------------------" << std::endl;
    for (int iCut = 0; iCut < cutFlow.size(); iCut++){
      std::cout << cutName[iCut];
      std::cout << " & " << std::setprecision(10) << plots[iCut].histoMap[ntuple]->Integral(0,21);
      std::cout << " \\\\" << std::endl;
    }
    std::cout<<"\\end{tabular}\n\\end{document}\n";
  }
  std::cout<<"\nCutflow saved in "<<outputFileName<<std::endl;
  outputFile->Close(); fin->Close();
}
