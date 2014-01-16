#!/bin/bash

name="eg-nasa-shell"

if [[ "$1" == '--clean' ]]; then
   rm -f automatic/$name.bte.ttl
   exit
fi

mkdir automatic &> /dev/null

python $DATAFAQS_HOME/services/sadi/faqt/naming/between-the-edges.py source/$name.rq.ttl text/turtle automatic/$name.bte.ttl
justify.sh source/$name.rq.ttl automatic/$name.bte.ttl https://github.com/timrdf/DataFAQs/blob/master/services/sadi/faqt/naming/between-the-edges.py

if [[ "$CSV2RDF4LOD_PUBLISH" == 'true' ]]; then
   aggregate-source-rdf.sh automatic/$name.bte.ttl
else
   echo "Avoiding publishing b/c \$CSV2RDF4LOD_PUBLISH is not 'true'; set to 'true' to publish."
fi
