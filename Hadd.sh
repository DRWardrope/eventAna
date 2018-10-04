#!/bin/bash

# help message
usage() {
    echo "Usage:"
    echo "./Hadd.sh -i <input-directory> -o <output-directory> -f <max-number-of-files>"
}

# read arguments
input=""
output=""
max=-1
while getopts "i:o:f:" opt
do
    case $opt in
	i) 
	    input=$OPTARG;;
	o) 
	    output=$OPTARG;;
	f) 
	    max=$OPTARG;;
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

# check if arguments were passed
if [ "$input" == "" ] || [ "$output" == "" ] || [ $max -eq -1 ]
then
    usage
    exit 1
fi

# check if input dir exists
if [ ! -d "$input" ]
then
    echo "Error: cannot find input directory : $input"
    usage
    exit 1
fi

# check if output dir exists
if [ ! -d "$output" ]
then
    echo "Error: cannot find output directory : $output"
    usage
    exit 1
fi

# get number of files in input directory
tot=`ls $input | wc -l`
echo "total number of input files = $tot"

# create lists of files and hadd the files
all=0
in=0
out=0
list=""
for file in `ls $input`
do
    in=$((in+1))
    all=$((all+1))
    list+="${input}/${file} "
    if [[ $in == $max ]] || [[ $all == $tot ]]
    then
	hadd ${output}/ntuple_${out}.root $list
	in=0
	list=""
	out=$((out+1))
    fi
done
