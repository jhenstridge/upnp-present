#!/bin/bash

set -e

outdir=.

while getopts "o:" opt; do
    case $opt in
        o)
            outdir="${OPTARG}"
            ;;
        \?)
            exit
            ;;
    esac
done

arch=$(dpkg-architecture -qDEB_HOST_MULTIARCH)
if [ ! -d ${outdir} ]; then
    mkdir ${outdir}
fi

shift $((OPTIND-1))
for lib in "$@"; do
    for libdir in /usr/lib/${arch} /usr/lib /lib/${arch} /lib; do
        if [ -e ${libdir}/${lib} ]; then
            cp -d ${libdir}/${lib}* ${outdir}
            rm ${outdir}/${lib}
            break
        fi
    done
done
