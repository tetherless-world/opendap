# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(netcdf_handler, 3.10.3, opendap-tech@opendap.org)
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])

AM_INIT_AUTOMAKE(1.10)

# The handler is a shared object library, so we don't normally need
# the static lib. Override this default by specifying `--enable-static',
# which can make running unit tests with valgrind far more pleasant. 
AM_DISABLE_STATIC

LT_INIT
AC_CONFIG_TESTDIR(bes-testsuite, [.])

PACKAGE_MAJOR_VERSION=`echo $PACKAGE_VERSION | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\1@'`
PACKAGE_MINOR_VERSION=`echo $PACKAGE_VERSION | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\2@'`
PACKAGE_SUBMINOR_VERSION=`echo $PACKAGE_VERSION | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\3@'`
AC_MSG_NOTICE(Package Major Version:     $PACKAGE_MAJOR_VERSION)
AC_MSG_NOTICE(Package Minor Version:     $PACKAGE_MINOR_VERSION)
AC_MSG_NOTICE(Package SubMinor Version:  $PACKAGE_SUBMINOR_VERSION)
AC_SUBST(PACKAGE_MAJOR_VERSION)
AC_SUBST(PACKAGE_MINOR_VERSION)
AC_SUBST(PACKAGE_SUBMINOR_VERSION)

AC_CONFIG_SRCDIR([NCStr.h])
AC_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.11.0],
 [
  LIBS="$LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])

AC_CHECK_NETCDF(
   [
   LIBS="$LIBS $NC_LIBS"
   LDFLAGS="$LDFLAGS $NC_LDFLAGS"
   CPPFLAGS="$CPPFLAGS $NC_CPPFLAGS"
   ],
   [
   echo "*** Use --with-netcdf for the root netcdf directory."
   echo "*** Otherwise use --with-netcdf-include switch for includes directory"
   echo "*** and --with-netcdf-libdir switch for libraries directory."
   AC_MSG_ERROR([netcdf library and netcdf headers are required.])
   ],[3]
)

AC_MSG_CHECKING([Checking the netCDF library version number])

AC_EGREP_HEADER(nc_inq_user_type, netcdf.h, 
        NETCDF_MAJOR_VERSION=4, NETCDF_MAJOR_VERSION=3)
AC_DEFINE_UNQUOTED(NETCDF_VERSION, $NETCDF_MAJOR_VERSION, [What version of netcdf were we built with?])

AC_MSG_RESULT($NETCDF_MAJOR_VERSION)

AM_CONDITIONAL([NETCDF4_TESTS], [test "$NETCDF_MAJOR_VERSION" = "4"])

AC_CHECK_BES([3.9.0],
[
],
[ AC_MSG_ERROR([Cannot find bes])
])


# Disabling cruft code from earlier ugrid work - ndp 03/25/2013
#
#dnl When pkgconfig support is added, use this
#libdap_libgridfields_module='libgridfields >= 0.7.0'
#
#libgf_ok="no"
#
#AC_ARG_WITH([gridfields],
#    AC_HELP_STRING([--with-gridfields=path], [Use the gridfields library at this location.]),
#    [if test "$withval" = "yes"
#     then
#         AC_MSG_ERROR([You must supply a path when using --with-gridfields; try --enable-gridfields.])
#     else
#         GF_LIBS="-L$withval/lib -lgridfields"
#         GF_CFLAGS="-I$withval/include"
#         AC_DEFINE([GRIDFIELDS],[1],[define if gridfields lib is present])
#         libgf_ok="yes"
#     fi],
#    [])
#
#AC_ARG_ENABLE([gridfields],
#    AC_HELP_STRING([--enable-gridfields], [Enable libgridfields-based server functions (default is NO)]))
#
#if test "$libgf_ok" = "no" -a "$enable_gridfields" = "yes"
#then
#     
#AC_MSG_CHECKING([for libgridfields])
#if gridfields-config --version > /dev/null 2>&1
#then
#    version_libgf=`gridfields-config --version | sed 's@libgridfields \(.*\)@\1@'`
#
#    version_M=`echo $version_libgf | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\1@'`
#    version_m=`echo $version_libgf | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\2@'`
#    version_m_m=`echo $version_libgf | sed 's@^\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$@\3@'`
#    
#    dnl Test for several different versions of libgridfields
#    
#    if test $version_M -gt 0
#    then
#        libgf_ok='yes'
#    elif test $version_M -eq 0 && test $version_m -gt 7
#    then
#        libgf_ok='yes'
#    elif test $version_M -eq 0 && test $version_m -eq 7 && test $version_m_m -gt 0
#    then
#        libgf_ok='yes'
#    else
#        libgf_ok='no'
#    fi
#
#    if test "$libgf_ok"="no"
#    then
#        AC_MSG_ERROR([must have libgf 0.7.0 or greater, found $version_libgf])
#    fi
#
#    GF_LIBS="`gridfields-config --libs`"
#    GF_CFLAGS="`gridfields-config --cflags`"
#
#    AC_DEFINE([GRDIFIELDS],[1],[define if gridfields lib is present])
#    AC_MSG_RESULT([yes; used gridfields-config])
#else
#    AC_MSG_ERROR([I could not find libgridfields])
#fi
#
#fi
#
#AC_SUBST([GF_LIBS])
#AC_SUBST([GF_CFLAGS])
#
#if test "$libgf_ok" = "yes"
#then
#    echo "Set gridfields CFLAGS to $GF_CFLAGS"
#    echo "Set gridfields LIBS to $GF_LIBS"
#    AM_CONDITIONAL([USING_GRIDFIELDS],[true])
#else
#    AM_CONDITIONAL([USING_GRIDFIELDS],[false])
#fi

OPENDAP_DEBUG_OPTION

AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([bes-testsuite/Makefile bes-testsuite/atlocal bes-testsuite/package.m4])
AC_CONFIG_FILES([bes-testsuite/generate_metadata_baseline.sh], [chmod +x bes-testsuite/generate_metadata_baseline.sh])
AC_CONFIG_FILES([bes-testsuite/generate_data_baseline.sh], [chmod +x bes-testsuite/generate_data_baseline.sh])

AS_IF([test "$libgf_ok" = "yes"], [AC_CONFIG_FILES([ugrid_project/bes.conf])])

AC_OUTPUT
