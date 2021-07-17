@echo off

set vs_com19="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\Common7\Tools\vsdevcmd.bat"

call %vs_com19%
call msbuild -t:restore /t:Build /p:Configuration=Debug /p:Platform=x64 /p:PublishProfile=x64
call msbuild -t:restore /t:Build /p:Configuration=Debug /p:Platform=x86 /p:PublishProfile=x86

nuget pack .\DirectXTexSharp.nuspec -NoPackageAnalysis -Properties 'Configuration=Debug;CConfiguration=Debug;' -OutputDirectory nuget -Version 0.4

echo "finished"