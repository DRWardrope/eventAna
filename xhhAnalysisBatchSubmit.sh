################################################################
# Submit jobs to UCL batch farm with
#
# > ./xhhAnalysisBatchSubmit.sh -i <input-files-directory>
#
# where <input-files-directory> is the path to the directory
# containing root files from ntuple maker (and nothing else)
#
# The output will appear in the folder batchSubmit-<time/date>
#
################################################################


################################################################
# help message
################################################################
usage() {
    echo "Usage:"
    echo "./xhhAnalysisBatchSubmit.sh -i <input-files-directory>"
}

################################################################
# read arguments
################################################################
input=""
while getopts "i:" opt
do
    case $opt in
	i) 
	    input=$OPTARG;;
	:) 
	    echo "Option -$OPTARG requires an argument."
	    exit 1
	    ;;
	'?') 
	    echo "Invalid option: -$OPTARG" 
	    exit 1
	    ;;
    esac
done

################################################################
# check if arguments were passed
################################################################
if [ "$input" == "" ] 
then
    usage
    exit 1
fi

################################################################
# Check if input directory is defined
################################################################
if [ ! -d "$input" ] 
then
    echo "Error: couldn't find input directory : $input"
    exit 1
fi

################################################################
# Define submission directory that the batch job runs from
################################################################
wkdir="$(pwd)"
outdir="$wkdir/batchSubmit-$(date +"%d-%m-%y"_%H-%M-%S)"
mkdir -p $outdir
################################################################
# Loop over input files and submit jobs
################################################################
counter=0
for infile in `ls $input`
do
    subdir=subjob_$counter
    mkdir -p $outdir/$subdir
	mkdir -p $outdir/$subdir/data
	ln -s $wkdir/data/BJetTurnOnFile_2016.root $outdir/$subdir/data/.
	ln -s $wkdir/data/2016BJetTriggerSignalEff.root $outdir/$subdir/data/.
	ln -s $wkdir/data/BJetTriggerEfficiencies-00-02-01.root $outdir/$subdir/data/.
    cd $outdir/$subdir
    sed -e "s;WKDIR;$wkdir;g" -e "s;OUTDIR;$outdir;g" -e "s;SUBDIR;$subdir;g" -e "s;INFILE;$input/$infile;g" < ../../xhhAnalysisBatchRun.sh > ./xhhAnalysisBatchRun.sh
    echo "Submitting subjob $counter"
    qsub -q medium xhhAnalysisBatchRun.sh
    counter=$((counter+1))
done
cd $wkdir
