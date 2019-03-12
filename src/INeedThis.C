#define INeedThis_cxx
#include "INeedThis.h"
#include <TH2.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <string>


vector<TH1F*> INeedThis::Loop(float EventWeight, string process = "", bool save_files = true){
  //   In a ROOT session, you can do:
  //      root> .L INeedThis.C
  //      root> INeedThis t
  //      root> t.Loop();       // Loop on all entries
  vector<TH1F*> dem_hists;
  if (fChain == 0) return dem_hists;

  float bins2[]={150,200,300,450,600,800};
  int binnum2 = 5;

  float bins1[]={300,500,700,900,1000};
  int binnum1 = 4;
  string histname = process + "_MET";
  TH1F *hist_Boost = new TH1F(histname.c_str(),histname.c_str(),binnum1, bins1);
  TH1F *hist_DiAK8 = new TH1F(histname.c_str(),histname.c_str(),binnum1, bins1);
  TH1F *hist_SemiResolved = new TH1F(histname.c_str(),histname.c_str(),binnum1, bins1);
  TH1F *hist_SingleAK8 = new TH1F(histname.c_str(),histname.c_str(),binnum1, bins1);
  TH1F *hist_Res4b = new TH1F(histname.c_str(),histname.c_str(),binnum2, bins2);
  TH1F *hist_Res3b = new TH1F(histname.c_str(),histname.c_str(),binnum2, bins2);

  dem_hists = {hist_Boost,hist_DiAK8,hist_SemiResolved,hist_SingleAK8,hist_Res4b,hist_Res3b};

  int nentries = fChain->GetEntriesFast();

  int nbytes = 0, nb = 0;
  for (int jentry=0; jentry<nentries;jentry++) {
    int ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    double resolved_mass = resolvedHCan_mass->at(0);

    bool is_resolved = (nAK4Cuts>=4 && nAK4Cuts<6 && HiggsCan_MassDiff<40 && deltaR_max<2.2 && HiggsCan_AvgMass>100 && HiggsCan_AvgMass<=140);
    bool is_4b = ((BTagsT>=2 && BTagsM>=1 && BTagsL>=1)||(BTagsT>=2 && BTagsM>=2)||(BTagsT>=3 && BTagsL>=1)||(BTagsT>=3 && BTagsM>=1)||(BTagsT>=4));
    bool is_3b = (BTagsT>=2 && BTags==3);
    float EventWeightHere = Weight*EventWeight;


    if(process.find("T5qqqq")!=string::npos) {
      if      (nGenHs==2 && nBoostedH>=2 && MET>300 && HT>600) hist_Boost->Fill(MET,EventWeightHere);
      else if (nGenHs==2 && nBoostedH==1 && nAK8_belowB>0 && MET>300 && HT>600) hist_DiAK8->Fill(MET,EventWeightHere);
      else if (nGenHs==2 && nBoostedH>=1 && nResolvedH==1 && MET>300 && HT>350 && resolved_mass>100 && resolved_mass<=140) hist_SemiResolved->Fill(MET,EventWeightHere);
      else if (nGenHs==2 && nBoostedH>=1 && nResolvedH==0 && nIsoBNotResolved>0 && MET>300 && HT>350) hist_SingleAK8->Fill(MET,EventWeightHere);
      else if (nGenHs==2 && is_resolved && is_4b) hist_Res4b->Fill(MET,EventWeightHere);
      else if (nGenHs==2 && is_resolved && is_3b) hist_Res3b->Fill(MET,EventWeightHere);
    }

    else {
      if      (nBoostedH>=2 && MET>300 && HT>600) hist_Boost->Fill(MET,EventWeightHere);
      else if (nBoostedH==1 && nAK8_belowB>0 && MET>300 && HT>600) hist_DiAK8->Fill(MET,EventWeightHere);
      else if (nBoostedH>=1 && nResolvedH==1 && MET>300 && HT>350 && resolved_mass>100 && resolved_mass<=140) hist_SemiResolved->Fill(MET,EventWeightHere);
      else if (nBoostedH>=1 && nResolvedH==0 && nIsoBNotResolved>0 && MET>300 && HT>350) hist_SingleAK8->Fill(MET,EventWeightHere);
      else if (is_resolved && is_4b) hist_Res4b->Fill(MET,EventWeightHere);
      else if (is_resolved && is_3b) hist_Res3b->Fill(MET,EventWeightHere);
    }

  } //end loop over entries
  //string savename = processes[p]+".root";

  string savename = process+"_MET.root";
  std::cout<<"Right before save files"<<endl;
  //save_files=true;
  if (save_files){
    std::cout<<"saving files...."<<endl;
    hist_Boost->SaveAs(("newMETbin/Boost/"+savename).c_str());
    hist_DiAK8->SaveAs(("newMETbin/DiAK8/"+savename).c_str());
    hist_SemiResolved->SaveAs(("newMETbin/SemiResolved/"+savename).c_str());
    hist_SingleAK8->SaveAs(("newMETbin/SingleAK8/"+savename).c_str());
    hist_Res4b->SaveAs(("newMETbin/Res4b/"+savename).c_str());
    hist_Res3b->SaveAs(("newMETbin/Res3b/"+savename).c_str());
  }

return dem_hists;
} //end Loop()
