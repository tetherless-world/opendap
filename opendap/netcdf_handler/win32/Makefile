
# Setup directories for intermediate files and built components
OBJDIR		= .\objects
SRCDIR		= ..
INSTALLBINDIR	= ..\..\..\bin
INSTALLETCDIR	= ..\..\..\etc
INSTALLLIBDIR	= ..\..\..\lib
INSTALLDLLDIR	= ..\..\..\dll
INSTALLINCDIR	= ..\..\..\include

INCLUDEDIR	= ..\..\..\include

# Setup basic compiler flags for all .obj's
CC		= cl
CFLAGS		= /nologo /MT /W0 /GR /GX /c
#CFLAGS		= /nologo /MTd /Zi /W3 /GR /GX /c	# flags for debugger

# Setup linker flags for libraries and .exe's
LINK		= link
LIBPATH		= /libpath:$(INSTALLLIBDIR)
#LEXEFLAGS	= /DEBUG /subsystem:console $(LIBPATH)	# flags for debugger
#LLIBFLAGS	= -lib					# flags for debugger
LEXEFLAGS	= /nologo /subsystem:console $(LIBPATH)
LLIBFLAGS	= -lib /nologo

DODSLIBS	= libdap++.lib pthreadVCE.lib libxml2.lib libcurl.lib librx.lib libz.lib libxdr.lib
SYSLIBS		= ws2_32.lib advapi32.lib
LINKLIBS	= $(DODSLIBS) $(SYSLIBS)

# Setup compiler flags
STCFLAGS	= /ML				# Single-Threaded Model
MTCFLAGS	= /MT 				# Multi-Threaded Model
MDCFLAGS	= /MD				# Multi-Threaded DLL Model
STDCFLAGS	= /MLd				# Single-Threaded Model, debug version
MTDCFLAGS	= /MTd 				# Multi-Threaded Model, debug version
MDDCFLAGS	= /MDd				# Multi-Thread DLL Model, debug version
CFLAGS		= /nologo /W0 /GR /GX /c

# Setup linker flags for libraries
STLFLAGS	= -lib /nologo -libpath:$(INSTALLLIBDIR)
MTLFLAGS	= -lib /nologo -libpath:$(INSTALLLIBDIR)
MDLFLAGS	= -libpath:$(INSTALLLIBDIR) /dll /def:libnc-dods.def /map /fixed:NO /base:0x07000000
STDLFLAGS	= -lib /nologo -libpath:$(INSTALLLIBDIR)
MTDLFLAGS	= -lib /nologo -libpath:$(INSTALLLIBDIR)
MDDLFLAGS	= -libpath:$(INSTALLLIBDIR) /dll /def:libnc-dods.def /map /debug /fixed:NO /base:0x07000000

# Link libraries for static libraries and dll's
STLIBS		=
MTLIBS		=
MDLIBS		= libdapMD.lib libxml2MD.lib libcurlMD.lib libzMD.lib libxdrMD.lib pthreadVCE.lib
STDLIBS		=
MTDLIBS		=
MDDLIBS		= llibdapMDD.lib ibxml2MDD.lib libcurlMDD.lib libzMDD.lib libxdrMDD.lib pthreadVCE.lib

# Setup linker flags for executables
ESTLFLAGS	= /nologo /libpath:$(INSTALLLIBDIR)
EMTLFLAGS	= /nologo /libpath:$(INSTALLLIBDIR)
EMDLFLAGS	= /nologo /libpath:$(INSTALLLIBDIR)
ESTDLFLAGS	= /nologo /libpath:$(INSTALLLIBDIR)
EMTDLFLAGS	= /nologo /libpath:$(INSTALLLIBDIR)
EMDDLFLAGS	= /nologo /libpath:$(INSTALLLIBDIR)

# Link libraries for exe's
ESTLLIBS	= libdapST.lib libnc-dodsST.lib libxml2ST.lib libcurlST.lib libzST.lib libxdrST.lib
EMTLLIBS	= libdapMT.lib libnc-dodsMT.lib libxml2MT.lib libcurlMT.lib libzMT.lib libxdrMT.lib pthreadVCE.lib
EMDLLIBS	= libdapMD.lib libnc-dodsMD.lib libxml2MD.lib libcurlMD.lib libzMD.lib libxdrMD.lib pthreadVCE.lib
ESTDLLIBS	= libdapSTD.lib libnc-dodsSTD.lib libxml2STD.lib libcurlSTD.lib libzSTD.lib libxdrSTD.lib
EMTDLLIBS	= libdapMTD.lib libnc-dodsMTD.lib libxml2MTD.lib libcurlMTD.lib libzMTD.lib libxdrMTD.lib pthreadVCE.lib
EMDDLLIBS	= libdapMDD.lib libnc-dodsMDD.lib libxml2MDD.lib libcurlMDD.lib libzMDD.lib libxdrMDD.lib pthreadVCE.lib

# Setup compiler includes and defines
INCS		= /I . /I ..\lnetcdf /I ..\fortran /I .. /I $(INCLUDEDIR)\GNU \
		  /I $(INCLUDEDIR) /I $(INCLUDEDIR)\pthreads
DEFS		= /D WIN32 /D WIN32_LEAN_AND_MEAN /D LOCAL
CPPFLAGS	= $(DEFS) $(INCS)

TARGETS		= $(LIBRARY) $(EXECUTABLE)

# Clear built-in rules and set new ones
.SUFFIXES :
.SUFFIXES :	.obj .c .cc

{$(SRCDIR)\lnetcdf}.c{$(OBJDIR)}.obj:
	@$(CC) $(CFLAGS) $(CPPFLAGS) /Fo$@ /Tc $<

{$(SRCDIR)\fortran}.c{$(OBJDIR)}.obj:
	@$(CC) $(CFLAGS) $(CPPFLAGS) /D PowerStationFortran /Fo$@ /Tc $<

{$(SRCDIR)\ncdump}.cc{$(OBJDIR)}.obj:
	@$(CC) $(CFLAGS) $(CPPFLAGS) /Fo$@ /Tp $<

{$(SRCDIR)}.cc{$(OBJDIR)}.obj:
	@$(CC) $(CFLAGS) $(CPPFLAGS) /Fo$@ /Tp $<

NCOBJ = $(OBJDIR)\NCArray.obj \
	$(OBJDIR)\NCByte.obj \
	$(OBJDIR)\NCFloat64.obj \
	$(OBJDIR)\NCGrid.obj \
	$(OBJDIR)\NCInt32.obj \
	$(OBJDIR)\NCList.obj \
	$(OBJDIR)\NCSequence.obj \
	$(OBJDIR)\NCStr.obj \
	$(OBJDIR)\NCStructure.obj \
	$(OBJDIR)\NCUrl.obj \
	$(OBJDIR)\NCUInt32.obj \
	$(OBJDIR)\NCUInt16.obj \
	$(OBJDIR)\NCInt16.obj \
	$(OBJDIR)\NCFloat32.obj

NETOBJ = $(OBJDIR)\NCConnect.obj \
	$(OBJDIR)\inst.obj \
	$(NCOBJ)

LIBOBJS = $(NETOBJ) \
	$(OBJDIR)\Ddim.obj \
	$(OBJDIR)\Dattr.obj \
	$(OBJDIR)\Dvar.obj \
	$(OBJDIR)\Dnc.obj \
	$(OBJDIR)\Dputget.obj \
	$(OBJDIR)\Dncx.obj

NCOBJS = $(OBJDIR)\lattr.obj \
	$(OBJDIR)\ldim.obj \
	$(OBJDIR)\lerror.obj \
	$(OBJDIR)\llibvers.obj \
	$(OBJDIR)\lnc.obj \
	$(OBJDIR)\lncio.obj \
	$(OBJDIR)\lncx.obj \
	$(OBJDIR)\lputget.obj \
	$(OBJDIR)\lstring.obj \
	$(OBJDIR)\lv1hpg.obj \
	$(OBJDIR)\lv2i.obj \
	$(OBJDIR)\lvar.obj

FOBJS = $(OBJDIR)\fort-attio.obj \
	$(OBJDIR)\fort-genvar.obj \
	$(OBJDIR)\fort-varaio.obj \
	$(OBJDIR)\fort-control.obj \
	$(OBJDIR)\fort-lib.obj \
	$(OBJDIR)\fort-vario.obj \
	$(OBJDIR)\fort-dim.obj \
	$(OBJDIR)\fort-misc.obj \
	$(OBJDIR)\fort-varmio.obj \
	$(OBJDIR)\fort-genatt.obj \
	$(OBJDIR)\fort-v2compat.obj \
	$(OBJDIR)\fort-varsio.obj \
	$(OBJDIR)\fort-geninq.obj \
	$(OBJDIR)\fort-var1io.obj

EXEOBJS = $(OBJDIR)\ncdump.obj \
	$(OBJDIR)\vardata.obj \
	$(OBJDIR)\dumplib.obj

# Convenience dependancies
libnc-dodsST.lib: $(OBJDIR)\libnc-dodsST.lib
libnc-dodsMT.lib: $(OBJDIR)\libnc-dodsMT.lib
libnc-dodsMD.dll: $(OBJDIR)\libnc-dodsMD.dll
libnc-dodsSTD.lib: $(OBJDIR)\libnc-dodsSTD.lib
libnc-dodsMTD.lib: $(OBJDIR)\libnc-dodsMTD.lib
libnc-dodsMDD.dll: $(OBJDIR)\libnc-dodsMDD.dll

dncdump: $(OBJDIR)\dncdump.exe
	@echo "built dncdump"

# Link the library
$(OBJDIR)\libnc-dodsST.lib: $(LIBOBJS) $(NCOBJS) $(FOBJS)
	@$(LINK) $(LFLAGS) /out:$@ $(LIBOBJS) $(NCOBJS) $(FOBJS) $(LIBS) 

$(OBJDIR)\libnc-dodsMT.lib: $(LIBOBJS) $(NCOBJS) $(FOBJS)
	@$(LINK) $(LFLAGS) /out:$@ $(LIBOBJS) $(NCOBJS) $(FOBJS) $(LIBS) 

$(OBJDIR)\libnc-dodsMD.dll: $(LIBOBJS) $(NCOBJS) $(FOBJS)
	@$(LINK) $(LFLAGS) /out:$@ $(LIBOBJS) $(NCOBJS) $(FOBJS) $(LIBS)

$(OBJDIR)\libnc-dodsSTD.lib: $(LIBOBJS) $(NCOBJS) $(FOBJS)
	@$(LINK) $(LFLAGS) /out:$@  $(LIBOBJS) $(NCOBJS) $(FOBJS) $(LIBS) 

$(OBJDIR)\libnc-dodsMTD.lib: $(LIBOBJS) $(NCOBJS) $(FOBJS)
	@$(LINK) $(LFLAGS) /out:$@ $(LIBOBJS) $(NCOBJS) $(FOBJS) $(LIBS) 

$(OBJDIR)\libnc-dodsMDD.dll: $(LIBOBJS) $(NCOBJS) $(FOBJS)
	@$(LINK) $(LFLAGS) /out:$@ $(LIBOBJS) $(NCOBJS) $(FOBJS) $(LIBS) 

$(OBJDIR)\dncdump.exe: $(EXEOBJS)
	@$(LINK) $(LFLAGS) /libpath:$(OBJDIR) /out:$@ $(EXEOBJS) $(LIBS)

# Dods Netcdf Objects
$(OBJDIR)\NCArray.obj		: ..\NCArray.cc
$(OBJDIR)\NCByte.obj		: ..\NCByte.cc
$(OBJDIR)\NCFloat64.obj		: ..\NCFloat64.cc
$(OBJDIR)\NCGrid.obj		: ..\NCGrid.cc
$(OBJDIR)\NCInt32.obj		: ..\NCInt32.cc
$(OBJDIR)\NCList.obj		: ..\NCList.cc
$(OBJDIR)\NCSequence.obj	: ..\NCSequence.cc
$(OBJDIR)\NCStr.obj		: ..\NCStr.cc
$(OBJDIR)\NCStructure.obj	: ..\NCStructure.cc
$(OBJDIR)\NCUrl.obj		: ..\NCUrl.cc
$(OBJDIR)\NCUInt32.obj		: ..\NCUInt32.cc
$(OBJDIR)\NCUInt16.obj		: ..\NCUInt16.cc
$(OBJDIR)\NCInt16.obj		: ..\NCInt16.cc
$(OBJDIR)\NCFloat32.obj		: ..\NCFloat32.cc
$(OBJDIR)\NCConnect.obj		: ..\NCConnect.cc
$(OBJDIR)\inst.obj		: ..\inst.cc
$(OBJDIR)\Ddim.obj		: ..\Ddim.cc
$(OBJDIR)\Dattr.obj		: ..\Dattr.cc
$(OBJDIR)\Dvar.obj		: ..\Dvar.cc
$(OBJDIR)\Dnc.obj		: ..\Dnc.cc
$(OBJDIR)\Dputget.obj		: ..\Dputget.cc
$(OBJDIR)\Dncx.obj		: ..\Dncx.cc

# NetCDF code
$(OBJDIR)\lattr.obj		: ..\lnetcdf\lattr.c
$(OBJDIR)\ldim.obj		: ..\lnetcdf\ldim.c
$(OBJDIR)\lerror.obj		: ..\lnetcdf\lerror.c
$(OBJDIR)\llibvers.obj		: ..\lnetcdf\llibvers.c
$(OBJDIR)\lnc.obj		: ..\lnetcdf\lnc.c
$(OBJDIR)\lncio.obj		: ..\lnetcdf\lncio.c
$(OBJDIR)\lncx.obj		: ..\lnetcdf\lncx.c
$(OBJDIR)\lputget.obj		: ..\lnetcdf\lputget.c
$(OBJDIR)\lstring.obj		: ..\lnetcdf\lstring.c
$(OBJDIR)\lv1hpg.obj		: ..\lnetcdf\lv1hpg.c
$(OBJDIR)\lv2i.obj		: ..\lnetcdf\lv2i.c
$(OBJDIR)\lvar.obj		: ..\lnetcdf\lvar.c

# Fortran wrappers
$(OBJDIR)\fort-attio.obj	: ..\fortran\fort-attio.c
$(OBJDIR)\fort-genvar.obj	: ..\fortran\fort-genvar.c
$(OBJDIR)\fort-varaio.obj	: ..\fortran\fort-varaio.c
$(OBJDIR)\fort-control.obj	: ..\fortran\fort-control.c
$(OBJDIR)\fort-lib.obj		: ..\fortran\fort-lib.c
$(OBJDIR)\fort-vario.obj	: ..\fortran\fort-vario.c
$(OBJDIR)\fort-dim.obj		: ..\fortran\fort-dim.c
$(OBJDIR)\fort-misc.obj		: ..\fortran\fort-misc.c
$(OBJDIR)\fort-varmio.obj	: ..\fortran\fort-varmio.c
$(OBJDIR)\fort-genatt.obj	: ..\fortran\fort-genatt.c
$(OBJDIR)\fort-v2compat.obj	: ..\fortran\fort-v2compat.c
$(OBJDIR)\fort-varsio.obj	: ..\fortran\fort-varsio.c
$(OBJDIR)\fort-geninq.obj	: ..\fortran\fort-geninq.c
$(OBJDIR)\fort-var1io.obj	: ..\fortran\fort-var1io.c

# Objects for dncdump
$(OBJDIR)\ncdump.obj		: ..\ncdump\ncdump.cc
$(OBJDIR)\vardata.obj		: ..\ncdump\vardata.cc
$(OBJDIR)\dumplib.obj		: ..\ncdump\dumplib.cc

# Don't use the built-in "extended DOS" commands with all the
# fancy flags.  Win95/98 systems don't have those so we must
# keep it simple.  Sticking with no-flag versions of copy, mkdir,
# rmdir and del are a safe bet.
setup:
	@-mkdir $(OBJDIR)
	@-mkdir $(INSTALLBINDIR)
	@-mkdir $(INSTALLETCDIR)
	@-mkdir $(INSTALLLIBDIR)
	@-mkdir $(INSTALLDLLDIR)
	@-mkdir $(INSTALLINCDIR)

# Install
install: installST installMT installSTD installMTD
#install: installST installMT installMD installSTD installMTD installMDD
# We use the Multi-threaded versions of executables by default here
	@copy $(OBJDIR)\MT\dncdump.exe $(INSTALLBINDIR)
	@copy ..\lnetcdf\netcdf.h $(INSTALLINCDIR)
	@copy ..\fortran\netcdf.inc $(INSTALLINCDIR)

installST:
	@copy $(OBJDIR)\ST\libnc-dodsST.lib $(INSTALLLIBDIR)

installMT:
	@copy $(OBJDIR)\MT\libnc-dodsMT.lib $(INSTALLLIBDIR)

installMD:
	@copy $(OBJDIR)\MD\libnc-dodsMD.lib $(INSTALLLIBDIR)
	@copy $(OBJDIR)\MD\libnc-dodsMD.dll $(INSTALLDLLDIR)

installSTD:
	@copy $(OBJDIR)\STD\libnc-dodsSTD.lib $(INSTALLLIBDIR)

installMTD:
	@copy $(OBJDIR)\MTD\libnc-dodsMTD.lib $(INSTALLLIBDIR)

installMDD:
	@copy $(OBJDIR)\MDD\libnc-dodsMDD.lib $(INSTALLLIBDIR)
	@copy $(OBJDIR)\MDD\libnc-dodsMDD.dll $(INSTALLDLLDIR)
>>>>>>> 1.2.4.9

clean:
	@-del $(OBJDIR)\*.obj
	@-del $(OBJDIR)\*.lib
	@-rmdir $(OBJDIR)

!INCLUDE "dependancies"



