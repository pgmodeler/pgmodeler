#!/bin/bash

QT_INSTALL_VERSION='5.6.2'
QT_BASE_VERSION='5.6'
PGSQL_VERSION='9.6'
INNOSETUP_CMD='/c/Program Files (x86)/Inno Setup 5/ISCC.exe'
LOG=windeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.cpp | grep PGMODELER_VERSION | sed 's/PGMODELER_VERSION=QString("//g' | sed 's/"),//g'`
DEPLOY_VER=${DEPLOY_VER/PGMODELER_VERSION=\"/}
DEPLOY_VER=`echo ${DEPLOY_VER/\",/} | tr -d ' '`

DEMO_VERSION_OPT='-demo-version'
DEMO_VERSION=0

BUILD_ALL_OPT='-build-all'
BUILD_ALL=0

# Setting key paths according to the arch build (x86|x64)
# If none of the build type parameter is specified, the default is tu use x86
X64_BUILD_OPT='-x64-build'
X64_BUILD=0
X86_BUILD=1
WIN_BITS="32"
DEST_ARCH="x86"

for param in $@; do 
 if [[ "$param" == "$DEMO_VERSION_OPT" ]]; then
   DEMO_VERSION=1
 fi
 if [[ "$param" == "$X64_BUILD_OPT" ]]; then
   X64_BUILD=1
   DEST_ARCH="x64"
   WIN_BITS="64"
 fi
 if [[ "$param" == "$BUILD_ALL_OPT" ]]; then
   BUILD_ALL=1
   DEMO_VERSION=0
 fi
done

# Define the base name of the binary
if [ $DEMO_VERSION = 1 ]; then
  PKGNAME="pgmodeler-$DEPLOY_VER-demo-windows$WIN_BITS"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-windows$WIN_BITS"
fi

if [ $X64_BUILD = 1 ]; then
  # Settings for x64 build
  QT_ROOT="/c/Qt/Qt${QT_INSTALL_VERSION}-x64/"
  QMAKE_ROOT=$QT_ROOT/bin
  MINGW_ROOT="/c/msys64/mingw64/bin"
  PGSQL_ROOT="/c/PostgreSQL/${PGSQL_VERSION}-x64/bin"  
  QMAKE_ARGS="-r -spec win32-g++ CONFIG+=release \
              XML_INC+=$PGSQL_ROOT/../include \
			  XML_LIB=$PGSQL_ROOT/libxml2.dll \
			  PGSQL_INC+=$PGSQL_ROOT/../include \
			  PGSQL_LIB+=$PGSQL_ROOT/libpq.dll"
  DEP_LIBS="$QMAKE_ROOT/icudt55.dll \
		    $QMAKE_ROOT/icuin55.dll \
		    $QMAKE_ROOT/icuuc55.dll \
		    $MINGW_ROOT/libgcc_s_seh-1.dll \
		    $MINGW_ROOT/libstdc++-6.dll \
		    $MINGW_ROOT/libwinpthread-1.dll \
			$PGSQL_ROOT/libiconv-2.dll \
			$PGSQL_ROOT/libintl-8.dll"
else
  # Default setting for x86 build
  QT_ROOT="/c/Qt/Qt${QT_INSTALL_VERSION}/${QT_BASE_VERSION}/mingw49_32/"
  QMAKE_ROOT=$QT_ROOT/bin
  QMAKE_ARGS="-r -spec win32-g++ CONFIG+=release"
  MINGW_ROOT="/c/Qt/Qt${QT_INSTALL_VERSION}/Tools/mingw492_32/bin"
  PGSQL_ROOT="/c/PostgreSQL/${PGSQL_VERSION}/bin"
  DEP_LIBS="$QMAKE_ROOT/icudt54.dll \
		   $QMAKE_ROOT/icuin54.dll \
		   $QMAKE_ROOT/icuuc54.dll \
		   $QMAKE_ROOT/libgcc_s_dw2-1.dll \
		   $QMAKE_ROOT/libstdc++-6.dll \
		   $QMAKE_ROOT/libwinpthread-1.dll \
		   $PGSQL_ROOT/libiconv-2.dll \
		   $PGSQL_ROOT/libintl-8.dll"
fi

if [ $DEMO_VERSION = 1 ]; then
  QMAKE_ARGS="$QMAKE_ARGS DEMO_VERSION+=true"
fi

PKGFILE=$PKGNAME.exe
GENINSTALLER=pgmodeler.exe
INSTALL_ROOT="$PWD/build"
DIST_ROOT="$PWD/dist"
ISSFILE=./installer/windows/pgmodeler.iss
QT_CONF="$INSTALL_ROOT/qt.conf"
DEP_PLUGINS_DIR="$INSTALL_ROOT/qtplugins"
PLUGINS="dummy xml2object"

# Common dependency libraries 
DEP_LIBS+=" $QMAKE_ROOT/Qt5Core.dll \
		  $QMAKE_ROOT/Qt5Gui.dll \
		  $QMAKE_ROOT/Qt5Widgets.dll \
		  $QMAKE_ROOT/Qt5PrintSupport.dll \
		  $QMAKE_ROOT/Qt5Network.dll \
		  $QMAKE_ROOT/Qt5Svg.dll \
		  $PGSQL_ROOT/libxml2.dll \
		  $PGSQL_ROOT/libpq.dll \
		  $PGSQL_ROOT/libeay32.dll \
		  $PGSQL_ROOT/ssleay32.dll \
		  $PGSQL_ROOT/zlib1.dll"
		  
#Dependency qt plugins copied to build dir
DEP_PLUGINS="imageformats/qdds.dll \
			 imageformats/qgif.dll \
			 imageformats/qicns.dll \
			 imageformats/qico.dll \
			 imageformats/qjpeg.dll \
			 imageformats/qsvg.dll \
			 imageformats/qtga.dll \
			 imageformats/qtiff.dll \
			 imageformats/qwbmp.dll \
			 imageformats/qwebp.dll \
			 platforms/qwindows.dll \
			 printsupport/windowsprintersupport.dll"
		 
export PATH=$QMAKE_ROOT:$MINGW_ROOT:$PATH

clear
echo
echo "pgModeler Windows deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2016 Raphael A. Silva <raphael@pgmodeler.com.br>"

# Identifying Qt version
if [ -e "$QMAKE_ROOT/qmake" ]; then
  QT_VER=`$QMAKE_ROOT/qmake --version | grep '[0-9].[0-9].[0-9]'`
  QT_VER=${QT_VER:0:5}
fi

# If Qt was not found
if [ -z "$QT_VER" ]; then
  echo
  echo "** No Qt framework was found!"
  echo
  exit 1
else
  # Checking if identified version is valid (>= 5.0.0)
  if [[ "$QT_VER" < "5.0.0" ]]; then
    echo
    echo "** Qt framework found but in no suitable version (>= 5.0.0)!"
    echo "** Installed Qt version: $QT_VER"
    echo
    exit 1
  fi
fi


echo
echo "Deploying version: $DEPLOY_VER"

if [ $DEMO_VERSION = 1 ]; then
  echo "Building demonstration version. (Found $DEMO_VERSION_OPT)"
fi

echo "Building for arch: $DEST_ARCH"
echo "Cleaning previous compilation..."

if [ $BUILD_ALL -eq 1 ]; then
  rm -r $DIST_ROOT/* > $LOG 2>&1
fi

rm -r build/* > $LOG 2>&1
$MINGW_ROOT/mingw32-make.exe distclean >> $LOG 2>&1

echo "Running qmake..."
$QMAKE_ROOT/qmake.exe $QMAKE_ARGS >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to execute qmake with arguments '$QMAKE_ARGS'"
  echo
  exit 1
fi

echo "Compiling code..."
$MINGW_ROOT/mingw32-make.exe -j7 >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Compilation failed!"
  echo
  exit 1
fi

echo "Deploying application..."
$MINGW_ROOT/mingw32-make.exe install >> $LOG 2>&1

	if [ $? -ne 0 ]; then
		echo
		echo "** Deployment failed!"
		echo
		exit 1
	fi

echo "Installing dependencies..."
for dll in $DEP_LIBS; do
	cp $dll $INSTALL_ROOT >> $LOG 2>&1
	if [ $? -ne 0 ]; then
		echo
		echo "** Installation failed!"
		echo
		exit 1
	fi
done

#Creates the file build/qt.conf to bind qt plugins
mkdir -p $DEP_PLUGINS_DIR
echo "[Paths]" > $QT_CONF
echo "Prefix=." >> $QT_CONF
echo "Plugins=qtplugins" >> $QT_CONF
echo "Libraries=." >> $QT_CONF

#Copies the qt plugins to build/qtplugins
for plug in $DEP_PLUGINS; do
	pdir=`dirname $plug`
	mkdir -p $DEP_PLUGINS_DIR/$pdir >> $LOG 2>&1
	cp $QT_ROOT/plugins/$plug $DEP_PLUGINS_DIR/$pdir >> $LOG 2>&1
	
	if [ $? -ne 0 ]; then
		echo
		echo "** Installation failed!"
		echo
		exit 1
	fi
done

$MINGW_ROOT/mingw32-make.exe install >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Installation failed!"
  echo
  exit 1
fi

echo "Packaging installation..."

"$INNOSETUP_CMD" $ISSFILE >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create installer package!"
  echo "** Proceeding with basic deployment."
  
  mkdir $PKGNAME >> $LOG 2>&1
  mv $INSTALL_ROOT/* $PKGNAME >> $LOG 2>&1
  mv $PKGNAME $INSTALL_ROOT >> $LOG 2>&1

  if [ $? -ne 0 ]; then
	echo "** Failed to execute basic deployment!"
	exit 1
  fi

  echo
  echo "Directory created: $PKGNAME"
else
  mv $GENINSTALLER build/$PKGFILE >> $LOG 2>&1
  echo "File created: $PKGFILE"
fi

mkdir -p $DIST_ROOT >> $LOG 2>&1
mv $INSTALL_ROOT/$PKGFILE $DIST_ROOT >> $LOG 2>&1

if [ $? -ne 0 ]; then
	echo "** Failed to move $PKGFILE to $DIST_ROOT!"
	exit 1
fi

echo "pgModeler successfully deployed!"
echo

if [ $BUILD_ALL -eq 1 ]; then
 sh windeploy.sh -demo-version
 sh windeploy.sh -x64-build
 sh windeploy.sh -x64-build -demo-version
fi
