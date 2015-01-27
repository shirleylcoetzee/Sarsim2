# ---------------------------------------------------------------------------
VERSION = BCB.01
# ---------------------------------------------------------------------------
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif
# ---------------------------------------------------------------------------
PROJECT = Sarsim2.exe
OBJFILES = Sarsim2.obj main.obj targetform.obj linkedlist.obj general.obj \
   graph.obj graphform.obj SimuForm.obj platformform.obj radarform.obj parser.obj \
   engine.obj focus.obj loadsavedata.obj CalcThread.obj SaveSimData.obj \
   SaveProgress.obj WaitForThread.obj OldSim.obj about.obj showscript.obj \
   mainquikview.obj loadfile.obj quickload.obj quikgeneral.obj selectpal.obj \
   surfaceform.obj vector.obj import.obj geomform.obj GeoOpt.obj logo.obj
RESFILES = sarsim2.res drag_cursor.RES sarsim2rc.res about.res
RESDEPEN = $(RESFILES) main.dfm targetform.dfm graphform.dfm SimuForm.dfm \
   platformform.dfm radarform.dfm focus.dfm SaveSimData.dfm SaveProgress.dfm \
   OldSim.dfm about.dfm showscript.dfm mainquikview.dfm loadfile.dfm \
   selectpal.dfm surfaceform.dfm import.dfm geomform.dfm GeoOpt.dfm logo.dfm
LIBFILES = 
DEFFILE = 
# ---------------------------------------------------------------------------
CFLAG1 = -Od -Hc -w -k -r- -y -v -vi- -c -a4 -b- -w-par -w-inl -Vx -Ve -x
CFLAG2 = -I.\bitmaps;d:\builder\threads_rolf;$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl \
   -H=$(BCB)\lib\vcld.csm 
PFLAGS = -AWinTypes=Windows;WinProcs=Windows;DbiTypes=BDE;DbiProcs=BDE;DbiErrs=BDE \
   -U.\bitmaps;d:\builder\threads_rolf;$(BCB)\projects;$(BCB)\lib\obj;$(BCB)\lib \
   -I.\bitmaps;d:\builder\threads_rolf;$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl \
   -v -$Y -$W -$O- -JPHNV -M  
RFLAGS = -i.\bitmaps;d:\builder\threads_rolf;$(BCB)\projects;$(BCB)\include;$(BCB)\include\vcl 
LFLAGS = -L.\bitmaps;d:\builder\threads_rolf;$(BCB)\projects;$(BCB)\lib\obj;$(BCB)\lib \
   -aa -Tpe -x -v -V4.0 
IFLAGS = 
LINKER = ilink32
# ---------------------------------------------------------------------------
ALLOBJ = c0w32.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) vcl.lib import32.lib cp32mt.lib 
# ---------------------------------------------------------------------------
.autodepend

$(PROJECT): $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES) 
!

.pas.hpp:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.pas.obj:
    $(BCB)\BIN\dcc32 $(PFLAGS) { $** }

.cpp.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $* 

.c.obj:
    $(BCB)\BIN\bcc32 $(CFLAG1) $(CFLAG2) -o$* $**

.rc.res:
    $(BCB)\BIN\brcc32 $(RFLAGS) $<
#-----------------------------------------------------------------------------
