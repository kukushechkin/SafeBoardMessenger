#!/bin/bash

echo "[*] ar-ios $@"

function IsFatBinary() {
	local binaryInfo="$($LIPO -info "$1")"
	[ `echo $binaryInfo | grep -c "Non-fat file"` -gt 0 ] && return 0 || return 1
}

function GetFatBinaryArchs() {
	echo "$($LIPO -detailed_info "$1" | awk '/^architecture /{print $2;}')"
}

function GetThinBinaryArchs() {
	echo "$($LIPO -detailed_info "$1" | awk '/architecture: /{print $6;}')"
}

function SplitBinary() {
	IsFatBinary "$1"
	isFatBin=$?
	binFileName="$(basename "$1")"

	if [ "$isFatBin" -gt 0 ] ; then
		local archs=$(GetFatBinaryArchs "$1")
	else
		local archs=$(GetThinBinaryArchs "$1")
	fi

	for arch in $archs ; do
		archTmpDir="$2/$arch"
		archBinFile="$archTmpDir/$binFileName"
		
		[ -d "$archTmpDir" ] || mkdir -p "$archTmpDir"

		if [ "$isFatBin" -gt 0 ] ; then
			$LIPO "$1" -thin "$arch" -output "$archBinFile"
		else
			cp -f "$1" "$archBinFile"
		fi
	done
}


if [ "$1" != 'r' ] ; then
	ar "$@"
	exit $?
fi

libFile="$2"
libFileName="$(basename "$libFile")"
tempDir="./ar_tmp"
rm -rf "$tempDir"

# split existing library
if [ -f "$libFile" ] ; then
	SplitBinary "$libFile" "$tempDir"
fi

# split object files
for objFile in "${@:3}" ; do
	SplitBinary "$objFile" "$tempDir"
done

# make library
archLibs=""
for arch in $(ls "$tempDir") ; do
	archTempDir="$tempDir/$arch"
	archLib="$archTempDir/$libFileName"
	archLibs="$archLibs $archLib"

	for objFile in $(ls "$archTempDir"/*.o) ; do
		ar r "$archLib" "$objFile"
	done
done

echo "[*] lipo $archLibs -create -output $libFile"
$LIPO $archLibs -create -output "$libFile"

rm -rf "$tempDir"