#include <TH1F.h>
#include <THStack.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TString.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <TGraphAsymmErrors.h>
#include <TLegend.h>
#include <TLine.h>
#include <TColor.h>
#include <TPie.h>
#include <TF1.h>
#include <TStyle.h>

void makeABCDPlot(vector<TH1F*> dem_histos, TString type, TString tagType, TString year);
void makeRpfPlot(vector<TH1F*> dem_histos, TString bkgType, TString jetType, TString tagType);
void makeStackPlot(TH1F* h_QCD,TH1F* h_TT,TH1F* h_WJets,TH1F* h_ZJets,TH1F* h_SnglT, TH1F* h_T5HH1600,TH1F* h_T5HH2000,TH1F* h_T5HH2200,TH1F* h_TChiHH500,TH1F* h_TChiHH800,TH1F* h_TChiHH1000,TString which);
void makeSingleLeptStackPlot(TH1F* h_TT,TH1F* h_WJets,TH1F* h_SnglT,TH1F* h_data,TString which);
void makePhotonStackPlot(TH1F* h_QCD,TH1F* h_GJets,TString which);
void makeMETStack(TH1F* h_QCD,TH1F* h_TT,TH1F* h_WJets,TH1F* h_ZJets, TH1F* h_SnglT, TH1F* h_T5HH1600,TH1F* h_T5HH2000,TH1F* h_T5HH2200,TH1F* h_TChiHH500,TH1F* h_TChiHH800,TH1F* h_TChiHH1000,TString tagType);
void makeFullBkgClosure(vector<TH1F*> dem_histos, TString bkgType, TString tagType, TString year);
void makeMETNorm(vector<TH1F*> dem_histos, TString bkgType, TString tagType);
void makeMETNormCompare(vector<TH1F*> dem_histos, TString bkgType);
void makeMETShapeComp(vector<TH1F*> MC_histos, vector<TH1F*> data_histos);
TH1F* make0EventUncSum(vector<TH1F*> dem_histos); //in this order: QCD, TT, WJets,ZJets,SnglT
TH1F* make0EventUncSum_photon(vector<TH1F*> dem_histos); //in this order: QCD, TT, WJets,ZJets,SnglT
TH1F* make0EventUncSum_1l(vector<TH1F*> dem_histos); //in this order: SnglT, TT, WJets
void styleCanvas(TCanvas *can_h);
void addLumiCanv(TCanvas * &can_h);
void styleGraph(TGraph *graph);
void DrawOverflow(TH1F* &h);
void NormalizeHisto(TH1F* &h);
void tableOfYields(vector<TH1F*> dem_histos, TString bkgType); //A, A1, B, B1, C, D
void giantTableOfYields(vector< vector<TH1F*> > dem_histos);
void tableOfMETNorm(vector<TH1F*> dem_histos, TString bkgType); //A, A1, B, B1, D, DOpt, C, COpt
void pieChart(vector<TH1F*> h_QCD, vector<TH1F*> h_WJets, vector<TH1F*> h_ZJets, vector<TH1F*> h_TT, vector<TH1F*> h_SnglT, TString regionLabel, TString bin);
void pieChart1l(vector<TH1F*> h_WJets, vector<TH1F*> h_TT, vector<TH1F*> h_SnglT, TString regionLabel, TString bin);
void pieChartPhoton(vector<TH1F*> h_GJets, vector<TH1F*> h_QCD, TString regionLabel, TString bin);
void massCorrelations(vector<TH1F*> dem_histos,vector<TH1F*> dem_histos_data, TString bkgType); //A, B, C, D
void makeMCvDataComp(TH1F* h_MC, TH1F* h_data, TString region, TString type);
void makeMCStackvDataComp(vector<TH1F*> h_MC, TH1F * h_MC_sum, TH1F* h_data,TH1F* h_sig1,TH1F* h_sig2, TString region, TString type, bool save); //Vector length: 5(0l), 3(1l), 2(photon)
void makeMCStackvDataMET(vector<TH1F*> h_MC, TH1F * h_MC_sum, TH1F* h_data,TH1F* h_2HSR,TH1F* h_1HSR);

void justBB(TH1F* h_bkgOrig, TH1F* h_sigOrig, TString bkgType, TString sigType, TString sigMass);
void tableOfFullPred(vector<TH1F*> h_histos, TString bkgType);

//for style
double W = 800; double H = 600;
double T = 0.08*H; double B = 0.12*H;
double L = 0.12*W; double R = 0.04*W;
double up_height = 0.8; // double dw_correction = 1.30;
double dw_correction = 1.18; double font_size_dw  = 0.1;
double dw_height = (1. - up_height) * dw_correction;
double dw_height_offset = 0.02;

TString cmsText = "CMS"; float cmsTextFont = 61; float cmsTextSize = 0.05;
bool writeExtraText = true;
TString extraText   = "Preliminary"; float extraTextFont = 52;
TString lumiText = "137 fb^{-1} (13 TeV)";
// TString lumiText = "35.9 fb^{-1} (13 TeV)";
float lumiTextSize = 0.6; float lumiTextOffset = 0.2;
float relPosX = 0.045; float relPosY = 0.035; float relExtraDY = 1.2; float relExtraDX = 1.5;
float extraOverCmsTextSize  = 0.76; // ratio of "CMS" and extra text size
float posX_= L + relPosX*(1-L-R);
float posY_ = 1-T - relPosY*(1-T-B);
int align_ = 10*2+ 3;

Int_t ci = TColor::GetColor(85, 82, 163);
Int_t ci2 = TColor::GetColor(255, 200, 0);
Int_t ci3 = TColor::GetColor(21, 117, 48);
Int_t c_error = TColor::GetColor(209, 209, 209);
Int_t c_errorRED = TColor::GetColor(235, 213, 213);
Int_t c_errorBLUE = TColor::GetColor(159, 172, 236);

//for running
ofstream myfile;
bool runABCDPlots = false;
bool runFullBkg = false;
bool runRPFPlots = false;
bool runStacks = false;
bool runMETNorm = true; //need for data v MC comparisons
bool runTableOfYields = false;
bool runPies = false;
bool showNoCutLine = false;
bool runPure = false;
bool runMassCorrelations = false;

bool savePDFs = false;
bool runSignal = true;

string whichRegion = "signal";
// string whichRegion = "singleLept";
// string whichRegion = "photon";


TString year = "Run2";

// TFile * f = TFile::Open("ALPHABET_V18_2BoostedH.root");
// TFile * f = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_Final/ALPHABET_0l.root");
TFile * f = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0l.root");
TFile * fVetoData = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0lData.root");
// TFile * f = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_Final/ALPHABETMC2016_V18_data0.root");



TFile * fSignal = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_noVeto_FastSIMSFs/ALPHABET_1DSignal.root"); //FastSIM TChiHH
// TFile * fSignal = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_FastSIMSFs/ALPHABET_1DSignal.root"); //FastSIM TChiHH
TFile * fSignal2 = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/T5HH1D_FullSIM/ALPHABET_1DT5HH_FullSIM.root"); //FullSIM T5HH


// TFile * fPhoton = TFile::Open("ALPHABET_V18_photon.root");
TFile * fPhoton = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/allSFsIncluded/ALPHABET_photon.root");
// TFile * fPhoton = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/updateTrigger/ALPHABET_photon.root");

// TFile * fPhoton = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/MET200/ALPHABET_photon.root");




TFile * fSingleLept = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/singleLept_WmassMET300_data/ALPHABET_1l_WmassMET300_1b_2018.root");
// TFile * fSingleLept = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/singleLept_opt2/ALPHABET_1l.root");
// TFile * fSingleLept = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/updateTrigger/ALPHABET_1l.root");

// TFile * fSingleLept = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/AK8Jet_AK4event/ALPHABET_1l.root");
// TFile * fSingleLept = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/MET200/ALPHABET_1l.root");

TString fout_name = "ABCDPlots_"+whichRegion+"_ANPlots.root";
// TString fout_name = "ABCDPlots_"+whichRegion+"_testDeepAK8.root";
// TString fout_name = "ABCDPlots_"+whichRegion+"_myClean.root";
TFile* fout = new TFile(fout_name,"recreate");

TDirectory *cdABCDReg  = fout->mkdir("ABCDRegion");
TDirectory *cdABCD  = fout->mkdir("ABCD");
TDirectory *cdClose = fout->mkdir("Closure");
TDirectory *cdRPFFinal  = fout->mkdir("RPF");
TDirectory *cdRPF  = fout->mkdir("RPF_Support");
TDirectory *cdPie  = fout->mkdir("PieCharts");
TDirectory *cdMET  = fout->mkdir("METShape");
TDirectory *cdDatavMC  = fout->mkdir("DatavMC");
TDirectory *cdStack  = fout->mkdir("StackPlots");
TDirectory *cdOther  = fout->mkdir("OtherPlots");



void runABCD() {
  gStyle->SetTextFont(52);

  if (whichRegion=="singleLept" || whichRegion=="photon") runSignal = false;

  TH1F * h_A_data; TH1F * h_B_data; TH1F * h_A1_data; TH1F * h_B1_data; TH1F *h_C_data; TH1F *h_D_data; TH1F *h_Opt1_data; TH1F * h_baseline_MET_data; TH1F * h_0H_MET_data; TH1F * h_1H_MET_data; TH1F * h_2H_MET_data; TH1F * h_H_MET_data;

  TH1F * h_nH_sum = new TH1F("nH_sum","nH_sum", 3,0,4);
  TH1F * h_nH_data = new TH1F("nH_data","nH_data", 3,0,4);
  h_nH_sum->GetXaxis()->SetBinLabel(1,"0H");h_nH_sum->GetXaxis()->SetBinLabel(2,"1H");h_nH_sum->GetXaxis()->SetBinLabel(3,"2H");
  h_nH_data->GetXaxis()->SetBinLabel(1,"0H");h_nH_data->GetXaxis()->SetBinLabel(2,"1H");h_nH_data->GetXaxis()->SetBinLabel(3,"2H");

  TH1F * h_A_sum; TH1F * h_B_sum; TH1F * h_A1_sum; TH1F * h_B1_sum; TH1F *h_C_sum; TH1F *h_D_sum; TH1F * h_baseline_MET_sum; TH1F * h_0H_MET_sum; TH1F * h_1H_MET_sum; TH1F * h_2H_MET_sum; TH1F * h_H_MET_sum;
  TH1F * hP_A_sum; TH1F * hP_B_sum; TH1F * hP_A1_sum; TH1F * hP_B1_sum; TH1F *hP_C_sum; TH1F *hP_D_sum;
  TH1F * h_A5_sum; TH1F * h_B5_sum; TH1F * h_A15_sum; TH1F * h_B15_sum; TH1F *h_C5_sum; TH1F *h_D5_sum;
  TH1F * h_A_QCD; TH1F * h_B_QCD; TH1F * h_A1_QCD; TH1F * h_B1_QCD; TH1F * h_C_QCD; TH1F * h_D_QCD; TH1F * h_baseline_MET_QCD;  TH1F * h_0H_MET_QCD; TH1F * h_1H_MET_QCD; TH1F * h_2H_MET_QCD; TH1F * h_H_MET_QCD;
  TH1F * hP_A_QCD; TH1F * hP_B_QCD; TH1F * hP_A1_QCD; TH1F * hP_B1_QCD; TH1F *hP_C_QCD; TH1F *hP_D_QCD;
  TH1F * h_A_SnglT; TH1F * h_B_SnglT; TH1F * h_A1_SnglT; TH1F * h_B1_SnglT; TH1F * h_C_SnglT; TH1F * h_D_SnglT; TH1F * h_baseline_MET_SnglT;  TH1F * h_0H_MET_SnglT; TH1F * h_1H_MET_SnglT; TH1F * h_2H_MET_SnglT; TH1F * h_H_MET_SnglT;
  TH1F * h_A_GJets; TH1F * h_B_GJets; TH1F * h_A1_GJets; TH1F * h_B1_GJets; TH1F * h_C_GJets; TH1F * h_D_GJets; TH1F * h_baseline_MET_GJets; TH1F * h_H_MET_GJets;
  TH1F * hP_A_GJets; TH1F * hP_B_GJets; TH1F * hP_A1_GJets; TH1F * hP_B1_GJets; TH1F *hP_C_GJets; TH1F *hP_D_GJets;
  TH1F * h_A_TT; TH1F * h_B_TT; TH1F * h_A1_TT; TH1F * h_B1_TT; TH1F * h_C_TT; TH1F * h_D_TT; TH1F * h_baseline_MET_TT; TH1F * h_0H_MET_TT; TH1F * h_1H_MET_TT; TH1F * h_2H_MET_TT; TH1F * h_H_MET_TT;
  TH1F * h_A_TT_Di; TH1F * h_B_TT_Di; TH1F * h_A1_TT_Di; TH1F * h_B1_TT_Di; TH1F * h_C_TT_Di; TH1F * h_D_TT_Di;
  TH1F * h_A_TT_SL; TH1F * h_B_TT_SL; TH1F * h_A1_TT_SL; TH1F * h_B1_TT_SL; TH1F * h_C_TT_SL; TH1F * h_D_TT_SL;
  TH1F * h_A_WJets; TH1F * h_B_WJets; TH1F * h_A1_WJets; TH1F * h_B1_WJets; TH1F * h_C_WJets; TH1F * h_D_WJets; TH1F * h_baseline_MET_WJets; TH1F * h_0H_MET_WJets; TH1F * h_1H_MET_WJets; TH1F * h_2H_MET_WJets; TH1F * h_H_MET_WJets;
  TH1F * h_A_ZJets; TH1F * h_B_ZJets; TH1F * h_A1_ZJets; TH1F * h_B1_ZJets; TH1F * h_C_ZJets; TH1F * h_D_ZJets; TH1F * h_baseline_MET_ZJets; TH1F * h_0H_MET_ZJets; TH1F * h_1H_MET_ZJets; TH1F * h_2H_MET_ZJets; TH1F * h_H_MET_ZJets;

  TH1F * h_A_T5HH1600; TH1F * h_B_T5HH1600; TH1F * h_A1_T5HH1600; TH1F * h_B1_T5HH1600; TH1F * h_C_T5HH1600; TH1F * h_D_T5HH1600; TH1F * h_baseline_MET_T5HH1600; TH1F * h_0H_MET_T5HH1600; TH1F * h_1H_MET_T5HH1600; TH1F * h_2H_MET_T5HH1600; TH1F * h_H_MET_T5HH1600;
  TH1F * h_A_T5HH2000; TH1F * h_B_T5HH2000; TH1F * h_A1_T5HH2000; TH1F * h_B1_T5HH2000; TH1F * h_C_T5HH2000; TH1F * h_D_T5HH2000; TH1F * h_baseline_MET_T5HH2000; TH1F * h_H_MET_T5HH2000;
  TH1F * h_A_T5HH2200; TH1F * h_B_T5HH2200; TH1F * h_A1_T5HH2200; TH1F * h_B1_T5HH2200; TH1F * h_C_T5HH2200; TH1F * h_D_T5HH2200; TH1F * h_baseline_MET_T5HH2200; TH1F * h_H_MET_T5HH2200;
  TH1F * h_A_TChiHH500; TH1F * h_B_TChiHH500; TH1F * h_A1_TChiHH500; TH1F * h_B1_TChiHH500; TH1F * h_C_TChiHH500; TH1F * h_D_TChiHH500; TH1F * h_baseline_MET_TChiHH500; TH1F * h_0H_MET_TChiHH500; TH1F * h_1H_MET_TChiHH500; TH1F * h_2H_MET_TChiHH500; TH1F * h_H_MET_TChiHH500;
  TH1F * h_A_TChiHH800; TH1F * h_B_TChiHH800; TH1F * h_A1_TChiHH800; TH1F * h_B1_TChiHH800; TH1F * h_C_TChiHH800; TH1F * h_D_TChiHH800; TH1F * h_baseline_MET_TChiHH800; TH1F * h_H_MET_TChiHH800;
  TH1F * h_A_TChiHH1000; TH1F * h_B_TChiHH1000; TH1F * h_A1_TChiHH1000; TH1F * h_B1_TChiHH1000; TH1F * h_C_TChiHH1000; TH1F * h_D_TChiHH1000; TH1F * h_baseline_MET_TChiHH1000; TH1F * h_H_MET_TChiHH1000;


  TH1F * h_J1M_doubletagSR_sum; TH1F * h_J1M_doubletagSB_sum; TH1F * h_J1M_tagSR_sum; TH1F * h_J1M_tagSB_sum; TH1F * h_J1M_antitagSR_sum; TH1F * h_J1M_antitagSB_sum;
  TH1F * h_J2M_doubletagSR_sum; TH1F * h_J2M_doubletagSB_sum; TH1F * h_J2M_tagSR_sum; TH1F * h_J2M_tagSB_sum; TH1F * h_J2M_antitagSR_sum; TH1F * h_J2M_antitagSB_sum;
  TH1F * h_J1J2M_doubletagSR_sum; TH1F * h_J1J2M_doubletagSB_sum; TH1F * h_J1J2M_tagSR_sum; TH1F * h_J1J2M_tagSB_sum;
  TH1F * h_J1J2M_antitagSR_sum; TH1F * h_J1J2M_antitagSB_sum;

  TH1F * h_J1M_mjBins_doubletagSR_sum; TH1F * h_J1M_mjBins_doubletagSB_sum; TH1F * h_J1M_mjBins_tagSR_sum; TH1F * h_J1M_mjBins_tagSB_sum; TH1F * h_J1M_mjBins_antitagSR_sum; TH1F * h_J1M_mjBins_antitagSB_sum;
  TH1F * h_J2M_mjBins_doubletagSR_sum; TH1F * h_J2M_mjBins_doubletagSB_sum; TH1F * h_J2M_mjBins_tagSR_sum; TH1F * h_J2M_mjBins_tagSB_sum; TH1F * h_J2M_mjBins_antitagSR_sum; TH1F * h_J2M_mjBins_antitagSB_sum;

  TH1F * h_baseline_j1mass_TT; TH1F * h_J1M_doubletagSR_TT; TH1F * h_J1M_doubletagSB_TT; TH1F * h_J1M_tagSR_TT; TH1F * h_J1M_tagSB_TT; TH1F * h_J1M_antitagSR_TT; TH1F * h_J1M_antitagSB_TT;
  TH1F * h_baseline_j2mass_TT; TH1F * h_J2M_doubletagSR_TT; TH1F * h_J2M_doubletagSB_TT; TH1F * h_J2M_tagSR_TT; TH1F * h_J2M_tagSB_TT; TH1F * h_J2M_antitagSR_TT; TH1F * h_J2M_antitagSB_TT;
  TH1F * h_J1M_mjBins_doubletagSR_TT; TH1F * h_J1M_mjBins_doubletagSB_TT; TH1F * h_J1M_mjBins_tagSR_TT; TH1F * h_J1M_mjBins_tagSB_TT; TH1F * h_J1M_mjBins_antitagSR_TT; TH1F * h_J1M_mjBins_antitagSB_TT;
  TH1F * h_J2M_mjBins_doubletagSR_TT; TH1F * h_J2M_mjBins_doubletagSB_TT; TH1F * h_J2M_mjBins_tagSR_TT; TH1F * h_J2M_mjBins_tagSB_TT; TH1F * h_J2M_mjBins_antitagSR_TT; TH1F * h_J2M_mjBins_antitagSB_TT;
  TH1F * h_0H_j1mass_TT; TH1F * h_0H_j2mass_TT; TH1F * h_1H_j1mass_TT; TH1F * h_1H_j2mass_TT; TH1F * h_2H_j1mass_TT; TH1F * h_2H_j2mass_TT; TH1F * h_H_j1mass_TT; TH1F * h_H_j2mass_TT;
  TH1F * h_0H_jmass_TT;  TH1F * h_1H_jmass_TT;  TH1F * h_2H_jmass_TT; TH1F * h_baseline_jmass_TT;

  TH1F * h_J1M_doubletagSR_TT_Di; TH1F * h_J1M_doubletagSB_TT_Di; TH1F * h_J1M_tagSR_TT_Di; TH1F * h_J1M_tagSB_TT_Di; TH1F * h_J1M_antitagSR_TT_Di; TH1F * h_J1M_antitagSB_TT_Di;
  TH1F * h_J2M_doubletagSR_TT_Di; TH1F * h_J2M_doubletagSB_TT_Di; TH1F * h_J2M_tagSR_TT_Di; TH1F * h_J2M_tagSB_TT_Di; TH1F * h_J2M_antitagSR_TT_Di; TH1F * h_J2M_antitagSB_TT_Di;
  TH1F * h_J1M_mjBins_doubletagSR_TT_Di; TH1F * h_J1M_mjBins_doubletagSB_TT_Di; TH1F * h_J1M_mjBins_tagSR_TT_Di; TH1F * h_J1M_mjBins_tagSB_TT_Di; TH1F * h_J1M_mjBins_antitagSR_TT_Di; TH1F * h_J1M_mjBins_antitagSB_TT_Di;
  TH1F * h_J2M_mjBins_doubletagSR_TT_Di; TH1F * h_J2M_mjBins_doubletagSB_TT_Di; TH1F * h_J2M_mjBins_tagSR_TT_Di; TH1F * h_J2M_mjBins_tagSB_TT_Di; TH1F * h_J2M_mjBins_antitagSR_TT_Di; TH1F * h_J2M_mjBins_antitagSB_TT_Di;

  TH1F * h_J1M_doubletagSR_TT_SL; TH1F * h_J1M_doubletagSB_TT_SL; TH1F * h_J1M_tagSR_TT_SL; TH1F * h_J1M_tagSB_TT_SL; TH1F * h_J1M_antitagSR_TT_SL; TH1F * h_J1M_antitagSB_TT_SL;
  TH1F * h_J2M_doubletagSR_TT_SL; TH1F * h_J2M_doubletagSB_TT_SL; TH1F * h_J2M_tagSR_TT_SL; TH1F * h_J2M_tagSB_TT_SL; TH1F * h_J2M_antitagSR_TT_SL; TH1F * h_J2M_antitagSB_TT_SL;
  TH1F * h_J1M_mjBins_doubletagSR_TT_SL; TH1F * h_J1M_mjBins_doubletagSB_TT_SL; TH1F * h_J1M_mjBins_tagSR_TT_SL; TH1F * h_J1M_mjBins_tagSB_TT_SL; TH1F * h_J1M_mjBins_antitagSR_TT_SL; TH1F * h_J1M_mjBins_antitagSB_TT_SL;
  TH1F * h_J2M_mjBins_doubletagSR_TT_SL; TH1F * h_J2M_mjBins_doubletagSB_TT_SL; TH1F * h_J2M_mjBins_tagSR_TT_SL; TH1F * h_J2M_mjBins_tagSB_TT_SL; TH1F * h_J2M_mjBins_antitagSR_TT_SL; TH1F * h_J2M_mjBins_antitagSB_TT_SL;

  TH1F * h_baseline_j1mass_SnglT; TH1F * h_J1M_doubletagSR_SnglT; TH1F * h_J1M_doubletagSB_SnglT; TH1F * h_J1M_tagSR_SnglT; TH1F * h_J1M_tagSB_SnglT; TH1F * h_J1M_antitagSR_SnglT; TH1F * h_J1M_antitagSB_SnglT;
  TH1F * h_baseline_j2mass_SnglT; TH1F * h_J2M_doubletagSR_SnglT; TH1F * h_J2M_doubletagSB_SnglT; TH1F * h_J2M_tagSR_SnglT; TH1F * h_J2M_tagSB_SnglT; TH1F * h_J2M_antitagSR_SnglT; TH1F * h_J2M_antitagSB_SnglT;
  TH1F * h_J1M_mjBins_doubletagSR_SnglT; TH1F * h_J1M_mjBins_doubletagSB_SnglT; TH1F * h_J1M_mjBins_tagSR_SnglT; TH1F * h_J1M_mjBins_tagSB_SnglT; TH1F * h_J1M_mjBins_antitagSR_SnglT; TH1F * h_J1M_mjBins_antitagSB_SnglT;
  TH1F * h_J2M_mjBins_doubletagSR_SnglT; TH1F * h_J2M_mjBins_doubletagSB_SnglT; TH1F * h_J2M_mjBins_tagSR_SnglT; TH1F * h_J2M_mjBins_tagSB_SnglT; TH1F * h_J2M_mjBins_antitagSR_SnglT; TH1F * h_J2M_mjBins_antitagSB_SnglT;
  TH1F * h_0H_j1mass_SnglT; TH1F * h_0H_j2mass_SnglT; TH1F * h_1H_j1mass_SnglT; TH1F * h_1H_j2mass_SnglT; TH1F * h_2H_j1mass_SnglT; TH1F * h_2H_j2mass_SnglT; TH1F * h_H_j1mass_SnglT; TH1F * h_H_j2mass_SnglT;
  TH1F * h_0H_jmass_SnglT; TH1F * h_1H_jmass_SnglT;  TH1F * h_2H_jmass_SnglT; TH1F * h_baseline_jmass_SnglT;

  TH1F * h_baseline_j1mass_QCD; TH1F * h_J1M_doubletagSR_QCD; TH1F * h_J1M_doubletagSB_QCD; TH1F * h_J1M_tagSR_QCD; TH1F * h_J1M_tagSB_QCD; TH1F * h_J1M_antitagSR_QCD; TH1F * h_J1M_antitagSB_QCD;
  TH1F * h_baseline_j2mass_QCD; TH1F * h_J2M_doubletagSR_QCD; TH1F * h_J2M_doubletagSB_QCD; TH1F * h_J2M_tagSR_QCD; TH1F * h_J2M_tagSB_QCD; TH1F * h_J2M_antitagSR_QCD; TH1F * h_J2M_antitagSB_QCD;
  TH1F * h_J1M_mjBins_doubletagSR_QCD; TH1F * h_J1M_mjBins_doubletagSB_QCD; TH1F * h_J1M_mjBins_tagSR_QCD; TH1F * h_J1M_mjBins_tagSB_QCD; TH1F * h_J1M_mjBins_antitagSR_QCD; TH1F * h_J1M_mjBins_antitagSB_QCD;
  TH1F * h_J2M_mjBins_doubletagSR_QCD; TH1F * h_J2M_mjBins_doubletagSB_QCD; TH1F * h_J2M_mjBins_tagSR_QCD; TH1F * h_J2M_mjBins_tagSB_QCD; TH1F * h_J2M_mjBins_antitagSR_QCD; TH1F * h_J2M_mjBins_antitagSB_QCD;
  TH1F * h_0H_j1mass_QCD; TH1F * h_0H_j2mass_QCD; TH1F * h_1H_j1mass_QCD; TH1F * h_1H_j2mass_QCD; TH1F * h_2H_j1mass_QCD; TH1F * h_2H_j2mass_QCD; TH1F * h_H_j1mass_QCD; TH1F * h_H_j2mass_QCD;
  TH1F * h_0H_jmass_QCD;  TH1F * h_1H_jmass_QCD;  TH1F * h_2H_jmass_QCD; TH1F * h_baseline_jmass_QCD;

  TH1F * h_baseline_j1mass_WJets; TH1F * h_J1M_doubletagSR_WJets; TH1F * h_J1M_doubletagSB_WJets; TH1F * h_J1M_tagSR_WJets; TH1F * h_J1M_tagSB_WJets; TH1F * h_J1M_antitagSR_WJets; TH1F * h_J1M_antitagSB_WJets;
  TH1F * h_baseline_j2mass_WJets; TH1F * h_J2M_doubletagSR_WJets; TH1F * h_J2M_doubletagSB_WJets; TH1F * h_J2M_tagSR_WJets; TH1F * h_J2M_tagSB_WJets; TH1F * h_J2M_antitagSR_WJets; TH1F * h_J2M_antitagSB_WJets;
  TH1F * h_J1M_mjBins_doubletagSR_WJets; TH1F * h_J1M_mjBins_doubletagSB_WJets; TH1F * h_J1M_mjBins_tagSR_WJets; TH1F * h_J1M_mjBins_tagSB_WJets; TH1F * h_J1M_mjBins_antitagSR_WJets; TH1F * h_J1M_mjBins_antitagSB_WJets;
  TH1F * h_J2M_mjBins_doubletagSR_WJets; TH1F * h_J2M_mjBins_doubletagSB_WJets; TH1F * h_J2M_mjBins_tagSR_WJets; TH1F * h_J2M_mjBins_tagSB_WJets; TH1F * h_J2M_mjBins_antitagSR_WJets; TH1F * h_J2M_mjBins_antitagSB_WJets;
  TH1F * h_0H_j1mass_WJets; TH1F * h_0H_j2mass_WJets; TH1F * h_1H_j1mass_WJets; TH1F * h_1H_j2mass_WJets; TH1F * h_2H_j1mass_WJets; TH1F * h_2H_j2mass_WJets; TH1F * h_H_j1mass_WJets; TH1F * h_H_j2mass_WJets;
  TH1F * h_0H_jmass_WJets;  TH1F * h_1H_jmass_WJets;  TH1F * h_2H_jmass_WJets;  TH1F * h_baseline_jmass_WJets;

  TH1F * h_baseline_j1mass_ZJets; TH1F * h_J1M_doubletagSR_ZJets; TH1F * h_J1M_doubletagSB_ZJets; TH1F * h_J1M_tagSR_ZJets; TH1F * h_J1M_tagSB_ZJets; TH1F * h_J1M_antitagSR_ZJets; TH1F * h_J1M_antitagSB_ZJets;
  TH1F * h_baseline_j2mass_ZJets; TH1F * h_J2M_doubletagSR_ZJets; TH1F * h_J2M_doubletagSB_ZJets; TH1F * h_J2M_tagSR_ZJets; TH1F * h_J2M_tagSB_ZJets; TH1F * h_J2M_antitagSR_ZJets; TH1F * h_J2M_antitagSB_ZJets;
  TH1F * h_J1M_mjBins_doubletagSR_ZJets; TH1F * h_J1M_mjBins_doubletagSB_ZJets; TH1F * h_J1M_mjBins_tagSR_ZJets; TH1F * h_J1M_mjBins_tagSB_ZJets; TH1F * h_J1M_mjBins_antitagSR_ZJets; TH1F * h_J1M_mjBins_antitagSB_ZJets;
  TH1F * h_J2M_mjBins_doubletagSR_ZJets; TH1F * h_J2M_mjBins_doubletagSB_ZJets; TH1F * h_J2M_mjBins_tagSR_ZJets; TH1F * h_J2M_mjBins_tagSB_ZJets; TH1F * h_J2M_mjBins_antitagSR_ZJets; TH1F * h_J2M_mjBins_antitagSB_ZJets;
  TH1F * h_0H_j1mass_ZJets; TH1F * h_0H_j2mass_ZJets; TH1F * h_1H_j1mass_ZJets; TH1F * h_1H_j2mass_ZJets; TH1F * h_2H_j1mass_ZJets; TH1F * h_2H_j2mass_ZJets; TH1F * h_H_j1mass_ZJets; TH1F * h_H_j2mass_ZJets;
  TH1F * h_0H_jmass_ZJets; TH1F * h_1H_jmass_ZJets; TH1F * h_2H_jmass_ZJets; TH1F * h_baseline_jmass_ZJets;

  TH1F * h_baseline_j1mass_GJets; TH1F * h_J1M_doubletagSR_GJets; TH1F * h_J1M_doubletagSB_GJets; TH1F * h_J1M_tagSR_GJets; TH1F * h_J1M_tagSB_GJets; TH1F * h_J1M_antitagSR_GJets; TH1F * h_J1M_antitagSB_GJets;
  TH1F * h_baseline_j2mass_GJets; TH1F * h_J2M_doubletagSR_GJets; TH1F * h_J2M_doubletagSB_GJets; TH1F * h_J2M_tagSR_GJets; TH1F * h_J2M_tagSB_GJets; TH1F * h_J2M_antitagSR_GJets; TH1F * h_J2M_antitagSB_GJets;
  TH1F * h_J1M_mjBins_doubletagSR_GJets; TH1F * h_J1M_mjBins_doubletagSB_GJets; TH1F * h_J1M_mjBins_tagSR_GJets; TH1F * h_J1M_mjBins_tagSB_GJets; TH1F * h_J1M_mjBins_antitagSR_GJets; TH1F * h_J1M_mjBins_antitagSB_GJets;
  TH1F * h_J2M_mjBins_doubletagSR_GJets; TH1F * h_J2M_mjBins_doubletagSB_GJets; TH1F * h_J2M_mjBins_tagSR_GJets; TH1F * h_J2M_mjBins_tagSB_GJets; TH1F * h_J2M_mjBins_antitagSR_GJets; TH1F * h_J2M_mjBins_antitagSB_GJets;


  TH1F * h_baseline_j1mass_T5HH1600; TH1F * h_baseline_j2mass_T5HH1600; TH1F * h_J1M_doubletagSR_T5HH1600; TH1F * h_J2M_doubletagSR_T5HH1600; TH1F * h_J1M_doubletagSB_T5HH1600; TH1F * h_J2M_doubletagSB_T5HH1600;
  TH1F * h_J1M_antitagSR_T5HH1600; TH1F * h_J2M_antitagSR_T5HH1600; TH1F * h_J1M_antitagSB_T5HH1600; TH1F * h_J2M_antitagSB_T5HH1600;
  TH1F * h_J1M_tagSR_T5HH1600; TH1F * h_J2M_tagSR_T5HH1600; TH1F * h_J1M_tagSB_T5HH1600; TH1F * h_J2M_tagSB_T5HH1600;
  TH1F * h_0H_j1mass_T5HH1600; TH1F * h_0H_j2mass_T5HH1600; TH1F * h_1H_j1mass_T5HH1600; TH1F * h_1H_j2mass_T5HH1600; TH1F * h_2H_j1mass_T5HH1600; TH1F * h_2H_j2mass_T5HH1600; TH1F * h_H_j1mass_T5HH1600; TH1F * h_H_j2mass_T5HH1600;
  TH1F * h_0H_jmass_T5HH1600; TH1F * h_1H_jmass_T5HH1600; TH1F * h_2H_jmass_T5HH1600; TH1F * h_baseline_jmass_T5HH1600;
  TH1F * h_baseline_j1mass_T5HH2000; TH1F * h_baseline_j2mass_T5HH2000; TH1F * h_J1M_doubletagSR_T5HH2000; TH1F * h_J2M_doubletagSR_T5HH2000; TH1F * h_J1M_doubletagSB_T5HH2000; TH1F * h_J2M_doubletagSB_T5HH2000;
  TH1F * h_J1M_antitagSR_T5HH2000; TH1F * h_J2M_antitagSR_T5HH2000; TH1F * h_J1M_antitagSB_T5HH2000; TH1F * h_J2M_antitagSB_T5HH2000;
  TH1F * h_J1M_tagSR_T5HH2000; TH1F * h_J2M_tagSR_T5HH2000; TH1F * h_J1M_tagSB_T5HH2000; TH1F * h_J2M_tagSB_T5HH2000;
  TH1F * h_H_j1mass_T5HH2000; TH1F * h_H_j2mass_T5HH2000;
  TH1F * h_0H_jmass_T5HH2000; TH1F * h_1H_jmass_T5HH2000; TH1F * h_2H_jmass_T5HH2000; TH1F * h_baseline_jmass_T5HH2000;
  TH1F * h_baseline_j1mass_T5HH2200; TH1F * h_baseline_j2mass_T5HH2200; TH1F * h_J1M_doubletagSR_T5HH2200; TH1F * h_J2M_doubletagSR_T5HH2200; TH1F * h_J1M_doubletagSB_T5HH2200; TH1F * h_J2M_doubletagSB_T5HH2200;
  TH1F * h_J1M_antitagSR_T5HH2200; TH1F * h_J2M_antitagSR_T5HH2200; TH1F * h_J1M_antitagSB_T5HH2200; TH1F * h_J2M_antitagSB_T5HH2200;
  TH1F * h_J1M_tagSR_T5HH2200; TH1F * h_J2M_tagSR_T5HH2200; TH1F * h_J1M_tagSB_T5HH2200; TH1F * h_J2M_tagSB_T5HH2200;
  TH1F * h_H_j1mass_T5HH2200; TH1F * h_H_j2mass_T5HH2200;
  TH1F * h_0H_jmass_T5HH2200; TH1F * h_1H_jmass_T5HH2200; TH1F * h_2H_jmass_T5HH2200; TH1F * h_baseline_jmass_T5HH2200;

  TH1F * h_baseline_j1mass_TChiHH500; TH1F * h_baseline_j2mass_TChiHH500; TH1F * h_J1M_doubletagSR_TChiHH500; TH1F * h_J2M_doubletagSR_TChiHH500; TH1F * h_J1M_doubletagSB_TChiHH500; TH1F * h_J2M_doubletagSB_TChiHH500;
  TH1F * h_J1M_antitagSR_TChiHH500; TH1F * h_J2M_antitagSR_TChiHH500; TH1F * h_J1M_antitagSB_TChiHH500; TH1F * h_J2M_antitagSB_TChiHH500;
  TH1F * h_J1M_tagSR_TChiHH500; TH1F * h_J2M_tagSR_TChiHH500; TH1F * h_J1M_tagSB_TChiHH500; TH1F * h_J2M_tagSB_TChiHH500;
  TH1F * h_0H_j1mass_TChiHH500; TH1F * h_0H_j2mass_TChiHH500; TH1F * h_1H_j1mass_TChiHH500; TH1F * h_1H_j2mass_TChiHH500; TH1F * h_2H_j1mass_TChiHH500; TH1F * h_2H_j2mass_TChiHH500; TH1F * h_H_j1mass_TChiHH500; TH1F * h_H_j2mass_TChiHH500;
  TH1F * h_0H_jmass_TChiHH500; TH1F * h_1H_jmass_TChiHH500; TH1F * h_2H_jmass_TChiHH500; TH1F * h_baseline_jmass_TChiHH500;
  TH1F * h_baseline_j1mass_TChiHH800; TH1F * h_baseline_j2mass_TChiHH800; TH1F * h_J1M_doubletagSR_TChiHH800; TH1F * h_J2M_doubletagSR_TChiHH800; TH1F * h_J1M_doubletagSB_TChiHH800; TH1F * h_J2M_doubletagSB_TChiHH800;
  TH1F * h_J1M_antitagSR_TChiHH800; TH1F * h_J2M_antitagSR_TChiHH800; TH1F * h_J1M_antitagSB_TChiHH800; TH1F * h_J2M_antitagSB_TChiHH800;
  TH1F * h_J1M_tagSR_TChiHH800; TH1F * h_J2M_tagSR_TChiHH800; TH1F * h_J1M_tagSB_TChiHH800; TH1F * h_J2M_tagSB_TChiHH800;
  TH1F * h_H_j1mass_TChiHH800; TH1F * h_H_j2mass_TChiHH800;
  TH1F * h_0H_jmass_TChiHH800; TH1F * h_1H_jmass_TChiHH800; TH1F * h_2H_jmass_TChiHH800; TH1F * h_baseline_jmass_TChiHH800;
  TH1F * h_baseline_j1mass_TChiHH1000; TH1F * h_baseline_j2mass_TChiHH1000; TH1F * h_J1M_doubletagSR_TChiHH1000; TH1F * h_J2M_doubletagSR_TChiHH1000; TH1F * h_J1M_doubletagSB_TChiHH1000; TH1F * h_J2M_doubletagSB_TChiHH1000;
  TH1F * h_J1M_antitagSR_TChiHH1000; TH1F * h_J2M_antitagSR_TChiHH1000; TH1F * h_J1M_antitagSB_TChiHH1000; TH1F * h_J2M_antitagSB_TChiHH1000;
  TH1F * h_J1M_tagSR_TChiHH1000; TH1F * h_J2M_tagSR_TChiHH1000; TH1F * h_J1M_tagSB_TChiHH1000; TH1F * h_J2M_tagSB_TChiHH1000;
  TH1F * h_H_j1mass_TChiHH1000; TH1F * h_H_j2mass_TChiHH1000;
  TH1F * h_0H_jmass_TChiHH1000; TH1F * h_1H_jmass_TChiHH1000; TH1F * h_2H_jmass_TChiHH1000; TH1F * h_baseline_jmass_TChiHH1000;

  TH1F * h_baseline_j1mass_sum; TH1F * h_baseline_j2mass_sum;  TH1F * h_0H_j1mass_sum; TH1F * h_0H_j2mass_sum; TH1F * h_1H_j1mass_sum; TH1F * h_1H_j2mass_sum; TH1F * h_2H_j1mass_sum; TH1F * h_2H_j2mass_sum;TH1F * h_H_j1mass_sum; TH1F * h_H_j2mass_sum;
  TH1F * h_0H_jmass_sum; TH1F * h_1H_jmass_sum; TH1F * h_2H_jmass_sum; TH1F * h_baseline_jmass_sum;
  TH1F * h_0H_jpt_sum; TH1F * h_1H_jpt_sum; TH1F * h_2H_jpt_sum; TH1F * h_baseline_jpt_sum;
  TH1F * h_0H_jbb_sum; TH1F * h_1H_jbb_sum; TH1F * h_2H_jbb_sum; TH1F * h_baseline_jbb_sum;
  TH1F * h_baseline_j1mass_data; TH1F * h_baseline_j2mass_data; TH1F * h_0H_j1mass_data; TH1F * h_0H_j2mass_data; TH1F * h_1H_j1mass_data; TH1F * h_1H_j2mass_data; TH1F * h_2H_j1mass_data; TH1F * h_2H_j2mass_data; TH1F * h_H_j1mass_data; TH1F * h_H_j2mass_data;
  TH1F * h_0H_jmass_data; TH1F * h_1H_jmass_data; TH1F * h_2H_jmass_data; TH1F * h_baseline_jmass_data;
  TH1F * h_0H_jpt_data; TH1F * h_1H_jpt_data; TH1F * h_2H_jpt_data; TH1F * h_baseline_jpt_data;
  TH1F * h_0H_jbb_data; TH1F * h_1H_jbb_data; TH1F * h_2H_jbb_data; TH1F * h_baseline_jbb_data;
  TH1F * h_J1M_doubletagSR_data; TH1F * h_J2M_doubletagSR_data; TH1F * h_J1M_doubletagSB_data; TH1F * h_J2M_doubletagSB_data;
  TH1F * h_J1M_tagSR_data; TH1F * h_J2M_tagSR_data; TH1F * h_J1M_tagSB_data; TH1F * h_J2M_tagSB_data;
  TH1F * h_J1M_antitagSR_data; TH1F * h_J2M_antitagSR_data; TH1F * h_J1M_antitagSB_data; TH1F * h_J2M_antitagSB_data;
  TH1F * h_J1Pt_doubletagSR_data; TH1F * h_J2Pt_doubletagSR_data;  TH1F * h_J1Pt_doubletagSB_data; TH1F * h_J2Pt_doubletagSB_data;
  TH1F * h_J1Pt_tagSR_data; TH1F * h_J2Pt_tagSR_data; TH1F * h_J1Pt_tagSB_data; TH1F * h_J2Pt_tagSB_data;
  TH1F * h_J1Pt_antitagSR_data; TH1F * h_J2Pt_antitagSR_data; TH1F * h_J1Pt_antitagSB_data; TH1F * h_J2Pt_antitagSB_data;
  TH1F * h_J1BB_doubletagSR_data; TH1F * h_J2BB_doubletagSR_data;  TH1F * h_J1BB_doubletagSB_data; TH1F * h_J2BB_doubletagSB_data;
  TH1F * h_J1BB_tagSR_data; TH1F * h_J2BB_tagSR_data; TH1F * h_J1BB_tagSB_data; TH1F * h_J2BB_tagSB_data;
  TH1F * h_J1BB_antitagSR_data; TH1F * h_J2BB_antitagSR_data; TH1F * h_J1BB_antitagSB_data; TH1F * h_J2BB_antitagSB_data;


  //For pt and bb
  TH1F * h_J1Pt_doubletagSR_sum; TH1F * h_J2Pt_doubletagSR_sum; TH1F * h_J1Pt_doubletagSB_sum; TH1F * h_J2Pt_doubletagSB_sum;
  TH1F * h_J1Pt_antitagSR_sum; TH1F * h_J2Pt_antitagSR_sum; TH1F * h_J1Pt_antitagSB_sum; TH1F * h_J2Pt_antitagSB_sum;
  TH1F * h_J1Pt_tagSR_sum; TH1F * h_J2Pt_tagSR_sum; TH1F * h_J1Pt_tagSB_sum; TH1F * h_J2Pt_tagSB_sum;
  TH1F * h_J1BB_doubletagSR_sum; TH1F * h_J2BB_doubletagSR_sum; TH1F * h_J1BB_tagSR_sum; TH1F * h_J2BB_tagSR_sum;

  TH1F * h_baseline_j1pt_QCD; TH1F * h_baseline_j2pt_QCD; TH1F * h_J1Pt_doubletagSR_QCD; TH1F * h_J2Pt_doubletagSR_QCD; TH1F * h_J1Pt_doubletagSB_QCD; TH1F * h_J2Pt_doubletagSB_QCD;
  TH1F * h_J1Pt_antitagSR_QCD; TH1F * h_J2Pt_antitagSR_QCD; TH1F * h_J1Pt_antitagSB_QCD; TH1F * h_J2Pt_antitagSB_QCD;
  TH1F * h_J1Pt_tagSR_QCD; TH1F * h_J2Pt_tagSR_QCD; TH1F * h_J1Pt_tagSB_QCD; TH1F * h_J2Pt_tagSB_QCD;
  TH1F * h_0H_j1pt_QCD; TH1F * h_0H_j2pt_QCD; TH1F * h_1H_j1pt_QCD; TH1F * h_1H_j2pt_QCD; TH1F * h_2H_j1pt_QCD; TH1F * h_2H_j2pt_QCD; TH1F * h_H_j1pt_QCD; TH1F * h_H_j2pt_QCD;
  TH1F * h_J1BB_doubletagSR_QCD; TH1F * h_J2BB_doubletagSR_QCD; TH1F * h_J1BB_tagSR_QCD; TH1F * h_J2BB_tagSR_QCD;
  TH1F * h_0H_jpt_QCD; TH1F * h_1H_jpt_QCD; TH1F * h_2H_jpt_QCD; TH1F * h_baseline_jpt_QCD;
  TH1F * h_0H_jbb_QCD; TH1F * h_1H_jbb_QCD; TH1F * h_2H_jbb_QCD; TH1F * h_baseline_jbb_QCD;


  TH1F * h_baseline_j1pt_SnglT;  TH1F * h_baseline_j2pt_SnglT; TH1F * h_J1Pt_doubletagSR_SnglT; TH1F * h_J2Pt_doubletagSR_SnglT; TH1F * h_J1Pt_doubletagSB_SnglT; TH1F * h_J2Pt_doubletagSB_SnglT;
  TH1F * h_J1Pt_antitagSR_SnglT; TH1F * h_J2Pt_antitagSR_SnglT; TH1F * h_J1Pt_antitagSB_SnglT; TH1F * h_J2Pt_antitagSB_SnglT;
  TH1F * h_J1Pt_tagSR_SnglT; TH1F * h_J2Pt_tagSR_SnglT; TH1F * h_J1Pt_tagSB_SnglT; TH1F * h_J2Pt_tagSB_SnglT;
  TH1F * h_0H_j1pt_SnglT; TH1F * h_0H_j2pt_SnglT; TH1F * h_1H_j1pt_SnglT; TH1F * h_1H_j2pt_SnglT; TH1F * h_2H_j1pt_SnglT; TH1F * h_2H_j2pt_SnglT; TH1F * h_H_j1pt_SnglT; TH1F * h_H_j2pt_SnglT;
  TH1F * h_J1BB_doubletagSR_SnglT; TH1F * h_J2BB_doubletagSR_SnglT; TH1F * h_J1BB_tagSR_SnglT; TH1F * h_J2BB_tagSR_SnglT;
  TH1F * h_0H_jpt_SnglT; TH1F * h_1H_jpt_SnglT; TH1F * h_2H_jpt_SnglT; TH1F * h_baseline_jpt_SnglT;
  TH1F * h_0H_jbb_SnglT; TH1F * h_1H_jbb_SnglT; TH1F * h_2H_jbb_SnglT; TH1F * h_baseline_jbb_SnglT;

  TH1F * h_baseline_j1pt_GJets;  TH1F * h_baseline_j2pt_GJets; TH1F * h_J1Pt_doubletagSR_GJets; TH1F * h_J2Pt_doubletagSR_GJets; TH1F * h_J1Pt_doubletagSB_GJets; TH1F * h_J2Pt_doubletagSB_GJets;
  TH1F * h_J1Pt_antitagSR_GJets; TH1F * h_J2Pt_antitagSR_GJets; TH1F * h_J1Pt_antitagSB_GJets; TH1F * h_J2Pt_antitagSB_GJets;
  TH1F * h_J1Pt_tagSR_GJets; TH1F * h_J2Pt_tagSR_GJets; TH1F * h_J1Pt_tagSB_GJets; TH1F * h_J2Pt_tagSB_GJets;

  TH1F * h_baseline_j1pt_TT;  TH1F * h_baseline_j2pt_TT;  TH1F * h_J1Pt_doubletagSR_TT; TH1F * h_J2Pt_doubletagSR_TT; TH1F * h_J1Pt_doubletagSB_TT; TH1F * h_J2Pt_doubletagSB_TT;
  TH1F * h_J1Pt_antitagSR_TT; TH1F * h_J2Pt_antitagSR_TT; TH1F * h_J1Pt_antitagSB_TT; TH1F * h_J2Pt_antitagSB_TT;
  TH1F * h_J1Pt_tagSR_TT; TH1F * h_J2Pt_tagSR_TT; TH1F * h_J1Pt_tagSB_TT; TH1F * h_J2Pt_tagSB_TT;
  TH1F * h_0H_j1pt_TT; TH1F * h_0H_j2pt_TT; TH1F * h_1H_j1pt_TT; TH1F * h_1H_j2pt_TT; TH1F * h_2H_j1pt_TT; TH1F * h_2H_j2pt_TT; TH1F * h_H_j1pt_TT; TH1F * h_H_j2pt_TT;
  TH1F * h_J1BB_doubletagSR_TT; TH1F * h_J2BB_doubletagSR_TT; TH1F * h_J1BB_tagSR_TT; TH1F * h_J2BB_tagSR_TT;
  TH1F * h_0H_jpt_TT; TH1F * h_1H_jpt_TT; TH1F * h_2H_jpt_TT; TH1F * h_baseline_jpt_TT;
  TH1F * h_0H_jbb_TT; TH1F * h_1H_jbb_TT; TH1F * h_2H_jbb_TT; TH1F * h_baseline_jbb_TT;


  TH1F * h_J1Pt_doubletagSR_TT_Di; TH1F * h_J2Pt_doubletagSR_TT_Di; TH1F * h_J1Pt_doubletagSB_TT_Di; TH1F * h_J2Pt_doubletagSB_TT_Di;
  TH1F * h_J1Pt_antitagSR_TT_Di; TH1F * h_J2Pt_antitagSR_TT_Di; TH1F * h_J1Pt_antitagSB_TT_Di; TH1F * h_J2Pt_antitagSB_TT_Di;
  TH1F * h_J1Pt_tagSR_TT_Di; TH1F * h_J2Pt_tagSR_TT_Di; TH1F * h_J1Pt_tagSB_TT_Di; TH1F * h_J2Pt_tagSB_TT_Di;

  TH1F * h_J1Pt_doubletagSR_TT_SL; TH1F * h_J2Pt_doubletagSR_TT_SL; TH1F * h_J1Pt_doubletagSB_TT_SL; TH1F * h_J2Pt_doubletagSB_TT_SL;
  TH1F * h_J1Pt_antitagSR_TT_SL; TH1F * h_J2Pt_antitagSR_TT_SL; TH1F * h_J1Pt_antitagSB_TT_SL; TH1F * h_J2Pt_antitagSB_TT_SL;
  TH1F * h_J1Pt_tagSR_TT_SL; TH1F * h_J2Pt_tagSR_TT_SL; TH1F * h_J1Pt_tagSB_TT_SL; TH1F * h_J2Pt_tagSB_TT_SL;

  TH1F * h_baseline_j1pt_WJets;  TH1F * h_baseline_j2pt_WJets; TH1F * h_J1Pt_doubletagSR_WJets; TH1F * h_J2Pt_doubletagSR_WJets; TH1F * h_J1Pt_doubletagSB_WJets; TH1F * h_J2Pt_doubletagSB_WJets;
  TH1F * h_J1Pt_antitagSR_WJets; TH1F * h_J2Pt_antitagSR_WJets; TH1F * h_J1Pt_antitagSB_WJets; TH1F * h_J2Pt_antitagSB_WJets;
  TH1F * h_J1Pt_tagSR_WJets; TH1F * h_J2Pt_tagSR_WJets; TH1F * h_J1Pt_tagSB_WJets; TH1F * h_J2Pt_tagSB_WJets;
  TH1F * h_0H_j1pt_WJets; TH1F * h_0H_j2pt_WJets; TH1F * h_1H_j1pt_WJets; TH1F * h_1H_j2pt_WJets; TH1F * h_2H_j1pt_WJets; TH1F * h_2H_j2pt_WJets; TH1F * h_H_j1pt_WJets; TH1F * h_H_j2pt_WJets;
  TH1F * h_J1BB_doubletagSR_WJets; TH1F * h_J2BB_doubletagSR_WJets; TH1F * h_J1BB_tagSR_WJets; TH1F * h_J2BB_tagSR_WJets;
  TH1F * h_0H_jpt_WJets; TH1F * h_1H_jpt_WJets; TH1F * h_2H_jpt_WJets; TH1F * h_baseline_jpt_WJets;
  TH1F * h_0H_jbb_WJets; TH1F * h_1H_jbb_WJets; TH1F * h_2H_jbb_WJets; TH1F * h_baseline_jbb_WJets;

  TH1F * h_baseline_j1pt_ZJets; TH1F * h_baseline_j2pt_ZJets; TH1F * h_J1Pt_doubletagSR_ZJets; TH1F * h_J2Pt_doubletagSR_ZJets; TH1F * h_J1Pt_doubletagSB_ZJets; TH1F * h_J2Pt_doubletagSB_ZJets;
  TH1F * h_J1Pt_antitagSR_ZJets; TH1F * h_J2Pt_antitagSR_ZJets; TH1F * h_J1Pt_antitagSB_ZJets; TH1F * h_J2Pt_antitagSB_ZJets;
  TH1F * h_J1Pt_tagSR_ZJets; TH1F * h_J2Pt_tagSR_ZJets; TH1F * h_J1Pt_tagSB_ZJets; TH1F * h_J2Pt_tagSB_ZJets;
  TH1F * h_0H_j1pt_ZJets; TH1F * h_0H_j2pt_ZJets; TH1F * h_1H_j1pt_ZJets; TH1F * h_1H_j2pt_ZJets; TH1F * h_2H_j1pt_ZJets; TH1F * h_2H_j2pt_ZJets; TH1F * h_H_j1pt_ZJets; TH1F * h_H_j2pt_ZJets;
  TH1F * h_J1BB_doubletagSR_ZJets; TH1F * h_J2BB_doubletagSR_ZJets; TH1F * h_J1BB_tagSR_ZJets; TH1F * h_J2BB_tagSR_ZJets;
  TH1F * h_0H_jpt_ZJets; TH1F * h_1H_jpt_ZJets; TH1F * h_2H_jpt_ZJets; TH1F * h_baseline_jpt_ZJets;
  TH1F * h_0H_jbb_ZJets; TH1F * h_1H_jbb_ZJets; TH1F * h_2H_jbb_ZJets; TH1F * h_baseline_jbb_ZJets;

  TH1F * h_baseline_j1pt_T5HH1600; TH1F * h_baseline_j2pt_T5HH1600; TH1F * h_J1Pt_doubletagSR_T5HH1600; TH1F * h_J2Pt_doubletagSR_T5HH1600; TH1F * h_J1Pt_doubletagSB_T5HH1600; TH1F * h_J2Pt_doubletagSB_T5HH1600;
  TH1F * h_J1Pt_antitagSR_T5HH1600; TH1F * h_J2Pt_antitagSR_T5HH1600; TH1F * h_J1Pt_antitagSB_T5HH1600; TH1F * h_J2Pt_antitagSB_T5HH1600;
  TH1F * h_J1Pt_tagSR_T5HH1600; TH1F * h_J2Pt_tagSR_T5HH1600; TH1F * h_J1Pt_tagSB_T5HH1600; TH1F * h_J2Pt_tagSB_T5HH1600;
  TH1F * h_baseline_j1bb_T5HH1600; TH1F * h_baseline_j2bb_T5HH1600;
  TH1F * h_0H_j1pt_T5HH1600; TH1F * h_0H_j2pt_T5HH1600; TH1F * h_1H_j1pt_T5HH1600; TH1F * h_1H_j2pt_T5HH1600; TH1F * h_2H_j1pt_T5HH1600; TH1F * h_2H_j2pt_T5HH1600; TH1F * h_H_j1pt_T5HH1600; TH1F * h_H_j2pt_T5HH1600;
  TH1F * h_J1BB_antitagSR_T5HH1600; TH1F * h_J2BB_antitagSR_T5HH1600; TH1F * h_J1BB_antitagSB_T5HH1600; TH1F * h_J2BB_antitagSB_T5HH1600;
  TH1F * h_J1BB_tagSR_T5HH1600; TH1F * h_J2BB_tagSR_T5HH1600; TH1F * h_J1BB_tagSB_T5HH1600; TH1F * h_J2BB_tagSB_T5HH1600;
  TH1F * h_J1BB_doubletagSR_T5HH1600; TH1F * h_J2BB_doubletagSR_T5HH1600; TH1F * h_J1BB_doubletagSB_T5HH1600; TH1F * h_J2BB_doubletagSB_T5HH1600;
  TH1F * h_H_j1bb_T5HH1600; TH1F * h_H_j2bb_T5HH1600; TH1F * h_0H_j1bb_T5HH1600; TH1F * h_0H_j2bb_T5HH1600; TH1F * h_1H_j1bb_T5HH1600; TH1F * h_1H_j2bb_T5HH1600; TH1F * h_2H_j1bb_T5HH1600; TH1F * h_2H_j2bb_T5HH1600;
  TH1F * h_0H_jpt_T5HH1600; TH1F * h_1H_jpt_T5HH1600; TH1F * h_2H_jpt_T5HH1600; TH1F * h_baseline_jpt_T5HH1600;
  TH1F * h_0H_jbb_T5HH1600; TH1F * h_1H_jbb_T5HH1600; TH1F * h_2H_jbb_T5HH1600; TH1F * h_baseline_jbb_T5HH1600;

  TH1F * h_baseline_j1pt_T5HH2000; TH1F * h_baseline_j2pt_T5HH2000; TH1F * h_J1Pt_doubletagSR_T5HH2000; TH1F * h_J2Pt_doubletagSR_T5HH2000; TH1F * h_J1Pt_doubletagSB_T5HH2000; TH1F * h_J2Pt_doubletagSB_T5HH2000;
  TH1F * h_J1Pt_antitagSR_T5HH2000; TH1F * h_J2Pt_antitagSR_T5HH2000; TH1F * h_J1Pt_antitagSB_T5HH2000; TH1F * h_J2Pt_antitagSB_T5HH2000;
  TH1F * h_J1Pt_tagSR_T5HH2000; TH1F * h_J2Pt_tagSR_T5HH2000; TH1F * h_J1Pt_tagSB_T5HH2000; TH1F * h_J2Pt_tagSB_T5HH2000;
  TH1F * h_baseline_j1bb_T5HH2000; TH1F * h_baseline_j2bb_T5HH2000;
  TH1F * h_H_j1pt_T5HH2000; TH1F * h_H_j2pt_T5HH2000;
  TH1F * h_J1BB_antitagSR_T5HH2000; TH1F * h_J2BB_antitagSR_T5HH2000; TH1F * h_J1BB_antitagSB_T5HH2000; TH1F * h_J2BB_antitagSB_T5HH2000;
  TH1F * h_J1BB_tagSR_T5HH2000; TH1F * h_J2BB_tagSR_T5HH2000; TH1F * h_J1BB_tagSB_T5HH2000; TH1F * h_J2BB_tagSB_T5HH2000;
  TH1F * h_J1BB_doubletagSR_T5HH2000; TH1F * h_J2BB_doubletagSR_T5HH2000; TH1F * h_J1BB_doubletagSB_T5HH2000; TH1F * h_J2BB_doubletagSB_T5HH2000;
  TH1F * h_H_j1bb_T5HH2000; TH1F * h_H_j2bb_T5HH2000;
  TH1F * h_0H_jpt_T5HH2000; TH1F * h_1H_jpt_T5HH2000; TH1F * h_2H_jpt_T5HH2000; TH1F * h_baseline_jpt_T5HH2000;
  TH1F * h_0H_jbb_T5HH2000; TH1F * h_1H_jbb_T5HH2000; TH1F * h_2H_jbb_T5HH2000; TH1F * h_baseline_jbb_T5HH2000;

  TH1F * h_baseline_j1pt_T5HH2200; TH1F * h_baseline_j2pt_T5HH2200; TH1F * h_J1Pt_doubletagSR_T5HH2200; TH1F * h_J2Pt_doubletagSR_T5HH2200; TH1F * h_J1Pt_doubletagSB_T5HH2200; TH1F * h_J2Pt_doubletagSB_T5HH2200;
  TH1F * h_J1Pt_antitagSR_T5HH2200; TH1F * h_J2Pt_antitagSR_T5HH2200; TH1F * h_J1Pt_antitagSB_T5HH2200; TH1F * h_J2Pt_antitagSB_T5HH2200;
  TH1F * h_J1Pt_tagSR_T5HH2200; TH1F * h_J2Pt_tagSR_T5HH2200; TH1F * h_J1Pt_tagSB_T5HH2200; TH1F * h_J2Pt_tagSB_T5HH2200;
  TH1F * h_baseline_j1bb_T5HH2200; TH1F * h_baseline_j2bb_T5HH2200;
  TH1F * h_H_j1pt_T5HH2200; TH1F * h_H_j2pt_T5HH2200;
  TH1F * h_J1BB_antitagSR_T5HH2200; TH1F * h_J2BB_antitagSR_T5HH2200; TH1F * h_J1BB_antitagSB_T5HH2200; TH1F * h_J2BB_antitagSB_T5HH2200;
  TH1F * h_J1BB_tagSR_T5HH2200; TH1F * h_J2BB_tagSR_T5HH2200; TH1F * h_J1BB_tagSB_T5HH2200; TH1F * h_J2BB_tagSB_T5HH2200;
  TH1F * h_J1BB_doubletagSR_T5HH2200; TH1F * h_J2BB_doubletagSR_T5HH2200; TH1F * h_J1BB_doubletagSB_T5HH2200; TH1F * h_J2BB_doubletagSB_T5HH2200;
  TH1F * h_H_j1bb_T5HH2200; TH1F * h_H_j2bb_T5HH2200;
  TH1F * h_0H_jpt_T5HH2200; TH1F * h_1H_jpt_T5HH2200; TH1F * h_2H_jpt_T5HH2200; TH1F * h_baseline_jpt_T5HH2200;
  TH1F * h_0H_jbb_T5HH2200; TH1F * h_1H_jbb_T5HH2200; TH1F * h_2H_jbb_T5HH2200; TH1F * h_baseline_jbb_T5HH2200;

  TH1F * h_baseline_j1pt_TChiHH500; TH1F * h_baseline_j2pt_TChiHH500; TH1F * h_J1Pt_doubletagSR_TChiHH500; TH1F * h_J2Pt_doubletagSR_TChiHH500; TH1F * h_J1Pt_doubletagSB_TChiHH500; TH1F * h_J2Pt_doubletagSB_TChiHH500;
  TH1F * h_J1Pt_antitagSR_TChiHH500; TH1F * h_J2Pt_antitagSR_TChiHH500; TH1F * h_J1Pt_antitagSB_TChiHH500; TH1F * h_J2Pt_antitagSB_TChiHH500;
  TH1F * h_J1Pt_tagSR_TChiHH500; TH1F * h_J2Pt_tagSR_TChiHH500; TH1F * h_J1Pt_tagSB_TChiHH500; TH1F * h_J2Pt_tagSB_TChiHH500;
  TH1F * h_baseline_j1bb_TChiHH500; TH1F * h_baseline_j2bb_TChiHH500;
  TH1F * h_0H_j1pt_TChiHH500; TH1F * h_0H_j2pt_TChiHH500; TH1F * h_1H_j1pt_TChiHH500; TH1F * h_1H_j2pt_TChiHH500; TH1F * h_2H_j1pt_TChiHH500; TH1F * h_2H_j2pt_TChiHH500; TH1F * h_H_j1pt_TChiHH500; TH1F * h_H_j2pt_TChiHH500;
  TH1F * h_J1BB_antitagSR_TChiHH500; TH1F * h_J2BB_antitagSR_TChiHH500; TH1F * h_J1BB_antitagSB_TChiHH500; TH1F * h_J2BB_antitagSB_TChiHH500;
  TH1F * h_J1BB_tagSR_TChiHH500; TH1F * h_J2BB_tagSR_TChiHH500; TH1F * h_J1BB_tagSB_TChiHH500; TH1F * h_J2BB_tagSB_TChiHH500;
  TH1F * h_J1BB_doubletagSR_TChiHH500; TH1F * h_J2BB_doubletagSR_TChiHH500; TH1F * h_J1BB_doubletagSB_TChiHH500; TH1F * h_J2BB_doubletagSB_TChiHH500;
  TH1F * h_H_j1bb_TChiHH500; TH1F * h_H_j2bb_TChiHH500; TH1F * h_0H_j1bb_TChiHH500; TH1F * h_0H_j2bb_TChiHH500; TH1F * h_1H_j1bb_TChiHH500; TH1F * h_1H_j2bb_TChiHH500; TH1F * h_2H_j1bb_TChiHH500; TH1F * h_2H_j2bb_TChiHH500;
  TH1F * h_0H_jpt_TChiHH500; TH1F * h_1H_jpt_TChiHH500; TH1F * h_2H_jpt_TChiHH500; TH1F * h_baseline_jpt_TChiHH500;
  TH1F * h_0H_jbb_TChiHH500; TH1F * h_1H_jbb_TChiHH500; TH1F * h_2H_jbb_TChiHH500; TH1F * h_baseline_jbb_TChiHH500;

  TH1F * h_baseline_j1pt_TChiHH800; TH1F * h_baseline_j2pt_TChiHH800; TH1F * h_J1Pt_doubletagSR_TChiHH800; TH1F * h_J2Pt_doubletagSR_TChiHH800; TH1F * h_J1Pt_doubletagSB_TChiHH800; TH1F * h_J2Pt_doubletagSB_TChiHH800;
  TH1F * h_J1Pt_antitagSR_TChiHH800; TH1F * h_J2Pt_antitagSR_TChiHH800; TH1F * h_J1Pt_antitagSB_TChiHH800; TH1F * h_J2Pt_antitagSB_TChiHH800;
  TH1F * h_J1Pt_tagSR_TChiHH800; TH1F * h_J2Pt_tagSR_TChiHH800; TH1F * h_J1Pt_tagSB_TChiHH800; TH1F * h_J2Pt_tagSB_TChiHH800;
  TH1F * h_baseline_j1bb_TChiHH800; TH1F * h_baseline_j2bb_TChiHH800;
  TH1F * h_H_j1pt_TChiHH800; TH1F * h_H_j2pt_TChiHH800;
  TH1F * h_J1BB_antitagSR_TChiHH800; TH1F * h_J2BB_antitagSR_TChiHH800; TH1F * h_J1BB_antitagSB_TChiHH800; TH1F * h_J2BB_antitagSB_TChiHH800;
  TH1F * h_J1BB_tagSR_TChiHH800; TH1F * h_J2BB_tagSR_TChiHH800; TH1F * h_J1BB_tagSB_TChiHH800; TH1F * h_J2BB_tagSB_TChiHH800;
  TH1F * h_J1BB_doubletagSR_TChiHH800; TH1F * h_J2BB_doubletagSR_TChiHH800; TH1F * h_J1BB_doubletagSB_TChiHH800; TH1F * h_J2BB_doubletagSB_TChiHH800;
  TH1F * h_H_j1bb_TChiHH800; TH1F * h_H_j2bb_TChiHH800;
  TH1F * h_0H_jpt_TChiHH800; TH1F * h_1H_jpt_TChiHH800; TH1F * h_2H_jpt_TChiHH800; TH1F * h_baseline_jpt_TChiHH800;
  TH1F * h_0H_jbb_TChiHH800; TH1F * h_1H_jbb_TChiHH800; TH1F * h_2H_jbb_TChiHH800; TH1F * h_baseline_jbb_TChiHH800;

  TH1F * h_baseline_j1pt_TChiHH1000; TH1F * h_baseline_j2pt_TChiHH1000; TH1F * h_J1Pt_doubletagSR_TChiHH1000; TH1F * h_J2Pt_doubletagSR_TChiHH1000; TH1F * h_J1Pt_doubletagSB_TChiHH1000; TH1F * h_J2Pt_doubletagSB_TChiHH1000;
  TH1F * h_J1Pt_antitagSR_TChiHH1000; TH1F * h_J2Pt_antitagSR_TChiHH1000; TH1F * h_J1Pt_antitagSB_TChiHH1000; TH1F * h_J2Pt_antitagSB_TChiHH1000;
  TH1F * h_J1Pt_tagSR_TChiHH1000; TH1F * h_J2Pt_tagSR_TChiHH1000; TH1F * h_J1Pt_tagSB_TChiHH1000; TH1F * h_J2Pt_tagSB_TChiHH1000;
  TH1F * h_baseline_j1bb_TChiHH1000; TH1F * h_baseline_j2bb_TChiHH1000;
  TH1F * h_H_j1pt_TChiHH1000; TH1F * h_H_j2pt_TChiHH1000;
  TH1F * h_J1BB_antitagSR_TChiHH1000; TH1F * h_J2BB_antitagSR_TChiHH1000; TH1F * h_J1BB_antitagSB_TChiHH1000; TH1F * h_J2BB_antitagSB_TChiHH1000;
  TH1F * h_J1BB_tagSR_TChiHH1000; TH1F * h_J2BB_tagSR_TChiHH1000; TH1F * h_J1BB_tagSB_TChiHH1000; TH1F * h_J2BB_tagSB_TChiHH1000;
  TH1F * h_J1BB_doubletagSR_TChiHH1000; TH1F * h_J2BB_doubletagSR_TChiHH1000; TH1F * h_J1BB_doubletagSB_TChiHH1000; TH1F * h_J2BB_doubletagSB_TChiHH1000;
  TH1F * h_H_j1bb_TChiHH1000; TH1F * h_H_j2bb_TChiHH1000;
  TH1F * h_0H_jpt_TChiHH1000; TH1F * h_1H_jpt_TChiHH1000; TH1F * h_2H_jpt_TChiHH1000; TH1F * h_baseline_jpt_TChiHH1000;
  TH1F * h_0H_jbb_TChiHH1000; TH1F * h_1H_jbb_TChiHH1000; TH1F * h_2H_jbb_TChiHH1000; TH1F * h_baseline_jbb_TChiHH1000;

  TH1F * h_baseline_j1pt_sum; TH1F * h_baseline_j2pt_sum;  TH1F * h_0H_j1pt_sum; TH1F * h_0H_j2pt_sum; TH1F * h_1H_j1pt_sum; TH1F * h_1H_j2pt_sum; TH1F * h_2H_j1pt_sum; TH1F * h_2H_j2pt_sum; TH1F * h_H_j1pt_sum; TH1F * h_H_j2pt_sum;
  TH1F * h_baseline_j1pt_data; TH1F * h_baseline_j2pt_data; TH1F * h_0H_j1pt_data; TH1F * h_0H_j2pt_data; TH1F * h_1H_j1pt_data; TH1F * h_1H_j2pt_data; TH1F * h_2H_j1pt_data; TH1F * h_2H_j2pt_data; TH1F * h_H_j1pt_data; TH1F * h_H_j2pt_data;
  TH1F * h_baseline_j1bb_sum; TH1F * h_baseline_j2bb_sum;  TH1F * h_0H_j1bb_sum; TH1F * h_0H_j2bb_sum; TH1F * h_1H_j1bb_sum; TH1F * h_1H_j2bb_sum; TH1F * h_2H_j1bb_sum; TH1F * h_2H_j2bb_sum; TH1F * h_H_j1bb_sum; TH1F * h_H_j2bb_sum;
  TH1F * h_baseline_j1bb_data; TH1F * h_baseline_j2bb_data; TH1F * h_0H_j1bb_data; TH1F * h_0H_j2bb_data; TH1F * h_1H_j1bb_data; TH1F * h_1H_j2bb_data; TH1F * h_2H_j1bb_data; TH1F * h_2H_j2bb_data; TH1F * h_H_j1bb_data; TH1F * h_H_j2bb_data;
  TH1F * h_baseline_j1bb_QCD; TH1F * h_baseline_j2bb_QCD;  TH1F * h_0H_j1bb_QCD; TH1F * h_0H_j2bb_QCD; TH1F * h_1H_j1bb_QCD; TH1F * h_1H_j2bb_QCD; TH1F * h_2H_j1bb_QCD; TH1F * h_2H_j2bb_QCD; TH1F * h_H_j1bb_QCD; TH1F * h_H_j2bb_QCD;
  TH1F * h_baseline_j1bb_GJets; TH1F * h_baseline_j2bb_GJets;
  TH1F * h_baseline_j1bb_WJets; TH1F * h_baseline_j2bb_WJets;  TH1F * h_0H_j1bb_WJets; TH1F * h_0H_j2bb_WJets; TH1F * h_1H_j1bb_WJets; TH1F * h_1H_j2bb_WJets; TH1F * h_2H_j1bb_WJets; TH1F * h_2H_j2bb_WJets; TH1F * h_H_j1bb_WJets; TH1F * h_H_j2bb_WJets;
  TH1F * h_baseline_j1bb_ZJets; TH1F * h_baseline_j2bb_ZJets; TH1F * h_0H_j1bb_ZJets; TH1F * h_0H_j2bb_ZJets; TH1F * h_1H_j1bb_ZJets; TH1F * h_1H_j2bb_ZJets; TH1F * h_2H_j1bb_ZJets; TH1F * h_2H_j2bb_ZJets; TH1F * h_H_j1bb_ZJets; TH1F * h_H_j2bb_ZJets;
  TH1F * h_baseline_j1bb_TT; TH1F * h_baseline_j2bb_TT; TH1F * h_0H_j1bb_TT; TH1F * h_0H_j2bb_TT; TH1F * h_1H_j1bb_TT; TH1F * h_1H_j2bb_TT; TH1F * h_2H_j1bb_TT; TH1F * h_2H_j2bb_TT; TH1F * h_H_j1bb_TT; TH1F * h_H_j2bb_TT;
  TH1F * h_baseline_j1bb_SnglT; TH1F * h_baseline_j2bb_SnglT; TH1F * h_0H_j1bb_SnglT; TH1F * h_0H_j2bb_SnglT; TH1F * h_1H_j1bb_SnglT; TH1F * h_1H_j2bb_SnglT; TH1F * h_2H_j1bb_SnglT; TH1F * h_2H_j2bb_SnglT; TH1F * h_H_j1bb_SnglT; TH1F * h_H_j2bb_SnglT;

  TH1F * h_Opt1_sum;
  TH1F * hP_Opt1_sum;
  TH1F * h_5Opt1_sum;
  TH1F * h_Opt1_TT;
  TH1F * h_Opt1_GJets; TH1F * hP_Opt1_GJets;
  TH1F * h_Opt1_WJets;
  TH1F * h_Opt1_ZJets;
  TH1F * h_Opt1_QCD;  TH1F * hP_Opt1_QCD;
  TH1F * h_Opt1_SnglT;

  double a_int; double a_error; double a1_int; double a1_error;
  double b_int; double b_error; double b1_int; double b1_error;
  double c_int; double c_error; double d_int; double d_error;
  double a_int_data; double a_error_data; double a1_int_data; double a1_error_data;
  double b_int_data; double b_error_data; double b1_int_data; double b1_error_data;
  double c_int_data; double c_error_data; double d_int_data; double d_error_data;

  if (whichRegion=="signal") {

    // h_A_data = (TH1F*)f->Get("MET_doubletagSR_data"); h_B_data = (TH1F*)f->Get("MET_doubletagSB_data");
    // h_A1_data = (TH1F*)f->Get("MET_tagSR_data"); h_B1_data = (TH1F*)f->Get("MET_tagSB_data");
    // h_C_data = (TH1F*)f->Get("MET_antitagSR_data"); h_D_data = (TH1F*)f->Get("MET_antitagSB_data");
    h_A_data = (TH1F*)fVetoData->Get("MET_doubletagSR_data"); h_B_data = (TH1F*)fVetoData->Get("MET_doubletagSB_data");
    h_A1_data = (TH1F*)fVetoData->Get("MET_tagSR_data"); h_B1_data = (TH1F*)fVetoData->Get("MET_tagSB_data");
    h_C_data = (TH1F*)fVetoData->Get("MET_antitagSR_data"); h_D_data = (TH1F*)fVetoData->Get("MET_antitagSB_data");


    // h_A5_sum = (TH1F*)f->Get("MET5_doubletagSR_sum"); h_B5_sum = (TH1F*)f->Get("MET5_doubletagSB_sum");
    // h_A15_sum = (TH1F*)f->Get("MET5_tagSR_sum"); h_B15_sum = (TH1F*)f->Get("MET5_tagSB_sum");
    // h_C5_sum = (TH1F*)f->Get("MET5_antitagSR_sum"); h_D5_sum = (TH1F*)f->Get("MET5_antitagSB_sum");
    // h_C5Opt1_sum = (TH1F*)f->Get("MET5_antitagSROpt1_sum"); h_D5Opt1_sum = (TH1F*)f->Get("MET5_antitagSBOpt1_sum");

    h_A_TT = (TH1F*)f->Get("MET_doubletagSR_TT"); h_B_TT = (TH1F*)f->Get("MET_doubletagSB_TT");
    h_A1_TT = (TH1F*)f->Get("MET_tagSR_TT"); h_B1_TT = (TH1F*)f->Get("MET_tagSB_TT");
    h_C_TT = (TH1F*)f->Get("MET_antitagSR_TT"); h_D_TT = (TH1F*)f->Get("MET_antitagSB_TT");

    // h_A_TT_Di = (TH1F*)f->Get("MET_doubletagSR_TT_Di"); h_B_TT_Di = (TH1F*)f->Get("MET_doubletagSB_TT_Di");
    // h_A1_TT_Di = (TH1F*)f->Get("MET_tagSR_TT_Di"); h_B1_TT_Di = (TH1F*)f->Get("MET_tagSB_TT_Di");
    // h_C_TT_Di = (TH1F*)f->Get("MET_antitagSR_TT_Di"); h_D_TT_Di = (TH1F*)f->Get("MET_antitagSB_TT_Di");
    // h_A_TT_SL = (TH1F*)f->Get("MET_doubletagSR_TT_SL"); h_B_TT_SL = (TH1F*)f->Get("MET_doubletagSB_TT_SL");
    // h_A1_TT_SL = (TH1F*)f->Get("MET_tagSR_TT_SL"); h_B1_TT_SL = (TH1F*)f->Get("MET_tagSB_TT_SL");
    // h_C_TT_SL = (TH1F*)f->Get("MET_antitagSR_TT_SL"); h_D_TT_SL = (TH1F*)f->Get("MET_antitagSB_TT_SL");

    h_A_WJets = (TH1F*)f->Get("MET_doubletagSR_WJets"); h_B_WJets = (TH1F*)f->Get("MET_doubletagSB_WJets");
    h_A1_WJets = (TH1F*)f->Get("MET_tagSR_WJets"); h_B1_WJets = (TH1F*)f->Get("MET_tagSB_WJets");
    h_C_WJets = (TH1F*)f->Get("MET_antitagSR_WJets");   h_D_WJets = (TH1F*)f->Get("MET_antitagSB_WJets");

    h_A_ZJets = (TH1F*)f->Get("MET_doubletagSR_ZJets"); h_B_ZJets = (TH1F*)f->Get("MET_doubletagSB_ZJets");
    h_A1_ZJets = (TH1F*)f->Get("MET_tagSR_ZJets"); h_B1_ZJets = (TH1F*)f->Get("MET_tagSB_ZJets");
    h_C_ZJets = (TH1F*)f->Get("MET_antitagSR_ZJets");   h_D_ZJets = (TH1F*)f->Get("MET_antitagSB_ZJets");

    h_A_QCD = (TH1F*)f->Get("MET_doubletagSR_QCD"); h_B_QCD = (TH1F*)f->Get("MET_doubletagSB_QCD");
    h_A1_QCD = (TH1F*)f->Get("MET_tagSR_QCD"); h_B1_QCD = (TH1F*)f->Get("MET_tagSB_QCD");
    h_C_QCD = (TH1F*)f->Get("MET_antitagSR_QCD"); h_D_QCD = (TH1F*)f->Get("MET_antitagSB_QCD");

    h_A_SnglT = (TH1F*)f->Get("MET_doubletagSR_SnglT"); h_B_SnglT = (TH1F*)f->Get("MET_doubletagSB_SnglT");
    h_A1_SnglT = (TH1F*)f->Get("MET_tagSR_SnglT"); h_B1_SnglT = (TH1F*)f->Get("MET_tagSB_SnglT");
    h_C_SnglT = (TH1F*)f->Get("MET_antitagSR_SnglT"); h_D_SnglT = (TH1F*)f->Get("MET_antitagSB_SnglT");

    h_Opt1_QCD = (TH1F*)f->Get("MET_antitagOpt1_QCD");
    h_Opt1_SnglT = (TH1F*)f->Get("MET_antitagOpt1_SnglT");
    h_Opt1_TT = (TH1F*)f->Get("MET_antitagOpt1_TT");
    h_Opt1_WJets = (TH1F*)f->Get("MET_antitagOpt1_WJets");
    h_Opt1_ZJets = (TH1F*)f->Get("MET_antitagOpt1_ZJets");
    // h_Opt1_data = (TH1F*)f->Get("MET_antitagOpt1_data");
    h_Opt1_data = (TH1F*)fVetoData->Get("MET_antitagOpt1_data");





    //Adding uncertainties to 0-event bins
    h_A_sum = make0EventUncSum({h_A_QCD,h_A_TT,h_A_WJets,h_A_ZJets,h_A_SnglT});
    h_B_sum = make0EventUncSum({h_B_QCD,h_B_TT,h_B_WJets,h_B_ZJets,h_B_SnglT});
    h_A1_sum = make0EventUncSum({h_A1_QCD,h_A1_TT,h_A1_WJets,h_A1_ZJets,h_A1_SnglT});
    h_B1_sum = make0EventUncSum({h_B1_QCD,h_B1_TT,h_B1_WJets,h_B1_ZJets,h_B1_SnglT});
    h_C_sum = make0EventUncSum({h_C_QCD,h_C_TT,h_C_WJets,h_C_ZJets,h_C_SnglT});
    h_D_sum = make0EventUncSum({h_D_QCD,h_D_TT,h_D_WJets,h_D_ZJets,h_D_SnglT});
    h_Opt1_sum = make0EventUncSum({h_Opt1_QCD,h_Opt1_TT,h_Opt1_WJets,h_Opt1_ZJets,h_Opt1_SnglT});

    //If unblinding signal region
    h_baseline_MET_SnglT = (TH1F*)f->Get("MET_baseline_SnglT");
    h_baseline_MET_QCD = (TH1F*)f->Get("MET_baseline_QCD");
    h_baseline_MET_ZJets = (TH1F*)f->Get("MET_baseline_ZJets");
    h_baseline_MET_WJets = (TH1F*)f->Get("MET_baseline_WJets");
    h_baseline_MET_TT = (TH1F*)f->Get("MET_baseline_TT");
    // h_baseline_MET_data = (TH1F*)f->Get("MET_baseline_data");
    h_baseline_MET_data = (TH1F*)fVetoData->Get("MET_baseline_data");
    h_baseline_MET_sum = (TH1F*)f->Get("MET_baseline_sum");

    h_0H_MET_sum = (TH1F*)h_C_sum->Clone("h_0H_MET_sum");  h_0H_MET_sum->Add(h_D_sum);
    h_0H_MET_SnglT = (TH1F*)h_C_SnglT->Clone("h_0H_MET_SnglT");  h_0H_MET_SnglT->Add(h_D_SnglT);
    h_0H_MET_QCD = (TH1F*)h_C_QCD->Clone("h_0H_MET_QCD");  h_0H_MET_QCD->Add(h_D_QCD);
    h_0H_MET_ZJets = (TH1F*)h_C_ZJets->Clone("h_0H_MET_ZJets");  h_0H_MET_ZJets->Add(h_D_ZJets);
    h_0H_MET_WJets = (TH1F*)h_C_WJets->Clone("h_0H_MET_WJets");  h_0H_MET_WJets->Add(h_D_WJets);
    h_0H_MET_TT = (TH1F*)h_C_TT->Clone("h_0H_MET_TT");  h_0H_MET_TT->Add(h_D_TT);
    h_0H_MET_data = (TH1F*)h_C_data->Clone("h_0H_MET_data");  h_0H_MET_data->Add(h_D_data);

    h_1H_MET_sum = (TH1F*)h_A1_sum->Clone("h_1H_MET_sum");  h_1H_MET_sum->Add(h_B1_sum);
    h_1H_MET_SnglT = (TH1F*)h_A1_SnglT->Clone("h_1H_MET_SnglT");  h_1H_MET_SnglT->Add(h_B1_SnglT);
    h_1H_MET_QCD = (TH1F*)h_A1_QCD->Clone("h_1H_MET_QCD");  h_1H_MET_QCD->Add(h_B1_QCD);
    h_1H_MET_ZJets = (TH1F*)h_A1_ZJets->Clone("h_1H_MET_ZJets");  h_1H_MET_ZJets->Add(h_B1_ZJets);
    h_1H_MET_WJets = (TH1F*)h_A1_WJets->Clone("h_1H_MET_WJets");  h_1H_MET_WJets->Add(h_B1_WJets);
    h_1H_MET_TT = (TH1F*)h_A1_TT->Clone("h_1H_MET_TT");  h_1H_MET_TT->Add(h_B1_TT);
    h_1H_MET_data = (TH1F*)h_A1_data->Clone("h_1H_MET_data");  h_1H_MET_data->Add(h_B1_data);

    h_2H_MET_sum = (TH1F*)h_A_sum->Clone("h_2H_MET_sum");  h_2H_MET_sum->Add(h_B_sum);
    h_2H_MET_SnglT = (TH1F*)h_A_SnglT->Clone("h_2H_MET_SnglT");  h_2H_MET_SnglT->Add(h_B_SnglT);
    h_2H_MET_QCD = (TH1F*)h_A_QCD->Clone("h_2H_MET_QCD");  h_2H_MET_QCD->Add(h_B_QCD);
    h_2H_MET_ZJets = (TH1F*)h_A_ZJets->Clone("h_2H_MET_ZJets");  h_2H_MET_ZJets->Add(h_B_ZJets);
    h_2H_MET_WJets = (TH1F*)h_A_WJets->Clone("h_2H_MET_WJets");  h_2H_MET_WJets->Add(h_B_WJets);
    h_2H_MET_TT = (TH1F*)h_A_TT->Clone("h_2H_MET_TT");  h_2H_MET_TT->Add(h_B_TT);
    h_2H_MET_data = (TH1F*)h_A_data->Clone("h_2H_MET_data");  h_2H_MET_data->Add(h_B_data);

    h_H_MET_sum  = (TH1F*)h_baseline_MET_sum->Clone("h_H_MET_sum"); h_H_MET_sum->Add(h_0H_MET_sum,-1);
    h_H_MET_data  = (TH1F*)h_baseline_MET_data->Clone("h_H_MET_data"); h_H_MET_data->Add(h_0H_MET_data,-1);
    h_H_MET_ZJets  = (TH1F*)h_baseline_MET_ZJets->Clone("h_H_MET_ZJets"); h_H_MET_ZJets->Add(h_0H_MET_ZJets,-1);
    h_H_MET_WJets  = (TH1F*)h_baseline_MET_WJets->Clone("h_H_MET_WJets"); h_H_MET_WJets->Add(h_0H_MET_WJets,-1);
    h_H_MET_TT  = (TH1F*)h_baseline_MET_TT->Clone("h_H_MET_TT"); h_H_MET_TT->Add(h_0H_MET_TT,-1);
    h_H_MET_SnglT  = (TH1F*)h_baseline_MET_SnglT->Clone("h_H_MET_SnglT"); h_H_MET_SnglT->Add(h_0H_MET_SnglT,-1);
    h_H_MET_QCD  = (TH1F*)h_baseline_MET_QCD->Clone("h_H_MET_QCD"); h_H_MET_QCD->Add(h_0H_MET_QCD,-1);


    b_int = h_B_sum->IntegralAndError(1,5,b_error,""); b1_int = h_B1_sum->IntegralAndError(1,5,b1_error,"");
    c_int = h_C_sum->IntegralAndError(1,5,c_error,""); d_int = h_D_sum->IntegralAndError(1,5,d_error,"");
    a_int = h_A_sum->IntegralAndError(1,5,a_error,""); a1_int = h_A1_sum->IntegralAndError(1,5,a1_error,"");

    h_nH_sum->SetBinContent(1,c_int+d_int);   h_nH_sum->SetBinError(1,sqrt(c_error*c_error + d_error*d_error));
    h_nH_sum->SetBinContent(2,a1_int+b1_int); h_nH_sum->SetBinError(2,sqrt(a1_error*a1_error + b1_error*b1_error));
    h_nH_sum->SetBinContent(3,a_int+b_int);   h_nH_sum->SetBinError(3,sqrt(a_error*a_error + b_error*b_error));
    h_nH_data->SetBinContent(1,c_int_data+d_int_data);   h_nH_data->SetBinError(1,sqrt(c_error_data*c_error_data + d_error_data*d_error_data));
    h_nH_data->SetBinContent(2,a1_int_data+b1_int_data); h_nH_data->SetBinError(2,sqrt(a1_error_data*a1_error_data + b1_error_data*b1_error_data));
    h_nH_data->SetBinContent(3,a_int_data+b_int_data);   h_nH_data->SetBinError(3,sqrt(a_error_data*a_error_data + b_error_data*b_error_data));

    cdABCDReg->cd();
    h_A_sum->Write("h_A_sum");
    h_A1_sum->Write("h_A1_sum");
    h_B_sum->Write("h_B_sum");
    h_B1_sum->Write("h_B1_sum");
    h_C_sum->Write("h_C_sum");
    h_D_sum->Write("h_D_sum");
    h_Opt1_sum->Write("h_T0_sum");
    std::cout<<"Here? 5"<<std::endl;


    hP_A_sum = (TH1F*)f->Get("METPhoton_doubletagSR_sum"); hP_A1_sum = (TH1F*)f->Get("METPhoton_tagSR_sum");
    hP_B_sum = (TH1F*)f->Get("METPhoton_doubletagSB_sum"); hP_B1_sum = (TH1F*)f->Get("METPhoton_tagSB_sum");
    hP_C_sum = (TH1F*)f->Get("METPhoton_antitagSR_sum"); hP_D_sum = (TH1F*)f->Get("METPhoton_antitagSB_sum");
    hP_Opt1_sum = (TH1F*)f->Get("METPhoton_antitagOpt1_sum");

    if (runSignal){
      std::cout<<"Here? 6"<<std::endl;

      h_A_T5HH1600 = (TH1F*)fSignal2->Get("MET_doubletagSR_T5HH1600_LSP1"); h_B_T5HH1600 = (TH1F*)fSignal2->Get("MET_doubletagSB_T5HH1600_LSP1");
      h_A1_T5HH1600 = (TH1F*)fSignal2->Get("MET_tagSR_T5HH1600_LSP1"); h_B1_T5HH1600 = (TH1F*)fSignal2->Get("MET_tagSB_T5HH1600_LSP1");
      h_C_T5HH1600 = (TH1F*)fSignal2->Get("MET_antitagSR_T5HH1600_LSP1"); h_D_T5HH1600 = (TH1F*)fSignal2->Get("MET_antitagSB_T5HH1600_LSP1");
      h_baseline_MET_T5HH1600 = (TH1F*)fSignal2->Get("MET_baseline_T5HH1600_LSP1");
      h_H_MET_T5HH1600 = (TH1F*)h_baseline_MET_T5HH1600->Clone("h_H_MET_T5HH1600"); h_H_MET_T5HH1600->Add(h_C_T5HH1600,-1); h_H_MET_T5HH1600->Add(h_D_T5HH1600,-1);
      h_0H_MET_T5HH1600 = (TH1F*)h_C_T5HH1600->Clone("h_0H_MET_T5HH1600"); h_0H_MET_T5HH1600->Add(h_D_T5HH1600);
      h_1H_MET_T5HH1600 = (TH1F*)h_A1_T5HH1600->Clone("h_1H_MET_T5HH1600"); h_1H_MET_T5HH1600->Add(h_B1_T5HH1600);
      h_2H_MET_T5HH1600 = (TH1F*)h_A_T5HH1600->Clone("h_2H_MET_T5HH1600"); h_2H_MET_T5HH1600->Add(h_B_T5HH1600);

      h_A_T5HH2000 = (TH1F*)fSignal2->Get("MET_doubletagSR_T5HH2000_LSP1"); h_B_T5HH2000 = (TH1F*)fSignal2->Get("MET_doubletagSB_T5HH2000_LSP1");
      h_A1_T5HH2000 = (TH1F*)fSignal2->Get("MET_tagSR_T5HH2000_LSP1"); h_B1_T5HH2000 = (TH1F*)fSignal2->Get("MET_tagSB_T5HH2000_LSP1");
      h_C_T5HH2000 = (TH1F*)fSignal2->Get("MET_antitagSR_T5HH2000_LSP1"); h_D_T5HH2000 = (TH1F*)fSignal2->Get("MET_antitagSB_T5HH2000_LSP1");
      h_baseline_MET_T5HH2000 = (TH1F*)fSignal2->Get("MET_baseline_T5HH2000_LSP1");
      h_H_MET_T5HH2000 = (TH1F*)h_baseline_MET_T5HH2000->Clone("h_H_MET_T5HH2000"); h_H_MET_T5HH2000->Add(h_C_T5HH2000,-1); h_H_MET_T5HH2000->Add(h_D_T5HH2000,-1);

      h_A_T5HH2200 = (TH1F*)fSignal2->Get("MET_doubletagSR_T5HH2200_LSP1"); h_B_T5HH2200 = (TH1F*)fSignal2->Get("MET_doubletagSB_T5HH2200_LSP1");
      h_A1_T5HH2200 = (TH1F*)fSignal2->Get("MET_tagSR_T5HH2200_LSP1"); h_B1_T5HH2200 = (TH1F*)fSignal2->Get("MET_tagSB_T5HH2200_LSP1");
      h_C_T5HH2200 = (TH1F*)fSignal2->Get("MET_antitagSR_T5HH2200_LSP1"); h_D_T5HH2200 = (TH1F*)fSignal2->Get("MET_antitagSB_T5HH2200_LSP1");
      h_baseline_MET_T5HH2200 = (TH1F*)fSignal2->Get("MET_baseline_T5HH2200_LSP1");
      h_H_MET_T5HH2200 = (TH1F*)h_baseline_MET_T5HH2200->Clone("h_H_MET_T5HH2200"); h_H_MET_T5HH2200->Add(h_C_T5HH2200,-1); h_H_MET_T5HH2200->Add(h_D_T5HH2200,-1);

      h_A_TChiHH500 = (TH1F*)fSignal->Get("MET_doubletagSR_TChiHH500_LSP1"); h_B_TChiHH500 = (TH1F*)fSignal->Get("MET_doubletagSB_TChiHH500_LSP1");
      h_A1_TChiHH500 = (TH1F*)fSignal->Get("MET_tagSR_TChiHH500_LSP1"); h_B1_TChiHH500 = (TH1F*)fSignal->Get("MET_tagSB_TChiHH500_LSP1");
      h_C_TChiHH500 = (TH1F*)fSignal->Get("MET_antitagSR_TChiHH500_LSP1"); h_D_TChiHH500 = (TH1F*)fSignal->Get("MET_antitagSB_TChiHH500_LSP1");
      h_baseline_MET_TChiHH500 = (TH1F*)fSignal->Get("MET_baseline_TChiHH500_LSP1");
      h_H_MET_TChiHH500 = (TH1F*)h_baseline_MET_TChiHH500->Clone("h_H_MET_TChiHH500"); h_H_MET_TChiHH500->Add(h_C_TChiHH500,-1); h_H_MET_TChiHH500->Add(h_D_TChiHH500,-1);
      h_0H_MET_TChiHH500 = (TH1F*)h_C_TChiHH500->Clone("h_0H_MET_TChiHH500"); h_0H_MET_TChiHH500->Add(h_D_TChiHH500);
      h_1H_MET_TChiHH500 = (TH1F*)h_A1_TChiHH500->Clone("h_1H_MET_TChiHH500"); h_1H_MET_TChiHH500->Add(h_B1_TChiHH500);
      h_2H_MET_TChiHH500 = (TH1F*)h_A_TChiHH500->Clone("h_2H_MET_TChiHH500"); h_2H_MET_TChiHH500->Add(h_B_TChiHH500);

      h_A_TChiHH800 = (TH1F*)fSignal->Get("MET_doubletagSR_TChiHH800_LSP1"); h_B_TChiHH800 = (TH1F*)fSignal->Get("MET_doubletagSB_TChiHH800_LSP1");
      h_A1_TChiHH800 = (TH1F*)fSignal->Get("MET_tagSR_TChiHH800_LSP1"); h_B1_TChiHH800 = (TH1F*)fSignal->Get("MET_tagSB_TChiHH800_LSP1");
      h_C_TChiHH800 = (TH1F*)fSignal->Get("MET_antitagSR_TChiHH800_LSP1"); h_D_TChiHH800 = (TH1F*)fSignal->Get("MET_antitagSB_TChiHH800_LSP1");
      h_baseline_MET_TChiHH800 = (TH1F*)fSignal->Get("MET_baseline_TChiHH800_LSP1");
      h_H_MET_TChiHH800 = (TH1F*)h_baseline_MET_TChiHH800->Clone("h_H_MET_TChiHH800"); h_H_MET_TChiHH800->Add(h_C_TChiHH800,-1); h_H_MET_TChiHH800->Add(h_D_TChiHH800,-1);

      h_A_TChiHH1000 = (TH1F*)fSignal->Get("MET_doubletagSR_TChiHH1000_LSP1"); h_B_TChiHH1000 = (TH1F*)fSignal->Get("MET_doubletagSB_TChiHH1000_LSP1");
      h_A1_TChiHH1000 = (TH1F*)fSignal->Get("MET_tagSR_TChiHH1000_LSP1"); h_B1_TChiHH1000 = (TH1F*)fSignal->Get("MET_tagSB_TChiHH1000_LSP1");
      h_C_TChiHH1000 = (TH1F*)fSignal->Get("MET_antitagSR_TChiHH1000_LSP1"); h_D_TChiHH1000 = (TH1F*)fSignal->Get("MET_antitagSB_TChiHH1000_LSP1");
      h_baseline_MET_TChiHH1000 = (TH1F*)fSignal->Get("MET_baseline_TChiHH1000_LSP1");
      h_H_MET_TChiHH1000 = (TH1F*)h_baseline_MET_TChiHH1000->Clone("h_H_MET_TChiHH1000"); h_H_MET_TChiHH1000->Add(h_C_TChiHH1000,-1); h_H_MET_TChiHH1000->Add(h_D_TChiHH1000,-1);
      std::cout<<"Here? 7"<<std::endl;

    }

    std::cout<<"Here? 8"<<std::endl;

    h_J1M_doubletagSR_sum = (TH1F*)f->Get("J1pt_M_doubletagSR_sum"); h_J2M_doubletagSR_sum = (TH1F*)f->Get("J2pt_M_doubletagSR_sum");
    h_J1M_doubletagSB_sum = (TH1F*)f->Get("J1pt_M_doubletagSB_sum"); h_J2M_doubletagSB_sum = (TH1F*)f->Get("J2pt_M_doubletagSB_sum");
    h_J1M_antitagSR_sum = (TH1F*)f->Get("J1pt_M_antitagSR_sum"); h_J2M_antitagSR_sum = (TH1F*)f->Get("J2pt_M_antitagSR_sum");
    h_J1M_antitagSB_sum = (TH1F*)f->Get("J1pt_M_antitagSB_sum"); h_J2M_antitagSB_sum = (TH1F*)f->Get("J2pt_M_antitagSB_sum");
    h_J1M_tagSR_sum = (TH1F*)f->Get("J1pt_M_tagSR_sum"); h_J2M_tagSR_sum = (TH1F*)f->Get("J2pt_M_tagSR_sum");
    h_J1M_tagSB_sum = (TH1F*)f->Get("J1pt_M_tagSB_sum"); h_J2M_tagSB_sum = (TH1F*)f->Get("J2pt_M_tagSB_sum");

    h_J1J2M_doubletagSR_sum = (TH1F*)h_J1M_doubletagSR_sum->Clone("J1J2M_doubletagSR_sum"); h_J1J2M_doubletagSR_sum->Add(h_J2M_doubletagSR_sum);
    h_J1J2M_doubletagSB_sum = (TH1F*)h_J1M_doubletagSB_sum->Clone("J1J2M_doubletagSB_sum"); h_J1J2M_doubletagSB_sum->Add(h_J2M_doubletagSB_sum);
    h_J1J2M_tagSR_sum = (TH1F*)h_J1M_tagSR_sum->Clone("J1J2M_tagSR_sum"); h_J1J2M_tagSR_sum->Add(h_J2M_tagSR_sum);
    h_J1J2M_tagSB_sum = (TH1F*)h_J1M_tagSB_sum->Clone("J1J2M_tagSB_sum"); h_J1J2M_tagSB_sum->Add(h_J2M_tagSB_sum);
    h_J1J2M_antitagSR_sum = (TH1F*)h_J1M_antitagSR_sum->Clone("J1J2M_antitagSR_sum"); h_J1J2M_antitagSR_sum->Add(h_J2M_antitagSR_sum);
    h_J1J2M_antitagSB_sum = (TH1F*)h_J1M_antitagSB_sum->Clone("J1J2M_antitagSB_sum"); h_J1J2M_antitagSB_sum->Add(h_J2M_antitagSB_sum);

    h_J1M_doubletagSR_ZJets = (TH1F*)f->Get("J1pt_M_doubletagSR_ZJets"); h_J2M_doubletagSR_ZJets = (TH1F*)f->Get("J2pt_M_doubletagSR_ZJets");
    h_J1M_doubletagSB_ZJets = (TH1F*)f->Get("J1pt_M_doubletagSB_ZJets"); h_J2M_doubletagSB_ZJets = (TH1F*)f->Get("J2pt_M_doubletagSB_ZJets");
    h_J1M_antitagSR_ZJets = (TH1F*)f->Get("J1pt_M_antitagSR_ZJets"); h_J2M_antitagSR_ZJets = (TH1F*)f->Get("J2pt_M_antitagSR_ZJets");
    h_J1M_antitagSB_ZJets = (TH1F*)f->Get("J1pt_M_antitagSB_ZJets"); h_J2M_antitagSB_ZJets = (TH1F*)f->Get("J2pt_M_antitagSB_ZJets");
    h_J1M_tagSR_ZJets = (TH1F*)f->Get("J1pt_M_tagSR_ZJets"); h_J2M_tagSR_ZJets = (TH1F*)f->Get("J2pt_M_tagSR_ZJets");
    h_J1M_tagSB_ZJets = (TH1F*)f->Get("J1pt_M_tagSB_ZJets"); h_J2M_tagSB_ZJets = (TH1F*)f->Get("J2pt_M_tagSB_ZJets");
    h_baseline_j1mass_ZJets = (TH1F*)f->Get("J1pt_M_baseline_ZJets"); h_baseline_j2mass_ZJets = (TH1F*)f->Get("J2pt_M_baseline_ZJets");
    h_0H_j1mass_ZJets = (TH1F*)h_J1M_antitagSR_ZJets->Clone("h_0H_j1mass_ZJets");  h_0H_j1mass_ZJets->Add(h_J1M_antitagSB_ZJets);
    h_0H_j2mass_ZJets = (TH1F*)h_J2M_antitagSR_ZJets->Clone("h_0H_j2mass_ZJets");  h_0H_j2mass_ZJets->Add(h_J2M_antitagSB_ZJets);
    h_H_j1mass_ZJets = (TH1F*)h_baseline_j1mass_ZJets->Clone("h_H_j1mass_ZJets");  h_H_j1mass_ZJets->Add(h_0H_j1mass_ZJets,-1);
    h_H_j2mass_ZJets = (TH1F*)h_baseline_j2mass_ZJets->Clone("h_H_j2mass_ZJets");  h_H_j2mass_ZJets->Add(h_0H_j2mass_ZJets,-1);
    h_1H_j1mass_ZJets = (TH1F*)h_J1M_tagSR_ZJets->Clone("h_1H_j1mass_ZJets");  h_1H_j1mass_ZJets->Add(h_J1M_tagSB_ZJets);
    h_1H_j2mass_ZJets = (TH1F*)h_J2M_tagSR_ZJets->Clone("h_1H_j2mass_ZJets");  h_1H_j2mass_ZJets->Add(h_J2M_tagSB_ZJets);
    h_2H_j1mass_ZJets = (TH1F*)h_J1M_doubletagSR_ZJets->Clone("h_2H_j1mass_ZJets");  h_2H_j1mass_ZJets->Add(h_J1M_doubletagSB_ZJets);
    h_2H_j2mass_ZJets = (TH1F*)h_J2M_doubletagSR_ZJets->Clone("h_2H_j2mass_ZJets");  h_2H_j2mass_ZJets->Add(h_J2M_doubletagSB_ZJets);

    h_baseline_jmass_ZJets = (TH1F*)h_baseline_j1mass_ZJets->Clone("h_baseline_jmass_ZJets");h_baseline_jmass_ZJets->Add(h_baseline_j2mass_ZJets);
    h_0H_jmass_ZJets = (TH1F*)h_0H_j1mass_ZJets->Clone("h_0H_jmass_ZJets");h_0H_jmass_ZJets->Add(h_0H_j2mass_ZJets);
    h_1H_jmass_ZJets = (TH1F*)h_1H_j1mass_ZJets->Clone("h_1H_jmass_ZJets");h_1H_jmass_ZJets->Add(h_1H_j2mass_ZJets);
    h_2H_jmass_ZJets = (TH1F*)h_2H_j1mass_ZJets->Clone("h_2H_jmass_ZJets");h_2H_jmass_ZJets->Add(h_2H_j2mass_ZJets);

    std::cout<<"Here? 9"<<std::endl;


    h_J1M_doubletagSR_QCD = (TH1F*)f->Get("J1pt_M_doubletagSR_QCD"); h_J2M_doubletagSR_QCD = (TH1F*)f->Get("J2pt_M_doubletagSR_QCD");
    h_J1M_doubletagSB_QCD = (TH1F*)f->Get("J1pt_M_doubletagSB_QCD"); h_J2M_doubletagSB_QCD = (TH1F*)f->Get("J2pt_M_doubletagSB_QCD");
    h_J1M_antitagSR_QCD = (TH1F*)f->Get("J1pt_M_antitagSR_QCD"); h_J2M_antitagSR_QCD = (TH1F*)f->Get("J2pt_M_antitagSR_QCD");
    h_J1M_antitagSB_QCD = (TH1F*)f->Get("J1pt_M_antitagSB_QCD"); h_J2M_antitagSB_QCD = (TH1F*)f->Get("J2pt_M_antitagSB_QCD");
    h_J1M_tagSR_QCD = (TH1F*)f->Get("J1pt_M_tagSR_QCD"); h_J2M_tagSR_QCD = (TH1F*)f->Get("J2pt_M_tagSR_QCD");
    h_J1M_tagSB_QCD = (TH1F*)f->Get("J1pt_M_tagSB_QCD"); h_J2M_tagSB_QCD = (TH1F*)f->Get("J2pt_M_tagSB_QCD");
    h_baseline_j1mass_QCD = (TH1F*)f->Get("J1pt_M_baseline_QCD"); h_baseline_j2mass_QCD = (TH1F*)f->Get("J2pt_M_baseline_QCD");
    h_0H_j1mass_QCD = (TH1F*)h_J1M_antitagSR_QCD->Clone("h_0H_j1mass_QCD");  h_0H_j1mass_QCD->Add(h_J1M_antitagSB_QCD);
    h_0H_j2mass_QCD = (TH1F*)h_J2M_antitagSR_QCD->Clone("h_0H_j2mass_QCD");  h_0H_j2mass_QCD->Add(h_J2M_antitagSB_QCD);
    h_H_j1mass_QCD = (TH1F*)h_baseline_j1mass_QCD->Clone("h_H_j1mass_QCD");  h_H_j1mass_QCD->Add(h_0H_j1mass_QCD,-1);
    h_H_j2mass_QCD = (TH1F*)h_baseline_j2mass_QCD->Clone("h_H_j2mass_QCD");  h_H_j2mass_QCD->Add(h_0H_j2mass_QCD,-1);
    h_1H_j1mass_QCD = (TH1F*)h_J1M_tagSR_QCD->Clone("h_1H_j1mass_QCD");  h_1H_j1mass_QCD->Add(h_J1M_tagSB_QCD);
    h_1H_j2mass_QCD = (TH1F*)h_J2M_tagSR_QCD->Clone("h_1H_j2mass_QCD");  h_1H_j2mass_QCD->Add(h_J2M_tagSB_QCD);
    h_2H_j1mass_QCD = (TH1F*)h_J1M_doubletagSR_QCD->Clone("h_2H_j1mass_QCD");  h_2H_j1mass_QCD->Add(h_J1M_doubletagSB_QCD);
    h_2H_j2mass_QCD = (TH1F*)h_J2M_doubletagSR_QCD->Clone("h_2H_j2mass_QCD");  h_2H_j2mass_QCD->Add(h_J2M_doubletagSB_QCD);

    h_baseline_jmass_QCD = (TH1F*)h_baseline_j1mass_QCD->Clone("h_baseline_jmass_QCD");h_baseline_jmass_QCD->Add(h_baseline_j2mass_QCD);
    h_0H_jmass_QCD = (TH1F*)h_0H_j1mass_QCD->Clone("h_0H_jmass_QCD");h_0H_jmass_QCD->Add(h_0H_j2mass_QCD);
    h_1H_jmass_QCD = (TH1F*)h_1H_j1mass_QCD->Clone("h_1H_jmass_QCD");h_1H_jmass_QCD->Add(h_1H_j2mass_QCD);
    h_2H_jmass_QCD = (TH1F*)h_2H_j1mass_QCD->Clone("h_2H_jmass_QCD");h_2H_jmass_QCD->Add(h_2H_j2mass_QCD);


    h_J1M_doubletagSR_SnglT = (TH1F*)f->Get("J1pt_M_doubletagSR_SnglT"); h_J2M_doubletagSR_SnglT = (TH1F*)f->Get("J2pt_M_doubletagSR_SnglT");
    h_J1M_doubletagSB_SnglT = (TH1F*)f->Get("J1pt_M_doubletagSB_SnglT"); h_J2M_doubletagSB_SnglT = (TH1F*)f->Get("J2pt_M_doubletagSB_SnglT");
    h_J1M_antitagSR_SnglT = (TH1F*)f->Get("J1pt_M_antitagSR_SnglT"); h_J2M_antitagSR_SnglT = (TH1F*)f->Get("J2pt_M_antitagSR_SnglT");
    h_J1M_antitagSB_SnglT = (TH1F*)f->Get("J1pt_M_antitagSB_SnglT"); h_J2M_antitagSB_SnglT = (TH1F*)f->Get("J2pt_M_antitagSB_SnglT");
    h_J1M_tagSR_SnglT = (TH1F*)f->Get("J1pt_M_tagSR_SnglT"); h_J2M_tagSR_SnglT = (TH1F*)f->Get("J2pt_M_tagSR_SnglT");
    h_J1M_tagSB_SnglT = (TH1F*)f->Get("J1pt_M_tagSB_SnglT"); h_J2M_tagSB_SnglT = (TH1F*)f->Get("J2pt_M_tagSB_SnglT");
    h_baseline_j1mass_SnglT = (TH1F*)f->Get("J1pt_M_baseline_SnglT"); h_baseline_j2mass_SnglT = (TH1F*)f->Get("J2pt_M_baseline_SnglT");
    h_0H_j1mass_SnglT = (TH1F*)h_J1M_antitagSR_SnglT->Clone("h_0H_j1mass_SnglT");  h_0H_j1mass_SnglT->Add(h_J1M_antitagSB_SnglT);
    h_0H_j2mass_SnglT = (TH1F*)h_J2M_antitagSR_SnglT->Clone("h_0H_j2mass_SnglT");  h_0H_j2mass_SnglT->Add(h_J2M_antitagSB_SnglT);
    h_H_j1mass_SnglT = (TH1F*)h_baseline_j1mass_SnglT->Clone("h_H_j1mass_SnglT");  h_H_j1mass_SnglT->Add(h_0H_j1mass_SnglT,-1);
    h_H_j2mass_SnglT = (TH1F*)h_baseline_j2mass_SnglT->Clone("h_H_j2mass_SnglT");  h_H_j2mass_SnglT->Add(h_0H_j2mass_SnglT,-1);
    h_1H_j1mass_SnglT = (TH1F*)h_J1M_tagSR_SnglT->Clone("h_1H_j1mass_SnglT");  h_1H_j1mass_SnglT->Add(h_J1M_tagSB_SnglT);
    h_1H_j2mass_SnglT = (TH1F*)h_J2M_tagSR_SnglT->Clone("h_1H_j2mass_SnglT");  h_1H_j2mass_SnglT->Add(h_J2M_tagSB_SnglT);
    h_2H_j1mass_SnglT = (TH1F*)h_J1M_doubletagSR_SnglT->Clone("h_2H_j1mass_SnglT");  h_2H_j1mass_SnglT->Add(h_J1M_doubletagSB_SnglT);
    h_2H_j2mass_SnglT = (TH1F*)h_J2M_doubletagSR_SnglT->Clone("h_2H_j2mass_SnglT");  h_2H_j2mass_SnglT->Add(h_J2M_doubletagSB_SnglT);

    h_baseline_jmass_SnglT = (TH1F*)h_baseline_j1mass_SnglT->Clone("h_baseline_jmass_SnglT");h_baseline_jmass_SnglT->Add(h_baseline_j2mass_SnglT);
    h_0H_jmass_SnglT = (TH1F*)h_0H_j1mass_SnglT->Clone("h_0H_jmass_SnglT");h_0H_jmass_SnglT->Add(h_0H_j2mass_SnglT);
    h_1H_jmass_SnglT = (TH1F*)h_1H_j1mass_SnglT->Clone("h_1H_jmass_SnglT");h_1H_jmass_SnglT->Add(h_1H_j2mass_SnglT);
    h_2H_jmass_SnglT = (TH1F*)h_2H_j1mass_SnglT->Clone("h_2H_jmass_SnglT");h_2H_jmass_SnglT->Add(h_2H_j2mass_SnglT);


    h_J1M_doubletagSR_TT = (TH1F*)f->Get("J1pt_M_doubletagSR_TT"); h_J2M_doubletagSR_TT = (TH1F*)f->Get("J2pt_M_doubletagSR_TT");
    h_J1M_doubletagSB_TT = (TH1F*)f->Get("J1pt_M_doubletagSB_TT"); h_J2M_doubletagSB_TT = (TH1F*)f->Get("J2pt_M_doubletagSB_TT");
    h_J1M_antitagSR_TT = (TH1F*)f->Get("J1pt_M_antitagSR_TT"); h_J2M_antitagSR_TT = (TH1F*)f->Get("J2pt_M_antitagSR_TT");
    h_J1M_antitagSB_TT = (TH1F*)f->Get("J1pt_M_antitagSB_TT"); h_J2M_antitagSB_TT = (TH1F*)f->Get("J2pt_M_antitagSB_TT");
    h_J1M_tagSR_TT = (TH1F*)f->Get("J1pt_M_tagSR_TT"); h_J2M_tagSR_TT = (TH1F*)f->Get("J2pt_M_tagSR_TT");
    h_J1M_tagSB_TT = (TH1F*)f->Get("J1pt_M_tagSB_TT"); h_J2M_tagSB_TT = (TH1F*)f->Get("J2pt_M_tagSB_TT");
    h_baseline_j1mass_TT = (TH1F*)f->Get("J1pt_M_baseline_TT"); h_baseline_j2mass_TT = (TH1F*)f->Get("J2pt_M_baseline_TT");
    h_0H_j1mass_TT = (TH1F*)h_J1M_antitagSR_TT->Clone("h_0H_j1mass_TT");  h_0H_j1mass_TT->Add(h_J1M_antitagSB_TT);
    h_0H_j2mass_TT = (TH1F*)h_J2M_antitagSR_TT->Clone("h_0H_j2mass_TT");  h_0H_j2mass_TT->Add(h_J2M_antitagSB_TT);
    h_H_j1mass_TT = (TH1F*)h_baseline_j1mass_TT->Clone("h_H_j1mass_TT");  h_H_j1mass_TT->Add(h_0H_j1mass_TT,-1);
    h_H_j2mass_TT = (TH1F*)h_baseline_j2mass_TT->Clone("h_H_j2mass_TT");  h_H_j2mass_TT->Add(h_0H_j2mass_TT,-1);
    h_1H_j1mass_TT = (TH1F*)h_J1M_tagSR_TT->Clone("h_1H_j1mass_TT");  h_1H_j1mass_TT->Add(h_J1M_tagSB_TT);
    h_1H_j2mass_TT = (TH1F*)h_J2M_tagSR_TT->Clone("h_1H_j2mass_TT");  h_1H_j2mass_TT->Add(h_J2M_tagSB_TT);
    h_2H_j1mass_TT = (TH1F*)h_J1M_doubletagSR_TT->Clone("h_2H_j1mass_TT");  h_2H_j1mass_TT->Add(h_J1M_doubletagSB_TT);
    h_2H_j2mass_TT = (TH1F*)h_J2M_doubletagSR_TT->Clone("h_2H_j2mass_TT");  h_2H_j2mass_TT->Add(h_J2M_doubletagSB_TT);

    h_baseline_jmass_TT = (TH1F*)h_baseline_j1mass_TT->Clone("h_baseline_jmass_TT");h_baseline_jmass_TT->Add(h_baseline_j2mass_TT);
    h_0H_jmass_TT = (TH1F*)h_0H_j1mass_TT->Clone("h_0H_jmass_TT");h_0H_jmass_TT->Add(h_0H_j2mass_TT);
    h_1H_jmass_TT = (TH1F*)h_1H_j1mass_TT->Clone("h_1H_jmass_TT");h_1H_jmass_TT->Add(h_1H_j2mass_TT);
    h_2H_jmass_TT = (TH1F*)h_2H_j1mass_TT->Clone("h_2H_jmass_TT");h_2H_jmass_TT->Add(h_2H_j2mass_TT);


    // h_J1M_doubletagSR_TT_SL = (TH1F*)f->Get("J1pt_M_doubletagSR_TT_SL"); h_J2M_doubletagSR_TT_SL = (TH1F*)f->Get("J2pt_M_doubletagSR_TT_SL");
    // h_J1M_doubletagSB_TT_SL = (TH1F*)f->Get("J1pt_M_doubletagSB_TT_SL"); h_J2M_doubletagSB_TT_SL = (TH1F*)f->Get("J2pt_M_doubletagSB_TT_SL");
    // h_J1M_antitagSR_TT_SL = (TH1F*)f->Get("J1pt_M_antitagSR_TT_SL"); h_J2M_antitagSR_TT_SL = (TH1F*)f->Get("J2pt_M_antitagSR_TT_SL");
    // h_J1M_antitagSB_TT_SL = (TH1F*)f->Get("J1pt_M_antitagSB_TT_SL"); h_J2M_antitagSB_TT_SL = (TH1F*)f->Get("J2pt_M_antitagSB_TT_SL");
    // h_J1M_tagSR_TT_SL = (TH1F*)f->Get("J1pt_M_tagSR_TT_SL"); h_J2M_tagSR_TT_SL = (TH1F*)f->Get("J2pt_M_tagSR_TT_SL");
    // h_J1M_tagSB_TT_SL = (TH1F*)f->Get("J1pt_M_tagSB_TT_SL"); h_J2M_tagSB_TT_SL = (TH1F*)f->Get("J2pt_M_tagSB_TT_SL");
    // h_J1M_doubletagSR_TT_Di = (TH1F*)f->Get("J1pt_M_doubletagSR_TT_Di"); h_J2M_doubletagSR_TT_Di = (TH1F*)f->Get("J2pt_M_doubletagSR_TT_Di");
    // h_J1M_doubletagSB_TT_Di = (TH1F*)f->Get("J1pt_M_doubletagSB_TT_Di"); h_J2M_doubletagSB_TT_Di = (TH1F*)f->Get("J2pt_M_doubletagSB_TT_Di");
    // h_J1M_antitagSR_TT_Di = (TH1F*)f->Get("J1pt_M_antitagSR_TT_Di"); h_J2M_antitagSR_TT_Di = (TH1F*)f->Get("J2pt_M_antitagSR_TT_Di");
    // h_J1M_antitagSB_TT_Di = (TH1F*)f->Get("J1pt_M_antitagSB_TT_Di"); h_J2M_antitagSB_TT_Di = (TH1F*)f->Get("J2pt_M_antitagSB_TT_Di");
    // h_J1M_tagSR_TT_Di = (TH1F*)f->Get("J1pt_M_tagSR_TT_Di"); h_J2M_tagSR_TT_Di = (TH1F*)f->Get("J2pt_M_tagSR_TT_Di");
    // h_J1M_tagSB_TT_Di = (TH1F*)f->Get("J1pt_M_tagSB_TT_Di"); h_J2M_tagSB_TT_Di = (TH1F*)f->Get("J2pt_M_tagSB_TT_Di");


    h_J1M_doubletagSR_WJets = (TH1F*)f->Get("J1pt_M_doubletagSR_WJets"); h_J2M_doubletagSR_WJets = (TH1F*)f->Get("J2pt_M_doubletagSR_WJets");
    h_J1M_doubletagSB_WJets = (TH1F*)f->Get("J1pt_M_doubletagSB_WJets"); h_J2M_doubletagSB_WJets = (TH1F*)f->Get("J2pt_M_doubletagSB_WJets");
    h_J1M_antitagSR_WJets = (TH1F*)f->Get("J1pt_M_antitagSR_WJets"); h_J2M_antitagSR_WJets = (TH1F*)f->Get("J2pt_M_antitagSR_WJets");
    h_J1M_antitagSB_WJets = (TH1F*)f->Get("J1pt_M_antitagSB_WJets"); h_J2M_antitagSB_WJets = (TH1F*)f->Get("J2pt_M_antitagSB_WJets");
    h_J1M_tagSR_WJets = (TH1F*)f->Get("J1pt_M_tagSR_WJets"); h_J2M_tagSR_WJets = (TH1F*)f->Get("J2pt_M_tagSR_WJets");
    h_J1M_tagSB_WJets = (TH1F*)f->Get("J1pt_M_tagSB_WJets"); h_J2M_tagSB_WJets = (TH1F*)f->Get("J2pt_M_tagSB_WJets");
    h_baseline_j1mass_WJets = (TH1F*)f->Get("J1pt_M_baseline_WJets"); h_baseline_j2mass_WJets = (TH1F*)f->Get("J2pt_M_baseline_WJets");
    h_0H_j1mass_WJets = (TH1F*)h_J1M_antitagSR_WJets->Clone("h_0H_j1mass_WJets");  h_0H_j1mass_WJets->Add(h_J1M_antitagSB_WJets);
    h_0H_j2mass_WJets = (TH1F*)h_J2M_antitagSR_WJets->Clone("h_0H_j2mass_WJets");  h_0H_j2mass_WJets->Add(h_J2M_antitagSB_WJets);
    h_H_j1mass_WJets = (TH1F*)h_baseline_j1mass_WJets->Clone("h_H_j1mass_WJets");  h_H_j1mass_WJets->Add(h_0H_j1mass_WJets,-1);
    h_H_j2mass_WJets = (TH1F*)h_baseline_j2mass_WJets->Clone("h_H_j2mass_WJets");  h_H_j2mass_WJets->Add(h_0H_j2mass_WJets,-1);
    h_1H_j1mass_WJets = (TH1F*)h_J1M_tagSR_WJets->Clone("h_1H_j1mass_WJets");  h_1H_j1mass_WJets->Add(h_J1M_tagSB_WJets);
    h_1H_j2mass_WJets = (TH1F*)h_J2M_tagSR_WJets->Clone("h_1H_j2mass_WJets");  h_1H_j2mass_WJets->Add(h_J2M_tagSB_WJets);
    h_2H_j1mass_WJets = (TH1F*)h_J1M_doubletagSR_WJets->Clone("h_2H_j1mass_WJets");  h_2H_j1mass_WJets->Add(h_J1M_doubletagSB_WJets);
    h_2H_j2mass_WJets = (TH1F*)h_J2M_doubletagSR_WJets->Clone("h_2H_j2mass_WJets");  h_2H_j2mass_WJets->Add(h_J2M_doubletagSB_WJets);

    h_baseline_jmass_WJets = (TH1F*)h_baseline_j1mass_WJets->Clone("h_baseline_jmass_WJets");h_baseline_jmass_WJets->Add(h_baseline_j2mass_WJets);
    h_0H_jmass_WJets = (TH1F*)h_0H_j1mass_WJets->Clone("h_0H_jmass_WJets");h_0H_jmass_WJets->Add(h_0H_j2mass_WJets);
    h_1H_jmass_WJets = (TH1F*)h_1H_j1mass_WJets->Clone("h_1H_jmass_WJets");h_1H_jmass_WJets->Add(h_1H_j2mass_WJets);
    h_2H_jmass_WJets = (TH1F*)h_2H_j1mass_WJets->Clone("h_2H_jmass_WJets");h_2H_jmass_WJets->Add(h_2H_j2mass_WJets);

    std::cout<<"Here? 10"<<std::endl;


    h_baseline_j1mass_sum = (TH1F*)f->Get("J1pt_M_baseline_sum"); h_baseline_j2mass_sum = (TH1F*)f->Get("J2pt_M_baseline_sum");
    h_0H_j1mass_sum = (TH1F*)h_0H_j1mass_WJets->Clone("h_0H_j1mass_sum"); h_0H_j1mass_sum->Add(h_0H_j1mass_ZJets);
    h_0H_j1mass_sum->Add(h_0H_j1mass_TT); h_0H_j1mass_sum->Add(h_0H_j1mass_QCD); h_0H_j1mass_sum->Add(h_0H_j1mass_SnglT);
    h_0H_j2mass_sum = (TH1F*)h_0H_j2mass_WJets->Clone("h_0H_j2mass_sum"); h_0H_j2mass_sum->Add(h_0H_j2mass_ZJets);
    h_0H_j2mass_sum->Add(h_0H_j2mass_TT); h_0H_j2mass_sum->Add(h_0H_j2mass_QCD); h_0H_j2mass_sum->Add(h_0H_j2mass_SnglT);
    h_H_j1mass_sum = (TH1F*)h_baseline_j1mass_sum->Clone("h_H_j1mass_sum");  h_H_j1mass_sum->Add(h_0H_j1mass_sum,-1);
    h_H_j2mass_sum = (TH1F*)h_baseline_j2mass_sum->Clone("h_H_j2mass_sum");  h_H_j2mass_sum->Add(h_0H_j2mass_sum,-1);
    h_1H_j1mass_sum = (TH1F*)h_J1M_tagSR_sum->Clone("h_1H_j1mass_sum");  h_1H_j1mass_sum->Add(h_J1M_tagSB_sum);
    h_1H_j2mass_sum = (TH1F*)h_J2M_tagSR_sum->Clone("h_1H_j2mass_sum");  h_1H_j2mass_sum->Add(h_J2M_tagSB_sum);
    h_2H_j1mass_sum = (TH1F*)h_J1M_doubletagSR_sum->Clone("h_2H_j1mass_sum");  h_2H_j1mass_sum->Add(h_J1M_doubletagSB_sum);
    h_2H_j2mass_sum = (TH1F*)h_J2M_doubletagSR_sum->Clone("h_2H_j2mass_sum");  h_2H_j2mass_sum->Add(h_J2M_doubletagSB_sum);

    h_baseline_jmass_sum = (TH1F*)h_baseline_j1mass_sum->Clone("h_baseline_jmass_sum");h_baseline_jmass_sum->Add(h_baseline_j2mass_sum);
    h_0H_jmass_sum = (TH1F*)h_0H_j1mass_sum->Clone("h_0H_jmass_sum");h_0H_jmass_sum->Add(h_0H_j2mass_sum);
    h_1H_jmass_sum = (TH1F*)h_1H_j1mass_sum->Clone("h_1H_jmass_sum");h_1H_jmass_sum->Add(h_1H_j2mass_sum);
    h_2H_jmass_sum = (TH1F*)h_2H_j1mass_sum->Clone("h_2H_jmass_sum");h_2H_jmass_sum->Add(h_2H_j2mass_sum);


    // h_baseline_j1mass_data = (TH1F*)f->Get("J1pt_M_baseline_data"); h_baseline_j2mass_data = (TH1F*)f->Get("J2pt_M_baseline_data");
    // h_J1M_doubletagSR_data = (TH1F*)f->Get("J1pt_M_doubletagSR_data");  h_J2M_doubletagSR_data = (TH1F*)f->Get("J2pt_M_doubletagSR_data");
    // h_J1M_tagSR_data = (TH1F*)f->Get("J1pt_M_tagSR_data");  h_J2M_tagSR_data = (TH1F*)f->Get("J2pt_M_tagSR_data");
    // h_0H_j1mass_data = (TH1F*)f->Get("J1pt_M_antitagSR_data"); h_0H_j1mass_data->Add( (TH1F*)f->Get("J1pt_M_antitagSB_data") );
    // h_0H_j2mass_data = (TH1F*)f->Get("J2pt_M_antitagSR_data"); h_0H_j2mass_data->Add( (TH1F*)f->Get("J2pt_M_antitagSB_data") );
    // h_H_j1mass_data = (TH1F*)h_baseline_j1mass_data->Clone("h_H_j1mass_data");  h_H_j1mass_data->Add(h_0H_j1mass_data,-1);
    // h_H_j2mass_data = (TH1F*)h_baseline_j2mass_data->Clone("h_H_j2mass_data");  h_H_j2mass_data->Add(h_0H_j2mass_data,-1);

    h_baseline_j1mass_data = (TH1F*)fVetoData->Get("J1pt_M_baseline_data"); h_baseline_j2mass_data = (TH1F*)fVetoData->Get("J2pt_M_baseline_data");
    h_J1M_doubletagSR_data = (TH1F*)fVetoData->Get("J1pt_M_doubletagSR_data");  h_J2M_doubletagSR_data = (TH1F*)fVetoData->Get("J2pt_M_doubletagSR_data");
    h_J1M_doubletagSB_data = (TH1F*)fVetoData->Get("J1pt_M_doubletagSB_data");  h_J2M_doubletagSB_data = (TH1F*)fVetoData->Get("J2pt_M_doubletagSB_data");
    h_J1M_tagSR_data = (TH1F*)fVetoData->Get("J1pt_M_tagSR_data");  h_J2M_tagSR_data = (TH1F*)fVetoData->Get("J2pt_M_tagSR_data");
    h_J1M_tagSB_data = (TH1F*)fVetoData->Get("J1pt_M_tagSB_data");  h_J2M_tagSB_data = (TH1F*)fVetoData->Get("J2pt_M_tagSB_data");
    h_J1M_antitagSR_data = (TH1F*)fVetoData->Get("J1pt_M_antitagSR_data");  h_J2M_antitagSR_data = (TH1F*)fVetoData->Get("J2pt_M_tantiagSR_data");
    h_J1M_antitagSB_data = (TH1F*)fVetoData->Get("J1pt_M_antitagSB_data");  h_J2M_antitagSB_data = (TH1F*)fVetoData->Get("J2pt_M_antitagSB_data");
    h_0H_j1mass_data = (TH1F*)fVetoData->Get("J1pt_M_antitagSR_data"); h_0H_j1mass_data->Add( (TH1F*)fVetoData->Get("J1pt_M_antitagSB_data") );
    h_0H_j2mass_data = (TH1F*)fVetoData->Get("J2pt_M_antitagSR_data"); h_0H_j2mass_data->Add( (TH1F*)fVetoData->Get("J2pt_M_antitagSB_data") );
    h_H_j1mass_data = (TH1F*)h_baseline_j1mass_data->Clone("h_H_j1mass_data");  h_H_j1mass_data->Add(h_0H_j1mass_data,-1);
    h_H_j2mass_data = (TH1F*)h_baseline_j2mass_data->Clone("h_H_j2mass_data");  h_H_j2mass_data->Add(h_0H_j2mass_data,-1);
    h_1H_j1mass_data = (TH1F*)h_J1M_tagSR_data->Clone("h_1H_j1mass_data");  h_1H_j1mass_data->Add(h_J1M_tagSB_data);
    h_1H_j2mass_data = (TH1F*)h_J2M_tagSR_data->Clone("h_1H_j2mass_data");  h_1H_j2mass_data->Add(h_J2M_tagSB_data);
    h_2H_j1mass_data = (TH1F*)h_J1M_doubletagSR_data->Clone("h_2H_j1mass_data");  h_2H_j1mass_data->Add(h_J1M_doubletagSB_data);
    h_2H_j2mass_data = (TH1F*)h_J2M_doubletagSR_data->Clone("h_2H_j2mass_data");  h_2H_j2mass_data->Add(h_J2M_doubletagSB_data);

    h_baseline_jmass_data = (TH1F*)h_baseline_j1mass_data->Clone("h_baseline_jmass_data");h_baseline_jmass_data->Add(h_baseline_j2mass_data);
    h_0H_jmass_data = (TH1F*)h_0H_j1mass_data->Clone("h_0H_jmass_data");h_0H_jmass_data->Add(h_0H_j2mass_data);
    h_1H_jmass_data = (TH1F*)h_1H_j1mass_data->Clone("h_1H_jmass_data");h_1H_jmass_data->Add(h_1H_j2mass_data);
    h_2H_jmass_data = (TH1F*)h_2H_j1mass_data->Clone("h_2H_jmass_data");h_2H_jmass_data->Add(h_2H_j2mass_data);


    // h_J2M_mjBins_doubletagSR_sum = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_sum"); h_J2M_mjBins_doubletagSB_sum = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_sum");
    // h_J2M_mjBins_antitagSR_sum = (TH1F*)f->Get("J2_M_jetBins_antitagSR_sum"); h_J2M_mjBins_antitagSB_sum = (TH1F*)f->Get("J2_M_jetBins_antitagSB_sum");
    h_J2M_mjBins_doubletagSR_QCD = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_QCD"); h_J2M_mjBins_doubletagSB_QCD = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_QCD");
    h_J2M_mjBins_antitagSR_QCD = (TH1F*)f->Get("J2_M_jetBins_antitagSR_QCD"); h_J2M_mjBins_antitagSB_QCD = (TH1F*)f->Get("J2_M_jetBins_antitagSB_QCD");
    h_J2M_mjBins_doubletagSR_SnglT = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_SnglT"); h_J2M_mjBins_doubletagSB_SnglT = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_SnglT");
    h_J2M_mjBins_antitagSR_SnglT = (TH1F*)f->Get("J2_M_jetBins_antitagSR_SnglT"); h_J2M_mjBins_antitagSB_SnglT = (TH1F*)f->Get("J2_M_jetBins_antitagSB_SnglT");

    h_J2M_mjBins_doubletagSR_TT = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_TT"); h_J2M_mjBins_doubletagSB_TT = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_TT");
    h_J2M_mjBins_antitagSR_TT = (TH1F*)f->Get("J2_M_jetBins_antitagSR_TT"); h_J2M_mjBins_antitagSB_TT = (TH1F*)f->Get("J2_M_jetBins_antitagSB_TT");
    // h_J2M_mjBins_doubletagSR_TT_Di = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_TT_Di"); h_J2M_mjBins_doubletagSB_TT_Di = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_TT_Di");
    // h_J2M_mjBins_antitagSR_TT_Di = (TH1F*)f->Get("J2_M_jetBins_antitagSR_TT_Di"); h_J2M_mjBins_antitagSB_TT_Di = (TH1F*)f->Get("J2_M_jetBins_antitagSB_TT_Di");
    // h_J2M_mjBins_doubletagSR_TT_SL = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_TT_SL"); h_J2M_mjBins_doubletagSB_TT_SL = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_TT_SL");
    // h_J2M_mjBins_antitagSR_TT_SL = (TH1F*)f->Get("J2_M_jetBins_antitagSR_TT_SL"); h_J2M_mjBins_antitagSB_TT_SL = (TH1F*)f->Get("J2_M_jetBins_antitagSB_TT_SL");
    h_J2M_mjBins_doubletagSR_WJets = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_WJets"); h_J2M_mjBins_doubletagSB_WJets = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_WJets");
    h_J2M_mjBins_antitagSR_WJets = (TH1F*)f->Get("J2_M_jetBins_antitagSR_WJets"); h_J2M_mjBins_antitagSB_WJets = (TH1F*)f->Get("J2_M_jetBins_antitagSB_WJets");

    h_J2M_mjBins_doubletagSR_ZJets = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_ZJets"); h_J2M_mjBins_doubletagSB_ZJets = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_ZJets");
    h_J2M_mjBins_antitagSR_ZJets = (TH1F*)f->Get("J2_M_jetBins_antitagSR_ZJets"); h_J2M_mjBins_antitagSB_ZJets = (TH1F*)f->Get("J2_M_jetBins_antitagSB_ZJets");

    std::cout<<"Here? 11"<<std::endl;

    //for signal
    if (runSignal){
      std::cout<<"Here? 12"<<std::endl;

      h_J1M_doubletagSR_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_doubletagSR_T5HH1600_LSP1"); h_J2M_doubletagSR_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_doubletagSR_T5HH1600_LSP1");
      h_J1M_doubletagSB_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_doubletagSB_T5HH1600_LSP1"); h_J2M_doubletagSB_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_doubletagSB_T5HH1600_LSP1");
      h_J1M_antitagSR_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_antitagSR_T5HH1600_LSP1"); h_J2M_antitagSR_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_antitagSR_T5HH1600_LSP1");
      h_J1M_antitagSB_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_antitagSB_T5HH1600_LSP1"); h_J2M_antitagSB_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_antitagSB_T5HH1600_LSP1");
      h_J1M_tagSR_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_tagSR_T5HH1600_LSP1"); h_J2M_tagSR_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_tagSR_T5HH1600_LSP1");
      h_J1M_tagSB_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_tagSB_T5HH1600_LSP1"); h_J2M_tagSB_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_tagSB_T5HH1600_LSP1");
      h_baseline_j1mass_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_M_baseline_T5HH1600_LSP1"); h_baseline_j2mass_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_M_baseline_T5HH1600_LSP1");
      h_H_j1mass_T5HH1600 = (TH1F*)h_baseline_j1mass_T5HH1600->Clone("h_H_j1mass_T5HH1600"); h_H_j1mass_T5HH1600->Add(h_J1M_antitagSR_T5HH1600,-1); h_H_j1mass_T5HH1600->Add(h_J1M_antitagSB_T5HH1600,-1);
      h_H_j2mass_T5HH1600 = (TH1F*)h_baseline_j2mass_T5HH1600->Clone("h_H_j2mass_T5HH1600"); h_H_j2mass_T5HH1600->Add(h_J2M_antitagSR_T5HH1600,-1); h_H_j2mass_T5HH1600->Add(h_J2M_antitagSB_T5HH1600,-1);
      h_0H_j1mass_T5HH1600 = (TH1F*)h_J1M_antitagSR_T5HH1600->Clone("h_0H_j1mass_T5HH1600");  h_0H_j1mass_T5HH1600->Add(h_J1M_antitagSB_T5HH1600);
      h_0H_j2mass_T5HH1600 = (TH1F*)h_J2M_antitagSR_T5HH1600->Clone("h_0H_j2mass_T5HH1600");  h_0H_j2mass_T5HH1600->Add(h_J2M_antitagSB_T5HH1600);
      h_1H_j1mass_T5HH1600 = (TH1F*)h_J1M_tagSR_T5HH1600->Clone("h_1H_j1mass_T5HH1600");  h_1H_j1mass_T5HH1600->Add(h_J1M_tagSB_T5HH1600);
      h_1H_j2mass_T5HH1600 = (TH1F*)h_J2M_tagSR_T5HH1600->Clone("h_1H_j2mass_T5HH1600");  h_1H_j2mass_T5HH1600->Add(h_J2M_tagSB_T5HH1600);
      h_2H_j1mass_T5HH1600 = (TH1F*)h_J1M_doubletagSR_T5HH1600->Clone("h_2H_j1mass_T5HH1600");  h_2H_j1mass_T5HH1600->Add(h_J1M_doubletagSB_T5HH1600);
      h_2H_j2mass_T5HH1600 = (TH1F*)h_J2M_doubletagSR_T5HH1600->Clone("h_2H_j2mass_T5HH1600");  h_2H_j2mass_T5HH1600->Add(h_J2M_doubletagSB_T5HH1600);

      h_baseline_jmass_T5HH1600 = (TH1F*)h_baseline_j1mass_T5HH1600->Clone("h_baseline_jmass_T5HH1600");h_baseline_jmass_T5HH1600->Add(h_baseline_j2mass_T5HH1600);
      h_0H_jmass_T5HH1600 = (TH1F*)h_0H_j1mass_T5HH1600->Clone("h_0H_jmass_T5HH1600");h_0H_jmass_T5HH1600->Add(h_0H_j2mass_T5HH1600);
      h_1H_jmass_T5HH1600 = (TH1F*)h_1H_j1mass_T5HH1600->Clone("h_1H_jmass_T5HH1600");h_1H_jmass_T5HH1600->Add(h_1H_j2mass_T5HH1600);
      h_2H_jmass_T5HH1600 = (TH1F*)h_2H_j1mass_T5HH1600->Clone("h_2H_jmass_T5HH1600");h_2H_jmass_T5HH1600->Add(h_2H_j2mass_T5HH1600);


      h_J1M_doubletagSR_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_doubletagSR_T5HH2000_LSP1"); h_J2M_doubletagSR_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_doubletagSR_T5HH2000_LSP1");
      h_J1M_doubletagSB_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_doubletagSB_T5HH2000_LSP1"); h_J2M_doubletagSB_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_doubletagSB_T5HH2000_LSP1");
      h_J1M_antitagSR_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_antitagSR_T5HH2000_LSP1"); h_J2M_antitagSR_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_antitagSR_T5HH2000_LSP1");
      h_J1M_antitagSB_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_antitagSB_T5HH2000_LSP1"); h_J2M_antitagSB_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_antitagSB_T5HH2000_LSP1");
      h_J1M_tagSR_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_tagSR_T5HH2000_LSP1"); h_J2M_tagSR_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_tagSR_T5HH2000_LSP1");
      h_J1M_tagSB_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_tagSB_T5HH2000_LSP1"); h_J2M_tagSB_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_tagSB_T5HH2000_LSP1");
      h_baseline_j1mass_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_M_baseline_T5HH2000_LSP1"); h_baseline_j2mass_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_M_baseline_T5HH2000_LSP1");
      h_H_j1mass_T5HH2000 = (TH1F*)h_baseline_j1mass_T5HH2000->Clone("h_H_j1mass_T5HH2000"); h_H_j1mass_T5HH2000->Add(h_J1M_antitagSR_T5HH2000,-1); h_H_j1mass_T5HH2000->Add(h_J1M_antitagSB_T5HH2000,-1);
      h_H_j2mass_T5HH2000 = (TH1F*)h_baseline_j2mass_T5HH2000->Clone("h_H_j2mass_T5HH2000"); h_H_j2mass_T5HH2000->Add(h_J2M_antitagSR_T5HH2000,-1); h_H_j2mass_T5HH2000->Add(h_J2M_antitagSB_T5HH2000,-1);

      h_J1M_doubletagSR_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_doubletagSR_T5HH2200_LSP1"); h_J2M_doubletagSR_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_doubletagSR_T5HH2200_LSP1");
      h_J1M_doubletagSB_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_doubletagSB_T5HH2200_LSP1"); h_J2M_doubletagSB_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_doubletagSB_T5HH2200_LSP1");
      h_J1M_antitagSR_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_antitagSR_T5HH2200_LSP1"); h_J2M_antitagSR_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_antitagSR_T5HH2200_LSP1");
      h_J1M_antitagSB_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_antitagSB_T5HH2200_LSP1"); h_J2M_antitagSB_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_antitagSB_T5HH2200_LSP1");
      h_J1M_tagSR_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_tagSR_T5HH2200_LSP1"); h_J2M_tagSR_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_tagSR_T5HH2200_LSP1");
      h_J1M_tagSB_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_tagSB_T5HH2200_LSP1"); h_J2M_tagSB_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_tagSB_T5HH2200_LSP1");
      h_baseline_j1mass_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_M_baseline_T5HH2200_LSP1"); h_baseline_j2mass_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_M_baseline_T5HH2200_LSP1");
      h_H_j1mass_T5HH2200 = (TH1F*)h_baseline_j1mass_T5HH2200->Clone("h_H_j1mass_T5HH2200"); h_H_j1mass_T5HH2200->Add(h_J1M_antitagSR_T5HH2200,-1); h_H_j1mass_T5HH2200->Add(h_J1M_antitagSB_T5HH2200,-1);
      h_H_j2mass_T5HH2200 = (TH1F*)h_baseline_j2mass_T5HH2200->Clone("h_H_j2mass_T5HH2200"); h_H_j2mass_T5HH2200->Add(h_J2M_antitagSR_T5HH2200,-1); h_H_j2mass_T5HH2200->Add(h_J2M_antitagSB_T5HH2200,-1);

      std::cout<<"Here? 13"<<std::endl;


      h_J1M_doubletagSR_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_doubletagSR_TChiHH500_LSP1"); h_J2M_doubletagSR_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_doubletagSR_TChiHH500_LSP1");
      h_J1M_doubletagSB_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_doubletagSB_TChiHH500_LSP1"); h_J2M_doubletagSB_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_doubletagSB_TChiHH500_LSP1");
      h_J1M_antitagSR_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_antitagSR_TChiHH500_LSP1"); h_J2M_antitagSR_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_antitagSR_TChiHH500_LSP1");
      h_J1M_antitagSB_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_antitagSB_TChiHH500_LSP1"); h_J2M_antitagSB_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_antitagSB_TChiHH500_LSP1");
      h_J1M_tagSR_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_tagSR_TChiHH500_LSP1"); h_J2M_tagSR_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_tagSR_TChiHH500_LSP1");
      h_J1M_tagSB_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_tagSB_TChiHH500_LSP1"); h_J2M_tagSB_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_tagSB_TChiHH500_LSP1");
      h_baseline_j1mass_TChiHH500 = (TH1F*)fSignal->Get("J1pt_M_baseline_TChiHH500_LSP1"); h_baseline_j2mass_TChiHH500 = (TH1F*)fSignal->Get("J2pt_M_baseline_TChiHH500_LSP1");
      h_H_j1mass_TChiHH500 = (TH1F*)h_baseline_j1mass_TChiHH500->Clone("h_H_j1mass_TChiHH500"); h_H_j1mass_TChiHH500->Add(h_J1M_antitagSR_TChiHH500,-1); h_H_j1mass_TChiHH500->Add(h_J1M_antitagSB_TChiHH500,-1);
      h_H_j2mass_TChiHH500 = (TH1F*)h_baseline_j2mass_TChiHH500->Clone("h_H_j2mass_TChiHH500"); h_H_j2mass_TChiHH500->Add(h_J2M_antitagSR_TChiHH500,-1); h_H_j2mass_TChiHH500->Add(h_J2M_antitagSB_TChiHH500,-1);
      h_0H_j1mass_TChiHH500 = (TH1F*)h_J1M_antitagSR_TChiHH500->Clone("h_0H_j1mass_TChiHH500");  h_0H_j1mass_TChiHH500->Add(h_J1M_antitagSB_TChiHH500);
      h_0H_j2mass_TChiHH500 = (TH1F*)h_J2M_antitagSR_TChiHH500->Clone("h_0H_j2mass_TChiHH500");  h_0H_j2mass_TChiHH500->Add(h_J2M_antitagSB_TChiHH500);
      h_1H_j1mass_TChiHH500 = (TH1F*)h_J1M_tagSR_TChiHH500->Clone("h_1H_j1mass_TChiHH500");  h_1H_j1mass_TChiHH500->Add(h_J1M_tagSB_TChiHH500);
      h_1H_j2mass_TChiHH500 = (TH1F*)h_J2M_tagSR_TChiHH500->Clone("h_1H_j2mass_TChiHH500");  h_1H_j2mass_TChiHH500->Add(h_J2M_tagSB_TChiHH500);
      h_2H_j1mass_TChiHH500 = (TH1F*)h_J1M_doubletagSR_TChiHH500->Clone("h_2H_j1mass_TChiHH500");  h_2H_j1mass_TChiHH500->Add(h_J1M_doubletagSB_TChiHH500);
      h_2H_j2mass_TChiHH500 = (TH1F*)h_J2M_doubletagSR_TChiHH500->Clone("h_2H_j2mass_TChiHH500");  h_2H_j2mass_TChiHH500->Add(h_J2M_doubletagSB_TChiHH500);

      h_baseline_jmass_TChiHH500 = (TH1F*)h_baseline_j1mass_TChiHH500->Clone("h_baseline_jmass_TChiHH500");h_baseline_jmass_TChiHH500->Add(h_baseline_j2mass_TChiHH500);
      h_0H_jmass_TChiHH500 = (TH1F*)h_0H_j1mass_TChiHH500->Clone("h_0H_jmass_TChiHH500");h_0H_jmass_TChiHH500->Add(h_0H_j2mass_TChiHH500);
      h_1H_jmass_TChiHH500 = (TH1F*)h_1H_j1mass_TChiHH500->Clone("h_1H_jmass_TChiHH500");h_1H_jmass_TChiHH500->Add(h_1H_j2mass_TChiHH500);
      h_2H_jmass_TChiHH500 = (TH1F*)h_2H_j1mass_TChiHH500->Clone("h_2H_jmass_TChiHH500");h_2H_jmass_TChiHH500->Add(h_2H_j2mass_TChiHH500);


      h_J1M_doubletagSR_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_doubletagSR_TChiHH800_LSP1"); h_J2M_doubletagSR_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_doubletagSR_TChiHH800_LSP1");
      h_J1M_doubletagSB_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_doubletagSB_TChiHH800_LSP1"); h_J2M_doubletagSB_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_doubletagSB_TChiHH800_LSP1");
      h_J1M_antitagSR_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_antitagSR_TChiHH800_LSP1"); h_J2M_antitagSR_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_antitagSR_TChiHH800_LSP1");
      h_J1M_antitagSB_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_antitagSB_TChiHH800_LSP1"); h_J2M_antitagSB_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_antitagSB_TChiHH800_LSP1");
      h_J1M_tagSR_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_tagSR_TChiHH800_LSP1"); h_J2M_tagSR_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_tagSR_TChiHH800_LSP1");
      h_J1M_tagSB_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_tagSB_TChiHH800_LSP1"); h_J2M_tagSB_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_tagSB_TChiHH800_LSP1");
      h_baseline_j1mass_TChiHH800 = (TH1F*)fSignal->Get("J1pt_M_baseline_TChiHH800_LSP1"); h_baseline_j2mass_TChiHH800 = (TH1F*)fSignal->Get("J2pt_M_baseline_TChiHH800_LSP1");
      h_H_j1mass_TChiHH800 = (TH1F*)h_baseline_j1mass_TChiHH800->Clone("h_H_j1mass_TChiHH800"); h_H_j1mass_TChiHH800->Add(h_J1M_antitagSR_TChiHH800,-1); h_H_j1mass_TChiHH800->Add(h_J1M_antitagSB_TChiHH800,-1);
      h_H_j2mass_TChiHH800 = (TH1F*)h_baseline_j2mass_TChiHH800->Clone("h_H_j2mass_TChiHH800"); h_H_j2mass_TChiHH800->Add(h_J2M_antitagSR_TChiHH800,-1); h_H_j2mass_TChiHH800->Add(h_J2M_antitagSB_TChiHH800,-1);

      h_J1M_doubletagSR_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_doubletagSR_TChiHH1000_LSP1"); h_J2M_doubletagSR_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_doubletagSR_TChiHH1000_LSP1");
      h_J1M_doubletagSB_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_doubletagSB_TChiHH1000_LSP1"); h_J2M_doubletagSB_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_doubletagSB_TChiHH1000_LSP1");
      h_J1M_antitagSR_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_antitagSR_TChiHH1000_LSP1"); h_J2M_antitagSR_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_antitagSR_TChiHH1000_LSP1");
      h_J1M_antitagSB_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_antitagSB_TChiHH1000_LSP1"); h_J2M_antitagSB_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_antitagSB_TChiHH1000_LSP1");
      h_J1M_tagSR_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_tagSR_TChiHH1000_LSP1"); h_J2M_tagSR_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_tagSR_TChiHH1000_LSP1");
      h_J1M_tagSB_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_tagSB_TChiHH1000_LSP1"); h_J2M_tagSB_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_tagSB_TChiHH1000_LSP1");
      h_baseline_j1mass_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_M_baseline_TChiHH1000_LSP1"); h_baseline_j2mass_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_M_baseline_TChiHH1000_LSP1");
      h_H_j1mass_TChiHH1000 = (TH1F*)h_baseline_j1mass_TChiHH1000->Clone("h_H_j1mass_TChiHH1000"); h_H_j1mass_TChiHH1000->Add(h_J1M_antitagSR_TChiHH1000,-1); h_H_j1mass_TChiHH1000->Add(h_J1M_antitagSB_TChiHH1000,-1);
      h_H_j2mass_TChiHH1000 = (TH1F*)h_baseline_j2mass_TChiHH1000->Clone("h_H_j2mass_TChiHH1000"); h_H_j2mass_TChiHH1000->Add(h_J2M_antitagSR_TChiHH1000,-1); h_H_j2mass_TChiHH1000->Add(h_J2M_antitagSB_TChiHH1000,-1);
      std::cout<<"Here? 14"<<std::endl;

    }
    //For pt
    h_J1Pt_doubletagSR_sum = (TH1F*)f->Get("J1pt_Pt_doubletagSR_sum"); h_J2Pt_doubletagSR_sum = (TH1F*)f->Get("J2pt_Pt_doubletagSR_sum");
    h_J1Pt_doubletagSB_sum = (TH1F*)f->Get("J1pt_Pt_doubletagSB_sum"); h_J2Pt_doubletagSB_sum = (TH1F*)f->Get("J2pt_Pt_doubletagSB_sum");
    h_J1Pt_antitagSR_sum = (TH1F*)f->Get("J1pt_Pt_antitagSR_sum"); h_J2Pt_antitagSR_sum = (TH1F*)f->Get("J2pt_Pt_antitagSR_sum");
    h_J1Pt_antitagSB_sum = (TH1F*)f->Get("J1pt_Pt_antitagSB_sum"); h_J2Pt_antitagSB_sum = (TH1F*)f->Get("J2pt_Pt_antitagSB_sum");
    h_J1Pt_tagSR_sum = (TH1F*)f->Get("J1pt_Pt_tagSR_sum"); h_J2Pt_tagSR_sum = (TH1F*)f->Get("J2pt_Pt_tagSR_sum");
    h_J1Pt_tagSB_sum = (TH1F*)f->Get("J1pt_Pt_tagSB_sum"); h_J2Pt_tagSB_sum = (TH1F*)f->Get("J2pt_Pt_tagSB_sum");

    h_J1Pt_doubletagSR_ZJets = (TH1F*)f->Get("J1pt_Pt_doubletagSR_ZJets"); h_J2Pt_doubletagSR_ZJets = (TH1F*)f->Get("J2pt_Pt_doubletagSR_ZJets");
    h_J1Pt_doubletagSB_ZJets = (TH1F*)f->Get("J1pt_Pt_doubletagSB_ZJets"); h_J2Pt_doubletagSB_ZJets = (TH1F*)f->Get("J2pt_Pt_doubletagSB_ZJets");
    h_J1Pt_antitagSR_ZJets = (TH1F*)f->Get("J1pt_Pt_antitagSR_ZJets"); h_J2Pt_antitagSR_ZJets = (TH1F*)f->Get("J2pt_Pt_antitagSR_ZJets");
    h_J1Pt_antitagSB_ZJets = (TH1F*)f->Get("J1pt_Pt_antitagSB_ZJets"); h_J2Pt_antitagSB_ZJets = (TH1F*)f->Get("J2pt_Pt_antitagSB_ZJets");
    h_J1Pt_tagSR_ZJets = (TH1F*)f->Get("J1pt_Pt_tagSR_ZJets"); h_J2Pt_tagSR_ZJets = (TH1F*)f->Get("J2pt_Pt_tagSR_ZJets");
    h_J1Pt_tagSB_ZJets = (TH1F*)f->Get("J1pt_Pt_tagSB_ZJets"); h_J2Pt_tagSB_ZJets = (TH1F*)f->Get("J2pt_Pt_tagSB_ZJets");
    h_baseline_j1pt_ZJets = (TH1F*)f->Get("J1pt_Pt_baseline_ZJets"); h_baseline_j2pt_ZJets = (TH1F*)f->Get("J2pt_Pt_baseline_ZJets");
    h_0H_j1pt_ZJets = (TH1F*)h_J1Pt_antitagSR_ZJets->Clone("h_0H_j1pt_ZJets"); h_0H_j1pt_ZJets->Add(h_J1Pt_antitagSB_ZJets);
    h_0H_j2pt_ZJets = (TH1F*)h_J2Pt_antitagSR_ZJets->Clone("h_0H_j2pt_ZJets"); h_0H_j2pt_ZJets->Add(h_J2Pt_antitagSB_ZJets);
    h_H_j1pt_ZJets = (TH1F*)h_baseline_j1pt_ZJets->Clone("h_H_j1pt_ZJets");  h_H_j1pt_ZJets->Add(h_0H_j1pt_ZJets,-1);
    h_H_j2pt_ZJets = (TH1F*)h_baseline_j2pt_ZJets->Clone("h_H_j2pt_ZJets");  h_H_j2pt_ZJets->Add(h_0H_j2pt_ZJets,-1);
    h_1H_j1pt_ZJets = (TH1F*)h_J1Pt_tagSR_ZJets->Clone("h_1H_j1pt_ZJets"); h_1H_j1pt_ZJets->Add(h_J1Pt_tagSB_ZJets);
    h_1H_j2pt_ZJets = (TH1F*)h_J2Pt_tagSR_ZJets->Clone("h_1H_j2pt_ZJets"); h_1H_j2pt_ZJets->Add(h_J2Pt_tagSB_ZJets);
    h_2H_j1pt_ZJets = (TH1F*)h_J1Pt_doubletagSR_ZJets->Clone("h_2H_j1pt_ZJets"); h_2H_j1pt_ZJets->Add(h_J1Pt_doubletagSB_ZJets);
    h_2H_j2pt_ZJets = (TH1F*)h_J2Pt_doubletagSR_ZJets->Clone("h_2H_j2pt_ZJets"); h_2H_j2pt_ZJets->Add(h_J2Pt_doubletagSB_ZJets);

    h_baseline_jpt_ZJets = (TH1F*)h_baseline_j1pt_ZJets->Clone("h_baseline_jpt_ZJets");h_baseline_jpt_ZJets->Add(h_baseline_j2pt_ZJets);
    h_0H_jpt_ZJets = (TH1F*)h_0H_j1pt_ZJets->Clone("h_0H_jpt_ZJets");h_0H_jpt_ZJets->Add(h_0H_j2pt_ZJets);
    h_1H_jpt_ZJets = (TH1F*)h_1H_j1pt_ZJets->Clone("h_1H_jpt_ZJets");h_1H_jpt_ZJets->Add(h_1H_j2pt_ZJets);
    h_2H_jpt_ZJets = (TH1F*)h_2H_j1pt_ZJets->Clone("h_2H_jpt_ZJets");h_2H_jpt_ZJets->Add(h_2H_j2pt_ZJets);


    h_J1Pt_doubletagSR_QCD = (TH1F*)f->Get("J1pt_Pt_doubletagSR_QCD"); h_J2Pt_doubletagSR_QCD = (TH1F*)f->Get("J2pt_Pt_doubletagSR_QCD");
    h_J1Pt_doubletagSB_QCD = (TH1F*)f->Get("J1pt_Pt_doubletagSB_QCD"); h_J2Pt_doubletagSB_QCD = (TH1F*)f->Get("J2pt_Pt_doubletagSB_QCD");
    h_J1Pt_antitagSR_QCD = (TH1F*)f->Get("J1pt_Pt_antitagSR_QCD"); h_J2Pt_antitagSR_QCD = (TH1F*)f->Get("J2pt_Pt_antitagSR_QCD");
    h_J1Pt_antitagSB_QCD = (TH1F*)f->Get("J1pt_Pt_antitagSB_QCD"); h_J2Pt_antitagSB_QCD = (TH1F*)f->Get("J2pt_Pt_antitagSB_QCD");
    h_J1Pt_tagSR_QCD = (TH1F*)f->Get("J1pt_Pt_tagSR_QCD"); h_J2Pt_tagSR_QCD = (TH1F*)f->Get("J2pt_Pt_tagSR_QCD");
    h_J1Pt_tagSB_QCD = (TH1F*)f->Get("J1pt_Pt_tagSB_QCD"); h_J2Pt_tagSB_QCD = (TH1F*)f->Get("J2pt_Pt_tagSB_QCD");
    h_baseline_j1pt_QCD = (TH1F*)f->Get("J1pt_Pt_baseline_QCD"); h_baseline_j2pt_QCD = (TH1F*)f->Get("J2pt_Pt_baseline_QCD");
    h_0H_j1pt_QCD = (TH1F*)h_J1Pt_antitagSR_QCD->Clone("h_0H_j1pt_QCD"); h_0H_j1pt_QCD->Add(h_J1Pt_antitagSB_QCD);
    h_0H_j2pt_QCD = (TH1F*)h_J2Pt_antitagSR_QCD->Clone("h_0H_j2pt_QCD"); h_0H_j2pt_QCD->Add(h_J2Pt_antitagSB_QCD);
    h_H_j1pt_QCD = (TH1F*)h_baseline_j1pt_QCD->Clone("h_H_j1pt_QCD");  h_H_j1pt_QCD->Add(h_0H_j1pt_QCD,-1);
    h_H_j2pt_QCD = (TH1F*)h_baseline_j2pt_QCD->Clone("h_H_j2pt_QCD");  h_H_j2pt_QCD->Add(h_0H_j2pt_QCD,-1);
    h_1H_j1pt_QCD = (TH1F*)h_J1Pt_tagSR_QCD->Clone("h_1H_j1pt_QCD"); h_1H_j1pt_QCD->Add(h_J1Pt_tagSB_QCD);
    h_1H_j2pt_QCD = (TH1F*)h_J2Pt_tagSR_QCD->Clone("h_1H_j2pt_QCD"); h_1H_j2pt_QCD->Add(h_J2Pt_tagSB_QCD);
    h_2H_j1pt_QCD = (TH1F*)h_J1Pt_doubletagSR_QCD->Clone("h_2H_j1pt_QCD"); h_2H_j1pt_QCD->Add(h_J1Pt_doubletagSB_QCD);
    h_2H_j2pt_QCD = (TH1F*)h_J2Pt_doubletagSR_QCD->Clone("h_2H_j2pt_QCD"); h_2H_j2pt_QCD->Add(h_J2Pt_doubletagSB_QCD);

    h_baseline_jpt_QCD = (TH1F*)h_baseline_j1pt_QCD->Clone("h_baseline_jpt_QCD");h_baseline_jpt_QCD->Add(h_baseline_j2pt_QCD);
    h_0H_jpt_QCD = (TH1F*)h_0H_j1pt_QCD->Clone("h_0H_jpt_QCD");h_0H_jpt_QCD->Add(h_0H_j2pt_QCD);
    h_1H_jpt_QCD = (TH1F*)h_1H_j1pt_QCD->Clone("h_1H_jpt_QCD");h_1H_jpt_QCD->Add(h_1H_j2pt_QCD);
    h_2H_jpt_QCD = (TH1F*)h_2H_j1pt_QCD->Clone("h_2H_jpt_QCD");h_2H_jpt_QCD->Add(h_2H_j2pt_QCD);


    std::cout<<"Here? 15"<<std::endl;


    h_J1Pt_doubletagSR_SnglT = (TH1F*)f->Get("J1pt_Pt_doubletagSR_SnglT"); h_J2Pt_doubletagSR_SnglT = (TH1F*)f->Get("J2pt_Pt_doubletagSR_SnglT");
    h_J1Pt_doubletagSB_SnglT = (TH1F*)f->Get("J1pt_Pt_doubletagSB_SnglT"); h_J2Pt_doubletagSB_SnglT = (TH1F*)f->Get("J2pt_Pt_doubletagSB_SnglT");
    h_J1Pt_antitagSR_SnglT = (TH1F*)f->Get("J1pt_Pt_antitagSR_SnglT"); h_J2Pt_antitagSR_SnglT = (TH1F*)f->Get("J2pt_Pt_antitagSR_SnglT");
    h_J1Pt_antitagSB_SnglT = (TH1F*)f->Get("J1pt_Pt_antitagSB_SnglT"); h_J2Pt_antitagSB_SnglT = (TH1F*)f->Get("J2pt_Pt_antitagSB_SnglT");
    h_J1Pt_tagSR_SnglT = (TH1F*)f->Get("J1pt_Pt_tagSR_SnglT"); h_J2Pt_tagSR_SnglT = (TH1F*)f->Get("J2pt_Pt_tagSR_SnglT");
    h_J1Pt_tagSB_SnglT = (TH1F*)f->Get("J1pt_Pt_tagSB_SnglT"); h_J2Pt_tagSB_SnglT = (TH1F*)f->Get("J2pt_Pt_tagSB_SnglT");
    h_baseline_j1pt_SnglT = (TH1F*)f->Get("J1pt_Pt_baseline_SnglT"); h_baseline_j2pt_SnglT = (TH1F*)f->Get("J2pt_Pt_baseline_SnglT");
    h_0H_j1pt_SnglT = (TH1F*)h_J1Pt_antitagSR_SnglT->Clone("h_0H_j1pt_SnglT"); h_0H_j1pt_SnglT->Add(h_J1Pt_antitagSB_SnglT);
    h_0H_j2pt_SnglT = (TH1F*)h_J2Pt_antitagSR_SnglT->Clone("h_0H_j2pt_SnglT"); h_0H_j2pt_SnglT->Add(h_J2Pt_antitagSB_SnglT);
    h_H_j1pt_SnglT = (TH1F*)h_baseline_j1pt_SnglT->Clone("h_H_j1pt_SnglT");  h_H_j1pt_SnglT->Add(h_0H_j1pt_SnglT,-1);
    h_H_j2pt_SnglT = (TH1F*)h_baseline_j2pt_SnglT->Clone("h_H_j2pt_SnglT");  h_H_j2pt_SnglT->Add(h_0H_j2pt_SnglT,-1);
    h_1H_j1pt_SnglT = (TH1F*)h_J1Pt_tagSR_SnglT->Clone("h_1H_j1pt_SnglT"); h_1H_j1pt_SnglT->Add(h_J1Pt_tagSB_SnglT);
    h_1H_j2pt_SnglT = (TH1F*)h_J2Pt_tagSR_SnglT->Clone("h_1H_j2pt_SnglT"); h_1H_j2pt_SnglT->Add(h_J2Pt_tagSB_SnglT);
    h_2H_j1pt_SnglT = (TH1F*)h_J1Pt_doubletagSR_SnglT->Clone("h_2H_j1pt_SnglT"); h_2H_j1pt_SnglT->Add(h_J1Pt_doubletagSB_SnglT);
    h_2H_j2pt_SnglT = (TH1F*)h_J2Pt_doubletagSR_SnglT->Clone("h_2H_j2pt_SnglT"); h_2H_j2pt_SnglT->Add(h_J2Pt_doubletagSB_SnglT);

    h_baseline_jpt_SnglT = (TH1F*)h_baseline_j1pt_SnglT->Clone("h_baseline_jpt_SnglT");h_baseline_jpt_SnglT->Add(h_baseline_j2pt_SnglT);
    h_0H_jpt_SnglT = (TH1F*)h_0H_j1pt_SnglT->Clone("h_0H_jpt_SnglT");h_0H_jpt_SnglT->Add(h_0H_j2pt_SnglT);
    h_1H_jpt_SnglT = (TH1F*)h_1H_j1pt_SnglT->Clone("h_1H_jpt_SnglT");h_1H_jpt_SnglT->Add(h_1H_j2pt_SnglT);
    h_2H_jpt_SnglT = (TH1F*)h_2H_j1pt_SnglT->Clone("h_2H_jpt_SnglT");h_2H_jpt_SnglT->Add(h_2H_j2pt_SnglT);


    h_J1Pt_doubletagSR_TT = (TH1F*)f->Get("J1pt_Pt_doubletagSR_TT"); h_J2Pt_doubletagSR_TT = (TH1F*)f->Get("J2pt_Pt_doubletagSR_TT");
    h_J1Pt_doubletagSB_TT = (TH1F*)f->Get("J1pt_Pt_doubletagSB_TT"); h_J2Pt_doubletagSB_TT = (TH1F*)f->Get("J2pt_Pt_doubletagSB_TT");
    h_J1Pt_antitagSR_TT = (TH1F*)f->Get("J1pt_Pt_antitagSR_TT"); h_J2Pt_antitagSR_TT = (TH1F*)f->Get("J2pt_Pt_antitagSR_TT");
    h_J1Pt_antitagSB_TT = (TH1F*)f->Get("J1pt_Pt_antitagSB_TT"); h_J2Pt_antitagSB_TT = (TH1F*)f->Get("J2pt_Pt_antitagSB_TT");
    h_J1Pt_tagSR_TT = (TH1F*)f->Get("J1pt_Pt_tagSR_TT"); h_J2Pt_tagSR_TT = (TH1F*)f->Get("J2pt_Pt_tagSR_TT");
    h_J1Pt_tagSB_TT = (TH1F*)f->Get("J1pt_Pt_tagSB_TT"); h_J2Pt_tagSB_TT = (TH1F*)f->Get("J2pt_Pt_tagSB_TT");
    h_baseline_j1pt_TT = (TH1F*)f->Get("J1pt_Pt_baseline_TT"); h_baseline_j2pt_TT = (TH1F*)f->Get("J2pt_Pt_baseline_TT");
    h_0H_j1pt_TT = (TH1F*)h_J1Pt_antitagSR_TT->Clone("h_0H_j1pt_TT"); h_0H_j1pt_TT->Add(h_J1Pt_antitagSB_TT);
    h_0H_j2pt_TT = (TH1F*)h_J2Pt_antitagSR_TT->Clone("h_0H_j2pt_TT"); h_0H_j2pt_TT->Add(h_J2Pt_antitagSB_TT);
    h_H_j1pt_TT = (TH1F*)h_baseline_j1pt_TT->Clone("h_H_j1pt_TT");  h_H_j1pt_TT->Add(h_0H_j1pt_TT,-1);
    h_H_j2pt_TT = (TH1F*)h_baseline_j2pt_TT->Clone("h_H_j2pt_TT");  h_H_j2pt_TT->Add(h_0H_j2pt_TT,-1);
    h_1H_j1pt_TT = (TH1F*)h_J1Pt_tagSR_TT->Clone("h_1H_j1pt_TT"); h_1H_j1pt_TT->Add(h_J1Pt_tagSB_TT);
    h_1H_j2pt_TT = (TH1F*)h_J2Pt_tagSR_TT->Clone("h_1H_j2pt_TT"); h_1H_j2pt_TT->Add(h_J2Pt_tagSB_TT);
    h_2H_j1pt_TT = (TH1F*)h_J1Pt_doubletagSR_TT->Clone("h_2H_j1pt_TT"); h_2H_j1pt_TT->Add(h_J1Pt_doubletagSB_TT);
    h_2H_j2pt_TT = (TH1F*)h_J2Pt_doubletagSR_TT->Clone("h_2H_j2pt_TT"); h_2H_j2pt_TT->Add(h_J2Pt_doubletagSB_TT);

    h_baseline_jpt_TT = (TH1F*)h_baseline_j1pt_TT->Clone("h_baseline_jpt_TT");h_baseline_jpt_TT->Add(h_baseline_j2pt_TT);
    h_0H_jpt_TT = (TH1F*)h_0H_j1pt_TT->Clone("h_0H_jpt_TT");h_0H_jpt_TT->Add(h_0H_j2pt_TT);
    h_1H_jpt_TT = (TH1F*)h_1H_j1pt_TT->Clone("h_1H_jpt_TT");h_1H_jpt_TT->Add(h_1H_j2pt_TT);
    h_2H_jpt_TT = (TH1F*)h_2H_j1pt_TT->Clone("h_2H_jpt_TT");h_2H_jpt_TT->Add(h_2H_j2pt_TT);


    h_J1Pt_doubletagSR_WJets = (TH1F*)f->Get("J1pt_Pt_doubletagSR_WJets"); h_J2Pt_doubletagSR_WJets = (TH1F*)f->Get("J2pt_Pt_doubletagSR_WJets");
    h_J1Pt_doubletagSB_WJets = (TH1F*)f->Get("J1pt_Pt_doubletagSB_WJets"); h_J2Pt_doubletagSB_WJets = (TH1F*)f->Get("J2pt_Pt_doubletagSB_WJets");
    h_J1Pt_antitagSR_WJets = (TH1F*)f->Get("J1pt_Pt_antitagSR_WJets"); h_J2Pt_antitagSR_WJets = (TH1F*)f->Get("J2pt_Pt_antitagSR_WJets");
    h_J1Pt_antitagSB_WJets = (TH1F*)f->Get("J1pt_Pt_antitagSB_WJets"); h_J2Pt_antitagSB_WJets = (TH1F*)f->Get("J2pt_Pt_antitagSB_WJets");
    h_J1Pt_tagSR_WJets = (TH1F*)f->Get("J1pt_Pt_tagSR_WJets"); h_J2Pt_tagSR_WJets = (TH1F*)f->Get("J2pt_Pt_tagSR_WJets");
    h_J1Pt_tagSB_WJets = (TH1F*)f->Get("J1pt_Pt_tagSB_WJets"); h_J2Pt_tagSB_WJets = (TH1F*)f->Get("J2pt_Pt_tagSB_WJets");
    h_baseline_j1pt_WJets = (TH1F*)f->Get("J1pt_Pt_baseline_WJets"); h_baseline_j2pt_WJets = (TH1F*)f->Get("J2pt_Pt_baseline_WJets");
    h_0H_j1pt_WJets = (TH1F*)h_J1Pt_antitagSR_WJets->Clone("h_0H_j1pt_WJets"); h_0H_j1pt_WJets->Add(h_J1Pt_antitagSB_WJets);
    h_0H_j2pt_WJets = (TH1F*)h_J2Pt_antitagSR_WJets->Clone("h_0H_j2pt_WJets"); h_0H_j2pt_WJets->Add(h_J2Pt_antitagSB_WJets);
    h_H_j1pt_WJets = (TH1F*)h_baseline_j1pt_WJets->Clone("h_H_j1pt_WJets");  h_H_j1pt_WJets->Add(h_0H_j1pt_WJets,-1);
    h_H_j2pt_WJets = (TH1F*)h_baseline_j2pt_WJets->Clone("h_H_j2pt_WJets");  h_H_j2pt_WJets->Add(h_0H_j2pt_WJets,-1);
    h_1H_j1pt_WJets = (TH1F*)h_J1Pt_tagSR_WJets->Clone("h_1H_j1pt_WJets"); h_1H_j1pt_WJets->Add(h_J1Pt_tagSB_WJets);
    h_1H_j2pt_WJets = (TH1F*)h_J2Pt_tagSR_WJets->Clone("h_1H_j2pt_WJets"); h_1H_j2pt_WJets->Add(h_J2Pt_tagSB_WJets);
    h_2H_j1pt_WJets = (TH1F*)h_J1Pt_doubletagSR_WJets->Clone("h_2H_j1pt_WJets"); h_2H_j1pt_WJets->Add(h_J1Pt_doubletagSB_WJets);
    h_2H_j2pt_WJets = (TH1F*)h_J2Pt_doubletagSR_WJets->Clone("h_2H_j2pt_WJets"); h_2H_j2pt_WJets->Add(h_J2Pt_doubletagSB_WJets);

    h_baseline_jpt_WJets = (TH1F*)h_baseline_j1pt_WJets->Clone("h_baseline_jpt_WJets");h_baseline_jpt_WJets->Add(h_baseline_j2pt_WJets);
    h_0H_jpt_WJets = (TH1F*)h_0H_j1pt_WJets->Clone("h_0H_jpt_WJets");h_0H_jpt_WJets->Add(h_0H_j2pt_WJets);
    h_1H_jpt_WJets = (TH1F*)h_1H_j1pt_WJets->Clone("h_1H_jpt_WJets");h_1H_jpt_WJets->Add(h_1H_j2pt_WJets);
    h_2H_jpt_WJets = (TH1F*)h_2H_j1pt_WJets->Clone("h_2H_jpt_WJets");h_2H_jpt_WJets->Add(h_2H_j2pt_WJets);


    std::cout<<"Here? 16"<<std::endl;


    h_baseline_j1pt_sum  = (TH1F*)f->Get("J1pt_Pt_baseline_sum");  h_baseline_j2pt_sum  = (TH1F*)f->Get("J2pt_Pt_baseline_sum");
    // h_baseline_j1pt_data = (TH1F*)f->Get("J1pt_Pt_baseline_data"); h_baseline_j2pt_data = (TH1F*)f->Get("J2pt_Pt_baseline_data");
    // h_J1Pt_doubletagSR_data = (TH1F*)f->Get("J1pt_Pt_doubletagSR_data");  h_J2Pt_doubletagSR_data = (TH1F*)f->Get("J2pt_Pt_doubletagSR_data");
    // h_J1Pt_tagSR_data = (TH1F*)f->Get("J1pt_Pt_tagSR_data");  h_J2Pt_tagSR_data = (TH1F*)f->Get("J2pt_Pt_tagSR_data");
    h_baseline_j1pt_data = (TH1F*)fVetoData->Get("J1pt_Pt_baseline_data"); h_baseline_j2pt_data = (TH1F*)fVetoData->Get("J2pt_Pt_baseline_data");
    h_J1Pt_doubletagSR_data = (TH1F*)fVetoData->Get("J1pt_Pt_doubletagSR_data");  h_J2Pt_doubletagSR_data = (TH1F*)fVetoData->Get("J2pt_Pt_doubletagSR_data");
    h_J1Pt_doubletagSB_data = (TH1F*)fVetoData->Get("J1pt_Pt_doubletagSB_data");  h_J2Pt_doubletagSB_data = (TH1F*)fVetoData->Get("J2pt_Pt_doubletagSB_data");
    h_J1Pt_tagSR_data = (TH1F*)fVetoData->Get("J1pt_Pt_tagSR_data");  h_J2Pt_tagSR_data = (TH1F*)fVetoData->Get("J2pt_Pt_tagSR_data");
    h_J1Pt_tagSB_data = (TH1F*)fVetoData->Get("J1pt_Pt_tagSB_data");  h_J2Pt_tagSB_data = (TH1F*)fVetoData->Get("J2pt_Pt_tagSB_data");
    h_J1Pt_antitagSR_data = (TH1F*)fVetoData->Get("J1pt_Pt_antitagSR_data");  h_J2Pt_antitagSR_data = (TH1F*)fVetoData->Get("J2pt_Pt_antitagSR_data");
    h_J1Pt_antitagSB_data = (TH1F*)fVetoData->Get("J1pt_Pt_antitagSB_data");  h_J2Pt_antitagSB_data = (TH1F*)fVetoData->Get("J2pt_Pt_antitagSB_data");

    h_0H_j1pt_sum=(TH1F*)h_0H_j1pt_WJets->Clone("h_0H_j1pt_sum");h_0H_j1pt_sum->Add(h_0H_j1pt_ZJets);
    h_0H_j1pt_sum->Add(h_0H_j1pt_TT); h_0H_j1pt_sum->Add(h_0H_j1pt_SnglT); h_0H_j1pt_sum->Add(h_0H_j1pt_QCD);
    h_0H_j2pt_sum=(TH1F*)h_0H_j2pt_WJets->Clone("h_0H_j2pt_sum");h_0H_j2pt_sum->Add(h_0H_j2pt_ZJets);
    h_0H_j2pt_sum->Add(h_0H_j2pt_TT); h_0H_j2pt_sum->Add(h_0H_j2pt_SnglT); h_0H_j2pt_sum->Add(h_0H_j2pt_QCD);
    h_H_j1pt_sum = (TH1F*)h_baseline_j1pt_sum->Clone("h_H_j1pt_sum");  h_H_j1pt_sum->Add(h_0H_j1pt_sum,-1);
    h_H_j2pt_sum = (TH1F*)h_baseline_j2pt_sum->Clone("h_H_j2pt_sum");  h_H_j2pt_sum->Add(h_0H_j2pt_sum,-1);
    h_1H_j1pt_sum = (TH1F*)h_J1Pt_tagSR_sum->Clone("h_1H_j1pt_sum"); h_1H_j1pt_sum->Add(h_J1Pt_tagSB_sum);
    h_1H_j2pt_sum = (TH1F*)h_J2Pt_tagSR_sum->Clone("h_1H_j2pt_sum"); h_1H_j2pt_sum->Add(h_J2Pt_tagSB_sum);
    h_2H_j1pt_sum = (TH1F*)h_J1Pt_doubletagSR_sum->Clone("h_2H_j1pt_sum"); h_2H_j1pt_sum->Add(h_J1Pt_doubletagSB_sum);
    h_2H_j2pt_sum = (TH1F*)h_J2Pt_doubletagSR_sum->Clone("h_2H_j2pt_sum"); h_2H_j2pt_sum->Add(h_J2Pt_doubletagSB_sum);

    h_baseline_jpt_sum = (TH1F*)h_baseline_j1pt_sum->Clone("h_baseline_jpt_sum");h_baseline_jpt_sum->Add(h_baseline_j2pt_sum);
    h_0H_jpt_sum = (TH1F*)h_0H_j1pt_sum->Clone("h_0H_jpt_sum");h_0H_jpt_sum->Add(h_0H_j2pt_sum);
    h_1H_jpt_sum = (TH1F*)h_1H_j1pt_sum->Clone("h_1H_jpt_sum");h_1H_jpt_sum->Add(h_1H_j2pt_sum);
    h_2H_jpt_sum = (TH1F*)h_2H_j1pt_sum->Clone("h_2H_jpt_sum");h_2H_jpt_sum->Add(h_2H_j2pt_sum);


    std::cout<<"Here? 16.2"<<std::endl;

    // h_0H_j1pt_data = (TH1F*)f->Get("J1pt_Pt_antitagSR_data"); h_0H_j1pt_data->Add( (TH1F*)f->Get("J1pt_Pt_antitagSB_data") );
    // h_0H_j2pt_data = (TH1F*)f->Get("J2pt_Pt_antitagSR_data"); h_0H_j2pt_data->Add( (TH1F*)f->Get("J2pt_Pt_antitagSB_data") );
    h_0H_j1pt_data = (TH1F*)fVetoData->Get("J1pt_Pt_antitagSR_data"); h_0H_j1pt_data->Add( (TH1F*)fVetoData->Get("J1pt_Pt_antitagSB_data") );
    h_0H_j2pt_data = (TH1F*)fVetoData->Get("J2pt_Pt_antitagSR_data"); h_0H_j2pt_data->Add( (TH1F*)fVetoData->Get("J2pt_Pt_antitagSB_data") );
    h_H_j1pt_data = (TH1F*)h_baseline_j1pt_data->Clone("h_H_j1pt_data");  h_H_j1pt_data->Add(h_0H_j1pt_data,-1);
    h_H_j2pt_data = (TH1F*)h_baseline_j2pt_data->Clone("h_H_j2pt_data");  h_H_j2pt_data->Add(h_0H_j2pt_data,-1);
    h_1H_j1pt_data = (TH1F*)h_J1Pt_tagSR_data->Clone("h_1H_j1pt_data"); h_1H_j1pt_data->Add(h_J1Pt_tagSB_data);
    h_1H_j2pt_data = (TH1F*)h_J2Pt_tagSR_data->Clone("h_1H_j2pt_data"); h_1H_j2pt_data->Add(h_J2Pt_tagSB_data);
    h_2H_j1pt_data = (TH1F*)h_J1Pt_doubletagSR_data->Clone("h_2H_j1pt_data"); h_2H_j1pt_data->Add(h_J1Pt_doubletagSB_data);
    h_2H_j2pt_data = (TH1F*)h_J2Pt_doubletagSR_data->Clone("h_2H_j2pt_data"); h_2H_j2pt_data->Add(h_J2Pt_doubletagSB_data);

    h_baseline_jpt_data = (TH1F*)h_baseline_j1pt_data->Clone("h_baseline_jpt_data");h_baseline_jpt_data->Add(h_baseline_j2pt_data);
    h_0H_jpt_data = (TH1F*)h_0H_j1pt_data->Clone("h_0H_jpt_data");h_0H_jpt_data->Add(h_0H_j2pt_data);
    h_1H_jpt_data = (TH1F*)h_1H_j1pt_data->Clone("h_1H_jpt_data");h_1H_jpt_data->Add(h_1H_j2pt_data);
    h_2H_jpt_data = (TH1F*)h_2H_j1pt_data->Clone("h_2H_jpt_data");h_2H_jpt_data->Add(h_2H_j2pt_data);



    h_baseline_j1bb_sum  = (TH1F*)f->Get("LeadDeepBBTag_baseline_sum");  h_baseline_j2bb_sum  = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_sum");
    h_J1BB_doubletagSR_sum  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_sum");  h_J2BB_doubletagSR_sum  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_sum");
    h_J1BB_tagSR_sum  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_sum");  h_J2BB_tagSR_sum  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_sum");
    // h_baseline_j1bb_data = (TH1F*)f->Get("LeadDeepBBTag_baseline_data"); h_baseline_j2bb_data = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_data");
    // h_J1BB_doubletagSR_data  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_data");  h_J2BB_doubletagSR_data  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_data");
    // h_J1BB_tagSR_data  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_data");  h_J2BB_tagSR_data  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_data");
    h_baseline_j1bb_data = (TH1F*)fVetoData->Get("LeadDeepBBTag_baseline_data"); h_baseline_j2bb_data = (TH1F*)fVetoData->Get("SubLeadDeepBBTag_baseline_data");
    h_J1BB_doubletagSR_data  = (TH1F*)fVetoData->Get("LeadDeepBBTag_doubletagSR_data");  h_J2BB_doubletagSR_data  = (TH1F*)fVetoData->Get("SubLeadDeepBBTag_doubletagSR_data");
    h_J1BB_tagSR_data  = (TH1F*)fVetoData->Get("LeadDeepBBTag_tagSR_data");  h_J2BB_tagSR_data  = (TH1F*)fVetoData->Get("SubLeadDeepBBTag_tagSR_data");
    h_baseline_j1bb_QCD  = (TH1F*)f->Get("LeadDeepBBTag_baseline_QCD");  h_baseline_j2bb_QCD  = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_QCD");
    h_J1BB_doubletagSR_QCD  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_QCD");  h_J2BB_doubletagSR_QCD  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_QCD");
    h_J1BB_tagSR_QCD  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_QCD");  h_J2BB_tagSR_QCD  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_QCD");
    h_baseline_j1bb_ZJets  = (TH1F*)f->Get("LeadDeepBBTag_baseline_ZJets");  h_baseline_j2bb_ZJets  = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_ZJets");
    h_J1BB_doubletagSR_ZJets  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_ZJets");  h_J2BB_doubletagSR_ZJets  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_ZJets");
    h_J1BB_tagSR_ZJets  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_ZJets");  h_J2BB_tagSR_ZJets  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_ZJets");
    h_baseline_j1bb_WJets  = (TH1F*)f->Get("LeadDeepBBTag_baseline_WJets");  h_baseline_j2bb_WJets  = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_WJets");
    h_J1BB_doubletagSR_WJets  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_WJets");  h_J2BB_doubletagSR_WJets  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_WJets");
    h_J1BB_tagSR_WJets  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_WJets");  h_J2BB_tagSR_WJets  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_WJets");
    h_baseline_j1bb_TT  = (TH1F*)f->Get("LeadDeepBBTag_baseline_TT");  h_baseline_j2bb_TT  = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_TT");
    h_J1BB_doubletagSR_TT  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_TT");  h_J2BB_doubletagSR_TT  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_TT");
    h_J1BB_tagSR_TT  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_TT");  h_J2BB_tagSR_TT  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_TT");
    h_baseline_j1bb_SnglT = (TH1F*)f->Get("LeadDeepBBTag_baseline_SnglT");  h_baseline_j2bb_SnglT  = (TH1F*)f->Get("SubLeadDeepBBTag_baseline_SnglT");
    h_J1BB_doubletagSR_SnglT  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_SnglT");  h_J2BB_doubletagSR_SnglT  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_SnglT");
    h_J1BB_tagSR_SnglT  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_SnglT");  h_J2BB_tagSR_SnglT  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_SnglT");

    h_0H_j1bb_QCD  = (TH1F*)f->Get("LeadDeepBBTag_antitagSR_QCD"); h_0H_j1bb_QCD->Add( (TH1F*)f->Get("LeadDeepBBTag_antitagSB_QCD") );
    h_0H_j1bb_ZJets  = (TH1F*)f->Get("LeadDeepBBTag_antitagSR_ZJets"); h_0H_j1bb_ZJets->Add( (TH1F*)f->Get("LeadDeepBBTag_antitagSB_ZJets") );
    h_0H_j1bb_WJets  = (TH1F*)f->Get("LeadDeepBBTag_antitagSR_WJets"); h_0H_j1bb_WJets->Add( (TH1F*)f->Get("LeadDeepBBTag_antitagSB_WJets") );
    h_0H_j1bb_TT  = (TH1F*)f->Get("LeadDeepBBTag_antitagSR_TT"); h_0H_j1bb_TT->Add( (TH1F*)f->Get("LeadDeepBBTag_antitagSB_TT") );
    h_0H_j1bb_SnglT  = (TH1F*)f->Get("LeadDeepBBTag_antitagSR_SnglT"); h_0H_j1bb_SnglT->Add( (TH1F*)f->Get("LeadDeepBBTag_antitagSB_SnglT") );

    h_0H_j2bb_QCD  = (TH1F*)f->Get("SubLeadDeepBBTag_antitagSR_QCD"); h_0H_j2bb_QCD->Add( (TH1F*)f->Get("SubLeadDeepBBTag_antitagSB_QCD") );
    h_0H_j2bb_ZJets  = (TH1F*)f->Get("SubLeadDeepBBTag_antitagSR_ZJets"); h_0H_j2bb_ZJets->Add( (TH1F*)f->Get("SubLeadDeepBBTag_antitagSB_ZJets") );
    h_0H_j2bb_WJets  = (TH1F*)f->Get("SubLeadDeepBBTag_antitagSR_WJets"); h_0H_j2bb_WJets->Add( (TH1F*)f->Get("SubLeadDeepBBTag_antitagSB_WJets") );
    h_0H_j2bb_TT  = (TH1F*)f->Get("SubLeadDeepBBTag_antitagSR_TT"); h_0H_j2bb_TT->Add( (TH1F*)f->Get("SubLeadDeepBBTag_antitagSB_TT") );
    h_0H_j2bb_SnglT  = (TH1F*)f->Get("SubLeadDeepBBTag_antitagSR_SnglT"); h_0H_j2bb_SnglT->Add( (TH1F*)f->Get("SubLeadDeepBBTag_antitagSB_SnglT") );

    h_1H_j1bb_QCD  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_QCD"); h_1H_j1bb_QCD->Add( (TH1F*)f->Get("LeadDeepBBTag_tagSB_QCD") );
    h_1H_j1bb_ZJets  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_ZJets"); h_1H_j1bb_ZJets->Add( (TH1F*)f->Get("LeadDeepBBTag_tagSB_ZJets") );
    h_1H_j1bb_WJets  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_WJets"); h_1H_j1bb_WJets->Add( (TH1F*)f->Get("LeadDeepBBTag_tagSB_WJets") );
    h_1H_j1bb_TT  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_TT"); h_1H_j1bb_TT->Add( (TH1F*)f->Get("LeadDeepBBTag_tagSB_TT") );
    h_1H_j1bb_SnglT  = (TH1F*)f->Get("LeadDeepBBTag_tagSR_SnglT"); h_1H_j1bb_SnglT->Add( (TH1F*)f->Get("LeadDeepBBTag_tagSB_SnglT") );

    h_1H_j2bb_QCD  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_QCD"); h_1H_j2bb_QCD->Add( (TH1F*)f->Get("SubLeadDeepBBTag_tagSB_QCD") );
    h_1H_j2bb_ZJets  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_ZJets"); h_1H_j2bb_ZJets->Add( (TH1F*)f->Get("SubLeadDeepBBTag_tagSB_ZJets") );
    h_1H_j2bb_WJets  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_WJets"); h_1H_j2bb_WJets->Add( (TH1F*)f->Get("SubLeadDeepBBTag_tagSB_WJets") );
    h_1H_j2bb_TT  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_TT"); h_1H_j2bb_TT->Add( (TH1F*)f->Get("SubLeadDeepBBTag_tagSB_TT") );
    h_1H_j2bb_SnglT  = (TH1F*)f->Get("SubLeadDeepBBTag_tagSR_SnglT"); h_1H_j2bb_SnglT->Add( (TH1F*)f->Get("SubLeadDeepBBTag_tagSB_SnglT") );

    h_2H_j1bb_QCD  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_QCD"); h_2H_j1bb_QCD->Add( (TH1F*)f->Get("LeadDeepBBTag_doubletagSB_QCD") );
    h_2H_j1bb_ZJets  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_ZJets"); h_2H_j1bb_ZJets->Add( (TH1F*)f->Get("LeadDeepBBTag_doubletagSB_ZJets") );
    h_2H_j1bb_WJets  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_WJets"); h_2H_j1bb_WJets->Add( (TH1F*)f->Get("LeadDeepBBTag_doubletagSB_WJets") );
    h_2H_j1bb_TT  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_TT"); h_2H_j1bb_TT->Add( (TH1F*)f->Get("LeadDeepBBTag_doubletagSB_TT") );
    h_2H_j1bb_SnglT  = (TH1F*)f->Get("LeadDeepBBTag_doubletagSR_SnglT"); h_2H_j1bb_SnglT->Add( (TH1F*)f->Get("LeadDeepBBTag_doubletagSB_SnglT") );

    h_2H_j2bb_QCD  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_QCD"); h_2H_j2bb_QCD->Add( (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSB_QCD") );
    h_2H_j2bb_ZJets  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_ZJets"); h_2H_j2bb_ZJets->Add( (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSB_ZJets") );
    h_2H_j2bb_WJets  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_WJets"); h_2H_j2bb_WJets->Add( (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSB_WJets") );
    h_2H_j2bb_TT  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_TT"); h_2H_j2bb_TT->Add( (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSB_TT") );
    h_2H_j2bb_SnglT  = (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSR_SnglT"); h_2H_j2bb_SnglT->Add( (TH1F*)f->Get("SubLeadDeepBBTag_doubletagSB_SnglT") );

    std::cout<<"Here? 18"<<std::endl;


    h_0H_j1bb_sum = (TH1F*)h_0H_j1bb_QCD->Clone("h_0H_j1bb_sum"); h_0H_j1bb_sum->Add(h_0H_j1bb_ZJets);
    h_0H_j1bb_sum->Add(h_0H_j1bb_WJets); h_0H_j1bb_sum->Add(h_0H_j1bb_TT); h_0H_j1bb_sum->Add(h_0H_j1bb_SnglT);
    h_0H_j2bb_sum = (TH1F*)h_0H_j2bb_QCD->Clone("h_0H_j2bb_sum"); h_0H_j2bb_sum->Add(h_0H_j2bb_ZJets);
    h_0H_j2bb_sum->Add(h_0H_j2bb_WJets); h_0H_j2bb_sum->Add(h_0H_j2bb_TT); h_0H_j2bb_sum->Add(h_0H_j2bb_SnglT);

    h_1H_j1bb_sum = (TH1F*)h_1H_j1bb_QCD->Clone("h_1H_j1bb_sum"); h_1H_j1bb_sum->Add(h_1H_j1bb_ZJets);
    h_1H_j1bb_sum->Add(h_1H_j1bb_WJets); h_1H_j1bb_sum->Add(h_1H_j1bb_TT); h_1H_j1bb_sum->Add(h_1H_j1bb_SnglT);
    h_1H_j2bb_sum = (TH1F*)h_1H_j2bb_QCD->Clone("h_1H_j2bb_sum"); h_1H_j2bb_sum->Add(h_1H_j2bb_ZJets);
    h_1H_j2bb_sum->Add(h_1H_j2bb_WJets); h_1H_j2bb_sum->Add(h_1H_j2bb_TT); h_1H_j2bb_sum->Add(h_1H_j2bb_SnglT);

    h_2H_j1bb_sum = (TH1F*)h_2H_j1bb_QCD->Clone("h_2H_j1bb_sum"); h_2H_j1bb_sum->Add(h_2H_j1bb_ZJets);
    h_2H_j1bb_sum->Add(h_2H_j1bb_WJets); h_2H_j1bb_sum->Add(h_2H_j1bb_TT); h_2H_j1bb_sum->Add(h_2H_j1bb_SnglT);
    h_2H_j2bb_sum = (TH1F*)h_2H_j2bb_QCD->Clone("h_2H_j2bb_sum"); h_2H_j2bb_sum->Add(h_2H_j2bb_ZJets);
    h_2H_j2bb_sum->Add(h_2H_j2bb_WJets); h_2H_j2bb_sum->Add(h_2H_j2bb_TT); h_2H_j2bb_sum->Add(h_2H_j2bb_SnglT);



    h_0H_j1bb_data = (TH1F*)fVetoData->Get("LeadDeepBBTag_antitagSR_data"); h_0H_j1bb_data->Add( (TH1F*)fVetoData->Get("LeadDeepBBTag_antitagSB_data") );
    h_0H_j2bb_data = (TH1F*)fVetoData->Get("SubLeadDeepBBTag_antitagSR_data"); h_0H_j2bb_data->Add( (TH1F*)fVetoData->Get("SubLeadDeepBBTag_antitagSB_data") );
    // h_0H_j1bb_data = (TH1F*)f->Get("LeadDeepBBTag_antitagSR_data"); h_0H_j1bb_data->Add( (TH1F*)f->Get("LeadDeepBBTag_antitagSB_data") );
    // h_0H_j2bb_data = (TH1F*)f->Get("SubLeadDeepBBTag_antitagSR_data"); h_0H_j2bb_data->Add( (TH1F*)f->Get("SubLeadDeepBBTag_antitagSB_data") );
    h_1H_j1bb_data = (TH1F*)fVetoData->Get("LeadDeepBBTag_tagSR_data"); h_1H_j1bb_data->Add( (TH1F*)fVetoData->Get("LeadDeepBBTag_tagSB_data") );
    h_1H_j2bb_data = (TH1F*)fVetoData->Get("SubLeadDeepBBTag_tagSR_data"); h_1H_j2bb_data->Add( (TH1F*)fVetoData->Get("SubLeadDeepBBTag_tagSB_data") );
    h_2H_j1bb_data = (TH1F*)fVetoData->Get("LeadDeepBBTag_doubletagSR_data"); h_2H_j1bb_data->Add( (TH1F*)fVetoData->Get("LeadDeepBBTag_doubletagSB_data") );
    h_2H_j2bb_data = (TH1F*)fVetoData->Get("SubLeadDeepBBTag_doubletagSR_data"); h_2H_j2bb_data->Add( (TH1F*)fVetoData->Get("SubLeadDeepBBTag_doubletagSB_data") );

    h_H_j1bb_ZJets = (TH1F*)h_baseline_j1bb_ZJets->Clone("h_H_j1bb_ZJets");  h_H_j1bb_ZJets->Add(h_0H_j1bb_ZJets,-1);
    h_H_j2bb_ZJets = (TH1F*)h_baseline_j2bb_ZJets->Clone("h_H_j2bb_ZJets");  h_H_j2bb_ZJets->Add(h_0H_j2bb_ZJets,-1);
    h_H_j1bb_WJets = (TH1F*)h_baseline_j1bb_WJets->Clone("h_H_j1bb_WJets");  h_H_j1bb_WJets->Add(h_0H_j1bb_WJets,-1);
    h_H_j2bb_WJets = (TH1F*)h_baseline_j2bb_WJets->Clone("h_H_j2bb_WJets");  h_H_j2bb_WJets->Add(h_0H_j2bb_WJets,-1);
    h_H_j1bb_TT = (TH1F*)h_baseline_j1bb_TT->Clone("h_H_j1bb_TT");  h_H_j1bb_TT->Add(h_0H_j1bb_TT,-1);
    h_H_j2bb_TT = (TH1F*)h_baseline_j2bb_TT->Clone("h_H_j2bb_TT");  h_H_j2bb_TT->Add(h_0H_j2bb_TT,-1);
    h_H_j1bb_SnglT = (TH1F*)h_baseline_j1bb_SnglT->Clone("h_H_j1bb_SnglT");  h_H_j1bb_SnglT->Add(h_0H_j1bb_SnglT,-1);
    h_H_j2bb_SnglT = (TH1F*)h_baseline_j2bb_SnglT->Clone("h_H_j2bb_SnglT");  h_H_j2bb_SnglT->Add(h_0H_j2bb_SnglT,-1);
    h_H_j1bb_QCD = (TH1F*)h_baseline_j1bb_QCD->Clone("h_H_j1bb_QCD");  h_H_j1bb_QCD->Add(h_0H_j1bb_QCD,-1);
    h_H_j2bb_QCD = (TH1F*)h_baseline_j2bb_QCD->Clone("h_H_j2bb_QCD");  h_H_j2bb_QCD->Add(h_0H_j2bb_QCD,-1);
    h_H_j1bb_sum = (TH1F*)h_baseline_j1bb_sum->Clone("h_H_j1bb_sum");  h_H_j1bb_sum->Add(h_0H_j1bb_sum,-1);
    h_H_j2bb_sum = (TH1F*)h_baseline_j2bb_sum->Clone("h_H_j2bb_sum");  h_H_j2bb_sum->Add(h_0H_j2bb_sum,-1);
    h_H_j1bb_data = (TH1F*)h_baseline_j1bb_data->Clone("h_H_j1bb_data");  h_H_j1bb_data->Add(h_0H_j1bb_data,-1);
    h_H_j2bb_data = (TH1F*)h_baseline_j2bb_data->Clone("h_H_j2bb_data");  h_H_j2bb_data->Add(h_0H_j2bb_data,-1);

    std::cout<<"Here? 19"<<std::endl;

    h_baseline_jbb_data = (TH1F*)h_baseline_j1bb_data->Clone("h_baseline_jbb_data");h_baseline_jbb_data->Add(h_baseline_j2bb_data);
    h_0H_jbb_data = (TH1F*)h_0H_j1bb_data->Clone("h_0H_jbb_data");h_0H_jbb_data->Add(h_0H_j2bb_data);
    h_1H_jbb_data = (TH1F*)h_1H_j1bb_data->Clone("h_1H_jbb_data");h_1H_jbb_data->Add(h_1H_j2bb_data);
    h_2H_jbb_data = (TH1F*)h_2H_j1bb_data->Clone("h_2H_jbb_data");h_2H_jbb_data->Add(h_2H_j2bb_data);

    h_baseline_jbb_sum = (TH1F*)h_baseline_j1bb_sum->Clone("h_baseline_jbb_sum");h_baseline_jbb_sum->Add(h_baseline_j2bb_sum);
    h_0H_jbb_sum = (TH1F*)h_0H_j1bb_sum->Clone("h_0H_jbb_sum");h_0H_jbb_sum->Add(h_0H_j2bb_sum);
    h_1H_jbb_sum = (TH1F*)h_1H_j1bb_sum->Clone("h_1H_jbb_sum");h_1H_jbb_sum->Add(h_1H_j2bb_sum);
    h_2H_jbb_sum = (TH1F*)h_2H_j1bb_sum->Clone("h_2H_jbb_sum");h_2H_jbb_sum->Add(h_2H_j2bb_sum);

    h_baseline_jbb_QCD = (TH1F*)h_baseline_j1bb_QCD->Clone("h_baseline_jbb_QCD");h_baseline_jbb_QCD->Add(h_baseline_j2bb_QCD);
    h_0H_jbb_QCD = (TH1F*)h_0H_j1bb_QCD->Clone("h_0H_jbb_QCD");h_0H_jbb_QCD->Add(h_0H_j2bb_QCD);
    h_1H_jbb_QCD = (TH1F*)h_1H_j1bb_QCD->Clone("h_1H_jbb_QCD");h_1H_jbb_QCD->Add(h_1H_j2bb_QCD);
    h_2H_jbb_QCD = (TH1F*)h_2H_j1bb_QCD->Clone("h_2H_jbb_QCD");h_2H_jbb_QCD->Add(h_2H_j2bb_QCD);

    h_baseline_jbb_SnglT = (TH1F*)h_baseline_j1bb_SnglT->Clone("h_baseline_jbb_SnglT");h_baseline_jbb_SnglT->Add(h_baseline_j2bb_SnglT);
    h_0H_jbb_SnglT = (TH1F*)h_0H_j1bb_SnglT->Clone("h_0H_jbb_SnglT");h_0H_jbb_SnglT->Add(h_0H_j2bb_SnglT);
    h_1H_jbb_SnglT = (TH1F*)h_1H_j1bb_SnglT->Clone("h_1H_jbb_SnglT");h_1H_jbb_SnglT->Add(h_1H_j2bb_SnglT);
    h_2H_jbb_SnglT = (TH1F*)h_2H_j1bb_SnglT->Clone("h_2H_jbb_SnglT");h_2H_jbb_SnglT->Add(h_2H_j2bb_SnglT);

    h_baseline_jbb_TT = (TH1F*)h_baseline_j1bb_TT->Clone("h_baseline_jbb_TT");h_baseline_jbb_TT->Add(h_baseline_j2bb_TT);
    h_0H_jbb_TT = (TH1F*)h_0H_j1bb_TT->Clone("h_0H_jbb_TT");h_0H_jbb_TT->Add(h_0H_j2bb_TT);
    h_1H_jbb_TT = (TH1F*)h_1H_j1bb_TT->Clone("h_1H_jbb_TT");h_1H_jbb_TT->Add(h_1H_j2bb_TT);
    h_2H_jbb_TT = (TH1F*)h_2H_j1bb_TT->Clone("h_2H_jbb_TT");h_2H_jbb_TT->Add(h_2H_j2bb_TT);

    h_baseline_jbb_WJets = (TH1F*)h_baseline_j1bb_WJets->Clone("h_baseline_jbb_WJets");h_baseline_jbb_WJets->Add(h_baseline_j2bb_WJets);
    h_0H_jbb_WJets = (TH1F*)h_0H_j1bb_WJets->Clone("h_0H_jbb_WJets");h_0H_jbb_WJets->Add(h_0H_j2bb_WJets);
    h_1H_jbb_WJets = (TH1F*)h_1H_j1bb_WJets->Clone("h_1H_jbb_WJets");h_1H_jbb_WJets->Add(h_1H_j2bb_WJets);
    h_2H_jbb_WJets = (TH1F*)h_2H_j1bb_WJets->Clone("h_2H_jbb_WJets");h_2H_jbb_WJets->Add(h_2H_j2bb_WJets);

    h_baseline_jbb_ZJets = (TH1F*)h_baseline_j1bb_ZJets->Clone("h_baseline_jbb_ZJets");h_baseline_jbb_ZJets->Add(h_baseline_j2bb_ZJets);
    h_0H_jbb_ZJets = (TH1F*)h_0H_j1bb_ZJets->Clone("h_0H_jbb_ZJets");h_0H_jbb_ZJets->Add(h_0H_j2bb_ZJets);
    h_1H_jbb_ZJets = (TH1F*)h_1H_j1bb_ZJets->Clone("h_1H_jbb_ZJets");h_1H_jbb_ZJets->Add(h_1H_j2bb_ZJets);
    h_2H_jbb_ZJets = (TH1F*)h_2H_j1bb_ZJets->Clone("h_2H_jbb_ZJets");h_2H_jbb_ZJets->Add(h_2H_j2bb_ZJets);

    //for signal
    if (runSignal){
      std::cout<<"Here? 18"<<std::endl;

      h_J1Pt_doubletagSR_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_doubletagSR_T5HH1600_LSP1"); h_J2Pt_doubletagSR_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_doubletagSR_T5HH1600_LSP1");
      h_J1Pt_doubletagSB_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_doubletagSB_T5HH1600_LSP1"); h_J2Pt_doubletagSB_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_doubletagSB_T5HH1600_LSP1");
      h_J1Pt_antitagSR_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_antitagSR_T5HH1600_LSP1"); h_J2Pt_antitagSR_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_antitagSR_T5HH1600_LSP1");
      h_J1Pt_antitagSB_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_antitagSB_T5HH1600_LSP1"); h_J2Pt_antitagSB_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_antitagSB_T5HH1600_LSP1");
      h_J1Pt_tagSR_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_tagSR_T5HH1600_LSP1"); h_J2Pt_tagSR_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_tagSR_T5HH1600_LSP1");
      h_J1Pt_tagSB_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_tagSB_T5HH1600_LSP1"); h_J2Pt_tagSB_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_tagSB_T5HH1600_LSP1");
      h_baseline_j1pt_T5HH1600 = (TH1F*)fSignal2->Get("J1pt_Pt_baseline_T5HH1600_LSP1"); h_baseline_j2pt_T5HH1600 = (TH1F*)fSignal2->Get("J2pt_Pt_baseline_T5HH1600_LSP1");
      h_H_j1pt_T5HH1600 = (TH1F*)h_baseline_j1pt_T5HH1600->Clone("h_H_j1pt_T5HH1600"); h_H_j1pt_T5HH1600->Add(h_J1Pt_antitagSR_T5HH1600,-1); h_H_j1pt_T5HH1600->Add(h_J1Pt_antitagSB_T5HH1600,-1);
      h_H_j2pt_T5HH1600 = (TH1F*)h_baseline_j2pt_T5HH1600->Clone("h_H_j2pt_T5HH1600"); h_H_j2pt_T5HH1600->Add(h_J2Pt_antitagSR_T5HH1600,-1); h_H_j2pt_T5HH1600->Add(h_J2Pt_antitagSB_T5HH1600,-1);
      h_0H_j1pt_T5HH1600 = (TH1F*)h_J1Pt_antitagSR_T5HH1600->Clone("h_0H_j1pt_T5HH1600"); h_0H_j1pt_T5HH1600->Add(h_J1Pt_antitagSB_T5HH1600);
      h_0H_j2pt_T5HH1600 = (TH1F*)h_J2Pt_antitagSR_T5HH1600->Clone("h_0H_j2pt_T5HH1600"); h_0H_j2pt_T5HH1600->Add(h_J2Pt_antitagSB_T5HH1600);
      h_1H_j1pt_T5HH1600 = (TH1F*)h_J1Pt_tagSR_T5HH1600->Clone("h_1H_j1pt_T5HH1600"); h_1H_j1pt_T5HH1600->Add(h_J1Pt_tagSB_T5HH1600);
      h_1H_j2pt_T5HH1600 = (TH1F*)h_J2Pt_tagSR_T5HH1600->Clone("h_1H_j2pt_T5HH1600"); h_1H_j2pt_T5HH1600->Add(h_J2Pt_tagSB_T5HH1600);
      h_2H_j1pt_T5HH1600 = (TH1F*)h_J1Pt_doubletagSR_T5HH1600->Clone("h_2H_j1pt_T5HH1600"); h_2H_j1pt_T5HH1600->Add(h_J1Pt_doubletagSB_T5HH1600);
      h_2H_j2pt_T5HH1600 = (TH1F*)h_J2Pt_doubletagSR_T5HH1600->Clone("h_2H_j2pt_T5HH1600"); h_2H_j2pt_T5HH1600->Add(h_J2Pt_doubletagSB_T5HH1600);

      h_baseline_jpt_T5HH1600 = (TH1F*)h_baseline_j1pt_T5HH1600->Clone("h_baseline_jpt_T5HH1600");h_baseline_jpt_T5HH1600->Add(h_baseline_j2pt_T5HH1600);
      h_0H_jpt_T5HH1600 = (TH1F*)h_0H_j1pt_T5HH1600->Clone("h_0H_jpt_T5HH1600");h_0H_jpt_T5HH1600->Add(h_0H_j2pt_T5HH1600);
      h_1H_jpt_T5HH1600 = (TH1F*)h_1H_j1pt_T5HH1600->Clone("h_1H_jpt_T5HH1600");h_1H_jpt_T5HH1600->Add(h_1H_j2pt_T5HH1600);
      h_2H_jpt_T5HH1600 = (TH1F*)h_2H_j1pt_T5HH1600->Clone("h_2H_jpt_T5HH1600");h_2H_jpt_T5HH1600->Add(h_2H_j2pt_T5HH1600);


      std::cout<<"Here? 20"<<std::endl;


      h_baseline_j1bb_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_baseline_T5HH1600_LSP1"); h_baseline_j2bb_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_baseline_T5HH1600_LSP1");
      h_J1BB_antitagSR_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_antitagSR_T5HH1600_LSP1"); h_J2BB_antitagSR_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_antitagSR_T5HH1600_LSP1");
      h_J1BB_antitagSB_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_antitagSB_T5HH1600_LSP1"); h_J2BB_antitagSB_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_antitagSB_T5HH1600_LSP1");
      h_J1BB_tagSR_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_tagSR_T5HH1600_LSP1"); h_J2BB_tagSR_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_tagSR_T5HH1600_LSP1");
      h_J1BB_tagSB_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_tagSB_T5HH1600_LSP1"); h_J2BB_tagSB_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_tagSB_T5HH1600_LSP1");
      h_J1BB_doubletagSR_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_doubletagSR_T5HH1600_LSP1"); h_J2BB_doubletagSR_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_doubletagSR_T5HH1600_LSP1");
      h_J1BB_doubletagSB_T5HH1600 = (TH1F*)fSignal2->Get("LeadDeepBBTag_doubletagSB_T5HH1600_LSP1"); h_J2BB_doubletagSB_T5HH1600 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_doubletagSB_T5HH1600_LSP1");
      h_H_j1bb_T5HH1600 = (TH1F*)h_baseline_j1bb_T5HH1600->Clone("h_H_j1bb_T5HH1600"); h_H_j1bb_T5HH1600->Add(h_J1BB_antitagSR_T5HH1600,-1); h_H_j1bb_T5HH1600->Add(h_J1BB_antitagSB_T5HH1600,-1);
      h_H_j2bb_T5HH1600 = (TH1F*)h_baseline_j2bb_T5HH1600->Clone("h_H_j2bb_T5HH1600"); h_H_j2bb_T5HH1600->Add(h_J2BB_antitagSR_T5HH1600,-1); h_H_j2bb_T5HH1600->Add(h_J2BB_antitagSB_T5HH1600,-1);
      h_0H_j1bb_T5HH1600 = (TH1F*)h_J1BB_antitagSR_T5HH1600->Clone("h_0H_j1bb_T5HH1600"); h_0H_j1bb_T5HH1600->Add(h_J1BB_antitagSB_T5HH1600);
      h_0H_j2bb_T5HH1600 = (TH1F*)h_J2BB_antitagSR_T5HH1600->Clone("h_0H_j2bb_T5HH1600"); h_0H_j2bb_T5HH1600->Add(h_J2BB_antitagSB_T5HH1600);
      h_1H_j1bb_T5HH1600 = (TH1F*)h_J1BB_tagSR_T5HH1600->Clone("h_1H_j1bb_T5HH1600"); h_1H_j1bb_T5HH1600->Add(h_J1BB_tagSB_T5HH1600);
      h_1H_j2bb_T5HH1600 = (TH1F*)h_J2BB_tagSR_T5HH1600->Clone("h_1H_j2bb_T5HH1600"); h_1H_j2bb_T5HH1600->Add(h_J2BB_tagSB_T5HH1600);
      h_2H_j1bb_T5HH1600 = (TH1F*)h_J1BB_doubletagSR_T5HH1600->Clone("h_2H_j1bb_T5HH1600"); h_2H_j1bb_T5HH1600->Add(h_J1BB_doubletagSB_T5HH1600);
      h_2H_j2bb_T5HH1600 = (TH1F*)h_J2BB_doubletagSR_T5HH1600->Clone("h_2H_j2bb_T5HH1600"); h_2H_j2bb_T5HH1600->Add(h_J2BB_doubletagSB_T5HH1600);

      h_baseline_jbb_T5HH1600 = (TH1F*)h_baseline_j1bb_T5HH1600->Clone("h_baseline_jbb_T5HH1600");h_baseline_jbb_T5HH1600->Add(h_baseline_j2bb_T5HH1600);
      h_0H_jbb_T5HH1600 = (TH1F*)h_0H_j1bb_T5HH1600->Clone("h_0H_jbb_T5HH1600");h_0H_jbb_T5HH1600->Add(h_0H_j2bb_T5HH1600);
      h_1H_jbb_T5HH1600 = (TH1F*)h_1H_j1bb_T5HH1600->Clone("h_1H_jbb_T5HH1600");h_1H_jbb_T5HH1600->Add(h_1H_j2bb_T5HH1600);
      h_2H_jbb_T5HH1600 = (TH1F*)h_2H_j1bb_T5HH1600->Clone("h_2H_jbb_T5HH1600");h_2H_jbb_T5HH1600->Add(h_2H_j2bb_T5HH1600);


      std::cout<<"Here? 22"<<std::endl;

      h_J1Pt_doubletagSR_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_doubletagSR_T5HH2000_LSP1"); h_J2Pt_doubletagSR_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_doubletagSR_T5HH2000_LSP1");
      h_J1Pt_doubletagSB_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_doubletagSB_T5HH2000_LSP1"); h_J2Pt_doubletagSB_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_doubletagSB_T5HH2000_LSP1");
      h_J1Pt_antitagSR_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_antitagSR_T5HH2000_LSP1"); h_J2Pt_antitagSR_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_antitagSR_T5HH2000_LSP1");
      h_J1Pt_antitagSB_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_antitagSB_T5HH2000_LSP1"); h_J2Pt_antitagSB_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_antitagSB_T5HH2000_LSP1");
      h_J1Pt_tagSR_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_tagSR_T5HH2000_LSP1"); h_J2Pt_tagSR_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_tagSR_T5HH2000_LSP1");
      h_J1Pt_tagSB_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_tagSB_T5HH2000_LSP1"); h_J2Pt_tagSB_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_tagSB_T5HH2000_LSP1");
      h_baseline_j1pt_T5HH2000 = (TH1F*)fSignal2->Get("J1pt_Pt_baseline_T5HH2000_LSP1"); h_baseline_j2pt_T5HH2000 = (TH1F*)fSignal2->Get("J2pt_Pt_baseline_T5HH2000_LSP1");

      h_H_j1pt_T5HH2000 = (TH1F*)h_baseline_j1pt_T5HH2000->Clone("h_H_j1pt_T5HH2000"); h_H_j1pt_T5HH2000->Add(h_J1Pt_antitagSR_T5HH2000,-1); h_H_j1pt_T5HH2000->Add(h_J1Pt_antitagSB_T5HH2000,-1);
      h_H_j2pt_T5HH2000 = (TH1F*)h_baseline_j2pt_T5HH2000->Clone("h_H_j2pt_T5HH2000"); h_H_j2pt_T5HH2000->Add(h_J2Pt_antitagSR_T5HH2000,-1); h_H_j2pt_T5HH2000->Add(h_J2Pt_antitagSB_T5HH2000,-1);
      h_baseline_j1bb_T5HH2000 = (TH1F*)fSignal2->Get("LeadDeepBBTag_baseline_T5HH2000_LSP1"); h_baseline_j2bb_T5HH2000 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_baseline_T5HH2000_LSP1");
      h_J1BB_antitagSR_T5HH2000 = (TH1F*)fSignal2->Get("LeadDeepBBTag_antitagSR_T5HH2000_LSP1"); h_J2BB_antitagSR_T5HH2000 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_antitagSR_T5HH2000_LSP1");
      h_J1BB_tagSR_T5HH2000 = (TH1F*)fSignal2->Get("LeadDeepBBTag_tagSR_T5HH2000_LSP1"); h_J2BB_tagSR_T5HH2000 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_tagSR_T5HH2000_LSP1");
      h_J1BB_doubletagSR_T5HH2000 = (TH1F*)fSignal2->Get("LeadDeepBBTag_doubletagSR_T5HH2000_LSP1"); h_J2BB_doubletagSR_T5HH2000 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_doubletagSR_T5HH2000_LSP1");

      h_J1BB_antitagSB_T5HH2000 = (TH1F*)fSignal2->Get("LeadDeepBBTag_antitagSB_T5HH2000_LSP1"); h_J2BB_antitagSB_T5HH2000 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_antitagSB_T5HH2000_LSP1");
      h_H_j1bb_T5HH2000 = (TH1F*)h_baseline_j1bb_T5HH2000->Clone("h_H_j1bb_T5HH2000"); h_H_j1bb_T5HH2000->Add(h_J1BB_antitagSR_T5HH2000,-1); h_H_j1bb_T5HH2000->Add(h_J1BB_antitagSB_T5HH2000,-1);
      h_H_j2bb_T5HH2000 = (TH1F*)h_baseline_j2bb_T5HH2000->Clone("h_H_j2bb_T5HH2000"); h_H_j2bb_T5HH2000->Add(h_J2BB_antitagSR_T5HH2000,-1); h_H_j2bb_T5HH2000->Add(h_J2BB_antitagSB_T5HH2000,-1);

      h_J1Pt_doubletagSR_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_doubletagSR_T5HH2200_LSP1"); h_J2Pt_doubletagSR_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_doubletagSR_T5HH2200_LSP1");
      h_J1Pt_doubletagSB_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_doubletagSB_T5HH2200_LSP1"); h_J2Pt_doubletagSB_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_doubletagSB_T5HH2200_LSP1");
      h_J1Pt_antitagSR_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_antitagSR_T5HH2200_LSP1"); h_J2Pt_antitagSR_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_antitagSR_T5HH2200_LSP1");
      h_J1Pt_antitagSB_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_antitagSB_T5HH2200_LSP1"); h_J2Pt_antitagSB_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_antitagSB_T5HH2200_LSP1");
      h_J1Pt_tagSR_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_tagSR_T5HH2200_LSP1"); h_J2Pt_tagSR_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_tagSR_T5HH2200_LSP1");
      h_J1Pt_tagSB_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_tagSB_T5HH2200_LSP1"); h_J2Pt_tagSB_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_tagSB_T5HH2200_LSP1");
      h_baseline_j1pt_T5HH2200 = (TH1F*)fSignal2->Get("J1pt_Pt_baseline_T5HH2200_LSP1"); h_baseline_j2pt_T5HH2200 = (TH1F*)fSignal2->Get("J2pt_Pt_baseline_T5HH2200_LSP1");

      h_H_j1pt_T5HH2200 = (TH1F*)h_baseline_j1pt_T5HH2200->Clone("h_H_j1pt_T5HH2200"); h_H_j1pt_T5HH2200->Add(h_J1Pt_antitagSR_T5HH2200,-1); h_H_j1pt_T5HH2200->Add(h_J1Pt_antitagSB_T5HH2200,-1);
      h_H_j2pt_T5HH2200 = (TH1F*)h_baseline_j2pt_T5HH2200->Clone("h_H_j2pt_T5HH2200"); h_H_j2pt_T5HH2200->Add(h_J2Pt_antitagSR_T5HH2200,-1); h_H_j2pt_T5HH2200->Add(h_J2Pt_antitagSB_T5HH2200,-1);
      h_baseline_j1bb_T5HH2200 = (TH1F*)fSignal2->Get("LeadDeepBBTag_baseline_T5HH2200_LSP1"); h_baseline_j2bb_T5HH2200 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_baseline_T5HH2200_LSP1");
      h_J1BB_antitagSR_T5HH2200 = (TH1F*)fSignal2->Get("LeadDeepBBTag_antitagSR_T5HH2200_LSP1"); h_J2BB_antitagSR_T5HH2200 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_antitagSR_T5HH2200_LSP1");
      h_J1BB_tagSR_T5HH2200 = (TH1F*)fSignal2->Get("LeadDeepBBTag_tagSR_T5HH2200_LSP1"); h_J2BB_tagSR_T5HH2200 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_tagSR_T5HH2200_LSP1");
      h_J1BB_doubletagSR_T5HH2200 = (TH1F*)fSignal2->Get("LeadDeepBBTag_doubletagSR_T5HH2200_LSP1"); h_J2BB_doubletagSR_T5HH2200 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_doubletagSR_T5HH2200_LSP1");
      h_J1BB_antitagSB_T5HH2200 = (TH1F*)fSignal2->Get("LeadDeepBBTag_antitagSB_T5HH2200_LSP1"); h_J2BB_antitagSB_T5HH2200 = (TH1F*)fSignal2->Get("SubLeadDeepBBTag_antitagSB_T5HH2200_LSP1");
      h_H_j1bb_T5HH2200 = (TH1F*)h_baseline_j1bb_T5HH2200->Clone("h_H_j1bb_T5HH2200"); h_H_j1bb_T5HH2200->Add(h_J1BB_antitagSR_T5HH2200,-1); h_H_j1bb_T5HH2200->Add(h_J1BB_antitagSB_T5HH2200,-1);
      h_H_j2bb_T5HH2200 = (TH1F*)h_baseline_j2bb_T5HH2200->Clone("h_H_j2bb_T5HH2200"); h_H_j2bb_T5HH2200->Add(h_J2BB_antitagSR_T5HH2200,-1); h_H_j2bb_T5HH2200->Add(h_J2BB_antitagSB_T5HH2200,-1);

      h_J1Pt_doubletagSR_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_doubletagSR_TChiHH500_LSP1"); h_J2Pt_doubletagSR_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_doubletagSR_TChiHH500_LSP1");
      h_J1Pt_doubletagSB_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_doubletagSB_TChiHH500_LSP1"); h_J2Pt_doubletagSB_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_doubletagSB_TChiHH500_LSP1");
      h_J1Pt_antitagSR_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_antitagSR_TChiHH500_LSP1"); h_J2Pt_antitagSR_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_antitagSR_TChiHH500_LSP1");
      h_J1Pt_antitagSB_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_antitagSB_TChiHH500_LSP1"); h_J2Pt_antitagSB_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_antitagSB_TChiHH500_LSP1");
      h_J1Pt_tagSR_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_tagSR_TChiHH500_LSP1"); h_J2Pt_tagSR_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_tagSR_TChiHH500_LSP1");
      h_J1Pt_tagSB_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_tagSB_TChiHH500_LSP1"); h_J2Pt_tagSB_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_tagSB_TChiHH500_LSP1");
      h_baseline_j1pt_TChiHH500 = (TH1F*)fSignal->Get("J1pt_Pt_baseline_TChiHH500_LSP1"); h_baseline_j2pt_TChiHH500 = (TH1F*)fSignal->Get("J2pt_Pt_baseline_TChiHH500_LSP1");
      h_H_j1pt_TChiHH500 = (TH1F*)h_baseline_j1pt_TChiHH500->Clone("h_H_j1pt_TChiHH500"); h_H_j1pt_TChiHH500->Add(h_J1Pt_antitagSR_TChiHH500,-1); h_H_j1pt_TChiHH500->Add(h_J1Pt_antitagSB_TChiHH500,-1);
      h_H_j2pt_TChiHH500 = (TH1F*)h_baseline_j2pt_TChiHH500->Clone("h_H_j2pt_TChiHH500"); h_H_j2pt_TChiHH500->Add(h_J2Pt_antitagSR_TChiHH500,-1); h_H_j2pt_TChiHH500->Add(h_J2Pt_antitagSB_TChiHH500,-1);
      h_0H_j1pt_TChiHH500 = (TH1F*)h_J1Pt_antitagSR_TChiHH500->Clone("h_0H_j1pt_TChiHH500"); h_0H_j1pt_TChiHH500->Add(h_J1Pt_antitagSB_TChiHH500);
      h_0H_j2pt_TChiHH500 = (TH1F*)h_J2Pt_antitagSR_TChiHH500->Clone("h_0H_j2pt_TChiHH500"); h_0H_j2pt_TChiHH500->Add(h_J2Pt_antitagSB_TChiHH500);
      h_1H_j1pt_TChiHH500 = (TH1F*)h_J1Pt_tagSR_TChiHH500->Clone("h_1H_j1pt_TChiHH500"); h_1H_j1pt_TChiHH500->Add(h_J1Pt_tagSB_TChiHH500);
      h_1H_j2pt_TChiHH500 = (TH1F*)h_J2Pt_tagSR_TChiHH500->Clone("h_1H_j2pt_TChiHH500"); h_1H_j2pt_TChiHH500->Add(h_J2Pt_tagSB_TChiHH500);
      h_2H_j1pt_TChiHH500 = (TH1F*)h_J1Pt_doubletagSR_TChiHH500->Clone("h_2H_j1pt_TChiHH500"); h_2H_j1pt_TChiHH500->Add(h_J1Pt_doubletagSB_TChiHH500);
      h_2H_j2pt_TChiHH500 = (TH1F*)h_J2Pt_doubletagSR_TChiHH500->Clone("h_2H_j2pt_TChiHH500"); h_2H_j2pt_TChiHH500->Add(h_J2Pt_doubletagSB_TChiHH500);

      h_baseline_jpt_TChiHH500 = (TH1F*)h_baseline_j1pt_TChiHH500->Clone("h_baseline_jpt_TChiHH500");h_baseline_jpt_TChiHH500->Add(h_baseline_j2pt_TChiHH500);
      h_0H_jpt_TChiHH500 = (TH1F*)h_0H_j1pt_TChiHH500->Clone("h_0H_jpt_TChiHH500");h_0H_jpt_TChiHH500->Add(h_0H_j2pt_TChiHH500);
      h_1H_jpt_TChiHH500 = (TH1F*)h_1H_j1pt_TChiHH500->Clone("h_1H_jpt_TChiHH500");h_1H_jpt_TChiHH500->Add(h_1H_j2pt_TChiHH500);
      h_2H_jpt_TChiHH500 = (TH1F*)h_2H_j1pt_TChiHH500->Clone("h_2H_jpt_TChiHH500");h_2H_jpt_TChiHH500->Add(h_2H_j2pt_TChiHH500);


      std::cout<<"Here? 23"<<std::endl;

      h_baseline_j1bb_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_baseline_TChiHH500_LSP1"); h_baseline_j2bb_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_baseline_TChiHH500_LSP1");
      h_J1BB_antitagSR_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_antitagSR_TChiHH500_LSP1"); h_J2BB_antitagSR_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_antitagSR_TChiHH500_LSP1");
      h_J1BB_antitagSB_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_antitagSB_TChiHH500_LSP1"); h_J2BB_antitagSB_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_antitagSB_TChiHH500_LSP1");
      h_J1BB_tagSR_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_tagSR_TChiHH500_LSP1"); h_J2BB_tagSR_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_tagSR_TChiHH500_LSP1");
      h_J1BB_tagSB_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_tagSB_TChiHH500_LSP1"); h_J2BB_tagSB_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_tagSB_TChiHH500_LSP1");
      h_J1BB_doubletagSR_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_doubletagSR_TChiHH500_LSP1"); h_J2BB_doubletagSR_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_doubletagSR_TChiHH500_LSP1");
      h_J1BB_doubletagSB_TChiHH500 = (TH1F*)fSignal->Get("LeadDeepBBTag_doubletagSB_TChiHH500_LSP1"); h_J2BB_doubletagSB_TChiHH500 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_doubletagSB_TChiHH500_LSP1");
      h_H_j1bb_TChiHH500 = (TH1F*)h_baseline_j1bb_TChiHH500->Clone("h_H_j1bb_TChiHH500"); h_H_j1bb_TChiHH500->Add(h_J1BB_antitagSR_TChiHH500,-1); h_H_j1bb_TChiHH500->Add(h_J1BB_antitagSB_TChiHH500,-1);
      h_H_j2bb_TChiHH500 = (TH1F*)h_baseline_j2bb_TChiHH500->Clone("h_H_j2bb_TChiHH500"); h_H_j2bb_TChiHH500->Add(h_J2BB_antitagSR_TChiHH500,-1); h_H_j2bb_TChiHH500->Add(h_J2BB_antitagSB_TChiHH500,-1);
      h_0H_j1bb_TChiHH500 = (TH1F*)h_J1BB_antitagSR_TChiHH500->Clone("h_0H_j1bb_TChiHH500"); h_0H_j1bb_TChiHH500->Add(h_J1BB_antitagSB_TChiHH500);
      h_0H_j2bb_TChiHH500 = (TH1F*)h_J2BB_antitagSR_TChiHH500->Clone("h_0H_j2bb_TChiHH500"); h_0H_j2bb_TChiHH500->Add(h_J2BB_antitagSB_TChiHH500);
      h_1H_j1bb_TChiHH500 = (TH1F*)h_J1BB_tagSR_TChiHH500->Clone("h_1H_j1bb_TChiHH500"); h_1H_j1bb_TChiHH500->Add(h_J1BB_tagSB_TChiHH500);
      h_1H_j2bb_TChiHH500 = (TH1F*)h_J2BB_tagSR_TChiHH500->Clone("h_1H_j2bb_TChiHH500"); h_1H_j2bb_TChiHH500->Add(h_J2BB_tagSB_TChiHH500);
      h_2H_j1bb_TChiHH500 = (TH1F*)h_J1BB_doubletagSR_TChiHH500->Clone("h_2H_j1bb_TChiHH500"); h_2H_j1bb_TChiHH500->Add(h_J1BB_doubletagSB_TChiHH500);
      h_2H_j2bb_TChiHH500 = (TH1F*)h_J2BB_doubletagSR_TChiHH500->Clone("h_2H_j2bb_TChiHH500"); h_2H_j2bb_TChiHH500->Add(h_J2BB_doubletagSB_TChiHH500);

      h_baseline_jbb_TChiHH500 = (TH1F*)h_baseline_j1bb_TChiHH500->Clone("h_baseline_jbb_TChiHH500");h_baseline_jbb_TChiHH500->Add(h_baseline_j2bb_TChiHH500);
      h_0H_jbb_TChiHH500 = (TH1F*)h_0H_j1bb_TChiHH500->Clone("h_0H_jbb_TChiHH500");h_0H_jbb_TChiHH500->Add(h_0H_j2bb_TChiHH500);
      h_1H_jbb_TChiHH500 = (TH1F*)h_1H_j1bb_TChiHH500->Clone("h_1H_jbb_TChiHH500");h_1H_jbb_TChiHH500->Add(h_1H_j2bb_TChiHH500);
      h_2H_jbb_TChiHH500 = (TH1F*)h_2H_j1bb_TChiHH500->Clone("h_2H_jbb_TChiHH500");h_2H_jbb_TChiHH500->Add(h_2H_j2bb_TChiHH500);


      h_J1Pt_doubletagSR_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_doubletagSR_TChiHH800_LSP1"); h_J2Pt_doubletagSR_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_doubletagSR_TChiHH800_LSP1");
      h_J1Pt_doubletagSB_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_doubletagSB_TChiHH800_LSP1"); h_J2Pt_doubletagSB_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_doubletagSB_TChiHH800_LSP1");
      h_J1Pt_antitagSR_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_antitagSR_TChiHH800_LSP1"); h_J2Pt_antitagSR_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_antitagSR_TChiHH800_LSP1");
      h_J1Pt_antitagSB_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_antitagSB_TChiHH800_LSP1"); h_J2Pt_antitagSB_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_antitagSB_TChiHH800_LSP1");
      h_J1Pt_tagSR_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_tagSR_TChiHH800_LSP1"); h_J2Pt_tagSR_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_tagSR_TChiHH800_LSP1");
      h_J1Pt_tagSB_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_tagSB_TChiHH800_LSP1"); h_J2Pt_tagSB_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_tagSB_TChiHH800_LSP1");
      h_baseline_j1pt_TChiHH800 = (TH1F*)fSignal->Get("J1pt_Pt_baseline_TChiHH800_LSP1"); h_baseline_j2pt_TChiHH800 = (TH1F*)fSignal->Get("J2pt_Pt_baseline_TChiHH800_LSP1");

      h_H_j1pt_TChiHH800 = (TH1F*)h_baseline_j1pt_TChiHH800->Clone("h_H_j1pt_TChiHH800"); h_H_j1pt_TChiHH800->Add(h_J1Pt_antitagSR_TChiHH800,-1); h_H_j1pt_TChiHH800->Add(h_J1Pt_antitagSB_TChiHH800,-1);
      h_H_j2pt_TChiHH800 = (TH1F*)h_baseline_j2pt_TChiHH800->Clone("h_H_j2pt_TChiHH800"); h_H_j2pt_TChiHH800->Add(h_J2Pt_antitagSR_TChiHH800,-1); h_H_j2pt_TChiHH800->Add(h_J2Pt_antitagSB_TChiHH800,-1);
      h_baseline_j1bb_TChiHH800 = (TH1F*)fSignal->Get("LeadDeepBBTag_baseline_TChiHH800_LSP1"); h_baseline_j2bb_TChiHH800 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_baseline_TChiHH800_LSP1");
      h_J1BB_antitagSR_TChiHH800 = (TH1F*)fSignal->Get("LeadDeepBBTag_antitagSR_TChiHH800_LSP1"); h_J2BB_antitagSR_TChiHH800 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_antitagSR_TChiHH800_LSP1");
      h_J1BB_tagSR_TChiHH800 = (TH1F*)fSignal->Get("LeadDeepBBTag_tagSR_TChiHH800_LSP1"); h_J2BB_tagSR_TChiHH800 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_tagSR_TChiHH800_LSP1");
      h_J1BB_doubletagSR_TChiHH800 = (TH1F*)fSignal->Get("LeadDeepBBTag_doubletagSR_TChiHH800_LSP1"); h_J2BB_doubletagSR_TChiHH800 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_doubletagSR_TChiHH800_LSP1");
      h_J1BB_antitagSB_TChiHH800 = (TH1F*)fSignal->Get("LeadDeepBBTag_antitagSB_TChiHH800_LSP1"); h_J2BB_antitagSB_TChiHH800 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_antitagSB_TChiHH800_LSP1");
      h_H_j1bb_TChiHH800 = (TH1F*)h_baseline_j1bb_TChiHH800->Clone("h_H_j1bb_TChiHH800"); h_H_j1bb_TChiHH800->Add(h_J1BB_antitagSR_TChiHH800,-1); h_H_j1bb_TChiHH800->Add(h_J1BB_antitagSB_TChiHH800,-1);
      h_H_j2bb_TChiHH800 = (TH1F*)h_baseline_j2bb_TChiHH800->Clone("h_H_j2bb_TChiHH800"); h_H_j2bb_TChiHH800->Add(h_J2BB_antitagSR_TChiHH800,-1); h_H_j2bb_TChiHH800->Add(h_J2BB_antitagSB_TChiHH800,-1);


      h_J1Pt_doubletagSR_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_doubletagSR_TChiHH1000_LSP1"); h_J2Pt_doubletagSR_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_doubletagSR_TChiHH1000_LSP1");
      h_J1Pt_doubletagSB_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_doubletagSB_TChiHH1000_LSP1"); h_J2Pt_doubletagSB_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_doubletagSB_TChiHH1000_LSP1");
      h_J1Pt_antitagSR_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_antitagSR_TChiHH1000_LSP1"); h_J2Pt_antitagSR_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_antitagSR_TChiHH1000_LSP1");
      h_J1Pt_antitagSB_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_antitagSB_TChiHH1000_LSP1"); h_J2Pt_antitagSB_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_antitagSB_TChiHH1000_LSP1");
      h_J1Pt_tagSR_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_tagSR_TChiHH1000_LSP1"); h_J2Pt_tagSR_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_tagSR_TChiHH1000_LSP1");
      h_J1Pt_tagSB_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_tagSB_TChiHH1000_LSP1"); h_J2Pt_tagSB_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_tagSB_TChiHH1000_LSP1");
      h_baseline_j1pt_TChiHH1000 = (TH1F*)fSignal->Get("J1pt_Pt_baseline_TChiHH1000_LSP1"); h_baseline_j2pt_TChiHH1000 = (TH1F*)fSignal->Get("J2pt_Pt_baseline_TChiHH1000_LSP1");

      h_H_j1pt_TChiHH1000 = (TH1F*)h_baseline_j1pt_TChiHH1000->Clone("h_H_j1pt_TChiHH1000"); h_H_j1pt_TChiHH1000->Add(h_J1Pt_antitagSR_TChiHH1000,-1); h_H_j1pt_TChiHH1000->Add(h_J1Pt_antitagSB_TChiHH1000,-1);
      h_H_j2pt_TChiHH1000 = (TH1F*)h_baseline_j2pt_TChiHH1000->Clone("h_H_j2pt_TChiHH1000"); h_H_j2pt_TChiHH1000->Add(h_J2Pt_antitagSR_TChiHH1000,-1); h_H_j2pt_TChiHH1000->Add(h_J2Pt_antitagSB_TChiHH1000,-1);
      h_baseline_j1bb_TChiHH1000 = (TH1F*)fSignal->Get("LeadDeepBBTag_baseline_TChiHH1000_LSP1"); h_baseline_j2bb_TChiHH1000 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_baseline_TChiHH1000_LSP1");
      h_J1BB_antitagSR_TChiHH1000 = (TH1F*)fSignal->Get("LeadDeepBBTag_antitagSR_TChiHH1000_LSP1"); h_J2BB_antitagSR_TChiHH1000 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_antitagSR_TChiHH1000_LSP1");
      h_J1BB_tagSR_TChiHH1000 = (TH1F*)fSignal->Get("LeadDeepBBTag_tagSR_TChiHH1000_LSP1"); h_J2BB_tagSR_TChiHH1000 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_tagSR_TChiHH1000_LSP1");
      h_J1BB_doubletagSR_TChiHH1000 = (TH1F*)fSignal->Get("LeadDeepBBTag_doubletagSR_TChiHH1000_LSP1"); h_J2BB_doubletagSR_TChiHH1000 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_doubletagSR_TChiHH1000_LSP1");
      h_J1BB_antitagSB_TChiHH1000 = (TH1F*)fSignal->Get("LeadDeepBBTag_antitagSB_TChiHH1000_LSP1"); h_J2BB_antitagSB_TChiHH1000 = (TH1F*)fSignal->Get("SubLeadDeepBBTag_antitagSB_TChiHH1000_LSP1");
      h_H_j1bb_TChiHH1000 = (TH1F*)h_baseline_j1bb_TChiHH1000->Clone("h_H_j1bb_TChiHH1000"); h_H_j1bb_TChiHH1000->Add(h_J1BB_antitagSR_TChiHH1000,-1); h_H_j1bb_TChiHH1000->Add(h_J1BB_antitagSB_TChiHH1000,-1);
      h_H_j2bb_TChiHH1000 = (TH1F*)h_baseline_j2bb_TChiHH1000->Clone("h_H_j2bb_TChiHH1000"); h_H_j2bb_TChiHH1000->Add(h_J2BB_antitagSR_TChiHH1000,-1); h_H_j2bb_TChiHH1000->Add(h_J2BB_antitagSB_TChiHH1000,-1);

      std::cout<<"Here? 24"<<std::endl;

    }

  }
  if (whichRegion=="photon") {
    h_A_data = (TH1F*)fPhoton->Get("MET_doubletagSR_data"); h_B_data = (TH1F*)fPhoton->Get("MET_doubletagSB_data");
    h_A1_data = (TH1F*)fPhoton->Get("MET_tagSR_data"); h_B1_data = (TH1F*)fPhoton->Get("MET_tagSB_data");
    h_C_data = (TH1F*)fPhoton->Get("MET_antitagSR_data"); h_D_data = (TH1F*)fPhoton->Get("MET_antitagSB_data");
    h_Opt1_data = (TH1F*)fPhoton->Get("MET_antitagSROpt1_data");

    h_baseline_MET_data = (TH1F*)fPhoton->Get("MET_baseline_data");
    h_baseline_MET_sum = (TH1F*)fPhoton->Get("MET_baseline_sum");

    // h_A_sum = (TH1F*)fPhoton->Get("MET_doubletagSR_sum"); h_B_sum = (TH1F*)fPhoton->Get("MET_doubletagSB_sum");
    // h_A1_sum = (TH1F*)fPhoton->Get("MET_tagSR_sum"); h_B1_sum = (TH1F*)fPhoton->Get("MET_tagSB_sum");
    // h_C_sum = (TH1F*)fPhoton->Get("MET_antitagSR_sum"); h_D_sum = (TH1F*)fPhoton->Get("MET_antitagSB_sum");

    // hP_A_sum = (TH1F*)fPhoton->Get("METPhoton_doubletagSR_sum"); hP_B_sum = (TH1F*)fPhoton->Get("METPhoton_doubletagSB_sum");
    // hP_A1_sum = (TH1F*)fPhoton->Get("METPhoton_tagSR_sum"); hP_B1_sum = (TH1F*)fPhoton->Get("METPhoton_tagSB_sum");
    // hP_C_sum = (TH1F*)fPhoton->Get("METPhoton_antitagSR_sum"); hP_D_sum = (TH1F*)fPhoton->Get("METPhoton_antitagSB_sum");

    h_A_QCD = (TH1F*)fPhoton->Get("MET_doubletagSR_QCD"); h_B_QCD = (TH1F*)fPhoton->Get("MET_doubletagSB_QCD");
    h_A1_QCD = (TH1F*)fPhoton->Get("MET_tagSR_QCD"); h_B1_QCD = (TH1F*)fPhoton->Get("MET_tagSB_QCD");
    h_C_QCD = (TH1F*)fPhoton->Get("MET_antitagSR_QCD"); h_D_QCD = (TH1F*)fPhoton->Get("MET_antitagSB_QCD");
    h_Opt1_QCD = (TH1F*)fPhoton->Get("MET_antitagOpt1_QCD");
    h_baseline_MET_QCD = (TH1F*)fPhoton->Get("MET_baseline_QCD");

    h_A_GJets = (TH1F*)fPhoton->Get("MET_doubletagSR_GJets"); h_B_GJets = (TH1F*)fPhoton->Get("MET_doubletagSB_GJets");
    h_A1_GJets = (TH1F*)fPhoton->Get("MET_tagSR_GJets"); h_B1_GJets = (TH1F*)fPhoton->Get("MET_tagSB_GJets");
    h_C_GJets = (TH1F*)fPhoton->Get("MET_antitagSR_GJets"); h_D_GJets = (TH1F*)fPhoton->Get("MET_antitagSB_GJets");
    h_Opt1_GJets = (TH1F*)fPhoton->Get("MET_antitagOpt1_GJets");
    h_baseline_MET_GJets = (TH1F*)fPhoton->Get("MET_baseline_GJets");

    hP_A_QCD = (TH1F*)fPhoton->Get("METPhoton_doubletagSR_QCD"); hP_B_QCD = (TH1F*)fPhoton->Get("METPhoton_doubletagSB_QCD");
    hP_A1_QCD = (TH1F*)fPhoton->Get("METPhoton_tagSR_QCD"); hP_B1_QCD = (TH1F*)fPhoton->Get("METPhoton_tagSB_QCD");
    hP_C_QCD = (TH1F*)fPhoton->Get("METPhoton_antitagSR_QCD"); hP_D_QCD = (TH1F*)fPhoton->Get("METPhoton_antitagSB_QCD");
    hP_Opt1_QCD = (TH1F*)fPhoton->Get("METPhoton_antitagOpt1_QCD");

    hP_A_GJets = (TH1F*)fPhoton->Get("METPhoton_doubletagSR_GJets"); hP_B_GJets = (TH1F*)fPhoton->Get("METPhoton_doubletagSB_GJets");
    hP_A1_GJets = (TH1F*)fPhoton->Get("METPhoton_tagSR_GJets"); hP_B1_GJets = (TH1F*)fPhoton->Get("METPhoton_tagSB_GJets");
    hP_C_GJets = (TH1F*)fPhoton->Get("METPhoton_antitagSR_GJets"); hP_D_GJets = (TH1F*)fPhoton->Get("METPhoton_antitagSB_GJets");
    hP_Opt1_GJets = (TH1F*)fPhoton->Get("METPhoton_antitagOpt1_GJets");


    h_A_sum = make0EventUncSum_photon({h_A_QCD,h_A_GJets}); h_B_sum = make0EventUncSum_photon({h_B_QCD,h_B_GJets});
    h_A1_sum = make0EventUncSum_photon({h_A1_QCD,h_A1_GJets}); h_B1_sum = make0EventUncSum_photon({h_B1_QCD,h_B1_GJets});
    h_C_sum = make0EventUncSum_photon({h_C_QCD,h_C_GJets}); h_D_sum = make0EventUncSum_photon({h_D_QCD,h_D_GJets});
    h_Opt1_sum = make0EventUncSum_photon({h_Opt1_QCD,h_Opt1_GJets});

    hP_A_sum = make0EventUncSum_photon({hP_A_QCD,hP_A_GJets}); hP_B_sum = make0EventUncSum_photon({hP_B_QCD,hP_B_GJets});
    hP_A1_sum = make0EventUncSum_photon({hP_A1_QCD,hP_A1_GJets}); hP_B1_sum = make0EventUncSum_photon({hP_B1_QCD,hP_B1_GJets});
    hP_C_sum = make0EventUncSum_photon({hP_C_QCD,hP_C_GJets}); hP_D_sum = make0EventUncSum_photon({hP_D_QCD,hP_D_GJets});
    hP_Opt1_sum = make0EventUncSum_photon({hP_Opt1_QCD,hP_Opt1_GJets});


    a_int = h_A_sum->IntegralAndError(1,5,a_error,""); a1_int = h_A1_sum->IntegralAndError(1,5,a1_error,"");
    b_int = h_B_sum->IntegralAndError(1,5,b_error,""); b1_int = h_B1_sum->IntegralAndError(1,5,b1_error,"");
    c_int = h_C_sum->IntegralAndError(1,5,c_error,""); d_int = h_D_sum->IntegralAndError(1,5,d_error,"");

    h_nH_sum->SetBinContent(1,c_int+d_int);   h_nH_sum->SetBinError(1,sqrt(c_error*c_error + d_error*d_error));
    h_nH_sum->SetBinContent(2,a1_int+b1_int); h_nH_sum->SetBinError(2,sqrt(a1_error*a1_error + b1_error*b1_error));
    h_nH_sum->SetBinContent(3,a_int+b_int);   h_nH_sum->SetBinError(3,sqrt(a_error*a_error + b_error*b_error));

    a_int_data = h_A_data->IntegralAndError(1,5,a_error_data,""); a1_int_data = h_A1_data->IntegralAndError(1,5,a1_error_data,"");
    b_int_data = h_B_data->IntegralAndError(1,5,b_error_data,""); b1_int_data = h_B1_data->IntegralAndError(1,5,b1_error_data,"");
    c_int_data = h_C_data->IntegralAndError(1,5,c_error_data,""); d_int_data = h_D_data->IntegralAndError(1,5,d_error_data,"");

    h_nH_data->SetBinContent(1,c_int_data+d_int_data);   h_nH_data->SetBinError(1,sqrt(c_error_data*c_error_data + d_error_data*d_error_data));
    h_nH_data->SetBinContent(2,a1_int_data+b1_int_data); h_nH_data->SetBinError(2,sqrt(a1_error_data*a1_error_data + b1_error_data*b1_error_data));
    h_nH_data->SetBinContent(3,a_int_data+b_int_data);   h_nH_data->SetBinError(3,sqrt(a_error_data*a_error_data + b_error_data*b_error_data));

    //For just GJets
    // a_int = h_A_GJets->IntegralAndError(1,5,a_error,""); a1_int = h_A1_GJets->IntegralAndError(1,5,a1_error,"");
    // b_int = h_B_GJets->IntegralAndError(1,5,b_error,""); b1_int = h_B1_GJets->IntegralAndError(1,5,b1_error,"");
    // c_int = h_C_GJets->IntegralAndError(1,5,c_error,""); d_int = h_D_GJets->IntegralAndError(1,5,d_error,"");
    //
    // h_nH_sum->SetBinContent(1,c_int+d_int);   h_nH_sum->SetBinError(1,sqrt(c_error*c_error + d_error*d_error));
    // h_nH_sum->SetBinContent(2,a1_int+b1_int); h_nH_sum->SetBinError(2,sqrt(a1_error*a1_error + b1_error*b1_error));
    // h_nH_sum->SetBinContent(3,a_int+b_int);   h_nH_sum->SetBinError(3,sqrt(a_error*a_error + b_error*b_error));
    // std::cout<<"_______________________________________________\n";
    // std::cout<<"Content and error for MC: ";
    // std::cout<<h_nH_sum->GetBinContent(1)<<"+/-"<<h_nH_sum->GetBinError(1)<<", "<<h_nH_sum->GetBinContent(2)<<"+/-"<<h_nH_sum->GetBinError(2)<<", "<<h_nH_sum->GetBinContent(3)<<"+/-"<<h_nH_sum->GetBinError(3)<<std::endl;
    //
    // a_int_data = h_A_data->IntegralAndError(1,5,a_error_data,""); a1_int_data = h_A1_data->IntegralAndError(1,5,a1_error_data,"");
    // b_int_data = h_B_data->IntegralAndError(1,5,b_error_data,""); b1_int_data = h_B1_data->IntegralAndError(1,5,b1_error_data,"");
    // c_int_data = h_C_data->IntegralAndError(1,5,c_error_data,""); d_int_data = h_D_data->IntegralAndError(1,5,d_error_data,"");
    //
    // h_nH_data->SetBinContent(1,c_int_data+d_int_data);   h_nH_data->SetBinError(1,sqrt(c_error_data*c_error_data + d_error_data*d_error_data));
    // h_nH_data->SetBinContent(2,a1_int_data+b1_int_data); h_nH_data->SetBinError(2,sqrt(a1_error_data*a1_error_data + b1_error_data*b1_error_data));
    // h_nH_data->SetBinContent(3,a_int_data+b_int_data);   h_nH_data->SetBinError(3,sqrt(a_error_data*a_error_data + b_error_data*b_error_data));
    //
    // std::cout<<"\nContent and error for data: ";
    // std::cout<<h_nH_data->GetBinContent(1)<<"+/-"<<h_nH_data->GetBinError(1)<<", "<<h_nH_data->GetBinContent(2)<<"+/-"<<h_nH_data->GetBinError(2)<<", "<<h_nH_data->GetBinContent(3)<<"+/-"<<h_nH_data->GetBinError(3)<<std::endl;
    // std::cout<<"_______________________________________________\n";

    h_J1M_doubletagSR_GJets = (TH1F*)fPhoton->Get("J1pt_M_doubletagSR_GJets"); h_J2M_doubletagSR_GJets = (TH1F*)fPhoton->Get("J2pt_M_doubletagSR_GJets");
    h_J1M_doubletagSB_GJets = (TH1F*)fPhoton->Get("J1pt_M_doubletagSB_GJets"); h_J2M_doubletagSB_GJets = (TH1F*)fPhoton->Get("J2pt_M_doubletagSB_GJets");
    h_J1M_antitagSR_GJets = (TH1F*)fPhoton->Get("J1pt_M_antitagSR_GJets"); h_J2M_antitagSR_GJets = (TH1F*)fPhoton->Get("J2pt_M_antitagSR_GJets");
    h_J1M_antitagSB_GJets = (TH1F*)fPhoton->Get("J1pt_M_antitagSB_GJets"); h_J2M_antitagSB_GJets = (TH1F*)fPhoton->Get("J2pt_M_antitagSB_GJets");
    h_J1M_tagSR_GJets = (TH1F*)fPhoton->Get("J1pt_M_tagSR_GJets"); h_J2M_tagSR_GJets = (TH1F*)fPhoton->Get("J2pt_M_tagSR_GJets");
    h_J1M_tagSB_GJets = (TH1F*)fPhoton->Get("J1pt_M_tagSB_GJets"); h_J2M_tagSB_GJets = (TH1F*)fPhoton->Get("J2pt_M_tagSB_GJets");
    h_baseline_j1mass_GJets = (TH1F*)fPhoton->Get("J1pt_M_baseline_GJets"); h_baseline_j2mass_GJets = (TH1F*)fPhoton->Get("J2pt_M_baseline_GJets");

    h_J1M_doubletagSR_QCD = (TH1F*)fPhoton->Get("J1pt_M_doubletagSR_QCD"); h_J2M_doubletagSR_QCD = (TH1F*)fPhoton->Get("J2pt_M_doubletagSR_QCD");
    h_J1M_doubletagSB_QCD = (TH1F*)fPhoton->Get("J1pt_M_doubletagSB_QCD"); h_J2M_doubletagSB_QCD = (TH1F*)fPhoton->Get("J2pt_M_doubletagSB_QCD");
    h_J1M_antitagSR_QCD = (TH1F*)fPhoton->Get("J1pt_M_antitagSR_QCD"); h_J2M_antitagSR_QCD = (TH1F*)fPhoton->Get("J2pt_M_antitagSR_QCD");
    h_J1M_antitagSB_QCD = (TH1F*)fPhoton->Get("J1pt_M_antitagSB_QCD"); h_J2M_antitagSB_QCD = (TH1F*)fPhoton->Get("J2pt_M_antitagSB_QCD");
    h_J1M_tagSR_QCD = (TH1F*)fPhoton->Get("J1pt_M_tagSR_QCD"); h_J2M_tagSR_QCD = (TH1F*)fPhoton->Get("J2pt_M_tagSR_QCD");
    h_J1M_tagSB_QCD = (TH1F*)fPhoton->Get("J1pt_M_tagSB_QCD"); h_J2M_tagSB_QCD = (TH1F*)fPhoton->Get("J2pt_M_tagSB_QCD");
    h_baseline_j1mass_QCD = (TH1F*)fPhoton->Get("J1pt_M_baseline_QCD"); h_baseline_j2mass_QCD = (TH1F*)fPhoton->Get("J2pt_M_baseline_QCD");
    h_baseline_j1mass_sum = (TH1F*)fPhoton->Get("J1pt_M_baseline_sum"); h_baseline_j2mass_sum = (TH1F*)fPhoton->Get("J2pt_M_baseline_sum");
    h_baseline_j1mass_data = (TH1F*)fPhoton->Get("J1pt_M_baseline_data"); h_baseline_j2mass_data = (TH1F*)fPhoton->Get("J2pt_M_baseline_data");

    h_J1M_doubletagSR_sum = (TH1F*)fPhoton->Get("J1pt_M_doubletagSR_sum"); h_J2M_doubletagSR_sum = (TH1F*)fPhoton->Get("J2pt_M_doubletagSR_sum");
    h_J1M_doubletagSB_sum = (TH1F*)fPhoton->Get("J1pt_M_doubletagSB_sum"); h_J2M_doubletagSB_sum = (TH1F*)fPhoton->Get("J2pt_M_doubletagSB_sum");
    h_J1M_antitagSR_sum = (TH1F*)fPhoton->Get("J1pt_M_antitagSR_sum"); h_J2M_antitagSR_sum = (TH1F*)fPhoton->Get("J2pt_M_antitagSR_sum");
    h_J1M_antitagSB_sum = (TH1F*)fPhoton->Get("J1pt_M_antitagSB_sum"); h_J2M_antitagSB_sum = (TH1F*)fPhoton->Get("J2pt_M_antitagSB_sum");
    h_J1M_tagSR_sum = (TH1F*)fPhoton->Get("J1pt_M_tagSR_sum"); h_J2M_tagSR_sum = (TH1F*)fPhoton->Get("J2pt_M_tagSR_sum");
    h_J1M_tagSB_sum = (TH1F*)fPhoton->Get("J1pt_M_tagSB_sum"); h_J2M_tagSB_sum = (TH1F*)fPhoton->Get("J2pt_M_tagSB_sum");

    h_J1J2M_doubletagSR_sum = (TH1F*)h_J1M_doubletagSR_sum->Clone("J1J2M_doubletagSR_sum"); h_J1J2M_doubletagSR_sum->Add(h_J2M_doubletagSR_sum);
    h_J1J2M_doubletagSB_sum = (TH1F*)h_J1M_doubletagSB_sum->Clone("J1J2M_doubletagSB_sum"); h_J1J2M_doubletagSB_sum->Add(h_J2M_doubletagSB_sum);
    h_J1J2M_tagSR_sum = (TH1F*)h_J1M_tagSR_sum->Clone("J1J2M_tagSR_sum"); h_J1J2M_tagSR_sum->Add(h_J2M_tagSR_sum);
    h_J1J2M_tagSB_sum = (TH1F*)h_J1M_tagSB_sum->Clone("J1J2M_tagSB_sum"); h_J1J2M_tagSB_sum->Add(h_J2M_tagSB_sum);
    h_J1J2M_antitagSR_sum = (TH1F*)h_J1M_antitagSR_sum->Clone("J1J2M_antitagSR_sum"); h_J1J2M_antitagSR_sum->Add(h_J2M_antitagSR_sum);
    h_J1J2M_antitagSB_sum = (TH1F*)h_J1M_antitagSB_sum->Clone("J1J2M_antitagSB_sum"); h_J1J2M_antitagSB_sum->Add(h_J2M_antitagSB_sum);

    h_J1Pt_doubletagSR_sum = (TH1F*)fPhoton->Get("J1pt_Pt_doubletagSR_sum"); h_J2Pt_doubletagSR_sum = (TH1F*)fPhoton->Get("J2pt_Pt_doubletagSR_sum");
    h_J1Pt_doubletagSB_sum = (TH1F*)fPhoton->Get("J1pt_Pt_doubletagSB_sum"); h_J2Pt_doubletagSB_sum = (TH1F*)fPhoton->Get("J2pt_Pt_doubletagSB_sum");
    h_J1Pt_antitagSR_sum = (TH1F*)fPhoton->Get("J1pt_Pt_antitagSR_sum"); h_J2Pt_antitagSR_sum = (TH1F*)fPhoton->Get("J2pt_Pt_antitagSR_sum");
    h_J1Pt_antitagSB_sum = (TH1F*)fPhoton->Get("J1pt_Pt_antitagSB_sum"); h_J2Pt_antitagSB_sum = (TH1F*)fPhoton->Get("J2pt_Pt_antitagSB_sum");
    h_J1Pt_tagSR_sum = (TH1F*)fPhoton->Get("J1pt_Pt_tagSR_sum"); h_J2Pt_tagSR_sum = (TH1F*)fPhoton->Get("J2pt_Pt_tagSR_sum");
    h_J1Pt_tagSB_sum = (TH1F*)fPhoton->Get("J1pt_Pt_tagSB_sum"); h_J2Pt_tagSB_sum = (TH1F*)fPhoton->Get("J2pt_Pt_tagSB_sum");

    h_J1Pt_doubletagSR_QCD = (TH1F*)fPhoton->Get("J1pt_Pt_doubletagSR_QCD"); h_J2Pt_doubletagSR_QCD = (TH1F*)fPhoton->Get("J2pt_Pt_doubletagSR_QCD");
    h_J1Pt_doubletagSB_QCD = (TH1F*)fPhoton->Get("J1pt_Pt_doubletagSB_QCD"); h_J2Pt_doubletagSB_QCD = (TH1F*)fPhoton->Get("J2pt_Pt_doubletagSB_QCD");
    h_J1Pt_antitagSR_QCD = (TH1F*)fPhoton->Get("J1pt_Pt_antitagSR_QCD"); h_J2Pt_antitagSR_QCD = (TH1F*)fPhoton->Get("J2pt_Pt_antitagSR_QCD");
    h_J1Pt_antitagSB_QCD = (TH1F*)fPhoton->Get("J1pt_Pt_antitagSB_QCD"); h_J2Pt_antitagSB_QCD = (TH1F*)fPhoton->Get("J2pt_Pt_antitagSB_QCD");
    h_J1Pt_tagSR_QCD = (TH1F*)fPhoton->Get("J1pt_Pt_tagSR_QCD"); h_J2Pt_tagSR_QCD = (TH1F*)fPhoton->Get("J2pt_Pt_tagSR_QCD");
    h_J1Pt_tagSB_QCD = (TH1F*)fPhoton->Get("J1pt_Pt_tagSB_QCD"); h_J2Pt_tagSB_QCD = (TH1F*)fPhoton->Get("J2pt_Pt_tagSB_QCD");
    h_baseline_j1pt_QCD  = (TH1F*)fPhoton->Get("J1pt_Pt_baseline_QCD");  h_baseline_j2pt_QCD  = (TH1F*)fPhoton->Get("J2pt_Pt_baseline_QCD");

    h_J1Pt_doubletagSR_GJets = (TH1F*)fPhoton->Get("J1pt_Pt_doubletagSR_GJets"); h_J2Pt_doubletagSR_GJets = (TH1F*)fPhoton->Get("J2pt_Pt_doubletagSR_GJets");
    h_J1Pt_doubletagSB_GJets = (TH1F*)fPhoton->Get("J1pt_Pt_doubletagSB_GJets"); h_J2Pt_doubletagSB_GJets = (TH1F*)fPhoton->Get("J2pt_Pt_doubletagSB_GJets");
    h_J1Pt_antitagSR_GJets = (TH1F*)fPhoton->Get("J1pt_Pt_antitagSR_GJets"); h_J2Pt_antitagSR_GJets = (TH1F*)fPhoton->Get("J2pt_Pt_antitagSR_GJets");
    h_J1Pt_antitagSB_GJets = (TH1F*)fPhoton->Get("J1pt_Pt_antitagSB_GJets"); h_J2Pt_antitagSB_GJets = (TH1F*)fPhoton->Get("J2pt_Pt_antitagSB_GJets");
    h_J1Pt_tagSR_GJets = (TH1F*)fPhoton->Get("J1pt_Pt_tagSR_GJets"); h_J2Pt_tagSR_GJets = (TH1F*)fPhoton->Get("J2pt_Pt_tagSR_GJets");
    h_J1Pt_tagSB_GJets = (TH1F*)fPhoton->Get("J1pt_Pt_tagSB_GJets"); h_J2Pt_tagSB_GJets = (TH1F*)fPhoton->Get("J2pt_Pt_tagSB_GJets");
    h_baseline_j1pt_GJets  = (TH1F*)fPhoton->Get("J1pt_Pt_baseline_GJets");  h_baseline_j2pt_GJets  = (TH1F*)fPhoton->Get("J2pt_Pt_baseline_GJets");

    h_baseline_j1pt_sum  = (TH1F*)fPhoton->Get("J1pt_Pt_baseline_sum");  h_baseline_j2pt_sum  = (TH1F*)fPhoton->Get("J2pt_Pt_baseline_sum");
    h_baseline_j1pt_data = (TH1F*)fPhoton->Get("J1pt_Pt_baseline_data"); h_baseline_j2pt_data = (TH1F*)fPhoton->Get("J2pt_Pt_baseline_data");
    h_baseline_j1bb_sum  = (TH1F*)fPhoton->Get("LeadDeepBBTag_baseline_sum");  h_baseline_j2bb_sum  = (TH1F*)fPhoton->Get("SubLeadDeepBBTag_baseline_sum");
    h_baseline_j1bb_data = (TH1F*)fPhoton->Get("LeadDeepBBTag_baseline_data"); h_baseline_j2bb_data = (TH1F*)fPhoton->Get("SubLeadDeepBBTag_baseline_data");
    h_baseline_j1bb_QCD  = (TH1F*)fPhoton->Get("LeadDeepBBTag_baseline_QCD");  h_baseline_j2bb_QCD  = (TH1F*)fPhoton->Get("SubLeadDeepBBTag_baseline_QCD");
    h_baseline_j1bb_GJets  = (TH1F*)fPhoton->Get("LeadDeepBBTag_baseline_GJets");  h_baseline_j2bb_GJets  = (TH1F*)fPhoton->Get("SubLeadDeepBBTag_baseline_GJets");



    h_J2M_mjBins_doubletagSR_sum = (TH1F*)fPhoton->Get("J2_M_jetBins_doubletagSR_sum"); h_J2M_mjBins_doubletagSB_sum = (TH1F*)fPhoton->Get("J2_M_jetBins_doubletagSB_sum");
    h_J2M_mjBins_antitagSR_sum = (TH1F*)fPhoton->Get("J2_M_jetBins_antitagSR_sum"); h_J2M_mjBins_antitagSB_sum = (TH1F*)fPhoton->Get("J2_M_jetBins_antitagSB_sum");
    h_J2M_mjBins_doubletagSR_QCD = (TH1F*)fPhoton->Get("J2_M_jetBins_doubletagSR_QCD"); h_J2M_mjBins_doubletagSB_QCD = (TH1F*)fPhoton->Get("J2_M_jetBins_doubletagSB_QCD");
    h_J2M_mjBins_antitagSR_QCD = (TH1F*)fPhoton->Get("J2_M_jetBins_antitagSR_QCD"); h_J2M_mjBins_antitagSB_QCD = (TH1F*)fPhoton->Get("J2_M_jetBins_antitagSB_QCD");
    h_J2M_mjBins_doubletagSR_GJets = (TH1F*)fPhoton->Get("J2_M_jetBins_doubletagSR_GJets"); h_J2M_mjBins_doubletagSB_GJets = (TH1F*)fPhoton->Get("J2_M_jetBins_doubletagSB_GJets");
    h_J2M_mjBins_antitagSR_GJets = (TH1F*)fPhoton->Get("J2_M_jetBins_antitagSR_GJets"); h_J2M_mjBins_antitagSB_GJets = (TH1F*)fPhoton->Get("J2_M_jetBins_antitagSB_GJets");

    // h_COpt1_sum = (TH1F*)fPhoton->Get("MET_antitagSROpt1_sum"); h_DOpt1_sum = (TH1F*)fPhoton->Get("MET_antitagSBOpt1_sum");
    // hP_COpt1_sum = (TH1F*)fPhoton->Get("METPhoton_antitagSROpt1_sum"); hP_DOpt1_sum = (TH1F*)fPhoton->Get("METPhoton_antitagSBOpt1_sum");
    // h_C5Opt1_sum = (TH1F*)fPhoton->Get("MET5_antitagSROpt1_sum"); h_D5Opt1_sum = (TH1F*)fPhoton->Get("MET5_antitagSBOpt1_sum");
  }
  if (whichRegion=="singleLept") {
    h_A_data = (TH1F*)fSingleLept->Get("MET_doubletagSR_data"); h_B_data = (TH1F*)fSingleLept->Get("MET_doubletagSB_data");
    h_A1_data = (TH1F*)fSingleLept->Get("MET_tagSR_data"); h_B1_data = (TH1F*)fSingleLept->Get("MET_tagSB_data");
    h_C_data = (TH1F*)fSingleLept->Get("MET_antitagSR_data"); h_D_data = (TH1F*)fSingleLept->Get("MET_antitagSB_data");
    h_Opt1_data = (TH1F*)fSingleLept->Get("MET_antitagOpt1_data");
    // h_Opt1_data = (TH1F*)fSingleLept->Get("MET_antitagSROpt1_data");
    h_baseline_MET_data = (TH1F*)fSingleLept->Get("MET_baseline_data"); h_baseline_MET_sum = (TH1F*)fSingleLept->Get("MET_baseline_sum");

    // h_A_sum = (TH1F*)fSingleLept->Get("MET_doubletagSR_sum"); h_B_sum = (TH1F*)fSingleLept->Get("MET_doubletagSB_sum");
    // h_A1_sum = (TH1F*)fSingleLept->Get("MET_tagSR_sum"); h_B1_sum = (TH1F*)fSingleLept->Get("MET_tagSB_sum");
    // h_C_sum = (TH1F*)fSingleLept->Get("MET_antitagSR_sum"); h_D_sum = (TH1F*)fSingleLept->Get("MET_antitagSB_sum");

    // h_A5_sum = (TH1F*)fSingleLept->Get("MET5_doubletagSR_sum"); h_B5_sum = (TH1F*)fSingleLept->Get("MET5_doubletagSB_sum");
    // h_A15_sum = (TH1F*)fSingleLept->Get("MET5_tagSR_sum"); h_B15_sum = (TH1F*)fSingleLept->Get("MET5_tagSB_sum");
    // h_C5_sum = (TH1F*)fSingleLept->Get("MET5_antitagSR_sum"); h_D5_sum = (TH1F*)fSingleLept->Get("MET5_antitagSB_sum");

    h_A_SnglT = (TH1F*)fSingleLept->Get("MET_doubletagSR_SnglT"); h_B_SnglT = (TH1F*)fSingleLept->Get("MET_doubletagSB_SnglT");
    h_A1_SnglT = (TH1F*)fSingleLept->Get("MET_tagSR_SnglT"); h_B1_SnglT = (TH1F*)fSingleLept->Get("MET_tagSB_SnglT");
    h_C_SnglT = (TH1F*)fSingleLept->Get("MET_antitagSR_SnglT"); h_D_SnglT = (TH1F*)fSingleLept->Get("MET_antitagSB_SnglT");
    h_Opt1_SnglT = (TH1F*)fSingleLept->Get("MET_antitagOpt1_SnglT");
    h_baseline_MET_SnglT = (TH1F*)fSingleLept->Get("MET_baseline_SnglT");

    h_A_TT = (TH1F*)fSingleLept->Get("MET_doubletagSR_TT"); h_B_TT = (TH1F*)fSingleLept->Get("MET_doubletagSB_TT");
    h_A1_TT = (TH1F*)fSingleLept->Get("MET_tagSR_TT"); h_B1_TT = (TH1F*)fSingleLept->Get("MET_tagSB_TT");
    h_C_TT = (TH1F*)fSingleLept->Get("MET_antitagSR_TT"); h_D_TT = (TH1F*)fSingleLept->Get("MET_antitagSB_TT");
    h_Opt1_TT = (TH1F*)fSingleLept->Get("MET_antitagOpt1_TT");
    h_baseline_MET_TT = (TH1F*)fSingleLept->Get("MET_baseline_TT");

    // h_A_TT_Di = (TH1F*)fSingleLept->Get("MET_doubletagSR_TT_Di"); h_B_TT_Di = (TH1F*)fSingleLept->Get("MET_doubletagSB_TT_Di");
    // h_A1_TT_Di = (TH1F*)fSingleLept->Get("MET_tagSR_TT_Di"); h_B1_TT_Di = (TH1F*)fSingleLept->Get("MET_tagSB_TT_Di");
    // h_C_TT_Di = (TH1F*)fSingleLept->Get("MET_antitagSR_TT_Di"); h_D_TT_Di = (TH1F*)fSingleLept->Get("MET_antitagSB_TT_Di");
    // h_A_TT_SL = (TH1F*)fSingleLept->Get("MET_doubletagSR_TT_SL"); h_B_TT_SL = (TH1F*)fSingleLept->Get("MET_doubletagSB_TT_SL");
    // h_A1_TT_SL = (TH1F*)fSingleLept->Get("MET_tagSR_TT_SL"); h_B1_TT_SL = (TH1F*)fSingleLept->Get("MET_tagSB_TT_SL");
    // h_C_TT_SL = (TH1F*)fSingleLept->Get("MET_antitagSR_TT_SL"); h_D_TT_SL = (TH1F*)fSingleLept->Get("MET_antitagSB_TT_SL");

    h_A_WJets = (TH1F*)fSingleLept->Get("MET_doubletagSR_WJets"); h_B_WJets = (TH1F*)fSingleLept->Get("MET_doubletagSB_WJets");
    h_A1_WJets = (TH1F*)fSingleLept->Get("MET_tagSR_WJets"); h_B1_WJets = (TH1F*)fSingleLept->Get("MET_tagSB_WJets");
    h_C_WJets = (TH1F*)fSingleLept->Get("MET_antitagSR_WJets");   h_D_WJets = (TH1F*)fSingleLept->Get("MET_antitagSB_WJets");
    h_Opt1_WJets = (TH1F*)fSingleLept->Get("MET_antitagOpt1_WJets");
    h_baseline_MET_WJets = (TH1F*)fSingleLept->Get("MET_baseline_WJets");

    //TH1F make0EventUncSum_1l(vector<TH1F*> dem_histos); //in this order: SnglT,TT, WJets
    h_A_sum = make0EventUncSum_1l({h_A_SnglT,h_A_TT,h_A_WJets});
    h_B_sum = make0EventUncSum_1l({h_B_SnglT,h_B_TT,h_B_WJets});
    h_A1_sum = make0EventUncSum_1l({h_A1_SnglT,h_A1_TT,h_A1_WJets});
    h_B1_sum = make0EventUncSum_1l({h_B1_SnglT,h_B1_TT,h_B1_WJets});
    h_C_sum = make0EventUncSum_1l({h_C_SnglT,h_C_TT,h_C_WJets});
    h_D_sum = make0EventUncSum_1l({h_D_SnglT,h_D_TT,h_D_WJets,});
    h_Opt1_sum = make0EventUncSum_1l({h_Opt1_SnglT,h_Opt1_TT,h_Opt1_WJets});

    cdABCDReg->cd();
    h_A_sum->Write("h_A_sum");
    h_A1_sum->Write("h_A1_sum");
    h_B_sum->Write("h_B_sum");
    h_B1_sum->Write("h_B1_sum");
    h_C_sum->Write("h_C_sum");
    h_D_sum->Write("h_D_sum");
    h_Opt1_sum->Write("h_T0_sum");

    a_int = h_A_sum->IntegralAndError(1,5,a_error,""); a1_int = h_A1_sum->IntegralAndError(1,5,a1_error,"");
    b_int = h_B_sum->IntegralAndError(1,5,b_error,""); b1_int = h_B1_sum->IntegralAndError(1,5,b1_error,"");
    c_int = h_C_sum->IntegralAndError(1,5,c_error,""); d_int = h_D_sum->IntegralAndError(1,5,d_error,"");

    h_nH_sum->SetBinContent(1,c_int+d_int);   h_nH_sum->SetBinError(1,sqrt(c_error*c_error + d_error*d_error));
    h_nH_sum->SetBinContent(2,a1_int+b1_int); h_nH_sum->SetBinError(2,sqrt(a1_error*a1_error + b1_error*b1_error));
    h_nH_sum->SetBinContent(3,a_int+b_int);   h_nH_sum->SetBinError(3,sqrt(a_error*a_error + b_error*b_error));

    a_int_data = h_A_data->IntegralAndError(1,5,a_error_data,""); a1_int_data = h_A1_data->IntegralAndError(1,5,a1_error_data,"");
    b_int_data = h_B_data->IntegralAndError(1,5,b_error_data,""); b1_int_data = h_B1_data->IntegralAndError(1,5,b1_error_data,"");
    c_int_data = h_C_data->IntegralAndError(1,5,c_error_data,""); d_int_data = h_D_data->IntegralAndError(1,5,d_error_data,"");

    h_nH_data->SetBinContent(1,c_int_data+d_int_data);   h_nH_data->SetBinError(1,sqrt(c_error_data*c_error_data + d_error_data*d_error_data));
    h_nH_data->SetBinContent(2,a1_int_data+b1_int_data); h_nH_data->SetBinError(2,sqrt(a1_error_data*a1_error_data + b1_error_data*b1_error_data));
    h_nH_data->SetBinContent(3,a_int_data+b_int_data);   h_nH_data->SetBinError(3,sqrt(a_error_data*a_error_data + b_error_data*b_error_data));


    h_J1M_doubletagSR_sum = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSR_sum"); h_J2M_doubletagSR_sum = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSR_sum");
    h_J1M_doubletagSB_sum = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSB_sum"); h_J2M_doubletagSB_sum = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSB_sum");
    h_J1M_antitagSR_sum = (TH1F*)fSingleLept->Get("J1pt_M_antitagSR_sum"); h_J2M_antitagSR_sum = (TH1F*)fSingleLept->Get("J2pt_M_antitagSR_sum");
    h_J1M_antitagSB_sum = (TH1F*)fSingleLept->Get("J1pt_M_antitagSB_sum"); h_J2M_antitagSB_sum = (TH1F*)fSingleLept->Get("J2pt_M_antitagSB_sum");
    h_J1M_tagSR_sum = (TH1F*)fSingleLept->Get("J1pt_M_tagSR_sum"); h_J2M_tagSR_sum = (TH1F*)fSingleLept->Get("J2pt_M_tagSR_sum");
    h_J1M_tagSB_sum = (TH1F*)fSingleLept->Get("J1pt_M_tagSB_sum"); h_J2M_tagSB_sum = (TH1F*)fSingleLept->Get("J2pt_M_tagSB_sum");

    h_J1J2M_doubletagSR_sum = (TH1F*)h_J1M_doubletagSR_sum->Clone("J1J2M_doubletagSR_sum"); h_J1J2M_doubletagSR_sum->Add(h_J2M_doubletagSR_sum);
    h_J1J2M_doubletagSB_sum = (TH1F*)h_J1M_doubletagSB_sum->Clone("J1J2M_doubletagSB_sum"); h_J1J2M_doubletagSB_sum->Add(h_J2M_doubletagSB_sum);
    h_J1J2M_tagSR_sum = (TH1F*)h_J1M_tagSR_sum->Clone("J1J2M_tagSR_sum"); h_J1J2M_tagSR_sum->Add(h_J2M_tagSR_sum);
    h_J1J2M_tagSB_sum = (TH1F*)h_J1M_tagSB_sum->Clone("J1J2M_tagSB_sum"); h_J1J2M_tagSB_sum->Add(h_J2M_tagSB_sum);
    h_J1J2M_antitagSR_sum = (TH1F*)h_J1M_antitagSR_sum->Clone("J1J2M_antitagSR_sum"); h_J1J2M_antitagSR_sum->Add(h_J2M_antitagSR_sum);
    h_J1J2M_antitagSB_sum = (TH1F*)h_J1M_antitagSB_sum->Clone("J1J2M_antitagSB_sum"); h_J1J2M_antitagSB_sum->Add(h_J2M_antitagSB_sum);

    h_J1M_doubletagSR_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSR_SnglT"); h_J2M_doubletagSR_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSR_SnglT");
    h_J1M_doubletagSB_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSB_SnglT"); h_J2M_doubletagSB_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSB_SnglT");
    h_J1M_antitagSR_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_antitagSR_SnglT"); h_J2M_antitagSR_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_antitagSR_SnglT");
    h_J1M_antitagSB_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_antitagSB_SnglT"); h_J2M_antitagSB_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_antitagSB_SnglT");
    h_J1M_tagSR_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_tagSR_SnglT"); h_J2M_tagSR_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_tagSR_SnglT");
    h_J1M_tagSB_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_tagSB_SnglT"); h_J2M_tagSB_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_tagSB_SnglT");
    h_baseline_j1mass_SnglT = (TH1F*)fSingleLept->Get("J1pt_M_baseline_SnglT"); h_baseline_j2mass_SnglT = (TH1F*)fSingleLept->Get("J2pt_M_baseline_SnglT");

    h_J1M_doubletagSR_TT = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSR_TT"); h_J2M_doubletagSR_TT = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSR_TT");
    h_J1M_doubletagSB_TT = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSB_TT"); h_J2M_doubletagSB_TT = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSB_TT");
    h_J1M_antitagSR_TT = (TH1F*)fSingleLept->Get("J1pt_M_antitagSR_TT"); h_J2M_antitagSR_TT = (TH1F*)fSingleLept->Get("J2pt_M_antitagSR_TT");
    h_J1M_antitagSB_TT = (TH1F*)fSingleLept->Get("J1pt_M_antitagSB_TT"); h_J2M_antitagSB_TT = (TH1F*)fSingleLept->Get("J2pt_M_antitagSB_TT");
    h_J1M_tagSR_TT = (TH1F*)fSingleLept->Get("J1pt_M_tagSR_TT"); h_J2M_tagSR_TT = (TH1F*)fSingleLept->Get("J2pt_M_tagSR_TT");
    h_J1M_tagSB_TT = (TH1F*)fSingleLept->Get("J1pt_M_tagSB_TT"); h_J2M_tagSB_TT = (TH1F*)fSingleLept->Get("J2pt_M_tagSB_TT");
    h_baseline_j1mass_TT = (TH1F*)fSingleLept->Get("J1pt_M_baseline_TT"); h_baseline_j2mass_TT = (TH1F*)fSingleLept->Get("J2pt_M_baseline_TT");

    // h_J1M_doubletagSR_TT_SL = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSR_TT_SL"); h_J2M_doubletagSR_TT_SL = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSR_TT_SL");
    // h_J1M_doubletagSB_TT_SL = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSB_TT_SL"); h_J2M_doubletagSB_TT_SL = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSB_TT_SL");
    // h_J1M_antitagSR_TT_SL = (TH1F*)fSingleLept->Get("J1pt_M_antitagSR_TT_SL"); h_J2M_antitagSR_TT_SL = (TH1F*)fSingleLept->Get("J2pt_M_antitagSR_TT_SL");
    // h_J1M_antitagSB_TT_SL = (TH1F*)fSingleLept->Get("J1pt_M_antitagSB_TT_SL"); h_J2M_antitagSB_TT_SL = (TH1F*)fSingleLept->Get("J2pt_M_antitagSB_TT_SL");
    // h_J1M_tagSR_TT_SL = (TH1F*)fSingleLept->Get("J1pt_M_tagSR_TT_SL"); h_J2M_tagSR_TT_SL = (TH1F*)fSingleLept->Get("J2pt_M_tagSR_TT_SL");
    // h_J1M_tagSB_TT_SL = (TH1F*)fSingleLept->Get("J1pt_M_tagSB_TT_SL"); h_J2M_tagSB_TT_SL = (TH1F*)fSingleLept->Get("J2pt_M_tagSB_TT_SL");
    // h_J1M_doubletagSR_TT_Di = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSR_TT_Di"); h_J2M_doubletagSR_TT_Di = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSR_TT_Di");
    // h_J1M_doubletagSB_TT_Di = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSB_TT_Di"); h_J2M_doubletagSB_TT_Di = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSB_TT_Di");
    // h_J1M_antitagSR_TT_Di = (TH1F*)fSingleLept->Get("J1pt_M_antitagSR_TT_Di"); h_J2M_antitagSR_TT_Di = (TH1F*)fSingleLept->Get("J2pt_M_antitagSR_TT_Di");
    // h_J1M_antitagSB_TT_Di = (TH1F*)fSingleLept->Get("J1pt_M_antitagSB_TT_Di"); h_J2M_antitagSB_TT_Di = (TH1F*)fSingleLept->Get("J2pt_M_antitagSB_TT_Di");
    // h_J1M_tagSR_TT_Di = (TH1F*)fSingleLept->Get("J1pt_M_tagSR_TT_Di"); h_J2M_tagSR_TT_Di = (TH1F*)fSingleLept->Get("J2pt_M_tagSR_TT_Di");
    // h_J1M_tagSB_TT_Di = (TH1F*)fSingleLept->Get("J1pt_M_tagSB_TT_Di"); h_J2M_tagSB_TT_Di = (TH1F*)fSingleLept->Get("J2pt_M_tagSB_TT_Di");

    h_J1M_doubletagSR_WJets = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSR_WJets"); h_J2M_doubletagSR_WJets = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSR_WJets");
    h_J1M_doubletagSB_WJets = (TH1F*)fSingleLept->Get("J1pt_M_doubletagSB_WJets"); h_J2M_doubletagSB_WJets = (TH1F*)fSingleLept->Get("J2pt_M_doubletagSB_WJets");
    h_J1M_antitagSR_WJets = (TH1F*)fSingleLept->Get("J1pt_M_antitagSR_WJets"); h_J2M_antitagSR_WJets = (TH1F*)fSingleLept->Get("J2pt_M_antitagSR_WJets");
    h_J1M_antitagSB_WJets = (TH1F*)fSingleLept->Get("J1pt_M_antitagSB_WJets"); h_J2M_antitagSB_WJets = (TH1F*)fSingleLept->Get("J2pt_M_antitagSB_WJets");
    h_J1M_tagSR_WJets = (TH1F*)fSingleLept->Get("J1pt_M_tagSR_WJets"); h_J2M_tagSR_WJets = (TH1F*)fSingleLept->Get("J2pt_M_tagSR_WJets");
    h_J1M_tagSB_WJets = (TH1F*)fSingleLept->Get("J1pt_M_tagSB_WJets"); h_J2M_tagSB_WJets = (TH1F*)fSingleLept->Get("J2pt_M_tagSB_WJets");
    h_baseline_j1mass_WJets = (TH1F*)fSingleLept->Get("J1pt_M_baseline_WJets"); h_baseline_j2mass_WJets = (TH1F*)fSingleLept->Get("J2pt_M_baseline_WJets");

    h_baseline_j1mass_sum = (TH1F*)fSingleLept->Get("J1pt_M_baseline_sum"); h_baseline_j2mass_sum = (TH1F*)fSingleLept->Get("J2pt_M_baseline_sum");
    h_baseline_j1mass_data = (TH1F*)fSingleLept->Get("J1pt_M_baseline_data"); h_baseline_j2mass_data = (TH1F*)fSingleLept->Get("J2pt_M_baseline_data");

    h_J1Pt_doubletagSR_TT = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSR_TT"); h_J2Pt_doubletagSR_TT = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSR_TT");
    h_J1Pt_doubletagSB_TT = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSB_TT"); h_J2Pt_doubletagSB_TT = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSB_TT");
    h_J1Pt_antitagSR_TT = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSR_TT"); h_J2Pt_antitagSR_TT = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSR_TT");
    h_J1Pt_antitagSB_TT = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSB_TT"); h_J2Pt_antitagSB_TT = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSB_TT");
    h_J1Pt_tagSR_TT = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSR_TT"); h_J2Pt_tagSR_TT = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSR_TT");
    h_J1Pt_tagSB_TT = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSB_TT"); h_J2Pt_tagSB_TT = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSB_TT");
    h_baseline_j1pt_TT  = (TH1F*)fSingleLept->Get("J1pt_Pt_baseline_TT");  h_baseline_j2pt_TT  = (TH1F*)fSingleLept->Get("J2pt_Pt_baseline_TT");


    h_J1Pt_doubletagSR_SnglT = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSR_SnglT"); h_J2Pt_doubletagSR_SnglT = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSR_SnglT");
    h_J1Pt_doubletagSB_SnglT = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSB_SnglT"); h_J2Pt_doubletagSB_SnglT = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSB_SnglT");
    h_J1Pt_antitagSR_SnglT = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSR_SnglT"); h_J2Pt_antitagSR_SnglT = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSR_SnglT");
    h_J1Pt_antitagSB_SnglT = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSB_SnglT"); h_J2Pt_antitagSB_SnglT = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSB_SnglT");
    h_J1Pt_tagSR_SnglT = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSR_SnglT"); h_J2Pt_tagSR_SnglT = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSR_SnglT");
    h_J1Pt_tagSB_SnglT = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSB_SnglT"); h_J2Pt_tagSB_SnglT = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSB_SnglT");
    h_baseline_j1pt_SnglT  = (TH1F*)fSingleLept->Get("J1pt_Pt_baseline_SnglT");  h_baseline_j2pt_SnglT  = (TH1F*)fSingleLept->Get("J2pt_Pt_baseline_SnglT");



    // h_J1Pt_doubletagSR_TT_SL = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSR_TT_SL"); h_J2Pt_doubletagSR_TT_SL = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSR_TT_SL");
    // h_J1Pt_doubletagSB_TT_SL = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSB_TT_SL"); h_J2Pt_doubletagSB_TT_SL = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSB_TT_SL");
    // h_J1Pt_antitagSR_TT_SL = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSR_TT_SL"); h_J2Pt_antitagSR_TT_SL = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSR_TT_SL");
    // h_J1Pt_antitagSB_TT_SL = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSB_TT_SL"); h_J2Pt_antitagSB_TT_SL = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSB_TT_SL");
    // h_J1Pt_tagSR_TT_SL = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSR_TT_SL"); h_J2Pt_tagSR_TT_SL = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSR_TT_SL");
    // h_J1Pt_tagSB_TT_SL = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSB_TT_SL"); h_J2Pt_tagSB_TT_SL = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSB_TT_SL");
    // h_J1Pt_doubletagSR_TT_Di = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSR_TT_Di"); h_J2Pt_doubletagSR_TT_Di = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSR_TT_Di");
    // h_J1Pt_doubletagSB_TT_Di = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSB_TT_Di"); h_J2Pt_doubletagSB_TT_Di = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSB_TT_Di");
    // h_J1Pt_antitagSR_TT_Di = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSR_TT_Di"); h_J2Pt_antitagSR_TT_Di = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSR_TT_Di");
    // h_J1Pt_antitagSB_TT_Di = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSB_TT_Di"); h_J2Pt_antitagSB_TT_Di = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSB_TT_Di");
    // h_J1Pt_tagSR_TT_Di = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSR_TT_Di"); h_J2Pt_tagSR_TT_Di = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSR_TT_Di");
    // h_J1Pt_tagSB_TT_Di = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSB_TT_Di"); h_J2Pt_tagSB_TT_Di = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSB_TT_Di");

    h_J1Pt_doubletagSR_WJets = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSR_WJets"); h_J2Pt_doubletagSR_WJets = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSR_WJets");
    h_J1Pt_doubletagSB_WJets = (TH1F*)fSingleLept->Get("J1pt_Pt_doubletagSB_WJets"); h_J2Pt_doubletagSB_WJets = (TH1F*)fSingleLept->Get("J2pt_Pt_doubletagSB_WJets");
    h_J1Pt_antitagSR_WJets = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSR_WJets"); h_J2Pt_antitagSR_WJets = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSR_WJets");
    h_J1Pt_antitagSB_WJets = (TH1F*)fSingleLept->Get("J1pt_Pt_antitagSB_WJets"); h_J2Pt_antitagSB_WJets = (TH1F*)fSingleLept->Get("J2pt_Pt_antitagSB_WJets");
    h_J1Pt_tagSR_WJets = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSR_WJets"); h_J2Pt_tagSR_WJets = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSR_WJets");
    h_J1Pt_tagSB_WJets = (TH1F*)fSingleLept->Get("J1pt_Pt_tagSB_WJets"); h_J2Pt_tagSB_WJets = (TH1F*)fSingleLept->Get("J2pt_Pt_tagSB_WJets");
    h_baseline_j1pt_WJets  = (TH1F*)fSingleLept->Get("J1pt_Pt_baseline_WJets");  h_baseline_j2pt_WJets  = (TH1F*)fSingleLept->Get("J2pt_Pt_baseline_WJets");



    h_baseline_j1pt_sum  = (TH1F*)fSingleLept->Get("J1pt_Pt_baseline_sum");  h_baseline_j2pt_sum  = (TH1F*)fSingleLept->Get("J2pt_Pt_baseline_sum");
    h_baseline_j1pt_data = (TH1F*)fSingleLept->Get("J1pt_Pt_baseline_data"); h_baseline_j2pt_data = (TH1F*)fSingleLept->Get("J2pt_Pt_baseline_data");
    h_baseline_j1bb_sum  = (TH1F*)fSingleLept->Get("LeadDeepBBTag_baseline_sum");  h_baseline_j2bb_sum  = (TH1F*)fSingleLept->Get("SubLeadDeepBBTag_baseline_sum");
    h_baseline_j1bb_data = (TH1F*)fSingleLept->Get("LeadDeepBBTag_baseline_data"); h_baseline_j2bb_data = (TH1F*)fSingleLept->Get("SubLeadDeepBBTag_baseline_data");
    h_baseline_j1bb_TT  = (TH1F*)fSingleLept->Get("LeadDeepBBTag_baseline_TT");  h_baseline_j2bb_TT  = (TH1F*)fSingleLept->Get("SubLeadDeepBBTag_baseline_TT");
    h_baseline_j1bb_SnglT  = (TH1F*)fSingleLept->Get("LeadDeepBBTag_baseline_SnglT");  h_baseline_j2bb_SnglT  = (TH1F*)fSingleLept->Get("SubLeadDeepBBTag_baseline_SnglT");
    h_baseline_j1bb_WJets  = (TH1F*)fSingleLept->Get("LeadDeepBBTag_baseline_WJets");  h_baseline_j2bb_WJets  = (TH1F*)fSingleLept->Get("SubLeadDeepBBTag_baseline_WJets");



    // h_J2M_mjBins_doubletagSR_sum = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSR_sum"); h_J2M_mjBins_doubletagSB_sum = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSB_sum");
    // h_J2M_mjBins_antitagSR_sum = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSR_sum"); h_J2M_mjBins_antitagSB_sum = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSB_sum");
    h_J2M_mjBins_doubletagSR_TT = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSR_TT"); h_J2M_mjBins_doubletagSB_TT = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSB_TT");
    h_J2M_mjBins_antitagSR_TT = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSR_TT"); h_J2M_mjBins_antitagSB_TT = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSB_TT");
    h_J2M_mjBins_doubletagSR_SnglT = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSR_SnglT"); h_J2M_mjBins_doubletagSB_SnglT = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSB_SnglT");
    h_J2M_mjBins_antitagSR_SnglT = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSR_SnglT"); h_J2M_mjBins_antitagSB_SnglT = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSB_SnglT");

    // h_J2M_mjBins_doubletagSR_TT_Di = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_TT_Di"); h_J2M_mjBins_doubletagSB_TT_Di = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_TT_Di");
    // h_J2M_mjBins_antitagSR_TT_Di = (TH1F*)f->Get("J2_M_jetBins_antitagSR_TT_Di"); h_J2M_mjBins_antitagSB_TT_Di = (TH1F*)f->Get("J2_M_jetBins_antitagSB_TT_Di");
    // h_J2M_mjBins_doubletagSR_TT_SL = (TH1F*)f->Get("J2_M_jetBins_doubletagSR_TT_SL"); h_J2M_mjBins_doubletagSB_TT_SL = (TH1F*)f->Get("J2_M_jetBins_doubletagSB_TT_SL");
    // h_J2M_mjBins_antitagSR_TT_SL = (TH1F*)f->Get("J2_M_jetBins_antitagSR_TT_SL"); h_J2M_mjBins_antitagSB_TT_SL = (TH1F*)f->Get("J2_M_jetBins_antitagSB_TT_SL");

    h_J2M_mjBins_doubletagSR_WJets = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSR_WJets"); h_J2M_mjBins_doubletagSB_WJets = (TH1F*)fSingleLept->Get("J2_M_jetBins_doubletagSB_WJets");
    h_J2M_mjBins_antitagSR_WJets = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSR_WJets"); h_J2M_mjBins_antitagSB_WJets = (TH1F*)fSingleLept->Get("J2_M_jetBins_antitagSB_WJets");
  }

  std::cout<<"Here? 20"<<std::endl;

  vector<TH1F*> histos_ABCD_data = {h_A_data, h_B_data, h_C_data, h_D_data, h_Opt1_data};
  vector<TH1F*> histos_A1B1CD_data = {h_A1_data, h_B1_data, h_C_data, h_D_data, h_Opt1_data};
  vector<TH1F*> histos_allRegions_data = {h_A_data, h_A1_data, h_B_data, h_B1_data, h_C_data, h_D_data,h_Opt1_data};

  vector<TH1F*> histos_allRegions_sum = {h_A_sum, h_A1_sum, h_B_sum, h_B1_sum, h_C_sum, h_D_sum,h_Opt1_sum};
  vector<TH1F*> histos_allRegions_QCD = {h_A_QCD, h_A1_QCD, h_B_QCD, h_B1_QCD, h_C_QCD, h_D_QCD,h_Opt1_QCD};
  vector<TH1F*> histos_allRegions_ZJets = {h_A_ZJets, h_A1_ZJets, h_B_ZJets, h_B1_ZJets, h_C_ZJets, h_D_ZJets,h_Opt1_ZJets};
  vector<TH1F*> histos_allRegions_WJets = {h_A_WJets, h_A1_WJets, h_B_WJets, h_B1_WJets, h_C_WJets, h_D_WJets,h_Opt1_WJets};
  vector<TH1F*> histos_allRegions_TT = {h_A_TT, h_A1_TT, h_B_TT, h_B1_TT, h_C_TT, h_D_TT,h_Opt1_TT};
  vector<TH1F*> histos_allRegions_SnglT = {h_A_SnglT, h_A1_SnglT, h_B_SnglT, h_B1_SnglT, h_C_SnglT, h_D_SnglT,h_Opt1_SnglT};
  vector<TH1F*> histos_allRegions_GJets = {h_A_GJets, h_A1_GJets, h_B_GJets, h_B1_GJets, h_C_GJets, h_D_GJets,h_Opt1_GJets};

  vector < vector<TH1F*> > all_ABCDHistos_0l = {histos_allRegions_sum, histos_allRegions_QCD,histos_allRegions_ZJets,histos_allRegions_WJets,histos_allRegions_TT, histos_allRegions_SnglT};
  vector < vector<TH1F*> > all_ABCDHistos_1l = {histos_allRegions_sum, histos_allRegions_WJets,histos_allRegions_TT, histos_allRegions_SnglT};
  vector < vector<TH1F*> > all_ABCDHistos_1p = {histos_allRegions_sum, histos_allRegions_QCD,histos_allRegions_GJets};

  vector<TH1F*> histosPhoton_ABCD_sum = {hP_A_sum, hP_B_sum, hP_C_sum, hP_D_sum};
  vector<TH1F*> histosPhoton_A1B1CD_sum = {hP_A1_sum, hP_B1_sum, hP_C_sum, hP_D_sum};


  vector<TH1F*> histos_ABCD_sum = {h_A_sum, h_B_sum, h_C_sum, h_D_sum, h_Opt1_sum};
  vector<TH1F*> histos_ABCD_QCD = {h_A_QCD, h_B_QCD, h_C_QCD, h_D_QCD};
  vector<TH1F*> histos_ABCD_GJets = {h_A_GJets, h_B_GJets, h_C_GJets, h_D_GJets};
  vector<TH1F*> histos_ABCD_SnglT = {h_A_SnglT, h_B_SnglT, h_C_SnglT, h_D_SnglT};
  vector<TH1F*> histos_ABCD_TT = {h_A_TT, h_B_TT, h_C_TT, h_D_TT};
  vector<TH1F*> histos_ABCD_TT_Di = {h_A_TT_Di, h_B_TT_Di, h_C_TT_Di, h_D_TT_Di};
  vector<TH1F*> histos_ABCD_TT_SL = {h_A_TT_SL, h_B_TT_SL, h_C_TT_SL, h_D_TT_SL};
  vector<TH1F*> histos_ABCD_WJets = {h_A_WJets, h_B_WJets, h_C_WJets, h_D_WJets};
  vector<TH1F*> histos_ABCD_ZJets = {h_A_ZJets, h_B_ZJets, h_C_ZJets, h_D_ZJets};

  vector<TH1F*> histos_ABCD_T5HH1600 = {h_A_T5HH1600, h_B_T5HH1600, h_C_T5HH1600, h_D_T5HH1600};
  vector<TH1F*> histos_ABCD_T5HH2000 = {h_A_T5HH2000, h_B_T5HH2000, h_C_T5HH2000, h_D_T5HH2000};
  vector<TH1F*> histos_ABCD_T5HH2200 = {h_A_T5HH2200, h_B_T5HH2200, h_C_T5HH2200, h_D_T5HH2200};
  vector<TH1F*> histos_ABCD_TChiHH1000 = {h_A_TChiHH1000, h_B_TChiHH1000, h_C_TChiHH1000, h_D_TChiHH1000};
  vector<TH1F*> histos_ABCD_TChiHH500 = {h_A_TChiHH500, h_B_TChiHH500, h_C_TChiHH500, h_D_TChiHH500};
  vector<TH1F*> histos_ABCD_TChiHH800 = {h_A_TChiHH800, h_B_TChiHH800, h_C_TChiHH800, h_D_TChiHH800};

  vector<TH1F*> histos_A1B1CD_sum = {h_A1_sum, h_B1_sum, h_C_sum, h_D_sum, h_Opt1_sum};
  vector<TH1F*> histos_A1B1CD_QCD = {h_A1_QCD, h_B1_QCD, h_C_QCD, h_D_QCD};
  vector<TH1F*> histos_A1B1CD_GJets = {h_A1_GJets, h_B1_GJets, h_C_GJets, h_D_GJets};
  vector<TH1F*> histos_A1B1CD_SnglT = {h_A1_SnglT, h_B1_SnglT, h_C_SnglT, h_D_SnglT};
  vector<TH1F*> histos_A1B1CD_TT = {h_A1_TT, h_B1_TT, h_C_TT, h_D_TT};
  vector<TH1F*> histos_A1B1CD_TT_Di = {h_A1_TT_Di, h_B1_TT_Di, h_C_TT_Di, h_D_TT_Di};
  vector<TH1F*> histos_A1B1CD_TT_SL = {h_A1_TT_SL, h_B1_TT_SL, h_C_TT_SL, h_D_TT_SL};
  vector<TH1F*> histos_A1B1CD_WJets = {h_A1_WJets, h_B1_WJets, h_C_WJets, h_D_WJets};
  vector<TH1F*> histos_A1B1CD_ZJets = {h_A1_ZJets, h_B1_ZJets, h_C_ZJets, h_D_ZJets};

  vector<TH1F*> histos_A1B1CD_T5HH1600 = {h_A1_T5HH1600, h_B1_T5HH1600, h_C_T5HH1600, h_D_T5HH1600};
  vector<TH1F*> histos_A1B1CD_T5HH2000 = {h_A1_T5HH2000, h_B1_T5HH2000, h_C_T5HH2000, h_D_T5HH2000};
  vector<TH1F*> histos_A1B1CD_T5HH2200 = {h_A1_T5HH2200, h_B1_T5HH2200, h_C_T5HH2200, h_D_T5HH2200};
  vector<TH1F*> histos_A1B1CD_TChiHH1000 = {h_A1_TChiHH1000, h_B1_TChiHH1000, h_C_TChiHH1000, h_D_TChiHH1000};
  vector<TH1F*> histos_A1B1CD_TChiHH500 = {h_A1_TChiHH500, h_B1_TChiHH500, h_C_TChiHH500, h_D_TChiHH500};
  vector<TH1F*> histos_A1B1CD_TChiHH800 = {h_A1_TChiHH800, h_B1_TChiHH800, h_C_TChiHH800, h_D_TChiHH800};

  vector<TH1F*> histos_Rpf_J1_sum = {h_J1M_doubletagSR_sum, h_J1M_doubletagSB_sum, h_J1M_antitagSR_sum, h_J1M_antitagSB_sum};
  vector<TH1F*> histos_Rpf_J2_sum = {h_J2M_doubletagSR_sum, h_J2M_doubletagSB_sum, h_J2M_antitagSR_sum, h_J2M_antitagSB_sum};
  vector<TH1F*> histos_Rpf_J2_mJBins_sum = {h_J2M_mjBins_doubletagSR_sum, h_J2M_mjBins_doubletagSB_sum, h_J2M_mjBins_antitagSR_sum, h_J2M_mjBins_antitagSB_sum};
  vector<TH1F*> histos_Rpfsingle_J1_sum = {h_J1M_tagSR_sum, h_J1M_tagSB_sum, h_J1M_antitagSR_sum, h_J1M_antitagSB_sum};
  vector<TH1F*> histos_Rpfsingle_J2_sum = {h_J2M_tagSR_sum, h_J2M_tagSB_sum, h_J2M_antitagSR_sum, h_J2M_antitagSB_sum};
  //
  vector<TH1F*> histos_Rpf_J1J2_sum = {h_J1J2M_doubletagSR_sum, h_J1J2M_doubletagSB_sum, h_J1J2M_antitagSR_sum, h_J1J2M_antitagSB_sum};
  vector<TH1F*> histos_Rpfsingle_J1J2_sum = {h_J1J2M_tagSR_sum, h_J1J2M_tagSB_sum, h_J1J2M_antitagSR_sum, h_J1J2M_antitagSB_sum};


  vector<TH1F*> histos_Rpf_J1_QCD = {h_J1M_doubletagSR_QCD, h_J1M_doubletagSB_QCD, h_J1M_antitagSR_QCD, h_J1M_antitagSB_QCD};
  vector<TH1F*> histos_Rpf_J2_QCD = {h_J2M_doubletagSR_QCD, h_J2M_doubletagSB_QCD, h_J2M_antitagSR_QCD, h_J2M_antitagSB_QCD};
  vector<TH1F*> histos_Rpf_J2_mJBins_QCD = {h_J2M_mjBins_doubletagSR_QCD, h_J2M_mjBins_doubletagSB_QCD, h_J2M_mjBins_antitagSR_QCD, h_J2M_mjBins_antitagSB_QCD};
  vector<TH1F*> histos_Rpfsingle_J1_QCD = {h_J1M_tagSR_QCD, h_J1M_tagSB_QCD, h_J1M_antitagSR_QCD, h_J1M_antitagSB_QCD};
  vector<TH1F*> histos_Rpfsingle_J2_QCD = {h_J2M_tagSR_QCD, h_J2M_tagSB_QCD, h_J2M_antitagSR_QCD, h_J2M_antitagSB_QCD};

  vector<TH1F*> histos_Rpf_J1_GJets = {h_J1M_doubletagSR_GJets, h_J1M_doubletagSB_GJets, h_J1M_antitagSR_GJets, h_J1M_antitagSB_GJets};
  vector<TH1F*> histos_Rpf_J2_GJets = {h_J2M_doubletagSR_GJets, h_J2M_doubletagSB_GJets, h_J2M_antitagSR_GJets, h_J2M_antitagSB_GJets};
  vector<TH1F*> histos_Rpf_J2_mJBins_GJets = {h_J2M_mjBins_doubletagSR_GJets, h_J2M_mjBins_doubletagSB_GJets, h_J2M_mjBins_antitagSR_GJets, h_J2M_mjBins_antitagSB_GJets};
  vector<TH1F*> histos_Rpfsingle_J1_GJets = {h_J1M_tagSR_GJets, h_J1M_tagSB_GJets, h_J1M_antitagSR_GJets, h_J1M_antitagSB_GJets};
  vector<TH1F*> histos_Rpfsingle_J2_GJets = {h_J2M_tagSR_GJets, h_J2M_tagSB_GJets, h_J2M_antitagSR_GJets, h_J2M_antitagSB_GJets};

  vector<TH1F*> histos_Rpf_J1_SnglT = {h_J1M_doubletagSR_SnglT, h_J1M_doubletagSB_SnglT, h_J1M_antitagSR_SnglT, h_J1M_antitagSB_SnglT};
  vector<TH1F*> histos_Rpf_J2_SnglT = {h_J2M_doubletagSR_SnglT, h_J2M_doubletagSB_SnglT, h_J2M_antitagSR_SnglT, h_J2M_antitagSB_SnglT};
  vector<TH1F*> histos_Rpf_J2_mJBins_SnglT = {h_J2M_mjBins_doubletagSR_SnglT, h_J2M_mjBins_doubletagSB_SnglT, h_J2M_mjBins_antitagSR_SnglT, h_J2M_mjBins_antitagSB_SnglT};
  vector<TH1F*> histos_Rpfsingle_J1_SnglT = {h_J1M_tagSR_SnglT, h_J1M_tagSB_SnglT, h_J1M_antitagSR_SnglT, h_J1M_antitagSB_SnglT};
  vector<TH1F*> histos_Rpfsingle_J2_SnglT = {h_J2M_tagSR_SnglT, h_J2M_tagSB_SnglT, h_J2M_antitagSR_SnglT, h_J2M_antitagSB_SnglT};

  vector<TH1F*> histos_Rpf_J1_TT = {h_J1M_doubletagSR_TT, h_J1M_doubletagSB_TT, h_J1M_antitagSR_TT, h_J1M_antitagSB_TT};
  vector<TH1F*> histos_Rpf_J2_TT = {h_J2M_doubletagSR_TT, h_J2M_doubletagSB_TT, h_J2M_antitagSR_TT, h_J2M_antitagSB_TT};
  vector<TH1F*> histos_Rpf_J2_mJBins_TT = {h_J2M_mjBins_doubletagSR_TT, h_J2M_mjBins_doubletagSB_TT, h_J2M_mjBins_antitagSR_TT, h_J2M_mjBins_antitagSB_TT};
  vector<TH1F*> histos_Rpfsingle_J1_TT = {h_J1M_tagSR_TT, h_J1M_tagSB_TT, h_J1M_antitagSR_TT, h_J1M_antitagSB_TT};
  vector<TH1F*> histos_Rpfsingle_J2_TT = {h_J2M_tagSR_TT, h_J2M_tagSB_TT, h_J2M_antitagSR_TT, h_J2M_antitagSB_TT};

  vector<TH1F*> histos_Rpf_J1_TT_Di = {h_J1M_doubletagSR_TT_Di, h_J1M_doubletagSB_TT_Di, h_J1M_antitagSR_TT_Di, h_J1M_antitagSB_TT_Di};
  vector<TH1F*> histos_Rpf_J2_TT_Di = {h_J2M_doubletagSR_TT_Di, h_J2M_doubletagSB_TT_Di, h_J2M_antitagSR_TT_Di, h_J2M_antitagSB_TT_Di};
  vector<TH1F*> histos_Rpf_J2_mJBins_TT_Di = {h_J2M_mjBins_doubletagSR_TT_Di, h_J2M_mjBins_doubletagSB_TT_Di, h_J2M_mjBins_antitagSR_TT_Di, h_J2M_mjBins_antitagSB_TT_Di};
  vector<TH1F*> histos_Rpfsingle_J1_TT_Di = {h_J1M_tagSR_TT_Di, h_J1M_tagSB_TT_Di, h_J1M_antitagSR_TT_Di, h_J1M_antitagSB_TT_Di};
  vector<TH1F*> histos_Rpfsingle_J2_TT_Di = {h_J2M_tagSR_TT_Di, h_J2M_tagSB_TT_Di, h_J2M_antitagSR_TT_Di, h_J2M_antitagSB_TT_Di};

  vector<TH1F*> histos_Rpf_J1_TT_SL = {h_J1M_doubletagSR_TT_SL, h_J1M_doubletagSB_TT_SL, h_J1M_antitagSR_TT_SL, h_J1M_antitagSB_TT_SL};
  vector<TH1F*> histos_Rpf_J2_TT_SL = {h_J2M_doubletagSR_TT_SL, h_J2M_doubletagSB_TT_SL, h_J2M_antitagSR_TT_SL, h_J2M_antitagSB_TT_SL};
  vector<TH1F*> histos_Rpf_J2_mJBins_TT_SL = {h_J2M_mjBins_doubletagSR_TT_SL, h_J2M_mjBins_doubletagSB_TT_SL, h_J2M_mjBins_antitagSR_TT_SL, h_J2M_mjBins_antitagSB_TT_SL};
  vector<TH1F*> histos_Rpfsingle_J1_TT_SL = {h_J1M_tagSR_TT_SL, h_J1M_tagSB_TT_SL, h_J1M_antitagSR_TT_SL, h_J1M_antitagSB_TT_SL};
  vector<TH1F*> histos_Rpfsingle_J2_TT_SL = {h_J2M_tagSR_TT_SL, h_J2M_tagSB_TT_SL, h_J2M_antitagSR_TT_SL, h_J2M_antitagSB_TT_SL};

  vector<TH1F*> histos_Rpf_J1_WJets = {h_J1M_doubletagSR_WJets, h_J1M_doubletagSB_WJets, h_J1M_antitagSR_WJets, h_J1M_antitagSB_WJets};
  vector<TH1F*> histos_Rpf_J2_WJets = {h_J2M_doubletagSR_WJets, h_J2M_doubletagSB_WJets, h_J2M_antitagSR_WJets, h_J2M_antitagSB_WJets};
  vector<TH1F*> histos_Rpf_J2_mJBins_WJets = {h_J2M_mjBins_doubletagSR_WJets, h_J2M_mjBins_doubletagSB_WJets, h_J2M_mjBins_antitagSR_WJets, h_J2M_mjBins_antitagSB_WJets};
  vector<TH1F*> histos_Rpfsingle_J1_WJets = {h_J1M_tagSR_WJets, h_J1M_tagSB_WJets, h_J1M_antitagSR_WJets, h_J1M_antitagSB_WJets};
  vector<TH1F*> histos_Rpfsingle_J2_WJets = {h_J2M_tagSR_WJets, h_J2M_tagSB_WJets, h_J2M_antitagSR_WJets, h_J2M_antitagSB_WJets};

  vector<TH1F*> histos_Rpf_J1_ZJets = {h_J1M_doubletagSR_ZJets, h_J1M_doubletagSB_ZJets, h_J1M_antitagSR_ZJets, h_J1M_antitagSB_ZJets};
  vector<TH1F*> histos_Rpf_J2_ZJets = {h_J2M_doubletagSR_ZJets, h_J2M_doubletagSB_ZJets, h_J2M_antitagSR_ZJets, h_J2M_antitagSB_ZJets};
  vector<TH1F*> histos_Rpf_J2_mJBins_ZJets = {h_J2M_mjBins_doubletagSR_ZJets, h_J2M_mjBins_doubletagSB_ZJets, h_J2M_mjBins_antitagSR_ZJets, h_J2M_mjBins_antitagSB_ZJets};
  vector<TH1F*> histos_Rpfsingle_J1_ZJets = {h_J1M_tagSR_ZJets, h_J1M_tagSB_ZJets, h_J1M_antitagSR_ZJets, h_J1M_antitagSB_ZJets};
  vector<TH1F*> histos_Rpfsingle_J2_ZJets = {h_J2M_tagSR_ZJets, h_J2M_tagSB_ZJets, h_J2M_antitagSR_ZJets, h_J2M_antitagSB_ZJets};

  vector<TH1F*> histos_j1mass_ZJets = {h_J1M_doubletagSR_ZJets,h_J1M_doubletagSB_ZJets,h_J1M_tagSR_ZJets,h_J1M_tagSB_ZJets, h_J1M_antitagSR_ZJets, h_J1M_antitagSB_ZJets};
  vector<TH1F*> histos_j1mass_WJets = {h_J1M_doubletagSR_WJets,h_J1M_doubletagSB_WJets,h_J1M_tagSR_WJets,h_J1M_tagSB_WJets, h_J1M_antitagSR_WJets, h_J1M_antitagSB_WJets};
  vector<TH1F*> histos_j1mass_SnglT    = {h_J1M_doubletagSR_SnglT,h_J1M_doubletagSB_SnglT,h_J1M_tagSR_SnglT,h_J1M_tagSB_SnglT, h_J1M_antitagSR_SnglT, h_J1M_antitagSB_SnglT};
  vector<TH1F*> histos_j1mass_TT    = {h_J1M_doubletagSR_TT,h_J1M_doubletagSB_TT,h_J1M_tagSR_TT,h_J1M_tagSB_TT, h_J1M_antitagSR_TT, h_J1M_antitagSB_TT};
  vector<TH1F*> histos_j1mass_TT_Di    = {h_J1M_doubletagSR_TT_Di,h_J1M_doubletagSB_TT_Di,h_J1M_tagSR_TT_Di,h_J1M_tagSB_TT_Di, h_J1M_antitagSR_TT_Di, h_J1M_antitagSB_TT_Di};
  vector<TH1F*> histos_j1mass_TT_SL    = {h_J1M_doubletagSR_TT_SL,h_J1M_doubletagSB_TT_SL,h_J1M_tagSR_TT_SL,h_J1M_tagSB_TT_SL, h_J1M_antitagSR_TT_SL, h_J1M_antitagSB_TT_SL};
  vector<TH1F*> histos_j1mass_QCD   = {h_J1M_doubletagSR_QCD,h_J1M_doubletagSB_QCD,h_J1M_tagSR_QCD,h_J1M_tagSB_QCD, h_J1M_antitagSR_QCD, h_J1M_antitagSB_QCD};
  vector<TH1F*> histos_j1mass_T5HH1600 = {h_J1M_doubletagSR_T5HH1600,h_J1M_doubletagSB_T5HH1600,h_J1M_tagSR_T5HH1600,h_J1M_tagSB_T5HH1600, h_J1M_antitagSR_T5HH1600, h_J1M_antitagSB_T5HH1600};
  vector<TH1F*> histos_j1mass_T5HH2000 = {h_J1M_doubletagSR_T5HH2000,h_J1M_doubletagSB_T5HH2000,h_J1M_tagSR_T5HH2000,h_J1M_tagSB_T5HH2000, h_J1M_antitagSR_T5HH2000, h_J1M_antitagSB_T5HH2000};
  vector<TH1F*> histos_j1mass_T5HH2200 = {h_J1M_doubletagSR_T5HH2200,h_J1M_doubletagSB_T5HH2200,h_J1M_tagSR_T5HH2200,h_J1M_tagSB_T5HH2200, h_J1M_antitagSR_T5HH2200, h_J1M_antitagSB_T5HH2200};

  vector<TH1F*> histos_j2mass_ZJets = {h_J2M_doubletagSR_ZJets,h_J2M_doubletagSB_ZJets,h_J2M_tagSR_ZJets,h_J2M_tagSB_ZJets, h_J2M_antitagSR_ZJets, h_J2M_antitagSB_ZJets};
  vector<TH1F*> histos_j2mass_WJets = {h_J2M_doubletagSR_WJets,h_J2M_doubletagSB_WJets,h_J2M_tagSR_WJets,h_J2M_tagSB_WJets, h_J2M_antitagSR_WJets, h_J2M_antitagSB_WJets};
  vector<TH1F*> histos_j2mass_SnglT    = {h_J2M_doubletagSR_SnglT,h_J2M_doubletagSB_SnglT,h_J2M_tagSR_SnglT,h_J2M_tagSB_SnglT, h_J2M_antitagSR_SnglT, h_J2M_antitagSB_SnglT};
  vector<TH1F*> histos_j2mass_TT    = {h_J2M_doubletagSR_TT,h_J2M_doubletagSB_TT,h_J2M_tagSR_TT,h_J2M_tagSB_TT, h_J2M_antitagSR_TT, h_J2M_antitagSB_TT};
  vector<TH1F*> histos_j2mass_TT_Di    = {h_J2M_doubletagSR_TT_Di,h_J2M_doubletagSB_TT_Di,h_J2M_tagSR_TT_Di,h_J2M_tagSB_TT_Di, h_J2M_antitagSR_TT_Di, h_J2M_antitagSB_TT_Di};
  vector<TH1F*> histos_j2mass_TT_SL    = {h_J2M_doubletagSR_TT_SL,h_J2M_doubletagSB_TT_SL,h_J2M_tagSR_TT_SL,h_J2M_tagSB_TT_SL, h_J2M_antitagSR_TT_SL, h_J2M_antitagSB_TT_SL};
  vector<TH1F*> histos_j2mass_QCD   = {h_J2M_doubletagSR_QCD,h_J2M_doubletagSB_QCD,h_J2M_tagSR_QCD,h_J2M_tagSB_QCD, h_J2M_antitagSR_QCD, h_J2M_antitagSB_QCD};
  vector<TH1F*> histos_j2mass_T5HH1600 = {h_J2M_doubletagSR_T5HH1600,h_J2M_doubletagSB_T5HH1600,h_J2M_tagSR_T5HH1600,h_J2M_tagSB_T5HH1600, h_J2M_antitagSR_T5HH1600, h_J2M_antitagSB_T5HH1600};
  vector<TH1F*> histos_j2mass_T5HH2000 = {h_J2M_doubletagSR_T5HH2000,h_J2M_doubletagSB_T5HH2000,h_J2M_tagSR_T5HH2000,h_J2M_tagSB_T5HH2000, h_J2M_antitagSR_T5HH2000, h_J2M_antitagSB_T5HH2000};
  vector<TH1F*> histos_j2mass_T5HH2200 = {h_J2M_doubletagSR_T5HH2200,h_J2M_doubletagSB_T5HH2200,h_J2M_tagSR_T5HH2200,h_J2M_tagSB_T5HH2200, h_J2M_antitagSR_T5HH2200, h_J2M_antitagSB_T5HH2200};

  vector<TH1F*> histos_j1pt_ZJets = {h_J1Pt_doubletagSR_ZJets,h_J1Pt_doubletagSB_ZJets,h_J1Pt_tagSR_ZJets,h_J1Pt_tagSB_ZJets, h_J1Pt_antitagSR_ZJets, h_J1Pt_antitagSB_ZJets};
  vector<TH1F*> histos_j1pt_WJets = {h_J1Pt_doubletagSR_WJets,h_J1Pt_doubletagSB_WJets,h_J1Pt_tagSR_WJets,h_J1Pt_tagSB_WJets, h_J1Pt_antitagSR_WJets, h_J1Pt_antitagSB_WJets};
  vector<TH1F*> histos_j1pt_SnglT    = {h_J1Pt_doubletagSR_SnglT,h_J1Pt_doubletagSB_SnglT,h_J1Pt_tagSR_SnglT,h_J1Pt_tagSB_SnglT, h_J1Pt_antitagSR_SnglT, h_J1Pt_antitagSB_SnglT};
  vector<TH1F*> histos_j1pt_TT    = {h_J1Pt_doubletagSR_TT,h_J1Pt_doubletagSB_TT,h_J1Pt_tagSR_TT,h_J1Pt_tagSB_TT, h_J1Pt_antitagSR_TT, h_J1Pt_antitagSB_TT};
  vector<TH1F*> histos_j1pt_TT_Di    = {h_J1Pt_doubletagSR_TT_Di,h_J1Pt_doubletagSB_TT_Di,h_J1Pt_tagSR_TT_Di,h_J1Pt_tagSB_TT_Di, h_J1Pt_antitagSR_TT_Di, h_J1Pt_antitagSB_TT_Di};
  vector<TH1F*> histos_j1pt_TT_SL    = {h_J1Pt_doubletagSR_TT_SL,h_J1Pt_doubletagSB_TT_SL,h_J1Pt_tagSR_TT_SL,h_J1Pt_tagSB_TT_SL, h_J1Pt_antitagSR_TT_SL, h_J1Pt_antitagSB_TT_SL};
  vector<TH1F*> histos_j1pt_QCD   = {h_J1Pt_doubletagSR_QCD,h_J1Pt_doubletagSB_QCD,h_J1Pt_tagSR_QCD,h_J1Pt_tagSB_QCD, h_J1Pt_antitagSR_QCD, h_J1Pt_antitagSB_QCD};
  vector<TH1F*> histos_j1pt_T5HH1600 = {h_J1Pt_doubletagSR_T5HH1600,h_J1Pt_doubletagSB_T5HH1600,h_J1Pt_tagSR_T5HH1600,h_J1Pt_tagSB_T5HH1600, h_J1Pt_antitagSR_T5HH1600, h_J1Pt_antitagSB_T5HH1600};
  vector<TH1F*> histos_j1pt_T5HH2000 = {h_J1Pt_doubletagSR_T5HH2000,h_J1Pt_doubletagSB_T5HH2000,h_J1Pt_tagSR_T5HH2000,h_J1Pt_tagSB_T5HH2000, h_J1Pt_antitagSR_T5HH2000, h_J1Pt_antitagSB_T5HH2000};
  vector<TH1F*> histos_j1pt_T5HH2200 = {h_J1Pt_doubletagSR_T5HH2200,h_J1Pt_doubletagSB_T5HH2200,h_J1Pt_tagSR_T5HH2200,h_J1Pt_tagSB_T5HH2200, h_J1Pt_antitagSR_T5HH2200, h_J1Pt_antitagSB_T5HH2200};

  vector<TH1F*> histos_j2pt_ZJets = {h_J2Pt_doubletagSR_ZJets,h_J2Pt_doubletagSB_ZJets,h_J2Pt_tagSR_ZJets,h_J2Pt_tagSB_ZJets, h_J2Pt_antitagSR_ZJets, h_J2Pt_antitagSB_ZJets};
  vector<TH1F*> histos_j2pt_WJets = {h_J2Pt_doubletagSR_WJets,h_J2Pt_doubletagSB_WJets,h_J2Pt_tagSR_WJets,h_J2Pt_tagSB_WJets, h_J2Pt_antitagSR_WJets, h_J2Pt_antitagSB_WJets};
  vector<TH1F*> histos_j2pt_SnglT    = {h_J2Pt_doubletagSR_SnglT,h_J2Pt_doubletagSB_SnglT,h_J2Pt_tagSR_SnglT,h_J2Pt_tagSB_SnglT, h_J2Pt_antitagSR_SnglT, h_J2Pt_antitagSB_SnglT};
  vector<TH1F*> histos_j2pt_TT    = {h_J2Pt_doubletagSR_TT,h_J2Pt_doubletagSB_TT,h_J2Pt_tagSR_TT,h_J2Pt_tagSB_TT, h_J2Pt_antitagSR_TT, h_J2Pt_antitagSB_TT};
  vector<TH1F*> histos_j2pt_TT_Di    = {h_J2Pt_doubletagSR_TT_Di,h_J2Pt_doubletagSB_TT_Di,h_J2Pt_tagSR_TT_Di,h_J2Pt_tagSB_TT_Di, h_J2Pt_antitagSR_TT_Di, h_J2Pt_antitagSB_TT_Di};
  vector<TH1F*> histos_j2pt_TT_SL    = {h_J2Pt_doubletagSR_TT_SL,h_J2Pt_doubletagSB_TT_SL,h_J2Pt_tagSR_TT_SL,h_J2Pt_tagSB_TT_SL, h_J2Pt_antitagSR_TT_SL, h_J2Pt_antitagSB_TT_SL};
  vector<TH1F*> histos_j2pt_QCD   = {h_J2Pt_doubletagSR_QCD,h_J2Pt_doubletagSB_QCD,h_J2Pt_tagSR_QCD,h_J2Pt_tagSB_QCD, h_J2Pt_antitagSR_QCD, h_J2Pt_antitagSB_QCD};
  vector<TH1F*> histos_j2pt_T5HH1600 = {h_J2Pt_doubletagSR_T5HH1600,h_J2Pt_doubletagSB_T5HH1600,h_J2Pt_tagSR_T5HH1600,h_J2Pt_tagSB_T5HH1600, h_J2Pt_antitagSR_T5HH1600, h_J2Pt_antitagSB_T5HH1600};
  vector<TH1F*> histos_j2pt_T5HH2000 = {h_J2Pt_doubletagSR_T5HH2000,h_J2Pt_doubletagSB_T5HH2000,h_J2Pt_tagSR_T5HH2000,h_J2Pt_tagSB_T5HH2000, h_J2Pt_antitagSR_T5HH2000, h_J2Pt_antitagSB_T5HH2000};
  vector<TH1F*> histos_j2pt_T5HH2200 = {h_J2Pt_doubletagSR_T5HH2200,h_J2Pt_doubletagSB_T5HH2200,h_J2Pt_tagSR_T5HH2200,h_J2Pt_tagSB_T5HH2200, h_J2Pt_antitagSR_T5HH2200, h_J2Pt_antitagSB_T5HH2200};

  vector<TH1F*> h_METShape_data = {h_A_data,h_A1_data,h_Opt1_data};
  vector<TH1F*> h_METShape_data_1H = {h_A1_data,h_B1_data,h_C_data,h_Opt1_data,h_D_data};
  vector<TH1F*> h_METShape_data_2H = {h_A_data,h_B_data,h_C_data,h_Opt1_data,h_D_data};
  vector<TH1F*> h_METShape_data_all = {h_A_data,h_A1_data,h_B_data,h_B1_data,h_C_data,h_Opt1_data,h_D_data};

  vector<TH1F*> h_METShape_bkgSum = {h_A_sum,h_A1_sum,h_Opt1_sum};
  vector<TH1F*> h_METShape_bkgSum_wData = {h_A_sum,h_A1_sum,h_Opt1_sum,h_Opt1_data};
  vector<TH1F*> h_METShape_bkgSum_1H = {h_A1_sum,h_B1_sum,h_C_sum,h_Opt1_sum,h_D_sum};
  vector<TH1F*> h_METShape_bkgSum_2H = {h_A_sum,h_B_sum,h_C_sum,h_Opt1_sum,h_D_sum};
  vector<TH1F*> h_METShape_sum_all = {h_A_sum,h_A1_sum,h_B_sum,h_B1_sum,h_C_sum,h_Opt1_sum,h_D_sum};
  vector<TH1F*> h_METShape5_bkgSum_2H = {h_A5_sum,h_B5_sum,h_C5_sum,h_5Opt1_sum,h_D5_sum};
  vector<TH1F*> h_METShape5_bkgSum_1H = {h_A15_sum,h_B15_sum,h_C5_sum,h_5Opt1_sum,h_D5_sum};

  vector<TH1F*> h_METShapePhoton_bkgSum_1H = {hP_A1_sum,hP_B1_sum,hP_C_sum,hP_Opt1_sum,hP_D_sum};
  vector<TH1F*> h_METShapePhoton_bkgSum_2H = {hP_A_sum,hP_B_sum,hP_C_sum,hP_Opt1_sum,hP_D_sum};


  vector<TH1F*> h_METShape_SnglT_1H = {h_A1_SnglT,h_B1_SnglT,h_C_SnglT,h_Opt1_SnglT,h_D_SnglT};
  vector<TH1F*> h_METShape_SnglT_2H = {h_A_SnglT,h_B_SnglT,h_C_SnglT,h_Opt1_SnglT,h_D_SnglT};

  vector<TH1F*> h_METShape_TT_1H = {h_A1_TT,h_B1_TT,h_C_TT,h_Opt1_TT,h_D_TT};
  vector<TH1F*> h_METShape_TT_2H = {h_A_TT,h_B_TT,h_C_TT,h_Opt1_TT,h_D_TT};

  vector<TH1F*> h_METShape_ZJets_1H = {h_A1_ZJets,h_B1_ZJets,h_C_ZJets,h_Opt1_ZJets,h_D_ZJets};
  vector<TH1F*> h_METShape_ZJets_2H = {h_A_ZJets,h_B_ZJets,h_C_ZJets,h_Opt1_ZJets,h_D_ZJets};

  vector<TH1F*> h_METShape_WJets_1H = {h_A1_WJets,h_B1_WJets,h_C_WJets,h_Opt1_WJets,h_D_WJets};
  vector<TH1F*> h_METShape_WJets_2H = {h_A_WJets,h_B_WJets,h_C_WJets,h_Opt1_WJets,h_D_WJets};

  vector<TH1F*> h_METShape_GJets_1H = {h_A1_GJets,h_B1_GJets,h_C_GJets,h_Opt1_GJets,h_D_GJets};
  vector<TH1F*> h_METShape_GJets_2H = {h_A_GJets,h_B_GJets,h_C_GJets,h_Opt1_GJets,h_D_GJets};

  vector<TH1F*> h_METShape_QCD_1H = {h_A1_QCD,h_B1_QCD,h_C_QCD,h_Opt1_QCD,h_D_QCD};
  vector<TH1F*> h_METShape_QCD_2H = {h_A_QCD,h_B_QCD,h_C_QCD,h_Opt1_QCD,h_D_QCD};



  vector<TH1F*> vec_H_MET_0l = {h_H_MET_ZJets,h_H_MET_WJets,h_H_MET_QCD,h_H_MET_TT,h_H_MET_SnglT};
  vector<TH1F*> vec_0H_MET_0l = {h_0H_MET_ZJets,h_0H_MET_WJets,h_0H_MET_QCD,h_0H_MET_TT,h_0H_MET_SnglT};
  vector<TH1F*> vec_0Hb_MET_0l = {h_Opt1_ZJets,h_Opt1_WJets,h_Opt1_QCD,h_Opt1_TT,h_Opt1_SnglT};
  vector<TH1F*> vec_1H_MET_0l = {h_1H_MET_ZJets,h_1H_MET_WJets,h_1H_MET_QCD,h_1H_MET_TT,h_1H_MET_SnglT};
  vector<TH1F*> vec_2H_MET_0l = {h_2H_MET_ZJets,h_2H_MET_WJets,h_2H_MET_QCD,h_2H_MET_TT,h_2H_MET_SnglT};
  vector<TH1F*> vec_baseline_MET_0l = {h_baseline_MET_ZJets,h_baseline_MET_WJets,h_baseline_MET_QCD,h_baseline_MET_TT,h_baseline_MET_SnglT};
  vector<TH1F*> vec_baseline_MET_1l = {h_baseline_MET_WJets,h_baseline_MET_TT,h_baseline_MET_SnglT};
  vector<TH1F*> vec_baseline_MET_1p = {h_baseline_MET_QCD,h_baseline_MET_GJets};
  vector<TH1F*> vec_2HSR_MET_0l = {h_A_ZJets,h_A_WJets,h_A_QCD,h_A_TT,h_A_SnglT};
  vector<TH1F*> vec_1HSR_MET_0l = {h_A1_ZJets,h_A1_WJets,h_A1_QCD,h_A1_TT,h_A1_SnglT};


  vector<TH1F*> vec_H_j1mass_0l = {h_H_j1mass_ZJets,h_H_j1mass_WJets,h_H_j1mass_QCD,h_H_j1mass_TT,h_H_j1mass_SnglT};
  vector<TH1F*> vec_0H_j1mass_0l = {h_0H_j1mass_ZJets,h_0H_j1mass_WJets,h_0H_j1mass_QCD,h_0H_j1mass_TT,h_0H_j1mass_SnglT};
  vector<TH1F*> vec_1H_j1mass_0l = {h_1H_j1mass_ZJets,h_1H_j1mass_WJets,h_1H_j1mass_QCD,h_1H_j1mass_TT,h_1H_j1mass_SnglT};
  vector<TH1F*> vec_2H_j1mass_0l = {h_2H_j1mass_ZJets,h_2H_j1mass_WJets,h_2H_j1mass_QCD,h_2H_j1mass_TT,h_2H_j1mass_SnglT};
  vector<TH1F*> vec_baseline_j1mass_0l = {h_baseline_j1mass_ZJets,h_baseline_j1mass_WJets,h_baseline_j1mass_QCD,h_baseline_j1mass_TT,h_baseline_j1mass_SnglT};
  vector<TH1F*> vec_baseline_j1mass_1l = {h_baseline_j1mass_WJets,h_baseline_j1mass_TT,h_baseline_j1mass_SnglT};
  vector<TH1F*> vec_baseline_j1mass_1p = {h_baseline_j1mass_QCD,h_baseline_j1mass_GJets};
  vector<TH1F*> vec_2HSR_j1mass_0l = {h_J1M_doubletagSR_ZJets,h_J1M_doubletagSR_WJets,h_J1M_doubletagSR_QCD,h_J1M_doubletagSR_TT,h_J1M_doubletagSR_SnglT};
  vector<TH1F*> vec_1HSR_j1mass_0l = {h_J1M_tagSR_ZJets,h_J1M_tagSR_WJets,h_J1M_tagSR_QCD,h_J1M_tagSR_TT,h_J1M_tagSR_SnglT};

  vector<TH1F*> vec_H_j2mass_0l = {h_H_j2mass_ZJets,h_H_j2mass_WJets,h_H_j2mass_QCD,h_H_j2mass_TT,h_H_j2mass_SnglT};
  vector<TH1F*> vec_0H_j2mass_0l = {h_0H_j2mass_ZJets,h_0H_j2mass_WJets,h_0H_j2mass_QCD,h_0H_j2mass_TT,h_0H_j2mass_SnglT};
  vector<TH1F*> vec_1H_j2mass_0l = {h_1H_j2mass_ZJets,h_1H_j2mass_WJets,h_1H_j2mass_QCD,h_1H_j2mass_TT,h_1H_j2mass_SnglT};
  vector<TH1F*> vec_2H_j2mass_0l = {h_2H_j2mass_ZJets,h_2H_j2mass_WJets,h_2H_j2mass_QCD,h_2H_j2mass_TT,h_2H_j2mass_SnglT};
  vector<TH1F*> vec_baseline_j2mass_0l = {h_baseline_j2mass_ZJets,h_baseline_j2mass_WJets,h_baseline_j2mass_QCD,h_baseline_j2mass_TT,h_baseline_j2mass_SnglT};
  vector<TH1F*> vec_baseline_j2mass_1l = {h_baseline_j2mass_WJets,h_baseline_j2mass_TT,h_baseline_j2mass_SnglT};
  vector<TH1F*> vec_baseline_j2mass_1p = {h_baseline_j2mass_QCD,h_baseline_j2mass_GJets};
  vector<TH1F*> vec_2HSR_j2mass_0l = {h_J2M_doubletagSR_ZJets,h_J2M_doubletagSR_WJets,h_J2M_doubletagSR_QCD,h_J2M_doubletagSR_TT,h_J2M_doubletagSR_SnglT};
  vector<TH1F*> vec_1HSR_j2mass_0l = {h_J2M_tagSR_ZJets,h_J2M_tagSR_WJets,h_J2M_tagSR_QCD,h_J2M_tagSR_TT,h_J2M_tagSR_SnglT};

  vector<TH1F*> vec_0H_jmass_0l = {h_0H_jmass_ZJets,h_0H_jmass_WJets,h_0H_jmass_QCD,h_0H_jmass_TT,h_0H_jmass_SnglT};
  vector<TH1F*> vec_1H_jmass_0l = {h_1H_jmass_ZJets,h_1H_jmass_WJets,h_1H_jmass_QCD,h_1H_jmass_TT,h_1H_jmass_SnglT};
  vector<TH1F*> vec_2H_jmass_0l = {h_2H_jmass_ZJets,h_2H_jmass_WJets,h_2H_jmass_QCD,h_2H_jmass_TT,h_2H_jmass_SnglT};
  vector<TH1F*> vec_baseline_jmass_0l = {h_baseline_jmass_ZJets,h_baseline_jmass_WJets,h_baseline_jmass_QCD,h_baseline_jmass_TT,h_baseline_jmass_SnglT};

  vector<TH1F*> vec_H_j1pt_0l = {h_H_j1pt_ZJets,h_H_j1pt_WJets,h_H_j1pt_QCD,h_H_j1pt_TT,h_H_j1pt_SnglT};
  vector<TH1F*> vec_0H_j1pt_0l = {h_0H_j1pt_ZJets,h_0H_j1pt_WJets,h_0H_j1pt_QCD,h_0H_j1pt_TT,h_0H_j1pt_SnglT};
  vector<TH1F*> vec_1H_j1pt_0l = {h_1H_j1pt_ZJets,h_1H_j1pt_WJets,h_1H_j1pt_QCD,h_1H_j1pt_TT,h_1H_j1pt_SnglT};
  vector<TH1F*> vec_2H_j1pt_0l = {h_2H_j1pt_ZJets,h_2H_j1pt_WJets,h_2H_j1pt_QCD,h_2H_j1pt_TT,h_2H_j1pt_SnglT};
  vector<TH1F*> vec_baseline_j1pt_0l = {h_baseline_j1pt_ZJets,h_baseline_j1pt_WJets,h_baseline_j1pt_QCD,h_baseline_j1pt_TT,h_baseline_j1pt_SnglT};
  vector<TH1F*> vec_baseline_j1pt_1l = {h_baseline_j1pt_WJets,h_baseline_j1pt_TT,h_baseline_j1pt_SnglT};
  vector<TH1F*> vec_baseline_j1pt_1p = {h_baseline_j1pt_QCD,h_baseline_j1pt_GJets};
  vector<TH1F*> vec_2HSR_j1pt_0l = {h_J1Pt_doubletagSR_ZJets,h_J1Pt_doubletagSR_WJets,h_J1Pt_doubletagSR_QCD,h_J1Pt_doubletagSR_TT,h_J1Pt_doubletagSR_SnglT};
  vector<TH1F*> vec_1HSR_j1pt_0l = {h_J1Pt_tagSR_ZJets,h_J1Pt_tagSR_WJets,h_J1Pt_tagSR_QCD,h_J1Pt_tagSR_TT,h_J1Pt_tagSR_SnglT};

  vector<TH1F*> vec_H_j2pt_0l = {h_H_j2pt_ZJets,h_H_j2pt_WJets,h_H_j2pt_QCD,h_H_j2pt_TT,h_H_j2pt_SnglT};
  vector<TH1F*> vec_0H_j2pt_0l = {h_0H_j2pt_ZJets,h_0H_j2pt_WJets,h_0H_j2pt_QCD,h_0H_j2pt_TT,h_0H_j2pt_SnglT};
  vector<TH1F*> vec_1H_j2pt_0l = {h_1H_j2pt_ZJets,h_1H_j2pt_WJets,h_1H_j2pt_QCD,h_1H_j2pt_TT,h_1H_j2pt_SnglT};
  vector<TH1F*> vec_2H_j2pt_0l = {h_2H_j2pt_ZJets,h_2H_j2pt_WJets,h_2H_j2pt_QCD,h_2H_j2pt_TT,h_2H_j2pt_SnglT};
  vector<TH1F*> vec_baseline_j2pt_0l = {h_baseline_j2pt_ZJets,h_baseline_j2pt_WJets,h_baseline_j2pt_QCD,h_baseline_j2pt_TT,h_baseline_j2pt_SnglT};
  vector<TH1F*> vec_baseline_j2pt_1l = {h_baseline_j2pt_WJets,h_baseline_j2pt_TT,h_baseline_j2pt_SnglT};
  vector<TH1F*> vec_baseline_j2pt_1p = {h_baseline_j2pt_QCD,h_baseline_j2pt_GJets};
  vector<TH1F*> vec_2HSR_j2pt_0l = {h_J2Pt_doubletagSR_ZJets,h_J2Pt_doubletagSR_WJets,h_J2Pt_doubletagSR_QCD,h_J2Pt_doubletagSR_TT,h_J2Pt_doubletagSR_SnglT};
  vector<TH1F*> vec_1HSR_j2pt_0l = {h_J2Pt_tagSR_ZJets,h_J2Pt_tagSR_WJets,h_J2Pt_tagSR_QCD,h_J2Pt_tagSR_TT,h_J2Pt_tagSR_SnglT};

  vector<TH1F*> vec_0H_jpt_0l = {h_0H_jpt_ZJets,h_0H_jpt_WJets,h_0H_jpt_QCD,h_0H_jpt_TT,h_0H_jpt_SnglT};
  vector<TH1F*> vec_1H_jpt_0l = {h_1H_jpt_ZJets,h_1H_jpt_WJets,h_1H_jpt_QCD,h_1H_jpt_TT,h_1H_jpt_SnglT};
  vector<TH1F*> vec_2H_jpt_0l = {h_2H_jpt_ZJets,h_2H_jpt_WJets,h_2H_jpt_QCD,h_2H_jpt_TT,h_2H_jpt_SnglT};
  vector<TH1F*> vec_baseline_jpt_0l = {h_baseline_jpt_ZJets,h_baseline_jpt_WJets,h_baseline_jpt_QCD,h_baseline_jpt_TT,h_baseline_jpt_SnglT};


  vector<TH1F*> vec_H_j1bb_0l = {h_H_j1bb_ZJets,h_H_j1bb_WJets,h_0H_j1bb_QCD,h_H_j1bb_TT,h_H_j1bb_SnglT};
  vector<TH1F*> vec_0H_j1bb_0l = {h_0H_j1bb_ZJets,h_0H_j1bb_WJets,h_0H_j1bb_QCD,h_0H_j1bb_TT,h_0H_j1bb_SnglT};
  vector<TH1F*> vec_1H_j1bb_0l = {h_1H_j1bb_ZJets,h_1H_j1bb_WJets,h_1H_j1bb_QCD,h_1H_j1bb_TT,h_1H_j1bb_SnglT};
  vector<TH1F*> vec_2H_j1bb_0l = {h_2H_j1bb_ZJets,h_2H_j1bb_WJets,h_2H_j1bb_QCD,h_2H_j1bb_TT,h_2H_j1bb_SnglT};
  vector<TH1F*> vec_baseline_j1bb_0l = {h_baseline_j1bb_ZJets,h_baseline_j1bb_WJets,h_baseline_j1bb_QCD,h_baseline_j1bb_TT,h_baseline_j1bb_SnglT};
  vector<TH1F*> vec_baseline_j1bb_1l = {h_baseline_j1bb_WJets,h_baseline_j1bb_TT,h_baseline_j1bb_SnglT};
  vector<TH1F*> vec_baseline_j1bb_1p = {h_baseline_j1bb_QCD,h_baseline_j1bb_GJets};
  vector<TH1F*> vec_2HSR_j1bb_0l = {h_J1BB_doubletagSR_ZJets,h_J1BB_doubletagSR_WJets,h_J1BB_doubletagSR_QCD,h_J1BB_doubletagSR_TT,h_J1BB_doubletagSR_SnglT};
  vector<TH1F*> vec_1HSR_j1bb_0l = {h_J1BB_tagSR_ZJets,h_J1BB_tagSR_WJets,h_J1BB_tagSR_QCD,h_J1BB_tagSR_TT,h_J1BB_tagSR_SnglT};


  vector<TH1F*> vec_H_j2bb_0l = {h_H_j2bb_ZJets,h_H_j2bb_WJets,h_H_j2bb_QCD,h_H_j2bb_TT,h_H_j2bb_SnglT};
  vector<TH1F*> vec_0H_j2bb_0l = {h_0H_j2bb_ZJets,h_0H_j2bb_WJets,h_0H_j2bb_QCD,h_0H_j2bb_TT,h_0H_j2bb_SnglT};
  vector<TH1F*> vec_1H_j2bb_0l = {h_1H_j2bb_ZJets,h_1H_j2bb_WJets,h_1H_j2bb_QCD,h_1H_j2bb_TT,h_1H_j2bb_SnglT};
  vector<TH1F*> vec_2H_j2bb_0l = {h_2H_j2bb_ZJets,h_2H_j2bb_WJets,h_2H_j2bb_QCD,h_2H_j2bb_TT,h_2H_j2bb_SnglT};
  vector<TH1F*> vec_baseline_j2bb_0l = {h_baseline_j2bb_ZJets,h_baseline_j2bb_WJets,h_baseline_j2bb_QCD,h_baseline_j2bb_TT,h_baseline_j2bb_SnglT};
  vector<TH1F*> vec_baseline_j2bb_1l = {h_baseline_j2bb_WJets,h_baseline_j2bb_TT,h_baseline_j2bb_SnglT};
  vector<TH1F*> vec_baseline_j2bb_1p = {h_baseline_j2bb_QCD,h_baseline_j2bb_GJets};
  vector<TH1F*> vec_2HSR_j2bb_0l = {h_J2BB_doubletagSR_ZJets,h_J2BB_doubletagSR_WJets,h_J2BB_doubletagSR_QCD,h_J2BB_doubletagSR_TT,h_J2BB_doubletagSR_SnglT};
  vector<TH1F*> vec_1HSR_j2bb_0l = {h_J2BB_tagSR_ZJets,h_J2BB_tagSR_WJets,h_J2BB_tagSR_QCD,h_J2BB_tagSR_TT,h_J2BB_tagSR_SnglT};


  vector<TH1F*> vec_0H_jbb_0l = {h_0H_jbb_ZJets,h_0H_jbb_WJets,h_0H_jbb_QCD,h_0H_jbb_TT,h_0H_jbb_SnglT};
  vector<TH1F*> vec_1H_jbb_0l = {h_1H_jbb_ZJets,h_1H_jbb_WJets,h_1H_jbb_QCD,h_1H_jbb_TT,h_1H_jbb_SnglT};
  vector<TH1F*> vec_2H_jbb_0l = {h_2H_jbb_ZJets,h_2H_jbb_WJets,h_2H_jbb_QCD,h_2H_jbb_TT,h_2H_jbb_SnglT};
  vector<TH1F*> vec_baseline_jbb_0l = {h_baseline_jbb_ZJets,h_baseline_jbb_WJets,h_baseline_jbb_QCD,h_baseline_jbb_TT,h_baseline_jbb_SnglT};


  if (runMassCorrelations){
    std::cout<<"Running mass correlations..."<<std::endl;
    massCorrelations(histos_allRegions_sum,histos_allRegions_data,"BkgSum");
  }

  if (runPies){
    std::cout<<"Running pies..."<<std::endl;

    if (whichRegion=="signal"){
      pieChart({h_baseline_MET_QCD}, {h_baseline_MET_WJets}, {h_baseline_MET_ZJets}, {h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "all");
      pieChart({h_A_QCD,h_B_QCD}, {h_A_WJets,h_B_WJets}, {h_A_ZJets,h_B_ZJets}, {h_A_TT,h_B_TT},{h_A_SnglT,h_B_SnglT}, "2H", "all");
      pieChart({h_A_QCD}, {h_A_WJets}, {h_A_ZJets}, {h_A_TT},{h_A_SnglT}, "2HSR", "all");
      pieChart({h_B_QCD}, {h_B_WJets}, {h_B_ZJets}, {h_B_TT},{h_B_SnglT}, "2HSB", "all");
      pieChart({h_A1_QCD,h_B1_QCD}, {h_A1_WJets,h_B1_WJets}, {h_A1_ZJets,h_B1_ZJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "all");
      pieChart({h_A1_QCD}, {h_A1_WJets}, {h_A1_ZJets}, {h_A1_TT},{h_A1_SnglT}, "1HSR", "all");
      pieChart({h_B1_QCD}, {h_B1_WJets}, {h_B1_ZJets}, {h_B1_TT},{h_B1_SnglT}, "1HSB", "all");
      pieChart({h_C_QCD,h_D_QCD}, {h_C_WJets,h_D_WJets}, {h_C_ZJets,h_D_ZJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_D_SnglT}, "0H", "all");
      pieChart({h_C_QCD}, {h_C_WJets}, {h_C_ZJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "all");
      pieChart({h_D_QCD}, {h_D_WJets}, {h_D_ZJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "all");
      pieChart({h_Opt1_QCD}, {h_Opt1_WJets}, {h_Opt1_ZJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "all");

      pieChart({h_baseline_MET_QCD}, {h_baseline_MET_WJets}, {h_baseline_MET_ZJets}, {h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "bin1");
      pieChart({h_A_QCD,h_B_QCD}, {h_A_WJets,h_B_WJets}, {h_A_ZJets,h_B_ZJets}, {h_A_TT,h_B_TT},{h_A_SnglT,h_B_SnglT}, "2H", "bin1");
      pieChart({h_A_QCD}, {h_A_WJets}, {h_A_ZJets}, {h_A_TT},{h_A_SnglT}, "2HSR", "bin1");
      pieChart({h_B_QCD}, {h_B_WJets}, {h_B_ZJets}, {h_B_TT},{h_B_SnglT}, "2HSB", "bin1");
      pieChart({h_A1_QCD,h_B1_QCD}, {h_A1_WJets,h_B1_WJets}, {h_A1_ZJets,h_B1_ZJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "bin1");
      pieChart({h_A1_QCD}, {h_A1_WJets}, {h_A1_ZJets}, {h_A1_TT},{h_A1_SnglT}, "1HSR", "bin1");
      pieChart({h_B1_QCD}, {h_B1_WJets}, {h_B1_ZJets}, {h_B1_TT},{h_B1_SnglT}, "1HSB", "bin1");
      pieChart({h_C_QCD,h_D_QCD}, {h_C_WJets,h_D_WJets}, {h_C_ZJets,h_D_ZJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_D_SnglT}, "0H", "bin1");
      pieChart({h_C_QCD}, {h_C_WJets}, {h_C_ZJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "bin1");
      pieChart({h_D_QCD}, {h_D_WJets}, {h_D_ZJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "bin1");
      pieChart({h_Opt1_QCD}, {h_Opt1_WJets}, {h_Opt1_ZJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "bin1");

      pieChart({h_baseline_MET_QCD}, {h_baseline_MET_WJets}, {h_baseline_MET_ZJets}, {h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "bin2");
      pieChart({h_A_QCD,h_B_QCD}, {h_A_WJets,h_B_WJets}, {h_A_ZJets,h_B_ZJets}, {h_A_TT,h_B_TT},{h_A_SnglT,h_B_SnglT}, "2H", "bin2");
      pieChart({h_A_QCD}, {h_A_WJets}, {h_A_ZJets}, {h_A_TT},{h_A_SnglT}, "2HSR", "bin2");
      pieChart({h_B_QCD}, {h_B_WJets}, {h_B_ZJets}, {h_B_TT},{h_B_SnglT}, "2HSB", "bin2");
      pieChart({h_A1_QCD,h_B1_QCD}, {h_A1_WJets,h_B1_WJets}, {h_A1_ZJets,h_B1_ZJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "bin2");
      pieChart({h_A1_QCD}, {h_A1_WJets}, {h_A1_ZJets}, {h_A1_TT},{h_A1_SnglT}, "1HSR", "bin2");
      pieChart({h_B1_QCD}, {h_B1_WJets}, {h_B1_ZJets}, {h_B1_TT},{h_B1_SnglT}, "1HSB", "bin2");
      pieChart({h_C_QCD,h_D_QCD}, {h_C_WJets,h_D_WJets}, {h_C_ZJets,h_D_ZJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_D_SnglT}, "0H", "bin2");
      pieChart({h_C_QCD}, {h_C_WJets}, {h_C_ZJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "bin2");
      pieChart({h_D_QCD}, {h_D_WJets}, {h_D_ZJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "bin2");
      pieChart({h_Opt1_QCD}, {h_Opt1_WJets}, {h_Opt1_ZJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "bin2");

      pieChart({h_baseline_MET_QCD}, {h_baseline_MET_WJets}, {h_baseline_MET_ZJets}, {h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "bin3");
      pieChart({h_A_QCD,h_B_QCD}, {h_A_WJets,h_B_WJets}, {h_A_ZJets,h_B_ZJets}, {h_A_TT,h_B_TT},{h_A_SnglT,h_B_SnglT}, "2H", "bin3");
      pieChart({h_A_QCD}, {h_A_WJets}, {h_A_ZJets}, {h_A_TT},{h_A_SnglT}, "2HSR", "bin3");
      pieChart({h_B_QCD}, {h_B_WJets}, {h_B_ZJets}, {h_B_TT},{h_B_SnglT}, "2HSB", "bin3");
      pieChart({h_A1_QCD,h_B1_QCD}, {h_A1_WJets,h_B1_WJets}, {h_A1_ZJets,h_B1_ZJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "bin3");
      pieChart({h_A1_QCD}, {h_A1_WJets}, {h_A1_ZJets}, {h_A1_TT},{h_A1_SnglT}, "1HSR", "bin3");
      pieChart({h_B1_QCD}, {h_B1_WJets}, {h_B1_ZJets}, {h_B1_TT},{h_B1_SnglT}, "1HSB", "bin3");
      pieChart({h_C_QCD,h_D_QCD}, {h_C_WJets,h_D_WJets}, {h_C_ZJets,h_D_ZJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_D_SnglT}, "0H", "bin3");
      pieChart({h_C_QCD}, {h_C_WJets}, {h_C_ZJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "bin3");
      pieChart({h_D_QCD}, {h_D_WJets}, {h_D_ZJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "bin3");
      pieChart({h_Opt1_QCD}, {h_Opt1_WJets}, {h_Opt1_ZJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "bin3");
    }
    else if (whichRegion=="singleLept"){
      pieChart1l({h_baseline_MET_WJets},{h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "all");
      pieChart1l({h_A_WJets,h_B_WJets}, {h_A_TT,h_B_TT}, {h_A_SnglT,h_B_SnglT}, "2H", "all");
      pieChart1l({h_A_WJets}, {h_A_TT}, {h_A_SnglT}, "2HSR", "all");
      pieChart1l({h_B_WJets},{h_B_TT}, {h_B_SnglT}, "2HSB", "all");
      pieChart1l({h_A1_WJets,h_B1_WJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "all");
      pieChart1l({h_A1_WJets}, {h_A1_TT}, {h_A1_SnglT}, "1HSR", "all");
      pieChart1l({h_B1_WJets}, {h_B1_TT}, {h_B1_SnglT}, "1HSB", "all");
      pieChart1l({h_C_WJets,h_D_WJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_C_SnglT}, "0H", "all");
      pieChart1l({h_C_WJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "all");
      pieChart1l({h_D_WJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "all");
      pieChart1l({h_Opt1_WJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "all");

      pieChart1l({h_baseline_MET_WJets},{h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "bin1");
      pieChart1l({h_A_WJets,h_B_WJets}, {h_A_TT,h_B_TT}, {h_A_SnglT,h_B_SnglT}, "2H", "bin1");
      pieChart1l({h_A_WJets}, {h_A_TT}, {h_A_SnglT}, "2HSR", "bin1");
      pieChart1l({h_B_WJets},{h_B_TT}, {h_B_SnglT}, "2HSB", "bin1");
      pieChart1l({h_A1_WJets,h_B1_WJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "bin1");
      pieChart1l({h_A1_WJets}, {h_A1_TT}, {h_A1_SnglT}, "1HSR", "bin1");
      pieChart1l({h_B1_WJets}, {h_B1_TT}, {h_B1_SnglT}, "1HSB", "bin1");
      pieChart1l({h_C_WJets,h_D_WJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_C_SnglT}, "0H", "bin1");
      pieChart1l({h_C_WJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "bin1");
      pieChart1l({h_D_WJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "bin1");
      pieChart1l({h_Opt1_WJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "bin1");

      pieChart1l({h_baseline_MET_WJets},{h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "bin2");
      pieChart1l({h_A_WJets,h_B_WJets}, {h_A_TT,h_B_TT}, {h_A_SnglT,h_B_SnglT}, "2H", "bin2");
      pieChart1l({h_A_WJets}, {h_A_TT}, {h_A_SnglT}, "2HSR", "bin2");
      pieChart1l({h_B_WJets},{h_B_TT}, {h_B_SnglT}, "2HSB", "bin2");
      pieChart1l({h_A1_WJets,h_B1_WJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "bin2");
      pieChart1l({h_A1_WJets}, {h_A1_TT}, {h_A1_SnglT}, "1HSR", "bin2");
      pieChart1l({h_B1_WJets}, {h_B1_TT}, {h_B1_SnglT}, "1HSB", "bin2");
      pieChart1l({h_C_WJets,h_D_WJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_C_SnglT}, "0H", "bin2");
      pieChart1l({h_C_WJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "bin2");
      pieChart1l({h_D_WJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "bin2");
      pieChart1l({h_Opt1_WJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "bin2");

      pieChart1l({h_baseline_MET_WJets},{h_baseline_MET_TT},{h_baseline_MET_SnglT}, "Baseline", "bin3");
      pieChart1l({h_A_WJets,h_B_WJets}, {h_A_TT,h_B_TT}, {h_A_SnglT,h_B_SnglT}, "2H", "bin3");
      pieChart1l({h_A_WJets}, {h_A_TT}, {h_A_SnglT}, "2HSR", "bin3");
      pieChart1l({h_B_WJets},{h_B_TT}, {h_B_SnglT}, "2HSB", "bin3");
      pieChart1l({h_A1_WJets,h_B1_WJets}, {h_A1_TT,h_B1_TT}, {h_A1_SnglT,h_B1_SnglT}, "1H", "bin3");
      pieChart1l({h_A1_WJets}, {h_A1_TT}, {h_A1_SnglT}, "1HSR", "bin3");
      pieChart1l({h_B1_WJets}, {h_B1_TT}, {h_B1_SnglT}, "1HSB", "bin3");
      pieChart1l({h_C_WJets,h_D_WJets}, {h_C_TT,h_D_TT}, {h_C_SnglT,h_C_SnglT}, "0H", "bin3");
      pieChart1l({h_C_WJets}, {h_C_TT}, {h_C_SnglT}, "0HSR", "bin3");
      pieChart1l({h_D_WJets}, {h_D_TT}, {h_D_SnglT}, "0HSB", "bin3");
      pieChart1l({h_Opt1_WJets}, {h_Opt1_TT}, {h_Opt1_SnglT}, "0Hb", "bin3");
    }

    else if (whichRegion=="photon"){ //GJets, QCD
      pieChartPhoton({h_baseline_MET_GJets},{h_baseline_MET_QCD}, "Baseline", "all");
      pieChartPhoton({h_A_GJets,h_B_GJets}, {h_A_QCD,h_B_QCD}, "2H", "all");
      pieChartPhoton({h_A_GJets}, {h_A_QCD}, "2HSR", "all");
      pieChartPhoton({h_B_GJets},{h_B_QCD},"2HSB", "all");
      pieChartPhoton({h_A1_GJets,h_B1_GJets}, {h_A1_QCD,h_B1_QCD}, "1H", "all");
      pieChartPhoton({h_A1_GJets}, {h_A1_QCD}, "1HSR", "all");
      pieChartPhoton({h_B1_GJets}, {h_B1_QCD}, "1HSB", "all");
      pieChartPhoton({h_C_GJets,h_D_GJets}, {h_C_QCD,h_D_QCD}, "0H", "all");
      pieChartPhoton({h_C_GJets}, {h_C_QCD}, "0HSR", "all");
      pieChartPhoton({h_D_GJets}, {h_D_QCD}, "0HSB", "all");
      pieChartPhoton({h_Opt1_GJets}, {h_Opt1_QCD}, "0Hb", "all");

      pieChartPhoton({h_baseline_MET_GJets},{h_baseline_MET_QCD}, "Baseline", "bin1");
      pieChartPhoton({h_A_GJets,h_B_GJets}, {h_A_QCD,h_B_QCD}, "2H", "bin1");
      pieChartPhoton({h_A_GJets}, {h_A_QCD}, "2HSR", "bin1");
      pieChartPhoton({h_B_GJets},{h_B_QCD}, "2HSB", "bin1");
      pieChartPhoton({h_A1_GJets,h_B1_GJets}, {h_A1_QCD,h_B1_QCD}, "1H", "bin1");
      pieChartPhoton({h_A1_GJets}, {h_A1_QCD}, "1HSR", "bin1");
      pieChartPhoton({h_B1_GJets}, {h_B1_QCD}, "1HSB", "bin1");
      pieChartPhoton({h_C_GJets,h_D_GJets}, {h_C_QCD,h_D_QCD}, "0H", "bin1");
      pieChartPhoton({h_C_GJets}, {h_C_QCD}, "0HSR", "bin1");
      pieChartPhoton({h_D_GJets}, {h_D_QCD}, "0HSB", "bin1");
      pieChartPhoton({h_Opt1_GJets}, {h_Opt1_QCD}, "0Hb", "bin1");

      pieChartPhoton({h_baseline_MET_GJets},{h_baseline_MET_QCD}, "Baseline", "bin2");
      pieChartPhoton({h_A_GJets,h_B_GJets}, {h_A_QCD,h_B_QCD}, "2H", "bin2");
      pieChartPhoton({h_A_GJets}, {h_A_QCD}, "2HSR", "bin2");
      pieChartPhoton({h_B_GJets},{h_B_QCD}, "2HSB", "bin2");
      pieChartPhoton({h_A1_GJets,h_B1_GJets}, {h_A1_QCD,h_B1_QCD}, "1H", "bin2");
      pieChartPhoton({h_A1_GJets}, {h_A1_QCD}, "1HSR", "bin2");
      pieChartPhoton({h_B1_GJets}, {h_B1_QCD}, "1HSB", "bin2");
      pieChartPhoton({h_C_GJets,h_D_GJets}, {h_C_QCD,h_D_QCD}, "0H", "bin2");
      pieChartPhoton({h_C_GJets}, {h_C_QCD}, "0HSR", "bin2");
      pieChartPhoton({h_D_GJets}, {h_D_QCD}, "0HSB", "bin2");
      pieChartPhoton({h_Opt1_GJets}, {h_Opt1_QCD}, "0Hb", "bin2");

      pieChartPhoton({h_baseline_MET_GJets},{h_baseline_MET_QCD}, "Baseline", "bin3");
      pieChartPhoton({h_A_GJets,h_B_GJets}, {h_A_QCD,h_B_QCD}, "2H", "bin3");
      pieChartPhoton({h_A_GJets}, {h_A_QCD},  "2HSR", "bin3");
      pieChartPhoton({h_B_GJets},{h_B_QCD},  "2HSB", "bin3");
      pieChartPhoton({h_A1_GJets,h_B1_GJets}, {h_A1_QCD,h_B1_QCD}, "1H", "bin3");
      pieChartPhoton({h_A1_GJets}, {h_A1_QCD}, "1HSR", "bin3");
      pieChartPhoton({h_B1_GJets}, {h_B1_QCD}, "1HSB", "bin3");
      pieChartPhoton({h_C_GJets,h_D_GJets}, {h_C_QCD,h_D_QCD}, "0H", "bin3");
      pieChartPhoton({h_C_GJets}, {h_C_QCD}, "0HSR", "bin3");
      pieChartPhoton({h_D_GJets}, {h_D_QCD}, "0HSB", "bin3");
      pieChartPhoton({h_Opt1_GJets}, {h_Opt1_QCD}, "0Hb", "bin3");
    }
  }


  if (runFullBkg) {
    std::cout<<"Running full bkg closure..."<<std::endl;

    makeFullBkgClosure(histos_ABCD_sum, "BkgSum", "Double",year);
    makeFullBkgClosure(histos_A1B1CD_sum, "BkgSum", "Single",year);

    // if (whichRegion!="signal") {
    makeFullBkgClosure(histos_ABCD_data, "Data", "Double",year);
    makeFullBkgClosure(histos_A1B1CD_data, "Data", "Single",year);
    // }
    // tableOfFullPred(histos_allRegions_sum,"BkgSum");
    // tableOfFullPred(histos_allRegions_data,"data");
  }

  if (runABCDPlots){
    std::cout<<"Running ABCD plots..."<<std::endl;

    if (whichRegion=="signal"){
      makeABCDPlot(histos_ABCD_sum, "BkgSum", "Double",year);
      makeABCDPlot(histos_A1B1CD_sum, "BkgSum", "Single",year);

      makeABCDPlot(histos_ABCD_QCD, "QCD", "Double",year);
      makeABCDPlot(histos_ABCD_SnglT, "SnglT", "Double",year);
      makeABCDPlot(histos_ABCD_TT, "TT", "Double",year);
      makeABCDPlot(histos_ABCD_WJets, "WJets", "Double",year);
      makeABCDPlot(histos_ABCD_ZJets, "ZJets", "Double",year);

      makeABCDPlot(histos_A1B1CD_QCD, "QCD", "Single",year);
      makeABCDPlot(histos_A1B1CD_SnglT, "SnglT", "Single",year);
      makeABCDPlot(histos_A1B1CD_TT, "TT", "Single",year);
      makeABCDPlot(histos_A1B1CD_WJets, "WJets", "Single",year);
      makeABCDPlot(histos_A1B1CD_ZJets, "ZJets", "Single",year);
    }

    else if (whichRegion=="singleLept"){
      makeABCDPlot(histos_ABCD_sum, "BkgSum", "Double",year);
      makeABCDPlot(histos_A1B1CD_sum, "BkgSum", "Single",year);

      makeABCDPlot(histos_ABCD_data, "Data", "Double",year);
      makeABCDPlot(histos_A1B1CD_data, "Data", "Single",year);

      makeABCDPlot(histos_ABCD_SnglT, "SnglT", "Double",year);
      makeABCDPlot(histos_ABCD_TT, "TT", "Double",year);
      makeABCDPlot(histos_ABCD_WJets, "WJets", "Double",year);

      makeABCDPlot(histos_A1B1CD_SnglT, "SnglT", "Single",year);
      makeABCDPlot(histos_A1B1CD_TT, "TT", "Single",year);
      makeABCDPlot(histos_A1B1CD_WJets, "WJets", "Single",year);
    }
    else if (whichRegion=="photon"){
      // makeABCDPlot(histosPhoton_ABCD_sum, "BkgSum", "Double",year);
      // makeABCDPlot(histosPhoton_A1B1CD_sum, "BkgSum", "Single",year);

      makeABCDPlot(histos_ABCD_sum, "BkgSum", "Double",year);
      makeABCDPlot(histos_A1B1CD_sum, "BkgSum", "Single",year);

      makeABCDPlot(histos_ABCD_data, "Data", "Double",year);
      makeABCDPlot(histos_A1B1CD_data, "Data", "Single",year);

      makeABCDPlot(histos_ABCD_QCD, "QCD", "Double",year);
      makeABCDPlot(histos_ABCD_GJets, "GJets", "Double",year);
      makeABCDPlot(histos_A1B1CD_QCD, "QCD", "Single",year);
      makeABCDPlot(histos_A1B1CD_GJets, "GJets", "Single",year);
    }
  }

  if (runRPFPlots){
    std::cout<<"Running RPF plots..."<<std::endl;

    if (whichRegion=="signal"){
      //J1+J2
      makeRpfPlot(histos_Rpf_J1J2_sum, "BkgSum", "Both", "Double");
      makeRpfPlot(histos_Rpfsingle_J1J2_sum, "BkgSum", "Both", "Single");

      //J1 Double tag region
      makeRpfPlot(histos_Rpf_J1_sum, "BkgSum", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_QCD, "QCD", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_SnglT, "SnglT", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_TT, "TT", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_WJets, "WJets", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_ZJets, "ZJets", "J1", "Double");

      //J1 Single tag region
      makeRpfPlot(histos_Rpfsingle_J1_sum, "BkgSum", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_QCD, "QCD", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_SnglT, "SnglT", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_TT, "TT", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_WJets, "WJets", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_ZJets, "ZJets", "J1", "Single");

      //J2 double tag region
      makeRpfPlot(histos_Rpf_J2_sum, "BkgSum", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_QCD, "QCD", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_SnglT, "SnglT", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_TT, "TT", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_WJets, "WJets", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_ZJets, "ZJets", "J2", "Double");

      //J2 double tag region, with three mass bins
      // makeRpfPlot(histos_Rpf_J2_mJBins_sum, "BkgSum_mJ", "J2", "Double");
      // makeRpfPlot(histos_Rpf_J2_mJBins_QCD, "QCD_mJ", "J2", "Double");
      // makeRpfPlot(histos_Rpf_J2_mJBins_SnglT, "SnglT_mJ", "J2", "Double");
      // makeRpfPlot(histos_Rpf_J2_mJBins_TT, "TT_mJ", "J2", "Double");
      // makeRpfPlot(histos_Rpf_J2_mJBins_WJets, "WJets_mJ", "J2", "Double");
      // makeRpfPlot(histos_Rpf_J2_mJBins_ZJets, "ZJets_mJ", "J2", "Double");


      //J2 single tag region
      makeRpfPlot(histos_Rpfsingle_J2_sum, "BkgSum", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_QCD, "QCD", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_SnglT, "SnglT", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_TT, "TT", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_WJets, "WJets", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_ZJets, "ZJets", "J2", "Single");
    }
    if (whichRegion=="singleLept"){
      //J1+J2
      makeRpfPlot(histos_Rpf_J1J2_sum, "BkgSum", "Both", "Double");
      makeRpfPlot(histos_Rpfsingle_J1J2_sum, "BkgSum", "Both", "Single");


      //J1 Double tag region
      makeRpfPlot(histos_Rpf_J1_sum, "BkgSum", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_SnglT, "SnglT", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_TT, "TT", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_WJets, "WJets", "J1", "Double");

      //J1 Single tag region
      makeRpfPlot(histos_Rpfsingle_J1_sum, "BkgSum", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_SnglT, "SnglT", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_TT, "TT", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_WJets, "WJets", "J1", "Single");

      //J2 double tag region
      makeRpfPlot(histos_Rpf_J2_sum, "BkgSum", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_SnglT, "SnglT", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_TT, "TT", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_WJets, "WJets", "J2", "Double");

      //J2 single tag region
      makeRpfPlot(histos_Rpfsingle_J2_sum, "BkgSum", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_SnglT, "SnglT", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_TT, "TT", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_WJets, "WJets", "J2", "Single");

      //J2 double tag region, with three mass bins
      makeRpfPlot(histos_Rpf_J2_mJBins_sum, "BkgSum_mJ", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_mJBins_SnglT, "SnglT_mJ", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_mJBins_TT, "TT_mJ", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_mJBins_WJets, "WJets_mJ", "J2", "Double");
    }
    if (whichRegion=="photon"){
      //J1 Double tag region
      makeRpfPlot(histos_Rpf_J1_sum, "BkgSum", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_QCD, "QCD", "J1", "Double");
      makeRpfPlot(histos_Rpf_J1_GJets, "GJets", "J1", "Double");

      //J1 Single tag region
      makeRpfPlot(histos_Rpfsingle_J1_sum, "BkgSum", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_QCD, "QCD", "J1", "Single");
      makeRpfPlot(histos_Rpfsingle_J1_GJets, "GJets", "J1", "Single");

      //J2 double tag region
      makeRpfPlot(histos_Rpf_J2_sum, "BkgSum", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_QCD, "QCD", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_GJets, "GJets", "J2", "Double");

      //J2 single tag region
      makeRpfPlot(histos_Rpfsingle_J2_sum, "BkgSum", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_QCD, "QCD", "J2", "Single");
      makeRpfPlot(histos_Rpfsingle_J2_GJets, "GJets", "J2", "Single");

      //J2 double tag region, with three mass bins
      makeRpfPlot(histos_Rpf_J2_mJBins_sum, "BkgSum_mJ", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_mJBins_QCD, "QCD_mJ", "J2", "Double");
      makeRpfPlot(histos_Rpf_J2_mJBins_GJets, "GJets_mJ", "J2", "Double");
    }
  }


  if (runStacks && whichRegion=="signal") {
    std::cout<<"Running stacks..."<<std::endl;

    // makeStackPlot(h_baseline_j1mass_QCD,h_baseline_j1mass_TT,h_baseline_j1mass_WJets,h_baseline_j1mass_ZJets,h_baseline_j1mass_SnglT,h_baseline_j1mass_T5HH1600,h_baseline_j1mass_T5HH2000,h_baseline_j1mass_T5HH2200,h_baseline_j1mass_TChiHH500,h_baseline_j1mass_TChiHH800,h_baseline_j1mass_TChiHH1000,"j1m");
    // makeStackPlot(h_baseline_j2mass_QCD,h_baseline_j2mass_TT,h_baseline_j2mass_WJets,h_baseline_j2mass_ZJets,h_baseline_j2mass_SnglT,h_baseline_j2mass_T5HH1600,h_baseline_j2mass_T5HH2000,h_baseline_j2mass_T5HH2200,h_baseline_j2mass_TChiHH500,h_baseline_j2mass_TChiHH800,h_baseline_j2mass_TChiHH1000,"j2m");
    // makeStackPlot(h_baseline_j1pt_QCD,h_baseline_j1pt_TT,h_baseline_j1pt_WJets,h_baseline_j1pt_ZJets,h_baseline_j1pt_SnglT,h_baseline_j1pt_T5HH1600,h_baseline_j1pt_T5HH2000,h_baseline_j1pt_T5HH2200,h_baseline_j1pt_TChiHH500,h_baseline_j1pt_TChiHH800,h_baseline_j1pt_TChiHH1000,"j1pt");
    // makeStackPlot(h_baseline_j2pt_QCD,h_baseline_j2pt_TT,h_baseline_j2pt_WJets,h_baseline_j2pt_ZJets,h_baseline_j2pt_SnglT,h_baseline_j2pt_T5HH1600,h_baseline_j2pt_T5HH2000,h_baseline_j2pt_T5HH2200,h_baseline_j2pt_TChiHH500,h_baseline_j2pt_TChiHH800,h_baseline_j2pt_TChiHH1000,"j2pt");
    // makeStackPlot(h_baseline_j1bb_QCD,h_baseline_j1bb_TT,h_baseline_j1bb_WJets,h_baseline_j1bb_ZJets,h_baseline_j1bb_SnglT,h_baseline_j1bb_T5HH1600,h_baseline_j1bb_T5HH2000,h_baseline_j1bb_T5HH2200,h_baseline_j1bb_TChiHH500,h_baseline_j1bb_TChiHH800,h_baseline_j1bb_TChiHH1000,"j1bb");
    // makeStackPlot(h_baseline_j2bb_QCD,h_baseline_j2bb_TT,h_baseline_j2bb_WJets,h_baseline_j2bb_ZJets,h_baseline_j2bb_SnglT,h_baseline_j2bb_T5HH1600,h_baseline_j2bb_T5HH2000,h_baseline_j2bb_T5HH2200,h_baseline_j2bb_TChiHH500,h_baseline_j2bb_TChiHH800,h_baseline_j2bb_TChiHH1000,"j2bb");


    // makeMETStack(h_baseline_MET_QCD, h_baseline_MET_TT,h_baseline_MET_WJets,h_baseline_MET_ZJets, h_baseline_MET_SnglT, h_baseline_MET_T5HH1600,h_baseline_MET_T5HH2000,h_baseline_MET_T5HH2200,h_baseline_MET_TChiHH500,h_baseline_MET_TChiHH800,h_baseline_MET_TChiHH1000,"baseline");
    makeMETStack(h_A_QCD, h_A_TT,h_A_WJets,h_A_ZJets, h_A_SnglT, h_A_T5HH1600,h_A_T5HH2000,h_A_T5HH2200,h_A_TChiHH500,h_A_TChiHH800,h_A_TChiHH1000,"2HSR");
    makeMETStack(h_A1_QCD, h_A1_TT,h_A1_WJets,h_A1_ZJets, h_A1_SnglT, h_A1_T5HH1600,h_A1_T5HH2000,h_A1_T5HH2200,h_A1_TChiHH500,h_A1_TChiHH800,h_A1_TChiHH1000,"1HSR");

  }

  if (runStacks && whichRegion=="singleLept") {
    std::cout<<"Running stacks..."<<std::endl;
    makeSingleLeptStackPlot(h_baseline_j1mass_TT,h_baseline_j1mass_WJets,h_baseline_j1mass_SnglT,h_baseline_j1mass_data,"j1m");
    makeSingleLeptStackPlot(h_baseline_j2mass_TT,h_baseline_j2mass_WJets,h_baseline_j2mass_SnglT,h_baseline_j2mass_data,"j2m");
    makeSingleLeptStackPlot(h_baseline_j1pt_TT,h_baseline_j1pt_WJets,h_baseline_j1pt_SnglT,h_baseline_j1pt_data,"j1pt");
    makeSingleLeptStackPlot(h_baseline_j2pt_TT,h_baseline_j2pt_WJets,h_baseline_j2pt_SnglT,h_baseline_j2pt_data,"j2pt");
    makeSingleLeptStackPlot(h_baseline_j1bb_TT,h_baseline_j1bb_WJets,h_baseline_j1bb_SnglT,h_baseline_j1bb_data,"j1bb");
    makeSingleLeptStackPlot(h_baseline_j2bb_TT,h_baseline_j2bb_WJets,h_baseline_j2bb_SnglT,h_baseline_j2bb_data,"j2bb");
  }

  if (runStacks && whichRegion=="photon") {
    std::cout<<"Running stacks..."<<std::endl;
    makePhotonStackPlot(h_baseline_j1mass_QCD,h_baseline_j1mass_GJets,"j1m");
    makePhotonStackPlot(h_baseline_j2mass_QCD,h_baseline_j2mass_GJets,"j2m");
    makePhotonStackPlot(h_baseline_j1pt_QCD,h_baseline_j1pt_GJets,"j1pt");
    makePhotonStackPlot(h_baseline_j2pt_QCD,h_baseline_j2pt_GJets,"j2pt");
    makePhotonStackPlot(h_baseline_j1bb_QCD,h_baseline_j1bb_GJets,"j1bb");
    makePhotonStackPlot(h_baseline_j1bb_QCD,h_baseline_j2bb_GJets,"j2bb");
  }

  makeMETNormCompare(h_METShape_bkgSum_wData,"BkgSum"); //REMOVE


  if (runMETNorm){
    std::cout<<"Running MET shape comparison..."<<std::endl;

    if (whichRegion=="signal"){
      // justBB(h_baseline_j1bb_TT, h_baseline_j1bb_T5HH1600, "TT", "T5HH", "1600");

      // makeMETNormCompare(h_METShape_bkgSum,"BkgSum");
      // makeMETNormCompare(h_METShape_data,"Data");

      // makeMCStackvDataMET(vec_0Hb_MET_0l, h_Opt1_sum, h_Opt1_data, h_A_sum,h_A1_sum);


      // makeMCvDataComp(h_A_sum, h_A_data, "double-tag SR", "MET");
      // makeMCvDataComp(h_A1_sum, h_A1_data, "single-tag SR", "MET");
      // makeMCvDataComp(h_B_sum, h_B_data, "2HSB", "MET");
      // makeMCvDataComp(h_B1_sum, h_B1_data, "1HSB", "MET");
      // makeMCvDataComp(h_C_sum, h_C_data, "0HSR", "MET");
      // makeMCvDataComp(h_D_sum, h_D_data, "0HSB", "MET");
      //
      //

      // makeMCStackvDataComp(vec_baseline_j1mass_0l, h_baseline_j1mass_sum, h_baseline_j1mass_data, h_baseline_j1mass_TChiHH500,h_baseline_j1mass_T5HH1600,"baseline", "j1m", true);
      // makeMCStackvDataComp(vec_baseline_j2mass_0l, h_baseline_j2mass_sum, h_baseline_j2mass_data, h_baseline_j2mass_TChiHH500,h_baseline_j2mass_T5HH1600,"baseline", "j2m", true);
      // makeMCStackvDataComp(vec_baseline_j1pt_0l, h_baseline_j1pt_sum, h_baseline_j1pt_data, h_baseline_j1pt_TChiHH500,h_baseline_j1pt_T5HH1600,"baseline", "j1pt", true);
      // makeMCStackvDataComp(vec_baseline_j2pt_0l, h_baseline_j2pt_sum, h_baseline_j2pt_data, h_baseline_j2pt_TChiHH500,h_baseline_j2pt_T5HH1600,"baseline", "j2pt", true);
      // makeMCStackvDataComp(vec_baseline_j1bb_0l, h_baseline_j1bb_sum, h_baseline_j1bb_data, h_baseline_j1bb_TChiHH500,h_baseline_j1bb_T5HH1600,"baseline", "j1bb", true);
      // makeMCStackvDataComp(vec_baseline_j2bb_0l, h_baseline_j2bb_sum, h_baseline_j2bb_data, h_baseline_j2bb_TChiHH500,h_baseline_j2bb_T5HH1600,"baseline", "j2bb", true);


      // makeMCStackvDataComp(vec_baseline_MET_0l, h_baseline_MET_sum, h_baseline_MET_data, h_baseline_MET_TChiHH500,h_baseline_MET_T5HH1600,"baseline", "MET", true);
      // makeMCStackvDataComp(vec_baseline_jmass_0l, h_baseline_jmass_sum, h_baseline_jmass_data, h_baseline_jmass_TChiHH500,h_baseline_jmass_T5HH1600,"baseline", "jm", true);
      // makeMCStackvDataComp(vec_baseline_jpt_0l, h_baseline_jpt_sum, h_baseline_jpt_data, h_baseline_jpt_TChiHH500,h_baseline_jpt_T5HH1600,"baseline", "jpt", true);
      // makeMCStackvDataComp(vec_baseline_jbb_0l, h_baseline_jbb_sum, h_baseline_jbb_data, h_baseline_jbb_TChiHH500,h_baseline_jbb_T5HH1600,"baseline", "jbb", true);
      // //
      // // makeMCStackvDataComp(vec_0H_MET_0l, h_0H_MET_sum, h_0H_MET_data, h_0H_MET_TChiHH500, h_0H_MET_T5HH1600,"0H", "MET", false);
      makeMCStackvDataComp(vec_0H_jmass_0l, h_0H_jmass_sum, h_0H_jmass_data, h_0H_jmass_TChiHH500,h_0H_jmass_T5HH1600,"0H", "jm", true);
      // // makeMCStackvDataComp(vec_0H_jpt_0l, h_0H_jpt_sum, h_0H_jpt_data, h_0H_jpt_TChiHH500,h_0H_jpt_T5HH1600,"0H", "jpt", false);
      // // makeMCStackvDataComp(vec_0H_jbb_0l, h_0H_jbb_sum, h_0H_jbb_data, h_0H_jbb_TChiHH500,h_0H_jbb_T5HH1600,"0H", "jbb", false);
      // //
      // // makeMCStackvDataComp(vec_1H_MET_0l, h_1H_MET_sum, h_1H_MET_data, h_1H_MET_TChiHH500,h_1H_MET_T5HH1600,"1H", "MET", false);
      makeMCStackvDataComp(vec_1H_jmass_0l, h_1H_jmass_sum, h_1H_jmass_data, h_1H_jmass_TChiHH500,h_1H_jmass_T5HH1600,"1H", "jm", true);
      // // makeMCStackvDataComp(vec_1H_jpt_0l, h_1H_jpt_sum, h_1H_jpt_data, h_1H_jpt_TChiHH500,h_1H_jpt_T5HH1600,"1H", "jpt", false);
      // // makeMCStackvDataComp(vec_1H_jbb_0l, h_1H_jbb_sum, h_1H_jbb_data, h_1H_jbb_TChiHH500,h_1H_jbb_T5HH1600,"1H", "jbb", false);
      // //
      // // makeMCStackvDataComp(vec_2H_MET_0l, h_2H_MET_sum, h_2H_MET_data, h_2H_MET_TChiHH500,h_2H_MET_T5HH1600,"2H", "MET", false);
      makeMCStackvDataComp(vec_2H_jmass_0l, h_2H_jmass_sum, h_2H_jmass_data, h_2H_jmass_TChiHH500,h_2H_jmass_T5HH1600,"2H", "jm", true);
      // // makeMCStackvDataComp(vec_2H_jpt_0l, h_2H_jpt_sum, h_2H_jpt_data, h_2H_jpt_TChiHH500,h_2H_jpt_T5HH1600,"2H", "jpt", false);
      // // makeMCStackvDataComp(vec_2H_jbb_0l, h_2H_jbb_sum, h_2H_jbb_data, h_2H_jbb_TChiHH500,h_2H_jbb_T5HH1600,"2H", "jbb", false);

      // makeMCStackvDataComp(vec_1HSR_MET_0l, h_A1_sum, h_A1_data, h_A1_TChiHH500,h_A1_T5HH1600,"1HSR", "MET");
      // makeMCStackvDataComp(vec_1HSR_j1mass_0l, h_J1M_tagSR_sum, h_J1M_tagSR_data, h_J1M_tagSR_TChiHH500,h_J1M_tagSR_T5HH1600,"1HSR", "j1m");
      // makeMCStackvDataComp(vec_1HSR_j2mass_0l, h_J2M_tagSR_sum, h_J2M_tagSR_data, h_J2M_tagSR_TChiHH500,h_J2M_tagSR_T5HH1600,"1HSR", "j2m");
      // makeMCStackvDataComp(vec_1HSR_j1pt_0l, h_J1Pt_tagSR_sum, h_J1Pt_tagSR_data, h_J1Pt_tagSR_TChiHH500,h_J1Pt_tagSR_T5HH1600,"1HSR", "j1pt");
      // makeMCStackvDataComp(vec_1HSR_j2pt_0l, h_J2Pt_tagSR_sum, h_J2Pt_tagSR_data, h_J2Pt_tagSR_TChiHH500,h_J2Pt_tagSR_T5HH1600,"1HSR", "j2pt");
      // makeMCStackvDataComp(vec_1HSR_j1bb_0l, h_J1BB_tagSR_sum, h_J1BB_tagSR_data, h_J1BB_tagSR_TChiHH500,h_J1BB_tagSR_T5HH1600,"1HSR", "j1bb");
      // makeMCStackvDataComp(vec_1HSR_j2bb_0l, h_J2BB_tagSR_sum, h_J2BB_tagSR_data, h_J2BB_tagSR_TChiHH500,h_J2BB_tagSR_T5HH1600,"1HSR", "j2bb");

      // makeMCStackvDataComp(vec_2HSR_MET_0l, h_A_sum, h_A_data, h_A_TChiHH500,h_A_T5HH1600,"2HSR", "MET");
      // makeMCStackvDataComp(vec_2HSR_j1mass_0l, h_J1M_doubletagSR_sum, h_J1M_doubletagSR_data, h_J1M_doubletagSR_TChiHH500,h_J1M_doubletagSR_T5HH1600,"2HSR", "j1m");
      // makeMCStackvDataComp(vec_2HSR_j2mass_0l, h_J2M_doubletagSR_sum, h_J2M_doubletagSR_data, h_J2M_doubletagSR_TChiHH500,h_J2M_doubletagSR_T5HH1600,"2HSR", "j2m");
      // makeMCStackvDataComp(vec_2HSR_j1pt_0l, h_J1Pt_doubletagSR_sum, h_J1Pt_doubletagSR_data, h_J1Pt_doubletagSR_TChiHH500,h_J1Pt_doubletagSR_T5HH1600,"2HSR", "j1pt");
      // makeMCStackvDataComp(vec_2HSR_j2pt_0l, h_J2Pt_doubletagSR_sum, h_J2Pt_doubletagSR_data, h_J2Pt_doubletagSR_TChiHH500,h_J2Pt_doubletagSR_T5HH1600,"2HSR", "j2pt");
      // makeMCStackvDataComp(vec_2HSR_j1bb_0l, h_J1BB_doubletagSR_sum, h_J1BB_doubletagSR_data, h_J1BB_doubletagSR_TChiHH500,h_J1BB_doubletagSR_T5HH1600,"2HSR", "j1bb");
      // makeMCStackvDataComp(vec_2HSR_j2bb_0l, h_J2BB_doubletagSR_sum, h_J2BB_doubletagSR_data, h_J2BB_doubletagSR_TChiHH500,h_J2BB_doubletagSR_T5HH1600,"2HSR", "j2bb");

      // makeMCStackvDataComp(vec_0H_MET_0l, h_0H_MET_sum, h_0H_MET_data, "0H", "MET");
      // makeMCStackvDataComp(vec_0H_j1mass_0l, h_0H_j1mass_sum, h_0H_j1mass_data, "0H", "j1m");
      // makeMCStackvDataComp(vec_0H_j2mass_0l, h_0H_j2mass_sum, h_0H_j2mass_data, "0H", "j2m");
      // makeMCStackvDataComp(vec_0H_j1pt_0l, h_0H_j1pt_sum, h_0H_j1pt_data, "0H", "j1pt");
      // makeMCStackvDataComp(vec_0H_j2pt_0l, h_0H_j2pt_sum, h_0H_j2pt_data, "0H", "j2pt");
      // makeMCStackvDataComp(vec_0H_j1bb_0l, h_0H_j1bb_sum, h_0H_j1bb_data, "0H", "j1bb");
      // makeMCStackvDataComp(vec_0H_j2bb_0l, h_0H_j2bb_sum, h_0H_j2bb_data, "0H", "j2bb");
    }
    else if (whichRegion=="singleLept"){
      makeMETNormCompare(h_METShape_bkgSum,"BkgSum");
      makeMETNormCompare(h_METShape_data,"Data");

      tableOfMETNorm(h_METShape_sum_all, "BkgSum");
      tableOfMETNorm(h_METShape_data_all, "Data");

      makeMCvDataComp(h_A_sum, h_A_data, "2HSR", "MET");
      makeMCvDataComp(h_B_sum, h_B_data, "2HSB", "MET");
      makeMCvDataComp(h_A1_sum, h_A1_data, "1HSR", "MET");
      makeMCvDataComp(h_B1_sum, h_B1_data, "1HSB", "MET");
      makeMCvDataComp(h_C_sum, h_C_data, "0HSR", "MET");
      makeMCvDataComp(h_D_sum, h_D_data, "0HSB", "MET");

      makeMCvDataComp(h_baseline_MET_sum, h_baseline_MET_data, "Baseline", "MET");
      makeMCvDataComp(h_baseline_j1mass_sum, h_baseline_j1mass_data, "Baseline", "j1m");
      makeMCvDataComp(h_baseline_j2mass_sum, h_baseline_j2mass_data, "Baseline", "j2m");
      makeMCvDataComp(h_baseline_j1pt_sum, h_baseline_j1pt_data, "Baseline", "j1pt");
      makeMCvDataComp(h_baseline_j2pt_sum, h_baseline_j2pt_data, "Baseline", "j2pt");
      makeMCvDataComp(h_baseline_j1bb_sum, h_baseline_j1bb_data, "Baseline", "j1bb");
      makeMCvDataComp(h_baseline_j2bb_sum, h_baseline_j2bb_data, "Baseline", "j2bb");
      makeMCvDataComp(h_nH_sum, h_nH_data, "Baseline", "nH");

      // makeMCStackvDataComp(vec_baseline_MET_1l, h_baseline_MET_sum, h_baseline_MET_data, "Baseline", "MET");
      // makeMCStackvDataComp(vec_baseline_j1mass_1l, h_baseline_j1mass_sum, h_baseline_j1mass_data, "Baseline", "j1m"); //this is broken...
      // makeMCStackvDataComp(vec_baseline_j2mass_1l, h_baseline_j2mass_sum, h_baseline_j2mass_data, "Baseline", "j2m"); //this is broken...
      // makeMCStackvDataComp(vec_baseline_j1pt_1l, h_baseline_j1pt_sum, h_baseline_j1pt_data, "Baseline", "j1pt");
      // makeMCStackvDataComp(vec_baseline_j2pt_1l, h_baseline_j2pt_sum, h_baseline_j2pt_data, "Baseline", "j2pt");
      // makeMCStackvDataComp(vec_baseline_j1bb_1l, h_baseline_j1bb_sum, h_baseline_j1bb_data, "Baseline", "j1bb");
      // makeMCStackvDataComp(vec_baseline_j2bb_1l, h_baseline_j2bb_sum, h_baseline_j2bb_data, "Baseline", "j2bb");

      makeMETShapeComp({h_A_sum,h_A1_sum,h_Opt1_sum},{h_A_data,h_A1_data,h_Opt1_data});
    }
    else if (whichRegion=="photon"){
      // makeMETNorm(h_METShape_bkgSum_2H,"BkgSum","Double");
      // makeMETNorm(h_METShape_bkgSum_1H,"BkgSum","Single");

      makeMETNormCompare(h_METShape_bkgSum,"BkgSum");
      makeMETNormCompare(h_METShape_data,"Data");

      tableOfMETNorm(h_METShape_sum_all, "BkgSum");
      tableOfMETNorm(h_METShape_data_all, "Data");


      makeMCvDataComp(h_A_sum, h_A_data, "2HSR", "MET");
      makeMCvDataComp(h_B_sum, h_B_data, "2HSB", "MET");
      makeMCvDataComp(h_A1_sum, h_A1_data, "1HSR", "MET");
      makeMCvDataComp(h_B1_sum, h_B1_data, "1HSB", "MET");
      makeMCvDataComp(h_C_sum, h_C_data, "0HSR", "MET");
      makeMCvDataComp(h_D_sum, h_D_data, "0HSB", "MET");

      makeMCvDataComp(h_baseline_MET_sum, h_baseline_MET_data, "Baseline", "MET");
      makeMCvDataComp(h_baseline_j1mass_sum, h_baseline_j1mass_data, "Baseline", "j1m");
      makeMCvDataComp(h_baseline_j2mass_sum, h_baseline_j2mass_data, "Baseline", "j2m");
      makeMCvDataComp(h_baseline_j1pt_sum, h_baseline_j1pt_data, "Baseline", "j1pt");
      makeMCvDataComp(h_baseline_j2pt_sum, h_baseline_j2pt_data, "Baseline", "j2pt");
      makeMCvDataComp(h_baseline_j1bb_sum, h_baseline_j1bb_data, "Baseline", "j1bb");
      makeMCvDataComp(h_baseline_j2bb_sum, h_baseline_j2bb_data, "Baseline", "j2bb");
      makeMCvDataComp(h_nH_sum, h_nH_data, "Baseline", "nH");

      // makeMCStackvDataComp(vec_baseline_MET_1p, h_baseline_MET_sum, h_baseline_MET_data, "Baseline", "MET");
      // makeMCStackvDataComp(vec_baseline_j1mass_1p, h_baseline_j1mass_sum, h_baseline_j1mass_data, "Baseline", "j1m");
      // makeMCStackvDataComp(vec_baseline_j2mass_1p, h_baseline_j2mass_sum, h_baseline_j2mass_data, "Baseline", "j2m");
      // makeMCStackvDataComp(vec_baseline_j1pt_1p, h_baseline_j1pt_sum, h_baseline_j1pt_data, "Baseline", "j1pt");
      // makeMCStackvDataComp(vec_baseline_j2pt_1p, h_baseline_j2pt_sum, h_baseline_j2pt_data, "Baseline", "j2pt");
      // makeMCStackvDataComp(vec_baseline_j1bb_1p, h_baseline_j1bb_sum, h_baseline_j1bb_data, "Baseline", "j1bb");
      // makeMCStackvDataComp(vec_baseline_j2bb_1p, h_baseline_j2bb_sum, h_baseline_j2bb_data, "Baseline", "j2bb");

      makeMETShapeComp({h_A_sum,h_A1_sum,h_Opt1_sum},{h_A_data,h_A1_data,h_Opt1_data});
    }
  }

  if (runTableOfYields){
    std::cout<<"Running table of yields..."<<std::endl;

    if (whichRegion=="signal"){
      // tableOfYields(histos_allRegions_data, "Data");
      // tableOfYields(histos_allRegions_sum, "BkgSum");
      // tableOfYields(histos_allRegions_QCD, "QCD");
      // tableOfYields(histos_allRegions_WJets, "WJets");
      // tableOfYields(histos_allRegions_ZJets, "ZJets");
      // tableOfYields(histos_allRegions_TT, "TT");
      // tableOfYields(histos_allRegions_SnglT, "SnglT");
      // giantTableOfYields(all_ABCDHistos_0l);

      tableOfMETNorm(h_METShape_sum_all, "BkgSum");
      // tableOfMETNorm(h_METShape_data_all, "Data");
    }

    else if (whichRegion=="singleLept"){
      tableOfYields(histos_allRegions_data, "Data");
      tableOfYields(histos_allRegions_sum, "BkgSum");
      tableOfYields(histos_allRegions_SnglT, "SnglT");
      tableOfYields(histos_allRegions_TT, "TT");
      tableOfYields(histos_allRegions_WJets, "WJets");
      giantTableOfYields(all_ABCDHistos_1l);
    }
    else if (whichRegion=="photon"){
      tableOfYields(histos_allRegions_data, "Data");
      tableOfYields(histos_allRegions_sum, "BkgSum");
      tableOfYields(histos_allRegions_QCD, "QCD");
      tableOfYields(histos_allRegions_GJets, "GJets");
      giantTableOfYields(all_ABCDHistos_1p);
    }
  }

  // fout->Close();
  // f->Close();
  // fSignal->Close();
} //end method ABCD()


void makeABCDPlot(vector<TH1F*> dem_histos, TString bkgType = "", TString tagType = "", TString year = "") {
  TH1F *histo_A = (TH1F*)dem_histos[0]->Clone("histo_A"); TH1F *histo_B = (TH1F*)dem_histos[1]->Clone("histo_B");
  TH1F *histo_C = (TH1F*)dem_histos[2]->Clone("histo_C"); TH1F *histo_D = (TH1F*)dem_histos[3]->Clone("histo_D");

  DrawOverflow(histo_A); DrawOverflow(histo_B); DrawOverflow(histo_C); DrawOverflow(histo_D);

  bool isData = false;
  TString histoName = dem_histos[0]->GetName();
  if (histoName.Contains("data")) isData=true;


  TH1F *histo_pred = (TH1F*)histo_B->Clone("histo_pred");
  histo_pred->Multiply(histo_C);histo_pred->Divide(histo_D);

  TGraphAsymmErrors * graph = new TGraphAsymmErrors(histo_A, histo_pred, "pois");
  graph->SetTitle(";p_{T}^{miss} [GeV]; #kappa (Sim/Pred)");
  styleGraph(graph);

  TString canvName = bkgType+tagType;
  TCanvas * can_h = new TCanvas(canvName,canvName, 50, 50, W, H);
  styleCanvas(can_h);


  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.25, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.25);
  pad1->SetTickx(0); pad1->SetTicky(0);
  pad1->SetPad(0., 1 - up_height, 1., 1.00);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTopMargin(0.08); pad1->SetLeftMargin(0.10); pad1->SetRightMargin(0.06);
  pad1->Draw();

  pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.33); pad2->SetTopMargin(0.01);
  pad2->SetLeftMargin(0.10); pad2->SetRightMargin(0.06);
  pad2->Draw();
  pad1->cd();


  histo_pred->SetStats(0); histo_A->SetStats(0);
  histo_pred->SetFillColor(kBlue); histo_pred->SetFillStyle(3445);
  histo_pred->SetMarkerSize(0); histo_pred->SetLineWidth(1);
  histo_pred->SetLineColor(kBlue);

  TString title = ";;nEvents";
  histo_A->SetTitle(title); histo_pred->SetTitle(title);
  histo_pred->SetMinimum(0); histo_A->SetMinimum(0);
  histo_A->SetLineColor(kBlack); histo_A->SetMarkerStyle(20);
  histo_A->SetMarkerColor(kBlack);

  TLegend* legend = new TLegend(0.60,0.7,0.93,0.91);
  legend->SetBorderSize(0);
  if (!isData){
    if (tagType=="Double") {
      legend->AddEntry(histo_A,"MC: A2","lp");
      legend->AddEntry(histo_pred,"Pred: B2*C/D","f");
    }
    else if (tagType=="Single") {
      legend->AddEntry(histo_A,"MC: A1","lp");
      legend->AddEntry(histo_pred,"Pred: B1*C/D","f");
    }
  }

  else {
    if (tagType=="Double") {
      legend->AddEntry(histo_A,"Data A2","lp");
      legend->AddEntry(histo_pred,"Pred: B2*C/D","f");
    }
    else if (tagType=="Single") {
      legend->AddEntry(histo_A,"Data A1","lp");
      legend->AddEntry(histo_pred,"Pred: B1*C/D","f");
    }
  }

  float pred_bin1 = histo_pred->GetBinContent(1);
  float sig_bin1 = histo_A->GetBinContent(1);

  if (pred_bin1>sig_bin1) {
    histo_pred->GetYaxis()->SetTitleSize(0.05);
    histo_pred->GetYaxis()->SetTitleOffset(0.75);
    histo_pred->SetMaximum(histo_pred->GetMaximum()*1.25);
    histo_pred->GetXaxis()->SetLabelSize(0);
    histo_pred->Draw("E2");
    histo_A->Draw("same");
  }
  else {
    histo_A->GetYaxis()->SetTitleOffset(0.75);
    histo_A->GetYaxis()->SetTitleSize(0.75);
    histo_A->SetMaximum(histo_A->GetMaximum()*1.25);
    histo_A->GetXaxis()->SetLabelSize(0);
    histo_A->Draw("same");
    histo_pred->Draw("E2 same");
  }
  legend->Draw("same");
  pad2->cd();

  graph->Draw("APE");
  graph->GetXaxis()->SetTitleSize(0.16);
  graph->GetXaxis()->SetRangeUser(300,1400);
  graph->GetYaxis()->SetRangeUser(-1.0,5.0);
  if (tagType=="Single") graph->GetYaxis()->SetRangeUser(0.0,2.0);
  graph->Draw("APE");
  can_h->Modified(); can_h->Update();
  graph->GetXaxis()->SetRangeUser(300,1400);
  can_h->Modified(); can_h->Update();

  TLine *line = new TLine(300,1.0,1400,1.0);
  line->SetLineColor(kRed); line->SetLineStyle(2);
  line->Draw("same");

  pad1->cd();
  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize/12);
  latex.DrawLatex(0.95,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(cmsTextSize*1.3);
  latex.DrawLatex(0.20, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextAlign(align_);
  latex.SetTextSize(cmsTextSize*0.85); latex.DrawLatex(0.26, 0.88, extraText);

  TString savename = "ABCD_"+tagType+"_"+bkgType;
  cdABCD->cd();
  can_h->Write(savename);
    if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/bkgest/"+savename+".pdf","PDF");
  delete can_h;
}

void makeFullBkgClosure(vector<TH1F*> dem_histos, TString bkgType = "", TString tagType = "", TString year = "") {
  TH1F *histo_A = (TH1F*)dem_histos[0]->Clone("histo_A"); TH1F *histo_B = (TH1F*)dem_histos[1]->Clone("histo_B");
  TH1F *histo_C = (TH1F*)dem_histos[2]->Clone("histo_C"); TH1F *histo_D = (TH1F*)dem_histos[3]->Clone("histo_D");

  TH1F *histo_MET = (TH1F*)dem_histos[4]->Clone("histo_MET");

  DrawOverflow(histo_A); DrawOverflow(histo_B); DrawOverflow(histo_C); DrawOverflow(histo_D); DrawOverflow(histo_MET);

  double b_int; double b_error; b_int = histo_B->IntegralAndError(1,5,b_error,"");
  double c_int; double c_error; c_int = histo_C->IntegralAndError(1,5,c_error,"");
  double d_int; double d_error; d_int = histo_D->IntegralAndError(1,5,d_error,"");

  // TH1F *histo_pred = (TH1F*)histo_B->Clone("histo_pred");
  // histo_pred->Multiply(histo_C);histo_pred->Divide(histo_D);

  double bkgNorm; double bkgNorm_error;
  // bkgNorm = histo_pred->IntegralAndError(1,5,bkgNorm_error,"");
  bkgNorm = b_int*c_int/d_int;
  bkgNorm_error = bkgNorm*sqrt( TMath::Power(b_error/b_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );

  TH1F *h_finalBkg = (TH1F*)histo_A->Clone("h_finalBkg");

  float kappa = 1.0; float kappa_error;
  float bkgFrac1; float bkgFrac1_error;
  float bkgFrac2; float bkgFrac2_error;
  float bkgFrac3; float bkgFrac3_error;

  float addErr_1 = 0.0036; float addErr_2 = 0.022; float addErr_3 = 0.027; //for each bin

  histo_MET->Scale(1.0/histo_MET->Integral());

  bkgFrac1 = histo_MET->GetBinContent(1); bkgFrac1_error = histo_MET->GetBinError(1);
  bkgFrac2 = histo_MET->GetBinContent(2); bkgFrac2_error = histo_MET->GetBinError(2);
  bkgFrac3 = histo_MET->GetBinContent(3); bkgFrac3_error = histo_MET->GetBinError(3);

  //V18
  if (whichRegion=="signal"){
    if (tagType=="Double") kappa_error = 0.17;
    else if (tagType=="Single") kappa_error = 0.04;
  }

  else if (whichRegion=="singleLept"){
    if (bkgType=="BkgSum"){
      if (tagType=="Double"){
        kappa = 1.03; kappa_error = 0.03;
      }
      else if (tagType=="Single"){
        kappa = 0.97; kappa_error = 0.01;
      }
    }
    else if (bkgType=="Data"){
      if (tagType=="Double"){
        kappa = 1.07; kappa_error = 0.14;
      }
      else if (tagType=="Single"){
        kappa = 0.92; kappa_error = 0.08;
      }
    }
  }

  else if (whichRegion=="photon"){
    if (tagType=="Double"){
      kappa = 0.52; kappa_error = 0.37;
    }
    else if (tagType=="Single"){
      kappa = 1.10; kappa_error = 0.20;
    }
  }

  float bin1Content = bkgNorm*kappa*bkgFrac1; h_finalBkg->SetBinContent(1,bin1Content);
  float bin2Content = bkgNorm*kappa*bkgFrac2; h_finalBkg->SetBinContent(2,bin2Content);
  float bin3Content = bkgNorm*kappa*bkgFrac3; h_finalBkg->SetBinContent(3,bin3Content);

  float bin1Error = bin1Content*sqrt( TMath::Power(bkgNorm_error/bkgNorm,2) + TMath::Power(kappa_error/kappa,2) + TMath::Power(bkgFrac1_error/bkgFrac1,2) + TMath::Power(addErr_1/1.0,2) );
  float bin2Error = bin2Content*sqrt( TMath::Power(bkgNorm_error/bkgNorm,2) + TMath::Power(kappa_error/kappa,2) + TMath::Power(bkgFrac2_error/bkgFrac2,2) + TMath::Power(addErr_2/1.0,2) );
  float bin3Error = bin3Content*sqrt( TMath::Power(bkgNorm_error/bkgNorm,2) + TMath::Power(kappa_error/kappa,2) + TMath::Power(bkgFrac3_error/bkgFrac3,2) + TMath::Power(addErr_3/1.0,2) );

  h_finalBkg->SetBinError(1,bin1Error);
  h_finalBkg->SetBinError(2,bin2Error);
  h_finalBkg->SetBinError(3,bin3Error);

  TGraphAsymmErrors * graph = new TGraphAsymmErrors(histo_A, h_finalBkg, "pois");
  // styleGraph(graph);
  graph->SetTitle(";p_{T}^{miss} [GeV]; SR/Pred");
  graph->GetYaxis()->CenterTitle();
  graph->SetMarkerSize(1.3); graph->SetLineWidth(2);

  TString canvName = bkgType+tagType;
  TCanvas * can_h = new TCanvas(canvName,canvName, 50, 50, 900, 900);
  styleCanvas(can_h);

  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.4, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.4);
  can_h->SetMargin(0., 0., 0., 0.); can_h->SetTicks(1,1); can_h->SetFillStyle(4000);

  pad1->SetPad(0., 0., 1., 1.);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTicks(1,1);pad1->SetFillStyle(4000);
  pad2->SetTicks(1,1);pad2->SetFillStyle(4000);

  pad2->SetPad(0., 0., 1., 1.);
  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad1->SetMargin(0.13, 0.055, 0.38, 0.07);
  pad2->SetMargin(0.13, 0.055, 0.12, 1.-0.37);
  pad1->Draw(); pad2->Draw(); pad1->cd();


  h_finalBkg->SetStats(0); histo_A->SetStats(0);
  h_finalBkg->SetFillColor(kBlue); h_finalBkg->SetFillStyle(3445);
  h_finalBkg->SetMarkerSize(0); h_finalBkg->SetLineWidth(1);
  h_finalBkg->SetLineColor(kBlue);

  TString title = ";;Events";
  histo_A->SetTitle(title); h_finalBkg->SetTitle(title);
  h_finalBkg->SetMinimum(0); histo_A->SetMinimum(0);
  histo_A->SetLineColor(kBlack); histo_A->SetMarkerStyle(20); histo_A->SetMarkerSize(1.3);
  histo_A->SetMarkerColor(kBlack); histo_A->SetLineWidth(2);

  graph->SetLineColor(kBlack); graph->SetMarkerColor(kBlack); graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.3); graph->SetLineWidth(2);

  TLegend* legend = new TLegend(0.55,0.7,0.93,0.91);
  legend->SetBorderSize(0);
  if (tagType=="Double") legend->AddEntry(histo_A,"2H SR yields","lp");
  else if (tagType=="Single") legend->AddEntry(histo_A,"1H SR yields","lp");
  legend->AddEntry(h_finalBkg,"Bkg prediction","f");


  float pred_bin1 = h_finalBkg->GetBinContent(1);
  float sig_bin1 = histo_A->GetBinContent(1);
  float max = h_finalBkg->GetBinContent(1); if (histo_A->GetBinContent(1)>max) max = histo_A->GetBinContent(1);

  h_finalBkg->SetMaximum(max*1.6);
  if (tagType=="Double" && bkgType=="Data") h_finalBkg->SetMaximum(7.9);
  else if (tagType=="Single" && bkgType=="BkgSum") h_finalBkg->SetMaximum(52.0);
  else if (tagType=="Single" && bkgType=="Data") h_finalBkg->SetMaximum(63.0);
  h_finalBkg->GetYaxis()->SetTitleOffset(0.9);
  h_finalBkg->GetXaxis()->SetLabelSize(0);
  h_finalBkg->GetXaxis()->SetTitleSize(0);
  h_finalBkg->GetYaxis()->SetTitleSize(0.05);
  h_finalBkg->GetYaxis()->SetLabelSize(0.038);
  h_finalBkg->Draw("E2");
  histo_A->Draw("same");

  legend->Draw("same");
  pad2->cd();

  graph->Draw("APE");
  graph->GetXaxis()->SetRangeUser(300,1400);
  graph->GetYaxis()->SetRangeUser(-0.95,4.95);
  graph->GetYaxis()->SetTitleSize(0.045);
  graph->GetXaxis()->SetTitleSize(0.05);
  graph->GetXaxis()->SetTitleOffset(0.9);
  graph->GetYaxis()->SetLabelSize(0.038);
  graph->GetXaxis()->SetLabelSize(0.038);

  if (tagType=="Single" && bkgType=="BkgSum") graph->GetYaxis()->SetRangeUser(0.71,1.29);
  else if (tagType=="Single" && bkgType=="Data") graph->GetYaxis()->SetRangeUser(0.0,2.99);

  TLine *line = new TLine(300,1.0,1400,1.0);
  line->SetLineColor(kRed); line->SetLineStyle(2);
  line->Draw("same");

  graph->Draw("APE same");
  line->Draw("same");

  can_h->Modified(); can_h->Update();
  graph->GetXaxis()->SetRangeUser(300,1400);
  can_h->Modified(); can_h->Update();

  pad1->cd();

  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(0.045);
  latex.DrawLatex(0.94,0.94,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(0.045);
  latex.DrawLatex(0.25, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextSize(0.03); if (bkgType=="BkgSum") latex.DrawLatex(0.38, 0.85, "Simulation");
  TLatex suppLabel(0.16, 0.82, "Supplementary");
  suppLabel.SetNDC(); suppLabel.SetTextFont(extraTextFont); suppLabel.SetTextSize(0.03);
  suppLabel.Draw("same");

  cdClose->cd();
  can_h->Write("FullBkg_"+bkgType+tagType);
  if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/fullClosure/FullClosure_"+bkgType+tagType+".pdf","PDF");
  can_h->SaveAs("FullClosure_"+bkgType+tagType+".pdf","PDF");
  can_h->SaveAs("FullClosure_"+bkgType+tagType+".png");
  delete can_h;
}


void makeRpfPlot(vector<TH1F*> dem_histos, TString bkgType = "", TString jetType = "", TString tagType = "") {
  TH1F * num   = (TH1F*)dem_histos[1]->Clone("num"+tagType+"_"+jetType+"_"+bkgType); //this should be 2H SB
  TH1F * denom = (TH1F*)dem_histos[3]->Clone("denom"+tagType+"_"+jetType+"_"+bkgType);  //this should be 0H SB
  num->Add(dem_histos[0]); denom->Add(dem_histos[2]); //Add 2H SR to num, and 0H SR to denom

  if (!bkgType.Contains("_mJ")){
    num->Rebin(2);
    denom->Rebin(2);
  }

  num->SetMarkerStyle(20); num->SetMarkerSize(0.85);
  num->SetLineColor(kRed); num->SetMarkerColor(kRed);
  denom->SetMarkerStyle(20); denom->SetMarkerSize(0.85);
  denom->SetLineColor(kBlue); denom->SetMarkerColor(kBlue);
  num->SetMinimum(0.0);denom->SetMinimum(0.0);

  TString thisType = tagType+"_"+jetType+"_"+bkgType;
  num->SetTitle(thisType+";;Normalized to area");
  num->GetYaxis()->SetTitleOffset(0.77);

  TString graphName = "ratio"+thisType;
  TGraphAsymmErrors * graph = new TGraphAsymmErrors(num, denom, "pois");
  styleGraph(graph);
  graph->SetName(graphName);

  // graph->SetMinimum(-0.1); graph->GetXaxis()->SetRangeUser(50,250);
  graph->SetMinimum(-0.1); graph->GetXaxis()->SetRangeUser(60,260);
  graph->SetLineColor(kBlack); graph->SetMarkerColor(kBlack);

  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, W, H);
  styleCanvas(can_h);

  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.25, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.25);
  pad1->SetTickx(0); pad1->SetTicky(0);
  pad1->SetPad(0., 1 - up_height, 1., 1.00);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTopMargin(0.08); pad1->SetLeftMargin(0.10); pad1->SetRightMargin(0.04);
  pad1->Draw();

  pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.33); pad2->SetTopMargin(0.01);
  pad2->SetLeftMargin(0.10); pad2->SetRightMargin(0.04);
  pad2->Draw(); pad1->cd();


  num->GetXaxis()->SetLabelSize(0);
  num->DrawNormalized();
  denom->DrawNormalized("same");

  TLegend* legend = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend->SetBorderSize(0);
  if (tagType == "Single"){
    legend->AddEntry(num, "1H SR+SB (numerator)", "PL");
    legend->AddEntry(denom, "0H SR+SB (denominator)", "PL");
    graph->SetTitle(";Soft-drop mass [GeV];R(1H/0H)");
  }
  else if (tagType == "Double"){
    legend->AddEntry(num, "2H SR+SB (numerator)", "PL");
    legend->AddEntry(denom, "0H SR+SB (denominator)", "PL");
    graph->SetTitle(";Soft-drop mass [GeV];R(2H/0H)");
  }

  legend->Draw();

  TLine *line2 = new TLine(95.0,0.0,95.0,1.0);
  line2->SetLineColor(kBlack);line2->SetLineStyle(2);
  line2->Draw("same");

  TLine *line3 = new TLine(145.0,0.0,145.0,1.0);
  line3->SetLineColor(kBlack);line3->SetLineStyle(2);
  line3->Draw("same");

  pad2->cd();
  graph->Draw("APE");
  graph->GetYaxis()->SetRangeUser(-0.1,0.5);
  can_h->Modified();
  can_h->Update();

  TF1*f0=new TF1("f0","pol0", 60,160);
  graph->Fit("f0","Q","R",60,160);
  double p0 = f0->GetParameter(0); //this does, indeed, work
  double error = f0->GetParError(0);
  graph->GetFunction("f0")->SetBit(TF1::kNotDraw);

  // TLine *line = new TLine(50.0,p0,250.0,p0);
  TLine *line = new TLine(60.0,p0,260.0,p0);
  line->SetLineColor(kRed);
  line->SetLineWidth(3);
  line->SetLineStyle(2);
  line->Draw("same");
  std::string lineConst = to_string(p0); std::string lineErr = to_string(error);
  TString constString = "Const: "+lineConst+" #pm "+ lineErr;
  TLatex *t = new TLatex(0.2,0.85,constString);
  t->SetNDC(); t->SetTextFont(52);
  t->SetTextSize(0.13);
  t->Draw();
  line2->Draw("same");
  line3->Draw("same");

  pad1->cd();
  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize/12);
  latex.DrawLatex(0.95,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(cmsTextSize*1.3);
  latex.DrawLatex(0.20, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextAlign(align_);
  latex.SetTextSize(cmsTextSize*0.85); latex.DrawLatex(0.26, 0.88, extraText);

  cdRPFFinal->cd();
  can_h->Write(thisType);
  if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/RPF/RPF"+thisType+".pdf", "PDF");

  cdRPF->cd();
  num->Write("Num_"+thisType);
  denom->Write("Denom_"+thisType);
  graph->Write("RPFRatio_"+thisType);
  delete can_h;
}


void makeStackPlot(TH1F* h_QCD,TH1F* h_TT,TH1F* h_WJets,TH1F* h_ZJets, TH1F* h_SnglT, TH1F* h_T5HH1600,TH1F* h_T5HH2000,TH1F* h_T5HH2200,TH1F* h_TChiHH500,TH1F* h_TChiHH800,TH1F* h_TChiHH1000,TString which = ""){
  Int_t col_zjets = TColor::GetColorTransparent(kOrange+1, 0.90);
  Int_t col_wjets = TColor::GetColorTransparent(ci3, 0.90);
  Int_t col_qcd = TColor::GetColorTransparent(ci2, 0.95);
  Int_t col_tt = TColor::GetColorTransparent(ci, 0.95);
  Int_t col_snglt = TColor::GetColorTransparent(kGray+2, 0.95);

  TH1F* h_QCD_stack = (TH1F*)h_QCD->Clone("QCDAll");
  h_QCD_stack->SetFillColor(col_qcd); h_QCD_stack->SetMarkerStyle(21); h_QCD_stack->SetMarkerColor(col_qcd); h_QCD_stack->SetLineColor(kBlack);

  TH1F *h_TT_stack = (TH1F*)h_TT->Clone("TTAll");
  h_TT_stack->SetFillColor(col_tt);h_TT_stack->SetMarkerStyle(21); h_TT_stack->SetMarkerColor(col_tt); h_TT_stack->SetLineColor(kBlack);

  TH1F *h_WJets_stack = (TH1F*)h_WJets->Clone("WJetsAll");
  h_WJets_stack->SetFillColor(col_wjets);h_WJets_stack->SetMarkerStyle(21); h_WJets_stack->SetMarkerColor(col_wjets); h_WJets_stack->SetLineColor(kBlack);

  TH1F *h_ZJets_stack = (TH1F*)h_ZJets->Clone("WZetsAll");
  h_ZJets_stack->SetFillColor(col_zjets); h_ZJets_stack->SetMarkerStyle(21); h_ZJets_stack->SetMarkerColor(col_zjets); h_ZJets_stack->SetLineColor(kBlack);

  TH1F *h_SnglT_stack = (TH1F*)h_SnglT->Clone("SnglTAll");
  h_SnglT_stack->SetFillColor(col_snglt); h_SnglT_stack->SetMarkerStyle(21); h_SnglT_stack->SetMarkerColor(col_snglt); h_SnglT_stack->SetLineColor(kBlack);

  TH1F *h_T5HH1600_stack = (TH1F*)h_T5HH1600->Clone("T5HH1600");
  h_T5HH1600_stack->SetMarkerStyle(1); h_T5HH1600_stack->SetMarkerColor(kRed); h_T5HH1600_stack->SetFillColor(0);
  h_T5HH1600_stack->SetLineColor(kRed); h_T5HH1600_stack->SetLineStyle(1); h_T5HH1600_stack->SetLineWidth(2);

  TH1F *h_T5HH2000_stack = (TH1F*)h_T5HH2000->Clone("T5HH2000");
  h_T5HH2000_stack->SetMarkerStyle(1); h_T5HH2000_stack->SetMarkerColor(kBlack); h_T5HH2000_stack->SetFillColor(0);
  h_T5HH2000_stack->SetLineColor(kBlack); h_T5HH2000_stack->SetLineStyle(1); h_T5HH2000_stack->SetLineWidth(2);

  TH1F *h_T5HH2200_stack = (TH1F*)h_T5HH2200->Clone("T5HH2200");
  h_T5HH2200_stack->SetMarkerStyle(1); h_T5HH2200_stack->SetMarkerColor(kMagenta); h_T5HH2200_stack->SetFillColor(0);
  h_T5HH2200_stack->SetLineColor(kMagenta); h_T5HH2200_stack->SetLineStyle(1); h_T5HH2200_stack->SetLineWidth(2);

  TH1F *h_TChiHH500_stack = (TH1F*)h_TChiHH500->Clone("TChiHH500_LSP1");
  h_TChiHH500_stack->SetMarkerStyle(1); h_TChiHH500_stack->SetMarkerColor(kRed); h_TChiHH500_stack->SetFillColor(0);
  h_TChiHH500_stack->SetLineColor(kRed); h_TChiHH500_stack->SetLineStyle(2); h_TChiHH500_stack->SetLineWidth(2);

  TH1F *h_TChiHH800_stack = (TH1F*)h_TChiHH800->Clone("TChiHH800_LSP1");
  h_TChiHH800_stack->SetMarkerStyle(1); h_TChiHH800_stack->SetMarkerColor(kBlack); h_TChiHH800_stack->SetFillColor(0);
  h_TChiHH800_stack->SetLineColor(kBlack); h_TChiHH800_stack->SetLineStyle(2); h_TChiHH800_stack->SetLineWidth(2);

  TH1F *h_TChiHH1000_stack = (TH1F*)h_TChiHH1000->Clone("TChiHH1000_LSP1");
  h_TChiHH1000_stack->SetMarkerStyle(1); h_TChiHH1000_stack->SetMarkerColor(kMagenta); h_TChiHH1000_stack->SetFillColor(0);
  h_TChiHH1000_stack->SetLineColor(kMagenta); h_TChiHH1000_stack->SetLineStyle(2); h_TChiHH1000_stack->SetLineWidth(2);

  THStack * doubleBStack = new THStack("hs","");
  doubleBStack->Add(h_SnglT_stack);
  doubleBStack->Add(h_QCD_stack);
  doubleBStack->Add(h_WJets_stack);
  doubleBStack->Add(h_ZJets_stack);
  doubleBStack->Add(h_TT_stack);

  TString graphName = "";
  if (which == "j1bb") graphName = "LeadJetBBTag";
  else if (which == "j2bb") graphName = "SubLeadJetBBTag";
  else if (which == "j1m") graphName = "LeadJetMass";
  else if (which == "j2m") graphName = "SubLeadJetMass";
  else if (which == "j1pt") graphName = "LeadJetPt";
  else if (which == "j2pt") graphName = "SubLeadJetPt";

  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, 900, 900);
  styleCanvas(can_h);
  can_h->SetLogy();

  doubleBStack->Draw("hist");
  doubleBStack->GetYaxis()->SetTitle("Events");
  doubleBStack->SetMinimum(0.1); doubleBStack->SetMaximum(3E3);
  // if (which=="j1pt" || which=="j2pt" || which=="j1bb" || which=="j2bb") doubleBStack->SetMaximum(3E3);


  if (which == "j1bb") doubleBStack->GetXaxis()->SetTitle("Leading jet D_{bb}");
  else if (which == "j2bb") doubleBStack->GetXaxis()->SetTitle("Subleading jet) D_{bb}");
  else if (which == "j1m") doubleBStack->GetXaxis()->SetTitle("Leading jet soft-drop mass [GeV]");
  else if (which == "j2m") doubleBStack->GetXaxis()->SetTitle("Subleading jet soft-drop mass [GeV]");
  else if (which == "j1pt") doubleBStack->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
  else if (which == "j2pt") doubleBStack->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");

  h_T5HH1600_stack->Draw("same hist"); h_T5HH2000_stack->Draw("same hist"); h_T5HH2200_stack->Draw("same hist");
  h_TChiHH500_stack->Draw("same hist"); h_TChiHH800_stack->Draw("same hist"); h_TChiHH1000_stack->Draw("same hist");

  TLegend* legend = new TLegend(0.38,0.7,0.58,0.9) ;
  legend->AddEntry(h_TT_stack, "t#bar{t}+X", "f");
  legend->AddEntry(h_ZJets_stack, "Z+jets", "f");
  legend->AddEntry(h_WJets_stack, "W+jets", "f");
  legend->AddEntry(h_QCD_stack, "QCD", "f");
  legend->AddEntry(h_SnglT_stack, "Other", "f");
  legend->SetNColumns(2);legend->SetBorderSize(0);
  legend->SetTextSize(0.022);

  TLegend* legend2 = new TLegend(0.58,0.7,0.95,0.9) ;
  legend2->AddEntry(h_T5HH1600_stack, "T5HH(1600,1)", "l");
  legend2->AddEntry(h_TChiHH500_stack, "TChiHH(500,1)", "l");
  legend2->AddEntry(h_T5HH2000_stack, "T5HH(2000,1)", "l");
  legend2->AddEntry(h_TChiHH800_stack, "TChiHH(800,1)", "l");
  legend2->AddEntry(h_T5HH2200_stack, "T5HH(2200,1)", "l");
  legend2->AddEntry(h_TChiHH1000_stack, "TChiHH(1000,1)", "l");
  legend2->SetNColumns(2);legend2->SetBorderSize(0);
  legend2->SetTextSize(0.022);
  legend->Draw("same"); legend2->Draw("same");

  can_h->Update();can_h->Modified();

  TString savename = graphName+"_Stack";
  cdStack->cd();
  addLumiCanv(can_h);
  can_h->Write(savename);
  if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/bkgest/"+savename+".pdf","PDF");
  // can_h->SaveAs(savename+".pdf","PDF");
  delete can_h;
}

/*

TH1F * h_QCD_stack = (TH1F*)J2pt_M_baseline_QCD->Clone("h_QCD_stack")
TH1F * h_TT_stack = (TH1F*)J2pt_M_baseline_TT->Clone("h_TT_stack")
TH1F * h_WJets_stack = (TH1F*)J2pt_M_baseline_WJets->Clone("h_WJets_stack")
TH1F * h_ZJets_stack = (TH1F*)J2pt_M_baseline_ZJets->Clone("h_ZJets_stack")
TH1F * h_SnglT_stack = (TH1F*)J2pt_M_baseline_SnglT->Clone("h_SnglT_stack")

TH1F * h_TChiHH500_stack = (TH1F*)J2pt_M_baseline_TChiHH500_LSP1->Clone("h_TChiHH500")
TH1F * h_TChiHH800_stack = (TH1F*)J2pt_M_baseline_TChiHH800_LSP1->Clone("h_TChiHH800")
TH1F * h_TChiHH1000_stack = (TH1F*)J2pt_M_baseline_TChiHH1000_LSP1->Clone("h_TChiHH1000")

h_QCD_stack->SetFillColor(kGray); h_QCD_stack->SetMarkerStyle(21); h_QCD_stack->SetMarkerColor(kGray); h_QCD_stack->SetLineColor(kBlack);
h_TT_stack->SetFillColor(kCyan);h_TT_stack->SetMarkerStyle(21); h_TT_stack->SetMarkerColor(kCyan); h_TT_stack->SetLineColor(kBlack);
h_WJets_stack->SetFillColor(kBlue);h_WJets_stack->SetMarkerStyle(21); h_WJets_stack->SetMarkerColor(kBlue); h_WJets_stack->SetLineColor(kBlack);
h_ZJets_stack->SetFillColor(kGreen+1); h_ZJets_stack->SetMarkerStyle(21); h_ZJets_stack->SetMarkerColor(kGreen+1); h_ZJets_stack->SetLineColor(kBlack);
h_SnglT_stack->SetFillColor(kOrange); h_SnglT_stack->SetMarkerStyle(21); h_SnglT_stack->SetMarkerColor(kOrange); h_SnglT_stack->SetLineColor(kBlack);

h_TChiHH500_stack->SetMarkerStyle(1); h_TChiHH500_stack->SetMarkerColor(kRed); h_TChiHH500_stack->SetFillColor(0);
h_TChiHH500_stack->SetLineColor(kRed); h_TChiHH500_stack->SetLineStyle(2); h_TChiHH500_stack->SetLineWidth(2);
h_TChiHH800_stack->SetMarkerStyle(1); h_TChiHH800_stack->SetMarkerColor(kBlack); h_TChiHH800_stack->SetFillColor(0);
h_TChiHH800_stack->SetLineColor(kBlack); h_TChiHH800_stack->SetLineStyle(2); h_TChiHH800_stack->SetLineWidth(2);
h_TChiHH1000_stack->SetMarkerStyle(1); h_TChiHH1000_stack->SetMarkerColor(kMagenta); h_TChiHH1000_stack->SetFillColor(0);
h_TChiHH1000_stack->SetLineColor(kMagenta); h_TChiHH1000_stack->SetLineStyle(2); h_TChiHH1000_stack->SetLineWidth(2);

THStack * doubleBStack = new THStack("hs","");
doubleBStack->Add(h_ZJets_stack);
doubleBStack->Add(h_WJets_stack);
doubleBStack->Add(h_TT_stack);
doubleBStack->Add(h_SnglT_stack);
doubleBStack->Add(h_QCD_stack);

TCanvas * can_h = new TCanvas("j1m","j1m", 50, 50, 800, 800);
can_h->SetLogy();

doubleBStack->Draw("hist");
doubleBStack->GetYaxis()->SetTitle("Events");
doubleBStack->SetMinimum(0.1); doubleBStack->SetMaximum(300);
doubleBStack->GetXaxis()->SetTitle("Subleading jet soft-drop mass [GeV]");

h_TChiHH500_stack->Draw("same hist"); h_TChiHH800_stack->Draw("same hist"); h_TChiHH1000_stack->Draw("same hist");


TLegend* legend = new TLegend(0.35,0.75,0.9,0.9) ;
legend->AddEntry(h_QCD_stack, "QCD", "f");
legend->AddEntry(h_TT_stack, "TT", "f");
legend->AddEntry(h_SnglT_stack, "SnglT", "f");
legend->AddEntry(h_WJets_stack, "WJets", "f");
legend->AddEntry(h_ZJets_stack, "ZJets", "f");
legend->AddEntry(h_TChiHH500_stack, "TChiHH500_LSP1", "l");
legend->AddEntry(h_TChiHH800_stack, "TChiHH800_LSP1", "l");
legend->AddEntry(h_TChiHH1000_stack, "TChiHH1000_LSP1", "l");
legend->SetNColumns(4);
legend->SetBorderSize(0);
legend->Draw("same");
can_h->Update();can_h->Modified();

can_h->SaveAs("SubLeadJetMass_J1SR.pdf","PDF");
*/

void makeSingleLeptStackPlot(TH1F* h_TT,TH1F* h_WJets,TH1F* h_SnglT, TH1F* h_dataOrig, TString which = ""){
  TH1F * h_TT_stack = (TH1F*)h_TT->Clone("TTAll");
  h_TT_stack->SetFillColor(kCyan); h_TT_stack->SetMarkerStyle(21); h_TT_stack->SetMarkerColor(kCyan);

  TH1F *h_WJets_stack = (TH1F*)h_WJets->Clone("WJetsAll");
  h_WJets_stack->SetFillColor(kBlue); h_WJets_stack->SetMarkerStyle(21); h_WJets_stack->SetMarkerColor(kBlue);

  TH1F * h_SnglT_stack = (TH1F*)h_SnglT->Clone("SnglTAll");
  h_SnglT_stack->SetFillColor(kOrange); h_SnglT_stack->SetMarkerStyle(21); h_SnglT_stack->SetMarkerColor(kOrange);

  THStack * doubleBStack = new THStack("hs","");
  doubleBStack->Add(h_TT_stack);
  doubleBStack->Add(h_WJets_stack);
  doubleBStack->Add(h_SnglT_stack);

  TH1F * h_MC_sum = (TH1F*)h_TT->Clone("TTAll"); h_MC_sum->Add(h_WJets_stack); h_MC_sum->Add(h_SnglT_stack);
  h_MC_sum->SetLineColor(kBlack);

  TH1F * h_data = (TH1F*)h_dataOrig->Clone("h_data");
  h_data->SetLineColor(kBlack); h_data->SetMarkerColor(kBlack); h_data->SetMarkerStyle(20); h_data->SetMarkerSize(0.7);

  TString graphName = "";
  if (which == "j1bb") graphName = "LeadJetBBTag";
  else if (which == "j2bb") graphName = "SubLeadJetBBTag";
  else if (which == "j1m") graphName = "LeadJetMass";
  else if (which == "j2m") graphName = "SubLeadJetMass";
  else if (which == "j1pt") graphName = "LeadJetPt";
  else if (which == "j2pt") graphName = "SubLeadJetPt";

  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, W, H);
  styleCanvas(can_h);
  can_h->SetLogy();

  doubleBStack->Draw("hist");
  doubleBStack->GetYaxis()->SetTitle("Events");
  h_MC_sum->Draw("l same");
  h_data->Draw("lp same");
  // doubleBStack->SetMinimum(0.1); doubleBStack->SetMaximum(300);

  if (which == "j1bb") doubleBStack->GetXaxis()->SetTitle("Leading jet deep bb-tag");
  else if (which == "j1bb") doubleBStack->GetXaxis()->SetTitle("Subleading jet deep bb-tag");
  else if (which == "j1m") doubleBStack->GetXaxis()->SetTitle("Leading jet soft-drop mass [GeV]");
  else if (which == "j2m") doubleBStack->GetXaxis()->SetTitle("Subleading jet soft-drop mass [GeV]");
  else if (which == "j1pt") doubleBStack->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
  else if (which == "j2pt") doubleBStack->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");

  TLegend* legend = new TLegend(0.35,0.8,0.9,0.9) ;
  legend->AddEntry(h_TT_stack, "TT", "f");
  legend->AddEntry(h_WJets_stack, "WJets", "f");
  legend->AddEntry(h_SnglT_stack, "SnglT", "f");
  legend->AddEntry(h_data, "Data", "lp");
  legend->SetNColumns(4);
  legend->SetBorderSize(0);
  legend->Draw("same");
  can_h->Update(); can_h->Modified();

  TString savename = graphName+"_Stack";
  cdStack->cd();
  addLumiCanv(can_h);
  can_h->Write(savename);
  if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/bkgest/"+savename+".pdf","PDF");
  delete can_h;
}

void makePhotonStackPlot(TH1F* h_QCD,TH1F* h_GJets, TString which = ""){
  TH1F * h_QCD_stack = (TH1F*)h_QCD->Clone("QCDAll");
  h_QCD_stack->SetFillColor(kCyan); h_QCD_stack->SetMarkerStyle(21); h_QCD_stack->SetMarkerColor(kCyan);

  TH1F *h_GJets_stack = (TH1F*)h_GJets->Clone("GJetsAll");
  h_GJets_stack->SetFillColor(kBlue); h_GJets_stack->SetMarkerStyle(21); h_GJets_stack->SetMarkerColor(kBlue);

  THStack * doubleBStack = new THStack("hs","");
  doubleBStack->Add(h_QCD_stack);
  doubleBStack->Add(h_GJets_stack);

  TString graphName = "";
  if (which == "j1bb") graphName = "LeadJetBBTag";
  else if (which == "j2bb") graphName = "SubLeadJetBBTag";
  else if (which == "j1m") graphName = "LeadJetMass";
  else if (which == "j2m") graphName = "SubLeadJetMass";
  else if (which == "j1pt") graphName = "LeadJetPt";
  else if (which == "j2pt") graphName = "SubLeadJetPt";

  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, W, H);
  styleCanvas(can_h);
  can_h->SetLogy();

  doubleBStack->Draw("hist");
  doubleBStack->GetYaxis()->SetTitle("Events");
  doubleBStack->SetMinimum(0.1); doubleBStack->SetMaximum(300);

  if (which == "j1bb") doubleBStack->GetXaxis()->SetTitle("Leading jet deep bb-tag");
  else if (which == "j1bb") doubleBStack->GetXaxis()->SetTitle("Subleading jet deep bb-tag");
  else if (which == "j1m") doubleBStack->GetXaxis()->SetTitle("Leading jet soft-drop mass [GeV]");
  else if (which == "j2m") doubleBStack->GetXaxis()->SetTitle("Subleading jet soft-drop mass [GeV]");
  else if (which == "j1pt") doubleBStack->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
  else if (which == "j2pt") doubleBStack->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");

  TLegend* legend = new TLegend(0.35,0.8,0.9,0.9) ;
  legend->AddEntry(h_QCD_stack, "QCD", "f");
  legend->AddEntry(h_GJets_stack, "GJets", "f");
  legend->SetNColumns(4);
  legend->SetBorderSize(0);
  legend->Draw("same");
  can_h->Update(); can_h->Modified();

  TString savename = graphName+"_Stack";
  cdStack->cd();
  addLumiCanv(can_h);
  can_h->Write(savename);
  if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/bkgest/"+savename+".pdf","PDF");
  delete can_h;
}

void makeMETStack(TH1F* h_QCD,TH1F* h_TT,TH1F* h_WJets,TH1F* h_ZJets,TH1F* h_SnglT, TH1F* h_T5HH1600,TH1F* h_T5HH2000,TH1F* h_T5HH2200,TH1F* h_TChiHH500,TH1F* h_TChiHH800, TH1F* h_TChiHH1000,TString tagType = ""){
  Int_t col_zjets = TColor::GetColorTransparent(kOrange+1, 0.90);
  Int_t col_wjets = TColor::GetColorTransparent(ci3, 0.90);
  Int_t col_qcd = TColor::GetColorTransparent(ci2, 0.95);
  Int_t col_tt = TColor::GetColorTransparent(ci, 0.95);
  Int_t col_snglt = TColor::GetColorTransparent(kGray+2, 0.95);

  h_QCD->SetFillColor(col_qcd); h_QCD->SetMarkerStyle(21); h_QCD->SetMarkerColor(col_qcd); h_QCD->SetLineColor(kBlack);
  h_TT->SetFillColor(col_tt); h_TT->SetMarkerStyle(21); h_TT->SetMarkerColor(col_tt); h_TT->SetLineColor(kBlack);
  h_SnglT->SetFillColor(col_snglt); h_SnglT->SetMarkerStyle(21); h_SnglT->SetMarkerColor(col_snglt); h_SnglT->SetLineColor(kBlack);
  h_WJets->SetFillColor(col_wjets);h_WJets->SetMarkerStyle(21); h_WJets->SetMarkerColor(col_wjets); h_WJets->SetLineColor(kBlack);
  h_ZJets->SetFillColor(col_zjets);h_ZJets->SetMarkerStyle(21); h_ZJets->SetMarkerColor(col_zjets); h_ZJets->SetLineColor(kBlack);

  h_T5HH1600->SetMarkerStyle(1); h_T5HH1600->SetMarkerColor(kRed); h_T5HH1600->SetLineColor(kRed); h_T5HH1600->SetFillColor(0); h_T5HH1600->SetLineWidth(3);
  h_T5HH2000->SetMarkerStyle(1); h_T5HH2000->SetMarkerColor(kBlack); h_T5HH2000->SetLineColor(kBlack); h_T5HH2000->SetFillColor(0); h_T5HH2000->SetLineWidth(3);
  h_T5HH2200->SetMarkerStyle(1); h_T5HH2200->SetMarkerColor(kMagenta); h_T5HH2200->SetLineColor(kMagenta); h_T5HH2200->SetFillColor(0); h_T5HH2200->SetLineWidth(3);

  h_TChiHH500->SetMarkerStyle(1); h_TChiHH500->SetMarkerColor(kRed); h_TChiHH500->SetLineColor(kRed); h_TChiHH500->SetLineStyle(2); h_TChiHH500->SetFillColor(0);  h_TChiHH500->SetLineWidth(3);
  h_TChiHH800->SetMarkerStyle(1); h_TChiHH800->SetMarkerColor(kBlack); h_TChiHH800->SetLineColor(kBlack); h_TChiHH800->SetLineStyle(2); h_TChiHH800->SetFillColor(0); h_TChiHH800->SetLineWidth(3);
  h_TChiHH1000->SetMarkerStyle(1); h_TChiHH1000->SetMarkerColor(kMagenta); h_TChiHH1000->SetLineColor(kMagenta); h_TChiHH1000->SetLineStyle(2); h_TChiHH1000->SetFillColor(0); h_TChiHH1000->SetLineWidth(3);


  THStack * METStack = new THStack("hs","");
  METStack->Add(h_SnglT);
  METStack->Add(h_QCD);
  METStack->Add(h_WJets);
  METStack->Add(h_ZJets);
  METStack->Add(h_TT);


  TString graphName = "METStack_"+tagType;
  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, 900, 900);
  // styleCanvas(can_h);
  can_h->SetLogy();
  can_h->SetTopMargin(0.08); can_h->SetLeftMargin(0.14); can_h->SetRightMargin(0.08); can_h->SetBottomMargin(0.12);

  METStack->Draw("hist");
  METStack->SetTitle(";p_{T}^{miss} [GeV];Events");
  METStack->GetYaxis()->SetLabelSize(0.035); METStack->GetYaxis()->SetTitleSize(0.05); METStack->GetYaxis()->SetTitleOffset(1.0);
  METStack->GetXaxis()->SetLabelSize(0.035); METStack->GetXaxis()->SetTitleSize(0.05); METStack->GetXaxis()->SetTitleOffset(0.95);

  METStack->SetMinimum(0.1); METStack->SetMaximum(3000);
  if (tagType=="baseline") METStack->SetMaximum(3E4);

  // h_T5HH1600->Draw("same hist"); h_T5HH2000->Draw("same hist"); h_T5HH2200->Draw("same hist");
  // h_TChiHH500->Draw("same hist"); h_TChiHH800->Draw("same hist"); h_TChiHH1000->Draw("same hist");

  h_T5HH1600->Draw("same hist"); h_TChiHH500->Draw("same hist");

  TLegend* legend = new TLegend(0.17,0.65,0.88,0.85) ;
  legend->AddEntry(h_TT, "t#bar{t}+X", "f");
  legend->AddEntry(h_QCD, "QCD", "f");
  legend->AddEntry(h_ZJets, "Z+jets", "f");
  legend->AddEntry(h_SnglT, "Other", "f");
  legend->AddEntry(h_WJets, "W+jets", "f");
  legend->AddEntry(h_TChiHH500, "TChiHH-G(500,1)", "l");
  legend->AddEntry((TObject*)0, "", "");
  legend->AddEntry(h_T5HH1600, "T5HH(1600,1)", "l");
  legend->SetNColumns(2);legend->SetBorderSize(0);
  legend->SetTextSize(0.042);


  legend->Draw("same");

  can_h->Update(); can_h->Modified();

  TString savename = "MET"+tagType+"_Stack";
  cdStack->cd();
  // addLumiCanv(can_h);

  TLatex ltitle(0.17, 0.88, "#font[62]{CMS}#scale[0.76]{#font[52]{ Simulation Supplementary}}");
  TLatex rtitle(0.92, 0.948, "#scale[0.8]{137 fb^{-1} (13 TeV)}");
  rtitle.SetTextFont(42);
  ltitle.SetNDC(); rtitle.SetNDC(); ltitle.SetTextAlign(12);  rtitle.SetTextAlign(32);
  ltitle.Draw("same"); rtitle.Draw("same");


  can_h->Write(savename);
  if (savePDFs) can_h->SaveAs("boostedFigures/"+whichRegion+"/bkgest/"+savename+".pdf","PDF");
  can_h->SaveAs(savename+".pdf","PDF");
  can_h->SaveAs(savename+".png");
  delete can_h;
}

void makeMETNorm(vector<TH1F*> dem_histos, TString bkgType = "", TString tagType = "") {
  //dem_histos will be [A,B,C,COpt,D,DOpt]
  TH1F * h_A   = (TH1F*)dem_histos[0]->Clone("h_A"); TH1F * h_B   = (TH1F*)dem_histos[1]->Clone("h_B");
  TH1F * h_C = (TH1F*)dem_histos[2]->Clone("h_C"); TH1F * h_COpt = (TH1F*)dem_histos[3]->Clone("h_COpt");
  TH1F * h_D = (TH1F*)dem_histos[4]->Clone("h_D"); TH1F * h_DOpt = (TH1F*)dem_histos[5]->Clone("h_DOpt");

  DrawOverflow(h_A); DrawOverflow(h_B); DrawOverflow(h_C); DrawOverflow(h_D); DrawOverflow(h_COpt); DrawOverflow(h_DOpt);

  TH1F *h_0H = (TH1F*)h_C->Clone("h_0H"); h_0H->Add(h_D);
  TH1F *h_0HOpt = (TH1F*)h_COpt->Clone("h_0HOpt"); h_0HOpt->Add(h_DOpt);

  h_A->SetMarkerStyle(20); h_A->SetMarkerSize(0.85); h_A->SetLineColor(kRed); h_A->SetMarkerColor(kRed);
  h_B->SetMarkerStyle(20); h_B->SetMarkerSize(0.85); h_B->SetLineColor(kMagenta); h_B->SetMarkerColor(kMagenta);
  h_C->SetMarkerStyle(20); h_C->SetMarkerSize(0.85); h_C->SetLineColor(kBlue); h_C->SetMarkerColor(kBlue);
  h_D->SetMarkerStyle(20); h_D->SetMarkerSize(0.85); h_D->SetLineColor(kGreen+1); h_D->SetMarkerColor(kGreen+1);
  h_0HOpt->SetMarkerStyle(20); h_0HOpt->SetMarkerSize(0.85); h_0HOpt->SetLineColor(kBlack); h_0HOpt->SetMarkerColor(kBlack);


  TString plotName;
  TString thisRegion;
  TString thisCut= "BTagsT>0";

  if (whichRegion=="signal") thisRegion = "0-lepton";
  else if (whichRegion=="singleLept") thisRegion = "1-lepton";

  plotName = thisRegion+": "+bkgType+";p_{T}^{miss} [GeV];Normalized to area";
  h_A->SetTitle(plotName);
  h_0HOpt->SetTitle(plotName);

  TString graphName = bkgType+"_"+tagType+"_METShape";
  TGraphAsymmErrors * graph = new TGraphAsymmErrors(h_A, h_0HOpt, "pois");
  styleGraph(graph);
  graph->SetMinimum(-0.1);
  graph->GetXaxis()->SetRangeUser(300,1400);
  graph->SetTitle(";p_{T}^{miss} [GeV];2HSR / 0HCuts");
  if (tagType=="Single") graph->SetTitle(";p_{T}^{miss} [GeV];1HSR / 0HCuts");

  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, W, H);
  styleCanvas(can_h);

  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.25, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.25);
  pad1->SetTickx(0); pad1->SetTicky(0);
  pad1->SetPad(0., 1 - up_height, 1., 1.00);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTopMargin(0.08); pad1->SetLeftMargin(0.10); pad1->SetRightMargin(0.04);
  pad1->Draw();

  pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.33); pad2->SetTopMargin(0.01);
  pad2->SetLeftMargin(0.10); pad2->SetRightMargin(0.04);
  pad2->Draw(); pad1->cd();

  h_A->GetXaxis()->SetLabelSize(0);
  h_A->GetYaxis()->SetTitleOffset(0.8);

  //try normalizing to 1
  h_A->Scale(1/h_A->Integral());
  h_B->Scale(1/h_B->Integral());
  h_C->Scale(1/h_C->Integral());
  h_D->Scale(1/h_D->Integral());
  h_0HOpt->Scale(1/h_0HOpt->Integral());

  h_A->Draw();
  h_B->Draw("same");
  h_C->Draw("same");
  h_D->Draw("same");
  h_0HOpt->Draw("same");

  TLegend* legend = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend->SetBorderSize(0);
  TString legendEntryName = tagType+" SR";
  if (tagType=="Single") {
    legend->AddEntry(h_A, "1H SR", "PL");
    legend->AddEntry(h_B, "1H SB", "PL");
  }
  else if (tagType=="Double") {
    legend->AddEntry(h_A, "2H SR", "PL");
    legend->AddEntry(h_B, "2H SB", "PL");
  }
  legend->AddEntry(h_C, "0H SR", "PL");
  legend->AddEntry(h_D, "0H SB", "PL");
  legend->AddEntry(h_0HOpt, "0H SB+SR,"+thisCut, "PL");

  legend->Draw();

  pad2->cd();
  graph->Draw("APE");
  graph->GetYaxis()->SetTitleOffset(0.25); graph->GetYaxis()->SetTitleSize(0.13);
  graph->GetXaxis()->SetTitleOffset(0.9); graph->GetXaxis()->SetTitleSize(0.14);
  graph->GetXaxis()->SetLabelSize(0.14); graph->GetYaxis()->SetLabelSize(0.10);
  graph->GetYaxis()->SetNdivisions(505);
  graph->Draw("APE");


  // TF1*f0=new TF1("f0","pol0", 300,1400);
  // graph->Fit("f0","Q","R",300,1400);
  // double p0 = f0->GetParameter(0); //this does, indeed, work
  // double error = f0->GetParError(0);
  // graph->GetFunction("f0")->SetBit(TF1::kNotDraw);
  //
  // TLine *line = new TLine(300.0,p0,1400.0,p0);
  // line->SetLineColor(kRed);
  // line->SetLineWidth(3);
  // line->SetLineStyle(2);
  // line->Draw("same");
  // std::string lineConst = to_string(p0); std::string lineErr = to_string(error);
  // TString constString = "Const: "+lineConst+" #pm "+ lineErr;
  // TLatex *t = new TLatex(0.2,0.85,constString);
  // t->SetNDC(); t->SetTextFont(52);
  // t->SetTextSize(0.13);
  // t->Draw();

  pad1->cd();
  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize/12);
  latex.DrawLatex(0.95,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(cmsTextSize*1.3);
  latex.DrawLatex(0.20, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextAlign(align_);
  latex.SetTextSize(cmsTextSize*0.85); latex.DrawLatex(0.26, 0.88, extraText);

  cdMET->cd();
  can_h->Write(graphName);
  delete can_h;
}

void makeMETNormCompare(vector<TH1F*> dem_histos, TString bkgType = "") {
  gStyle->SetTextFont(52);
  TH1F * h_2HSR  = (TH1F*)dem_histos[0]->Clone("h_2HSR"); TH1F * h_1HSR  = (TH1F*)dem_histos[1]->Clone("h_1HSR");
  TH1F * h_0HbSR = (TH1F*)dem_histos[2]->Clone("h_0HbSR");
  TH1F * h_0HbSRData = (TH1F*)dem_histos[3]->Clone("h_0HbSRData");

  TH1F * h_2HSRNoScale = (TH1F*)dem_histos[0]->Clone("h_2HSRNoScale");
  TH1F * h_1HSRNoScale = (TH1F*)dem_histos[1]->Clone("h_1HSRNoScale");
  TH1F * h_0HbNoScale = (TH1F*)h_0HbSR->Clone("h_0HbNoScale");
  TH1F * h_0HbDataNoScale = (TH1F*)h_0HbSRData->Clone("h_0HbDataNoScale");
  DrawOverflow(h_2HSR); DrawOverflow(h_1HSR); DrawOverflow(h_0HbSR); DrawOverflow(h_0HbSRData);

  h_2HSR->SetMarkerStyle(20); h_2HSR->SetMarkerSize(1.2); h_2HSR->SetLineColor(kRed); h_2HSR->SetLineWidth(2); h_2HSR->SetMarkerColor(kRed);
  h_1HSR->SetMarkerStyle(20); h_1HSR->SetMarkerSize(1.2); h_1HSR->SetLineColor(kBlue); h_1HSR->SetLineWidth(2); h_1HSR->SetMarkerColor(kBlue);
  h_0HbSR->SetMarkerStyle(20); h_0HbSR->SetMarkerSize(1.2); h_0HbSR->SetLineColor(kBlack); h_0HbSR->SetLineWidth(2); h_0HbSR->SetMarkerColor(kBlack);
  h_0HbSRData->SetMarkerStyle(4); h_0HbSRData->SetMarkerSize(1.2); h_0HbSRData->SetLineColor(kBlack); h_0HbSRData->SetLineWidth(2); h_0HbSRData->SetLineStyle(2); h_0HbSRData->SetMarkerColor(kBlack);

  // TString test = "#it{#lower[0.22]{f}#kern[-1.3]{#scale[0.85]{_{bkg}}}}";
  // TString test = "#it{#lower[0.1]{f}#kern[0.03]{#lower[-0.2]{#scale[0.85]{_{bkg}}}}}";
  TString test = "#lower[0.15]{f}#kern[0.08]{#lower[-0.25]{#scale[0.85]{_{bkg}}}}";
  h_2HSR->SetTitle("; ;Fractional yield"); h_1HSR->SetTitle("; ;Fractional yield");
  // h_2HSR->SetTitle("; ;Fractional yield, "+test+" "); h_1HSR->SetTitle("; ;Fractional yield, "+test+" ");
  // h_2HSR->SetTitle("; ;Fractional yield, "+test+" "); h_1HSR->SetTitle("; ;Fractional yield, "+test+" ");


  h_2HSR->GetXaxis()->SetTitleSize(0); h_2HSR->SetStats(0); h_1HSR->GetXaxis()->SetTitleSize(0); h_1HSR->SetStats(0);

  h_2HSR->GetYaxis()->SetTitleSize(0.05); h_2HSR->GetYaxis()->SetLabelSize(0.04); h_2HSR->GetYaxis()->SetTitleOffset(0.9);
  // h_2HSR->GetYaxis()->SetTitleSize(0.046); h_2HSR->GetYaxis()->SetLabelSize(0.036);
  // h_1HSR->GetYaxis()->SetTitleOffset(0.6); h_1HSR->GetYaxis()->SetTitleSize(0.06);

  TString graphName2H = bkgType+"_2HMETShape";
  TString graphName1H = bkgType+"_1HMETShape";
  TGraphAsymmErrors * graph2H = new TGraphAsymmErrors(h_2HSR, h_0HbSR, "pois");
  TGraphAsymmErrors * graph1H = new TGraphAsymmErrors(h_1HSR, h_0HbSR, "pois");
  // styleGraph(graph2H); styleGraph(graph2H);
  graph2H->SetMinimum(-0.1); graph1H->SetMinimum(-0.1);
  graph2H->GetXaxis()->SetRangeUser(300,1400); graph1H->GetXaxis()->SetRangeUser(300,1400);
  // graph2H->SetTitle(";p_{T}^{miss} [GeV];#frac{SR}{0H+b}");
  graph2H->SetTitle(";p_{T}^{miss} [GeV];N_{SR} / N_{0H+b}");
  graph2H->GetYaxis()->SetTitleOffset(0.9);

  TCanvas * can_h = new TCanvas(graphName2H,graphName2H, 50, 50, 1200, 900);
  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.4, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.4);
  can_h->SetMargin(0., 0., 0., 0.); can_h->SetTicks(1,1); can_h->SetFillStyle(4000);

  pad1->SetPad(0., 0., 1., 1.);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTicks(1,1);pad1->SetFillStyle(4000);
  pad2->SetTicks(1,1);pad2->SetFillStyle(4000);

  pad2->SetPad(0., 0., 1., 1.);
  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad1->SetMargin(0.13, 0.055, 0.45, 0.07);
  pad2->SetMargin(0.13, 0.055, 0.12, 1.-0.44);
  pad1->Draw(); pad2->Draw(); pad1->cd();

  // gPad->SetLogy(); //doesn't help


  h_2HSR->GetXaxis()->SetLabelSize(0); h_1HSR->GetXaxis()->SetLabelSize(0);
  // h_2HSR->GetYaxis()->SetLabelSize(0.05);

  // Normalizing to 1
  h_2HSR->Scale(1/h_2HSR->Integral()); h_1HSR->Scale(1/h_1HSR->Integral());
  h_0HbSR->Scale(1/h_0HbSR->Integral()); h_0HbSRData->Scale(1/h_0HbSRData->Integral());
  h_2HSR->SetMaximum(1.2);
  h_2HSR->Draw(); h_1HSR->Draw("same"); h_0HbSR->Draw("same"); h_0HbSRData->Draw("same");


  TLegend* legend = new TLegend(0.58,0.7,0.92,0.9);
  // TLegend* legend = new TLegend(0.5,0.63,0.95,0.88,NULL,"brNDC") ;
  legend->SetBorderSize(0);
  legend->AddEntry(h_2HSR, "2H SR, MC", "PL");
  legend->AddEntry(h_1HSR, "1H SR, MC", "PL");
  legend->AddEntry(h_0HbSR, "0H+b, MC", "PL");
  legend->AddEntry(h_0HbSRData, "0H+b, Data", "PL");
  legend->Draw();

  pad2->cd();
  graph2H->Draw("APE");
  // graph2H->GetYaxis()->SetTitleSize(0.046); graph2H->GetXaxis()->SetTitleSize(0.046);
  // graph2H->GetYaxis()->SetLabelSize(0.036); graph2H->GetXaxis()->SetLabelSize(0.036);
  graph2H->GetYaxis()->SetTitleSize(0.05); graph2H->GetXaxis()->SetTitleSize(0.05);
  graph2H->GetYaxis()->SetLabelSize(0.04); graph2H->GetXaxis()->SetLabelSize(0.04);
  // graph2H->GetXaxis()->SetLabelSize(0.08); graph2H->GetYaxis()->SetLabelSize(0.07);
  graph2H->GetYaxis()->SetNdivisions(505);
  graph2H->GetYaxis()->CenterTitle();
  graph2H->SetMinimum(0.0);
  graph2H->SetMaximum(0.29);
  if (bkgType=="Data") graph2H->SetMaximum(0.39);
  graph2H->GetXaxis()->SetRangeUser(301.,1399.0);
  graph2H->SetLineColor(kRed); graph2H->SetLineWidth(2); graph2H->SetMarkerColor(kRed); graph2H->SetMarkerSize(1.2);


  // graph1H->GetYaxis()->SetTitleOffset(0.4); graph1H->GetYaxis()->SetTitleSize(0.08);
  // graph1H->GetXaxis()->SetTitleOffset(0.9); graph1H->GetXaxis()->SetTitleSize(0.10);
  // graph1H->GetXaxis()->SetLabelSize(0.08); graph1H->GetYaxis()->SetLabelSize(0.07);
  graph1H->GetYaxis()->SetNdivisions(505);
  graph1H->SetMinimum(0.0);
  graph1H->SetLineColor(kBlue); graph1H->SetLineWidth(2); graph1H->SetMarkerColor(kBlue); graph1H->SetMarkerSize(1.2);

  graph2H->Draw("APE");
  graph1H->Draw("PE same");
  // gPad->RedrawAxis();


  pad1->cd();
  // TLatex ltitle(0.15, 1.-0.5*can_h->GetTopMargin()-0.12, "#font[62]{CMS}#scale[0.76]{#font[52]{ Simulation}}");
  // TLatex ltitle(0.15, 1.-0.5*can_h->GetTopMargin()-0.12, "#font[62]{CMS}#scale[0.76]{#font[52]{ Preliminary}}");
  TLatex ltitle(0.15, 1.-0.5*can_h->GetTopMargin()-0.12, "#font[62]{CMS}");
  TLatex ltitle_sim(0.14, 1.-0.5*can_h->GetTopMargin()-0.17, "#scale[0.76]{#font[52]{ Preliminary}}");
  TLatex rtitle(0.94, 1.-0.5*can_h->GetTopMargin()-0.043, "#scale[0.95]{137 fb^{-1} (13 TeV)}");
  rtitle.SetTextFont(42);
  ltitle.SetNDC(); rtitle.SetNDC(); ltitle_sim.SetNDC();
  ltitle.SetTextAlign(12); ltitle_sim.SetTextAlign(12); rtitle.SetTextAlign(32);
  ltitle.Draw("same"); rtitle.Draw("same");


  cdMET->cd();
  can_h->Write(bkgType+"_METShapeComp");
  TString savename = "boostedFigures/"+whichRegion+"/bkgest/METShape_"+bkgType+".pdf";
  if (bkgType=="BkgSum") savename = "boostedFigures/"+whichRegion+"/bkgest/METShape.pdf";
  if (savePDFs) can_h->SaveAs(savename,"PDF");
  // can_h->SaveAs("METShape_Data.pdf","PDF");
  can_h->SaveAs("METShape.pdf","PDF");

  ltitle_sim.Draw("same");
  can_h->SaveAs("METShape_prelim.pdf","PDF");


  pad1->cd();
  h_2HSR->Draw(); h_0HbSR->Draw("same");
  pad2->cd();
  graph2H->Draw("APE");
  cdMET->cd(); can_h->Write(bkgType+"_METShapeComp2H");

  pad1->cd();
  h_1HSR->SetLineColor(kRed); h_1HSR->SetMarkerColor(kRed);
  h_1HSR->Draw(); h_0HbSR->Draw("same");
  pad2->cd();
  graph1H->SetLineColor(kBlack); graph1H->SetMarkerColor(kBlack);
  graph1H->Draw("APE");
  cdMET->cd(); can_h->Write(bkgType+"_METShapeComp1H");


  delete can_h;
}

void tableOfYields(vector<TH1F*> dem_histos, TString bkgType){
  TH1F * histo_A = (TH1F*)dem_histos[0]->Clone("histo_A"); TH1F * histo_A1 = (TH1F*)dem_histos[1]->Clone("histo_A1");
  TH1F * histo_B = (TH1F*)dem_histos[2]->Clone("histo_B"); TH1F * histo_B1 = (TH1F*)dem_histos[3]->Clone("histo_B1");
  TH1F * histo_C = (TH1F*)dem_histos[4]->Clone("histo_C"); TH1F * histo_D = (TH1F*)dem_histos[5]->Clone("histo_D");

  DrawOverflow(histo_A); DrawOverflow(histo_A1); DrawOverflow(histo_B); DrawOverflow(histo_B1); DrawOverflow(histo_C); DrawOverflow(histo_D);

  double a_int; double a_error;
  double a1_int; double a1_error;
  double b_int; double b_error;
  double b1_int; double b1_error;
  double c_int; double c_error;
  double d_int; double d_error;

  if (whichRegion=="signal"){
    a_int = histo_A->IntegralAndError(1,5,a_error,"");
    a1_int = histo_A1->IntegralAndError(1,5,a1_error,"");
    b_int = histo_B->IntegralAndError(1,5,b_error,"");
    b1_int = histo_B1->IntegralAndError(1,5,b1_error,"");
    c_int = histo_C->IntegralAndError(1,5,c_error,"");
    d_int = histo_D->IntegralAndError(1,5,d_error,"");
  }
  else {
    a_int = histo_A->IntegralAndError(0,5,a_error,"");
    a1_int = histo_A1->IntegralAndError(0,5,a1_error,"");
    b_int = histo_B->IntegralAndError(0,5,b_error,"");
    b1_int = histo_B1->IntegralAndError(0,5,b1_error,"");
    c_int = histo_C->IntegralAndError(0,5,c_error,"");
    d_int = histo_D->IntegralAndError(0,5,d_error,"");
  }


  TH1F *histo_pred2H = (TH1F*)histo_B->Clone("histo_pred");
  histo_pred2H->Multiply(histo_C);histo_pred2H->Divide(histo_D);

  TH1F *histo_pred1H = (TH1F*)histo_B1->Clone("histo_pred");
  histo_pred1H->Multiply(histo_C);histo_pred1H->Divide(histo_D);

  double bkgNorm2H; double bkgNorm2H_error;
  bkgNorm2H = b_int*c_int/d_int;
  bkgNorm2H_error = bkgNorm2H*sqrt( TMath::Power(b_error/b_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );

  double bkgNorm1H; double bkgNorm1H_error;
  bkgNorm1H = b1_int*c_int/d_int;
  bkgNorm1H_error = bkgNorm1H*sqrt( TMath::Power(b1_error/b1_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );

  double kappa2H = a_int/bkgNorm2H;
  double kappa2H_error = kappa2H*sqrt(TMath::Power(a_error/a_int,2) + TMath::Power(bkgNorm2H_error/bkgNorm2H,2));

  double kappa1H = a1_int/bkgNorm1H;
  double kappa1H_error = kappa1H*sqrt(TMath::Power(a1_error/a1_int,2) + TMath::Power(bkgNorm1H_error/bkgNorm1H,2));

  // TString yieldsFileName = "Yields/Yields_"+whichRegion+"_"+bkgType+".txt";
  TString yieldsFileName = "Yields_"+whichRegion+"_"+bkgType+".txt";

  ofstream yields;
  yields.open(yieldsFileName);

  yields<<"% "<< bkgType <<endl;
  yields <<"\\documentclass[11pt, oneside]{article}\n\n";
  yields<<"\\begin{document}\n"<<"\\begin{table}\n\\centering\n";
  yields<<"\\begin{tabular}{ |c|c| }\n"<<"\\hline\n";
  if (bkgType!="Data") yields<<"Region &MC Yields \\\\  \\hline \\hline\n";
  else yields<<"Region &Data Yields \\\\  \\hline \\hline\n";
  yields<<"2H SR & "<<std::setprecision(4)<< a_int <<" $\\pm$ "<< a_error <<"\\\\ \n";
  yields<<"2H SB & "<<std::setprecision(4)<< b_int <<" $\\pm$ "<< b_error <<"\\\\ \n";
  yields<<"1H SR & "<<std::setprecision(4)<< a1_int <<" $\\pm$ "<< a1_error <<"\\\\ \n";
  yields<<"1H SB & "<<std::setprecision(4)<< b1_int <<" $\\pm$ "<< b1_error <<"\\\\ \n";
  yields<<"0H SR & "<<std::setprecision(4)<< c_int <<" $\\pm$ "<< c_error <<"\\\\ \n";
  yields<<"0H SB & "<<std::setprecision(4)<< d_int <<" $\\pm$ "<< d_error <<"\\\\ \n";
  yields<<"BkgNorm2H & "<<std::setprecision(4)<< bkgNorm2H <<" $\\pm$ "<< bkgNorm2H_error<<"\\\\ \n";
  yields<<"BkgNorm1H & "<<std::setprecision(4)<< bkgNorm1H <<" $\\pm$ "<< bkgNorm1H_error <<"\\\\ \\hline\n \\hline\n";
  yields<<"Kappa2H & "<<std::setprecision(3)<< kappa2H << " $\\pm$ "<< kappa2H_error <<"\\\\ \n";
  yields<<"Kappa1H & "<<std::setprecision(3)<< kappa1H << " $\\pm$ "<< kappa1H_error <<"\\\\ \\hline\n";
  yields<<"\\end{tabular}\n"<<"\\end{table}\n"<<"\\end{document}\n";
  yields.close();
}

void giantTableOfYields(vector< vector<TH1F*> > dem_histos){
  vector<string> histoNames;
  vector<float> vec_aint; vector<float> vec_a1int; vector<float> vec_bint; vector<float> vec_b1int; vector<float> vec_cint; vector<float> vec_dint;
  vector<float> vec_aerr; vector<float> vec_a1err; vector<float> vec_berr; vector<float> vec_b1err; vector<float> vec_cerr; vector<float> vec_derr;
  vector<float> vec_bkgNorm2H; vector<float> vec_bkgNorm2H_err; vector<float> vec_bkgNorm1H; vector<float> vec_bkgNorm1H_err;
  vector<float> vec_kappa2H; vector<float> vec_kappa2H_err; vector<float> vec_kappa1H; vector<float> vec_kappa1H_err;

  if (dem_histos.size()==6) histoNames = {"BkgSum", "QCD", "ZJets", "WJets", "TT","SnglT"};
  else if (dem_histos.size()==4) histoNames = {"BkgSum", "WJets", "TT","SnglT"};
  else if (dem_histos.size()==3) histoNames = {"BkgSum", "QCD", "GJets"};
  else {
    std::cout<<"Histos be broke"<<std::endl;
    return;
  }
  //Now loop through each background
  for (int i = 0; i<dem_histos.size(); i++){
    TH1F * histo_A = (TH1F*)dem_histos[i][0]->Clone("histo_A"); TH1F * histo_A1 = (TH1F*)dem_histos[i][1]->Clone("histo_A1");
    TH1F * histo_B = (TH1F*)dem_histos[i][2]->Clone("histo_B"); TH1F * histo_B1 = (TH1F*)dem_histos[i][3]->Clone("histo_B1");
    TH1F * histo_C = (TH1F*)dem_histos[i][4]->Clone("histo_C"); TH1F * histo_D = (TH1F*)dem_histos[i][5]->Clone("histo_D");
    DrawOverflow(histo_A); DrawOverflow(histo_A1); DrawOverflow(histo_B); DrawOverflow(histo_B1); DrawOverflow(histo_C); DrawOverflow(histo_D);

    double a_int; double a_error;
    double a1_int; double a1_error;
    double b_int; double b_error;
    double b1_int; double b1_error;
    double c_int; double c_error;
    double d_int; double d_error;

    if (whichRegion=="signal"){
      a_int = histo_A->IntegralAndError(1,5,a_error,"");
      a1_int = histo_A1->IntegralAndError(1,5,a1_error,"");
      b_int = histo_B->IntegralAndError(1,5,b_error,"");
      b1_int = histo_B1->IntegralAndError(1,5,b1_error,"");
      c_int = histo_C->IntegralAndError(1,5,c_error,"");
      d_int = histo_D->IntegralAndError(1,5,d_error,"");
    }
    else {
      a_int = histo_A->IntegralAndError(0,5,a_error,"");
      a1_int = histo_A1->IntegralAndError(0,5,a1_error,"");
      b_int = histo_B->IntegralAndError(0,5,b_error,"");
      b1_int = histo_B1->IntegralAndError(0,5,b1_error,"");
      c_int = histo_C->IntegralAndError(0,5,c_error,"");
      d_int = histo_D->IntegralAndError(0,5,d_error,"");
    }

    TH1F *histo_pred2H = (TH1F*)histo_B->Clone("histo_pred"); histo_pred2H->Multiply(histo_C);histo_pred2H->Divide(histo_D);
    TH1F *histo_pred1H = (TH1F*)histo_B1->Clone("histo_pred"); histo_pred1H->Multiply(histo_C);histo_pred1H->Divide(histo_D);

    double bkgNorm2H; double bkgNorm2H_error;
    bkgNorm2H = b_int*c_int/d_int; bkgNorm2H_error = bkgNorm2H*sqrt( TMath::Power(b_error/b_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );
    double bkgNorm1H; double bkgNorm1H_error;
    bkgNorm1H = b1_int*c_int/d_int; bkgNorm1H_error = bkgNorm1H*sqrt( TMath::Power(b1_error/b1_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );
    double kappa2H = a_int/bkgNorm2H; double kappa2H_error = kappa2H*sqrt(TMath::Power(a_error/a_int,2) + TMath::Power(bkgNorm2H_error/bkgNorm2H,2));
    double kappa1H = a1_int/bkgNorm1H; double kappa1H_error = kappa1H*sqrt(TMath::Power(a1_error/a1_int,2) + TMath::Power(bkgNorm1H_error/bkgNorm1H,2));

    vec_aint.push_back(a_int); vec_aerr.push_back(a_error); vec_bint.push_back(b_int); vec_berr.push_back(b_error);
    vec_a1int.push_back(a1_int); vec_a1err.push_back(a1_error); vec_b1int.push_back(b1_int); vec_b1err.push_back(b1_error);
    vec_cint.push_back(c_int); vec_cerr.push_back(c_error); vec_dint.push_back(d_int); vec_derr.push_back(d_error);
    vec_bkgNorm2H.push_back(bkgNorm2H); vec_bkgNorm2H_err.push_back(bkgNorm2H_error); vec_bkgNorm1H.push_back(bkgNorm1H); vec_bkgNorm1H_err.push_back(bkgNorm1H_error);
    vec_kappa2H.push_back(kappa2H); vec_kappa2H_err.push_back(kappa2H_error); vec_kappa1H.push_back(kappa1H); vec_kappa1H_err.push_back(kappa1H_error);
  }



  TString yieldsFileName = "Yields/GiantYields_"+whichRegion+".txt";
  ofstream yields; yields.open(yieldsFileName);

  yields<<"% "<< whichRegion <<endl;
  yields <<"\\documentclass[11pt, oneside]{article}\n \\usepackage{rotating, graphicx}\n";
  yields<<"\\begin{document}\n"<<"\\begin{sidewaystable}\n";
  if (whichRegion=="signal") yields<<"\\begin{tabular}{ |c|c||ccccc| }\n"<<"\\hline\n";
  else if (whichRegion=="singleLept") yields<<"\\begin{tabular}{ |c|c||ccc| }\n"<<"\\hline\n";
  else if (whichRegion=="photon") yields<<"\\begin{tabular}{ |c|c||cc| }\n"<<"\\hline\n";
  yields<<"Region ";
  for (int i=0;i<vec_aint.size();i++) yields<<"& "<<histoNames[i];
  yields<<"\\\\  \\hline \\hline\n";
  yields<<"2H SR  ";
  for (int i=0;i<vec_aint.size();i++) yields<<std::setprecision(4)<< "& "<<vec_aint[i] <<"$\\pm$"<<std::setprecision(3)<< vec_aerr[i];
  yields<<"\\\\ \n";
  yields<<"2H SB  ";
  for (int i=0;i<vec_bint.size();i++) yields<<std::setprecision(4)<< "& "<<vec_bint[i] <<"$\\pm$"<<std::setprecision(3)<< vec_berr[i];
  yields<<"\\\\ \n";
  yields<<"1H SR  ";
  for (int i=0;i<vec_a1int.size();i++) yields<<std::setprecision(4)<< "& "<<vec_a1int[i] <<"$\\pm$"<<std::setprecision(3)<< vec_a1err[i];
  yields<<"\\\\ \n";
  yields<<"1H SB  ";
  for (int i=0;i<vec_b1int.size();i++) yields<<std::setprecision(4)<< "& "<<vec_b1int[i] <<"$\\pm$"<<std::setprecision(3)<< vec_b1err[i];
  yields<<"\\\\ \n";
  yields<<"0H SR  ";
  for (int i=0;i<vec_cint.size();i++) yields<<std::setprecision(4)<< "& "<<vec_cint[i] <<"$\\pm$"<<std::setprecision(3)<< vec_cerr[i];
  yields<<"\\\\ \n";
  yields<<"0H SR  ";
  for (int i=0;i<vec_dint.size();i++) yields<<std::setprecision(4)<< "& "<<vec_dint[i] <<"$\\pm$"<<std::setprecision(3)<< vec_derr[i];
  yields<<"\\\\ \n";
  yields<<"BkgNorm2H  ";
  for (int i=0;i<vec_bkgNorm2H.size();i++) yields<<std::setprecision(4)<< "& "<<vec_bkgNorm2H[i] <<"$\\pm$"<<std::setprecision(3)<< vec_bkgNorm2H_err[i];
  yields<<"\\\\ \n";
  yields<<"BkgNorm1H  ";
  for (int i=0;i<vec_bkgNorm1H.size();i++) yields<<std::setprecision(4)<< "& "<<vec_bkgNorm1H[i] <<"$\\pm$"<<std::setprecision(3)<< vec_bkgNorm1H_err[i];
  yields<<"\\\\ \n";
  yields<<"\\hline \\hline\n";
  yields<<"Kappa2H  ";
  for (int i=0;i<vec_kappa2H.size();i++) yields<<std::setprecision(3)<< "& "<<vec_kappa2H[i] <<"$\\pm$"<<std::setprecision(2)<< vec_kappa2H_err[i];
  yields<<"\\\\ \n";
  yields<<"Kappa1H  ";
  for (int i=0;i<vec_kappa1H.size();i++) yields<<std::setprecision(3)<< "& "<<vec_kappa1H[i] <<"$\\pm$"<<std::setprecision(2)<< vec_kappa1H_err[i];
  yields<<"\\\\ \n";
  yields<<"\\hline\n";
  yields<<"\\end{tabular}\n"<<"\\end{sidewaystable}\n"<<"\\end{document}\n";
  yields.close();
}

void tableOfFullPred(vector<TH1F*> dem_histos, TString bkgType){
  TH1F * histo_A = (TH1F*)dem_histos[0]->Clone("histo_A"); TH1F * histo_A1 = (TH1F*)dem_histos[1]->Clone("histo_A1");
  TH1F * histo_B = (TH1F*)dem_histos[2]->Clone("histo_B"); TH1F * histo_B1 = (TH1F*)dem_histos[3]->Clone("histo_B1");
  TH1F * histo_C = (TH1F*)dem_histos[4]->Clone("histo_C"); TH1F * histo_D = (TH1F*)dem_histos[5]->Clone("histo_D");
  TH1F * histo_METShape = (TH1F*)dem_histos[6]->Clone("histo_METShape");

  DrawOverflow(histo_A); DrawOverflow(histo_A1); DrawOverflow(histo_B); DrawOverflow(histo_B1); DrawOverflow(histo_C); DrawOverflow(histo_D); DrawOverflow(histo_METShape);

  double a_int; double a_error;
  double a1_int; double a1_error;
  double b_int; double b_error;
  double b1_int; double b1_error;
  double c_int; double c_error;
  double d_int; double d_error;

  if (whichRegion=="signal"){
    a_int = histo_A->IntegralAndError(1,5,a_error,"");
    a1_int = histo_A1->IntegralAndError(1,5,a1_error,"");
    b_int = histo_B->IntegralAndError(1,5,b_error,"");
    b1_int = histo_B1->IntegralAndError(1,5,b1_error,"");
    c_int = histo_C->IntegralAndError(1,5,c_error,"");
    d_int = histo_D->IntegralAndError(1,5,d_error,"");
  }
  else {
    a_int = histo_A->IntegralAndError(0,5,a_error,"");
    a1_int = histo_A1->IntegralAndError(0,5,a1_error,"");
    b_int = histo_B->IntegralAndError(0,5,b_error,"");
    b1_int = histo_B1->IntegralAndError(0,5,b1_error,"");
    c_int = histo_C->IntegralAndError(0,5,c_error,"");
    d_int = histo_D->IntegralAndError(0,5,d_error,"");
  }


  TH1F *histo_pred2H = (TH1F*)histo_METShape->Clone("histo_pred2H"); histo_pred2H->Scale(1/histo_pred2H->Integral());
  TH1F *histo_pred1H = (TH1F*)histo_METShape->Clone("histo_pred1H"); histo_pred1H->Scale(1/histo_pred1H->Integral());

  double bkgNorm2H; double bkgNorm2H_error;
  bkgNorm2H = b_int*c_int/d_int;
  bkgNorm2H_error = bkgNorm2H*sqrt( TMath::Power(b_error/b_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );

  double bkgNorm1H; double bkgNorm1H_error;
  bkgNorm1H = b1_int*c_int/d_int;
  bkgNorm1H_error = bkgNorm1H*sqrt( TMath::Power(b1_error/b1_int,2) + TMath::Power(c_error/c_int,2) + TMath::Power(d_error/d_int,2) );

  double bin1err_2H = histo_pred2H->GetBinContent(1)*bkgNorm2H*sqrt(TMath::Power(histo_pred2H->GetBinError(1)/histo_pred2H->GetBinContent(1),2) + TMath::Power(bkgNorm2H_error/bkgNorm2H,2));
  double bin2err_2H = histo_pred2H->GetBinContent(2)*bkgNorm2H*sqrt(TMath::Power(histo_pred2H->GetBinError(2)/histo_pred2H->GetBinContent(2),2) + TMath::Power(bkgNorm2H_error/bkgNorm2H,2));
  double bin3err_2H = histo_pred2H->GetBinContent(3)*bkgNorm2H*sqrt(TMath::Power(histo_pred2H->GetBinError(3)/histo_pred2H->GetBinContent(3),2) + TMath::Power(bkgNorm2H_error/bkgNorm2H,2));

  double bin1err_1H = histo_pred1H->GetBinContent(1)*bkgNorm1H*sqrt(TMath::Power(histo_pred1H->GetBinError(1)/histo_pred1H->GetBinContent(1),2) + TMath::Power(bkgNorm1H_error/bkgNorm1H,2));
  double bin2err_1H = histo_pred1H->GetBinContent(2)*bkgNorm1H*sqrt(TMath::Power(histo_pred1H->GetBinError(2)/histo_pred1H->GetBinContent(2),2) + TMath::Power(bkgNorm1H_error/bkgNorm1H,2));
  double bin3err_1H = histo_pred1H->GetBinContent(3)*bkgNorm1H*sqrt(TMath::Power(histo_pred1H->GetBinError(3)/histo_pred1H->GetBinContent(3),2) + TMath::Power(bkgNorm1H_error/bkgNorm1H,2));

  histo_pred2H->SetBinContent(1,histo_pred2H->GetBinContent(1)*bkgNorm2H);  histo_pred2H->SetBinError(1,bin1err_2H);
  histo_pred2H->SetBinContent(2,histo_pred2H->GetBinContent(2)*bkgNorm2H);  histo_pred2H->SetBinError(2,bin2err_2H);
  histo_pred2H->SetBinContent(3,histo_pred2H->GetBinContent(3)*bkgNorm2H);  histo_pred2H->SetBinError(3,bin3err_2H);

  histo_pred1H->SetBinContent(1,histo_pred1H->GetBinContent(1)*bkgNorm1H);  histo_pred1H->SetBinError(1,bin1err_1H);
  histo_pred1H->SetBinContent(2,histo_pred1H->GetBinContent(2)*bkgNorm1H);  histo_pred1H->SetBinError(2,bin2err_1H);
  histo_pred1H->SetBinContent(3,histo_pred1H->GetBinContent(3)*bkgNorm1H);  histo_pred1H->SetBinError(3,bin3err_1H);

  TString yieldsFileName = "Yields/FullClosureYields_"+whichRegion+"_"+bkgType+".txt";

  ofstream yields;
  yields.open(yieldsFileName);

  yields<<"% Full closure yeilds"<< bkgType <<", "<<whichRegion<<endl;
  yields <<"\\documentclass[11pt, oneside]{article}\n\n";
  yields<<"\\begin{document}\n"<<"\\begin{table}\n\\centering\n";
  yields<<"\\begin{tabular}{ |c|c|c|c| }\n"<<"\\hline\n";
  yields<<"Region &BkgNorm & Prediction & Actual \\\\  \\hline \\hline\n";
  yields<<"2H SR & "<<std::setprecision(4)<< bkgNorm2H <<" $\\pm$ "<< bkgNorm2H_error <<"& - & - \\\\ \n";
  yields<<"MET [300,500] GeV & - &"<<std::setprecision(4)<< histo_pred2H->GetBinContent(1) <<" $\\pm$ "<< histo_pred2H->GetBinError(1) <<"&"<< histo_A->GetBinContent(1)<<" $\\pm$ "<<histo_A->GetBinError(1)<<"\\\\ \n";
  yields<<"MET [500,700] GeV & - &"<<std::setprecision(4)<< histo_pred2H->GetBinContent(2) <<" $\\pm$ "<< histo_pred2H->GetBinError(2) <<"&"<< histo_A->GetBinContent(2)<<" $\\pm$ "<<histo_A->GetBinError(2)<<"\\\\ \n";
  yields<<"MET [700, Inf] GeV & - &"<<std::setprecision(4)<< histo_pred2H->GetBinContent(3) <<" $\\pm$ "<< histo_pred2H->GetBinError(3) <<"&"<< histo_A->GetBinContent(3)<<" $\\pm$ "<<histo_A->GetBinError(3)<<"\\\\ \n";
  yields<<"\\hline\n";
  yields<<"1H SR & "<<std::setprecision(4)<< bkgNorm1H <<" $\\pm$ "<< bkgNorm1H_error <<"& - & - \\\\ \n";
  yields<<"MET [300,500] GeV & - &"<<std::setprecision(4)<< histo_pred1H->GetBinContent(1) <<" $\\pm$ "<< histo_pred1H->GetBinError(1) <<"&"<< histo_A1->GetBinContent(1)<<" $\\pm$ "<<histo_A1->GetBinError(1)<<"\\\\ \n";
  yields<<"MET [500,700] GeV & - &"<<std::setprecision(4)<< histo_pred1H->GetBinContent(2) <<" $\\pm$ "<< histo_pred1H->GetBinError(2) <<"&"<< histo_A1->GetBinContent(2)<<" $\\pm$ "<<histo_A1->GetBinError(2)<<"\\\\ \n";
  yields<<"MET [700, Inf] GeV & - &"<<std::setprecision(4)<< histo_pred1H->GetBinContent(3) <<" $\\pm$ "<< histo_pred1H->GetBinError(3) <<"&"<< histo_A1->GetBinContent(3)<<" $\\pm$ "<<histo_A1->GetBinError(3)<<"\\\\ \n";
  yields<<"\\hline\n";
  yields<<"\\end{tabular}\n"<<"\\end{table}\n"<<"\\end{document}\n";
  yields.close();
}

void tableOfMETNorm(vector<TH1F*> dem_histos, TString bkgType){
  TH1F * histo_A = (TH1F*)dem_histos[0]->Clone("histo_A"); TH1F * histo_A1 = (TH1F*)dem_histos[1]->Clone("histo_A1");
  TH1F * histo_B = (TH1F*)dem_histos[2]->Clone("histo_B"); TH1F * histo_B1 = (TH1F*)dem_histos[3]->Clone("histo_B1");
  TH1F * histo_CNoCuts = (TH1F*)dem_histos[4]->Clone("histo_CNoCuts");
  TH1F * histo_0HWithCuts = (TH1F*)dem_histos[5]->Clone("histo_0HWithCuts");
  TH1F * histo_DNoCuts = (TH1F*)dem_histos[6]->Clone("histo_DNoCuts");

  DrawOverflow(histo_A); DrawOverflow(histo_A1); DrawOverflow(histo_B); DrawOverflow(histo_B1); DrawOverflow(histo_CNoCuts); DrawOverflow(histo_0HWithCuts); DrawOverflow(histo_DNoCuts);

  TH1F *h_2H_Norm = (TH1F*) histo_A->Clone("h_2H_Norm"); h_2H_Norm->Scale(1/h_2H_Norm->Integral(1,4));
  TH1F *h_1H_Norm = (TH1F*) histo_A1->Clone("h_1H_Norm"); h_1H_Norm->Scale(1/h_1H_Norm->Integral(1,4));
  TH1F *h_0H_Norm = (TH1F*) histo_0HWithCuts->Clone("h_0H_Norm"); h_0H_Norm->Scale(1/h_0H_Norm->Integral(1,4));

  // TString yieldsFileName = "Yields/METShape_"+whichRegion+"_"+bkgType+".txt";
  TString yieldsFileName = "METShape_"+whichRegion+"_"+bkgType+".txt";
  ofstream yields;
  yields.open(yieldsFileName);
  yields<<"% Background: "<< bkgType <<endl;
  yields<<"% Region: "<< whichRegion <<endl;
  yields <<"\\documentclass[12pt]{article}\n\n";
  yields<<"\\usepackage{numprint}\n"<<"\\begin{document}\n"<<"\\begin{table}\n";
  yields<<"\\npdecimalsign{.}\n"<<"\\nprounddigits{3}\n"<<"\\centering\n";
  yields<<"\\begin{tabular}{ |c|n{4}{3}n{2}{3}|n{3}{3}n{2}{3}| }\n"<<"\\hline\n";
  yields<<"MET region & \\multicolumn{2}{c|}{MC Yields} & \\multicolumn{2}{c|}{Bkg Frac.}  \\\\ \n";
  yields<<"\\hline \\hline\n";
  yields<<"\\multicolumn{5}{|c|}{2H SR} \\\\ \n";
  yields<<"\\hline\n";
  yields<<"$[300,500]$ GeV & "<<  histo_A->GetBinContent(1)<<"& \\pm "<< histo_A->GetBinError(1)<< "& " << h_2H_Norm->GetBinContent(1)<<" & \\pm "<<h_2H_Norm->GetBinError(1)<<" \\\\ \n";
  yields<<"$[500,700]$ GeV & "<<  histo_A->GetBinContent(2)<<"& \\pm "<< histo_A->GetBinError(2)<< "& " << h_2H_Norm->GetBinContent(2)<<" & \\pm "<<h_2H_Norm->GetBinError(2)<<" \\\\ \n";
  yields<<"$[700+]$ GeV & "<<  histo_A->GetBinContent(3)<<"& \\pm "<< histo_A->GetBinError(3)<< "& " << h_2H_Norm->GetBinContent(3)<<" & \\pm "<<h_2H_Norm->GetBinError(3)<<" \\\\ \n";
  yields<<"\\hline\n";
  yields<<"\\multicolumn{5}{|c|}{1H SR} \\\\ \n";
  yields<<"\\hline\n";
  yields<<"$[300,500]$ GeV & "<<  histo_A1->GetBinContent(1)<<"& \\pm "<< histo_A1->GetBinError(1)<< "& " << h_1H_Norm->GetBinContent(1)<<" & \\pm "<<h_1H_Norm->GetBinError(1)<<" \\\\ \n";
  yields<<"$[500,700]$ GeV & "<<  histo_A1->GetBinContent(2)<<"& \\pm "<< histo_A1->GetBinError(2)<< "& " << h_1H_Norm->GetBinContent(2)<<" & \\pm "<<h_1H_Norm->GetBinError(2)<<" \\\\ \n";
  yields<<"$[700+]$ GeV & "<<  histo_A1->GetBinContent(3)<<"& \\pm "<< histo_A1->GetBinError(3)<< "& " << h_1H_Norm->GetBinContent(3)<<" & \\pm "<<h_1H_Norm->GetBinError(3)<<" \\\\ \n";
  yields<<"\\hline\n";
  yields<<"\\multicolumn{5}{|c|}{0H, $B_{T}>$0} \\\\ \n";
  yields<<"\\hline\n";
  yields<<"$[300,500]$ GeV & "<<  histo_0HWithCuts->GetBinContent(1)<<" & \\pm "<< histo_0HWithCuts->GetBinError(1)<< "& " << h_0H_Norm->GetBinContent(1)<<" & \\pm "<<h_0H_Norm->GetBinError(1)<<"\\\\ \n";
  yields<<"$[500,700]$ GeV & "<<  histo_0HWithCuts->GetBinContent(2)<<" & \\pm "<< histo_0HWithCuts->GetBinError(2)<< "& " << h_0H_Norm->GetBinContent(2)<<" & \\pm "<<h_0H_Norm->GetBinError(2)<<" \\\\ \n";
  yields<<"$[700+]$ GeV & "<<  histo_0HWithCuts->GetBinContent(3)<<" & \\pm "<< histo_0HWithCuts->GetBinError(3)<< "& " << h_0H_Norm->GetBinContent(3)<<" & \\pm "<<h_0H_Norm->GetBinError(3)<<" \\\\ \n";
  yields<<"\\hline\n";
  yields<<"\\end{tabular}\n"<<"\\npnoround\n"<<"\\end{table}\n"<<"\\end{document}\n";
  yields.close();
}


void pieChart(vector<TH1F*> h_QCD, vector<TH1F*> h_WJets, vector<TH1F*> h_ZJets, vector<TH1F*> h_TTJets,vector<TH1F*> h_SnglT, TString regionLabel, TString bin){
  TH1F * h_Q = (TH1F*)h_QCD[0]->Clone("h_Q"); for (int i=1;i<h_QCD.size();i++) h_Q->Add(h_QCD[i]);
  TH1F * h_W = (TH1F*)h_WJets[0]->Clone("h_W"); for (int i=1;i<h_WJets.size();i++) h_W->Add(h_WJets[i]);
  TH1F * h_Z = (TH1F*)h_ZJets[0]->Clone("h_Z"); for (int i=1;i<h_ZJets.size();i++) h_Z->Add(h_ZJets[i]);
  TH1F * h_TT = (TH1F*)h_TTJets[0]->Clone("h_TT");  for (int i=1;i<h_TTJets.size();i++) h_TT->Add(h_TTJets[i]);
  TH1F * h_T = (TH1F*)h_SnglT[0]->Clone("h_T");  for (int i=1;i<h_SnglT.size();i++) h_T->Add(h_SnglT[i]);

  DrawOverflow(h_Q); DrawOverflow(h_W); DrawOverflow(h_Z); DrawOverflow(h_TT); DrawOverflow(h_T);

  double Q_yields;double W_yields;double Z_yields;double TT_yields;double T_yields;

  if (bin=="all"){
    Q_yields = h_Q->Integral();
    W_yields = h_W->Integral();
    Z_yields = h_Z->Integral();
    TT_yields = h_TT->Integral();
    T_yields = h_T->Integral();
  }

  if (bin=="bin1"){
    Q_yields = h_Q->GetBinContent(1);
    W_yields = h_W->GetBinContent(1);
    Z_yields = h_Z->GetBinContent(1);
    TT_yields = h_TT->GetBinContent(1);
    T_yields = h_T->GetBinContent(1);
  }

  if (bin=="bin2"){
    Q_yields = h_Q->GetBinContent(2);
    W_yields = h_W->GetBinContent(2);
    Z_yields = h_Z->GetBinContent(2);
    TT_yields = h_TT->GetBinContent(2);
    T_yields = h_T->GetBinContent(2);
  }

  if (bin=="bin3"){
    Q_yields = h_Q->GetBinContent(3)+h_Q->GetBinContent(4);
    W_yields = h_W->GetBinContent(3)+h_W->GetBinContent(4);
    Z_yields = h_Z->GetBinContent(3)+h_Z->GetBinContent(4);
    TT_yields = h_TT->GetBinContent(3)+h_TT->GetBinContent(4);
    T_yields = h_T->GetBinContent(3)+h_T->GetBinContent(4);
  }

  TString yieldsFileName;
  yieldsFileName = whichRegion+", "+regionLabel;
  TCanvas * can_h = new TCanvas(yieldsFileName,yieldsFileName, 50, 50, 1200, 1200);
  styleCanvas(can_h);

  double vals[] = {Q_yields,W_yields,Z_yields,TT_yields,T_yields};

  Int_t col_zjets = TColor::GetColorTransparent(kOrange+1, 0.90);
  Int_t col_wjets = TColor::GetColorTransparent(ci3, 0.90);
  Int_t col_qcd = TColor::GetColorTransparent(ci2, 0.95);
  Int_t col_tt = TColor::GetColorTransparent(ci, 0.95);
  Int_t col_snglt = TColor::GetColorTransparent(kGray+2, 0.95);

  int colors[] = {col_qcd,col_wjets,col_zjets,col_tt,col_snglt};
  const char *labels[] = {"QCD","W+jets","Z+jets","t#bar{t}+X","Other"};
  int nvals = sizeof(vals)/sizeof(vals[0]);
  TString binDef;
  if (bin=="all") binDef = "allMET";
  else if (bin=="bin1")binDef = "MET300to500";
  else if (bin=="bin2")binDef = "MET500to700";
  else if (bin=="bin3")binDef = "MET700up";
  TString pieName = yieldsFileName + ": " + binDef;
  TPie *pie1 = new TPie("pie1", "",nvals,vals,colors, labels);
  pie1->SetAngularOffset(145.);
  // pie1->SetTitle("Over all MET");
  // pie1->SetAngle3D(45.);
  // pie1->SetHeight(0.04);
  pie1->SetCircle(0.5,0.5,0.3);
  pie1->SetLabelsOffset(.03);
  pie1->SetLabelFormat("%perc");
  pie1->SetTextFont(42);
  pie1->Draw("2d");

  TString savename = whichRegion+"_"+regionLabel+"_"+binDef;
  cdPie->cd();
  can_h->Write(savename);
  if (savePDFs && whichRegion=="signal") can_h->SaveAs("boostedFigures/"+whichRegion+"/pies/pie_"+regionLabel+"_"+binDef+".pdf","PDF");
  // can_h->SaveAs("pie_"+regionLabel+"_"+binDef+".pdf","PDF");
  if (regionLabel=="2H" && bin=="all"){
    TCanvas * can2 = new TCanvas("PieChartLegend","PieChartLegend", 50, 50, 1200, 1200);
    TLegend *leg = pie1->MakeLegend();
    leg->SetBorderSize(0);
    leg->Draw();
    can2->Write("Legend");
    TString legSavename = "boostedFigures/"+whichRegion+"/pies/pieLegend.pdf";
    // can2->SaveAs(legSavename,"PDF");
    // can2->SaveAs("pieLegend.pdf","PDF");
    delete can2;
  }

  delete pie1;
  delete can_h;
}

void pieChart1l(vector<TH1F*> h_WJets, vector<TH1F*> h_TTJets, vector<TH1F*> h_SnglT, TString regionLabel, TString bin){
  TH1F * h_W = (TH1F*)h_WJets[0]->Clone("h_W"); for (int i=1;i<h_WJets.size();i++) h_W->Add(h_WJets[i]);
  TH1F * h_TT = (TH1F*)h_TTJets[0]->Clone("h_TT");  for (int i=1;i<h_TTJets.size();i++) h_TT->Add(h_TTJets[i]);
  TH1F * h_T = (TH1F*)h_SnglT[0]->Clone("h_T");  for (int i=1;i<h_SnglT.size();i++) h_T->Add(h_SnglT[i]);

  DrawOverflow(h_W); DrawOverflow(h_TT); DrawOverflow(h_T);
  double W_yields;double TT_yields;double T_yields;

  if (bin=="all"){
    W_yields = h_W->Integral();
    TT_yields = h_TT->Integral();
    T_yields = h_T->Integral();
  }

  if (bin=="bin1"){
    W_yields = h_W->GetBinContent(1);
    TT_yields = h_TT->GetBinContent(1);
    T_yields = h_T->GetBinContent(1);
  }

  if (bin=="bin2"){
    W_yields = h_W->GetBinContent(2);
    TT_yields = h_TT->GetBinContent(2);
    T_yields = h_T->GetBinContent(2);
  }

  if (bin=="bin3"){
    W_yields = h_W->GetBinContent(3)+h_W->GetBinContent(4);
    TT_yields = h_TT->GetBinContent(3)+h_TT->GetBinContent(4);
    T_yields = h_T->GetBinContent(3)+h_T->GetBinContent(4);
  }

  TString yieldsFileName;
  yieldsFileName = whichRegion+", "+regionLabel;
  TCanvas * can_h = new TCanvas(yieldsFileName,yieldsFileName, 50, 50, 1200, 1200);
  styleCanvas(can_h);

  double vals[] = {W_yields,TT_yields,T_yields};
  // int colors[] = {kAzure+1,kGreen+3,kSpring-5};
  int colors[] = {kBlue,kCyan,kOrange};

  const char *labels[] = {"WJets","TT","SnglT"};
  int nvals = sizeof(vals)/sizeof(vals[0]);
  TString binDef;
  if (bin=="all") binDef = "allMET";
  else if (bin=="bin1")binDef = "MET300to500";
  else if (bin=="bin2")binDef = "MET500to700";
  else if (bin=="bin3")binDef = "MET700up";
  TString pieName = yieldsFileName + ": " + binDef;
  TPie *pie1 = new TPie("pie1", "",nvals,vals,colors, labels);
  // pie1->SetTitle("MET [700+] GeV");
  pie1->SetAngularOffset(145.);
  // pie1->SetAngle3D(45.);
  // pie1->SetHeight(0.04);
  pie1->SetCircle(0.5,0.5,0.3);
  pie1->SetLabelsOffset(.03);
  pie1->SetLabelFormat("%perc");
  pie1->Draw("2d");

  TString savename = whichRegion+"_"+regionLabel+"_"+binDef;
  cdPie->cd();
  can_h->Write(savename);
  if (savePDFs && whichRegion=="singleLept") can_h->SaveAs("boostedFigures/"+whichRegion+"/pies/pie_"+regionLabel+"_"+binDef+".pdf","PDF");

  if (regionLabel=="2H" && bin=="all"){
    TLegend *leg = pie1->MakeLegend();
    TCanvas * can2 = new TCanvas("PieChartLegend","PieChartLegend", 50, 50, 1200, 1200);
    leg->SetBorderSize(0);
    leg->Draw();
    can2->Write("Legend");
    TString legSavename = "boostedFigures/"+whichRegion+"/pies/pieLegend.pdf";
    can2->SaveAs(legSavename,"PDF");
    delete can2;
  }
  delete pie1;delete can_h;
}

void pieChartPhoton(vector<TH1F*> h_GJets, vector<TH1F*> h_QCD,  TString regionLabel, TString bin){
  TH1F * h_G = (TH1F*)h_GJets[0]->Clone("h_G"); for (int i=1;i<h_GJets.size();i++) h_G->Add(h_GJets[i]);
  TH1F * h_Q = (TH1F*)h_QCD[0]->Clone("h_Q");  for (int i=1;i<h_QCD.size();i++) h_Q->Add(h_QCD[i]);

  DrawOverflow(h_G); DrawOverflow(h_Q);
  double G_yields;double Q_yields;

  if (bin=="all"){
    G_yields = h_G->Integral();
    Q_yields = h_Q->Integral();
  }

  if (bin=="bin1"){
    G_yields = h_G->GetBinContent(1);
    Q_yields = h_Q->GetBinContent(1);
  }

  if (bin=="bin2"){
    G_yields = h_G->GetBinContent(2);
    Q_yields = h_Q->GetBinContent(2);
  }

  if (bin=="bin3"){
    G_yields = h_G->GetBinContent(3)+h_G->GetBinContent(4);
    Q_yields = h_Q->GetBinContent(3)+h_Q->GetBinContent(4);
  }

  TString yieldsFileName;
  yieldsFileName = whichRegion+", "+regionLabel;
  TCanvas * can_h = new TCanvas(yieldsFileName,yieldsFileName, 50, 50, 1200, 1200);
  styleCanvas(can_h);

  double vals[] = {G_yields,Q_yields};
  // int colors[] = {kAzure+1,kGreen+3};
  int colors[] = {kGreen,kGray};

  const char *labels[] = {"GJets","QCD"};
  int nvals = sizeof(vals)/sizeof(vals[0]);
  TString binDef;
  if (bin=="all") binDef = "allMET";
  else if (bin=="bin1")binDef = "MET300to500";
  else if (bin=="bin2")binDef = "MET500to700";
  else if (bin=="bin3")binDef = "MET700up";
  TString pieName = yieldsFileName + ": " + binDef;
  if (G_yields+Q_yields>0){
    TPie *pie1 = new TPie("pie1", "",nvals,vals,colors, labels);
    // pie1->SetTitle("MET [700+] GeV");
    pie1->SetAngularOffset(145.);
    // pie1->SetAngle3D(45.);
    // pie1->SetHeight(0.04);
    pie1->SetCircle(0.5,0.5,0.3);
    pie1->SetLabelsOffset(.03);
    pie1->SetLabelFormat("%perc");
    pie1->Draw("2d");

    TString savename = whichRegion+"_"+regionLabel+"_"+binDef;
    cdPie->cd();
    can_h->Write(savename);
    if (savePDFs && whichRegion=="photon") can_h->SaveAs("boostedFigures/"+whichRegion+"/pies/pie_"+regionLabel+"_"+binDef+".pdf","PDF");

    if (regionLabel=="2H" && bin=="all"){
      TLegend *leg = pie1->MakeLegend();
      TCanvas * can2 = new TCanvas("PieChartLegend","PieChartLegend", 50, 50, 1200, 1200);
      leg->SetBorderSize(0);
      leg->Draw();
      can2->Write("Legend");
      TString legSavename = "boostedFigures/"+whichRegion+"/pies/pieLegend.pdf";
      can2->SaveAs(legSavename,"PDF");
      delete can2;
    }
    delete pie1;delete can_h;
  }
}

TH1F* make0EventUncSum(vector<TH1F*> dem_histos) {
  //Histograms should be in this order: QCD, TT, WJets,ZJets, SnglT
  // TH1F * h_sum   = (TH1F*)dem_histos[0]->Clone("h_sum");
  TH1F * h_QCD   = (TH1F*)dem_histos[0]->Clone("h_QCD");
  TH1F * h_TT    = (TH1F*)dem_histos[1]->Clone("h_TT");
  TH1F * h_WJets = (TH1F*)dem_histos[2]->Clone("h_WJets");
  TH1F * h_ZJets = (TH1F*)dem_histos[3]->Clone("h_ZJets");
  TH1F * h_SnglT = (TH1F*)dem_histos[4]->Clone("h_SnglT");

  TH1F *h_sum = (TH1F*)h_QCD->Clone("h_sum");
  h_sum->Add(h_TT);
  h_sum->Add(h_WJets);
  h_sum->Add(h_ZJets);
  h_sum->Add(h_SnglT);

  DrawOverflow(h_sum);

  return h_sum;
}

TH1F* make0EventUncSum_photon(vector<TH1F*> dem_histos) {
  //Histograms should be in this order: QCD, GJets
  TH1F * h_QCD   = (TH1F*)dem_histos[0]->Clone("h_QCD");
  TH1F * h_GJets = (TH1F*)dem_histos[1]->Clone("h_GJets");
  TH1F *h_sum = (TH1F*)h_QCD->Clone("h_sum");
  h_sum->Add(h_GJets);
  DrawOverflow(h_sum);
  return h_sum;
}

TH1F *make0EventUncSum_1l(vector<TH1F*> dem_histos) {
  //Histograms should be in this order: TT, WJets, SnglT
  TH1F * h_TT    = (TH1F*)dem_histos[0]->Clone("h_TT");
  TH1F * h_WJets = (TH1F*)dem_histos[1]->Clone("h_WJets");
  TH1F * h_SnglT = (TH1F*)dem_histos[2]->Clone("h_SnglT");

  TH1F *h_sum = (TH1F*)h_TT->Clone("h_sum");
  h_sum->Add(h_WJets);
  h_sum->Add(h_SnglT);
  DrawOverflow(h_sum);
  return h_sum;
}

void styleCanvas(TCanvas * can_h){
  can_h->SetFillColor(0); can_h->SetBorderMode(0);
  can_h->SetFrameFillStyle(0); can_h->SetFrameBorderMode(0);
  can_h->SetLeftMargin( L/W ); can_h->SetRightMargin( R/W );
  can_h->SetTopMargin( T/H ); can_h->SetBottomMargin( B/H );
  can_h->SetTickx(0); can_h->SetTicky(0);
}

void addLumiCanv(TCanvas * &can_h){
  TString cmsText = "CMS"; float cmsTextFont = 61; float cmsTextSize = 0.75;
  bool writeExtraText = true;
  TString extraText   = "Preliminary"; float extraTextFont = 52;
  TString lumiText = "137 fb^{-1} (13 TeV)"; float lumiTextSize = 0.6; float lumiTextOffset = 0.2;
  float relPosX = 0.045; float relPosY = 0.035; float relExtraDY = 1.2; float relExtraDX = 1.5;

  // ratio of "CMS" and extra text size
  float extraOverCmsTextSize  = 0.76;
  bool outOfFrame = false;

  int iPosX=10; int alignY_=3; int alignX_=2;
  if (iPosX/10==0) alignX_=1;
  if (iPosX==0) {alignX_=1; alignY_=1;}
  if (iPosX/10==1) alignX_=1;
  if (iPosX/10==2) alignX_=2;
  if (iPosX/10==3) alignX_=3;
  int align_ = 10*alignX_ + alignY_;
  float H = can_h->GetWh(); float W = can_h->GetWw();
  float l = can_h->GetLeftMargin(); float t = can_h->GetTopMargin();
  float r = can_h->GetRightMargin(); float b = can_h->GetBottomMargin();

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;
  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(0.04);
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  float posX_=0;
  if ( iPosX%10<=1 ) posX_ =   l + relPosX*(1-l-r);
  else if( iPosX%10==2 ) posX_ =  l + 0.5*(1-l-r);
  else if( iPosX%10==3 ) posX_ =  1-r - relPosX*(1-l-r);
  float posY_ = 1-t - relPosY*(1-t-b);
  if (!outOfFrame) {
    latex.SetTextFont(cmsTextFont);
    latex.SetTextSize(cmsTextSize*t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_, posY_, cmsText);
    if ( writeExtraText ) {
      latex.SetTextFont(extraTextFont);
      latex.SetTextAlign(align_);
      latex.SetTextSize(extraTextSize*t);
      latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
    }
  }
  else if ( writeExtraText ) {
    if (iPosX==0) {
      posX_ = l +  relPosX*(1-l-r);
      posY_ = 1-t+lumiTextOffset*t;
    }
    latex.SetTextFont(extraTextFont);
    latex.SetTextSize(extraTextSize*t);
    latex.SetTextAlign(align_);
    latex.DrawLatex(posX_+relExtraDX*cmsTextSize*t, posY_, extraText);
  }
}

void styleGraph(TGraph *graph){
  graph->SetMarkerStyle(20);
  graph->SetLineColor(kBlack); graph->SetMarkerColor(kBlack);
  graph->GetYaxis()->SetTitleOffset(0.25);
  graph->GetYaxis()->SetTitleSize(0.13);
  graph->GetXaxis()->SetTitleOffset(0.9);
  graph->GetXaxis()->SetTitleSize(0.14);
  graph->GetXaxis()->SetLabelSize(0.14);
  graph->GetYaxis()->SetLabelSize(0.14);
  graph->GetXaxis()->SetNdivisions(507);
  graph->GetYaxis()->SetNdivisions(505);
}

void DrawOverflow(TH1F* &h){
  int lastBin = h->GetNbinsX();
  double lastBinError = sqrt(h->GetBinError(lastBin)*h->GetBinError(lastBin) + h->GetBinError(lastBin+1)*h->GetBinError(lastBin+1));
  double lastBinContent = h->GetBinContent(lastBin)+h->GetBinContent(lastBin+1);
  h->SetBinContent(lastBin, lastBinContent);
  h->SetBinError(lastBin, lastBinError);
  h->SetBinContent(lastBin+1, 0.0);
  h->SetBinError(lastBin+1, 0.0);
}

void NormalizeHisto(TH1F* &h){
  h->Scale(1/h->Integral());
}

void massCorrelations(vector<TH1F*> dem_histos,vector<TH1F*> dem_histos_data, TString bkgType = ""){
  bool runDataHere = false;
  gStyle->SetTextFont(52);
  TH1F *histo_A = (TH1F*)dem_histos[0]->Clone("histo_A"); TH1F *histo_A1 = (TH1F*)dem_histos[1]->Clone("histo_A1");
  TH1F *histo_B = (TH1F*)dem_histos[2]->Clone("histo_B"); TH1F *histo_B1 = (TH1F*)dem_histos[3]->Clone("histo_B1");
  TH1F *histo_C = (TH1F*)dem_histos[4]->Clone("histo_C"); TH1F *histo_D = (TH1F*)dem_histos[5]->Clone("histo_D");

  TH1F *histo_A_data = (TH1F*)dem_histos_data[0]->Clone("histo_A_data"); TH1F *histo_A1_data = (TH1F*)dem_histos_data[1]->Clone("histo_A1_data");
  TH1F *histo_B_data = (TH1F*)dem_histos_data[2]->Clone("histo_B_data"); TH1F *histo_B1_data = (TH1F*)dem_histos_data[3]->Clone("histo_B1_data");
  TH1F *histo_C_data = (TH1F*)dem_histos_data[4]->Clone("histo_C_data"); TH1F *histo_D_data = (TH1F*)dem_histos_data[5]->Clone("histo_D_data");

  DrawOverflow(histo_A); DrawOverflow(histo_A1); DrawOverflow(histo_B); DrawOverflow(histo_B1); DrawOverflow(histo_C); DrawOverflow(histo_D);
  DrawOverflow(histo_A_data ); DrawOverflow(histo_A1_data ); DrawOverflow(histo_B_data ); DrawOverflow(histo_B1_data ); DrawOverflow(histo_C_data ); DrawOverflow(histo_D_data );

  //calculate ratios (SIG/SB) and errors
  double a_int; double a_error; a_int = histo_A->IntegralAndError(0,5,a_error,"");
  double a1_int; double a1_error; a1_int = histo_A1->IntegralAndError(0,5,a1_error,"");
  double b_int; double b_error; b_int = histo_B->IntegralAndError(0,5,b_error,"");
  double b1_int; double b1_error; b1_int = histo_B1->IntegralAndError(0,5,b1_error,"");
  double c_int; double c_error; c_int = histo_C->IntegralAndError(0,5,c_error,"");
  double d_int; double d_error; d_int = histo_D->IntegralAndError(0,5,d_error,"");

  double a_int_data; double a_error_data; a_int_data = histo_A_data->IntegralAndError(0,5,a_error_data,"");
  double a1_int_data; double a1_error_data; a1_int_data = histo_A1_data->IntegralAndError(0,5,a1_error_data,"");
  double b_int_data; double b_error_data; b_int_data = histo_B_data->IntegralAndError(0,5,b_error_data,"");
  double b1_int_data; double b1_error_data; b1_int_data = histo_B1_data->IntegralAndError(0,5,b1_error_data,"");
  double c_int_data; double c_error_data; c_int_data = histo_C_data->IntegralAndError(0,5,c_error_data,"");
  double d_int_data; double d_error_data; d_int_data = histo_D_data->IntegralAndError(0,5,d_error_data,"");


  float ratio_0H = c_int / d_int;
  float err_0H = ratio_0H * sqrt((c_error/c_int)*(c_error/c_int) + (d_error/d_int)*(d_error/d_int));
  float ratio_1H = a1_int / b1_int;
  float err_1H = ratio_1H * sqrt((a1_error/a1_int)*(a1_error/a1_int) + (b1_error/b1_int)*(b1_error/b1_int));
  float ratio_2H = a_int / b_int;
  float err_2H = ratio_2H * sqrt((a_error/a_int)*(a_error/a_int) + (b_error/b_int)*(b_error/b_int));

  float ratio_0H_data = c_int_data / d_int_data;
  float err_0H_data = ratio_0H_data * sqrt((c_error_data/c_int_data)*(c_error_data/c_int_data) + (d_error_data/d_int_data)*(d_error_data/d_int_data));
  float ratio_1H_data = a1_int_data / b1_int_data;
  float err_1H_data = ratio_1H_data * sqrt((a1_error_data/a1_int_data)*(a1_error_data/a1_int_data) + (b1_error_data/b1_int_data)*(b1_error_data/b1_int_data));
  float ratio_2H_data = a_int_data / b_int_data;
  float err_2H_data = ratio_2H_data * sqrt((a_error_data/a_int_data)*(a_error_data/a_int_data) + (b_error_data/b_int_data)*(b_error_data/b_int_data));
  // std::cout<<"2H MC: "<<ratio_2H<<"+/-"<<err_2H<<std::endl;
  // std::cout<<"2H data: "<<ratio_2H_data<<"+/-"<<err_2H_data<<std::endl;

  //adding data statistics error bars
  // float err_0H_data = ratio_0H * sqrt((sqrt(c_int)/c_int)*(sqrt(c_int)/c_int) + (sqrt(d_int)/d_int)*(sqrt(d_int)/d_int));
  // float err_1H_data = ratio_1H * sqrt((sqrt(a1_int)/a1_int)*(sqrt(a1_int)/a1_int) + (sqrt(b1_int)/b1_int)*(sqrt(b1_int)/b1_int));
  // float err_2H_data = ratio_2H * sqrt((sqrt(a_int)/a_int)*(sqrt(a_int)/a_int) + (sqrt(b_int)/b_int)*(sqrt(b_int)/b_int));

  TString name = "massCorr"+bkgType;
  TString title = ";;N_{(C)SR} / N_{(C)SB}";
  TH1F * massCorrPlot = new TH1F(name, title,3,0,3);
  TH1F * massCorrPlot_data = new TH1F(name+"data", title,3,0,3);

  massCorrPlot->SetBinContent(1,ratio_0H); massCorrPlot->SetBinContent(2,ratio_1H); massCorrPlot->SetBinContent(3,ratio_2H);
  massCorrPlot->SetBinError(1,err_0H); massCorrPlot->SetBinError(2,err_1H); massCorrPlot->SetBinError(3,err_2H);

  massCorrPlot_data->SetBinContent(1,ratio_0H_data); massCorrPlot_data->SetBinContent(2,ratio_1H_data); massCorrPlot_data->SetBinContent(3,ratio_2H_data);
  massCorrPlot_data->SetBinError(1,err_0H_data); massCorrPlot_data->SetBinError(2,err_1H_data); massCorrPlot_data->SetBinError(3,err_2H_data);


  const int n = 3;
  double x[n]   = {0.5,1.5,2.5}; double y[n]   = {0.0,0.0,0.0};
  double exl[n] = {0.5,0.5,0.5}; double exh[n] = {0.5,0.5,0.5};
  double eyl[n] = {0.0,0.0,0.0}; double eyh[n] = {0.0,0.0,0.0};
  y[0] = ratio_0H; y[1] = ratio_1H; y[2] = ratio_2H;
  eyl[0] = err_0H_data; eyl[1] = err_1H_data; eyl[2] = err_2H_data;
  eyh[0] = err_0H_data; eyh[1] = err_1H_data; eyh[2] = err_2H_data;


  TGraphAsymmErrors * graph_errorBands = new TGraphAsymmErrors(n,x,y,exl,exh,eyl,eyh);

  // graph_errorBands->SetFillColor(kBlue); graph_errorBands->SetFillStyle(3445);
  // graph_errorBands->SetMarkerSize(0); graph_errorBands->SetLineWidth(1);
  // graph_errorBands->SetLineColor(kBlue);

  // massCorrPlot->GetXaxis()->SetBinLabel(1,"0H");
  // massCorrPlot->GetXaxis()->SetBinLabel(2,"1H");
  // massCorrPlot->GetXaxis()->SetBinLabel(3,"2H");
  massCorrPlot->GetXaxis()->SetBinLabel(1,"0");
  massCorrPlot->GetXaxis()->SetBinLabel(2,"1");
  massCorrPlot->GetXaxis()->SetBinLabel(3,"2");
  massCorrPlot->GetYaxis()->SetTitleOffset(1.35);

  massCorrPlot->GetYaxis()->SetTitleSize(0.052);
  massCorrPlot->GetYaxis()->SetLabelSize(0.041);
  // massCorrPlot->GetXaxis()->SetLabelSize(0.07);
  // massCorrPlot->GetXaxis()->SetTitleSize(0.05);
  massCorrPlot->GetXaxis()->SetLabelSize(0.07);
  massCorrPlot->GetXaxis()->SetTitleSize(0.055);
  massCorrPlot->GetXaxis()->SetTitleOffset(1.0);

  massCorrPlot->SetStats(0);
  massCorrPlot->GetYaxis()->SetRangeUser(0.05, 0.22);


  // graph2H->GetYaxis()->SetTitleSize(0.05); graph2H->GetXaxis()->SetTitleSize(0.05);
  // graph2H->GetYaxis()->SetLabelSize(0.04); graph2H->GetXaxis()->SetLabelSize(0.04);

  // TF1*f0=new TF1("f0","pol0");
  // massCorrPlot->Fit("f0","Q");
  // double p0 = f0->GetParameter(0); double error = f0->GetParError(0);
  // std::string lineConst = to_string(p0); std::string lineErr = to_string(error);
  // TString constString = "p0 = "+lineConst+" #pm "+ lineErr;
  // TLatex *t = new TLatex(0.2,0.35,constString);
  // t->SetNDC(); //t->SetTextFont(52);
  // t->SetTextSize(0.05);

  TLegend* legend = new TLegend(0.60,0.75,0.93,0.91);
  legend->SetBorderSize(0);
  legend->AddEntry(massCorrPlot,"MC","lp");
  legend->AddEntry(massCorrPlot_data,"Data","lp");
  gStyle->SetTextFont(52);

  TString massCorrFileName;
  massCorrFileName = "massCorr, "+bkgType;
  TCanvas * can_h = new TCanvas(massCorrFileName,massCorrFileName, 50, 50, 1200, 1200);
  styleCanvas(can_h);
  gStyle->SetTextFont(52);

  can_h->SetMargin(0.145, 0.055, 0.12, 0.055);
  massCorrPlot->SetMarkerStyle(20); massCorrPlot->SetMarkerSize(2.5); massCorrPlot->SetMarkerColor(kBlack); massCorrPlot->SetLineColor(kBlack); massCorrPlot->SetLineWidth(3);
  massCorrPlot_data->SetMarkerStyle(20); massCorrPlot_data->SetMarkerSize(2.5); massCorrPlot_data->SetMarkerColor(kRed); massCorrPlot_data->SetLineColor(kRed); massCorrPlot_data->SetLineWidth(3);
  massCorrPlot->GetXaxis()->SetTitle("N_{H}");
  massCorrPlot->GetYaxis()->SetRangeUser(0.1,0.2);
  if (runDataHere && whichRegion=="signal") massCorrPlot->GetYaxis()->SetRangeUser(0.0,0.34);
  if (runDataHere && whichRegion=="singleLept") massCorrPlot->GetYaxis()->SetRangeUser(0.051,0.16);
  if (runDataHere && whichRegion=="photon") massCorrPlot->GetYaxis()->SetRangeUser(0.0,0.9);
  massCorrPlot->Draw();
  // graph_errorBands->Draw("E2");
  if (runDataHere) massCorrPlot_data->Draw("same");
  // t->Draw("same");
  if (runDataHere) legend->Draw("same");
  gStyle->SetTextFont(52);

  TLatex ltitle(0.17, 1.-0.5*can_h->GetTopMargin()-0.08, "#font[62]{CMS}");
  TLatex ltitle_sim(0.17, 1.-0.5*can_h->GetTopMargin()-0.08, "#font[62]{CMS}#scale[0.76]{#font[52]{ Simulation}}");
  TLatex ltitle_prelim(0.17, 1.-0.5*can_h->GetTopMargin()-0.14, "#scale[0.76]{#font[52]{ Preliminary}}");
  TLatex rtitle(0.94, 1.-0.5*can_h->GetTopMargin()+0.003, "#scale[0.98]{137 fb^{-1} (13 TeV)}");
  rtitle.SetTextFont(42);
  ltitle.SetNDC(); rtitle.SetNDC(); ltitle_sim.SetNDC(); ltitle_prelim.SetNDC();
  ltitle.SetTextAlign(12); ltitle_sim.SetTextAlign(12); ltitle_prelim.SetTextAlign(12); rtitle.SetTextAlign(32);
  ltitle.Draw("same"); rtitle.Draw("same"); ltitle_sim.Draw("same"); //ltitle_prelim.Draw("same");


  cdOther->cd();
  can_h->Write("massCorr");
  TString savename = "boostedFigures/"+whichRegion+"/bkgest/SIGSBRatio.pdf";
  if (savePDFs) can_h->SaveAs(savename,"PDF");
  // can_h->SaveAs("nH_MConly_prelim.pdf","PDF");
  can_h->SaveAs("nH_MConly.pdf","PDF");
  // can_h->SaveAs("nH_MCandData.pdf","PDF");
  // can_h->SaveAs("SIGSBRatio.pdf","PDF");
}

void makeMCvDataComp(TH1F* h_MC_orig, TH1F* h_data_orig, TString region, TString type) {
  TH1F* h_MC = (TH1F*)h_MC_orig->Clone("h_MC"); TH1F* h_data = (TH1F*)h_data_orig->Clone("h_data");
  if (type.Contains("j1") || type.Contains("j2")) {
    h_MC->Rebin(2); h_data->Rebin(2);
  }
  h_MC->SetStats(0);
  h_MC->SetFillColor(kRed); h_MC->SetFillStyle(3445);
  h_MC->SetMarkerSize(0); h_MC->SetLineWidth(1);
  h_MC->SetLineColor(kRed);

  h_data->SetStats(0); h_data->SetLineColor(kBlack);
  h_data->SetMarkerSize(0.7); h_data->SetMarkerColor(kBlack); h_data->SetMarkerStyle(20);

  h_MC->SetTitle(";p_{T}^{miss} [GeV];Events (137 fb^{-1})");
  // if (type!="MET") h_MC->GetXaxis()->SetTitle(type);

  TLegend* legend = new TLegend(0.50,0.5,0.93,0.91);
  legend->SetBorderSize(0);
  legend->SetHeader(region);
  legend->AddEntry(h_MC,"MC","f");
  legend->AddEntry(h_data,"Data","lp");

  TGraphAsymmErrors * graph = new TGraphAsymmErrors(h_data, h_MC, "pois");
  graph->SetLineColor(kBlack); graph->SetMarkerColor(kBlack);
  graph->SetTitle(";p_{T}^{miss} [GeV]; Data/MC");

  TH1F * graph_histo = (TH1F*)h_data->Clone("graph_histo");graph_histo->Divide(h_MC);
  graph_histo->SetLineColor(kBlack); graph_histo->SetMarkerColor(kBlack); graph_histo->SetMarkerStyle(20);
  graph_histo->SetTitle(";p_{T}^{miss} [GeV]; Data/MC");



  TString canvName = region+type;
  TCanvas * can_h = new TCanvas(canvName,canvName, 50, 50, 800, 800);
  styleCanvas(can_h);


  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.5, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.5);
  pad1->SetTickx(0); pad1->SetTicky(0);
  // pad1->SetPad(0., 1 - up_height, 1., 1.00);
  pad1->SetPad(0., 0.5, 1., 1.00);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTopMargin(0.08); pad1->SetLeftMargin(0.10); pad1->SetRightMargin(0.06);
  pad1->Draw();

  // pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
  pad2->SetPad(0., 0., 1., 0.5);
  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.20); pad2->SetTopMargin(0.01);
  pad2->SetLeftMargin(0.10); pad2->SetRightMargin(0.06);
  pad2->Draw();
  pad1->cd();

  if (h_MC->GetMaximum()>h_data->GetMaximum()){
    h_MC->GetXaxis()->SetLabelSize(0); h_MC->GetXaxis()->SetTitleSize(0);
    h_MC->GetYaxis()->SetLabelSize(0.055); h_MC->GetYaxis()->SetTitleSize(0.07); h_MC->GetYaxis()->SetTitleOffset(0.65);
    h_MC->SetMaximum(h_MC->GetMaximum()*1.2);
    h_MC->Draw("E2"); h_data->Draw("E same");
  }
  else {
    h_data->GetXaxis()->SetLabelSize(0); h_data->GetXaxis()->SetTitleSize(0);
    h_data->GetYaxis()->SetLabelSize(0.055); h_data->GetYaxis()->SetTitleSize(0.07); h_data->GetYaxis()->SetTitleOffset(0.65);
    h_data->SetMaximum(h_data->GetMaximum()*1.2);
    h_data->Draw("E"); h_MC->Draw("E2 same");
  }

  legend->Draw("same");
  pad2->cd();

  graph->Draw("APE");
  //Now change everything about this graph

  if (type=="MET") {
    graph->GetXaxis()->SetTitle("p_{T}^{miss} [GeV]");
    graph->GetXaxis()->SetRangeUser(300.0,1400.0);
  }
  else if (type=="nH") {
    graph_histo->GetXaxis()->SetTitle("Number Higgs tags");
    graph_histo->GetXaxis()->SetBinLabel(1,"0H");graph_histo->GetXaxis()->SetBinLabel(2,"1H");graph_histo->GetXaxis()->SetBinLabel(3,"2H");
  }
  else if (type=="j1m") {
    graph->GetXaxis()->SetTitle("Leading jet softdrop mass [GeV]");
    graph->GetXaxis()->SetRangeUser(60.0,260.0);
  }
  else if (type=="j2m") {
    graph->GetXaxis()->SetTitle("Subleading jet softdrop mass [GeV]");
    graph->GetXaxis()->SetRangeUser(60.0,260.0);
  }
  else if (type=="j1pt") {
    graph->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
    graph->GetXaxis()->SetRangeUser(300.0,1300.0);
  }
  else if (type=="j2pt") {
    graph->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");
    graph->GetXaxis()->SetRangeUser(300.0,1300.0);
  }
  else if (type=="j1bb") {
    graph->GetXaxis()->SetTitle("Leading jet deep bb-tag");
    graph->GetXaxis()->SetRangeUser(0.0,1.0);
  }
  else if (type=="j2bb") {
    graph->GetXaxis()->SetTitle("Subleading jet deep bb-tag");
    graph->GetXaxis()->SetRangeUser(0.0,1.0);
  }
  else graph->GetXaxis()->SetTitle(type);

  if (type=="nH") {
    graph_histo->GetXaxis()->SetLabelSize(0.15); graph_histo->GetXaxis()->SetTitleSize(0.07); graph_histo->GetXaxis()->SetTitleOffset(1.20);
    graph_histo->GetYaxis()->SetLabelSize(0.06); graph_histo->GetYaxis()->SetTitleSize(0.07); graph_histo->GetYaxis()->SetTitleOffset(0.55);
    graph_histo->Draw();
    // graph_histo->Fit("pol0","Q");
  }
  else {
    graph->GetXaxis()->SetLabelSize(0.06); graph->GetXaxis()->SetTitleSize(0.09); graph->GetXaxis()->SetTitleOffset(0.80);
    graph->GetYaxis()->SetLabelSize(0.055); graph->GetYaxis()->SetTitleSize(0.07); graph->GetYaxis()->SetTitleOffset(0.65);
    // graph->Fit("pol0","Q");
    graph->Draw("APE");
  }

  pad1->cd();
  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize/12);
  latex.DrawLatex(0.95,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(cmsTextSize*1.3);
  latex.DrawLatex(0.20, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextAlign(align_);
  latex.SetTextSize(cmsTextSize*0.85); latex.DrawLatex(0.26, 0.88, extraText);

  cdDatavMC->cd();
  can_h->Write(canvName);
  TString savename = "DatavMC_"+region+type;
  if (savePDFs)  can_h->SaveAs("boostedFigures/"+whichRegion+"/dataVsMC/"+savename+".pdf","PDF");
}

void makeMETShapeComp(vector<TH1F*> MC_histos, vector<TH1F*> data_histos) {
  //MC_histos will be [2HSR,1HSR,0H+b_SR, 0H+b_SB]
  //data_histos will be same
  TH1F * h_2HSR = (TH1F*)MC_histos[0]->Clone("h_2HSR"); TH1F * h_1HSR = (TH1F*)MC_histos[1]->Clone("h_1HSR");
  TH1F * h_0HbSR = (TH1F*)MC_histos[2]->Clone("h_0HbSR");

  TH1F * h_2HSR_data = (TH1F*)data_histos[0]->Clone("h_2HSR_data"); TH1F * h_1HSR_data = (TH1F*)data_histos[1]->Clone("h_1HSR_data");
  TH1F * h_0HbSR_data = (TH1F*)data_histos[2]->Clone("h_0HbSR_data");


  DrawOverflow(h_2HSR); DrawOverflow(h_1HSR); DrawOverflow(h_0HbSR);
  DrawOverflow(h_2HSR_data); DrawOverflow(h_1HSR_data); DrawOverflow(h_0HbSR_data);
  NormalizeHisto(h_2HSR); NormalizeHisto(h_1HSR); NormalizeHisto(h_0HbSR);
  NormalizeHisto(h_2HSR_data); NormalizeHisto(h_1HSR_data); NormalizeHisto(h_0HbSR_data);

  h_2HSR->SetMarkerStyle(20); h_2HSR->SetMarkerSize(0.85); h_2HSR->SetLineColor(kGreen+1); h_2HSR->SetMarkerColor(kGreen+1);
  h_1HSR->SetMarkerStyle(20); h_1HSR->SetMarkerSize(0.85); h_1HSR->SetLineColor(kGreen+1); h_1HSR->SetMarkerColor(kGreen+1);
  h_2HSR_data->SetMarkerStyle(20); h_2HSR_data->SetMarkerSize(0.85); h_2HSR_data->SetLineColor(kBlue); h_2HSR_data->SetMarkerColor(kBlue);
  h_1HSR_data->SetMarkerStyle(20); h_1HSR_data->SetMarkerSize(0.85); h_1HSR_data->SetLineColor(kBlue); h_1HSR_data->SetMarkerColor(kBlue);

  h_0HbSR->SetMarkerStyle(20); h_0HbSR->SetMarkerSize(0.85); h_0HbSR->SetLineColor(kBlack); h_0HbSR->SetMarkerColor(kBlack);
  h_0HbSR_data->SetMarkerStyle(20); h_0HbSR_data->SetMarkerSize(0.85); h_0HbSR_data->SetLineColor(kBlack); h_0HbSR_data->SetMarkerColor(kBlack);


  h_2HSR->SetTitle(";p_{T}^{miss} [GeV];Normalized to area"); h_2HSR->GetYaxis()->SetTitleOffset(0.45);
  h_2HSR->GetYaxis()->SetTitleSize(0.09); h_2HSR->GetYaxis()->SetLabelSize(0.08);
  h_2HSR->GetXaxis()->SetTitleSize(0); h_2HSR->SetStats(0);

  h_1HSR->SetTitle(";p_{T}^{miss} [GeV];Normalized to area"); h_1HSR->GetYaxis()->SetTitleOffset(0.45);
  h_1HSR->GetYaxis()->SetTitleSize(0.09);  h_1HSR->GetYaxis()->SetLabelSize(0.08);
  h_1HSR->GetXaxis()->SetTitleSize(0); h_1HSR->SetStats(0);

  h_2HSR_data->SetTitle(";p_{T}^{miss} [GeV];Normalized to area"); h_2HSR_data->GetYaxis()->SetTitleOffset(0.45);
  h_2HSR_data->GetYaxis()->SetTitleSize(0.09); h_2HSR_data->GetYaxis()->SetLabelSize(0.08);
  h_2HSR_data->GetXaxis()->SetTitleSize(0); h_2HSR_data->SetStats(0);

  h_1HSR_data->SetTitle(";p_{T}^{miss} [GeV];Normalized to area"); h_1HSR_data->GetYaxis()->SetTitleOffset(0.45);
  h_1HSR_data->GetYaxis()->SetTitleSize(0.09); h_1HSR_data->GetYaxis()->SetLabelSize(0.08);
  h_1HSR_data->GetXaxis()->SetTitleSize(0); h_1HSR_data->SetStats(0);


  TGraphAsymmErrors * graph_2HMC = new TGraphAsymmErrors(h_2HSR, h_0HbSR, "pois");
  TGraphAsymmErrors * graph_1HMC = new TGraphAsymmErrors(h_1HSR, h_0HbSR, "pois");
  TGraphAsymmErrors * graph_2HData = new TGraphAsymmErrors(h_2HSR_data, h_0HbSR_data, "pois");
  TGraphAsymmErrors * graph_1HData = new TGraphAsymmErrors(h_1HSR_data, h_0HbSR_data, "pois");

  Double_t *y_2HMC = graph_2HMC->GetY(); std::cout<<"2HMC: "; for (int i = 0; i<3; i++) printf("%g, ", y_2HMC[i]); std::cout<<std::endl;
  Double_t *y_2HData = graph_2HData->GetY(); std::cout<<"2HData: ";for (int i = 0; i<3; i++) printf("%g, ", y_2HData[i]); std::cout<<std::endl;
  Double_t *y_1HMC = graph_1HMC->GetY(); std::cout<<"1HMC: "; for (int i = 0; i<3; i++) printf("%g, ", y_1HMC[i]); std::cout<<std::endl;
  Double_t *y_1HData = graph_1HData->GetY(); std::cout<<"1HData: "; for (int i = 0; i<3; i++) printf("%g, ", y_1HData[i]); std::cout<<std::endl;




  styleGraph(graph_2HMC); styleGraph(graph_1HMC); styleGraph(graph_2HData); styleGraph(graph_1HData);
  graph_2HMC->SetMinimum(-0.1); graph_1HMC->SetMinimum(-0.1); graph_2HData->SetMinimum(-0.1); graph_1HData->SetMinimum(-0.1);
  graph_2HMC->GetXaxis()->SetRangeUser(300,1400); graph_1HMC->GetXaxis()->SetRangeUser(300,1400);
  graph_2HData->GetXaxis()->SetRangeUser(300,1400); graph_1HData->GetXaxis()->SetRangeUser(300,1400);


  graph_2HMC->SetLineColor(kGreen+1); graph_2HMC->SetMarkerColor(kGreen+1); graph_1HMC->SetLineColor(kGreen+1); graph_1HMC->SetMarkerColor(kGreen+1);
  graph_2HData->SetLineColor(kBlue);  graph_2HData->SetMarkerColor(kBlue); graph_1HData->SetLineColor(kBlue); graph_1HData->SetMarkerColor(kBlue);

  graph_2HMC->SetTitle(";p_{T}^{miss} [GeV];2HSR/0H+b"); graph_1HMC->SetTitle(";p_{T}^{miss} [GeV];1HSR/0H+b");
  graph_2HData->SetTitle(";p_{T}^{miss} [GeV];2HSR/0H+b"); graph_1HData->SetTitle(";p_{T}^{miss} [GeV];1HSR/0H+b");


  TCanvas * can_h = new TCanvas("METComp_datavmc","METComp_datavmc", 50, 50, 600, 800);
  styleCanvas(can_h);

  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.75, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "middle pad", 0.0, 0.5, 1.0, 0.75);
  TPad *pad3 = new TPad("pad3", "bottom pad", 0.0, 0.0, 1.0, 0.5);
  pad1->SetTickx(0); pad1->SetTicky(0);
  // pad1->SetPad(0., 1 - up_height, 1., 1.00);
  pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
  pad1->SetTopMargin(0.05); pad1->SetBottomMargin(0.05); pad1->SetLeftMargin(0.10); pad1->SetRightMargin(0.04);
  pad1->Draw();

  pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.05); pad2->SetTopMargin(0.05); pad2->SetLeftMargin(0.10); pad2->SetRightMargin(0.04);
  pad2->Draw();

  pad3->SetFillColor(0); pad3->SetFrameFillColor(0);
  pad3->SetBottomMargin(0.20); pad3->SetTopMargin(0.05); pad3->SetLeftMargin(0.10); pad3->SetRightMargin(0.04);
  pad3->Draw();


  //start with drawing 2H
  pad1->cd();
  h_2HSR->GetXaxis()->SetLabelSize(0);
  // h_2HSR->DrawNormalized(); h_0HbSR->DrawNormalized("same");
  h_2HSR->Draw(); h_0HbSR->Draw("same");
  TLegend* legend_2HMC = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend_2HMC->SetBorderSize(0);
  legend_2HMC->AddEntry(h_2HSR, "MC: 2H SR", "PL");
  legend_2HMC->AddEntry(h_0HbSR, "MC: 0H+b", "PL");
  legend_2HMC->Draw();

  pad2->cd();
  h_2HSR_data->GetXaxis()->SetLabelSize(0);
  // h_2HSR_data->DrawNormalized(); h_0HbSR_data->DrawNormalized("same");
  h_2HSR_data->Draw(); h_0HbSR_data->Draw("same");
  TLegend* legend_2HData = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend_2HData->SetBorderSize(0);
  legend_2HData->AddEntry(h_2HSR_data, "Data: 2H SR", "PL");
  legend_2HData->AddEntry(h_0HbSR_data, "Data: 0H+b", "PL");
  legend_2HData->Draw();


  pad3->cd();
  graph_2HMC->Draw("APE");
  graph_2HMC->GetYaxis()->SetTitleOffset(0.9); graph_2HMC->GetYaxis()->SetTitleSize(0.05); graph_2HMC->GetYaxis()->SetLabelSize(0.04);
  graph_2HMC->GetXaxis()->SetTitleOffset(0.9); graph_2HMC->GetXaxis()->SetTitleSize(0.07); graph_2HMC->GetXaxis()->SetLabelSize(0.05);
  graph_2HMC->SetMinimum(0.0);
  graph_2HMC->Draw("APE");
  graph_2HData->Draw("PE same");

  TLegend* legend_2HGraph = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend_2HGraph->SetBorderSize(0);
  legend_2HGraph->AddEntry(graph_2HMC, "MC", "PL");
  legend_2HGraph->AddEntry(graph_2HData, "Data", "PL");
  legend_2HGraph->Draw();

  pad1->cd();
  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize/12);
  latex.DrawLatex(0.95,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(cmsTextSize*1.3);
  latex.DrawLatex(0.20, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextAlign(align_);
  latex.SetTextSize(cmsTextSize*0.85); latex.DrawLatex(0.26, 0.88, extraText);

  cdMET->cd();
  can_h->Write("METshape_FullComp_2H");
  TString savename = "boostedFigures/"+whichRegion+"/dataVsMC/METShapeComp2H.pdf";
  if (savePDFs) can_h->SaveAs(savename,"PDF");


  //Then 1H
  pad1->cd();
  h_1HSR->GetXaxis()->SetLabelSize(0);
  h_1HSR->Draw(); h_0HbSR->Draw("same");
  // h_1HSR->DrawNormalized(); h_0HbSR->DrawNormalized("same");

  TLegend* legend_1HMC = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend_1HMC->SetBorderSize(0);
  legend_1HMC->AddEntry(h_1HSR, "MC: 1H SR", "PL");
  legend_1HMC->AddEntry(h_0HbSR, "MC: 0H+b", "PL");
  legend_1HMC->Draw();

  pad2->cd();
  h_1HSR_data->GetXaxis()->SetLabelSize(0);
  // h_1HSR_data->DrawNormalized(); h_0HbSR_data->DrawNormalized("same");
  h_1HSR_data->Draw(); h_0HbSR_data->Draw("same");

  TLegend* legend_1HData = new TLegend(0.5,0.65,0.95,0.88,NULL,"brNDC") ;
  legend_1HData->SetBorderSize(0);
  legend_1HData->AddEntry(h_1HSR_data, "Data: 1H SR", "PL");
  legend_1HData->AddEntry(h_0HbSR_data, "Data: 0H+b", "PL");
  legend_1HData->Draw();

  pad3->cd();
  graph_1HMC->Draw("APE");
  graph_1HMC->GetYaxis()->SetTitleOffset(0.9); graph_1HMC->GetYaxis()->SetTitleSize(0.05); graph_1HMC->GetYaxis()->SetLabelSize(0.04);
  graph_1HMC->GetXaxis()->SetTitleOffset(0.9); graph_1HMC->GetXaxis()->SetTitleSize(0.07); graph_1HMC->GetXaxis()->SetLabelSize(0.05);
  graph_1HMC->SetMinimum(0.0);
  graph_1HMC->Draw("APE");
  graph_1HData->Draw("PE same");
  legend_2HGraph->Draw();

  pad1->cd();
  latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize/12);
  latex.DrawLatex(0.95,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(cmsTextSize*1.3);
  latex.DrawLatex(0.20, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); latex.SetTextAlign(align_);
  latex.SetTextSize(cmsTextSize*0.85); latex.DrawLatex(0.26, 0.88, extraText);

  cdMET->cd();
  can_h->Write("METshape_FullComp_1H");
  savename = "boostedFigures/"+whichRegion+"/dataVsMC/METShapeComp1H.pdf";
  if (savePDFs) can_h->SaveAs(savename,"PDF");

  delete can_h;
}

void makeMCStackvDataMET(vector<TH1F*> h_MC,  TH1F* h_MC_sum_orig, TH1F* h_data_orig, TH1F* h_2HSR,TH1F* h_1HSR){
  gStyle->SetTextFont(52);
  TH1F * h_MC_sum =(TH1F*)h_MC_sum_orig->Clone("h_MC_sum"); TH1F * h_data =(TH1F*)h_data_orig->Clone("h_data");
  THStack * MCstack = new THStack("hs","");
  TString graphName = "MET_0Hb";
  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, 800, 800);
  styleCanvas(can_h);
  TLegend* legend = new TLegend(0.45,0.7,0.9,0.9);

  bool drawPrelim = false;

  h_2HSR->SetLineColor(kRed);h_2HSR->SetFillColor(c_errorRED); h_2HSR->SetLineStyle(1); h_2HSR->SetLineWidth(5); //h_sig1->SetMarkerColor(kRed); h_sig1->SetMarkerStyle(20); h_sig1->SetMarkerSize(0.7);
  h_1HSR->SetLineColor(kBlue+2); h_1HSR->SetFillColor(c_errorBLUE);  h_1HSR->SetLineWidth(5); //h_sig2->SetMarkerStyle(20); h_sig2->SetMarkerSize(0.7);

  h_MC_sum->SetStats(0); h_MC_sum->SetFillColor(c_error);
  h_data->SetStats(0); h_data->SetLineColor(kBlack); h_data->SetMarkerColor(kBlack); h_data->SetMarkerStyle(20); h_data->SetLineWidth(3); h_data->SetMarkerSize(1.7);
  // h_data->SetBinErrorOption(TH1::kPoisson);
  h_MC_sum->SetTitle(";p_{T}^{miss} [GeV];Fractional yield");

  Int_t col_zjets = TColor::GetColorTransparent(kOrange+1, 0.90);
  Int_t col_wjets = TColor::GetColorTransparent(ci3, 0.90);
  Int_t col_qcd = TColor::GetColorTransparent(ci2, 0.95);
  Int_t col_tt = TColor::GetColorTransparent(ci, 0.95);
  Int_t col_snglt = TColor::GetColorTransparent(kGray+2, 0.95);

  TH1F *h_ZJets = (TH1F*)h_MC[0]->Clone("h_ZJets");h_ZJets->SetFillColor(col_zjets); h_ZJets->SetLineColor(kBlack); h_ZJets->SetMarkerStyle(21); h_ZJets->SetMarkerColor(kOrange+1);
  TH1F *h_WJets = (TH1F*)h_MC[1]->Clone("h_WJets");h_WJets->SetFillColor(col_wjets);h_WJets->SetLineColor(kBlack);h_WJets->SetMarkerStyle(21); h_WJets->SetMarkerColor(kGreen+3);
  TH1F *h_QCD = (TH1F*)h_MC[2]->Clone("h_QCD"); h_QCD->SetFillColor(col_qcd);h_QCD->SetLineColor(kBlack); h_QCD->SetMarkerStyle(21); h_QCD->SetMarkerColor(ci2);
  TH1F *h_TT = (TH1F*)h_MC[3]->Clone("h_TT"); h_TT->SetFillColor(col_tt);h_TT->SetLineColor(kBlack);h_TT->SetMarkerStyle(21); h_TT->SetMarkerColor(ci);
  TH1F *h_SnglT = (TH1F*)h_MC[4]->Clone("h_SnglT");h_SnglT->SetFillColor(col_snglt);h_SnglT->SetLineColor(kBlack); h_SnglT->SetMarkerStyle(21); h_SnglT->SetMarkerColor(kGray+2); //now "Other" category

  h_QCD->Scale(1.0/h_MC_sum->Integral()); h_SnglT->Scale(1.0/h_MC_sum->Integral()); h_WJets->Scale(1.0/h_MC_sum->Integral());
  h_TT->Scale(1.0/h_MC_sum->Integral()); h_ZJets->Scale(1.0/h_MC_sum->Integral());
  MCstack->Add(h_SnglT); MCstack->Add(h_QCD);  MCstack->Add(h_WJets);
  MCstack->Add(h_ZJets); MCstack->Add(h_TT);

  h_data->Scale(1.0/h_data->Integral());
  h_MC_sum->Scale(1.0/h_MC_sum->Integral());

  h_2HSR->Scale(1.0/h_2HSR->Integral()); h_1HSR->Scale(1.0/h_1HSR->Integral());

  legend->AddEntry(h_TT, "t#bar{t}+X", "f");
  legend->AddEntry(h_ZJets, "Z+jets", "f");
  legend->AddEntry(h_WJets, "W+jets", "f");
  legend->AddEntry(h_QCD, "QCD", "f");
  legend->AddEntry(h_SnglT, "Other", "f");
  legend->AddEntry(h_data, "Data", "lep");
  legend->AddEntry(h_2HSR, "2H SR", "l");
  legend->AddEntry(h_1HSR, "1H SR", "l");
  legend->SetNColumns(2);
  legend->SetBorderSize(0);
  MCstack->Draw("hist");


  h_data->GetXaxis()->SetLabelSize(0.04); h_data->GetXaxis()->SetTitleSize(0.05); h_data->GetXaxis()->SetTitleOffset(0.95);
  MCstack->GetXaxis()->SetLabelSize(0.04); MCstack->GetXaxis()->SetTitleSize(0.05); MCstack->GetXaxis()->SetTitleOffset(0.95);
  MCstack->SetTitle(";p_{T}^{miss} [GeV];Fractional yield"); MCstack->GetXaxis()->SetRangeUser(300.0,1400.0);
  h_data->SetTitle(";p_{T}^{miss} [GeV];Fractional yield"); h_data->GetXaxis()->SetRangeUser(300.0,1400.0);


  h_MC_sum->GetYaxis()->SetLabelSize(0.04); h_MC_sum->GetYaxis()->SetTitleSize(0.05); h_MC_sum->GetYaxis()->SetTitleOffset(0.95);
  h_MC_sum->GetXaxis()->SetLabelSize(0.04); h_MC_sum->GetXaxis()->SetTitleSize(0.05); h_MC_sum->GetXaxis()->SetTitleOffset(0.95);

  h_MC_sum->SetMaximum(1.8);
  h_MC_sum->Draw("E2");
  h_2HSR->Draw("E2 same");
  h_1HSR->Draw("E2 same");
  h_2HSR->Draw("hist same"); h_1HSR->Draw("hist same");
  MCstack->Draw("hist same");
  h_data->Draw("same");

  gPad->RedrawAxis(); can_h->SetLogy();
  can_h->Update();can_h->Modified();


  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(0.04);
  latex.DrawLatex(0.94,0.93,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(0.05);
  latex.DrawLatex(0.25, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); //latex.SetTextAlign(align_);
  latex.SetTextSize(0.03);
  if (drawPrelim) latex.DrawLatex(0.40, 0.85, extraText);
  legend->Draw("same");

  cdDatavMC->cd();
  TString savename = "DatavMCComp_MET_0Hb";
  can_h->Write(savename);
  can_h->SaveAs(savename+".pdf","PDF");
}


void makeMCStackvDataComp(vector<TH1F*> h_MC, TH1F* h_MC_sum_orig, TH1F* h_data_orig,TH1F* h_sig1,TH1F* h_sig2, TString region, TString type, bool save){ //Vector length: 5(0l), 3(1l), 2(photon)
  TH1F * h_MC_sum =(TH1F*)h_MC_sum_orig->Clone("h_MC_sum"); TH1F * h_data =(TH1F*)h_data_orig->Clone("h_data");
  THStack * MCstack = new THStack("hs","");
  TString graphName = region+"_"+type;
  TCanvas * can_h = new TCanvas(graphName,graphName, 50, 50, 800, 800);
  styleCanvas(can_h);
  TLegend* legend = new TLegend(0.36,0.7,0.9,0.9);

  bool drawPrelim = false;
  DrawOverflow(h_MC_sum); DrawOverflow(h_data); DrawOverflow(h_sig1); DrawOverflow(h_sig2);
  h_sig1->SetLineColor(kRed); h_sig1->SetLineStyle(1); h_sig1->SetLineWidth(5); //h_sig1->SetMarkerColor(kRed); h_sig1->SetMarkerStyle(20); h_sig1->SetMarkerSize(0.7);
  h_sig2->SetLineColor(kBlue+2); h_sig2->SetMarkerColor(kBlue+2);  h_sig2->SetLineWidth(5); //h_sig2->SetMarkerStyle(20); h_sig2->SetMarkerSize(0.7);

  if (type.Contains("j") || type.Contains("j")) {
    h_MC_sum->Rebin(2); h_data->Rebin(2); h_sig1->Rebin(2);h_sig2->Rebin(2);
    if (region=="2H" && type.Contains("jm") ) {h_MC_sum->Rebin(2); h_data->Rebin(2);h_sig1->Rebin(2);h_sig2->Rebin(2);}
  }
  h_MC_sum->SetStats(0); h_MC_sum->SetFillColor(c_error);
  //h_MC_sum->SetMarkerStyle(20); h_MC_sum->SetMarkerColor(kRed);h_MC_sum->SetMarkerSize(0.3); h_MC_sum->SetLineWidth(1);h_MC_sum->SetLineColor(kRed);
  h_data->SetStats(0); h_data->SetLineColor(kBlack); h_data->SetMarkerColor(kBlack); h_data->SetMarkerStyle(20); h_data->SetLineWidth(3); h_data->SetMarkerSize(1.7);
  h_data->SetBinErrorOption(TH1::kPoisson);
  h_MC_sum->SetTitle(";p_{T}^{miss} [GeV];Events (137 fb^{-1})");

  bool doRatioPanel = true;
  TPad *pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  TPad *pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  if (doRatioPanel){
    pad1->SetTickx(0); pad1->SetTicky(0);
    // pad1->SetPad(0., 1 - up_height, 1., 1.00);
    pad1->SetPad(0., 0., 1., 1.);
    pad1->SetFrameFillColor(0); pad1->SetFillColor(0);
    // pad1->SetTopMargin(0.08); pad1->SetLeftMargin(0.10); pad1->SetRightMargin(0.06); pad2->SetBottomMargin(0.0);

    can_h->SetMargin(0., 0., 0., 0.);
    can_h->SetTicks(1,1);
    can_h->SetFillStyle(4000);

    pad1->SetTicks(1,1);
    pad1->SetFillStyle(4000);
    pad2->SetTicks(1,1);
    pad2->SetFillStyle(4000);

    // pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
    pad2->SetPad(0., 0., 1., 1.);
    pad2->SetFillColor(0); pad2->SetFrameFillColor(0);
    pad1->SetMargin(0.13, 0.055, 0.33, 0.07);
    pad2->SetMargin(0.13, 0.055, 0.12, 1.-0.33);
    // pad2->SetBottomMargin(0.20); pad2->SetTopMargin(0.00);
    // pad2->SetLeftMargin(0.10); pad2->SetRightMargin(0.06);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();
  }

  float scaleMCtoData = h_data_orig->Integral()/h_MC_sum_orig->Integral();
  h_MC_sum->Scale(scaleMCtoData);
  std::cout<<"Scale, region: "<<region<<", type: "<<type<<":   "<<scaleMCtoData<<std::endl;
  TGraphAsymmErrors * graph = new TGraphAsymmErrors(h_data, h_MC_sum, "pois");
  graph->SetLineColor(kBlack); graph->SetMarkerColor(kBlack);
  graph->SetTitle(";p_{T}^{miss} [GeV]; #frac{Data}{MC}");

  TH1F * graph_histo = (TH1F*)h_data->Clone("graph_histo");graph_histo->Divide(h_MC_sum);
  graph_histo->SetLineColor(kBlack); graph_histo->SetMarkerColor(kBlack); graph_histo->SetMarkerStyle(20);
  graph_histo->SetTitle(";p_{T}^{miss} [GeV]; #frac{Data}{MC}");

  h_MC_sum->Scale(1.0/scaleMCtoData);

  TH1F *h_ZJets; TH1F *h_WJets; TH1F *h_QCD; TH1F *h_TT; TH1F *h_SnglT; TH1F *h_GJets;

  if (h_MC.size()==5){ //0l, ZJets, WJets, QCD, TT, SnglT

    Int_t col_zjets = TColor::GetColorTransparent(kOrange+1, 0.90);
    Int_t col_wjets = TColor::GetColorTransparent(ci3, 0.90);
    Int_t col_qcd = TColor::GetColorTransparent(ci2, 0.95);
    Int_t col_tt = TColor::GetColorTransparent(ci, 0.95);
    Int_t col_snglt = TColor::GetColorTransparent(kGray+2, 0.95);

    h_ZJets = (TH1F*)h_MC[0]->Clone("h_ZJets");h_ZJets->SetFillColor(col_zjets); h_ZJets->SetLineColor(kBlack); h_ZJets->SetMarkerStyle(21); h_ZJets->SetMarkerColor(kOrange+1);
    h_WJets = (TH1F*)h_MC[1]->Clone("h_WJets");h_WJets->SetFillColor(col_wjets);h_WJets->SetLineColor(kBlack);h_WJets->SetMarkerStyle(21); h_WJets->SetMarkerColor(kGreen+3);
    h_QCD = (TH1F*)h_MC[2]->Clone("h_QCD"); h_QCD->SetFillColor(col_qcd);h_QCD->SetLineColor(kBlack); h_QCD->SetMarkerStyle(21); h_QCD->SetMarkerColor(ci2);
    h_TT = (TH1F*)h_MC[3]->Clone("h_TT"); h_TT->SetFillColor(col_tt);h_TT->SetLineColor(kBlack);h_TT->SetMarkerStyle(21); h_TT->SetMarkerColor(ci);
    h_SnglT = (TH1F*)h_MC[4]->Clone("h_SnglT");h_SnglT->SetFillColor(col_snglt);h_SnglT->SetLineColor(kBlack); h_SnglT->SetMarkerStyle(21); h_SnglT->SetMarkerColor(kGray+2); //now "Other" category
    if (type.Contains("j") || type.Contains("j")) {
      h_ZJets->Rebin(2); h_WJets->Rebin(2); h_QCD->Rebin(2);
      h_TT->Rebin(2); h_SnglT->Rebin(2);
      if (region=="2H" && type.Contains("jm") ) {h_ZJets->Rebin(2); h_WJets->Rebin(2); h_QCD->Rebin(2); h_TT->Rebin(2); h_SnglT->Rebin(2);}
    }
    DrawOverflow(h_ZJets); DrawOverflow(h_WJets); DrawOverflow(h_QCD); DrawOverflow(h_TT); DrawOverflow(h_SnglT);
    //scaling if necessary
    // scaleMCtoData = 0.865;
    // h_MC_sum->Scale(scaleMCtoData);
    h_QCD->Scale(scaleMCtoData); h_SnglT->Scale(scaleMCtoData); h_WJets->Scale(scaleMCtoData);
    h_TT->Scale(scaleMCtoData); h_ZJets->Scale(scaleMCtoData);

    MCstack->Add(h_SnglT); MCstack->Add(h_QCD);  MCstack->Add(h_WJets);
    MCstack->Add(h_ZJets); MCstack->Add(h_TT);
    h_MC_sum->Scale(scaleMCtoData);

    std::string scaleString = to_string(scaleMCtoData);
    // TString legHeader = "MC scaled: "+scaleString;
    // legend->SetHeader(legHeader);
    // legend->AddEntry(h_TT, "t#bar{t}+X", "f");
    // legend->AddEntry(h_ZJets, "Z+jets", "f");
    // legend->AddEntry(h_WJets, "W+jets", "f");
    // legend->AddEntry(h_QCD, "QCD", "f");
    // legend->AddEntry(h_SnglT, "Other", "f");
  }
  else if (h_MC.size()==3) { //1l, WJets, TT, SnglT
    h_WJets = (TH1F*)h_MC[0]->Clone("h_WJets");h_WJets->SetFillColor(kBlue);h_WJets->SetLineColor(kBlack);h_WJets->SetMarkerStyle(21); h_WJets->SetMarkerColor(kBlue);
    h_TT = (TH1F*)h_MC[1]->Clone("h_TT"); h_TT->SetFillColor(kCyan);h_TT->SetLineColor(kBlack);h_TT->SetMarkerStyle(21); h_TT->SetMarkerColor(kCyan);
    h_SnglT = (TH1F*)h_MC[2]->Clone("h_SnglT");h_SnglT->SetFillColor(kOrange);h_SnglT->SetLineColor(kBlack); h_SnglT->SetMarkerStyle(21); h_SnglT->SetMarkerColor(kOrange);
    if (type.Contains("j1") || type.Contains("j2")) {
      h_WJets->Rebin(2); h_TT->Rebin(2); h_SnglT->Rebin(2);
    }
    // scaleMCtoData = 0.573;
    // h_MC_sum->Scale(scaleMCtoData);
    h_SnglT->Scale(scaleMCtoData); h_WJets->Scale(scaleMCtoData); h_TT->Scale(scaleMCtoData);

    MCstack->Add(h_WJets); MCstack->Add(h_SnglT); MCstack->Add(h_TT);

    std::string scaleString = to_string(scaleMCtoData);
    // TString legHeader = "MC scaled: "+scaleString;
    // legend->SetHeader(legHeader);
    legend->AddEntry(h_WJets, "WJets", "f");
    legend->AddEntry(h_SnglT, "SnglT", "f");
    legend->AddEntry(h_TT, "TT", "f");
  }
  else if (h_MC.size()==2) { //1l, QCD, GJets
    h_QCD = (TH1F*)h_MC[0]->Clone("h_QCD"); h_QCD->SetFillColor(kGray);h_QCD->SetLineColor(kBlack); h_QCD->SetMarkerStyle(21); h_QCD->SetMarkerColor(kGray);
    h_GJets = (TH1F*)h_MC[1]->Clone("h_GJets");h_GJets->SetFillColor(kGreen);h_GJets->SetLineColor(kBlack); h_GJets->SetMarkerStyle(21); h_GJets->SetMarkerColor(kGreen);
    if (type.Contains("j1") || type.Contains("j2")) {
      h_GJets->Rebin(2); h_QCD->Rebin(2);
    }
    // scaleMCtoData = 0.813;
    // h_MC_sum->Scale(scaleMCtoData);
    h_GJets->Scale(scaleMCtoData); h_QCD->Scale(scaleMCtoData);

    MCstack->Add(h_QCD); MCstack->Add(h_GJets);

    std::string scaleString = to_string(scaleMCtoData);
    TString legHeader = "MC scaled: "+scaleString;
    legend->SetHeader(legHeader);
    legend->AddEntry(h_QCD, "QCD", "f");
    legend->AddEntry(h_GJets, "GJets", "f");
  }
  else {
    std::cout<<"Length of histogram vector incorrect!! No MC stack will be produced!"<<std::endl;
  }
  legend->AddEntry(h_data, "Data", "lep");
  legend->AddEntry(h_QCD, "QCD", "f");
  legend->AddEntry(h_TT, "t#bar{t}+X", "f");
  legend->AddEntry(h_SnglT, "Other", "f");
  legend->AddEntry(h_ZJets, "Z+jets", "f");
  legend->AddEntry(h_sig1, "TChiHH-G(500,1)", "l");
  legend->AddEntry(h_WJets, "W+jets", "f");
  legend->AddEntry(h_sig2, "T5HH(1600,1)", "l");
  legend->SetNColumns(2);
  legend->SetBorderSize(0);
  legend->SetTextSize(0.042);
  MCstack->Draw("hist");

  if (!doRatioPanel){
    h_data->GetXaxis()->SetLabelSize(0.04); h_data->GetXaxis()->SetTitleSize(0.06); h_data->GetXaxis()->SetTitleOffset(0.9);
    MCstack->GetXaxis()->SetLabelSize(0.04); MCstack->GetXaxis()->SetTitleSize(0.06); MCstack->GetXaxis()->SetTitleOffset(0.9);
    if (type=="MET") {
      MCstack->SetTitle(";p_{T}^{miss} [GeV];Events");
      MCstack->GetXaxis()->SetRangeUser(300.0,1400.0);
      h_data->SetTitle(";p_{T}^{miss} [GeV];Events");
      h_data->GetXaxis()->SetRangeUser(300.0,1400.0);
    }
    else if (type=="jm") {
      MCstack->SetTitle(";Jet softdrop mass [GeV];Entries");
      MCstack->GetXaxis()->SetRangeUser(60.0,260.0);
      h_data->SetTitle(";Jet softdrop mass [GeV];Entries");
      h_data->GetXaxis()->SetRangeUser(60.0,260.0);
    }
    else if (type=="jpt") {
      MCstack->SetTitle(";Jet p_{T} [GeV];Entries");
      MCstack->GetXaxis()->SetRangeUser(300.0,1300.0);
      h_data->SetTitle(";Jet p_{T} [GeV];Entries");
      h_data->GetXaxis()->SetRangeUser(300.0,1300.0);
    }
    else if (type=="jbb") {
      MCstack->SetTitle(";Jet deep bb-tag;Entries");
      MCstack->GetXaxis()->SetRangeUser(0.0,1.0);
      h_data->SetTitle(";Jet deep bb-tag;Entries");
      h_data->GetXaxis()->SetRangeUser(0.0,1.0);
    }
  }
  else {
    if (type=="MET") {
      h_MC_sum->GetXaxis()->SetRangeUser(300.0,1400.0);
      h_MC_sum->SetTitle(";;Events");
    }
    else if (type=="jm" || type=="j1m" || type=="j2m") {
      h_MC_sum->SetTitle(";;Entries / 5 GeV");
      if (region=="2H") h_MC_sum->SetTitle(";;Entries / 10 GeV");
      h_MC_sum->GetXaxis()->SetRangeUser(60.0,260.0);
    }
    else if (type=="jpt" || type=="j1pt" || type=="j2pt") {
      h_MC_sum->SetTitle(";;Entries / 40 GeV");
      h_MC_sum->GetXaxis()->SetRangeUser(300.0,1300.0);
    }
    else if (type=="jbb" || type=="j1bb" || type=="j2bb") {
      h_MC_sum->SetTitle(";;Entries / 0.04");
      h_MC_sum->GetXaxis()->SetRangeUser(0.0,1.0);
    }
  }

  if (doRatioPanel) {h_MC_sum->GetXaxis()->SetLabelSize(0); h_MC_sum->GetXaxis()->SetTitleSize(0);}
  h_MC_sum->GetYaxis()->SetLabelSize(0.038); h_MC_sum->GetYaxis()->SetTitleSize(0.044); h_MC_sum->GetYaxis()->SetTitleOffset(1.35);
  float thisMax = h_MC_sum->GetMaximum(); if (h_data->GetMaximum()>thisMax) thisMax = h_data->GetMaximum();
  if (type=="MET") {
    if (region=="baseline") h_MC_sum->GetYaxis()->SetRangeUser(4E0,4E4);
    else h_MC_sum->SetMaximum(thisMax*2.5);
  }
  else if (type=="jbb" || type=="j1bb" || type=="j2bb") h_MC_sum->SetMaximum(thisMax*10.5);
  else if (type=="j1m" || type=="j2m") h_MC_sum->SetMaximum(thisMax*1.6);
  else if (type=="jm" && region=="baseline") h_MC_sum->SetMaximum(thisMax*1.6);
  else if (type=="jm" && region=="0H") h_MC_sum->SetMaximum(thisMax*1.6);
  else if (type=="jm" && region=="1H") h_MC_sum->SetMaximum(thisMax*1.7);
  else if (type=="jm" && region=="2H") h_MC_sum->SetMaximum(80.0);
  else if (type=="j1pt") h_MC_sum->SetMaximum(thisMax*1.6);
  else h_MC_sum->SetMaximum(thisMax*1.3);

  h_MC_sum->Draw("E2");
  MCstack->Draw("hist same");
  h_data->Draw("E0 same");
  h_sig1->Draw("hist same"); h_sig2->Draw("hist same");
  gPad->RedrawAxis();

  if (type=="MET" || type=="jbb" || type=="j1bb" || type=="j2bb") gPad->SetLogy();
  can_h->Update();can_h->Modified();

  if (doRatioPanel){
    pad2->cd();
    graph->Draw("APE");
    //Now change everything about this graph
    double minOfLine = 300.0;
    double maxOfLine = 1400.0;
    graph->GetYaxis()->SetNdivisions(505);
    if (type=="MET") {
      graph->GetXaxis()->SetTitle("p_{T}^{miss} [GeV]");
      graph->GetXaxis()->SetRangeUser(300.0,1400.0);
      minOfLine=300.0; maxOfLine=1400.0;
    }
    else if (type=="nH") {
      graph_histo->GetXaxis()->SetTitle("Number Higgs tags");
      graph_histo->GetXaxis()->SetBinLabel(1,"0H");graph_histo->GetXaxis()->SetBinLabel(2,"1H");graph_histo->GetXaxis()->SetBinLabel(3,"2H");
    }
    else if (type=="jm") {
      graph->GetXaxis()->SetTitle("m_{J} [GeV]");
      graph->GetXaxis()->SetRangeUser(60.0,260.0);
      minOfLine=60.0; maxOfLine=260.0;
    }
    else if (type=="j1m") {
      graph->GetXaxis()->SetTitle("Leading m_{J} [GeV]");
      graph->GetXaxis()->SetRangeUser(60.0,260.0);
      minOfLine=60.0; maxOfLine=260.0;
    }
    else if (type=="j2m") {
      graph->GetXaxis()->SetTitle("Subleading m_{J} [GeV]");
      graph->GetXaxis()->SetRangeUser(60.0,260.0);
      minOfLine=60.0; maxOfLine=260.0;
    }
    else if (type=="jpt") {
      graph->GetXaxis()->SetTitle("Jet p_{T} [GeV]");
      graph->GetXaxis()->SetRangeUser(300.0,1300.0);
      minOfLine=300.0; maxOfLine=1300.0;
    }
    else if (type=="j1pt") {
      graph->GetXaxis()->SetTitle("Leading jet p_{T} [GeV]");
      graph->GetXaxis()->SetRangeUser(300.0,1300.0);
      minOfLine=300.0; maxOfLine=1300.0;
    }
    else if (type=="j2pt") {
      graph->GetXaxis()->SetTitle("Subleading jet p_{T} [GeV]");
      graph->GetXaxis()->SetRangeUser(300.0,1300.0);
      minOfLine=300.0; maxOfLine=1300.0;
    }
    else if (type=="jbb") {
      graph->GetXaxis()->SetTitle("D_{bb}");
      graph->GetXaxis()->SetRangeUser(0.0,1.0);
      minOfLine=0.0; maxOfLine=1.0;
    }
    else if (type=="j1bb") {
      graph->GetXaxis()->SetTitle("Leading jet D_{bb}");
      graph->GetXaxis()->SetRangeUser(0.0,1.0);
      minOfLine=0.0; maxOfLine=1.0;
    }
    else if (type=="j2bb") {
      graph->GetXaxis()->SetTitle("Subleading jet D_{bb}");
      graph->GetXaxis()->SetRangeUser(0.0,1.0);
      minOfLine=0.0; maxOfLine=1.0;
    }

    else graph->GetXaxis()->SetTitle(type);
    TLine *line_at1 = new TLine(minOfLine,1.0,maxOfLine,1.0);
    line_at1->SetLineColor(kBlack);
    line_at1->SetLineStyle(2);

    if (type=="nH") {
      graph_histo->SetBinError(1,graph->GetErrorY(1));
      graph_histo->SetBinError(2,graph->GetErrorY(2));
      graph_histo->SetBinError(3,graph->GetErrorY(3));
      graph_histo->GetXaxis()->SetLabelSize(0.15); graph_histo->GetXaxis()->SetTitleSize(0.07); graph_histo->GetXaxis()->SetTitleOffset(1.20);
      graph_histo->GetYaxis()->SetLabelSize(0.06); graph_histo->GetYaxis()->SetTitleSize(0.07); graph_histo->GetYaxis()->SetTitleOffset(0.55);
      graph_histo->Draw();
      // graph_histo->Fit("pol0","Q");
    }
    else {
      graph->Draw("APE");
      graph->GetXaxis()->SetLabelSize(0.038); graph->GetXaxis()->SetTitleSize(0.045); graph->GetXaxis()->SetTitleOffset(1.1);
      graph->GetYaxis()->SetLabelSize(0.04); graph->GetYaxis()->SetTitleSize(0.036); graph->GetYaxis()->SetTitleOffset(1.7);
      graph->GetYaxis()->CenterTitle();
      graph->GetYaxis()->SetRangeUser(0.0,1.95);
      if (region=="2H" && type=="jm") graph->GetYaxis()->SetRangeUser(0.0,2.95);
      // graph->Fit("pol0","Q");
      graph->Draw("APE");
      line_at1->Draw("same");
    }
    pad1->cd();
  }

  TLatex latex; latex.SetNDC();
  latex.SetTextAngle(0); latex.SetTextColor(kBlack);
  latex.SetTextFont(42); latex.SetTextAlign(31);
  latex.SetTextSize(0.045);
  latex.DrawLatex(0.94,0.94,lumiText);
  latex.SetTextFont(cmsTextFont); latex.SetTextSize(0.045);
  latex.DrawLatex(0.25, 0.85, cmsText);
  latex.SetTextFont(extraTextFont); //latex.SetTextAlign(align_);
  latex.SetTextSize(0.03);

  TLatex regionLabel(0.75, 0.65, region);
  regionLabel.SetNDC(); regionLabel.SetTextAlign(12); regionLabel.SetTextFont(42); regionLabel.SetTextSize(0.06);
  if (region!="baseline") regionLabel.Draw("same");

  TLatex suppLabel(0.16, 0.82, "Supplementary");
  suppLabel.SetNDC(); suppLabel.SetTextFont(extraTextFont); suppLabel.SetTextSize(0.03);
  // suppLabel.Draw("same");

  //Draw lines for signal region
  if (type=="jm" || type=="j1m" || type=="j2m") {
    float max = MCstack->GetMaximum();
    if (h_data->GetMaximum()>max) max = h_data->GetMaximum();
    max = max*1.2;
    if (max>230.0) max=230.0;
    if (region=="1H") max = 48.0;
    else if (region=="2H") max = 58.0;
    TLine *line1 = new TLine(95.0,0,95.0,max);
    TLine *line2 = new TLine(145.0,0,145.0,max);
    line1->SetLineColor(kBlack); line1->SetLineStyle(9); line1->SetLineWidth(3); line1->Draw("same");
    line2->SetLineColor(kBlack); line2->SetLineStyle(9); line2->SetLineWidth(3); line2->Draw("same");
  }
  else if (type=="jbb" || type=="j1bb" || type=="j2bb") {
    float max = MCstack->GetMaximum();
    if (h_data->GetMaximum()>max) max = h_data->GetMaximum();
    max = max*1.2;
    TLine *line1 = new TLine(0.7,0,0.7,max);
    line1->SetLineColor(kBlack); line1->SetLineStyle(9); line1->SetLineWidth(3); line1->Draw("same");
    can_h->SetLogy();
  }

  legend->Draw("same");

  cdDatavMC->cd();
  TString savename = "DatavMC_"+type+"_"+region;
  can_h->Write(savename);
  std::cout<<"Integral of data events in "<<region<<" region, type: "<<type<<",    "<<h_data->Integral()<<std::endl;
  if (savePDFs)  can_h->SaveAs("boostedFigures/"+whichRegion+"/dataVsMC/"+savename+".pdf","PDF");
  if (save) {
    can_h->SaveAs(savename+".pdf","PDF");
    latex.DrawLatex(0.30, 0.82, extraText);
    can_h->SaveAs(savename+"_prelim.pdf","PDF");
  }
}

void justBB(TH1F* h_bkgOrig, TH1F* h_sigOrig, TString bkgType, TString sigType, TString sigMass){
  TH1F* h_bkg = (TH1F*)h_bkgOrig->Clone("h_bkg");
  TH1F* h_sig = (TH1F*)h_sigOrig->Clone("h_sig");

  h_bkg->SetLineColor(kBlack); h_bkg->SetFillColor(0); h_bkg->SetLineWidth(3);
  h_sig->SetLineColor(kRed); h_sig->SetFillColor(0); h_sig->SetLineWidth(3);

  TString sigLeg;
  if (sigType=="TChiHH") sigLeg = "TChiHH, m_{#tilde{h}} = "+sigMass+" GeV";
  else if (sigType=="T5HH") sigLeg = "T5HH("+sigMass+",1)";

  TLegend* legend = new TLegend(0.5,0.65,0.93,0.91);
  legend->SetBorderSize(0);
  if (bkgType=="AllMC") legend->AddEntry(h_bkg,"All MC background","l");
  else if (bkgType=="TT") legend->AddEntry(h_bkg,"t#bar{t}+X","l");
  legend->AddEntry(h_sig,sigLeg,"l");

  TString canvName = "justBB_"+bkgType+"vs"+sigType+sigMass;
  TCanvas * can_h = new TCanvas(canvName,canvName, 50, 50, 900, 900);
  styleCanvas(can_h);
  if (h_bkg->GetMaximum()>h_sig->GetMaximum()) {
    h_bkg->SetStats(0);
    h_bkg->SetTitle(";#it{D_{bb}};Events");
    h_bkg->GetYaxis()->SetTitleSize(0.05);
    h_bkg->GetYaxis()->SetTitleOffset(0.90);
    h_bkg->GetXaxis()->SetTitleSize(0.05);
    h_bkg->GetXaxis()->SetTitleOffset(1.0);
    h_bkg->Draw("hist");
    h_sig->Draw("hist same");
  }
  else {
    h_sig->SetStats(0);
    h_sig->SetTitle(";#it{D_{bb}};Events");
    h_sig->GetYaxis()->SetTitleSize(0.05);
    h_sig->GetYaxis()->SetTitleOffset(0.9);
    h_sig->GetXaxis()->SetTitleSize(0.05);
    h_sig->GetXaxis()->SetTitleOffset(1.0);
    h_sig->Draw("hist");
    h_bkg->Draw("hist same");
  }

  legend->Draw();
  addLumiCanv(can_h);
  cdOther->cd();
  can_h->Write(canvName);
  if (savePDFs)  can_h->SaveAs("boostedFigures/"+whichRegion+"/bkgest/"+canvName+".pdf","PDF");
}



/*
TotalBkg->Draw("MET>>h_bkgAll(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300)*totalWeight");
TChiHH500_LSP1->Draw("MET>>h_TChiHH500All(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300)*totalWeight");
TChiHH1000_LSP1->Draw("MET>>h_TChiHH1000All(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300)*totalWeight");
T5HH1600_LSP1->Draw("MET>>h_T5HH1600All(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300)*totalWeight");
T5HH2200_LSP1->Draw("MET>>h_T5HH2200All(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300)*totalWeight");

TotalBkg->Draw("MET>>h_bkg(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300&&!resBaseline)*totalWeight");
TChiHH500_LSP1->Draw("MET>>h_TChiHH500(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300&&!resBaseline)*totalWeight");
TChiHH1000_LSP1->Draw("MET>>h_TChiHH1000(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300&&!resBaseline)*totalWeight");
T5HH1600_LSP1->Draw("MET>>h_T5HH1600(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300&&!resBaseline)*totalWeight");
T5HH2200_LSP1->Draw("MET>>h_T5HH2200(56,200,3000)","(HT>600&&J1_pt>300&&J2_pt>300&&!resBaseline)*totalWeight");

h_bkg->SetLineColor(kRed); h_bkg->SetMarkerColor(kRed); h_bkg->SetMarkerStyle(20); h_bkg->SetMarkerSize(0.7);
h_TChiHH500->SetLineColor(kBlue); h_TChiHH500->SetMarkerColor(kBlue); h_TChiHH500->SetMarkerStyle(20); h_TChiHH500->SetMarkerSize(0.7);
h_TChiHH1000->SetLineColor(kBlack); h_TChiHH1000->SetMarkerColor(kBlack); h_TChiHH1000->SetMarkerStyle(20); h_TChiHH1000->SetMarkerSize(0.7);
h_T5HH1600->SetLineColor(kMagenta); h_T5HH1600->SetMarkerColor(kMagenta); h_T5HH1600->SetMarkerStyle(20); h_T5HH1600->SetMarkerSize(0.7);
h_T5HH2200->SetLineColor(kGreen); h_T5HH2200->SetMarkerColor(kGreen); h_T5HH2200->SetMarkerStyle(20); h_T5HH2200->SetMarkerSize(0.7);

TLegend* legend = new TLegend(0.60,0.7,0.93,0.91);
legend->SetBorderSize(0);
legend->AddEntry(h_bkg,"Total MC background","lp")
legend->AddEntry(h_TChiHH500,"TChiHH500","lp")
legend->AddEntry(h_TChiHH1000,"TChiHH1000","lp")
legend->AddEntry(h_T5HH1600,"T5HH1600","lp")
legend->AddEntry(h_T5HH2200,"T5HH2200","lp")

h_bkg->SetStats(0); h_TChiHH500->SetStats(0); h_TChiHH1000->SetStats(0); h_T5HH1600->SetStats(0); h_T5HH2200->SetStats(0);

h_bkg->DrawNormalized();
h_TChiHH500->DrawNormalized("same");
h_TChiHH1000->DrawNormalized("same");
h_T5HH1600->DrawNormalized("same");
h_T5HH2200->DrawNormalized("same");
legend->Draw();
*/

void mass2D_plane(TString region) {
  bool addPrelim = true; TString prelimString = "_prelim";
  gStyle->SetTextFont(52);
  TLegend l(0.18,0.72,0.48,0.85);

  //153,148,193
  //0.5333, 0.6667, 0.9137

  Double_t red[9]   = { 0.6, 0.5333, 0.5176, 0.5529, 0.6627, 0.8039, 0.9216, 0.9922, 0.9843};
  Double_t green[9] = { 0.5804, 0.7333, 0.7922, 0.8353, 0.8588, 0.8667, 0.8667, 0.8980, 0.9843};
  Double_t blue[9]  = { 0.7569, 0.9176, 0.8980, 0.8471, 0.7765, 0.7098, 0.6549, 0.588, 0.5255};
  Double_t stops[9] = { 0.00000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
  Int_t nb=255;
  TColor::CreateGradientColorTable(9, stops, red, green, blue, nb, 1.0);


  TFile * f0 = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0lData.root"); //Data
  TH2F* j1vj2M_data;
  if (region=="Baseline") j1vj2M_data = (TH2F*)f0->Get("j1vj2M_baseline");
  else if (region=="0H")  j1vj2M_data = (TH2F*)f0->Get("j1vj2M_0H_data");
  else if (region=="1H")  j1vj2M_data = (TH2F*)f0->Get("j1vj2M_1H_data");
  else if (region=="2H")  j1vj2M_data = (TH2F*)f0->Get("j1vj2M_2H_data");
  j1vj2M_data->SetMarkerStyle(21); j1vj2M_data->SetMarkerSize(0.5); j1vj2M_data->SetMarkerColor(kBlack);

  TH2F* j1vj2M_ZJets; TH2F* j1vj2M_WJets; TH2F* j1vj2M_SnglT; TH2F* j1vj2M_TT; TH2F* j1vj2M_QCD;
  TFile * f1 = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0l.root"); //MC
  if (region=="Baseline") {
    j1vj2M_ZJets = (TH2F*)f1->Get("j1vj2M_baseline_ZJets");
    j1vj2M_WJets = (TH2F*)f1->Get("j1vj2M_baseline_WJets");
    j1vj2M_SnglT = (TH2F*)f1->Get("j1vj2M_baseline_SnglT");
    j1vj2M_TT = (TH2F*)f1->Get("j1vj2M_baseline_TT");
    j1vj2M_QCD = (TH2F*)f1->Get("j1vj2M_baseline_QCD");
  }
  else if (region=="0H") {
    j1vj2M_ZJets = (TH2F*)f1->Get("j1vj2M_0H_ZJets");
    j1vj2M_WJets = (TH2F*)f1->Get("j1vj2M_0H_WJets");
    j1vj2M_SnglT = (TH2F*)f1->Get("j1vj2M_0H_SnglT");
    j1vj2M_TT = (TH2F*)f1->Get("j1vj2M_0H_TT");
    j1vj2M_QCD = (TH2F*)f1->Get("j1vj2M_0H_QCD");
  }
  else if (region=="1H") {
    j1vj2M_ZJets = (TH2F*)f1->Get("j1vj2M_1H_ZJets");
    j1vj2M_WJets = (TH2F*)f1->Get("j1vj2M_1H_WJets");
    j1vj2M_SnglT = (TH2F*)f1->Get("j1vj2M_1H_SnglT");
    j1vj2M_TT = (TH2F*)f1->Get("j1vj2M_1H_TT");
    j1vj2M_QCD = (TH2F*)f1->Get("j1vj2M_1H_QCD");
  }
  else if (region=="2H") {
    j1vj2M_ZJets = (TH2F*)f1->Get("j1vj2M_2H_ZJets");
    j1vj2M_WJets = (TH2F*)f1->Get("j1vj2M_2H_WJets");
    j1vj2M_SnglT = (TH2F*)f1->Get("j1vj2M_2H_SnglT");
    j1vj2M_TT = (TH2F*)f1->Get("j1vj2M_2H_TT");
    j1vj2M_QCD = (TH2F*)f1->Get("j1vj2M_2H_QCD");
  }

  TH2F *h_MC_sum = (TH2F*)j1vj2M_ZJets->Clone("h_MC_sum");
  h_MC_sum->Add(j1vj2M_WJets); h_MC_sum->Add(j1vj2M_SnglT);
  h_MC_sum->Add(j1vj2M_TT); h_MC_sum->Add(j1vj2M_QCD);
  h_MC_sum->RebinX(8); h_MC_sum->RebinY(8); h_MC_sum->SetStats(0);
  h_MC_sum->SetTitle(";m_{J} (leading jet) [GeV]; m_{J} (subleading jet) [GeV]; SM background [Events]");
  h_MC_sum->GetZaxis()->SetTitleOffset(1.0);
  h_MC_sum->GetXaxis()->SetTitleSize(0.04); h_MC_sum->GetYaxis()->SetTitleSize(0.04); h_MC_sum->GetZaxis()->SetTitleSize(0.04);
  h_MC_sum->GetXaxis()->SetLabelSize(0.03); h_MC_sum->GetYaxis()->SetLabelSize(0.03); h_MC_sum->GetZaxis()->SetLabelSize(0.03);

  TFile * f2 = TFile::Open("ALPHABETMC2016_V18_1DT5HH_FullSIM_resVeto_1600.root"); //TChiHH 600_1, veto, 170 events after baseline selection only
  TH2F* j1vj2M_TChiHH500_LSP1;
  if (region=="Baseline") j1vj2M_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_baseline_T5HH1600_LSP1");
  else if (region=="0H")  j1vj2M_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_0H_T5HH1600_LSP1");
  else if (region=="1H")  j1vj2M_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_1H_T5HH1600_LSP1");
  else if (region=="2H")  j1vj2M_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_2H_T5HH1600_LSP1");
  j1vj2M_TChiHH500_LSP1->SetMarkerColor(kRed); j1vj2M_TChiHH500_LSP1->SetMarkerStyle(20); j1vj2M_TChiHH500_LSP1->SetMarkerSize(0.3);

  TH2F* h_red = (TH2F*)j1vj2M_TChiHH500_LSP1->Clone("h_red");
  h_red->SetMarkerStyle(20); h_red->SetMarkerSize(0.8);
  TH2F* h_black = (TH2F*)j1vj2M_data->Clone("h_black");
  h_black->SetMarkerSize(0.8);
  l.AddEntry(h_red,"T5HH(1600,1)","p");
  l.AddEntry(h_black,"Data","p");

  TCanvas *c = new TCanvas("can","can", 800, 800);
  c->SetMargin(0.13, 0.15, 0.11, 0.08);

  //gStyle->SetPalette(kLightTerrain,0,1.0); TColor::InvertPalette(); //this works OK but is super ugly
  h_MC_sum->SetContour(50);
  h_MC_sum->Draw("colz");
  j1vj2M_TChiHH500_LSP1->Draw("same");
  j1vj2M_data->Draw("same");
  l.Draw("same");

  TLatex ltitle_prelim(0.26, 0.943, "#scale[0.76]{#font[52]{ Preliminary}}");
  TLatex ltitle(0.16, 0.948, "#font[62]{CMS}");
  TLatex rtitle(0.85, 0.948, "#scale[0.8]{137 fb^{-1} (13 TeV)}");
  rtitle.SetTextFont(42);
  ltitle.SetNDC(); ltitle_prelim.SetNDC(); rtitle.SetNDC();
  ltitle.SetTextAlign(12); ltitle_prelim.SetTextAlign(12); rtitle.SetTextAlign(32);
  ltitle.Draw("same"); rtitle.Draw("same");
  TLatex regionLabel(0.625, 0.80, region);
  regionLabel.SetNDC(); regionLabel.SetTextAlign(12); regionLabel.SetTextFont(42); regionLabel.SetTextSize(0.06);
  regionLabel.Draw("same");

  TLine *line1 = new TLine(95,95,145,95);
  TLine *line2 = new TLine(95,145,145,145);
  TLine *line3 = new TLine(95,95,95,145);
  TLine *line4 = new TLine(145,95,145,145);

  line1->SetLineColor(kBlack); line1->SetLineStyle(1);
  line2->SetLineColor(kBlack); line2->SetLineStyle(1);
  line3->SetLineColor(kBlack); line3->SetLineStyle(1);
  line4->SetLineColor(kBlack); line4->SetLineStyle(1);
  line1->Draw("same"); line2->Draw("same");
  line3->Draw("same"); line4->Draw("same");


  TString saveName = "j1vj2M_"+region;
  c->SaveAs(saveName+".pdf","PDF");

  ltitle_prelim.Draw("same");
  c->SaveAs(saveName+prelimString+".pdf","PDF");

  delete c;

  f0->Close(); f1->Close(); f2->Close();
}

void runMass2D() {
  // mass2D_plane("Baseline");
  mass2D_plane("0H");
  mass2D_plane("1H");
  mass2D_plane("2H");
}

void TESTmass2D_plane(TString region) {
  bool addPrelim = false; TString prelimString = ""; if (addPrelim) prelimString="_prelim";
  gStyle->SetTextFont(52);
  TCanvas c("can","can", 800, 800); c.cd();
  c.SetMargin(0.13, 0.15, 0.11, 0.08);
  TLegend l(0.18,0.72,0.48,0.85);
  //l.SetTextSize(0.04); l.SetBorderSize(0); l.SetFillStyle(0); l.SetFillColor(0);

  TFile * f0 = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0lData.root","r"); //Data
  TFile * f1 = TFile::Open("/eos/uscms/store/user/emacdona/boostedHiggsPlusMET/boosted_veto_Final/ALPHABET_0l.root","r"); //MC
  TFile * f2 = TFile::Open("ALPHABETMC2016_V18_1DT5HH_FullSIM_resVeto_1600.root","r"); //TChiHH 600_1, veto, 170 events after baseline selection only

  // f1->cd();
  TH2F* j1vj2M_baseline_ZJets = (TH2F*)f1->Get("j1vj2M_baseline_ZJets");
  TH2F* j1vj2M_baseline_WJets = (TH2F*)f1->Get("j1vj2M_baseline_WJets");
  TH2F* j1vj2M_baseline_SnglT = (TH2F*)f1->Get("j1vj2M_baseline_SnglT");
  TH2F* j1vj2M_baseline_TT = (TH2F*)f1->Get("j1vj2M_baseline_TT");
  TH2F* j1vj2M_baseline_QCD = (TH2F*)f1->Get("j1vj2M_baseline_QCD");

  TH2F *h_baseline_sum = (TH2F*)j1vj2M_baseline_ZJets->Clone("h_baseline_sum");
  h_baseline_sum->Add(j1vj2M_baseline_WJets);
  h_baseline_sum->Add(j1vj2M_baseline_SnglT);
  h_baseline_sum->Add(j1vj2M_baseline_TT);
  h_baseline_sum->Add(j1vj2M_baseline_QCD);
  h_baseline_sum->RebinX(8); h_baseline_sum->RebinY(8);
  // h_baseline_sum->SetTitle(";Leading m_{J} [GeV]; Subleading m_{J} [GeV]; SM background [Events]");
  h_baseline_sum->SetTitle(";m_{J} (leading jet) [GeV]; m_{J} (subleading jet) [GeV]; SM background [Events]");
  h_baseline_sum->GetZaxis()->SetTitleOffset(1.0);
  h_baseline_sum->GetXaxis()->SetTitleSize(0.04);
  h_baseline_sum->GetYaxis()->SetTitleSize(0.04);
  h_baseline_sum->GetZaxis()->SetTitleSize(0.04);

  h_baseline_sum->GetXaxis()->SetLabelSize(0.03);
  h_baseline_sum->GetYaxis()->SetLabelSize(0.03);
  h_baseline_sum->GetZaxis()->SetLabelSize(0.03);

  h_baseline_sum->SetStats(0);

  Double_t red[9]   = { 0.2082, 0.0592, 0.0780, 0.0232, 0.1802, 0.5301, 0.8186, 0.9956, 0.9764};
  Double_t green[9] = { 0.1664, 0.3599, 0.5041, 0.6419, 0.7178, 0.7492, 0.7328, 0.7862, 0.9832};
  Double_t blue[9]  = { 0.5293, 0.8684, 0.8385, 0.7914, 0.6425, 0.4662, 0.3499, 0.1968, 0.0539};
  Double_t stops[9] = { 0.0000, 0.1250, 0.2500, 0.3750, 0.5000, 0.6250, 0.7500, 0.8750, 1.0000};
  Int_t nb=255;
  TColor::CreateGradientColorTable(9, stops, red, green, blue, nb,0.5);
  h_baseline_sum->SetContour(nb);
  h_baseline_sum->Draw("colz");


  // f2->cd();
  TH2F* j1vj2M_baseline_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_baseline_T5HH1600_LSP1");
  j1vj2M_baseline_TChiHH500_LSP1->SetMarkerColor(kRed); j1vj2M_baseline_TChiHH500_LSP1->SetMarkerStyle(20); j1vj2M_baseline_TChiHH500_LSP1->SetMarkerSize(0.3);
  j1vj2M_baseline_TChiHH500_LSP1->Draw("same");
  // f0->cd();
  TH2F* j1vj2M_baseline = (TH2F*)f0->Get("j1vj2M_baseline");
  j1vj2M_baseline->SetMarkerStyle(21); j1vj2M_baseline->SetMarkerSize(0.5); j1vj2M_baseline->SetMarkerColor(kBlack);
  j1vj2M_baseline->Draw("same");

  TH2F* h_red = (TH2F*)j1vj2M_baseline_TChiHH500_LSP1->Clone("h_red");
  h_red->SetMarkerStyle(20); h_red->SetMarkerSize(0.8);
  TH2F* h_black = (TH2F*)j1vj2M_baseline->Clone("h_black");
  h_black->SetMarkerSize(0.8);
  // l.AddEntry(h_red,"TChiHH(1000,1) * 0.05","p");
  l.AddEntry(h_red,"T5HH(1600,1)","p");
  l.AddEntry(h_black,"Data","p");
  l.Draw("same");

  TLatex ltitle_prelim(c.GetLeftMargin()+0.02, 1.-0.5*c.GetTopMargin()-0.015, "#font[62]{CMS}#scale[0.76]{#font[52]{ Preliminary}}");
  TLatex ltitle(c.GetLeftMargin()+0.02, 1.-0.5*c.GetTopMargin()-0.015, "#font[62]{CMS}");
  TLatex rtitle(1.-c.GetRightMargin(), 1.-0.5*c.GetTopMargin()-0.015, "#scale[0.8]{137 fb^{-1} (13 TeV)}");
  rtitle.SetTextFont(42);
  ltitle.SetNDC(); ltitle_prelim.SetNDC(); rtitle.SetNDC();
  ltitle.SetTextAlign(12); ltitle_prelim.SetTextAlign(12); rtitle.SetTextAlign(32);
  if (addPrelim) ltitle_prelim.Draw("same");
  else  ltitle.Draw("same");
  rtitle.Draw("same");


  TLine *line1 = new TLine(95,95,145,95);
  TLine *line2 = new TLine(95,145,145,145);
  TLine *line3 = new TLine(95,95,95,145);
  TLine *line4 = new TLine(145,95,145,145);

  line1->SetLineColor(kBlack); line1->SetLineStyle(1);
  line2->SetLineColor(kBlack); line2->SetLineStyle(1);
  line3->SetLineColor(kBlack); line3->SetLineStyle(1);
  line4->SetLineColor(kBlack); line4->SetLineStyle(1);
  line1->Draw("same"); line2->Draw("same");
  line3->Draw("same"); line4->Draw("same");
  gPad->RedrawAxis();
  // c.SaveAs("j1vj2M_baseline"+prelimString+".png");

  ////////////////////////////////////
  gStyle->SetTextFont(52);

  // f1->cd();
  TH2F* j1vj2M_0H_ZJets = (TH2F*)f1->Get("j1vj2M_0H_ZJets");
  TH2F* j1vj2M_0H_WJets = (TH2F*)f1->Get("j1vj2M_0H_WJets");
  TH2F* j1vj2M_0H_SnglT = (TH2F*)f1->Get("j1vj2M_0H_SnglT");
  TH2F* j1vj2M_0H_TT = (TH2F*)f1->Get("j1vj2M_0H_TT");
  TH2F* j1vj2M_0H_QCD = (TH2F*)f1->Get("j1vj2M_0H_QCD");

  TH2F *h_0H_sum = (TH2F*)j1vj2M_0H_ZJets->Clone("h_0H_sum");
  h_0H_sum->Add(j1vj2M_0H_WJets);
  h_0H_sum->Add(j1vj2M_0H_SnglT);
  h_0H_sum->Add(j1vj2M_0H_TT);
  h_0H_sum->Add(j1vj2M_0H_QCD);

  h_0H_sum->RebinX(8); h_0H_sum->RebinY(8);
  h_0H_sum->SetTitle(";m_{J} (leading jet) [GeV]; m_{J} (subleading jet) [GeV]; SM background [Events]");
  h_0H_sum->GetZaxis()->SetTitleOffset(1.0);
  h_0H_sum->GetXaxis()->SetTitleSize(0.04);
  h_0H_sum->GetYaxis()->SetTitleSize(0.04);
  h_0H_sum->GetZaxis()->SetTitleSize(0.04);

  h_0H_sum->GetXaxis()->SetLabelSize(0.03);
  h_0H_sum->GetYaxis()->SetLabelSize(0.03);
  h_0H_sum->GetZaxis()->SetLabelSize(0.03);

  h_0H_sum->SetStats(0);
  h_0H_sum->SetContour(nb);
  h_0H_sum->Draw("colz");

  // f2->cd();
  TH2F* j1vj2M_0H_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_0H_T5HH1600_LSP1");
  j1vj2M_0H_TChiHH500_LSP1->SetMarkerColor(kRed);  j1vj2M_0H_TChiHH500_LSP1->SetMarkerStyle(20); j1vj2M_0H_TChiHH500_LSP1->SetMarkerSize(0.3);
  j1vj2M_0H_TChiHH500_LSP1->Draw("same");
  // f0->cd();
  TH2F* j1vj2M_0H_data = (TH2F*)f0->Get("j1vj2M_0H_data");
  j1vj2M_0H_data->SetMarkerStyle(21); j1vj2M_0H_data->SetMarkerSize(0.5); j1vj2M_0H_data->SetMarkerColor(kBlack);
  j1vj2M_0H_data->Draw("same");

  l.Draw("same");
  if (addPrelim) ltitle_prelim.Draw("same");
  else  ltitle.Draw("same");
  rtitle.Draw("same");
  TLatex regionLabel0H(0.508, 0.78, "0H");
  regionLabel0H.SetNDC(); regionLabel0H.SetTextAlign(12); regionLabel0H.SetTextFont(42); regionLabel0H.SetTextSize(0.06);
  regionLabel0H.Draw("same");

  line1->Draw("same"); line2->Draw("same");
  line3->Draw("same"); line4->Draw("same");
  gPad->RedrawAxis();
  c.SaveAs("j1vj2M_0H"+prelimString+".png");
  c.SaveAs("j1vj2M_0H"+prelimString+".pdf","PDF");


  ////////////////////////////////////
  gStyle->SetTextFont(52);

  // f1->cd();
  TH2F* j1vj2M_1H_ZJets = (TH2F*)f1->Get("j1vj2M_1H_ZJets");
  TH2F* j1vj2M_1H_WJets = (TH2F*)f1->Get("j1vj2M_1H_WJets");
  TH2F* j1vj2M_1H_SnglT = (TH2F*)f1->Get("j1vj2M_1H_SnglT");
  TH2F* j1vj2M_1H_TT = (TH2F*)f1->Get("j1vj2M_1H_TT");
  TH2F* j1vj2M_1H_QCD = (TH2F*)f1->Get("j1vj2M_1H_QCD");

  TH2F *h_1H_sum = (TH2F*)j1vj2M_1H_ZJets->Clone("h_1H_sum");
  h_1H_sum->Add(j1vj2M_1H_WJets);
  h_1H_sum->Add(j1vj2M_1H_SnglT);
  h_1H_sum->Add(j1vj2M_1H_TT);
  h_1H_sum->Add(j1vj2M_1H_QCD);

  h_1H_sum->RebinX(8); h_1H_sum->RebinY(8);
  h_1H_sum->SetTitle(";m_{J} (leading jet) [GeV]; m_{J} (subleading jet) [GeV]; SM background [Events]");
  h_1H_sum->GetZaxis()->SetTitleOffset(1.0);
  h_1H_sum->GetXaxis()->SetTitleSize(0.04);
  h_1H_sum->GetYaxis()->SetTitleSize(0.04);
  h_1H_sum->GetZaxis()->SetTitleSize(0.04);

  h_1H_sum->GetXaxis()->SetLabelSize(0.03);
  h_1H_sum->GetYaxis()->SetLabelSize(0.03);
  h_1H_sum->GetZaxis()->SetLabelSize(0.03);

  h_1H_sum->SetStats(0);
  h_1H_sum->SetContour(nb);
  h_1H_sum->Draw("colz");

  // f2->cd();
  TH2F* j1vj2M_1H_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_1H_T5HH1600_LSP1");
  j1vj2M_1H_TChiHH500_LSP1->SetMarkerColor(kRed);  j1vj2M_1H_TChiHH500_LSP1->SetMarkerStyle(20); j1vj2M_1H_TChiHH500_LSP1->SetMarkerSize(0.3);
  j1vj2M_1H_TChiHH500_LSP1->Draw("same");
  // f0->cd();
  TH2F* j1vj2M_1H_data = (TH2F*)f0->Get("j1vj2M_1H_data");
  j1vj2M_1H_data->SetMarkerStyle(21); j1vj2M_1H_data->SetMarkerSize(0.5); j1vj2M_1H_data->SetMarkerColor(1);
  j1vj2M_1H_data->Draw("same");

  l.Draw("same");
  if (addPrelim) ltitle_prelim.Draw("same");
  else  ltitle.Draw("same");
  rtitle.Draw("same");

  TLatex regionLabel1H(0.508, 0.78, "1H");
  regionLabel1H.SetNDC(); regionLabel1H.SetTextAlign(12); regionLabel1H.SetTextFont(42); regionLabel1H.SetTextSize(0.06);
  regionLabel1H.Draw("same");

  line1->Draw("same"); line2->Draw("same");
  line3->Draw("same"); line4->Draw("same");
  gPad->RedrawAxis();
  c.SaveAs("j1vj2M_1H"+prelimString+".png");
  c.Print("j1vj2M_1H"+prelimString+".pdf","PDF");

  return;
  ////////////////////////////////////
  gStyle->SetTextFont(52);
  // f1->cd();
  TH2F* j1vj2M_2H_ZJets = (TH2F*)f1->Get("j1vj2M_2H_ZJets");
  TH2F* j1vj2M_2H_WJets = (TH2F*)f1->Get("j1vj2M_2H_WJets");
  TH2F* j1vj2M_2H_SnglT = (TH2F*)f1->Get("j1vj2M_2H_SnglT");
  TH2F* j1vj2M_2H_TT = (TH2F*)f1->Get("j1vj2M_2H_TT");
  TH2F* j1vj2M_2H_QCD = (TH2F*)f1->Get("j1vj2M_2H_QCD");
  TH2F *h_2H_sum = (TH2F*)j1vj2M_2H_ZJets->Clone("h_2H_sum");
  h_2H_sum->Add(j1vj2M_2H_WJets);
  h_2H_sum->Add(j1vj2M_2H_SnglT);
  h_2H_sum->Add(j1vj2M_2H_TT);
  h_2H_sum->Add(j1vj2M_2H_QCD);

  h_2H_sum->RebinX(8); h_2H_sum->RebinY(8);
  h_2H_sum->SetTitle(";m_{J} (leading jet) [GeV]; m_{J} (subleading jet) [GeV]; SM background [Events]");
  h_2H_sum->GetZaxis()->SetTitleOffset(1.0);
  h_2H_sum->GetXaxis()->SetTitleSize(0.04);
  h_2H_sum->GetYaxis()->SetTitleSize(0.04);
  h_2H_sum->GetZaxis()->SetTitleSize(0.04);

  h_2H_sum->GetXaxis()->SetLabelSize(0.03);
  h_2H_sum->GetYaxis()->SetLabelSize(0.03);
  h_2H_sum->GetZaxis()->SetLabelSize(0.03);

  h_2H_sum->SetStats(0);
  h_2H_sum->SetContour(nb);
  h_2H_sum->Draw("colz");

  // f2->cd();
  TH2F* j1vj2M_2H_TChiHH500_LSP1 = (TH2F*)f2->Get("j1vj2M_2H_T5HH1600_LSP1");
  j1vj2M_2H_TChiHH500_LSP1->SetMarkerColor(kRed);  j1vj2M_2H_TChiHH500_LSP1->SetMarkerStyle(20); j1vj2M_2H_TChiHH500_LSP1->SetMarkerSize(0.3);
  j1vj2M_2H_TChiHH500_LSP1->Draw("same");
  // f0->cd();
  TH2F* j1vj2M_2H_data = (TH2F*)f0->Get("j1vj2M_2H_data");
  j1vj2M_2H_data->SetMarkerStyle(21); j1vj2M_2H_data->SetMarkerSize(0.5); j1vj2M_2H_data->SetMarkerColor(1);
  j1vj2M_2H_data->Draw("same");

  l.Draw("same");
  if (addPrelim) ltitle_prelim.Draw("same");
  else  ltitle.Draw("same");
  rtitle.Draw("same");

  TLatex regionLabel2H(0.508, 0.78, "2H");
  regionLabel2H.SetNDC(); regionLabel2H.SetTextAlign(12); regionLabel2H.SetTextFont(42); regionLabel2H.SetTextSize(0.06);
  regionLabel2H.Draw("same");

  line1->Draw("same"); line2->Draw("same");
  line3->Draw("same"); line4->Draw("same");
  gPad->RedrawAxis();
  c.SaveAs("j1vj2M_2H"+prelimString+".png");
}
