#!/bin/bash
#
#

if [ $# -lt 3 ]; then
	echo "Usage error"
	echo "Usage: build.sh <osys> <arch> <HSM>"
	echo "<osys>: win cygwin or macos"
	echo "<arch>: 32 or 64"
	echo "<HSM> : 0  or 1"
	exit 1
fi

osys="$1"
arch=$2
HSME=$3


#
#	Decide OS
#
case "$osys" in
	win)
		if [ "$arch" -eq '32' ]; then
			CROSS_COMPIL="i686-w64-mingw32-"
		fi
		if [ "$arch" -eq '64' ]; then
			CROSS_COMPIL="x86_64-w64-mingw32-"
		fi
		;;
	
	cygwin)	
		if [ "$arch" -eq '32' ]; then
			CROSS_COMPIL="i686-w64-mingw32-"
		fi
		if [ "$arch" -eq '64' ]; then
			CROSS_COMPIL="/opt/cross-compiler/x86_64-pc-cygwin/bin/x86_64-pc-cygwin-"
		fi
		;;	
		
	linux)	
		echo "Do nothing for Linux"
		;;	
		
	macos)
		echo "Do nothing for macos"
		;;
		
	#
	#	Below section is test purpose
	#
	clean)
		echo "Todo clean"
		;;
		
	*)
		echo "Error! Not supported OS"
		exit 1
esac

if [ "$HSME" -eq '1' ]; then
	HSM=1
fi

# Set version number
gittag=$(git describe --tags)
sed -i -e "s/#__VERSION__#/$gittag/" ./include/ca_sign_build.h

# make it
make OS=$osys ARCH=$arch CROSS_COMPIL=$CROSS_COMPIL HSM=$HSM
if [ $? -ne 0 ]; then
	echo "Make Failed"
	exit 1
fi

echo "${gittag}" > vers.number

exit 0
