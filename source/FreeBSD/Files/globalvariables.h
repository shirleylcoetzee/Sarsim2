#include "dest.h"
//---------------------------------------------------------------------------
#ifndef globalvariablesH
#define globalvariablesH

// RTL - replaced <values.h> with <math.h>
//#include <values.h>
#include <math.h>

// for random number generator in file general.cpp
double r_seed = 1.0;

// used for threads
bool UserAbort;     // TRUE if user wants to abort
int CalcProgress;   // show progress (0-100%)
int CalcProgress2;   // Percentage Blocks written ....

// current version number
//char version[30];

// barker codes
int BarkerCode[7][13] = {{1,-1,0,0,0,0,0,0,0,0,0,0,0},
                           {1,1,-1,0,0,0,0,0,0,0,0,0,0},
                           {1,1,-1,1,0,0,0,0,0,0,0,0,0},
                           {1,1,1,-1,1,0,0,0,0,0,0,0,0},
                           {1,1,1,-1,-1,1,-1,0,0,0,0,0,0},
                           {1,1,1,-1,-1,-1,1,-1,-1,1,-1,0,0},
                           {1,1,1,1,1,-1,-1,1,1,-1,1,-1,1}};
int BarkerCodeLength[7] = {2,3,4,5,7,11,13};

// default data for a new object
// setup default values for a point target
STarget  DefaultTargetValues =
    { "Earth",{0,0,0},{0,0,0},   // Pos, Posdev
	  1,0,0,0,0,0,0,0,    // RCS,RCSdev,ReflecType,Dir*4,GainType
	  { ID_INLINE,{LINEAR,LINEAR,LINEAR},"",1,
	    {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
	    { {"Offset Angle (°)",1,DEGREES,-1,NULL,0,180,0,180},
	      {"",1,NONE,-1,NULL,0,0,0,0},
	      {"",1,NONE,-1,NULL,0,0,0,0}
	    },
	    {  {"Reflectivity (dB)",1,DB,-1,NULL,-30,0,-100,0},
	       {"",1,NONE,-1,NULL,0,0,0,0},
	 	   {"",1,NONE,-1,NULL,0,0,0,0}
	    }
	  },NULL,NULL};
    
// setup default values for a point Surface
SSurface  DefaultSurfaceValues =
//    { "Earth",{{10000,-3000,0},{10000,3000,0},{12000,0,2000}},   // Pos
//    { "Earth",{{1,2,0},{3,4,0},{-1,-2,0}},   // Pos
//    { "Earth",{{-20,30,0},{0,-30,0},{20,30,0}},   // Pos
//    { "Earth",{{0,-30,0},{-20,30,0},{20,30,0}},   // Pos
    { "Earth",{{0,-30,0},{-40,30,0},{40,30,0}},   // Pos
//    { "Earth",{{-50,90,0},{0,-90,0},{50,90,0}},   // Pos
//    { "Earth",{{-400,800,500},{400,600,700},{500,1000,200}},   // Pos
	    NULL,NULL};

// setup default values for a platform
SPlatform DefaultPlatformValues =
    {"Earth",0,{0,0,0},
	{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",3,
	  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
      {  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	     {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	     {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	  },
	  {  {"Position X (m)",1,METERS,-1,NULL,-1000,1000,MIN_FLOAT,MAXFLOAT},
	     {"Position Y (m)",1,METERS,-1,NULL,-1000,1000,MIN_FLOAT,MAXFLOAT},
	     {"Position Z (m)",1,METERS,-1,NULL,-1000,1000,MIN_FLOAT,MAXFLOAT},
	  }
	},{0,0,0}, 0,
	{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",3,
	  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
      {  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	     {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	     {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	  },
	  {  {"Rotation X (deg)",1,DEGREES,-1,NULL,-100,100,MIN_FLOAT,MAXFLOAT},
	     {"Rotation Y (deg)",1,DEGREES,-1,NULL,-100,100,MIN_FLOAT,MAXFLOAT},
	     {"Rotation Z (deg)",1,DEGREES,-1,NULL,-100,100,MIN_FLOAT,MAXFLOAT},
	  }
   },
  {0,0,0},FALSE,0,0,{0,0,0},{0,0,0},
	{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",3,
	  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
	  {  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
         {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	     {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT}
	  },
    {  {"Motion Error X (m)",1,METERS,-1,NULL,-1,1,MIN_FLOAT,MAXFLOAT},
       {"Motion Error Y (m)",1,METERS,-1,NULL,-1,1,MIN_FLOAT,MAXFLOAT},
       {"Motion Error Z (m)",1,METERS,-1,NULL,-1,1,MIN_FLOAT,MAXFLOAT}
    }
    },
     { ID_INLINE,{LINEAR,LINEAR,LINEAR},"",3,
  	  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
  	  {  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
    	  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
	      {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT}
	  },
	  {  {"Motion Error X (m)",1,METERS,-1,NULL,-1,1,MIN_FLOAT,MAXFLOAT},
	     {"Motion Error Y (m)",1,METERS,-1,NULL,-1,1,MIN_FLOAT,MAXFLOAT},
	     {"Motion Error Z (m)",1,METERS,-1,NULL,-1,1,MIN_FLOAT,MAXFLOAT}
	  }
	},NULL,NULL };

// setup default values for a radar
SRadar    DefaultRadarValues = {"Earth","Radar1",0,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",2,     //PulseDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Pulsetime (0-1)",1,NONE,-1,NULL,0,1,0,1},
											  {"Pulsetime (0-1)",1,NONE,-1,NULL,0,1,0,1},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Amplitude I (V)",1,VOLT,-1,NULL,-100,100,MIN_FLOAT,MAXFLOAT},
											  {"Amplitude Q (V)",1,VOLT,-1,NULL,-100,100,MIN_FLOAT,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},
									5E7,6, 5E-6,0,0,0,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",1,   //EnvelopeDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Pulsetime (0-1)",1,NONE,-1,NULL,0,1,0,1},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Amplitude (Unity)",1,NONE,-1,NULL,0,1,0,1},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},
									0,1000,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",1,   //PRFDef
										  {INTEGER,INTEGER,INTEGER},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Pulse (n)",1,NONE,-1,NULL,0,100,0,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"PRI (s)",1,SECONDS,-1,NULL,0,1E-3,0,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},
									0,1E9,1E9,1E8,66,2,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",1,    //FreqDef
										  {INTEGER,INTEGER,INTEGER},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Pulse (n)",1,NONE,-1,NULL,0,100,0,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Frequency (GHz)",1,GHERTZ,-1,NULL,0,2,0,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},
									1E3,1,0,0,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",2,   //AntennaTDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Angle (°)",1,DEGREES,-1,NULL,0,360,0,360},
											  {"Angle (°)",1,DEGREES,-1,NULL,0,360,0,360},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Gain (Azimuth) dB",1,DB,-1,NULL,0,100,MIN_FLOAT,MAXFLOAT},
											  {"Gain (Elevation) dB",1,DB,-1,NULL,0,100,MIN_FLOAT,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},
								 10*DegToRad,10*DegToRad,2,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",2,  //AntennaRDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Angle (°)",1,DEGREES,-1,NULL,0,360,0,360},
											  {"Angle (°)",1,DEGREES,-1,NULL,0,360,0,360},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Gain (Azimuth) dB",1,DB,-1,NULL,0,100,MIN_FLOAT,MAXFLOAT},
											  {"Gain (Elevation) dB",1,DB,-1,NULL,0,100,MIN_FLOAT,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},
								10*DegToRad,10*DegToRad,0,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",3,  //DirectionTDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
											  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT},
											  {"Time (s)",1,SECONDS,-1,NULL,0,10,MIN_FLOAT,MAXFLOAT}
										  },
										  {  {"Position X (m)",1,METERS,-1,NULL,-1000,1000,MIN_FLOAT,MAXFLOAT},
											  {"Position Y (m)",1,METERS,-1,NULL,-1000,1000,MIN_FLOAT,MAXFLOAT},
											  {"Position Z (m)",1,METERS,-1,NULL,-1000,1000,MIN_FLOAT,MAXFLOAT}
										  }
										},
								0,0,0,0,{0,0,0},0,1,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",1,  //MFDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Time (0-Pulsewidth)",1,NONE,-1,NULL,0,1,0,1},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Amplitude",1,NONE,-1,NULL,0,1,0,1},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										},

								 FALSE,0,
										{ ID_INLINE,{LINEAR,LINEAR,LINEAR},"",1,  //AGCDef
										  {0,0,0},{0,0,0},{0,0,0},{NULL,NULL,NULL},
										  {  {"Distance (m)",1,METERS,-1,NULL,0,10000,0,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  },
										  {  {"Gain",1,METERS,-1,NULL,0,1000,0,MAXFLOAT},
											  {"",1,NONE,-1,NULL,0,0,0,0},
											  {"",1,NONE,-1,NULL,0,0,0,0}
										  }
										}, NULL,NULL
								  };
struct SSimulation DefaultSimulationValues = {"","sim1.bin",0,0,0,0.01,REAL,8,0,0,BINARY,
											RAW_RETURN,0,NULL,NULL};

struct SGeometry DefaultGeometryValues = {"","geo1.txt",0,100000,0,0.1,0,227,NULL,NULL};

#endif
