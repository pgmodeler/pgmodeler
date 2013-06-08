#/bin/bash

QT_ROOT=/c/Qt/Qt5.0.2/5.0.2/mingw47_32/
QMAKE_ROOT=$QT_ROOT/bin
MINGW_ROOT=/c/Qt/Qt5.0.2/Tools/MinGW/bin
PGSQL_ROOT=/c/PostgreSQL/9.2/bin
QMAKE_ARGS="-r -spec win32-g++"
DEPLOY_VER=$1
INNOSETUP_CMD='/c/Program Files (x86)/Inno Setup 5/ISCC.exe'
LOG=windeploy.log
PKGNAME="pgmodeler-$DEPLOY_VER-windows"
PKGFILE=$PKGNAME.exe
GENINSTALLER=pgmodeler.exe
ISSFILE=pgmodeler.iss
QT_CONF=build/qt.conf
DEP_PLUGINS_DIR=build/qtplugins
PLUGINS="dummy xml2object"

#Dependency dlls copied to build dir
DEP_LIBS="$QMAKE_ROOT/D3DCompiler_43.dll \
		  $QMAKE_ROOT/iconv.dll \
		  $QMAKE_ROOT/icudt49.dll \
		  $QMAKE_ROOT/icuin49.dll \
		  $QMAKE_ROOT/icuuc49.dll \
		  $QMAKE_ROOT/libEGL.dll \
		  $QMAKE_ROOT/libgcc_s_sjlj-1.dll \
		  $QMAKE_ROOT/libGLESv2.dll \
		  $QMAKE_ROOT/libstdc++-6.dll \
		  $QMAKE_ROOT/libwinpthread-1.dll \
		  $QMAKE_ROOT/libxml2.dll \
		  $QMAKE_ROOT/iconv.dll \
		  $QMAKE_ROOT/Qt5Core.dll \
		  $QMAKE_ROOT/Qt5Gui.dll \
		  $QMAKE_ROOT/Qt5Widgets.dll \
		  $QMAKE_ROOT/Qt5PrintSupport.dll \
		  $PGSQL_ROOT/libpq.dll \
		  $PGSQL_ROOT/libintl.dll \
		  $PGSQL_ROOT/libeay32.dll \
		  $PGSQL_ROOT/ssleay32.dll \
		  $PGSQL_ROOT/zlib1.dll"
		  
#Dependency qt plugins copied to build dir
DEP_PLUGINS="imageformats/qgif.dll \
			 imageformats/qico.dll \
			 imageformats/qjpeg.dll \
			 imageformats/qmng.dll \
			 imageformats/qsvg.dll \
			 imageformats/qtga.dll \
			 imageformats/qtiff.dll \
			 imageformats/qwbmp.dll \
			 platforms/qwindows.dll \
			 printsupport/windowsprintersupport.dll"
			 
export PATH=$QMAKE_ROOT:$MINGW_ROOT:$PATH

clear
echo
echo "pgModeler Windows deployment script"
echo "PostgreSQL Database Modeler Project - pgmodeler.com.br"
echo "Copyright 2006-2013 Raphael A. Silva <rkhaotix@gmail.com>"

if [ -z $DEPLOY_VER ]; then
  echo
  echo "** Deploy version not specified!"
  echo
  exit 1
fi

# Identifying Qt version
if [ -e "$QMAKE_ROOT/qmake" ]; then
  QT_VER=`$QMAKE_ROOT/qmake --version | grep '[0-9].[0-9].[0-9]'`
  QT_VER=${QT_VER:0:5}
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
echo "Cleaning previous compilation..."
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
$MINGW_ROOT/mingw32-make.exe -j5 >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Compilation failed!"
  echo
  exit 1
fi

echo "Installing dependencies..."

for dll in $DEP_LIBS; do
	cp $dll build/ >> $LOG 2>&1
	if [ $? -ne 0 ]; then
		echo
		echo "** Installation failed!"
		echo
		exit 1
	fi
done

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
		echo "** Installation failed!"
		echo
		exit 1
	fi
done

$MINGW_ROOT/mingw32-make.exe install >> $LOG 2>&1

#Fixing the pgModeler plugin deployment.
#Moving dlls from build/plugins/[PLUGIN]/build to build/plugins/[PLUGIN]
for plugin in $PLUGINS; do
	mv build/plugins/$plugin/build/* build/plugins/$plugin >> $LOG 2>&1
	rm -r build/plugins/$plugin/build/  >> $LOG 2>&1
done

if [ $? -ne 0 ]; then
  echo
  echo "** Installation failed!"
  echo
  exit 1
fi

echo "Packaging installation..."
rm -r $PKGNAME >> $LOG 2>&1

"$INNOSETUP_CMD" $ISSFILE >> $LOG 2>&1

if [ $? -ne 0 ]; then
  echo
  echo "** Failed to create package!"
  echo
fi

mv $GENINSTALLER $PKGFILE >> $LOG 2>&1

echo "File created: $PKGFILE"
echo "pgModeler successfully deployed!"
echo
