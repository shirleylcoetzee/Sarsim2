//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop

#define COMPILE_FOR_WINDOWS32  // we want to compile it for windows

//---------------------------------------------------------------------------
USEFORM("main.cpp", MainForm);
USERES("sarsim2.res");
USEFORM("targetform.cpp", TargetF);
USEUNIT("linkedlist.cpp");
USEUNIT("general.cpp");
USEUNIT("graph.cpp");
USEFORM("graphform.cpp", GraphForm1);
USEFORM("SimuForm.cpp", SimulationForm);
USEFORM("platformform.cpp", PlatformF);
USEFORM("radarform.cpp", RadarF);
USEUNIT("parser.cpp");
USERES("drag_cursor.RES");
USEUNIT("engine.cpp");
USEFORM("focus.cpp", FocusForm);
USEUNIT("loadsavedata.cpp");
USEUNIT("CalcThread.cpp");
USERC("sarsim2rc.rc");
USEFORM("SaveSimData.cpp", SaveSimForm);
USEFORM("SaveProgress.cpp", SaveProgressForm);
USEUNIT("WaitForThread.cpp");
USEFORM("OldSim.cpp", PreviousSimForm);
USEFORM("about.cpp", AboutForm);
USERC("about.rc");
USEFORM("showscript.cpp", ShowScriptForm);
USEFORM("mainquikview.cpp", MainViewForm);
USEFORM("loadfile.cpp", FormLoadSimFile);
USEUNIT("quickload.cpp");
USEUNIT("quikgeneral.cpp");
USEFORM("selectpal.cpp", LoadPalForm);
USEFORM("surfaceform.cpp", SurfaceF);
USEUNIT("vector.cpp");
USEFORM("import.cpp", ImportForm);
USEFORM("geomform.cpp", GeoForm);
USEFORM("GeoOpt.cpp", GeoOptForm);
USEFORM("logo.cpp", LogoForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "Radar Simulator";
		Application->HelpFile = "SARSIMHLP.HLP";
     Application->CreateForm(__classid(TMainForm), &MainForm);
     Application->CreateForm(__classid(TLogoForm), &LogoForm);
     Application->CreateForm(__classid(TTargetF), &TargetF);
     Application->CreateForm(__classid(TGraphForm1), &GraphForm1);
     Application->CreateForm(__classid(TSimulationForm), &SimulationForm);
     Application->CreateForm(__classid(TPlatformF), &PlatformF);
     Application->CreateForm(__classid(TRadarF), &RadarF);
     Application->CreateForm(__classid(TFocusForm), &FocusForm);
     Application->CreateForm(__classid(TSaveSimForm), &SaveSimForm);
     Application->CreateForm(__classid(TSaveProgressForm), &SaveProgressForm);
     Application->CreateForm(__classid(TPreviousSimForm), &PreviousSimForm);
     Application->CreateForm(__classid(TAboutForm), &AboutForm);
     Application->CreateForm(__classid(TShowScriptForm), &ShowScriptForm);
     Application->CreateForm(__classid(TMainViewForm), &MainViewForm);
     Application->CreateForm(__classid(TFormLoadSimFile), &FormLoadSimFile);
     Application->CreateForm(__classid(TLoadPalForm), &LoadPalForm);
     Application->CreateForm(__classid(TSurfaceF), &SurfaceF);
     Application->CreateForm(__classid(TImportForm), &ImportForm);
     Application->CreateForm(__classid(TGeoForm), &GeoForm);
     Application->CreateForm(__classid(TGeoOptForm), &GeoOptForm);
     Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------
