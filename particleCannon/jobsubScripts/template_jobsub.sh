#!/bin/bash
printenv
set -x #start bash debugging at this point
echo Start  `date`
echo Site:${GLIDEIN_ResourceName}
echo "the worker node is " `hostname` "OS: "  `uname -a`
echo "the user id is " `whoami`
echo "the output of id is " `id`
set +x #stop bash debugging at this point

umask 002 #set the read/write permission of files created to be 775

cd $_CONDOR_SCRATCH_DIR

echo "pwd is " `pwd`

SCRATCH_DIR=/pnfs/dune/persistent/users/dlast/reco_output/anumu

source /cvmfs/fermilab.opensciencegrid.org/products/common/etc/setup
source /cvmfs/fermilab.opensciencegrid.org/products/larsoft/setup
source /cvmfs/dune.opensciencegrid.org/products/dune/setup

setup dunetpc v06_73_00 -q e15:prof

if [ -z ${GRID_USER} ]; then
GRID_USER=`basename $X509_USER_PROXY | cut -d "_" -f 2`
fi

echo "GRID_USER = `echo $GRID_USER`"

echo "Here is the your environment in this job: " > job_output_${CLUSTER}.${PROCESS}.log 2> job_output_${CLUSTER}.${PROCESS}.err #Creates file for logging information, note that this is stdout only
env >> job_output_${CLUSTER}.${PROCESS}.log 2>> job_output_${CLUSTER}.${PROCESS}.err #since file already exists use ">>" instead of ">"

ifdh cp /pnfs/dune/persistent/users/${GRID_USER}/threshold_comparison.fcl `pwd`/threshold_comparison.fcl

if [ $? -ne 0]; then
        echo "Failed to find fcl file for input"
        echo "are you sure this file exists:"
        echo "/dune/data/users/dlast/Working_Dir/threshold_comparison.fcl"
        exit 71
fi
