#include <typeinfo>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>
#include <TPDF.h>
#include "TH2.h"
#include "TProfile.h"
#include <TProfile2D.h>
#include <TStyle.h>
#include <TPaveStats.h>
#include <TGaxis.h>
#include <TChain.h>
#include <TObject.h>
#include <TStopwatch.h>
#include <TSystem.h>
#include "TTree.h"
#include "TGraphErrors.h"
#include "TFile.h"
#include "./GIDMapping.h"
#include "/data4/YuSiang/personalLib/RPU/DBMTCanvas.h"


//@HV-56, 200k-MIP = 5255, Sigma = 1168
//@HV-56, 300k-MIP = 7676, Sigma = 1113
using namespace std;
void FitPeakCode( 
  const string NameRoot = "/data4/YuSiang/LYSOReadOut/Datas/120222.214_HV54/"
){
  TFile *_file0 = TFile::Open(Form("%s/RChHGDivLG.root",NameRoot.data()));

  ofstream out(Form("%s/FitPeakFigure.dat",NameRoot.data()));
  TCanvas *c1 = EasyCanvas("c1",1600*4,1600*4,8,8,"false");
  for(int ich=0;ich<NumAllChs;ich++){
    c1->cd(ich+1);
    TH1D *h1 = (TH1D*) _file0->Get(Form("hGADC-GID%02.f",1.*ich));
    h1->GetXaxis()->SetRangeUser(0,30000);

    h1->Draw("hist");
    float MaxBinCent3 = h1->GetXaxis()->GetBinCenter(h1->GetMaximumBin());
    float RMIP = MaxBinCent3/7676.;
    float estErr3 = 1113.*RMIP*1.1;
    cout<<"RMIP = "<<RMIP<<endl;
    cout<<"estMIP3: "<<MaxBinCent3<<", estErr3 = "<<estErr3<<endl; 
    float MaxBinCent2 = MaxBinCent3/7676.*5255;
    float estErr2 = 1168.*RMIP*1.1;
    cout<<"estMIP2: "<<MaxBinCent2<<", estErr2 = "<<estErr2<<endl; 
    
    TF1 *g0 = new TF1("g0", "gaus", MaxBinCent2-estErr2, MaxBinCent2+estErr2);
    TF1 *g1 = new TF1("g1", "gaus", MaxBinCent3-estErr3, MaxBinCent3+estErr3);
    // TF1 *g0 = new TF1("g0", "gaus", 0, 3500);
    // TF1 *g1 = new TF1("g1", "gaus", 3500, 6500);
    // TF1 *g2 = new TF1("g2", "gaus", 6500, 9500);
    // TF1 *g3 = new TF1("g3", "pol3", 9500, 30000);
    h1->Fit("g0", "R");
    h1->Fit("g1", "R");
    // h1->Fit("g2", "R");
    // h1->Fit("g3", "R");
    // h1->Draw("hist");
    g0->Draw("lsame"); 
    g1->Draw("lsame"); 
    
    float Rpeak2hkv = g0->GetParameter(1)/5255.;
    float Rpeak3hkv = g1->GetParameter(1)/7676.;
    
    out<<ich<<"\t"<<Rpeak2hkv<<"\t"<<Rpeak3hkv<<"\n";
    cout<<ich<<"\t"<<Rpeak2hkv<<"\t"<<Rpeak3hkv<<"\n";
    
    // if(ich>1) break;
  }
  c1->Print(Form("%s/FitPeakResult.png",NameRoot.data()));
  c1->Print(Form("%s/FitPeakResult.pdf",NameRoot.data()));
  c1->Print(Form("%s/FitPeakResult.gif",NameRoot.data()));

  // g2->Draw("lsame"); 
  // g3->Draw("lsame"); 

  // float peakX[2] = {g1->GetParameter(1),g2->GetParameter(1)};
  // float peakY[4] = {2E-4,2E-4};
  // float errx[2]  = {g1->GetParameter(2),g2->GetParameter(2)};
  // float erry [4] = {1,1};
  // cout<<g0->GetParameter(1)<<endl;
  // cout<<g1->GetParameter(1)<<endl;
  // cout<<g2->GetParameter(1)<<endl;
  // cout<<g3->GetParameter(1)<<endl;


  // TGraphErrors *gMIPs = new TGraphErrors(4,peakX,peakY,errx,erry);
  // gMIPs->SetMarkerStyle(33);
  // gMIPs->Draw("Pesame");

  // TH1F *hfit = new TH1F("hfit","hfit",30000,0,30000);
  // for(float i=0.5;i<30000.5;i+=1)
    // hfit->Fill(i,
      // (0<i&&i<3500)*g0->Eval(i)
     // +(3500<i&&i<6500)*g1->Eval(i)
     // +(6500<i&&i<9500)*g2->Eval(i)
     // +(9500<i&&i<30000)*g3->Eval(i)
    // );
  // hfit->SetLineColor(3);
  // hfit->Draw("l][same");
}