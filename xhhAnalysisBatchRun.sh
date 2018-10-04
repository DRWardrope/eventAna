#!/bin/bash

# this script is executed by qsub
# the following variables are set separately for each subjob (with sed command in xhhAnalysisBatchSubmit.sh)
wkdir="WKDIR"
outdir="OUTDIR"
subdir="SUBDIR"
infile="INFILE"

# make sure /unix/atlasvhbb/ is automounted
cd /unix/atlasvhbb/

# setup environment
echo "Setting up environments..."
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase; 
source /cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase/user/atlasLocalSetup.sh

# setup root
lsetup root

# execute
echo "Executing xhhAnalysis..."
starttime=$(date +%s)
cd $outdir/$subdir
$wkdir/bin/xhhAnalysis --resolved --inputFile $infile | tee logfile
#$wkdir/bin/xhhAnalysis --resolved --noSyst --inputFile $infile | tee logfile
#$wkdir/bin/xhhAnalysis --merged --R10 --vetoResolved --inputFile $infile | tee logfile
#$wkdir/bin/xhhAnalysis --combined --inputFile $infile | tee logfile
endtime=$(date +%s)
echo "Elapsed time : $(($endtime - $starttime)) seconds"
