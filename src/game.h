#define EXPORT extern "C" __declspec( dllexport )

EXPORT int init();
EXPORT int deinit();
EXPORT int loop();

struct
game {
	int (*init)();
	int (*deinit)();
	int (*loop)();

	bool isInitialized;
	bool isRunning;
};

