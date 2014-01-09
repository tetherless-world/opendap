#!/bin/bash

if [[ "$1" == '--clean' ]]; then
   rm -f automatic/svn-files.bte.ttl
   exit
fi

mkdir automatic &> /dev/null

python $DATAFAQS_HOME/services/sadi/faqt/naming/between-the-edges.py source/svn-files.rq.ttl text/turtle automatic/svn-files.bte.ttl
