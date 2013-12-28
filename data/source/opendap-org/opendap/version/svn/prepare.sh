#!/bin/bash

mkdir manual &> /dev/null

for portion in tags trunk; do
   echo manual/$portion.log
   if [[ source/$portion -nt manual/$portion.log ]]; then
      pushd source/$portion
         svn log -v --xml > ../../manual/$portion.log
      popd
   fi
done
