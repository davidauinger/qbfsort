#!/bin/bash
inputDir=$1
outputDir=$2
shift 2
mkdir --parents "${outputDir}"
for f in $(ls "${inputDir}")
do
  ./qbfsort --input "${inputDir}"/"$f" --output "${outputDir}"/$(basename "$f") $@
done
