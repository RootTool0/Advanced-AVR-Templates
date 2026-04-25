@echo off

mkdir Build 2>nul
cd Build

cmake ..

cd ..
(goto) 2>nul & del "%~f0"
