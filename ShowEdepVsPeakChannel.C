#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <TH1F.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TLegend.h>
#include "./GIDMapping.h"
using namespace std;
void ShowEdepVsPeakChannel(
  string FilePath =  "/data4/YuSiang/LYSOReadOut/Datas/20240216/Run19-HV4DC-VFEC-Na22_192751.381",
  string FileName = "Run19-HV4DC-VFEC-Na22_192751.381"
){
  
  
  string FN = FilePath+"/"+FileName+"_Sci_GADC.root";
  TFile *_file0 = TFile::Open(FN.data());
  TTree *t = (TTree*) _file0->Get("t");
  
  TCanvas *c1 = new TCanvas("c1","c1",1920+4,1080+28);
  c1->SetLogz(1);
  
  TH2F *h1X1 = new TH2F("h1X1","Peak channel Edep  only",500,0,11000,64,0,64);
  TH2F *h3X3 = new TH2F("h3X3","Peak channel Edep + 3X3",500,0,55000,64,0,64);
  TH2F *h5X5 = new TH2F("h5X5","Peak channel Edep + 5X5",500,0,110000,64,0,64);
  TH2F *hAll = new TH2F("hAll","Peak channel Edep +Else",500,0,150000,64,0,64);
  t->Draw("GID:Edep>>h1X1","iHit==0","colz");
  t->Draw("GID:Edep>>h3X3","iHit==0","colz");
  t->Draw("GID:Edep>>h5X5","iHit==0","colz");
  t->Draw("GID:Edep>>hAll","iHit==0","colz");
  c1->Print((FilePath+"/"+"EdepVsChannels.gif").data()) ;
  // c1->Print("EDepVsChannels.gif");
  TH1D *hAllProj[64];
  TCanvas *c2 = new TCanvas("c2","c2",1600*4+4,1600*4+28);
  c2->Divide(8,8);

  for(int i=0;i<64;i++){
    int gid = proGID[i];
    cout<<"Draw the slice of gain VS channel: GID="<<gid<<endl;
    hAllProj[gid]= hAll->ProjectionX(Form("hChVHEdep=%02.f",gid*1.),gid+1,gid+1,"cutg");
    hAllProj[gid]->SetTitle(Form("ROCID = %d, ChID = %02.f",gid/32,(gid%32)*1.));
    hAllProj[gid]->GetXaxis()->SetNdivisions(205);
    c2->cd(i+1);
    c2->cd(i+1)->SetLogy(0);
    
    hAllProj[gid]->Draw("hist");
  }
  c2->Print((FilePath+"/"+"EdepVsChannelsSeps.gif").data());
  
  // TCanvas *c3 = new TCanvas("c3","c3",1600*4+4,1600*4+28); c3->cd();
  // t->Draw("GID:Edep3>>hAll","iHit==0","colz");
  // TH2F *h1x1Mov = new TH2F("h1x1Mov","Peak channel Edep and 1x1",7,0,7,7,0,7);
  // t->Draw("GID:Edep>>h1x1Mov","iHit==0","colz");

}