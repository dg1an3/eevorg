# Microsoft Visual C++ generated build script - Do not modify

PROJ = WINORG
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG
R_RCDEFINES = /dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\DEREK\EEVORG\WINORG\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = /YcSTDAFX.H
CUSEPCHFLAG = 
CPPUSEPCHFLAG = /YuSTDAFX.H
FIRSTC =             
FIRSTCPP = STDAFX.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /G2 /W3 /Zi /AL /Od /D "_DEBUG" /FR /GA /Fd"WINORG.PDB" 
CFLAGS_R_WEXE = /nologo /Gs /G2 /W3 /AM /O1 /D "NDEBUG" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE
LIBS_D_WEXE = lafxcwd oldnames libw llibcew commdlg olesvr olecli shell 
LIBS_R_WEXE = mafxcw oldnames libw mlibcew commdlg olesvr olecli shell 
RCFLAGS = /nologo /z
RESFLAGS = /nologo /t
RUNFLAGS = 
DEFFILE = WINORG.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = STDAFX.SBR \
		WINORG.SBR \
		MAINFRM.SBR \
		EEVORG.SBR \
		WINORVW.SBR


WINORG_RCDEP = c:\derek\eevorg\winorg\res\winorg.ico \
	c:\derek\eevorg\winorg\res\eevorg.ico \
	c:\derek\eevorg\winorg\res\winorg.rc2


STDAFX_DEP = c:\derek\eevorg\winorg\stdafx.h


WINORG_DEP = c:\derek\eevorg\winorg\stdafx.h \
	c:\derek\eevorg\winorg\winorg.h \
	c:\derek\eevorg\winorg\mainfrm.h \
	c:\derek\eevorg\winorg\eevorg.h \
	c:\derek\eevorg\winorg\winorvw.h


MAINFRM_DEP = c:\derek\eevorg\winorg\stdafx.h \
	c:\derek\eevorg\winorg\winorg.h \
	c:\derek\eevorg\winorg\mainfrm.h


EEVORG_DEP = c:\derek\eevorg\winorg\stdafx.h \
	c:\derek\eevorg\winorg\winorg.h \
	c:\derek\eevorg\winorg\eevorg.h


WINORVW_DEP = c:\derek\eevorg\winorg\stdafx.h \
	c:\derek\eevorg\winorg\winorg.h \
	c:\derek\eevorg\winorg\eevorg.h \
	c:\derek\eevorg\winorg\winorvw.h


all:	$(PROJ).EXE $(PROJ).BSC

WINORG.RES:	WINORG.RC $(WINORG_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r WINORG.RC

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

WINORG.OBJ:	WINORG.CPP $(WINORG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WINORG.CPP

MAINFRM.OBJ:	MAINFRM.CPP $(MAINFRM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAINFRM.CPP

EEVORG.OBJ:	EEVORG.CPP $(EEVORG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c EEVORG.CPP

WINORVW.OBJ:	WINORVW.CPP $(WINORVW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WINORVW.CPP


$(PROJ).EXE::	WINORG.RES

$(PROJ).EXE::	STDAFX.OBJ WINORG.OBJ MAINFRM.OBJ EEVORG.OBJ WINORVW.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ +
WINORG.OBJ +
MAINFRM.OBJ +
EEVORG.OBJ +
WINORVW.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
c:\msvc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) WINORG.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	WINORG.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) WINORG.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
