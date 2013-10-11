#!/bin/bash

STYLE_DIRECTORY=`dirname $0`
CHECK_DIRECTORY=$1
for FILENAME in $(find ${CHECK_DIRECTORY} -name "*.cpp" -or -name "*.h" )
do
  echo
  echo    ------------------------------------------------------------------------------------------
  echo "check style in $FILENAME"
  echo
  ${STYLE_DIRECTORY}/style_check.sh $FILENAME

done

