#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <unistd.h>
#include <getopt.h>
#include <vector>

#include "TCanvas.h"
#include "TGraph.h"
#include "TGraph2D.h"
#include "TColor.h"
#include "TH2D.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TFile.h"
#include "TLatex.h"
#include "TLine.h"

#include <sys/stat.h> //to check if file exists
#include <unistd.h>

using namespace std;

// string in_dir = "/uscms_data/d3/emacdona/WorkingArea/CombinedHiggs/forGithub/CMSSW_10_2_13/src/boostedHiggsPlusMET/datacards/";
string src_dir = "../src/";
string out_dir = "../output/";
string tag = "";

string model_ = "N1N2";
// string model_ = "Gluino";

void ReadPointsCombo(vector<double> &vmx,vector<double> &vmy,vector<double> &vxsec,vector<double> &vobs,vector<double> &vobsup,vector<double> &vobsdown, vector<double> &vobsup2,vector<double> &vobsdown2, vector<double> &vexp,vector<double> &vup,vector<double> &vdown, vector<double> &vup2,vector<double> &vdown2);
void ReadPointsAll(vector<double> &vmxComb,vector<double> &vmyComb,vector<double> &vxsecComb,vector<double> &vobsBoost,vector<double> &vobsRes,vector<double> &vobsComb);
void SaveRootFile();
void MakeLimitPlotCombo(vector<double> vmx,vector<double> vmy,vector<double> vlim,vector<double> vobs,vector<double> vobsup,vector<double> vobsdown,vector<double> vobsup2,vector<double> vobsdown2,vector<double> vexp,vector<double> vup,vector<double> vdown, vector<double> vup2,vector<double> vdown2);
void MakeLimitPlotAll(vector<double> vmxComb,vector<double> vmyComb,vector<double> vlim,vector<double> vobsComb,vector<double> vobsBoost,vector<double> vobsRes);
void GetParticleNames(string &xparticle, string &yparticle);
void Style(TGraph *g, int color, int style, float width);
TGraph DrawContours(TGraph2D &g2, int color, int style, double width, double val, bool draw);
TGraph* joinGraphs(TGraph *graph1, TGraph *graph2);
void ReverseGraph(TGraph &graph);
void SetupColors();
bool doesFileExist (const std::string& name);

void limit_scan() {
  vector<double> vmx, vmy, vxsec, vobs, vobsup, vobsdown,vobsup2, vobsdown2, vexp, vup, vdown, vup2, vdown2;
  ReadPointsCombo(vmx, vmy, vxsec, vobs, vobsup, vobsdown, vobsup2, vobsdown2, vexp, vup, vdown, vup2, vdown2); //just for boosted+resolved combo
  vector<double> vlim(vxsec.size());
  for (size_t i = 0; i < vxsec.size(); ++i) {
    vlim.at(i) = vxsec.at(i)*vobs.at(i);
  }
  MakeLimitPlotCombo(vmx, vmy, vlim, vobs, vobsup, vobsdown, vobsup2, vobsdown2, vexp, vup, vdown, vup2, vdown2);

  SaveRootFile(); //For HEPdata
  std::exit(1);
}

void ReadPointsCombo(vector<double> &vmx, vector<double> &vmy, vector<double> &vxsec,
                     vector<double> &vobs, vector<double> &vobsup, vector<double> &vobsdown,
                     vector<double> &vobsup2, vector<double> &vobsdown2,
                     vector<double> &vexp, vector<double> &vup, vector<double> &vdown,
                     vector<double> &vup2, vector<double> &vdown2
                    ) {

  string txtname = src_dir+"/limitsCombined_"+model_+"_data.txt";
  if (!doesFileExist(txtname)) {
    std::cout<<"You need to run scan_point.cxx first! Exiting..."<<std::endl;
    std::exit(1);
  }
  ifstream infile(txtname); string line;

  while(getline(infile, line)) {
    istringstream iss(line);
    double pmx, pmy, pxsec, pxsecunc, pobs, pexp, pup, pdown, pup2, pdown2;
    iss >> pmx >> pmy >> pxsec >> pxsecunc >> pobs >> pexp >> pup >> pdown >> pup2 >> pdown2;

    vmx.push_back(pmx);
    if (pmy==1.0){
      vmy.push_back(-0.01); //shift for plotting purposes
    }
    else {
      vmy.push_back(pmy);
    }
    vxsec.push_back(pxsec);
    vobs.push_back(pobs);
    vobsup.push_back(pobs/(1+pxsecunc));
    vobsdown.push_back(pobs/(1-pxsecunc));
    vobsup2.push_back(pobs/(1+2*pxsecunc));
    vobsdown2.push_back(pobs/(1-2*pxsecunc));

    vexp.push_back(pexp);
    vup.push_back(pup);
    vdown.push_back(pdown);
    vup2.push_back(pup2);
    vdown2.push_back(pdown2);
  }
  infile.close();

  if (vmx.size() <= 2) {
    std::cout<<"Need at least 3 models to draw scan"<<std::endl;
    return;
  }
  if (vmx.size() != vmy.size()
     || vmx.size() != vxsec.size()
     || vmx.size() != vobs.size()
     || vmx.size() != vobsup.size()
     || vmx.size() != vobsdown.size()
     || vmx.size() != vexp.size()
     || vmx.size() != vup.size()
     || vmx.size() != vdown.size()
   )
  {
    std::cout<<"Error parsing text file. Model point not fully specified\n";
  }
}

//This is just observed lines for all
//vmxComb, vmyComb, vxsecComb, vobsBoost, vobsRes, vobsComb
void ReadPointsAll(vector<double> &vmxComb,
                vector<double> &vmyComb,
                vector<double> &vxsecComb,
                vector<double> &vobsBoost,
                vector<double> &vobsRes,
                vector<double> &vobsComb
              ) {


  TString  txtname(src_dir+"/limitsCombined_"+model_+"_data.txt");
  TString txtname2(src_dir+"/limitsBoostOnly_"+model_+"_data.txt");
  TString txtname3(src_dir+"/limitsResOnly_"+model_+"_data.txt");

  ifstream infile(txtname);
  ifstream infile2(txtname2);
  ifstream infile3(txtname3);
  string line; string line2; string line3;

  while(getline(infile, line)) {
    istringstream iss(line);
    double pmx, pmy, pxsec, pxsecunc, pobs, pexp, pup, pdown, pup2, pdown2;
    iss >> pmx >> pmy >> pxsec >> pxsecunc >> pobs >> pexp >> pup >> pdown >> pup2 >> pdown2;
    vmxComb.push_back(pmx);
    vmyComb.push_back(pmy);
    vxsecComb.push_back(pxsec);

    // vobsComb.push_back(pobs);
    vobsComb.push_back(pexp);
  }
  //Get "obs" for boostOnly and resOnly
  while(getline(infile2, line2)) {
    istringstream iss2(line2);
    double pmx_2, pmy_2, pxsec_2, pxsecunc_2, pobs_2, pexp_2, pup_2, pdown_2, pup2_2, pdown2_2;
    iss2 >> pmx_2 >> pmy_2 >> pxsec_2 >> pxsecunc_2 >> pobs_2 >> pexp_2 >> pup_2 >> pdown_2 >> pup2_2 >> pdown2_2;
    // vobsBoost.push_back(pobs_2);
    vobsBoost.push_back(pexp_2);
  }
  while(getline(infile3, line3)) {
    istringstream iss3(line3);
    double pmx_3, pmy_3, pxsec_3, pxsecunc_3, pobs_3, pexp_3, pup_3, pdown_3, pup2_3, pdown2_3;
    iss3 >> pmx_3 >> pmy_3 >> pxsec_3 >> pxsecunc_3 >> pobs_3 >> pexp_3 >> pup_3 >> pdown_3 >> pup2_3 >> pdown2_3;
    // vobsRes.push_back(pobs_3);
    vobsRes.push_back(pexp_3);
  }
  infile.close(); infile2.close(); infile3.close();

  if (vmxComb.size() <= 2) {
    std::cout<<"Need at least 3 models to draw scan"<<std::endl;
    return;
  }
  if (vmxComb.size() != vmyComb.size()
     || vmxComb.size() != vxsecComb.size()
     || vmxComb.size() != vobsComb.size()
   )
  {
    std::cout<<"Error parsing text file. Model point not fully specified\n";
  }
}

void MakeLimitPlotCombo(vector<double> vmx, vector<double> vmy, vector<double> vlim,
                        vector<double> vobs, vector<double> vobsup, vector<double> vobsdown,
                        vector<double> vobsup2, vector<double> vobsdown2,
                        vector<double> vexp, vector<double> vup, vector<double> vdown,
                        vector<double> vup2, vector<double> vdown2
                        ) {
  SetupColors();

  string xparticle, yparticle;
  GetParticleNames(xparticle, yparticle);
  TString chi03 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.9]{#scale[0.85]{_{3}}}";
  string chi02_str = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.95]{#scale[0.85]{_{2}}}";
  TString chi02 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-0.95]{#scale[0.85]{_{2}}}";
  TString chi01 = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0 }}}#kern[-1.2]{#scale[0.85]{_{1 }}}";
  string title = ";m("+chi02_str+") [GeV];m("+yparticle+") [GeV];Cross section upper limit (95% CL) [fb]";

  TGraph2D glim("", title.c_str(), vlim.size(), &vmx.at(0), &vmy.at(0), &vlim.at(0));
  TGraph2D gobs("", "Observed Limit", vobs.size(), &vmx.at(0), &vmy.at(0), &vobs.at(0));
  TGraph2D gobsup("", "Observed +1#sigma Limit", vobsup.size(), &vmx.at(0), &vmy.at(0), &vobsup.at(0));
  TGraph2D gobsdown("", "Observed -1#sigma Limit", vobsdown.size(), &vmx.at(0), &vmy.at(0), &vobsdown.at(0));
  TGraph2D gexp("", "Expected Limit", vexp.size(), &vmx.at(0), &vmy.at(0), &vexp.at(0));
  TGraph2D gup("", "Expected +1#sigma Limit", vup.size(), &vmx.at(0), &vmy.at(0), &vup.at(0));
  TGraph2D gdown("", "Expected -1#sigma Limit", vdown.size(), &vmx.at(0), &vmy.at(0), &vdown.at(0));

  TGraph2D gup2("", "Expected +2#sigma Limit", vup2.size(), &vmx.at(0), &vmy.at(0), &vup2.at(0));
  TGraph2D gdown2("", "Expected -2#sigma Limit", vdown2.size(), &vmx.at(0), &vmy.at(0), &vdown2.at(0));
  TGraph2D gobsup2("", "Observed +2#sigma Limit", vobsup2.size(), &vmx.at(0), &vmy.at(0), &vobsup2.at(0));
  TGraph2D gobsdown2("", "Observed -2#sigma Limit", vobsdown2.size(), &vmx.at(0), &vmy.at(0), &vobsdown2.at(0));

  if (model_=="N1N2"){
    gexp.SetMinimum(0.0001); gexp.SetMaximum(2); gexp.SetNpx(38.); gexp.SetNpy(35.);
  }
  else {
    gobs.SetMinimum(0.000001); gobs.SetMaximum(2); gobs.SetNpx(38.); gobs.SetNpy(35.);
    gobsup.SetMinimum(0.000001); gobsup.SetMaximum(2); gobsup.SetNpx(38.); gobsup.SetNpy(35.);
    gobsdown.SetMinimum(0.000001); gobsdown.SetMaximum(2); gobsdown.SetNpx(38.); gobsdown.SetNpy(35.);
    gexp.SetMinimum(0.000001); gexp.SetMaximum(2); gexp.SetNpx(38.); gexp.SetNpy(35.);
    gup.SetMinimum(0.000001); gup.SetMaximum(2); gup.SetNpx(38.); gup.SetNpy(35.);
    gdown.SetMinimum(0.000001); gdown.SetMaximum(2); gdown.SetNpx(38.); gdown.SetNpy(35.);
  }

  glim.SetMinimum(0.5); glim.SetMaximum(9000); //fb
  glim.SetTitle(title.c_str());
  glim.GetXaxis()->SetTitleOffset(0.8); glim.GetYaxis()->SetTitleOffset(0.95); glim.GetZaxis()->SetTitleOffset(1.25);
  glim.GetXaxis()->SetTitleSize(0.051); glim.GetYaxis()->SetTitleSize(0.051); glim.GetZaxis()->SetTitleSize(0.042);


  TCanvas c("","",1200, 1100);
  c.cd();
  TLegend l(0.12, 0.6,0.84,0.9);
  l.SetTextSize(0.045);
  l.SetBorderSize(1); l.SetFillColor(kWhite);

  TLatex ltitle(0.12, 1.-0.5*c.GetTopMargin()-0.02, "#font[62]{CMS}");
  TLatex ltitle_prelim(0.12, 1.-0.5*c.GetTopMargin()-0.02, "#font[62]{CMS}#scale[0.76]{#font[52]{ Preliminary}}");
  TLatex rtitle(1.-c.GetRightMargin()-0.052, 1.-0.5*c.GetTopMargin()-0.02, "#scale[0.9]{137 fb^{-1} (13 TeV)}"); rtitle.SetTextFont(42);
  ltitle.SetNDC(); rtitle.SetNDC(); ltitle_prelim.SetNDC();
  ltitle.SetTextAlign(12); rtitle.SetTextAlign(32); ltitle_prelim.SetTextAlign(12);

  TLatex txtd(0.15,0.825,"pp#rightarrow "+chi03+" "+chi02+" #rightarrow HH "+chi01+" "+chi01);
  txtd.SetNDC(); txtd.SetTextFont(42); txtd.SetTextSize(0.045);
  c.SetRightMargin(0.16);
  c.SetLeftMargin(0.12); c.SetLogz();
  glim.Draw("colz");


  TGraph cexp = DrawContours(gexp, 2, 1, 2, 1.0, true);
  TGraph cup = DrawContours(gup, 2, 2, 2, 1.0, true);
  TGraph cdown = DrawContours(gdown, 2, 2, 2, 1.0, true);
  TGraph cup2 = DrawContours(gup2, 2, 3, 2, 1.0, true);
  TGraph cdown2 = DrawContours(gdown2, 2, 3, 2, 1.0, true);
  TGraph cobsup = DrawContours(gobsup, 1, 2, 3, 1., true);
  TGraph cobsdown = DrawContours(gobsdown, 1, 2, 2, 1., true);
  TGraph cobs = DrawContours(gobs, 1, 1, 2, 1., true);
  // TGraph cobsup2 = DrawContours(gobsup2, 1, 3, 5, 1., true);
  // TGraph cobsdown2 = DrawContours(gobsdown2, 1, 3, 5, 1., true);

  cobs.SetLineColor(kBlack);  cobs.SetLineWidth(2);
  cobsup.SetLineWidth(2);
  l.AddEntry((TObject*)0, "", "");
  l.AddEntry(&cexp, "Expected #pm1, #pm2 #sigma_{experiment}", "l");
  l.AddEntry(&cobs, "Observed #pm1 #sigma_{theory}", "l");

  //Add the stupid bands to the legend
  TLegend leg2(0.12,0.617,0.84,0.717);
  leg2.SetBorderSize(0); leg2.SetFillColor(0); leg2.SetFillStyle(0);
  leg2.AddEntry(&cobsup," ","l");

  TLegend leg3(0.12,0.583,0.84,0.683);
  leg3.SetBorderSize(0); leg3.SetFillColor(0); leg3.SetFillStyle(0);
  leg3.AddEntry(&cobsup," ","l");

  TLegend leg4(0.12,0.6885,0.84,0.7885);
  leg4.SetBorderSize(0); leg4.SetFillColor(0); leg4.SetFillStyle(0);
  leg4.AddEntry(&cdown," ","l");

  TLegend leg5(0.12,0.71,0.84,0.81);
  leg5.SetBorderSize(0); leg5.SetFillColor(0); leg5.SetFillStyle(0);
  leg5.AddEntry(&cdown," ","l");

  TLegend leg6(0.12,0.678,0.84,0.778);
  leg6.SetBorderSize(0); leg6.SetFillColor(0); leg6.SetFillStyle(0);
  leg6.AddEntry(&cdown2," ","l");

  TLegend leg7(0.12,0.72,0.84,0.82);
  leg7.SetBorderSize(0); leg7.SetFillColor(0); leg7.SetFillStyle(0);
  leg7.AddEntry(&cdown2," ","l");

  l.Draw("same");
  ltitle.Draw("same"); //Preliminary
  rtitle.Draw("same"); txtd.Draw("same");
  leg2.Draw("same"); leg3.Draw("same");
  leg4.Draw("same"); leg5.Draw("same");
  leg6.Draw("same"); leg7.Draw("same");

  string filebase = out_dir+model_+"_limitScan";
  string which = "_ComboData";
  filebase+=which;
  gPad->Update();
  glim.GetYaxis()->SetRangeUser(-0.01,630.0);
  gPad->Update();

  c.Print((filebase+".pdf").c_str());
  ltitle_prelim.Draw("same"); //Preliminary
  gPad->Update();
  c.Print((filebase+"_prelim.pdf").c_str());
}

void MakeLimitPlotAll(vector<double> vmx, vector<double> vmy, vector<double> vlim,
                        vector<double> vobsComb, vector<double> vobsBoost, vector<double> vobsRes
                        ) {
  SetupColors();

  string xparticle, yparticle;
  GetParticleNames(xparticle, yparticle);
  string title = ";m_{"+xparticle+"} [GeV];m_{"+yparticle+"} [GeV];95% CL upper limit on cross section [pb]";

  TGraph2D glim("", title.c_str(), vlim.size(), &vmx.at(0), &vmy.at(0), &vlim.at(0));
  TGraph2D gobsComb("", "Expected Limit, Combo", vobsComb.size(), &vmx.at(0), &vmy.at(0), &vobsComb.at(0));
  TGraph2D gobsBoost("", "Expected Limit, BoostOnly", vobsBoost.size(), &vmx.at(0), &vmy.at(0), &vobsBoost.at(0));
  TGraph2D gobsRes("", "Expected Limit, ResOnly", vobsRes.size(), &vmx.at(0), &vmy.at(0), &vobsRes.at(0));

  if (model_=="N1N2"){
    gobsComb.SetMinimum(0.000001); gobsComb.SetMaximum(2); gobsComb.SetNpx(38.); gobsComb.SetNpy(35.);
  }
  else {
    gobsComb.SetMinimum(0.000001); gobsComb.SetMaximum(2); gobsComb.SetNpx(38.); gobsComb.SetNpy(35.);
  }

  glim.SetMinimum(0.00001); glim.SetMaximum(2);
  glim.SetTitle(title.c_str());
  glim.GetYaxis()->SetTitleOffset(1.3);
  glim.GetXaxis()->SetTitleOffset(1.3);


  TLegend l(0.15,0.85,0.45,0.6);
  l.SetTextSize(0.04);
  l.SetBorderSize(0);
  TCanvas c("","",1200, 950);
  c.cd();
  c.SetRightMargin(0.15);

  TLatex ltitle(c.GetLeftMargin(), 1.-0.5*c.GetTopMargin(), "#font[62]{CMS}#scale[0.76]{#font[52]{ Supplementary}}");
  TLatex rtitle(1.-c.GetRightMargin()+0.1, 1.-0.5*c.GetTopMargin(), "#scale[0.8]{137 fb^{-1} (13 TeV)}");
  ltitle.SetNDC();
  rtitle.SetNDC();
  ltitle.SetTextAlign(12);
  rtitle.SetTextAlign(32);

  c.SetLeftMargin(0.12);
  c.SetLogz();
  glim.Draw("colz");

  glim.GetYaxis()->SetRangeUser(0,2000);
  glim.Draw("colz");
  glim.GetYaxis()->SetRangeUser(0,2000);

  TGraph cobsComb = DrawContours(gobsComb, 1, 1, 3, 1.0, true);
  TGraph cobsBoost = DrawContours(gobsBoost, 2, 1, 3, 1.0, true);
  TGraph cobsRes = DrawContours(gobsRes, 3, 1, 3, 1.0, true);

  cobsComb.SetLineColor(1);  cobsBoost.SetLineColor(2); cobsRes.SetLineColor(3);
  l.AddEntry(&cobsComb, "Combination", "l");
  l.AddEntry(&cobsBoost, "BoostedOnly", "l");
  l.AddEntry(&cobsRes, "ResolvedOnly", "l");

  l.Draw("same");
  ltitle.Draw("same");
  rtitle.Draw("same");

  string filebase = out_dir+model_+"_limit_scanEXPECTED";
  string which = "_ComboAll_Data";
  filebase+=which;

  gPad->Update();
  glim.GetZaxis()->SetTitleOffset(1.3);
  glim.GetYaxis()->SetRangeUser(0,2000);
  glim.GetYaxis()->SetTitleOffset(1.3);
  glim.GetXaxis()->SetTitleOffset(1.3);
  gPad->Update();
  c.Print((filebase+".pdf").c_str());
}

void GetParticleNames(string &xparticle, string &yparticle) {
  if (model_=="N1N2") {
    xparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{2}}}";
    yparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  }
  else {
    xparticle = "#tilde{g}";
    yparticle = "#lower[-0.12]{#tilde{#chi}}#lower[0.2]{#scale[0.85]{^{0}}}#kern[-1.3]{#scale[0.85]{_{1}}}";
  }
}

void Style(TGraph *g, int color, int style, float width) {
  g->SetLineColor(color);
  g->SetLineStyle(style);
  g->SetLineWidth(width);
}

TGraph DrawContours(TGraph2D &g2, int color, int style, double width,
                    double val, bool draw) {
  TGraph graph;
  TList *l;

  g2.GetHistogram();
  l = g2.GetContourList(val);
  if (l == nullptr || l->GetSize()==0) {
    std::cout<<"Contour list is empty"<<std::endl;
    return graph;
  }
  int max_points = -1;
  vector<TGraph*> g;
  int startLoop = 0;
  int endLoop = 1;
  int lSize = l->GetSize();
  if (lSize>=1){
    startLoop=1;
    endLoop=2;
  }
  for (int i = 0; i < l->GetSize(); ++i) {
    g.push_back(static_cast<TGraph*>(l->At(i)));
    Style(g[i], color, style, width);
    if (g[i] == nullptr) continue;
    int n_points = g[i]->GetN();
    if (n_points > max_points) {
      TGraph* old_graph = static_cast<TGraph*>(graph.Clone());
      if (i>0) ReverseGraph(*(g[i]));
      graph = *(joinGraphs(old_graph, g[i]));
      graph.RemovePoint(graph.GetN()-1);
      max_points = n_points;
    }
    if (draw) g[i]->Draw("L same");
  }
  graph.SetTitle(g2.GetTitle());
  graph.SetLineColor(color);
  graph.SetLineWidth(width);
  graph.SetLineStyle(style);
  return graph;
}

TGraph* joinGraphs(TGraph *graph1, TGraph *graph2) {
  TGraph *graph = new TGraph;
  double mglu, mlsp;
  for (int point(0); point < graph1->GetN(); point++) {
    graph1->GetPoint(point, mglu, mlsp);
    graph->SetPoint(graph->GetN(), mglu, mlsp);
  } // Points in graph1
  for (int point(0); point < graph2->GetN(); point++) {
    graph2->GetPoint(point, mglu, mlsp);
    graph->SetPoint(graph->GetN(), mglu, mlsp);
  } // Points in graph1
  graph1->GetPoint(0, mglu, mlsp);
  graph->SetPoint(graph->GetN(), mglu, mlsp);
  TString gname = graph1->GetName(); gname += graph2->GetName();
  graph->SetName(gname);

  return graph;
}

void ReverseGraph(TGraph &graph) {
  int np(graph.GetN());
  double mglu, mlsp;
  vector<double> mglus, mlsps;
  for (int point(np-1); point >= 0; point--) {
    graph.GetPoint(point, mglu, mlsp);
    mglus.push_back(mglu);
    mlsps.push_back(mlsp);
  }
  for (int point(0); point < np; point++)
    graph.SetPoint(point, mglus[point], mlsps[point]);
}

void SetupColors() {
  const unsigned num = 5;
  const int bands = 255;
  int colors[bands];
  double stops[num] = {0.00, 0.34, 0.61, 0.84, 1.00};
  double red[num] = {0.50, 0.50, 1.00, 1.00, 1.00};
  double green[num] = {0.50, 1.00, 1.00, 0.60, 0.50};
  double blue[num] = {1.00, 1.00, 0.50, 0.40, 0.50};
  int fi = TColor::CreateGradientColorTable(num,stops,red,green,blue,bands);
  for (int i = 0; i < bands; ++i) colors[i] = fi+i;
  gStyle->SetNumberContours(bands);
  gStyle->SetPalette(bands, colors);
}

void SaveRootFile() {
  TString  txtname(src_dir+"/limitsCombined_"+model_+"_data.txt");
  TString  txtname2(src_dir+"/limitsBoostOnly_"+model_+"_data.txt");
  TString  txtname3(src_dir+"/limitsResOnly_"+model_+"_data.txt");
  ifstream infile(txtname); string line;
  ifstream infile2(txtname2); string line2;
  ifstream infile3(txtname3); string line3;
  vector<double> vmx, vmy, vxsec, vobs, vobsup, vobsdown, vexp, vup, vdown, vup2, vdown2, vlim;
  vector<double> vmx_boost, vmy_boost, vxsec_boost, vobs_boost, vobsup_boost, vobsdown_boost, vexp_boost, vup_boost, vdown_boost, vup2_boost, vdown2_boost, vlim_boost;
  vector<double> vmx_res, vmy_res, vxsec_res, vobs_res, vobsup_res, vobsdown_res, vexp_res, vup_res, vdown_res, vup2_res, vdown2_res, vlim_res;
  vector<double> vlimexp_res, vlimexp_boost, vlimexp;

  while(getline(infile, line)) { //combo
    istringstream iss(line);
    double pmx, pmy, pxsec, pxsecunc, pobs, pexp, pup, pdown, pup2, pdown2;
    iss >> pmx >> pmy >> pxsec >> pxsecunc >> pobs >> pexp >> pup >> pdown >> pup2 >> pdown2;

    //Regular N2N1-only xsec OR gluino
    vmx.push_back(pmx);
    vmy.push_back(pmy);
    vxsec.push_back(pxsec);
    vobs.push_back(pobs);
    vobsup.push_back(pobs/(1+pxsecunc));
    vobsdown.push_back(pobs/(1-pxsecunc));

    vexp.push_back(pexp);
    vup.push_back(pup);
    vdown.push_back(pdown);
    vup2.push_back(pup2);
    vdown2.push_back(pdown2);

    vlim.push_back(pxsec*pobs);
    vlimexp.push_back(pxsec*pexp);
  }
  infile.close();

  while(getline(infile2, line2)) { //boost
    istringstream iss2(line2);
    double pmx, pmy, pxsec, pxsecunc, pobs, pexp, pup, pdown, pup2, pdown2;
    iss2 >> pmx >> pmy >> pxsec >> pxsecunc >> pobs >> pexp >> pup >> pdown >> pup2 >> pdown2;

    //Regular N2N1-only xsec OR gluino
    vmx_boost.push_back(pmx);
    vmy_boost.push_back(pmy);
    vxsec_boost.push_back(pxsec);
    vobs_boost.push_back(pobs);
    vobsup_boost.push_back(pobs/(1+pxsecunc));
    vobsdown_boost.push_back(pobs/(1-pxsecunc));

    vexp_boost.push_back(pexp);
    vup_boost.push_back(pup);
    vdown_boost.push_back(pdown);
    vup2_boost.push_back(pup2);
    vdown2_boost.push_back(pdown2);

    vlim_boost.push_back(pxsec*pobs);
    vlimexp_boost.push_back(pxsec*pexp);

  }
  infile2.close();

  while(getline(infile3, line3)) { //res
    istringstream iss3(line3);
    double pmx, pmy, pxsec, pxsecunc, pobs, pexp, pup, pdown, pup2, pdown2;
    iss3 >> pmx >> pmy >> pxsec >> pxsecunc >> pobs >> pexp >> pup >> pdown >> pup2 >> pdown2;

    //Regular N2N1-only xsec OR gluino
    vmx_res.push_back(pmx);
    vmy_res.push_back(pmy);
    vxsec_res.push_back(pxsec);
    vobs_res.push_back(pobs);
    vobsup_res.push_back(pobs/(1+pxsecunc));
    vobsdown_res.push_back(pobs/(1-pxsecunc));

    vexp_res.push_back(pexp);
    vup_res.push_back(pup);
    vdown_res.push_back(pdown);
    vup2_res.push_back(pup2);
    vdown2_res.push_back(pdown2);

    vlim_res.push_back(pxsec*pobs);
    vlimexp_res.push_back(pxsec*pexp);
  }
  infile3.close();

  string xparticle, yparticle;
  GetParticleNames(xparticle, yparticle);
  string title = ";m("+xparticle+") [GeV];m("+yparticle+") [GeV];Cross section upper limit (95% CL) [pb]";

  TGraph2D glim("XSecUpperLimit_Combo", title.c_str(), vlim.size(), &vmx.at(0), &vmy.at(0), &vlim.at(0));
  TGraph2D glimexp("XSecUpperLimitExpected_Combo", title.c_str(), vlimexp.size(), &vmx.at(0), &vmy.at(0), &vlimexp.at(0));
  TGraph2D gobs("ObservedLimit_Combo", "Observed Limit", vobs.size(), &vmx.at(0), &vmy.at(0), &vobs.at(0));
  TGraph2D gobsup("ObservedLimit1SigmaUp_Combo", "Observed +1#sigma Limit", vobsup.size(), &vmx.at(0), &vmy.at(0), &vobsup.at(0));
  TGraph2D gobsdown("ObservedLimit1SigmaDown_Combo", "Observed -1#sigma Limit", vobsdown.size(), &vmx.at(0), &vmy.at(0), &vobsdown.at(0));
  TGraph2D gexp("ExpectedLimit_Combo", "Expected Limit", vexp.size(), &vmx.at(0), &vmy.at(0), &vexp.at(0));
  TGraph2D gup("ExpectedLimit1SigmaUp_Combo", "Expected +1#sigma Limit", vup.size(), &vmx.at(0), &vmy.at(0), &vup.at(0));
  TGraph2D gdown("ExpectedLimit1SigmaDown_Combo", "Expected -1#sigma Limit", vdown.size(), &vmx.at(0), &vmy.at(0), &vdown.at(0));
  TGraph2D gup2("ExpectedLimit2SigmaUp_Combo", "Expected +2#sigma Limit", vup2.size(), &vmx.at(0), &vmy.at(0), &vup2.at(0));
  TGraph2D gdown2("ExpectedLimit2SigmaDown_Combo", "Expected -2#sigma Limit", vdown2.size(), &vmx.at(0), &vmy.at(0), &vdown2.at(0));

  TGraph2D glim_boost("XSecUpperLimit_Boosted", title.c_str(), vlim_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vlim_boost.at(0));
  TGraph2D glimexp_boost("XSecUpperLimitExpected_Boosted", title.c_str(), vlimexp_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vlimexp_boost.at(0));
  TGraph2D gobs_boost("ObservedLimit_Boosted", "Observed Limit", vobs_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vobs_boost.at(0));
  TGraph2D gobsup_boost("ObservedLimit1SigmaUp_Boosted", "Observed +1#sigma Limit", vobsup_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vobsup_boost.at(0));
  TGraph2D gobsdown_boost("ObservedLimit1SigmaDown_Boosted", "Observed -1#sigma Limit", vobsdown_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vobsdown_boost.at(0));
  TGraph2D gexp_boost("ExpectedLimit_Boosted", "Expected Limit", vexp_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vexp_boost.at(0));
  TGraph2D gup_boost("ExpectedLimit1SigmaUp_Boosted", "Expected +1#sigma Limit", vup_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vup_boost.at(0));
  TGraph2D gdown_boost("ExpectedLimit1SigmaDown_Boosted", "Expected -1#sigma Limit", vdown_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vdown_boost.at(0));
  TGraph2D gup2_boost("ExpectedLimit2SigmaUp_Boosted", "Expected +2#sigma Limit", vup2_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vup2_boost.at(0));
  TGraph2D gdown2_boost("ExpectedLimit2SigmaDown_Boosted", "Expected -2#sigma Limit", vdown2_boost.size(), &vmx_boost.at(0), &vmy_boost.at(0), &vdown2_boost.at(0));

  TGraph2D glim_res("XSecUpperLimit_Resolved", title.c_str(), vlim_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vlim_res.at(0));
  TGraph2D glimexp_res("XSecUpperLimitExpected_Resolved", title.c_str(), vlimexp_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vlimexp_res.at(0));
  TGraph2D gobs_res("ObservedLimit_Resolved", "Observed Limit", vobs_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vobs_res.at(0));
  TGraph2D gobsup_res("ObservedLimit1SigmaUp_Resolved", "Observed +1#sigma Limit", vobsup_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vobsup_res.at(0));
  TGraph2D gobsdown_res("ObservedLimit1SigmaDown_Resolved", "Observed -1#sigma Limit", vobsdown_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vobsdown_res.at(0));
  TGraph2D gexp_res("ExpectedLimit_Resolved", "Expected Limit", vexp_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vexp_res.at(0));
  TGraph2D gup_res("ExpectedLimit1SigmaUp_Resolved", "Expected +1#sigma Limit", vup_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vup_res.at(0));
  TGraph2D gdown_res("ExpectedLimit1SigmaDown_Resolved", "Expected -1#sigma Limit", vdown_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vdown_res.at(0));
  TGraph2D gup2_res("ExpectedLimit2SigmaUp_Resolved", "Expected +2#sigma Limit", vup2_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vup2_res.at(0));
  TGraph2D gdown2_res("ExpectedLimit2SigmaDown_Resolved", "Expected -2#sigma Limit", vdown2_res.size(), &vmx_res.at(0), &vmy_res.at(0), &vdown2_res.at(0));

  // Save ROOT file with all limits
  TString filename = out_dir+"CMS-SUS-20-004_Figure_013.root";
  TFile * fNEW = new TFile(filename, "recreate");
  glim_res.GetHistogram()->Write();
  glim_boost.GetHistogram()->Write();
  glim.GetHistogram()->Write();
  glimexp_res.GetHistogram()->Write();
  glimexp_boost.GetHistogram()->Write();
  glimexp.GetHistogram()->Write();


  TGraph cobs = DrawContours(gobs, 1, 1, 2, 1., false);
  TGraph cobsup = DrawContours(gobsup, 1, 2, 3, 1., false);
  TGraph cobsdown = DrawContours(gobsdown, 1, 2, 2, 1., false);
  TGraph cexp = DrawContours(gexp, 2, 1, 2, 1.0, false);
  TGraph cup = DrawContours(gup, 2, 2, 2, 1.0, false);
  TGraph cdown = DrawContours(gdown, 2, 2, 2, 1.0, false);
  TGraph cup2 = DrawContours(gup2, 2, 3, 2, 1.0, false);
  TGraph cdown2 = DrawContours(gdown2, 2, 3, 2, 1.0, false);
  TGraph cobs_boost = DrawContours(gobs_boost, 1, 1, 2, 1., false);
  TGraph cobsup_boost = DrawContours(gobsup_boost, 1, 2, 3, 1., false);
  TGraph cobsdown_boost = DrawContours(gobsdown_boost, 1, 2, 2, 1., false);
  TGraph cexp_boost = DrawContours(gexp_boost, 2, 1, 2, 1.0, false);
  TGraph cup_boost = DrawContours(gup_boost, 2, 2, 2, 1.0, false);
  TGraph cdown_boost = DrawContours(gdown_boost, 2, 2, 2, 1.0, false);
  TGraph cup2_boost = DrawContours(gup2_boost, 2, 3, 2, 1.0, false);
  TGraph cdown2_boost = DrawContours(gdown2_boost, 2, 3, 2, 1.0, false);
  TGraph cobs_res = DrawContours(gobs_res, 1, 1, 2, 1., false);
  TGraph cobsup_res = DrawContours(gobsup_res, 1, 2, 3, 1., false);
  TGraph cobsdown_res = DrawContours(gobsdown_res, 1, 2, 2, 1., false);
  TGraph cexp_res = DrawContours(gexp_res, 2, 1, 2, 1.0, false);
  TGraph cup_res = DrawContours(gup_res, 2, 2, 2, 1.0, false);
  TGraph cdown_res = DrawContours(gdown_res, 2, 2, 2, 1.0, false);
  TGraph cup2_res = DrawContours(gup2_res, 2, 3, 2, 1.0, false);
  TGraph cdown2_res = DrawContours(gdown2_res, 2, 3, 2, 1.0, false);

  cobs.Write("ObservedLimit_Combo"); cobsup.Write("ObservedLimit1SigmaUp_Combo"); cobsdown.Write("ObservedLimit1SigmaDown_Combo");
  cexp.Write("ExpectedLimit_Combo"); cup.Write("ExpectedLimit1SigmaUp_Combo"); cdown.Write("ExpectedLimit1SigmaDown_Combo");
  cup2.Write("ExpectedLimit2SigmaUp_Combo"); cdown2.Write("ExpectedLimit2SigmaDown_Combo");

  cobs_boost.Write("ObservedLimit_Boosted"); cobsup_boost.Write("ObservedLimit1SigmaUp_Boosted"); cobsdown_boost.Write("ObservedLimit1SigmaDown_Boosted");
  cexp_boost.Write("ExpectedLimit_Boosted"); cup_boost.Write("ExpectedLimit1SigmaUp_Boosted"); cdown_boost.Write("ExpectedLimit1SigmaDown_Boosted");
  cup2_boost.Write("ExpectedLimit2SigmaUp_Boosted"); cdown2_boost.Write("ExpectedLimit2SigmaDown_Boosted");

  cobs_res.Write("ObservedLimit_Resolved"); cobsup_res.Write("ObservedLimit1SigmaUp_Resolved"); cobsdown_res.Write("ObservedLimit1SigmaDown_Resolved");
  cexp_res.Write("ExpectedLimit_Resolved"); cup_res.Write("ExpectedLimit1SigmaUp_Resolved"); cdown_res.Write("ExpectedLimit1SigmaDown_Resolved");
  cup2_res.Write("ExpectedLimit2SigmaUp_Resolved"); cdown2_res.Write("ExpectedLimit2SigmaDown_Resolved");

  fNEW->Close();
}

bool doesFileExist (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}
