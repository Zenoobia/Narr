@echo off
REM IF NOT ("%VSINSTALLDIR%"=="") ( EXIT 0 )
REM echo "CALL"
REM call "%VS120COMNTOOLS%\..\..\VC\bin\vcvars32.bat"
REM call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"

w:
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x86
set path=w:\misc;%path%
