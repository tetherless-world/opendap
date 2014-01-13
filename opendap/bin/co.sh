#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/opendap/bin/co.sh>;
#3>    prov:wasDerivedFrom   <https://github.com/tetherless-world/opendap/issues/7>;
#3> .

conf="$1"

base="https://scm.opendap.org/svn/tags"

if [[ -e "$conf" ]]; then
   for i in `cat $conf`; do
      comp=`echo $i | cut -d'/' -f1`
      vers=`echo $i | cut -d'/' -f2`
      toget="${base}/${comp}/${vers}"
      svn co $toget $comp
   done
fi
