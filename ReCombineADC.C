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
#include "./GIDMapping.h"
using namespace std;
// const int NumAllChs = 64;
void ReCombineADC(string FileRoot, string FileName){
  float GRatio[NumAllChs]={0};
  ifstream in(Form("%sRChHGDivLG.ptxt",FileRoot.data()));
  TFile *file = new TFile(Form("%s%s_Sci.root",FileRoot.data(),FileName.data()),"read");
  TFile *Rfile= new TFile(Form("%s%s_Sci_GADC.root",FileRoot.data(),FileName.data()),"recreate");
  
  string line;
  int    Ivaltmp, gid;
  float  fvaltmp, hgDlg;//, rhlg;
  getline(in,line); cout<<line<<endl;
  while(in>>gid>>Ivaltmp>>Ivaltmp>>Ivaltmp>>hgDlg>>fvaltmp){
    if(hgDlg!=0&&hgDlg!=-1) GRatio[gid] = hgDlg;
    else GRatio[gid] = 10;
    cout<<gid<<"\t"<<GRatio[gid]<<endl;
  }
  cout<<"L29"<<endl;
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
  float Edep,Edep3,Edep5,EdepA;//test variable !! Sum of all channel, 100~120ev/ADC
  
  TTree *t = new TTree("t", "frame data");
  t->Branch("eventID",   &eventID   );
  t->Branch("SeqSAD",    &SeqSAD    );
  t->Branch("UTCDay",    &UTCDay    );
  t->Branch("UTCHour",   &UTCHour   );
  t->Branch("UTCMin",    &UTCMin    );
  t->Branch("UTCSec",    &UTCSec    );
  t->Branch("UTCSecS",   &UTCSecS   );
  t->Branch("pcnt",      &pcnt      );
  t->Branch("fcnt",      &fcnt      );
  t->Branch("dfcnt",     &dfcnt     );
  t->Branch("nHits",     &nHits     );
  t->Branch("Edep",      &Edep      );
  t->Branch("Edep3",     &Edep3     );
  t->Branch("Edep5",     &Edep5     );
  t->Branch("EdepA",     &EdepA     );
  t->Branch("iHit",      &iHit      );
  t->Branch("Fire",      &Fire      ); 
  t->Branch("HGTMID",    &HGTMID    );
  t->Branch("ROCID",     &ROCID     );
  t->Branch("channel",   &channel   );
  t->Branch("HGMode",    &HGMode    );
  t->Branch("ADC",       &ADC       );
  t->Branch("GADC",      &GADC      );
  t->Branch("GID",       &GID       );
  t->Branch("iX",        &iX        );
  t->Branch("iY",        &iY        );
  t->Branch("iZ",        &iZ        );
  t->Branch("pX",        &pX        );
  t->Branch("pY",        &pY        );
  t->Branch("pZ",        &pZ        );
  
  
  TTree *t0 = (TTree*) file->Get("t");
  t0->SetBranchAddress("eventID",   &eventID   );
  t0->SetBranchAddress("SeqSAD",    &SeqSAD    );
  t0->SetBranchAddress("UTCDay",    &UTCDay    );
  t0->SetBranchAddress("UTCHour",   &UTCHour   );
  t0->SetBranchAddress("UTCMin",    &UTCMin    );
  t0->SetBranchAddress("UTCSec",    &UTCSec    );
  t0->SetBranchAddress("UTCSecS",   &UTCSecS   );
  t0->SetBranchAddress("pcnt",      &pcnt      );
  t0->SetBranchAddress("fcnt",      &fcnt      );
  t0->SetBranchAddress("dfcnt",     &dfcnt     );
  t0->SetBranchAddress("nHits",     &nHits     );
  t0->SetBranchAddress("Edep",      &Edep      );
  t0->SetBranchAddress("iHit",      &iHit      );
  t0->SetBranchAddress("Fire",      &Fire      ); 
  t0->SetBranchAddress("HGTMID",    &HGTMID    );
  t0->SetBranchAddress("ROCID",     &ROCID     );
  t0->SetBranchAddress("channel",   &channel   );
  t0->SetBranchAddress("HGMode",    &HGMode    );
  t0->SetBranchAddress("ADC",       &ADC       );
  t0->SetBranchAddress("GID",       &GID       );
  t0->SetBranchAddress("iX",        &iX        );
  t0->SetBranchAddress("iY",        &iY        );
  t0->SetBranchAddress("iZ",        &iZ        );
  t0->SetBranchAddress("pX",        &pX        );
  t0->SetBranchAddress("pY",        &pY        );
  t0->SetBranchAddress("pZ",        &pZ        );
  Long64_t evs = t0->GetEntries();
  
  for(Long64_t iev=0;iev<evs;iev++){
    t0->GetEntry(iev);
    if(iev%1000 == 0) cout<<"\r "<<iev/float(evs)*100.<<"%"<<flush;
    Edep = 0; Edep3 = 0; Edep5 = 0; EdepA = 0;
    for(int iH=0;iH<nHits;iH++){
      int gidtmp = GID->at(iH), ADCtmp = ADC->at(iH);
      
      bool HGModeON = HGMode->at(iH);
      // GADC.push_back((ADC->at(iH))*(HGMode->at(iH)?1:GRatio[gidtmp])); 
      // if(HGModeON) 
      GADC.push_back(HGModeON?ADCtmp/GRatio[gidtmp]:ADCtmp); 
      
      // else{//} if(!HGMode->at(iH)){
        // GADC.push_back(); 
      // }
      // cout<<ADCtmp<<" "<<GRatio[gidtmp]<<" "<<(HGModeON);
      // cout<<" "<<GADC[iH]<<endl;
    }
    int iHMax = 0;
    for(int iH=0;iH<nHits;iH++){
      if(iHit->at(iH)==0){
        iHMax = iH;
        Edep=GADC[iH];
        break;
      }
    }
    for(int iH=0;iH<nHits;iH++){
      EdepA +=GADC[iH];
      int dx = abs(iX->at(iH)-iX->at(iHMax)), dy = abs(iY->at(iH)-iY->at(iHMax));//, dz = abs(iZ->at(iH)-iZ->at(iHMax));
      if(dx<=2&&dy<=2){
        Edep5+=GADC[iH];
        if(dx<=1&&dy<=1){
          Edep3+=GADC[iH];
          
        }
      }
    }
    t->Fill(); //cout<<107<<endl;
    GADC.clear(); //cout<<108<<endl;
    // break;
    // if(iev/float(evs)*100.>0.1) break;
  }
  
  Rfile->WriteObject(t,"t");
  cout<<"\nFinish convert!"<<endl;
  Rfile->Close();
}