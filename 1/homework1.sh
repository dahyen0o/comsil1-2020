#!/bin/bash

echo 'Working directory:'
read dirc

if [ ! $dirc ]
then
    echo 'Error, no input'
    exit 0
fi

if [ ! -d $dirc ]
then
    echo 'Error, not exist'
    exit 0
else
    cd $dirc
fi

for i in *
do
    mv $i `echo $i | tr '[A-Z][a-z]' '[a-z][A-Z]'`
done
