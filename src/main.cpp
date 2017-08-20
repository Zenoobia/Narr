
#include <Windows.h>
#include <cstdio>
#include <string>
#include "game.h"


#define DLL_FUNCTIONS_N 3
FARPROC func_ptr[DLL_FUNCTIONS_N];

const char* DLL_FUNCTION_NAMES[DLL_FUNCTIONS_N] =
{
	"init",
	"deinit",
	"loop"
};


HMODULE GameDLL;
FILETIME GameDLLWriteTime;
game Game;

FILETIME Win32GetLastWriteTime( char* path )
{
	FILETIME time = {};
	WIN32_FILE_ATTRIBUTE_DATA data;

	if ( GetFileAttributesEx( path, GetFileExInfoStandard, &data ) )
		time = data.ftLastWriteTime;

	return time;
}

void UnloadGameDLL()
{
	if (Game.isInitialized == true)
		Game.deinit();
	Game = { 0 };
	FreeLibrary(GameDLL);
	GameDLL = 0;
}

#define GetKnownProcAddress(dll, name) reinterpret_cast<int(*)()>(GetProcAddress(dll, name));

void LoadGameDLL( )
{
	WIN32_FILE_ATTRIBUTE_DATA unused;
	if (!GetFileAttributesEx("lock.tmp", GetFileExInfoStandard, &unused))
	{
		UnloadGameDLL();
		CopyFile("game.dll", "game_temp.dll", 0);
		GameDLL = LoadLibrary("game_temp.dll");

		if (!GameDLL)
		{
			DWORD err = GetLastError();
			printf("Can't load lib: %d\n", err);
			return;
		}

		GameDLLWriteTime = Win32GetLastWriteTime( "game.dll" );
		Game.init = GetKnownProcAddress(GameDLL, "init");
		Game.deinit = GetKnownProcAddress(GameDLL, "deinit");
		Game.loop = GetKnownProcAddress(GameDLL, "loop");

	Game.init();
	Game.isInitialized = true;
	Game.isRunning = true;
	}
}

int main( )
{
	char buf[ 256 ];
	GetCurrentDirectory( 256, buf );

	LoadGameDLL( );

	while(Game.isRunning)
	{
		FILETIME newTime = Win32GetLastWriteTime( "game.dll" );

		if ( CompareFileTime( &newTime, &GameDLLWriteTime )) {
			LoadGameDLL( );
		}
		if (Game.loop() != 0)
		{
			Game.isRunning = false;
		}
	}
	return Game.deinit();
}
