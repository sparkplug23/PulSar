#!/bin/bash

echo "Installing Python Wheel ..."
pip install wheel > /dev/null 2>&1

echo "Installing PlatformIO ..."
pip install -U https://github.com/platformio/platformio/archive/master.zip > /dev/null 2>&1
echo "PlatformIO has been installed"
echo ""

function build_pio_sketch(){ # build_pio_sketch <board> <options> <path-to-ino>
    if [ "$#" -lt 3 ]; then
        echo "ERROR: Illegal number of parameters"
        echo "USAGE: build_pio_sketch <board> <options> <path-to-ino>"
        return 1
    fi

    local board="$1"
    local options="$2"
    local sketch="$3"
    local buildFlags="$4"
    local sketch_dir=$(dirname "$sketch")
    echo ""
    echo "Compiling '"$(basename "$sketch")"' ..."
    python -m platformio ci -l "." --board "$board" "$sketch_dir" --project-option="$options" --project-option="build_flags=$buildFlags"
}

function count_sketches(){ # count_sketches <examples-path>
    local examples="$1"
    rm -rf sketches.txt
    if [ ! -d "$examples" ]; then
        touch sketches.txt
        return 0
    fi
    local sketches=$(find $examples -name *.ino)
    local sketchnum=0
    for sketch in $sketches; do
        local sketchdir=$(dirname $sketch)
        local sketchdirname=$(basename $sketchdir)
        local sketchname=$(basename $sketch)
        if [[ "${sketchdirname}.ino" != "$sketchname" ]]; then
            continue
        fi
        if [[ -f "$sketchdir/.test.skip" ]]; then
            continue
        fi
        echo $sketch >> sketches.txt
        sketchnum=$(($sketchnum + 1))
    done
    return $sketchnum
}

function build_pio_sketches(){ # build_pio_sketches <board> <options> <examples-path> <chunk> <total-chunks>
    if [ "$#" -lt 3 ]; then
        echo "ERROR: Illegal number of parameters"
        echo "USAGE: build_pio_sketches <board> <options> <examples-path> [<chunk> <total-chunks>]"
        return 1
    fi

    local board=$1
    local options="$2"
    local examples=$3
    local chunk_idex=$4
    local chunks_num=$5

    if [ "$#" -lt 5 ]; then
        chunk_idex="0"
        chunks_num="1"
    fi

    if [ "$chunks_num" -le 0 ]; then
        echo "ERROR: Chunks count must be positive number"
        return 1
    fi
    if [ "$chunk_idex" -ge "$chunks_num" ]; then
        echo "ERROR: Chunk index must be less than chunks count"
        return 1
    fi

    set +e
    count_sketches "$examples"
    local sketchcount=$?
    set -e
    local sketches=$(cat sketches.txt)
    rm -rf sketches.txt

    local chunk_size=$(( $sketchcount / $chunks_num ))
    local all_chunks=$(( $chunks_num * $chunk_size ))
    if [ "$all_chunks" -lt "$sketchcount" ]; then
        chunk_size=$(( $chunk_size + 1 ))
    fi

    local start_index=$(( $chunk_idex * $chunk_size ))
    if [ "$sketchcount" -le "$start_index" ]; then
        echo "Skipping job"
        return 0
    fi

    local end_index=$(( $(( $chunk_idex + 1 )) * $chunk_size ))
    if [ "$end_index" -gt "$sketchcount" ]; then
        end_index=$sketchcount
    fi

    local start_num=$(( $start_index + 1 ))
    echo "Found $sketchcount Sketches";
    echo "Chunk Count : $chunks_num"
    echo "Chunk Size  : $chunk_size"
    echo "Start Sketch: $start_num"
    echo "End Sketch  : $end_index"

    local sketchnum=0
    for sketch in $sketches; do
        local sketchdir=$(dirname $sketch)
        local sketchdirname=$(basename $sketchdir)
        local sketchname=$(basename $sketch)
        if [ "${sketchdirname}.ino" != "$sketchname" ] \
        || [ -f "$sketchdir/.test.skip" ]; then
            continue
        fi
        local sketchBuildFlags=""
        if [ -f "$sketchdir/.test.build_flags" ]; then
            while read line; do
                sketchBuildFlags="$sketchBuildFlags $line"
            done < "$sketchdir/.test.build_flags"
        fi                
        sketchnum=$(($sketchnum + 1))
        if [ "$sketchnum" -le "$start_index" ] \
        || [ "$sketchnum" -gt "$end_index" ]; then
            continue
        fi
        build_pio_sketch "$board" "$options" "$sketch" "$sketchBuildFlags"
        local result=$?
        if [ $result -ne 0 ]; then
            return $result
        fi
    done
    return 0
}
