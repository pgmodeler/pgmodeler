#!/bin/bash

LOG=windeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.cpp | grep PgModelerVersion | sed 's/.\+PgModelerVersion=QString("//g' | sed 's/")//g'`
DEPLOY_VER=${DEPLOY_VER/PGMODELER_VERSION=\"/}
DEPLOY_VER=`echo ${DEPLOY_VER/\",/} | tr -d ' '`

DEMO_VERSION_OPT='-demo-version'
DEMO_VERSION=0

BUILD_ALL_OPT='-build-all'
BUILD_ALL=0

SNAPSHOT_OPT='-snapshot'
SNAPSHOT=0

# Installer settings
FMT_PREFIX="C:\/Program Files\/pgModeler"
INSTALLER_APP_VER=`echo $DEPLOY_VER | cut -d '-' -f1`
INSTALLER_CONF_DIR="$PWD/installer/template/config"
INSTALLER_PKG_DIR="$PWD/installer/template/packages"
INSTALLER_DATA_DIR="$INSTALLER_PKG_DIR/io.pgmodeler/data"
INSTALLER_META_DIR="$INSTALLER_PKG_DIR/io.pgmodeler/meta"
INSTALLER_TMPL_CONFIG="config.xml.tmpl"
INSTALLER_CONFIG="config.xml"
INSTALLER_TMPL_PKG_CONFIG="package.xml.tmpl"
INSTALLER_PKG_CONFIG="package.xml"
BUILD_DATE=`date '+%Y-%m-%d'`
BUILD_NUM=`date '+%Y%m%d'`

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
   BUILD_ARCH_PARAM=$X64_BUILD_OPT
 fi
 if [[ "$param" == "$BUILD_ALL_OPT" ]]; then
   BUILD_ALL=1
   DEMO_VERSION=0
 fi
 if [[ "$param" == "$SNAPSHOT_OPT" ]]; then
   SNAPSHOT=1
   DEPLOY_VER="${DEPLOY_VER}_snapshot${BUILD_NUM}"
 fi
done

# Define the base name of the binary
if [ $DEMO_VERSION = 1 ]; then
  PKGNAME="pgmodeler-$DEPLOY_VER-demo-windows$WIN_BITS"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-windows$WIN_BITS"
fi

# Settings for x64 build
if [ $X64_BUILD = 1 ]; then
	QT_ROOT="/c/msys64/mingw64"
	MINGW_ROOT="/c/msys64/mingw64/bin"
	DEP_LIBS="$DEP_LIBS \
			$MINGW_ROOT/libssl-1_1-x64.dll \
			$MINGW_ROOT/libcrypto-1_1-x64.dll \
			$MINGW_ROOT/libgcc_s_seh-1.dll"
# Settings for x86 build
else
	QT_ROOT="/c/msys64/mingw32"
	MINGW_ROOT="/c/msys64/mingw32/bin"
	DEP_LIBS="$DEP_LIBS \
			$MINGW_ROOT/libssl-1_1.dll \
			$MINGW_ROOT/libcrypto-1_1.dll \
			$MINGW_ROOT/libgcc_s_dw2-1.dll"
fi

# Common settings for both architectures
QT_INSTALL_VERSION='5.12.3'
QT_BASE_VERSION='5.12.3'
QT_PLUGINS_ROOT="$QT_ROOT/share/qt5/plugins"
QMAKE_ROOT=$MINGW_ROOT
PGSQL_ROOT=$MINGW_ROOT
QMAKE_ARGS="-r -spec win32-g++ CONFIG+=release \
		  XML_INC+=$MINGW_ROOT/../include/libxml2 \
		  XML_LIB+=$MINGW_ROOT/libxml2-2.dll \
		  PGSQL_INC+=$MINGW_ROOT/../include \
		  PGSQL_LIB+=$MINGW_ROOT/libpq.dll"

if [ $DEMO_VERSION = 1 ]; then
  QMAKE_ARGS="$QMAKE_ARGS DEMO_VERSION+=true"
fi

if [ $SNAPSHOT = 1 ]; then
 QMAKE_ARGS="$QMAKE_ARGS SNAPSHOT_BUILD+=true"
fi


PKGFILE=$PKGNAME.exe
GENINSTALLER=pgmodeler.exe
INSTALL_ROOT="$PWD/build"
DIST_ROOT="$PWD/dist"
QT_CONF="$INSTALL_ROOT/qt.conf"
DEP_PLUGINS_DIR="$INSTALL_ROOT/qtplugins"
PLUGINS="dummy xml2object"

# Common dependency libraries
DEP_LIBS="$DEP_LIBS \
		$MINGW_ROOT/libdouble-conversion.dll \
		$MINGW_ROOT/libicuin*.dll \
		$MINGW_ROOT/libicuuc*.dll \
		$MINGW_ROOT/libicudt*.dll \
		$MINGW_ROOT/libpcre2-16-0.dll \
		$MINGW_ROOT/libharfbuzz-0.dll \
		$MINGW_ROOT/libpng16-16.dll \
		$MINGW_ROOT/libfreetype-6.dll \
		$MINGW_ROOT/libgraphite2.dll \
		$MINGW_ROOT/libglib-2.0-0.dll \
		$MINGW_ROOT/libpcre-1.dll \
		$MINGW_ROOT/libbz2-1.dll \
		$MINGW_ROOT/libstdc++-6.dll \
		$MINGW_ROOT/libwinpthread-1.dll \
		$MINGW_ROOT/libzstd.dll \
		$MINGW_ROOT/zlib1.dll \
		$MINGW_ROOT/libpq.dll \
		$MINGW_ROOT/libxml2-2.dll \
		$MINGW_ROOT/liblzma-5.dll \
		$MINGW_ROOT/libiconv-2.dll \
		$MINGW_ROOT/libintl-8.dll \
		$QMAKE_ROOT/Qt5Core.dll \
		$QMAKE_ROOT/Qt5Gui.dll \
		$QMAKE_ROOT/Qt5Widgets.dll \
		$QMAKE_ROOT/Qt5PrintSupport.dll \
		$QMAKE_ROOT/Qt5Network.dll \
		$QMAKE_ROOT/Qt5Svg.dll "

#Dependency qt plugins copied to build dir
DEP_PLUGINS="imageformats/qicns.dll \
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
echo "PostgreSQL Database Modeler Project - pgmodeler.io"
echo "Copyright 2006-2020 Raphael A. Silva <raphael@pgmodeler.io>"

# Identifying Qt version
if [ -e "$QMAKE_ROOT/qmake" ]; then
  QT_VER=`$QMAKE_ROOT/qmake --version | grep -m 1 -o -E '[0-9]\.[0-9]+\.[0-9]+'`
  QT_VER=${QT_VER:0:6}
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
echo "Qt version detected: $QT_VER"

if [ $SNAPSHOT = 1 ]; then
  echo "Building snapshot version. (Found $SNAPSHOT_OPT)"
fi

if [ $DEMO_VERSION = 1 ]; then
  echo "Building demonstration version. (Found $DEMO_VERSION_OPT)"
fi

echo "Building for arch: $DEST_ARCH"
echo "Cleaning previous compilation..."

mkdir -p "${DIST_ROOT}"   > $LOG 2>&1
rm    -r "${DIST_ROOT}/*" > $LOG 2>&1

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
$MINGW_ROOT/mingw32-make.exe -j10 >> $LOG 2>&1

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
	cp $QT_PLUGINS_ROOT/$plug $DEP_PLUGINS_DIR/$pdir >> $LOG 2>&1

	if [ $? -ne 0 ]; then
		echo
		echo "** Installation failed!"
		echo
		exit 1
	fi
done

echo "Packaging installation..."

rm -r $INSTALLER_DATA_DIR >> $LOG 2>&1
ln -sf "$INSTALL_ROOT" $INSTALLER_DATA_DIR >> $LOG 2>&1

if [ $? -ne 0 ]; then
    echo
    echo "** Failed to configure installer data dir!"
    echo
    exit 1
fi

# Configuing installer scripts before packaging
cat $INSTALLER_CONF_DIR/$INSTALLER_TMPL_CONFIG | sed -e "s/{version}/$INSTALLER_APP_VER/g" | sed -e "s/{prefix}/$FMT_PREFIX/g" > $INSTALLER_CONF_DIR/$INSTALLER_CONFIG

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create the installer config file!"
  echo
  exit 1
fi

cat $INSTALLER_META_DIR/$INSTALLER_TMPL_PKG_CONFIG | sed -e "s/{version}/$INSTALLER_APP_VER/g" | sed -e "s/{date}/$BUILD_DATE/g" > $INSTALLER_META_DIR/$INSTALLER_PKG_CONFIG

if [ $? -ne 0 ]; then
   echo
   echo "** Failed to create the package info file!"
   echo
   exit 1
fi

binarycreator -v -c $INSTALLER_CONF_DIR/config.xml -p $INSTALLER_PKG_DIR "$DIST_ROOT/$PKGNAME.exe" >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create installer!"
  echo
  exit 1
fi

echo "File created: dist/$PKGNAME.exe"

echo "pgModeler successfully deployed!"
echo

if [ $BUILD_ALL -eq 1 ]; then
 EXTRA_OPT=""

 if [ $SNAPSHOT = 1 ]; then
    EXTRA_OPT="$SNAPSHOT_OPT"
 fi

 sh windeploy.sh -demo-version $BUILD_ARCH_PARAM $EXTRA_OPT
fi
