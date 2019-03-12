#include "TString.h"
#include "TChain.h"
#include "TH1F.h"
#include "TROOT.h"
#include "THStack.h"
#include "TString.h"
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
double FindHiggs( vector<TLorentzVector> Jets, std::vector<unsigned int >MedTags){
	TLorentzVector tempLead;
	TLorentzVector tempSubLead;
	TLorentzVector ResolvedCandidate;
	vector<double> MaybeHiggs_mass;
	vector<double> MaybeHiggs_AbsMass;
	double DasMass = -999.;
	double HiggsMass = 125.;
	for(unsigned int m=0; m<MedTags.size(); ++m){
		unsigned int jetindex=MedTags[m];
		tempLead=Jets.at(jetindex);
		for(unsigned int n=(m+1); n<MedTags.size(); ++n){
			unsigned int subjetindex=MedTags[n];
			tempSubLead=Jets.at(jetindex);
			ResolvedCandidate=tempLead+tempSubLead;
			if (abs(ResolvedCandidate.M()-HiggsMass)>60) continue;
			//std::cout<<"Maybe Higgs mass: "<<ResolvedCandidate.M()<<std::endl;
			MaybeHiggs_mass.push_back(ResolvedCandidate.M());
			MaybeHiggs_AbsMass.push_back(abs(ResolvedCandidate.M()-HiggsMass));
		} //end loop over sub-jet
	} // end loop over "lead" jet

	if(MaybeHiggs_AbsMass.size()>0){
		//int size = (int)MaybeHiggs_AbsMass.size();
		auto min = min_element(MaybeHiggs_AbsMass.begin(),MaybeHiggs_AbsMass.end());
		int min_pos = distance(MaybeHiggs_AbsMass.begin(),min);
		//std::cout<<"Jet pairing with mass closest to Higgs has mass of: "<< MaybeHiggs_mass[min_pos]<<std::endl;
		DasMass = MaybeHiggs_mass[min_pos];
	}
	else{
		//finally what if there are no tags? Just take two AK4 Jets and build the mass
		if(Jets.size()>1){
			ResolvedCandidate=Jets.at(0)+Jets.at(1);
			DasMass = ResolvedCandidate.M();
		}

	}
	if(MedTags.size()>=2	|| Jets.size()>1)return DasMass;
	else return -999.;
	//	}
}


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
			if(m==0 && MedTags.size()==1)tempSubLead=Jets.at(jetindex); //Tight-Medium bb candidates
			if(m==1 && MedTags.size()==0)tempSubLead=Jets.at(jetindex); //Medium-Medium bb candidates
			if(m>1)break;
		}
	}
	if(TightTags.size()<1 && MedTags.size()<1){ //don't have at least 1 Medium or 1 Tight Tag
	for(unsigned int l=0; l<LooseTags.size(); ++l){
		unsigned int jetindex=LooseTags[l];
		if(l>1)break;
		if(l==1 && (MedTags.size()==1 || TightTags.size()==1 ))tempSubLead=Jets.at(jetindex); //Medium-Loose bb candidate
		if(TightTags.size()>0)continue;
		if(MedTags.size()>0)continue;
		if(MedTags.size()==0 && l==0)tempLead=Jets.at(jetindex);
		if(l==1)tempSubLead=Jets.at(jetindex); //Loose-Loose bb candidates
	}
}
TLorentzVector ResolvedCandidate;
if(TightTags.size()+MedTags.size()>2){//have at least 2 b-tags
	ResolvedCandidate=tempLead+tempSubLead;
}
else{
	//finally what if there are no tags? Just take two AK4 Jets and build the mass
	if(Jets.size()>1){
		ResolvedCandidate=Jets.at(0)+Jets.at(1);
	}

}
//	if(TightTags.size()+MedTags.size()+LooseTags.size()>2   || Jets.size()>1)std::cout<<"Mass of Candidate "<<ResolvedCandidate.M()<<std::endl;
if(TightTags.size()+MedTags.size()+LooseTags.size()>=2	|| Jets.size()>1)return ResolvedCandidate.M();
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
	//TFile* outputFile = new TFile("SkimFileMass"+cutName+regionName+".root","RECREATE");
	TFile* outputFile = new TFile("output.root","RECREATE");

	// background MC samples - 0 lepton regions
	for( int iSample = 0 ; iSample < skims.ntuples.size() ; iSample++){
		cout << skims.sampleName[iSample] << endl;
		RA2bTree* ntuple = skims.ntuples[iSample];
		int numEvents = ntuple->fChain->GetEntries();
		ntupleBranchStatus<RA2bTree>(ntuple);

		TTree* newtree = new TTree("newtree","newtree");
		int nAK8=0;
		int nAK4=0;
		int nAK4Cuts=0;
		int nBoostedH=0;
		int nAK8_belowB=0;
		double deltaMAK8 = -999;
		int nResolvedH=0;
		int nIsoBNotResolved=0;
		int nNotIsoResolvedH=0;
		int BTagsL=0;
		int BTagsM=0;
		int BTagsT=0;
		float MET=0;
		float HT = 0;
		int BTags = 0;
		int nGenZs = 0;
		int nGenHs = 0;
		double CSV_leading =0;
		double CSV_subleading =0;
		double CSV_3leading =0;
		double CSV_4leading =0;
		double HiggsCan_AvgMass=0;
		double deltaR_max=0;
		double HiggsCan_MassDiff=0;
		vector<TLorentzVector> *JetsAK8;
		vector<TLorentzVector> *Jets;
		vector<TLorentzVector> JetsBCan;
		vector<double>  JetsBCan_bDiscriminatorCSV;
		vector<double>  *Jets_bDiscriminatorCSV;
		double Weight = 0;
		vector<unsigned int> IsoAK4Jets;
		vector<unsigned int> NotIsoAK4Jets;
		vector<double> boostedHiggsCan_eta;
		vector<double> boostedHiggsCan_phi;
		vector<double> *resolvedHCan_mass;
		vector<double> *boostedHCan_mass;
		vector<double> *boostedHCan_pt;
		vector<double> *AK8_doubleBDiscriminator;

		TBranch *b_nAK8, *b_nAK8_belowB, *b_nBoostedH, *b_nResolvedH;
		TBranch *b_nAK4, *b_nAK4Cuts, *b_nIsoBNotResolved, *b_nNotIsoResolvedH;
		TBranch *b_BTagsL, *b_BTagsM, *b_BTagsT;
		TBranch *b_MET, *b_HT;
		TBranch *b_deltaR_max;
		TBranch *b_BTags, *b_Jets_bDiscriminatorCSV, *b_Weight;
		TBranch *b_nGenZs, *b_nGenHs;
		TBranch *b_boostedHCan_pt, *b_boostedHCan_mass, *b_resolvedHCan_mass;
		TBranch *b_HiggsCan_MassDiff, *b_HiggsCan_AvgMass;
		TBranch *b_CSV_leading, *b_CSV_subleading, *b_CSV_3leading, *b_CSV_4leading;
		TBranch *b_deltaMAK8;
		TBranch *b_AK8_doubleBDiscriminator;

		newtree->Branch("BTags", &BTags, "BTags/I");
		newtree->Branch("BTagsL", &BTagsL, "BTagsL/I");
		newtree->Branch("BTagsM", &BTagsM, "BTagsM/I");
		newtree->Branch("BTagsT", &BTagsT, "BTagsT/I");
		newtree->Branch("deltaR_max", &deltaR_max, "deltaR_max/D");
		newtree->Branch("HT",&HT,"HT/F");
		newtree->Branch("MET",&MET,"MET/F");
		newtree->Branch("nGenZs", &nGenZs, "nGenZs/I");
		newtree->Branch("nGenHs", &nGenHs, "nGenHs/I");
		newtree->Branch("Jets_bDiscriminatorCSV",&Jets_bDiscriminatorCSV);
		newtree->Branch("Weight", &Weight, "Weight/D");

		newtree->Branch("nAK8", &nAK8, "nAK8/I");
		newtree->Branch("nAK8_belowB", &nAK8_belowB, "nAK8_belowB/I");
		newtree->Branch("AK8_doubleBDiscriminator", &AK8_doubleBDiscriminator);
		newtree->Branch("nAK4", &nAK4, "nAK4/I");
		newtree->Branch("nAK4Cuts", &nAK4Cuts, "nAK4Cuts/I");
		newtree->Branch("nBoostedH", &nBoostedH, "nBoostedH/I");
		newtree->Branch("boostedHCan_mass", &boostedHCan_mass);
		newtree->Branch("boostedHCan_pt", &boostedHCan_pt);

		newtree->Branch("nResolvedH", &nResolvedH, "nResolvedH/I");
		newtree->Branch("nNotIsoResolvedH", &nNotIsoResolvedH, "nNotIsoResolvedH/I");
		newtree->Branch("resolvedHCan_mass", &resolvedHCan_mass);
		newtree->Branch("HiggsCan_AvgMass", &HiggsCan_AvgMass, "HiggsCan_AvgMass/D");
		newtree->Branch("HiggsCan_MassDiff", &HiggsCan_MassDiff, "HiggsCan_MassDiff/D");
		newtree->Branch("CSV_leading", &CSV_leading, "CSV_leading/D");
		newtree->Branch("CSV_subleading", &CSV_subleading, "CSV_subleading/D");
		newtree->Branch("CSV_3leading", &CSV_3leading, "CSV_3leading/D");
		newtree->Branch("CSV_4leading", &CSV_4leading, "CSV_4leading/D");
		newtree->Branch("deltaMAK8", &deltaMAK8, "deltaMAK8/D");
		newtree->Branch("nIsoBNotResolved", &nIsoBNotResolved, "nIsoBNotResolved/I");


		newtree->SetBranchAddress("nAK8",&nAK8, &b_nAK8);
		newtree->SetBranchAddress("nAK8_belowB",&nAK8_belowB, &b_nAK8_belowB);
		newtree->SetBranchAddress("nAK4",&nAK4, &b_nAK4);
		newtree->SetBranchAddress("nAK4Cuts",&nAK4Cuts, &b_nAK4Cuts);
		newtree->SetBranchAddress("nBoostedH",&nBoostedH, &b_nBoostedH);
		newtree->SetBranchAddress("nResolvedH",&nResolvedH, &b_nResolvedH);
		newtree->SetBranchAddress("nGenZs",&nGenZs, &b_nGenZs);
		newtree->SetBranchAddress("nGenHs",&nGenHs, &b_nGenHs);
		newtree->SetBranchAddress("nNotIsoResolvedH",&nNotIsoResolvedH, &b_nNotIsoResolvedH);//added
		newtree->SetBranchAddress("BTagsL", &BTagsL, &b_BTagsL);
		newtree->SetBranchAddress("BTagsM", &BTagsM, &b_BTagsM);
		newtree->SetBranchAddress("BTagsT", &BTagsT, &b_BTagsT);
		newtree->SetBranchAddress("BTags", &BTags, &b_BTags);
		newtree->SetBranchAddress("Jets_bDiscriminatorCSV",&Jets_bDiscriminatorCSV, &b_Jets_bDiscriminatorCSV);
		newtree->SetBranchAddress("Weight", &Weight, &b_Weight);
		newtree->SetBranchAddress("MET", &MET, &b_MET);
		newtree->SetBranchAddress("HT", &HT, &b_HT);
		newtree->SetBranchAddress("deltaR_max", &deltaR_max, &b_deltaR_max);
		newtree->SetBranchAddress("resolvedHCan_mass", &resolvedHCan_mass, &b_resolvedHCan_mass);
		newtree->SetBranchAddress("boostedHCan_mass", &boostedHCan_mass, &b_boostedHCan_mass);
		newtree->SetBranchAddress("boostedHCan_pt", &boostedHCan_pt, &b_boostedHCan_pt);
		newtree->SetBranchAddress("HiggsCan_AvgMass", &HiggsCan_AvgMass, &b_HiggsCan_AvgMass);
		newtree->SetBranchAddress("HiggsCan_MassDiff", &HiggsCan_MassDiff, &b_HiggsCan_MassDiff);
		newtree->SetBranchAddress("CSV_leading", &CSV_leading, &b_CSV_leading);
		newtree->SetBranchAddress("CSV_subleading", &CSV_subleading, &b_CSV_subleading);
		newtree->SetBranchAddress("CSV_3leading", &CSV_3leading, &b_CSV_3leading);
		newtree->SetBranchAddress("CSV_4leading", &CSV_4leading, &b_CSV_4leading);
		newtree->SetBranchAddress("deltaMAK8", &deltaMAK8, &b_deltaMAK8);
		newtree->SetBranchAddress("nIsoBNotResolved", &nIsoBNotResolved, &b_nIsoBNotResolved);
		newtree->SetBranchAddress("AK8_doubleBDiscriminator", &AK8_doubleBDiscriminator, &b_AK8_doubleBDiscriminator);


		float minAK8JetPt=300;
		float bbtagCut=0.3;
		TString filename = ntuple->fChain->GetFile()->GetName();
		for( int iEvt = 0 ; iEvt < numEvents ; iEvt++ ){
			//if (iEvt > 100) break;
			ntuple->GetEntry(iEvt);
			if( iEvt % 100000 == 0 ) cout << skims.sampleName[iSample] << ": " << iEvt << "/" << numEvents << endl;
			if(!baselineCut(ntuple))continue;
			if( ( filename.Contains("SingleLept") || filename.Contains("DiLept") ) && ntuple->HT>600. )continue;
			MET=ntuple->MET;
			HT=ntuple->HT;
			BTags=ntuple->BTags;
			Jets_bDiscriminatorCSV=ntuple->Jets_bDiscriminatorCSV;
			Weight=ntuple->Weight;
			AK8_doubleBDiscriminator=ntuple->JetsAK8_doubleBDiscriminator;

			nAK8=0;
			nAK8_belowB=0;
			nAK4=0;
			nAK4Cuts=0;
			nBoostedH=0;
			nResolvedH=0;
			nNotIsoResolvedH=0;
			nIsoBNotResolved=0;
			double test_boostH_mass = -999;
			nGenZs = getNumGenZs(ntuple);
			nGenHs = getNumGenHiggses(ntuple);

			IsoAK4Jets.clear();
			NotIsoAK4Jets.clear();
			boostedHiggsCan_eta.clear();
			boostedHiggsCan_phi.clear();
			JetsBCan.clear();
			JetsBCan_bDiscriminatorCSV.clear();

			vector<double> test_boostedMass;
			vector<double> test_boostedPt;

			for(unsigned int fj=0; fj<ntuple->JetsAK8->size();++fj){
				if(ntuple->JetsAK8->at(fj).Pt()>minAK8JetPt)++nAK8;
				//double prunedMass = ntuple->JetsAK8_prunedMass->at(fj);
				if(ntuple->JetsAK8->at(fj).Pt()>300 && ntuple->JetsAK8_prunedMass->at(fj)>=85.0 && ntuple->JetsAK8_prunedMass->at(fj)<=135.0) {
					if (ntuple->JetsAK8_doubleBDiscriminator->at(fj)<bbtagCut){
						 nAK8_belowB++;
						 continue;
					 }
					++nBoostedH;
					boostedHiggsCan_eta.push_back(ntuple->JetsAK8->at(fj).Eta());
					boostedHiggsCan_phi.push_back(ntuple->JetsAK8->at(fj).Phi());
					test_boostedMass.push_back(ntuple->JetsAK8_prunedMass->at(fj));
					test_boostH_mass = ntuple->JetsAK8_prunedMass->at(fj);
					test_boostedPt.push_back(ntuple->JetsAK8->at(fj).Pt());
				}
			} //end loop over AK8 jets
			boostedHCan_mass = &test_boostedMass;
			boostedHCan_pt = &test_boostedPt;

			//based on Moriond Recommendations for combined CSV: https://twiki.cern.ch/twiki/bin/view/CMS/BtagRecommendation80XReReco#Recommendation_for_combining_b_a
			double CSVBtagLoose = 0.5426;
			double CSVBtagMed   = 0.8484;
			double CSVBtagTight = 0.9535;
			BTagsL = 0;
			BTagsM = 0;
			BTagsT = 0;
			//Save NBTags for different points
			for(unsigned int j=0; j<ntuple->Jets->size();++j){
				++nAK4;
				if(ntuple->Jets->at(j).Pt()<30 || fabs(ntuple->Jets->at(j).Eta())>2.4)continue;
				float CSV = ntuple->Jets_bDiscriminatorCSV->at(j);
				if (CSV > CSVBtagLoose && CSV < CSVBtagMed) BTagsL++;
				if (CSV > CSVBtagMed && CSV < CSVBtagTight) BTagsM++;
				if (CSV > CSVBtagTight) BTagsT++;
			}

			for(unsigned int j=0; j<ntuple->Jets->size();++j){
				++nAK4;
				if(ntuple->Jets->at(j).Pt()<30 || fabs(ntuple->Jets->at(j).Eta())>2.4)continue;
				++nAK4Cuts;
				JetsBCan.push_back(ntuple->Jets->at(j));
				JetsBCan_bDiscriminatorCSV.push_back(ntuple->Jets_bDiscriminatorCSV->at(j));
				if(ntuple->Jets_bDiscriminatorCSV->at(j)<CSVBtagMed) continue;
				NotIsoAK4Jets.push_back(j);
				//Check overlap between AK8 and AK4 jet
				bool Overlap=false;
				for(unsigned int fj=0; fj<boostedHiggsCan_eta.size();++fj){
					float deta=ntuple->Jets->at(j).Eta() - boostedHiggsCan_eta.at(fj);
					float dphi=CalcdPhi(ntuple->Jets->at(j).Phi(), boostedHiggsCan_phi.at(fj));
					float dR=sqrt((deta*deta)+(dphi*dphi));
					if(dR<0.8){
						Overlap=true;
						break;
					}
				}
				if(!Overlap){
					IsoAK4Jets.push_back(j);
					nIsoBNotResolved++; //Num of b-tagged AK4 jets that are isolated from the AK8 jets, with pT and eta cuts
				}
			} // End loop over AK4 jets
			//Look over all possible resolved Higgs candidates, without requiring any isolation (completely ignore presence of AK8 jets)

			if (BTags == 2){ //numBJets are # jets in event with pT>30, eta<2.4, and bDiscrim>med
				TLorentzVector JetComboTest;
				for(unsigned int i=0; i<NotIsoAK4Jets.size();++i){
					unsigned int j=NotIsoAK4Jets.at(i);
					JetComboTest+=(ntuple->Jets->at(j));
				}
				double HiggsMassCan = JetComboTest.M();
				if (HiggsMassCan>100 && HiggsMassCan<140)	nNotIsoResolvedH=1;
			}

			if (BTags>1 && JetsBCan.size() >=4){ //Find the four jets with the highest b-discriminator
				double HighestValuesTest[] = {-11,-11,-11,-11};
				int IndicesTest[] = {-1,-1,-1,-1};
				for(unsigned int j=0; j<JetsBCan.size();++j){
					double *current_min = min_element(HighestValuesTest,HighestValuesTest+4);
					int min_pos = distance(HighestValuesTest,min_element(HighestValuesTest,HighestValuesTest+4));
					double *CSVofJet = &(JetsBCan_bDiscriminatorCSV.at(j));
					if (*CSVofJet > *current_min){
						HighestValuesTest[min_pos] = *CSVofJet;
						IndicesTest[min_pos] = j;
					}
				} //end loop to find jets with 4 highest CSV
				//Save CSV values for problem-solving things
				std::sort(HighestValuesTest, HighestValuesTest+4, std::greater<double>());
				CSV_leading = HighestValuesTest[0];
				CSV_subleading = HighestValuesTest[1];
				CSV_3leading = HighestValuesTest[2];
				CSV_4leading = HighestValuesTest[3];

				//cout<<"Test: "<<CSV_leading<<", "<<CSV_subleading<<", "<<CSV_3leading<<", "<<CSV_4leading<<endl;
				//loop through candidate combinations
				TLorentzVector JetComboTest1a = JetsBCan[IndicesTest[0]]+JetsBCan[IndicesTest[1]];
				TLorentzVector JetComboTest1b = JetsBCan[IndicesTest[2]]+JetsBCan[IndicesTest[3]];
				TLorentzVector JetComboTest2a = JetsBCan[IndicesTest[0]]+JetsBCan[IndicesTest[2]];
				TLorentzVector JetComboTest2b = JetsBCan[IndicesTest[1]]+JetsBCan[IndicesTest[3]];
				TLorentzVector JetComboTest3a = JetsBCan[IndicesTest[0]]+JetsBCan[IndicesTest[3]];
				TLorentzVector JetComboTest3b = JetsBCan[IndicesTest[1]]+JetsBCan[IndicesTest[2]];

				double MassDiff1 = abs(JetComboTest1a.M()-JetComboTest1b.M());
				double MassDiff2 = abs(JetComboTest2a.M()-JetComboTest2b.M());
				double MassDiff3 = abs(JetComboTest3a.M()-JetComboTest3b.M());
				double MassAvg1 = (JetComboTest1a.M()+JetComboTest1b.M())/2;
				double MassAvg2 = (JetComboTest2a.M()+JetComboTest2b.M())/2;
				double MassAvg3 = (JetComboTest3a.M()+JetComboTest3b.M())/2;
				double deltaR1 = -1;
				double deltaR2 = -1;
				//Find smallest mass difference of the 3 candidates
				if (MassDiff1<MassDiff2 && MassDiff1<MassDiff3){
					HiggsCan_MassDiff = MassDiff1;
					HiggsCan_AvgMass = MassAvg1;
					double deltaEta1 = (JetsBCan[IndicesTest[0]].Eta()-JetsBCan[IndicesTest[1]].Eta());
					double deltaPhi1 = CalcdPhi(JetsBCan[IndicesTest[0]].Phi(),JetsBCan[IndicesTest[1]].Phi());
					double deltaEta2 = (JetsBCan[IndicesTest[2]].Eta()-JetsBCan[IndicesTest[3]].Eta());
					double deltaPhi2 = CalcdPhi(JetsBCan[IndicesTest[2]].Phi(),JetsBCan[IndicesTest[3]].Phi());
					deltaR1 = sqrt((deltaEta1*deltaEta1)+(deltaPhi1*deltaPhi1));
					deltaR2 = sqrt((deltaEta2*deltaEta2)+(deltaPhi2*deltaPhi2));
					deltaR_max = max(deltaR1, deltaR2);
					if (MassAvg1>100 && MassAvg1<=140 && abs(MassDiff1)<40) {
						nNotIsoResolvedH=2;
					}
					else if ((JetComboTest1a.M()>100 && JetComboTest1a.M()<140) || (JetComboTest1b.M()>100 && JetComboTest1b.M()<140) ) nNotIsoResolvedH=1;
				}
				else if (MassDiff2<MassDiff1 && MassDiff2<MassDiff3){
					HiggsCan_MassDiff = MassDiff2;
					HiggsCan_AvgMass = MassAvg2;
					double deltaEta1 = (JetsBCan[IndicesTest[0]].Eta()-JetsBCan[IndicesTest[2]].Eta());
					double deltaPhi1 = CalcdPhi(JetsBCan[IndicesTest[0]].Phi(),JetsBCan[IndicesTest[2]].Phi());
					double deltaEta2 = (JetsBCan[IndicesTest[1]].Eta()-JetsBCan[IndicesTest[3]].Eta());
					double deltaPhi2 = CalcdPhi(JetsBCan[IndicesTest[1]].Phi(),JetsBCan[IndicesTest[3]].Phi());
					deltaR1 = sqrt((deltaEta1*deltaEta1)+(deltaPhi1*deltaPhi1));
					deltaR2 = sqrt((deltaEta2*deltaEta2)+(deltaPhi2*deltaPhi2));
					deltaR_max = max(deltaR1, deltaR2);
					if (MassAvg2>100 && MassAvg2<=140 && abs(MassDiff2)<40) {
						nNotIsoResolvedH=2;
					}
					else if ((JetComboTest2a.M()>100 && JetComboTest2a.M()<140) || (JetComboTest2b.M()>100 && JetComboTest2b.M()<140) ) nNotIsoResolvedH=1;

				}
				else if (MassDiff3<MassDiff1 && MassDiff3<MassDiff2){
					HiggsCan_MassDiff = MassDiff3;
					HiggsCan_AvgMass = MassAvg3;
					double deltaEta1 = (JetsBCan[IndicesTest[0]].Eta()-JetsBCan[IndicesTest[3]].Eta());
					double deltaPhi1 = CalcdPhi(JetsBCan[IndicesTest[0]].Phi(),JetsBCan[IndicesTest[3]].Phi());
					double deltaEta2 = (JetsBCan[IndicesTest[1]].Eta()-JetsBCan[IndicesTest[2]].Eta());
					double deltaPhi2 = CalcdPhi(JetsBCan[IndicesTest[1]].Phi(),JetsBCan[IndicesTest[2]].Phi());
					deltaR1 = sqrt((deltaEta1*deltaEta1)+(deltaPhi1*deltaPhi1));
					deltaR2 = sqrt((deltaEta2*deltaEta2)+(deltaPhi2*deltaPhi2));
					deltaR_max = max(deltaR1, deltaR2);
					if (MassAvg3>100 && MassAvg3<=140 && abs(MassDiff3)<40) {
						nNotIsoResolvedH=2;
					}
					else if ((JetComboTest3a.M()>100 && JetComboTest3a.M()<140) || (JetComboTest3b.M()>100 && JetComboTest3b.M()<140) ) nNotIsoResolvedH=1;
				}
				else {
					//cout<<MassDiff1<<", "<<MassDiff2<<", "<<MassDiff3<<", "<<endl;
					//cout<< JetComboTest1a.M()<<", "<<JetComboTest1b.M()<<endl;
					//cout<<JetComboTest2a.M()<<", "<<JetComboTest2b.M()<<endl;
					//cout<<JetComboTest3a.M()<<", "<<JetComboTest3b.M()<<endl;
					//cout<<IndicesTest[0]<<", "<<IndicesTest[1]<<", "<<IndicesTest[2]<<", "<<IndicesTest[3]<<endl;
				}
			} // end numBJets>2 cut

			//For one boosted and one resolved Higgs
			//store jet indices for different levels of b-tag probability
			std::vector< unsigned int > LooseTags;
			std::vector< unsigned int > MedTags;
			std::vector< unsigned int > TightTags;


			vector<TLorentzVector> CleanedJets;
			if(nBoostedH == 1 && IsoAK4Jets.size() >= 2){ // start to look for resolved candidates
				for(unsigned int i=0; i<IsoAK4Jets.size();++i){
					unsigned int j=IsoAK4Jets.at(i);
					CleanedJets.push_back(ntuple->Jets->at(j));
					if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagLoose && ntuple->Jets_bDiscriminatorCSV->at(j)<CSVBtagMed){
						LooseTags.push_back(i);
						//++BTagsL;
					}
					//if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagMed && ntuple->Jets_bDiscriminatorCSV->at(j)<CSVBtagTight){
					if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagMed){
						//++BTagsM;
						MedTags.push_back(i);
					}
					if(ntuple->Jets_bDiscriminatorCSV->at(j)>CSVBtagTight ){
						//++BTagsT;
						TightTags.push_back(i);
					}
				} //end loop over IsoAK4Jets
				//count them up  and use these to make the mass four-vector from AK4 jets
			} //end nBoostedH==1
			//double ResolvedHiggsMass=BuildBCombinations(CleanedJets, LooseTags, MedTags, TightTags); //returns -999 if it can't make a combo of AK4 jets
			double ResolvedHiggsMass=FindHiggs(CleanedJets, MedTags); //returns -999 if it can't make a combo of AK4 jets

			deltaMAK8 = -999;
			if (test_boostH_mass > -999 && ResolvedHiggsMass > -999) deltaMAK8 = abs(ResolvedHiggsMass-test_boostH_mass);
			//if (deltaMAK8 > -999) cout <<"test mass: "<< deltaMAK8<<endl;
			vector<double> test_mass;
			test_mass.push_back(ResolvedHiggsMass);
			if (ResolvedHiggsMass > -999) nResolvedH=1;
			resolvedHCan_mass = &test_mass;


			// Fully resolved case
			double HighestValues[] = {-11,-11,-11,-11};
			int Indices[] = {-1,-1,-1,-1};
			if(nBoostedH == 0 && BTags > 2 && JetsBCan.size() >=4){
				for(unsigned int j=0; j<JetsBCan.size();++j){
					double *current_min = min_element(HighestValues,HighestValues+4);
					int min_pos = distance(HighestValues,min_element(HighestValues,HighestValues+4));
					double *CSVofJet = &(JetsBCan_bDiscriminatorCSV.at(j));
					if (*CSVofJet > *current_min){
						HighestValues[min_pos] = *CSVofJet;
						Indices[min_pos] = j;
					}
				} //end loop to find jets with 4 highest CSV
				//loop through candidate combinations
				TLorentzVector JetCombo1a = JetsBCan[Indices[0]]+JetsBCan[Indices[1]];
				TLorentzVector JetCombo1b = JetsBCan[Indices[2]]+JetsBCan[Indices[3]];
				TLorentzVector JetCombo2a = JetsBCan[Indices[0]]+JetsBCan[Indices[2]];
				TLorentzVector JetCombo2b = JetsBCan[Indices[1]]+JetsBCan[Indices[3]];
				TLorentzVector JetCombo3a = JetsBCan[Indices[0]]+JetsBCan[Indices[3]];
				TLorentzVector JetCombo3b = JetsBCan[Indices[1]]+JetsBCan[Indices[2]];

				double MassDiff1 = abs(JetCombo1a.M()-JetCombo1b.M());
				double MassDiff2 = abs(JetCombo2a.M()-JetCombo2b.M());
				double MassDiff3 = abs(JetCombo3a.M()-JetCombo3b.M());
				double MassAvg1 = (JetCombo1a.M()+JetCombo1b.M())/2;
				double MassAvg2 = (JetCombo2a.M()+JetCombo2b.M())/2;
				double MassAvg3 = (JetCombo3a.M()+JetCombo3b.M())/2;

				//Find smallest mass difference of the 3 candidates, then check if the average mass is in the Higgs window
				if (MassDiff1<MassDiff2 && MassDiff1<MassDiff3){
					if (MassAvg1>100 && MassAvg1<=140 && abs(MassDiff1)<40) nResolvedH=2;
				}
				else if (MassDiff2<MassDiff1 && MassDiff2<MassDiff3){
					if (MassAvg2>100 && MassAvg2<=140 && abs(MassDiff2)<40) nResolvedH=2;
				}
				else if (MassDiff3<MassDiff1 && MassDiff3<MassDiff2){
					if (MassAvg3>100 && MassAvg3<=140 && abs(MassDiff3)<40) nResolvedH=2;
				}

			}
			newtree->Fill();
			HiggsCan_AvgMass = -999;
		} //end event loop
		outputFile->cd();
		newtree->Write(skims.sampleName[iSample]);
	} //end sample loop
	outputFile->Close();
	return 0;
}
