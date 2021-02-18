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

PER_DIR=/pnfs/minerva/persistent/users/dlast/particle_cannon/
SCRATCH_DIR=/pnfs/minerva/scratch/users/dlast/particle_cannon/
#SCRATCH_DIR_OUT=/pnfs/minerva/scratch/users/dlast/particle_cannon_analysis_output/
#Line above replaced by use of moving files to $CONDOR_DIR_OUT by using -d OUT /pnfs/minerva/scratch/users/dlast/particle_cannon_analysis_output/

source /cvmfs/minerva.opensciencegrid.org/minerva/software_releases/v22r1p1/setup.sh 

if [ -z ${GRID_USER} ]; then
GRID_USER=`basename $X509_USER_PROXY | cut -d "_" -f 2`
fi

echo "GRID_USER = `echo $GRID_USER`"

OUTTAG=`date +%m_%d_%Y_%H_%M`_${CLUSTER}.${PROCESS}

echo "Here is the your environment in this job: " > job_output_${OUTTAG}.log 2> job_output_${OUTTAG}.err #Creates file for logging information, note that this is stdout only
env >> job_output_${OUTTAG}.log 2>> job_output_${OUTTAG}.err #since file already exists use ">>" instead of ">"

ifdh cp ${PER_DIR}/G4_processing_jobsub `pwd`/G4_processing_jobsub.exe

if [ $? -ne 0 ]; then
        echo "Failed to find executable for this job."
        echo "Are you sure this file exists?:"
        echo "/pnfs/minerva/persistent/users/${GRID_USER}/particle_cannon/G4_processing_jobsub"
        exit 71
fi

chmod u+x G4_processing_jobsub.exe

ifdh cp ${SCRATCH_DIR}/MAD_Fixed_Neutrons_Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_425_07_21_2020.root `pwd`/MAD_Fixed_Neutrons_Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_425_07_21_2020.root

if [ $? -ne 0 ]; then
        echo "Failed to find the first input for this job."
        echo "Are you sure this file exists?:"
        echo "${SCRATCH_DIR}/MAD_Fixed_Neutrons_Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_425_07_21_2020.root"
        exit 72
fi

./G4_processing_jobsub.exe MAD MAD_Fixed_Neutrons_Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_425_07_21_2020 ${OUTTAG} 2112 > COUT_MAD_Fixed_Neutrons_Run_126000_Target5_NeutronPlusMuon_PDG_2112_Ek_lo_0.0_Ek_hi_5000.0_nFiles_425_07_21_2020_tagged_${OUTTAG}.txt

mv G4_processed_*.root $CONDOR_DIR_OUT
mv COUT_*.txt $CONDOR_DIR_OUT
mv job_output_${OUTTAG}.* $CONDOR_DIR_OUT
