#!/bin/bash

for portion in tags trunk; do
   echo manual/$portion.log
   if [[ ! -e manual/$portion.log ]]; then
      if [[ ! -e source/$portion ]]; then
         pushd source &> /dev/null
            echo svn co https://scm.opendap.org/svn/$portion/
                 svn co https://scm.opendap.org/svn/$portion/
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
