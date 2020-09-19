#!/bin/sh
if [ "$1" = "" ] ; then
	exit 0
fi

inkscape --export-type=png --export-width=64 --export-height=64 --export-filename=z1.0/icons/${1%.*}.png $1
inkscape --export-type=png --export-width=80 --export-height=80 --export-filename=z1.25/icons/${1%.*}.png $1
inkscape --export-type=png --export-width=96 --export-height=96 --export-filename=z1.5/icons/${1%.*}.png $1
inkscape --export-type=png --export-width=112 --export-height=112 --export-filename=z1.75/icons/${1%.*}.png $1
inkscape --export-type=png --export-width=128 --export-height=128 --export-filename=z2.0/icons/${1%.*}.png $1
