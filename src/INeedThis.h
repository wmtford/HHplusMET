//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 11 12:42:38 2018 by ROOT version 6.06/09
// from TTree newtree/newtree
// found on file: output.root
//////////////////////////////////////////////////////////

#ifndef INeedThis_h
#define INeedThis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class INeedThis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           BTags;
   Int_t           BTagsL;
   Int_t           BTagsM;
   Int_t           BTagsT;
   Double_t        deltaR_max;
   Float_t         HT;
   Float_t         MET;
   Int_t           nGenZs;
   Int_t           nGenHs;
   vector<double>  *Jets_bDiscriminatorCSV;
   Double_t        Weight;
   Int_t           nAK8;
   Int_t           nAK8_belowB;
   vector<double>  *AK8_doubleBDiscriminator;
   Int_t           nAK4;
   Int_t           nAK4Cuts;
   Int_t           nBoostedH;
   vector<double>  *boostedHCan_mass;
   vector<double>  *boostedHCan_pt;
   Int_t           nResolvedH;
   Int_t           nNotIsoResolvedH;
   vector<double>  *resolvedHCan_mass;
   Double_t        HiggsCan_AvgMass;
   Double_t        HiggsCan_MassDiff;
   Double_t        CSV_leading;
   Double_t        CSV_subleading;
   Double_t        CSV_3leading;
   Double_t        CSV_4leading;
   Double_t        deltaMAK8;
   Int_t           nIsoBNotResolved;

   // List of branches
   TBranch        *b_BTags;   //!
   TBranch        *b_BTagsL;   //!
   TBranch        *b_BTagsM;   //!
   TBranch        *b_BTagsT;   //!
   TBranch        *b_deltaR_max;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_nGenZs;   //!
   TBranch        *b_nGenHs;   //!
   TBranch        *b_Jets_bDiscriminatorCSV;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_nAK8;   //!
   TBranch        *b_nAK8_belowB;   //!
   TBranch        *b_AK8_doubleBDiscriminator;   //!
   TBranch        *b_nAK4;   //!
   TBranch        *b_nAK4Cuts;   //!
   TBranch        *b_nBoostedH;   //!
   TBranch        *b_boostedHCan_mass;   //!
   TBranch        *b_boostedHCan_pt;   //!
   TBranch        *b_nResolvedH;   //!
   TBranch        *b_nNotIsoResolvedH;   //!
   TBranch        *b_resolvedHCan_mass;   //!
   TBranch        *b_HiggsCan_AvgMass;   //!
   TBranch        *b_HiggsCan_MassDiff;   //!
   TBranch        *b_CSV_leading;   //!
   TBranch        *b_CSV_subleading;   //!
   TBranch        *b_CSV_3leading;   //!
   TBranch        *b_CSV_4leading;   //!
   TBranch        *b_deltaMAK8;   //!
   TBranch        *b_nIsoBNotResolved;   //!

   INeedThis(TTree *tree=0);
   virtual ~INeedThis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual vector<TH1F*>     Loop(float EventWeight, string file,bool save_files);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef INeedThis_cxx
INeedThis::INeedThis(TTree *tree) : fChain(0) {
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("output.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("output.root");
      }
      f->GetObject("QCD",tree);

   }
   Init(tree);
}

INeedThis::~INeedThis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t INeedThis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t INeedThis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void INeedThis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   Jets_bDiscriminatorCSV = 0;
   AK8_doubleBDiscriminator = 0;
   boostedHCan_mass = 0;
   boostedHCan_pt = 0;
   resolvedHCan_mass = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
   fChain->SetBranchAddress("BTagsL", &BTagsL, &b_BTagsL);
   fChain->SetBranchAddress("BTagsM", &BTagsM, &b_BTagsM);
   fChain->SetBranchAddress("BTagsT", &BTagsT, &b_BTagsT);
   fChain->SetBranchAddress("deltaR_max", &deltaR_max, &b_deltaR_max);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("nGenZs", &nGenZs, &b_nGenZs);
   fChain->SetBranchAddress("nGenHs", &nGenHs, &b_nGenHs);
   fChain->SetBranchAddress("Jets_bDiscriminatorCSV", &Jets_bDiscriminatorCSV, &b_Jets_bDiscriminatorCSV);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("nAK8", &nAK8, &b_nAK8);
   fChain->SetBranchAddress("nAK8_belowB", &nAK8_belowB, &b_nAK8_belowB);
   fChain->SetBranchAddress("AK8_doubleBDiscriminator", &AK8_doubleBDiscriminator, &b_AK8_doubleBDiscriminator);
   fChain->SetBranchAddress("nAK4", &nAK4, &b_nAK4);
   fChain->SetBranchAddress("nAK4Cuts", &nAK4Cuts, &b_nAK4Cuts);
   fChain->SetBranchAddress("nBoostedH", &nBoostedH, &b_nBoostedH);
   fChain->SetBranchAddress("boostedHCan_mass", &boostedHCan_mass, &b_boostedHCan_mass);
   fChain->SetBranchAddress("boostedHCan_pt", &boostedHCan_pt, &b_boostedHCan_pt);
   fChain->SetBranchAddress("nResolvedH", &nResolvedH, &b_nResolvedH);
   fChain->SetBranchAddress("nNotIsoResolvedH", &nNotIsoResolvedH, &b_nNotIsoResolvedH);
   fChain->SetBranchAddress("resolvedHCan_mass", &resolvedHCan_mass, &b_resolvedHCan_mass);
   fChain->SetBranchAddress("HiggsCan_AvgMass", &HiggsCan_AvgMass, &b_HiggsCan_AvgMass);
   fChain->SetBranchAddress("HiggsCan_MassDiff", &HiggsCan_MassDiff, &b_HiggsCan_MassDiff);
   fChain->SetBranchAddress("CSV_leading", &CSV_leading, &b_CSV_leading);
   fChain->SetBranchAddress("CSV_subleading", &CSV_subleading, &b_CSV_subleading);
   fChain->SetBranchAddress("CSV_3leading", &CSV_3leading, &b_CSV_3leading);
   fChain->SetBranchAddress("CSV_4leading", &CSV_4leading, &b_CSV_4leading);
   fChain->SetBranchAddress("deltaMAK8", &deltaMAK8, &b_deltaMAK8);
   fChain->SetBranchAddress("nIsoBNotResolved", &nIsoBNotResolved, &b_nIsoBNotResolved);
   Notify();
}

Bool_t INeedThis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void INeedThis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t INeedThis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef INeedThis_cxx
