#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TPad.h"

#include <vector>
#include <map>
#include <iostream>
#include <assert.h>

#include "plotterUtils.cc"
#include "skimSamples.cc"
#include "definitions.cc"
#include "RA2bTree.cc"
#include "TriggerEfficiencySextet.cc"

using namespace std;
double BuildBCombinations( vector<TLorentzVector> Jets, std::vector<unsigned int >LooseTags, std::vector<unsigned int >MedTags,std::vector<unsigned int >TightTags){
//	if(nBoostedCandidates==1){
		TLorentzVector tempLead;
		TLorentzVector tempSubLead;
		//Highest Purity b-tags
		for(unsigned int t=0; t<TightTags.size(); ++t){
			unsigned int jetindex=TightTags.at(t);
		 	if(t==0)tempLead=Jets.at(jetindex);
			if(t==1)tempSubLead=Jets.at(jetindex); //Tight-Tight bb candidates
			if(t>1)break;
		}
		if(TightTags.size()<2){
		for(unsigned int m=0; m<MedTags.size(); ++m){
			unsigned int jetindex=MedTags[m];
			
		 	if(m==0 && MedTags.size()==0)tempLead=Jets.at(jetindex);
		 	if(m==0 && MedTags.size()==1)tempSubLead=Jets.at(jetindex); //Tight-Medium bb-candidates
			if(m==1 && MedTags.size()==0)tempSubLead=Jets.at(jetindex);//Medium-Medium bb candidates
			if(m>1)break;
		       }	
		}
	       	if(TightTags.size()<1 && MedTags.size()<1){ //don't have at least 1 Medium tag or 1 Tight Tag
			for(unsigned int l=0; l<LooseTags.size(); ++l){
				unsigned int jetindex=LooseTags[l];
				if(l>1)break;	
				if(l==1 && (MedTags.size()==1 || TightTags.size()==1 ))tempSubLead=Jets.at(jetindex); //Medium=Loose bb canddiate
				if(TightTags.size()>0)continue;
				if(MedTags.size()>0)continue;
				if(MedTags.size()==0 && l==0)tempLead=Jets.at(jetindex); 
				if(l==1)tempSubLead=Jets.at(jetindex); //Loose-Loose bb canddiate
			}
		}
		TLorentzVector ResolvedCandidate;		
		if(TightTags.size()+MedTags.size()+LooseTags.size()>2){//have at least 2 b-tags of 
			
			ResolvedCandidate=tempLead+tempSubLead;
		}	
		else{
		//finally what if there are no tags? Just take two AK8 Jets and build the mass
			if(Jets.size()>1){
			   	ResolvedCandidate=Jets.at(0)+Jets.at(1);		
			}

		}
	if(TightTags.size()+MedTags.size()+LooseTags.size()>2   || Jets.size()>1)std::cout<<"Mass of Candidate "<<ResolvedCandidate.M()<<std::endl;
	if(TightTags.size()+MedTags.size()+LooseTags.size()>2	|| Jets.size()>1)return ResolvedCandidate.M();
	else return -999.;	
//	}
}
int main(int argc, char** argv){

    skimSamples::region reg;
    int reg_(0);
    bool looseCuts(false);
    reg = static_cast<skimSamples::region>(reg_);
    
    gROOT->ProcessLine(".L tdrstyle.C");
    gROOT->ProcessLine("setTDRStyle()");
    
    skimSamples* skims_ = new skimSamples(reg);

    typedef bool(*cuts)(RA2bTree*);
    vector<cuts> baselineCuts;

    baselineCuts.push_back(*baselineCut<RA2bTree>);

    skimSamples skims = *skims_;
    TString regionName;
    TString cutName="baseline";
    regionName="signal";
    TFile* outputFile = new TFile("SkimFileMass"+cutName+regionName+".root","RECREATE");
    // background MC samples - 0 lepton regions
   for( int iSample = 0 ; iSample < skims.ntuples.size() ; iSample++){
	cout << skims.sampleName[iSample] << endl;
        RA2bTree* ntuple = skims.ntuples[iSample];
	int numEvents = ntuple->fChain->GetEntries();
	ntupleBranchStatus<RA2bTree>(ntuple);
 	TTree*newtree=(TTree*)ntuple->fChain->CloneTree(0);
	newtree->SetBranchStatus("*",0);
        newtree->SetBranchStatus("BTags",1);
        newtree->SetBranchStatus("MET",1);
        newtree->SetBranchStatus("JetsAK8*",1);
        newtree->SetBranchStatus("Jets",1);
        newtree->SetBranchStatus("Jets_bDiscriminatorCSV",1);
        newtree->SetBranchStatus("Weight",1);
	int nAK8=0;
	int nBoostedH=0;
	int nResolvedH=0;
	int BTagsL=0;
	int BTagsM=0;
	int BTagsT=0;
	vector<unsigned int> JetsIsoAK8Jets;
	TBranch*b_nAK8, *b_nBoostedH, *b_nResolvedH;
	TBranch*b_BTagsL, *b_BTagsM, *b_BTagsT;
	newtree->Branch("nAK8", &nAK8, "nAK8/I");
	newtree->Branch("BTagsL", &BTagsL, "BTagsL/I");
	newtree->Branch("BTagsM", &BTagsM, "BTagsM/I");
	newtree->Branch("BTagsT", &BTagsT, "BTagsT/I");
	newtree->Branch("nBoostedH", &nBoostedH, "nBoostedH/I");
	newtree->Branch("nResolvedH", &nResolvedH, "nResolvedH/I");
	newtree->SetBranchAddress("nAK8",&nAK8, &b_nAK8);
	newtree->SetBranchAddress("nBoostedH",&nBoostedH, &b_nBoostedH);
	newtree->SetBranchAddress("nResolvedH",&nResolvedH, &b_nResolvedH);
	newtree->SetBranchAddress("BTagsL", &BTagsL, &b_BTagsL);
	newtree->SetBranchAddress("BTagsM", &BTagsM, &b_BTagsM);
	newtree->SetBranchAddress("BTagsT", &BTagsT, &b_BTagsT);
	float minAK8JetPt=300;
	float bbtagCut=0.3;

	for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
	 ntuple->GetEntry(iEvt);
	if( iEvt % 100000 == 0 ) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;
	 if(!baselineCut(ntuple))continue;
	   nAK8=0;
	   nBoostedH=0;
	   nResolvedH=0;//this is not filled yet
	   JetsIsoAK8Jets.clear();
	   for(unsigned int fj=0; fj<ntuple->JetsAK8->size();++fj){
		if(ntuple->JetsAK8->at(fj).Pt()>minAK8JetPt)++nAK8;
		if(ntuple->JetsAK8_doubleBDiscriminator->at(fj)>bbtagCut && ntuple->JetsAK8_prunedMass->at(fj)>85 && ntuple->JetsAK8_prunedMass->at(fj)<135)++nBoostedH; 
	   }
	
    	   for(unsigned int j=0; j<ntuple->Jets->size();++j){
			if(ntuple->Jets->at(j).Pt()<30 || fabs(ntuple->Jets->at(j).Eta())>2.4)continue;
	   		for(unsigned int fj=0; fj<ntuple->JetsAK8->size();++fj){
				if(ntuple->JetsAK8_doubleBDiscriminator->at(fj)<bbtagCut && !(ntuple->JetsAK8_prunedMass->at(fj)>85 && ntuple->JetsAK8_prunedMass->at(fj)<135))continue; //match only to non H-tagged jets
				float deta=ntuple->Jets->at(j).Eta()-ntuple->JetsAK8->at(fj).Eta();
				float dphi=ntuple->Jets->at(j).Phi()-ntuple->JetsAK8->at(fj).Phi();
				float dR=sqrt((deta*deta)+(dphi*dphi));
				if(dR>0.8){
				JetsIsoAK8Jets.push_back(j);
		}
	    }
        }
//	std::cout<<"N AK4 Jets away from AK& Jets "<<JetsIsoAK8Jets.size()<<std::endl;
	//based on Moriond Recommnedations for comb CSV: https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco#Recommendation_for_combining_b_a 
	   double CSVBtagLoose=0.5426;
	   double CSVBtagMed= 0.8484 ;
	   double CSVBtagTight= 0.9535  ;
 	   //store jet indices for different levels of b-tag probability 
	   std::vector<unsigned int >LooseTags;
	   std::vector<unsigned int >MedTags;
	   std::vector<unsigned int >TightTags;
	   BTagsL=0;
	   BTagsM=0;
	   BTagsT=0;
	   vector<TLorentzVector> CleanedJets;	  
	   if(nBoostedH<2){ //now start to look for resolved canddiates  count btags in the event
    	     	for(unsigned int i=0; i<JetsIsoAK8Jets.size();++i){
			unsigned int j=JetsIsoAK8Jets.at(i);
			CleanedJets.push_back(ntuple->Jets->at(j));
			if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagLoose && ntuple->Jets_bDiscriminatorCSV->at(j)<CSVBtagMed){
				LooseTags.push_back(i);
				++BTagsL;
			}
			if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagMed && ntuple->Jets_bDiscriminatorCSV->at(j)<CSVBtagTight){
				++BTagsM;
				MedTags.push_back(i);
			}
			if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagTight ){
				++BTagsT;
				TightTags.push_back(i);
			}
		}	
		//count them up  and use these to make the mass four-vector from AK4 jets

	}			
	    //std::cout<<"NBtags loose "<<BTagsL <<" medium "<<BTagsM<< " tight "<<BTagsT<<std::endl;
	    //std::cout<<"Resolved Candidte "<< BuildBCombinations(CleanedJets, LooseTags, MedTags, TightTags)<<std::endl;;   	
	    double ResolvedHiggs=BuildBCombinations(CleanedJets, LooseTags, MedTags, TightTags);
	    if(ResolvedHiggs>100 && ResolvedHiggs<140)++nResolvedH;
	    //std::cout<<"Resolved "<<ResolvedHiggs<<std::endl;
	    newtree->Fill();
	}
outputFile->cd();
newtree->Write(skims.sampleName[iSample]);
  }
}
