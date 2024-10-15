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
#include "TF1.h"
#include "TCanvas.h"
#include "./GIDMapping.h"
using namespace std;
// const int NumAllChs = 64;
struct DBMHit{
  bool HGTMID, HGMode;
  Int_t channel, Fire, ADC, ROCID, GID, iHit;
  Int_t iX, iY, iZ;
  float pX, pY, pZ, GADC;
};
bool comparator(DBMHit a, DBMHit b){ return a.GADC > b.GADC; };
class DBEvent{
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
  Int_t nROC;
  float dPos, MdPos;
  float Edep11, Edep33, Edep55, EdepXs;
  DBEvent(DBEvent* Ev0){
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
    nROC = Ev0->nROC;
    dPos = Ev0->dPos;
    MdPos = Ev0->MdPos;
    Edep11 = Ev0->Edep11;
    Edep33 = Ev0->Edep33;
    Edep55 = Ev0->Edep55;
    EdepXs = Ev0->EdepXs;
  }
  DBEvent(){
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
    nROC = 0;     dPos = 0;    MdPos = 0;
    Edep11 = 0;    Edep33 = 0;    Edep55 = 0;    EdepXs = 0;
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
    T->SetBranchAddress("nROC",      &nROC      );
    T->SetBranchAddress("dPos",      &dPos      );
    T->SetBranchAddress("MdPos",     &MdPos     );
    // T->SetBranchAddress("Edep",      &Edep      );
    // T->SetBranchAddress("Edep11",    &Edep11    );
    // T->SetBranchAddress("Edep33",    &Edep33    );
    // T->SetBranchAddress("Edep55",    &Edep55    );
    // T->SetBranchAddress("EdepXs",    &EdepXs    );
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
    T->Branch("nROC",      &nROC      );
    T->Branch("dPos",      &dPos      );
    T->Branch("MdPos",     &MdPos     );
    T->Branch("Edep",      &Edep      );
    T->Branch("Edep11",    &Edep11    );
    T->Branch("Edep33",    &Edep33    );
    T->Branch("Edep55",    &Edep55    );
    T->Branch("EdepXs",    &EdepXs    );
  }
};

void PackageTestReEnergy(
  string FileRoot = "/data4/YuSiang/LYSOReadOut/Datas/20240118/RB2_SIPM1_VFEA_HV4E_CosRay_Pb_4Hr_030336.960/" , 
  string FileName = "RB2_SIPM1_VFEA_HV4E_CosRay_Pb_4Hr_030336.960"
){
  float GRatio[NumAllChs]={0};
  TFile *file = new TFile(Form("%s%s_Sci_CB_GADC.root",FileRoot.data(),FileName.data()),"read");
  TFile *Rfile= new TFile(Form("%s%s_Sci_CB_GADC_ReEdep.root",FileRoot.data(),FileName.data()),"recreate");
  cout<<"In file = "<<file->GetName()<<endl;
  
  DBEvent *Etmp = new DBEvent;
  TTree *t0 = (TTree*) file->Get("t");
  Etmp->SetTBranchAddress(t0);
  
  
  TTree *t = new TTree("t", "frame data");
  DBEvent *Esav = new DBEvent;
  Esav->SetTBranch(t); 
  vector<float> Energy;
  t->Branch("EdepXs",    &Energy    );
  
  Long64_t evs = t0->GetEntries();
  // TF1 *fGADCtoEdep = new TF1("fGADCtoEdep","log(1-x/5294.07)/(-0.0221)",0,11001);
  TF1 *fGADCtoEdep = new TF1("fGADCtoEdep","log(1-x/5337.62)/(-0.0186)",0,11001);
  for(Long64_t iev=0;iev<evs*1.;iev++){
    t0->GetEntry(iev);
    if(iev%1000 == 0) 
      cout<<"\r "<<iev/float(evs)*100.<<"%"<<flush;
    
    *Esav = DBEvent(Etmp);
    int PeakiX = Esav->iX->at(0);
    int PeakiY = Esav->iY->at(0);
    float Echannel = fGADCtoEdep->Eval(Esav->GADC->at(0));
    Esav->Edep11 = Echannel;
    Esav->Edep   = Echannel;
    Esav->Edep33 = Echannel;
    Esav->EdepXs = Echannel;
    Esav->Edep55 = Echannel;
    Energy.push_back(Echannel);
    for(int i=1;i<Esav->nHits;i++){
      Echannel = fGADCtoEdep->Eval(Esav->GADC->at(i));
      Energy.push_back(Echannel);
      int TmpiX = Esav->iX->at(i);
      int TmpiY = Esav->iY->at(i);
      Esav->Edep += Echannel;
      if(abs(PeakiX-TmpiX)<2&&abs(PeakiY-TmpiY)<2) Esav->Edep33 += Echannel;
      if((abs(PeakiX-TmpiX) +abs(PeakiY-TmpiY))<2) Esav->EdepXs += Echannel;
      if(abs(PeakiX-TmpiX)<3&&abs(PeakiY-TmpiY)<3) Esav->Edep55 += Echannel;
    }
    if(Esav->Edep11*1./Esav->Edep55*1.>1 && Esav->nROC==1) cout<<Esav->eventID<<"\t R = "<<Esav->Edep11*1./Esav->Edep55*1.<<endl;
    t->Fill();
    Energy.clear();
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
}