#include <typeinfo>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>
#include <TString.h>
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
// #include "/data4/YuSiang/LYSOReadOut/GIDMapping.h"
using namespace std;
const int NumAllChs = 64;
struct Hit{
  bool HGTMID, HGMode;
  Int_t channel, Fire, ADC, ROCID, GID, iHit;
  Int_t iX, iY, iZ;
  float pX, pY, pZ, GADC;
};
bool comparator(Hit a, Hit b){ return a.GADC > b.GADC; };
class Event{
public:
  Long64_t eventID;
  int  SeqSAD, UTCDay, UTCHour, UTCMin, UTCSec, UTCSecS;
  bool GTMID;
  int bufferID, ChannelID, ADCVal, nHits;
  int dfcnt, fcnt;
  int pcnt;
  vector<bool>  *HGTMID, *HGMode;
  vector<Int_t> *channel, *Fire, *ADC, *ROCID, *GID, *iHit;
  vector<Int_t> *iX, *iY, *iZ;
  vector<float> *pX, *pY, *pZ, *GADC;
  float Edep;
  Event(Event* Ev0){
    eventID = Ev0->eventID;
    SeqSAD=Ev0->SeqSAD; UTCDay=Ev0->UTCDay; UTCHour=Ev0->UTCHour; UTCMin=Ev0->UTCMin; UTCSec=Ev0->UTCSec; UTCSecS=Ev0->UTCSecS;
    GTMID = Ev0->GTMID;
    bufferID = Ev0->bufferID; ChannelID=Ev0->ChannelID; ADCVal=Ev0->ADCVal; nHits=Ev0->nHits;
    dfcnt = Ev0->dfcnt; fcnt = Ev0->fcnt;
    pcnt = Ev0->pcnt;
    HGTMID  = new vector<bool>(Ev0->HGTMID->begin(), Ev0->HGTMID->end());
    HGMode  = new vector<bool>(Ev0->HGMode->begin(), Ev0->HGMode->end());
    channel = new vector<Int_t>(Ev0->channel->begin(), Ev0->channel->end());
    Fire    = new vector<Int_t>(Ev0->Fire->begin(), Ev0->Fire->end());
    ADC     = new vector<Int_t>(Ev0->ADC->begin(), Ev0->ADC->end());
    ROCID   = new vector<Int_t>(Ev0->ROCID->begin(), Ev0->ROCID->end());
    GID     = new vector<Int_t>(Ev0->GID->begin(), Ev0->GID->end());
    GADC    = new vector<float>(Ev0->GADC->begin(), Ev0->GADC->end());
    iHit    = new vector<Int_t>(Ev0->iHit->begin(), Ev0->iHit->end());
    iX    = new vector<Int_t>(Ev0->iX->begin(), Ev0->iX->end());
    iY    = new vector<Int_t>(Ev0->iY->begin(), Ev0->iY->end());
    iZ    = new vector<Int_t>(Ev0->iZ->begin(), Ev0->iZ->end());
    pX    = new vector<float>(Ev0->pX->begin(), Ev0->pX->end());
    pY    = new vector<float>(Ev0->pY->begin(), Ev0->pY->end());
    pZ    = new vector<float>(Ev0->pZ->begin(), Ev0->pZ->end());
    Edep = Ev0->Edep;
  }
  Event(){
    Ini();
  }
  void Ini(){
    eventID = 0;
    SeqSAD=0; UTCDay=0; UTCHour=0; UTCMin=0; UTCSec=0; UTCSecS=0;
    GTMID = false;
    bufferID = -1; ChannelID=0; ADCVal=0; nHits=0;
    dfcnt = -1; fcnt = -1;
    pcnt = -1;
    HGTMID=0; HGMode=0;
    channel=0; Fire=0; ADC=0; ROCID=0; GID=0; iHit=0;
    iX=0; iY=0; iZ=0;
    pX=0; pY=0; pZ=0; GADC=0;
    Edep = 0;
    
    
  // vector<bool>  *HGTMID=0, *HGMode=0;
  // vector<Int_t> *channel=0, *Fire=0, *ADC=0, *ROCID=0, *GID=0, *iHit=0;
  // vector<Int_t> *iX=0, *iY=0, *iZ=0;
  // vector<float> *pX=0, *pY=0, *pZ=0, GADC;
  }
  void SetTBranchAddress(TTree *T){
    T->SetBranchAddress("eventID",   &eventID   );
    T->SetBranchAddress("SeqSAD",    &SeqSAD    );
    T->SetBranchAddress("UTCDay",    &UTCDay    );
    T->SetBranchAddress("UTCHour",   &UTCHour   );
    T->SetBranchAddress("UTCMin",    &UTCMin    );
    T->SetBranchAddress("UTCSec",    &UTCSec    );
    T->SetBranchAddress("UTCSecS",   &UTCSecS   );
    T->SetBranchAddress("pcnt",      &pcnt      );
    T->SetBranchAddress("fcnt",      &fcnt      );
    T->SetBranchAddress("dfcnt",     &dfcnt     );
    T->SetBranchAddress("nHits",     &nHits     );
    T->SetBranchAddress("Edep",      &Edep      );
    T->SetBranchAddress("iHit",      &iHit      );
    T->SetBranchAddress("Fire",      &Fire      ); 
    T->SetBranchAddress("HGTMID",    &HGTMID    );
    T->SetBranchAddress("ROCID",     &ROCID     );
    T->SetBranchAddress("channel",   &channel   );
    T->SetBranchAddress("HGMode",    &HGMode    );
    T->SetBranchAddress("ADC",       &ADC       );
    T->SetBranchAddress("GID",       &GID       );
    T->SetBranchAddress("GADC",      &GADC      );
    T->SetBranchAddress("iX",        &iX        );
    T->SetBranchAddress("iY",        &iY        );
    T->SetBranchAddress("iZ",        &iZ        );
    T->SetBranchAddress("pX",        &pX        );
    T->SetBranchAddress("pY",        &pY        );
    T->SetBranchAddress("pZ",        &pZ        );
  }
  void SetTBranch(TTree *T){
    T->Branch("eventID",   &eventID   );
    T->Branch("SeqSAD",    &SeqSAD    );
    T->Branch("UTCDay",    &UTCDay    );
    T->Branch("UTCHour",   &UTCHour   );
    T->Branch("UTCMin",    &UTCMin    );
    T->Branch("UTCSec",    &UTCSec    );
    T->Branch("UTCSecS",   &UTCSecS   );
    T->Branch("pcnt",      &pcnt      );
    T->Branch("fcnt",      &fcnt      );
    T->Branch("dfcnt",     &dfcnt     );
    T->Branch("nHits",     &nHits     );
    T->Branch("Edep",      &Edep      );
    T->Branch("iHit",      &iHit      );
    T->Branch("Fire",      &Fire      ); 
    T->Branch("HGTMID",    &HGTMID    );
    T->Branch("ROCID",     &ROCID     );
    T->Branch("channel",   &channel   );
    T->Branch("HGMode",    &HGMode    );
    T->Branch("ADC",       &ADC       );
    T->Branch("GID",       &GID       );
    T->Branch("GADC",      &GADC      );
    T->Branch("iX",        &iX        );
    T->Branch("iY",        &iY        );
    T->Branch("iZ",        &iZ        );
    T->Branch("pX",        &pX        );
    T->Branch("pY",        &pY        );
    T->Branch("pZ",        &pZ        );
  }
  void Add(Event *eV1){
    for(int iHit1 = 0; iHit1 < eV1->nHits ; iHit1++){
      HGTMID  ->push_back(eV1->HGTMID->at(iHit1));
      HGMode  ->push_back(eV1->HGMode->at(iHit1));
      channel ->push_back(eV1->channel->at(iHit1));
      Fire    ->push_back(eV1->Fire->at(iHit1));
      ADC     ->push_back(eV1->ADC->at(iHit1));
      ROCID   ->push_back(eV1->ROCID->at(iHit1));
      GID     ->push_back(eV1->GID->at(iHit1));
      iHit    ->push_back(eV1->iHit->at(iHit1));
      iX      ->push_back(eV1->iX->at(iHit1));
      iY      ->push_back(eV1->iY->at(iHit1));
      iZ      ->push_back(eV1->iZ->at(iHit1));
      pX      ->push_back(eV1->pX->at(iHit1));
      pY      ->push_back(eV1->pY->at(iHit1));
      pZ      ->push_back(eV1->pZ->at(iHit1));
      GADC    ->push_back(eV1->GADC->at(iHit1));
    }
  }
  void SortGADC(){
    vector<Hit> vHittmp;
    for(int i=0;i<nHits;i++){
      Hit hittmp;
      hittmp.iHit    = iHit->at(i);
      hittmp.GADC    = GADC->at(i);
      hittmp.HGTMID  = HGTMID->at(i);
      hittmp.HGMode  = HGMode->at(i);
      hittmp.channel = channel->at(i);
      hittmp.Fire    = Fire->at(i);
      hittmp.ADC     = ADC->at(i);
      hittmp.ROCID   = ROCID->at(i);
      hittmp.GID     = GID->at(i);
      hittmp.iHit    = iHit->at(i);
      hittmp.iX      = iX->at(i);
      hittmp.iY      = iY->at(i);
      hittmp.iZ      = iZ->at(i);
      hittmp.pX      = pX->at(i);
      hittmp.pY      = pY->at(i);
      hittmp.pZ      = pZ->at(i);
      hittmp.GADC    = GADC->at(i);
      vHittmp.push_back(hittmp);
    }
    sort(vHittmp.begin(), vHittmp.end(), comparator);
    for(int iHit1 = 0; iHit1 < nHits ; iHit1++){
      iHit   ->at(iHit1) = iHit1;
      HGTMID ->at(iHit1) = (vHittmp[iHit1].HGTMID);
      HGMode ->at(iHit1) = (vHittmp[iHit1].HGMode);
      channel->at(iHit1) = (vHittmp[iHit1].channel);
      Fire   ->at(iHit1) = (vHittmp[iHit1].Fire);
      ADC    ->at(iHit1) = (vHittmp[iHit1].ADC);
      ROCID  ->at(iHit1) = (vHittmp[iHit1].ROCID);
      GID    ->at(iHit1) = (vHittmp[iHit1].GID);
      iX     ->at(iHit1) = (vHittmp[iHit1].iX);
      iY     ->at(iHit1) = (vHittmp[iHit1].iY);
      iZ     ->at(iHit1) = (vHittmp[iHit1].iZ);
      pX     ->at(iHit1) = (vHittmp[iHit1].pX);
      pY     ->at(iHit1) = (vHittmp[iHit1].pY);
      pZ     ->at(iHit1) = (vHittmp[iHit1].pZ);
      GADC   ->at(iHit1) = (vHittmp[iHit1].GADC);
    }
    // for(int iHit1 = 0; iHit1 < nHits ; iHit1++)
      // cout<<eventID<<"\t"<<iHit->at(iHit1)<<" "<<GADC->at(iHit1)<<endl;
    // throw;
  }
};

void PackageTestV2(
  string FileRoot = "/data4/YuSiang/LYSOReadOut/Datas/20240118/RB2_SIPM1_VFEA_HV4E_CosRay_Pb_4Hr_030336.960/" , 
  string FileName="RB2_SIPM1_VFEA_HV4E_CosRay_Pb_4Hr_030336.960"
){
  float GRatio[NumAllChs]={0};
  ifstream in(Form("%sRChHGDivLG.ptxt",FileRoot.data()));
  TFile *file = new TFile(Form("%s%s_Sci_GADC.root",FileRoot.data(),FileName.data()),"read");
  TFile *Rfile= new TFile(Form("%s%s_Sci_CB_GADC.root",FileRoot.data(),FileName.data()),"recreate");
  cout<<"In file = "<<file->GetName()<<endl;
  
  string line;
  int    Ivaltmp, gid;
  float  fvaltmp, hgDlg;//, rhlg;
  getline(in,line); cout<<line<<endl;
  
  int  SeqSAD=0, UTCDay=0, UTCHour=0, UTCMin=0, UTCSec=0, UTCSecS=0;
  bool GTMID = false;
  int bufferID = -1, bufferID0 = -1, ChannelID=0, ADCVal=0, nHits=0;
  int dfcnt = -1, fcnt = -1;
  int pcnt = -1, pcnt0 = -1;
  Long64_t eventID = 0;
  vector<bool>  *HGTMID=0, *HGMode=0;
  vector<Int_t> *channel=0, *Fire=0, *ADC=0, *ROCID=0, *GID=0, *iHit=0;
  vector<Int_t> *iX=0, *iY=0, *iZ=0;
  vector<float> *pX=0, *pY=0, *pZ=0, GADC;
  //test variable !! Sum of all channel, 100~120ev/ADC
  
  Event *Etmp = new Event;
  TTree *t0 = (TTree*) file->Get("t");
  Etmp->SetTBranchAddress(t0);
  
  
  TTree *t = new TTree("t", "frame data");
  Event *Esav = new Event;
  Esav->SetTBranch(t); 
  int nROC; float dPos, MdPos;
  float Edep11, Edep33, Edep55, EdepXs, Emax;
  int PeakID = -1;
  float CenpX, CenpY; 
  int    Cortex; 
  t->Branch("nROC",&nROC);
  t->Branch("dPos",&dPos);
  t->Branch("MdPos",&MdPos);
  t->Branch("Edep11",&Edep11);
  t->Branch("Edep33",&Edep33);
  t->Branch("Edep55",&Edep55);
  t->Branch("EdepXs",&EdepXs);
  t->Branch("PeakID",&PeakID);
  t->Branch("CenpX",&CenpX);
  t->Branch("CenpY",&CenpY);
  t->Branch("Cortex",&Cortex);
  
  Long64_t evs = t0->GetEntries();
  int lastROC = -1, nHittmp = -1, ROCbuf = 0, icnt = 0; cout<<83<<endl;
  Event *Ev0 = new Event() , *Ev1  = new Event();
  for(Long64_t iev=0;iev<evs*1.;iev++){
    t0->GetEntry(iev);
    if(iev%1000 == 0) 
      cout<<"\r "<<iev/float(evs)*100.<<"%"<<flush;
    if(Etmp->nHits == 0 ){ //cout<<154<<endl;
      if(iev!=evs-1)
        cout<<"Why contain 0 hits data???????"<<endl;
      continue;
      // break;
    }
    if( Etmp->pcnt < 0){
      continue;
    }else{
      // cout<<iev<<endl;
      if( Ev0->pcnt == -1 ){
        *Ev0 = Event(Etmp);
        dPos = -1;
      }else{ 
        *Ev1 = Event(Etmp);
        // if(Ev0->ROCID->at(0) == Ev1->ROCID->at(0))
        // cout<<" "<<Ev1->fcnt-Ev0->fcnt<<endl;
        if( Ev1->fcnt==Ev0->fcnt && Ev0->ROCID->at(0) != Ev1->ROCID->at(0) ){
          // bool sameEv = false; sameEv = true;//@@
          float postmp, min = 99, max = 0;
          // if(Ev0->eventID == 8544) cout<<Ev0->nHits<<"\t"<<Ev1->nHits<<endl;
          int nRanE00 = 0,nRanE01 = 0;
          for(int ihit0=0; ihit0 < Ev0->nHits ; ihit0++){
            for(int ihit1=0; ihit1 < Ev1->nHits ; ihit1++){
              postmp = sqrt(
                  pow(Ev1->iX->at(ihit1)-Ev0->iX->at(ihit0),2)
                 +pow(Ev1->iY->at(ihit1)-Ev0->iY->at(ihit0),2)
                );
              if(postmp<min) min = postmp;
              if(postmp>max) max = postmp;
              // if(Ev0->eventID == 8544) cout<<endl<<min<<"	"<<max<<"	"<<postmp<<endl;
            }
          }
          // if(Ev0->eventID == 8544) break;
          dPos  = min;
          MdPos = max;
          // if(dPos > sqrt(2.)*1.01){
            // sameEv = false;
          // }
          
          if(dPos <= sqrt(2.)*1.01){
            // cout<<"Double ROC EV: EVID = EA:EB "<<Ev0->eventID<<":"<<Ev1->eventID<<endl; 
            // cout<<"fcntA:fcntB "<<Ev0->fcnt<<":"<<Ev1->fcnt<<endl; 
            // cout<<"ROCIDA:ROCIDA "<<Ev0->ROCID->at(0)<<":"<<Ev1->ROCID->at(0)<<endl; 
            *Esav = Event(Ev0); //cout<<195<<endl;
            Esav->Add(Ev1);  //cout<<196<<endl;
            Esav->nHits = Ev0->nHits+Ev1->nHits;
            int ET0 = -1;
            Emax = 0;
            
            if(Esav->nHits<=1){
              cout<<"Error: Ev = "<<iev<<" ,nH = "<<0<<endl;
              throw;
            }
            Esav->SortGADC();
            for(int i=0;i<Esav->nHits;i++){
              // if(iev==534016){
                // cout<<i<<" "<<Esav->GADC->at(i)<<" "<<Emax<<" "<<int(Esav->GADC->at(i) > Emax) <<endl;
              // }
              if( Esav->GADC->at(i) > Emax){
                Emax = Esav->GADC->at(i);
                ET0 = i;
              }
            }
            if(ET0==-1){
              cout<<"Error: Ev = "<< iev <<endl;
              for(int i=0;i<Esav->nHits;i++){
                cout<<"CH:GADC:iHit: "<<Esav->channel->at(i)<<" : "<<Esav->GADC->at(i)<<" : "<<Esav->iHit->at(i)<<endl;
              }
              throw;
            }
            PeakID = ET0;
            int PeakiX = Esav->iX->at(ET0);
            int PeakiY = Esav->iY->at(ET0);
            Edep11 = Emax;
            Esav->Edep = 0;
            Edep33 = 0;
            EdepXs = 0;
            Edep55 = 0;
            CenpX = 0; CenpY = 0;
            for(int i=0;i<Esav->nHits;i++){
              // CenpX += Esav->pX->at(i)*Esav->GADC->at(i);
              // CenpY += Esav->pY->at(i)*Esav->GADC->at(i);
              int TmpiX = Esav->iX->at(i);
              int TmpiY = Esav->iY->at(i);
              Esav->Edep+=Esav->GADC->at(i);
              if(abs(PeakiX-TmpiX)<2&&abs(PeakiY-TmpiY)<2) Edep33 += Esav->GADC->at(i);
              if((abs(PeakiX-TmpiX) +abs(PeakiY-TmpiY))<2) EdepXs += Esav->GADC->at(i);
              if(abs(PeakiX-TmpiX)<3&&abs(PeakiY-TmpiY)<3){
                Edep55 += Esav->GADC->at(i);
                CenpX += Esav->pX->at(i)*Esav->GADC->at(i);
                CenpY += Esav->pY->at(i)*Esav->GADC->at(i);
              } 
            }
            
            CenpX /= Edep55;
            CenpY /= Edep55;
            Cortex = abs(PeakiX-3.5)>abs(PeakiY-3.5)
                ? abs(PeakiX-3.5)-0.5 
                : abs(PeakiY-3.5)-0.5;
            
            nROC = 2;
            t->Fill();
            
            Ev0->Ini();
            Ev1->Ini();
            dPos = -1;
            MdPos = -1;
            continue;
          }
        }
        *Esav = Event(Ev0);
        nROC = 1;
        if(Esav->nHits<1){
          cout<<"Error306: Ev = "<<iev<<" ,nH = "<<0<<endl;
          for(int i=0;i<Esav->nHits;i++){
            cout<<"CH:GADC:iHit: "<<Esav->channel->at(i)<<" : "<<Esav->GADC->at(i)<<" : "<<Esav->iHit->at(i)<<endl;
          }
          throw;
        }
        float edm = 0;
        Esav->Edep = 0;
        Esav->SortGADC();
        for(int ihit0=0; ihit0 < Esav->nHits ; ihit0++){
          Esav->Edep+=Esav->GADC->at(ihit0);
          if(Esav->GADC->at(ihit0)>edm){
            PeakID = ihit0;
            edm = Esav->GADC->at(ihit0);
          }
        }
        int PeakiX = Esav->iX->at(PeakID);
        int PeakiY = Esav->iY->at(PeakID);
        Edep11 = edm;
        Edep33 = 0;
        EdepXs = 0;
        Edep55 = 0;
        CenpX = 0; CenpY = 0;
        for(int i=0;i<Esav->nHits;i++){
          // CenpX += Esav->pX->at(i)*Esav->GADC->at(i);
          // CenpY += Esav->pY->at(i)*Esav->GADC->at(i);
          int TmpiX = Esav->iX->at(i);
          int TmpiY = Esav->iY->at(i);
          if(abs(PeakiX-TmpiX)<2&&abs(PeakiY-TmpiY)<2) Edep33 += Esav->GADC->at(i);
          if((abs(PeakiX-TmpiX) +abs(PeakiY-TmpiY))<2) EdepXs += Esav->GADC->at(i);
          if(abs(PeakiX-TmpiX)<3&&abs(PeakiY-TmpiY)<3){
            Edep55 += Esav->GADC->at(i);
            CenpX += Esav->pX->at(i)*Esav->GADC->at(i);
            CenpY += Esav->pY->at(i)*Esav->GADC->at(i);
          } 
        }
        if(Edep11*1./Edep55*1.>1 && nROC==1)
          cout<<Esav->eventID<<"\t R = "<<Edep11*1./Edep55*1.<<endl;
        
        CenpX /= Edep55;
        CenpY /= Edep55;
        Cortex = abs(PeakiX-3.5)>abs(PeakiY-3.5)
            ? abs(PeakiX-3.5)-0.5 
            : abs(PeakiY-3.5)-0.5;
        t->Fill();
        *Ev0 = Event(Ev1);
        Ev1->Ini();
        dPos = -1;
        MdPos = -1;
      }
    }
    
  }
  // cout<<98<<endl;
  Rfile->WriteObject(t,"t");
  cout<<"\nFinish convert!"<<endl;
  TH1F *hits = new TH1F("hits","hits",64,0,64);
  hits->GetXaxis()->SetTitle("Number of hit");
  hits->GetYaxis()->SetTitle("Number of event");
  TCanvas *ctmp = new TCanvas("ctmp","ctmp",640+4,480+28);
  t->Draw("nHits>>hits","","h");
  ctmp->Print(Form("%sCMNHitsLin.png",FileRoot.data()));
  ctmp->SetLogy(1);
  ctmp->Print(Form("%sCMNHitsLG.png",FileRoot.data()));
  Rfile->Close();
  
  // system(Form("root -l %s",Rfile->GetName()));
  // system(Form("TBrowser b "));
}