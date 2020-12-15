#!/bin/bash

commit=""
date=`date '+%Y%m%d'`
IFS=':'

for dir in $PATH; do
    if [ -x $dir/git ]; then
        commit=`git rev-parse HEAD 2> /dev/null`
        
        if [ $? != 0 ]; then
            commit="";
        else
            commit=`echo ".$commit" | cut -c1-9`
        fi

        break
    fi
done

echo "${date}${commit}"
