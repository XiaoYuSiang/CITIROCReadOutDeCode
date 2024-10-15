#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <sstream>
#include <TH1F.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TChain.h>
#include <TLegend.h>
#include <TGaxis.h>
#include <TText.h>
#include <TFile.h>
#include <TPad.h>
#include <TTree.h>
#include <TColor.h>
#include <TStyle.h>
#include <TPDF.h>
#include <TPaveStats.h>
#include "/data4/YuSiang/personalLib/RPU/DBMTCanvas.h"
#include "/data4/YuSiang/personalLib/RootFile/untuplizerv8_YuSiang01.h"
#if defined (__MAKECINT__) 
#pragma link C++ class vector<Long64_t>+; 
#endif
using namespace std;
void PositionEnergy(
  const string NameRoot = "/data4/YuSiang/LYSOReadOut/Datas/151521.030_HV51",
  const string FileName = "151521.030_HV51"

){
  const int NumCx=8, NumCy=8;
  gStyle->SetNumberContours(255);
  string dirGraph = Form("%s/",NameRoot.data());
  string SourceFileNameBin = NameRoot+"/"+FileName+"_Sci_CB_GADC.root";
  // cout<<NameRoot+"/"+FileName+"_Sci.root"<<endl;
  // cout<<SourceFileNameBin<<endl;
  TFile *_file0 = TFile::Open(SourceFileNameBin.data());
  TreeReader data(SourceFileNameBin.data());
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.2);
  TCanvas *c1 = new TCanvas("c1","c1",1200+4,1000+28); 
  c1->cd();
  TH2F *h2 = new TH2F("h2","h2",NumCx,0,NumCx,NumCy,0,NumCy);
  TTree *t = (TTree*) _file0->Get("t");
  // t->Draw("channel%8:int(channel/8)+ROCID*4>>h2","(Edep>7.E+7)*GADC","colz");
  t->Draw("iY:iX>>h2","(Edep>7.E+7)*GADC","colz");
  h2->GetXaxis()->SetTitle("channel index on X axis(7.12mm)");
  h2->GetYaxis()->SetTitle("channel index on Y axis(7.12mm)");
  h2->GetZaxis()->SetTitle("HG ADC count(=LG Mode ADCx10)~110#pm10eV");
  h2->GetZaxis()->SetTitleOffset(1.4);
  h2->GetXaxis()->CenterTitle(1);
  h2->GetYaxis()->CenterTitle(1);
  h2->GetZaxis()->RotateTitle(1);
  
  for(int i=0;i<NumCx;i++) h2->GetXaxis()->SetBinLabel(i+1,Form("%d",i));
  for(int i=0;i<NumCy;i++) h2->GetYaxis()->SetBinLabel(i+1,Form("%d",i));
  
  h2->SetTitle("Highest edep : (eventID = 3548517)");
  h2->GetZaxis()->SetTitleOffset(2);
  h2->SetStats(0);
  
  h2->Draw("colz");
  c1->Print(Form("%sMaxPositionVsEnergy.png",dirGraph.data()));
  c1->Print(Form("%sMaxPositionVsEnergy.pdf",dirGraph.data()));
  c1->Print(Form("%sMaxPositionVsEnergy.gif",dirGraph.data()));
  c1->Print(Form("%sPositionVsEnergy.pdf(",dirGraph.data()));
  
  TH1F *hnH = new TH1F("hnH","nH of event",63,1,64);
  hnH->GetXaxis()->SetTitle("Number of hits");
  hnH->GetYaxis()->SetTitle("Number of events");
  t->Draw("nHits>>hnH","1","hist");
  c1->SetLogy(1);
  c1->Print(Form("%snHitVsEv.png",dirGraph.data()));
  c1->Print(Form("%snHitVsEv.pdf",dirGraph.data()));
  c1->Print(Form("%snHitVsEv.gif",dirGraph.data()));
  c1->Clear();
  c1->SetLogy(0);
  TCanvas *c2 = new TCanvas("c2","c2",1200+4,1000+28); 
  c2->cd();
  vector<double> vBin;
  vBin.push_back(0);
  for(int i1=40;i1<=100;i1++) vBin.push_back(pow(10,0.1*i1));
  
  TH2F *h3 = new TH2F("h3","total Energy VS #Number of hits in events",64,1,65,vBin.size()-1,vBin.data());

  h3->SetStats(0);
  h3->GetXaxis()->SetTitleOffset(1.4);
  h3->GetZaxis()->SetTitleOffset(1.4);
  h3->GetZaxis()->RotateTitle(1);
  // h3->GetXaxis()->SetTitle("Edep(eV)");
  h3->GetYaxis()->SetTitle("ADC count(HG or LG*10)");
  h3->GetXaxis()->SetTitle("nHits");
  h3->GetZaxis()->SetTitle("number of events");
  // c2->SetLogy(1);
  // c2->SetLogy(1);
  c2->SetLogz(1);
  t->Draw("Edep:nHits>>h3","1","colz");
  c2->Print(Form("%snHVsEdep.png",dirGraph.data()));
  c2->Print(Form("%snHVsEdep.pdf",dirGraph.data()));
  c2->Print(Form("%snHVsEdep.gif",dirGraph.data()));
  
  int nHits, *channel, *ROCID, *GADC, pcnt;
  Long64_t eventID;
  // float *Edep;

  // t->SetBranchAddress("nHits",&nHits);
  // t->SetBranchAddress("channel",&channel);
  // t->SetBranchAddress("ROCID",&ROCID);
  // t->SetBranchAddress("GADC",&GADC);
  // t->SetBranchAddress("Edep",&Edep);
  size_t evs = t->GetEntries();
  int Goodcnt = 0;
  // h2->GetZaxis()->SetTitle("\"Edep\"(keV)");
  h2->GetZaxis()->SetTitle("ADC count(HG or LG*10)");;
  int pcnt0 = -1,pcnttmp = 0;
  for(size_t iev=0;iev<evs;iev++){
    data.GetEntry(iev);// cout<<iev<<endl;
    pcnttmp ++;
    if(pcnttmp<150) break;
    nHits     = data.GetInt("nHits");
    if(nHits!=5) continue;
    // if(Goodcnt>30) break;
    h2->Reset();
    eventID     = data.GetLong64("eventID");
    h2->SetTitle(Form("Fire channel position, eventID = %lld",eventID));
    channel   = data.GetPtrInt("channel");
    ROCID     = data.GetPtrInt("ROCID");
    GADC      = data.GetPtrInt("GADC");
    cout<<iev<<"/"<<evs<<"\t"<<nHits<<endl;
    for(int iH=0;iH<nHits;iH++){
      // h2->Fill(int(channel[iH]/8)+ROCID[iH]*4,channel[iH]%8,GADC[iH]*0.11);
      h2->Fill(int(channel[iH]/8)+ROCID[iH]*4,channel[iH]%8,GADC[iH]);
      cout<<int(channel[iH]/8)+ROCID[iH]*4<<"\t"<<channel[iH]%8<<"\t"<<GADC[iH]<<endl;
    }
    c1->cd();
    h2->Draw("colz");
    
    c1->Print(Form("%sPositionVsEnergy.pdf",dirGraph.data()));
    Goodcnt++;
  }
  
  c2->Print(Form("%sPositionVsEnergy.pdf)",dirGraph.data()));
  h2->Reset();
  t->Draw("iY:iX>>h2","1","colz");

  h2->GetZaxis()->SetTitle("Number if hits");

  h2->SetTitle("Nuber of Hit of all channels");
  c1->cd();
  h2->Draw("colz");
  
  c1->Print(Form("%sNumOfHitsVSPos.pdf",dirGraph.data()));
  c1->Print(Form("%sNumOfHitsVSPos.png",dirGraph.data()));
  c1->Print(Form("%sNumOfHitsVSPos.gif",dirGraph.data()));
  data.GetEntry(0);      //int pcnt0 = data.GetInt("pcnt");
  data.GetEntry(evs-1); int pcnt1 = data.GetInt("pcnt");
  int Worktime = pcnt1-pcnt0;
  cout<<"total pcnt = "<<Worktime<<" sec\n";
  h2->Scale(1./float(Worktime));
  h2->SetTitle("Rate of all channels");
  h2->GetZaxis()->SetTitle("Rate of channel(Hz)");
  c1->Print(Form("%sRateVSPos.pdf",dirGraph.data()));
  c1->Print(Form("%sRateVSPos.png",dirGraph.data()));
  c1->Print(Form("%sRateVSPos.gif",dirGraph.data()));
  
}