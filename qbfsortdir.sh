#!/bin/bash
#
# usage: qbfsortdir.sh [-j parallelJobs|-n] inputDir outputDir [options]
#
parallelJobs=1
while getopts "j:n" option
do
  case "${option}" in
    j) parallelJobs=$OPTARG;;
    n) parallelJobs=$(nproc);;
  esac
done
shift $((${OPTIND} - 1))
inputDir=$1
outputDir=$2
shift 2

mkfifo fifo-$$
exec 3<>fifo-$$
rm fifo-$$
for ((i=0; i<${parallelJobs}; ++i))
do
  printf "." >&3
done

mkdir --parents "${outputDir}"
for f in $(ls "${inputDir}")
do
  read -u 3 -n 1 x
  (
    qbfsort --input "${inputDir}"/"$f" --output "${outputDir}"/$(basename "$f") $@
    printf "." >&3
  )&
done

wait
