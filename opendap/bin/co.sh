#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/opendap/bin/co.sh>;
#3>    prov:wasDerivedFrom   <https://github.com/tetherless-world/opendap/issues/7>;
#3> .

configuration="$1"
# e.g. https://github.com/tetherless-world/opendap/blob/master/opendap/conf/hyrax-1.8_1.txt

base="https://scm.opendap.org/svn/tags"

if [[ -e "$configuration" ]]; then
   #
   # OPeNDAP Modules are listed at:
   # https://docs.google.com/spreadsheet/ccc?key=0An84UEjofnaydFRrUF9YWk03Y3NHNjJqUEg0NUhUZXc#gid=0
   #
   for $module in `cat $configuration`; do
      #
      # e.g. module = 'libdap/3.11.7'
      #
      comp=`echo $module | cut -d'/' -f1`
      vers=`echo $module | cut -d'/' -f2`
      toget="${base}/${comp}/${vers}"
      svn co $toget $comp
   done
fi
