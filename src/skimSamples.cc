// ROOT/custom libraries
#include "TChain.h"
#include "RA2bTree.cc"
#include "TString.h"

// STL libraries
#include <iostream>
#include <vector>

//static const TString BASE_DIR="root://cmseos.fnal.gov//store/user/rgp230/SUSYRA2Higgsino/Skims/Run2ProductionV12/";
//static const TString BASE_DIR="/nfs/data37/cms/emacdonald/WorkingArea/CombinedHiggs/CMSSW_8_1_0/src/boostedHiggsPlusMET/src/samples/";
static const TString BASE_DIR="/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV16/tree_signal/";


class skimSamples{

  public :

  TChain *WJets,*ZJets,*QCD,*SnglT,*TT,*GJets,*GJets0p4,*Other,*DY,*TTinc;
  TChain *T5HH750, *T5HH1000, *T5HH1100,*T5HH1200,*T5HH1300,*T5HH1400,*T5HH1500,*T5HH1600,*T5HH1700,*T5HH1800,*T5HH1900,*T5HH2000,*T5HH2100;
  TChain *T5qqqqZH750, *T5qqqqZH1000, *T5qqqqZH1100, *T5qqqqZH1200, *T5qqqqZH1300, *T5qqqqZH1400, *T5qqqqZH1500, *T5qqqqZH1600, *T5qqqqZH1700, *T5qqqqZH1800, *T5qqqqZH1900, *T5qqqqZH2000, *T5qqqqZH2100, *T5qqqqZH2200;
  TChain *TChiHH127, *TChiHH150, *TChiHH175,*TChiHH200,*TChiHH225, *TChiHH250, *TChiHH275, *TChiHH300, *TChiHH325,*TChiHH350, *TChiHH375, *TChiHH400, *TChiHH425, *TChiHH450,*TChiHH475,*TChiHH500, *TChiHH525, *TChiHH550,*TChiHH575, *TChiHH600, *TChiHH625, *TChiHH650, *TChiHH675, *TChiHH700,*TChiHH725, *TChiHH750,*TChiHH775, *TChiHH800, *TChiHH825, *TChiHH850, *TChiHH875, *TChiHH900, *TChiHH925, *TChiHH950, *TChiHH975, *TChiHH1000;
  TChain *data;
  std::vector<RA2bTree*> ntuples,signalNtuples;
  RA2bTree* dataNtuple;
  std::vector<TString> sampleName, signalSampleName;
  std::vector<TString> dataSampleName;
  std::vector<int> fillColor, lineColor, sigLineColor;

  enum region {kSignal,kPhoton,kSLm,kSLe,kLowDphi, kNumRegions};
  TString regionNames[kNumRegions]={"signal","photon","SLm","SLe","kLowDphi"};

  TString skimType;

  skimSamples(region r=kSignal){
    skimType="";
    if( r == kSignal ){
      skimType=BASE_DIR+"background/";
    }

    bool onCUBL = false;
    ///////////////////////////////////////////////////////////////////////
    // - - - - - - - - - - BACKGROUND INPUTS - - - - - - - - - - - - - - //
    ///////////////////////////////////////////////////////////////////////
    std::vector<TString> ZJetsFileNames;

    //If on CUBL:
    if (onCUBL){
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-100to200.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-200to400.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-400to600.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-600to800.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-800to1200.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-1200to2500.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-2500toInf.root");
    }


    //If on LPC:
    else {
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-100to200_MC2017.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-200to400_MC2017.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-400to600_MC2017.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-600to800_MC2017.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-800to1200_MC2017.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-1200to2500_MC2017.root");
      ZJetsFileNames.push_back("tree_ZJetsToNuNu_HT-2500toInf_MC2017.root");
    }


    ZJets = new TChain("tree");
    for( int i = 0 ; i < ZJetsFileNames.size() ; i++ ){
      ZJets->Add(skimType+"/"+ZJetsFileNames[i]);
    }
    if( r == kSignal || r == kLowDphi ){
      ntuples.push_back(new RA2bTree(ZJets));//Need this one
      sampleName.push_back("ZJets");
      fillColor.push_back(kGreen+1);
      lineColor.push_back(1);
    }

    std::vector<TString> WJetsFileNames;

    //If on CUBL:
    if (onCUBL){
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-100to200.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-200to400.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-400to600.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-600to800.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-800to1200.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-1200to2500.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-2500toInf.root");
    }

    //If on LPC:
    else {
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-100to200_MC2017.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-200to400_MC2017.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-400to600_MC2017.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-600to800_MC2017.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-800to1200_MC2017.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-1200to2500_MC2017.root");
      WJetsFileNames.push_back("tree_WJetsToLNu_HT-2500toInf_MC2017.root");
    }

    WJets = new TChain("tree");
    for( int i = 0 ; i < WJetsFileNames.size() ; i++ ){
      WJets->Add(skimType+"/"+WJetsFileNames[i]);
    }
    if( r == kSignal || r == kSLm || r == kSLe || r == kLowDphi ){
      ntuples.push_back(new RA2bTree(WJets));//Need this one
      sampleName.push_back("WJets");
      fillColor.push_back(kBlue);
      lineColor.push_back(1);
    }

    std::vector<TString> TTincFileNames;
    TTincFileNames.push_back("tree_TTJets.root");
    TTinc = new TChain("tree");
    for( int i = 0 ; i < TTincFileNames.size() ; i++ ){
      TTinc->Add(skimType+"/"+TTincFileNames[i]);
    }

    std::vector<TString> TTFileNames;

    //If on CUBL
    if (onCUBL){
      TTFileNames.push_back("tree_TTJets_HT-600to800.root");
      TTFileNames.push_back("tree_TTJets_HT-800to1200.root");
      TTFileNames.push_back("tree_TTJets_HT-1200to2500.root");
      TTFileNames.push_back("tree_TTJets_HT-2500toInf.root");
      TTFileNames.push_back("tree_TTJets_SingleLeptFromT.root");
      TTFileNames.push_back("tree_TTJets_SingleLeptFromTbar.root");
      TTFileNames.push_back("tree_TTJets_DiLept.root");
      TTFileNames.push_back("tree_TTTT.root");
      TTFileNames.push_back("tree_TTWJetsToLNu_fast.root");
      TTFileNames.push_back("tree_TTWJetsToQQ.root");
      TTFileNames.push_back("tree_TTZToLLNuNu.root");
      TTFileNames.push_back("tree_TTGJets_fast.root");
    }


    //If on LPC
    else {
      TTFileNames.push_back("tree_TTJets_HT-600to800_MC2017.root");
      TTFileNames.push_back("tree_TTJets_HT-800to1200_MC2017.root");
      TTFileNames.push_back("tree_TTJets_HT-1200to2500_MC2017.root");
      TTFileNames.push_back("tree_TTJets_HT-2500toInf_MC2017.root");
      TTFileNames.push_back("tree_TTJets_SingleLeptFromT_MC2017.root");
      TTFileNames.push_back("tree_TTJets_SingleLeptFromTbar_MC2017.root");
      TTFileNames.push_back("tree_TTJets_DiLept_MC2017.root");
      TTFileNames.push_back("tree_TTTT_MC2017.root");
      TTFileNames.push_back("tree_TTWJetsToLNu_MC2017.root");
      TTFileNames.push_back("tree_TTWJetsToQQ_MC2017.root");
      TTFileNames.push_back("tree_TTZToLLNuNu_MC2017.root");
      TTFileNames.push_back("tree_TTGJets_MC2017.root");
    }

    TT = new TChain("tree");
    for( int i = 0 ; i < TTFileNames.size() ; i++ ){
      TT->Add(skimType+"/"+TTFileNames[i]);
    }
    if( r == kSignal || r == kSLm || r == kSLe || r == kLowDphi ){
      ntuples.push_back(new RA2bTree(TT));//Need this one
      sampleName.push_back("TT");
      fillColor.push_back(kCyan);
      lineColor.push_back(kCyan);
    }

    if (onCUBL){
      TChiHH127 = new TChain("tree");
      TChiHH150 = new TChain("tree");
      TChiHH175 = new TChain("tree");
      TChiHH200 = new TChain("tree");
      TChiHH225 = new TChain("tree");
      TChiHH250 = new TChain("tree");
      TChiHH275 = new TChain("tree");
      TChiHH300 = new TChain("tree");
      TChiHH325 = new TChain("tree");
      TChiHH350 = new TChain("tree");
      TChiHH375 = new TChain("tree");
      TChiHH400 = new TChain("tree");
      TChiHH425 = new TChain("tree");
      TChiHH450 = new TChain("tree");
      TChiHH475 = new TChain("tree");
      TChiHH500 = new TChain("tree");
      TChiHH525 = new TChain("tree");
      TChiHH550 = new TChain("tree");
      TChiHH575 = new TChain("tree");
      TChiHH600 = new TChain("tree");
      TChiHH625 = new TChain("tree");
      TChiHH650 = new TChain("tree");
      TChiHH675 = new TChain("tree");
      TChiHH700 = new TChain("tree");
      TChiHH725 = new TChain("tree");
      TChiHH750 = new TChain("tree");
      TChiHH775 = new TChain("tree");
      TChiHH800 = new TChain("tree");
      TChiHH825 = new TChain("tree");
      TChiHH850 = new TChain("tree");
      TChiHH875 = new TChain("tree");
      TChiHH900 = new TChain("tree");
      TChiHH925 = new TChain("tree");
      TChiHH950 = new TChain("tree");
      TChiHH975 = new TChain("tree");
      TChiHH1000 = new TChain("tree");

      T5qqqqZH750 = new TChain("tree");
      T5qqqqZH1000 = new TChain("tree");
      T5qqqqZH1100 = new TChain("tree");
      T5qqqqZH1200 = new TChain("tree");
      T5qqqqZH1300 = new TChain("tree");
      T5qqqqZH1400 = new TChain("tree");
      T5qqqqZH1500 = new TChain("tree");
      T5qqqqZH1600 = new TChain("tree");
      T5qqqqZH1700 = new TChain("tree");
      T5qqqqZH1800 = new TChain("tree");
      T5qqqqZH1900 = new TChain("tree");
      T5qqqqZH2000 = new TChain("tree");
      T5qqqqZH2100 = new TChain("tree");
      T5qqqqZH2200 = new TChain("tree");
    }
    else {
      TChiHH700 = new TChain("tree");
      TChiHH1000 = new TChain("tree");
      T5qqqqZH1700 = new TChain("tree");
      T5qqqqZH2100 = new TChain("tree");
    }


    if( r == kSignal ){
      if (onCUBL){
        TChiHH127->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_127_1_fast.root");
        TChiHH150->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_150_1_fast.root");
        TChiHH175->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_175_1_fast.root");
        TChiHH200->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_200_1_fast.root");
        TChiHH225->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_225_1_fast.root");
        TChiHH250->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_250_1_fast.root");
        TChiHH275->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_275_1_fast.root");
        TChiHH300->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_300_1_fast.root");
        TChiHH325->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_325_1_fast.root");
        TChiHH350->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_350_1_fast.root");
        TChiHH375->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_375_1_fast.root");
        TChiHH400->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_400_1_fast.root");
        TChiHH425->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_425_1_fast.root");
        TChiHH450->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_450_1_fast.root");
        TChiHH475->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_475_1_fast.root");
        TChiHH500->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_500_1_fast.root");
        TChiHH525->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_525_1_fast.root");
        TChiHH550->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_550_1_fast.root");
        TChiHH575->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_575_1_fast.root");
        TChiHH600->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_600_1_fast.root");
        TChiHH625->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_625_1_fast.root");
        TChiHH650->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_650_1_fast.root");
        TChiHH675->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_675_1_fast.root");
        TChiHH700->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_700_1_fast.root");
        TChiHH725->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_725_1_fast.root");
        TChiHH750->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_750_1_fast.root");
        TChiHH775->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_775_1_fast.root");
        TChiHH800->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_800_1_fast.root");
        TChiHH825->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_825_1_fast.root");
        TChiHH850->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_850_1_fast.root");
        TChiHH875->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_875_1_fast.root");
        TChiHH900->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_900_1_fast.root");
        TChiHH925->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_925_1_fast.root");
        TChiHH950->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_950_1_fast.root");
        TChiHH975->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_975_1_fast.root");
        TChiHH1000->Add("samples/TChiHH/tree_TChiHH_HToBB_HToBB_1000_1_fast.root");


        T5qqqqZH750->Add("samples/StrongProduction/tree_T5qqqqZH_750.root");
        T5qqqqZH1000->Add("samples/StrongProduction/tree_T5qqqqZH_1000.root");
        T5qqqqZH1100->Add("samples/StrongProduction/tree_T5qqqqZH_1100.root");
        T5qqqqZH1200->Add("samples/StrongProduction/tree_T5qqqqZH_1200.root");
        T5qqqqZH1300->Add("samples/StrongProduction/tree_T5qqqqZH_1300.root");
        T5qqqqZH1400->Add("samples/StrongProduction/tree_T5qqqqZH_1400.root");
        T5qqqqZH1500->Add("samples/StrongProduction/tree_T5qqqqZH_1500.root");
        T5qqqqZH1600->Add("samples/StrongProduction/tree_T5qqqqZH_1600.root");
        T5qqqqZH1700->Add("samples/StrongProduction/tree_T5qqqqZH_1700.root");
        T5qqqqZH1800->Add("samples/StrongProduction/tree_T5qqqqZH_1800.root");
        T5qqqqZH1900->Add("samples/StrongProduction/tree_T5qqqqZH_1900.root");
        T5qqqqZH2000->Add("samples/StrongProduction/tree_T5qqqqZH_2000.root");
        T5qqqqZH2100->Add("samples/StrongProduction/tree_T5qqqqZH_2100.root");
        T5qqqqZH2200->Add("samples/StrongProduction/tree_T5qqqqZH_2200.root");


        ntuples.push_back(new RA2bTree(TChiHH127));
        ntuples.push_back(new RA2bTree(TChiHH150));
        ntuples.push_back(new RA2bTree(TChiHH175));
        ntuples.push_back(new RA2bTree(TChiHH200));
        ntuples.push_back(new RA2bTree(TChiHH225));
        ntuples.push_back(new RA2bTree(TChiHH250));
        ntuples.push_back(new RA2bTree(TChiHH275));
        ntuples.push_back(new RA2bTree(TChiHH300));
        ntuples.push_back(new RA2bTree(TChiHH325));
        ntuples.push_back(new RA2bTree(TChiHH350));
        ntuples.push_back(new RA2bTree(TChiHH375));
        ntuples.push_back(new RA2bTree(TChiHH400));
        ntuples.push_back(new RA2bTree(TChiHH425));
        ntuples.push_back(new RA2bTree(TChiHH450));
        ntuples.push_back(new RA2bTree(TChiHH475));
        ntuples.push_back(new RA2bTree(TChiHH500));
        ntuples.push_back(new RA2bTree(TChiHH525));
        ntuples.push_back(new RA2bTree(TChiHH550));
        ntuples.push_back(new RA2bTree(TChiHH575));
        ntuples.push_back(new RA2bTree(TChiHH600));
        ntuples.push_back(new RA2bTree(TChiHH625));
        ntuples.push_back(new RA2bTree(TChiHH650));
        ntuples.push_back(new RA2bTree(TChiHH675));
        ntuples.push_back(new RA2bTree(TChiHH700));
        ntuples.push_back(new RA2bTree(TChiHH725));
        ntuples.push_back(new RA2bTree(TChiHH750));
        ntuples.push_back(new RA2bTree(TChiHH775));
        ntuples.push_back(new RA2bTree(TChiHH800));
        ntuples.push_back(new RA2bTree(TChiHH825));
        ntuples.push_back(new RA2bTree(TChiHH850));
        ntuples.push_back(new RA2bTree(TChiHH875));
        ntuples.push_back(new RA2bTree(TChiHH900));
        ntuples.push_back(new RA2bTree(TChiHH925));
        ntuples.push_back(new RA2bTree(TChiHH950));
        ntuples.push_back(new RA2bTree(TChiHH975));
        ntuples.push_back(new RA2bTree(TChiHH1000));


        ntuples.push_back(new RA2bTree(T5qqqqZH750));
        ntuples.push_back(new RA2bTree(T5qqqqZH1000));
        ntuples.push_back(new RA2bTree(T5qqqqZH1100));
        ntuples.push_back(new RA2bTree(T5qqqqZH1200));
        ntuples.push_back(new RA2bTree(T5qqqqZH1300));
        ntuples.push_back(new RA2bTree(T5qqqqZH1400));
        ntuples.push_back(new RA2bTree(T5qqqqZH1500));
        ntuples.push_back(new RA2bTree(T5qqqqZH1600));
        ntuples.push_back(new RA2bTree(T5qqqqZH1700));
        ntuples.push_back(new RA2bTree(T5qqqqZH1800));
        ntuples.push_back(new RA2bTree(T5qqqqZH1900));
        ntuples.push_back(new RA2bTree(T5qqqqZH2000));
        ntuples.push_back(new RA2bTree(T5qqqqZH2100));
        ntuples.push_back(new RA2bTree(T5qqqqZH2200));


        sampleName.push_back("TChiHH127");
        sampleName.push_back("TChiHH150");
        sampleName.push_back("TChiHH175");
        sampleName.push_back("TChiHH200");
        sampleName.push_back("TChiHH225");
        sampleName.push_back("TChiHH250");
        sampleName.push_back("TChiHH275");
        sampleName.push_back("TChiHH300");
        sampleName.push_back("TChiHH325");
        sampleName.push_back("TChiHH350");
        sampleName.push_back("TChiHH375");
        sampleName.push_back("TChiHH400");
        sampleName.push_back("TChiHH425");
        sampleName.push_back("TChiHH450");
        sampleName.push_back("TChiHH475");
        sampleName.push_back("TChiHH500");
        sampleName.push_back("TChiHH525");
        sampleName.push_back("TChiHH550");
        sampleName.push_back("TChiHH575");
        sampleName.push_back("TChiHH600");
        sampleName.push_back("TChiHH625");
        sampleName.push_back("TChiHH650");
        sampleName.push_back("TChiHH675");
        sampleName.push_back("TChiHH700");
        sampleName.push_back("TChiHH725");
        sampleName.push_back("TChiHH750");
        sampleName.push_back("TChiHH775");
        sampleName.push_back("TChiHH800");
        sampleName.push_back("TChiHH825");
        sampleName.push_back("TChiHH850");
        sampleName.push_back("TChiHH875");
        sampleName.push_back("TChiHH900");
        sampleName.push_back("TChiHH925");
        sampleName.push_back("TChiHH950");
        sampleName.push_back("TChiHH975");
        sampleName.push_back("TChiHH1000");

        sampleName.push_back("T5qqqqZH750");
        sampleName.push_back("T5qqqqZH1000");
        sampleName.push_back("T5qqqqZH1100");
        sampleName.push_back("T5qqqqZH1200");
        sampleName.push_back("T5qqqqZH1300");
        sampleName.push_back("T5qqqqZH1400");
        sampleName.push_back("T5qqqqZH1500");
        sampleName.push_back("T5qqqqZH1600");
        sampleName.push_back("T5qqqqZH1700");
        sampleName.push_back("T5qqqqZH1800");
        sampleName.push_back("T5qqqqZH1900");
        sampleName.push_back("T5qqqqZH2000");
        sampleName.push_back("T5qqqqZH2100");
        sampleName.push_back("T5qqqqZH2200");
      }
      else {
        TChiHH700->Add("signal/tree_TChiHH_HToBB_HToBB_700_1_fast.root");
        TChiHH1000->Add("signal/tree_TChiHH_HToBB_HToBB_1000_1_fast.root");
        T5qqqqZH1700->Add("signal/tree_T5qqqqZH_1700.root");
        T5qqqqZH2100->Add("signal/tree_T5qqqqZH_2100.root");

        ntuples.push_back(new RA2bTree(TChiHH700));
        ntuples.push_back(new RA2bTree(TChiHH1000));
        ntuples.push_back(new RA2bTree(T5qqqqZH1700));
        ntuples.push_back(new RA2bTree(T5qqqqZH2100));

        sampleName.push_back("TChiHH700");
        sampleName.push_back("TChiHH1000");
        sampleName.push_back("T5qqqqZH1700");
        sampleName.push_back("T5qqqqZH2100");
      }
      for(unsigned int i=0; i<sampleName.size(); ++i)sigLineColor.push_back(kRed);
    }

    std::vector<TString> QCDFileNames;

    //If on CUBL
    if (onCUBL){
      QCDFileNames.push_back("tree_QCD_HT-200to300.root");
      QCDFileNames.push_back("tree_QCD_HT-300to500.root");
      QCDFileNames.push_back("tree_QCD_HT-500to700.root");
      QCDFileNames.push_back("tree_QCD_HT-700to1000.root");
      QCDFileNames.push_back("tree_QCD_HT-1000to1500.root");
      QCDFileNames.push_back("tree_QCD_HT-1500to2000.root");
      QCDFileNames.push_back("tree_QCD_HT-2000toInf.root");
    }


    //If on LPC
    else {
      QCDFileNames.push_back("tree_QCD_HT-200to300_MC2017.root");
      QCDFileNames.push_back("tree_QCD_HT-300to500_MC2017.root");
      QCDFileNames.push_back("tree_QCD_HT-500to700_MC2017.root");
      QCDFileNames.push_back("tree_QCD_HT-700to1000_MC2017.root");
      QCDFileNames.push_back("tree_QCD_HT-1000to1500_MC2017.root");
      QCDFileNames.push_back("tree_QCD_HT-1500to2000_MC2017.root");
      QCDFileNames.push_back("tree_QCD_HT-2000toInf_MC2017.root");
    }

    QCD = new TChain("tree");

    for( int i = 0 ; i < QCDFileNames.size() ; i++ ){
      QCD->Add(skimType+"/"+QCDFileNames[i]);
    }
    if( r == kSignal || r == kPhoton || r == kLowDphi ){
      ntuples.push_back(new RA2bTree(QCD));//Need this one
      sampleName.push_back("QCD");
      fillColor.push_back(kGray);
      lineColor.push_back(1);
    }
  };

  RA2bTree* findNtuple(TString name){
    for( int iSam = 0 ; iSam < sampleName.size() ; iSam++ ){
      if( sampleName[iSam] == name )
      return ntuples[iSam] ;
    }
    for( int iSam = 0 ; iSam < signalSampleName.size() ; iSam++ ){
      if( signalSampleName[iSam] == name )
      return signalNtuples[iSam] ;
    }
    return NULL;
  };

};
