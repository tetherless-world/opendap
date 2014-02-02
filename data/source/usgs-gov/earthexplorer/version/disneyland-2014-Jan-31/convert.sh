#!/bin/bash

source ~/*/utilities/netcdf/local-paths.sh

mkdir -p automatic

ncgen -k 3 -o automatic/CA_OrangeCo_2011_000402.sample.cdl.nc manual/CA_OrangeCo_2011_000402.sample.cdl
ncdump automatic/CA_OrangeCo_2011_000402.sample.cdl.nc
