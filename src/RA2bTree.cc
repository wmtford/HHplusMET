#ifndef RA2bTree_cxx
#define RA2bTree_cxx
#include "RA2bTree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void RA2bTree::Loop() {
   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
   }
}
#endif
