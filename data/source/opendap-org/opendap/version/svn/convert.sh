#!/bin/bash

mkdir automatic &> /dev/null

for portion in tags trunk; do
   if [[ manual/$portion.log -nt automatic/$portion.log.ttl || ../../src/grddl.xsl -nt automatic/$portion.log.ttl ]]; then
      saxon.sh ../../src/grddl.xsl xml ttl -v `cr-sdv.sh --attribute-value` -in manual/$portion.log > automatic/$portion.log.ttl
   fi
done
