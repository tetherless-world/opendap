
dnl -*- autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(dap-server, 4.1.3, opendap-tech@opendap.org)
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])

AM_INIT_AUTOMAKE(1.10)

# The handler is a shared object library, so we don't normally need
# the static lib. Override this default by specifying `--enable-static',
# which can make running unit tests with valgrind far more pleasant. 
AM_DISABLE_STATIC

AC_CONFIG_SRCDIR([usage.cc])
AM_CONFIG_HEADER([config.h])

AC_DEFINE([DAP_PROTOCOL_VERSION], ["3.2"], [What DAP version is supported?])
AC_SUBST([DAP_PROTOCOL_VERSION])


PACKAGE_MAJOR_VERSION=`echo $PACKAGE_VERSION | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\1@'`
PACKAGE_MINOR_VERSION=`echo $PACKAGE_VERSION | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\2@'`
PACKAGE_SUBMINOR_VERSION=`echo $PACKAGE_VERSION | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\3@'`
AC_MSG_NOTICE(Package Major Version:     $PACKAGE_MAJOR_VERSION)
AC_MSG_NOTICE(Package Minor Version:     $PACKAGE_MINOR_VERSION)
AC_MSG_NOTICE(Package SubMinor Version:  $PACKAGE_SUBMINOR_VERSION)
AC_SUBST(PACKAGE_MAJOR_VERSION)
AC_SUBST(PACKAGE_MINOR_VERSION)
AC_SUBST(PACKAGE_SUBMINOR_VERSION)


dnl flags for the compilers and linkers - set these before locating the
dnl actual tools since some of the AC_PROG macros set these `flag variables'
dnl to default values otherwise.

AC_CANONICAL_HOST
AC_SUBST(host)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

dnl Perl is  used by www-interface during the build
dnl DODS_PROG_PERL
AC_CHECK_PROG([PERL], [perl], [`which perl`])

dods_HTML_Filter=no
if $PERL -e "use HTML::Filter;" > /dev/null 2>&1; then 
   dods_HTML_Filter=yes
else 
   dods_HTML_Filter=no
fi

if test "z$dods_HTML_Filter" = "zno"; then
   AC_MSG_WARN([HTML::Filter not found. See INSTALL])
fi  

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

AC_ARG_WITH([modulesdir],
   [AS_HELP_STRING([--with-modulesdir],[perl modules dir (default ${datadir}/dap-server)])],
   [
        dap_modulesdir="$withval"
   ],
   [
        dap_modulesdir=${datadir}/dap-server
   ]
)

AC_SUBST([dap_modulesdir])

AC_ARG_WITH([cgidir],
   [AS_HELP_STRING([--with-cgidir],[CGI scripts dir (default ${datadir}/dap-server-cgi)])],
   [
        dap_cgidir="$withval"
   ],
   [
        dap_cgidir=${datadir}/dap-server-cgi
   ]
)

AC_SUBST([dap_cgidir])

AC_ARG_WITH([cgiconfdir],
   [AS_HELP_STRING([--with-cgiconfdir],[CGI config dir (default ${dap_cgidir})])],
   [
        dap_cgiconfdir="$withval"
   ],
   [
        dap_cgiconfdir=${dap_cgidir}
   ]
)

AC_SUBST([dap_cgiconfdir])


# Checks for library functions.
AC_CHECK_FUNCS([putenv strtol])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.11.0],
 [
  LIBS="$LIBS $DAP_CLIENT_LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])

AC_CHECK_BES([3.9.0],
[
],
[ AC_MSG_ERROR([Cannot find bes])
])

AM_PATH_CPPUNIT(1.12.0,
	[AM_CONDITIONAL([CPPUNIT], [true])],
	[AM_CONDITIONAL([CPPUNIT], [false])])

dnl I wrote these checks because we need the *-config scripts to build, so 
dnl the AC_CHECK_LIB macro is not needed.

AC_PATH_PROG([CURL], [curl], [*** Where is curl? ***], [/bin:/usr/bin])

OPENDAP_DEBUG_OPTION

dnl autoheader macros; tack some text at the top and bottom of config.h.in
AH_TOP([#ifndef _config_h
#define _config_h])

AH_BOTTOM([/* Shorthand for gcc's unused attribute feature */
#if defined(__GNUG__) || defined(__GNUC__)
#define not_used __attribute__ ((unused))
#else
#define not_used 
#endif /* __GNUG__ || __GNUC__ */

#endif /* _config_h */])

AC_CONFIG_FILES([Makefile
                 asciival/Makefile
		 asciival/unit-tests/Makefile
		 asciival/unit-tests/test_config.h
                 www-interface/Makefile])
                 
AC_CONFIG_FILES([www-interface/js2h.pl], [chmod +x www-interface/js2h.pl])

AC_OUTPUT
