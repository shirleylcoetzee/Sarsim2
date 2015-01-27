#include "dest.h"
//---------------------------------------------------------------------------

#ifdef COMPILE_FOR_WINDOWS32
  #include <vcl\vcl.h>
  #pragma hdrstop
  #include "main.h"
#endif
//---------------------------------------------------------------------------
#include "loadsavedata.h"
#include "parser.h"
#include "objectstructdef.h"
#include "general.h"
#include "engine.h"
#include "linkedlist.h"
#include <values.h>
#include <cstring>

extern const char FunctionNames[MAX_FUNCTIONS][12];
extern struct SSimulation DefaultSimulationValues;
extern struct SPlatform DefaultPlatformValues;
extern struct STarget DefaultTargetValues;
extern struct SRadar DefaultRadarValues;
extern struct SSurface DefaultSurfaceValues;
extern struct SGeometry DefaultGeometryValues;


// geometry parser
struct SStateM StateMG[8] = {
				// state 0
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {1,0,0,0,0},NULL     // next states
				  },
				// state 1
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {2,0,0,0,0},NULL     // next states
				  },
				// state 2
				  { 1,MIN_DOUBLE, MAX_DOUBLE, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {3,0,0,0,0},NULL              // next states
				  },
				// state 3
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,                    // Type, min, max
					 {"","","","",""},    // names
					 {4,0,0,0,0},NULL             // next states
				  },
				// state 4
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {5,0,0,0,0},NULL               // next states
				  },
				// state 5
				  { 12, 0, 0,1,             // Type, min, max
					 {"SPA","COM","","",""},    // names
					 {6,6,0,0,0},NULL     // next states
				  },
				// state 6
				  { 0, 0,255,1,     // Type, min, max
					 {"","","","",""},              // names
					 {7,0,0,0,0},NULL               // next states
				  },
				// state 7
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {-1,0,0,0,0},NULL     // next states
				  },
     };

// simulation parser
struct SStateM StateMS[14] = {
				// state 0
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {1,0,0,0,0},NULL     // next states
				  },
				// state 1
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {2,0,0,0,0},NULL     // next states
				  },
				// state 2
				  { 1,MIN_DOUBLE, MAX_DOUBLE, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {3,0,0,0,0},NULL              // next states
				  },
				// state 3
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,                    // Type, min, max
					 {"","","","",""},    // names
					 {4,0,0,0,0},NULL             // next states
				  },
				// state 4
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {5,0,0,0,0},NULL               // next states
				  },
				// state 5
				  { 0, 2, 64,1,     // Type, min, max
					 {"","","","",""},              // names
					 {6,0,0,0,0},NULL               // next states
				  },
				// state 6
				  { 1, 0.001,MAX_DOUBLE,1E9,     // Type, min, max
					 {"","","","",""},              // names
					 {7,0,0,0,0},NULL               // next states
				  },
				// state 7
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1E-3,             // Type, min, max
					 {"","","","",""},    // names
					 {8,0,0,0,0},NULL     // next states
				  },
				// state 8
				  { 13, 0, 0,1,             // Type, min, max
					 {"ASC","BIN","BRA","",""},    // names
					 {9,9,9,0,0},NULL     // next states
				  },
				// state 9
				  { 13, 0, 0,1,     // Type, min, max
					 {"RAW","MAT","SRP","",""},              // names
					 {10,10,10,0,0},NULL               // next states
				  },
				// state 10
				  { 2, 0, 0,1,        // Type, min, max
					 {"","","","",""},    // names
					 {11,0,0,0,0},NULL           // next states
				  },
				// state 11
				  { 12, 0, 0,1,        // Type, min, max
					 {"BEG","CEN","","",""},    // names
					 {12,12,0,0,0},NULL           // next states
				  },
				// state 12
				  { 1, 1, 100,1,        // Type, min, max
					 {"","","","",""},    // names
					 {13,0,0,0,0},NULL           // next states
				  },
				// state 13
				  { 1, 1, 100,1,        // Type, min, max
					 {"","","","",""},    // names
					 {-1,0,0,0,0},NULL           // next states
				  },
		};
// radar parser
struct SStateM StateMR[47] = {
				// state 0
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {1,0,0,0,0},NULL     // next states
				  },
				// state 1
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {2,0,0,0,0},NULL     // next states
				  },
				// state 2
				  { 14, 0, 0, 1,     // Type, min, max
					 {"MON","CHI","OTH","BAR",""},              // names
					 {5,3,4,46,0},NULL              // next states
				  },
				// state 3
				  { 1, 0, MAX_DOUBLE, 1E9,                    // Type, min, max
					 {"","","","",""},    // names
					 {5,0,0,0,0},NULL             // next states
				  },
				// state 4
				  { 3, 0,0,1,     // Type, min, max
					 {"","","","",""},              // names
					 {5,0,0,0,0},NULL               // next states
				  },
				// state 5
				  { 1, 0, MAX_DOUBLE,1E-9,     // Type, min, max
					 {"","","","",""},              // names
					 {6,0,0,0,0},NULL               // next states
				  },
				// state 6
				  { 13, 0,0,1,     // Type, min, max
					 {"REC","LIN","OTH","",""},              // names
					 {10,7,9,0,0},NULL               // next states
				  },
				// state 7
				  { 1, 0, MAX_DOUBLE,1E-9,             // Type, min, max
					 {"","","","",""},    // names
					 {8,0,0,0,0},NULL     // next states
				  },
				// state 8
				  { 1, 0, MAX_DOUBLE,1E-9,             // Type, min, max
					 {"","","","",""},    // names
					 {10,0,0,0,0},NULL     // next states
				  },
				// state 9
				  { 3, 0, 0,1,     // Type, min, max
					 {"","","","",""},              // names
					 {10,0,0,0,0},NULL               // next states
				  },
				// state 10
				  { 12, 0, 0,1,        // Type, min, max
					 {"CON","OTH","","",""},    // names
					 {11,12,0,0,0},NULL           // next states
				  },
				// state 11
				  { 1, 0.00001, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {13,0,0,0,0},NULL               // next states
				  },
				// state 12
				  { 3, 0, 0,1,     // Type, min, max
					 {"","","","",""},              // names
					 {13,0,0,0,0},NULL               // next states
				  },
				// state 13
				  { 13, 0, 0,1,     // Type, min, max
					 {"SIN","STE","OTH","",""},              // names
					 {14,15,19,0,0},NULL               // next states
				  },
				// state 14
				  { 1, 0, MAX_DOUBLE ,1E9,     // Type, min, max
					 {"COS","OTH","","",""},              // names
					 {20,0,0,0,0},NULL               // next states
				  },
				// state 15
				  { 1, 0, MAX_DOUBLE, 1E9,     // Type, min, max
					 {"","","","",""},              // names
					 {16,0,0,0,0},NULL               // next states
				  },
				// state 16
				  { 1, 0, MAX_DOUBLE,1E9,     // Type, min, max
					 {"","","","",""},              // names
					 {17,0,0,0,0},NULL               // next states
				  },
				// state 17
				  { 0, 1, 1000, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {18,10,0,0,0},NULL              // next states
				  },
				// state 18
				  { 0, 1, 100, 1E9,                    // Type, min, max
					 {"","","","",""},    // names
					 {20,0,0,0,0},NULL             // next states
				  },
				// state 19
				  { 3, 0,0,1,     // Type, min, max
					 {"","","","",""},              // names
					 {20,0,0,0,0},NULL               // next states
				  },
				// state 20
				  { 1, 0, MAX_DOUBLE,1E3,     // Type, min, max
					 {"","","","",""},              // names
					 {21,0,0,0,0},NULL               // next states
				  },
				// state 21
				  { 1, 0,MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {22,7,9,0,0},NULL               // next states
				  },
				// state 22
				  { 1, 0, MAX_DOUBLE,1,             // Type, min, max
					 {"","","","",""},    // names
					 {23,0,0,0,0},NULL     // next states
				  },
				// state 23
				  { 13, 0, 0,1,             // Type, min, max
					 {"ISO","SIN","OTH","",""},    // names
					 {27,24,26,0,0},NULL     // next states
				  },
				// state 24
				  { 1, 0, 360,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {25,0,0,0,0},NULL               // next states
				  },
				// state 25
				  { 1, 0, 360,DegToRad,        // Type, min, max
					 {"","","","",""},    // names
					 {27,0,0,0,0},NULL           // next states
				  },
				// state 26
				  { 3, 0, 0,1,     // Type, min, max
					 {"","","","",""},              // names
					 {27,0,0,0,0},NULL               // next states
				  },
				// state 27
				  { 14, 0, 0,1,     // Type, min, max
					 {"ISO","SIN","SAM","OTH",""},              // names
					 {31,28,31,30,0},NULL               // next states
				  },
				// state 28
				  { 1, 0,360,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {29,0,0,0,0},NULL               // next states
				  },
				// state 29
				  { 1, 0, 360 ,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {31,0,0,0,0},NULL               // next states
				  },
				// state 30
				  { 3, 0, 0, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {31,0,0,0,0},NULL               // next states
				  },
				// state 31
				  { 14, 0, 0,1,     // Type, min, max
					 {"FIX","ROT","SPO","OTH",""},              // names
					 {32,34,36,39,0},NULL               // next states
				  },
				// state 32
				  { 1, -90, 90, DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {33,0,0,0,0},NULL              // next states
				  },
				// state 33
				  { 1, -360, 360,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {40,0,0,0,0},NULL               // next states
				  },
				// state 34
				  { 1, -90,90,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {35,0,0,0,0},NULL               // next states
				  },
				// state 35
				  { 1, 0, MAX_DOUBLE,DegToRad,             // Type, min, max
					 {"","","","",""},    // names
					 {40,0,0,0,0},NULL     // next states
				  },
				// state 36
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,             // Type, min, max
					 {"","","","",""},    // names
					 {37,0,0,0,0},NULL     // next states
				  },
				// state 37
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {38,0,0,0,0},NULL               // next states
				  },
				// state 38
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,        // Type, min, max
					 {"","","","",""},    // names
					 {40,0,0,0,0},NULL           // next states
				  },
				// state 39
				  { 3, 0,0,1,     // Type, min, max
					 {"","","","",""},              // names
					 {40,0,0,0,0},NULL               // next states
				  },
				// state 40
				  { 15, 0, 0,1,     // Type, min, max
					 {"REC","HAN","BAR","HAM","OTH"},              // names
					 {43,41,43,43,42},NULL               // next states
				  },
				// state 41
				  { 1, 0, 1,1,     // Type, min, max
					 {"","","","",""},              // names
					 {43,0,0,0,0},NULL               // next states
				  },
				// state 42
				  { 3, 0, 0 ,1,     // Type, min, max
					 {"","","","",""},              // names
					 {43,0,0,0,0},NULL               // next states
				  },
				// state 43
				  { 12, 0, 0, 1,     // Type, min, max
					 {"NO_","APP","","",""},              // names
					 {-1,44,0,0,0},NULL               // next states
				  },
				// state 44
				  { 12, 0, 0,1,     // Type, min, max
					 {"1OV","OTH","","",""},              // names
					 {-1,45,0,0,0},NULL               // next states
				  },
				// state 45
				  { 3, 0, 0, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {-1,0,0,0,0},NULL              // next states
				  },
				// state 46
				  { 0, 0, 6, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {5,0,0,0,0},NULL              // next states
				  },
	 };

// surface parser
struct SStateM StateMSur[16] = {
				// state 0
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {1,0,0,0,0},NULL     // next states
				  },
				// state 1
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,                    // Type, min, max
					 {"","","","",""},    // names
					 {2,0,0,0,0},NULL             // next states
				  },
				// state 2
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {3,0,0,0,0},NULL               // next states
				  },
				// state 3
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {4,0,0,0,0},NULL               // next states
				  },
				// state 4
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,                    // Type, min, max
					 {"","","","",""},    // names
					 {5,0,0,0,0},NULL             // next states
				  },
				// state 5
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {6,0,0,0,0},NULL               // next states
				  },
				// state 6
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {7,0,0,0,0},NULL               // next states
				  },
				// state 7
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,                    // Type, min, max
					 {"","","","",""},    // names
					 {8,0,0,0,0},NULL             // next states
				  },
				// state 8
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {9,0,0,0,0},NULL               // next states
				  },
				// state 9
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {-1,0,0,0,0},NULL               // next states
				  }
	 };

// target parser
struct SStateM StateMT[16] = {
				// state 0
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {1,0,0,0,0},NULL     // next states
				  },
				// state 1
				  { 1, MIN_DOUBLE, MAX_DOUBLE, 1,                    // Type, min, max
					 {"","","","",""},    // names
					 {2,0,0,0,0},NULL             // next states
				  },
				// state 2
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {3,0,0,0,0},NULL               // next states
				  },
				// state 3
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {4,0,0,0,0},NULL               // next states
				  },
				// state 4
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {5,0,0,0,0},NULL               // next states
				  },
				// state 5
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,             // Type, min, max
					 {"","","","",""},    // names
					 {6,0,0,0,0},NULL     // next states
				  },
				// state 6
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,             // Type, min, max
					 {"","","","",""},    // names
					 {7,7,0,0,0},NULL     // next states
				  },
				// state 7
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {8,0,0,0,0},NULL               // next states
				  },
				// state 8
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {9,0,0,0,0},NULL               // next states
				  },
				// state 9
				  { 12, 0, 0, 1,     // Type, min, max
					 {"ISO","DIR","","",""},              // names
					 {-1,10,0,0,0},NULL              // next states
				  },
				// state 10
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,        // Type, min, max
					 {"","","","",""},    // names
					 {11,17,0,0,0},NULL           // next states
				  },
				// state 11
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {12,0,0,0,0},NULL               // next states
				  },
				// state 12
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {13,0,0,0,0},NULL               // next states
				  },
				// state 13
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {14,0,0,0,0},NULL               // next states
				  },
				// state 14
				  { 12, 0, 0 ,1,     // Type, min, max
					 {"COS","OTH","","",""},              // names
					 {-1,15,0,0,0},NULL               // next states
				  },
				// state 15
				  { 3, 0, 0, 1,     // Type, min, max
					 {"","","","",""},              // names
					 {-1,0,0,0,0},NULL               // next states
				  },
	 };

// platform parser
struct SStateM StateMPF[25] = {
				// state 0
				  { 2, 0, 0, 1,            // Type, min, max
					 {"","","","",""},    // names
					 {1,0,0,0,0},NULL     // next states
				  },
				// state 1
				  { 12, 0, 0, 1,                    // Type, min, max
					 {"STA","TRA","","",""},    // names
					 {2,5,0,0,0},NULL             // next states
				  },
				// state 2
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {3,0,0,0,0},NULL               // next states
				  },
				// state 3
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {4,0,0,0,0},NULL               // next states
				  },
				// state 4
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {7,0,0,0,0},NULL               // next states
				  },
				// state 5
				  { 3, 0, 0,1,             // Type, min, max
					 {"","","","",""},    // names
					 {6,0,0,0,0},NULL     // next states
				  },
				// state 6
				  { 12, 0, 0,1,             // Type, min, max
					 {"NOT","ALI","","",""},    // names
					 {7,7,0,0,0},NULL     // next states
				  },
//			// state 7
				  { 12, 0, 0, 1,                    // Type, min, max
					 {"CON","OTH","","",""},    // names
					 {8,11,0,0,0},NULL             // next states
				  },
				// state 8
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {9,0,0,0,0},NULL               // next states
				  },
				// state 9
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {10,0,0,0,0},NULL               // next states
				  },
				// state 10
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {12,0,0,0,0},NULL              // next states
				  },
//			// state 11
				  { 3, 0, 0,1,             // Type, min, max
					 {"","","","",""},    // names
					 {12,0,0,0,0},NULL     // next states
				  },
//ins		// state 12
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {13,0,0,0,0},NULL               // next states
				  },
				// state 13
          { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {14,0,0,0,0},NULL               // next states
				  },
				// state 14
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {15,0,0,0,0},NULL              // next states
				  },
//endins// state 15
				  { 12, 0, 0,1,                    // Type, min, max
					 {"STD","OTH","","",""},    // names
					 {16,19,0,0,0},NULL           // next states
				  },
                // state 16
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {17,0,0,0,0},NULL               // next states
				  },
				// state 17
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {18,0,0,0,0},NULL               // next states
				  },
				// state 18
				  { 1, MIN_DOUBLE, MAX_DOUBLE,1,     // Type, min, max
					 {"","","","",""},              // names
					 {20,0,0,0,0},NULL               // next states
				  },
				// state 19
				  { 3, 0, 0,1,             // Type, min, max
					 {"","","","",""},    // names
					 {208,0,0,0,0},NULL     // next states
				  },
                // state 20
				  { 12, 0, 0,1,                    // Type, min, max
					 {"STD","OTH","","",""},    // names
					 {21,24,0,0,0},NULL           // next states
				  },
				// state 21
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {22,0,0,0,0},NULL               // next states
				  },
				// state 22
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {23,0,0,0,0},NULL               // next states
				  },
				// state 23
				  { 1, MIN_DOUBLE, MAX_DOUBLE,DegToRad,     // Type, min, max
					 {"","","","",""},              // names
					 {-1,0,0,0,0},NULL               // next states
				  },
				// state 24
				  { 3, 0, 0,1,             // Type, min, max
					 {"","","","",""},    // names
					 {-1,0,0,0,0},NULL     // next states
				  }
	 };


//-------------------------------------------------------------------------//
// Function ParseOneItem                                                   //
//                                                                         //
// Read one item of specified file.                                        //
//-------------------------------------------------------------------------//
int ParseOneItem(FILE *ScriptFile, char *DataString, int LineCount)
{
  char CurrentChar;
  int  DataStringIndex = 0;
  char ReturnCode      = _ITEM_NONE,
		 ExitLoop        = FALSE;

  do
	 {
		CurrentChar = (char)getc(ScriptFile);
		switch (CurrentChar)
		  {
			 // ! character indicates a comment, so ignore entire line
			 // until end-of-line or end-of-file is reached
			 case '!' :
				do
				  CurrentChar = (char) getc( ScriptFile );
				while ( (CurrentChar!='\n') && (CurrentChar!=EOF) );
				ungetc( CurrentChar, ScriptFile );
				break;
			 // Ignore spaces, tabs, and commas
			 case ' ' :
       case 13 :  // carriage return character
			 case '\t':
			 case ',' : break;
			 // End-Of-Line, End-Of-File or function call found
			 case '\n': ReturnCode    = _ITEM_EOL;
				DataString[0] = '\0';
				ExitLoop      = TRUE;
				break;
			 case EOF : ReturnCode    = _ITEM_EOF;
				DataString[0] = '\0';
				ExitLoop      = TRUE;
				break;
			 case '$' : ReturnCode    = _ITEM_FUNCTION;
				break;
			 // Actual data found, so read it into DataString and
			 // return to calling function
			 default  : if ( ReturnCode == _ITEM_NONE )
				 ReturnCode = _ITEM_PARAMETER;

			  do
				 {
					DataString[DataStringIndex++] = CurrentChar;
					CurrentChar = (char)getc(ScriptFile);
					if ( DataStringIndex >= MAX_LENGTH_OF_PARA_STRING - 1 )
					  {
						 ErrorM(ERROR_TOO_LONG_PARM, LineCount,
						(long)(MAX_LENGTH_OF_PARA_STRING-1), NULL );
						 break;
					  }
				 }
			  while ( CurrentChar!=EOF && CurrentChar!='\n' &&
				  CurrentChar!=' ' && CurrentChar!='\t' &&
				  CurrentChar!=',' );

			  ungetc( CurrentChar, ScriptFile );
			  DataString[DataStringIndex] = '\0';
			  //ConvertToUpperCase(DataString);
			  //UpperCase(DataString);
			  ExitLoop = TRUE;
		  }  // end switch
	 }  // end do

  while ( ExitLoop == FALSE );

  return ReturnCode;
}


//-------------------------------------------------------------------------//
// Function ErrorM                                                         //
//                                                                         //
// Print error message                                                     //
//-------------------------------------------------------------------------//
void ErrorM(const unsigned int Error,int LineCount, long Multi, char* String)
{
  char s[256];

  switch(Error) {
  case ERROR_UNKNOWN_FUNC :
	 sprintf(s,"Unknown function '%s' encountered in line %d !",String, LineCount);
	 break;
  case ERROR_TOO_MANY_PARM :
	 sprintf(s,"Too many parameters encountered encountered in line %d (limit = %ld)!", LineCount, Multi );
	 break;
  case ERROR_TOO_LONG_PARM :
	 sprintf(s,"Parameter to long in line %d, limit = %ld characters !", LineCount, Multi);
	 break;
  case ERROR_ILLEGAL_NUMBER_PARA_TARGET :
	 sprintf(s,"Function 'TARGET' in line %d accepts only 5,6 or 9 parameters !", LineCount);
	 break;
  case ERROR_ILLEGAL_NUMBER_PARA_ARRAY :
	 sprintf(s,"%ld items expected in function 'ARRAY' in line %d !",Multi, LineCount);
	 break;
  case ERROR_TOO_MANY_NUMBER_PARA_ARRAY :
	 sprintf(s,"Too many items for function 'ARRAY' in line %d !", LineCount);
	 break;
  case ERROR_ILLEGAL_NUMBER_PARA_RADAR :
	 sprintf(s,"Function 'RADAR' accepts only 16 or 21 parameters in line %d !", LineCount);
	 break;
  case ERROR_ILLEGAL_NUMBER_PARA_PLATFORM :
	 sprintf(s,"Function 'PLATFORM' accepts only exactly 6 parameters in line %d !", LineCount);
	 break;
  case ERROR_ARRAY_NOT_FOUND_PLATFORM :
	 sprintf(s,"Cannot find array '%s' in line %d !\nMake sure array is defined before the platform definition.", String, LineCount);
	 break;
  case ERROR_ILLEGAL_NUMBER_PARA_SIM :
	 sprintf(s,"Function 'SIMULATION' accepts only exactly 11 parameters in line %d !", LineCount);
	 break;
  case ERROR_PLATFORM_NOT_FOUND :
	 sprintf(s,"Platform '%s' not defined in line %d.\nMake sure platform is defined before it is referenced to.", String,LineCount);
	 break;
  case ERROR_RADAR_NOT_FOUND :
	 sprintf(s,"Radar '%s' not defined in line %d.\nMake sure the radar is defined before it is referenced to.", String,LineCount);
	 break;
// new stuff
  case E_NOT_ENOUGH_PARA :
	 sprintf(s,"Not enough parameters given for object defined in line %d.",LineCount);
	 break;
  case E_ILLEGAL_BRANCH :
	 sprintf(s,"Selection name '%s' unknown in line %d.",String,LineCount);
	 break;
  case E_NO_LINE_FILE :
	 sprintf(s,"Either 'INLINE' or 'FILE' expected in line %d.",LineCount);
	 break;
  case E_TOO_MANY_POINTS :
	 sprintf(s,"Only up to %d parameters possible in line %d.",(int)Multi,LineCount);
	 break;
  case E_MORE_POINTS_EXP :
	 sprintf(s,"%d parameters expected in line %d.",(int)Multi,LineCount);
	 break;
  case E_INTERPOLATION_EXP :
	 sprintf(s,"Either CUBIC, MFILTER or LINEAR expected in line %d.",LineCount);
	 break;
  case E_OUT_OF_LIMITS :
	 sprintf(s,"Parameter %d (%s) not within limits for object defined in line %d.",
     (int)Multi,String,LineCount);
	 break;

  default :
	 sprintf(s,"Unknown error encountered !");
  }
#ifdef COMPILE_FOR_WINDOWS32
 Application->MessageBox(s, "Parsing error...", MB_OK);
#else
 printf("Parsing Error : %s\n",s);
#endif


}
//---------------------------------------------------------------------------
bool RunStateMachine(struct SStateM *StateM,
						 char ParameterList[][MAX_LENGTH_OF_PARA_STRING],
						 int ParameterCount, int LineCount)
{
  bool Error = FALSE;
  int CS;      // current state
  int CP;      // current parameter
  int branch;  // current branch
  bool found;  // found branch
  long i,j,k;    // loop variables
  double tempd;
  int tempi;

  CS = 0;
  CP = 0;
  do
	 {
		if (StateM[CS].Type == 0)  // int
		  {
        tempi = atoi(ParameterList[CP++]);
			 *(int *)(StateM[CS].SP) = tempi;
       if ((tempi < StateM[CS].Min) || (tempi > StateM[CS].Max))
         {
     			 Error = TRUE;
					 ErrorM(E_OUT_OF_LIMITS, LineCount+1, CS, ParameterList[CP-1]);
					 break;
         }
 			  CS = StateM[CS].NextState[0];
		  }
		else if (StateM[CS].Type == 1)  // double
		  {
       tempd = atof(ParameterList[CP++])*StateM[CS].MultF;
			 *(double *)(StateM[CS].SP) = tempd;
       if ((tempd < StateM[CS].Min) || (tempd > StateM[CS].Max))
         {
     			 Error = TRUE;
					 ErrorM(E_OUT_OF_LIMITS, LineCount+1, CS, ParameterList[CP-1]);
					 break;
         }
			 CS = StateM[CS].NextState[0];
		  }
		else if (StateM[CS].Type == 2)  // char string
		  {
			 strcpy((char *)(StateM[CS].SP),ParameterList[CP++]);
			 CS = StateM[CS].NextState[0];
		  }
		else if (StateM[CS].Type == 3)  // Datadef
		  {
			 SDataDefinition *DF;
			 DF = &((*(SDataDefinition *)(StateM[CS].SP)));

             for (k=0;(k<DF->DataTypes && (CP < ParameterCount));k++)
               {
                 if (CompStrNI((char *)ParameterList[CP],(char *)"CUB",3) == 0)
				   {
				     DF->IntMethod[k] = CUBIC;
				     CP++;
				   }
                 else if (CompStrNI((char *)ParameterList[CP],(char *)"MFI",3) == 0)
				   {
				     DF->IntMethod[k] = FILTER;
				     CP++;
				   }
                 else if (CompStrNI((char *)ParameterList[CP],(char *)"LIN",3) == 0)
				   {
				     DF->IntMethod[k] = LINEAR;
				     CP++;
				   }
                 else
				   {
				     // error - not CUBIC, MFILTER or LINEAR
				     ErrorM(E_INTERPOLATION_EXP, LineCount+1, 0, (char *)"");
				     Error = TRUE;
                     break;
				   }

               }
             if (Error) break;
			 if (CompStrNI((char *)ParameterList[CP],(char *)"FIL",3) == 0)
				{
				  DF->Source = ID_FILE;
				  CP++;
				  strcpy( DF->FileName, ParameterList[CP]);
				  CS = StateM[CS].NextState[0];
				  CP++;
				  Error = LoadDataFile(DF->FileName,DF); // Load Data File
				}
			 else if (CompStrNI((char *)ParameterList[CP],(char *)"INL",3) == 0)
				{
				  DF->Source = ID_INLINE;
				  CP++;
				  for (j=0;j<DF->DataTypes;j++)
					 {
						DF->NoP[j] = atoi(ParameterList[CP]);
						if (DF->NoP[j] >= MAX_SAMPLES)
						  {
							 Error = TRUE;
							 ErrorM(E_TOO_MANY_POINTS, LineCount+1,MAX_SAMPLES, (char *)"");
							 break;
						  }
						CP++;
						// free old data arrays
						Free_DVector(DF->XAxis[j].DataArray,0);
						Free_DVector(DF->YAxis[j].DataArray,0);
						Free_DMatrix(DF->Coeff[j],0,0);
						// make space for new data
						DF->Coeff[j] = DMatrix(0,DF->NoP[j]-1,0,3);
						DF->XAxis[j].DataArray = DVector(0,DF->NoP[j]-1);
						DF->YAxis[j].DataArray = DVector(0,DF->NoP[j]-1);
						for (i = 0; i<DF->NoP[j];i++)
						  {
							 DF->XAxis[j].DataArray[i] = atof(ParameterList[CP++]);
							 if (CP >= ParameterCount)
								{
								  Error = TRUE;
								  ErrorM(E_MORE_POINTS_EXP, LineCount+1,DF->NoP[j], (char *)"");
								  break;
								}
							 DF->YAxis[j].DataArray[i] = atof(ParameterList[CP++]);
							 if ((CP >= ParameterCount) && (i != (DF->NoP[j]-1)))
								{
								  Error = TRUE;
								  ErrorM(E_MORE_POINTS_EXP, LineCount+1,DF->NoP[j], (char *)"");
								  break;
								}
						  }
						if (Error) break;
						// update interpolation coeffs
						FindPolyCoeff(DF->XAxis[j].DataArray,DF->YAxis[j].DataArray,
						 DF->NoP[j], (DF->Coeff[j]));

					 }
				  CS = StateM[CS].NextState[0];

				}
			 else
				{
				  // error - not in-line or file
				  ErrorM(E_NO_LINE_FILE, LineCount+1, 0, (char *)"");
				  Error = TRUE;
				}
		  }
		else // branch
		  {
			 found = FALSE;
			 for (branch = 0; branch < (StateM[CS].Type - 10); branch++)
				{
				  // check which branch to take
				  if (CompStrNI((char *)StateM[CS].Name[branch],(char *)ParameterList[CP],3) == 0)
					 {
						*(int *)(StateM[CS].SP) = branch;
						CS = StateM[CS].NextState[branch];
						CP++;
						found = TRUE;
						break;
					 }
				}
			 if (!found)
				{
				  ErrorM(E_ILLEGAL_BRANCH, LineCount+1, 0, ParameterList[CP]);
				  Error = TRUE;
				}
		  }
	  } // end while
	while ((CS != -1) && (CP < ParameterCount) && (!Error));

	if ((CS != -1) && (!Error))
	 {
		ErrorM(E_NOT_ENOUGH_PARA, LineCount+1, 0, (char *)"");
		Error = TRUE;
	 }

  return Error;
}

//-------------------------------------------------------------------------//
// Function ExecuteCommand                                                 //
//                                                                         //
// Inteprets commands..                                                    //
//-------------------------------------------------------------------------//
int ExecuteCommand(int LastFunction,
				    char ParameterList[][MAX_LENGTH_OF_PARA_STRING],
				    int ParameterCount, int LineCount, struct LinkedLists *FirstObjectP)
{
  struct SPlatform *tempp;
  struct SRadar *tempr;
  long templ;
  int tempi;
  bool Error = FALSE;
/*
			 geo=MainForm->FirstGeometry;
			 while (geo != NULL)
				{
				  sprintf(str,"$GEOMETRY");
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20s ! Name of radar", geo->RadarName);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Slant range start", geo->SlantStart);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (m) Slant range end", geo->SlantEnd);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (s) Azimuth range start", geo->AzimuthStart);
    	          ThisMemo->Lines->Add(str);
				  sprintf(str,"%-20.6G ! (s) Azimuth range end", geo->AzimuthEnd);
    	          ThisMemo->Lines->Add(str);

				  if (geo->FileType == 0) sprintf(s,"SPACE");
				  else sprintf(s,"COMMA");
				  sprintf(str,"%-20s ! Separation character", s);
    	          ThisMemo->Lines->Add(str);

				  sprintf(str,"%-20d ! Info included in file", geo->ShowVar);
    	          ThisMemo->Lines->Add(str);

				  sprintf(str,"%-20s ! File Name of output file", geo->FileName);
    	          ThisMemo->Lines->Add(str);
		      ThisMemo->Lines->Add("");
		      ThisMemo->Lines->Add("");

*/

  switch (LastFunction) {
	 case GEOMETRY :
		{
		  // make space for the new platform
		  SGeometry NG;
		  // set everything to default first
		  CopyGeometry(&NG,&DefaultGeometryValues);
		  // set state machine pointers
		  StateMG[0].SP = &(NG.RadarName);
		  StateMG[1].SP = &(NG.SlantStart);
		  StateMG[2].SP = &(NG.SlantEnd);
		  StateMG[3].SP = &(NG.AzimuthStart);
		  StateMG[4].SP = &(NG.AzimuthEnd);
		  StateMG[5].SP = &(NG.FileType);
		  StateMG[6].SP = &(NG.ShowVar);
		  StateMG[7].SP = &(NG.FileName);
		  Error =  RunStateMachine(StateMG, ParameterList,ParameterCount,LineCount);

      // check if platform exists
      if (!Error)
        if (!FindRadar(NG.RadarName,&tempr, &tempi, FirstObjectP->FirstRadar))
        {
          #ifndef COMPILE_FOR_WINDOWS32
            printf("Cannot find radar '%s' for Geometry.\n",NG.RadarName);
          #endif
          Error = true;
        }

		  if (!Error)
			 AddGeometry(&NG, &(FirstObjectP->FirstGeometry));
		  else
			 {
				// free memory allocated by copying structure
			 }
		  break;
		} // end case
	 case SIMULATION :
		{
		  // make space for the new platform
		  SSimulation NS;
		  // set everything to default first
		  CopySimulation(&NS,&DefaultSimulationValues);
		  // set state machine pointers
		  StateMS[0].SP = &(NS.RadarName);
		  StateMS[1].SP = &(NS.SlantStart);
		  StateMS[2].SP = &(NS.SlantEnd);
		  StateMS[3].SP = &(NS.AzimuthStart);
		  StateMS[4].SP = &(NS.AzimuthEnd);
		  StateMS[5].SP = &(NS.A2Dbits);
		  StateMS[6].SP = &(NS.SampleFreq);
		  StateMS[7].SP = &(NS.LSBvalue);
		  StateMS[8].SP = &(NS.FileType);
		  StateMS[9].SP = &(NS.SimTYPE);
		  StateMS[10].SP = &(NS.FileName);
		  StateMS[11].SP = &(NS.PTPos);
		  StateMS[12].SP = &(NS.OverSampleFactor);
		  StateMS[13].SP = &(NS.PWidth);
		  Error =  RunStateMachine(StateMS, ParameterList,ParameterCount,LineCount);

      // check if platform exists
      if (!Error)
        if (!FindRadar(NS.RadarName,&tempr, &tempi, FirstObjectP->FirstRadar))
        {
          #ifndef COMPILE_FOR_WINDOWS32
            printf("Cannot find radar '%s' for simulation.\n",NS.RadarName);
          #endif
          Error = true;
        }

		  if (!Error)
			 AddSimulation(&NS, &(FirstObjectP->FirstSimulation));
		  else
			 {
				// free memory allocated by copying structure
			 }
		  break;
		} // end case
	 case PLATFORM :
		{
		  // make space for the new platform
		  SPlatform NP;
		  // set everything to default first
		  CopyPlatform(&NP,&DefaultPlatformValues);
		  // set state machine pointers
		  StateMPF[0].SP = &(NP.Name);
		  StateMPF[1].SP = &(NP.Stat_Traj);
		  StateMPF[2].SP = &(NP.SPos[0]);
		  StateMPF[3].SP = &(NP.SPos[1]);
		  StateMPF[4].SP = &(NP.SPos[2]);
		  StateMPF[5].SP = &(NP.TrajDef);
		  StateMPF[6].SP = &(NP.Align);
		  StateMPF[7].SP = &(NP.RotType);
		  StateMPF[8].SP = &(NP.Rot[0]);
		  StateMPF[9].SP = &(NP.Rot[1]);
		  StateMPF[10].SP = &(NP.Rot[2]);
		  StateMPF[11].SP = &(NP.RotDef);
		  StateMPF[12].SP = &(NP.RotRate[0]);
		  StateMPF[13].SP = &(NP.RotRate[1]);
		  StateMPF[14].SP = &(NP.RotRate[2]);
		  StateMPF[15].SP = &(NP.MotionErrorPos);
		  StateMPF[16].SP = &(NP.PosDev[0]);
		  StateMPF[17].SP = &(NP.PosDev[1]);
		  StateMPF[18].SP = &(NP.PosDev[2]);
      StateMPF[19].SP = &(NP.PosDevDef);
		  StateMPF[20].SP = &(NP.MotionErrorRot);
		  StateMPF[21].SP = &(NP.RotDev[0]);
		  StateMPF[22].SP = &(NP.RotDev[1]);
		  StateMPF[23].SP = &(NP.RotDev[2]);
		  StateMPF[24].SP = &(NP.RotDevDef);
		  Error =  RunStateMachine(StateMPF, ParameterList,ParameterCount,LineCount);

		  if (!Error)
			 AddPlatform(&NP, &(FirstObjectP->FirstPlatform));
		  else
			 {
				// free memory allocated by copying structure
				FreePlatformDataDef(&NP);
			 }
		  break;
		} // end case
	 case TARGET :
		{
		  // make space for the new target
		  STarget NT;
		  // set everything to default first
		  CopyTarget(&NT,&DefaultTargetValues);
		  // set state machine pointers
		  StateMT[0].SP = &(NT.Name);
		  StateMT[1].SP = &(NT.Pos[0]);
		  StateMT[2].SP = &(NT.Pos[1]);
		  StateMT[3].SP = &(NT.Pos[2]);
		  StateMT[4].SP = &(NT.PosDev[0]);
		  StateMT[5].SP = &(NT.PosDev[1]);
		  StateMT[6].SP = &(NT.PosDev[2]);
		  StateMT[7].SP = &(NT.RCS);
		  StateMT[8].SP = &(NT.RCSdev);
		  StateMT[9].SP = &(NT.ReflecType);
		  StateMT[10].SP = &(NT.DirAzi);
		  StateMT[11].SP = &(NT.DirEle);
		  StateMT[12].SP = &(NT.DirAziDev);
		  StateMT[13].SP = &(NT.DirEleDev);
		  StateMT[14].SP = &(NT.GainType);
		  StateMT[15].SP = &(NT.DataDef);
		  Error =  RunStateMachine(StateMT, ParameterList,ParameterCount,LineCount);

      // check if platform exists
      if (!Error)
        if (!FindPlatform(NT.Name,&tempp, &templ, FirstObjectP->FirstPlatform))
        {
          #ifndef COMPILE_FOR_WINDOWS32
            printf("Cannot find platform '%s' for target.\n",NT.Name);
          #endif
          Error = true;
        }

		  if (!Error)
			 AddTarget(&NT, &(FirstObjectP->FirstTarget));
		  else
			 {
				// free memory allocated by copying structure
				FreeTargetDataDef(&NT);
			 }
		  break;
		} // end case
	 case SURFACE :
		{
		  // make space for the new Surface
		  SSurface NSur;
		  // set everything to default first
		  CopySurface(&NSur,&DefaultSurfaceValues);
		  // set state machine pointers
		  StateMSur[0].SP = &(NSur.Name);
		  StateMSur[1].SP = &(NSur.Tri[0][0]);
		  StateMSur[2].SP = &(NSur.Tri[0][1]);
		  StateMSur[3].SP = &(NSur.Tri[0][2]);
		  StateMSur[4].SP = &(NSur.Tri[1][0]);
		  StateMSur[5].SP = &(NSur.Tri[1][1]);
		  StateMSur[6].SP = &(NSur.Tri[1][2]);
		  StateMSur[7].SP = &(NSur.Tri[2][0]);
		  StateMSur[8].SP = &(NSur.Tri[2][1]);
		  StateMSur[9].SP = &(NSur.Tri[2][2]);
		  Error =  RunStateMachine(StateMSur, ParameterList,ParameterCount,LineCount);

      // check if platform exists
      if (!Error)
        if (!FindPlatform(NSur.Name,&tempp, &templ, FirstObjectP->FirstPlatform))
        {
          #ifndef COMPILE_FOR_WINDOWS32
            printf("Cannot find platform '%s' for surface.\n",NSur.Name);
          #endif
          Error = true;
        }

		  if (!Error)
			 AddSurface(&NSur, &(FirstObjectP->FirstSurface));
		  else
			 {
				// free memory allocated by copying structure
				FreeSurfaceDataDef(&NSur);
			 }
		  break;
		} // end case
	 case RADAR :
		{
		  // make space for the new radar
		  SRadar NR;
		  // set everything to default first
		  CopyRadar(&NR,&DefaultRadarValues);
		  // set state machine pointers
		  StateMR[0].SP = &(NR.RadarName);
		  StateMR[1].SP = &(NR.PlatformName);
		  StateMR[2].SP = &(NR.PulseType);
		  StateMR[3].SP = &(NR.ChirpBW);
		  StateMR[4].SP = &(NR.PulseDef);
		  StateMR[5].SP = &(NR.PulseWidth);
		  StateMR[6].SP = &(NR.Envelope);
		  StateMR[7].SP = &(NR.RiseTime);
		  StateMR[8].SP = &(NR.FallTime);
		  StateMR[9].SP = &(NR.EnvelopeDef);
		  StateMR[10].SP = &(NR.PRFType);
		  StateMR[11].SP = &(NR.PRF);
		  StateMR[12].SP = &(NR.PRFDef);
		  StateMR[13].SP = &(NR.FreqType);
		  StateMR[14].SP = &(NR.SingleFreq);
		  StateMR[15].SP = &(NR.StartFreq);
		  StateMR[16].SP = &(NR.StepSize);
		  StateMR[17].SP = &(NR.FreqSteps);
		  StateMR[18].SP = &(NR.PulsesPerFreq);
		  StateMR[19].SP = &(NR.FreqDef);
		  StateMR[20].SP = &(NR.PowerOutput);
		  StateMR[21].SP = &(NR.Losses);
		  StateMR[22].SP = &(NR.NoiseTemp);
		  StateMR[23].SP = &(NR.AntennaGainTypeT);
		  StateMR[24].SP = &(NR.ElevBeamWidthT);
		  StateMR[25].SP = &(NR.AziBeamWidthT);
		  StateMR[26].SP = &(NR.AntennaTDef);
		  StateMR[27].SP = &(NR.AntennaGainTypeR);
		  StateMR[28].SP = &(NR.ElevBeamWidthR);
		  StateMR[29].SP = &(NR.AziBeamWidthR);
		  StateMR[30].SP = &(NR.AntennaRDef);
		  StateMR[31].SP = &(NR.BeamDirection);
		  StateMR[32].SP = &(NR.ElevationAngleF);
		  StateMR[33].SP = &(NR.AzimuthAngleF);
		  StateMR[34].SP = &(NR.ElevationAngleR);
		  StateMR[35].SP = &(NR.RotationRate);
		  StateMR[36].SP = &(NR.Spot[0]);
		  StateMR[37].SP = &(NR.Spot[1]);
		  StateMR[38].SP = &(NR.Spot[2]);
		  StateMR[39].SP = &(NR.MFDef);
		  StateMR[40].SP = &(NR.MatchedFilterW);
		  StateMR[41].SP = &(NR.HanningC);
		  StateMR[42].SP = &(NR.MFDef);
		  StateMR[43].SP = &(NR.ApplyAGC);
		  StateMR[44].SP = &(NR.AGCType);
		  StateMR[45].SP = &(NR.AGCDef);
		  StateMR[46].SP = &(NR.BarkerCode);
		  Error =  RunStateMachine(StateMR, ParameterList,ParameterCount,LineCount);

      // check if platform exists
      if (!Error)
        if (!FindPlatform(NR.PlatformName,&tempp, &templ, FirstObjectP->FirstPlatform))
        {
          #ifndef COMPILE_FOR_WINDOWS32
            printf("Cannot find platform '%s' for radar.\n",NR.PlatformName);
          #endif
          Error = true;
        }

		  if (!Error)
			 {
				NR.Losses = dBToFac(NR.Losses);
				AddRadar(&NR, &(FirstObjectP->FirstRadar));
			 }
		  else
			 {
				// free memory allocated by copying structure
				FreeRadarDataDef(&NR);
			 }
		  break;
		} // end case
  } // end switch
  if (Error)
  #ifdef COMPILE_FOR_WINDOWS32
  	Application->MessageBox("Object ignored", "Info", MB_OK);
  #else
    printf(" => Object ignored.\n");
  #endif

  return 0;
}




