#!/bin/bash

USR=`whoami`
PGSQL_ROOT=/Library/PostgreSQL/11
QT_ROOT=/Users/$USR/Qt5.14.0/5.14.0/clang_64
QMAKE_ARGS="-r CONFIG+=x86_64 CONFIG+=release -spec macx-clang"
LOG=macdeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.cpp | grep PgModelerVersion | sed 's/.*PgModelerVersion=QString("//g' | sed 's/")//g' | sed 's/^ *//g'`
BUILD_NUM=$(date '+%Y%m%d')

DEMO_VERSION_OPT='-demo-version'
DEMO_VERSION=0
SNAPSHOT_OPT='-snapshot'
SNAPSHOT=0

for param in $@; do
 if [[ "$param" == "$DEMO_VERSION_OPT" ]]; then
   DEMO_VERSION=1
   QMAKE_ARGS="$QMAKE_ARGS DEMO_VERSION+=true"
 fi

 if [[ "$param" == "$SNAPSHOT_OPT" ]]; then
   SNAPSHOT=1
   QMAKE_ARGS="$QMAKE_ARGS SNAPSHOT_BUILD+=true"
   DEPLOY_VER="${DEPLOY_VER}_snapshot${BUILD_NUM}"
 fi
done

if [ $DEMO_VERSION = 1 ]; then
  PKGNAME="pgmodeler-$DEPLOY_VER-demo-macos"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-macos"
fi

PKGFILE=$PKGNAME.dmg
APPNAME=pgmodeler
INSTALL_ROOT="$PWD/build"
APP_PREFIX="Applications"
BUNDLE="$INSTALL_ROOT/$APP_PREFIX/$APPNAME.app"

clear
echo
echo "pgModeler macOS deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.io"
echo "Copyright 2006-2020 Raphael A. Silva <raphael@pgmodeler.io>"

# Identifying System Qt version
if [ -e "$QT_ROOT/bin/qmake" ]; then
  QT_VER=`$QT_ROOT/bin/qmake --version | grep -m 1 -o -E '[0-9]\.[0-9]+\.[0-9]+'`
  QT_VER=${QT_VER:0:6}
fi

# If Qt was not found in system path
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

echo "Cleaning previous compilation..."
rm -r $INSTALL_ROOT/* &> $LOG
mkdir -p ./dist  >> $LOG 2>&1
make distclean  >> $LOG 2>&1

echo "Running qmake..."
$QT_ROOT/bin/qmake $QMAKE_ARGS  >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to execute qmake with arguments '$QMAKE_ARGS'"
  echo
  exit 1
fi

echo "Compiling code..."
make -j6 >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Compilation failed!"
  echo
  exit 1
fi

echo "Installing dependencies..."
make install INSTALL_ROOT=$INSTALL_ROOT >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Installation failed!"
  echo
  exit 1
fi

echo "Packaging installation..."

# Deploy the Qt libraries onto app bundle
$QT_ROOT/bin/macdeployqt $BUNDLE -executable=$BUNDLE/Contents/MacOS/pgmodeler-ch -executable=$BUNDLE/Contents/MacOS/pgmodeler-cli >> $LOG 2>&1
cp $PGSQL_ROOT/lib/libpq.5.dylib $BUNDLE/Contents/Frameworks >> $LOG 2>&1
cp $PGSQL_ROOT/lib/libssl.1.* $BUNDLE/Contents/Frameworks >> $LOG 2>&1
cp $PGSQL_ROOT/lib/libcrypto.1.* $BUNDLE/Contents/Frameworks >> $LOG 2>&1

# Fixing the support of ssl by forcing the usage of the bundled libpq
install_name_tool -change "@loader_path/../lib/libcrypto.1.0.0.dylib" "@loader_path/../Frameworks/libcrypto.1.0.0.dylib" $BUNDLE/Contents/Frameworks/libssl.1.0.0.dylib >> $LOG 2>&1
install_name_tool -change "@loader_path/../lib/libcrypto.1.0.0.dylib" "@loader_path/../Frameworks/libcrypto.1.0.0.dylib" $BUNDLE/Contents/Frameworks/libpq.5.dylib >> $LOG 2>&1
install_name_tool -change "@loader_path/../lib/libssl.1.0.0.dylib" "@loader_path/../Frameworks/libssl.1.0.0.dylib" $BUNDLE/Contents/Frameworks/libpq.5.dylib >> $LOG 2>&1
install_name_tool -change libpq.5.dylib "@loader_path/../Frameworks/libpq.5.dylib" $BUNDLE/Contents/Frameworks/libpgconnector.dylib >> $LOG 2>&1

# Creates an empty dmg file named
cp installer/macosx/installer_icon.icns $INSTALL_ROOT/.VolumeIcon.icns >> $LOG 2>&1
mv $BUNDLE $INSTALL_ROOT >> $LOG 2>&1
rm -r "$INSTALL_ROOT/$APP_PREFIX" >> $LOG 2>&1
ln -s /Applications $INSTALL_ROOT/Applications >> $LOG 2>&1

rm $PKGFILE >> $LOG 2>&1
hdiutil create -format UDRW -fs HFS+ $PKGFILE -volname $APPNAME -srcfolder $INSTALL_ROOT >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create package!"
  echo
  exit 1
fi

echo "Updating package default icon..."
open $PKGFILE --hide >> $LOG 2>&1
sleep 3
SetFile -a C /Volumes/$APPNAME >> $LOG 2>&1
umount /Volumes/$APPNAME >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to update package icon!"
  echo
  exit 1
fi

mv $PKGFILE ./dist/ >> $LOG 2>&1
PKGFILE="dist/$PKGFILE"

echo "File created: $PKGFILE"
echo "pgModeler successfully deployed!"
echo
