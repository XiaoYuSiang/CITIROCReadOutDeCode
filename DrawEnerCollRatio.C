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
#include "/data4/YuSiang/personalLib/RPU/DBMTCanvas.h"

void DrawEnerCollRatio(
  string FileRoot = "/data4/YuSiang/LYSOReadOut/Datas/20240122/RB2_SIPM1_VF62_HV4E0_Co60_front_131917.110/",
  string FileName = "RB2_SIPM1_VF62_HV4E0_Co60_front_131917.110"

){
  gStyle->SetPalette(55);
  gStyle->SetPadRightMargin(.15);
  TFile *Rfile= new TFile
    (Form("%s%s_Sci_GADC.root",FileRoot.data(),FileName.data()),"read");
  cout<<24<<endl;
  TTree *t = (TTree*) Rfile->Get("t");

  TCanvas *c1 = new TCanvas("c1","c1",1200,100,400+4,400*3+28);
  TCanvas *c3 = new TCanvas("c3","c3",1200+(400+4)*1,100,400+4,400*3+28);
  TCanvas *c2 = new TCanvas("c2","c2",1200+(400+4)*2,100,400+4,400*3+28);
  c1->Divide(1,3);
  c2->Divide(1,3);
  c3->Divide(1,3);
  TH2F *hN = new TH2F("hN","hN",8,0,8,8,0,8);
  hN->SetStats(0); cout<<34<<endl;
  hN->GetXaxis()->SetTitle("iX"); hN->GetXaxis()->CenterTitle(1);
  hN->GetYaxis()->SetTitle("iY"); hN->GetYaxis()->CenterTitle(1);
  hN->GetYaxis()->SetTitleOffset(1.2);
  hN->GetZaxis()->SetTitleOffset(1.5);
  hN->GetZaxis()->RotateTitle(1);
  hN->GetZaxis()->SetTitle("Number of event");
  hN->GetXaxis()->CenterLabels(1);
  hN->GetYaxis()->CenterLabels(1);
  hN->GetZaxis()->SetLabelSize(0.025);
  c2->cd(1);
  hN->SetTitle("Number of Event for Peak channel position");  cout<<45<<endl;
  t->Draw("iY:iX>>hN","(iHit==0&&nHits>1)","colz"); cout<<46<<endl;
   cout<<47<<endl;
  c3->cd(1);
  TH2F *h1 = (TH2F*) hN->Clone(); h1->SetName("h1");
  h1->SetTitle("Edep of peak channel for position");
  h1->GetZaxis()->SetTitle("Sum of Energy Ratio * Event Number");
  t->Draw("iY:iX>>h1","Edep/EdepA*(iHit==0&&nHits>1)","colz"); cout<<52<<endl;
  c3->cd(2);
  TH2F *h3 = (TH2F*) hN->Clone(); h3->SetName("h3"); 
  h3->SetTitle("Edep of peak channel + 3x3 arr for position");
  h3->GetZaxis()->SetTitle("Sum of Energy Ratio * Event Number");
  t->Draw("iY:iX>>h3","Edep3/EdepA*(iHit==0&&nHits>1)","colz"); cout<<57<<endl;
  c3->cd(3);
  TH2F *h5 = (TH2F*) hN->Clone(); h5->SetName("h5");
  h5->SetTitle("Edep of peak channel + 5x5 arr for position");
  h5->GetZaxis()->SetTitle("Sum of Energy Ratio * Event Number");
  t->Draw("iY:iX>>h5","Edep5/EdepA*(iHit==0&&nHits>1)","colz"); cout<<62<<endl;

  c1->cd(1);
  TH2F *h1D = (TH2F*) h1->Clone();
  h1D->SetName("h1D"); h1D->SetTitle("h1D");
  h1D->SetTitle("Ratio Edep of Ch_{peak} + 1x1 arr / all Edep @ position");
  h1D->GetZaxis()->SetTitle("Energy Ratio");
  h1D->Divide(hN); h1D->Draw("colz"); cout<<69<<endl;
  c1->cd(2);
  TH2F *h3D = (TH2F*) h3->Clone(); 
  h3D->SetName("h3D"); h3D->SetTitle("h3D");
  h3D->SetTitle("Ratio Edep of Ch_{peak} + 3x3 arr / all Edep @ position");
  h3D->GetZaxis()->SetTitle("Energy Ratio");
  h3D->Divide(hN); h3D->Draw("colz"); cout<<75<<endl;
  c1->cd(3);
  TH2F *h5D = (TH2F*) h5->Clone(); 
  h5D->SetName("h5D");
  h5D->SetTitle("Ratio Edep of Ch_{peak} + 5x5 arr / all Edep @ position");
  h5D->GetZaxis()->SetTitle("Energy Ratio");
  h5D->Divide(hN); h5D->Draw("colz"); cout<<81<<endl;


  c2->cd(2)->SetRightMargin(.10);
  TH1F *h1N = new TH1F("h1N","h1N",64,0,64); 
  h1N->SetTitle("1-D peak channel VS event number"); 
  h1N->GetXaxis()->SetTitle("General channel ID(ROC ID*32+Channel ID)"); 
  h1N->GetYaxis()->SetTitle("Number of event"); 
  h1N->GetXaxis()->CenterTitle(1);
  h1N->GetYaxis()->SetTitleOffset(1.6);
  h1N->GetYaxis()->SetLabelSize(0.025);
  h1N->SetStats(0);
  h1N->Sumw2();
  t->Draw("GID>>h1N","(iHit==0&&nHits>1)","h");  cout<<94<<endl;
  c2->cd(3)->SetRightMargin(.10);
  TH1F *h11 = new TH1F("h11","h11",64,0,64);
  h11->SetTitle("1-D peak channel VS Energy ratio"); 
  h11->GetXaxis()->SetTitle("General channel ID(ROC ID*32+Channel ID)"); 
  h11->GetXaxis()->CenterTitle(1); 
  h11->GetYaxis()->SetTitle("Ratio of energy"); 
  h11->SetStats(0);
  h11->SetLineColorAlpha(1,.5);
  h11->GetYaxis()->SetTitleOffset(1.5);
  h11->GetYaxis()->SetLabelSize(0.025);
  h11->Sumw2();
  t->Draw("GID>>h11","Edep/EdepA*(iHit==0&&nHits>1)","he"); cout<<106<<endl;
  h11->Divide(h1N);
  float mintmp = 1;
  for(int i=0;i<64;i++){
    float val = h11->GetBinContent(i+1) - h11->GetBinError(i+1);
    if(val>0&&val<mintmp){
      mintmp = val;
    }
  } 
  h11->GetYaxis()->SetRangeUser(mintmp-(1-mintmp)*0.2,1);
  h11->Draw("he");
  TH1F *h13 = new TH1F("h13","h13",64,0,64); 
  t->Draw("GID>>h13","Edep3/EdepA*(iHit==0&&nHits>1)","hesame");  cout<<118<<endl;
  h13->Divide(h1N);
  h13->SetLineColorAlpha(2,.5);
  h13->Sumw2();
  TH1F *h15 = new TH1F("h15","h15",64,0,64); 
  t->Draw("GID>>h15","Edep5/EdepA*(iHit==0&&nHits>1)","hesame"); cout<<123<<endl;
  h15->Divide(h1N);
  h15->SetLineColorAlpha(3,.5);
  h15->Sumw2();
  TLegend *l1 = new TLegend(0.15,0.12,0.5,0.3);
  l1->SetLineColorAlpha(0,0);
  l1->SetFillColorAlpha(0,0);
  l1->AddEntry(h11,"peak    / total","le");
  l1->AddEntry(h13,"peak+3x3/ total","le");
  l1->AddEntry(h15,"peak+5x5/ total","le");
  l1->Draw();
  TCanvas *c4 = new TCanvas("c4","c4",400*3+4,400*3+28);
  c4->Divide(3,1);
  DrawPadCloneOn(c2,c4->cd(1));
  DrawPadCloneOn(c3,c4->cd(2));
  DrawPadCloneOn(c1,c4->cd(3));
  c4->Print(Form("%s/collectionEffOfPeakChannel.png",FileRoot.data()));
  c4->Print(Form("%s/collectionEffOfPeakChannel.pdf",FileRoot.data()));
  c4->Print(Form("%s/collectionEffOfPeakChannel.gif",FileRoot.data()));
   cout<<"Finish drawing of Collection Energy ratio."<<endl;
}