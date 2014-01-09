#!/bin/bash
#
#3> <> prov:specializationOf <https://github.com/tetherless-world/opendap/blob/master/data/source/opendap-org/opendap/version/svn/retrieve.sh>;
#3>    rdfs:seeAlso <https://github.com/tetherless-world/opendap/wiki/OPeNDAP-Provenance>;
#3> .

for portion in tags trunk; do
   echo manual/$portion.log
   if [[ ! -e manual/$portion.log ]]; then
      if [[ ! -e source/$portion ]]; then
         pushd source &> /dev/null
            echo svn co https://scm.opendap.org/svn/$portion/
                 svn co https://scm.opendap.org/svn/$portion/
            justify.sh  https://scm.opendap.org/svn/$portion source/$portion svn
         popd &> /dev/null
      else
         pushd source/$portion &> /dev/null
            echo svn update
                 svn update
         popd &> /dev/null
      fi
   else
      echo "   (Avoiding SVN checkout b/c manual/$portion.log already exists.)"
   fi
done
