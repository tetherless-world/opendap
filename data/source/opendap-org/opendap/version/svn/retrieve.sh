#!/bin/bash

if [[ ! -e source/tags ]]; then
   pushd source &> /dev/null
      echo svn co https://scm.opendap.org/svn/tags/
           svn co https://scm.opendap.org/svn/tags/
   popd &> /dev/null
else
   pushd source/tags &> /dev/null
      echo svn update
           svn update
   popd &> /dev/null
fi
