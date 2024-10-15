#pragma once

#include <typeinfo>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>
#include <TString.h>
using namespace std;
const int NumAllChs = 64, NumChs    = 32, NumROCs   = 2;
int proGID[NumAllChs] ={//general Index of channels in ROC A/B
	0,	8,	16,	17,	39,	40,	48,	56,
	1,	9,	18,	19,	38,	41,	49,	57,
	2,	10,	20,	21,	37,	42,	50,	58,
	3,	11,	22,	23,	36,	43,	51,	59,
	4,	12,	24,	25,	35,	44,	55,	60,
	5,	13,	26,	29,	34,	45,	54,	61,
	6,	14,	28,	30,	33,	46,	53,	62,
	7,	15,	27,	31,	32,	47,	52,	63
};
int proiX[NumAllChs] ={//index of X at channel 
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0,
	7,	6,	5,	4,	3,	2,	1,	0
};
int proiY[NumAllChs] ={//index of Y at channel 
	0,	0,	0,	0,	0,	0,	0,	0,
	1,	1,	1,	1,	1,	1,	1,	1,
	2,	2,	2,	2,	2,	2,	2,	2,
	3,	3,	3,	3,	3,	3,	3,	3,
	4,	4,	4,	4,	4,	4,	4,	4,
	5,	5,	5,	5,	5,	5,	5,	5,
	6,	6,	6,	6,	6,	6,	6,	6,
	7,	7,	7,	7,	7,	7,	7,	7
};
float propX[NumAllChs] ={//position of Y at channel 
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2,
	25.2,	18,	10.8,	3.6,	-3.6,	-10.8,	-18,	-25.2
};
float propY[NumAllChs] ={//position of Y at channel 
	-25.2,	-25.2,	-25.2,	-25.2,	-25.2,	-25.2,	-25.2,	-25.2,
	-18,	-18,	-18,	-18,	-18,	-18,	-18,	-18,
	-10.8,	-10.8,	-10.8,	-10.8,	-10.8,	-10.8,	-10.8,	-10.8,
	-3.6,	-3.6,	-3.6,	-3.6,	-3.6,	-3.6,	-3.6,	-3.6,
	3.6,	3.6,	3.6,	3.6,	3.6,	3.6,	3.6,	3.6,
	10.8,	10.8,	10.8,	10.8,	10.8,	10.8,	10.8,	10.8,
	18,	18,	18,	18,	18,	18,	18,	18,
	25.2,	25.2,	25.2,	25.2,	25.2,	25.2,	25.2,	25.2
};


class Sensor{//class for 1 sensor imformation
public:
  int   GID, RID, CID;
  int   iX , iY , iZ ;
  float pX , pY , pZ ;
  Sensor(int inGID = -1){
    GID = inGID; RID = -1; CID = -1;
    iX  = 0; iY  = 0; iZ  = 0;
    pX  = 0; pY  = 0; pZ  = 0;
  };
  void SetSensorID(int inGID, int inRID, int inCID){
    GID = inGID; RID = inRID; CID = inCID;
  };
  void SetSensorIXYZ(int iniX, int iniY, int iniZ){
    iX = iniX; iY = iniY; iZ = iniZ;
  };
  void SetSensorPXYZ(float inpX, float inpY, float inpZ){
    pX = inpX; pY = inpY; pZ = inpZ;
  };
  string Show(){
    string out = 
      Form(
        "%d , %d , %d , %d , %d , %d , %f , %f , %f;\n",
        GID, RID, CID,  iX , iY , iZ, pX , pY , pZ
      );
    return out;
  }
};


// Load the sensors by the setup imformation into the map
map< int, Sensor > LoadSensorMap(bool quiet = false){
  cout<<"Start to load the sensor position vs ID mapping"<<endl;
  map< int, Sensor > sensorMap;
  for(int ich = 0; ich < NumAllChs ; ich++ ){
    Sensor TmpSensor(-1);
    TmpSensor.SetSensorID(proGID[ich],proGID[ich]/NumChs,proGID[ich]%NumChs);
    TmpSensor.SetSensorIXYZ(proiX[ich],proiY[ich],0);
    TmpSensor.SetSensorPXYZ(propX[ich],propY[ich],0);
    sensorMap[TmpSensor.GID] = TmpSensor;
  }
  if(!quiet){
    cout<<"Shows the mapping :"<<endl;
    cout<<"GID, RID, CID,  iX , iY , iZ, pX , pY , pZ"<<endl;
    for(int ich = 0; ich < NumAllChs ; ich++ ){
      cout<<sensorMap[ich].Show();
    }
  }
  cout<<"Finish to load the sensor position vs ID mapping"<<endl;
  return sensorMap;
}