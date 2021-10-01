#include <cstdlib>
#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <limits>
#include <getopt.h>

#include "TFile.h"
#include "TTree.h"
#include "TSystem.h"
#include "TDirectory.h"

using namespace std;

std::vector<std::string> split(const std::string& s, char delimiter);
void higgsino2DCrossSection(int hig_mass, double &xsec, double &xsec_unc);
void gluino2DCrossSection(int hig_mass, double &xsec, double &xsec_unc);

string in_dir = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/forGithub/CMSSW_10_2_13/src/boostedHiggsPlusMET/datacards";
string out_dir = in_dir;
string model = "N1N2";
// string model = "Gluino";

string which = "comb";
// string which = "res";
// string which = "boost";

void scan_point() {
  string inFile = in_dir+"/higgsino2DFileNames.txt";
  if (model=="Gluino") inFile = in_dir+"/Gluino2DScanNames.txt";
  ifstream file(inFile);
  string line; TString filename;
  string txtname(out_dir+"/limitsCombined_"+model+"_data.txt");
  if (which=="res") txtname = out_dir+"/limitsResOnly_"+model+"_data.txt";
  else if (which=="boost") txtname = out_dir+"/limitsBoostOnly_"+model+"_data.txt";
  ofstream txtfile(txtname);

  while(std::getline(file, line)) {
    std::vector<std::string> x = split(line, '_');
    string hino_mass = ""; string LSP_mass = "";
    if (model=="Gluino") {hino_mass = x[3]; LSP_mass = x[4];}
    else {hino_mass = x[5]; LSP_mass = x[6];}
    std::cout<<"hino: "<<hino_mass<<", LSP: "<<LSP_mass<<std::endl;
    int hino_mass_int = std::stoi(hino_mass);
    int LSP_mass_int = std::stoi(LSP_mass);
    if (model=="N1N2" && hino_mass_int>810) continue;

    filename = in_dir+"/higgsCombine2DTChiHH"+hino_mass+"_LSP"+LSP_mass+"_Data_Combo.AsymptoticLimits.mH120.root";
    if (which=="boost") filename = in_dir+"/higgsCombine2DTChiHH"+hino_mass+"_LSP"+LSP_mass+"_BothBoostedH_Data.AsymptoticLimits.mH120.root";
    else if (which=="res") filename = in_dir+"/higgsCombine2DTChiHH"+hino_mass+"_LSP"+LSP_mass+"_Data_ResOnly.AsymptoticLimits.mH120.root";
    if (model=="Gluino") filename = in_dir+"/higgsCombine2DT5HH"+hino_mass+"_LSP"+LSP_mass+"_BothBoostedH_Data.AsymptoticLimits.mH120.root"; //gluino

    double xsec, xsec_unc;
    if (model=="N1N2") higgsino2DCrossSection(hino_mass_int, xsec, xsec_unc);
    else if (model=="Gluino") gluino2DCrossSection(hino_mass_int, xsec, xsec_unc);

    TFile limits_file(filename, "read");
    if (!limits_file.IsOpen()) std::cout<<"Could not open limits file "<<filename<<std::endl;
    TTree *tree = static_cast<TTree*>(limits_file.Get("limit"));
    if (tree == nullptr) std::cout<<"Could not get limits tree"<<std::endl;

    double limit;
    tree->SetBranchAddress("limit", &limit);
    int num_entries = tree->GetEntries();
    if (num_entries != 6) std::cout<<"Expected 6 tree entries. Saw "<<num_entries<<std::endl;
    tree->GetEntry(0); double exp_2down = limit;
    tree->GetEntry(1); double exp_down = limit;
    tree->GetEntry(2); double exp = limit;
    tree->GetEntry(3); double exp_up = limit;
    tree->GetEntry(4); double exp_2up = limit;
    tree->GetEntry(5); double obs = limit;
    limits_file.Close();

    txtfile << setprecision(numeric_limits<double>::max_digits10)
      << ' ' << hino_mass
      << ' ' << LSP_mass
      << ' ' << xsec
      << ' ' << xsec_unc
      << ' ' << obs
      << ' ' << exp
      << ' ' << exp_up
      << ' ' << exp_down
      << ' ' << exp_2up
      << ' ' << exp_2down;
    txtfile << endl;
  } // end while loop

  //include 1D scan at bottom of 2D TChiHH
  if (model=="N1N2") {
    for (int i=150;i<810;i+=25) {
      string hino_mass = std::to_string(i);
      string LSP_mass = "1";
      std::cout<<"hino: "<<hino_mass<<", LSP: "<<LSP_mass<<std::endl;
      int hino_mass_int = i;
      int LSP_mass_int = 1;

      filename = in_dir+"/higgsCombine2DTChiHH"+hino_mass+"_LSP"+LSP_mass+"_Data_Combo.AsymptoticLimits.mH120.root";
      if (which=="boost") filename = in_dir+"/higgsCombine2DTChiHH"+hino_mass+"_LSP"+LSP_mass+"_BothBoostedH_Data.AsymptoticLimits.mH120.root";
      else if (which=="res") filename = in_dir+"/higgsCombine2DTChiHH"+hino_mass+"_LSP"+LSP_mass+"_Data_ResOnly.AsymptoticLimits.mH120.root";


      double xsec, xsec_unc;
      higgsino2DCrossSection(hino_mass_int, xsec, xsec_unc);

      TFile limits_file(filename, "read");
      if (!limits_file.IsOpen()) std::cout<<"Could not open limits file "<<filename<<std::endl;
      TTree *tree = static_cast<TTree*>(limits_file.Get("limit"));
      if (tree == nullptr) std::cout<<"Could not get limits tree"<<std::endl;

      double limit;
      tree->SetBranchAddress("limit", &limit);
      int num_entries = tree->GetEntries();
      if (num_entries != 6) std::cout<<"Expected 6 tree entries. Saw "<<num_entries<<std::endl;
      tree->GetEntry(0); double exp_2down = limit;
      tree->GetEntry(1); double exp_down = limit;
      tree->GetEntry(2); double exp = limit;
      tree->GetEntry(3); double exp_up = limit;
      tree->GetEntry(4); double exp_2up = limit;
      tree->GetEntry(5); double obs = limit;
      limits_file.Close();

      txtfile << setprecision(numeric_limits<double>::max_digits10)
        << ' ' << hino_mass
        << ' ' << LSP_mass
        << ' ' << xsec
        << ' ' << xsec_unc
        << ' ' << obs
        << ' ' << exp
        << ' ' << exp_up
        << ' ' << exp_down
        << ' ' << exp_2up
        << ' ' << exp_2down;
      txtfile << endl;
    } // end loop through 1D
  }
  txtfile.close();
} // end scan point

std::vector<std::string> split(const std::string& s, char delimiter) {
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)) { tokens.push_back(token);}
   return tokens;
}

void higgsino2DCrossSection(int hig_mass, double &xsec, double &xsec_unc) {
  // float BR = 0.5824;
  float BR = 1.0;
  float factor = 1000.0; //returns fb
  if (hig_mass ==127) { xsec = BR*BR*1.44725*factor; xsec_unc = 0.0395277; return;}
  else if (hig_mass ==150) { xsec = BR*BR*0.71514*factor; xsec_unc = 0.0421496; return;}
  else if (hig_mass ==175) { xsec = BR*BR*0.419059*factor; xsec_unc = 0.0453279; return;}
  else if (hig_mass ==200) { xsec = BR*BR*0.244213*factor; xsec_unc = 0.047925; return;}
  else if (hig_mass ==225) { xsec = BR*BR*0.156286*factor; xsec_unc = 0.0502876; return;}
  else if (hig_mass ==250) { xsec = BR*BR*0.104252*factor; xsec_unc = 0.0526169; return;}
  else if (hig_mass ==275) { xsec = BR*BR*0.0719125*factor; xsec_unc = 0.0549666; return;}
  else if (hig_mass ==300) { xsec = BR*BR*0.0509994*factor; xsec_unc = 0.0572762; return;}
  else if (hig_mass ==325) { xsec = BR*BR*0.0369715*factor; xsec_unc = 0.0590317; return;}
  else if (hig_mass ==350) { xsec = BR*BR*0.0273286*factor; xsec_unc = 0.0607766; return;}
  else if (hig_mass ==375) { xsec = BR*BR*0.0205429*factor; xsec_unc = 0.0625031; return;}
  else if (hig_mass ==400) { xsec = BR*BR*0.0156691*factor; xsec_unc = 0.0642085; return;}
  else if (hig_mass ==425) { xsec = BR*BR*0.0120965*factor; xsec_unc = 0.0657801; return;}
  else if (hig_mass ==450) { xsec = BR*BR*0.00944017*factor; xsec_unc = 0.0674544; return;}
  else if (hig_mass ==475) { xsec = BR*BR*0.00743587*factor; xsec_unc = 0.0686033; return;}
  else if (hig_mass ==500) { xsec = BR*BR*0.00590757*factor; xsec_unc = 0.0699909; return;}
  else if (hig_mass ==525) { xsec = BR*BR*0.00469101*factor; xsec_unc = 0.0713704; return;}
  else if (hig_mass ==550) { xsec = BR*BR*0.0038167*factor; xsec_unc = 0.0722834; return;}
  else if (hig_mass ==575) { xsec = BR*BR*0.003073*factor; xsec_unc = 0.0739957; return;}
  else if (hig_mass ==600) { xsec = BR*BR*0.00253015*factor; xsec_unc = 0.0754291; return;}
  else if (hig_mass ==625) { xsec = BR*BR*0.00206136*factor; xsec_unc = 0.0763466; return;}
  else if (hig_mass ==650) { xsec = BR*BR*0.00171418*factor; xsec_unc = 0.0775695; return;}
  else if (hig_mass ==675) { xsec = BR*BR*0.00140934*factor; xsec_unc = 0.0783375; return;}
  else if (hig_mass ==700) { xsec = BR*BR*0.00118113*factor; xsec_unc = 0.0796388; return;}
  else if (hig_mass ==725) { xsec = BR*BR*0.000979349*factor; xsec_unc = 0.0809883; return;}
  else if (hig_mass ==750) { xsec = BR*BR*0.000826366*factor; xsec_unc = 0.081879; return;}
  else if (hig_mass ==775) { xsec = BR*BR*0.000690208*factor; xsec_unc = 0.0842049; return;}
  else if (hig_mass ==800) { xsec = BR*BR*0.000586211*factor; xsec_unc = 0.0862527; return;}
  else if (hig_mass ==825) { xsec = BR*BR*0.00049277*factor; xsec_unc = 0.0864444; return;}
  else if (hig_mass ==850) { xsec = BR*BR*0.000420556*factor; xsec_unc = 0.085742; return;}
  else if (hig_mass ==875) { xsec = BR*BR*0.000358734*factor; xsec_unc = 0.0889174; return;}
  else if (hig_mass ==900) { xsec = BR*BR*0.000305935*factor; xsec_unc = 0.0912439; return;}
  else if (hig_mass ==925) { xsec = BR*BR*0.000260948*factor; xsec_unc = 0.091372; return;}
  else if (hig_mass ==950) { xsec = BR*BR*0.00022285*factor; xsec_unc = 0.0919538; return;}
  else if (hig_mass ==975) { xsec = BR*BR*0.000189681*factor; xsec_unc = 0.0938108; return;}
  else if (hig_mass ==1000) { xsec = BR*BR*0.00016428*factor; xsec_unc = 0.0954285; return;}
  else if (hig_mass ==1025) { xsec = BR*BR*0.000142206*factor; xsec_unc = 0.0957231; return;}
  else if (hig_mass ==1050) { xsec = BR*BR*0.000120971*factor; xsec_unc = 0.0968997; return;}
  else if (hig_mass ==1075) { xsec = BR*BR*0.000105301*factor; xsec_unc = 0.0979041; return;}
  else if (hig_mass ==1100) { xsec = BR*BR*9.12469e-05*factor; xsec_unc = 0.0964142; return;}
  else if (hig_mass ==1125) { xsec = BR*BR*7.9765e-05*factor; xsec_unc = 0.099902; return;}
  else if (hig_mass ==1150) { xsec = BR*BR*6.78234e-05*factor; xsec_unc = 0.101061; return;}
  else if (hig_mass ==1175) { xsec = BR*BR*5.9016e-05*factor; xsec_unc = 0.102051; return;}
  else if (hig_mass ==1200) { xsec = BR*BR*5.16263e-05*factor; xsec_unc = 0.102499; return;}
  else if (hig_mass ==1225) { xsec = BR*BR*4.5147e-05*factor; xsec_unc = 0.10403; return;}
  else if (hig_mass ==1250) { xsec = BR*BR*3.88343e-05*factor; xsec_unc = 0.105206; return;}
  else if (hig_mass ==1275) { xsec = BR*BR*3.41304e-05*factor; xsec_unc = 0.10619; return;}
  else if (hig_mass ==1300) { xsec = BR*BR*2.99353e-05*factor; xsec_unc = 0.10783; return;}
  else if (hig_mass ==1325) { xsec = BR*BR*2.63637e-05*factor; xsec_unc = 0.108024; return;}
  else if (hig_mass ==1350) { xsec = BR*BR*2.26779e-05*factor; xsec_unc = 0.109016; return;}
  else if (hig_mass ==1375) { xsec = BR*BR*1.99318e-05*factor; xsec_unc = 0.109822; return;}
  else if (hig_mass ==1400) { xsec = BR*BR*1.75031e-05*factor; xsec_unc = 0.111631; return;}
  else if (hig_mass ==1425) { xsec = BR*BR*1.53974e-05*factor; xsec_unc = 0.111417; return;}
  else if (hig_mass ==1455) { xsec = BR*BR*1.3245e-05*factor; xsec_unc = 0.112313; return;}
  else if (hig_mass ==1475) { xsec = BR*BR*1.16416e-05*factor; xsec_unc = 0.113058; return;}
  else { xsec = 0; xsec_unc = 0;}
}

void gluino2DCrossSection(int hig_mass, double &xsec, double &xsec_unc) { //eventually update with 50 GeV points
  if (hig_mass ==1000) { xsec = 0.325388; xsec_unc = 16.758*xsec/100; return;}
  else if (hig_mass ==1100) { xsec = 0.163491; xsec_unc = 17.6402*xsec/100; return;}
  else if (hig_mass ==1200) { xsec = 0.0856418; xsec_unc = 18.4814*xsec/100; return;}
  else if (hig_mass ==1300) { xsec = 0.0460525; xsec_unc = 19.64*xsec/100; return;}
  else if (hig_mass ==1400) { xsec = 0.0252977; xsec_unc = 20.9163*xsec/100; return;}
  else if (hig_mass ==1500) { xsec = 0.0141903; xsec_unc = 22.7296*xsec/100; return;}
  else if (hig_mass ==1600) { xsec = 0.00810078; xsec_unc = 24.2679*xsec/100; return;}
  else if (hig_mass ==1700) { xsec = 0.00470323; xsec_unc = 26.1021*xsec/100; return;}
  else if (hig_mass ==1800) { xsec = 0.00276133;  xsec_unc = 28.108*xsec/100;return;}
  else if (hig_mass ==1900) { xsec = 0.00163547;  xsec_unc = 29.9045*xsec/100;return;}
  else if (hig_mass ==2000) { xsec = 0.000981077; xsec_unc = 31.8422*xsec/100; return;}
  else if (hig_mass ==2100) { xsec = 0.000591918; xsec_unc = 33.9326*xsec/100; return;}
  else if (hig_mass ==2200) { xsec = 0.000359318; xsec_unc = 35.9623*xsec/100; return;}
  else if (hig_mass ==2300) { xsec = 0.000219049; xsec_unc = 38.5249*xsec/100; return;}
  else if (hig_mass ==2400) { xsec = 0.000133965; xsec_unc = 40.7945*xsec/100; return;}
  else if (hig_mass ==2500) { xsec = 8.20068E-05; xsec_unc = 43.1071*xsec/100; return;}
  else if (hig_mass ==2600) { xsec = 5.03066e-05; xsec_unc = 45.6584*xsec/100; return;}
}
