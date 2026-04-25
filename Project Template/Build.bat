@echo off

xcopy "../Include" "Source/" /E /I /H /Y

mkdir Build 2>nul
cd Build

cmake ..

cd ..
(goto) 2>nul & del "%~f0"
