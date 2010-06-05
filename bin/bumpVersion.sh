#! /usr/bin/env bash

## Command line parameters
if [[ $# != 2 ]]; then
    cat <<USAGE
usage:
  $0 <old> <new>
example:
  $0 '1.2.3' '1.2.4'
USAGE
    exit 1
fi


## Process and show version
OLD=`sed 's/\./\\\\./g' <<<"$1"`
NEW=`sed 's/\./\\\\./g' <<<"$2"`

OLD_MAJOR=`sed 's/^\([0-9]*\)\.[0-9]*\.[0-9]*/\1/' <<<"$1"`
NEW_MAJOR=`sed 's/^\([0-9]*\)\.[0-9]*\.[0-9]*/\1/' <<<"$2"`

OLD_MINOR=`sed 's/^[0-9]*\.\([0-9]*\)\.[0-9]*/\1/' <<<"$1"`
NEW_MINOR=`sed 's/^[0-9]*\.\([0-9]*\)\.[0-9]*/\1/' <<<"$2"`

OLD_RELEASE=`sed 's/^[0-9]*\.[0-9]*\.\([0-9]*\)/\1/' <<<"$1"`
NEW_RELEASE=`sed 's/^[0-9]*\.[0-9]*\.\([0-9]*\)/\1/' <<<"$2"`

echo "#==============================================="
echo "# Plain    '${OLD}'     -> '${NEW}'"
echo "#-----------------------------------------------"
echo "# Major    '${OLD_MAJOR}'           -> '${NEW_MAJOR}'"
echo "# Minor    '${OLD_MINOR}'           -> '${NEW_MINOR}'"
echo "# Release  '${OLD_RELEASE}'           -> '${NEW_RELEASE}'"
echo "#==============================================="
echo


## Make script safe to call from anywhere by going home first
SCRIPT_DIR=`dirname "${PWD}/$0"`/..
echo "Entering directory \`${SCRIPT_DIR}'"
pushd "${SCRIPT_DIR}" &>/dev/null || exit 1



## Patch coreconstants.h
TMPFILE=`mktemp versionPatch.XXXXXX`
CORE_CONSTANT_H="${SCRIPT_DIR}/src/libs/core/coreconstants.h"
echo "Patching \`${CORE_CONSTANT_H}'"
sed \
        -e 's/^\(#define APP_VERSION_MAJOR \)'"${OLD_MAJOR}"'/\1'"${NEW_MAJOR}"'/' \
        -e 's/^\(#define APP_VERSION_MINOR \)'"${OLD_MINOR}"'/\1'"${NEW_MINOR}"'/' \
        -e 's/^\(#define APP_VERSION_RELEASE \)'"${OLD_RELEASE}"'/\1'"${NEW_RELEASE}"'/' \
    "${CORE_CONSTANT_H}" > "${TMPFILE}"
mv -f "${TMPFILE}" "${CORE_CONSTANT_H}"


## Go back to original $PWD
echo "Leaving directory \`${SCRIPT_DIR}'"
popd &>/dev/null || exit 1
exit 0
