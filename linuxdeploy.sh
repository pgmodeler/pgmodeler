#!/bin/bash

# Identify architecture
case `uname -m` in
  "x86_64")
    ARCH="linux64"
    FALLBACK_QT_ROOT=/opt/qt-5.6.2/5.6/gcc_64
    FALLBACK_QMAKE_ROOT="$FALLBACK_QT_ROOT/bin"
    ;;
    
   *)
    ARCH="linux32"
    FALLBACK_QT_ROOT=/opt/qt-5.6.2/5.6/gcc
    FALLBACK_QMAKE_ROOT="$FALLBACK_QT_ROOT/bin"
    ;;
esac

# Uncomment this line if your system doesn't have LLVM (clang) compiler tools
#QMAKE_ARGS="-r -spec linux-g++"
QMAKE_ARGS="-r -spec linux-clang"
QMAKE_ROOT=/usr/bin
QMAKE_CMD=qmake
LOG="$PWD/linuxdeploy.log"
QT_IFW_ROOT=/opt/qt-ifw-1.5.0

STARTUP_SCRIPT="start-pgmodeler.sh"
MIME_UPDATE_SCRIPT="dbm-mime-type.sh"
ENV_VARS_SCRIPT="pgmodeler.vars"
BUILD_DIR="$PWD/build"
DIST_DIR="$PWD/dist"
INSTALL_ROOT="/opt/pgmodeler"
INSTALLER_CONF_DIR="$PWD/installer/linux/config"
INSTALLER_PKG_DIR="$PWD/installer/linux/packages"
INSTALLER_DATA_DIR="$INSTALLER_PKG_DIR/br.com.pgmodeler/data"
QT_CONF="$BUILD_DIR/$INSTALL_ROOT/qt.conf"
DEP_PLUGINS_DIR="$BUILD_DIR/$INSTALL_ROOT/lib/qtplugins"
  
# Detecting current pgModeler version
DEPLOY_VER=`cat libutils/src/globalattributes.cpp | grep PGMODELER_VERSION | sed 's/PGMODELER_VERSION=QString("//g' | sed 's/"),//g' | sed 's/^ *//g' | cut -s -f2`
GEN_INSTALLER_OPT='-gen-installer'
DEMO_VERSION_OPT='-demo-version'
NO_QT_LIBS_OPT='-no-qt-libs'
BUILD_ALL_OPT='-build-all'
COMPRESS_INSTALLER_OPT='-comp-installer'
GEN_INST_PKG=0
COMP_INST_PKG=0
DEMO_VERSION=0
BUNDLE_QT_LIBS=1
BUILD_ALL=0

# pgModeler output paths settings
PREFIX="/opt/pgmodeler"
BINDIR=$PREFIX
PRIVATEBINDIR=$PREFIX
PRIVATELIBDIR="$PREFIX/lib"
LANGDIR="$PREFIX/lang"
SAMPLESDIR="$PREFIX/samples"
SCHEMASDIR="$PREFIX/schemas"
TEMPDIR="$PREFIX/tmp"
PLUGINSDIR="$PREFIX/plugins"
CONFDIR="$PREFIX/conf"
DOCDIR="$PREFIX"
SHAREDIR="$PREFIX"

QMAKE_ARGS="$QMAKE_ARGS \
	    PREFIX=$PREFIX \
	    BINDIR=$BINDIR \
            PRIVATEBINDIR=$PRIVATEBINDIR \
            PRIVATELIBDIR=$PRIVATELIBDIR \
            LANGDIR=$LANGDIR \
            SAMPLESDIR=$SAMPLESDIR \
            SCHEMASDIR=$SCHEMASDIR \
            PLUGINSDIR=$PLUGINSDIR \
            CONFDIR=$CONFDIR \
            DOCDIR=$DOCDIR \
            SHAREDIR=$SHAREDIR \
            TEMPDIR=$TEMPDIR"
            
for param in $@; do
 if [[ "$param" == "$BUILD_ALL_OPT" ]]; then
  BUILD_ALL=1
 fi

 if [[ "$param" == "$GEN_INSTALLER_OPT" ]]; then
   GEN_INST_PKG=1
 fi

 if [[ "$param" == "$COMPRESS_INSTALLER_OPT" ]]; then
   COMP_INST_PKG=1
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


if [ $BUILD_ALL = 1 ]; then
  DEMO_VERSION=0
  GEN_INST_PKG=1
fi

if [ $DEMO_VERSION = 1 ]; then
  PKGNAME="pgmodeler-demo-$ARCH"
else
  PKGNAME="pgmodeler-$DEPLOY_VER-$ARCH"
fi

PKGFILE=$PKGNAME.tgz

if [ $BUNDLE_QT_LIBS = 0 ]; then
  PKGFILE=$PKGNAME.tgz
else 
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
           libQt5XcbQpa.so.5 \
           libQt5Svg.so.5 \
           libicui18n.so.5* \
           libicuuc.so.5* \
           libicudata.so.5*"
fi

clear 
echo 
echo "pgModeler Linux deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2016 Raphael A. Silva <raphael@pgmodeler.com.br>"

# Identifying System Qt version
if [ -e "$QMAKE_ROOT/$QMAKE_CMD" ]; then
  QT_VER_1=`$QMAKE_ROOT/$QMAKE_CMD --version | grep --color=never -m 1 -o '[0-9].[0-9].[0-9]'`
  QT_VER_1=${QT_VER_1:0:5}
fi

# Identifying Fallback Qt version
if [ -e "$FALLBACK_QMAKE_ROOT/$QMAKE_CMD" ]; then
  QT_VER_2=`$FALLBACK_QMAKE_ROOT/$QMAKE_CMD --version | grep --color=never -m 1 -o '[0-9].[0-9].[0-9]'`
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
  
  if [ $COMP_INST_PKG = 1 ]; then
    echo "The installer will be compressed (Found $COMPRESS_INSTALLER_OPT)"
  fi

fi

if [ $DEMO_VERSION = 1 ]; then
  echo "Building demonstration version. (Found $DEMO_VERSION_OPT)"
fi

echo "Cleaning previous compilation..."

rm -r $BUILD_DIR/* &> $LOG
mkdir -p $DIST_DIR >> $LOG 2>&1

if [ $BUILD_ALL -eq 1 ]; then
  rm -r $DIST_DIR/* >> $LOG 2>&1
fi

make distclean  >> $LOG 2>&1

echo "Running qmake..."
$QMAKE_ROOT/$QMAKE_CMD $QMAKE_ARGS  >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to execute qmake with arguments '$QMAKE_ARGS'"
  echo
  exit 1
fi

echo "Compiling code..."
make -j8  >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Compilation failed!"
  echo
  exit 1
fi

echo "Installing dependencies..."
make install INSTALL_ROOT=$BUILD_DIR  >> $LOG 2>&1

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
  cp -v $QT_ROOT/lib/$lib $BUILD_DIR/$INSTALL_ROOT/lib >> $LOG 2>&1
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
   
   if [ -e $QT_ROOT/plugins/$plug ]; then
        mkdir -p $DEP_PLUGINS_DIR/$pdir >> $LOG 2>&1
        cp -v $QT_ROOT/plugins/$plug $DEP_PLUGINS_DIR/$pdir >> $LOG 2>&1

        if [ $? -ne 0 ]; then
            echo
            echo "** Plugins copy failed!"
            echo
            exit 1
        fi
   fi     
 done

fi

echo "Copying scripts..."
cp -v $STARTUP_SCRIPT "$BUILD_DIR/$INSTALL_ROOT" >> $LOG 2>&1
cp -v $MIME_UPDATE_SCRIPT "$BUILD_DIR/$INSTALL_ROOT" >> $LOG 2>&1
cp -v $ENV_VARS_SCRIPT "$BUILD_DIR/$INSTALL_ROOT" >> $LOG 2>&1

if [ $? -ne 0 ]; then
    echo
    echo "** Failed to copy scripts!"
    echo
    exit 1
fi

chmod +x "$BUILD_DIR/$INSTALL_ROOT/$STARTUP_SCRIPT"
chmod +x "$BUILD_DIR/$INSTALL_ROOT/$MIME_UPDATE_SCRIPT"

if [ $? -ne 0 ]; then
    echo
    echo "** Failed to set permisions to scripts!"
    echo
    exit 1
fi

if [ $DEMO_VERSION = 0 ]; then
  echo "Generating tarball..."
  rm -r $PKGNAME  >> $LOG 2>&1
  mkdir $BUILD_DIR/$PKGNAME  >> $LOG 2>&1
  cp -r $BUILD_DIR/$INSTALL_ROOT/* $BUILD_DIR/$PKGNAME  >> $LOG 2>&1
  
  cd $BUILD_DIR >> $LOG 2>&1
  tar -zcvf $PKGFILE $PKGNAME  >> $LOG 2>&1
  rm -r $PKGNAME  >> $LOG 2>&1
  cd .. >> $LOG 2>&1

  if [ $? -ne 0 ]; then
    echo
    echo "** Failed to create package!"
    echo
    exit 1
  fi

  mv $BUILD_DIR/$PKGFILE $DIST_DIR  >> $LOG 2>&1
  
  if [ $? -ne 0 ]; then
    echo
    echo "** Failed to move $PKGFILE to $DIST_DIR"
    echo
    exit 1
  fi   
  
  echo "File created: dist/$PKGFILE"
fi


if [ $GEN_INST_PKG = 1 ]; then
  echo "Generating installer..."
 
  rm $INSTALLER_DATA_DIR >> $LOG 2>&1
  ln -sf "$BUILD_DIR/$INSTALL_ROOT" $INSTALLER_DATA_DIR >> $LOG 2>&1
   
  if [ $? -ne 0 ]; then
    echo
    echo "** Failed to configure installer data dir!"
    echo
    exit 1
  fi   
 
  $QT_IFW_ROOT/bin/binarycreator -v -c $INSTALLER_CONF_DIR/config.xml -p $INSTALLER_PKG_DIR "$DIST_DIR/$PKGNAME.run" >> $LOG 2>&1

 if [ $? -ne 0 ]; then
   echo
   echo "** Failed to create installer!"
   echo
   exit 1
 fi
 
 if [ $COMP_INST_PKG = 1 ]; then
   _PWD=`pwd`
   cd $DIST_DIR  >> $LOG 2>&1
   tar -zcvf $PKGNAME.run.tgz $PKGNAME.run >> $LOG 2>&1
   rm $PKGNAME.run >> $LOG 2>&1
   
    if [ $? -ne 0 ]; then
       echo
       echo "** Failed to create compressed installer!"
       echo
       exit 1
    fi
    echo "File created: dist/$PKGNAME.run.tgz"
    cd $_PWD >> $LOG 2>&1
 else 
    echo "File created: dist/$PKGNAME.run"
 fi
 

fi

echo "pgModeler successfully deployed!"
echo

if [ $BUILD_ALL = 1 ]; then
    if [ $COMP_INST_PKG = 1 ]; then
        ./linuxdeploy.sh $DEMO_VERSION_OPT $COMPRESS_INSTALLER_OPT
    else
        ./linuxdeploy.sh $DEMO_VERSION_OPT 
    fi    
fi
