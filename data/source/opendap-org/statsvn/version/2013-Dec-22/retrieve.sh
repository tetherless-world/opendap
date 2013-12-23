#!/bin/bash

mkdir manual &> /dev/null

log="../../../opendap/version/svn/manual/tags.log"
if [[ ! -e $log ]]; then
   echo "error: need $log"
   exit 1
else
   pushd manual &> /dev/null
      tags="../../../../opendap/version/svn/source/tags"

      echo repo-statistics.xml
      if [[ ../$log -nt repo-statistics.xml ]]; then
         java -jar ../../../src/statsvn-0.7.0/statsvn.jar -xml                 ../$log $tags
      fi
      echo rendered/index.html
      if [[ ../$log -nt rendered/index.html ]]; then
         java -jar ../../../src/statsvn-0.7.0/statsvn.jar -output-dir rendered ../$log $tags
      fi
   popd &> /dev/null
fi
