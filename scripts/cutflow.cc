#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TPad.h"
#include "TLorentzVector.h"
#include <vector>
#include <map>
#include <iostream>
#include <cassert>

#include "../src/plotterUtils.cc"
#include "../src/skimSamples.cc"
#include "../src/definitions.cc"
#include "../src/RA2bTree.cc"
#include "../src/BTagSF.h"

using namespace std;

int main(int argc, char** argv) {
  int region(0); region = atoi(argv[1]);
  TString Year(argv[2]); setYear(Year);
  int runVeto(0); runVeto = atoi(argv[3]);
  int mass2D(0); mass2D = atoi(argv[4]);
  std::string mass2D_string = std::to_string(mass2D);

  string yearStr = "2016"; if (Year=="MC2017") yearStr = "2017"; if (Year=="MC2018") yearStr = "2018";

  bool runData = true; if (region == 1 || region == 2) runData = false;
  bool applySFs_ = false; //btag SFs, SFs for photons, electrons, and muons - only used for MC/data comparisons
  if (region == 1 || region == 2) runData = false;
  bool runFullSIM = false; //only for 1D T5HH

  std::cout<<"Cutflow! Region: "<<region<<", year: "<<Year<<", 2Dmass: "<<mass2D_string<<std::endl;
  skimSamples* skims_;
  if (region == 0 ) skims_ = new skimSamples(skimSamples::kSignal, Year,mass2D);
  else if (region == 1 ) skims_ = new skimSamples(skimSamples::k1DSignal, Year,mass2D);
  else if (region == 2 ) skims_ = new skimSamples(skimSamples::k2DSignal, Year,mass2D);
  else if (region == 3 ) skims_ = new skimSamples(skimSamples::kSLm, Year,mass2D);
  else if (region == 4 ) skims_ = new skimSamples(skimSamples::kSLe, Year,mass2D);
  else if (region == 5 ) skims_ = new skimSamples(skimSamples::kPhoton, Year,mass2D);
  else assert(1);

  TString regionName="MCBkg";
  if (region == 1) regionName = "1Dsignal";
  if (region == 2) regionName = "2Dsignal";
  if (region == 3) regionName="singleMu";
  if (region == 4) regionName="singleEle";
  if (region == 5) regionName="photon";

  TFile* outputFile = new TFile("cutflow"+Year+"_V18_"+regionName+".root","RECREATE");

  //Open SF files
  std::vector<TH2F*> hSFeff_; if (region!=1 && region!=2) hSFeff_ = openSFFiles(Year, region);
  std::vector<TEfficiency*> eTrigEff_; if (region == 5) eTrigEff_ = photonTrigEffHist(Year);

  skimSamples skims = *skims_;
  typedef plot<RA2bTree> plot; vector<vector<plot> > plots;
  typedef bool(*cutFunc)(RA2bTree*);

  vector<cutFunc> cutFlow; vector<TString> cutName;
  cutFlow.push_back(*alwaysTrue<RA2bTree>);
  cutName.push_back("SkimCuts");
  cutFlow.push_back(*FiltersCutflow<RA2bTree>);
  cutName.push_back("Filters");
  cutFlow.push_back(*DeltaPhiCuts<RA2bTree>);
  cutName.push_back("DeltaPhiCuts");
  cutFlow.push_back(*METHTCut<RA2bTree>);
  cutName.push_back("METHT");
  cutFlow.push_back(*AK8MultCut<RA2bTree>);
  cutName.push_back("nAK8");
  cutFlow.push_back(*AK8JetPtCut<RA2bTree>);
  cutName.push_back("JetPt");
  cutFlow.push_back(*looseMassCut<RA2bTree>);
  cutName.push_back("LooseJetMass");
  cutFlow.push_back(*resVetoCutflow<RA2bTree>);
  cutName.push_back("ResVeto");
  cutFlow.push_back(*cutflowBoostBBTag<RA2bTree>);
  cutName.push_back("BB2H");
  cutFlow.push_back(*doubleMassCut<RA2bTree>);
  cutName.push_back("MassSR");


  const int numOpts = cutName.size();
  vector<TString> testingOpts = cutName;
  TDirectory* cdOpt[numOpts];

  for (int iCut = 0 ; iCut < cutFlow.size() ; iCut++) {
    vector<plot> tempPlots;
    tempPlots.push_back(plot(*fillMET<RA2bTree>,"MET_"+cutName[iCut],"MET [GeV]",20,300.,1300.));
    plots.push_back(tempPlots);
  }


  if (runVeto) {
    if (region==1 && !runFullSIM) readResVeto_Sig1D(yearStr,"TChiHH");
    else if (region==1 && runFullSIM) readResVeto_Sig1D(yearStr,"T5HH");
    else if (region==2) readResVeto_Sig2D(yearStr,"TChiHH");
    else if (region==0 && runData) readResVeto_Data(yearStr);
    else readResVeto_MC(yearStr);
  }


  // background MC samples
  for (int iSample = 0 ; iSample < skims.ntuples.size() ; iSample++) {
    RA2bTree* ntuple = skims.ntuples[iSample];
    for (int iCut = 0 ; iCut < plots.size() ; iCut++) {
      for (int iPlot = 0 ; iPlot < plots[iCut].size() ; iPlot++) {
        plots[iCut][iPlot].addNtuple(ntuple,skims.sampleName[iSample],true);
        plots[iCut][iPlot].dataHist=0;
      }
    }

    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
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
        TH1F *nEventsHisto = (TH1F*)fin->Get("nEventProc");
        TotalEvents = nEventsHisto->GetBinContent(1); //used to fix weight branch in signal
      }
    }

    for (int iEvt = 0 ; iEvt < numEvents ; iEvt++) {
      ntuple->GetEntry(iEvt);
      if (iEvt % 100000 == 0) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;
      TString thisFilename = ntuple->fChain->GetFile()->GetName();
      double weight = 1.0; double isrweight = 1.0; float trigWeight = 1.0;
      double puweight = 1.0; double prefireWeight = 1.0; float scalesSyst = 1.0;
      double BSFwt = 1.0; double SFweight = 1.0;
      float bb_tag = 1.0; float bbtag_FastSIM = 1.0; float sdMass_FastSIM = 1.0;
      //because I never re-ran skims, so at least only do necessary calcualtions once per event
      setHT(ntuple,"none"); setDeltaPhis(ntuple);
      setMET(ntuple); setJetPT(ntuple); setJetMass(ntuple);

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
        trigWeight = triggerEfficiencies(whichTrigRegion, Year, thisMET, thisHT, lept_pt, "none");
      }

      isrweight = ISRCorrections(ntuple,region,Year,"none");
      puweight = PUCorrections(ntuple,"none");
      prefireWeight = prefireCorr(ntuple,Year,"none");
      scalesSyst = scalesSystematic(ntuple,h_norm,"none"); //none returns 1.0
      weight = ntuple->Weight*this_lumi*trigWeight*isrweight*puweight*prefireWeight*SFweight*scalesSyst;

      if ( thisFilename.Contains("TChiHH_HToBB")||thisFilename.Contains("TChiHH") || (thisFilename.Contains("T5qqqqZH")||thisFilename.Contains("T5HH")) && !runFullSIM) weight = weight/TotalEvents*(0.5823329*0.5823329);
      else if ( (thisFilename.Contains("T5qqqqZH")||thisFilename.Contains("T5HH"))) weight = weight/TotalEvents*4; //for FullSIM

      //Re-weight 1D TChiHH to 2D
      if (region==2 && thisFilename.Contains("TChiHH") && !(thisFilename.Contains("2D"))) weight = weight/higgsinoCrossSection1D(NLSPmass)*higgsinoCrossSection2D(NLSPmass);

      sdMass_FastSIM = softdropmassFastSIMSFs(ntuple,"none");
      bb_tag = bbSFs(ntuple,"none");
      bbtag_FastSIM = bbFastSIMSFs(ntuple,"none");
      weight = weight*bb_tag*bbtag_FastSIM*sdMass_FastSIM;


      for (int iCut = 0 ; iCut < cutFlow.size() ; iCut++) {
        if (! cutFlow[iCut](ntuple)) break;
        for (int iPlot = 0 ; iPlot < plots[iCut].size() ; iPlot++) {
          plots[iCut][iPlot].fill(ntuple,weight);
        }
      }
    }
  }


  if (runData) {
    RA2bTree* ntuple = skims.dataNtuple;
    for (int iCut = 0 ; iCut < cutFlow.size() ; iCut++) {
      for (int iPlot = 0 ; iPlot < plots[iCut].size() ; iPlot++) {
        plots[iCut][iPlot].addDataNtuple(ntuple,"data");
      }
    }

    int numEvents = ntuple->fChain->GetEntries();
    ntupleBranchStatus<RA2bTree>(ntuple);
    for (int iEvt = 0 ; iEvt < numEvents; iEvt++) {
      ntuple->GetEntry(iEvt);
      if (iEvt % 100000 == 0) cout << "data: " << iEvt << "/" << numEvents << endl;

      if (region == 0 && !signalTriggerCut(ntuple)) continue;
      else if (region == 3 && !singleMuTriggerCut(ntuple)) continue;
      else if (region == 4 && !singleEleTriggerCut(ntuple)) continue;
      else if (region == 5 && !photonTriggerCut(ntuple)) continue;

      for (int iCut = 0 ; iCut < cutFlow.size() ; iCut++) {
        if (! cutFlow[iCut](ntuple)) break;
        for (int iPlot = 0 ; iPlot < plots[iCut].size() ; iPlot++) {
          plots[iCut][iPlot].fillData(ntuple);
        }
      }
    }
  }

  outputFile->cd();
  for (int iCut = 0 ; iCut < cutFlow.size() ; iCut++) {
    cdOpt[iCut] = outputFile->mkdir(testingOpts[iCut]);
    cdOpt[iCut]->cd();
    for (int iPlot = 0 ; iPlot < plots[iCut].size() ; iPlot++) {
      plots[iCut][iPlot].Write(runData);
    }
  }

  cout << "Cut";
  if (region!=1 &&region!=2) cout << " & All Bkg. ";
  for (int iSample = 0; iSample < skims.ntuples.size(); iSample++){
    cout << " & " << skims.sampleName[iSample];
  }
  if (region!=0 && region!=1 &&region!=2)  cout << " & Data"<<endl;
  cout << endl;
  cout << "% ------------------------------------------------------------------" << endl;
  for (int iCut = 0; iCut < cutFlow.size(); iCut++){
    plots[iCut][0].buildSum();
    cout << cutName[iCut];
    if (region!=1 &&region!=2) cout << " & " << std::setprecision(10) << plots[iCut][0].sum->Integral(0,21);
    for (int iSample = 0; iSample < skims.ntuples.size(); iSample++) {
      RA2bTree* ntuple = skims.ntuples[iSample];
      cout << " & " << std::setprecision(10) << plots[iCut][0].histoMap[ntuple]->Integral(0,21);
    }
    if (region!=0 && region!=1 &&region!=2) cout << " & " << std::setprecision(15) << plots[iCut][0].dataHist->Integral(0,21);
    cout << " \\\\ \\hline" << endl;
  }
}
