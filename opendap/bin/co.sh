#!/bin/sh

base="https://scm.opendap.org/svn/tags"

for i in `cat versionsToGet`
do
	comp=`echo $i | cut -d'/' -f1`
	vers=`echo $i | cut -d'/' -f2`
	toget="${base}/${comp}/${vers}"
	svn co $toget $comp
done

