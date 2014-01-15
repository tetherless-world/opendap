#!/bin/bash

echo source/loaded-graphs.rq.csv
if [[ ! -e source/loaded-graphs.rq.csv && "$CSV2RDF4LOD_PUBLISH_VIRTUOSO_SPARQL_ENDPOINT" =~ http* ]]; then
    cache-queries.sh $CSV2RDF4LOD_PUBLISH_VIRTUOSO_SPARQL_ENDPOINT -o csv -q manual/loaded-graphs.rq -od source/
else
    echo "could not"
fi

echo publish.sh
grep http source/loaded-graphs.rq.csv | awk -F, '{local=$2;gsub(/^.*\//,"",local);gsub(/\"/,"",local);local="source/"local;ng=$1"#given";gsub(/\"/,"",ng);printf("curl -L %s > %s\nvload ttl %s %s\n\n",$2,local,local,ng)}' > publish.sh
chmod +x publish.sh

