#!/bin/sh
if [ "$1" = "" ] ; then
	exit 0
fi
for r in `ls -d */ |sed 's/\///g'` ; do
	convert -resize $r $1 $r/harbour-kontroller.png
done
