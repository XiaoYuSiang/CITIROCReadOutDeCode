#include <typeinfo>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <TStyle.h>
#include <TF1.h>
#include <TH1F.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>
#include <TLatex.h>
#include <TAxis.h>
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
#include "TFile.h"
using namespace std;

const int Nch = 64;
  
const int map_rockNch[Nch] = { //= rock*100+ch
  124,  116,  108,  107,  17,  16,   8,  0,  //last = 7
  125,  117,  109,  106,  19,  18,   9,  1,  //last = 15
  126,  118,  110,  105,  21,  20,  10,  2,  //last = 23
  127,  119,  111,  104,  23,  22,  11,  3,  //last = 31
  128,  123,  112,  103,  25,  24,  12,  4,  //last = 39
  129,  122,  113,  102,  29,  26,  13,  5,  //last = 47
  130,  121,  114,  101,  30,  28,  14,  6,  //last = 55
  131,  120,  115,  100,  31,  27,  15,  7}; //last = 63
  
int map_posX[Nch] ={
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
  7,  6,  5,  4,  3,  2,  1,  0,
};
int map_posY[Nch] ={
  0,  0,  0,  0,  0,  0,  0,  0,
  1,  1,  1,  1,  1,  1,  1,  1,
  2,  2,  2,  2,  2,  2,  2,  2,
  3,  3,  3,  3,  3,  3,  3,  3,
  4,  4,  4,  4,  4,  4,  4,  4,
  5,  5,  5,  5,  5,  5,  5,  5,
  6,  6,  6,  6,  6,  6,  6,  6,
  7,  7,  7,  7,  7,  7,  7,  7,
};
float map_posX_real[Nch] ={
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
  25.2,  18,  10.8,  3.6,  -3.6,  -10.8,  -18,  -25.2,
};
float map_posY_real[Nch] ={
  -25.2,  -25.2,  -25.2,  -25.2,  -25.2,  -25.2,  -25.2,  -25.2,
  -18,  -18,  -18,  -18,  -18,  -18,  -18,  -18,
  -10.8,  -10.8,  -10.8,  -10.8,  -10.8,  -10.8,  -10.8,  -10.8,
  -3.6,  -3.6,  -3.6,  -3.6,  -3.6,  -3.6,  -3.6,  -3.6,
  3.6,  3.6,  3.6,  3.6,  3.6,  3.6,  3.6,  3.6,
  10.8,  10.8,  10.8,  10.8,  10.8,  10.8,  10.8,  10.8,
  18,  18,  18,  18,  18,  18,  18,  18,
  25.2,  25.2,  25.2,  25.2,  25.2,  25.2,  25.2,  25.2,
};

bool comparator(int a, int b){ return a > b; };
vector<int> sortADC(std::vector<int> hit_adc){
  
  std::vector<int> rank(hit_adc.size());
  for(size_t i=0;i>hit_adc.size();i++){
    rank[i] = i;
  }
  std::sort(rank.begin(), rank.end(), comparator);

  return rank;
}

void match(
  string filePath,
  string fileName
){
  double adcMax = 4000;
  //------------ read tree ------------------
  TFile* infile = new TFile(Form("%s/%s_Sci.root",filePath.data(),fileName.data()),"open");
  //TFile* infile = TFile::Open("VFB2_HV539_Co60_SIPM2_BeamCenter_174556920.root");
  //TFile* infile = TFile::Open("run2_150520130.root" );
  //TFile* infile = TFile::Open("run3_151331675.root" );
  //TFile* infile = TFile::Open("run4_152205132.root" );
  //TFile* infile = TFile::Open("run5_153324676.root" );
  //TFile* infile = TFile::Open("run6_153700895.root" );
  //TFile* infile = TFile::Open("run7_154445461.root" );
  //TFile* infile = TFile::Open("run8_155200052.root" );
  //TFile* infile = TFile::Open("run9_155928680.root" );
  //TFile* infile = TFile::Open("run10_161022495.root");
  //TFile* infile = TFile::Open("run11_162020071.root");
  //TFile* infile = TFile::Open("run12_162500961.root");
  //TFile* infile = TFile::Open("run13_163114880.root");
  //TFile* infile = TFile::Open("run14_164016868.root");
  //TFile* infile = TFile::Open("run15_164436376.root");
  //TFile* infile = TFile::Open("run16_165204449.root");
  //TFile* infile = TFile::Open("run17_170001889.root");



  TTree* tree = (TTree*)infile-> Get("t");
  vector<bool> *GTM=0, *Gain=0;
  vector<int> *ROC=0, *Ch=0, *ADC=0; 
  int PCNT, FCNT;
  tree->SetBranchAddress("HGTMID", &GTM);
  tree->SetBranchAddress("ROCID", &ROC);
  tree->SetBranchAddress("channel", &Ch);
  tree->SetBranchAddress("HGMode", &Gain);
  tree->SetBranchAddress("ADC", &ADC);
  tree->SetBranchAddress("pcnt", &PCNT);
  tree->SetBranchAddress("fcnt", &FCNT);

  //----------- create map for LYSO ----------------
  
  //create mapping
  map<int, int> map_LYSO;
  for(int ir = 0; ir<2; ir++) //rock
    for(int ich = 0; ich<32; ich++) //ch
      {
  int numA = ir*100+ich;
  for(int ich2 = 0; ich2<64; ich2++)
    if(numA==map_rockNch[ich2]) 
      map_LYSO.insert(pair<int, int>(numA, ich2));
      }
      
  //// test map
  //int findN = 112;
  //map<int, int>::iterator it;
  //it = map_LYSO.find(findN);
  //if(it!=map_LYSO.end()) 
  //  {
  //    int idx = map_LYSO.find(findN)->second;
  //    printf("rock*100+ch %3i", findN);
  //    printf("| map_idx  %2i" , idx);
  //    printf("| map_posX %2i", map_posX[idx]);
  //    printf("| map_posY %2i", map_posY[idx]);
  //    printf("| map_posX_real % 4.3f", map_posX_real[idx]);
  //    printf("| map_posY_real % 4.3f", map_posY_real[idx]);
  //    printf("\n");
  //  }

  //----------- check how to match --------------
  TH1D* h1_ADC[Nch];
  for(int ich=0; ich<Nch; ich++)
    h1_ADC[ich] = new TH1D(Form("h1_ADC[%i]", ich), Form("h1_ADC[%i]", ich), 1000, 0, 20000);
  TH2D* h2_hit  = new TH2D("h2_hit", "h2_hit", 10, -1, 9, 10, -1, 9);
  TH1D* h1_hit  = new TH1D("h1_hit", "h1_hit", 64, 0, 63); //Nhit/Nevt
  TH1D* h1_Mul  = new TH1D("h1_Mul", "h1_Mul", 15, 0, 15); //Nhit per event; hit multiplicity
  TH1D* h1_1x1  = new TH1D("h1_1x1", "h1_1x1", 1000, 0, 20000); //energy sum 1x1
  TH1D* h1_3x3  = new TH1D("h1_3x3", "h1_3x3", 1000, 0, 20000); //energy sum 3x3
  TH1D* h1_5x5  = new TH1D("h1_5x5", "h1_5x5", 1000, 0, 20000); //energy sum 5x5

  const int NcheckE = 5;
  TH1D*h1_hitE[NcheckE];
  for(int ihit=0; ihit<NcheckE; ihit++)
    h1_hitE[ihit] = new TH1D(Form("h1_hitE[%i]", ihit), Form("h1_hitE[%i]", ihit), 1000, 0, 20000);
  
  TH1D* h1_Nhit1_0 = new TH1D("h1_Nhit1_0", "h1_Nhit1_0", 1000, 0, 20000);

  TH1D* h1_Nhit2_0 = new TH1D("h1_Nhit2_0", "h1_Nhit2_0", 1000, 0, 20000);
  TH1D* h1_Nhit2_1 = new TH1D("h1_Nhit2_1", "h1_Nhit2_1", 1000, 0, 20000);
  TH1D* h1_Nhit2_s = new TH1D("h1_Nhit2_s", "h1_Nhit2_s", 1000, 0, 20000);

  TH1D* h1_Nhit3_0 = new TH1D("h1_Nhit3_0", "h1_Nhit3_0", 1000, 0, 20000);
  TH1D* h1_Nhit3_1 = new TH1D("h1_Nhit3_1", "h1_Nhit3_1", 1000, 0, 20000);
  TH1D* h1_Nhit3_2 = new TH1D("h1_Nhit3_2", "h1_Nhit3_2", 1000, 0, 20000);
  TH1D* h1_Nhit3_s = new TH1D("h1_Nhit3_s", "h1_Nhit3_s", 1000, 0, 20000);
  
  double adcCut = 0; //basically no cut for now
  int Nevt = tree->GetEntries();
  // cout<<191<<" NEV = "<< Nevt<<endl; sleep(5);
  for(int ievt=0; ievt<Nevt;ievt++){
    // if(ievt>484064)
      // cout<<192<<" "<<"iev = "<<ievt<<endl;
    tree->GetEntry(ievt);
    //- loop all events
    int Nhit = GTM->size();
    int count = 0;
    vector<int> hit_idx, hit_adc, hit_posX, hit_posY;
    h1_Mul->Fill(Nhit);
    // if(ievt>) cout<<"nH = "<<Nhit<<endl;
    for(int i=0; i<Nhit; i++){
      // if(ievt>) cout<<"iH = "<< i<<endl;
      int pTime = PCNT;
      int fTime = FCNT;     
      int rock  = ROC->at(i);
      int ch    = Ch->at(i);
      int gain  = Gain->at(i);
      int adc   = ADC->at(i);
      // if(ievt>484064) cout<<209<<endl;
      //-find idx
      int findN = rock*100+ch;
      map<int, int>::iterator it;
      it = map_LYSO.find(findN);
      int idx = map_LYSO.find(findN)->second;
      int _adc;
      if(gain==0) _adc = adc*10;
      if(gain==1) _adc = adc;
      // if(ievt>484064) cout<<218<<endl;
      
      if(_adc<adcCut) continue; 
      // if(_adc<0) cout<<221<<" ADC = "<< _adc<<endl;; 
      // if(ievt>484064) cout<<221<<endl;
      h1_ADC[idx]->Fill(_adc); //gain==0, adc<2000, gain==1, adc<8500
      h1_hit->Fill(32*rock + ch);
      h2_hit->Fill(map_posX[idx], map_posY[idx]);
      // if(ievt>484064) cout<<225<<endl;
      hit_idx.push_back(idx);
      hit_adc.push_back(_adc);
      hit_posX.push_back(map_posX[idx]);
      hit_posY.push_back(map_posY[idx]);
      // if(ievt>484064) cout<<230<<endl;
      ////- print, check hit by hit
      //printf("PCNT %3i FCNT %8i ROC %2i Ch%2i Gain %i ADC %5i",
      //   pTime, fTime, rock, ch, gain, _adc);     
      //printf("| map_posX %2i", map_posX[idx]);
      //printf("| map_posY %2i", map_posY[idx]);
      //printf("\n");
      
    }

    // if(ievt>484064) cout<<241<<endl;
    // //- reshuffle adc from high to low!
    vector<int> rank; 
    // if(ievt>484064) cout<<244<<"\t"<<hit_adc.size()<<endl;
    if(hit_adc.size()==0) continue;
    else if(hit_adc.size()==1) rank.push_back(hit_adc[0]);
    else rank = sortADC(hit_adc);
    // if(ievt>484064) cout<<246<<endl;

    int max_idx  = rank[0];
    int max_ADC  = hit_adc [max_idx];
    int max_posX = hit_posX[max_idx];
    int max_posY = hit_posY[max_idx];
    int adcSum_1x1 = 0;
    int adcSum_3x3 = 0;
    int adcSum_5x5 = 0;
    // if(ievt>484064) cout<<250<<endl;
    for(size_t ir=0; ir<rank.size(); ir++) {
      int i    = rank[ir];
      int adc  = hit_adc[i];
      int posX = hit_posX[i];
      int posY = hit_posY[i];
      int disX = TMath::Abs(posX-max_posX);
      int disY = TMath::Abs(posY-max_posY);
      
      bool flag_1x1 = false, flag_3x3 = false, flag_5x5 = false; 
      if(disX<=0 && disY<=0) { flag_1x1 = true; } //dis=0
      if(disX<=1 && disY<=1) { flag_3x3 = true; } //dis=0, 1
      if(disX<=2 && disY<=2) { flag_5x5 = true; } //dis=0, 1, 2
      
      if(flag_1x1) adcSum_1x1 = adcSum_1x1 + adc;
      if(flag_3x3) adcSum_3x3 = adcSum_3x3 + adc;
      if(flag_5x5) adcSum_5x5 = adcSum_5x5 + adc;

      if(ir<NcheckE) h1_hitE[ir]->Fill(adc);



      //- reshuffle adc from high to low!
      if(ir==0 && flag_1x1)
        {
          if(rank.size()==1) h1_Nhit1_0->Fill(hit_adc[rank[0]]);
          if(rank.size()==2) h1_Nhit2_0->Fill(hit_adc[rank[0]]);
          if(rank.size()==3) h1_Nhit3_0->Fill(hit_adc[rank[0]]);
        }
      if(ir==1 && flag_3x3)
        {
          if(rank.size()==2) h1_Nhit2_1->Fill(hit_adc[rank[1]]);
          if(rank.size()==3) h1_Nhit3_1->Fill(hit_adc[rank[1]]);
          h1_Nhit2_s->Fill(hit_adc[rank[0]] + hit_adc[rank[1]]);
        }
      if(ir==2 && flag_5x5)
        {
          if(rank.size()==3) h1_Nhit3_2->Fill(hit_adc[rank[2]]);
          h1_Nhit3_s->Fill(hit_adc[rank[0]] + hit_adc[rank[1]] + hit_adc[rank[2]]);
        }

    
      ////- matching check
            //printf("%i ", i);
            //printf(" | adc %8i", adc);
            //printf(" | posX %2i", posX);
            //printf(" | posY %2i", posY);
      //printf(" | maxX %2i", max_posX);
            //printf(" | maxY %2i", max_posY);
      //printf(" | disX %2i", disX);
            //printf(" | disY %2i", disY);
      //if(flag_1x1) printf(" | 1x1");
      //if(flag_3x3) printf(" | 3x3");    
      //if(flag_5x5) printf(" | 5x5");
      //printf("\n");


      }
    ////- energy sum check
    //printf("Energy sum (1x1, 3x3, 5x5) = (%6i, %6i, %6i)\n", 
    //       adcSum_1x1, adcSum_3x3, adcSum_5x5);
    //printf("\n");

    h1_1x1->Fill(adcSum_1x1);
    h1_3x3->Fill(adcSum_3x3);
    h1_5x5->Fill(adcSum_5x5); 

  }

  cout<<315<<endl;
  
  //--------- histogram setting ----------

  //- h1_ADC
  double maxPeak = 0;
  for(int ich=0; ich<Nch; ich++)
    {
      //h1_ADC[ich]->SetStats(0);
      h1_ADC[ich]->GetXaxis()->SetRangeUser(0, adcMax);
      h1_ADC[ich]->GetXaxis()->SetTitle("ADC(digit)");
      h1_ADC[ich]->GetYaxis()->SetTitle("Nhit");
      double peak = h1_ADC[ich]->GetMaximum();
      if(peak>maxPeak) maxPeak = peak;
    }
  for(int ich=0; ich<Nch; ich++)
    h1_ADC[ich]->GetYaxis()->SetRangeUser(0, maxPeak);
  
  //- h1_hit
  h1_hit->Scale(1./(double)Nevt); //Nhit/Nevt
  h1_hit->GetXaxis()->SetTitle("ch");
  h1_hit->GetYaxis()->SetTitle("Nhit/Nevt");


  //- h2_hit  h2_hit->GetXaxis()->SetTitle("pixcel in x");
  h2_hit->GetYaxis()->SetTitle("pixcel in y");

  //- h1_Mul
  h1_Mul->GetXaxis()->SetTitle("Nhit per event");
  h1_Mul->GetYaxis()->SetTitle("Count");
  
  //- h1_1x1
  h1_1x1->GetXaxis()->SetRangeUser(0, adcMax);
  h1_1x1->SetTitle("Energy Sum");
  h1_1x1->GetXaxis()->SetTitle("ADCsum");
  h1_1x1->GetYaxis()->SetTitle("Count");
  h1_1x1->SetLineColor(kRed);
  // h1_1x1->GetXaxis()->SetMaxDigits(3);
  h1_1x1->GetXaxis()->SetNdivisions(550);

  //- h1_3x3
  h1_3x3->GetXaxis()->SetRangeUser(0, adcMax);
  h1_3x3->GetXaxis()->SetTitle("ADCsum : 3x3 (digit)");
  h1_3x3->GetYaxis()->SetTitle("Count");
  h1_3x3->SetLineColor(kBlue);

  //- h1_5x5
  h1_5x5->GetXaxis()->SetRangeUser(0, adcMax);
  h1_5x5->GetXaxis()->SetTitle("ADCsum : 5x5 (digit)");
  h1_5x5->GetYaxis()->SetTitle("Count");
  h1_5x5->SetLineColor(kGreen+4);

  //- h1_hitE
  for(int i=0; i<NcheckE; i++)
    {
      h1_hitE[i]->GetXaxis()->SetRangeUser(0, adcMax);
      h1_hitE[i]->GetXaxis()->SetTitle("ADC(digit)");
      h1_hitE[i]->GetYaxis()->SetTitle("Count");
    }

  //- Nhit energy
  //- Nhit=1
  h1_Nhit1_0->GetXaxis()->SetRangeUser(0, adcMax);
  h1_Nhit1_0->GetXaxis()->SetTitle("ADC(digit)");
  h1_Nhit1_0->SetTitle("Nhit=1");
  h1_Nhit1_0->SetLineColor(kRed);
  //- Nhit=2
  h1_Nhit2_0->GetXaxis()->SetRangeUser(0, adcMax);
  h1_Nhit2_0->GetXaxis()->SetTitle("ADC(digit)");
  h1_Nhit2_0->SetTitle("Nhit=2");
  h1_Nhit2_0->SetLineColor(kRed);
  h1_Nhit2_1->SetLineColor(kBlue);
  h1_Nhit2_s->SetLineColor(kMagenta);
  //-Nhit=3
  h1_Nhit3_0->GetXaxis()->SetRangeUser(0, adcMax);
  h1_Nhit3_0->GetXaxis()->SetTitle("ADC(digit)");
  h1_Nhit3_0->SetTitle("Nhit=3");
  h1_Nhit3_0->SetLineColor(kRed);
  h1_Nhit3_1->SetLineColor(kBlue);
  h1_Nhit3_2->SetLineColor(kGreen+4);
  h1_Nhit3_s->SetLineColor(kBlack);
  
  //---------- c_ADC ---------------
  TCanvas* c_ADC = new TCanvas("c_ADC", "c_ADC", 1600, 800);
  c_ADC->Divide(8, 8, 0, 0);
  for(int ich=0; ich<Nch; ich++)
    {
      c_ADC->cd(ich+1); 
      if((ich+1)%8==0) gPad->SetRightMargin(0.1);
      h1_ADC[ich]->Draw();
      
      TLatex latex;
      latex.SetTextSize(0.2);
      latex.SetTextColor(kRed);
      latex.DrawLatexNDC(0.1, 0.8, Form("%i", map_rockNch[ich]));
    }  


  //---------- c_hitE ---------------
  TCanvas* c_hitE = new TCanvas("c_hitE", "c_hitE", 1600, 800);
  c_hitE->Divide(3, 2);
  for(int i=0; i<NcheckE; i++)
    {
      c_hitE->cd(i+1); 
      h1_hitE[i]->Draw();
    }  

  //--------- c_sum -------------------
  TCanvas* c_sum = new TCanvas("c_sum", "c_sum", 1600, 800);
  c_sum->Divide(4, 2);
  c_sum->cd(1); 
  gPad->SetLeftMargin(0.15); 
  h2_hit->Draw("colz");
  //c_sum->cd(2); gPad->SetLeftMargin(0.15); h1_hit->Draw();
  
  c_sum->cd(2); 
  gPad->SetLogy();
  gPad->SetLeftMargin(0.15); 
  h1_Mul->Draw();
  
  c_sum->cd(3); 
  gPad->SetLeftMargin(0.15); 
  h1_1x1->Draw();
  h1_3x3->Draw("sames");
  h1_5x5->Draw("sames");

  gPad->Update();
  TPaveStats *st_1x1 = (TPaveStats*)h1_1x1->FindObject("stats");
  st_1x1->SetOptStat(1111);
  st_1x1->SetX1NDC(0.70); 
  st_1x1->SetX2NDC(0.90); 
  st_1x1->SetY1NDC(0.80); 
  st_1x1->SetY2NDC(0.90);
  st_1x1->SetLineColor(kRed);
  st_1x1->SetTextColor(kRed);
  
  //gPad->Update();
  TPaveStats *st_3x3 = (TPaveStats*)h1_3x3->FindObject("stats");
  st_3x3->SetOptStat(1111);
  st_3x3->SetX1NDC(0.70); 
  st_3x3->SetX2NDC(0.90); 
  st_3x3->SetY1NDC(0.80); 
  st_3x3->SetY2NDC(0.70);
  st_3x3->SetLineColor(kBlue);
  st_3x3->SetTextColor(kBlue);

  //gPad->Update();
  TPaveStats *st_5x5 = (TPaveStats*)h1_5x5->FindObject("stats");
  st_5x5->SetOptStat(1111);
  st_5x5->SetX1NDC(0.70); 
  st_5x5->SetX2NDC(0.90); 
  st_5x5->SetY1NDC(0.70); 
  st_5x5->SetY2NDC(0.60);
  st_5x5->SetLineColor(kGreen+4);
  st_5x5->SetTextColor(kGreen+4);

  c_sum->cd(4); 
  h1_hitE[0]->SetTitle("Energy for hit#N");
  h1_hitE[0]->SetLineColor(kRed);     h1_hitE[0]->Draw("");
  h1_hitE[1]->SetLineColor(kBlue);    h1_hitE[1]->Draw("sames");
  h1_hitE[2]->SetLineColor(kGreen+4); h1_hitE[2]->Draw("sames");
  h1_hitE[3]->SetLineColor(kMagenta); h1_hitE[3]->Draw("sames");
  h1_hitE[4]->SetLineColor(kCyan+3);  h1_hitE[4]->Draw("sames");

  c_sum->cd(5); 
  h1_Nhit1_0->Draw();

  c_sum->cd(6);
  h1_Nhit2_0->Draw();
  h1_Nhit2_1->Draw("sames"); 
  h1_Nhit2_s->Draw("sames"); 

  c_sum->cd(7); 
  h1_Nhit3_0->Draw();
  h1_Nhit3_1->Draw("sames"); 
  h1_Nhit3_2->Draw("sames"); 
  h1_Nhit3_s->Draw("sames"); 

  c_sum->cd(8); 
  TH1D* htmp = (TH1D*)h1_Nhit1_0->Clone();
  htmp->SetTitle("Energy Sum (Nhit<=3)");
  htmp->Draw();
  h1_Nhit2_s->Draw("sames"); 
  h1_Nhit3_s->Draw("sames"); 
  
  //---------- save ---------------------
  TFile* outfile = new TFile(Form("%s/match_%s.root", filePath.data(), fileName.data()), "RECREATE");
  std::cout<<"outfile : "<<outfile->GetName()<<std::endl;
  cout<<"The process of matching is finished!!"<<endl;
  outfile->cd();
 
  c_sum->Write(); 
  c_ADC->Write();
  c_hitE->Write();
  h1_hit->Write();
  h2_hit->Write();
  h1_Mul->Write();
  for(int ich = 0; ich<Nch; ich++) h1_ADC[ich]->Write();
  for(int i   = 0; i<NcheckE; i++) h1_hitE[i]->Write();
  h1_1x1->Write();
  h1_3x3->Write();
  h1_5x5->Write();
  h1_Nhit1_0->Write();
  h1_Nhit2_0->Write();
  h1_Nhit2_1->Write(); 
  h1_Nhit2_s->Write(); 
  h1_Nhit3_0->Write();
  h1_Nhit3_1->Write(); 
  h1_Nhit3_2->Write(); 
  h1_Nhit3_s->Write(); 
  
  outfile->Close();
  

}
