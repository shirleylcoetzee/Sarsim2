//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("mainview.cpp", MainViewForm);
USERES("viewer.res");
USEFORM("loaddata.cpp", FormLoadSimFile);
USEUNIT("general.cpp");
USEUNIT("vector.cpp");
USEUNIT("quickload.cpp");
USEUNIT("quikgeneral.cpp");
USEFORM("selectpal.cpp", LoadPalForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
     Application->Initialize();
     Application->CreateForm(__classid(TMainViewForm), &MainViewForm);
     Application->CreateForm(__classid(TFormLoadSimFile), &FormLoadSimFile);
     Application->CreateForm(__classid(TLoadPalForm), &LoadPalForm);
     Application->Run();
  }
  catch (Exception &exception)
  {
     Application->ShowException(&exception);
  }
  return 0;
}
//---------------------------------------------------------------------------
