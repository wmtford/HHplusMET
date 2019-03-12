#include "TROOT.h"
#include "TSystem.h"
#include "TFile.h"
#include "TMath.h"
#include "TTree.h"
#include "TH1.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "THStack.h"
#include "TLegend.h"
#include "TLine.h"
#include "TPave.h"
#include "TString.h"
#include "tdrstyle.C"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "INeedThis.C"

using namespace std;

void MakeTable(string path="./") {
  setTDRStyle();
  if(!TString(path).EndsWith("/")) path+="/";
  TFile* inf = TFile::Open((path+"output.root").c_str(),"READ");

  bool save_files = true;
  bool save_stacks = true;
  bool make_table = true;

  //string which_process = "all";
  string which_process = "some";
  vector<string> processes = {"",""};
  vector<double> nEvents_signal = {0,0};
  float table_array[26][8];


  if (which_process=="all") {
    cout<<"Using all higgsino mass points + backgrounds"<<endl;
    processes = {"ZJets","WJets","TT","QCD", "TChiHH127", "TChiHH150", "TChiHH175","TChiHH200","TChiHH225", "TChiHH250", "TChiHH275", "TChiHH300", "TChiHH325","TChiHH350", "TChiHH375", "TChiHH400", "TChiHH425", "TChiHH450","TChiHH475","TChiHH500", "TChiHH525", "TChiHH550","TChiHH575", "TChiHH600", "TChiHH625", "TChiHH650", "TChiHH675", "TChiHH700","TChiHH725", "TChiHH750","TChiHH775", "TChiHH800", "TChiHH825", "TChiHH850", "TChiHH875", "TChiHH900", "TChiHH925", "TChiHH950", "TChiHH975", "TChiHH1000", "T5qqqqZH750", "T5qqqqZH1000", "T5qqqqZH1100", "T5qqqqZH1200", "T5qqqqZH1300", "T5qqqqZH1400", "T5qqqqZH1500", "T5qqqqZH1600", "T5qqqqZH1700", "T5qqqqZH1800", "T5qqqqZH1900", "T5qqqqZH2000", "T5qqqqZH2100", "T5qqqqZH2200"};
    nEvents_signal = {84454, 106406, 99099, 102285, 99208, 91650, 98101, 92229, 109068, 90086, 91969, 106716, 100323, 87694, 102881, 94578, 89209, 98000, 102377, 100218, 100554, 97914, 100716, 99027, 99564, 98288, 103443, 101643, 87285, 91713, 96388, 104283, 110373, 104522,97574, 101476};

  }
  else if (which_process=="some") {
    processes = {"TT","ZJets","WJets","QCD","TChiHH700","TChiHH1000","T5qqqqZH1700","T5qqqqZH2100"};
    nEvents_signal = {99027, 101476};

  }
  else {
    cout<<"Improper process! Use some or all "<<endl;
    return;
  }

  THStack* stack_Boost = new THStack("stack_Boost","stack_Boost");
  THStack* stack_DiAK8 = new THStack("stack_DiAK8","stack_DiAK8");
  THStack* stack_SemiResolved = new THStack("stack_SemiResolved","stack_SemiResolved");
  THStack* stack_SingleAK8 = new THStack("stack_SingleAK8","stack_SingleAK8");
  THStack* stack_Res4b = new THStack("stack_Res4b","stack_Res4b");
  THStack* stack_Res3b = new THStack("stack_Res3b","stack_Res3b");
  TLegend *legend = new TLegend(0.6,0.6,0.9,0.9);

  string var = "MET";
  string direct = "";
  vector<int> colors = {kOrange+1,kGreen+2,kBlue-2,kYellow+2};
  vector<TH1F*> dem_hists;
  int signal_counter = 0;
  int color_counter = 0;

  for(unsigned int p = 0; p< processes.size(); p++) {
    //double EventWeight_signal = 35900.0*TMath::Power(0.57,2)/nEvents_signal[signal_counter];
    //double EventWeight_T5HH = 35900.0*4.0;
    //double EventWeight = 35900.0;
    double EventWeight_signal = 150000.0*TMath::Power(0.57,2)/nEvents_signal[signal_counter];
    double EventWeight_T5HH = 150000.0*4.0;
    double EventWeight = 150000.0;
    cout<<"Beginning "<< processes[p].c_str()<<endl;
    TTree *tree;
    inf->GetObject(processes[p].c_str(),tree);
    INeedThis test;
    test.Init(tree);

    if (processes[p].find("TChiHH")!=string::npos) {
      cout<<"Right before going to HistoMaker"<<endl;
      dem_hists = test.Loop(EventWeight_signal, processes[p].c_str(), save_files);
      signal_counter++;
    }
    else if (processes[p].find("T5qqqq")!=string::npos) dem_hists = test.Loop(EventWeight_T5HH, processes[p].c_str(), save_files);

    else dem_hists = test.Loop(EventWeight, processes[p].c_str(), save_files);

    if ( (!(processes[p].find("TChiHH")!=string::npos) || !(processes[p].find("T5qqqqZH")!=string::npos)) && save_stacks){
      legend->AddEntry(dem_hists.back(),processes[p].c_str(),"f");//for stack plot
      for (unsigned int i=0; i<dem_hists.size();i++){
        dem_hists[i]->SetLineColor(colors[color_counter]);
        dem_hists[i]->SetFillColor(colors[color_counter]);
      }
      stack_Boost->Add(dem_hists[0]);
      stack_DiAK8->Add(dem_hists[1]);
      stack_SemiResolved->Add(dem_hists[2]);
      stack_SingleAK8->Add(dem_hists[3]);
      stack_Res4b->Add(dem_hists[4]);
      stack_Res3b->Add(dem_hists[5]);
    }

    table_array[0][p] = dem_hists[0]->Integral(1,dem_hists[0]->GetNbinsX()+1);
    for (int i=1; i<4; i++){
      float this_bin = dem_hists[0]->GetBinContent(i);
      float next_bin = dem_hists[0]->GetBinContent(i+1);
      if (this_bin<1e-20) this_bin=0;
      if (next_bin<1e-20) next_bin=0;
      table_array[i][p] = this_bin;
      if (i==3)table_array[i][p] = this_bin+next_bin;
    }
    table_array[4][p] = dem_hists[1]->Integral(1,dem_hists[1]->GetNbinsX()+1);
    for (int i=1; i<4; i++){
      float this_bin = dem_hists[1]->GetBinContent(i);
      float next_bin = dem_hists[1]->GetBinContent(i+1);
      if (this_bin<1e-20) this_bin=0;
      if (next_bin<1e-20) next_bin=0;
      table_array[i+4][p] = this_bin;
      if (i==3)table_array[i+4][p] = this_bin+next_bin;
    }
    table_array[8][p] = dem_hists[2]->Integral(1,dem_hists[2]->GetNbinsX()+1);
    for (int i=1; i<4; i++){
      float this_bin = dem_hists[2]->GetBinContent(i);
      float next_bin = dem_hists[2]->GetBinContent(i+1);
      if (this_bin<1e-20) this_bin=0;
      if (next_bin<1e-20) next_bin=0;
      table_array[i+8][p] = this_bin;
      if (i==3)table_array[i+8][p] = this_bin+next_bin;
    }
    table_array[12][p] = dem_hists[3]->Integral(1,dem_hists[3]->GetNbinsX()+1);
    for (int i=1; i<4; i++){
      float this_bin = dem_hists[3]->GetBinContent(i);
      float next_bin = dem_hists[3]->GetBinContent(i+1);
      if (this_bin<1e-20) this_bin=0;
      if (next_bin<1e-20) next_bin=0;
      table_array[i+12][p] = this_bin;
      if (i==3)table_array[i+12][p] = this_bin+next_bin;
    }


    table_array[16][p] = dem_hists[4]->Integral(1,dem_hists[4]->GetNbinsX()+1);
    for (int i=1; i<5; i++){
      float this_bin = dem_hists[4]->GetBinContent(i);
      float next_bin = dem_hists[4]->GetBinContent(i+1);
      if (this_bin<1e-20) this_bin=0;
      if (next_bin<1e-20) next_bin=0;
      table_array[i+16][p] = this_bin;
      if (i==4)table_array[i+16][p] = this_bin+next_bin;
    }

    table_array[21][p] = dem_hists[5]->Integral(1,dem_hists[5]->GetNbinsX()+1);
    for (int i=1; i<5; i++){
      float this_bin = dem_hists[5]->GetBinContent(i);
      float next_bin = dem_hists[5]->GetBinContent(i+1);
      if (this_bin<1e-20) this_bin=0;
      if (next_bin<1e-20) next_bin=0;
      table_array[i+21][p] = this_bin;
      if (i==4)table_array[i+21][p] = this_bin+next_bin;
    }

    delete tree;
    color_counter++;
  } //end loop through processes

  //For Rishi's stack plots
  if (save_stacks){
    TCanvas *c1 = new TCanvas("c1","stacked hists, boost",10,10,1000,800);
    stack_Boost->Draw("hist");
    c1->SetLogy();
    legend->Draw("same");
    c1->Modified();
    c1->SaveAs("newMETbin/Boost/stack_MET.root");

    TCanvas *c2 = new TCanvas("c2","stacked hists, DiAK8",10,10,1000,800);
    stack_DiAK8->Draw("hist");
    c2->SetLogy();
    legend->Draw("same");
    c2->Modified();
    c2->SaveAs("newMETbin/DiAK8/stack_MET.root");

    TCanvas *c3 = new TCanvas("c3","stacked hists, SemiResolved",10,10,1000,800);
    stack_SemiResolved->Draw("hist");
    c3->SetLogy();
    legend->Draw("same");
    c3->Modified();
    c3->SaveAs("newMETbin/SemiResolved/stack_MET.root");

    TCanvas *c4 = new TCanvas("c4","stacked hists, SingleAK8",10,10,1000,800);
    stack_SingleAK8->Draw("hist");
    c4->SetLogy();
    legend->Draw("same");
    c4->Modified();
    c4->SaveAs("newMETbin/SingleAK8/stack_MET.root");

    TCanvas *c5 = new TCanvas("c5","stacked hists, 4b resolved",10,10,1000,800);
    stack_Res4b->Draw("hist");
    c5->SetLogy();
    legend->Draw("same");
    c5->Modified();
    c5->SaveAs("newMETbin/Res4b/stack_MET.root");

    TCanvas *c6 = new TCanvas("c6","stacked hists, 3b resolved",10,10,1000,800);
    stack_Res3b->Draw("hist");
    c6->SetLogy();
    legend->Draw("same");
    c6->Modified();
    c6->SaveAs("newMETbin/Res3b/stack_MET.root");
  }


  if (make_table){
    //Print table to screen by looping through the values of table_arry[row][column=process]
    cout<<endl<<endl;
    cout<<"  _________________________________________________________________________________________________________"<<endl;
    cout<<" |                 |    TT    |  Zjets   |  WJets   |   QCD    |TChiHH700 |TChiHH1000| T5HH1700 | T5HH2100 |"<<endl;
    cout<<" |-----------------+----------+----------+----------+----------+----------+----------+----------+----------|"<<endl;
    cout<<" |2 Boosted H      |";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[0][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 300-500|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[1][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 500-700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[2][i]<<"|";
    cout<<endl;
    cout<<" |        MET: >700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[3][i]<<"|";
    cout<<endl;
    cout<<" |-----------------+----------+----------+----------+----------+----------+----------+----------+----------|"<<endl;


    cout<<" |1 Boosted, 1 AK8 |";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[4][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 300-500|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[5][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 500-700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[6][i]<<"|";
    cout<<endl;
    cout<<" |        MET: >700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[7][i]<<"|";
    cout<<endl;
    cout<<" |-----------------+----------+----------+----------+----------+----------+----------+----------+----------|"<<endl;


    cout<<" |1Boost, 1Resolved|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[8][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 300-500|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[9][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 500-700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[10][i]<<"|";
    cout<<endl;
    cout<<" |        MET: >700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[11][i]<<"|";
    cout<<endl;
    cout<<" |-----------------+----------+----------+----------+----------+----------+----------+----------+----------|"<<endl;


    cout<<" |1Boost, 0Resolved|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[12][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 300-500|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[13][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 500-700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[14][i]<<"|";
    cout<<endl;
    cout<<" |        MET: >700|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[15][i]<<"|";
    cout<<endl;
    cout<<" |-----------------+----------+----------+----------+----------+----------+----------+----------+----------|"<<endl;


    cout<<" |2 Resolved (TTML)|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[16][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 150-200|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[17][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 200-300|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[18][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 300-450|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[19][i]<<"|";
    cout<<endl;
    cout<<" |        MET: >450|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[20][i]<<"|";
    cout<<endl;
    cout<<" |-----------------+----------+----------+----------+----------+----------+----------+----------+----------|"<<endl;


    cout<<" |2 Resolved  (TTM)|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[21][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 150-200|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[22][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 200-300|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[23][i]<<"|";
    cout<<endl;
    cout<<" |     MET: 300-450|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[24][i]<<"|";
    cout<<endl;
    cout<<" |        MET: >450|";
    for(int i =0; i<8; i++) cout<<setw(10)<<setprecision(5)<<table_array[25][i]<<"|";
    cout<<endl;
    cout<<" |_________________|__________|__________|__________|__________|__________|__________|__________|__________|"<<endl;
    cout<<endl<<endl;
  }
inf->Close(); //is this needed?
} // MakeTable
