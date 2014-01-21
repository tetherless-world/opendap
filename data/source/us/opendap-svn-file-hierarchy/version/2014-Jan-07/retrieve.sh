#!/bin/bash

name="eg-nasa-shell"

if [[ "$1" == '--clean' ]]; then
   rm -f source/$name.rq.ttl*
   exit
fi

cache-queries.sh $CSV2RDF4LOD_PUBLISH_SPARQL_ENDPOINT -o ttl -q manual/$name.rq -od source/
