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

//Don't use for 2D
int main(int argc, char** argv) {
  int region(0); region = atoi(argv[1]); // 0=1DTChiHH, 1=1DT5HH
  int NLSPmass(0); NLSPmass = atoi(argv[2]);
  std::string strNLSPmass = std::to_string(NLSPmass);
  int doDetailed(0); doDetailed = atoi(argv[3]);
  //if doDetailed, include all filters and everything before HadronicBaseline. Also only 4b events

  bool runVeto = false;
  bool runFullSIM = false;  //if (region==1) runFullSIM=true;
  bool runData = false;
  bool printToScreen = true; //Print cutflow in latex format to screen - do NOT use for auxilary material! Just quick checks


  TString strLSPmass = "1";
  setMasses(strNLSPmass,strLSPmass);

  // TString fileStart = "root://cmsxrootd.fnal.gov/";
  TString fileStart = "/eos/uscms";

  TChain *chain = new TChain("tree");
  if (region==0){
    chain->Add(fileStart+"/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/TChiHH_HToBB_HToBB_mMother-"+strNLSPmass+"_mLSP-1_MC2016_fast.root");
    chain->Add(fileStart+"/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/TChiHH_HToBB_HToBB_mMother-"+strNLSPmass+"_mLSP-1_MC2017_fast.root");
    chain->Add(fileStart+"/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/TChiHH_HToBB_HToBB_mMother-"+strNLSPmass+"_mLSP-1_MC2018_fast.root");
  }
  else if (region==1) {
    // chain->Add(fileStart+"/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-"+strNLSPmass+"_mLSP-1_MC2016.root");
    // chain->Add(fileStart+"/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-"+strNLSPmass+"_mLSP-1_MC2017.root");
    // chain->Add(fileStart+"/store/user/lpcsusyhad/SusyRA2Analysis2015/Run2ProductionV18/scan/T5qqqqZH-mGluino-1000to2500_mMother-"+strNLSPmass+"_mLSP-1_MC2018.root");

    chain->Add(fileStart+"/store/user/lpcsusyhad/EWhad_WW_WZ_WH/Run2ProductionV18/Signal_"+strNLSPmass+"_1_Summer16v3Fast_TChiHH_official.root");
    chain->Add(fileStart+"/store/user/lpcsusyhad/EWhad_WW_WZ_WH/Run2ProductionV18/Signal_"+strNLSPmass+"_1_Fall17Fast_TChiHH_official.root");
    chain->Add(fileStart+"/store/user/lpcsusyhad/EWhad_WW_WZ_WH/Run2ProductionV18/Signal_"+strNLSPmass+"_1_Autumn18Fast_TChiHH_official.root");
  }

  RA2bTree* ntuple = new RA2bTree(chain);
  TString thisSample = "";
  thisSample="TChiHH"+strNLSPmass+"OLD";
  // if (region==1) thisSample = "T5HH"+strNLSPmass;
  if (region==1) thisSample="TChiHH"+strNLSPmass+"NEW";

  TString detailString = ""; if (doDetailed==1) detailString = "_detailedMOREPLOTS";
  // TString outputFileName = "cutflow_allYears_"+thisSample+detailString+".root";
  TString outputFileName = "cutflow_1718_"+thisSample+detailString+".root";
  TFile* outputFile = new TFile(outputFileName,"RECREATE");

  typedef plot<RA2bTree> plot; vector< plot > plots; vector< vector<plot> > plotsVec;
  typedef bool(*cutFunc)(RA2bTree*);
  vector<cutFunc> cutFlow; vector<TString> cutName;

  cutFlow.push_back(*alwaysTrue<RA2bTree>); cutName.push_back("AllEventsNoWeights");
  cutFlow.push_back(*alwaysTrue<RA2bTree>);
  cutName.push_back("AllEvents");

  if (doDetailed==1) {
    cutFlow.push_back(*gen4bs<RA2bTree>); cutName.push_back("bbOnly");
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
  if (runVeto) {
    cutFlow.push_back(*resVetoCutflow<RA2bTree>);
    cutName.push_back("ResVeto");
  }

  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("TriggerWeight");
  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("ISRWeight");
  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("PUWeight");
  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("PrefireWeight");

  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("MassFastSIM");
  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("AllWeightsBaseline");

  cutFlow.push_back(*cutflowBoostBBTag<RA2bTree>);
  cutName.push_back("BB2H");
  // cutFlow.push_back(*cutflowBoostBBTag1H<RA2bTree>);
  // cutName.push_back("BB1H");
  cutFlow.push_back(*doubleMassCut<RA2bTree>);
  cutName.push_back("MassSR");
  cutFlow.push_back(*alwaysTrue<RA2bTree>);
  cutName.push_back("BBTagSF");
  cutFlow.push_back(*alwaysTrue<RA2bTree>);
  cutName.push_back("BBTagFastSIM");
  cutFlow.push_back(*alwaysTrue<RA2bTree>);
  cutName.push_back("BothBB");


  // cutFlow.push_back(*alwaysTrue<RA2bTree>);
  // cutName.push_back("AllWeights2HSR");




  for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
    if (!doDetailed) plots.push_back(plot(*fillMET<RA2bTree>,cutName[iCut],"MET [GeV]",20,300.,1300.));
    if (doDetailed) {
      vector< plot > tempPlots;
      tempPlots.push_back(plot(*fillMET<RA2bTree>,cutName[iCut]+"_MET","MET [GeV]",20,300.,1300.));
      tempPlots.push_back(plot(*fillLeadingJetMass<RA2bTree>,cutName[iCut]+"_J1M","Lead m_{J} [GeV]",80,0.,300.));
      tempPlots.push_back(plot(*fillSubLeadingJetMass<RA2bTree>,cutName[iCut]+"_J2M","Sublead m_{J} [GeV]",80,0.,300.));
      tempPlots.push_back(plot(*fillLeadingdeepBBtag<RA2bTree>,cutName[iCut]+"_J1BB","Lead jet deep bb-tag",50,0.0,1.0));
      tempPlots.push_back(plot(*fillSubLeadingdeepBBtag<RA2bTree>,cutName[iCut]+"_J2BB","Sublead jet deep bb-tag",50,0.0,1.0));
      tempPlots.push_back(plot(*fillLeadingJetPt<RA2bTree>,cutName[iCut]+"_J1Pt","Lead jet p_{T} [GeV]",50,0.,1300.));
      tempPlots.push_back(plot(*fillSubLeadingJetPt<RA2bTree>,cutName[iCut]+"_J2Pt","Sublead jet p_{T} [GeV]",50,0.,1300.));
      plotsVec.push_back(tempPlots);
    }
  }

  if (runVeto) { // region 0=1DTChiHH, 1=1DT5HH
    if (region==0){readResVeto_Sig1D("all","TChiHH");}
    else if (region==1){readResVeto_Sig1D("all","T5HH");}
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
  if ( startFilename.Contains("2017")  || startFilename.Contains("Fall17")) this_lumi = 41529.0;
  else if ( startFilename.Contains("2018") || startFilename.Contains("Autumn18")) this_lumi = 59740.0;
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

    if ( thisFileName.Contains("2016") || thisFileName.Contains("Summer16v3")) {this_lumi = 35922.0; Year = "MC2016";}
    else if ( thisFileName.Contains("2017") || thisFileName.Contains("Fall17")) {this_lumi = 41529.0; Year = "MC2017";}
    else if ( thisFileName.Contains("2018") || thisFileName.Contains("Autumn18")) {this_lumi = 59740.0; Year = "MC2018";}

    if (region==1 && getNumGenHiggses(ntuple)!=2) continue;
    double thisMET = fillMET(ntuple); double thisHT = fillHT(ntuple);

    if (startFilename!=thisFileName) {
      std::cout<<"Open file: "<< thisFileName<<std::endl;
      fin = TFile::Open(thisFileName);
      TTree *treeNew = (TTree*)fin->Get("tree");
      TotalEvents = treeNew->GetEntries();
      startFilename=thisFileName;
    }
    float reweightXSEC = reweightSignalXSEC(strNLSPmass);

    trigWeight = trigEff_0l(Year,thisMET,thisHT,"none");
    if (region==1) isrweight = SignalISRCorrection(ntuple,"none");
    else if (region==0) isrweight = SignalEWKISRCorrection(ntuple,"none");
    puweight = ntuple->puWeight;
    prefireWeight = prefireCorr(ntuple,Year,"none");
    weight = ntuple->Weight*this_lumi*trigWeight*isrweight*puweight*prefireWeight;
    // weight = ntuple->Weight*this_lumi;

    if (region==0) weight = weight/TotalEvents*(0.5823329*0.5823329);
    // else if (region==1) weight = weight/TotalEvents*4.0;
    else if (region==1) weight = weight*reweightXSEC;

    sdMass_FastSIM = softdropmassFastSIMSFs(ntuple,"none");
    bb_tag = bbSFs(ntuple,"none");
    bbtag_FastSIM = bbFastSIMSFs(ntuple,"none");
    // weight = weight*bb_tag*bbtag_FastSIM*sdMass_FastSIM;
    weight = weight*sdMass_FastSIM;

    if (!doDetailed) {
      for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
        if (!cutFlow[iCut](ntuple)) break; plots[iCut].fill(ntuple,weight);
      }
    }
    else {
      for (int iCut = 0; iCut < cutFlow.size(); iCut++) {
        if (! cutFlow[iCut](ntuple)) break;
        for (int iPlot = 0; iPlot < plotsVec[iCut].size(); iPlot++) {
          if (cutName[iCut]=="AllEventsNoWeights") plotsVec[iCut][iPlot].fill(ntuple,1.0);
          // else if (cutName[iCut]=="TriggerWeight") plotsVec[iCut][iPlot].fill(ntuple,weight*trigWeight);
          // else if (cutName[iCut]=="ISRWeight") plotsVec[iCut][iPlot].fill(ntuple,weight*isrweight);
          // else if (cutName[iCut]=="PUWeight") plotsVec[iCut][iPlot].fill(ntuple,weight*puweight);
          // else if (cutName[iCut]=="PrefireWeight") plotsVec[iCut][iPlot].fill(ntuple,weight*prefireWeight);
          else if (cutName[iCut]=="BBTagSF") plotsVec[iCut][iPlot].fill(ntuple,weight*bb_tag);
          else if (cutName[iCut]=="BBTagFastSIM") plotsVec[iCut][iPlot].fill(ntuple,weight*bbtag_FastSIM);
          else if (cutName[iCut]=="BothBB") plotsVec[iCut][iPlot].fill(ntuple,weight*bb_tag*bbtag_FastSIM);
          // else if (cutName[iCut]=="MassFastSIM") plotsVec[iCut][iPlot].fill(ntuple,weight*sdMass_FastSIM);
          // else if (cutName[iCut]=="AllWeightsBaseline" || cutName[iCut]=="AllWeights2HSR") plotsVec[iCut][iPlot].fill(ntuple,weight*trigWeight*isrweight*puweight*prefireWeight*bb_tag*bbtag_FastSIM*sdMass_FastSIM);
          else plotsVec[iCut][iPlot].fill(ntuple,weight);
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
  else if (printToScreen && doDetailed) {
    std::cout<<"\n---------------------------------------------------------------\nCutflow for LaTeX:\n"<<std::endl;
    std::cout<<"\\documentclass{standalone}\n\\begin{document}\n\\begin{tabular}{l|c}\n";
    std::cout << "Selection requirement & "<<thisSample << " \\\\ \\hline" << std::endl;
    std::cout << "% ------------------------------------------------------------------" << std::endl;
    for (int iCut = 0; iCut < cutFlow.size(); iCut++){
      std::cout << cutName[iCut];
      for (int iPlot = 0; iPlot < plotsVec[iCut].size(); iPlot++) {
        std::cout << " & " << std::setprecision(10) << plotsVec[iCut][iPlot].histoMap[ntuple]->Integral(0,21);
        std::cout << " \\\\" << std::endl;
        break;
      }
    }
    std::cout<<"\\end{tabular}\n\\end{document}\n";

  }
  std::cout<<"\nCutflow saved in "<<outputFileName<<std::endl;
  outputFile->Close(); fin->Close();
}
