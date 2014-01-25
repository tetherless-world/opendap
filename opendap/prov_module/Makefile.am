
# Automake file for prov-handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libprov_module.la

Prov_SRCS:=

Prov_HDRS:=

BES_SRCS = \
		ProvModule.cc ProvRequestHandler.cc ProvReporter.cc

BES_HDRS = \
		ProvModule.h ProvRequestHandler.h ProvReporter.h

libprov_module_la_SOURCES = $(Prov_SRCS) $(BES_SRCS) $(Prov_HDRS) $(BES_HDRS)
libprov_module_la_CPPFLAGS = $(BES_CPPFLAGS)
libprov_module_la_LDFLAGS = -avoid-version -module 
libprov_module_la_LIBADD = $(BES_LIBS)

EXTRA_DIST = COPYRIGHT COPYING prov.conf.in

CLEANFILES = *~ prov.conf

install-data-local: prov.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/prov.conf && mv -f $(DESTDIR)$(sysconfdir)/bes/modules/prov.conf $(DESTDIR)$(sysconfdir)/bes/modules/prov.conf.$$currdate; \
	$(INSTALL_DATA) prov.conf $(DESTDIR)$(sysconfdir)/bes/modules/prov.conf

uninstall-local:
	rm -f $(DESTDIR)$(sysconfdir)/bes/modules/prov.conf

prov.conf: prov.conf.in config.status
	sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > prov.conf

