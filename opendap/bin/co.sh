#!/bin/bash

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
