#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPave.h"
#include "TString.h"
#include "tdrstyle.C"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void test(string var="", string cuts="", string path="./") {
  setTDRStyle();
  if(!TString(path).EndsWith("/")) path+="/";
  TFile* inf = TFile::Open((path+"test_output.root").c_str(),"READ");
  vector<string> processes = {"ZJets","WJets","TT","QCD", "TChiHH127", "TChiHH150", "TChiHH175","TChiHH200","TChiHH225", "TChiHH250", "TChiHH275", "TChiHH300", "TChiHH325","TChiHH350", "TChiHH375", "TChiHH400", "TChiHH425", "TChiHH450","TChiHH475","TChiHH500", "TChiHH525", "TChiHH550","TChiHH575", "TChiHH600", "TChiHH625", "TChiHH650", "TChiHH675", "TChiHH700","TChiHH725", "TChiHH750","TChiHH775", "TChiHH800", "TChiHH825", "TChiHH850", "TChiHH875", "TChiHH900", "TChiHH925", "TChiHH950", "TChiHH975", "TChiHH1000", "T5qqqqZH750", "T5qqqqZH1000", "T5qqqqZH1100", "T5qqqqZH1200", "T5qqqqZH1300", "T5qqqqZH1400", "T5qqqqZH1500", "T5qqqqZH1600", "T5qqqqZH1700", "T5qqqqZH1800", "T5qqqqZH1900", "T5qqqqZH2000", "T5qqqqZH2100", "T5qqqqZH2200"};
  //vector<string> processes = {"ZJets","WJets","TT","QCD", "TChiHH225","TChiHH400","TChiHH700"}; //for plots

  vector<int> colors = {kOrange+1,kGreen+2,kBlue-2,kYellow+2};
  //vector<int> signal_colors = {kGreen,kRed,kCyan};
  vector<TTree*> vtree;
  vector<TH1F*> vhist;
  THStack* stack = new THStack((string("stack_")+var).c_str(),(string("stack_")+var).c_str());
  string direct = "";
  if(cuts=="boost") direct = "Boosted/";
  else if(cuts=="AK8") direct = "Boosted_AK8/";
  else if(cuts=="com") direct =  "Combined/";
  else if(cuts=="boost1") direct =  "OneBoosted/";
  else if(cuts=="4b") direct =  "Resolved4b/";
  else if(cuts=="3b") direct =  "Resolved3b/";
  else direct = "ForPlots/";

  if(var=="doubleB") direct = "DoubleBPlots/";

  vector<string> nEvents_signal = {"84454", "106406", "99099", "102285", "99208", "91650", "98101", "92229", "109068", "90086", "91969", "106716", "100323", "87694", "102881", "94578", "89209", "98000", "102377", "100218", "100554", "97914", "100716", "99027", "99564", "98288", "103443", "101643", "87285", "91713", "96388", "104283", "110373", "104522","97574", "101476"};
  //vector<string> nEvents_signal = {"99208","106716","99027"}; // for plots

  string baseCuts = "(Weight*39500)*(";
  string baseCutsT5qHH = "(Weight*39500*4)*(nGenHs==2 &&"; //because weight is divided by numEvents, which includes the 2HZ + 1ZZ cases
  string stdCuts = "";

  if(cuts=="boost")      stdCuts = "nBoostedH>=2 && MET>300 && HT>600)";
  else if (cuts=="AK8")  stdCuts = "nBoostedH==1 && nAK8_belowB>0 && MET>300 && HT>600)";
  else if(cuts=="com")   stdCuts = "nBoostedH>=1 && nResolvedH==1 && MET>300 && HT>350 && resolvedHCan_mass>100 && resolvedHCan_mass<=140)";
  else if(cuts=="boost1")stdCuts = "nBoostedH>=1 && nResolvedH==0 && nIsoBNotResolved>0 && MET>300 && HT>350)";
  else if(cuts=="4b")    stdCuts = "nAK4Cuts>=4 && nAK4Cuts<6 && HiggsCan_MassDiff<40 && deltaR_max<2.2 && HiggsCan_AvgMass>100 && HiggsCan_AvgMass<=140 && ((BTagsT>=2 && BTagsM>=1 && BTagsL>=1)||(BTagsT>=2 && BTagsM>=2)||(BTagsT>=3 && BTagsL>=1)||(BTagsT>=3 && BTagsM>=1)||(BTagsT>=4)))";
  else if(cuts=="3b")    stdCuts = "nAK4Cuts>=4 && nAK4Cuts<6 && HiggsCan_MassDiff<40 && deltaR_max<2.2 && HiggsCan_AvgMass>100 && HiggsCan_AvgMass<=140 && BTagsT>=2 && BTags==3)";
  else stdCuts = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2 && deltaR_max<2.2 && HiggsCan_MassDiff<40)";

  int counter = 0;
  int signal_counter = 0;
  TLegend *legend = new TLegend(0.6,0.6,0.9,0.9);
  for(auto p : processes) {
    vtree.push_back((TTree*)inf->Get(p.c_str()));
    vtree.back()->SetBranchStatus("*",0);
    vtree.back()->SetBranchStatus("MET",1);
    vtree.back()->SetBranchStatus("HT",1);
    vtree.back()->SetBranchStatus("Weight",1);
    vtree.back()->SetBranchStatus("nAK4Cuts",1);
    vtree.back()->SetBranchStatus("nIsoBNotResolved",1);
    vtree.back()->SetBranchStatus("nAK8_belowB",1);
    vtree.back()->SetBranchStatus("BTagsT",1);
    vtree.back()->SetBranchStatus("BTagsM",1);
    vtree.back()->SetBranchStatus("BTagsL",1);
    vtree.back()->SetBranchStatus("BTags",1);
    vtree.back()->SetBranchStatus("nGenHs",1);
    vtree.back()->SetBranchStatus("nBoostedH",1);
    vtree.back()->SetBranchStatus("nResolvedH",1);
    vtree.back()->SetBranchStatus("nNotIsoResolvedH",1);
    vtree.back()->SetBranchStatus("HiggsCan_MassDiff",1);
    vtree.back()->SetBranchStatus("HiggsCan_AvgMass",1);
    vtree.back()->SetBranchStatus("resolvedHCan_mass",1);
    vtree.back()->SetBranchStatus("boostedHCan_mass",1);
    vtree.back()->SetBranchStatus("AK8_doubleBDiscriminator",1);
    vtree.back()->SetBranchStatus("deltaR_max",1);

    string name = p+"_"+var;
    //if(var=="HiggsCan_AvgMass") vhist.push_back(new TH1F(name.c_str(),name.c_str(),20,0,200));
    //else if(var=="HiggsCan_MassDiff") vhist.push_back(new TH1F(name.c_str(),name.c_str(),15,0,120));
    if(cuts=="3b" || cuts=="4b") vhist.push_back(new TH1F(name.c_str(),name.c_str(),9,150,600));
    else vhist.push_back(new TH1F(name.c_str(),name.c_str(),3,300,900));
    //else if(var=="deltaR_max") vhist.push_back(new TH1F(name.c_str(),name.c_str(),20,0,4));
    //else if(var=="doubleB") vhist.push_back(new TH1F(name.c_str(),name.c_str(),80,-1.2,1.2));
    //else vhist.push_back(new TH1F(name.c_str(),name.c_str(),20,0,100));

    string savename = direct+name+".root";

    //Drawing double b discriminator for all background and signal
    /*if(var=="doubleB"){
      vtree.back()->Draw(("AK8_doubleBDiscriminator>>"+name).c_str(),"","goff");
      vhist.back()->SaveAs((savename).c_str());
      continue;
    }*/

    if(p.find("TChiHH")!=string::npos) {
      string nEvents = nEvents_signal[signal_counter];
      vtree.back()->Draw((var+">>"+name).c_str(),(baseCuts+stdCuts+"*TMath::Power(0.55,2)/"+nEvents).c_str(),"goff");
      //vhist.back()->SetLineStyle(2);
      //vhist.back()->SetLineWidth(2);
      //vhist.back()->SetLineColor(signal_colors[signal_counter]);
      //legend->AddEntry(vhist.back(),p.c_str(),"l");//for stack plot
      //vhist.back()->SaveAs((savename).c_str());
      //cout << name<< ": "<<vhist.back()->Integral(1,vhist.back()->GetNbinsX()+1)<<endl;
      cout<< p <<": "<<vhist.back()->GetBinContent(1)<<", "<<vhist.back()->GetBinContent(2)<<", "<<vhist.back()->GetBinContent(3)+vhist.back()->GetBinContent(4)<<endl;
      signal_counter++;
      continue;
    }

    if(p.find("T5qqqqZH")!=string::npos) {
      vtree.back()->Draw((var+">>"+name).c_str(),(baseCutsT5qHH+stdCuts).c_str(),"goff");
      //vhist.back()->SetLineStyle(2);
      //vhist.back()->SetLineWidth(2);
      //legend->AddEntry(vhist.back(),p.c_str(),"l");//for stack plot
      //vhist.back()->SaveAs((savename).c_str());
      //cout << name<< ": "<<vhist.back()->Integral(1,vhist.back()->GetNbinsX()+1)<<endl;
      cout<< p <<": "<<vhist.back()->GetBinContent(1)<<", "<<vhist.back()->GetBinContent(2)<<", "<<vhist.back()->GetBinContent(3)+vhist.back()->GetBinContent(4)<<endl;
      continue;
    }
    vtree.back()->Draw((var+">>"+name).c_str(),(baseCuts+stdCuts).c_str(),"goff");
    //vhist.back()->SetLineColor(colors[counter]);
    //vhist.back()->SetFillColor(colors[counter]);
    //vhist.back()->SaveAs((savename).c_str());
    //legend->AddEntry(vhist.back(),p.c_str(),"f");
    //stack->Add(vhist.back());
    counter++;
    //cout << name<< ": "<<vhist.back()->Integral(1,vhist.back()->GetNbinsX()+1)<<endl;
    cout<< p <<": "<<vhist.back()->GetBinContent(1)<<", "<<vhist.back()->GetBinContent(2)<<", "<<vhist.back()->GetBinContent(3)+vhist.back()->GetBinContent(4)<<endl;
  } //end loop through processes

  /*if(var!="doubleB"){
    TCanvas *c1 = new TCanvas("c1","stacked hists",10,10,1000,800);
    stack->Draw("hist");
    cout<<"hist size "<<vhist.size()<<endl;
    //vhist[6]->Draw("h same");
    //vhist[5]->Draw("h same");
    //vhist[4]->Draw("h same");
    float ymax = stack->GetMaximum();
    stack->SetMaximum(ymax);
    stack->SetMinimum(1);
    stack->GetYaxis()->SetTitle("Events");
    string savename = direct+var+"_stack.root";


    if(var=="HiggsCan_AvgMass") {
      stack->GetXaxis()->SetTitle("<m> [GeV]");
      TLine *line1 = new TLine(100,0,100,ymax);
      line1->SetLineColor(kBlack);
      line1->SetLineStyle(3);
      line1->Draw();
      TLine *line2 = new TLine(140,0,140,ymax);
      line2->SetLineColor(kBlack);
      line2->SetLineStyle(3);
      line2->Draw();
    }
    else if(var=="HiggsCan_MassDiff") {
      stack->GetXaxis()->SetTitle("|#Deltam| [GeV]");
      TLine *line = new TLine(40,0,40,ymax);
      line->SetLineColor(kBlack);
      line->SetLineStyle(3);
      line->Draw();
    }
    else if(var=="MET") {
      stack->GetXaxis()->SetTitle("p_{T}^{miss} [GeV]");
      c1->SetLogy();
    }
    else if(var=="deltaR_max") {
      stack->GetXaxis()->SetTitle("#DeltaR_{max}");
      TLine *line = new TLine(2.2,0,2.2,ymax);
      line->SetLineColor(kBlack);
      line->SetLineStyle(3);
      line->Draw();
    }
    else {
      stack->GetXaxis()->SetTitle(var.c_str());
    }

    legend->Draw("same");
    c1->Modified();
    //c1->SaveAs((savename).c_str());
  }*/
} //end test


void cutflow(string process = "", string path="./") {
  setTDRStyle();
  if(!TString(path).EndsWith("/")) path+="/";
  TFile* inf = TFile::Open((path+"output.root").c_str(),"READ");
  string var = "MET";
  string stdCuts_base_TChi700 = "(Weight*39500*TMath::Power(0.55,2)/99027)*(";
  string stdCuts_base_TChi400 = "(Weight*39500*TMath::Power(0.55,2)/106716)*(";
  string stdCuts_base = "(Weight*39500)*(";
  string stdCuts_base_T5 = "(Weight*39500)*(nGenHs==2 &&";
  string stdCuts1 = "nAK4Cuts>=4 && nAK4Cuts<6)";
  string stdCuts2 = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2)";
  string stdCuts3 = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2 && HiggsCan_MassDiff<40)";
  string stdCuts4 = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2 && HiggsCan_MassDiff<40 && deltaR_max<2.2)";
  string stdCuts5 = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2 && HiggsCan_MassDiff<40 && deltaR_max<2.2 && HiggsCan_AvgMass>100 && HiggsCan_AvgMass<=140)";
  string stdCuts6 = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2 && HiggsCan_MassDiff<40 && deltaR_max<2.2 && HiggsCan_AvgMass>100 && HiggsCan_AvgMass<=140 && BTags>=3)";
  string stdCuts7a = "MET>300 && nBoostedH>=2 && HT>600)";
  //string stdCuts7a = "nBoostedH==1 && nResolvedH==1 && MET>400 && HT>400 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";
  //string stdCuts7b = "nBoostedH==1 && nResolvedH==1 && MET>450 && HT>400 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";
  string stdCuts7c = "nBoostedH==1 && nResolvedH==1 && MET>500 && HT>400 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";
  string stdCuts7d = "nBoostedH==1 && nResolvedH==1 && MET>450 && HT>550 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";
  string stdCuts7e = "nBoostedH==1 && nResolvedH==1 && MET>450 && HT>600 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";
  //string stdCuts7f = "nBoostedH==1 && nResolvedH==1 && MET>600 && HT>400 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";
  string stdCuts7 = "nBoostedH==1 && nResolvedH==1 && MET>700 && HT>400 && resolvedHCan_mass>100 && resolvedHCan_mass<=140 && boostedHCan_mass>=80 && boostedHCan_mass<=135 )";

  string stdCuts7b = "nAK4Cuts>=4 && nAK4Cuts<6 && BTagsT>=2 && HiggsCan_MassDiff<40 && deltaR_max<2.2 && HiggsCan_AvgMass>100 && HiggsCan_AvgMass<=140 && ((BTagsT>=2 && BTagsM>=1 && BTagsL>=1)||(BTagsT>=2 && BTagsM>=2)||(BTagsT>=3 && BTagsL>=1)||(BTagsT>=3 && BTagsM>=1)||(BTagsT>=4)) && nBoostedH==0)";
  //above is changing cuts for the boosted and combined cases

  if (process=="all") {
    vector<string> processes = {"ZJets","WJets","TT","QCD","TChiHH700","T5qqqqZH1700"}; //change for individual samples
    for(auto p : processes) {
      TTree* thistree = (TTree*)inf->Get(p.c_str());
      thistree->SetBranchStatus("*",0);
      //thistree->SetBranchStatus(var.c_str(),1);
      thistree->SetBranchStatus("MET",1);
      thistree->SetBranchStatus("HT",1);
      thistree->SetBranchStatus("Weight",1);
      thistree->SetBranchStatus("nBoostedH",1);
      thistree->SetBranchStatus("boostedHCan_mass",1);
      thistree->SetBranchStatus("resolvedHCan_mass",1);
      thistree->SetBranchStatus("nNotIsoResolvedH",1);
      thistree->SetBranchStatus("nResolvedH",1);
      thistree->SetBranchStatus("deltaMAK8",1);
      thistree->SetBranchStatus("nAK4Cuts",1);
      thistree->SetBranchStatus("BTagsT",1);
      thistree->SetBranchStatus("BTagsM",1);
      thistree->SetBranchStatus("BTagsL",1);
      thistree->SetBranchStatus("BTags",1);
      thistree->SetBranchStatus("nGenHs",1);
      thistree->SetBranchStatus("HiggsCan_MassDiff",1);
      thistree->SetBranchStatus("HiggsCan_AvgMass",1);
      thistree->SetBranchStatus("deltaR_max",1);

      string name = p+"_"+var;
      cout<<p.c_str()<<endl;
      if(p.find("TChiHH")!=string::npos) {
        /*thistree->Draw("MET>>thishist(9,150,600)",(stdCuts_base_TChi700).c_str(),"goff");
        TH1F *thishist = (TH1F*)gDirectory->Get("thishist");
        cout << "0l, delta phi, track veto, MET > 150 "<< ": "<<thishist->Integral(1,thishist->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist2(9,150,600)",(stdCuts_base_TChi700+stdCuts1).c_str(),"goff");
        TH1F *thishist2 = (TH1F*)gDirectory->Get("thishist2");
        cout << "4<N_jets<=6"<< ": "<<thishist2->Integral(1,thishist2->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist3(9,150,600)",(stdCuts_base_TChi700+stdCuts2).c_str(),"goff");
        TH1F *thishist3 = (TH1F*)gDirectory->Get("thishist3");
        cout << "N_{b,T} >= 2"<< ": "<<thishist3->Integral(1,thishist3->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist4(9,150,600)",(stdCuts_base_TChi700+stdCuts3).c_str(),"goff");
        TH1F *thishist4 = (TH1F*)gDirectory->Get("thishist4");
        cout << "|#Deltam|<40"<< ": "<<thishist4->Integral(1,thishist4->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist5(9,150,600)",(stdCuts_base_TChi700+stdCuts4).c_str(),"goff");
        TH1F *thishist5 = (TH1F*)gDirectory->Get("thishist5");
        cout << "#DeltaR_{max} < 2.2"<< ": "<<thishist5->Integral(1,thishist5->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist6(9,150,600)",(stdCuts_base_TChi700+stdCuts5).c_str(),"goff");
        TH1F *thishist6 = (TH1F*)gDirectory->Get("thishist6");
        cout << "100 < <m> <= 140"<< ": "<<thishist6->Integral(1,thishist6->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist7(9,150,600)",(stdCuts_base_TChi700+stdCuts6).c_str(),"goff");
        TH1F *thishist7 = (TH1F*)gDirectory->Get("thishist7");
        cout << "3b and 4b"<< ": "<<thishist7->Integral(1,thishist7->GetNbinsX()+1)<<endl;*/

        thistree->Draw("MET>>thishista(5,350,850)",(stdCuts_base_TChi700+stdCuts7a).c_str(),"goff");
        TH1F *thishista = (TH1F*)gDirectory->Get("thishista");
        cout << "MET>450, HT>400: "<<thishista->Integral(1,thishista->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishistb(9,150,600)",(stdCuts_base_TChi700+stdCuts7b).c_str(),"goff");
        TH1F *thishistb = (TH1F*)gDirectory->Get("thishistb");
        cout << "MET>450, HT>450: "<<thishistb->Integral(1,thishistb->GetNbinsX()+1)<<endl;

        /*thistree->Draw("MET>>thishistc(9,150,600)",(stdCuts_base_TChi700+stdCuts7c).c_str(),"goff");
        TH1F *thishistc = (TH1F*)gDirectory->Get("thishistc");
        cout << "MET>450, HT>500: "<<thishistc->Integral(1,thishistc->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishistd(9,150,600)",(stdCuts_base_TChi700+stdCuts7d).c_str(),"goff");
        TH1F *thishistd = (TH1F*)gDirectory->Get("thishistd");
        cout << "MET>450, HT>550: "<<thishistd->Integral(1,thishistd->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishiste(9,150,600)",(stdCuts_base_TChi700+stdCuts7e).c_str(),"goff");
        TH1F *thishiste = (TH1F*)gDirectory->Get("thishiste");
        cout << "MET>450, HT>600: "<<thishiste->Integral(1,thishiste->GetNbinsX()+1)<<endl;*/

        //thistree->Draw("MET>>thishistf(9,150,600)",(stdCuts_base_TChi700+stdCuts7f).c_str(),"goff");
        //TH1F *thishistf = (TH1F*)gDirectory->Get("thishistf");
        //cout << "MET>600, HT>400: "<<thishistf->Integral(1,thishistf->GetNbinsX()+1)<<endl;

        //thistree->Draw("MET>>thishistg(9,150,600)",(stdCuts_base_TChi700+stdCuts7g).c_str(),"goff");
        //TH1F *thishistg = (TH1F*)gDirectory->Get("thishistg");
        //cout << "MET>700, HT>400: "<<thishistg->Integral(1,thishistg->GetNbinsX()+1)<<endl;
      }

      else if(p.find("T5qqqq")!=string::npos) {
        thistree->Draw("MET>>thishista(5,350,850)",(stdCuts_base_T5+stdCuts7a).c_str(),"goff");
        TH1F *thishista = (TH1F*)gDirectory->Get("thishista");
        cout << "MET>450, HT>400: "<<thishista->Integral(1,thishista->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishistb(9,150,600)",(stdCuts_base_T5+stdCuts7b).c_str(),"goff");
        TH1F *thishistb = (TH1F*)gDirectory->Get("thishistb");
        cout << "MET>450, HT>450: "<<thishistb->Integral(1,thishistb->GetNbinsX()+1)<<endl;

        /*thistree->Draw("MET>>thishistc(9,150,600)",(stdCuts_base_T5+stdCuts7c).c_str(),"goff");
        TH1F *thishistc = (TH1F*)gDirectory->Get("thishistc");
        cout << "MET>450, HT>500: "<<thishistc->Integral(1,thishistc->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishistd(9,150,600)",(stdCuts_base_T5+stdCuts7d).c_str(),"goff");
        TH1F *thishistd = (TH1F*)gDirectory->Get("thishistd");
        cout << "MET>450, HT>550: "<<thishistd->Integral(1,thishistd->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishiste(9,150,600)",(stdCuts_base_T5+stdCuts7e).c_str(),"goff");
        TH1F *thishiste = (TH1F*)gDirectory->Get("thishiste");
        cout << "MET>450, HT>600: "<<thishiste->Integral(1,thishiste->GetNbinsX()+1)<<endl;*/

        //thistree->Draw("MET>>thishistf(9,150,600)",(stdCuts_base_T5+stdCuts7f).c_str(),"goff");
        //TH1F *thishistf = (TH1F*)gDirectory->Get("thishistf");
        //cout << "MET>600, HT>400: "<<thishistf->Integral(1,thishistf->GetNbinsX()+1)<<endl;

        //thistree->Draw("MET>>thishistg(9,150,600)",(stdCuts_base_T5+stdCuts7g).c_str(),"goff");
        //TH1F *thishistg = (TH1F*)gDirectory->Get("thishistg");
        //cout << "MET>700, HT>400: "<<thishistg->Integral(1,thishistg->GetNbinsX()+1)<<endl;
      }

      else {
        /*thistree->Draw("MET>>thishist(9,150,600)",(stdCuts_base).c_str(),"goff");
        TH1F *thishist = (TH1F*)gDirectory->Get("thishist");
        cout << "0l, delta phi, track veto, MET > 150 "<< ": "<<thishist->Integral(1,thishist->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist2(9,150,600)",(stdCuts_base+stdCuts1).c_str(),"goff");
        TH1F *thishist2 = (TH1F*)gDirectory->Get("thishist2");
        cout << "4<N_jets<=6"<< ": "<<thishist2->Integral(1,thishist2->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist3(9,150,600)",(stdCuts_base+stdCuts2).c_str(),"goff");
        TH1F *thishist3 = (TH1F*)gDirectory->Get("thishist3");
        cout << "N_{b,T} >= 2"<< ": "<<thishist3->Integral(1,thishist3->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist4(9,150,600)",(stdCuts_base+stdCuts3).c_str(),"goff");
        TH1F *thishist4 = (TH1F*)gDirectory->Get("thishist4");
        cout << "|#Deltam|<40"<< ": "<<thishist4->Integral(1,thishist4->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist5(9,150,600)",(stdCuts_base+stdCuts4).c_str(),"goff");
        TH1F *thishist5 = (TH1F*)gDirectory->Get("thishist5");
        cout << "#DeltaR_{max} < 2.2"<< ": "<<thishist5->Integral(1,thishist5->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist6(9,150,600)",(stdCuts_base+stdCuts5).c_str(),"goff");
        TH1F *thishist6 = (TH1F*)gDirectory->Get("thishist6");
        cout << "100 < <m> <= 140"<< ": "<<thishist6->Integral(1,thishist6->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist7(9,150,600)",(stdCuts_base+stdCuts6).c_str(),"goff");
        TH1F *thishist7 = (TH1F*)gDirectory->Get("thishist7");
        cout << "3b and 4b"<< ": "<<thishist7->Integral(1,thishist7->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishist8(9,150,600)",(stdCuts_base+stdCuts7).c_str(),"goff");
        TH1F *thishist8 = (TH1F*)gDirectory->Get("thishist8");
        cout << "4b"<< ": "<<thishist8->Integral(1,thishist8->GetNbinsX()+1)<<endl;*/

        thistree->Draw("MET>>thishista(5,350,850)",(stdCuts_base+stdCuts7a).c_str(),"goff");
        TH1F *thishista = (TH1F*)gDirectory->Get("thishista");
        cout << "MET>450, HT>400: "<<thishista->Integral(1,thishista->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishistb(9,150,600)",(stdCuts_base+stdCuts7b).c_str(),"goff");
        TH1F *thishistb = (TH1F*)gDirectory->Get("thishistb");
        cout << "MET>450, HT>450: "<<thishistb->Integral(1,thishistb->GetNbinsX()+1)<<endl;

        /*thistree->Draw("MET>>thishistc(9,150,600)",(stdCuts_base+stdCuts7c).c_str(),"goff");
        TH1F *thishistc = (TH1F*)gDirectory->Get("thishistc");
        cout << "MET>450, HT>500: "<<thishistc->Integral(1,thishistc->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishistd(9,150,600)",(stdCuts_base+stdCuts7d).c_str(),"goff");
        TH1F *thishistd = (TH1F*)gDirectory->Get("thishistd");
        cout << "MET>450, HT>550: "<<thishistd->Integral(1,thishistd->GetNbinsX()+1)<<endl;

        thistree->Draw("MET>>thishiste(9,150,600)",(stdCuts_base+stdCuts7e).c_str(),"goff");
        TH1F *thishiste = (TH1F*)gDirectory->Get("thishiste");
        cout << "MET>450, HT>600: "<<thishiste->Integral(1,thishiste->GetNbinsX()+1)<<endl;*/

        //thistree->Draw("MET>>thishistf(9,150,600)",(stdCuts_base+stdCuts7f).c_str(),"goff");
        //TH1F *thishistf = (TH1F*)gDirectory->Get("thishistf");
        //cout << "MET>600, HT>400: "<<thishistf->Integral(1,thishistf->GetNbinsX()+1)<<endl;

        //thistree->Draw("MET>>thishistg(9,150,600)",(stdCuts_base+stdCuts7g).c_str(),"goff");
        //TH1F *thishistg = (TH1F*)gDirectory->Get("thishistg");
        //cout << "MET>700, HT>400: "<<thishistg->Integral(1,thishistg->GetNbinsX()+1)<<endl;

      } //end for "not sample"
    } //end loop over all processes
  } //end if statment over if processes == "all"
  else {
    string name = process+"_"+var;
    TTree* thistree = (TTree*)inf->Get(process.c_str());
    thistree->SetBranchStatus("*",0);
    //thistree->SetBranchStatus(var.c_str(),1);
    thistree->SetBranchStatus("MET",1);
    thistree->SetBranchStatus("HT",1);
    thistree->SetBranchStatus("Weight",1);
    thistree->SetBranchStatus("nBoostedH",1);
    thistree->SetBranchStatus("boostedHCan_mass",1);
    thistree->SetBranchStatus("resolvedHCan_mass",1);
    thistree->SetBranchStatus("nNotIsoResolvedH",1);
    thistree->SetBranchStatus("nResolvedH",1);
    thistree->SetBranchStatus("deltaMAK8",1);
    thistree->SetBranchStatus("nAK4Cuts",1);
    thistree->SetBranchStatus("BTagsT",1);
    thistree->SetBranchStatus("BTagsM",1);
    thistree->SetBranchStatus("BTagsL",1);
    thistree->SetBranchStatus("BTags",1);
    thistree->SetBranchStatus("HiggsCan_MassDiff",1);
    thistree->SetBranchStatus("HiggsCan_AvgMass",1);
    thistree->SetBranchStatus("deltaR_max",1);
    cout<<process.c_str()<<endl;



    if(process.find("TChiHH")!=string::npos) {
      /*  thistree->Draw("MET>>thishist(9,150,600)",(stdCuts_base_TChi700).c_str(),"goff");
      TH1F *thishist = (TH1F*)gDirectory->Get("thishist");
      cout << "0l, delta phi, track veto, MET > 150 "<< ": "<<thishist->Integral(1,thishist->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist2(9,150,600)",(stdCuts_base_TChi700+stdCuts1).c_str(),"goff");
      TH1F *thishist2 = (TH1F*)gDirectory->Get("thishist2");
      cout << "4<N_jets<=6"<< ": "<<thishist2->Integral(1,thishist2->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist3(9,150,600)",(stdCuts_base_TChi700+stdCuts2).c_str(),"goff");
      TH1F *thishist3 = (TH1F*)gDirectory->Get("thishist3");
      cout << "N_{b,T} >= 2"<< ": "<<thishist3->Integral(1,thishist3->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist4(9,150,600)",(stdCuts_base_TChi700+stdCuts3).c_str(),"goff");
      TH1F *thishist4 = (TH1F*)gDirectory->Get("thishist4");
      cout << "|#Deltam|<40"<< ": "<<thishist4->Integral(1,thishist4->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist5(9,150,600)",(stdCuts_base_TChi700+stdCuts4).c_str(),"goff");
      TH1F *thishist5 = (TH1F*)gDirectory->Get("thishist5");
      cout << "#DeltaR_{max} < 2.2"<< ": "<<thishist5->Integral(1,thishist5->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist6(9,150,600)",(stdCuts_base_TChi700+stdCuts5).c_str(),"goff");
      TH1F *thishist6 = (TH1F*)gDirectory->Get("thishist6");
      cout << "100 < <m> <= 140"<< ": "<<thishist6->Integral(1,thishist6->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist7(9,150,600)",(stdCuts_base_TChi700+stdCuts6).c_str(),"goff");
      TH1F *thishist7 = (TH1F*)gDirectory->Get("thishist7");
      cout << "3b and 4b"<< ": "<<thishist7->Integral(1,thishist7->GetNbinsX()+1)<<endl;*/

      //thistree->Draw("MET>>thishist8(9,150,600)",(stdCuts_base_TChi700+stdCuts7).c_str(),"goff");
      //TH1F *thishist8 = (TH1F*)gDirectory->Get("thishist8");
      //cout << "4b"<< ": "<<thishist8->Integral(1,thishist8->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishista(9,150,600)",(stdCuts_base_TChi400+stdCuts7a).c_str(),"goff");
      TH1F *thishista = (TH1F*)gDirectory->Get("thishista");
      cout << "MET>400, HT>400: "<<thishista->Integral(1,thishista->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishistb(9,150,600)",(stdCuts_base_TChi400+stdCuts7b).c_str(),"goff");
      TH1F *thishistb = (TH1F*)gDirectory->Get("thishistb");
      cout << "MET>450, HT>400: "<<thishistb->Integral(1,thishistb->GetNbinsX()+1)<<endl;

      //thistree->Draw("MET>>thishistc(5,350,850)",(stdCuts_base_TChi400+stdCuts7c).c_str(),"goff");
      //TH1F *thishistc = (TH1F*)gDirectory->Get("thishistc");
      //cout << "MET>500, HT>400: "<<thishistc->Integral(1,thishistc->GetNbinsX()+1)<<endl;
    }

    else{
      /*  thistree->Draw("MET>>thishist(9,150,600)",(stdCuts_base).c_str(),"goff");
      TH1F *thishist = (TH1F*)gDirectory->Get("thishist");
      cout << "0l, delta phi, track veto, MET > 150 "<< ": "<<thishist->Integral(1,thishist->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist2(9,150,600)",(stdCuts_base+stdCuts1).c_str(),"goff");
      TH1F *thishist2 = (TH1F*)gDirectory->Get("thishist2");
      cout << "4<N_jets<=6"<< ": "<<thishist2->Integral(1,thishist2->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist3(9,150,600)",(stdCuts_base+stdCuts2).c_str(),"goff");
      TH1F *thishist3 = (TH1F*)gDirectory->Get("thishist3");
      cout << "N_{b,T} >= 2"<< ": "<<thishist3->Integral(1,thishist3->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist4(9,150,600)",(stdCuts_base+stdCuts3).c_str(),"goff");
      TH1F *thishist4 = (TH1F*)gDirectory->Get("thishist4");
      cout << "|#Deltam|<40"<< ": "<<thishist4->Integral(1,thishist4->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist5(9,150,600)",(stdCuts_base+stdCuts4).c_str(),"goff");
      TH1F *thishist5 = (TH1F*)gDirectory->Get("thishist5");
      cout << "#DeltaR_{max} < 2.2"<< ": "<<thishist5->Integral(1,thishist5->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist6(9,150,600)",(stdCuts_base+stdCuts5).c_str(),"goff");
      TH1F *thishist6 = (TH1F*)gDirectory->Get("thishist6");
      cout << "100 < <m> <= 140"<< ": "<<thishist6->Integral(1,thishist6->GetNbinsX()+1)<<endl;

      thistree->Draw("MET>>thishist7(9,150,600)",(stdCuts_base+stdCuts6).c_str(),"goff");
      TH1F *thishist7 = (TH1F*)gDirectory->Get("thishist7");
      cout << "3b and 4b"<< ": "<<thishist7->Integral(1,thishist7->GetNbinsX()+1)<<endl;*/

      thistree->Draw("MET>>thishist8(9,150,600)",(stdCuts_base+stdCuts7).c_str(),"goff");
      TH1F *thishist8 = (TH1F*)gDirectory->Get("thishist8");
      cout << "4b"<< ": "<<thishist8->Integral(1,thishist8->GetNbinsX()+1)<<endl;

    }//end else not signal
  }


} //end method
