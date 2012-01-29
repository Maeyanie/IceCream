#!/bin/sh

COMMIT=`git show -s --format=%h`

for x in `git tag`
do
	TC=`git show -s --format=%h "$x"`
	if [ "$TC" == "$COMMIT" ]
	then
		echo $x
		exit
	fi
done

echo $COMMIT
