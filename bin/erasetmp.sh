#!/bin/bash
# This script erases the tmp dir in epsimulator
# which can be left behind if testepsimulator.sh fails

dirname=`dirname $0`
if [ "${dirname:0:1}" != "/" ]; then
    dirname=$PWD/$dirname
fi

rm -fr $dirname/../tmp

