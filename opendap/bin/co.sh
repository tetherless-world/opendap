#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/opendap/bin/co.sh>;
#3>    prov:wasDerivedFrom   <https://github.com/tetherless-world/opendap/issues/7>;
#3> .

configuration="$1"
# e.g. https://github.com/tetherless-world/opendap/blob/master/opendap/conf/hyrax-1.8_1.txt

base="https://scm.opendap.org/svn/tags"

# TODO: include directory for `basename $configuration` and check out everything in THAT.
# e.g. into /opt/opendap/hyrax-1.8_1

if [[ -e "$configuration" ]]; then
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
      echo svn co $url $component-$version
           svn co $url $component-$version
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
