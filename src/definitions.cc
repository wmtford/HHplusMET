#include <TLorentzVector.h>
#include <TEfficiency.h>
#include <TRandom3.h>
#include <TMath.h>
#include <TString.h>
#include <TChain.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TPad.h>
#include <bitset>
#include <vector>
#include <chrono>
#include <set>
#include <string>


using namespace std;


// constants
const double HmassWindowLow = 95.0;
const double HmassWindowHigh = 145.0;
const double baselineMassLow = 60.0;
const double baselineMassHigh = 260.0;
float eventMET = 0.0; float eventMET_phi = 0.0;
float eventHT = 0.0;
float jetPt1=1.0; float jetPt2=1.0;
double jetMass1 = 1.0; double jetMass2 = 1.0;
float thisDeltaPhi1=10.0; float thisDeltaPhi2=10.0; float thisDeltaPhi3=10.0; float thisDeltaPhi4=10.0;
int numIsoMuonTracks = 0; int numIsoElectronTracks = 0; int numIsoPionTracks = 0;
set<string> vYearRunLumiEvt;
TString thisYear; TString NLSPmass; TString LSPmass;

TString whichJEC = "none";
bool do3PerCorr = true;

// ==============================================
void setYear(TString Year) {thisYear = Year; thisYear.Remove(0,2);}
void setMasses(TString thisNLSPmass,TString thisLSPmass) {NLSPmass = thisNLSPmass; LSPmass = thisLSPmass;}
template<typename ntupleType> double deepBBTagCut(ntupleType* ntuple) {return 0.7;}
template<typename ntupleType> bool alwaysTrue(ntupleType* ntuple) {return true;}

double quadSum(double x, double y) {return TMath::Sqrt(TMath::Power(x,2) + TMath::Power(y,2));};
double quadSum(double x, double y, double z) {return TMath::Sqrt(TMath::Power(x,2) + TMath::Power(y,2) + TMath::Power(z,2));};

std::vector<std::string> split(const std::string& s, char delimiter) {
  std::vector<std::string> tokens;
  std::string token;
  std::istringstream tokenStream(s);
  while (std::getline(tokenStream, token, delimiter)) { tokens.push_back(token);}
  return tokens;
}

double CalcdPhi(double phi1 , double phi2) {
  double dPhi = phi1-phi2;
  if (dPhi < -TMath::Pi()) dPhi += 2*TMath::Pi();
  if (dPhi > TMath::Pi()) dPhi -= 2*TMath::Pi();
  return fabs(dPhi);
}

double CalcDeltaR(double eta1, double phi1, double eta2, double phi2) {
  double deta = eta1 - eta2;
  double dphi = phi1-phi2;
  if (dphi < -TMath::Pi()) dphi += 2*TMath::Pi();
  if (dphi > TMath::Pi()) dphi -= 2*TMath::Pi();
  return sqrt(deta*deta + dphi*dphi);
}

template<typename ntupleType>void ntupleBranchStatus(ntupleType* ntuple) {
  ntuple->fChain->SetBranchStatus("*",0);
  ntuple->fChain->SetBranchStatus("RunNum",1);
  ntuple->fChain->SetBranchStatus("EvtNum",1);
  ntuple->fChain->SetBranchStatus("LumiBlockNum",1);
  ntuple->fChain->SetBranchStatus("Muons*",1);
  ntuple->fChain->SetBranchStatus("Electrons*",1);
  ntuple->fChain->SetBranchStatus("NMuons",1);
  ntuple->fChain->SetBranchStatus("NElectrons",1);
  ntuple->fChain->SetBranchStatus("isoElectronTracks",1);
  ntuple->fChain->SetBranchStatus("isoMuonTracks",1);
  ntuple->fChain->SetBranchStatus("isoPionTracks",1);
  ntuple->fChain->SetBranchStatus("Photons*",1);
  ntuple->fChain->SetBranchStatus("DeltaPhi*",1);
  ntuple->fChain->SetBranchStatus("MHT",1);
  ntuple->fChain->SetBranchStatus("HT",1);
  ntuple->fChain->SetBranchStatus("NJets",1);
  ntuple->fChain->SetBranchStatus("BTags",1);
  ntuple->fChain->SetBranchStatus("MET",1);
  ntuple->fChain->SetBranchStatus("METPhi",1);
  ntuple->fChain->SetBranchStatus("NJetsISR",1);
  ntuple->fChain->SetBranchStatus("JetsAK8*",1);
  ntuple->fChain->SetBranchStatus("Jets*",1);
  ntuple->fChain->SetBranchStatus("Weight",1);
  ntuple->fChain->SetBranchStatus("TrueNumInteractions",1);
  ntuple->fChain->SetBranchStatus("TriggerPass",1);
  ntuple->fChain->SetBranchStatus("CaloMET",1);
  ntuple->fChain->SetBranchStatus("NVtx",1);
  ntuple->fChain->SetBranchStatus("NumInteractions",1);
  ntuple->fChain->SetBranchStatus("nAllVertices",1);
  ntuple->fChain->SetBranchStatus("JetID*",1);
  ntuple->fChain->SetBranchStatus("madHT",1);
  ntuple->fChain->SetBranchStatus("madMinDeltaRStatus",1);
  ntuple->fChain->SetBranchStatus("madMinPhotonDeltaR",1);
  ntuple->fChain->SetBranchStatus("GenParticles*",1);
  ntuple->fChain->SetBranchStatus("GenMET",1);
  ntuple->fChain->SetBranchStatus("GenMETPhi",1);
  ntuple->fChain->SetBranchStatus("*Filter",1);
  ntuple->fChain->SetBranchStatus("Jets_bJetTagDeepCSVBvsAll",1);
  ntuple->fChain->SetBranchStatus("puWeight",1);
  ntuple->fChain->SetBranchStatus("puSys*",1);
  ntuple->fChain->SetBranchStatus("GenParticles*",1);
  ntuple->fChain->SetBranchStatus("NonPrefiringProb*",1);
  ntuple->fChain->SetBranchStatus("JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb",1);
  ntuple->fChain->SetBranchStatus("JetsAK8_ID",1);
  ntuple->fChain->SetBranchStatus("ScaleWeights",1);
  ntuple->fChain->SetBranchStatus("TAP*",1);
}
/***************************************************************/
/* - - - - - - - - - - - - gen-level - - - - - - - - - - - - - */
/***************************************************************/

template<typename ntupleType> bool isB(ntupleType* ntuple, int jetIndex) {
  bool isMatched = false;
  float jet_eta = ntuple->Jets->at(jetIndex).Eta();
  float jet_phi = ntuple->Jets->at(jetIndex).Phi();
  for (unsigned int i=0 ; i < ntuple->GenParticles->size(); i++) {
    if (ntuple->GenParticles_PdgId->at(i) == 5) {
      double dr = CalcDeltaR(ntuple->GenParticles->at(i).Eta(),ntuple->GenParticles->at(i).Phi(),jet_eta,jet_phi);
      if (dr<0.4) {isMatched=true; break;}
    }
  } //end loop over gen particles
  return isMatched;
}

template<typename ntupleType> bool bothGenH(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2) return false;
  bool j1_gen = false;
  bool j2_gen = false;
  j1_gen = genHMatched(ntuple, ntuple->JetsAK8->at(0).Eta(), ntuple->JetsAK8->at(0).Phi());
  j2_gen = genHMatched(ntuple, ntuple->JetsAK8->at(1).Eta(), ntuple->JetsAK8->at(1).Phi());
  return (j1_gen&&j2_gen);
}

template<typename ntupleType> double bothJetsGen(ntupleType* ntuple) {
  bool j1_gen = false;
  bool j2_gen = false;
  j1_gen = genHMatched(ntuple, ntuple->JetsAK8->at(0).Eta(), ntuple->JetsAK8->at(0).Phi());
  j2_gen = genHMatched(ntuple, ntuple->JetsAK8->at(1).Eta(), ntuple->JetsAK8->at(1).Phi());
  if (j1_gen==false && j2_gen==false) return 0.5;
  else if (j1_gen==true && j2_gen==false) return 1.5;
  else if (j1_gen==false && j2_gen==true) return 1.5;
  else if (j1_gen==true && j2_gen==true) return 2.5;
}

template<typename ntupleType> bool genHMatched(ntupleType* ntuple, double jet_eta, double jet_phi) {
  bool isMatched = false;
  for (unsigned int i=0 ; i < ntuple->GenParticles->size(); i++) {
    if (ntuple->GenParticles_PdgId->at(i) == 25 &&
    ntuple->GenParticles_Status->at(i) == 22) {
      double dr = CalcDeltaR(ntuple->GenParticles->at(i).Eta(),ntuple->GenParticles->at(i).Phi(),jet_eta,jet_phi);
      if (dr<0.6) {isMatched=true; break;}
    }
  } //end loop over gen particles
  return isMatched;
}

template<typename ntupleType> bool genbbMatched(ntupleType* ntuple, double jet_eta, double jet_phi) {
  int numBsMatched = 0;
  for (unsigned int i=0 ; i < ntuple->GenParticles->size(); i++) {
    if (abs(ntuple->GenParticles_PdgId->at(i)) == 5) {
      double dr = CalcDeltaR(ntuple->GenParticles->at(i).Eta(),ntuple->GenParticles->at(i).Phi(),jet_eta,jet_phi);
      if (dr<0.8) numBsMatched++;
      // if (numBsMatched>1) break;
    }
  } //end loop over gen particles
  return (numBsMatched>1);
}

template<typename ntupleType> bool genHbbMatched(ntupleType* ntuple, double jet_eta, double jet_phi) {
  bool isH = genHMatched(ntuple, jet_eta, jet_phi);
  bool isbb =  genbbMatched(ntuple, jet_eta, jet_phi);
  return (isH && isbb);
}

template<typename ntupleType> bool gen4bs(ntupleType* ntuple) {
  int numHiggses=0; int numBs=0;
  for (unsigned int i=0 ; i < ntuple->GenParticles->size(); i++) {
    if (ntuple->GenParticles_PdgId->at(i) == 25 && ntuple->GenParticles_Status->at(i) == 22) numHiggses++;
    // if (ntuple->GenParticles_PdgId->at(i) == 25) numHiggses++;
    if (abs(ntuple->GenParticles_PdgId->at(i)) == 5) numBs++;
  }
  return (numHiggses==2 && numBs>3);
}

template<typename ntupleType> int getNumGenHiggses(ntupleType* ntuple) {
  int numHiggses=0;
  for (unsigned int i=0 ; i < ntuple->GenParticles->size(); i++) {
    if (ntuple->GenParticles_PdgId->at(i) == 25 &&
    ntuple->GenParticles_Status->at(i) == 22)
    numHiggses++;
  }
  return numHiggses;
}

template<typename ntupleType> int getNumGenZs(ntupleType* ntuple) {
  int numZs=0;
  for (unsigned int i=0 ; i < ntuple->GenParticles->size(); i++) {
    if (ntuple->GenParticles_PdgId->at(i) == 23 &&
    ntuple->GenParticles_ParentId->at(i) == 1000023 &&
    ntuple->GenParticles_Status->at(i) == 22)
    numZs++;
  }
  return numZs;
}

template<typename ntupleType> bool genLevelHHcut(ntupleType* ntuple) {
  int numHiggses=getNumGenHiggses(ntuple),numZs=getNumGenZs(ntuple);
  if (numHiggses==2 && numZs==0) return true;
  else return false;
}

template<typename ntupleType> bool genLevelZHcut(ntupleType* ntuple) {
  int numHiggses=getNumGenHiggses(ntuple),numZs=getNumGenZs(ntuple);
  if (numHiggses==1 && numZs==1) return true;
  else return false;
}

template<typename ntupleType> bool genLevelZZcut(ntupleType* ntuple) {
  int numHiggses=getNumGenHiggses(ntuple),numZs=getNumGenZs(ntuple);
  if (numHiggses==0 && numZs==2) return true;
  else return false;
}

//////////////////////
// Lepton functions //
//////////////////////
template<typename ntupleType> double computeMuonMT(ntupleType* ntuple) {
  if (ntuple->Muons->size() == 0) return -9999.;
  double lepPt = ntuple->Muons->at(0).Pt();
  double lepPhi = ntuple->Muons->at(0).Phi();
  double MET = eventMET;
  double METPhi = eventMET_phi;
  return sqrt(2*lepPt*MET * (1 - cos(METPhi-lepPhi)));
}

template<typename ntupleType> double computeElectronMT(ntupleType* ntuple) {
  if (ntuple->Electrons->size() == 0) return -9999.;
  double lepPt = ntuple->Electrons->at(0).Pt();
  double lepPhi = ntuple->Electrons->at(0).Phi();
  double MET = eventMET;
  double METPhi = eventMET_phi;
  return sqrt(2*lepPt*MET * (1 - cos(METPhi-lepPhi)));
}

////////////////////////////////////////////////////////////
// - - - - - - - - EVENT LEVEL VARIABLES - - - - - - - -  //
////////////////////////////////////////////////////////////
template<typename ntupleType> double fillJetPt1(ntupleType* ntuple) {
  if (ntuple->Jets->size() >= 1) return ntuple->Jets->at(0).Pt();
  else return -999.;
}

template<typename ntupleType> double fillJetPt2(ntupleType* ntuple) {
  if (ntuple->Jets->size() >= 2) return ntuple->Jets->at(1).Pt();
  else return -999.;
}

template<typename ntupleType> double fillJetPt3(ntupleType* ntuple) {
  if (ntuple->Jets->size() >= 3) return ntuple->Jets->at(2).Pt();
  else return -999.;
}

template<typename ntupleType> double fillJetPt4(ntupleType* ntuple) {
  if (ntuple->Jets->size() >= 4) return ntuple->Jets->at(3).Pt();
  else return -999.;
}

template<typename ntupleType> double fillNVtx(ntupleType* ntuple) {
  return ntuple->NVtx;
}

template<typename ntupleType> double fillnAllVertices(ntupleType* ntuple) {
  return ntuple->nAllVertices;
}

template<typename ntupleType> double fillNumInteractions(ntupleType* ntuple) {
  return ntuple->NumInteractions;
}

template<typename ntupleType> double fillMadHT(ntupleType* ntuple) {
  return ntuple->madHT;
}

template<typename ntupleType> double fillDeltaPhi1(ntupleType* ntuple) {
  return thisDeltaPhi1;
}

template<typename ntupleType> double fillDeltaPhi2(ntupleType* ntuple) {
  return thisDeltaPhi2;
}

template<typename ntupleType> double fillDeltaPhi3(ntupleType* ntuple) {
  return thisDeltaPhi3;
}

template<typename ntupleType> double fillDeltaPhi4(ntupleType* ntuple) {
  return thisDeltaPhi4;
}


template<typename ntupleType> double fillHT(ntupleType* ntuple) {
  return eventHT;
}

template<typename ntupleType> double fillMHT(ntupleType* ntuple) {
  return ntuple->MHT;
}

template<typename ntupleType> double fillMET(ntupleType* ntuple) {
  return eventMET;
}

template<typename ntupleType> void setJetPT(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()>1) {
    jetPt1 = RemakeAK8Jets(ntuple,0); //returns jet pt
    jetPt2 = RemakeAK8Jets(ntuple,1);
  }
}

template<typename ntupleType> void setJetMass(ntupleType* ntuple, bool smear=false) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  bool isSignal = (filename.Contains("TChiHH")||filename.Contains("T5qqqqZH")||filename.Contains("T5qqqqHH"));
  bool doTheB = (isSignal && !filename.Contains("2016") && !filename.Contains("Summer16v3") && (filename.Contains("fast")||filename.Contains("Fast")) && (filename.Contains("MC")||filename.Contains("TChiHH")) && do3PerCorr);
  if (ntuple->JetsAK8->size()>0 && ntuple->JetsAK8_softDropMass->size()>0) {
    jetMass1 = ntuple->JetsAK8_softDropMass->at(0);
    if (doTheB) { //recalculate softdrop mass, reducing subjets by 3% only for 2017/2018 fast sim
      std::vector<TLorentzVector> theseSubjets1 = ntuple->JetsAK8_subjets->at(0);
      TLorentzVector AK8jet1_corr;
      for (unsigned int i=0;i<theseSubjets1.size();i++) {AK8jet1_corr+=theseSubjets1.at(i)*0.97;} //should be a loop through two subjets
      jetMass1 = AK8jet1_corr.M();
    } //if 2017 and 2018 signal
    if (smear && isSignal) {
      jetMass1 = jetMass1+2.6; //what we determined was a good smearing factor for signal syst
    }
  } //if at least 1 jet
  if (ntuple->JetsAK8->size()>1 && ntuple->JetsAK8_softDropMass->size()>1) {
    jetMass2 = ntuple->JetsAK8_softDropMass->at(1);
    if (doTheB) { //recalculate softdrop mass, reducing subjets by 3% only for 2017/2018 fast sim
      std::vector<TLorentzVector> theseSubjets2 = ntuple->JetsAK8_subjets->at(1);
      TLorentzVector AK8jet2_corr;
      for (unsigned int i=0;i<theseSubjets2.size();i++) {AK8jet2_corr+=theseSubjets2.at(i)*0.97;} //should be a loop through two subjets
      jetMass2 = AK8jet2_corr.M();
    } //if 2017 and 2018 signal
    if (smear && isSignal) { jetMass2 = jetMass2+2.6; }
  } //if 2 jets
}

template<typename ntupleType> double fillMETRatio(ntupleType* ntuple) {
  return eventMET/ntuple->MHT; //not recalculating MHT for jetID nor b-jet correction
}

template<typename ntupleType> double fillOne(ntupleType* ntuple) { return 1.; }

template<typename ntupleType> double fillNJets(ntupleType* ntuple) {
  return ntuple->NJets;
}

template<typename ntupleType> double fillBTags(ntupleType* ntuple) {
  return ntuple->BTags;
}

template<typename ntupleType> void setSignalMasses(ntupleType* ntuple, int region) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  TString justSample = filename;
  if (region==1 || region==2) {
    justSample.Remove(0,93);
    justSample.Remove(justSample.Length()-12,justSample.Length());
  }
  TString testThis = justSample;
  TObjArray * x = testThis.Tokenize("_");

  if (region==1 && filename.Contains("TChiHH_HToBB")) {
    NLSPmass = ((TObjString *)(x->At(4)))->String();
    LSPmass = "0";
  }
  else if (region==2 && filename.Contains("TChiHH_HToBB") && filename.Contains("2D")) {
    NLSPmass = ((TObjString *)(x->At(5)))->String();
    LSPmass = ((TObjString *)(x->At(6)))->String();
    if (LSPmass.EndsWith("3")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("5");
    }
    else if (LSPmass.EndsWith("8")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("0");
    }
  }
  else if (region==2 && filename.Contains("TChiHH_HToBB") && !filename.Contains("2D")) {
    NLSPmass = ((TObjString *)(x->At(4)))->String();
    LSPmass = "0";
  }
  else if (region==2 && filename.Contains("T5qqqqZH")) {
    NLSPmass = ((TObjString *)(x->At(3)))->String();
    LSPmass = ((TObjString *)(x->At(4)))->String();
    if (LSPmass.EndsWith("3")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("5");
    }
    else if (LSPmass.EndsWith("8")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("0");
    }
  }
  else if (region==2 && filename.Contains("T5qqqqHH")) {
    NLSPmass = ((TObjString *)(x->At(4)))->String();
    LSPmass = ((TObjString *)(x->At(5)))->String();
    if (LSPmass.EndsWith("3")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("5");
    }
    else if (LSPmass.EndsWith("8")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("0");
    }
    std::cout<<"Check masses: "<< NLSPmass<<", "<<LSPmass<<std::endl;
  }
  else if (region==1 && filename.Contains("T5qqqqZH") && !filename.Contains("fast") && !filename.Contains("Fast")) {
    NLSPmass = ((TObjString *)(x->At(2)))->String();
    LSPmass = ((TObjString *)(x->At(3)))->String();
    if (LSPmass.EndsWith("3")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("5");
    }
    else if (LSPmass.EndsWith("8")) {
      LSPmass.Remove(LSPmass.Length()-1,LSPmass.Length());
      LSPmass.Append("0");
    }
  }
}

template<typename ntupleType> void setMET(ntupleType* ntuple) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  bool useGen = false;
  if (useGen) {
    eventMET = ntuple->GenMET;
    eventMET_phi = ntuple->GenMETPhi;
    return;
  }

  if (filename.Contains("MC") || filename.Contains("TChiHH")) {
    bool doTheB = !filename.Contains("2016") && !filename.Contains("Summer16v3") && (filename.Contains("fast")||filename.Contains("Fast"));
    vector<TLorentzVector> JetsJECFriend(ntuple->Jets->size());
    TLorentzVector v_old, v_new;

    float thisMET = ntuple->MET; float thisMETphi = ntuple->METPhi;
    float sumPx = thisMET*cos(thisMETphi); float sumPy = thisMET*sin(thisMETphi);
    float et = sqrt(sumPx*sumPx + sumPy*sumPy);
    TLorentzVector v_MET;
    v_MET.SetPxPyPzE(sumPx, sumPy, 0, et);

    vector<int> newIndex;;
    newIndex.reserve(ntuple->Jets_origIndex->size());
    for (unsigned j = 0; j < ntuple->Jets_origIndex->size(); ++j) {
      newIndex[ntuple->Jets_origIndex->at(j)] = j;
    }

    //for 3% scaling down of ONLY b-jets, for 2017 and 2018
    if (whichJEC=="none" && doTheB) {
      for (unsigned j = 0; j < ntuple->JetsJECup_origIndex->size(); ++j) {
        int i = newIndex[ntuple->JetsJECup_origIndex->at(j)];
        v_old += ntuple->Jets->at(i);
        bool isThisAB = isB(ntuple,i) && do3PerCorr;
        if (isThisAB) {JetsJECFriend[j] = ntuple->Jets->at(i)*0.97;}
        else JetsJECFriend[j] = ntuple->Jets->at(i);
        v_new += JetsJECFriend[j];
      }
    }

    //JEC up
    else if (whichJEC=="JECUp") {
      for (unsigned j = 0; j < ntuple->JetsJECup_origIndex->size(); ++j) {
        int i = newIndex[ntuple->JetsJECup_origIndex->at(j)];
        v_old += ntuple->Jets->at(i);
        if (doTheB) {
          bool isThisAB = isB(ntuple,i) && do3PerCorr;
          if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1+ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECup_jerFactor->at(j)*0.97;
          else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1+ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECup_jerFactor->at(j);
        }
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1+ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECup_jerFactor->at(j);
        v_new += JetsJECFriend[j];
      }
    }

    //JEC down
    else if (whichJEC=="JECDown") {
      for (unsigned j = 0; j < ntuple->JetsJECdown_origIndex->size(); ++j) {
        int i = newIndex[ntuple->JetsJECdown_origIndex->at(j)];
        v_old += ntuple->Jets->at(i);
        if (doTheB) {
          bool isThisAB = isB(ntuple,i) && do3PerCorr;
          if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1-ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECdown_jerFactor->at(j)*0.97;
          else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1-ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECdown_jerFactor->at(j);
        }
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1-ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECdown_jerFactor->at(j);
        v_new += JetsJECFriend[j];
      }
    }

    //JER up
    else if (whichJEC=="JERUp") {
      for (unsigned j = 0; j < ntuple->JetsJERup_origIndex->size(); ++j) {
        int i = newIndex[ntuple->JetsJERup_origIndex->at(j)];
        v_old += ntuple->Jets->at(i);
        if (doTheB) {
          bool isThisAB = isB(ntuple,i) && do3PerCorr;
          if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorUp->at(i)*0.97;
          else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorUp->at(i);
        }
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorUp->at(i);
        v_new += JetsJECFriend[j];
      }
    }

    //JER down
    else if (whichJEC=="JERDown") {
      for (unsigned j = 0; j < ntuple->JetsJERdown_origIndex->size(); ++j) {
        int i = newIndex[ntuple->JetsJERdown_origIndex->at(j)];
        v_old += ntuple->Jets->at(i);
        if (doTheB) {
          bool isThisAB = isB(ntuple,i) && do3PerCorr;
          if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorDown->at(i)*0.97;
          else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorDown->at(i);
        }
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorDown->at(i);
        v_new += JetsJECFriend[j];
      }
    }
    v_MET = v_MET + v_old - v_new;
    eventMET = v_MET.Pt();
    eventMET_phi = v_MET.Phi();
  } //end MC only
  else {
    eventMET = ntuple->MET;
    eventMET_phi = ntuple->METPhi;
  }
}

template<typename ntupleType> vector<TLorentzVector> RemakeAK4Jets(ntupleType* ntuple) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  bool doTheB = !filename.Contains("2016") && !filename.Contains("Summer16v3") && (filename.Contains("fast")||filename.Contains("Fast"));
  vector<TLorentzVector> JetsJECFriend(ntuple->Jets->size());
  if (!filename.Contains("MC") && !filename.Contains("TChiHH")) {
    for (unsigned j = 0; j < ntuple->Jets->size(); ++j) {
      JetsJECFriend[j] = ntuple->Jets->at(j);
    }
    return JetsJECFriend;
  }

  vector<int> newIndex;;
  newIndex.reserve(ntuple->Jets_origIndex->size());
  for (unsigned j = 0; j < ntuple->Jets_origIndex->size(); ++j) {
    newIndex[ntuple->Jets_origIndex->at(j)] = j;
  }

  //for 3% scaling down of ONLY b-jets, 2017 and 2018 fast sim only
  if (whichJEC=="none" && doTheB) {
    for (unsigned j = 0; j < ntuple->JetsJECup_origIndex->size(); ++j) {
      int i = newIndex[ntuple->JetsJECup_origIndex->at(j)];
      bool isThisAB = isB(ntuple,i) && do3PerCorr;
      if (isThisAB) {JetsJECFriend[j] = ntuple->Jets->at(i)*0.97;}
      else JetsJECFriend[j] = ntuple->Jets->at(i);
    }
  }

  //JEC up
  else if (whichJEC=="JECUp") {
    for (unsigned j = 0; j < ntuple->JetsJECup_origIndex->size(); ++j) {
      int i = newIndex[ntuple->JetsJECup_origIndex->at(j)];
      if (doTheB) {
        bool isThisAB = isB(ntuple,i) && do3PerCorr;
        if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1+ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECup_jerFactor->at(j)*0.97;
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1+ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECup_jerFactor->at(j);
      }
      else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1+ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECup_jerFactor->at(j);
    }
  }

  //JEC down
  else if (whichJEC=="JECDown") {
    for (unsigned j = 0; j < ntuple->JetsJECdown_origIndex->size(); ++j) {
      int i = newIndex[ntuple->JetsJECdown_origIndex->at(j)];
      if (doTheB) {
        bool isThisAB = isB(ntuple,i) && do3PerCorr;
        if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1-ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECdown_jerFactor->at(j)*0.97;
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1-ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECdown_jerFactor->at(j);
      }
      else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*(1-ntuple->Jets_jecUnc->at(i))*ntuple->JetsJECdown_jerFactor->at(j);
    }
  }

  //JER up
  else if (whichJEC=="JERUp") {
    for (unsigned j = 0; j < ntuple->JetsJERup_origIndex->size(); ++j) {
      int i = newIndex[ntuple->JetsJERup_origIndex->at(j)];
      if (doTheB) {
        bool isThisAB = isB(ntuple,i) && do3PerCorr;
        if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorUp->at(i)*0.97;
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorUp->at(i);
      }
      else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorUp->at(i);
    }
  }

  //JER down
  else if (whichJEC=="JERDown") {
    for (unsigned j = 0; j < ntuple->JetsJERdown_origIndex->size(); ++j) {
      int i = newIndex[ntuple->JetsJERdown_origIndex->at(j)];
      if (doTheB) {
        bool isThisAB = isB(ntuple,i) && do3PerCorr;
        if (isThisAB) JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorDown->at(i)*0.97;
        else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorDown->at(i);
      }
      else JetsJECFriend[j] = ntuple->Jets->at(i)*(1./ntuple->Jets_jerFactor->at(i))*ntuple->Jets_jerFactorDown->at(i);
    }
  }

  else {   //2016 fast sim signal, or for no JEC and FullSIM signal
    for (unsigned j = 0; j < ntuple->Jets->size(); ++j) {
      JetsJECFriend[j] = ntuple->Jets->at(j);
    }
  }
	return JetsJECFriend;
}

template<typename ntupleType> vector<TLorentzVector> newJets_JetID(ntupleType* ntuple) {
  vector<TLorentzVector> newJets;
  vector<TLorentzVector> ak4JECJets = RemakeAK4Jets(ntuple); //assume indices are the same...
  for (unsigned int j=0; j<ak4JECJets.size();++j) {
    if (ak4JECJets.at(j).Pt()<30 || fabs(ak4JECJets.at(j).Eta())>2.4 || ntuple->Jets_ID->at(j)!=1) continue;
    newJets.push_back(ak4JECJets.at(j));
  }
  return newJets;
}

template<typename ntupleType> int numOverlapBs(ntupleType* ntuple, int whichAK8) {
  int numBs = 0;
  float AK8_eta = ntuple->JetsAK8->at(whichAK8).Eta();
  float AK8_phi = ntuple->JetsAK8->at(whichAK8).Phi();

  TString filename = ntuple->fChain->GetFile()->GetName();
  double CSVBtagMed   = 0.6321; //2016
  if (filename.Contains("2017") || filename.Contains("Fall17")) CSVBtagMed = 0.4941;
  else if (filename.Contains("2018") || filename.Contains("Autumn18")) CSVBtagMed = 0.4184;

  for (unsigned int j=0; j<ntuple->Jets->size();++j) {
    if (ntuple->Jets_bJetTagDeepCSVBvsAll->at(j)<CSVBtagMed) continue;
    float deltaEta = AK8_eta - ntuple->Jets->at(j).Eta();
    float deltaPhi = CalcdPhi(AK8_phi , ntuple->Jets->at(j).Phi());
    float deltaR = sqrt((deltaEta*deltaEta)+(deltaPhi*deltaPhi));
    if (deltaR<0.8) numBs++;
  }
  return numBs;
}

//for jetID and b-jet 3% correction
template<typename ntupleType> void setHT(ntupleType* ntuple, TString thisJEC) {
  if (thisJEC!="none") whichJEC = thisJEC; //sets for signal systematics
  vector<TLorentzVector> newJets = newJets_JetID(ntuple);
  float newHT = 0;
  for (unsigned int j=0; j<newJets.size();++j) {
    newHT += newJets.at(j).Pt();
  }
  eventHT = newHT;
}

template<typename ntupleType> void setIsoTracks(ntupleType* ntuple) {
  //if running from ntuples
  int isoMuonCounter = 0;
  int isoElectronCounter = 0;
  int isoPionCounter = 0;

  //Muons first, branches: TAPMuonTracks, TAPMuonTracks_dxypv, TAPMuonTracks_leptonMatch, TAPMuonTracks_mT, TAPMuonTracks_pfRelIso03chg, TAPMuonTracks_trkiso
  for(unsigned im = 0; im < ntuple->TAPMuonTracks->size(); ++im) {  //dz and pdgid cuts in place at ntuple creation
    if (ntuple->TAPMuonTracks->at(im).Pt()<=5.0 || fabs(ntuple->TAPMuonTracks->at(im).Eta())>2.5 || ntuple->TAPMuonTracks_dxypv->at(im)>=0.2 || ntuple->TAPMuonTracks_mT->at(im)>100.0) continue;
    bool isIso = false;
    isIso = (
      (ntuple->TAPMuonTracks->at(im).Pt()<25.0 && ntuple->TAPMuonTracks_pfRelIso03chg->at(im)*ntuple->TAPMuonTracks->at(im).Pt() < 5.0) ||
      (ntuple->TAPMuonTracks_pfRelIso03chg->at(im)<0.2)
    );
    if (isIso) isoMuonCounter++;
  }

  //Then electrons
  for(unsigned im = 0; im < ntuple->TAPElectronTracks->size(); ++im) {  //dz and pdgid cuts in place at ntuple creation
    if (ntuple->TAPElectronTracks->at(im).Pt()<=5.0 || fabs(ntuple->TAPElectronTracks->at(im).Eta())>2.5 || ntuple->TAPElectronTracks_dxypv->at(im)>=0.2 || ntuple->TAPElectronTracks_mT->at(im)>100.0) continue;
    bool isIso =  false;
    isIso = (
      (ntuple->TAPElectronTracks->at(im).Pt()<25.0 && ntuple->TAPElectronTracks_pfRelIso03chg->at(im)*ntuple->TAPElectronTracks->at(im).Pt() < 5.0) ||
      (ntuple->TAPElectronTracks_pfRelIso03chg->at(im)<0.2)
    );
    if (isIso) isoElectronCounter++;
  }

  //Lastly pions
  for(unsigned im = 0; im < ntuple->TAPPionTracks->size(); ++im) {  //dz and pdgid cuts in place at ntuple creation
    if (ntuple->TAPPionTracks->at(im).Pt()<=10.0 || fabs(ntuple->TAPPionTracks->at(im).Eta())>2.5 || ntuple->TAPPionTracks_dxypv->at(im)>=0.2 || ntuple->TAPPionTracks_mT->at(im)>100.0) continue;
    if (ntuple->TAPPionTracks_leptonMatch->at(im)!=0) continue;
    bool isIso =  false;
    isIso = (
      (ntuple->TAPPionTracks->at(im).Pt()<25.0 && ntuple->TAPPionTracks_pfRelIso03chg->at(im)*ntuple->TAPPionTracks->at(im).Pt() < 5.0) ||
      (ntuple->TAPPionTracks_pfRelIso03chg->at(im)<0.1)
    );
    if (isIso) isoPionCounter++;
  }

  numIsoMuonTracks = isoMuonCounter;
  numIsoElectronTracks = isoElectronCounter;
  numIsoPionTracks = isoPionCounter;
}

template<typename ntupleType> int numJets(ntupleType* ntuple) { //Returns the number jets within pT and eta
  int NJets = 0;
  vector<TLorentzVector> newJets = newJets_JetID(ntuple);
  NJets = newJets.size();
  return NJets;
}

//Using deepCSV, returns the number of b's based on loose, medium, and tight WPs
//2016: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation2016Legacy#Supported_Algorithms_and_Operati
//2017: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation94X
//2018: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation102X#Supported_Algorithms_and_Operati
template<typename ntupleType> std::vector<int> numDeepBs(ntupleType* ntuple) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  double CSVBtagLoose = 0.2217;
  double CSVBtagMed   = 0.6321;
  double CSVBtagTight = 0.8953;

  if (filename.Contains("2016") || filename.Contains("Summer16v3")) {
    CSVBtagLoose = 0.2217;
    CSVBtagMed   = 0.6321;
    CSVBtagTight = 0.8953;
  }
  else if (filename.Contains("2017") || filename.Contains("Fall17")) {
    CSVBtagLoose = 0.1522;
    CSVBtagMed   = 0.4941;
    CSVBtagTight = 0.8001;
  }
  else if (filename.Contains("2018") || filename.Contains("Autumn18")) {
    CSVBtagLoose = 0.1241;
    CSVBtagMed   = 0.4184;
    CSVBtagTight = 0.7527;
  }

  int BTagsL = 0; int BTagsM = 0; int BTagsT = 0;
  vector<TLorentzVector> ak4JECJets = RemakeAK4Jets(ntuple); //assume indices are the same...
  for (unsigned int j=0; j<ak4JECJets.size();++j) {
    if (ak4JECJets.at(j).Pt()<30 || fabs(ak4JECJets.at(j).Eta())>2.4 || ntuple->Jets_ID->at(j)!=1) continue;
    float this_CSV_value = ntuple->Jets_bJetTagDeepCSVBvsAll->at(j);
    if (this_CSV_value > CSVBtagTight) BTagsT++;
    if (this_CSV_value > CSVBtagMed) BTagsM++;
    if (this_CSV_value > CSVBtagLoose) BTagsL++;
  }
  std::vector<int> thisNBs = {BTagsL, BTagsM, BTagsT};
  return thisNBs;
}


////////////////////////////////
// HIGHEST PT JET PROPERTIES  //
////////////////////////////////
template<typename ntupleType> double fillLeadingJetMass(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()==0) return -99999.;
  return jetMass1;
}

template<typename ntupleType> double fillLeadingJetFlavor(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()==0) return -99999.;
  if (ntuple->JetsAK8_NumBhadrons->at(0)==2) return 21.;
  else if (ntuple->JetsAK8_NumBhadrons->at(0)==1) return 5.;
  else return 1.;
}

template<typename ntupleType> double fillLeadingNbHadrons(ntupleType* ntuple) {
  return ntuple->JetsAK8->size()>=1?ntuple->JetsAK8_NumBhadrons->at(0):-999.;
}

template<typename ntupleType> double fillLeadingJetPt(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()==0) return-99999.;
  return jetPt1;
}

template<typename ntupleType> double fillLeadingdeepBBtag(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()==0) return-99999.;
  return ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0);
}

template<typename ntupleType> double fillLeadingTau21(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()==0) return-99999.;
  return ntuple->JetsAK8_NsubjettinessTau2->at(0)/ntuple->JetsAK8_NsubjettinessTau1->at(0);
}

//////////////////////////////////////////
// SECOND HIGHEST PT AK8 JET PROPERTIES //
//////////////////////////////////////////
template<typename ntupleType> double fillSubLeadingJetMass(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<=1) return-99999.;
  return jetMass2;
}

template<typename ntupleType> double fillSubLeadingJetFlavor(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<=1) return-99999.;
  if (ntuple->JetsAK8_NumBhadrons->at(1)==2) return 21.;
  else if (ntuple->JetsAK8_NumBhadrons->at(1)==1) return 5.;
  else return 1.;
}

template<typename ntupleType> double fillSubLeadingNbHadrons(ntupleType* ntuple) {
  return ntuple->JetsAK8->size()>=2?ntuple->JetsAK8_NumBhadrons->at(1):-999.;
}

template<typename ntupleType> double fillSubLeadingJetPt(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<=1) return -99999.;
  return jetPt2;
}

template<typename ntupleType> double fillSubLeadingdeepBBtag(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<=1) return-99999.;
  return ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1);
}

template<typename ntupleType> double fillSubLeadingTau21(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<=1) return-99999.;
  return ntuple->JetsAK8_NsubjettinessTau2->at(1)/ntuple->JetsAK8_NsubjettinessTau1->at(1);
}

template<typename ntupleType> bool isHighPU(ntupleType* ntuple) { // high is >35
  bool isHighPU = false;
  if (ntuple->NumInteractions>=35) isHighPU=true;
  return isHighPU;
}


/////////////////////////////
////////   Filters   ////////
/////////////////////////////
template<typename ntupleType> bool METRatioFilterCut(ntupleType* ntuple) {
  return (eventMET/ntuple->CaloMET<2.0);
}

template<typename ntupleType> bool METMHTFilterCut(ntupleType* ntuple) {
  return (eventMET/ntuple->MHT < 2.0);
}

template<typename ntupleType> bool HEMRegion(ntupleType* ntuple, bool isMC_) {
  if (!isMC_ && ntuple->RunNum < 319077) return false;
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (isMC_ && !sample.Contains("2018") && !sample.Contains("Autumn18")) return false;
  if (ntuple->EvtNum%1000 < 1000*21.0/59.6) return false;
  return true;
}

template<typename ntupleType> bool LowNeutralJetFilter(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;

  bool LowNeutralJetFilter = true;
  bool tight = false;

  if (ntuple->Jets_neutralEmEnergyFraction->empty()) LowNeutralJetFilter = true;
  else {
    LowNeutralJetFilter = !(ntuple->Jets_neutralEmEnergyFraction->at(0)<(tight ? 0.05 : 0.03) && abs(thisDeltaPhi1)>(TMath::Pi()-0.4));
  }
  return LowNeutralJetFilter;
}

template<typename ntupleType> bool HEMDPhiVetoFilter(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  bool passHEM = true;
  if (sample.Contains("SingleMuon") || sample.Contains("SingleElectron") || sample.Contains("SinglePhoton") || sample.Contains("EGamma") || sample.Contains("tree_MET_201")) {
    if (HEMRegion(ntuple, false) && ntuple->HEMDPhiVetoFilter!=1) passHEM=false;
  }
  else {
    if (HEMRegion(ntuple, true) && ntuple->HEMDPhiVetoFilter!=1) passHEM=false;
  }
  return passHEM;
}

template<typename ntupleType> bool HEMDPhiVetoFilterCutflow(ntupleType* ntuple) {
  //if running from ntuples and not skims
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  bool passHEM = true;
  bool isHEMRegion = true;

  if (!sample.Contains("MC2018")) isHEMRegion=false;
  if (ntuple->EvtNum%1000 < 1000*21.0/59.6) isHEMRegion=false;

  if (isHEMRegion) { //determine if we skip
    bool activity = false;
    for (unsigned e = 0; e < ntuple->Electrons->size(); ++e) {
      if (!ntuple->Electrons_passIso->at(e)) continue;
      const auto& Electron = ntuple->Electrons->at(e);
      bool inHEMRegion = false;
      if (Electron.Pt()>0.0 && Electron.Eta()>-3 && Electron.Eta()<-1.4 && Electron.Phi()>-1.57 && Electron.Phi()<-0.87) inHEMRegion=true;
      if (!activity && inHEMRegion) activity = true;
      if (activity) break;
    }
    //check jets
    for (const auto& Jet : *ntuple->Jets) {
      bool inHEMRegion = false;
      if (Jet.Pt()>30.0 && Jet.Eta()>-3 && Jet.Eta()<-1.4 && Jet.Phi()>-1.57 && Jet.Phi()<-0.87) inHEMRegion=true;
      if (!activity && inHEMRegion) activity = true;
      if (activity) break;
    }
    passHEM = !activity;
  } // in HEM time period
  return passHEM;
}

template<typename ntupleType> bool GoodVertexCutflow(ntupleType* ntuple) {
  return(ntuple->NVtx>0);
}

template<typename ntupleType> bool globalSuperTightHalo2016FilterCutflow(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  return(ntuple->globalSuperTightHalo2016Filter==1);
}

template<typename ntupleType> bool HBHENoiseFilterCutflow(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  return(ntuple->HBHENoiseFilter==1);
}

template<typename ntupleType> bool HBHEIsoNoiseFilterCutflow(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  return(ntuple->HBHEIsoNoiseFilter==1);
}

template<typename ntupleType> bool EcalDeadCellTriggerPrimitiveFilterCutflow(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  return(ntuple->EcalDeadCellTriggerPrimitiveFilter==1);
}

template<typename ntupleType> bool BadPFMuonFilterCutflow(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  return(ntuple->BadPFMuonFilter==1);
}

template<typename ntupleType> bool FakeJetFilter(ntupleType* ntuple) {
  bool noFakeJet = true;
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (!sample.Contains("fast") && !sample.Contains("Fast")) return(noFakeJet);
  //reject events with any jet pt>20, |eta|<2.5 NOT matched to a GenJet (w/in DeltaR<0.3) and chfrac < 0.1
  for (unsigned j = 0; j < ntuple->Jets->size(); ++j) {
    if (ntuple->Jets->at(j).Pt()<=20 || abs(ntuple->Jets->at(j).Eta())>=2.5) continue;
    bool genMatched = false;
    for (unsigned g = 0; g < ntuple->GenJets->size(); ++g) {
      float dr = CalcDeltaR(ntuple->GenJets->at(g).Eta(), ntuple->GenJets->at(g).Phi(), ntuple->Jets->at(j).Eta(), ntuple->Jets->at(j).Phi());
      if (dr<0.3) { genMatched=true; break;}
    }
    if (!genMatched && ntuple->Jets_chargedHadronEnergyFraction->at(j) < 0.1) {
      noFakeJet = false;
      break;
    }
  }
  return(noFakeJet);
}

template<typename ntupleType> bool MuonJetFilter(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;

  bool noMuonJet = true;
  //check for inconsistent case
  if (ntuple->Jets->size()!=ntuple->Jets_muonEnergyFraction->size()) {
    std::cout<<"Can't make MuonJetFilter myself"<<std::endl;
    return true;
  }
  for (unsigned j = 0; j < ntuple->Jets->size(); ++j) {
    if (ntuple->Jets->at(j).Pt() > 200 && ntuple->Jets_muonEnergyFraction->at(j) > 0.5 && abs(CalcdPhi(ntuple->Jets->at(j).Phi(),eventMET_phi)) > (TMath::Pi() - 0.4)) {
      noMuonJet = false;
      break;
    }
  }
  return(noMuonJet);
}

template<typename ntupleType> bool HTRatioDPhiTightFilter(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;
  double htratio = ntuple->HT5/eventHT;
  bool tight = true;
  //keep any event with ht5/ht < 1.2
  bool result = (htratio < 1.2 ? true : (thisDeltaPhi1 >= (tight ? 5.3*htratio - 4.78 : 1.025*htratio - 0.5875) ) );
  return(result);
}

template<typename ntupleType> bool EcalNoiseJetFilter(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) return true;

  int counter = 0;
  bool goodJet[2] = {true,true};
  for (unsigned j = 0; j < ntuple->Jets->size(); ++j) {
    if (counter>=2) break;
    const auto& Jet = ntuple->Jets->at(j);
    //MHT mask doesn't work for 2017 because of EE "fix", so check manually
    if (Jet.Pt()>30 and abs(Jet.Eta())<5.0 and abs(Jet.Eta())>2.4) {
      double dphi = abs(CalcdPhi(Jet.Phi(),eventMET_phi));
      if (Jet.Pt()>250 and (dphi > 2.6 || dphi < 0.1)) goodJet[counter] = false;
      ++counter;
    }
  }
  bool result = (goodJet[0] && goodJet[1]);
  return(result);
}

template<typename ntupleType> bool FiltersCut(ntupleType* ntuple) {
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) {
    return (
      ntuple->FakeJetFilter &&
      ntuple->NVtx>0 &&
      METRatioFilterCut(ntuple) && //apply to 0-lepton only and signal
      METMHTFilterCut(ntuple) //apply to 0-lepton only and signal
    );
  }
  return (
    HEMDPhiVetoFilter(ntuple) &&
    ntuple->NVtx>0 &&
    ntuple->globalSuperTightHalo2016Filter==1 &&
    ntuple->HBHENoiseFilter==1 &&
    ntuple->HBHEIsoNoiseFilter==1 &&
    ntuple->EcalDeadCellTriggerPrimitiveFilter == 1 &&
    ntuple->BadPFMuonFilter == 1 &&
    LowNeutralJetFilter(ntuple) && //I forgot to save in the skims, sorry
    ntuple->MuonJetFilter &&
    ntuple->HTRatioDPhiTightFilter==1 &&
    ntuple->EcalNoiseJetFilter==1
  );
}

template<typename ntupleType> bool FiltersCutflow(ntupleType* ntuple) {
  //if running from ntuples and not skims
  TString sample = ntuple->fChain->GetFile()->GetName();
  if (sample.Contains("fast") || sample.Contains("Fast")) {
    return (
      FakeJetFilter(ntuple) && // ntuple->FakeJetFilter
      GoodVertexCutflow(ntuple) &&
      METRatioFilterCut(ntuple) &&
      METMHTFilterCut(ntuple)
    );
  }
  else {
    return (
      HEMDPhiVetoFilterCutflow(ntuple) &&
      GoodVertexCutflow(ntuple) &&
      globalSuperTightHalo2016FilterCutflow(ntuple) &&
      HBHENoiseFilterCutflow(ntuple) &&
      HBHEIsoNoiseFilterCutflow(ntuple) &&
      EcalDeadCellTriggerPrimitiveFilterCutflow(ntuple) &&
      BadPFMuonFilterCutflow(ntuple) &&
      LowNeutralJetFilter(ntuple) &&
      MuonJetFilter(ntuple) &&
      HTRatioDPhiTightFilter(ntuple) &&
      EcalNoiseJetFilter(ntuple) &&
      METRatioFilterCut(ntuple) &&
      METMHTFilterCut(ntuple)
    );
  }
}


template<typename ntupleType> bool DeltaPhi1Cut(ntupleType* ntuple) {
  return thisDeltaPhi1>0.5;
}

template<typename ntupleType> bool DeltaPhi2Cut(ntupleType* ntuple) {
  return thisDeltaPhi2>0.5;
}

template<typename ntupleType> bool DeltaPhi3Cut(ntupleType* ntuple) {
  return thisDeltaPhi3>0.3;
}

template<typename ntupleType> bool DeltaPhi4Cut(ntupleType* ntuple) {
  return thisDeltaPhi4>0.3;
}

template<typename ntupleType> void setDeltaPhis(ntupleType* ntuple) {
  //Because jetId
  std::vector<double> deltaphi(4,10.);
  vector<TLorentzVector> newJets = newJets_JetID(ntuple);
  for (unsigned int i=0; i<newJets.size();i++) {
    deltaphi[i] = std::abs(CalcdPhi(newJets.at(i).Phi(),eventMET_phi));
    if (i==3) break;
  }
  thisDeltaPhi1 = deltaphi[0]; thisDeltaPhi2 = deltaphi[1];
  thisDeltaPhi3 = deltaphi[2]; thisDeltaPhi4 = deltaphi[3];
}

template<typename ntupleType> bool DeltaPhiCuts(ntupleType* ntuple) {
  return (thisDeltaPhi1>0.5 && thisDeltaPhi2>0.5 && thisDeltaPhi3>0.3 && thisDeltaPhi4>0.3);
}

template<typename ntupleType> bool lowDPhiCuts(ntupleType* ntuple) {
  return !DeltaPhiCuts(ntuple);
}

template<typename ntupleType> bool METHTlooseCut(ntupleType* ntuple) {
  return (eventMET > 100. && eventHT > 300.);
}

template<typename ntupleType> bool METHTCut(ntupleType* ntuple) {
  return (    eventMET > 300. && eventHT > 600. );
}
template<typename ntupleType> bool AK8MultCut(ntupleType* ntuple) {
  return (ntuple->JetsAK8->size() >= 2 && ntuple->JetsAK8_ID->at(0)==1 && ntuple->JetsAK8_ID->at(1)==1);
}

template<typename ntupleType> bool METTightCutflow(ntupleType* ntuple) { //Used for cutflow
  return (
    eventMET > 300.
  );
}

template<typename ntupleType> bool skimCuts(ntupleType* ntuple) { //Used for cutflow, run over ntuples
  return (
    cutflowNJets(ntuple) &&
    cutflowlooseHT(ntuple) &&
    cutflowlooseMET(ntuple) &&
    cutflowMuonVeto(ntuple) &&
    cutflowElectronVeto(ntuple) &&
    cutflowIsoMuonTrackVeto(ntuple) &&
    cutflowIsoElectronTrackVeto(ntuple) &&
    cutflowIsoPionTrackVeto(ntuple)
  );
}

template<typename ntupleType> bool HadronicBaseline(ntupleType* ntuple) { //Used for cutflow, run from ntuples
  return (
    skimCuts(ntuple) &&
    FiltersCutflow(ntuple) &&
    DeltaPhiCuts(ntuple) &&
    METTightCutflow(ntuple)
  );
}

template<typename ntupleType> bool AK8JetPtCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2) return false;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  if (ntuple->JetsAK8_softDropMass->size()<2) return false;
  return (
    ntuple->JetsAK8->size() >= 2 &&
    ntuple->JetsAK8_ID->at(0)==1 && ntuple->JetsAK8_ID->at(1)==1 &
    jetPt1 > 300. && jetPt2 > 300. &&
    eventHT > 600.
 );
}

template<typename ntupleType> bool looseMassCut(ntupleType* ntuple) {
  return (
    jetMass1 > baselineMassLow &&
    jetMass1 < baselineMassHigh &&
    jetMass2 > baselineMassLow &&
    jetMass2 < baselineMassHigh
 );
}

template<typename ntupleType> bool cutflowNJets(ntupleType* ntuple) {
  return (ntuple->NJets>1);
}

template<typename ntupleType> bool cutflowlooseHT(ntupleType* ntuple) {
  return (eventHT>200);
}

template<typename ntupleType> bool cutflowlooseMET(ntupleType* ntuple) {
  return (eventMET>200);
}

template<typename ntupleType> bool cutflowMuonVeto(ntupleType* ntuple) {
  //if not in skims
  return (ntuple->NMuons==0);
}

template<typename ntupleType> bool cutflowElectronVeto(ntupleType* ntuple) {
  //if not in skims
  return (ntuple->NElectrons==0);
}

template<typename ntupleType> bool cutflowIsoMuonTrackVeto(ntupleType* ntuple) {
  //if not in skims
  return (numIsoMuonTracks==0);
}

template<typename ntupleType> bool cutflowIsoElectronTrackVeto(ntupleType* ntuple) {
  //if not in skims
  return (numIsoElectronTracks==0);
}

template<typename ntupleType> bool cutflowIsoPionTrackVeto(ntupleType* ntuple) {
  //if not in skims
  return (numIsoPionTracks==0);
}

template<typename ntupleType> bool cutflowLeptVeto(ntupleType* ntuple) {
  //if not in skims
  return (ntuple->NMuons==0 && ntuple->NElectrons==0);
}

template<typename ntupleType> bool cutflowDPhiCut(ntupleType* ntuple) {
  //Didn't update since we don't use
  return (DeltaPhiCuts(ntuple) && ntuple->isoElectronTracks+ntuple->isoMuonTracks +ntuple->isoPionTracks==0);
}

template<typename ntupleType> bool cutflowBoostBase(ntupleType* ntuple) {
  //AKA Hadronic Baselin
  TString filename = ntuple->fChain->GetFile()->GetName();
  return (
    eventMET > 300. && eventHT > 300. &&
    FiltersCut(ntuple) &&
    DeltaPhiCuts(ntuple) &&
    ntuple->NMuons==0 && ntuple->NElectrons==0 &&
    numIsoElectronTracks+numIsoMuonTracks+numIsoPionTracks==0
 );
}

template<typename ntupleType> bool cutflowBoostHT(ntupleType* ntuple) {
  return (eventHT > 600.);
}

template<typename ntupleType> bool cutflowBoost2AK8(ntupleType* ntuple) {
  return (ntuple->JetsAK8->size()>1);
}

template<typename ntupleType> bool cutflowBoostPt(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2) return false;
  else return (jetPt1 > 300. && jetPt2 > 300.);
}


template<typename ntupleType> bool cutflowBoostBBTag(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  else return (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0) > deepBBTagCut(ntuple) && ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1) > deepBBTagCut(ntuple));
}

template<typename ntupleType> bool cutflowBoostBBTag1H(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  bool isJ1 = false; bool isJ2 = false;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0) > deepBBTagCut(ntuple)) isJ1=true;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1) > deepBBTagCut(ntuple)) isJ2=true;
  if ((isJ1&&!isJ2) || (!isJ1&&isJ2)) return true;
  else return false;
}

int getTotHHEvents(TString NLSPmass, TString LSPmass) {
  ifstream file("NumHHEvents.txt"); string line;
  int totEvt = 0;
  while (std::getline(file, line)) {
    std::vector<std::string> x = split(line, ' ');
    if (x[0]!=NLSPmass) continue;
    if (x[1]!=LSPmass) continue;
    if (x[2]!=thisYear) continue;
    totEvt = std::stoi(x[3]);
  }
  return totEvt;
}

////////////////////////////////////////////////////////////
/////////////////////// FOR RESOLVED ///////////////////////
////////////////////////////////////////////////////////////
void readResVeto_MC(string whichYear) {
  string in_dir = "../src/evtCount/res/";
  string line;
  if (whichYear != "all") {
    string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_MC_"+whichYear+".txt";
    string fileName2 = in_dir+"processed_resolved_list_CR_met300_SCAN_MC_"+whichYear+".txt";
    vector<string> fileList = {fileName1, fileName2};
    for (int i=0; i<fileList.size(); i++) {
      string fileName = fileList[i];
      ifstream file(fileName);
      while (std::getline(file, line)) {
        std::vector<std::string> x = split(line, ',');
        string thisOne = x[1].erase(0,1) + "," + x[2].erase(0,1) + "," + x[3].erase(0,1) + "," + x[4].erase(0,1)+ ",0";
        vYearRunLumiEvt.insert(thisOne);
      }
    }
  }
  else {
    string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_MC_2016.txt";
    string fileName2 = in_dir+"processed_resolved_list_SR_met300_SCAN_MC_2017.txt";
    string fileName3 = in_dir+"processed_resolved_list_SR_met300_SCAN_MC_2018.txt";
    string fileName4 = in_dir+"processed_resolved_list_CR_met300_SCAN_MC_2016.txt";
    string fileName5 = in_dir+"processed_resolved_list_CR_met300_SCAN_MC_2017.txt";
    string fileName6 = in_dir+"processed_resolved_list_CR_met300_SCAN_MC_2018.txt";
    vector<string> fileList = {fileName1, fileName2, fileName3, fileName4, fileName5, fileName6};
    for (int i=0; i<fileList.size(); i++) {
      string fileName = fileList[i];
      ifstream file(fileName);
      while (std::getline(file, line)) {
        std::vector<std::string> x = split(line, ',');
        string thisOne = x[1].erase(0,1) + "," + x[2].erase(0,1) + "," + x[3].erase(0,1) + "," + x[4].erase(0,1)+ ",0";
        vYearRunLumiEvt.insert(thisOne);
      }
    }
  }
}


void readResVeto_Data(string whichYear) {
  string in_dir = "../src/evtCount/res/";
  string line;
  if (whichYear=="all") {std::cout<<"Res veto not setup for all years!"; return;}
  string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_DATA_"+whichYear+".txt";
  string fileName2 = in_dir+"processed_resolved_list_CR_met300_SCAN_DATA_"+whichYear+".txt";
  vector<string> fileList = {fileName1, fileName2};
  for (int i=0; i<fileList.size(); i++) {
    string fileName = fileList[i];
    ifstream file(fileName);
    while (std::getline(file, line)) {
      std::vector<std::string> x = split(line, ',');
      string thisOne = x[1].erase(0,1) + "," + x[2].erase(0,1) + "," + x[3].erase(0,1) + "," + x[4].erase(0,1)+ ",1";
      vYearRunLumiEvt.insert(thisOne);
      // std::cout<<"Check: "<<thisOne<<std::endl;
    }
  }
}


void readResVeto_Sig1D(string whichYear, string model) {
  string in_dir = "../src/evtCount/res/";
  string line;
  if (whichYear!="all") {
    string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_"+model+"1D_"+whichYear+".txt";
    string fileName2 = in_dir+"processed_resolved_list_CR_met300_SCAN_"+model+"1D_"+whichYear+".txt";
    vector<string> fileList = {fileName1, fileName2};
    for (int i=0; i<fileList.size(); i++) {
      string fileName = fileList[i];
      ifstream file(fileName);
      while (std::getline(file, line)) {
        std::vector<std::string> x = split(line, ',');
        if (model=="TChiHH") {
          string checkNLSPMass = x[6].erase(0,1);
          if (checkNLSPMass!=NLSPmass) continue;
        }
        else if (model=="T5HH") {
          std::vector<std::string> sample = split(x[0], '-');
          std::vector<std::string> mNLSP = split(sample[3], '_');
          if (mNLSP[0]!=NLSPmass) continue;
        }
        string thisOne = x[1].erase(0,1) + "," + x[2].erase(0,1) + "," + x[3].erase(0,1) + "," + x[4].erase(0,1)+ ",0";
        vYearRunLumiEvt.insert(thisOne);
      }
    }
  }
  else {
    string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_"+model+"1D_2016.txt";
    string fileName2 = in_dir+"processed_resolved_list_SR_met300_SCAN_"+model+"1D_2017.txt";
    string fileName3 = in_dir+"processed_resolved_list_SR_met300_SCAN_"+model+"1D_2018.txt";
    string fileName4 = in_dir+"processed_resolved_list_CR_met300_SCAN_"+model+"1D_2016.txt";
    string fileName5 = in_dir+"processed_resolved_list_CR_met300_SCAN_"+model+"1D_2017.txt";
    string fileName6 = in_dir+"processed_resolved_list_CR_met300_SCAN_"+model+"1D_2018.txt";

    vector<string> fileList = {fileName1, fileName2, fileName3, fileName4, fileName5, fileName6};
    for (int i=0; i<fileList.size(); i++) {
      string fileName = fileList[i];
      ifstream file(fileName);
      while (std::getline(file, line)) {
        std::vector<std::string> x = split(line, ',');
        if (model=="TChiHH") {
          string checkNLSPMass = x[6].erase(0,1);
          if (checkNLSPMass!=NLSPmass) continue;
        }
        else if (model=="T5HH") {
          std::vector<std::string> sample = split(x[0], '-');
          std::vector<std::string> mNLSP = split(sample[3], '_');
          if (mNLSP[0]!=NLSPmass) continue;
        }
        string thisOne = x[1].erase(0,1) + "," + x[2].erase(0,1) + "," + x[3].erase(0,1) + "," + x[4].erase(0,1)+ ",0";
        vYearRunLumiEvt.insert(thisOne);
      }
    }
  }
}

void readResVeto_Sig2D(string whichYear, string model) {
  string in_dir = "../src/evtCount/res/";
  string line;
  if (model=="T5HH") {std::cout<<"Res veto not setup for this!!"<<std::endl; return;}

  if (whichYear!="all") {

    string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_TChiHH2D_"+whichYear+".txt";
    string fileName2 = in_dir+"processed_resolved_list_CR_met300_SCAN_TChiHH2D_"+whichYear+".txt";
    vector<string> fileList = {fileName1, fileName2};
    for (int i=0; i<fileList.size(); i++) {
      string fileName = fileList[i];
      ifstream file(fileName);
      while (std::getline(file, line)) {
        std::vector<std::string> x = split(line, ',');
        string checkNLSPMass = x[6].erase(0,1); if (checkNLSPMass!=NLSPmass) continue;
        string thisOne = x[1].erase(0,1)+","+x[2].erase(0,1)+","+x[3].erase(0,1)+","+x[4].erase(0,1)+","+x[7].erase(0,1);
        vYearRunLumiEvt.insert(thisOne);
      }
    }
  }
  else {
    string fileName1 = in_dir+"processed_resolved_list_SR_met300_SCAN_TChiHH2D_2016.txt";
    string fileName2 = in_dir+"processed_resolved_list_SR_met300_SCAN_TChiHH2D_2017.txt";
    string fileName3 = in_dir+"processed_resolved_list_SR_met300_SCAN_TChiHH2D_2018.txt";
    string fileName4 = in_dir+"processed_resolved_list_CR_met300_SCAN_TChiHH2D_2016.txt";
    string fileName5 = in_dir+"processed_resolved_list_CR_met300_SCAN_TChiHH2D_2017.txt";
    string fileName6 = in_dir+"processed_resolved_list_CR_met300_SCAN_TChiHH2D_2018.txt";

    vector<string> fileList = {fileName1, fileName2, fileName3, fileName4, fileName5, fileName6};
    for (int i=0; i<fileList.size(); i++) {
      string fileName = fileList[i];
      ifstream file(fileName);
      while (std::getline(file, line)) {
        std::vector<std::string> x = split(line, ',');
        string checkNLSPMass = x[6].erase(0,1); if (checkNLSPMass!=NLSPmass) continue;
        string thisOne = x[1].erase(0,1) + "," + x[2].erase(0,1) + "," + x[3].erase(0,1) + "," + x[4].erase(0,1)+","+x[7].erase(0,1);
        vYearRunLumiEvt.insert(thisOne);
      }
    }
  }
}


template<typename ntupleType> bool resEventFound(ntupleType* ntuple,string yearStr) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  string runNumber;  string lumiNumber; string evtNumber;
  runNumber = std::to_string(ntuple->RunNum);
  lumiNumber = std::to_string(ntuple->LumiBlockNum);
  evtNumber = std::to_string(ntuple->EvtNum);
  string thisLSP = "0";
  if (LSPmass!="0") {
    int lspmass = LSPmass.Atoi();
    thisLSP = std::to_string(lspmass);
  }
  string check = yearStr+","+runNumber+","+lumiNumber+","+evtNumber+","+thisLSP;
  // std::cout<<"My check: "<<check<<std::endl;

  bool foundEvent = false;
  set<string>::iterator it = vYearRunLumiEvt.find(check);
  if (it != vYearRunLumiEvt.end()) {
    foundEvent = true;
  }
  return foundEvent;
}

template<typename ntupleType> bool resVetoCutflow(ntupleType* ntuple) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  string thisyearinstring = "2016";
  if (filename.Contains("2017") || filename.Contains("Fall17")) thisyearinstring = "2017";
  else if (filename.Contains("2018") || filename.Contains("Autumn18")) thisyearinstring = "2018";
  bool isFound = resEventFound(ntuple,thisyearinstring);
  return !isFound;
}

template<typename ntupleType> bool resVetoCutflowAll(ntupleType* ntuple) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  bool isFound = resEventFound(ntuple,"all");
  return !isFound;
}
////////////////////////////////////////////////////////////
/////////////////////////// BOOSTED ////////////////////////
////////////////////////////////////////////////////////////

//isoTrack veto and lepton veto applied at skim-level
template<typename ntupleType> bool boostedBaselineCut(ntupleType* ntuple) { //only run on 0l and signal
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  if (ntuple->JetsAK8_ID->at(0)!=1 || ntuple->JetsAK8_ID->at(1)!=1) return false;

  return (
    eventMET > 300. &&
    eventHT > 600. &&
    ntuple->JetsAK8->size() >= 2 &&
    jetPt1 > 300. &&
    jetPt2 > 300. &&
    jetMass1 > baselineMassLow &&
    jetMass1 < baselineMassHigh &&
    jetMass2 > baselineMassLow &&
    jetMass2 < baselineMassHigh &&
    DeltaPhiCuts(ntuple) &&
    FiltersCut(ntuple) &&
    METRatioFilterCut(ntuple) && //apply to 0-lepton only
    METMHTFilterCut(ntuple) //apply to 0-lepton only
 );
}

template<typename ntupleType> bool singleMuCut(ntupleType* ntuple) {
  if (ntuple->NMuons != 1 || ntuple->NElectrons != 0) return false;
  double MT = computeMuonMT(ntuple);
  return (ntuple->Muons->at(0).Pt()>30. && MT < 100.);
}

//No isoTrack veto nor lepton veto nor deltaPhi cuts applied at skim-level - don't apply here either!
template<typename ntupleType> bool singleMuBaselineCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  if (ntuple->JetsAK8_ID->at(0)!=1 || ntuple->JetsAK8_ID->at(1)!=1) return false;
  return (
    singleMuCut(ntuple) &&
    eventMET > 300. &&
    eventHT > 600. &&
    ntuple->JetsAK8->size() >= 2 &&
    ntuple->JetsAK8->size() >= 1 &&
    jetPt1 > 300. &&
    jetPt2 > 300. &&
    jetMass1 > baselineMassLow &&
    jetMass1 < baselineMassHigh &&
    jetMass2 > baselineMassLow &&
    jetMass2 < baselineMassHigh &&
    FiltersCut(ntuple)
 );
}

template<typename ntupleType> bool singleEleCut(ntupleType* ntuple) {
  if (ntuple->NMuons != 0 || ntuple->NElectrons != 1) return false;
  double MT = computeElectronMT(ntuple);
  return (ntuple->Electrons->at(0).Pt()>30. && MT < 100.);
}

//No isoTrack veto nor lepton veto nor deltaPhi cuts applied at skim-level, so DON'T APPLY HERE
template<typename ntupleType> bool singleEleBaselineCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  if (ntuple->JetsAK8_ID->at(0)!=1 || ntuple->JetsAK8_ID->at(1)!=1) return false;
  return (
    singleEleCut(ntuple) &&
    eventMET > 300. &&
    eventHT > 600. &&
    ntuple->JetsAK8->size() >= 2 &&
    ntuple->JetsAK8->size() >= 1 &&
    jetPt1 > 300. &&
    jetPt2 > 300. &&
    jetMass1 > baselineMassLow &&
    jetMass1 < baselineMassHigh &&
    jetMass2 > baselineMassLow &&
    jetMass2 < baselineMassHigh &&
    FiltersCut(ntuple)
 );
}

template<typename ntupleType> bool singlePhotonCut(ntupleType* ntuple) {
  if (ntuple->Photons->size()<1 || ntuple->Photons_fullID->size()<1) return false;
  return (
    ntuple->Photons->size()==1 &&
    ntuple->Photons->at(0).Pt() > 100. &&
    ntuple->Photons_fullID->size() == 1 &&
    ntuple->Photons_fullID->at(0) == 1
  );
}

//isoTrack veto and lepton veto applied at skim-level
template<typename ntupleType> bool photonBaselineCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  if (ntuple->JetsAK8_ID->at(0)!=1 || ntuple->JetsAK8_ID->at(1)!=1) return false;
  return (
    singlePhotonCut(ntuple) &&
    eventMET > 200. &&
    eventHT > 600. &&
    ntuple->JetsAK8->size()>=2 &&
    jetMass1 > baselineMassLow &&
    jetMass1 < baselineMassHigh &&
    jetMass2 > baselineMassLow &&
    jetMass2 < baselineMassHigh &&
    jetPt1 > 300. &&
    jetPt2 > 300. &&
    DeltaPhiCuts(ntuple) &&
    FiltersCut(ntuple)
 );
}

template<typename ntupleType> bool singleHiggsTagLooseCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  double thisBBTagCut = deepBBTagCut(ntuple);
  float jet1_bb = ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0);
  float jet2_bb = ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1);
  return ((jet1_bb > thisBBTagCut) && (jet2_bb < thisBBTagCut)) ||
  ((jet1_bb < thisBBTagCut) && (jet2_bb > thisBBTagCut));
}

template<typename ntupleType> bool antiTaggingLooseCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  double thisBBTagCut = deepBBTagCut(ntuple);
  return (
    (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0) < thisBBTagCut) &&
    (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1) < thisBBTagCut)
 );
}

template<typename ntupleType> bool doubleTaggingLooseCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size()<2) return false;
  double thisBBTagCut = deepBBTagCut(ntuple);
  float jet1_bb = ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0);
  float jet2_bb = ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1);
  return (
    jet1_bb > thisBBTagCut &&
    jet2_bb > thisBBTagCut
 );
}

template<typename ntupleType> bool isMassSB(ntupleType* ntuple, int whichJet) {
  float jet_mass = jetMass1;
  if (whichJet==1) jet_mass = jetMass2;
  if ( (jet_mass>baselineMassLow && jet_mass<HmassWindowLow) || (jet_mass>HmassWindowHigh && jet_mass<baselineMassHigh) ) return true;
  else return false;
}

template<typename ntupleType> bool isMassSR(ntupleType* ntuple, int whichJet) {
  float jet_mass = jetMass1;
  if (whichJet==1) jet_mass = jetMass2;
  if ( jet_mass>HmassWindowLow && jet_mass<HmassWindowHigh ) return true;
  else return false;
}

//both jets in mass SR
template<typename ntupleType> bool doubleMassCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8_softDropMass->size()<2) return false;
  return (
    jetMass1 > HmassWindowLow &&
    jetMass1 < HmassWindowHigh &&
    jetMass2 > HmassWindowLow &&
    jetMass2 < HmassWindowHigh
 );
}

//at least one jet in mass SB
template<typename ntupleType> bool SBMassCut(ntupleType* ntuple) {
  bool bool_J1_SB = false; bool bool_J2_SB = false;
  if ( (jetMass1>baselineMassLow && jetMass1<HmassWindowLow) || (jetMass1>HmassWindowHigh && jetMass1<baselineMassHigh) ) bool_J1_SB = true;
  if ( (jetMass2>baselineMassLow && jetMass2<HmassWindowLow) || (jetMass2>HmassWindowHigh && jetMass2<baselineMassHigh) ) bool_J2_SB = true;
  if (bool_J1_SB || bool_J2_SB) return true;
  else return false;
}

template<typename ntupleType> bool singleHiggsTagCut(ntupleType* ntuple) {
  double thisBBTagCut = deepBBTagCut(ntuple);
  return (
    (jetMass1 > HmassWindowLow && jetMass1 < HmassWindowHigh &&
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0) > thisBBTagCut) ||
    (jetMass2 > HmassWindowLow && jetMass2 < HmassWindowHigh &&
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at() > thisBBTagCut)
 );
}

template<typename ntupleType> bool doubleHiggsTagCut(ntupleType* ntuple) {
  double thisBBTagCut = deepBBTagCut(ntuple);
  return (
    jetMass1 > HmassWindowLow && jetMass1 < HmassWindowHigh &&
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0) > thisBBTagCut &&
    jetMass2 > HmassWindowLow && jetMass2 < HmassWindowHigh &&
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(1) > thisBBTagCut
 );
}

template<typename ntupleType> bool tagSR(ntupleType* ntuple, int i) {
  if (ntuple->JetsAK8->size()<2) return false;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2 ||
  ntuple->JetsAK8_softDropMass->size() < 2) return false;
  float jet_mass = jetMass1;
  if (i==1) jet_mass = jetMass2;
  return (
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(i) > deepBBTagCut(ntuple) &&
    jet_mass > HmassWindowLow && jet_mass < HmassWindowHigh
 );
}

template<typename ntupleType> bool tagSB(ntupleType* ntuple, int i) {
  if (ntuple->JetsAK8->size()<2) return false;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2 ||
  ntuple->JetsAK8_softDropMass->size() < 2) return false;
  float jet_mass = jetMass1;
  if (i==1) jet_mass = jetMass2;
  return (
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(i) > deepBBTagCut(ntuple) &&
    (jet_mass < HmassWindowLow && jet_mass > baselineMassLow) ||
    (jet_mass > HmassWindowHigh && jet_mass < baselineMassHigh)
 );
}

template<typename ntupleType> bool antitagSR(ntupleType* ntuple, int i) {
  if (ntuple->JetsAK8->size()<2) return false;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2 ||
  ntuple->JetsAK8_softDropMass->size() < 2) return false;
  float jet_mass = jetMass1;
  if (i==1) jet_mass = jetMass2;
  return (
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(i) < deepBBTagCut(ntuple) &&
    jet_mass > HmassWindowLow && jet_mass < HmassWindowHigh
 );
}

template<typename ntupleType> bool antitagSB(ntupleType* ntuple, int i) {
  if (ntuple->JetsAK8->size()<2) return false;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2 ||
  ntuple->JetsAK8_softDropMass->size() < 2) return false;
  float jet_mass = jetMass1;
  if (i==1) jet_mass = jetMass2;
  return (
    ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(i) < deepBBTagCut(ntuple) &&
    ((jet_mass < HmassWindowLow && jet_mass > baselineMassLow) ||
    (jet_mass > HmassWindowHigh && jet_mass < baselineMassHigh))
 );
}

template<typename ntupleType> bool antitagSRCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  return (antitagSR(ntuple,0) && antitagSR(ntuple,1));
}

template<typename ntupleType> bool antitagSBCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  return ((antitagSB(ntuple,0) && antitagSB(ntuple,1)) ||
  (antitagSB(ntuple,0) && antitagSR(ntuple,1)) ||
  (antitagSR(ntuple,0) && antitagSB(ntuple,1)));
}

template<typename ntupleType> bool antitagPlusBCut(ntupleType* ntuple) { //BTagsT>0, used for MET shape
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  vector<int> thisNBs = numDeepBs(ntuple);
  return ((antitagSBCut(ntuple)||antitagSRCut(ntuple)) && thisNBs.at(2)>0);
}

template<typename ntupleType> bool tagSRCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  return ((tagSR(ntuple,0) && antitagSR(ntuple,1)) ||
  (antitagSR(ntuple,0) && tagSR(ntuple,1)));
}

template<typename ntupleType> bool tagSBCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  return (
    (tagSB(ntuple,0) && antitagSB(ntuple,1)) ||
    (tagSR(ntuple,0) && antitagSB(ntuple,1)) ||
    (tagSB(ntuple,0) && antitagSR(ntuple,1)) ||
    (antitagSB(ntuple,0) && tagSB(ntuple,1)) ||
    (antitagSR(ntuple,0) && tagSB(ntuple,1)) ||
    (antitagSB(ntuple,0) && tagSR(ntuple,1))
 );
}

template<typename ntupleType> bool doubletagSRCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  return (tagSR(ntuple,0) && tagSR(ntuple,1));
}

template<typename ntupleType> bool doubletagSBCut(ntupleType* ntuple) {
  if (ntuple->JetsAK8->size()<2 || ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->size() < 2) return false;
  return (
    (tagSB(ntuple,0) && tagSB(ntuple,1)) ||
    (tagSB(ntuple,0) && tagSR(ntuple,1)) ||
    (tagSR(ntuple,0) && tagSB(ntuple,1))
 );
}

template<typename ntupleType> bool SR_2H1H(ntupleType* ntuple) {
  return (doubletagSRCut(ntuple) || tagSRCut(ntuple));
}

/////////////////////////////////////////////////
//               Trigger Cuts                  //
/////////////////////////////////////////////////
template<typename ntupleType> bool signalTriggerCut(ntupleType* ntuple) {
  string listOfAllTriggerNames = ntuple->fChain->GetBranch("TriggerPass")->GetTitle();
  vector<string> triggerNames;
  stringstream ss(listOfAllTriggerNames);
  while (ss.good()) {
    string substr; std::getline(ss, substr, ',');
    triggerNames.push_back(substr);
  }

  vector<string> passingTriggers;
  passingTriggers.push_back("HLT_PFMET90_PFMHT90_IDTight_v");
  passingTriggers.push_back("HLT_PFMET100_PFMHT100_IDTight_v");
  passingTriggers.push_back("HLT_PFMET110_PFMHT110_IDTight_v");
  passingTriggers.push_back("HLT_PFMET120_PFMHT120_IDTight_v");
  passingTriggers.push_back("HLT_PFMET130_PFMHT130_IDTight_v");
  passingTriggers.push_back("HLT_PFMET140_PFMHT140_IDTight_v");
  passingTriggers.push_back("HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v");
  passingTriggers.push_back("HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v");
  passingTriggers.push_back("HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v");
  passingTriggers.push_back("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v");
  passingTriggers.push_back("HLT_PFMETNoMu130_PFMHTNoMu130_IDTight_v");
  passingTriggers.push_back("HLT_PFMETNoMu140_PFMHTNoMu140_IDTight_v");
  passingTriggers.push_back("HLT_PFMET100_PFMHT100_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMET110_PFMHT110_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMET120_PFMHT120_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMET130_PFMHT130_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMET140_PFMHT140_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMETNoMu130_PFMHTNoMu130_IDTight_PFHT60_v");
  passingTriggers.push_back("HLT_PFMETNoMu140_PFMHTNoMu140_IDTight_PFHT60_v");

  //Need to check these
  passingTriggers.push_back("HLT_PFMET120_PFMHT120_IDTight_HFCleaned_v");
  passingTriggers.push_back("HLT_PFMET120_PFMHT120_IDTight_PFHT60_HFCleaned_v");
  passingTriggers.push_back("HLT_PFMETNoMu120_PFMHTNoMu120 IDTight HFCleaned_v");

  for (size_t i = 0; i < triggerNames.size(); i++) {
    for (size_t j = 0; j < passingTriggers.size(); j++) {
      if (triggerNames[i] == passingTriggers[j] && ntuple->TriggerPass->at(i)==1) return true;
    }
  }
  return false; //if no triggers we want were fired
}

template<typename ntupleType> bool singleMuTriggerCut(ntupleType* ntuple) {
  string listOfAllTriggerNames = ntuple->fChain->GetBranch("TriggerPass")->GetTitle();
  vector<string> triggerNames;
  stringstream ss(listOfAllTriggerNames);
  while (ss.good()) {
    string substr; std::getline(ss, substr, ',');
    triggerNames.push_back(substr);
  }

  vector<string> passingTriggers;
  passingTriggers.push_back("HLT_IsoMu20_v");
  passingTriggers.push_back("HLT_IsoMu22_v");
  passingTriggers.push_back("HLT_IsoMu24_v");
  passingTriggers.push_back("HLT_IsoMu27_v");
  passingTriggers.push_back("HLT_IsoMu22_eta2p1_v");
  passingTriggers.push_back("HLT_IsoMu24_eta2p1_v");
  passingTriggers.push_back("HLT_IsoTkMu22_v");
  passingTriggers.push_back("HLT_IsoTkMu24_v");
  passingTriggers.push_back("HLT_Mu15_IsoVVVL_PFHT350_v");
  passingTriggers.push_back("HLT_Mu15_IsoVVVL_PFHT400_v");
  passingTriggers.push_back("HLT_Mu15_IsoVVVL_PFHT450_v");
  passingTriggers.push_back("HLT_Mu15_IsoVVVL_PFHT600_v");
  passingTriggers.push_back("HLT_Mu50_IsoVVVL_PFHT400_v");
  passingTriggers.push_back("HLT_Mu50_IsoVVVL_PFHT450_v");
  passingTriggers.push_back("HLT_Mu50_v");
  passingTriggers.push_back("HLT_Mu55_v");

  for (size_t i = 0; i < triggerNames.size(); i++) {
    for (size_t j = 0; j < passingTriggers.size(); j++) {
      if (triggerNames[i] == passingTriggers[j]) {
        if (ntuple->TriggerPass->at(i)==1) return true;
      }
    }
  }
  return false; //if no triggers we want were fired
}

template<typename ntupleType> bool singleEleTriggerCut(ntupleType* ntuple) {
  string listOfAllTriggerNames = ntuple->fChain->GetBranch("TriggerPass")->GetTitle();
  vector<string> triggerNames;
  stringstream ss(listOfAllTriggerNames);
  while (ss.good()) {
    string substr; std::getline(ss, substr, ',');
    triggerNames.push_back(substr);
  }

  vector<string> passingTriggers;
  passingTriggers.push_back("HLT_Ele105_CaloIdVT_GsfTrkIdT_v");
  passingTriggers.push_back("HLT_Ele115_CaloIdVT_GsfTrkIdT_v");
  passingTriggers.push_back("HLT_Ele135_CaloIdVT_GsfTrkIdT_v");
  passingTriggers.push_back("HLT_Ele145_CaloIdVT_GsfTrkIdT_v");
  passingTriggers.push_back("HLT_Ele15_IsoVVVL_PFHT350_v");
  passingTriggers.push_back("HLT_Ele15_IsoVVVL_PFHT400_v");
  passingTriggers.push_back("HLT_Ele15_IsoVVVL_PFHT450_v");
  passingTriggers.push_back("HLT_Ele15_IsoVVVL_PFHT600_v");
  passingTriggers.push_back("HLT_Ele27_WPTight_Gsf_v");
  passingTriggers.push_back("HLT_Ele35_WPTight_Gsf_v");
  passingTriggers.push_back("HLT_Ele20_WPLoose_Gsf_v");
  passingTriggers.push_back("HLT_Ele45_WPLoose_Gsf_v");
  passingTriggers.push_back("HLT_Ele25_eta2p1_WPTight_Gsf_v");
  passingTriggers.push_back("HLT_Ele20_eta2p1_WPLoose_Gsf_v");
  passingTriggers.push_back("HLT_Ele27_eta2p1_WPLoose_Gsf_v");

  for (size_t i = 0; i < triggerNames.size(); i++) {
    for (size_t j = 0; j < passingTriggers.size(); j++) {
      if (triggerNames[i] == passingTriggers[j] && ntuple->TriggerPass->at(i)==1) return true;
    }
  }
  return false; //if no triggers we want were fired
}

template<typename ntupleType> bool photonTriggerCut(ntupleType* ntuple) {
  string listOfAllTriggerNames = ntuple->fChain->GetBranch("TriggerPass")->GetTitle();
  vector<string> triggerNames;
  stringstream ss(listOfAllTriggerNames);
  while (ss.good()) {
    string substr; std::getline(ss, substr, ',');
    triggerNames.push_back(substr);
  }

  vector<string> passingTriggers;
  passingTriggers.push_back("HLT_Photon175_v");
  passingTriggers.push_back("HLT_Photon200_v");

  for (size_t i = 0; i < triggerNames.size(); i++) {
    for (size_t j = 0; j < passingTriggers.size(); j++) {
      if (triggerNames[i] == passingTriggers[j] && ntuple->TriggerPass->at(i)==1) return true;
    }
  }
  return false; //if no triggers we want were fired
}

template<typename ntupleType>float PUCorrections(ntupleType* ntuple, TString whichSyst) {
  float pu = 1.0;
  if (whichSyst=="none") pu = ntuple->puWeight;
  else if (whichSyst=="up") pu = ntuple->puSysUp;
  else if (whichSyst=="down") pu = ntuple->puSysDown;
  return pu;
}

template<typename ntupleType>float prefireCorr(ntupleType* ntuple, TString year, TString whichSyst) {
  float prefire = 1.0;
  if (year=="MC2018") return prefire;
  if (whichSyst=="none") prefire = ntuple->NonPrefiringProb;
  else if (whichSyst=="up") prefire = ntuple->NonPrefiringProbUp;
  else if (whichSyst=="down") prefire = ntuple->NonPrefiringProbDown;
  return prefire;
}

template<typename ntupleType>float scalesSystematic(ntupleType* ntuple, TH1F* h_norm, TString whichSyst) {
  float scalesSyst = 1.0;
  if (whichSyst=="none") return scalesSyst;

  //stole from Kevin Pedro, so this does a wee bit more than necessary, namely with PDFs
  vector<double> pdfnorms = vector<double>(4,1.0); //0: PDF up, 1: PDF down, 2: scale up, 3: scale down
  double nominal = h_norm->GetBinContent(1);
  for (unsigned n = 0; n < 4; ++n) {
    pdfnorms[n] = nominal/h_norm->GetBinContent(n+2); //(bin in histo = index + 2)
  }

  vector<float> ScaleWeightsMod = *ntuple->ScaleWeights;
  //remove unwanted variations
  if(ScaleWeightsMod.size()>7) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+7);
  if(ScaleWeightsMod.size()>5) ScaleWeightsMod.erase(ScaleWeightsMod.begin()+5);
  if(ScaleWeightsMod.size()>0) ScaleWeightsMod.erase(ScaleWeightsMod.begin());

  if (whichSyst=="up") scalesSyst = *(TMath::LocMax(ScaleWeightsMod.begin(),ScaleWeightsMod.end()))*pdfnorms[2]; //up
  else if (whichSyst=="down") scalesSyst = *(TMath::LocMin(ScaleWeightsMod.begin(),ScaleWeightsMod.end()))*pdfnorms[3]; //down
  return scalesSyst;
}


template<typename ntupleType>float ISRCorrections(ntupleType* ntuple, int region, TString year, TString whichSyst) {
  TString filename = ntuple->fChain->GetFile()->GetName();
  float isr = 1.0;
  if (!filename.Contains("MC") && !filename.Contains("TChiHH")) return isr;
  if (region==0 || region==1 || region==2) {
    if (filename.Contains("T5qqqqZH") || filename.Contains("T5HH") || filename.Contains("T5qqqqHH")) isr = SignalISRCorrection(ntuple,whichSyst);
    else if (filename.Contains("TTJets") && year=="MC2016") isr = SignalISRCorrection(ntuple,whichSyst);
    else if (filename.Contains("TChiHH_HToBB") && (filename.Contains("fast") || filename.Contains("Fast"))) isr = SignalEWKISRCorrection(ntuple,whichSyst);
  }
  return isr;
}


//For strong SUSY and 2016 tt MC only
template<typename ntupleType>float SignalISRCorrection(ntupleType* ntuple, TString whichSyst) {
  //Changing to match resolved: https://github.com/richstu/nano2pico/blob/206e96eaf7bbfeaafac62fbd1103fbec81b700e0/src/isr_tools.cpp#L129-L163
  float ISRWeights[7] = {1.0, 0.920, 0.821, 0.715, 0.662, 0.561,0.511}; //called isr_wgt in resolved
  const float isr_norm_tt = 1.117;
  float isr_weight = 1.0;
  int numISR = ntuple->NJetsISR;
  if (numISR>6) numISR=6;
  isr_weight = ISRWeights[numISR];
  if (whichSyst=="none") return isr_norm_tt*isr_weight; //nominal

  //For systematics
  //assign relative unc = 50% of the deviation from flat
  float absolute_unc = (1-isr_weight)/2.;
  if (whichSyst=="up") return isr_norm_tt*(isr_weight+absolute_unc); //up
  if (whichSyst=="down") return isr_norm_tt*(isr_weight-absolute_unc); //down
}

//For EWK SUSY only
template<typename ntupleType>bool IsLastCopyBeforeFSR_or_LastCopy(ntupleType* ntuple, int imc) {
  //Stolen from resolved: https://github.com/richstu/nano2pico/blob/206e96eaf7bbfeaafac62fbd1103fbec81b700e0/src/isr_tools.cpp
  bitset<15> mc_statusFlags(ntuple->GenParticles_Status->at(imc));
  int mc_mom_index = ntuple->GenParticles_ParentIdx->at(imc);
  int mc_id = ntuple->GenParticles_PdgId->at(imc);
  // 0: isLastCopyBeforeFSR
  // 1: isLastCopy
  if (mc_statusFlags[1] == 1) {
    if (mc_mom_index == -1) return true;
    bitset<15> mom_statusFlags(ntuple->GenParticles_Status->at(mc_mom_index));
    int mom_id = ntuple->GenParticles_PdgId->at(mc_mom_index);
    // A lastCopyBeforeFSR exists
    if (mom_id == mc_id && mom_statusFlags[0] == 1) return false;
    else return true;
  }
  if (mc_statusFlags[0] == 1) return true;
  return false;
}

//For EWK SUSY only
template<typename ntupleType>float SignalEWKISRCorrection(ntupleType* ntuple, TString whichSyst) {
  //Changing to match resolved: https://github.com/richstu/nano2pico/blob/206e96eaf7bbfeaafac62fbd1103fbec81b700e0/src/isr_tools.cpp
  TLorentzVector isr_p4;
  for (unsigned int imc = 0; imc<ntuple->GenParticles->size(); ++imc) {
    if (IsLastCopyBeforeFSR_or_LastCopy(ntuple, imc)) { //function taken from resolved
      int mc_absid = abs(ntuple->GenParticles_PdgId->at(imc));
      //types defined in event tools
      TLorentzVector mc_v4;
      mc_v4.SetPtEtaPhiM(ntuple->GenParticles->at(imc).Pt(), ntuple->GenParticles->at(imc).Eta(), ntuple->GenParticles->at(imc).Phi(), ntuple->GenParticles->at(imc).M());
      isr_p4 -= mc_v4;
    }
  }
  float thisISRPt = isr_p4.Pt();


  float isr_wgt = 1.;
  if      (thisISRPt<=50)  isr_wgt = 1.;
  else if (thisISRPt<=100) isr_wgt = 1.052;
  else if (thisISRPt<=150) isr_wgt = 1.179;
  else if (thisISRPt<=200) isr_wgt = 1.150;
  else if (thisISRPt<=300) isr_wgt = 1.057;
  else if (thisISRPt<=400) isr_wgt = 1.000;
  else if (thisISRPt<=600) isr_wgt = 0.912;
  else                     isr_wgt = 0.783;
  if (whichSyst=="none") return isr_wgt; //nominal

  // For systematics
  // assign relative unc = 100% of the deviation from flat for up, and 1.0 for down
  if (isr_wgt>1) isr_wgt = 1+2*(isr_wgt-1);
  else isr_wgt = 1-2*(1-isr_wgt);
  if (whichSyst=="up") return isr_wgt; //up
  if (whichSyst=="down") return 1.0; //down
}

template<typename ntupleType> double bbFastSIMSFs(ntupleType* ntuple, TString whichSyst) { //returns an event weight
  TString filename = ntuple->fChain->GetFile()->GetName();
  if (!filename.Contains("fast") && !filename.Contains("Fast")) return 1.0;
  double evtWeight=1.0;
  float sf1 = 1.0; float sf1_unc = 0.0; float sf2 = 1.0; float sf2_unc = 0.0;
  float j1Mass = jetMass1; float j2Mass = jetMass2;
  TString region = "0H";
  if (doubleTaggingLooseCut(ntuple)) region = "2H";
  else if (singleHiggsTagLooseCut(ntuple)) region = "1H";

  //For the systematics, using the larger between stat and syst
  //First, figure out which jet is tagged...
  if (region=="1H") {
    float jetPt = jetPt1; float sdmass = jetMass1;
    if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0)<0.7) {jetPt = jetPt2; sdmass=jetMass2;}

    if (filename.Contains("2016") || filename.Contains("Summer16v3")) {
      if (sdmass>HmassWindowLow && sdmass<HmassWindowHigh) { //jet is in SR
        if      (jetPt>=300 && jetPt<500) {sf1 = 0.998; sf1_unc = 0.004;}
        else if (jetPt>=500 && jetPt<700) {sf1 = 0.993; sf1_unc = 0.003;}
        else if (jetPt>=700 && jetPt<900) {sf1 = 0.980; sf1_unc = 0.004;}
        else if (jetPt>=900)              {sf1 = 0.971; sf1_unc = 0.002;}
      }
      else { //jet is in SB
        if      (jetPt>=300 && jetPt<500) {sf1 = 1.003; sf1_unc = 0.010;}
        else if (jetPt>=500 && jetPt<700) {sf1 = 0.992; sf1_unc = 0.008;}
        else if (jetPt>=700 && jetPt<900) {sf1 = 0.981; sf1_unc = 0.009;}
        else if (jetPt>=900)              {sf1 = 0.993; sf1_unc = 0.009;}
      }

    }
    else if (filename.Contains("2017") || filename.Contains("Fall17")) {
      if (sdmass>HmassWindowLow && sdmass<HmassWindowHigh) { //jet is in SR
        if      (jetPt>=300 && jetPt<500) {sf1 = 0.972; sf1_unc = 0.015;}
        else if (jetPt>=500 && jetPt<700) {sf1 = 0.965; sf1_unc = 0.016;}
        else if (jetPt>=700 && jetPt<900) {sf1 = 0.947; sf1_unc = 0.016;}
        else if (jetPt>=900)              {sf1 = 0.923; sf1_unc = 0.024;}
      }
      else { //jet is in SB
        if      (jetPt>=300 && jetPt<500) {sf1 = 0.961; sf1_unc = 0.023;}
        else if (jetPt>=500 && jetPt<700) {sf1 = 0.922; sf1_unc = 0.025;}
        else if (jetPt>=700 && jetPt<900) {sf1 = 0.892; sf1_unc = 0.074;}
        else if (jetPt>=900)              {sf1 = 0.854; sf1_unc = 0.031;}
      }
    }
    else if (filename.Contains("2018") || filename.Contains("Autumn18")) {
      if (sdmass>HmassWindowLow && sdmass<HmassWindowHigh) { //jet is in SR
        if      (jetPt>=300 && jetPt<500) {sf1 = 0.986; sf1_unc = 0.019;}
        else if (jetPt>=500 && jetPt<700) {sf1 = 0.982; sf1_unc = 0.014;}
        else if (jetPt>=700 && jetPt<900) {sf1 = 0.969; sf1_unc = 0.016;}
        else if (jetPt>=900)              {sf1 = 0.946; sf1_unc = 0.016;}
      }
      else { //jet is in SB
        if      (jetPt>=300 && jetPt<500) {sf1 = 0.979; sf1_unc = 0.022;}
        else if (jetPt>=500 && jetPt<700) {sf1 = 0.955; sf1_unc = 0.049;}
        else if (jetPt>=700 && jetPt<900) {sf1 = 0.919; sf1_unc = 0.088;}
        else if (jetPt>=900)              {sf1 = 0.881; sf1_unc = 0.075;}
      }
    }
    evtWeight=evtWeight*sf1; //nominal
    // evtWeight=evtWeight*(sf1+sf1_unc); //up
    // evtWeight=evtWeight*(sf1-sf1_unc); //down
    return evtWeight;
  } //end 1H
  else if (region=="2H") {
    if (filename.Contains("2016") || filename.Contains("Summer16v3")) {
      if (j1Mass>HmassWindowLow && j1Mass<HmassWindowHigh) { //j1 is in SR
        if      (jetPt1>=300 && jetPt1<500) {sf1 = 0.998; sf1_unc = 0.004;}
        else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.993; sf1_unc = 0.003;}
        else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.980; sf1_unc = 0.004;}
        else if (jetPt1>=900)               {sf1 = 0.971; sf1_unc = 0.002;}
      }
      else { //j1 is in SB
        if      (jetPt1>=300 && jetPt1<500) {sf1 = 1.003; sf1_unc = 0.010;}
        else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.992; sf1_unc = 0.008;}
        else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.981; sf1_unc = 0.009;}
        else if (jetPt1>=900)               {sf1 = 0.993; sf1_unc = 0.009;}
      }

      if (j2Mass>HmassWindowLow && j2Mass<HmassWindowHigh) { //j2 is in SR
        if      (jetPt2>=300 && jetPt2<500) {sf2 = 0.998; sf2_unc = 0.004;}
        else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.993; sf2_unc = 0.003;}
        else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.980; sf2_unc = 0.004;}
        else if (jetPt2>=900)               {sf2 = 0.971; sf2_unc = 0.002;}
      }
      else { //j2 is in SB
        if      (jetPt2>=300 && jetPt2<500) {sf2 = 1.003; sf2_unc = 0.010;}
        else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.992; sf2_unc = 0.008;}
        else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.981; sf2_unc = 0.009;}
        else if (jetPt2>=900)               {sf2 = 0.993; sf2_unc = 0.009;}
      }
    }
    else if (filename.Contains("2017") || filename.Contains("Fall17")) {
      if (j1Mass>HmassWindowLow && j1Mass<HmassWindowHigh) { //j1 is in SR
        if      (jetPt1>=300 && jetPt1<500) {sf1 = 0.972; sf1_unc = 0.015;}
        else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.965; sf1_unc = 0.016;}
        else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.947; sf1_unc = 0.016;}
        else if (jetPt1>=900)               {sf1 = 0.923; sf1_unc = 0.024;}
      }
      else { //j1 is in SB
        if      (jetPt1>=300 && jetPt1<500) {sf1 = 0.961; sf1_unc = 0.023;}
        else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.922; sf1_unc = 0.025;}
        else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.892; sf1_unc = 0.074;}
        else if (jetPt1>=900)               {sf1 = 0.854; sf1_unc = 0.031;}
      }

      if (j2Mass>HmassWindowLow && j2Mass<HmassWindowHigh) { //j2 is in SR
        if      (jetPt2>=300 && jetPt2<500) {sf2 = 0.972; sf2_unc = 0.015;}
        else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.965; sf2_unc = 0.016;}
        else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.947; sf2_unc = 0.016;}
        else if (jetPt2>=900)               {sf2 = 0.923; sf2_unc = 0.024;}
      }
      else {
        if      (jetPt2>=300 && jetPt2<500) {sf2 = 0.961; sf2_unc = 0.023;}
        else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.922; sf2_unc = 0.025;}
        else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.892; sf2_unc = 0.074;}
        else if (jetPt2>=900)               {sf2 = 0.854; sf2_unc = 0.031;}
      }
    }
    else if (filename.Contains("2018") || filename.Contains("Autumn18")) {
      if (j1Mass>HmassWindowLow && j1Mass<HmassWindowHigh) { //j1 is in SR
        if      (jetPt1>=300 && jetPt1<500) {sf1 = 0.986; sf1_unc = 0.019;}
        else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.982; sf1_unc = 0.014;}
        else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.969; sf1_unc = 0.016;}
        else if (jetPt1>=900)               {sf1 = 0.946; sf1_unc = 0.016;}
      }
      else { //jet is in SB
        if      (jetPt1>=300 && jetPt1<500) {sf1 = 0.979; sf1_unc = 0.022;}
        else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.955; sf1_unc = 0.049;}
        else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.919; sf1_unc = 0.088;}
        else if (jetPt1>=900)               {sf1 = 0.881; sf1_unc = 0.075;}
      }

      if (j2Mass>HmassWindowLow && j2Mass<HmassWindowHigh) { //j2 is in SR
        if      (jetPt2>=300 && jetPt2<500) {sf2 = 0.986; sf2_unc = 0.019;}
        else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.982; sf2_unc = 0.014;}
        else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.969; sf2_unc = 0.016;}
        else if (jetPt2>=900)               {sf2 = 0.946; sf2_unc = 0.016;}
      }
      else {
        if      (jetPt2>=300 && jetPt2<500) {sf2 = 0.979; sf2_unc = 0.022;}
        else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.955; sf2_unc = 0.049;}
        else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.919; sf2_unc = 0.088;}
        else if (jetPt2>=900)               {sf2 = 0.881; sf2_unc = 0.075;}
      }
    }

    if (whichSyst=="none") evtWeight=evtWeight*sf1*sf2; //nominal
    else if (whichSyst=="up") evtWeight=evtWeight*(sf1+sf1_unc)*(sf2+sf2_unc); //up
    else if (whichSyst=="down") evtWeight=evtWeight*(sf1-sf1_unc)*(sf2-sf2_unc); //down
    return evtWeight;
  } //end 2H
  return 1.0; //if 0H
}

template<typename ntupleType> double softdropmassFastSIMSFs(ntupleType* ntuple, TString whichSyst) { //returns an event weight
  TString filename = ntuple->fChain->GetFile()->GetName();
  if (!filename.Contains("fast") && !filename.Contains("Fast")) return 1.0;
  double evtWeight=1.0;
  float sf1 = 1.0; float sf1_unc = 0.0;
  float sf2 = 1.0; float sf2_unc = 0.0;

  //Check if jet is in SR or SB
  //using Noah's numbers, applying whicher is bigger between stat and syst as unc
  if (filename.Contains("2016") || filename.Contains("Summer16v3")) {
    if (jetMass1>HmassWindowLow && jetMass1<HmassWindowHigh) { //SR for J1
      if      (jetPt1>=300 && jetPt1<500) {sf1 = 0.988; sf1_unc = 1-sf1;} //systematic is the full correction, difference from 1
      else if (jetPt1>=500 && jetPt1<700) {sf1 = 0.986; sf1_unc = 1-sf1;}
      else if (jetPt1>=700 && jetPt1<900) {sf1 = 0.986; sf1_unc = 1-sf1;}
      else if (jetPt1>=900)               {sf1 = 0.973; sf1_unc = 1-sf1;}
    }
    else { //SB for J1
      if      (jetPt1>=300 && jetPt1<500) {sf1 = 1-0.988+1; sf1_unc = sf1-1;}
      else if (jetPt1>=500 && jetPt1<700) {sf1 = 1-0.986+1; sf1_unc = sf1-1;}
      else if (jetPt1>=700 && jetPt1<900) {sf1 = 1-0.986+1; sf1_unc = sf1-1;}
      else if (jetPt1>=900)               {sf1 = 1-0.973+1; sf1_unc = sf1-1;}
    }
    if (jetMass2>HmassWindowLow && jetMass2<HmassWindowHigh) { //SR for J2
      if      (jetPt2>=300 && jetPt2<500) {sf2 = 0.988; sf2_unc = 1-sf2;}
      else if (jetPt2>=500 && jetPt2<700) {sf2 = 0.986; sf2_unc = 1-sf2;}
      else if (jetPt2>=700 && jetPt2<900) {sf2 = 0.986; sf2_unc = 1-sf2;}
      else if (jetPt2>=900)               {sf2 = 0.973; sf2_unc = 1-sf2;}
    }
    else { //SB for J2
      if      (jetPt2>=300 && jetPt2<500) {sf2 = 1-0.988+1; sf2_unc = sf2-1;}
      else if (jetPt2>=500 && jetPt2<700) {sf2 = 1-0.986+1; sf2_unc = sf2-1;}
      else if (jetPt2>=700 && jetPt2<900) {sf2 = 1-0.986+1; sf2_unc = sf2-1;}
      else if (jetPt2>=900)               {sf2 = 1-0.973+1; sf2_unc = sf2-1;}
    }
  }
  else if (filename.Contains("2017") || filename.Contains("Fall17")) {
    if (jetMass1>HmassWindowLow && jetMass1<HmassWindowHigh) { //SR for J1
      if      (jetPt1>=300 && jetPt1<500) {sf1 = 1.002; sf1_unc = 0.006;} //stat unc is greater than syst
      else if (jetPt1>=500 && jetPt1<700) {sf1 = 1.007; sf1_unc = 0.007;}
      else if (jetPt1>=700 && jetPt1<900) {sf1 = 1.005; sf1_unc = 0.006;} //stat unc is greater than syst
      else if (jetPt1>=900)               {sf1 = 1.034; sf1_unc = 0.034;}
    }
    else { //SB for J1
      if      (jetPt1>=300 && jetPt1<500) {sf1 = 1-1.002+1; sf1_unc = 1-sf1;}
      else if (jetPt1>=500 && jetPt1<700) {sf1 = 1-1.007+1; sf1_unc = 1-sf1;}
      else if (jetPt1>=700 && jetPt1<900) {sf1 = 1-1.005+1; sf1_unc = 1-sf1;}
      else if (jetPt1>=900)               {sf1 = 1-1.034+1; sf1_unc = 1-sf1;}
    }
    if (jetMass2>HmassWindowLow && jetMass2<HmassWindowHigh) { //SR for J2
      if      (jetPt2>=300 && jetPt2<500) {sf2 = 1.002; sf2_unc = 0.006;} //stat unc is greater than syst
      else if (jetPt2>=500 && jetPt2<700) {sf2 = 1.007; sf2_unc = 0.007;}
      else if (jetPt2>=700 && jetPt2<900) {sf2 = 1.005; sf2_unc = 0.006;}  //stat unc is greater than syst
      else if (jetPt2>=900)               {sf2 = 1.034; sf2_unc = 0.034;}
    }
    else { //SB for J2
      if      (jetPt2>=300 && jetPt2<500) {sf2 = 1-1.002+1; sf2_unc = 1-sf2;}
      else if (jetPt2>=500 && jetPt2<700) {sf2 = 1-1.007+1; sf2_unc = 1-sf2;}
      else if (jetPt2>=700 && jetPt2<900) {sf2 = 1-1.005+1; sf2_unc = 1-sf2;}
      else if (jetPt2>=900)               {sf2 = 1-1.034+1; sf2_unc = 1-sf2;}
    }
  }
  else if (filename.Contains("2018") || filename.Contains("Autumn18")) {
    if (jetMass1>HmassWindowLow && jetMass1<HmassWindowHigh) { //SR for J1
      if      (jetPt1>=300 && jetPt1<500) {sf1 = 1.015; sf1_unc = 0.015;}
      else if (jetPt1>=500 && jetPt1<700) {sf1 = 1.014; sf1_unc = 0.014;}
      else if (jetPt1>=700 && jetPt1<900) {sf1 = 1.027; sf1_unc = 0.027;}
      else if (jetPt1>=900)               {sf1 = 1.029; sf1_unc = 0.029;}
    }
    else { //SB for J1
      if      (jetPt1>=300 && jetPt1<500) {sf1 = 1-1.015+1; sf1_unc = 1-sf1;}
      else if (jetPt1>=500 && jetPt1<700) {sf1 = 1-1.014+1; sf1_unc = 1-sf1;}
      else if (jetPt1>=700 && jetPt1<900) {sf1 = 1-1.027+1; sf1_unc = 1-sf1;}
      else if (jetPt1>=900)               {sf1 = 1-1.029+1; sf1_unc = 1-sf1;}
    }
    if (jetMass2>HmassWindowLow && jetMass2<HmassWindowHigh) { //SR for J2
      if      (jetPt2>=300 && jetPt2<500) {sf2 = 1.015; sf2_unc = 0.015;}
      else if (jetPt2>=500 && jetPt2<700) {sf2 = 1.014; sf2_unc = 0.014;}
      else if (jetPt2>=700 && jetPt2<900) {sf2 = 1.027; sf2_unc = 0.027;}
      else if (jetPt2>=900)               {sf2 = 1.029; sf2_unc = 0.029;}
    }
    else { //SB for J2
      if      (jetPt2>=300 && jetPt2<500) {sf2 = 1-1.015+1; sf2_unc = 1-sf2;}
      else if (jetPt2>=500 && jetPt2<700) {sf2 = 1-1.014+1; sf2_unc = 1-sf2;}
      else if (jetPt2>=700 && jetPt2<900) {sf2 = 1-1.027+1; sf2_unc = 1-sf2;}
      else if (jetPt2>=900)               {sf2 = 1-1.029+1; sf2_unc = 1-sf2;}
    }
  }
  if (whichSyst=="none") evtWeight=evtWeight*sf1*sf2; //nominal
  else if (whichSyst=="up") evtWeight=evtWeight*(sf1+sf1_unc)*(sf2+sf2_unc); //up
  else if (whichSyst=="down") evtWeight=evtWeight*(sf1-sf1_unc)*(sf2-sf2_unc); //down
  return evtWeight;
}

template<typename ntupleType> double bbSFs(ntupleType* ntuple, TString whichSyst) {
  double evtWeight=1.0;
  if (doubleTaggingLooseCut(ntuple)) evtWeight = bbSFs_2H(ntuple,whichSyst);
  else if (singleHiggsTagLooseCut(ntuple)) evtWeight = bbSFs_1H(ntuple,whichSyst);
  return evtWeight;
}

template<typename ntupleType> double bbSFs_2H(ntupleType* ntuple, TString whichSyst) { //return an event weight
  TString filename = ntuple->fChain->GetFile()->GetName();
  if (!filename.Contains("MC") && !filename.Contains("TChiHH")) return 1.0;
  double evtWeight=1.0; float uncLowPt = 0.0; float uncHighPt = 0.0;
  bool isSignal = (filename.Contains("TChiHH")||filename.Contains("T5qqqqZH")||filename.Contains("T5HH")||filename.Contains("T5qqqqHH"));

  if (isSignal) {
    if (filename.Contains("2016") || filename.Contains("Summer16v3")) {
      if (whichSyst=="up") {uncLowPt = 0.10; uncHighPt = 0.10;} // up
      else if (whichSyst=="down") {uncLowPt = -0.04; uncHighPt = -0.04;} // down
      if (jetPt1>250. && jetPt1<=350) evtWeight=evtWeight*(0.95+uncLowPt); //this is currently missing, so using a general SF
      else if (jetPt1>350.) evtWeight=evtWeight*(0.95+uncHighPt);

      if (jetPt2>250. && jetPt2<=350) evtWeight=evtWeight*(0.95+uncLowPt);
      else if (jetPt2>350.) evtWeight=evtWeight*(0.95+uncHighPt);
      return evtWeight;
    }
    else if (filename.Contains("2017") || filename.Contains("Fall17")) {
      if (whichSyst=="up") {uncLowPt = 0.04; uncHighPt = 0.07;} // up
      else if (whichSyst=="down") {uncLowPt = -0.04; uncHighPt = -0.12;} // down
      if (jetPt1>250. && jetPt1<=350) evtWeight=evtWeight*(0.92+uncLowPt);
      else if (jetPt1>350.) evtWeight=evtWeight*(1.01+uncHighPt);

      if (jetPt2>250. && jetPt2<=350) evtWeight=evtWeight*(0.92+uncLowPt);
      else if (jetPt2>350.) evtWeight=evtWeight*(1.01+uncHighPt);

      return evtWeight;
    }
    else if (filename.Contains("2018") || filename.Contains("Autumn18")) {
      if (whichSyst=="up") {uncLowPt = 0.04; uncHighPt = 0.07;} // up
      else if (whichSyst=="down") {uncLowPt = -0.05; uncHighPt = -0.06;} // down
      if (jetPt1>250. && jetPt1<=350) evtWeight=evtWeight*(0.97+uncLowPt);
      else if (jetPt1>350.) evtWeight=evtWeight*(0.96+uncHighPt);

      if (jetPt2>250. && jetPt2<=350) evtWeight=evtWeight*(0.97+uncLowPt);
      else if (jetPt2>350.) evtWeight=evtWeight*(0.96+uncHighPt);
      return evtWeight;
    }
  } //end signal

  else {
    //Commented out bits are for deriving the MC mismodeling systematic
    int numBs_1 = numOverlapBs(ntuple,0);
    if (numBs_1==0) {
      if (jetPt1>300. && jetPt1<=400) evtWeight=evtWeight*0.989;
      else if (jetPt1>400.) evtWeight=evtWeight*1.092;
      // if (jetPt1>300. && jetPt1<=400) evtWeight=evtWeight*(0.989-0.097-0.179);
      // else if (jetPt1>400.) evtWeight=evtWeight*(1.092-0.285-0.146);
    }
    else if (numBs_1==1) {
      if (jetPt1>300. && jetPt1<=400) evtWeight=evtWeight*0.941;
      else if (jetPt1>400.) evtWeight=evtWeight*0.875;
      // if (jetPt1>300. && jetPt1<=400) evtWeight=evtWeight*(0.941-0.017-0.108);
      // else if (jetPt1>400.) evtWeight=evtWeight*(0.875-0.053-0.060);
    }
    else if (numBs_1>1) {
      if (jetPt1>300. && jetPt1<=400) evtWeight=evtWeight*0.976;
      else if (jetPt1>400.) evtWeight=evtWeight*1.004;
      // if (jetPt1>300. && jetPt1<=400) evtWeight=evtWeight*(0.976-0.022-0.060);
      // else if (jetPt1>400.) evtWeight=evtWeight*(1.004-0.072-0.074);
    }

    int numBs_2 = numOverlapBs(ntuple,1);
    if (numBs_2==0) {
      if (jetPt2>300. && jetPt2<=400) evtWeight=evtWeight*0.989;
      else if (jetPt2>400.) evtWeight=evtWeight*1.092;
      // if (jetPt2>300. && jetPt2<=400) evtWeight=evtWeight*(0.989-0.097-0.179);
      // else if (jetPt2>400.) evtWeight=evtWeight*(1.092-0.285-0.146);
    }
    else if (numBs_2==1) {
      if (jetPt2>300. && jetPt2<=400) evtWeight=evtWeight*0.941;
      else if (jetPt2>400.) evtWeight=evtWeight*0.875;
      // if (jetPt2>300. && jetPt2<=400) evtWeight=evtWeight*(0.941-0.017-0.108);
      // else if (jetPt2>400.) evtWeight=evtWeight*(0.875-0.053-0.060);
    }
    else if (numBs_2>1) {
      if (jetPt2>300. && jetPt2<=400) evtWeight=evtWeight*0.976;
      else if (jetPt2>400.) evtWeight=evtWeight*1.004;
      // if (jetPt2>300. && jetPt2<=400) evtWeight=evtWeight*(0.976-0.022-0.060);
      // else if (jetPt2>400.) evtWeight=evtWeight*(1.004-0.072-0.074);
    }
  }
  return evtWeight;
}

template<typename ntupleType> double bbSFs_1H(ntupleType* ntuple, TString whichSyst) { //return an event weight
  TString filename = ntuple->fChain->GetFile()->GetName();
  if (!filename.Contains("MC") && !filename.Contains("TChiHH")) return 1.0;
  double evtWeight=1.0; float uncLowPt = 0.0; float uncHighPt = 0.0;
  bool isSignal = (filename.Contains("TChiHH")||filename.Contains("T5qqqqZH")||filename.Contains("T5HH")||filename.Contains("T5qqqqHH"));

  //First, figure out which jet is tagged...
  float jetPt = jetPt1; int thisIsOurJet = 0;
  if (ntuple->JetsAK8_pfMassIndependentDeepDoubleBvLJetTagsProbHbb->at(0)<0.7) {jetPt = jetPt2; thisIsOurJet=1;}

  if (isSignal) {
    if (filename.Contains("2016") || filename.Contains("Summer16v3")) {
      if (whichSyst=="up") {uncLowPt = 0.10; uncHighPt = 0.10;} // up
      else if (whichSyst=="down") {uncLowPt = -0.04; uncHighPt = -0.04;} // down
      if (jetPt>250. && jetPt<=350) evtWeight=evtWeight*(0.95+uncLowPt); //this is currently missing, so using a general SF
      else if (jetPt>350.) evtWeight=evtWeight*(0.95+uncHighPt);
      return evtWeight;
    }
    else if (filename.Contains("2017") || filename.Contains("Fall17")) {
      if (whichSyst=="up") {uncLowPt = 0.04; uncHighPt = 0.07;} // up
      else if (whichSyst=="down") {uncLowPt = -0.04; uncHighPt = -0.12;} // down
      if (jetPt>250. && jetPt<=350) evtWeight=evtWeight*(0.92+uncLowPt);
      else if (jetPt>350.) evtWeight=evtWeight*(1.01+uncHighPt);
      return evtWeight;
    }
    else if (filename.Contains("2018") || filename.Contains("Autumn18")) {
      if (whichSyst=="up") {uncLowPt = 0.04; uncHighPt = 0.07;} // up
      else if (whichSyst=="down") {uncLowPt = -0.05; uncHighPt = -0.06;} // down
      if (jetPt>250. && jetPt<=350) evtWeight=evtWeight*(0.97+uncLowPt);
      else if (jetPt>350.) evtWeight=evtWeight*(0.96+uncHighPt);
      return evtWeight;
    }
  } //end signal

  else {
    //commented out parts are for deriving the MC mismodeling systematic
    int numBs = numOverlapBs(ntuple,thisIsOurJet);
    if (numBs==0) {
      if (jetPt>300. && jetPt<=400) evtWeight=evtWeight*0.989;
      else if (jetPt>400.) evtWeight=evtWeight*1.092;
      // if (jetPt>300. && jetPt<=400) evtWeight=evtWeight*(0.989-0.097-0.179); //-0.082
      // else if (jetPt>400.) evtWeight=evtWeight*(1.092-0.285-0.146); //-0.431
    }
    else if (numBs==1) {
      if (jetPt>300. && jetPt<=400) evtWeight=evtWeight*0.941;
      else if (jetPt>400.) evtWeight=evtWeight*0.875;
      // if (jetPt>300. && jetPt<=400) evtWeight=evtWeight*(0.941-0.017-0.108); //-0.125
      // else if (jetPt>400.) evtWeight=evtWeight*(0.875-0.053-0.060); //-0.113
    }
    else if (numBs>1) {
      if (jetPt>300. && jetPt<=400) evtWeight=evtWeight*0.976;
      else if (jetPt>400.) evtWeight=evtWeight*1.004;
      // if (jetPt>300. && jetPt<=400) evtWeight=evtWeight*(0.976-0.022-0.060); //-0.082
      // else if (jetPt>400.) evtWeight=evtWeight*(1.004-0.072-0.074); //-0.146
    }
  }
  return evtWeight;
}

template<typename ntupleType> double RemakeAK8Jets(ntupleType* ntuple,int j) {
  //also used for 3% correction for signal jets
  TString filename = ntuple->fChain->GetFile()->GetName();
  double jetPt = ntuple->JetsAK8->at(j).Pt();
  if (!filename.Contains("MC") && !filename.Contains("TChiHH")) return jetPt;

  bool doTheB = !filename.Contains("2016") && !filename.Contains("Summer16v3") && (filename.Contains("fast") || filename.Contains("Fast")) && do3PerCorr;
  bool isSignal = filename.Contains("TChiHH")||filename.Contains("T5qqqqZH")||filename.Contains("T5qqqqHH");
  double doubleBSF=1.0;

  vector<int> newIndex;
  newIndex.reserve(ntuple->JetsAK8_origIndex->size());
  for (unsigned jet = 0; jet < ntuple->JetsAK8_origIndex->size(); ++jet) {
    newIndex[ntuple->JetsAK8_origIndex->at(jet)] = jet;
  }

  int i = newIndex[ntuple->JetsAK8JECup_origIndex->at(j)];

  if (whichJEC=="none" && isSignal && doTheB) {
    TLorentzVector newAK8Jet = ntuple->JetsAK8->at(i)*0.97;
    jetPt = newAK8Jet.Pt();
  }
  else if (whichJEC=="JECUp") {
    TLorentzVector newAK8Jet = ntuple->JetsAK8->at(i)*(1./ntuple->JetsAK8_jerFactor->at(i))*(1+ntuple->JetsAK8_jecUnc->at(i))*ntuple->JetsAK8JECup_jerFactor->at(j); //JEC up
    if (isSignal && doTheB) newAK8Jet=newAK8Jet*0.97;
    jetPt = newAK8Jet.Pt();
  }
  else if (whichJEC=="JECDown") {
    TLorentzVector newAK8Jet = ntuple->JetsAK8->at(i)*(1./ntuple->JetsAK8_jerFactor->at(i))*(1-ntuple->JetsAK8_jecUnc->at(i))*ntuple->JetsAK8JECdown_jerFactor->at(j); //JEC down
    if ( isSignal && !filename.Contains("2016") && !filename.Contains("Summer16v3")) newAK8Jet=newAK8Jet*0.97;
    jetPt = newAK8Jet.Pt();
  }
  else if (whichJEC=="JERUp") {
    TLorentzVector newAK8Jet =  ntuple->JetsAK8->at(i)*(1./ntuple->JetsAK8_jerFactor->at(i))*ntuple->JetsAK8_jerFactorUp->at(i); //JER up
    if (isSignal && doTheB) newAK8Jet=newAK8Jet*0.97;
    jetPt = newAK8Jet.Pt();
  }
  else if (whichJEC=="JERDown") {
    TLorentzVector newAK8Jet =  ntuple->JetsAK8->at(i)*(1./ntuple->JetsAK8_jerFactor->at(i))*ntuple->JetsAK8_jerFactorDown->at(i); //JER down
    if (isSignal && doTheB) newAK8Jet=newAK8Jet*0.97;
    jetPt = newAK8Jet.Pt();
  }
  return jetPt;
}

std::vector<TEfficiency*> photonTrigEffHist(TString Year) {
  TFile* photonTriggerEffFile = TFile::Open("../data/triggersRa2bRun2_v4_withTEffs.root");
  std::vector<TEfficiency*> eTrigEff_;
  if (Year == "MC2016") {
    eTrigEff_.push_back((TEfficiency*) photonTriggerEffFile->Get("teff_SinglePhotonBarrelLooseHdp_hists_Run2016_JetHT"));
    eTrigEff_.push_back((TEfficiency*) photonTriggerEffFile->Get("teff_SinglePhotonEndcapLooseHdp_hists_Run2016_JetHT"));
  }
  else if (Year == "MC2017") {
    eTrigEff_.push_back((TEfficiency*) photonTriggerEffFile->Get("teff_SinglePhotonBarrelLooseHdp_hists_Run2017_JetHT"));
    eTrigEff_.push_back((TEfficiency*) photonTriggerEffFile->Get("teff_SinglePhotonEndcapLooseHdp_hists_Run2017_JetHT"));
  }
  else if (Year == "MC2018") {
    eTrigEff_.push_back((TEfficiency*) photonTriggerEffFile->Get("teff_SinglePhotonBarrelLooseHdp_hists_Run2018_JetHT"));
    eTrigEff_.push_back((TEfficiency*) photonTriggerEffFile->Get("teff_SinglePhotonEndcapLooseHdp_hists_Run2018_JetHT"));
  }
  return eTrigEff_;
}

template<typename ntupleType> double photonTrigEff(ntupleType* ntuple, std::vector<TEfficiency*> eTrigEff_) {
  double effWt = 1, effSys = 0;
  if (ntuple->Photons->size() == 1) {
    if(ntuple->Photons_isEB->at(0) == 1 && eTrigEff_[0] != nullptr) {
      TH1F* htot = (TH1F*) eTrigEff_[0]->GetTotalHistogram();
      Int_t bin = min(htot->GetNbinsX(), htot->FindBin(ntuple->Photons->at(0).Pt()));
      double eff = eTrigEff_[0]->GetEfficiency(bin);
      effWt *= eff;
      effSys = eff > 0 ? eTrigEff_[0]->GetEfficiencyErrorLow(bin) / eff : 0;
    }
    else if(ntuple->Photons_isEB->at(0) == 0 && eTrigEff_[1] != nullptr) {
      TH1F* htot = (TH1F*) eTrigEff_[1]->GetTotalHistogram();
      Int_t bin = min(htot->GetNbinsX(), htot->FindBin(ntuple->Photons->at(0).Pt()));
      double eff = eTrigEff_[1]->GetEfficiency(bin);
      effWt *= eff;
      effSys = eff > 0 ? eTrigEff_[1]->GetEfficiencyErrorLow(bin) / eff : 0;
    }
  }
  return effWt; // return pair<double, double>(effWt, effSys);
}

std::vector<TH2F*> openSFFiles(TString Year, int region) {
  std::vector<TH2F*> hSFeff_;
  if (region==0 || region==1 || region==2) return hSFeff_;
  else if (region == 3) { //Muons first
    if (Year == "MC2016") {
      TFile* muonIDSFFile_ = TFile::Open("../data/Muons2016_SF_ID.root");
      TFile* muonIsoSFFile_ = TFile::Open("../data/Muons2016_SF_ISO.root");
      hSFeff_.push_back((TH2F*) muonIDSFFile_->Get("NUM_MediumID_DEN_genTracks_eta_pt"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for 2016 muon ID SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) muonIsoSFFile_->Get("NUM_TightRelIso_DEN_MediumID_eta_pt"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for 2016 muon iso SFs not found *****" << std::endl;
    }
    else if (Year == "MC2017") {
      TFile* muonIDSFFile_ = TFile::Open("../data/Muons2017_SF_ID.root");
      TFile* muonIsoSFFile_ = TFile::Open("../data/Muons2017_SF_ISO.root");
      hSFeff_.push_back((TH2F*) muonIDSFFile_->Get("NUM_MediumID_DEN_genTracks_pt_abseta"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for 2017 muon ID SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) muonIsoSFFile_->Get("NUM_TightRelIso_DEN_MediumID_pt_abseta"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for 2017 muon iso SFs not found *****" << std::endl;
    }
    else if (Year == "MC2018") {
      TFile* muonIDSFFile_ = TFile::Open("../data/Muons2018_SF_ID.root");
      TFile* muonIsoSFFile_ = TFile::Open("../data/Muons2018_SF_ISO.root");
      hSFeff_.push_back((TH2F*) muonIDSFFile_->Get("NUM_MediumID_DEN_TrackerMuons_pt_abseta"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for 2018 muon ID SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) muonIsoSFFile_->Get("NUM_TightRelIso_DEN_MediumID_pt_abseta"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for 2018 muon iso SFs not found *****" << std::endl;
    }
  }
  else if (region == 4) {  //Electrons
    if (Year == "MC2016") {
      TFile* elecIDandIsoSFFile_ = TFile::Open("../data/Electrons2016_SF_ISOandID.root");
      TFile* elecRecoLowSFFile_ = TFile::Open("../data/Electrons2016_SF_RECOlow.root");
      TFile* elecRecoHighSFFile_ = TFile::Open("../data/Electrons2016_SF_RECOhigh.root");

      hSFeff_.push_back((TH2F*) elecIDandIsoSFFile_->Get("Run2016_CutBasedVetoNoIso94XV2"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron ID SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecIDandIsoSFFile_->Get("Run2016_Mini"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron iso SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecRecoHighSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron reco (high pT) SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecRecoLowSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron reco (low pT) SFs not found *****" << std::endl;
    }
    else if (Year == "MC2017") {
      TFile* elecIDandIsoSFFile_ = TFile::Open("../data/Electrons2017_SF_ISOandID.root");
      TFile* elecRecoLowSFFile_ = TFile::Open("../data/Electrons2017_SF_RECOlow.root");
      TFile* elecRecoHighSFFile_ = TFile::Open("../data/Electrons2017_SF_RECOhigh.root");

      hSFeff_.push_back((TH2F*) elecIDandIsoSFFile_->Get("Run2017_CutBasedVetoNoIso94XV2"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron ID SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecIDandIsoSFFile_->Get("Run2017_MVAVLooseTightIP2DMini"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron iso SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecRecoHighSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron reco (high pT) SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecRecoLowSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron reco (low pT) SFs not found *****" << std::endl;
    }
    else if (Year == "MC2018") {
      TFile* elecIDandIsoSFFile_ = TFile::Open("../data/Electrons2018_SF_ISOandID.root");
      // TFile* elecRecoLowSFFile_ = TFile::Open("../data/Electrons2017_SF_RECOlow.root");//not available yet for 2018 - FIXME
      TFile* elecRecoHighSFFile_ = TFile::Open("../data/Electrons2018_SF_RECOhigh.root");

      hSFeff_.push_back((TH2F*) elecIDandIsoSFFile_->Get("Run2018_CutBasedVetoNoIso94XV2"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron ID SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecIDandIsoSFFile_->Get("Run2018_Mini"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron iso SFs not found *****" << std::endl;
      hSFeff_.push_back((TH2F*) elecRecoHighSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for electron reco (high pT) SFs not found *****" << std::endl;
    }
  }
  else if (region == 5) { //Photons
    if (Year == "MC2016") {
      TFile* photonSFFile_ = TFile::Open("../data/Photons2016_SF_all.root");
      hSFeff_.push_back((TH2F*) photonSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for photon SFs not found *****" << std::endl;
    }
    else if (Year == "MC2017") {
      TFile* photonSFFile_ = TFile::Open("../data/Photons2017_SF_all.root");
      hSFeff_.push_back((TH2F*) photonSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for photon SFs not found *****" << std::endl;
    }
    else if (Year == "MC2018") {
      TFile* photonSFFile_ = TFile::Open("../data/Photons2018_SF_all.root");
      hSFeff_.push_back((TH2F*) photonSFFile_->Get("EGamma_SF2D"));
      if (hSFeff_.back() == nullptr) std::cout << "***** Histogram for photon SFs not found *****" << std::endl;
    }
  }
  return hSFeff_;
}

template<typename ntupleType> double electronSFs(ntupleType* ntuple, TString Year, std::vector<TH2F*> hSFeff_) {
  double effWt = 1, effSys = 0;
  if (hSFeff_[0] != nullptr && hSFeff_[1] != nullptr) {
    float pt = 0; float eta = 0;
    int globalbin_ID = 0; int globalbin_ISO = 0;

    double sysCorr = 0;
    if (hSFeff_[2]!=nullptr && ntuple->NElectrons == 1) {
      int globalbin_RECO = 0;
      int numElectrons = ntuple->Electrons->size();
      for (int i=0; i<numElectrons; i++) { //loop through electrons, but will only ever have exactly one electron
        if (!(ntuple->Electrons_passIso->at(i))) continue;
        pt  = ntuple->Electrons->at(i).Pt(); if (pt>500) pt=499.9;
        eta = ntuple->Electrons->at(i).Eta();
        globalbin_ID  = hSFeff_[0]->FindBin(eta,pt); globalbin_ISO = hSFeff_[1]->FindBin(eta,pt);
        double effID = hSFeff_[0]->GetBinContent(globalbin_ID);
        double effISO = hSFeff_[1]->GetBinContent(globalbin_ISO);
        effWt *= effID * effISO;
        double sysOne = quadSum(effID > 0 ? hSFeff_[0]->GetBinError(globalbin_ID)/effID : 0, effISO > 0 ? hSFeff_[1]->GetBinError(globalbin_ISO)/effISO : 0);
        if (Year == "MC2018") { //2018 only has reco SFs for pt>10
    	    if (pt<=10.0) pt = 10.1;
    	    globalbin_RECO = hSFeff_[2]->FindBin(eta,pt);
    	    double eff = hSFeff_[2]->GetBinContent(globalbin_RECO);
    	    effWt *= eff;
    	    sysOne = eff > 0 ? quadSum(sysOne, hSFeff_[2]->GetBinError(globalbin_RECO)/eff) : sysOne;
    	  }
    	  else { //2016 and 2017 have reco SFs for 0-20 GeV and >20 GeV
    	    if (pt>20) {
    	      globalbin_RECO = hSFeff_[2]->FindBin(eta,pt);
    	      double eff = hSFeff_[2]->GetBinContent(globalbin_RECO);
    	      effWt *= eff;
    	      sysOne = eff > 0 ? quadSum(sysOne, hSFeff_[2]->GetBinError(globalbin_RECO)/eff) : sysOne;
    	    }
    	    else {
    	      globalbin_RECO = hSFeff_[3]->FindBin(eta,pt);
    	      double eff = hSFeff_[3]->GetBinContent(globalbin_RECO);
    	      effWt *= eff;
    	      sysOne = eff > 0 ? quadSum(sysOne, hSFeff_[3]->GetBinError(globalbin_RECO)/eff) : 0;
    	    }
    	  } //2016 or 2017
        sysCorr += sysOne;
      } //loop over all electrons (should be one)
    }
    effSys = quadSum(effSys, sysCorr);
  }
  // return pair<double, double>(effWt, effSys);
  return effWt;
}

template<typename ntupleType> double muonSFs(ntupleType* ntuple, TString Year, std::vector<TH2F*> hSFeff_) {
  double effWt = 1.0, effSys = 0.0;
  double muIDISOsys = 0.003;  //  Hard-wired based on _syst histo for 2017 from POG
  if (hSFeff_[0] != nullptr && hSFeff_[1] != nullptr) {
    float pt = 0; float eta = 0;
    int globalbin_ID = 0; int globalbin_ISO = 0;
    double sysCorr = 0;  // Errors are correlated between Z daughter leptons
    int numMuons = ntuple->Muons->size();
    for (int i=0; i<numMuons; i++) { //should only be one
      if (!(ntuple->Muons_mediumID->at(i) && ntuple->Muons_passIso->at(i))) continue;
      pt = ntuple->Muons->at(i).Pt(); if (pt>120) pt=119.9;
      if (Year == "MC2016") {
  	    eta = ntuple->Muons->at(i).Eta();
  	    globalbin_ID = hSFeff_[0]->FindBin(eta,pt); globalbin_ISO = hSFeff_[1]->FindBin(eta,pt);
  	  }
  	  else { //2017 and 2018 have abs(eta), and the x- and y-axis are swapped compared to 2016
  	    eta = abs(ntuple->Muons->at(i).Eta());
  	    globalbin_ID = hSFeff_[0]->FindBin(pt,eta); globalbin_ISO = hSFeff_[1]->FindBin(pt,eta);
  	  }

      double effSF = hSFeff_[0]->GetBinContent(globalbin_ID);
      double effISO = hSFeff_[1]->GetBinContent(globalbin_ISO);
      effWt *= effSF * effISO;
      sysCorr += quadSum(effSF > 0 ? hSFeff_[0]->GetBinError(globalbin_ID)/effSF : 0, effISO > 0 ? hSFeff_[1]->GetBinError(globalbin_ISO)/effISO : 0, muIDISOsys);
    } //loop over all muons (should be one)
    effSys = quadSum(effSys, sysCorr);
  }
  // return pair<double, double>(effWt, effSys);
  return effWt;
}

template<typename ntupleType> double photonSFs(ntupleType* ntuple, std::vector<TH2F*> hSFeff_) {
  double effWt = 1, effSys = 0;
  if (hSFeff_[0] != nullptr && ntuple->Photons->size() == 1) {
    float photon_pt = 0; float photon_eta = 0;
    int globalbin_photon = 0;
    photon_pt = ntuple->Photons->at(0).Pt(); photon_eta = ntuple->Photons->at(0).Eta();
    if (photon_pt>500) photon_pt=499.9;
    globalbin_photon  = hSFeff_[0]->FindBin(photon_eta, photon_pt);
    double eff = hSFeff_[0]->GetBinContent(globalbin_photon);
    effWt *= eff;
    effSys = eff > 0 ? quadSum(effSys, hSFeff_[0]->GetBinError(globalbin_photon)/eff) : effSys;
  }
  // return pair<double, double>(effWt, effSys);
  return effWt;
}

void clearGlobalConstants() {
  eventMET = 0.0; eventMET_phi = 0.0;
  eventHT = 0.0;
  jetPt1=1.0; jetPt2=1.0;
  jetMass1 = 1.0; jetMass2 = 1.0;
  thisDeltaPhi1=10.0; thisDeltaPhi2=10.0; thisDeltaPhi3=10.0; thisDeltaPhi4=10.0;
  numIsoMuonTracks = 0; numIsoElectronTracks = 0; numIsoPionTracks = 0;
}




//////////////////////////////////////////////
///          Trigger Efficiencies          ///
//////////////////////////////////////////////
//From Michael
//I've removed all 0l with MET<300 GeV, since 0l and 1l both require MET>300
//Keeping down to 200 for 1l just in case

float trigEff_0l(TString year, float met, float ht, TString whichSyst) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.; errup+=errdown; //suppress unused warning
  if (year=="MC2016") {
    if (ht> 0 && ht<= 200 && met> 200) {eff = 0.927419; errup = 0.0725806; errdown = 0.0731884;}
    else if (ht> 200 && ht<= 300 && met> 275) {eff = 1; errup = 0; errdown = 0.00305686;}
    else if (ht> 300 && ht<= 400 && met> 300) {eff = 1; errup = 0; errdown = 0;}
    else if (ht> 400 && ht<= 600 && met> 300) {eff = 1; errup = 0; errdown = 0;}
    else if (ht> 600 && ht<= 950 && met> 300) {eff = 1; errup = 0; errdown = 9.2029e-05;}
    else if (ht> 950 && met> 300) {eff = 1; errup = 0; errdown = 2.32418e-05;}
  }
  else if (year=="MC2017") {
    if (ht> 0 && ht<= 200 && met> 200 ) {eff = 0.574468; errup = 0.130371; errdown = 0.130371;}
    else if (ht> 200 && ht<= 300 && met> 275 ) {eff = 0.977778; errup = 0.0222222; errdown = 0.0297024;}
    else if (ht> 300 && ht<= 400 && met> 300 ) {eff = 0.984848; errup = 0.0151515; errdown = 0.0912803;}
    else if (ht> 400 && ht<= 600 && met> 300 ) {eff = 0.998663; errup = 0.0013369; errdown = 0.0840732;}
    else if (ht> 600 && ht<= 950 && met> 300 ) {eff = 0.997106; errup = 0.00289436; errdown = 0.0843903;}
    else if (ht> 950 && met> 275 && met<= 300) {eff = 0.972222; errup = 0.0277778; errdown = 0.0335147;}
    else if (ht> 950 && met> 300 ) {eff = 1; errup = 0; errdown = 0.089518;}
  }
  else if (year=="MC2018") {
    if (ht> 0 && ht<= 200 && met> 200 ) {eff = 0.336957; errup = 0.0757904; errdown = 0.0757904;}
    else if (ht> 200 && ht<= 300 && met> 275 ) {eff = 0.972222; errup = 0.0277778; errdown = 0.0842695;}
    else if (ht> 300 && ht<= 400 && met> 300 ) {eff = 0.99; errup = 0.01; errdown = 0.0169057;}
    else if (ht> 400 && ht<= 600 && met> 300 ) {eff = 1; errup = 0; errdown = 0.00358039;}
    else if (ht> 600 && ht<= 950 && met> 300 ) {eff = 0.996324; errup = 0.00367647; errdown = 0.00711638;}
    else if (ht> 950 && met> 300 ) {eff = 1; errup = 0; errdown = 0.021753;}
  }
  if (whichSyst=="none") return eff;
  else if (whichSyst=="up") return eff+errup;
  else if (whichSyst=="down") return eff-errdown;
}

float trigEff_0lFake(TString year, float met, float ht) {
  //Fake MET for QCD
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.; errup+=errdown; //suppress unused warning
  if (year=="MC2016") {
    if (ht> 0 && ht<= 350 && met> 250 ) {eff = 0.73913; errup = 0.0915605; errdown = 0.0915605;}
    else if (ht> 350 && ht<= 450 && met> 300) {eff = 0.755556; errup = 0.0640644; errdown = 0.0640644;}
    else if (ht> 450 && ht<= 550 && met> 300 && met<= 400) {eff = 0.609626; errup = 0.035674; errdown = 0.035674;}
    else if (ht> 450 && ht<= 550 && met> 400) {eff = 0.926471; errup = 0.0316513; errdown = 0.0316513;}
    else if (ht> 550 && ht<= 650 && met> 300 && met<= 400) {eff = 0.704819; errup = 0.017701; errdown = 0.017701;}
    else if (ht> 550 && ht<= 650 && met> 400) {eff = 0.955; errup = 0.00732931; errdown = 0.00732931;}
    else if (ht> 650 && ht<= 800 && met> 300 && met<= 350) {eff = 0.844642; errup = 0.00697529; errdown = 0.00697529;}
    else if (ht> 650 && ht<= 800 && met> 350 && met<= 400) {eff = 0.904545; errup = 0.00748778; errdown = 0.00748778;}
    else if (ht> 650 && ht<= 800 && met> 400 && met<= 450) {eff = 0.953261; errup = 0.00695909; errdown = 0.00695909;}
    else if (ht> 650 && ht<= 800 && met> 450 && met<= 500) {eff = 0.969849; errup = 0.00699865; errdown = 0.00699865;}
    else if (ht> 650 && ht<= 800 && met> 500) {eff = 0.990217; errup = 0.00324488; errdown = 0.00324488;}
    else if (ht> 800 && ht<= 1000 && met> 300 && met<= 350) {eff = 0.905381; errup = 0.00318308; errdown = 0.00318308;}
    else if (ht> 800 && ht<= 1000 && met> 350 && met<= 400) {eff = 0.927529; errup = 0.00414893; errdown = 0.00414893;}
    else if (ht> 800 && ht<= 1000 && met> 400 && met<= 450) {eff = 0.947605; errup = 0.00497749; errdown = 0.00497749;}
    else if (ht> 800 && ht<= 1000 && met> 450 && met<= 500) {eff = 0.967402; errup = 0.00549868; errdown = 0.00549868;}
    else if (ht> 800 && ht<= 1000 && met> 500) {eff = 0.985313; errup = 0.0030399; errdown = 0.0030399;}
    else if (ht> 1000 && met> 300 && met<= 350) {eff = 0.805159; errup = 0.00296283; errdown = 0.00296283;}
    else if (ht> 1000 && met> 350 && met<= 400) {eff = 0.847872; errup = 0.00376673; errdown = 0.00376673;}
    else if (ht> 1000 && met> 400 && met<= 450) {eff = 0.875209; errup = 0.00478206; errdown = 0.00478206;}
    else if (ht> 1000 && met> 450 && met<= 500) {eff = 0.888848; errup = 0.00603014; errdown = 0.00603014;}
    else if (ht> 1000 && met> 500) {eff = 0.92654; errup = 0.00382917; errdown = 0.00382917;}
  }
  else if (year=="MC2017") {
    if (ht> 0 && ht<= 350 && met> 250 ) {eff = 0.818182; errup = 0.116291; errdown = 0.116291;}
    else if (ht> 350 && ht<= 450 && met> 300 ) {eff = 0.954545; errup = 0.0314022; errdown = 0.0314022;}
    else if (ht> 450 && ht<= 550 && met> 300 && met<= 400) {eff = 0.975309; errup = 0.0121923; errdown = 0.0121923;}
    else if (ht> 450 && ht<= 550 && met> 400 ) {eff = 0.966667; errup = 0.0231741; errdown = 0.0231741;}
    else if (ht> 550 && ht<= 650 && met> 300 && met<= 400) {eff = 0.933333; errup = 0.0117589; errdown = 0.0117589;}
    else if (ht> 550 && ht<= 650 && met> 400 ) {eff = 0.978102; errup = 0.00510454; errdown = 0.00510454;}
    else if (ht> 650 && ht<= 800 && met> 300 && met<= 350) {eff = 0.941468; errup = 0.00522821; errdown = 0.00522821;}
    else if (ht> 650 && ht<= 800 && met> 350 && met<= 400) {eff = 0.959596; errup = 0.00528902; errdown = 0.00528902;}
    else if (ht> 650 && ht<= 800 && met> 400 && met<= 450) {eff = 0.976963; errup = 0.00485453; errdown = 0.00485453;}
    else if (ht> 650 && ht<= 800 && met> 450 && met<= 500) {eff = 0.991124; errup = 0.00360739; errdown = 0.00360739;}
    else if (ht> 650 && ht<= 800 && met> 500 ) {eff = 0.973333; errup = 0.00515956; errdown = 0.00515956;}
    else if (ht> 800 && ht<= 1000 && met> 300 && met<= 350) {eff = 0.941998; errup = 0.0028653; errdown = 0.0028653;}
    else if (ht> 800 && ht<= 1000 && met> 350 && met<= 400) {eff = 0.952878; errup = 0.00376834; errdown = 0.00376834;}
    else if (ht> 800 && ht<= 1000 && met> 400 && met<= 450) {eff = 0.963603; errup = 0.00465149; errdown = 0.00465149;}
    else if (ht> 800 && ht<= 1000 && met> 450 && met<= 500) {eff = 0.954495; errup = 0.00694311; errdown = 0.00694311;}
    else if (ht> 800 && ht<= 1000 && met> 500 ) {eff = 0.983751; errup = 0.00328979; errdown = 0.00328979;}
    else if (ht> 1000 && met> 300 && met<= 350) {eff = 0.916497; errup = 0.00185908; errdown = 0.00185908;}
    else if (ht> 1000 && met> 350 && met<= 400) {eff = 0.936176; errup = 0.00234582; errdown = 0.00234582;}
    else if (ht> 1000 && met> 400 && met<= 450) {eff = 0.947549; errup = 0.00292349; errdown = 0.00292349;}
    else if (ht> 1000 && met> 450 && met<= 500) {eff = 0.954474; errup = 0.0037065; errdown = 0.0037065;}
    else if (ht> 1000 && met> 500 ) {eff = 0.967807; errup = 0.00238727; errdown = 0.00238727;}
  }
  else if (year=="MC2018") {
    if (ht> 0 && ht<= 350 && met> 250 ) {eff = 0.823529; errup = 0.0924594; errdown = 0.0924594;}
    else if (ht> 350 && ht<= 450 && met> 300 ) {eff = 0.973684; errup = 0.0259672; errdown = 0.0259672;}
    else if (ht> 450 && ht<= 550 && met> 300 && met<= 400) {eff = 0.948387; errup = 0.0177708; errdown = 0.0177708;}
    else if (ht> 450 && ht<= 550 && met> 400 ) {eff = 0.959459; errup = 0.0229267; errdown = 0.0229267;}
    else if (ht> 550 && ht<= 650 && met> 300 && met<= 400) {eff = 0.887029; errup = 0.014479; errdown = 0.014479;}
    else if (ht> 550 && ht<= 650 && met> 400 ) {eff = 0.977528; errup = 0.00473689; errdown = 0.00473689;}
    else if (ht> 650 && ht<= 800 && met> 300 && met<= 350) {eff = 0.934977; errup = 0.00508305; errdown = 0.00508305;}
    else if (ht> 650 && ht<= 800 && met> 350 && met<= 400) {eff = 0.95888; errup = 0.00484746; errdown = 0.00484746;}
    else if (ht> 650 && ht<= 800 && met> 400 && met<= 450) {eff = 0.969673; errup = 0.00484455; errdown = 0.00484455;}
    else if (ht> 650 && ht<= 800 && met> 450 && met<= 500) {eff = 0.979543; errup = 0.00491061; errdown = 0.00491061;}
    else if (ht> 650 && ht<= 800 && met> 500 ) {eff = 0.986716; errup = 0.00311023; errdown = 0.00311023;}
    else if (ht> 800 && ht<= 1000 && met> 300 && met<= 350) {eff = 0.943467; errup = 0.0025356; errdown = 0.0025356;}
    else if (ht> 800 && ht<= 1000 && met> 350 && met<= 400) {eff = 0.952579; errup = 0.00332293; errdown = 0.00332293;}
    else if (ht> 800 && ht<= 1000 && met> 400 && met<= 450) {eff = 0.968186; errup = 0.00382436; errdown = 0.00382436;}
    else if (ht> 800 && ht<= 1000 && met> 450 && met<= 500) {eff = 0.969146; errup = 0.00486385; errdown = 0.00486385;}
    else if (ht> 800 && ht<= 1000 && met> 500 ) {eff = 0.987481; errup = 0.00248803; errdown = 0.00248803;}
    else if (ht> 1000 && met> 300 && met<= 350) {eff = 0.927802; errup = 0.00151072; errdown = 0.00151072;}
    else if (ht> 1000 && met> 350 && met<= 400) {eff = 0.941449; errup = 0.00195205; errdown = 0.00195205;}
    else if (ht> 1000 && met> 400 && met<= 450) {eff = 0.944989; errup = 0.00260629; errdown = 0.00260629;}
    else if (ht> 1000 && met> 450 && met<= 500) {eff = 0.954409; errup = 0.00317334; errdown = 0.00317334;}
    else if (ht> 1000 && met> 500 ) {eff = 0.960595; errup = 0.00225241; errdown = 0.00225241;}
  }
  return eff;
}

float trigEff_el_2016(float met, float ht, float el_pt) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.;
  errup+=errdown; //suppress unused warning
  if (ht> 0 && ht<= 400 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.123222;}
  else if (ht> 400 && ht<= 600 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0709947;}
  else if (ht> 600 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.95572; errup = 0.00884752; errdown = 0.0107028;}
  else if (ht> 0 && ht<= 400 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.115502;}
  else if (ht> 400 && ht<= 600 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.96875; errup = 0.025866; errdown = 0.0682225;}
  else if (ht> 600 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.974895; errup = 0.00709912; errdown = 0.00937376;}
  else if (ht> 0 && ht<= 400 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.102638;}
  else if (ht> 400 && ht<= 600 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0376284;}
  else if (ht> 600 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.982256; errup = 0.00466259; errdown = 0.00604897;}
  else if (ht> 0 && ht<= 400 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.993333; errup = 0.00551564; errdown = 0.0151622;}
  else if (ht> 400 && ht<= 600 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.990291; errup = 0.00527929; errdown = 0.00935364;}
  else if (ht> 600 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.981423; errup = 0.00219908; errdown = 0.00247001;}
  else if (ht> 0 && ht<= 400 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.205568;}
  else if (ht> 400 && ht<= 600 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0419109;}
  else if (ht> 600 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 0.994505; errup = 0.00354816; errdown = 0.00720076;}
  else if (ht> 0 && ht<= 400 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0879414;}
  else if (ht> 400 && ht<= 600 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0180628;}
  else if (ht> 600 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 0.992408; errup = 0.00279574; errdown = 0.00406535;}
  else if (ht> 0 && ht<= 400 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.168149;}
  else if (ht> 400 && ht<= 600 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.108691;}
  else if (ht> 600 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 0.991718; errup = 0.00395933; errdown = 0.00649962;}
  else if (ht> 0 && ht<= 400 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.264229;}
  else if (ht> 400 && ht<= 600 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0839348;}
  else if (ht> 600 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 0.986143; errup = 0.00548298; errdown = 0.00818471;}
  else if (ht> 0 && ht<= 400 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 1; errup = 0; errdown = 0.115502;}
  else if (ht> 400 && ht<= 600 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0559083;}
  else if (ht> 600 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 0.99409; errup = 0.00255052; errdown = 0.00397839;}
  else if (ht> 0 && ht<= 400 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0173807;}
  else if (ht> 400 && ht<= 600 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 0.996441; errup = 0.00294413; errdown = 0.00813542;}
  else if (ht> 600 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 0.994857; errup = 0.0011116; errdown = 0.00138053;}
  else if (ht> 0 && ht<= 400 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.458642;}
  else if (ht> 400 && ht<= 600 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0498539;}
  else if (ht> 600 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 0.995455; errup = 0.00293541; errdown = 0.00596358;}
  else if (ht> 0 && ht<= 400 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.142229;}
  else if (ht> 400 && ht<= 600 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0149771;}
  else if (ht> 600 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 0.998031; errup = 0.00127137; errdown = 0.0025904;}
  return eff;
}

float trigEff_mu_2016(float met, float ht, float mu_pt) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.;
  errup+=errdown; //suppress unused warning
  if (ht> 0 && ht<= 400 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.123222;}
  else if (ht> 400 && ht<= 600 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.973684; errup = 0.02178; errdown = 0.0579268;}
  else if (ht> 600 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.982704; errup = 0.00511276; errdown = 0.00685994;}
  else if (ht> 0 && ht<= 400 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0802771;}
  else if (ht> 400 && ht<= 600 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0512411;}
  else if (ht> 600 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 0.998138; errup = 0.00154055; errdown = 0.00426903;}
  else if (ht> 0 && ht<= 400 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0361508;}
  else if (ht> 400 && ht<= 600 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 0.991379; errup = 0.00713254; errdown = 0.019543;}
  else if (ht> 600 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 0.997409; errup = 0.00111857; errdown = 0.00174877;}
  else if (ht> 0 && ht<= 400 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 0.985294; errup = 0.0121686; errdown = 0.0330032;}
  else if (ht> 400 && ht<= 600 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.00811302;}
  else if (ht> 600 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 0.996846; errup = 0.000979628; errdown = 0.00134259;}
  else if (ht> 0 && ht<= 400 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.205568;}
  else if (ht> 400 && ht<= 600 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0738409;}
  else if (ht> 600 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 0.996764; errup = 0.00209004; errdown = 0.00425238;}
  else if (ht> 0 && ht<= 400 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.23126;}
  else if (ht> 400 && ht<= 600 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0559083;}
  else if (ht> 600 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 0.994709; errup = 0.0028782; errdown = 0.00511987;}
  else if (ht> 0 && ht<= 400 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0498539;}
  else if (ht> 400 && ht<= 600 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0172182;}
  else if (ht> 600 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 0.997996; errup = 0.000958848; errdown = 0.0015817;}
  else if (ht> 0 && ht<= 400 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0335184;}
  else if (ht> 400 && ht<= 600 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.00783675;}
  else if (ht> 600 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 0.999413; errup = 0.000378822; errdown = 0.00077304;}
  return eff;
}

float trigEff_el_2017(float met, float ht, float el_pt) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.; errup+=errdown; //suppress unused warning
  if (ht> 0 && ht<= 400 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.8; errup = 0.106751; errdown = 0.157061;}
  else if (ht> 400 && ht<= 600 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.916667; errup = 0.0536391; errdown = 0.0995072;}
  else if (ht> 600 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.955645; errup = 0.0130229; errdown = 0.017253;}
  else if (ht> 0 && ht<= 400 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.769231; errup = 0.122762; errdown = 0.174724;}
  else if (ht> 400 && ht<= 600 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.869565; errup = 0.0701228; errdown = 0.110814;}
  else if (ht> 600 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.920833; errup = 0.0175659; errdown = 0.0214819;}
  else if (ht> 0 && ht<= 400 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.944444; errup = 0.046004; errdown = 0.116415;}
  else if (ht> 400 && ht<= 600 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.97619; errup = 0.0197048; errdown = 0.0526298;}
  else if (ht> 600 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.961039; errup = 0.00899222; errdown = 0.0112252;}
  else if (ht> 0 && ht<= 400 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.950413; errup = 0.0194949; errdown = 0.0284436;}
  else if (ht> 400 && ht<= 600 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.978022; errup = 0.0104893; errdown = 0.0170363;}
  else if (ht> 600 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.969503; errup = 0.00381731; errdown = 0.00430889;}
  else if (ht> 0 && ht<= 400 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 0.916667; errup = 0.0690403; errdown = 0.16652;}
  else if (ht> 400 && ht<= 600 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0802771;}
  else if (ht> 600 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 0.964706; errup = 0.013912; errdown = 0.0204859;}
  else if (ht> 0 && ht<= 400 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 0.966667; errup = 0.0275914; errdown = 0.072517;}
  else if (ht> 400 && ht<= 600 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0384134;}
  else if (ht> 600 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 0.980815; errup = 0.00661202; errdown = 0.00932515;}
  else if (ht> 0 && ht<= 400 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.205568;}
  else if (ht> 400 && ht<= 600 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 0.913043; errup = 0.0559625; errdown = 0.103371;}
  else if (ht> 600 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 0.981651; errup = 0.00725455; errdown = 0.0107985;}
  else if (ht> 0 && ht<= 400 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.205568;}
  else if (ht> 400 && ht<= 600 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0769247;}
  else if (ht> 600 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 0.984674; errup = 0.00732055; errdown = 0.0119517;}
  else if (ht> 0 && ht<= 400 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0659133;}
  else if (ht> 400 && ht<= 600 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 1; errup = 0; errdown = 0.042887;}
  else if (ht> 600 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 0.997938; errup = 0.00170573; errdown = 0.00472518;}
  else if (ht> 0 && ht<= 400 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 0.990099; errup = 0.00819202; errdown = 0.0223979;}
  else if (ht> 400 && ht<= 600 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 1; errup = 0; errdown = 0.00964279;}
  else if (ht> 600 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 0.992439; errup = 0.00171528; errdown = 0.00215231;}
  else if (ht> 0 && ht<= 400 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.205568;}
  else if (ht> 400 && ht<= 600 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.115502;}
  else if (ht> 600 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.00804214;}
  else if (ht> 0 && ht<= 400 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.108691;}
  else if (ht> 400 && ht<= 600 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0283562;}
  else if (ht> 600 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 0.998397; errup = 0.00132575; errdown = 0.0036754;}
  return eff;
}

float trigEff_mu_2017(float met, float ht, float mu_pt) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.; errup+=errdown; //suppress unused warning
  if (ht> 0 && ht<= 400 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.142229;}
  else if (ht> 400 && ht<= 600 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.888889; errup = 0.0598486; errdown = 0.0963981;}
  else if (ht> 600 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.98374; errup = 0.00643122; errdown = 0.00958564;}
  else if (ht> 0 && ht<= 400 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 0.947368; errup = 0.0435805; errdown = 0.110836;}
  else if (ht> 400 && ht<= 600 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0738409;}
  else if (ht> 600 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 0.988473; errup = 0.00550887; errdown = 0.00902056;}
  else if (ht> 0 && ht<= 400 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0392319;}
  else if (ht> 400 && ht<= 600 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0182418;}
  else if (ht> 600 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 0.990955; errup = 0.00295229; errdown = 0.00410363;}
  else if (ht> 0 && ht<= 400 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0249041;}
  else if (ht> 400 && ht<= 600 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 0.993711; errup = 0.0052034; errdown = 0.0143129;}
  else if (ht> 600 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 0.995131; errup = 0.00168324; errdown = 0.00239245;}
  else if (ht> 0 && ht<= 400 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.205568;}
  else if (ht> 400 && ht<= 600 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0709947;}
  else if (ht> 600 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 0.992788; errup = 0.00392226; errdown = 0.00696502;}
  else if (ht> 0 && ht<= 400 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.15411;}
  else if (ht> 400 && ht<= 600 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0972223;}
  else if (ht> 600 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 0.99505; errup = 0.00319693; errdown = 0.00649192;}
  else if (ht> 0 && ht<= 400 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0542609;}
  else if (ht> 400 && ht<= 600 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0224723;}
  else if (ht> 600 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 0.994958; errup = 0.00199818; errdown = 0.00299934;}
  else if (ht> 0 && ht<= 400 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0461088;}
  else if (ht> 400 && ht<= 600 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.012972;}
  else if (ht> 600 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 0.997174; errup = 0.00112047; errdown = 0.0016842;}
  return eff;
}

float trigEff_el_2018(float met, float ht, float el_pt) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.; errup+=errdown; //suppress unused warning
  if (ht> 0 && ht<= 400 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.666667; errup = 0.277375; errdown = 0.414535;}
  else if (ht> 400 && ht<= 600 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.8; errup = 0.0835235; errdown = 0.112668;}
  else if (ht> 600 && el_pt> 20 && el_pt<= 25 && met> 175 && met<= 215) {eff = 0.952381; errup = 0.0116187; errdown = 0.0146509;}
  else if (ht> 0 && ht<= 400 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.777778; errup = 0.142118; errdown = 0.221429;}
  else if (ht> 400 && ht<= 600 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.8; errup = 0.0835235; errdown = 0.112668;}
  else if (ht> 600 && el_pt> 25 && el_pt<= 30 && met> 175 && met<= 215) {eff = 0.983444; errup = 0.00713312; errdown = 0.0110449;}
  else if (ht> 0 && ht<= 400 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.9; errup = 0.0643201; errdown = 0.116971;}
  else if (ht> 400 && ht<= 600 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.918919; errup = 0.0438002; errdown = 0.0726265;}
  else if (ht> 600 && el_pt> 30 && el_pt<= 40 && met> 175 && met<= 215) {eff = 0.965649; errup = 0.00793934; errdown = 0.00992767;}
  else if (ht> 0 && ht<= 400 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.957983; errup = 0.0180304; errdown = 0.027424;}
  else if (ht> 400 && ht<= 600 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.960199; errup = 0.0136568; errdown = 0.0190433;}
  else if (ht> 600 && el_pt> 40 && el_pt<= 110 && met> 175 && met<= 215) {eff = 0.984898; errup = 0.00245839; errdown = 0.00288698;}
  else if (ht> 0 && ht<= 400 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.168149;}
  else if (ht> 400 && ht<= 600 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 0.95; errup = 0.0413995; errdown = 0.105764;}
  else if (ht> 600 && el_pt> 110 && el_pt<= 120 && met> 175 && met<= 215) {eff = 0.975845; errup = 0.0103945; errdown = 0.0160098;}
  else if (ht> 0 && ht<= 400 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0576587;}
  else if (ht> 400 && ht<= 600 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0472931;}
  else if (ht> 600 && el_pt> 120 && el_pt<= 9999 && met> 175 && met<= 215) {eff = 0.994718; errup = 0.00287314; errdown = 0.0051109;}
  else if (ht> 0 && ht<= 400 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.15411;}
  else if (ht> 400 && ht<= 600 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0879414;}
  else if (ht> 600 && el_pt> 20 && el_pt<= 25 && met> 215 ) {eff = 0.991758; errup = 0.00448218; errdown = 0.00795189;}
  else if (ht> 0 && ht<= 400 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.264229;}
  else if (ht> 400 && ht<= 600 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 0.947368; errup = 0.0435805; errdown = 0.110836;}
  else if (ht> 600 && el_pt> 25 && el_pt<= 30 && met> 215 ) {eff = 0.988201; errup = 0.00563868; errdown = 0.00923117;}
  else if (ht> 0 && ht<= 400 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 1; errup = 0; errdown = 0.184992;}
  else if (ht> 400 && ht<= 600 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 0.979592; errup = 0.0168888; errdown = 0.0453679;}
  else if (ht> 600 && el_pt> 30 && el_pt<= 40 && met> 215 ) {eff = 0.995208; errup = 0.00260705; errdown = 0.00463962;}
  else if (ht> 0 && ht<= 400 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0200277;}
  else if (ht> 400 && ht<= 600 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0104058;}
  else if (ht> 600 && el_pt> 40 && el_pt<= 110 && met> 215 ) {eff = 0.996068; errup = 0.00111791; errdown = 0.00148999;}
  else if (ht> 0 && ht<= 400 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.308024;}
  else if (ht> 400 && ht<= 600 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0802771;}
  else if (ht> 600 && el_pt> 110 && el_pt<= 120 && met> 215 ) {eff = 0.993266; errup = 0.00434837; errdown = 0.00881245;}
  else if (ht> 0 && ht<= 400 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.102638;}
  else if (ht> 400 && ht<= 600 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0271039;}
  else if (ht> 600 && el_pt> 120 && el_pt<= 9999 && met> 215 ) {eff = 0.99867; errup = 0.00110009; errdown = 0.00305118;}
  return eff;
}

float trigEff_mu_2018(float met, float ht, float mu_pt) {
  float errup=0., errdown=0.; // Not used, but for reference
  float eff = 1.; errup+=errdown; //suppress unused warning
  if (ht> 0 && ht<= 400 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.9; errup = 0.082873; errdown = 0.194135;}
  else if (ht> 400 && ht<= 600 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.9; errup = 0.0539222; errdown = 0.0877974;}
  else if (ht> 600 && mu_pt> 20 && mu_pt<= 25 && met> 175 && met<= 215) {eff = 0.989035; errup = 0.00472821; errdown = 0.00734954;}
  else if (ht> 0 && ht<= 400 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 0.857143; errup = 0.11848; errdown = 0.257124;}
  else if (ht> 400 && ht<= 600 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0659133;}
  else if (ht> 600 && mu_pt> 25 && mu_pt<= 30 && met> 175 && met<= 215) {eff = 0.994819; errup = 0.00334597; errdown = 0.00679283;}
  else if (ht> 0 && ht<= 400 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 0.971429; errup = 0.0236478; errdown = 0.0626552;}
  else if (ht> 400 && ht<= 600 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 0.98913; errup = 0.00899357; errdown = 0.0245495;}
  else if (ht> 600 && mu_pt> 30 && mu_pt<= 50 && met> 175 && met<= 215) {eff = 0.989756; errup = 0.00279678; errdown = 0.00367595;}
  else if (ht> 0 && ht<= 400 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 0.967213; errup = 0.0211491; errdown = 0.0416131;}
  else if (ht> 400 && ht<= 600 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 1; errup = 0; errdown = 0.0133482;}
  else if (ht> 600 && mu_pt> 50 && mu_pt<= 9999 && met> 175 && met<= 215) {eff = 0.99708; errup = 0.00115753; errdown = 0.0017398;}
  else if (ht> 0 && ht<= 400 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.601684;}
  else if (ht> 400 && ht<= 600 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0769247;}
  else if (ht> 600 && mu_pt> 20 && mu_pt<= 25 && met> 215 ) {eff = 1; errup = 0; errdown = 0.00350724;}
  else if (ht> 0 && ht<= 400 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.601684;}
  else if (ht> 400 && ht<= 600 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0802771;}
  else if (ht> 600 && mu_pt> 25 && mu_pt<= 30 && met> 215 ) {eff = 0.995204; errup = 0.00309728; errdown = 0.00629067;}
  else if (ht> 0 && ht<= 400 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0636358;}
  else if (ht> 400 && ht<= 600 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 1; errup = 0; errdown = 0.016449;}
  else if (ht> 600 && mu_pt> 30 && mu_pt<= 50 && met> 215 ) {eff = 0.998025; errup = 0.00107467; errdown = 0.00191738;}
  else if (ht> 0 && ht<= 400 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.0439098;}
  else if (ht> 400 && ht<= 600 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 1; errup = 0; errdown = 0.01428;}
  else if (ht> 600 && mu_pt> 50 && mu_pt<= 9999 && met> 215 ) {eff = 0.998493; errup = 0.000720896; errdown = 0.00118964;}
  return eff;
}


float triggerEfficiencies(TString region, TString year, float met, float ht, float lept_pt, TString whichSyst) {
  float trigEff = 1.0;
  if (region=="0l") trigEff = trigEff_0l(year,met,ht,whichSyst);
  else if (region=="fake") trigEff = trigEff_0lFake(year,met,ht);
  else if (region=="el") {
    if (year=="MC2016") trigEff = trigEff_el_2016(met,ht,lept_pt);
    else if (year=="MC2017") trigEff = trigEff_el_2017(met,ht,lept_pt);
    else if (year=="MC2018") trigEff = trigEff_el_2018(met,ht,lept_pt);
  }
  else if (region=="mu") {
    if (year=="MC2016") trigEff = trigEff_mu_2016(met,ht,lept_pt);
    else if (year=="MC2017") trigEff = trigEff_mu_2017(met,ht,lept_pt);
    else if (year=="MC2018") trigEff = trigEff_mu_2018(met,ht,lept_pt);
  }
  return trigEff;
}

double higgsinoCrossSection1D(TString hig_mass) {
  double xsec = 1.0;
  if (hig_mass=="127") xsec = 7.6022;
  else if (hig_mass =="150") xsec = 3.83231;
  else if (hig_mass =="175") xsec = 2.26794;
  else if (hig_mass =="200") xsec = 1.33562;
  else if (hig_mass =="225") xsec = 0.860597;
  else if (hig_mass =="250") xsec = 0.577314;
  else if (hig_mass =="275") xsec = 0.400107;
  else if (hig_mass =="300") xsec = 0.284855;
  else if (hig_mass =="325") xsec = 0.20736;
  else if (hig_mass =="350") xsec = 0.153841;
  else if (hig_mass =="375") xsec = 0.116006;
  else if (hig_mass =="400") xsec = 0.0887325;
  else if (hig_mass =="425") xsec = 0.0686963;
  else if (hig_mass =="450") xsec = 0.0537702;
  else if (hig_mass =="475") xsec = 0.0424699;
  else if (hig_mass =="500") xsec = 0.0338387;
  else if (hig_mass =="525") xsec = 0.0271867;
  else if (hig_mass =="550") xsec = 0.0219868;
  else if (hig_mass =="575") xsec = 0.0179062;
  else if (hig_mass =="600") xsec = 0.0146677;
  else if (hig_mass =="625") xsec = 0.012062;
  else if (hig_mass =="650") xsec = 0.00996406;
  else if (hig_mass =="675") xsec = 0.00828246;
  else if (hig_mass =="700") xsec = 0.00689981;
  else if (hig_mass =="725") xsec = 0.00578355;
  else if (hig_mass =="750") xsec = 0.0048731;
  else if (hig_mass =="775") xsec = 0.00409781;
  else if (hig_mass =="800") xsec = 0.00346143;
  else if (hig_mass =="825") xsec = 0.0029337;
  else if (hig_mass =="850") xsec = 0.0024923;
  else if (hig_mass =="875") xsec = 0.00213679;
  else if (hig_mass =="900") xsec = 0.00180616;
  else if (hig_mass =="925") xsec = 0.00155453;
  else if (hig_mass =="950") xsec = 0.00132692;
  else if (hig_mass =="975") xsec = 0.00112975;
  else if (hig_mass =="1000") xsec = 0.000968853;
  else if (hig_mass =="1025") xsec = 0.000840602;
  else if (hig_mass =="1050") xsec = 0.000731306;
  else if (hig_mass =="1075") xsec = 0.000627083;
  else if (hig_mass =="1100") xsec = 0.000538005;
  else if (hig_mass =="1125") xsec = 0.00046747;
  else if (hig_mass =="1150") xsec = 0.000405108;
  else if (hig_mass =="1175") xsec = 0.000348261;
  else if (hig_mass =="1200") xsec = 0.000299347;
  else if (hig_mass =="1225") xsec = 0.000265935;
  else if (hig_mass =="1250") xsec = 0.000240471;
  else if (hig_mass =="1275") xsec = 0.000190411;
  else if (hig_mass =="1300") xsec = 0.000160765;
  else if (hig_mass =="1325") xsec = 0.000136272;
  else if (hig_mass =="1350") xsec = 0.000111174;
  else if (hig_mass =="1375") xsec = 9.74728e-05;
  else if (hig_mass =="1400") xsec = 7.80263e-05;
  else if (hig_mass =="1425") xsec = 6.96843e-05;
  else if (hig_mass =="1450") xsec = 6.96962e-05;
  else if (hig_mass =="1475") xsec = 4.98006e-05;
  else xsec = 0;
  return xsec*1000.0;
}

double higgsinoCrossSection2D(TString hig_mass) {
  double xsec = 1.0;
  if (hig_mass =="127") xsec = 1.44725;
  else if (hig_mass =="150") xsec = 0.71514;
  else if (hig_mass =="175") xsec = 0.419059;
  else if (hig_mass =="200") xsec = 0.244213;
  else if (hig_mass =="225") xsec = 0.156286;
  else if (hig_mass =="250") xsec = 0.104252;
  else if (hig_mass =="275") xsec = 0.0719125;
  else if (hig_mass =="300") xsec = 0.0509994;
  else if (hig_mass =="325") xsec = 0.0369715;
  else if (hig_mass =="350") xsec = 0.0273286;
  else if (hig_mass =="375") xsec = 0.0205429;
  else if (hig_mass =="400") xsec = 0.0156691;
  else if (hig_mass =="425") xsec = 0.0120965;
  else if (hig_mass =="450") xsec = 0.00944017;
  else if (hig_mass =="475") xsec = 0.00743587;
  else if (hig_mass =="500") xsec = 0.00590757;
  else if (hig_mass =="525") xsec = 0.00469101;
  else if (hig_mass =="550") xsec = 0.0038167;
  else if (hig_mass =="575") xsec = 0.003073;
  else if (hig_mass =="600") xsec = 0.00253015;
  else if (hig_mass =="625") xsec = 0.00206136;
  else if (hig_mass =="650") xsec = 0.00171418;
  else if (hig_mass =="675") xsec = 0.00140934;
  else if (hig_mass =="700") xsec = 0.00118113;
  else if (hig_mass =="725") xsec = 0.000979349;
  else if (hig_mass =="750") xsec = 0.000826366;
  else if (hig_mass =="775") xsec = 0.000690208;
  else if (hig_mass =="800") xsec = 0.000586211;
  else if (hig_mass =="825") xsec = 0.00049277;
  else if (hig_mass =="850") xsec = 0.000420556;
  else if (hig_mass =="875") xsec = 0.000358734;
  else if (hig_mass =="900") xsec = 0.000305935;
  else if (hig_mass =="925") xsec = 0.000260948;
  else if (hig_mass =="950") xsec = 0.00022285;
  else if (hig_mass =="975") xsec = 0.000189681;
  else if (hig_mass =="1000") xsec = 0.00016428;
  else if (hig_mass =="1025") xsec = 0.000142206;
  else if (hig_mass =="1050") xsec = 0.000120971;
  else if (hig_mass =="1075") xsec = 0.000105301;
  else if (hig_mass =="1100") xsec = 9.12469e-05;
  else if (hig_mass =="1125") xsec = 7.9765e-05;
  else if (hig_mass =="1150") xsec = 6.78234e-05;
  else if (hig_mass =="1175") xsec = 5.9016e-05;
  else if (hig_mass =="1200") xsec = 5.16263e-05;
  else if (hig_mass =="1225") xsec = 4.5147e-05;
  else if (hig_mass =="1250") xsec = 3.88343e-05;
  else if (hig_mass =="1275") xsec = 3.41304e-05;
  else if (hig_mass =="1300") xsec = 2.99353e-05;
  else if (hig_mass =="1325") xsec = 2.63637e-05;
  else if (hig_mass =="1350") xsec = 2.26779e-05;
  else if (hig_mass =="1375") xsec = 1.99318e-05;
  else if (hig_mass =="1400") xsec = 1.75031e-05;
  else if (hig_mass =="1425") xsec = 1.53974e-05;
  else if (hig_mass =="1455") xsec = 1.3245e-05;
  else if (hig_mass =="1475") xsec = 1.16416e-05;
  else xsec = 0;
  return xsec*1000.0;
}

double reweightSignalXSEC(TString hig_mass) {
  //reweights 2D xsec to 1D xsec
  double xsec_1D = higgsinoCrossSection1D(hig_mass);
  double xsec_2D = higgsinoCrossSection2D(hig_mass);
  return (xsec_1D/xsec_2D);
}
