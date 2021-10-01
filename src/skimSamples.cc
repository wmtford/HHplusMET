#include <TChain.h>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TDirectory.h>
#include <TLorentzVector.h>

#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <limits>
#include <getopt.h>
#include <iostream>
#include <vector>

#include "RA2bTree.cc"

std::vector<std::string> mySplit(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) { tokens.push_back(token);}
  return tokens;
}

static const TString BASE_DIR="root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/";
static const TString keithsDir="root://cmseos.fnal.gov//eos/uscms/store/user/kaulmer/Skims/Run2ProductionV18/scan/tree_signal_METVars/"; //this is for TChiHH
static const TString BASE_DIRUnblind="root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/tree_signalUnblind_METVars/";
static const TString V18Signal_DIR="root://cmseos.fnal.gov//store/user/emacdona/Skims/Run2ProductionV18/scan/tree_signal_METVars_FullSIM/";

class skimSamples {
  public :
  TChain *WJets,*ZJets,*QCD,*SnglT,*TT,*TT_Di,*TT_SingleLept,*GJets,*GJets0p4,*Other,*DY,*TTinc;
  TChain *T5HH, *TChiHH, *TChiHH1D;
  TChain *data2016; TChain *data2017; TChain *data2018;

  std::vector<RA2bTree*> ntuples;
  RA2bTree* dataNtuple;
  std::vector<TString> sampleName, dataSampleName;

  enum region {kSignal,k1DSignal,k2DSignal,kSLm,kSLe,kPhoton, k1DTChiHHAll, k1DT5HHAll, k2DTChiHHAll, kNumRegions};
  TString regionNames[kNumRegions]={"signal", "1DSignal", "2DSignal", "SLm", "SLe", "photon", "1DTChiHHAll", "1DT5HHAll", "2DTChiHHAll"};
  TString skimType;

  skimSamples(region r=kSignal, TString Year="MC2016", int mass2D=0) {
    skimType="";
    if (r == kSignal) skimType=BASE_DIR+"tree_signal_METVars";
    if (r == kSLm) skimType=BASE_DIR+"tree_SLm_METVars";
    if (r == kSLe) skimType=BASE_DIR+"tree_SLe_METVars";
    if (r == kPhoton) skimType=BASE_DIR+"tree_GJet_CleanMETVars";

    //bools that determine which processes are run, as a subset of signal
    bool runData = true;
    bool run_singleT = false;
    bool run_TT = false;
    bool run_QCD = false;
    bool run_GJets = true;
    bool run_WJets = false;
    bool run_ZJets = false;
    bool run_TChiHH1D = false;
    bool run_TChiHH2D = false;
    bool run_T5HH1D = false;
    bool run_T5HH2D = false;
    bool run_TChiHH1DAll = false;
    bool run_TChiHH2DAll = false;
    bool run_T5HH1DAll = false;

    if (r == k1DSignal) {
      run_singleT = false;
      run_TT = false;
      run_QCD = false;
      run_WJets = false;
      run_ZJets = false;
      run_TChiHH2D = false;
      run_T5HH2D = false;
      run_T5HH1D = false; //for fullsim
      run_TChiHH1D = true;

      run_TChiHH1DAll = false;
      run_TChiHH2DAll = false;
      run_T5HH1DAll = false;
    }
    else if (r == k2DSignal) {
      run_TChiHH1DAll = false;
      run_TChiHH2DAll = false;
      run_T5HH1DAll = false;

      run_TChiHH1D = false;
      run_T5HH1D = false;
      // run_TChiHH2D = false;
      // run_T5HH2D = true;
      if (mass2D>810){
        run_TChiHH2D = false;
        run_T5HH2D = true;
      }
      else {
        run_TChiHH2D = true;
        run_T5HH2D = false;
      }
    }

    else if (r == k1DTChiHHAll) {
      run_singleT = false;
      run_TT = false;
      run_QCD = false;
      run_WJets = false;
      run_ZJets = false;
      run_TChiHH1D = false;
      run_TChiHH2D = false;
      run_T5HH1D = false;
      run_T5HH2D = false;
      run_TChiHH1DAll = true;
      run_TChiHH2DAll = false;
      run_T5HH1DAll = false;
    }

    else if (r == k1DT5HHAll) {
      run_singleT = false;
      run_TT = false;
      run_QCD = false;
      run_WJets = false;
      run_ZJets = false;
      run_TChiHH1D = false;
      run_TChiHH2D = false;
      run_T5HH1D = false;
      run_T5HH2D = false;
      run_TChiHH1DAll = false;
      run_TChiHH2DAll = false;
      run_T5HH1DAll = true;
    }

    else if (r == k2DTChiHHAll) {
      run_singleT = false;
      run_TT = false;
      run_QCD = false;
      run_WJets = false;
      run_ZJets = false;
      run_TChiHH1D = false;
      run_TChiHH2D = false;
      run_T5HH1D = false;
      run_T5HH2D = false;
      run_TChiHH1DAll = false;
      run_TChiHH2DAll = true;
      run_T5HH1DAll = false;
    }

    ///////////////////////////////////////////////////////////////////////
    // - - - - - - - - - - BACKGROUND INPUTS - - - - - - - - - - - - - - //
    ///////////////////////////////////////////////////////////////////////
    if (run_ZJets && r == kSignal) {
      std::vector<TString> ZJetsFileNames;
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-100to200_"+Year+".root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-200to400_"+Year+".root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-400to600_"+Year+".root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-600to800_"+Year+".root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-800to1200_"+Year+".root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-1200to2500_"+Year+".root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-2500toInf_"+Year+".root");
      ZJets = new TChain("tree");
      for (unsigned int i = 0 ; i < ZJetsFileNames.size() ; i++) {
        ZJets->Add(skimType+"/"+ZJetsFileNames[i]);
      }
      ntuples.push_back(new RA2bTree(ZJets));
      sampleName.push_back("ZJets");
    }

    if (run_WJets && (r == kSignal || r == kSLm || r == kSLe) ) {
      std::vector<TString> WJetsFileNames;
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-100to200_"+Year+".root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-200to400_"+Year+".root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-400to600_"+Year+".root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-600to800_"+Year+".root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-800to1200_"+Year+".root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-1200to2500_"+Year+".root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-2500toInf_"+Year+".root");

      WJets = new TChain("tree");
      for (unsigned int i = 0 ; i < WJetsFileNames.size() ; i++) {
        WJets->Add(skimType+"/"+WJetsFileNames[i]);
      }
      ntuples.push_back(new RA2bTree(WJets));
      sampleName.push_back("WJets");
    }

    if (run_TT && (r == kSignal || r == kSLm || r == kSLe) ) {
      std::vector<TString> TTFileNames;
      TTFileNames.push_back("tree_TTJets_DiLept_"+Year+".root");
      TTFileNames.push_back("tree_TTJets_SingleLeptFromT_"+Year+".root");
      TTFileNames.push_back("tree_TTJets_SingleLeptFromTbar_"+Year+".root");

      if (Year.Contains("MC2018")) {
        TTFileNames.push_back("tree_TTJets_DiLept_genMET-80_"+Year+".root");
        TTFileNames.push_back("tree_TTJets_SingleLeptFromT_genMET-80_"+Year+".root");
        TTFileNames.push_back("tree_TTJets_SingleLeptFromTbar_genMET-80_"+Year+".root");
      }
      else {
        TTFileNames.push_back("tree_TTJets_DiLept_genMET-150_"+Year+".root");
        TTFileNames.push_back("tree_TTJets_SingleLeptFromT_genMET-150_"+Year+".root");
        TTFileNames.push_back("tree_TTJets_SingleLeptFromTbar_genMET-150_"+Year+".root");
      }

      TT = new TChain("tree");
      for (unsigned int i = 0 ; i < TTFileNames.size() ; i++) {
        TT->Add(skimType+"/"+TTFileNames[i]);
      }
      ntuples.push_back(new RA2bTree(TT));
      sampleName.push_back("TT");
    }

    if (run_QCD && (r == kSignal || r == kPhoton) ) {
      std::vector<TString> QCDFileNames;
      QCDFileNames.push_back("tree_QCD_HT-200to300_"+Year+".root");
      QCDFileNames.push_back("tree_QCD_HT-300to500_"+Year+".root");
      QCDFileNames.push_back("tree_QCD_HT-500to700_"+Year+".root");
      QCDFileNames.push_back("tree_QCD_HT-700to1000_"+Year+".root");
      QCDFileNames.push_back("tree_QCD_HT-1000to1500_"+Year+".root");
      QCDFileNames.push_back("tree_QCD_HT-1500to2000_"+Year+".root");
      QCDFileNames.push_back("tree_QCD_HT-2000toInf_"+Year+".root");
      QCD = new TChain("tree");
      for (unsigned int i = 0 ; i < QCDFileNames.size() ; i++) {
        QCD->Add(skimType+"/"+QCDFileNames[i]);
      }
      ntuples.push_back(new RA2bTree(QCD));
      sampleName.push_back("QCD");
    }

    if (run_singleT && (r == kSignal || r == kSLm || r == kSLe) ) {
      std::vector<TString> SnglTFileNames;
      SnglTFileNames.push_back("tree_ST_s-channel_"+Year+".root");
      SnglTFileNames.push_back("tree_ST_t-channel_antitop_"+Year+".root");
      SnglTFileNames.push_back("tree_ST_t-channel_top_"+Year+".root");
      SnglTFileNames.push_back("tree_ST_tW_antitop_"+Year+".root");
      SnglTFileNames.push_back("tree_ST_tW_top_"+Year+".root");
      SnglT = new TChain("tree");
      for (unsigned int i = 0 ; i < SnglTFileNames.size() ; i++) {
        SnglT->Add(skimType+"/"+SnglTFileNames[i]);
      }
      ntuples.push_back(new RA2bTree(SnglT));
      sampleName.push_back("SnglT");
    }

    if (run_GJets && r == kPhoton) {
      std::vector<TString> GJets0p4FileNames;
      GJets0p4FileNames.push_back("tree_GJets_DR-0p4_HT-100to200_"+Year+".root");
      GJets0p4FileNames.push_back("tree_GJets_DR-0p4_HT-200to400_"+Year+".root");
      GJets0p4FileNames.push_back("tree_GJets_DR-0p4_HT-400to600_"+Year+".root");
      GJets0p4FileNames.push_back("tree_GJets_DR-0p4_HT-600toInf_"+Year+".root");
      GJets0p4 = new TChain("tree");
      for (unsigned int i = 0 ; i < GJets0p4FileNames.size() ; i++) {
        GJets0p4->Add(skimType+"/"+GJets0p4FileNames[i]);
      }
      ntuples.push_back(new RA2bTree(GJets0p4));
      sampleName.push_back("GJets");
    }


    ////////////////////////////////////////////////////////////
    // - - - - - - - - - - - DATA INPUTS - - - - - - - - - -  //
    ////////////////////////////////////////////////////////////

    std::vector<TString> METFileNames;

    if (Year.Contains("2016") && runData && r == kSignal) {
      METFileNames.push_back("tree_MET_2016B.root");
      METFileNames.push_back("tree_MET_2016C.root");
      METFileNames.push_back("tree_MET_2016D.root");
      METFileNames.push_back("tree_MET_2016E.root");
      METFileNames.push_back("tree_MET_2016F.root");
      METFileNames.push_back("tree_MET_2016G.root");
      METFileNames.push_back("tree_MET_2016H.root");
      data2016 = new TChain("tree");
      for (unsigned int i = 0 ; i < METFileNames.size() ; i++) {
        data2016->Add(BASE_DIRUnblind+METFileNames[i]);
      }
      dataNtuple = new RA2bTree(data2016);
    }

    if (Year.Contains("2017") && runData && r == kSignal) {
      METFileNames.push_back("tree_MET_2017B.root");
      METFileNames.push_back("tree_MET_2017C.root");
      METFileNames.push_back("tree_MET_2017D.root");
      METFileNames.push_back("tree_MET_2017E.root");
      METFileNames.push_back("tree_MET_2017F.root");
      data2017 = new TChain("tree");
      for (unsigned int i = 0 ; i < METFileNames.size() ; i++) {
        data2017->Add(BASE_DIRUnblind+METFileNames[i]);
      }
      dataNtuple = new RA2bTree(data2017);
    }

    if (Year.Contains("2018") && runData && r == kSignal) {
      METFileNames.push_back("tree_MET_2018A.root");
      METFileNames.push_back("tree_MET_2018B.root");
      METFileNames.push_back("tree_MET_2018C.root");
      METFileNames.push_back("tree_MET_2018D.root");
      data2018 = new TChain("tree");
      for (unsigned int i = 0 ; i < METFileNames.size() ; i++) {
        data2018->Add(BASE_DIRUnblind+METFileNames[i]);
      }
      dataNtuple = new RA2bTree(data2018);
    }

    std::vector<TString> SingleElectronNames;
    std::vector<TString> SingleMuonNames;
    std::vector<TString> SinglePhotonFileNames;

    if (Year.Contains("2016") && runData && r == kSLe) {
      SingleElectronNames.push_back("tree_SingleElectron_2016B.root");
      SingleElectronNames.push_back("tree_SingleElectron_2016C.root");
      SingleElectronNames.push_back("tree_SingleElectron_2016D.root");
      SingleElectronNames.push_back("tree_SingleElectron_2016E.root");
      SingleElectronNames.push_back("tree_SingleElectron_2016F.root");
      SingleElectronNames.push_back("tree_SingleElectron_2016G.root");
      SingleElectronNames.push_back("tree_SingleElectron_2016H.root");
      data2016 = new TChain("tree");
      for (unsigned int i = 0 ; i < SingleElectronNames.size() ; i++) {
        data2016->Add(skimType+"/"+SingleElectronNames[i]);
      }
      dataNtuple = new RA2bTree(data2016);
    }

    if (Year.Contains("2016") && runData && r == kSLm) {
      SingleMuonNames.push_back("tree_SingleMuon_2016B.root");
      SingleMuonNames.push_back("tree_SingleMuon_2016C.root");
      SingleMuonNames.push_back("tree_SingleMuon_2016D.root");
      SingleMuonNames.push_back("tree_SingleMuon_2016E.root");
      SingleMuonNames.push_back("tree_SingleMuon_2016F.root");
      SingleMuonNames.push_back("tree_SingleMuon_2016G.root");
      SingleMuonNames.push_back("tree_SingleMuon_2016H.root");
      data2016 = new TChain("tree");
      for (unsigned int i = 0 ; i < SingleMuonNames.size() ; i++) {
        data2016->Add(skimType+"/"+SingleMuonNames[i]);
      }
      dataNtuple = new RA2bTree(data2016);
    }

    if (Year.Contains("2016") && runData && r == kPhoton) {
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016B.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016C.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016D.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016E.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016F.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016G.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2016H.root");
      data2016 = new TChain("tree");
      for (unsigned int i = 0 ; i < SinglePhotonFileNames.size() ; i++) {
        data2016->Add(skimType+"/"+SinglePhotonFileNames[i]);
      }
      dataNtuple = new RA2bTree(data2016);
    }

    if (Year.Contains("2017") && runData && r == kSLe) {
      SingleElectronNames.push_back("tree_SingleElectron_2017B.root");
      SingleElectronNames.push_back("tree_SingleElectron_2017C.root");
      SingleElectronNames.push_back("tree_SingleElectron_2017D.root");
      SingleElectronNames.push_back("tree_SingleElectron_2017E.root");
      SingleElectronNames.push_back("tree_SingleElectron_2017F.root");
      data2017 = new TChain("tree");
      for (unsigned int i = 0 ; i < SingleElectronNames.size() ; i++) {
        data2017->Add(skimType+"/"+SingleElectronNames[i]);
      }
      dataNtuple = new RA2bTree(data2017);
    }

    if (Year.Contains("2017") && runData && r == kSLm) {
      SingleMuonNames.push_back("tree_SingleMuon_2017B.root");
      SingleMuonNames.push_back("tree_SingleMuon_2017C.root");
      SingleMuonNames.push_back("tree_SingleMuon_2017D.root");
      SingleMuonNames.push_back("tree_SingleMuon_2017E.root");
      SingleMuonNames.push_back("tree_SingleMuon_2017F.root");
      data2017 = new TChain("tree");
      for (unsigned int i = 0 ; i < SingleMuonNames.size() ; i++) {
        data2017->Add(skimType+"/"+SingleMuonNames[i]);
      }
      dataNtuple = new RA2bTree(data2017);
    }

    if (Year.Contains("2017") && runData && r == kPhoton) {
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2017B.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2017C.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2017D.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2017E.root");
      SinglePhotonFileNames.push_back("tree_SinglePhoton_2017F.root");
      data2017 = new TChain("tree");
      for (unsigned int i = 0 ; i < SinglePhotonFileNames.size() ; i++) {
        data2017->Add(skimType+"/"+SinglePhotonFileNames[i]);
      }
      dataNtuple = new RA2bTree(data2017);
    }


    if (Year.Contains("2018") && runData && (r == kSLe || r == kPhoton) ) {
      SingleElectronNames.push_back("tree_EGamma_2018A.root");
      SingleElectronNames.push_back("tree_EGamma_2018B.root");
      SingleElectronNames.push_back("tree_EGamma_2018C.root");
      SingleElectronNames.push_back("tree_EGamma_2018D.root");
      data2018 = new TChain("tree");
      for (unsigned int i = 0 ; i < SingleElectronNames.size() ; i++) {
        data2018->Add(skimType+"/"+SingleElectronNames[i]);
      }
      dataNtuple = new RA2bTree(data2018);
    }

    if (Year.Contains("2018") && runData && r == kSLm) {
      SingleMuonNames.push_back("tree_SingleMuon_2018A.root");
      SingleMuonNames.push_back("tree_SingleMuon_2018B.root");
      SingleMuonNames.push_back("tree_SingleMuon_2018C.root");
      SingleMuonNames.push_back("tree_SingleMuon_2018D.root");
      data2018 = new TChain("tree");
      for (unsigned int i = 0 ; i < SingleMuonNames.size() ; i++) {
        data2018->Add(skimType+"/"+SingleMuonNames[i]);
      }
      dataNtuple = new RA2bTree(data2018);
    }


    /////////////////////////////////////////////////////////////////////
    // - - - - - - - - - - - - - - Signal  - - - - - - - - - - - - - - //
    /////////////////////////////////////////////////////////////////////

    if (run_T5HH1D) { //to check FullSIM
      for (int i=1000;i<2700;i+=100) {
        if (i!=mass2D) continue;
        T5HH = new TChain("tree");
        TString fileName = V18Signal_DIR+"tree_T5qqqqZH-mGluino-1000to2500_"+TString::Format("%d",i)+"_1_"+Year+".root";
        T5HH->Add(fileName);
        ntuples.push_back(new RA2bTree(T5HH));
        sampleName.push_back(TString::Format("T5HH%d_LSP1", i));
      }
    }

    if (run_T5HH2D) {
      ifstream file("Gluino2DScanNames.txt");
      string line; TString fileName;
      while(std::getline(file, line)) {
       std::vector<std::string> x = mySplit(line, '_');
       int gluino_mass = std::stoi(x[3]); int LSP_mass = std::stoi(x[4]);
       if (mass2D!=815 && gluino_mass!=mass2D) continue;
       T5HH = new TChain("tree");
       fileName = V18Signal_DIR+"tree_T5qqqqZH_HToBB_"+TString::Format("%d_%d_",gluino_mass,LSP_mass)+Year+"_fast.root";
       T5HH->Add(fileName);
       ntuples.push_back(new RA2bTree(T5HH));
       sampleName.push_back(TString::Format("T5HH%d_LSP%d", gluino_mass, LSP_mass));
      }
    }

    if (run_TChiHH2D) {
      ifstream file("higgsino2DFileNames.txt");
      string line; TString fileName;
      while(std::getline(file, line)) {
       std::vector<std::string> x = mySplit(line, '_');
       int hino_mass = std::stoi(x[5]); int LSP_mass = std::stoi(x[6]);
       if (mass2D!=805 && hino_mass!=mass2D) continue;
       TChiHH = new TChain("tree");
       fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_2D_"+TString::Format("%d_%d_",hino_mass,LSP_mass)+Year+"_fast.root";
       TChiHH->Add(fileName);
       ntuples.push_back(new RA2bTree(TChiHH));
       sampleName.push_back(TString::Format("TChiHH%d_LSP%d", hino_mass, LSP_mass));
      }
      //Include 1D scan
      for (int i=150;i<810;i+=25) {
        if (mass2D!=805 && i!=mass2D) continue;
        TChiHH = new TChain("tree");
        TString fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_"+Year+"_fast.root";
        TChiHH->Add(fileName);
        ntuples.push_back(new RA2bTree(TChiHH));
        sampleName.push_back(TString::Format("TChiHH%d_LSP1", i));
      }
    }

    if (run_TChiHH1D) {
      for (int i=150;i<1525;i+=25) {
        if (i!=mass2D) continue;
        TChiHH1D = new TChain("tree");
        TString fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_"+Year+"_fast.root";
        TChiHH1D->Add(fileName);
        ntuples.push_back(new RA2bTree(TChiHH1D));
        sampleName.push_back(TString::Format("TChiHH%d_LSP1", i));
      }
    }
    if (run_TChiHH1DAll) {
      for (int i=150;i<1525;i+=25) {
        if (i!=mass2D) continue;
        TChiHH1D = new TChain("tree"); TString fileName;
        fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_MC2016_fast.root";
        TChiHH1D->Add(fileName);
        fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_MC2017_fast.root";
        TChiHH1D->Add(fileName);
        fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_MC2018_fast.root";
        TChiHH1D->Add(fileName);
        ntuples.push_back(new RA2bTree(TChiHH1D));
        sampleName.push_back(TString::Format("TChiHH%d_LSP1", i));
      }
    }

    if (run_TChiHH2DAll) {
      ifstream file("higgsino2DFileNames.txt");
      string line; TString fileName;
      while(std::getline(file, line)) {
       std::vector<std::string> x = mySplit(line, '_');
       int hino_mass = std::stoi(x[5]); int LSP_mass = std::stoi(x[6]);
       if (mass2D!=805 && hino_mass!=mass2D) continue;
       TChiHH = new TChain("tree");
       fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_2D_"+TString::Format("%d_%d_",hino_mass,LSP_mass)+"MC2016_fast.root";
       TChiHH->Add(fileName);
       fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_2D_"+TString::Format("%d_%d_",hino_mass,LSP_mass)+"MC2017_fast.root";
       TChiHH->Add(fileName);
       fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_2D_"+TString::Format("%d_%d_",hino_mass,LSP_mass)+"MC2018_fast.root";
       TChiHH->Add(fileName);
       ntuples.push_back(new RA2bTree(TChiHH));
       sampleName.push_back(TString::Format("TChiHH%d_LSP%d", hino_mass, LSP_mass));
      }
      for (int i=150;i<810;i+=25) {
        TString fileName;
        if (mass2D!=805 && i!=mass2D) continue;
        TChiHH = new TChain("tree");
        fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_MC2016_fast.root";
        TChiHH->Add(fileName);
        fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_MC2017_fast.root";
        TChiHH->Add(fileName);
        fileName = keithsDir+"tree_TChiHH_HToBB_HToBB_"+TString::Format("%d",i)+"_1_MC2018_fast.root";
        TChiHH->Add(fileName);
        ntuples.push_back(new RA2bTree(TChiHH));
        sampleName.push_back(TString::Format("TChiHH%d_LSP1", i));
      }
    }

    if (run_T5HH1DAll) { //to check FullSIM
      for (int i=1000;i<2700;i+=100) {
        if (i!=mass2D) continue;
        TString fileName;
        T5HH = new TChain("tree");
        fileName = V18Signal_DIR+"tree_T5qqqqZH-mGluino-1000to2500_"+TString::Format("%d",i)+"_1_MC2016.root";
        T5HH->Add(fileName);
        fileName = V18Signal_DIR+"tree_T5qqqqZH-mGluino-1000to2500_"+TString::Format("%d",i)+"_1_MC2017.root";
        T5HH->Add(fileName);
        fileName = V18Signal_DIR+"tree_T5qqqqZH-mGluino-1000to2500_"+TString::Format("%d",i)+"_1_MC2018.root";
        T5HH->Add(fileName);
        ntuples.push_back(new RA2bTree(T5HH));
        sampleName.push_back(TString::Format("T5HH%d_LSP1", i));
      }
    }
  }; //end Skim samples

  RA2bTree* findNtuple(TString name) {
    for (unsigned int iSam = 0 ; iSam < sampleName.size() ; iSam++) {
      if (sampleName[iSam] == name)
      return ntuples[iSam] ;
    }
    return NULL;
  };
};
