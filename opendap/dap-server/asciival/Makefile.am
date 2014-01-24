
# Automake file for asciival
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 27145 2013-09-27 02:09:28Z jimg $

AUTOMAKE_OPTIONS = foreign
AM_CPPFLAGS = -I$(top_srcdir)

# These are not used by automake but are often useful for certain types of
# debugging.
CXXFLAGS_DEBUG = -g3 -O0 -Wall 
TEST_COV_FLAGS = -ftest-coverage -fprofile-arcs

SUBDIRS = unit-tests

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libascii_module.la

libascii_module_la_SOURCES = $(ASCII_SOURCES) $(BES_SOURCES)
libascii_module_la_CPPFLAGS = $(BES_CPPFLAGS)
libascii_module_la_LDFLAGS = -avoid-version -module 
libascii_module_la_LIBADD = $(BES_DAP_LIBS)

ASCII_SOURCES = AsciiArray.cc AsciiInt16.cc AsciiStructure.cc		   \
		AsciiInt32.cc AsciiUInt16.cc AsciiByte.cc AsciiOutput.cc   \
		AsciiUInt32.cc AsciiFloat32.cc AsciiUrl.cc AsciiFloat64.cc \
		AsciiSequence.cc  AsciiGrid.cc AsciiStr.cc AsciiArray.h	   \
		AsciiInt32.h AsciiUInt32.h AsciiByte.h AsciiOutput.h	   \
		AsciiUrl.h AsciiFloat32.h AsciiSequence.h AsciiFloat64.h   \
		AsciiStr.h AsciiGrid.h AsciiStructure.h AsciiInt16.h	   \
		AsciiUInt16.h AsciiOutputFactory.cc AsciiOutputFactory.h   \
		get_ascii.cc get_ascii.h

BES_SOURCES = BESAsciiModule.cc BESAsciiResponseHandler.cc		   \
	      BESAsciiTransmit.cc BESAsciiRequestHandler.cc		   \
	      BESAsciiModule.h BESAsciiResponseHandler.h		   \
	      BESAsciiTransmit.h BESAsciiRequestHandler.h BESAsciiNames.h

EXTRA_DIST = 

CLEANFILES = *~ *.log *.sum

###########################################################################

# Fortify targets.

.PHONY: fortify
fortify:
	sourceanalyzer -b @PACKAGE@ $(MAKE) libascii_module.la
	sourceanalyzer -b @PACKAGE@ -scan -f ../@PACKAGE@-@PACKAGE_VERSION@.fpr

# Use this to clean the fortify project.
.PHONY: fortifyclean
fortifyclean:
	sourceanalyzer -b @PACKAGE@ -clean

