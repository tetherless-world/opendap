
dnl -*- autoconf -*-
dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT([ais], [0.2], [dods-tech@unidata.ucar.edu])
AC_CONFIG_AUX_DIR([conf])

AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([nph-ais.in])
AM_CONFIG_HEADER([config.h])

AC_DEFINE([DAP_VERSION], ["2.0"], [What version of the data server is this?])

dnl flags for the compilers and linkers - set these before locating the
dnl actual tools since some of the AC_PROG macros set these `flag variables'
dnl to default values otherwise.

AC_CANONICAL_HOST
AC_SUBST(host)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL
AM_PROG_LEX
AC_PROG_RANLIB
AC_PROG_YACC
AC_PROG_MAKE_SET

DODS_PROG_PERL

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

# Checks for library functions.
AC_CHECK_FUNCS([putenv strtol])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.7.10],
 [
  LIBS="$LIBS $DAP_CLIENT_LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])

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

AC_CONFIG_FILES([Makefile])
                 
AC_CONFIG_FILES([nph-ais], [chmod +x nph-ais; touch stamp-nph-ais])

AC_OUTPUT

