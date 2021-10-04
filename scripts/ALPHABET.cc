#include "../src/plotterUtils.cc"
#include "../src/skimSamples.cc"
#include "../src/definitions.cc" //all root dependencies in here
#include "../src/RA2bTree.cc"
#include "../src/BTagSF.h"

#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <assert.h>

using namespace std;

int main(int argc, char** argv){
  int region(0); region = atoi(argv[1]); //0 lepton, 1D signal, 2D signal, single muon, single electron, photon
  TString Year(argv[2]); //MC2016
  int runVeto(0); runVeto = atoi(argv[3]); //remove events that overlap with resolved
  int mass2D(0); mass2D = atoi(argv[4]); //For running over signal
  std::string mass2D_string = std::to_string(mass2D);
  string yearStr = "2016"; if (Year=="MC2017") yearStr = "2017"; if (Year=="MC2018") yearStr = "2018";

  setMasses(mass2D_string,"1");
  bool runMC = true;
  bool runData = true;
  bool applySFs_ = true; //btag SFs, SFs for photons, electrons, and muons - only used for MC/data comparisons
  bool runFullSIM = false; //only for 1D T5HH
  bool saveBoostedEvt = true; //for WX, saves the boosted event counts in the analysis regions
  string outDir_evtCount = "../src/evtCount/boost/";


  if (region==0 && runData) runMC = false; //don't run MC for 0-lepton at the same time as data
  if (region==1 || region==2) runData=false; //don't run data for SUSY signals
  if (region==0 || region==1 || region==2) applySFs_ = false;
  // if (region==1 || region==2) applySFs_ = false; //toggle if doing data/MC comparisons for 0-lepton region

  skimSamples* skims_;
  if (region == 0 ) skims_ = new skimSamples(skimSamples::kSignal, Year, mass2D);
  else if (region == 1 ) skims_ = new skimSamples(skimSamples::k1DSignal, Year, mass2D);
  else if (region == 2 ) skims_ = new skimSamples(skimSamples::k2DSignal, Year, mass2D);
  else if (region == 3 ) skims_ = new skimSamples(skimSamples::kSLm, Year, mass2D);
  else if (region == 4 ) skims_ = new skimSamples(skimSamples::kSLe, Year, mass2D);
  else if (region == 5 ) skims_ = new skimSamples(skimSamples::kPhoton, Year, mass2D);
  else assert(1);
  skimSamples skims = *skims_;

  typedef bool(*cuts)(RA2bTree*);
  vector<cuts> baselineCuts;

  if (region == 0 || region == 1 || region == 2) baselineCuts.push_back(*boostedBaselineCut<RA2bTree>);
  else if (region == 3) baselineCuts.push_back(*singleMuBaselineCut<RA2bTree>);
  else if (region == 4) baselineCuts.push_back(*singleEleBaselineCut<RA2bTree>);
  else if (region == 5) baselineCuts.push_back(*photonBaselineCut<RA2bTree>);
  else assert(1);

  TString regionName=""; TString cutName="_"; TString dataCut = "";
  if (region == 1) regionName = "1DTChiHH";
  if (region == 1 && runFullSIM) regionName = "1DT5HH_FullSIM";
  else if (region == 2 && mass2D<810) regionName = "2DTChiHH"; //for datacards
  else if (region == 2 && mass2D>810) regionName = "2DT5HH";  //for datacards
  // else if (region == 2) regionName = "2DTChiHH";
  // else if (region == 2) regionName = "2DT5HH";
  else if (region == 3) regionName="singleMu";
  else if (region == 4) regionName="singleEle";
  else if (region == 5) regionName="photon";
  if (runVeto) cutName = "_resVeto_";
  if (runData) dataCut = "_data";

  cout <<"In ALPHABET region: "<<regionName<<", veto?: "<<runVeto<<endl;
  string out_dir = "./";
  TString fileName = "ALPHABET"+Year+"_V18_"+regionName+dataCut+cutName+mass2D_string+".root";
  TFile* outputFile = new TFile(fileName,"RECREATE");

  typedef plot<RA2bTree> plot; vector<vector<plot> > plots; //so many are useless...
  double mJbins[4]={baselineMassLow,HmassWindowLow,HmassWindowHigh,baselineMassHigh};
  double METbinsPhoton[5]={200.,300.,500.,700.,1400.};
  double METbinsExtra[6]={300.,400.,500.,600.,700.,1400.};
  double METbins[4]={300.,500.,700.,1400.};


  plot METPhoton_Plot(*fillMET<RA2bTree>,"METPhoton","MET [GeV]",4,METbinsPhoton);
  plot MET_Plot(*fillMET<RA2bTree>,"MET","MET [GeV]",3,METbins);
  plot HT_Plot(*fillHT<RA2bTree>,"HT","HT [GeV]",100,0.,5000.);
  plot METall_Plot(*fillMET<RA2bTree>,"METall","MET [GeV]",100,0.,3000.);
  plot MET5_Plot(*fillMET<RA2bTree>,"MET5","MET [GeV]",5,METbinsExtra);

  plot J1pt_Ptplot(*fillLeadingJetPt<RA2bTree>,"J1pt_Pt","p_{T,J} [GeV]",50,300.,1300.);
  plot J1pt_Mplot(*fillLeadingJetMass<RA2bTree>,"J1pt_M","m_{J} [GeV]",80,60.,260.);
  plot J1_M_jetBins(*fillLeadingJetMass<RA2bTree>,"J1_M_jetBins","m_{J} [GeV]",3,mJbins);
  plot J2pt_Ptplot(*fillSubLeadingJetPt<RA2bTree>,"J2pt_Pt","p_{T,J} [GeV]",50,300.,1300.);
  plot J2pt_Mplot(*fillSubLeadingJetMass<RA2bTree>,"J2pt_M","m_{J} [GeV]",80,60.,260.);
  plot J2_M_jetBins(*fillSubLeadingJetMass<RA2bTree>,"J2_M_jetBins","m_{J} [GeV]",3,mJbins);
  plot J1_deepbbtag(*fillLeadingdeepBBtag<RA2bTree>,"LeadDeepBBTag","Lead jet deep bb-tag",50,0.0,1.0);
  plot J2_deepbbtag(*fillSubLeadingdeepBBtag<RA2bTree>,"SubLeadDeepBBTag","Sub-lead jet deep bb-tag",50,0.0,1.0);

  TH2F * h_j1vj2_mass_baseline = new TH2F("j1vj2_mass_baseline",";J2 soft-drop mass [GeV]; J1 soft-drop mass [GeV]",400,60, 260, 400, 60, 260);
  TH2F * h_j1vj2_mass_0H = new TH2F("j1vj2_mass_0H",";J2 soft-drop mass [GeV]; J1 soft-drop mass [GeV]",400,60, 260, 400, 60, 260);
  TH2F * h_j1vj2_mass_1H = new TH2F("j1vj2_mass_1H",";J2 soft-drop mass [GeV]; J1 soft-drop mass [GeV]",400,60, 260, 400, 60, 260);
  TH2F * h_j1vj2_mass_2H = new TH2F("j1vj2_mass_2H",";J2 soft-drop mass [GeV]; J1 soft-drop mass [GeV]",400,60, 260, 400, 60, 260);


  vector<plot> baselinePlots;
  vector<plot> doubletagSRPlots;
  vector<plot> doubletagSBPlots;
  vector<plot> tagSRPlots;
  vector<plot> tagSBPlots;
  vector<plot> antitagSRPlots;
  vector<plot> antitagSBPlots;
  vector<plot> antitagBTagPlots; //this is BTagsT>0, used for the MET shape

  baselinePlots.push_back(plot(MET_Plot));
  doubletagSRPlots.push_back(plot(MET_Plot));
  doubletagSBPlots.push_back(plot(MET_Plot));
  tagSRPlots.push_back(plot(MET_Plot));
  tagSBPlots.push_back(plot(MET_Plot));
  antitagSRPlots.push_back(plot(MET_Plot));
  antitagSBPlots.push_back(plot(MET_Plot));
  antitagBTagPlots.push_back(plot(MET_Plot));

  baselinePlots.push_back(plot(J1pt_Ptplot));
  baselinePlots.push_back(plot(J2pt_Ptplot));
  baselinePlots.push_back(plot(J1pt_Mplot));
  baselinePlots.push_back(plot(J2pt_Mplot));
  baselinePlots.push_back(plot(J1_deepbbtag));
  baselinePlots.push_back(plot(J2_deepbbtag));

  doubletagSRPlots.push_back(plot(J1pt_Ptplot));
  doubletagSRPlots.push_back(plot(J2pt_Ptplot));
  doubletagSRPlots.push_back(plot(J1pt_Mplot));
  doubletagSRPlots.push_back(plot(J2pt_Mplot));
  doubletagSRPlots.push_back(plot(J1_deepbbtag));
  doubletagSRPlots.push_back(plot(J2_deepbbtag));

  doubletagSBPlots.push_back(plot(J1pt_Ptplot));
  doubletagSBPlots.push_back(plot(J2pt_Ptplot));
  doubletagSBPlots.push_back(plot(J1pt_Mplot));
  doubletagSBPlots.push_back(plot(J2pt_Mplot));
  doubletagSBPlots.push_back(plot(J1_deepbbtag));
  doubletagSBPlots.push_back(plot(J2_deepbbtag));

  tagSRPlots.push_back(plot(J1pt_Ptplot));
  tagSRPlots.push_back(plot(J2pt_Ptplot));
  tagSRPlots.push_back(plot(J1pt_Mplot));
  tagSRPlots.push_back(plot(J2pt_Mplot));
  tagSRPlots.push_back(plot(J1_deepbbtag));
  tagSRPlots.push_back(plot(J2_deepbbtag));

  tagSBPlots.push_back(plot(J1pt_Ptplot));
  tagSBPlots.push_back(plot(J2pt_Ptplot));
  tagSBPlots.push_back(plot(J1pt_Mplot));
  tagSBPlots.push_back(plot(J2pt_Mplot));
  tagSBPlots.push_back(plot(J1_deepbbtag));
  tagSBPlots.push_back(plot(J2_deepbbtag));

  antitagSRPlots.push_back(plot(J1pt_Ptplot));
  antitagSRPlots.push_back(plot(J2pt_Ptplot));
  antitagSRPlots.push_back(plot(J1pt_Mplot));
  antitagSRPlots.push_back(plot(J2pt_Mplot));
  antitagSRPlots.push_back(plot(J1_deepbbtag));
  antitagSRPlots.push_back(plot(J2_deepbbtag));

  antitagSBPlots.push_back(plot(J1pt_Ptplot));
  antitagSBPlots.push_back(plot(J2pt_Ptplot));
  antitagSBPlots.push_back(plot(J1pt_Mplot));
  antitagSBPlots.push_back(plot(J2pt_Mplot));
  antitagSBPlots.push_back(plot(J1_deepbbtag));
  antitagSBPlots.push_back(plot(J2_deepbbtag));

  antitagBTagPlots.push_back(plot(J1pt_Ptplot));
  antitagBTagPlots.push_back(plot(J2pt_Ptplot));
  antitagBTagPlots.push_back(plot(J1pt_Mplot));
  antitagBTagPlots.push_back(plot(J2pt_Mplot));
  antitagBTagPlots.push_back(plot(J1_deepbbtag));
  antitagBTagPlots.push_back(plot(J2_deepbbtag));

  baselinePlots.push_back(plot(METall_Plot));
  baselinePlots.push_back(plot(METPhoton_Plot));
  baselinePlots.push_back(plot(MET5_Plot));
  baselinePlots.push_back(plot(HT_Plot));
  baselinePlots.push_back(plot(J1_M_jetBins));
  baselinePlots.push_back(plot(J2_M_jetBins));

  doubletagSRPlots.push_back(plot(METall_Plot));
  doubletagSRPlots.push_back(plot(METPhoton_Plot));
  doubletagSRPlots.push_back(plot(MET5_Plot));
  doubletagSRPlots.push_back(plot(HT_Plot));
  doubletagSRPlots.push_back(plot(J1_M_jetBins));
  doubletagSRPlots.push_back(plot(J2_M_jetBins));

  doubletagSBPlots.push_back(plot(METall_Plot));
  doubletagSBPlots.push_back(plot(METPhoton_Plot));
  doubletagSBPlots.push_back(plot(MET5_Plot));
  doubletagSBPlots.push_back(plot(HT_Plot));
  doubletagSBPlots.push_back(plot(J1_M_jetBins));
  doubletagSBPlots.push_back(plot(J2_M_jetBins));

  tagSRPlots.push_back(plot(METall_Plot));
  tagSRPlots.push_back(plot(METPhoton_Plot));
  tagSRPlots.push_back(plot(MET5_Plot));
  tagSRPlots.push_back(plot(HT_Plot));
  tagSRPlots.push_back(plot(J1_M_jetBins));
  tagSRPlots.push_back(plot(J2_M_jetBins));

  tagSBPlots.push_back(plot(METall_Plot));
  tagSBPlots.push_back(plot(METPhoton_Plot));
  tagSBPlots.push_back(plot(MET5_Plot));
  tagSBPlots.push_back(plot(HT_Plot));
  tagSBPlots.push_back(plot(J1_M_jetBins));
  tagSBPlots.push_back(plot(J2_M_jetBins));

  antitagSRPlots.push_back(plot(METall_Plot));
  antitagSRPlots.push_back(plot(METPhoton_Plot));
  antitagSRPlots.push_back(plot(MET5_Plot));
  antitagSRPlots.push_back(plot(HT_Plot));
  antitagSRPlots.push_back(plot(J1_M_jetBins));
  antitagSRPlots.push_back(plot(J2_M_jetBins));

  antitagSBPlots.push_back(plot(METall_Plot));
  antitagSBPlots.push_back(plot(METPhoton_Plot));
  antitagSBPlots.push_back(plot(MET5_Plot));
  antitagSBPlots.push_back(plot(HT_Plot));
  antitagSBPlots.push_back(plot(J1_M_jetBins));
  antitagSBPlots.push_back(plot(J2_M_jetBins));

  antitagBTagPlots.push_back(plot(METall_Plot));
  antitagBTagPlots.push_back(plot(METPhoton_Plot));
  antitagBTagPlots.push_back(plot(MET5_Plot));
  antitagBTagPlots.push_back(plot(HT_Plot));
  antitagBTagPlots.push_back(plot(J1_M_jetBins));
  antitagBTagPlots.push_back(plot(J2_M_jetBins));

  BTagSF* btagsf_;
  std::vector<TH2F*> hSFeff_; if (region!=1 && region!=2) hSFeff_ = openSFFiles(Year, region);
  std::vector<TEfficiency*> eTrigEff_; if (region == 5) eTrigEff_ = photonTrigEffHist(Year);

  //Files for boosted event counts
  ofstream txtfile_2HSR; ofstream txtfile_2HSB;
  ofstream txtfile_1HSR; ofstream txtfile_1HSB;
  ofstream txtfile_0HSR; ofstream txtfile_0HSB; ofstream txtfile_0Hb;
  if (saveBoostedEvt) {
    string thisRegion = "MC_";
    if (region==1) thisRegion = "1DTChiHH"+mass2D_string+"_";
    if (region==1 && runFullSIM) thisRegion = "1DT5HH"+mass2D_string+"FullSIM_";
    else if (region == 2) thisRegion = "2DTChiHH"+mass2D_string+"_";

    string txtname1 = "evtCount_boosted_2HSR_"+thisRegion+yearStr+".txt";
    string txtname2 = "evtCount_boosted_2HSB_"+thisRegion+yearStr+".txt";
    string txtname3 = "evtCount_boosted_1HSR_"+thisRegion+yearStr+".txt";
    string txtname4 = "evtCount_boosted_1HSB_"+thisRegion+yearStr+".txt";
    string txtname5 = "evtCount_boosted_0HSR_"+thisRegion+yearStr+".txt";
    string txtname6 = "evtCount_boosted_0HSB_"+thisRegion+yearStr+".txt";
    string txtname7 = "evtCount_boosted_0Hb_"+thisRegion+yearStr+".txt";

    txtfile_2HSR.open(outDir_evtCount+txtname1); txtfile_2HSB.open(outDir_evtCount+txtname2);
    txtfile_1HSR.open(outDir_evtCount+txtname3); txtfile_1HSB.open(outDir_evtCount+txtname4);
    txtfile_0HSR.open(outDir_evtCount+txtname5); txtfile_0HSB.open(outDir_evtCount+txtname6);
    txtfile_0Hb.open(outDir_evtCount+txtname7);

    txtfile_2HSR<<"#SR1,2HSR\n#Run:Lumi:EvtNum:MET"<<endl;
    txtfile_2HSB<<"#CR1,2HSB\n#Run:Lumi:EvtNum:MET"<<endl;
    txtfile_1HSR<<"#SR2,1HSR\n#Run:Lumi:EvtNum:MET"<<endl;
    txtfile_1HSB<<"#CR2,1HSB\n#Run:Lumi:EvtNum:MET"<<endl;
    txtfile_0HSR<<"#CR3,0HSR\n#Run:Lumi:EvtNum:MET"<<endl;
    txtfile_0HSB<<"#CR4,0HSB\n#Run:Lumi:EvtNum:MET"<<endl;
    txtfile_0Hb<<"#CR5,0Hb\n#Run:Lumi:EvtNum:MET"<<endl;
  }


  if (runVeto) {
    if (region==1 && !runFullSIM) {
      readResVeto_Sig1D(yearStr,"TChiHH");
    }
    else if (region==1 && runFullSIM) readResVeto_Sig1D(yearStr,"T5HH");
    else if (region==2) readResVeto_Sig2D(yearStr,"TChiHH");
    else if (region==0 && runData) readResVeto_Data(yearStr);
    else readResVeto_MC(yearStr);
  }

  if (runMC) {
    // MC samples - setup to not run 0-lepton MC if you run the data as well
    for (unsigned int iSample = 0; iSample < skims.ntuples.size(); iSample++) {
      RA2bTree* ntuple = skims.ntuples[iSample];
      for (unsigned int i = 0; i < baselinePlots.size(); i++) {
        baselinePlots[i].addNtuple(ntuple,"baseline_"+skims.sampleName[iSample],true);
        doubletagSRPlots[i].addNtuple(ntuple,"doubletagSR_"+skims.sampleName[iSample],true);
        doubletagSBPlots[i].addNtuple(ntuple,"doubletagSB_"+skims.sampleName[iSample],true);
        tagSRPlots[i].addNtuple(ntuple,"tagSR_"+skims.sampleName[iSample],true);
        tagSBPlots[i].addNtuple(ntuple,"tagSB_"+skims.sampleName[iSample],true);
        antitagSRPlots[i].addNtuple(ntuple,"antitagSR_"+skims.sampleName[iSample],true);
        antitagSBPlots[i].addNtuple(ntuple,"antitagSB_"+skims.sampleName[iSample],true);
        antitagBTagPlots[i].addNtuple(ntuple,"antitagOpt1_"+skims.sampleName[iSample],true);
      }
      TH2F * h_baseline = (TH2F*)h_j1vj2_mass_baseline->Clone("j1vj2M_baseline_"+skims.sampleName[iSample]);
      TH2F * h_0H = (TH2F*)h_j1vj2_mass_0H->Clone("j1vj2M_0H_"+skims.sampleName[iSample]);
      TH2F * h_1H = (TH2F*)h_j1vj2_mass_1H->Clone("j1vj2M_1H_"+skims.sampleName[iSample]);
      TH2F * h_2H = (TH2F*)h_j1vj2_mass_2H->Clone("j1vj2M_2H_"+skims.sampleName[iSample]);
      int numEvents = ntuple->fChain->GetEntries();
      ntupleBranchStatus<RA2bTree>(ntuple); //these are set in definitions
      bool passBaseline;
      TFile *fin;

      TString startFilename = ntuple->fChain->GetFile()->GetName();
      double this_lumi = 35922.0;
      if ( startFilename.Contains("2017") ) this_lumi = 41529.0;
      else if ( startFilename.Contains("2018") ) this_lumi = 59740.0;

      setSignalMasses(ntuple,region);

      int TotalEvents = 0; TH1F* h_norm;
      if (startFilename.Contains("TChiHH_HToBB") || startFilename.Contains("T5qqqqZH") || startFilename.Contains("T5HH") ) {
        if (startFilename.Contains("TChiHH_HToBB") || runFullSIM) {
          fin = TFile::Open(startFilename);
          h_norm = (TH1F*)fin->Get("PDFNorm"); //used for scales for signal systematics
          TH1F *nEventsHisto = (TH1F*)fin->Get("nEventProc");
          TotalEvents = nEventsHisto->GetBinContent(1); //used to fix weight branch in signal
        }
      }

      // For b-tag corrections (mostly for 0H+b region)
      if (applySFs_) btagsf_ = new BTagSF();
      else btagsf_ = nullptr;

      if (btagsf_) {
        if ( startFilename.Contains("2016") ) btagsf_->SetCalib(0);
        else if ( startFilename.Contains("2017") ) btagsf_->SetCalib(1);
        else if ( startFilename.Contains("2018") ) btagsf_->SetCalib(2);
        std::cout<<"Open file: "<< startFilename<<std::endl;
        fin = TFile::Open(startFilename);
        btagsf_->SetEffs(fin);
      }

      //Counters are for filling the signal mass points for the 2D mass plots in the paper
      //Specifically for 1D T5HH1600
      int counter2HSR = 0; int counter2HSB = 0;
      int counter1HSR = 0; int counter1HSB = 0;
      int counter0HSR = 0; int counter0HSB = 0;

      for (int iEvt = 0; iEvt < numEvents ; iEvt++) {
        ntuple->GetEntry(iEvt);
        if (runVeto && iEvt % 50000 == 0) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;
        else if (iEvt % 100000 == 0) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;
        double weight = 1.0; double isrweight = 1.0; float trigWeight = 1.0;
        double puweight = 1.0; double prefireWeight = 1.0; float scalesSyst = 1.0;
        double BSFwt = 1.0; double SFweight = 1.0;
        float bb_tag = 1.0; float bbtag_FastSIM = 1.0; float sdMass_FastSIM = 1.0;
        //because I never re-ran skims, so at least only do necessary calcualtions once per event
        setHT(ntuple,"none"); //also sets JEC/JER systematics
        setMET(ntuple); setJetPT(ntuple); setJetMass(ntuple); setDeltaPhis(ntuple);

        passBaseline=true;
        for (auto baselineCut : baselineCuts) {
          passBaseline&=baselineCut(ntuple);
          if (!passBaseline) continue;
        }
        if (!passBaseline) continue; //probably not necessary, but ya know CYA


        if (runVeto){
          if (resEventFound(ntuple,yearStr)) continue;
        }

        TString thisFilename = ntuple->fChain->GetFile()->GetName();
        if (thisFilename.Contains("SingleLeptFromT_MC") || thisFilename.Contains("SingleLeptFromTbar_MC")  || thisFilename.Contains("DiLept_MC")) {
          if (thisFilename.Contains("MC2018") && ntuple->GenMET>80.0) continue;
          else if (ntuple->GenMET>150.0) continue;
        }


        if ((thisFilename.Contains("T5qqqqZH")||thisFilename.Contains("T5HH")) && getNumGenHiggses(ntuple)!=2) continue;
        double thisMET = fillMET(ntuple); double thisHT = fillHT(ntuple);


        if (region == 5) {
          trigWeight = photonTrigEff(ntuple, eTrigEff_);
          if (applySFs_) SFweight = photonSFs(ntuple, hSFeff_);
        }
        else {
          TString whichTrigRegion = "0l"; float lept_pt = 0.0;
          if (skims.sampleName[iSample]=="QCD") whichTrigRegion = "fake";
          if (region == 3) {
            whichTrigRegion = "mu";
            if (applySFs_) SFweight = muonSFs(ntuple, Year, hSFeff_);
            lept_pt = ntuple->Muons->at(0).Pt();
          }
          if (region == 4) {
            whichTrigRegion = "el";
            if (applySFs_) SFweight = electronSFs(ntuple, Year, hSFeff_);
            lept_pt = ntuple->Electrons->at(0).Pt();
          }
          //up and down for signal systematics
          trigWeight = triggerEfficiencies(whichTrigRegion, Year, thisMET, thisHT, lept_pt, "none");
        }

        //"up" and "down" for systematics, "none" for nominal
        isrweight = ISRCorrections(ntuple,region,Year,"none");
        puweight = PUCorrections(ntuple,"none");
        prefireWeight = prefireCorr(ntuple,Year,"none");
        scalesSyst = scalesSystematic(ntuple,h_norm,"none"); //none returns 1.0
        weight = ntuple->Weight*this_lumi*trigWeight*isrweight*puweight*prefireWeight*SFweight*scalesSyst;
        // weight = ntuple->Weight*this_lumi; //used for signal efficiency

        //check if a new file has been opened, and if yes, reset btag efficiency histos
        if (btagsf_ && startFilename!=thisFilename) {
          std::cout<<"Open file: "<< thisFilename<<std::endl;
          fin = TFile::Open(thisFilename);
          btagsf_->SetEffs(fin);
          startFilename=thisFilename;
        }

        //b-tag SF, only used for the MC mismodeling systematic (not signal systematics)
        //Applied later on for just 0H+region
        // if (applySFs_) {
        //   BSFwt = btagsf_->weight(ntuple->Jets, ntuple->Jets_hadronFlavor, ntuple->Jets_HTMask, ntuple->Jets_bJetTagDeepCSVBvsAll,0,0); //central
          // float btag_pt1 = btagsf_->weight(ntuple->Jets, ntuple->Jets_hadronFlavor, ntuple->Jets_HTMask, ntuple->Jets_bJetTagDeepCSVBvsAll,1,0); //up
          // float btag_pt2 = btagsf_->weight(ntuple->Jets, ntuple->Jets_hadronFlavor, ntuple->Jets_HTMask, ntuple->Jets_bJetTagDeepCSVBvsAll,0,1); //up
          // float btag_pt1 = btagsf_->weight(ntuple->Jets, ntuple->Jets_hadronFlavor, ntuple->Jets_HTMask, ntuple->Jets_bJetTagDeepCSVBvsAll,-1,0); //down
          // float btag_pt2 = btagsf_->weight(ntuple->Jets, ntuple->Jets_hadronFlavor, ntuple->Jets_HTMask, ntuple->Jets_bJetTagDeepCSVBvsAll,0,-1); //down
          // BSFwt = sqrt(btag_pt1*btag_pt1 + btag_pt2*btag_pt2);
        // }

        if ( thisFilename.Contains("TChiHH_HToBB")||thisFilename.Contains("TChiHH") || (thisFilename.Contains("T5qqqqZH")||thisFilename.Contains("T5HH")) && !runFullSIM) weight = weight/TotalEvents*(0.5823329*0.5823329);
        else if ( (thisFilename.Contains("T5qqqqZH")||thisFilename.Contains("T5HH"))) weight = weight/TotalEvents*4; //for FullSIM

        //Re-weight 1D TChiHH to 2D
        if (region==2 && thisFilename.Contains("TChiHH") && !(thisFilename.Contains("2D"))) weight = weight/higgsinoCrossSection1D(NLSPmass)*higgsinoCrossSection2D(NLSPmass);

        // up and down for signal systematics
        sdMass_FastSIM = softdropmassFastSIMSFs(ntuple,"none");
        bb_tag = bbSFs(ntuple,"none");
        bbtag_FastSIM = bbFastSIMSFs(ntuple,"none");
        weight = weight*bb_tag*bbtag_FastSIM*sdMass_FastSIM;

        //For boosted evtCount
        std::stringstream METstream;
        METstream << std::fixed << std::setprecision(3) << thisMET;
        std::string METstring = METstream.str();
        string runLumiEvtMET = std::to_string(ntuple->RunNum)+":"+std::to_string(ntuple->LumiBlockNum)+":"+std::to_string(ntuple->EvtNum)+":"+METstring;


        for (unsigned int i = 0; i < baselinePlots.size(); i++) {baselinePlots[i].fill(ntuple,weight);}
        h_baseline->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple), weight);

        // first check for 2H
        if (doubleTaggingLooseCut(ntuple)) {
          if (doubletagSRCut(ntuple)) {
            if (saveBoostedEvt) txtfile_2HSR << runLumiEvtMET << endl;
            counter2HSR++;
            if (counter2HSR<=54) h_2H->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple),weight);
            for (unsigned int i = 0; i < doubletagSRPlots.size(); i++) {doubletagSRPlots[i].fill(ntuple,weight);}
          }
          else if (doubletagSBCut(ntuple)) {
            if (saveBoostedEvt) txtfile_2HSB << runLumiEvtMET << endl;
            counter2HSB++;
            if (counter2HSB<=27) h_2H->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple),weight);
            for (unsigned int i = 0; i < doubletagSBPlots.size(); i++) {doubletagSBPlots[i].fill(ntuple,weight);}
          }
        } //end 2H

        //then check for 1H
        else if (singleHiggsTagLooseCut(ntuple)) {
          if (tagSRCut(ntuple)) {
            if (saveBoostedEvt) txtfile_1HSR << runLumiEvtMET << endl;
            counter1HSR++;
            if (counter1HSR<=67) h_1H->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple),weight);
            for (unsigned int i = 0; i < tagSRPlots.size(); i++) tagSRPlots[i].fill(ntuple,weight);
          }
          else if (tagSBCut(ntuple)) {
            if (saveBoostedEvt) txtfile_1HSB << runLumiEvtMET << endl;
            counter1HSB++;
            if (counter1HSB<=53) h_1H->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple),weight);
            for (unsigned int i = 0; i < tagSBPlots.size(); i++) tagSBPlots[i].fill(ntuple,weight);
          }
        } //end 1H

        //then check for 0H
        else if (antiTaggingLooseCut(ntuple)) {
          if (applySFs_) BSFwt = btagsf_->weight(ntuple->Jets, ntuple->Jets_hadronFlavor, ntuple->Jets_HTMask, ntuple->Jets_bJetTagDeepCSVBvsAll,0,0); //central
          if (antitagPlusBCut(ntuple)) {
            if (saveBoostedEvt) txtfile_0Hb << runLumiEvtMET << endl;
            for (unsigned int i = 0; i < antitagBTagPlots.size(); i++) antitagBTagPlots[i].fill(ntuple,weight*BSFwt);
          }
          if (antitagSRCut(ntuple)) {
            if (saveBoostedEvt) txtfile_0HSR << runLumiEvtMET << endl;
            counter0HSR++;
            if (counter0HSR<=20) h_0H->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple),weight);
            for (unsigned int i = 0; i < antitagSRPlots.size(); i++) antitagSRPlots[i].fill(ntuple,weight);
          }
          else if (antitagSBCut(ntuple)) {
            if (saveBoostedEvt) txtfile_0HSB << runLumiEvtMET << endl;
            counter0HSB++;
            if (counter0HSB<=23) h_0H->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple), weight);
            for (unsigned int i = 0; i < antitagSBPlots.size(); i++) antitagSBPlots[i].fill(ntuple,weight);
          }
        } //end 0H region
        clearGlobalConstants();
      } // end event loop
      outputFile->cd();
      h_baseline->Write(); h_0H->Write(); h_1H->Write(); h_2H->Write();
    } // end sample loop
    if (saveBoostedEvt) {
      txtfile_2HSR.close(); txtfile_2HSB.close(); txtfile_1HSR.close(); txtfile_1HSB.close();
      txtfile_0HSR.close(); txtfile_0HSB.close(); txtfile_0Hb.close();
    }
  }


  // Begin data
  if (region!=1 && region!=2 && runData) {
    RA2bTree* ntuple = skims.dataNtuple;
    for (unsigned int i = 0; i < baselinePlots.size(); i++) {
      baselinePlots[i].addDataNtuple(ntuple,"baseline_data");
      doubletagSRPlots[i].addDataNtuple(ntuple,"doubletagSR_data");
      doubletagSBPlots[i].addDataNtuple(ntuple,"doubletagSB_data");
      tagSRPlots[i].addDataNtuple(ntuple,"tagSR_data");
      tagSBPlots[i].addDataNtuple(ntuple,"tagSB_data");
      antitagSRPlots[i].addDataNtuple(ntuple,"antitagSR_data");
      antitagSBPlots[i].addDataNtuple(ntuple,"antitagSB_data");
      antitagBTagPlots[i].addDataNtuple(ntuple,"antitagOpt1_data");
    }

    TH2F * h_baseline_data = (TH2F*)h_j1vj2_mass_baseline->Clone("j1vj2M_baseline");
    TH2F * h_0H_data = (TH2F*)h_j1vj2_mass_0H->Clone("j1vj2M_0H_data");
    TH2F * h_1H_data = (TH2F*)h_j1vj2_mass_1H->Clone("j1vj2M_1H_data");
    TH2F * h_2H_data = (TH2F*)h_j1vj2_mass_2H->Clone("j1vj2M_2H_data");


    if (saveBoostedEvt && region==0) {
      string thisRegion = "data_";
      string txtname1 = "evtCount_boosted_2HSR_"+thisRegion+yearStr+".txt";
      string txtname2 = "evtCount_boosted_2HSB_"+thisRegion+yearStr+".txt";
      string txtname3 = "evtCount_boosted_1HSR_"+thisRegion+yearStr+".txt";
      string txtname4 = "evtCount_boosted_1HSB_"+thisRegion+yearStr+".txt";
      string txtname5 = "evtCount_boosted_0HSR_"+thisRegion+yearStr+".txt";
      string txtname6 = "evtCount_boosted_0HSB_"+thisRegion+yearStr+".txt";
      string txtname7 = "evtCount_boosted_0Hb_"+thisRegion+yearStr+".txt";

      txtfile_2HSR.open(outDir_evtCount+txtname1); txtfile_2HSB.open(outDir_evtCount+txtname2);
      txtfile_1HSR.open(outDir_evtCount+txtname3); txtfile_1HSB.open(outDir_evtCount+txtname4);
      txtfile_0HSR.open(outDir_evtCount+txtname5); txtfile_0HSB.open(outDir_evtCount+txtname6);
      txtfile_0Hb.open(outDir_evtCount+txtname7);

      txtfile_2HSR<<"#SR1,2HSR\n#Run:Lumi:EvtNum:MET"<<endl;
      txtfile_2HSB<<"#CR1,2HSB\n#Run:Lumi:EvtNum:MET"<<endl;
      txtfile_1HSR<<"#SR2,1HSR\n#Run:Lumi:EvtNum:MET"<<endl;
      txtfile_1HSB<<"#CR2,1HSB\n#Run:Lumi:EvtNum:MET"<<endl;
      txtfile_0HSR<<"#CR3,0HSR\n#Run:Lumi:EvtNum:MET"<<endl;
      txtfile_0HSB<<"#CR4,0HSB\n#Run:Lumi:EvtNum:MET"<<endl;
      txtfile_0Hb<<"#CR5,0Hb\n#Run:Lumi:EvtNum:MET"<<endl;
    }

    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
    bool passBaseline;
    for (int iEvt = 0; iEvt < numEvents; iEvt++) {
      ntuple->GetEntry(iEvt);
      if (iEvt % 100000 == 0 ) cout << "data: " << iEvt << "/" << numEvents << endl;
       //because I never re-ran skims, so at least only do necessary calcualtions once per event
      setMET(ntuple); setHT(ntuple,"none"); setJetPT(ntuple);
      setJetMass(ntuple,false); //true smears AK8 mass for signal systematic, so don't
      setDeltaPhis(ntuple);

      passBaseline=true;
      for (auto baselineCut : baselineCuts) {
        passBaseline&=baselineCut(ntuple);
        if (!passBaseline ) continue;
      }
      if (!passBaseline) continue; //probably not necessary, but just in case, ya know. CYA
      if (ntuple->eeBadScFilter!=1) continue; //apply to data only, moved from FiltersCut

      if (region == 0 && !signalTriggerCut(ntuple)) continue;
      else if (region == 3 && !singleMuTriggerCut(ntuple)) continue;
      else if (region == 4 && !singleEleTriggerCut(ntuple)) continue;
      else if (region == 5 && !photonTriggerCut(ntuple)) continue;

      //Toggle whether or not we veto resolved events
      if (runVeto && region==0){
        if (resEventFound(ntuple,yearStr)) continue;
      }

      //For boosted evtCount
      double thisMET = fillMET(ntuple);
      std::stringstream METstream;
      METstream << std::fixed << std::setprecision(3) << thisMET;
      std::string METstring = METstream.str();
      string runLumiEvtMET = std::to_string(ntuple->RunNum)+":"+std::to_string(ntuple->LumiBlockNum)+":"+std::to_string(ntuple->EvtNum)+":"+METstring;

      for (unsigned int i = 0; i < baselinePlots.size(); i++) {baselinePlots[i].fillData(ntuple);}
      h_baseline_data->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple));

      // first check for 2H
      if (doubleTaggingLooseCut(ntuple)) {
        if (doubletagSRCut(ntuple)) {
          if (saveBoostedEvt) txtfile_2HSR << runLumiEvtMET << endl;
          for (unsigned int i = 0; i < doubletagSRPlots.size(); i++) doubletagSRPlots[i].fillData(ntuple);
        }
        else if (doubletagSBCut(ntuple)) {
          for (unsigned int i = 0; i < doubletagSBPlots.size(); i++) {doubletagSBPlots[i].fillData(ntuple);}
          if (saveBoostedEvt) txtfile_2HSB << runLumiEvtMET << endl;
        }
        h_2H_data->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple));
      } //end 2H

      //then check for 1H
      else if (singleHiggsTagLooseCut(ntuple)) {
        if (tagSRCut(ntuple)) {
          for (unsigned int i = 0; i < tagSRPlots.size(); i++) tagSRPlots[i].fillData(ntuple);
          if (saveBoostedEvt) txtfile_1HSR << runLumiEvtMET << endl;
        }
        else if (tagSBCut(ntuple)) {
          for (unsigned int i = 0; i < tagSBPlots.size(); i++) tagSBPlots[i].fillData(ntuple);
          if (saveBoostedEvt) txtfile_1HSB << runLumiEvtMET << endl;
        }
        h_1H_data->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple));
      } //end 1H

      //then check for 0H
      else if (antiTaggingLooseCut(ntuple)) {
        if (antitagPlusBCut(ntuple)) {
          for (unsigned int i = 0; i < antitagBTagPlots.size(); i++) antitagBTagPlots[i].fillData(ntuple);
          if (saveBoostedEvt) txtfile_0Hb << runLumiEvtMET << endl;
        }
        if (antitagSRCut(ntuple)) {
          for (unsigned int i = 0; i < antitagSRPlots.size(); i++) antitagSRPlots[i].fillData(ntuple);
          if (saveBoostedEvt) txtfile_0HSR << runLumiEvtMET << endl;
        }
        else if (antitagSBCut(ntuple)) {
          for (unsigned int i = 0; i < antitagSBPlots.size(); i++) antitagSBPlots[i].fillData(ntuple);
          if (saveBoostedEvt) txtfile_0HSB << runLumiEvtMET << endl;
        }
        h_0H_data->Fill(fillLeadingJetMass(ntuple),fillSubLeadingJetMass(ntuple));
      } //end 0H region
    } // end event loop
    outputFile->cd();
    h_baseline_data->Write(); h_0H_data->Write(); h_1H_data->Write(); h_2H_data->Write();
    clearGlobalConstants();
    if (saveBoostedEvt) {
      txtfile_2HSR.close(); txtfile_2HSB.close(); txtfile_1HSR.close(); txtfile_1HSB.close();
      txtfile_0HSR.close(); txtfile_0HSB.close(); txtfile_0Hb.close();
    }
  } // end run data

  bool sumBkgs = true;
  if (region == 1 || region == 2 || runData) sumBkgs = false;
  outputFile->cd();
  for (unsigned int i = 0; i < baselinePlots.size(); i++) {
    baselinePlots[i].Write(runData);
    if (sumBkgs) {
      baselinePlots[i].buildSum("baseline");
      baselinePlots[i].sum->Write();
    }
  }

  for (unsigned int i = 0; i < doubletagSRPlots.size(); i++) {
    doubletagSRPlots[i].Write(runData);
    if (sumBkgs) {
      doubletagSRPlots[i].buildSum("doubletagSR");
      doubletagSRPlots[i].sum->Write();
    }
  }
  for (unsigned int i = 0; i < doubletagSBPlots.size(); i++) {
    doubletagSBPlots[i].Write(runData);
    if (sumBkgs) {
      doubletagSBPlots[i].buildSum("doubletagSB");
      doubletagSBPlots[i].sum->Write();
    }
  }
  for (unsigned int i = 0; i < tagSRPlots.size(); i++) {
    tagSRPlots[i].Write(runData);
    if (sumBkgs) {
      tagSRPlots[i].buildSum("tagSR");
      tagSRPlots[i].sum->Write();
    }
  }
  for (unsigned int i = 0; i < tagSBPlots.size(); i++) {
    tagSBPlots[i].Write(runData);
    if (sumBkgs) {
      tagSBPlots[i].buildSum("tagSB");
      tagSBPlots[i].sum->Write();
    }
  }
  for (unsigned int i = 0; i < antitagSRPlots.size(); i++) {
    antitagSRPlots[i].Write(runData);
    if (sumBkgs) {
      antitagSRPlots[i].buildSum("antitagSR");
      antitagSRPlots[i].sum->Write();
    }
  }
  for (unsigned int i = 0; i < antitagSBPlots.size(); i++) {
    antitagSBPlots[i].Write(runData);
    if (sumBkgs) {
      antitagSBPlots[i].buildSum("antitagSB");
      antitagSBPlots[i].sum->Write();
    }
  }
  for (unsigned int i = 0; i < antitagBTagPlots.size(); i++) {
    antitagBTagPlots[i].Write(runData);
    if (sumBkgs) {
      antitagBTagPlots[i].buildSum("antitagOpt1");
      antitagBTagPlots[i].sum->Write();
    }
  }
  outputFile->Close();
  return 0;
}
