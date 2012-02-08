#!/bin/sh

COMMIT=`git show -s --format=%h`

MODIFIED=""
if [ -n "`git diff-index HEAD`" ]
then
	MODIFIED="-mod"
fi

for x in `git tag`
do
	TC=`git show -s --format=%h "$x"`
	if [ "$TC" == "$COMMIT" ]
	then
		echo $x$MODIFIED
		exit
	fi
done

echo $COMMIT$MODIFIED
