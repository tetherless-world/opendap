
# Automake file for netcdf-handler
#
# 5/17/2005 jhrg
#
# $Id$

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

SUBDIRS = bes-testsuite

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libnc_module.la

libnc_module_la_SOURCES = $(NCTYPE_SRC) $(NCTYPE_HDR) $(SERVER_SRC) $(SERVER_HDR)
libnc_module_la_CPPFLAGS = $(BES_CPPFLAGS)
libnc_module_la_LDFLAGS = -avoid-version -module 
libnc_module_la_LIBADD = $(BES_DAP_LIBS)

#if 0 // Disabling cruft code from earlier ugrid work - ndp 03/25/2013
#if USING_GRIDFIELDS 
#libnc_module_la_CPPFLAGS += $(GF_CFLAGS)
#libnc_module_la_LIBADD += $(GF_LIBS)
#endif
#endif // Disabling cruft code from earlier ugrid work - ndp 03/25/2013


NCTYPE_SRC = ncdas.cc ncdds.cc nc_util.cc \
	NCArray.cc NCByte.cc NCFloat64.cc NCGrid.cc NCUInt32.cc		\
	NCInt32.cc NCSequence.cc NCStr.cc NCStructure.cc NCUrl.cc	\
	NCUInt16.cc NCInt16.cc NCFloat32.cc

NCTYPE_HDR = NCFloat64.h NCArray.h NCGrid.h NCSequence.h NCUInt16.h 	\
	NCByte.h NCInt16.h NCStr.h NCUInt32.h NCFloat32.h NCInt32.h	\
	NCStructure.h NCUrl.h nc_util.h config_nc.h

SERVER_SRC = NCRequestHandler.cc NCModule.cc

SERVER_HDR = NCRequestHandler.h NCModule.h

#if 0 // Disabling cruft code from earlier ugrid work - ndp 03/25/2013
#if USING_GRIDFIELDS 
#SERVER_SRC += gridfields_functions.cc
#SERVER_HDR += gridfields_functions.h
#endif
#endif // Disabling cruft code from earlier ugrid work - ndp 03/25/2013

EXTRA_DIST = data COPYRIGHT COPYING netcdf_handler.spec \
    nc.conf.in OSX_Resources

CLEANFILES = *~ nc.conf

sample_datadir = $(datadir)/hyrax/data/nc
sample_data_DATA = data/bears.nc data/bears.nc.das data/coads_climatology.nc data/fnoc1.nc data/fnoc1.das data/fnoc1.nc.html

install-data-local: nc.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/nc.conf && mv -f $(DESTDIR)$(sysconfdir)/bes/modules/nc.conf $(DESTDIR)$(sysconfdir)/bes/modules/nc.conf.$$currdate; \
	$(INSTALL_DATA) nc.conf $(DESTDIR)$(sysconfdir)/bes/modules/nc.conf

uninstall-local:
	rm -f $(DESTDIR)$(sysconfdir)/bes/modules/nc.conf

nc.conf: nc.conf.in config.status
	sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%" $< > nc.conf

# Not nearly as clean as it could be, but this removes .svn directories
# in subdirs.
dist-hook:
	rm -rf `find $(distdir) -name .svn`

C4_DIR=./cccc
.PHONY: cccc
cccc:	
	cccc --outdir=$(C4_DIR) $(libnc_module_la_SOURCES)

###########################################################################

# Fortify targets.

.PHONY: fortify
fortify:
	sourceanalyzer -b @PACKAGE@ $(MAKE)
	sourceanalyzer -b @PACKAGE@ -scan -f @PACKAGE@-@PACKAGE_VERSION@.fpr

# Use this to clean the fortify project.
.PHONY: fortifyclean
fortifyclean:
	sourceanalyzer -b @PACKAGE@ -clean

###########################################################################
#
# Build linux RPMs

srpm: dist
	rpmbuild -ts --clean $(RPM_OPTIONS) @PACKAGE@-@PACKAGE_VERSION@.tar.gz

rpm: dist
	rpmbuild -tb --clean $(RPM_OPTIONS) @PACKAGE@-@PACKAGE_VERSION@.tar.gz

###########################################################################

# Build OS/X Packages. The strange operations with configure and make
# are there so that the values built into bes-config will match the mac
# osx install dirs and not the temp directory used to build the packages

PACKAGEMAKER=/Developer/usr/bin/packagemaker
PKG_CONF_FLAGS=

clean-pkg:
	-rm -rf mac_osx @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@

pkg-build: clean-pkg
	./configure --prefix=$(prefix) --with-netcdf=$(NC_PATH) --disable-dependency-tracking $(PKG_CONF_FLAGS)
	make clean all
	DESTDIR=`pwd`/mac_osx make install

pkg-main: pkg-build
	./OSX_Resources/update_mac_package_contents.pl README
	cat ./OSX_Resources/InstallationCheck.proto | sed -e "s^_PREFIX_^$(prefix)^g" > foo
	mv foo ./OSX_Resources/InstallationCheck
	chmod 755 ./OSX_Resources/InstallationCheck
	cat ./OSX_Resources/Info.plist.proto | \
	   sed -e "s^_PREFIX_^$(prefix)^g" \
	       -e "s^_FULL_VERSION_^@PACKAGE_VERSION@^g" \
	       -e "s^_MAJOR_VERSION_^@PACKAGE_MAJOR_VERSION@^g" \
	       -e "s^_MINOR_VERSION_^@PACKAGE_MINOR_VERSION@^g" \
	       > foo
	mv foo ./OSX_Resources/Info.plist
	${PACKAGEMAKER} --root mac_osx --id org.opendap.@PACKAGE@ \
		--title "@PACKAGE@ @PACKAGE_VERSION@" --version @PACKAGE_VERSION@ \
		--out @PACKAGE@-@PACKAGE_VERSION@.pkg --resources OSX_Resources

pkg-dmg: pkg-main
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@.dmg
	mkdir @PACKAGE@-@PACKAGE_VERSION@
	cp -R @PACKAGE@-@PACKAGE_VERSION@.pkg @PACKAGE@-@PACKAGE_VERSION@
	cp README NEWS @PACKAGE@-@PACKAGE_VERSION@
	dropdmg -i --sanitize-for-servers --format zlib @PACKAGE@-@PACKAGE_VERSION@
	-rm -rf @PACKAGE@-@PACKAGE_VERSION@

pkg: pkg-main

