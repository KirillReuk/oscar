#requires -version 2.0
Param(
    [Parameter(Position=0,Mandatory=$True)][ValidateSet("Debug", "Release", ignorecase=$false)][string]$buildType,
    [Parameter(Position=1)][ValidateSet("0", "1", "3", "5", "7", "9")][string]$compressionLevel="3"
)

$buildFolder = "build-ontolis-meta-" + $buildType
$ontolisFolder = $buildFolder + "\ontolis"

if(!(Test-Path -Path ($ontolisFolder + "\ontolis.exe")))
{
    Throw "Ontolis executable not found"
}

$execScript = "runontolis.bat"
echo ("cd " + $ontolisFolder) 'set login=kaf@nevod.ru' 'if not "%~1"=="" set login=%1' 'start ontolis %login%'  | out-file -encoding ascii $execScript
$itemsToArchive = @("ontolis-meta\ontolis\examples\_visualizeMaster.vis.ont", $ontolisFolder, $execScript)

cd $buildFolder
ForEach ($folder in Get-ChildItem -filter "ontolis-avis-*" | ?{ $_.PSIsContainer })
{
    $itemsToArchive += $buildFolder + "\" + $folder.name
    cd $folder.name
    $dllName = $folder.name + ".dll"
    Remove-Item * -Recurse -exclude $dllName,debug,release
    cd ..
}
cd ontolis
Remove-Item * -Recurse -exclude "ontolis*",debug,release,"ui_*.h"
windeployqt ontolis.exe
cd ..\..

cp "ontolis-meta\ontolis\conf.json" $ontolisFolder

$archiveName = "ontolis-win.zip"
rm @($archiveName) -ErrorAction SilentlyContinue

$7zParams = @("a", "-tzip", ("-mx" + $compressionLevel), "-xr!debug", "-xr!release", "-xr!ui_*.h")
& 7z $7zParams $archiveName $itemsToArchive
rm $execScript
