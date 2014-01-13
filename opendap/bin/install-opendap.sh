#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/opendap/bin/install-opendap.sh>;
#3>    prov:wasDerivedFrom   <https://github.com/tetherless-world/opendap/issues/7>;
#3> .
#
# This script will install OPeNDAP from the current working directory.
#
# When run from:
#    /opt
#
# Running:
#    install-opendap.sh ~/path/to/hyrax-1.8_1.conf
#
# Will create structure:
#    /opt/opendap/hyrax-1.8_1/bes-3.11.0/
#    /opt/opendap/hyrax-1.8_1/hdf5_handler-2.1.1
#    /opt/opendap/hyrax-1.8_1/libdap-3.11.7
#    ...

configuration="$1"
# e.g. https://github.com/tetherless-world/opendap/blob/master/opendap/conf/hyrax-1.8_1.conf

base="https://scm.opendap.org/svn/tags"

# TODO: include directory for `basename $configuration` and check out everything in THAT.
# e.g. into /opt/opendap/hyrax-1.8_1

if [[ -e "$configuration" ]]; then
   configuration_absolute=`readlink -e $configuration`
   config_local="`basename $configuration`" # e.g. /path/to/hyrax-1.8_1.conf
   config_id="${config_local%.*}"           # e.g.          hyrax-1.8_1.conf -> hyrax-1.8_1
   mkdir -p "opendap/$config_id"
   ln -s "$configuration_absolute" "opendap/$config_id/$config_local"

   #
   # OPeNDAP Modules are listed at:
   # https://docs.google.com/spreadsheet/ccc?key=0An84UEjofnaydFRrUF9YWk03Y3NHNjJqUEg0NUhUZXc#gid=0
   #
   for module in `cat $configuration`; do
      #
      # e.g. module = 'libdap/3.11.7'
      #
      component=`echo $module | cut -d'/' -f1`
      version=`echo $module | cut -d'/' -f2`
      url="${base}/${component}/${version}"
      echo svn co $url "opendap/$config_id/$component-$version"
           svn co $url "opendap/$config_id/$component-$version"
      if [[ "$component" == 'libdap' ]]; then
            echo hi
            # TODO: check which and offer to apt-get if not exists.
            # automake
            # autoconf
            # libtool
            # m4
            # g++
            # flex
            # bison
            # doxygen
            # libcurl
         if [[ ! `which libcurl` ]]; then
            echo "TODO: install libcurl"
         else
            echo "$component needs libcurl; it is installed at `which libcurl`"
         fi
      elif [[ "$component" == 'bes' ]]; then
         echo we need ...
      elif [[ "$component" == 'dap-server' ]]; then
         echo we need ...
      fi
   done
fi
