#!/bin/bash

mkdir manual &> /dev/null

echo manual/tags.log
if [[ source/tags -nt manual/tags.log ]]; then
   pushd source/tags
      svn log -v --xml > ../../manual/tags.log
   popd
fi
