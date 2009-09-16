#!/bin/sh
pwd = $PWD
dirname=`dirname $0`
if [ "${dirname:0:1}" != "/" ]; then
    dirname=$PWD/$dirname
fi
cd $dirname/..
doxygen Doxyfile
cd $pwd
