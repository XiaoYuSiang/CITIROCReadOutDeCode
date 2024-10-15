#include <iostream>
#include <vector>
#include <fstream>
#include <typeinfo>
#include <algorithm>
#include <sstream>
/*
root -b -q GetScripe.C
*/
using namespace std;
const int    NumDirs = 1;
const int cpumax = 16;
const string Opmode = "0";
const string BashFile = "FileList241011.sh";
const string VDirs[NumDirs]={
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231214/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231217/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231218/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231221/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231222/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231225/*"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20231229/*VF0C*"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240103/",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240105/",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240110/"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240112/",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240115/"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240118/"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240122/"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/hospital_20240127/"
    
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240130/"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240131/",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240202/"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240216/HNData/",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240217/",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240220/*Run13[5-9]",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240220/*Run1[4-5][0-9]"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240217/Run32*"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240217/*"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240219/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240220/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240221/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240227/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240229/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240301/*",
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240903/*"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/LYSOReadOut/Datas/20240909/*"
  // "/data8/ZDC/EMCal/LYSO/prototype8Xi0/CosmicRayTest/241011/*"
  "/data8/ZDC/EMCal/LYSO/prototype8Xi0/CosmicRayTest/Test/Run349_HV4E0_VFD2_Cosmic_hr__005729.534"
};
const string OutputPath = "./";
void GetScripe(){
  system(Form("mkdir -p %s",OutputPath.data()));
  ofstream outbash((OutputPath+BashFile).data());
  outbash<<"root -b -q -l CompilarV2.C"<<endl;
  int ifiles = 0, icpu = 0;
  for(int i=0;i<NumDirs;i++){
    string dirfs = Form("%sdirfs%d.dat",OutputPath.data(),i);
    system(Form("ls -1 %s*.bin>>%s",VDirs[i].data(),dirfs.data()));
    ifstream in(dirfs.data());
    string strtmp, fileroot, cmdline;
    while(in>>strtmp){
      fileroot = strtmp.substr(0,strtmp.find(".bin"));
      int delay = ifiles ==0? 0 : 0;
      cmdline = Form("sleep %d &&root -l -b -q Read.C\\(\"\\\"",delay);
      cmdline+= fileroot+"\\\"\","+Opmode+"\\) &";
      cout<<cmdline<<endl;
      outbash<<cmdline<<endl;
      ifiles++;
      icpu++;
      if(icpu>=cpumax){
        icpu = 0;
        outbash<<"wait"<<endl;
      }
    }
    system(("rm "+dirfs).data());
    in.close();
  }
  outbash<<"wait"<<endl;
  outbash<<"echo \"Finish the scripe\"\n"<<endl;
  cout<<"finish: "<<(OutputPath+BashFile).data()<<endl;
  cout<<"sh "<<(OutputPath+BashFile).data()<<endl;
  system(Form("sh %s%s",OutputPath.data(),BashFile.data()));
  outbash.close();
}