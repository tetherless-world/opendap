
AUTOMAKE_OPTIONS = foreign 

AM_CPPFLAGS = -I$(top_srcdir)
AM_CXXFLAGS =

# These are not used by automake but are often useful for certain types of
# debugging. The best way to use these is to run configure as:
#     export CXXFLAGS='...'; ./configure --disable-shared
# the --disable-shared is not required, but it seems to help with debuggers.
CXXFLAGS_DEBUG = -g3 -O0  -Wall -W -Wcast-align -Werror
TEST_COV_FLAGS = -ftest-coverage -fprofile-arcs

noinst_DATA = bes.conf

CLEANFILES = *.log *.sum gmon.out

EXTRA_DIST = nc netcdf_handlerTest.at $(TESTSUITE_3) $(TESTSUITE_4) \
        atlocal.in $(srcdir)/package.m4 bes.conf.in handler_tests_macros.m4 \
        nc4_netcdf_handler_tests.at

DISTCLEANFILES = bes.conf

bes.conf: bes.conf.in ../config.status
	@my_topdir=`echo ${abs_top_srcdir} | sed 's/\(.*\)\/\(.[^\/]*\)\/\.\./\1/g'`; \
	sed -e "s%[@]pkgdatadir[@]%${pkgdatadir}%" \
		-e "s%[@]modulesdir[@]%${BES_MODULE_DIR}%" \
		-e "s%[@]sysconfdir[@]%${sysconfdir}%" \
		-e "s%[@]libdir[@]%${libdir}%" \
		-e "s%[@]datadir[@]%${datadir}%" \
		-e "s%[@]abs_top_srcdir[@]%$$my_topdir%" \
		-e "s%[@]abs_top_builddir[@]%${abs_top_builddir}%" \
		-e "s%[@]bindir[@]%${bindir}%" $< > bes.conf

############## Autotest follows #####################

AUTOM4TE = autom4te

TESTSUITE_3 = $(srcdir)/netcdf_handlerTest

if NETCDF4_TESTS
TESTSUITE_4 = $(srcdir)/nc4_netcdf_handler_tests
else
TESTSUITE_4 =
endif

# TESTSUITE_3 is always built, but not TESTSUITE_4

# package.m4 was in the list of deps, but now it's built by configure.
# jhrg 1/11/12

check-local: atconfig atlocal $(TESTSUITE_3) $(TESTSUITE_4)
	$(SHELL) '$(TESTSUITE_3)' $(TESTSUITEFLAGS)
	test ! -f '$(TESTSUITE_4)' || $(SHELL) '$(TESTSUITE_4)' $(TESTSUITEFLAGS)

clean-local:
	test ! -f '$(TESTSUITE_3)' || $(SHELL) '$(TESTSUITE_3)' --clean
	test ! -f '$(TESTSUITE_4)' || $(SHELL) '$(TESTSUITE_4)' --clean

distclean-local:
	-rm atconfig

AUTOTEST = $(AUTOM4TE) --language=autotest

% : %.at
	$(AUTOTEST) -I '$(srcdir)' -o $@ $@.at

# The `:;' works around a Bash 3.2 bug when the output is not writable.
# Replaced with a *.in file and built by configure. jhrg 1/11/12
#package.m4: $(top_srcdir)/configure.ac
#	:;{ \
#	echo '# Signature of the current package.' && \
#	echo 'm4_define([AT_PACKAGE_NAME],      [@PACKAGE_NAME@])' && \
#	echo 'm4_define([AT_PACKAGE_TARNAME],   [@PACKAGE_TARNAME@])' && \
#	echo 'm4_define([AT_PACKAGE_VERSION],   [@PACKAGE_VERSION@])' && \
#	echo 'm4_define([AT_PACKAGE_STRING],    [@PACKAGE_STRING@])' && \
#	echo 'm4_define([AT_PACKAGE_BUGREPORT], [@PACKAGE_BUGREPORT@])'; \
#	} >'$(srcdir)/package.m4'
