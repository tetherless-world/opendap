#!/bin/bash

mkdir automatic &> /dev/null

python $DATAFAQS_HOME/services/sadi/faqt/naming/between-the-edges.py source/svn-files.rq.ttl text/turtle automatic/svn-files.bte.ttl
