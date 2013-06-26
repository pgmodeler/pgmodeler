#/bin/bash

QMAKE_ROOT=/usr/bin
FALLBACK_QMAKE_ROOT=/usr/local/qt-5.0.2/5.0.2/gcc_64/bin
LOG=linuxdeploy.log

# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.h | grep PGMODELER_VERSION | grep -o '[0-9].[0-9].[0-9]\(.\)*'`
DEPLOY_VER=${DEPLOY_VER/\",/}

# Identify architecture
case `uname -m` in
  "x86_64")
    ARCH="linux64"
    QMAKE_ARGS="-r -spec linux-g++-64"
    ;;
    
   *)
    ARCH="linux32"
    QMAKE_ARGS="-r -spec linux-g++"
    ;;
esac

PKGNAME="pgmodeler-$DEPLOY_VER-$ARCH"
PKGFILE=$PKGNAME.tar.gz

clear
echo 
echo "pgModeler Linux deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2013 Raphael A. Silva <rkhaotix@gmail.com>"

# Identifying System Qt version
if [ -e "$QMAKE_ROOT/qmake" ]; then
  QT_VER_1=`$QMAKE_ROOT/qmake --version | grep -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER_1=${QT_VER_1:0:5}
fi

# Identifying Fallback Qt version
if [ -e "$FALLBACK_QMAKE_ROOT/qmake" ]; then
  QT_VER_2=`$FALLBACK_QMAKE_ROOT/qmake --version | grep -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER_2=${QT_VER_2:0:5}
fi

# If Qt was not found in system path or fallback path
if [ -z "$QT_VER_1" -a -z "$QT_VER_2" ]; then
  echo
  echo "** No Qt framework was found!"
  echo
  exit 1
else

  # Checking if identified versions are valid (>= 5.0.0)
  if [[ "$QT_VER_1" < "5.0.0" ]]; then
  
     if [[ "$QT_VER_2" < "5.0.0" ]]; then
	
	if [ -z $QT_VER_2 ]; then 
	  QT_VER_2="not found" 
	fi
	
	echo
	echo "** Qt framework found but in no suitable version (>= 5.0.0)!"
	echo "** System Qt version: $QT_VER_1"
	echo "** Fallback Qt version: $QT_VER_2"
	echo
	exit 1
     else
	# If fallback Qt is suitable then change the standard qmake root dir
	QMAKE_ROOT=$FALLBACK_QMAKE_ROOT
     fi
  fi
fi

echo
echo "Deploying version: $DEPLOY_VER"
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