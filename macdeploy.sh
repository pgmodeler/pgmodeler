#/bin/bash

USR=`whoami`
QT_ROOT=/Users/$USR/Qt5.0.2/5.0.2/clang_64
QMAKE_ARGS="-r CONFIG+=x86_64 -spec macx-clang-libc++"
LOG=macdeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.h | grep PGMODELER_VERSION | grep -o '[0-9].[0-9].[0-9]\(.\)*'`
DEPLOY_VER=${DEPLOY_VER/\",/}

PKGNAME="pgmodeler-$DEPLOY_VER-macosx"
PKGFILE=$PKGNAME.dmg
APPNAME=pgmodeler
BUNDLE=$APPNAME.app

clear
echo
echo "pgModeler Mac OSX deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2013 Raphael A. Silva <rkhaotix@gmail.com>"

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
echo "Cleaning previous compilation..."
rm -r build/* &> $LOG
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
make install  >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Installation failed!"
  echo
  exit 1
fi

echo "Packaging installation..."
rm $PKGFILE  >> $LOG 2>&1

# Deploy the Qt libraries onto app bundle
$QT_ROOT/bin/macdeployqt build/$BUNDLE >> $LOG 2>&1

# Remove the libpq from Frameworks path
rm build/$BUNDLE/Contents/Frameworks/libpq*  >> $LOG 2>&1

# Creates an empty dmg file named
hdiutil create -fs HFS+ $PKGFILE -volname $APPNAME -srcfolder build/ >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create package!"
  echo
  exit 1
fi

echo "File created: $PKGFILE"
echo "pgModeler successfully deployed!"
echo
