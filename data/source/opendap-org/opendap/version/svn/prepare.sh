#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/data/source/opendap-org/opendap/version/svn/prepare.sh>;
#3>    rdfs:seeAlso <https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Provenance>;
#3> .

for portion in tags trunk; do
   echo manual/$portion.log
   if [[ -e source/$portion && source/$portion -nt manual/$portion.log ]]; then
      mkdir manual &> /dev/null
      pushd source/$portion
         svn log -v --xml > ../../manual/$portion.log
      popd
      justify.sh source/$portion manual/$portion.log svn_log
   else
      echo "   (Skipping manual/$portion.log because svn repository is not checked out at source/$portion, or it's older than the log.)"
   fi
done
