@echo off

echo "nuget pack x64"

nuget pack .\DirectXTexSharp.nuspec -Build -NoPackageAnalysis -Properties 'Configuration=Debug;Platform=x64;CConfiguration=Debug;CPlatform=x64;' -OutputDirectory nuget -Version 0.2

::echo "nuget pack win32"
::nuget pack .\DirectXTexSharp.nuspec -Build -NoPackageAnalysis -Properties 'Configuration=Release;Platform=win32;CConfiguration=Release;CPlatform=win32;' -OutputDirectory nuget -Version 0.4


echo "finished"