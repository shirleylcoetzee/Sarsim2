#include "dest.h"

//---------------------------------------------------------------------------
#ifndef ObjectStructDefH
#define ObjectStructDefH

#include <values.h>

#ifndef COMPILE_FOR_WINDOWS32
  #define MAXPATH 255   // longest path
#endif

#define PI 3.141592653589793238462643383279

#ifdef COMPILE_FOR_WINDOWS32
  #include <dir.h>
#endif

#define FALSE 0
#define TRUE 1

// maximum number of variables per graph i.e. position (XYZ) vs time = 3
#define MAX_VARIABLES_PER_GRAPH 3
// is function defined in a file or locally ?
#define ID_FILE 0
#define ID_INLINE 1
// function interpolation method
#define CUBIC 0
#define FILTER 1
#define LINEAR 2
// units for graphs
#define NONE 0
#define METERS 1
#define SECONDS 2
#define DEGREES 3
#define VOLT 4
#define HERTZ 5
#define DB 6
#define GHERTZ 7
// precision format string (used to print most doubles in dialogs)
#define FORMAT_1  "%-.13G"
#define FORMAT_2 "%d"
// for readability
#define ISOTROPIC 0
#define DIRECTIONAL 1
#define GAINCOS 0
#define GAINOTHER 1
// platform para
#define STATIONARY 0
#define TRAJECTORY 1
// graph type
#define CONT 0
#define INTEGER 1
// pulse type
#define MONO 0
#define CHIRP 1
#define OTHER_PULSE 2
#define BARKER 3
// envelope type
#define RECT_ENV 0
#define LINEAR_ENV 1
#define OTHER_ENV 2
// PRF type
#define CONSTANT_PRF 0
#define OTHER_PRF 1
// carrier freq
#define SINGLE_F 0
#define STEPPED_F 1
#define OTHER_F 2
// antenna
#define TRANS_ISO 0
#define TRANS_SIN 1
#define TRANS_OTHER 2
#define REC_ISO 0
#define REC_SIN 1
#define REC_SAME 2
#define REC_OTHER 3
// direction
#define DIR_FIX 0
#define DIR_CONSTROT 1
#define DIR_SPOT 2
#define DIR_OTHER 3
// matched filter
#define MF_RECT 0
#define MF_HANNING 1
#define MF_BARTLETT 2
#define MF_HAMMING 3
#define MF_OTHER 4
// STC
#define STC_R4 0
#define STC_OTHER 1
// maximum name length of platforms, radars
#define MAX_NAME_LENGTH 16
// maximum number of samples for a userdefined function
#define MAX_SAMPLES 10000
// Maximum length of each parameter
#define MAX_LENGTH_OF_PARA_STRING 64
// display what ? (struct SSimulation)
#define REAL 0
#define IMAGINARY 1
#define MAGNITUDE 2
#define PHASE 3
#define DB_POWER_MAG 4

#define ASCII 0
#define BINARY 1
#define ASCIIwithBrackets 2

#define RAW_RETURN 0
#define MATCHED_FILTER 1
#define SRP 2

// pulse position
#define PULSE_BEGIN 0
#define PULSE_CENTER 1

#define ROT_FIXED 0
#define ROT_USER 1

#define ENTER_KEY 13
#define TAB_KEY 9
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

// various constants
#define DegToRad 0.017453292519943
#define RadToDeg 57.29577951308232
#define NANO double(1E-9)
#define GIGA double(1E9)
#define MICRO double(1E-6)
#define MEGA double(1E6)
#define MILLI double(1E-3)
#define KILO double(1E3)
#define MIN_FLOAT (-MAXFLOAT)
#define MAX_FLOAT (MAXFLOAT)
#define MIN_DOUBLE (-MAXFLOAT)
#define MAX_DOUBLE (MAXFLOAT)
#define PIOver2 (PI*0.5)
#define TwoOverPI (0.636619772)
#define TwoPi (PI*2)
#define LIGHT_SPEED 299792500
#define TwoOverLight (6.671280969E-9)
#define BOLTZMANN_CONSTANT (1.38E-23)
// output formats
#define FORMAT_5  "%-.4f"
#define FORMAT_6  "%-.6f"

// for defining graphs - how should graph be drawn
#define LINE_ONLY 0
#define LINE_MARKER 1
#define MARKER_ONLY 2
#define BAR 3

// Maximum number of parameters in script file
#define MAX_PARMS        4096

// Script functions
#define TARGET 0
#define PLATFORM 1
#define RADAR 2
#define SIMULATION 3
#define SURFACE 4
#define GEOMETRY 5
#define MAX_FUNCTIONS     6

#define TEMP_SCRIPT "__temp__.scr"  // temporary script file
#define TEMP_GEO "__temp__.geo" // temporary geometry file

// pixels between 2 lines in simulation graph
#define PixStep 20
// dynamic range if dB are shown
#define DYNAMIC_RANGE 200       // (in dB)

#define TINY_NUM 1E-30

// used for conversion of surfaces into PTs
#define USE_SURFP_ARRAY 0
#define USE_TRI_ARRAY   1

// variables used for each coordinate
struct SAxis
  {
	 char   Name[25];                 // Name of variable e.g. X - coordinate
	 double MultFactor;               // multplication factor e.g. ns = 1e-9
	 int    Unit;                     // Unit e.g. m
	 int    DecPlaces;                // decimal places e.g. 2
	 double *DataArray;               // actual samples
	 double Start, End;               // default range
	 double Min,Max;                  // maximum range
  };

// structure for complete graph
struct SDataDefinition
  {
	 bool   Source;                   // 0 = file, 1 = inline
     int    IntMethod[MAX_VARIABLES_PER_GRAPH];  // interpolation method e.g. cubic
	 char   FileName[MAXPATH];        // name of file (if Source = file)
	 int    DataTypes;                // number of variables used (1-3)
									  // e.g. timeX+X+timeY+Y+timeZ+Z = 3
	 int    Type[MAX_VARIABLES_PER_GRAPH];  // type of graph e.g. Bar, continuous
										   	// 0 = continuous
										   	// 1 = integers, bar
	 long   NoP[MAX_VARIABLES_PER_GRAPH];        // Number of samples defined
	 long   CurrentP[MAX_VARIABLES_PER_GRAPH];   // Selected sample
	 double **Coeff[MAX_VARIABLES_PER_GRAPH];    // coefficients for interpolation
	 struct SAxis  XAxis[MAX_VARIABLES_PER_GRAPH];      // X data for each DataType
	 struct SAxis  YAxis[MAX_VARIABLES_PER_GRAPH];      // Y data for each DataType
  };

// Create a point target structure
struct STarget
  {
	 char   Name[MAX_NAME_LENGTH];      // Name of moving platform structure
	 double Pos[3];                     // Position (m)
	 double PosDev[3];                  // Standard deviation of position (m)
	 double RCS,RCSdev;                 // Cross Section and Std. Deviation
	 int    ReflecType;                 // 0 = Isotropic, 1 = Directional
	 double DirAzi;                     // Pointing direction azimuth
	 double DirEle;                     // Pointing direction elevation
	 double DirAziDev;                  // Standard deviation of azimuth
	 double DirEleDev;                  // Standard deviation of elevation
	 int    GainType;                   // Gain pattern, 0 = cos, 1 = other
	 SDataDefinition DataDef;           // define 'other' gain pattern
	 struct STarget *next;              // Pointer to next element
	 struct STarget *previous;          // Pointer to previous element
  };

// Create a point Surface structure
struct SSurface
  {
	 char   Name[MAX_NAME_LENGTH];       // Name of moving platform structure
	 double Tri[3][3];                   // Position of all 3 points in m
     int    GlobalUnderSample;           // 0 = Do not use global undersample
                                         // factor, 1-use it
     double USampleFactor;               // 1 = no undersample, e.g. 100
                                         // = calc every 100th sample
     double InPlaneStdDev;               // In-plane jitter (% of sample spacing)
     double OutPlaneStdDev;              // Out-plane jitter (% of sample spacing)
//     int    Pattern;                     // 0 =square, 1 = triangle
     double PTDensity;                   // density between targets
     int    Reflec;                      // 0 = isotropic, 1 = directional
     int    Gain;                        // 0 = cos, 1 = other
	 SDataDefinition GainDataDef;        // define 'other' gain pattern
     int    BothSidesReflect;            // 0 - no, 1 - yes
     int    AssumeConstNormal;           // 0 - no, 1 - yes
     double RCSdev;                      // RCS deviations in percent
     double RCSmult;                     // RCS multiplication factor
     double PosSeed;                     // seed values for position errors
     double RCSSeed;                     // seed values for RCS deviations
	 struct SSurface *next;              // Pointer to next element
	 struct SSurface *previous;          // Pointer to previous element
  };

// Create a moving platform structure
struct SPlatform
  {
	 char   Name[MAX_NAME_LENGTH];      // Name of PLatform
	 int    Stat_Traj;                  // 0 = stationary, 1 = trajectory
	 double SPos[3];                    // Position (m)
	 SDataDefinition TrajDef;           // definition of trajectory
	 double Rot[3];                     // rotation around X,Y and Z axis
   int     RotType;                   // roation type 0 = constant, 1 = userdef
	 SDataDefinition RotDef;            // definition of rotation
	 double RotRate[3];                 // rotation rate around X,Y and Z axis
	 int    Align;                      // 0 = x-axis of platform is
													// aligned to trajectory
	 int    MotionErrorPos;             // 0 = Standard dev pos else userdef
	 int    MotionErrorRot;             // 0 = Standard dev pos else userdef
	 double PosDev[3];                  // Position deviations (m)
	 double RotDev[3];                  // rotation deviations
													// around X,Y and Z axis
	 SDataDefinition PosDevDef;         // definition of motion errors in Pos
	 SDataDefinition RotDevDef;         // definition of motion errors in Rot
	 struct SPlatform *next;            // Pointer to next element
	 struct SPlatform *previous;        // Pointer to previous element
  };

// Create a radar parameter structure
struct SRadar
  {
	 char   PlatformName[MAX_NAME_LENGTH]; // Name of Radar Platform
	 char   RadarName[MAX_NAME_LENGTH]; // Name of Radar itself
	 int    PulseType;                  // Mono = 0, Chirp = 1, Barker = 3,Other = 2
	 SDataDefinition PulseDef;          // definition of 'other' pulse
	 double ChirpBW;                    // Chirp Bandwith (GHz)
   int BarkerCode;                    // which Code 0 - 6
//   int Repetitions;                   // repetitions of code in pulse 
	 double  PulseWidth;                // Total pulse width (ns)
	 int    Envelope;                   // 0 = rect, 1 = linear 2 = other
	 double RiseTime;                   // 0 - max rise time (ns)
	 double FallTime;                   // max - 0 fall time (ns)
	 SDataDefinition EnvelopeDef;       // definition of 'other' envelope
	 int    PRFType;                    // 0 = constant, 1 = other
	 double PRF;                        // PRF (Hz)
	 SDataDefinition PRFDef;            // definition of 'other' PRF
	 int    FreqType;                   // 0 = single, 1 = stepped, 2 = other
	 double SingleFreq;                 // frequency (GHz)
	 double  StartFreq;                 // Start frequency (GHz)
	 double StepSize;                   // Frequency step size (GHz)
	 int FreqSteps;                     // No. of frequency steps
	 int PulsesPerFreq;                 // Pulses / frequency
	 SDataDefinition FreqDef;           // definition of 'other' frequency
	 double PowerOutput;                // Power emitted by antenna (kW)
	 double Losses;                     // All system losses (dB)
	 double NoiseTemp;                  // Noise temperature (K)
	 int AntennaGainTypeT;              // transmitter, 0 = iso, 1 = sinx/x
                                        // 2 = other
	 SDataDefinition AntennaTDef;       // definition of 'other' antenna gain(T)
	 double ElevBeamWidthT;             // Elevation antenna beamwidth (rad)
	 double AziBeamWidthT;              // Azimuth antenna beamwidth (rad)
	 int AntennaGainTypeR;              // receiver, 0 = iso, 1 = sinx/x,
                                        // 2 = same as transmitter, 3 = other
	 SDataDefinition AntennaRDef;       // definition of 'other' antenna gain(R)
	 double ElevBeamWidthR;             // Elevation antenna beamwidth (rad)
	 double AziBeamWidthR;              // Azimuth antenna beamwidth (rad)
	 int BeamDirection;                 // Antenna beam direction
										// (0=fixed,1=rot,2=spot,3=other)
	 SDataDefinition DirectionTDef;     // definition of 'other' direction
	 double ElevationAngleF;            // Elevation angle of fixed beam (rad)
	 double AzimuthAngleF;              // Azimuth angle of fixed beam (rad)
	 double ElevationAngleR;            // Elevation angle of rotating beam (rad)
	 double RotationRate;               // RotationRate of radar (deg./s)
	 double Spot[3];                    // focus point for spot mode
	 int MatchedFilterW;                // 0 = rect, 1=hanning, 2=bartlett etc.
	 double HanningC;                   // Hanning constant
	 SDataDefinition MFDef;             // definition of 'other' matched filter window
	 int ApplyAGC;                      // 0 - no AGC, 1 - apply AGC
	 int AGCType;                       // 0 = 1/R4, 1 = other
	 SDataDefinition AGCDef;            // definition of 'other' AGC
	 struct SRadar *next;               // Pointer to next element
	 struct SRadar *previous;           // Pointer to previous element
//	 double OldBeam[10][3];             // for animations
//	 double BeamL;
  };

// Create a simulation structure
struct SSimulation
  {
	 char   RadarName[MAX_NAME_LENGTH]; // Name of radar
	 char   FileName[MAXPATH];
	 double SlantStart;
	 double SlantEnd;
	 double AzimuthStart;
	 double AzimuthEnd;
	 int Display;                           // what to show (I,Q, abs etc.)
	 int A2Dbits;
	 double SampleFreq;                     // Hz
	 double LSBvalue;                       // V
	 int FileType;                          // 0 = ASCII, 1 = BINARY
	 int SimTYPE;                   // 0 = raw, 1 = MatchedFilter, 2 = SRP etc.
     int PTPos;                     // 0 = begin, 1 = center
     double OverSampleFactor;               // Bandlimited oversampled pulse (1=
                                            // no oversampling, etc.)  
     double PWidth;                         // Actual width used of pulse
	 struct SSimulation *next;              // Pointer to next element
	 struct SSimulation *previous;          // Pointer to previous element
  };

// create a new geometry structure
struct SGeometry
  {
	 char   RadarName[MAX_NAME_LENGTH]; // Name of radar
	 char   FileName[MAXPATH];
	 double SlantStart;
	 double SlantEnd;
	 double AzimuthStart;
	 double AzimuthEnd;
	 int FileType;                          // 0 = space sep, 1 = comma sep
   unsigned int ShowVar;                  // what to include in list
	 struct SGeometry *next;                // Pointer to next element
	 struct SGeometry *previous;            // Pointer to previous element
  };

struct Complex
  {
	 double real;
	 double imag;
  };

// state machine for parsing text files
struct SStateM
  {
	 int Type;          // 0 = int, 1 = double, 2 = char,
							  // 3 = DataDef, >10 = branch (x-10 branches)
	 double Min,Max;    // min and max values (if apply)
	 double MultF;      // multiplication factor e.g stored
							  // in file as deg. but rad in memory
	 char Name[5][4];   // branch name
	 int NextState[5];  // if x = Name[y] then goto NextState[y] etc.
							  // if no branch, nextstate = NextState[0]
	 void *SP;          // pointer where variable will be stored
  };

// contains all the pointers to first elements
struct LinkedLists
  {
    struct SSimulation *FirstSimulation;
    struct SPlatform *FirstPlatform;
    struct STarget *FirstTarget;
    struct SRadar *FirstRadar;
    struct SSurface *FirstSurface;
    struct SGeometry *FirstGeometry;
  };

// structure used for quick loading files (loadfile.cpp)
struct ValueIQMag
  {
    double I;
    double Q;
    double Mag;
  };

#endif

