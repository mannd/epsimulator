#!/bin/bash
# This script erases all epsimulator catalog, settings and 
# data files!  Also erases main database
# This is handy for test purposes, but don't run it
# on data you want to save!!
# Note location of configuration files is different in Qt4 vs 3

dirname=`dirname $0`
if [ "${dirname:0:1}" != "/" ]; then
    dirname=$PWD/$dirname
fi

$dirname/erasedata.sh
$dirname/erasedatabase.sh
