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
using namespace std;


void ADCShow(
const string NameRoot = "/data4/YuSiang/LYSOReadOut/Datas/151521.030_HV51",
const string FileName = "151521.030_HV51"
){
  string dirGraph = Form("%s/",NameRoot.data());
  system(Form("mkdir -p %s",dirGraph.data()));
  string SourceFileNameBin = NameRoot+"/"+FileName+"_Sci.root";
  TFile *_file0 = TFile::Open(SourceFileNameBin.data());
  vector<double> vBin;
  for(int i=0;i<8192;i++) vBin.push_back(i);
  for(int i=820;i<11001;i++) vBin.push_back(i*10);
  TH2F *h5 = new TH2F("h5","h5",vBin.size()-1,vBin.data(),64,0,64);
  gStyle->SetPadRightMargin(.15);
  TCanvas *c1 = new TCanvas("c1","c1",640*2,480*2);
  TTree *t = (TTree*) _file0->Get("t");
  t->Draw("ROCID*32+channel:ADC*(HGMode*1+!HGMode*10)>>h5","1","colz");
  // c1->Print("eth.png");
  // throw;
  // TH2F *h2 = new TH2F("h2","h2",vBin.size()-1,vBin.data(),64,0,64);

  TH2F *h2 = new TH2F("h2","h2",11000,0,110000,64,0,64);
  h2->GetXaxis()->SetTitle("ADC Number: HG ADC or LG ADCx10");
  // h2->GetXaxis()->SetRangeUser(0,20000);
  h2->GetYaxis()->SetTitle("Channel ID + ROC ID *32");
  h2->SetTitle("Channel VS ADC Number for self radiation");
  h2->SetStats(0);
  h2->GetZaxis()->SetTitle("Number of Hits");
  h2->GetXaxis()->SetTitleOffset(1.4);
  h2->GetZaxis()->SetTitleOffset(1.4);
  h2->GetZaxis()->RotateTitle();
  TH2F *h2H = (TH2F*)h2->Clone(); h2H->SetName("h2H");// new TH2F("h2","h2",11000,0,110000,64,0,64);
  TH2F *h2L = (TH2F*)h2->Clone(); h2L->SetName("h2L");// new TH2F("h2","h2",11000,0,110000,64,0,64);
  t->Draw("ROCID*32+channel:ADC*(HGMode*1+!HGMode*10)>>h2","1","colz");
  t->Draw("ROCID*32+channel:ADC>>h2H","HGMode","colz");
  t->Draw("ROCID*32+channel:ADC*10>>h2L","!HGMode","colz");

  c1->cd();
  c1->SetLogz(1);
  h2->Draw("lego2z");

  h2->Draw("col2z");
  // h2->GetZaxis()->CenterTitle();
  c1->Print(Form("%sSelfRadition_ADCcnt_show.png",dirGraph.data()));
  c1->Print(Form("%sSelfRadition_ADCcnt_show.pdf",dirGraph.data()));
  c1->Print(Form("%sSelfRadition_ADCcnt_show.gif",dirGraph.data()));
  
  TH1D *h2Proj[64],*h2HProj[64],*h2LProj[64]; 
  
  gStyle->SetPadTopMargin(0.1);
  gStyle->SetPadBottomMargin(0.1);
  gStyle->SetPadLeftMargin(0.1);
  gStyle->SetPadRightMargin(0.1);

  TCanvas *c2 = EasyCanvas("c2",1600*4,1600*4,8,8,"false");
  for(int i=0;i<64;i++){
    c2->cd(i+1);
    c2->cd(i+1)->SetLogy(1);
    h2Proj[i]= h2->ProjectionX(Form("h2ProjY=%02.f",i*1.),i+1,i+1,"cutg");
    h2Proj[i]->SetTitle(Form("ROCID = %d, ChID = %02.f",i/32,(i%32)*1.));
    h2Proj[i]->GetXaxis()->SetTitle("ADC Number: HG ADC or LG ADCx10");
    h2Proj[i]->GetXaxis()->SetRangeUser(0,110000);
    h2Proj[i]->GetYaxis()->SetTitle("Number of Hits");
    h2Proj[i]->GetYaxis()->SetTitleOffset(1.4);
    h2Proj[i]->Draw("hist");
    
    h2HProj[i]  = h2H->ProjectionX(Form("h2HProj=%02.f",i*1.),i+1,i+1,"cutg");
    h2LProj[i]  = h2L->ProjectionX(Form("h2LProj=%02.f",i*1.),i+1,i+1,"cutg");
    h2HProj[i] ->SetLineColor(1);
    h2LProj[i] ->SetLineColor(2);
    h2HProj[i]->Draw("histsame");
    h2LProj[i]->Draw("histsame");
    
    
    
  }
  
  c2->Print(Form("%sSelfRadition_ADCcnt_show_channels.png",dirGraph.data()));
  c2->Print(Form("%sSelfRadition_ADCcnt_show_channels.pdf",dirGraph.data()));
  c2->Print(Form("%sSelfRadition_ADCcnt_show_channels.gif",dirGraph.data()));
}