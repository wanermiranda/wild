#!/bin/sh
if test $# -lt 1
then
    echo "Usage: ./extract_descriptors descriptor"    
    exit 1
fi

cat dataset.txt | while read file
do
    #define filename wihtout \n 
    file=$(echo $file | sed "s/\n//g")
    ppm_file=$(echo $file | sed "s/\.png/\.ppm/g" )

    #gen the ppm file for the image
    echo convert $file $ppm_file
    convert $file $ppm_file

    #define output filename
    output=$(echo $file | sed "s/\.png/\.$1\.bin/g" | sed "s/eth80/hist/g" | sed "s/\.\.\///g")

    # create output dir 
    dir=$(dirname "${output}")
    mkdir -p $dir

    # extract the feature vectors for the image
    echo ./extractors/$1_extraction $ppm_file $output
    ./extractors/$1_extraction $ppm_file $output
done 