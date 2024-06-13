#!/usr/bin/env bash

for FILE in *.vert; do

    split=(${FILE//./ })
    output="compiled/"${split[0]}".vert.spv"

    /usr/bin/glslc $FILE -o $output

done

for FILE in *.frag; do

    split=(${FILE//./ })
    output="compiled/"${split[0]}".frag.spv"

    /usr/bin/glslc $FILE -o $output

done