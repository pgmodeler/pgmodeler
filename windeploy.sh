#!/bin/bash

QT_INSTALL_VERSION='5.4.1'
QT_BASE_VERSION='5.4'
PGSQL_VERSION='9.4'
QT_ROOT="/c/Qt/Qt${QT_INSTALL_VERSION}/${QT_BASE_VERSION}/mingw491_32/"
QMAKE_ROOT=$QT_ROOT/bin
MINGW_ROOT="/c/Qt/Qt${QT_INSTALL_VERSION}/Tools/mingw491_32/bin"
PGSQL_ROOT="/c/PostgreSQL/${PGSQL_VERSION}/bin"
QMAKE_ARGS="-r -spec win32-g++ CONFIG+=release"
INNOSETUP_CMD='/c/Program Files (x86)/Inno Setup 5/ISCC.exe'
LOG=windeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.h | grep PGMODELER_VERSION | sed 's/PGMODELER_VERSION=QString("//g' | sed 's/"),//g'`
DEPLOY_VER=${DEPLOY_VER/PGMODELER_VERSION=\"/}
DEPLOY_VER=`echo ${DEPLOY_VER/\",/} | tr -d ' '`
BUILD_NUM=$(date '+%Y%m%d')

WITH_BUILD_NUM='-with-build-num'
DEMO_VERSION_OPT='-demo-version'
DEMO_VERSION=0

for param in $@; do
 if [[ "$param" == "$WITH_BUILD_NUM" ]]; then
   PKGNAME="${PKGNAME}_${BUILD_NUM}"
 fi

 if [[ "$param" == "$DEMO_VERSION_OPT" ]]; then
   DEMO_VERSION=1
   QMAKE_ARGS="$QMAKE_ARGS DEMO_VERSION+=true"
 fi
done

if [ $DEMO_VERSION = 1 ]; then
  PKGNAME="pgmodeler-demo-windows"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-windows"
fi

PKGFILE=$PKGNAME.exe
GENINSTALLER=pgmodeler.exe
INSTALL_ROOT="$PWD/build"
ISSFILE=./installer/windows/pgmodeler.iss
QT_CONF="$INSTALL_ROOT/qt.conf"
DEP_PLUGINS_DIR="$INSTALL_ROOT/qtplugins"
PLUGINS="dummy xml2object"
  
DEP_LIBS="$QMAKE_ROOT/icudt53.dll \
		  $QMAKE_ROOT/icuin53.dll \
		  $QMAKE_ROOT/icuuc53.dll \
		  $QMAKE_ROOT/libgcc_s_dw2-1.dll \
		  $QMAKE_ROOT/libstdc++-6.dll \
		  $QMAKE_ROOT/libwinpthread-1.dll \
		  $QMAKE_ROOT/Qt5Core.dll \
		  $QMAKE_ROOT/Qt5Gui.dll \
		  $QMAKE_ROOT/Qt5Widgets.dll \
		  $QMAKE_ROOT/Qt5PrintSupport.dll \
		  $QMAKE_ROOT/Qt5Network.dll \
		  $PGSQL_ROOT/libxml2.dll \
		  $PGSQL_ROOT/libiconv.dll \
		  $PGSQL_ROOT/libpq.dll \
		  $PGSQL_ROOT/intl.dll \
		  $PGSQL_ROOT/libeay32.dll \
		  $PGSQL_ROOT/ssleay32.dll \
		  $PGSQL_ROOT/zlib1.dll"
		  
#Dependency qt plugins copied to build dir
DEP_PLUGINS="imageformats/qgif.dll \
			 imageformats/qico.dll \
			 imageformats/qjpeg.dll \
			 imageformats/qmng.dll \
			 imageformats/qsvg.dll \
			 imageformats/qtga.dll \
			 imageformats/qtiff.dll \
			 imageformats/qwbmp.dll \
			 platforms/qwindows.dll \
			 printsupport/windowsprintersupport.dll"
		 
export PATH=$QMAKE_ROOT:$MINGW_ROOT:$PATH

clear
echo
echo "pgModeler Windows deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2014 Raphael A. Silva <raphael@pgmodeler.com.br>"

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

echo "Cleaning previous compilation..."
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
$MINGW_ROOT/mingw32-make.exe -j5 >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Compilation failed!"
  echo
  exit 1
fi

echo "Installing dependencies..."

$MINGW_ROOT/mingw32-make.exe install >> $LOG 2>&1

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


#Fixing the pgModeler plugin deployment.
#Moving dlls from build/plugins/[PLUGIN]/build to build/plugins/[PLUGIN]
#for plugin in $PLUGINS; do
#	mv build/plugins/$plugin/build/* build/plugins/$plugin >> $LOG 2>&1
#	rm -r build/plugins/$plugin/build/  >> $LOG 2>&1
#done

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
  echo "File created: $INSTALL_ROOT/$PKGFILE"
fi

echo "pgModeler successfully deployed!"
echo
