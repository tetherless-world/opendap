#!/bin/bash

if [[ "$1" == '--clean' ]]; then
   rm -f automatic/svn-files.bte.ttl
   exit
fi

mkdir automatic &> /dev/null

python $DATAFAQS_HOME/services/sadi/faqt/naming/between-the-edges.py source/svn-files.rq.ttl text/turtle automatic/svn-files.bte.ttl
justify.sh source/svn-files.rq.ttl automatic/svn-files.bte.ttl https://github.com/timrdf/DataFAQs/blob/master/services/sadi/faqt/naming/between-the-edges.py

if [[ "$CSV2RDF4LOD_PUBLISH" == 'true' ]]; then
   aggregate-source-rdf.sh automatic/svn-files.bte.ttl
else
   echo "Avoiding publishing b/c \$CSV2RDF4LOD_PUBLISH is not 'true'; set to 'true' to publish."
fi
