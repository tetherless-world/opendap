#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/opendap/bin/install-opendap.sh>;
#3>    prov:wasDerivedFrom   <https://github.com/tetherless-world/opendap/issues/7>;
#3> .
#
# This script will install OPeNDAP from the current working directory.
#
# When run from:
#    /opt/opendap
#
# running:
#    install-opendap.sh ~/path/to/conf/hyrax-1.8_1.conf
#
# will create structure:
#    /opt/opendap/hyrax-1.8_1/hyrax-1.8_1.conf -(soft link)-> /full/path/to/conf/hyrax-1.8_1.conf
#    /opt/opendap/hyrax-1.8_1/bes-3.11.0/
#    /opt/opendap/hyrax-1.8_1/hdf5_handler-2.1.1
#    /opt/opendap/hyrax-1.8_1/libdap-3.11.7
#    ...

if [[ $# -lt 1 || "$1" == '--help' ]]; then
   echo "Usage:"
   echo "   `basename $0` <path/to/module-list.conf>"
   echo
   echo " <path/to/module-list.conf> - a file listing the <module>/<version> to install."
   echo
   echo "see https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Installer#shell-based-installer"
   exit
fi

configuration="$1"
# e.g. https://github.com/tetherless-world/opendap/blob/master/opendap/conf/hyrax-1.8_1.conf

base="https://scm.opendap.org/svn/tags"

if [[ -e "$configuration" ]]; then
   configuration_absolute=`readlink -e $configuration`
   config_local="`basename $configuration`" # e.g. /path/to/hyrax-1.8_1.conf
   config_id="${config_local%.*}"           # e.g.          hyrax-1.8_1.conf -> hyrax-1.8_1
   mkdir -p "$config_id"
   ln -s "$configuration_absolute" "$config_id/$config_local"

   #
   # OPeNDAP Modules are listed at:
   # https://docs.google.com/spreadsheet/ccc?key=0An84UEjofnaydFRrUF9YWk03Y3NHNjJqUEg0NUhUZXc#gid=0
   #
   for module in `cat $configuration | grep -v '^#'`; do
      #
      # e.g. module = 'libdap/3.11.7'
      #
      component=`echo $module | cut -d'/' -f1`
      version=`echo $module | cut -d'/' -f2`
      url="${base}/${component}/${version}"
      echo svn co $url "$config_id/$component-$version"
           svn co $url "$config_id/$component-$version"
      if [[ "$component" == 'libdap' ]]; then
            echo hi
            # TODO: check which and offer to apt-get if not exists.
            # More coplete list of dependencies and how to resolve at:
            # https://hackpad.com/OPeNDAPPROV-2014-Jan-13-pw36WNo46OU
            # automake
            # autoconf
            # libtool
            # m4
            # g++
            # flex
            # bison
            # doxygen
            # libcurl
         if [[ ! `which curl-config` ]]; then
            echo "TODO: install libcurl"
         else
            echo "$component needs libcurl; it is installed at `which curl-config`"
         fi
		 if [[ -e /usr/include/uuid/uuid.h ]]; then
			echo "TODO: install uuid-dev"
		 else
		    echo "$component needs uuid.h; it is installed at /usr/include/uuid"
		 fi
      elif [[ "$component" == 'bes' ]]; then
         echo we need ...
      elif [[ "$component" == 'dap-server' ]]; then
         echo we need ...
      fi
   done
fi
