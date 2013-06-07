#/bin/bash

QMAKE_ROOT=/usr/local/qt-5.0.2/5.0.2/gcc_64/bin
QMAKE_ARGS="-r -spec linux-g++-64"
DEPLOY_VER=$1
LOG=linuxdeploy.log

if [ `uname -m`="x86_64" ]; then
  ARCH="linux64"
else
  ARCH="linux32"
fi

PKGNAME="pgmodeler-$DEPLOY_VER-$ARCH"
PKGFILE=$PKGNAME.tar.gz

clear
echo
echo "pgModeler Linux deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2013 Raphael A. Silva <rkhaotix@gmail.com>"

if [ -z $DEPLOY_VER ]; then
  echo
  echo "** Deploy version not specified!"
  echo
  exit 1
fi

echo
echo "Cleaning previous compilation..."
rm -r build/* &> $LOG
make distclean  >> $LOG 2>&1

echo "Running qmake..."
$QMAKE_ROOT/qmake $QMAKE_ARGS  >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to execute qmake with arguments '$QMAKE_ARGS'"
  echo
  exit 1
fi

echo "Compiling code..."
make -j5  >> $LOG 2>&1

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
rm -r $PKGNAME  >> $LOG 2>&1
mkdir $PKGNAME  >> $LOG 2>&1
cp -r build/* $PKGNAME  >> $LOG 2>&1
tar -zcvf $PKGFILE $PKGNAME  >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create package!"
  echo
  exit 1
fi

echo "File created: $PKGFILE"
echo "pgModeler successfully deployed!"
echo