#/bin/bash

QMAKE_ROOT=/usr/bin
FALLBACK_QMAKE_ROOT=/usr/local/qt-5.2.0/5.2.0/gcc_64/bin
LOG=linuxdeploy.log

# Detecting current pgModeler version
DEPLOY_VER=$(cat libutils/src/globalattributes.h | grep --color=never PGMODELER_VERSION | sed -r 's/.*PGMODELER_VERSION="(.*)",/\1/')
BUILD_NUM=$(date '+%Y%m%d')

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
WITH_BUILD_NUM='-with-build-num'

if [[ "$*" == "$WITH_BUILD_NUM" ]]; then
  PKGNAME="${PKGNAME}_${BUILD_NUM}"
fi

PKGFILE=$PKGNAME.tar.gz
NO_QT_LIBS_OPT='-no-qt-libs'

if [[ "$*" == "$NO_QT_LIBS_OPT" ]]; then
  PKGFILE=$PKGNAME.tar.gz
  BUNDLE_QT_LIBS=0
else
  BUNDLE_QT_LIBS=1
  QT_CONF=build/qt.conf
  DEP_PLUGINS_DIR=build/qtplugins
  
  #Dependency qt plugins copied to build dir
  DEP_PLUGINS="imageformats/libqgif.so \
               imageformats/libqico.so \
               imageformats/libqjpeg.so \
               imageformats/libqmng.so \
               imageformats/libqsvg.so \
               imageformats/libqtga.so \
               imageformats/libqtiff.so \
               imageformats/libqwbmp.so \
               printsupport/libcupsprintersupport.so \
               platforms/libqxcb.so"
               
  #Needed Qt libs
  QT_LIBS="libQt5DBus.so.5 \
           libQt5PrintSupport.so.5 \
           libQt5Widgets.so.5 \
           libQt5Gui.so.5 \
           libQt5Core.so.5 \
           libicui18n.so.51 \
           libicuuc.so.51 \
           libicudata.so.51"
fi

clear
echo 
echo "pgModeler Linux deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2013 Raphael A. Silva <rkhaotix@gmail.com>"

# Identifying System Qt version
if [ -e "$QMAKE_ROOT/qmake" ]; then
  QT_VER_1=`$QMAKE_ROOT/qmake --version | grep --color=never -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER_1=${QT_VER_1:0:5}
fi

# Identifying Fallback Qt version
if [ -e "$FALLBACK_QMAKE_ROOT/qmake" ]; then
  QT_VER_2=`$FALLBACK_QMAKE_ROOT/qmake --version | grep --color=never -m 1 -o '[0-9].[0-9].[0-9]'`
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

if [ $BUNDLE_QT_LIBS = 0 ]; then
  echo "Qt libs will not be included on the package. (Found $NO_QT_LIBS_OPT)"
fi

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

if [ $BUNDLE_QT_LIBS = 1 ]; then
 echo "Copying Qt core libraries..."
 
 QT_ROOT=`$QMAKE_ROOT/qtpaths --install-prefix`  >> $LOG 2>&1
 
 for lib in $QT_LIBS; do
  cp $QT_ROOT/lib/$lib build/ >> $LOG 2>&1
 done
 
 if [ $? -ne 0 ]; then
  echo
  echo "** Library copy failed!"
  echo
  exit 1
 fi
 
 echo "Copying Qt plugins..."
 
 #Creates the file build/qt.conf to bind qt plugins
 mkdir $DEP_PLUGINS_DIR
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
    echo "** Plugins copy failed!"
    echo
    exit 1
  fi
 done

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