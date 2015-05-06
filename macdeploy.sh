#!/bin/bash

USR=`whoami`
QT_ROOT=/Users/$USR/Qt5.4.1/5.4/clang_64
QMAKE_ARGS="-r CONFIG+=x86_64 CONFIG+=release -spec macx-clang"
LOG=macdeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.cpp | grep PGMODELER_VERSION | sed 's/PGMODELER_VERSION=QString("//g' | sed 's/"),//g' | sed 's/^ *//g'`
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
  PKGNAME="pgmodeler-demo-macosx"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-macosx"
fi

PKGFILE=$PKGNAME.dmg
APPNAME=pgmodeler
INSTALL_ROOT="$PWD/build"
APP_PREFIX="Applications"
BUNDLE="$INSTALL_ROOT/$APP_PREFIX/$APPNAME.app"

clear
echo
echo "pgModeler Mac OSX deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2015 Raphael A. Silva <raphael@pgmodeler.com.br>"

# Identifying System Qt version
if [ -e "$QT_ROOT/bin/qmake" ]; then
  QT_VER=`$QT_ROOT/bin/qmake --version | grep -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER=${QT_VER:0:5}
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

if [ $DEMO_VERSION = 1 ]; then
  echo "Building demonstration version. (Found $DEMO_VERSION_OPT)"
fi

echo "Cleaning previous compilation..."
rm -r $INSTALL_ROOT/* &> $LOG
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
make   >> $LOG 2>&1

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
$QT_ROOT/bin/macdeployqt $BUNDLE >> $LOG 2>&1

# Creates an empty dmg file named
cp installer/macosx/installer_icon.icns $INSTALL_ROOT/.VolumeIcon.icns >> $LOG 2>&1
mv $BUNDLE $INSTALL_ROOT >> $LOG 2>&1
rm -r "$INSTALL_ROOT/$APP_PREFIX" >> $LOG 2>&1
ln -s /Applications $INSTALL_ROOT/Applications >> $LOG 2>&1

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

mv $PKGFILE $INSTALL_ROOT >> $LOG 2>&1
PKGFILE="build/$PKGFILE"

echo "File created: $PKGFILE"
echo "pgModeler successfully deployed!"
echo
