#/bin/bash

# Identify architecture
case `uname -m` in
  "x86_64")
    ARCH="linux64"
    FALLBACK_QT_ROOT=/opt/qt-5.4.0/5.4/gcc_64
    FALLBACK_QMAKE_ROOT="$FALLBACK_QT_ROOT/bin"
    ;;
    
   *)
    ARCH="linux32"
    FALLBACK_QT_ROOT=/opt/qt-5.4.0/5.4/gcc
    FALLBACK_QMAKE_ROOT="$FALLBACK_QT_ROOT/bin"
    ;;
esac

# Uncomment this line if your system doesn't have LLVM (clang) compiler tools
#QMAKE_ARGS="-r -spec linux-g++"
QMAKE_ARGS="-r -spec linux-clang"
QMAKE_ROOT=/usr/bin
LOG=linuxdeploy.log
QT_IFW_ROOT=/opt/qt-if-1.5.0

STARTUP_SCRIPT="start-pgmodeler.sh"
BUILD_DIR="$PWD/build"
INSTALLER_DATA_DIR="$PWD/installer/linux/packages/br.com.pgmodeler/data"

# Detecting current pgModeler version
#DEPLOY_VER=$(cat libutils/src/globalattributes.h | grep --color=never PGMODELER_VERSION | sed -r 's/.*PGMODELER_VERSION="(.*)",/\1/')
DEPLOY_VER=`cat libutils/src/globalattributes.h | grep PGMODELER_VERSION | sed 's/PGMODELER_VERSION=QString("//g' | sed 's/"),//g' | sed 's/^ *//g'`
BUILD_NUM=$(date '+%Y%m%d')

WITH_BUILD_NUM='-with-build-num'
GEN_INSTALLER_OPT='-gen-installer'
DEMO_VERSION_OPT='-demo-version'
NO_QT_LIBS_OPT='-no-qt-libs'
GEN_INST_PKG=0
DEMO_VERSION=0
BUNDLE_QT_LIBS=1

for param in $@; do
 if [[ "$param" == "$WITH_BUILD_NUM" ]]; then
   PKGNAME="${PKGNAME}_${BUILD_NUM}"
 fi

 if [[ "$param" == "$GEN_INSTALLER_OPT" ]]; then
   GEN_INST_PKG=1
 fi

 if [[ "$param" == "$DEMO_VERSION_OPT" ]]; then
   DEMO_VERSION=1
   GEN_INST_PKG=1
   QMAKE_ARGS="$QMAKE_ARGS DEMO_VERSION+=true"
 fi
 
 if [[ "$param" == "$NO_QT_LIBS_OPT" ]]; then
  BUNDLE_QT_LIBS=0
 fi
done

if [ $DEMO_VERSION = 1 ]; then
  PKGNAME="pgmodeler-demo-$ARCH"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-$ARCH"
fi

PKGFILE=$PKGNAME.tar.gz

if [ $BUNDLE_QT_LIBS = 0 ]; then
  PKGFILE=$PKGNAME.tar.gz
else
  QT_CONF=build/qt.conf
  DEP_PLUGINS_DIR=build/lib/qtplugins
  
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
           libQt5Network.so.5 \
           libQt5Gui.so.5 \
           libQt5Core.so.5 \
           libicui18n.so.5* \
           libicuuc.so.5* \
           libicudata.so.5*"
fi

clear 
echo 
echo "pgModeler Linux deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2015 Raphael A. Silva <raphael@pgmodeler.com.br>"

# Identifying System Qt version
if [ -e "$QMAKE_ROOT/qmake" ]; then
  QT_VER_1=`$QMAKE_ROOT/qmake --version | grep --color=never -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER_1=${QT_VER_1:0:5}
fi

# Identifying Fallback Qt version
if [ -e "$FALLBACK_QMAKE_ROOT/qmake" ]; then
  QT_VER_2=`$FALLBACK_QMAKE_ROOT/qmake --version | grep --color=never -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER_2=${QT_VER_2:0:5}
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"$FALLBACK_QT_ROOT/lib"
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

if [ $GEN_INST_PKG = 1 ]; then
  echo "The installer will be generated. (Found $GEN_INSTALLER_OPT)"
fi

if [ $DEMO_VERSION = 1 ]; then
  echo "Building demonstration version. (Found $DEMO_VERSION_OPT)"
fi

echo "Cleaning previous compilation..."
rm -r $BUILD_DIR/* &> $LOG
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
  cp $QT_ROOT/lib/$lib $BUILD_DIR/lib >> $LOG 2>&1
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
 echo "Plugins=lib/qtplugins" >> $QT_CONF
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

echo "Copying startup script..."
cp $STARTUP_SCRIPT $BUILD_DIR >> $LOG 2>&1

if [ $? -ne 0 ]; then
    echo
    echo "** Failed to copy startup script!"
    echo
    exit 1
fi

if [ $DEMO_VERSION = 0 ]; then
  echo "Generating tarball..."
  rm -r $PKGNAME  >> $LOG 2>&1
  mkdir $PKGNAME  >> $LOG 2>&1
  cp -r build/* $PKGNAME  >> $LOG 2>&1
  tar -zcvf $PKGFILE $PKGNAME  >> $LOG 2>&1
  rm -r $PKGNAME  >> $LOG 2>&1

  if [ $? -ne 0 ]; then
    echo
    echo "** Failed to create package!"
    echo
    exit 1
  fi

  echo "File created: $PKGFILE"
fi


if [ $GEN_INST_PKG = 1 ]; then
  echo "Generating installer..."
  
 if ! [ -e "$INSTALLER_DATA_DIR" ]; then
   ln -sf $BUILD_DIR $INSTALLER_DATA_DIR >> $LOG 2>&1
   
   if [ $? -ne 0 ]; then
     echo
     echo "** Failed to installer data dir!"
     echo
     exit 1
   fi   
 fi
  
  $QT_IFW_ROOT/bin/binarycreator -c installer/linux/config/config.xml -p installer/linux/packages "$PKGNAME.run" >> $LOG 2>&1

 if [ $? -ne 0 ]; then
   echo
   echo "** Failed to create installer!"
   echo
   exit 1
 fi

 echo "File created: $PKGNAME.run"
fi


echo "pgModeler successfully deployed!"
echo
