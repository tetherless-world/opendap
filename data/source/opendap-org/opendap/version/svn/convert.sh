#!/bin/bash

saxon.sh ../../src/grddl.xsl xml ttl -v `cr-sdv.sh --attribute-value` -in manual/tags.log 
