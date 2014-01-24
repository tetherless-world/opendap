
# Automake file for www-interface
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 27145 2013-09-27 02:09:28Z jimg $

AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = -I$(top_srcdir) $(BES_CPPFLAGS)

# These are not used by automak but are often useful for certain types of
# debugging.
CXXFLAGS_DEBUG = -g3 -O0 -Wall 
TEST_COV_FLAGS = -ftest-coverage -fprofile-arcs

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libwww_module.la

libwww_module_la_SOURCES = $(WWW_SOURCES) $(BES_SOURCES)
libwww_module_la_CPPFLAGS = $(BES_CPPFLAGS)
libwww_module_la_LDFLAGS = -avoid-version -module 
libwww_module_la_LIBADD = $(BES_DAP_LIBS)

BUILT_SOURCES = javascript.h

WWW_SOURCES = WWWArray.cc WWWFloat64.cc WWWInt32.cc WWWSequence.cc	 \
	WWWUInt16.cc WWWByte.cc WWWGrid.cc WWWStr.cc WWWUInt32.cc	 \
	WWWFloat32.cc WWWInt16.cc WWWOutput.cc WWWStructure.cc WWWUrl.cc \
	WWWFloat32.h WWWInt16.h WWWSequence.h WWWUInt16.h WWWArray.h	 \
	WWWFloat64.h WWWInt32.h WWWStr.h WWWUInt32.h WWWByte.h WWWGrid.h \
	WWWOutput.h WWWStructure.h WWWUrl.h javascript.h		 \
	WWWOutputFactory.cc WWWOutputFactory.h get_html_form.cc		 \
	get_html_form.h  

BES_SOURCES = BESWWWModule.cc BESWWWResponseHandler.cc BESWWWTransmit.cc \
	BESXMLWWWGetCommand.cc BESWWWRequestHandler.cc			\
	BESWWW.h BESWWWModule.h BESWWWNames.h BESWWWRequestHandler.h	\
	BESWWWResponseHandler.h BESWWWTransmit.h BESXMLWWWGetCommand.h

EXTRA_DIST = js2h.pl www.js

javascript.h: www.js js2h.pl js2h.pl.in
	./js2h.pl $< > $@

#get_html_form.cc: javascript.h

CLEANFILES = *~ javascript.h

###########################################################################

# Fortify targets.

.PHONY: fortify
fortify: javascript.h
	sourceanalyzer -b @PACKAGE@ $(MAKE) libwww_module.la
	sourceanalyzer -b @PACKAGE@ -scan -f ../@PACKAGE@-@PACKAGE_VERSION@.fpr

# Use this to clean the fortify project.
.PHONY: fortifyclean
fortifyclean:
	sourceanalyzer -b @PACKAGE@ -clean

