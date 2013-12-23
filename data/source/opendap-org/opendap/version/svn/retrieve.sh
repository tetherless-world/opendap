#!/bin/bash

if [[ ! -e source/tags ]]; then
   pushd source &> /dev/null
      svn co https://scm.opendap.org/svn/tags/
   popd &> /dev/null
else
   pushd source/tags &> /dev/null
      svn update
   popd &> /dev/null
fi
