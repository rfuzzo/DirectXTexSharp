@echo off

@RD /S /Q .\bin\publish\

msbuild -t:restore /t:Build;Publish;Pack /p:Configuration=Release /p:Platform=x64 /p:PublishProfile=x64 /p:OutputPath=bin/Publish/x64


echo "publish completed"