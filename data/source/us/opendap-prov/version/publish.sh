#!/bin/bash
#
#3> <> a conversion:PublicationTrigger, conversion:Idempotent;
#3>    foaf:name    "publish.sh";
#3>    rdfs:seeAlso <https://github.com/timrdf/csv2rdf4lod-automation/wiki/Triggers#4-publication-triggers>;
#3> .
#

filename="opendap-provenance.ttl"
for record in `find . -mindepth 3 -maxdepth 3 -name $filename`; do
    # e.g. record =
    # ./20140122-1390405127-63a9/source/opendap-provenance.ttl
    # ./20140121-1390404237-df2b/source/opendap-provenance.ttl

    cockpit=${record%/source*}         # e.g. ./20140122-1390405127-63a9
    publish=${record%/source*}/publish # e.g. ./20140122-1390405127-63a9/publish
    if [[ ! -e $publish ]]; then
        pushd $cockpit &> /dev/null
            echo "INFO: `cr-pwd.sh` has not been published; publishing it."
            aggregate-source-rdf.sh source/$filename
        popd &> /dev/null
    fi
done
