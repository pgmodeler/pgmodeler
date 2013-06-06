#/bin/bash

QT_ROOT=/Users/raphael/Qt5.0.2/5.0.2/clang_64
QMAKE_ARGS="-r CONFIG+=x86_64 -spec macx-clang-libc++"
DEPLOY_VER=$1
LOG=macdeploy.log
PKGNAME="pgmodeler-$DEPLOY_VER-macosx"
PKGFILE=$PKGNAME.dmg
APPNAME=pgmodeler

echo
echo "pgModeler Mac OSX deployment script"
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
make distclean &> $LOG

echo "Running qmake..."
$QT_ROOT/bin/qmake $QMAKE_ARGS &> $LOG

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to execute qmake with arguments '$QMAKE_ARGS'"
  echo
fi

echo "Compiling code..."
make -j5 &> $LOG

if [ $? -ne 0 ]; then
  echo
  echo "** Compilation failed!"
  echo
fi

echo "Installing dependencies..."
make install &> $LOG

if [ $? -ne 0 ]; then
  echo
  echo "** Installation failed!"
  echo
fi

echo "Packaging installation..."
rm -r ./$PKGNAME.dmg &> $LOG
rm build/$APPNAME.app/Contents/Frameworks/libpq* &> $LOG
$QT_ROOT/bin/macdeployqt build/$APPNAME.app -dmg &> $LOG
mv build/$APPNAME.dmg ./$PKGFILE &> $LOG

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create package!"
  echo
fi

echo "File created: $PKGFILE"
echo "pgModeler successfully deployed!"
echo
