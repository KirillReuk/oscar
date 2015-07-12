#!/bin/bash
if [[ $# != 1 || ("$1" != "Debug" && "$1" != "Release") ]]
then
        echo "Usage: $0 <Debug/Release>"
        exit 1
fi
shopt -s extglob
buildtype=$1

buildFolder=build-ontolis-meta-$buildtype
ontolisFolder=$buildFolder/ontolis

if [ ! -f $ontolisFolder/ontolis ]; then
    echo "Ontolis executable not found"
    exit 1
fi

execScript="runontolis.sh"
echo "#!/bin/sh" > $execScript
echo "export LD_LIBRARY_PATH=\`pwd\`/libs" >> $execScript
echo "export QT_PLUGIN_PATH=\`pwd\`" >> $execScript
echo "cd $ontolisFolder" >> $execScript
echo './ontolis ${1:-kaf@nevod.ru}' >> $execScript
chmod u+x $execScript

includeList=files_to_include.txt

echo "ontolis-meta/ontolis/examples/_visualizeMaster.vis.ont" > $includeList
echo "$ontolisFolder" >> $includeList
echo "$execScript" >> $includeList

cd $buildFolder
for fld in `find . -type d -name "ontolis-avis-*"`
do
    echo $buildFolder"/"${fld#./}  >> ../$includeList
    cd $fld
    rm -f !(lib*)
    cd ..
done
cd ontolis
rm -f !(ontolis)
cd ../..

qtgccdir=Qt5.3.1/5.3/gcc
libsdir=$PWD/libs
rm -rf $libsdir
mkdir $libsdir
echo libs >> $includeList
for dep in `ldd $ontolisFolder/ontolis | awk '{print $3}' | grep -v "("`
do
  cp $dep $libsdir
  echo "Copied dependency "$dep" to "$libsdir
done

qtplatformplugin=$qtgccdir/plugins/platforms/libqxcb.so
for dep in `ldd $qtplatformplugin | awk '{print $3}' | grep -v "("`
do
  cp $dep $libsdir
  echo "Copied dependency "$dep" to "$libsdir
done

qtplatformplugindir=$PWD/platforms
rm -rf $qtplatformplugindir
mkdir $qtplatformplugindir
echo platforms >> $includeList
cp $qtplatformplugin $qtplatformplugindir

cp "ontolis-meta/ontolis/conf.json" $ontolisFolder

archiveName="ontolis_alt.tar"
rm $archiveName

tar -czf $archiveName -T $includeList
rm -rf $execScript libs platforms $includeList
