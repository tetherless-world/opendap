#!/bin/bash

if [[ "$1" == '--clean' ]]; then
   rm -f source/svn-files.rq.ttl*
   exit
fi

cache-queries.sh $CSV2RDF4LOD_PUBLISH_SPARQL_ENDPOINT -o ttl -q manual/svn-files.rq -od source/
