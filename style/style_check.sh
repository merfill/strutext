#!/bin/bash

FILE=$1
if [[ -z $FILE ]]; then
    echo "usage style_check.sh <file name>"
    exit 1
fi

filters=-readability/streams,-build/include_what_you_use,-whitespace/comments,-runtime/references,-runtime/rtti

$(dirname $0)/cpplint.py --filter=${filters} "$@"
