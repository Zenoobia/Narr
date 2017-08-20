@echo off

if not defined DEV_ENV_DIR (
	call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64
)
set DEV_ENV_DIR= ???

set CFLAGS= -Zi -nologo -EHsc
set LFLAGS= -incremental:no user32.lib kernel32.lib

if not exist .\bin mkdir .\bin
pushd .\bin

del *.pdb > NUL 2> NUL

REM game dll
echo "WAITING FOR PDB ..." > lock.tmp
cl %CFLAGS% ..\src\game.cpp -LD /link -PDB:game_%random%.pdb %LFLAGS%
del lock.tmp

REM platform exe
REM COPY /B main.exe+NUL main.exe >NUL 2< NUL || GOTO :POPD
2>nul (
	>>main.exe echo off
) && (echo Building Main) || (GOTO :POPD)
cl %CFLAGS% ..\src\main.cpp /link %LFLAGS%
:POPD
popd

echo Done!