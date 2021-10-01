#include <TROOT.h>
#include <TH1F.h>
#include <TString.h>
#include <iostream>
#include <vector>
#include <map>

using namespace std;
template <typename ntupleType> class plot {
  public:
    plot() {};
    plot(double (*fillerFunc_)(ntupleType*), TString label_="var", TString xlabel_="var", int nbins_=20, double lower_=200., double upper_=2200.) {
      fillerFunc = fillerFunc_;
      label = label_;
      xlabel = xlabel_ ;
      nbins = nbins_; lower = lower_; upper = upper_;
      binEdges=NULL;
    };

    plot(double (*fillerFunc_)(ntupleType*), TString label_, TString xlabel_, int nbins_, double* bins_) {
      fillerFunc = fillerFunc_;
      label = label_;
      xlabel = xlabel_ ;
      binEdges = bins_;
      nbins = nbins_; lower = binEdges[0]; upper = binEdges[nbins];
    };

    void addNtuple(ntupleType* ntuple_, TString tag="", bool includeTag=true) {
      tagMap[ntuple_] = tag;
      if (includeTag) {
        if (binEdges) {histoMap[ntuple_] = new TH1F(label+"_"+tag,label+"_"+tag,nbins,binEdges);}
        else {histoMap[ntuple_] = new TH1F(label+"_"+tag,label+"_"+tag,nbins,lower,upper);}
      }
      else {
        if (binEdges) {histoMap[ntuple_] = new TH1F(label,label,nbins,binEdges);}
        else {histoMap[ntuple_] = new TH1F(label,label,nbins,lower,upper);}
      }
      histoMap[ntuple_]->Sumw2();
    };

    void addDataNtuple(ntupleType* ntuple_,TString tag="test") {
      tagMap[ntuple_] = tag ;
      if (binEdges) {dataHist = new TH1F(label+"_"+tag,label+"_"+tag,nbins,binEdges);}
      else {dataHist = new TH1F(label+"_"+tag,label+"_"+tag,nbins,lower,upper);}
      dataHist->SetMarkerStyle(8);
    };

    int fill(ntupleType* ntuple , float customWeight) {
      if (histoMap[ntuple]) {return histoMap[ntuple]->Fill(fillerFunc(ntuple),customWeight);}
      else {
        cout << "plot::fill - ERROR: key not found: " << ntuple << endl;
        return 0;
      }
    };

    int fillData(ntupleType* ntuple) {
      if (dataHist) {return dataHist->Fill(fillerFunc(ntuple));}
      else return 0;
    };

    void buildSum(TString tag="") {
      sum = NULL;
      for (typename map<ntupleType*,TH1F*>::iterator it = histoMap.begin(); it != histoMap.end(); ++it) {
        if (sum == NULL) {
          sum = new TH1F(*(it->second));
          if (tag == "") sum->SetNameTitle(label+"_sum",label+"_sum");
          else sum->SetNameTitle(label+"_"+tag+"_sum",label+"_"+tag+"_sum");
        }
        else sum->Add(it->second);
      }
    };

    void Write(bool runData) {
      for (typename map<ntupleType*,TH1F*>::iterator it = histoMap.begin();it != histoMap.end() ; ++it) {
        if (it->second) it->second->Write();
      }
      if (runData) {if (dataHist) { dataHist->Write();} }
    };

    TString label;
    TString xlabel;
    int nbins;
    double lower, upper;
    double* binEdges;
    map<ntupleType*,TH1F*> histoMap;
    map<ntupleType*,TString> tagMap;
    TH1F* dataHist;
    double (*fillerFunc)(ntupleType*);
    TH1F* sum;
};
